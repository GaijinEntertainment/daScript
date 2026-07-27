options gen2
options indenting = 4
options no_aot = true

module llvm_tune shared public

require daslib/ast
require daslib/rtti
require daslib/ast_boost
require daslib/templates_boost
require daslib/fio
require daslib/strings_boost
require daslib/command_line
require daslib/json_boost
require daslib/tty
require daslib/clargs
require strings
require math
require llvm/daslib/llvm_code public

//! Generalized tune framework (gemm_generator_plan.md part 2). `[tune_perm(...), ...,
//! tune(gen="key", fallback="suffix")]` — perm rows first, `tune` last — turns one reference
//! function into a tuned `[llvm_code]` kernel family. `[tune_companion(fn=, gen=)]` rows
//! (between the perm rows and `tune`) stamp sibling functions with the SAME perm from the same
//! manifest entry — the two-function stamp (a kernel and its repack-layout query can never
//! desync). `DAS_TUNE_MODE` selects the compile-time mode:
//!  - normal (default): stamp exactly one winner onto the function itself — the manifest
//!    entry when one exists, else the `fallback=` perm, else the reference body (a manifest
//!    with no entry for a function falls through to its fallback — new kernel families must
//!    not be dropped to the reference tier by a stale manifest; an explicit "reference"
//!    entry forces the original body). No variant stubs are emitted; the `<name>_variants()`
//!    registry still exists (reference row only) so harness code compiles in every mode.
//!  - tune / test: stamp the full grid as `<name>__<suffix>` clones plus a `<name>_variants()`
//!    registry; a harness benches them (tune, then `tune_manifest_set` the winner) or
//!    bit-exact-gates every variant against the reference row (test).
//!
//! Winners live in the PER-APP sidecar `<app>.tune.json` beside the app — the root script
//! this process runs (the first `.das` on the command line), or the binary itself when
//! there is none. One sectioned file per app: `"kernels"` (this module's stamps and the
//! loop-hint perms), `"runtime"` (library runtime knobs), `"provenance"` (who wrote it).
//! A sidecar OLDER than the running binary is stale and reads as absent everywhere — a
//! rebuilt binary invalidates every measured winner. `DAS_TUNE_MANIFEST` env overrides the
//! location outright. A library declares `[tune_scope(name=, tuner=)]` to wire the harness
//! that regenerates its kernels; an app declares `[tune_policy(missing = fallback|warn|
//! error|auto|restart)]`; `--tune` after `--` on the app's command line forces a fresh
//! tuner run. AOT builds are fully tune-free (`tune_aot_gate`); `-exe` builds stamp
//! normally but run no policy (`tune_exe_gate`) — an exe built WITH a sidecar is a
//! local-use artifact shipping those winners, one built without ships the generic per-ISA
//! `fallback=` stamps.

// ===== manifest + mode =====

//! Compile-time tune mode from the `DAS_TUNE_MODE` env var: "normal" (default), "tune", "test".
def tune_mode() : string {
    let m = get_env_variable("DAS_TUNE_MODE")
    return empty(m) ? "normal" : m
}

//! True when this compile must stay stamp-free. Two triggers: `policies.tune_frozen` — the
//! dedicated "cross-box artifact" mark (set by the `-aot` C++ generator and by dastest's AST
//! serializer; per-box stamps must never leak into an artifact another box consumes) — and an
//! AOT-consuming run (`policies.aot`) WITHOUT the JIT (a stamp changes the semantic hash, so a
//! stamped function would fail the AOT link; under `-jit` the JIT supersedes AOT bodies and
//! tuned stamps are wanted). When the gate fires every tune annotation is fully inert: no
//! manifest/scope reads, no stamps (`fallback=` included), no policy, no injected checks —
//! only the reference-row `<name>_variants()` registries are emitted so harness code compiles.
def tune_aot_gate() : bool {
    return (compiling_program().policies.tune_frozen
        || (compiling_program().policies.aot && !compiling_program().policies.jit_enabled))
}

//! True when this compile must retain reference bodies. Documentation roots
//! inspect source APIs and, like frozen/AOT consumers, must never consult or
//! stamp per-box tune state.
def tune_stamp_free_gate() : bool {
    return tune_aot_gate() || is_building_documentation()
}

//! True when this compile produces a standalone exe (`llvm-jit -exe`). Stamping still runs:
//! an exe built on a box WITH a manifest ships those winners (a local-use artifact by
//! definition), and without one it ships the generic per-ISA `fallback=` stamps. Only the
//! policy rail (`[tune_policy]`, `--tune`) is dead — a frozen artifact must not demand or
//! run tuning.
def tune_exe_gate() : bool {
    return compiling_program().policies.jit_exe_mode
}

// ===== the per-app sidecar =====

//! The app this process runs: the first `.das` argument on the command line before `--`.
//! Macro time and runtime read the same argv, so compile-time stamps and the runtime write
//! API agree on the file. "" in a standalone exe or an embedded host — the binary itself is
//! the app then (see `tune_manifest_path`).
def tune_app_script() : string {
    // aliases a process-global Array header (locked) — do not delete
    var argv <- get_command_line_arguments()
    var first = true
    for (a in argv) {
        if (!first) {
            break if (a == "--")
            if (a |> ends_with(".das")) {
                return "{a}"
            }
        }
        first = false
    }
    return ""
}

// `<stem>.tune.json` — foo/app.das, foo/app.exe and foo/app all map to foo/app.tune.json
def private sidecar_for(app : string) : string {
    var stem = app
    if (stem |> ends_with(".das") || stem |> ends_with(".exe")) {
        stem = slice(stem, 0, length(stem) - 4)
    }
    return "{stem}.tune.json"
}

// the binary THIS process runs as — argv[0] verbatim (a standalone exe is its own binary;
// a `daslang app.das` run anchors on the daslang binary). NOT get_das_exe(): that derives
// "<argv[0] dir>/daslang.exe" — the interpreter-to-SPAWN, which does not exist inside a
// released bundle and silently mis-anchored the exe's sidecar + staleness checks.
def private running_binary() : string {
    // aliases a process-global Array header (locked) — do not delete
    let argv <- get_command_line_arguments()
    return empty(argv) ? "" : replace("{argv[0]}", "\\", "/")
}

// manual override for harnesses that manage their own sidecar location
var private g_runtime_manifest = ""

//! Point the get/set APIs (and nothing else — compile-time stamps read argv/env only) at an
//! explicit sidecar; harnesses that manage their own location call it before get/set.
def set_tune_manifest_runtime_path(p : string) {
    g_runtime_manifest = p
}

//! The tune sidecar location: `DAS_TUNE_MANIFEST` env override, else a harness override
//! (`set_tune_manifest_runtime_path`), else `<app>.tune.json` beside the app — the root
//! script this process runs, or the binary itself (standalone exe, embedded host) when
//! there is none. Never "" — every app has a sidecar location.
def tune_manifest_path() : string {
    let env = get_env_variable("DAS_TUNE_MANIFEST")
    if (!empty(env)) return env
    if (!empty(g_runtime_manifest)) return g_runtime_manifest
    let app = tune_app_script()
    return sidecar_for(empty(app) ? running_binary() : app)
}

//! True when the sidecar predates the binary that runs (or builds) the app — a rebuilt
//! binary invalidates every measured winner, so a stale sidecar reads as ABSENT everywhere
//! (compile-time stamps, the policy rail, the auto guard) and gets regenerated. This kills
//! the copy-a-stale-file trap: measurements never outlive the binary that made them.
def tune_sidecar_stale(path : string) : bool {
    var sfs : FStat
    return false if (!stat(path, sfs))   // absent is "missing", not "stale"
    var bfs : FStat
    return false if (!stat(running_binary(), bfs))
    return sfs.mtime < bfs.mtime
}

// the sidecar's "kernels" section as a flat { function name : perm suffix } map;
// found = the file existed, parsed to an object, and is NOT stale vs the running binary
// (a "kernels"-less but fresh sidecar counts as found-empty: runtime-knob-only files)
def private read_manifest(path : string; var found : bool&) : table<string; string> {
    found = false
    var tab : table<string; string>
    if (empty(path) || tune_sidecar_stale(path)) {
        return <- tab
    }
    let text = fread(path)
    if (empty(text)) {
        return <- tab
    }
    var err = ""
    var doc = read_json(text, err)
    if (doc == null || !(doc is _object)) {
        delete_json(doc)
        return <- tab
    }
    found = true
    let kv = doc?["kernels"]
    if (kv != null && kv is _object) {
        for (k, v in keys(kv as _object), values(kv as _object)) {
            if (v != null && v is _string) {
                tab[k] = v as _string   // das strings survive the node delete below (string heap)
            }
        }
    }
    delete_json(doc)
    return <- tab
}

//! The winning perm suffix recorded for `fname`, or "" when the manifest is absent or has no
//! entry — the read-your-own-tune-value API.
def tune_manifest_get(fname : string) : string {
    var found = false
    var tab <- read_manifest(tune_manifest_path(), found)
    let r = tab?[fname] ?? ""
    delete tab
    return r
}

// the sidecar document for read-modify-write: parsed when fresh, a new empty object when
// absent, unparsable, or STALE (stale measurements are invalid; the first save makes the
// file newer than the binary so the rest of the run appends)
def private load_sidecar_doc(path : string) : JsonValue? {
    var doc : JsonValue?
    if (!tune_sidecar_stale(path)) {
        let text = fread(path)
        if (!empty(text)) {
            var err = ""
            doc = read_json(text, err)
        }
    }
    if (doc == null || !(doc.value is _object)) {
        var fresh : table<string; JsonValue?>
        doc = JV(fresh)
    }
    return doc
}

// refresh "provenance" and write the document back
def private save_sidecar_doc(path : string; var doc : JsonValue?) : bool {
    update(doc, "provenance", JV((binary = running_binary(), platform = get_platform_name(),
        arch = get_architecture_name())))
    return fwrite(path, write_json(doc))
}

// upsert one "kernels" entry into an already-loaded document
def private doc_kernels_set(var doc : JsonValue?; fname : string; suffix : string) {
    var kernels = doc?["kernels"]
    if (kernels == null || !(kernels.value is _object)) {
        update(doc, "kernels", JV({ fname => JV(suffix) }))   // frees a non-object leftover
        return
    }
    // kern aliases the pointee behind the kernels JsonValue? — stable across doc rehashes
    var kern & = unsafe(kernels.value as _object)
    if (kern |> key_exists(fname)) {
        delete_json(kern[fname])
    }
    kern[fname] = JV(suffix)
}

//! Upsert `fname -> suffix` into the sidecar's `"kernels"` section and save it — the
//! set-a-value API the tune harness calls after picking a winner. Every other section
//! (`"runtime"`, keys of other scopes' tuners) is preserved — upsert semantics are what let
//! several libraries' tuners share the one per-app file. A STALE sidecar (older than the
//! running binary) is reset to a fresh document first. Refreshes `"provenance"` on every
//! write. Returns false when the file can't be written.
def tune_manifest_set(fname : string; suffix : string) : bool {
    let path = tune_manifest_path()
    if (empty(path)) {
        return false
    }
    var doc = load_sidecar_doc(path)
    doc_kernels_set(doc, fname, suffix)
    let ok = save_sidecar_doc(path, doc)
    delete_json(doc)
    return ok
}

//! The bulk form for library tuners that record many kernels plus their runtime knobs in
//! one write: upsert every `kernels` entry, and (when `runtime` is non-null) REPLACE the
//! sidecar's `"runtime"` section with it — the tuner owns that section outright, and the
//! document ADOPTS the passed tree (freed with it — don't reuse it after). Same
//! preservation and stale-reset semantics as `tune_manifest_set`.
def tune_sidecar_merge(kernels : table<string; string>; var runtime : JsonValue?) : bool {
    let path = tune_manifest_path()
    if (empty(path)) {
        return false
    }
    var doc = load_sidecar_doc(path)
    for (fname, suffix in keys(kernels), values(kernels)) {
        doc_kernels_set(doc, fname, suffix)
    }
    if (runtime != null) {
        update(doc, "runtime", runtime)   // frees the previous section; doc adopts the new one
    }
    let ok = save_sidecar_doc(path, doc)
    delete_json(doc)
    return ok
}

// ===== tune status (runtime) =====

//! One tuned function's final compile-time decision, as reported by `tune_status()`.
struct TuneStatus {
    fname : string
    scope : string          // [tune_scope] name, "" when env/app-manifest driven
    suffix : string         // winning perm suffix, "" for the reference body
    source : string         // "manifest" | "fallback" | "reference"
    manifestPath : string   // the manifest the winner came from (or would come from)
}

var private g_tune_status : array<TuneStatus>

//! Hook the `[tune_policy]`-injected [init] calls once per tuned function.
def tune_status_add(fname, scope, suffix, source, manifestPath : string) {
    g_tune_status |> push(TuneStatus(fname = fname, scope = scope, suffix = suffix,
        source = source, manifestPath = manifestPath))
}

//! The program's tune decisions — one row per `[tune]` function. Populated whenever the
//! policy rail ran (a declared `[tune_policy]` or the auto default on any app with tune
//! scopes); empty under the gates (AOT, `-exe`, grid modes, `DAS_TUNE_MANIFEST`).
def tune_status() : array<TuneStatus> {
    return clone_to_move(g_tune_status)
}

//! Log the tune status table at `LOG_INFO` — the shared "am I tuned?" surface for any app.
//! `appName` prefixes the lines. No-op when the table is empty (no `[tune_policy]`, or a
//! standalone build). Prints `<n>/<total> kernels tuned for this box`, one line per function,
//! and a `--tune` hint when any kernel is on a fallback/reference tier.
def log_tune_status(appName : string) {
    var st <- tune_status()
    if (empty(st)) {
        delete st
        return
    }
    var tuned = 0
    for (s in st) {
        tuned += s.source == "manifest" ? 1 : 0
    }
    to_log(LOG_INFO, "{appName}: tune status ({tuned}/{length(st)} kernels tuned for this box):\n")
    for (s in st) {
        to_log(LOG_INFO, "  {s.fname}: {empty(s.suffix) ? "reference" : s.suffix} ({s.source})\n")
    }
    if (tuned < length(st)) {
        to_log(LOG_INFO, "{appName}: run with --tune to tune the remaining kernels for this box\n")
    }
    delete st
}

// ===== [tune_scope] data — library-owned tuner wiring =====

// one registered scope: the harness that regenerates this library's kernels, plus the
// sidecar it writes into (the per-app file, resolved at scope-apply time)
struct private TuneScope {
    scopeName : string
    modName : string     // the declaring module — anchors the demanded-kernel AST walk
    covers : string      // ';'-joined extra modules whose [tune]/[tuned] kernels belong to this scope
    manifestPath : string
    tunerPath : string   // resolved against the declaring file; "" = no tuner declared
}

// declaring module name -> scope. [tune_policy] drives tuner runs off this bank; the [tune]
// read path itself is scope-independent (every module reads the app sidecar).
var private g_scopes : table<string; TuneScope>

// the kernels `mod` demands sidecar entries for, read OFF THE AST: every [tune] function
// plus every non-perm-pinned [tuned] function (the loop-hint kernels; a perm= pin is a
// profiler variant stub, not a tunable). AST-derived on purpose — shared macro-module
// globals are per-requiring-context copies, so a bank would read empty across modules.
def private module_kernel_fnames(mod : Module?; var fnames : array<string>) {
    for_each_function(mod, "") $(fn) {
        var demand = false
        for (ann in fn.annotations) {
            if (ann.annotation.name == "tune") {
                demand = true
            } elif (ann.annotation.name == "tuned") {
                let pv = find_arg(ann.arguments, "perm")
                demand = !(pv is tString)
            }
        }
        if (demand) {
            fnames |> push("{fn.name}")
        }
    }
}

// the ';'-joined demanded-kernel list of a scope: its declaring module plus every covers=
// module present in the compiling program — the literal the policy guard injection carries
// into runtime, where the AST no longer exists
def private scope_fnames_joined(scope : TuneScope) : string {
    var wanted : table<string>
    wanted |> insert(scope.modName)
    for (m in split(scope.covers, ";")) {
        if (!empty(m)) {
            wanted |> insert(m)
        }
    }
    var fnames : array<string>
    program_for_each_module(compiling_program()) $(mod) {
        if (wanted |> key_exists("{mod.name}")) {
            module_kernel_fnames(mod, fnames)
        }
    }
    let joined = join(fnames, ";")
    delete wanted
    delete fnames
    return joined
}

//! One `[tune_scope]`'s identity + completeness, as reported by `tune_scopes_status()` —
//! the macro-time surface build orchestrators (the `-exe` release-deps JSON, daspkg) read
//! to decide whether to run tuners before a rebuild.
struct TuneScopeStatus {
    name : string       //!< [tune_scope] name
    manifest : string   //!< the app sidecar the winners live in
    tuner : string      //!< tuner harness path ("" = none declared)
    complete : bool     //!< every [tune] kernel of this scope has a fresh sidecar entry
    fallbacks : string  //!< ';'-joined fname=suffix stamps selected by this compile/host
}

//! Every declared scope in `prog` with its per-key completeness against the app sidecar,
//! read OFF THE AST (the `[tune_scope]` structure annotations + each module's `[tune]`
//! functions) — NOT off this module's macro globals: a shared module's globals are
//! per-requiring-context copies, so a caller in another macro module (llvm_exe writing the
//! release-deps JSON) would read an empty bank. Deduped by scope name.
def tune_scopes_status(prog : Program?) : array<TuneScopeStatus> {
    // pass 1: the declared scopes (name, tuner, covers) off the [tune_scope] annotations
    var found : array<TuneScope>
    var seen : table<string>
    prog |> for_each_module() $(mod) {
        mod |> for_each_structure() $(st) {
            for (ann in st.annotations) {
                if (ann.annotation.name == "tune_scope") {
                    var sc = TuneScope(modName = "{mod.name}")
                    let nv = find_arg(ann.arguments, "name")
                    if (nv is tString) { sc.scopeName = nv as tString }
                    let tv = find_arg(ann.arguments, "tuner")
                    if (tv is tString) {
                        sc.tunerPath = resolve_decl_path("{st.at.fileInfo.name}", tv as tString)
                    }
                    let cv = find_arg(ann.arguments, "covers")
                    if (cv is tString) { sc.covers = cv as tString }
                    if (!empty(sc.scopeName) && !(seen |> key_exists(sc.scopeName))) {
                        seen |> insert(sc.scopeName)
                        found |> push(sc)
                    }
                }
            }
        }
    }
    // pass 2: each scope's demanded kernels across its declaring + covers= modules
    var rows : array<TuneScopeStatus>
    rows |> reserve(length(found))
    let mpath = tune_manifest_path()
    var compileStatus <- collect_status(prog)
    for (sc in found) {
        var wanted : table<string>
        wanted |> insert(sc.modName)
        for (m in split(sc.covers, ";")) {
            if (!empty(m)) {
                wanted |> insert(m)
            }
        }
        var fnames : array<string>
        prog |> for_each_module() $(mod) {
            if (wanted |> key_exists("{mod.name}")) {
                module_kernel_fnames(mod, fnames)
            }
        }
        var fallbacks : array<string>
        for (fname in fnames) {
            for (st in compileStatus) {
                if (st.fname == fname && st.source != "manifest") {
                    fallbacks |> push("{fname}={st.suffix}")
                    break
                }
            }
        }
        rows |> push(TuneScopeStatus(name = sc.scopeName, manifest = mpath,
            tuner = sc.tunerPath,
            complete = tune_sidecar_entries_complete(mpath, join(fnames, ";")),
            fallbacks = join(fallbacks, ";")))
        delete wanted
        delete fnames
        delete fallbacks
    }
    delete compileStatus
    delete found
    delete seen
    return <- rows
}

//! The demanded kernels in the ';'-joined `fnames` that LACK a sidecar entry — "" when
//! complete. The loud half of the completeness check: a tuner that doesn't record a kernel
//! it should re-tunes every start, and this names the drift so it's fixed in one glance.
def tune_sidecar_missing_entries(path : string; fnames : string) : string {
    var found = false
    var manifest <- read_manifest(path, found)
    var missing <- [for (fname in split(fnames, ";"));
        fname;
        where !empty(fname) && !(found && manifest |> key_exists(fname))]
    delete manifest
    let r = join(missing, ";")
    delete missing
    return r
}

//! Every kernel in the ';'-joined `fnames` list has a `"kernels"` entry in the sidecar at
//! `path` (companions ride their main entry; an explicit "reference" entry counts). A
//! missing, stale, or unparsable sidecar has no entries. An empty list is trivially
//! complete. This is the "is this scope tuned" predicate — with one shared per-app sidecar,
//! file existence proves nothing (another library's tuner may have written it first).
def tune_sidecar_entries_complete(path : string; fnames : string) : bool {
    var found = false
    var manifest <- read_manifest(path, found)
    var complete = found || empty(fnames)
    if (found) {
        for (fname in split(fnames, ";")) {
            if (!empty(fname)) {
                complete &&= manifest |> key_exists(fname)
            }
        }
    }
    delete manifest
    return complete
}

// ===== the perm grid =====

struct private TunePerm {
    suffix : string
    requires : string   // fallback-eligibility hint: "a,b" = AND, "a|b" = OR per element; "" = everywhere
    args : array<tuple<argname : string; argvalue : RttiValue>>
}

// fallback tier truth: host cpuid OR'd with the DAS_JIT_X64_FORCE_FEATURES emission-only
// override — same names and semantics as llvm_jit_common's x64_target_feature (keep in
// lockstep; that module lives outside this macro context). requires= is an ELIGIBILITY
// hint for fallback selection only — the generator's own decline predicate remains the
// emission truth (a stamped-but-declining row still lands on the reference body).
// arm64 feature names (e.g. "i8mm") ride DAS_JIT_ARM64_FORCE_FEATURES: cpu_supports is
// x64-cpuid-only (fail-closed on arm), and this is macro context — no LLVM host-features
// query here — so on aarch64 the env IS the eligibility truth (the same env the emission
// rail and the DLL cache key read; a native-i8mm box without it still runs smmla via the
// manifest, which bypasses requires=).
def private fallback_feature_ok(name : string) : bool {
    if (cpu_supports(name)) return true
    for (part in split(get_env_variable("DAS_JIT_X64_FORCE_FEATURES"), ",")) {
        if (part == name) return true
    }
    for (part in split(get_env_variable("DAS_JIT_ARM64_FORCE_FEATURES"), ",")) {
        if (part == name) return true
    }
    return false
}

def private perm_requires_ok(p : TunePerm) : bool {
    if (empty(p.requires)) return true
    for (conj in split(p.requires, ",")) {
        continue if (empty(conj))
        var anyOk = false
        for (alt in split(conj, "|")) {
            anyOk ||= !empty(alt) && fallback_feature_ok(alt)
        }
        if (!anyOk) return false
    }
    return true
}

def private suffix_piece(name : string; v : RttiValue) : string {
    if (v is tBool) return (v as tBool) ? name : "no{name}"
    if (v is tInt) return "{name}{v as tInt}"
    if (v is tString) return "{name}_{v as tString}"
    if (v is tFloat) return replace("{name}{v as tFloat}", ".", "d")
    return name
}

// Annotation applies run in declaration order, and a function's annotations only attach to
// the Function AFTER every apply has run — so [tune] cannot read its [tune_perm] siblings off
// func.annotations. Instead each [tune_perm] apply banks its row here (all tune annotations
// share llvm_tune's macro context), and the trailing [tune] apply consumes them.
var private g_perm_rows : table<string; array<TunePerm>>

def private perm_key(func : FunctionPtr) : string => "{compiling_module().name}::{func.name}"

// one TunePerm from one [tune_perm(...)] declaration's args; suffix= overrides the
// auto-derived name
def private build_perm_row(args : AnnotationArgumentList) : TunePerm {
    var row : TunePerm
    for (aa in args) {
        let v = get_annotation_argument_value(aa)
        if (aa.name == "suffix") {
            if (v is tString) { row.suffix = v as tString }
            continue
        }
        if (aa.name == "requires") {
            // eligibility hint, consumed here — must not leak into the stamp args or the
            // auto-derived suffix
            if (v is tString) { row.requires = v as tString }
            continue
        }
        row.args |> emplace((argname = string(aa.name), argvalue = v))
    }
    if (empty(row.suffix)) {
        var pieces <- [for (a in row.args); suffix_piece(a.argname, a.argvalue)]
        row.suffix = join(pieces, "_")
        delete pieces
    }
    return <- row
}

// one companion row: a sibling function stamped with the same perm as the tuned function
struct private TuneCompanion {
    fnname : string
    genkey : string
    @do_not_delete fn : FunctionPtr   // gc_node-owned by the module, not this row
}

var private g_companion_rows : table<string; array<TuneCompanion>>

// pull the banked companion rows for `func` and resolve each sibling function
def private take_companions(func : FunctionPtr; var errors : das_string) : array<TuneCompanion> {
    var comps : array<TuneCompanion>
    let key = perm_key(func)
    if (g_companion_rows |> key_exists(key)) {
        comps <- g_companion_rows[key]
        g_companion_rows |> erase(key)
    }
    var seen : table<string>
    for (c in comps) {
        if (seen |> key_exists(c.fnname)) {
            errors := "[tune] on {func.name}: duplicate companion '{c.fnname}'"
        } elif (c.fnname == func.name) {
            errors := "[tune] on {func.name}: a companion can't be the tuned function itself"
        } else {
            c.fn = compiling_module() |> find_unique_function(c.fnname, true)
            if (c.fn == null) {
                errors := "[tune] on {func.name}: companion function '{c.fnname}' not found — declare exactly one, before the tuned function, in the same module"
            } elif (c.fn.result.baseType == Type.autoinfer) {
                errors := "[tune] on {func.name}: companion {c.fnname} needs an explicit return type"
            } else {
                // even when this compile leaves the companion unstamped (no fallback, no env
                // manifest), a root-module [tune_manifest] may re-stamp it later — a pure
                // zero-arg companion const-folded at ITS module's compile would leave stale
                // reference values at call sites while the main function moves to the
                // manifest perm. Force the side effect up front, not just at stamp time.
                c.fn.sideEffectFlags.userScenario = true
            }
        }
        if (!empty(errors)) {
            break
        }
        seen |> insert(c.fnname)
    }
    delete seen
    return <- comps
}

// pull the banked rows for `func` (moving them out of the registry) and validate the set
def private take_perms(func : FunctionPtr; var errors : das_string) : array<TunePerm> {
    var perms : array<TunePerm>
    let key = perm_key(func)
    if (g_perm_rows |> key_exists(key)) {
        perms <- g_perm_rows[key]
        g_perm_rows |> erase(key)
    }
    var seen : table<string>
    for (p in perms) {
        if (empty(p.suffix)) {
            errors := "[tune] on {func.name}: a [tune_perm] with no args needs an explicit suffix=\"...\""
        } elif (p.suffix == "reference") {
            errors := "[tune] on {func.name}: perm suffix 'reference' is reserved for the unstamped body"
        } elif (seen |> key_exists(p.suffix)) {
            errors := "[tune] on {func.name}: duplicate perm suffix '{p.suffix}'"
        }
        if (!empty(errors)) {
            break
        }
        seen |> insert(p.suffix)
    }
    delete seen
    return <- perms
}

// ===== stamping =====

// [llvm_code(name=<generator>, <perm args>)] onto `fn`. The appended declaration's own apply
// does not run for macro-added annotations, so force the side effects here too — without
// userScenario the front-end const-folds constant-arg calls before they reach the JIT.
// tune_suffix/tune_from ride along as extra args (generators ignore unknown args by
// contract): they are the stamped-truth record `collect_status` reads back off the AST —
// `from` is the manifest the winner came from, "" for a fallback= stamp.
def private stamp_llvm_code(var fn : FunctionPtr; genKey : string; perm : TunePerm; from : string) {
    var aargs : array<tuple<argname : string; argvalue : RttiValue>>
    aargs |> reserve(length(perm.args) + 3)
    aargs |> push((argname = "name", argvalue = RttiValue(tString = genKey)))
    for (a in perm.args) {
        aargs |> push((argname = a.argname, argvalue = a.argvalue))
    }
    aargs |> push((argname = "tune_suffix", argvalue = RttiValue(tString = perm.suffix)))
    aargs |> push((argname = "tune_from", argvalue = RttiValue(tString = from)))
    fn |> append_annotation("llvm_code", "llvm_code", aargs)
    fn.sideEffectFlags.userScenario = true
    delete aargs
}

// tune/test modes: one `<base>__<suffix>` clone per perm, each carrying its [llvm_code] stamp,
// plus the `<base>_variants()` registry — and the same treatment for every companion (its
// clones carry the companion's generator key with the same perm args/suffixes). The base
// functions keep their reference bodies — they are the oracle rows. (Clones are
// annotation-free: the parsed declarations attach to the base only after every apply has run,
// which is after the clones are made.)
def private stamp_grid(var func : FunctionPtr; genKey : string; perms : array<TunePerm>; comps : array<TuneCompanion>; var errors : das_string) : bool {
    var mod = compiling_module()
    for (p in perms) {
        var fclone = clone_function(func)
        fclone.name := "{func.name}__{p.suffix}"
        stamp_llvm_code(fclone, genKey, p, "")
        fclone.flags.generated = true
        if (!(mod |> add_function(fclone))) {
            errors := "[tune] on {func.name}: failed to add variant {fclone.name}"
            return false
        }
    }
    if (!emit_registry(mod, func, perms, errors)) {
        return false
    }
    for (c in comps) {
        for (p in perms) {
            var cclone = clone_function(c.fn)
            cclone.name := "{c.fnname}__{p.suffix}"
            stamp_llvm_code(cclone, c.genkey, p, "")
            cclone.flags.generated = true
            if (!(mod |> add_function(cclone))) {
                errors := "[tune] on {func.name}: failed to add companion variant {cclone.name}"
                return false
            }
        }
        if (!emit_registry(mod, c.fn, perms, errors)) {
            return false
        }
    }
    return true
}

// `<base>_variants() : array<tuple<string; <sig>>>` — ("reference", @@base) first, then one row
// per perm. `<sig>` is the base function's own type, so the registry is generic across kernels.
def private emit_registry(var mod : Module?; base : FunctionPtr; perms : array<TunePerm>; var errors : das_string) : bool {
    let sig = function_to_type(base)
    var body : array<ExpressionPtr>
    body |> reserve(length(perms) + 3)
    body |> push <| qmacro_expr() {
        var result : array<tuple<string; $t(sig)>>
    }
    var baseAddr = new ExprAddr(at = base.at, target := "{base.name}")
    body |> push <| qmacro_expr() {
        result |> push(("reference", $e(baseAddr)))
    }
    for (p in perms) {
        var fnAddr = new ExprAddr(at = base.at, target := "{base.name}__{p.suffix}")
        body |> push <| qmacro_expr() {
            result |> push(($v(p.suffix), $e(fnAddr)))
        }
    }
    body |> push <| qmacro_expr() {
        return <- result
    }
    var fn = qmacro_function("{base.name}_variants") $() : array<tuple<string; $t(sig)>> {
        $b(body)
    }
    fn.flags.generated = true
    if (!(mod |> add_function(fn))) {
        errors := "[tune] on {base.name}: failed to add {base.name}_variants"
        return false
    }
    return true
}

// ===== the annotations =====

//! `[tune_perm(<generator args>)]` — one grid row for the `tune(...)` annotation that must
//! follow it in the same bracket. The args are passed to the generator verbatim (they become
//! `[llvm_code]` annotation args, and fold into the JIT DLL cache key); `suffix="..."`
//! overrides the auto-derived variant name.
[function_macro(name = "tune_perm")]
class private TunePermAnnotation : AstFunctionAnnotation {
    def override apply(var func : FunctionPtr; var group : ModuleGroup;
                       args : AnnotationArgumentList; var errors : das_string) : bool {
        g_perm_rows[perm_key(func)] |> emplace(build_perm_row(args))
        return true
    }
}

//! `[tune_companion(fn = "sibling", gen = "generator_key")]` — stamps `sibling` with the SAME
//! perm as the tuned function, from the same manifest entry: the two-function stamp (e.g. a
//! GEMM kernel and its repack-layout query can never desync — including on JIT-time declines,
//! when both generators fall back to their reference bodies together). List companion rows
//! AFTER the [tune_perm] grid and BEFORE tune(...); the sibling is a plain function (no [tune]
//! of its own) declared earlier in the same module, with an explicit return type.
[function_macro(name = "tune_companion")]
class private TuneCompanionAnnotation : AstFunctionAnnotation {
    def override apply(var func : FunctionPtr; var group : ModuleGroup;
                       args : AnnotationArgumentList; var errors : das_string) : bool {
        // the perm bank doubles as the order guard: rows placed after tune(...) (bank already
        // consumed) or before any [tune_perm] would be silently ignored otherwise
        if (!(g_perm_rows |> key_exists(perm_key(func)))) {
            errors := "[tune_companion] on {func.name}: list it AFTER the [tune_perm] rows and BEFORE tune(...) in the same bracket"
            return false
        }
        var row : TuneCompanion
        let fv = find_arg(args, "fn")
        if (fv is tString) { row.fnname = fv as tString }
        let gv = find_arg(args, "gen")
        if (gv is tString) { row.genkey = gv as tString }
        if (empty(row.fnname) || empty(row.genkey)) {
            errors := "[tune_companion] on {func.name}: fn=\"<function>\" and gen=\"<generator key>\" are both required"
            return false
        }
        g_companion_rows[perm_key(func)] |> emplace(row)
        return true
    }
}

//! `[tune(gen="key", fallback="suffix")]` — LAST in a bracket of `[tune_perm]` rows (applies
//! run in declaration order; `tune` consumes the rows banked before it). Normal runs stamp
//! exactly one winner (manifest > fallback > reference); tune/test runs stamp the whole grid
//! as `<name>__<suffix>` stubs plus a `<name>_variants()` registry. See the module docstring
//! for the mode contract.
[function_macro(name = "tune")]
class private TuneAnnotation : AstFunctionAnnotation {
    def override apply(var func : FunctionPtr; var group : ModuleGroup;
                       args : AnnotationArgumentList; var errors : das_string) : bool {
        let gv = find_arg(args, "gen")
        if (!(gv is tString) || empty(gv as tString)) {
            errors := "[tune] on {func.name}: gen=\"<[llvm_code] generator key>\" is required"
            return false
        }
        let genKey = gv as tString
        var perms <- take_perms(func, errors)
        if (empty(errors) && empty(perms)) {
            errors := "[tune] on {func.name}: no [tune_perm(...)] rows — list them BEFORE tune(...) in the same annotation bracket"
        }
        var comps <- take_companions(func, errors)
        if (!empty(errors)) {
            delete perms
            delete comps
            return false
        }
        if (func.result.baseType == Type.autoinfer) {
            // function_to_type runs pre-infer: an implicit result type reaches the
            // variants registry as `auto` and its rows fail to unify
            errors := "[tune] on {func.name}: an explicit return type is required (add ': void' or the real type)"
            delete perms
            delete comps
            return false
        }
        // AOT is tune-free by contract — reference bodies only, registries emitted so harness
        // code compiles. Overrides grid modes too: a stale DAS_TUNE_MODE env must not leak
        // variant stubs into generated C++. (-exe is NOT gated here: an exe ships the box's
        // manifest winners — local-use build — or the generic fallback= stamps without one.)
        if (tune_stamp_free_gate()) {
            let noPerms : array<TunePerm>
            var gok = emit_registry(compiling_module(), func, noPerms, errors)
            for (c in comps) {
                if (gok) {
                    gok = emit_registry(compiling_module(), c.fn, noPerms, errors)
                }
            }
            delete perms
            delete comps
            return gok
        }
        let mode = tune_mode()
        if (mode == "tune" || mode == "test") {
            let ok = stamp_grid(func, genKey, perms, comps, errors)
            delete perms
            delete comps
            return ok
        }
        if (mode != "normal") {
            errors := "[tune] on {func.name}: unknown DAS_TUNE_MODE '{mode}' (normal | tune | test)"
            delete perms
            delete comps
            return false
        }
        // normal: sidecar entry > fallback= > reference. The sidecar is the per-app file
        // (env override folded in — see tune_manifest_path); a fresh sidecar with no entry
        // for this function falls back to the declared fallback= (a sidecar written before
        // a kernel family landed must not silently drop that family to the reference tier);
        // an explicit "reference" entry forces the original body; a STALE sidecar (older
        // than the binary) reads as absent.
        let fname = "{func.name}"
        var manifestFound = false
        let mpath = tune_manifest_path()
        var manifest <- read_manifest(mpath, manifestFound)
        var suffix = ""
        if (manifestFound) {
            suffix = manifest?[fname] ?? ""
        }
        let fromManifest = !empty(suffix)
        var ok = true
        if (empty(suffix)) {
            // fallback= is a ';'-chain tried in declaration order: the first perm whose
            // requires= hint passes on this box (per-ISA defaults — a manifest-less box
            // still lands on its ISA's known-good row instead of the reference tier).
            // Every listed suffix must exist in the grid; none eligible => reference.
            let fv = find_arg(args, "fallback")
            if (fv is tString) {
                for (cand in split(fv as tString, ";")) {
                    continue if (empty(cand))
                    var found = false
                    for (p in perms) {
                        if (p.suffix == cand) {
                            found = true
                            if (perm_requires_ok(p)) {
                                suffix = cand
                            }
                            break
                        }
                    }
                    if (!found) {
                        errors := "[tune] on {func.name}: fallback perm '{cand}' is not in the [tune_perm] grid"
                        ok = false
                    }
                    break if (!ok || !empty(suffix))
                }
            }
        }
        delete manifest
        if (ok && !empty(suffix) && suffix != "reference") {
            ok = false
            for (p in perms) {
                if (p.suffix == suffix) {
                    stamp_llvm_code(func, genKey, p, fromManifest ? mpath : "")
                    for (c in comps) {
                        stamp_llvm_code(c.fn, c.genkey, p, fromManifest ? mpath : "")
                    }
                    print("llvm_tune: {fname} <- {suffix} ({fromManifest ? mpath : "fallback"})\n")
                    ok = true
                    break
                }
            }
            if (!ok) {
                errors := "[tune] on {func.name}: perm '{suffix}' is not in the [tune_perm] grid"
            }
        }
        // reference-row-only registries (base + companions), so harness code referencing
        // <name>_variants() compiles (and lints) in every mode — the perm stubs themselves
        // exist only in tune/test runs
        if (ok) {
            let noPerms : array<TunePerm>
            ok = emit_registry(compiling_module(), func, noPerms, errors)
            for (c in comps) {
                if (ok) {
                    ok = emit_registry(compiling_module(), c.fn, noPerms, errors)
                }
            }
        }
        delete perms
        delete comps
        return ok
    }
}

// ===== [tune_scope] / [tune_policy] — the tuner-wiring UX rail =====

def private resolve_decl_path(declFile : string; p : string) : string {
    return is_absolute(p) ? p : path_join(dir_name(declFile), p)
}

//! `[tune_scope(name = "mylib", tuner = "harness.das")]` on a (dummy) struct in the library
//! module that owns the `[tune]` functions, declared ABOVE them: names the library's tuner
//! unit. `tuner=` (optional) is the harness that regenerates this library's winners in the
//! app sidecar, resolved against the declaring file; `[tune_policy]` and `--tune` run it
//! (with `DAS_TUNE_MANIFEST` pointed at the sidecar — the harness upserts its own keys and
//! leaves other scopes' keys alone). The winners themselves live in the ONE per-app sidecar
//! (see the module docstring) — there is no per-scope file.
[structure_macro(name = "tune_scope")]
class private TuneScopeAnnotation : AstStructureAnnotation {
    def override apply(var st : StructurePtr; var group : ModuleGroup;
                       args : AnnotationArgumentList; var errors : das_string) : bool {
        if (tune_aot_gate()) return true
        let nv = find_arg(args, "name")
        if (!(nv is tString) || empty(nv as tString)) {
            errors := "[tune_scope]: name=\"<scope name>\" is required"
            return false
        }
        let sname = nv as tString
        var tuner = ""
        let tv = find_arg(args, "tuner")
        if (tv is tString) {
            tuner = resolve_decl_path(string(st.at.fileInfo.name), tv as tString)
        }
        var covers = ""
        let cv = find_arg(args, "covers")
        if (cv is tString) {
            covers = cv as tString
        }
        // overwrite, not error: shared-module macro state persists across compiles in one process
        g_scopes["{compiling_module().name}"] = TuneScope(scopeName = sname,
            modName = "{compiling_module().name}", covers = covers,
            manifestPath = tune_manifest_path(), tunerPath = tuner)
        return true
    }
}

// The tune flags, parsed off the APP's command line — we share an argv we do not own, so this
// is the two-arg parse_args form (clargs skips unknown flag-shaped tokens). Keep every field a
// bool and never read positionals: an app's own `--model foo.gguf` leaves `foo.gguf` in there.
[CommandLineArgs]
struct private TuneCliArgs {
    @clarg_doc = "Re-tune this box's kernels before the app runs"
    tune : bool

    @clarg_name = "tune-quiet"
    @clarg_doc = "Tune with no output at all"
    tune_quiet : bool

    @clarg_name = "tune-verbose"
    @clarg_doc = "Tune with the full per-variant output instead of a progress display"
    tune_verbose : bool
}

// Adopts --tune-quiet/--tune-verbose and answers whether --tune was passed. Runs at MACRO
// time, so the verbosity has to travel by env: the runtime guard is a different context and
// would not see this context's global. get_user_args also gets the standalone-exe slice right.
def private apply_cli_tune_flags() : bool {
    var r <- parse_args(type<TuneCliArgs>, get_user_args())
    if (r |> is_err) {
        return false
    }
    let cfg <- r |> move_unwrap
    if (cfg.tune_quiet) {
        tune_set_verbosity("silent")
    } elif (cfg.tune_verbose) {
        tune_set_verbosity("verbose")
    }
    return cfg.tune
}

// the exact command a user (or the auto path) runs to tune one scope; the env prefix leads
// so the command never starts with a quote (cmd.exe strips a leading quote)
def private tuner_cmd(scope : TuneScope) : string {
    let win = get_platform_name() == "windows"
    // quote the env values: <das_root> (hence the manifest path) can contain spaces — on POSIX
    // an unquoted VAR=/a b/x.json splits the assignment; Windows `set "VAR=..."` is the safe form.
    let envs = (win ?
        "set \"DAS_TUNE_MODE=tune\"&& set \"DAS_TUNE_MANIFEST={scope.manifestPath}\"&& " :
        "DAS_TUNE_MODE=tune DAS_TUNE_MANIFEST=\"{scope.manifestPath}\" ")
    return "{envs}\"{get_das_exe()}\" -jit \"{scope.tunerPath}\" -dasroot \"{get_das_root()}\""
}

// ===== progress events =====

//! Prefix of a tune progress event. Events are lines on stdout because the tuner runs as a
//! chain of child processes — a pipe is the only channel that spans them.
let TUNE_EVENT_PREFIX = "@tune "

struct private TuneProgress {
    active : bool
    scope : string
    kernel : string
    phase : string
    outerDone : int
    outerTotal : int
    innerDone : int
    innerTotal : int
    live : int
    startTicks : int64
}

var private g_prog : TuneProgress
var private g_verbosity = ""

//! Display verbosity from `DAS_TUNE_VERBOSITY`: `silent` (nothing), `normal` (the default —
//! a progress display and the closing summary) or `verbose` (every line the tuner prints, no
//! display). Anything unrecognized reads as `normal`, so a typo never silences the tune.
def tune_verbosity() : string {
    if (empty(g_verbosity)) {
        let v = get_env_variable("DAS_TUNE_VERBOSITY")
        g_verbosity = v == "silent" ? "silent" : (v == "verbose" ? "verbose" : "normal")
    }
    return g_verbosity
}

//! Override the verbosity a `--tune-quiet` / `--tune-verbose` flag selected, before any tuner
//! runs. Exported so the flag rail does not have to go through the environment twice.
def tune_set_verbosity(v : string) {
    g_verbosity = v == "silent" ? "silent" : (v == "verbose" ? "verbose" : "normal")
    set_env_variable("DAS_TUNE_VERBOSITY", g_verbosity)   // the tuner children inherit it
}

// A process draws the display only when its own stdout is a terminal. Otherwise it forwards
// events untouched, so whoever is capturing us (a supervisor, a parent tuner) can draw instead.
def private is_progress_renderer() : bool {
    return is_stdout_terminal()
}

def private emit_event(kind : string; fields : string) {
    print("{TUNE_EVENT_PREFIX}{kind} {fields}\n")
}

//! Announce how many kernels this tuner is about to work through. Call once per tuner half.
def tune_progress_plan(scope : string; total : int) {
    emit_event("plan", "scope={scope} total={total}")
}

//! Enter a kernel: `total` is its own round budget, the inner counter's denominator.
def tune_progress_kernel_begin(name : string; total : int) {
    emit_event("begin", "name={name} total={total}")
}

//! One round of the current kernel. `phase` names the pass (`screening` / `narrowing` /
//! `finalists`) and `live` is how many grid rows are still in contention.
def tune_progress_kernel_step(i : int; phase : string; live : int) {
    emit_event("step", "i={i} phase={phase} live={live}")
}

//! Leave a kernel. `verdict` is `holds`, `beats` or `rejected`.
def tune_progress_kernel_end(name, winner, verdict : string) {
    emit_event("end", "name={name} winner={winner} verdict={verdict}")
}

def private event_field(parts : array<string>; key, dflt : string) : string {
    for (p in parts) {
        let eq = find(p, "=")
        if (eq > 0 && slice(p, 0, eq) == key) {
            return slice(p, eq + 1)
        }
    }
    return dflt
}

def private elapsed_text(sinceTicks : int64) : string {
    let secs = get_time_usec(sinceTicks) / 1000000
    let m = secs / 60
    let s = secs % 60
    if (m <= 0) return "{s}s"
    let pad = s < 10 ? "0" : ""
    return "{m}m{pad}{s}s"
}

//! The display as a plain string: bar, outer counter, kernel, pass + inner counter, live rows,
//! observed elapsed — every part of it measured, none of it predicted. Exposed so another front
//! end (a GUI, a supervisor's status page) can render the state without redoing the layout.
def tune_progress_line(cols : int) : string {
    let head = "{g_prog.outerDone}/{g_prog.outerTotal}"
    var tail = "  {g_prog.kernel}"
    if (g_prog.innerTotal > 0) {
        tail += "  {g_prog.phase} {g_prog.innerDone}/{g_prog.innerTotal}"
    }
    if (g_prog.live > 0) {
        tail += "  {g_prog.live} live"
    }
    tail += "  {elapsed_text(g_prog.startTicks)}"
    // the bar takes whatever is left after the text, clamped to something still readable
    let barw = clamp(cols - length(head) - length(tail) - 6, 8, 28)
    let filled = g_prog.outerTotal > 0 ? barw * g_prog.outerDone / g_prog.outerTotal : 0
    let bar = build_string() $(w) {
        w |> write("[")
        for (i in range(barw)) {
            w |> write(i < filled ? "#" : "-")
        }
        w |> write("] ")
    }
    return "{bar}{head}{tail}"
}

def private render_progress() {
    let cols = terminal_columns()
    let line = tune_progress_line(cols)
    // pad to the full width so a shorter line cannot leave the previous one's tail behind
    let padding = build_string() $(w) {
        for (_i in range(max(0, cols - length(line) - 1))) {
            w |> write(" ")
        }
    }
    print("\r{line}{padding}\r")
}

def private finish_progress() {
    if (g_prog.active) {
        print("\n")
        g_prog.active = false
    }
}

// Fold one event into the display state, drawing it when `render`. State is folded either way
// so a forwarding process still knows a display is live and can mute the raw chatter under it.
def private consume_event(line : string; render : bool) {
    var parts <- split(strip(slice(line, length(TUNE_EVENT_PREFIX))), " ")
    if (empty(parts)) {
        delete parts
        return
    }
    let kind = parts[0]
    if (kind == "plan") {
        g_prog.scope = event_field(parts, "scope", "")
        g_prog.outerTotal = to_int(event_field(parts, "total", "0"))
        g_prog.outerDone = 0
        g_prog.startTicks = ref_time_ticks()
        g_prog.active = true
    } elif (kind == "begin") {
        g_prog.kernel = event_field(parts, "name", "")
        g_prog.innerTotal = to_int(event_field(parts, "total", "0"))
        g_prog.innerDone = 0
        g_prog.phase = ""
        g_prog.live = 0
    } elif (kind == "step") {
        g_prog.innerDone = to_int(event_field(parts, "i", "0"))
        g_prog.phase = event_field(parts, "phase", "")
        g_prog.live = to_int(event_field(parts, "live", "0"))
    } elif (kind == "end") {
        g_prog.outerDone++
        g_prog.innerTotal = 0
        g_prog.live = 0
    }
    delete parts
    if (render && g_prog.active) {
        render_progress()
    }
}

//! Fold one relayed line into the progress state, drawing nothing. True when the line WAS a
//! progress event — i.e. not tuner chatter. This plus `tune_progress_line` is the whole seam a
//! different front end (a GUI, a supervisor's status page) needs.
def tune_progress_feed(line : string) : bool {
    if (!starts_with(line, TUNE_EVENT_PREFIX)) return false
    consume_event(line, false)
    return true
}

//! True while a progress display is live — a `plan` event has arrived and no scope has ended.
def tune_progress_active() : bool {
    return g_prog.active
}

//! Drop the progress state, for a front end starting a fresh scope.
def tune_progress_reset() {
    g_prog = TuneProgress()
}

// Forwarding events is unconditional; verbosity gates only what a human sees. Raw chatter is
// muted only while a display is actually LIVE — a tuner that emits no events must keep printing
// exactly as it always did, or adopting the protocol becomes a prerequisite for being visible.
def private relay_line(line : string) {
    let v = tune_verbosity()
    let renderer = is_progress_renderer()
    if (starts_with(line, TUNE_EVENT_PREFIX)) {
        consume_event(line, renderer && v == "normal")
        if (!renderer) {
            print(line)
        }
        return
    }
    if (v == "silent" || (v == "normal" && g_prog.active)) return
    finish_progress()
    print(line)
}

// The up-front promise. A tune is minutes of work that ends in a relaunch, so say both BEFORE
// the wait — a user (or a supervisor) staring at a silent process cannot infer either.
def private announce_tuning(scopeName : string; force, selfRestarts : bool) {
    if (tune_verbosity() == "silent") return
    let why = force ? "re-tuning (--tune)" : "is untuned on this box"
    let tail = (selfRestarts
        ? "the process restarts itself when it is done"
        : "the process exits when it is done, and must be restarted to pick up the winners")
    print("llvm_tune: scope '{scopeName}' {why} — tuning now. This takes a while; {tail}.\n")
}

// spawn the scope's tuner, relaying its output LIVE — RUNTIME only (the auto and restart
// guards); never from macro context. Streaming is load-bearing: a tune is minutes of work, and
// buffering it to a file left us and any supervisor with no signal until the child exited.
def private run_scope_tuner(scope : TuneScope) : bool {
    let cmd = "{tuner_cmd(scope)} 2>&1"
    if (tune_verbosity() != "silent") {
        print("llvm_tune: tuning scope '{scope.scopeName}' -> {scope.manifestPath}\n")
    }
    if (tune_verbosity() == "verbose") {
        print("llvm_tune: {cmd}\n")
    }
    var rc : int
    unsafe {
        rc = popen(cmd) $(f) {
            if (f != null) {
                while (!feof(f)) {
                    relay_line(fgets(f))
                }
            }
        }
    }
    finish_progress()
    if (rc != 0 && tune_verbosity() != "silent") {
        print("llvm_tune: tuner for scope '{scope.scopeName}' exited with {rc}\n")
    }
    return rc == 0
}

// runtime accumulators for the auto/--tune guard: how many scopes just got tuned, and the
// re-exec'd child's exit code
var private g_tuned_scopes = 0
var private g_reexec_code = 0

//! Runtime half of `[tune_policy(missing = "auto")]` / `--tune` — injected at the top of the
//! app's `main`, one call per scope, followed by `tune_auto_reexec()`. Runs the scope's tuner
//! when any of the scope's kernels (`fnames`, ';'-joined — the compile-time list baked into
//! the guard) lacks a sidecar entry, or the sidecar is stale (unconditionally under
//! `--tune`); tuning happens at RUNTIME because the winners are compile-time stamps — this
//! process compiled without them, and the re-exec that follows picks them up in a fresh
//! compile. A stray empty / half-written / corrupt sidecar reads as untuned — otherwise the
//! guard would no-op while the compile fell back to fallback stamps, never self-healing.
def tune_auto_prepare(scopeName : string; manifestPath : string; tunerPath : string; force : bool; fnames : string) {
    if (!force && tune_sidecar_entries_complete(manifestPath, fnames)) {
        return
    }
    announce_tuning(scopeName, force, true)
    run_scope_tuner(TuneScope(scopeName = scopeName, manifestPath = manifestPath, tunerPath = tunerPath))
    if (tune_sidecar_entries_complete(manifestPath, fnames)) {
        g_tuned_scopes++
    } else {
        print("llvm_tune: tuner left scope '{scopeName}' incomplete in {manifestPath} — missing: {tune_sidecar_missing_entries(manifestPath, fnames)}\n")
    }
}

//! Relaunch the current process so a fresh compile stamps the just-written manifests (the
//! winners are compile-time stamps — they cannot be adopted mid-run). Relays the child's
//! output; `--tune` is stripped from the relaunch, so the child converges (its manifests are
//! present) instead of looping. Returns true when the child ran and main must return
//! immediately — `tune_reexec_code()` is its exit code.
def tune_auto_reexec() : bool {
    if (g_tuned_scopes == 0) {
        return false
    }
    // aliases a process-global Array header — do not delete
    var argv <- get_command_line_arguments()
    var args : array<string>
    args |> reserve(length(argv))
    for (a in argv) {
        if (a != "--tune") {
            args |> push("{a}")
        }
    }
    print("llvm_tune: re-launching to apply the fresh manifests\n")
    var rc : int
    unsafe {
        rc = popen_argv(args, 0.0) $(f) {
            if (f != null) {
                while (!feof(f)) {
                    print(fgets(f))
                }
            }
        }
    }
    delete args
    g_reexec_code = rc
    return true
}

//! The re-exec'd child's exit code — what an `int` main returns after `tune_auto_reexec()`.
def tune_reexec_code() : int {
    return g_reexec_code
}

//! Runtime half of `[tune_policy(missing = "restart")]` — injected as a guard at the top of
//! the app's `main`. When any of the scope's kernels (`fnames`, ';'-joined) lacks a sidecar
//! entry: runs the tuner NOW, prints the restart banner, and returns true so main returns
//! immediately (the winners are compile-time stamps — this process compiled without them).
//! False = scope complete, carry on.
def tune_restart_needed(scopeName : string; manifestPath : string; tunerPath : string; fnames : string) : bool {
    if (tune_sidecar_entries_complete(manifestPath, fnames)) {
        return false
    }
    if (empty(tunerPath)) {
        print("llvm_tune: scope '{scopeName}' is untuned on this box\n")
        print("llvm_tune: no tuner declared — produce {manifestPath} with the library's tuner, then restart\n")
        return true
    }
    announce_tuning(scopeName, false, false)
    run_scope_tuner(TuneScope(scopeName = scopeName, manifestPath = manifestPath, tunerPath = tunerPath))
    if (tune_sidecar_entries_complete(manifestPath, fnames)) {
        print("llvm_tune: scope '{scopeName}' tuned -> {manifestPath}\nllvm_tune: restart to apply the winners\n")
    } else {
        print("llvm_tune: tuner left scope '{scopeName}' incomplete in {manifestPath} — missing: {tune_sidecar_missing_entries(manifestPath, fnames)}\n")
    }
    return true
}

// final stamped truth per [tune] function, read off the AST (the tune_suffix/tune_from args
// stamp_llvm_code records) — no macro-state bank, so shared-context reuse across compiles
// can't pollute it
def private collect_status(prog : Program?) : array<TuneStatus> {
    var rows : array<TuneStatus>
    let sidecar = tune_manifest_path()
    prog |> for_each_module() $(mod) {
        let modName = "{mod.name}"
        var scopeName = ""
        if (g_scopes |> key_exists(modName)) {
            scopeName = g_scopes[modName].scopeName
        }
        for_each_function(mod, "") $(fn) {
            var isTuned = false
            var suffix = ""
            var from = ""
            for (ann in fn.annotations) {
                if (ann.annotation.name == "tune") {
                    isTuned = true
                } elif (ann.annotation.name == "llvm_code") {
                    let sv = find_arg(ann.arguments, "tune_suffix")
                    if (sv is tString) { suffix = sv as tString }
                    let fv = find_arg(ann.arguments, "tune_from")
                    if (fv is tString) { from = fv as tString }
                }
            }
            if (isTuned) {
                let source = empty(suffix) ? "reference" : (empty(from) ? "fallback" : "manifest")
                let mpath = !empty(from) ? from : sidecar
                rows |> push(TuneStatus(fname = "{fn.name}", scope = scopeName,
                    suffix = suffix, source = source, manifestPath = mpath))
            }
        }
    }
    return <- rows
}

// AOT builds run no policy at all; grid-mode children manage their own manifest (and the
// spawned tuner compiles in tune mode — the recursion guard); a DAS_TUNE_MANIFEST env
// override means the user drives manually. -exe is NOT in this gate: a standalone build
// runs no policy either (a frozen artifact must not demand or run tuning), but it still
// emits the status [init] so the artifact can self-report its baked stamps — the two call
// sites below split on tune_exe_gate() explicitly.
def private tune_policy_gated() : bool {
    return (tune_stamp_free_gate() || tune_mode() != "normal"
        || !empty(get_env_variable("DAS_TUNE_MANIFEST")))
}

// the runtime status table: one tune_status_add per [tune] function, off the stamped AST
// truth — emitted by policy runs AND by -exe builds (an artifact must be able to report
// which perms it carries: log_tune_status / tune_status work inside a standalone exe)
def private emit_status_init(var errors : string&) {
    var rows <- collect_status(get_ptr(compiling_program()))
    if (!empty(rows)) {
        var body : array<ExpressionPtr>
        body |> reserve(length(rows))
        for (row in rows) {
            body |> push <| qmacro_expr() {
                tune_status_add($v(row.fname), $v(row.scope), $v(row.suffix),
                    $v(row.source), $v(row.manifestPath))
            }
        }
        var fn = qmacro_function("`tune`status`init") $() : void {
            $b(body)
        }
        fn |> append_annotation("$", "init")
        fn.flags.generated = true
        if (!(compiling_module() |> add_function(fn))) {
            errors := "[tune_policy]: duplicate declaration (one per program)"
        }
    }
    delete rows
}

// the policy core, shared by the declared annotation and the auto default (the infer pass
// below): resolve the policy (env > declared), then per incomplete scope warn / error /
// inject the auto or restart guard into `func` (the app's main)
def private apply_tune_policy(var func : FunctionPtr; declared : string; var errors : string&) : bool {
    var policy = declared
    {
        let penv = get_env_variable("DAS_TUNE_POLICY")
        if (!empty(penv)) {
            policy = penv
        }
        if (policy != "fallback" && policy != "warn" && policy != "error" && policy != "auto" && policy != "restart") {
            errors := "[tune_policy] on {func.name}: missing=\"{policy}\" — use fallback | warn | error | auto | restart"
            return false
        }
        let force = apply_cli_tune_flags()
        if (force && empty(g_scopes)) {
            print("llvm_tune: --tune requested but no [tune_scope] is declared by any required library\n")
        }
        // snapshot: values() locks the table for the loop's duration, and the body reads
        // g_scopes indirectly (the [] op is insert-capable — it trips the lock even on hits)
        var scopes <- [for (scope in values(g_scopes)); scope]
        var seen : table<string>
        var autoScopes : array<TuneScope>
        for (scope in scopes) {
            continue if (seen |> key_exists(scope.scopeName))
            seen |> insert(scope.scopeName)
            // per-scope KEY completeness, not file existence — the sidecar is shared
            continue if (!force && tune_sidecar_entries_complete(scope.manifestPath, scope_fnames_joined(scope)))
            if (force || policy == "auto") {
                // NO tuner run and NO restamp at compile time: the tuner is a daslang child
                // (can't run mid-compile), and restamping a REQUIRED module's compiled
                // function is a cross-module mutation the re-infer pass can't survive.
                // Instead a runtime guard tunes and RE-EXECS — the fresh compile stamps the
                // winners at [tune] time, which is cross-module-safe.
                if (empty(scope.tunerPath)) {
                    errors := "[tune_policy]: scope '{scope.scopeName}' declares no tuner= — cannot {force ? "--tune" : "auto-tune"}; produce {scope.manifestPath} manually"
                } else {
                    autoScopes |> push(scope)
                }
            } elif (policy == "error") {
                errors := "[tune_policy]: scope '{scope.scopeName}' is untuned on this box — run: {empty(scope.tunerPath) ? "<no tuner declared>" : tuner_cmd(scope)} (or start with --tune)"
            } elif (policy == "warn") {
                print("llvm_tune: WARNING scope '{scope.scopeName}' is untuned on this box (fallback stamps in effect)\nllvm_tune: run: {empty(scope.tunerPath) ? "<no tuner declared>" : tuner_cmd(scope)} (or start with --tune)\n")
            } elif (policy == "restart") {
                // prepend a graceful guard to the annotated function (main) — a hard exit()
                // from an [init] would skip environment shutdown (g_envTotal FATAL at exit)
                if (!(func.body is ExprBlock)) {
                    errors := "[tune_policy]: restart — {func.name} has no body block"
                } elif (func.result.baseType != Type.tVoid && func.result.baseType != Type.tInt
                        && func.result.baseType != Type.autoinfer) {
                    // autoinfer = undeclared result, void by convention for main
                    errors := "[tune_policy]: restart — {func.name} must return void or int"
                } else {
                    let fnames = scope_fnames_joined(scope)
                    var guard : ExpressionPtr
                    if (func.result.baseType == Type.tInt) {
                        guard = qmacro_expr() {
                            if (tune_restart_needed($v(scope.scopeName), $v(scope.manifestPath), $v(scope.tunerPath), $v(fnames))) {
                                return 3
                            }
                        }
                    } else {
                        guard = qmacro_expr() {
                            if (tune_restart_needed($v(scope.scopeName), $v(scope.manifestPath), $v(scope.tunerPath), $v(fnames))) {
                                return
                            }
                        }
                    }
                    var blk = unsafe(reinterpret<ExprBlock?>(func.body))
                    blk.list |> push(guard, 0)
                }
            }
            break if (!empty(errors))
        }
        delete seen
        delete scopes
        // the auto/--tune guard: prepend one tune_auto_prepare per scope, then the re-exec —
        // execution order is prepare(s) first, re-exec last, ahead of the original body
        if (empty(errors) && !empty(autoScopes)) {
            if (!(func.body is ExprBlock)) {
                errors := "[tune_policy]: {func.name} has no body block"
            } elif (func.result.baseType != Type.tVoid && func.result.baseType != Type.tInt
                    && func.result.baseType != Type.autoinfer) {
                // autoinfer = undeclared result, void by convention for main
                errors := "[tune_policy]: auto/--tune — {func.name} must return void or int"
            } else {
                var guards : array<ExpressionPtr>
                guards |> reserve(length(autoScopes) + 1)
                for (scope in autoScopes) {
                    let fnames = scope_fnames_joined(scope)
                    guards |> push <| qmacro_expr() {
                        tune_auto_prepare($v(scope.scopeName), $v(scope.manifestPath), $v(scope.tunerPath), $v(force), $v(fnames))
                    }
                }
                if (func.result.baseType == Type.tInt) {
                    guards |> push <| qmacro_expr() {
                        if (tune_auto_reexec()) {
                            return tune_reexec_code()
                        }
                    }
                } else {
                    guards |> push <| qmacro_expr() {
                        if (tune_auto_reexec()) {
                            return
                        }
                    }
                }
                var blk = unsafe(reinterpret<ExprBlock?>(func.body))
                var gi = length(guards) - 1
                while (gi >= 0) {
                    blk.list |> push(guards[gi], 0)
                    gi--
                }
            }
        }
        delete autoScopes
        // with auto now tuning at runtime the rows always reflect this compile's stamps
        if (empty(errors)) {
            emit_status_init(errors)
        }
    }
    return empty(errors)
}

//! `[tune_policy(missing = "...")]` on the app's `main` (must be `main` for auto/restart —
//! they prepend a guard to its body): what to do about scopes whose sidecar entries are
//! missing (or stale) at compile time —
//!   fallback: stamp `fallback=` silently
//!   warn:    loud compile-time banner with the exact tuner command
//!   error:   fail the compile with the same message — the dev mode
//!   auto:    THE DEFAULT (declared or not — see TuneDefaultPolicy below): tune at startup
//!            (injected guard runs each incomplete scope's tuner), then RE-EXEC the process —
//!            the fresh compile stamps the winners; one launch, then it serves. (Tuning
//!            cannot happen mid-compile: the tuner is a daslang child, and adopting its
//!            winners means new compile-time stamps anyway.)
//!   restart: like auto but no re-exec — tune at startup, then exit asking for a manual
//!            restart (one incomplete scope per start)
//! `--tune` after `--` on the app's command line forces the auto path even when the sidecar
//! is complete (re-tune; the flag is stripped from the re-exec). `DAS_TUNE_POLICY` env
//! overrides the declared value (CI kill switch: `DAS_TUNE_POLICY=fallback`). Grid modes
//! (`DAS_TUNE_MODE=tune`/`test` — also the recursion guard for the spawned tuner), a
//! `DAS_TUNE_MANIFEST` env override, and standalone builds (`-exe`, AOT) skip policy entirely.
[function_macro(name = "tune_policy")]
class private TunePolicyAnnotation : AstFunctionAnnotation {
    def override apply(var func : FunctionPtr; var group : ModuleGroup;
                       args : AnnotationArgumentList; var errors : das_string) : bool {
        if (tune_policy_gated()) {
            return true
        }
        var errs = ""
        if (tune_exe_gate()) {
            // standalone build: no policy, but the artifact self-reports its stamps
            emit_status_init(errs)
        } else {
            var policy = "auto"
            let mv = find_arg(args, "missing")
            if (mv is tString) {
                policy = mv as tString
            }
            apply_tune_policy(func, policy, errs)
        }
        if (!empty(errs)) {
            errors := errs
            return false
        }
        return true
    }
}

// inert once-only marker the TuneDefaultPolicy pass stamps onto main — appending an
// annotation RUNS its apply (add_function_annotation), so the marker must not be
// [tune_policy] itself (that would re-enter the policy with whatever env is set)
[function_macro(name = "tune_policy_default_applied")]
class private TunePolicyDefaultMarker : AstFunctionAnnotation {
    def override apply(var func : FunctionPtr; var group : ModuleGroup;
                       args : AnnotationArgumentList; var errors : das_string) : bool {
        return true
    }
}

//! The "untuned does not start" default: when the PROGRAM ROOT (the anonymous main module)
//! has a `main`, any `[tune_scope]` is in play, and no `[tune_policy]` was declared, behave
//! as `[tune_policy(missing = "auto")]` — an incomplete/stale sidecar tunes at startup and
//! re-execs. Escapes: `DAS_TUNE_POLICY=fallback` (the CI kill switch), the standing gates
//! (AOT / -exe / grid modes / `DAS_TUNE_MANIFEST`), and structurally any root without a
//! `main` (dastest-driven test files never tune — dastest runs `[test]` functions, not
//! `main`). Fires as an infer pass on the root module; the appended inert marker
//! annotation makes it once-only and a declared `[tune_policy]` wins.
[infer_macro]
class private TuneDefaultPolicy : AstPassMacro {
    def override apply(prog : ProgramPtr; _mod : Module?) : bool {
        // libraries compile under their module name; the app root module is anonymous.
        // NOTE the pass only fires for roots that SEE llvm_tune — a library owning tune
        // scopes must re-export it (`require llvm/daslib/llvm_tune public`), or its apps
        // silently get no default policy (dasllama_math_gen / dasllama_common do).
        return false if (is_in_completion() || !empty("{compiling_module().name}")
            || empty(g_scopes) || tune_policy_gated())
        var main_fn = compiling_module() |> find_unique_function("main", false)
        return false if (main_fn == null)
        for (ann in main_fn.annotations) {
            // declared policy wins; the marker means this pass already ran
            return false if (ann.annotation.name == "tune_policy"
                || ann.annotation.name == "tune_policy_default_applied")
        }
        main_fn |> append_annotation("llvm_tune", "tune_policy_default_applied")
        var errs = ""
        var ok = true
        if (tune_exe_gate()) {
            // standalone build: no policy, but the artifact self-reports its stamps
            emit_status_init(errs)
        } else {
            ok = apply_tune_policy(main_fn, "auto", errs)
        }
        if (!ok || !empty(errs)) {
            // macro_error reported mid-infer-pass is silently dropped by the retry loop —
            // print + panic instead, so the error flavor actually stops the compile
            print("llvm_tune: {errs}\n")
            panic("llvm_tune default policy: {errs}")
        }
        return true   // modified — re-run infer over the injected status/guard
    }
}
