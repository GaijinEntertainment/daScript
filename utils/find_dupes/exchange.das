options gen2
options rtti

require strings
require daslib/json_boost
require daslib/fio
require cluster
require minhash
require canonical


// Slim per-function record for the on-disk export. Drops the MinHash
// signature (recomputed on import — deterministic and cheap) and
// `cluster_id` (assigned by `exact_buckets`, not part of the input
// shape).
struct public ExportedFunction {
    name : string
    file : string
    line : int
    is_lambda : bool
    canonical : string
}


// `schema_version` lets a future schema bump reject older readers
// loudly instead of silently mis-parsing. Bump on any breaking change
// to ExportedFunction's field set. Default to 0 (an invalid version)
// so JSON missing the key is rejected by the explicit version check
// in `read_import` rather than silently treated as v1.
struct public ExportEnvelope {
    schema_version : int = 0
    functions : array<ExportedFunction>
}


let public EXCHANGE_SCHEMA_VERSION = 1


// Single source of truth for "given a canonical string, finish
// populating the FuncRecord" — used both by the compile path
// (`collect_from_program`) and by the import path. Keeps the two
// in sync if tokenization or signing ever changes.
//
// Always (re)sets `sig`: when `want_sigs` is false (or token_count
// is below the floor), `sig` is cleared so a reused record can't
// carry a stale signature into fuzzy_pairs.
def public derive_sig_and_count(var rec : FuncRecord; want_sigs : bool; min_tokens : int) {
    let tokens <- tokenize_canonical(rec.canonical)
    rec.token_count = length(tokens)
    if (want_sigs && rec.token_count >= min_tokens) {
        rec.sig <- minhash_signature(tokens)
    } else {
        rec.sig |> clear()
    }
}


def public write_export(records : array<FuncRecord>; path : string) : bool {
    var env = ExportEnvelope()
    env.schema_version = EXCHANGE_SCHEMA_VERSION
    env.functions |> reserve(length(records))
    for (rec in records) {
        var ef : ExportedFunction
        ef.name := rec.ref.name
        ef.file := rec.ref.file
        ef.line = rec.ref.line
        ef.is_lambda = rec.ref.is_lambda
        ef.canonical := rec.canonical
        env.functions |> emplace(ef)
    }
    let body = sprint_json(env, true)
    var ok = false
    fopen(path, "wb") $(fw) {
        if (fw == null) {
            return
        }
        ok = true
        fw |> fprint(body)
    }
    return ok
}


// An imported function record is "well-formed" when it has a non-empty
// canonical (the only field that affects clustering), a non-empty name
// and file (so reports are interpretable), and a non-negative line.
// Empty canonicals are the dangerous case: with `--min-tokens 0` they
// would all collide in a single fake exact-cluster.
def is_well_formed(ef : ExportedFunction) : bool {
    return !empty(ef.canonical) && !empty(ef.name) && !empty(ef.file) && ef.line >= 0
}


// Slash-direction normalization for file paths. Lives here (rather than
// in pipeline.das) because exchange is below pipeline in the dep graph
// and `member_id` needs it. pipeline.das re-uses this via `require
// exchange`, so callers see one canonical helper.
def public normalize_path(p : string) : string {
    // `fio::normalize` resolves `..`/`.` and uses the platform separator
    // (backslash on Windows, forward slash on POSIX). The trailing
    // `replace` lands on a canonical forward-slash form so a corpus
    // exported on one OS matches candidates compiled on another.
    return normalize(p) |> replace("\\", "/")
}


// Stable member identity used to compare a freshly-extracted record
// against a baseline. Same shape as cluster.das `member_key`, copied
// here to avoid a require cycle (exchange already needs to know about
// records but not about clustering helpers). Path is normalized so
// Windows runs (mixed `\` and `/` from `fileInfo.name`) match Unix
// baselines.
def member_id(file : string; line : int; name : string) : string {
    return "{normalize_path(file)}:{line}:{name}"
}


// Read an exported envelope and project it down to two sets:
//   * `canonicals`  — set of canonical texts (used by --baseline-strict
//     to drop clusters whose canonical already existed in baseline).
//   * `members`     — set of `file:line:name` identities (used by default
//     baseline mode to tag records that DIDN'T exist in baseline as
//     candidates). Member-level keying catches "growth" — a new copy of
//     an already-tracked canonical added in a new location — which a
//     pure canonical-set diff would miss (the canonical is in baseline,
//     so the new member would never be flagged).
//
// Trade-off of `file:line:name`: an unrelated edit that shifts line
// numbers will look like "new members" and surface those clusters in
// the report. Acceptable for CI gating — re-checking touched code is
// the right default; the user can ignore false positives the same way
// they would for any line-based diff tool.
def public load_baseline_canonicals(path : string;
                                    var canonicals : table<string; void?>;
                                    var members : table<string; void?>;
                                    var error : string&) : bool {
    let text = fread(path)
    if (empty(text)) {
        error := "could not read baseline file (empty or missing): {path}"
        return false
    }
    var env = ExportEnvelope()
    if (!sscan_json(text, env)) {
        error := "could not parse baseline JSON: {path}"
        return false
    }
    if (env.schema_version != EXCHANGE_SCHEMA_VERSION) {
        error := "unsupported baseline schema_version {env.schema_version} (expected {EXCHANGE_SCHEMA_VERSION}): {path}"
        return false
    }
    for (ef in env.functions) {
        if (empty(ef.canonical)) {
            continue
        }
        if (!key_exists(canonicals, ef.canonical)) {
            canonicals |> insert(ef.canonical, null)
        }
        let mid = member_id(ef.file, ef.line, ef.name)
        if (!key_exists(members, mid)) {
            members |> insert(mid, null)
        }
    }
    return true
}


// `quiet=true` suppresses the "skipped N malformed records" warning print —
// required when called from the MCP server, whose stdio transport reserves
// stdout for JSON-RPC. Default false preserves CLI behavior.
def public read_import(path : string; var records : array<FuncRecord>;
                       want_sigs : bool; min_tokens : int;
                       var error : string&;
                       quiet : bool = false) : bool {
    let text = fread(path)
    if (empty(text)) {
        error := "could not read file (empty or missing): {path}"
        return false
    }
    var env = ExportEnvelope()
    if (!sscan_json(text, env)) {
        error := "could not parse JSON: {path}"
        return false
    }
    if (env.schema_version != EXCHANGE_SCHEMA_VERSION) {
        error := "unsupported schema_version {env.schema_version} (expected {EXCHANGE_SCHEMA_VERSION}): {path}"
        return false
    }
    records |> reserve(length(records) + length(env.functions))
    var skipped = 0
    for (ef in env.functions) {
        if (!is_well_formed(ef)) {
            skipped++
            continue
        }
        var rec = FuncRecord()
        rec.ref.name := ef.name
        rec.ref.file := ef.file
        rec.ref.line = ef.line
        rec.ref.is_lambda = ef.is_lambda
        rec.canonical := ef.canonical
        derive_sig_and_count(rec, want_sigs, min_tokens)
        records |> emplace(rec)
    }
    if (skipped > 0 && !quiet) {
        print("warning: skipped {skipped} malformed function record(s) during import\n")
    }
    return true
}
