options gen2
options indenting = 4

module exchange_schema shared public

require daslib/json_boost
require daslib/strings_boost      // split — the box identity parse
require daslib/strings_convert    // try_to_int — provenance version is validated, never trusted
require daslib/fio                // base_name — the record path redactor (engine-free)
require strings                   // peek_data — charset checks

// Engine-free validation for exchange submissions (bench-record stores + tune sidecars): the
// ladder service builds on this, so requiring dasLLAMA/dasLLVM here is forbidden. Field truth
// is pinned by tests/test_exchange_schema.das (in-tree corpus + live writer round-trip).

let private MAX_ERRORS = 64
let private MAX_MODELS = 256
let private MAX_RUNS = 32
let private MAX_TESTS = 256
let private MAX_FILES = 128
let private MAX_TABLE_KEYS = 64      // hardware / provenance / per-test objects
let private MAX_KERNELS = 512
let private MAX_RACE_VARIANTS = 128
let private MAX_RUNTIME_KNOBS = 256
let private MAX_NAME = 256
let private MAX_SHORT = 64
let private MAX_CMD = 8192
let private MAX_TEXT = 65536         // ASR transcripts — the largest legitimate string in a record
let private MAX_PROV_VALUE = 1024
let private MAX_MEASURE = 1.0e12lf   // tok/s, ms, counts — beyond is garbage; a NaN fails the compare too
let private MAX_BYTES = 1.0e15lf

//! The five '|'-joined fields of a sidecar provenance box string
//! ("darwin|arm64|MacBookPro18,2|25F84|Apple M1 Max").
struct BoxIdentity {
    platform : string
    arch : string
    model_id : string
    os_build : string
    cpu : string
}

//! What the exchange indexes a sidecar by, lifted without trusting the rest of the document.
//! `dasllama_version` 0 = pre-exchange sidecar (no stamp); submissions require >= 1.
struct SidecarInfo {
    dasllama_version : int
    box : string
    identity : BoxIdentity
    engine_sha : string
    written : string
    noise : string
    kernel_count : int
}

//! Split a provenance box string into its five identity fields. False when the shape is off;
//! platform, arch, and cpu must be non-empty (model_id and os_build may legitimately be blank).
def parse_box_string(box : string; var out : BoxIdentity) : bool {
    let parts <- split(box, "|")
    if (length(parts) != 5) {
        return false
    }
    out.platform = parts[0]
    out.arch = parts[1]
    out.model_id = parts[2]
    out.os_build = parts[3]
    out.cpu = parts[4]
    return !empty(out.platform) && !empty(out.arch) && !empty(out.cpu)
}

// ===== error accumulation =====

def private err(var errors : array<string>; path : string; msg : string) {
    if (length(errors) < MAX_ERRORS) {
        errors |> push("{path}: {msg}")
    } elif (length(errors) == MAX_ERRORS) {
        errors |> push("... further errors suppressed")
    }
}

// a missing key and an explicit JSON null are the same thing: absent. json_boost's ?[]
// returns a _null sentinel for missing keys, so == null alone never detects absence.
def private jget(v : JsonValue const?; key : string) : JsonValue const? {
    var f = v?[key]
    if (f == null || f is _null) {
        return null
    }
    return f
}

// ===== charset / shape primitives =====

// [a-z0-9_.-] — kernel names, variant suffixes, runtime knob names; these strings reach web
// pages verbatim, so the charset is the injection guard, not a style preference
def private ident_ok(s : string) : bool {
    if (empty(s)) {
        return false
    }
    var ok = true
    peek_data(s) $(arr) {
        for (b in arr) {
            let c = int(b)
            ok = ok && ((c >= 'a' && c <= 'z') || is_number(c) || c == '_' || c == '.' || c == '-')
        }
    }
    return ok
}

// a tuner selection: empty (nothing applied), one identifier, or a comma-joined identifier
// list (the metal_tensor crown list) — same injection guarantee as ident_ok, comma added
def private selection_ok(s : string) : bool {
    if (empty(s)) {
        return true
    }
    if (length(s) > MAX_NAME) {
        return false
    }
    let parts <- split(s, ",")
    for (p in parts) {
        if (!ident_ok(p) || length(p) > MAX_SHORT) {
            return false
        }
    }
    return true
}

// Safe for verbatim rendering on a public page: printable ASCII only, excluding the bytes
// that break out of an HTML text or quoted-attribute context. Permissive enough for CPU brand
// strings ("AMD64 Family 23 Model 49 Stepping 0, AuthenticAMD", "Apple M1 Max") and the noise
// probe log; strict enough that no provenance value can inject a tag or close an attribute.
// The client escapes too — this is the fail-closed server half of that guard (a hostile or
// non-browser client cannot land markup in a stored column).
def private render_safe_ok(s : string) : bool {
    var ok = true
    peek_data(s) $(arr) {
        for (b in arr) {
            let c = int(b)
            ok = ok && (c >= 0x20 && c <= 0x7E
                && c != '<' && c != '>' && c != '"' && c != '\'' && c != '\\' && c != '`')
        }
    }
    return ok
}

def private hex_ok(s : string; want_len : int) : bool {
    if (length(s) != want_len) {
        return false
    }
    var ok = true
    peek_data(s) $(arr) {
        for (b in arr) {
            let c = int(b)
            ok = ok && (is_number(c) || (c >= 'a' && c <= 'f'))
        }
    }
    return ok
}

def private date_ok(s : string) : bool {
    if (length(s) != 10) {
        return false
    }
    var ok = true
    peek_data(s) $(arr) {
        for (i in range(10)) {
            let c = int(arr[i])
            ok = ok && ((i == 4 || i == 7) ? c == '-' : is_number(c))
        }
    }
    return ok
}

// ===== typed field checks =====

// returns the value ("" when absent/invalid) so callers can branch on it after the check
def private chk_str(v : JsonValue const?; key : string; required : bool; max_len : int; path : string;
                    var errors : array<string>) : string {
    let f = jget(v, key)
    if (f == null) {
        if (required) {
            err(errors, "{path}.{key}", "required string missing")
        }
        return ""
    }
    if (!(f is _string)) {
        err(errors, "{path}.{key}", "not a string")
        return ""
    }
    let s = f as _string
    if (required && empty(s)) {
        err(errors, "{path}.{key}", "empty")
    }
    if (length(s) > max_len) {
        err(errors, "{path}.{key}", "longer than {max_len}")
    }
    return s
}

// accepts _number and _longint; the [lo..hi] compare is written so a NaN fails it
def private chk_num(v : JsonValue const?; key : string; required : bool; lo, hi : double; path : string;
                    var errors : array<string>) : double {
    let f = jget(v, key)
    if (f == null) {
        if (required) {
            err(errors, "{path}.{key}", "required number missing")
        }
        return 0.0lf
    }
    var d = 0.0lf
    if (f is _number) {
        d = f as _number
    } elif (f is _longint) {
        d = double(f as _longint)
    } else {
        err(errors, "{path}.{key}", "not a number")
        return 0.0lf
    }
    if (!(d >= lo && d <= hi)) {
        err(errors, "{path}.{key}", "out of range")
    }
    return d
}

// ===== bench record stores =====

def private validate_hardware(run : JsonValue const?; path : string; var errors : array<string>) {
    let hw = jget(run, "hardware")
    if (hw == null || !(hw is _object)) {
        err(errors, "{path}.hardware", "required object missing")
        return
    }
    if (length(hw as _object) > MAX_TABLE_KEYS) {
        err(errors, "{path}.hardware", "too many fields")
        return
    }
    chk_str(hw, "cpu", true, MAX_NAME, "{path}.hardware", errors)
    // the rest of the block is auto-probed and evolves; enforce flat scalars, not a field list
    for (k, f in keys(hw as _object), values(hw as _object)) {
        if (f == null) {
            continue
        }
        if (f is _string) {
            if (length(f as _string) > MAX_NAME * 2) {
                err(errors, "{path}.hardware.{k}", "longer than {MAX_NAME * 2}")
            }
        } elif (!(f is _number) && !(f is _longint) && !(f is _bool)) {
            err(errors, "{path}.hardware.{k}", "not a flat scalar")
        }
    }
}

def private validate_tests(run : JsonValue const?; path : string; var errors : array<string>) {
    let tests = jget(run, "tests")
    if (tests == null || !(tests is _object)) {
        err(errors, "{path}.tests", "required object missing")
        return
    }
    if (empty(tests as _object)) {
        err(errors, "{path}.tests", "empty")
        return
    }
    if (length(tests as _object) > MAX_TESTS) {
        err(errors, "{path}.tests", "more than {MAX_TESTS} rows")
        return
    }
    for (k, row in keys(tests as _object), values(tests as _object)) {
        let rp = "{path}.tests[{k}]"
        if (empty(k) || length(k) > MAX_NAME) {
            err(errors, rp, "bad test key")
        }
        if (row == null || !(row is _object)) {
            err(errors, rp, "not an object")
            continue
        }
        if (length(row as _object) > 16) {
            err(errors, rp, "too many fields")
            continue
        }
        chk_num(row, "tok_s", false, 0.0lf, MAX_MEASURE, rp, errors)
        chk_num(row, "stddev", false, 0.0lf, MAX_MEASURE, rp, errors)
        chk_num(row, "ms", false, 0.0lf, MAX_MEASURE, rp, errors)
        chk_num(row, "audio_s", false, 0.0lf, MAX_MEASURE, rp, errors)
        chk_num(row, "encode_ms", false, 0.0lf, MAX_MEASURE, rp, errors)
        chk_str(row, "text", false, MAX_TEXT, rp, errors)
        if (jget(row, "tok_s") == null && jget(row, "ms") == null) {
            err(errors, rp, "carries neither tok_s nor ms")
        }
    }
}

def private validate_files(run : JsonValue const?; path : string; var errors : array<string>) {
    let files = jget(run, "files")
    if (files == null) {
        return
    }
    if (!(files is _array)) {
        err(errors, "{path}.files", "not an array")
        return
    }
    if (length(files as _array) > MAX_FILES) {
        err(errors, "{path}.files", "more than {MAX_FILES} entries")
        return
    }
    var i = 0
    for (f in files as _array) {
        let fp = "{path}.files[{i}]"
        if (f == null || !(f is _object)) {
            err(errors, fp, "not an object")
        } else {
            chk_str(f, "role", true, MAX_SHORT, fp, errors)
            chk_str(f, "name", true, MAX_NAME, fp, errors)
            let sha = chk_str(f, "sha256", false, MAX_SHORT, fp, errors)
            if (!empty(sha) && !hex_ok(sha, 64)) {
                err(errors, fp, "sha256 is not 64 hex chars")
            }
            chk_num(f, "bytes", false, 0.0lf, MAX_BYTES, fp, errors)
        }
        i++
    }
}

def private validate_run(run : JsonValue const?; submission : bool; path : string;
                         var errors : array<string>) : bool {
    if (run == null || !(run is _object)) {
        err(errors, path, "not an object")
        return false
    }
    let engine = chk_str(run, "engine", true, MAX_SHORT, path, errors)
    chk_str(run, "backend", true, MAX_SHORT, path, errors)
    chk_str(run, "flavor", true, MAX_SHORT, path, errors)
    chk_str(run, "box", true, MAX_NAME, path, errors)
    chk_str(run, "cmd", true, MAX_CMD, path, errors)
    let date = chk_str(run, "date", true, MAX_SHORT, path, errors)
    if (!empty(date) && !date_ok(date)) {
        err(errors, "{path}.date", "not YYYY-MM-DD")
    }
    chk_num(run, "threads", true, 1.0lf, 65536.0lf, path, errors)
    chk_str(run, "env", false, MAX_CMD, path, errors)
    chk_str(run, "sha", false, MAX_SHORT, path, errors)
    chk_str(run, "version", false, MAX_SHORT, path, errors)
    chk_str(run, "versions", false, MAX_PROV_VALUE, path, errors)
    chk_str(run, "tune", false, MAX_PROV_VALUE, path, errors)
    chk_str(run, "tune_sha", false, MAX_NAME, path, errors)
    chk_str(run, "workload", false, MAX_SHORT, path, errors)
    chk_str(run, "exec_fmt", false, MAX_NAME * 2, path, errors)
    let source = chk_str(run, "source", false, MAX_SHORT, path, errors)
    chk_str(run, "comment", false, MAX_CMD, path, errors)
    chk_str(run, "void_clips", false, MAX_PROV_VALUE, path, errors)
    chk_num(run, "dasllama_version", false, 0.0lf, 1000000.0lf, path, errors)
    validate_hardware(run, path, errors)
    validate_tests(run, path, errors)
    validate_files(run, path, errors)
    let is_das = engine == "das"
    if (submission) {
        // the service stamps source itself; a submission claiming official is hostile by definition
        if (!empty(source) && source != "community") {
            err(errors, "{path}.source", "a submission may not claim '{source}'")
        }
        if (is_das) {
            // stamped or it does not ship: the receipts that make a das row reproducible
            chk_num(run, "dasllama_version", true, 1.0lf, 1000000.0lf, path, errors)
            chk_str(run, "version", true, MAX_SHORT, path, errors)
            chk_str(run, "sha", true, MAX_SHORT, path, errors)
            chk_str(run, "exec_fmt", true, MAX_NAME * 2, path, errors)
            chk_str(run, "tune", true, MAX_PROV_VALUE, path, errors)
            chk_str(run, "tune_sha", true, MAX_NAME, path, errors)
        }
    }
    return is_das
}

def private validate_store_doc(doc : JsonValue const?; submission : bool; var errors : array<string>) : bool {
    if (!(doc is _array)) {
        err(errors, "$", "root is not an array")
        return false
    }
    if (length(doc as _array) > MAX_MODELS) {
        err(errors, "$", "more than {MAX_MODELS} models")
        return false
    }
    var das_runs = 0
    var i = 0
    for (m in doc as _array) {
        let mp = "models[{i}]"
        if (m == null || !(m is _object)) {
            err(errors, mp, "not an object")
        } else {
            chk_str(m, "gguf", true, MAX_NAME, mp, errors)
            chk_str(m, "arch", false, MAX_NAME, mp, errors)
            chk_str(m, "quant", false, MAX_SHORT, mp, errors)
            chk_str(m, "note", false, MAX_CMD, mp, errors)
            chk_num(m, "params_b", false, 0.0lf, 1000000.0lf, mp, errors)
            chk_num(m, "active_b", false, 0.0lf, 1000000.0lf, mp, errors)
            chk_num(m, "size_bytes", false, 0.0lf, MAX_BYTES, mp, errors)
            let runs = jget(m, "runs")
            if (runs == null || !(runs is _array)) {
                err(errors, "{mp}.runs", "required array missing")
            } else {
                if (empty(runs as _array) || length(runs as _array) > MAX_RUNS) {
                    err(errors, "{mp}.runs", "empty or more than {MAX_RUNS} runs")
                } else {
                    var j = 0
                    for (r in runs as _array) {
                        if (validate_run(r, submission, "{mp}.runs[{j}]", errors)) {
                            das_runs++
                        }
                        j++
                    }
                }
            }
        }
        i++
    }
    if (submission && das_runs == 0) {
        err(errors, "$", "a submission carries at least one das run")
    }
    return empty(errors)
}

// a path-bearing string loses its leading directories; a KEY=path token keeps its key
def private redact_bare(s : string) : string {
    return (find(s, "/") >= 0 || find(s, "\\") >= 0) ? base_name(s) : s
}

def private redact_token(tok : string) : string {
    let eq = find(tok, "=")
    if (eq >= 0) {
        return "{slice(tok, 0, eq)}={redact_bare(slice(tok, eq + 1))}"
    }
    return redact_bare(tok)
}

def private redact_cmd(s : string) : string {
    let toks <- split(s, " ")
    return join([for (tok in toks); redact_token(tok)], " ")
}

def private redact_run(var r : JsonValue?) {
    if (r == null || !(r.value is _object)) {
        return
    }
    for (key in fixed_array("cmd", "env")) {
        let v : string = r?[key] ?? ""
        if (!empty(v)) {
            update(r, key, JV(redact_cmd(v)))
        }
    }
    var files = r?["files"]
    if (files != null && files.value is _array) {
        var farr & = unsafe(files.value as _array)
        for (f in farr) {
            if (f != null && f.value is _object) {
                let nm : string = f?["name"] ?? ""
                if (!empty(nm)) {
                    update(f, "name", JV(base_name(nm)))
                }
            }
        }
    }
}

//! Redact absolute paths from a community record store: each run's `cmd` / `env` token that
//! carries a path separator loses its leading directories (basename), and each `files[].name`
//! is reduced to its basename — the receipt keeps its flags and model name, drops the username
//! and home layout. Idempotent (client redacts, server redacts again). "" if it does not parse.
def redact_record_paths(text : string) : string {
    var jerr = ""
    var doc = read_json(text, jerr)
    if (doc == null || !(doc.value is _array)) {
        return ""
    }
    var models & = unsafe(doc.value as _array)   // nolint:LINT003 — a var ref: the run objects below are mutated in place
    for (m in models) {
        var runs = m?["runs"]
        if (runs != null && runs.value is _array) {
            var rarr & = unsafe(runs.value as _array)
            for (r in rarr) {
                redact_run(r)
            }
        }
    }
    let out = write_json(doc)
    delete_json(doc)
    return out
}

//! Shape-level validation of a bench-record store (the `performance/records/<box>.json` form:
//! an array of models, each with sibling das/reference runs). True = well-formed; false fills
//! `errors` with `path: problem` lines (capped).
def validate_record_store(text : string; var errors : array<string>) : bool {
    var jerr = ""
    var doc = read_json(text, jerr)
    if (doc == null) {
        err(errors, "$", "json parse failed: {jerr}")
        return false
    }
    return validate_store_doc(doc, false, errors)
}

//! Submission-grade validation for community-submitted records: shape rules plus the receipts —
//! every das row stamped (dasllama_version/version/sha/exec_fmt/tune/tune_sha), `source` never
//! "official", and at least one das run present.
def validate_record_submission(text : string; var errors : array<string>) : bool {
    var jerr = ""
    var doc = read_json(text, jerr)
    if (doc == null) {
        err(errors, "$", "json parse failed: {jerr}")
        return false
    }
    return validate_store_doc(doc, true, errors)
}

// ===== tune sidecars =====

def private validate_kernels(doc : JsonValue const?; var errors : array<string>) {
    let kernels = jget(doc, "kernels")
    if (kernels == null || !(kernels is _object)) {
        err(errors, "kernels", "required object missing")
        return
    }
    if (empty(kernels as _object) || length(kernels as _object) > MAX_KERNELS) {
        err(errors, "kernels", "empty or more than {MAX_KERNELS} entries")
        return
    }
    for (k, f in keys(kernels as _object), values(kernels as _object)) {
        if (!ident_ok(k) || length(k) > MAX_SHORT) {
            err(errors, "kernels", "bad family name '{k}'")
        }
        // an empty winner is a real tuner state: the family raced and no variant applied
        if (f == null || !(f is _string) || !selection_ok(f as _string)) {
            err(errors, "kernels[{k}]", "winner is not an identifier")
        }
    }
}

def private validate_provenance(doc : JsonValue const?; var errors : array<string>) {
    let prov = jget(doc, "provenance")
    if (prov == null || !(prov is _object)) {
        err(errors, "provenance", "required object missing")
        return
    }
    if (length(prov as _object) > MAX_TABLE_KEYS) {
        err(errors, "provenance", "too many entries")
        return
    }
    for (k, f in keys(prov as _object), values(prov as _object)) {
        if (!ident_ok(k) || length(k) > MAX_SHORT) {
            err(errors, "provenance", "bad key '{k}'")
        }
        if (f == null || !(f is _string)) {
            err(errors, "provenance[{k}]", "not a string")
        } elif (length(f as _string) > MAX_PROV_VALUE) {
            err(errors, "provenance[{k}]", "longer than {MAX_PROV_VALUE}")
        } elif (!render_safe_ok(f as _string)) {
            // every provenance value can reach a public page verbatim — the charset is the
            // injection guard, not a style preference
            err(errors, "provenance[{k}]", "carries a byte unsafe to render")
        }
    }
    let box = chk_str(prov, "box", true, MAX_NAME, "provenance", errors)
    var ident : BoxIdentity
    if (!empty(box) && !parse_box_string(box, ident)) {
        err(errors, "provenance.box", "not the five-field platform|arch|model_id|os_build|cpu form")
    }
    let engine_sha = chk_str(prov, "engine_sha", true, MAX_SHORT, "provenance", errors)
    if (!empty(engine_sha) && !ident_ok(engine_sha)) {
        err(errors, "provenance.engine_sha", "not an identifier")   // a git short sha or 'unknown'
    }
}

def private validate_runtime(doc : JsonValue const?; var errors : array<string>) {
    let rt = jget(doc, "runtime")
    if (rt == null) {
        return
    }
    if (!(rt is _object)) {
        err(errors, "runtime", "not an object")
        return
    }
    if (length(rt as _object) > MAX_RUNTIME_KNOBS) {
        err(errors, "runtime", "more than {MAX_RUNTIME_KNOBS} knobs")
        return
    }
    // knobs are clamped numeric scalars or family selections by contract — a nested
    // structure here is a hostile document
    for (k, f in keys(rt as _object), values(rt as _object)) {
        if (!ident_ok(k) || length(k) > MAX_SHORT) {
            err(errors, "runtime", "bad knob name '{k}'")
        }
        if (f != null && f is _string) {
            if (!selection_ok(f as _string)) {
                err(errors, "runtime[{k}]", "selection is not an identifier list")
            }
        } elif (f == null || (!(f is _number) && !(f is _longint))) {
            err(errors, "runtime[{k}]", "not a number or selection")
        } else {
            let d = (f is _number) ? (f as _number) : double(f as _longint)
            if (!(d >= -MAX_MEASURE && d <= MAX_MEASURE)) {
                err(errors, "runtime[{k}]", "out of range")
            }
        }
    }
}

def private validate_race(doc : JsonValue const?; var errors : array<string>) {
    let race = jget(doc, "race")
    if (race == null) {
        return
    }
    if (!(race is _object)) {
        err(errors, "race", "not an object")
        return
    }
    if (length(race as _object) > MAX_KERNELS) {
        err(errors, "race", "more than {MAX_KERNELS} families")
        return
    }
    for (k, fam in keys(race as _object), values(race as _object)) {
        let fp = "race[{k}]"
        if (!ident_ok(k) || length(k) > MAX_SHORT) {
            err(errors, "race", "bad family name '{k}'")
        }
        if (fam == null || !(fam is _object)) {
            err(errors, fp, "not an object")
            continue
        }
        let w = chk_str(fam, "winner", true, MAX_SHORT, fp, errors)
        if (!empty(w) && !ident_ok(w)) {
            err(errors, "{fp}.winner", "not an identifier")
        }
        let fb = chk_str(fam, "fallback", false, MAX_SHORT, fp, errors)
        if (!empty(fb) && !ident_ok(fb)) {
            err(errors, "{fp}.fallback", "not an identifier")
        }
        chk_num(fam, "floor_pct", false, 0.0lf, 1000000.0lf, fp, errors)
        let rows = jget(fam, "rows")
        if (rows == null) {
            continue
        }
        if (!(rows is _object)) {
            err(errors, "{fp}.rows", "not an object")
            continue
        }
        if (length(rows as _object) > MAX_RACE_VARIANTS) {
            err(errors, "{fp}.rows", "more than {MAX_RACE_VARIANTS} variants")
            continue
        }
        for (vk, row in keys(rows as _object), values(rows as _object)) {
            if (!ident_ok(vk) || length(vk) > MAX_SHORT) {
                err(errors, "{fp}.rows", "bad variant name '{vk}'")
            }
            if (row == null || !(row is _object) || length(row as _object) > 16) {
                err(errors, "{fp}.rows[{vk}]", "not a small object")
                continue
            }
            for (mk, mv in keys(row as _object), values(row as _object)) {
                if (!ident_ok(mk) || mv == null || (!(mv is _number) && !(mv is _longint))) {
                    err(errors, "{fp}.rows[{vk}].{mk}", "not a numeric measurement")
                }
            }
        }
    }
}

def private validate_sidecar_doc(doc : JsonValue const?; submission : bool; var errors : array<string>) : bool {
    if (!(doc is _object)) {
        err(errors, "$", "root is not an object")
        return false
    }
    // allow-list the four known sections: an unknown top-level key is stored verbatim, served
    // by /api/sidecar/:sha, and written into an adopter's local manifest — and the privacy
    // strip only walks `provenance`, so `notes: {binary: "/home/..."}` would leak straight
    // through. Reject the whole document rather than store an un-validated section.
    for (k in keys(doc as _object)) {
        if (k != "kernels" && k != "runtime" && k != "provenance" && k != "race") {
            err(errors, "$", "unknown top-level section '{k}'")
        }
    }
    validate_kernels(doc, errors)
    validate_provenance(doc, errors)
    validate_runtime(doc, errors)
    validate_race(doc, errors)
    if (submission) {
        let prov = jget(doc, "provenance")
        let vs = prov?["dasllama_version"] ?? ""
        let parsed = try_to_int(vs)
        if (!(parsed |> is_ok) || (parsed ?? 0) < 1) {
            err(errors, "provenance.dasllama_version", "a submitted sidecar carries the release counter (>= 1)")
        }
    }
    return empty(errors)
}

//! Strip what must never leave (or reach) a public store: `provenance.binary` (a full local
//! path with the username in it) and any other path-shaped provenance value. Returns the
//! stripped document, "" when the input does not parse. The submitting client AND the ladder
//! service both run this — defense in depth against a client that skipped it.
def exchange_strip_private(text : string) : string {
    var jerr = ""
    var doc = read_json(text, jerr)
    if (doc == null) {
        return ""
    }
    var prov = doc?["provenance"]
    if (prov != null && prov.value is _object) {
        var pt & = unsafe(prov.value as _object)
        var drop : array<string>
        for (k, v in keys(pt), values(pt)) {
            var path_shaped = false
            if (v != null && v.value is _string) {
                let s = v.value as _string
                path_shaped = find(s, "/") >= 0 || find(s, "\\") >= 0
            }
            if (k == "binary" || path_shaped) {
                drop |> push("{k}")   // clone — k aliases the key the erase below frees
            }
        }
        for (k in drop) {
            delete_json(pt[k])
            pt |> erase(k)
        }
        delete drop
    }
    let out = write_json(doc)
    delete_json(doc)
    return out
}

//! Shape-level validation of a tune sidecar (`<box>.tune.<sha>.json`): kernels + provenance
//! required and identifier-clean, runtime knobs flat numeric, race rows well-formed. The
//! charset rules double as the injection guard for everything a site renders verbatim.
def validate_sidecar(text : string; var errors : array<string>) : bool {
    var jerr = ""
    var doc = read_json(text, jerr)
    if (doc == null) {
        err(errors, "$", "json parse failed: {jerr}")
        return false
    }
    return validate_sidecar_doc(doc, false, errors)
}

//! Submission-grade sidecar validation: shape rules plus `provenance.dasllama_version` >= 1 —
//! the exchange keys validity on (version, box), so an unversioned sidecar cannot be served.
def validate_sidecar_submission(text : string; var errors : array<string>) : bool {
    var jerr = ""
    var doc = read_json(text, jerr)
    if (doc == null) {
        err(errors, "$", "json parse failed: {jerr}")
        return false
    }
    return validate_sidecar_doc(doc, true, errors)
}

//! Lift the exchange index fields out of a sidecar without trusting the rest of the document.
//! False when it does not parse, the box identity is malformed, or there are no kernels.
//! Strings are copied out, so the info outlives the parsed tree under any heap policy.
def parse_sidecar_info(text : string; var info : SidecarInfo) : bool {
    var jerr = ""
    var doc = read_json(text, jerr)
    if (doc == null || !(doc is _object)) {
        return false
    }
    let prov = jget(doc, "provenance")
    let bx = prov?["box"] ?? ""
    let sha = prov?["engine_sha"] ?? ""
    let wr = prov?["written"] ?? ""
    let noise = prov?["noise"] ?? ""
    info.box = "{bx}"
    info.engine_sha = "{sha}"
    info.written = "{wr}"
    info.noise = "{noise}"
    let vs = prov?["dasllama_version"] ?? ""
    if (!empty(vs)) {
        let parsed = try_to_int(vs)
        info.dasllama_version = (parsed |> is_ok) ? (parsed ?? 0) : 0
    }
    let kernels = jget(doc, "kernels")
    info.kernel_count = (kernels != null && kernels is _object) ? length(kernels as _object) : 0
    var ident : BoxIdentity
    if (!parse_box_string(info.box, ident)) {
        return false
    }
    info.identity.platform = "{ident.platform}"
    info.identity.arch = "{ident.arch}"
    info.identity.model_id = "{ident.model_id}"
    info.identity.os_build = "{ident.os_build}"
    info.identity.cpu = "{ident.cpu}"
    return info.kernel_count > 0
}
