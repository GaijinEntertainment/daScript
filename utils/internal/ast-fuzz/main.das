options gen2

require daslib/clargs
require daslib/fio
require daslib/strings_boost
require math

let FUZZ_DIR = "utils/internal/ast-fuzz"
let ALL_PATH = "utils/internal/ast-fuzz/_ast_synth_all.das"
var WRAP_PATH = "utils/internal/ast-fuzz/__wrap.das"
var MEMCAP_KB = 4194304
// Distinguishes concurrent sweeps in one tree. Sharing a victim or a worker log means each
// sweep reads the other's file, and every number both report is garbage.
var RUN_TOKEN = 0

[CommandLineArgs]
struct Config {
    @clarg_short = "b"
    @clarg_doc = "Path to the daslang binary under test (default: bin/daslang)"
    bin : string

    @clarg_doc = "Number of seeds to run (default 20)"
    seeds : int

    @clarg_name = "base-seed"
    @clarg_doc = "First seed; the run covers [base-seed, base-seed+seeds)"
    base_seed : int

    @clarg_doc = "Per-compile timeout in seconds (default 20)"
    timeout : float

    @clarg_doc = "Per-compile virtual memory cap in MB (default 4096). Unbounded allocation is a resource problem, not a crash; the capped failure is reported as `resource`"
    memcap : int

    @clarg_doc = "Compile with daslib/ast_verify active, so a malformed-AST report is distinguished from a crash"
    verify : bool

    @clarg_name = "no-opt"
    @clarg_doc = "Compile the victim with `options no_optimization`, to tell an optimizer crash from an inference crash"
    no_opt : bool

    @clarg_doc = "Run the sweep in N parallel workers, splitting the seed range between them (default 1)"
    threads : int

    @clarg_doc = "Worker id: gives this process its own wrapper file (__wrap<N>.das). Set by --threads; only pass it by hand to run one worker of a split sweep"
    worker : int

    @clarg_name = "run-token"
    @clarg_doc = "Distinguishes one sweep's victim and log files from another's in the same tree; set automatically for workers"
    run_token : int

    @clarg_name = "synth-size"
    @clarg_doc = "Statements per synthesized body (default 12)"
    synth_size : int

    @clarg_name = "synth-depth"
    @clarg_doc = "Max expression nesting depth (default 4)"
    synth_depth : int

    @clarg_name = "synth-funcs"
    @clarg_doc = "Whole functions with random signatures and bodies (default 3)"
    synth_funcs : int

    @clarg_name = "synth-decls"
    @clarg_doc = "Structures, enumerations and globals to synthesize, so a named TypeDecl can carry a real payload instead of a null one (default 3)"
    synth_decls : int

    @clarg_name = "synth-bind"
    @clarg_doc = "Bind resolved pointers (ExprVar.variable, ExprCall.func, ExprConstEnumeration.enumType) directly, as a macro does, instead of leaving the name for inference"
    synth_bind : bool

    @clarg_name = "synth-typed"
    @clarg_doc = "Typed mode: synthesize whole functions that are well-typed by construction, so inference runs to completion and the program reaches codegen instead of stopping at the first error"
    synth_typed : bool

    @clarg_name = "synth-pretype"
    @clarg_doc = "Pre-set Expression._type on some nodes. Infer treats a typed node as already inferred, so this is how a wrong type reaches codegen"
    synth_pretype : bool

    @clarg_name = "synth-rflags"
    @clarg_doc = "Randomize per-node flags (constexpression, noSideEffects, ...). Off by default: a wrong flag is a lie the compiler trusts, not a structural break"
    synth_rflags : bool

    @clarg_name = "synth-skip"
    @clarg_doc = "Comma-separated node kinds to exclude from the pool; bisect this to identify which kind a crash needs"
    synth_skip : string

    @clarg_name = "synth-keep"
    @clarg_doc = "Truncate each body to the first N statements AFTER generation, so shrinking leaves the kept statements byte-identical"
    synth_keep : int

    @clarg_name = "synth-only"
    @clarg_doc = "Replace only this one function body, still counting every match so the per-function seeds are unchanged"
    synth_only : string

    @clarg_name = "synth-prune"
    @clarg_doc = "Replace the Nth generated node with a constant; bisect N to find the smallest subtree a crash needs"
    synth_prune : int

    @clarg_name = "synth-dump"
    @clarg_doc = "Print each synthesized body as source"
    synth_dump : bool

    @clarg_name = "synth-report"
    @clarg_doc = "Print a per-node-kind histogram of what was generated"
    synth_report : bool

    @clarg_name = "mutate"
    @clarg_doc = "Mutate the AST of this real .das file instead of synthesizing one. The seed range sweeps the candidate node index, so --seeds N covers the first N nodes"
    mutate : string

    @clarg_name = "mutate-dir"
    @clarg_doc = "Mutate every .das in this directory: each file x each kind, with the candidate-node count measured per pair so the seed range covers exactly the nodes that exist"
    mutate_dir : string

    @clarg_name = "mut-kinds"
    @clarg_doc = "Which kinds --mutate-dir sweeps, comma separated (default 0,1,2,3,4)"
    mut_kinds : string

    @clarg_name = "mut-count"
    @clarg_doc = "Apply N edits at once instead of one, kinds and positions drawn from the seed. Each edit stays source-expressible; the combination is what a single edit cannot reach"
    mut_count : int

    @clarg_name = "mut-stride"
    @clarg_doc = "--mutate-dir takes every Nth file (default 1)"
    mut_stride : int

    @clarg_name = "mut-cap"
    @clarg_doc = "Never sweep more than this many nodes per file/kind (default 300)"
    mut_cap : int

    @clarg_name = "mut-kind"
    @clarg_doc = "Which AST edit to apply: 0 operator, 1 int literal, 2 swap two call arguments, 3 variable name, 4 declared type of a let, 5 let type to auto, 6 let const, 7 let type wrapped in array, 8 copy to move or clone, 9 statement into unsafe/try/scope, 10 statement into a closure, 11 statement duplicated, 12 argument const, 13 default value on the last argument, 14 result type to auto, 15 let type nested, 16 statement nested in closures (default 0)"
    mut_kind : int

    @clarg_name = "mut-depth"
    @clarg_doc = "How many levels the nesting kinds (15, 16) pile on (default 8)"
    mut_depth : int

    @clarg_doc = "Reproduce mode: compile this victim path once and report"
    file : string

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

def read_text(path : string) : string {
    return fread(path)
}

// Mutation victim: the real file, with the macro required in front of it and the node
// index carried in the options. Everything downstream - subprocess isolation, the
// crash/verifier/timeout classification, --threads - is the existing driver.
def build_mutation_victim(cfg : Config; seed : int) : string {
    return build_string() $(var w) {
        w |> write("options _enable_ast_synth = true\n")
        if (cfg.mut_count > 0) {
            w |> write("options _ast_synth_mut_count = {cfg.mut_count}\n")
        } else {
            w |> write("options _ast_synth_mut_at = {seed}\n")
            w |> write("options _ast_synth_mut_kind = {cfg.mut_kind}\n")
        }
        w |> write("options _ast_synth_seed = {seed}\n")
        if (cfg.mut_depth > 0) {
            w |> write("options _ast_synth_mut_depth = {cfg.mut_depth}\n")
        }
        w |> write("require _ast_synth\n")
        w |> write(read_text(cfg.mutate))
    }
}

def build_victim(cfg : Config; seed : int) : string {
    if (!empty(cfg.mutate)) {
        return build_mutation_victim(cfg, seed)
    }
    return build_string() $(var w) {
        w |> write("options gen2\n")
        w |> write("options _enable_ast_synth = true\n")
        w |> write("options _ast_synth_all = true\n")
        w |> write("options _ast_synth_seed = {seed}\n")
        if (cfg.no_opt) {
            w |> write("options no_optimization = true\n")
        }
        if (cfg.synth_size > 0) {
            w |> write("options _ast_synth_size = {cfg.synth_size}\n")
        }
        if (cfg.synth_depth > 0) {
            w |> write("options _ast_synth_depth = {cfg.synth_depth}\n")
        }
        w |> write("options _ast_synth_funcs = {cfg.synth_funcs > 0 ? cfg.synth_funcs : 1}\n")
        w |> write("options _ast_synth_decls = {cfg.synth_decls > 0 ? cfg.synth_decls : 1}\n")
        if (cfg.synth_bind) {
            w |> write("options _ast_synth_bind = true\n")
        }
        if (cfg.synth_pretype) {
            w |> write("options _ast_synth_pretype = true\n")
        }
        if (cfg.synth_typed) {
            w |> write("options _ast_synth_typed = true\n")
        }
        if (cfg.synth_rflags) {
            w |> write("options _ast_synth_rflags = true\n")
        }
        if (!empty(cfg.synth_skip)) {
            w |> write("options _ast_synth_skip = \"{cfg.synth_skip}\"\n")
        }
        if (cfg.synth_keep > 0) {
            w |> write("options _ast_synth_keep = {cfg.synth_keep}\n")
        }
        if (!empty(cfg.synth_only)) {
            w |> write("options _ast_synth_only = \"{cfg.synth_only}\"\n")
        }
        if (cfg.synth_prune > 0) {
            w |> write("options _ast_synth_prune = {cfg.synth_prune}\n")
        }
        if (cfg.synth_dump) {
            w |> write("options _ast_synth_dump = true\n")
        }
        if (cfg.synth_report) {
            w |> write("options _ast_synth_report = true\n")
        }
        if (cfg.synth_typed) {
            // the typed pool calls math builtins, which have to be visible in the victim
            w |> write("require math\n")
        }
        w |> write("require _ast_synth\n")
        w |> write("\n")
        w |> write("[export]\n")
        w |> write("def main \{ \}\n")
    }
}

def run_wrapper(bin : string; verify : bool; var out : string&; timeout : float) : int {
    var flags = "-compile-only"
    if (verify) {
        flags += " --ast-verify"
    }
    let argv <- ["/bin/sh", "-c", "ulimit -v {MEMCAP_KB}; exec '{bin}' {flags} '{WRAP_PATH}'"]
    return run_and_capture(argv, out, timeout)
}

def is_timeout(exit_code : int) : bool {
    return exit_code == popen_timed_out
}

def is_resource(out : string) : bool {
    return (find(out, "bad_alloc") >= 0 || find(out, "std::unexpected") >= 0
        || find(out, "Cannot allocate memory") >= 0 || find(out, "out of memory") >= 0)
}

def is_crash(exit_code : int; out : string) : bool {
    if (is_timeout(exit_code) || is_resource(out)) {
        return false
    }
    if (exit_code >= 128 || exit_code < 0) {
        return true
    }
    // An assertions-enabled build reports an internal invariant violation and traps; the
    // banner is what identifies it even if the trap itself is swallowed.
    return (find(out, "CRASH:") >= 0 || find(out, "Segmentation fault") >= 0
        || find(out, "assertion failed") >= 0)
}

def ensure_generated(bin : string) : bool {
    if (!empty(read_text(ALL_PATH))) {
        return true
    }
    to_log(LOG_INFO, "ast-fuzz: generating the node builder from the AST headers...\n")
    var out : string
    let ec = run_and_capture([bin, "{FUZZ_DIR}/gen_invariants.das"], out, 120.0)
    if (ec != 0) {
        to_log(LOG_ERROR, "ast-fuzz: gen_invariants failed (exit {ec})\n{out}\n")
        return false
    }
    return true
}

def private child_flags(cfg : Config) : string {
    return build_string() $(var w) {
        if (cfg.synth_size > 0) {
            w |> write(" --synth-size {cfg.synth_size}")
        }
        if (cfg.synth_depth > 0) {
            w |> write(" --synth-depth {cfg.synth_depth}")
        }
        if (cfg.synth_funcs > 0) {
            w |> write(" --synth-funcs {cfg.synth_funcs}")
        }
        if (cfg.synth_decls > 0) {
            w |> write(" --synth-decls {cfg.synth_decls}")
        }
        if (cfg.synth_keep > 0) {
            w |> write(" --synth-keep {cfg.synth_keep}")
        }
        if (cfg.synth_prune > 0) {
            w |> write(" --synth-prune {cfg.synth_prune}")
        }
        if (!empty(cfg.synth_skip)) {
            w |> write(" --synth-skip {cfg.synth_skip}")
        }
        if (!empty(cfg.synth_only)) {
            w |> write(" --synth-only {cfg.synth_only}")
        }
        if (cfg.synth_bind) {
            w |> write(" --synth-bind")
        }
        if (cfg.synth_pretype) {
            w |> write(" --synth-pretype")
        }
        if (cfg.synth_typed) {
            w |> write(" --synth-typed")
        }
        if (!empty(cfg.mutate)) {
            w |> write(" --mutate {cfg.mutate}")
            w |> write(" --mut-kind {cfg.mut_kind}")
            if (cfg.mut_count > 0) {
                w |> write(" --mut-count {cfg.mut_count}")
            }
        }
        if (cfg.synth_rflags) {
            w |> write(" --synth-rflags")
        }
        if (cfg.verify) {
            w |> write(" --verify")
        }
        if (cfg.no_opt) {
            w |> write(" --no-opt")
        }
        if (cfg.memcap > 0) {
            w |> write(" --memcap {cfg.memcap}")
        }
        if (cfg.timeout > 0.0) {
            w |> write(" --timeout {cfg.timeout}")
        }
        w |> write(" --run-token {RUN_TOKEN}")
    }
}

def private run_threaded(cfg : Config; bin : string; threads, base, seeds : int;
                         timeout : float) : int {
    let per = max(seeds / threads, 1)
    let flags = child_flags(cfg)
    let cmd = build_string() $(var w) {
        for (i in range(threads)) {
            w |> write("nice -n 15 '{bin}' {FUZZ_DIR}/main.das -- --bin '{bin}'")
            w |> write(" --worker {i + 1} --base-seed {base + i * per} --seeds {per}{flags}")
            w |> write(" > {FUZZ_DIR}/__worker{RUN_TOKEN}_{i + 1}.log 2>&1 & ")
        }
        w |> write("wait")
    }
    to_log(LOG_INFO, "ast-fuzz: {threads} worker(s) x {per} seed(s) from {base}\n")
    var out : string
    let budget = timeout * float(per) + 120.0
    let ec = run_and_capture(["/bin/sh", "-c", cmd], out, budget)
    if (ec == popen_timed_out) {
        to_log(LOG_ERROR, "ast-fuzz: the batch exceeded {budget}s; raise --timeout or lower --seeds\n")
        return 2
    }
    var crashes = 0
    for (i in range(threads)) {
        let log = read_text("{FUZZ_DIR}/__worker{RUN_TOKEN}_{i + 1}.log")
        for (ln in split(log, "\n")) {
            if (find(ln, "run(s) —") >= 0) {
                to_log(LOG_INFO, "  worker {i + 1}: {strip(slice(ln, find(ln, "run(s) —")))}\n")
            } elif (find(ln, "CRASH seed=") >= 0) {
                to_log(LOG_INFO, "  {slice(ln, find(ln, "CRASH seed="))}\n")
                crashes++
            }
        }
    }
    to_log(LOG_INFO, "ast-fuzz: {crashes} crash(es) across {threads} worker(s); logs in {FUZZ_DIR}/__worker{RUN_TOKEN}_*.log\n")
    return crashes > 0 ? 1 : 0
}

// How many nodes of this kind the file actually has. The macro reports it, so one
// compile answers it - without this the sweep spends most of its seeds on indices that
// do not exist and every run comes back a no-op.
def private count_candidates(cfg : Config; bin : string; file : string; kind : int;
                             timeout : float) : int {
    var probe = cfg
    probe.mutate = file
    probe.mut_kind = kind
    if (!fwrite(WRAP_PATH, build_victim(probe, 0))) {
        return 0
    }
    var out : string
    run_and_capture([bin, "-compile-only", WRAP_PATH], out, timeout)
    let at = find(out, "of ")
    if (at < 0) {
        return 0
    }
    let rest = slice(out, at + 3)
    let sp = find(rest, " ")
    return sp < 0 ? 0 : to_int(slice(rest, 0, sp))
}

def private run_mutation_sweep(cfg : Config; bin : string; timeout : float) : int {
    var kinds : array<int>
    let spec = empty(cfg.mut_kinds) ? "0,1,2,3,4" : cfg.mut_kinds
    for (k in split(spec, ",")) {
        if (!empty(strip(k))) {
            kinds |> push(to_int(strip(k)))
        }
    }
    let cap = cfg.mut_cap > 0 ? cfg.mut_cap : 300
    var files : array<string>
    glob(cfg.mutate_dir, "*.das") $(filename, is_dir) {
        if (!is_dir && !starts_with(base_name(filename), "_")) {
            files |> push(path_join(cfg.mutate_dir, filename))
        }
    }
    files |> sort()
    let stride = max(cfg.mut_stride, 1)
    var picked <- [for (f, fi in files, count()); f; where fi % stride == 0]
    to_log(LOG_INFO, "ast-fuzz: mutating {length(picked)} of {length(files)} file(s) x {length(kinds)} kind(s)\n")
    var findings = 0
    for (f in picked) {
        for (k in kinds) {
            // in --mut-count mode every seed is a different combination of edits, so
            // there is nothing to calibrate: the seed range is just how many to try
            var n = cap
            if (cfg.mut_count <= 0) {
                n = min(count_candidates(cfg, bin, f, k, timeout), cap)
            }
            if (n < 1) {
                continue
            }
            var c = cfg
            c.mutate = f
            c.mut_kind = k
            c.seeds = n
            c.base_seed = 1
            c.mutate_dir = ""
            to_log(LOG_INFO, "ast-fuzz: {base_name(f)} kind={k} nodes={n}\n")
            let th = cfg.threads > 1 && n >= cfg.threads ? cfg.threads : 1
            let rc = run_threaded(c, bin, th, 1, n, timeout)
            if (rc == 1) {
                findings++
            }
        }
    }
    to_log(LOG_INFO, "ast-fuzz: mutation sweep done, {findings} file/kind pair(s) with findings\n")
    return findings > 0 ? 1 : 0
}

[export]
def main : int { // nolint:STYLE037,STYLE038 — one linear sweep with a flat classification tally; splitting hides it
    var r <- parse_args(type<Config>)
    if (r |> is_err) {
        to_log(LOG_ERROR, "error: {r |> unwrap_err}\n")
        print_help(get_command_info(type<Config>), "ast-fuzz")
        return 2
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<Config>), "ast-fuzz")
        return 0
    }
    let bin = empty(cfg.bin) ? "bin/daslang" : cfg.bin
    if (!ensure_generated(bin)) {
        return 2
    }
    if (cfg.memcap > 0) {
        MEMCAP_KB = cfg.memcap * 1024
    }
    RUN_TOKEN = cfg.run_token > 0 ? cfg.run_token : int(int64(get_clock()) % 99991)
    if (cfg.worker > 0) {
        // The run token keeps two sweeps in one tree off each other's victims. Sharing one
        // file means each sweep compiles the other's input and every number is garbage.
        WRAP_PATH = "{FUZZ_DIR}/__wrap{RUN_TOKEN}_{cfg.worker}.das"
    }
    let timeout = cfg.timeout > 0.0 ? cfg.timeout : 20.0

    if (!empty(cfg.file)) {
        WRAP_PATH = cfg.file
        var out : string
        let ec = run_wrapper(bin, cfg.verify, out, timeout)
        let crashed = is_crash(ec, out)
        let caught = cfg.verify && find(out, "AST verify:") >= 0
        to_log(LOG_INFO, "[reproduce] file={cfg.file} exit={ec} crash={crashed} verifier_caught={caught}\n")
        if (!empty(out)) {
            to_log(LOG_ERROR, "{out}\n")
        }
        return crashed && !caught ? 1 : 0
    }

    let seeds = cfg.seeds > 0 ? cfg.seeds : 20
    let base = cfg.base_seed > 0 ? cfg.base_seed : 1
    if (!empty(cfg.mutate_dir)) {
        return run_mutation_sweep(cfg, bin, timeout)
    }
    if (cfg.threads > 1) {
        return run_threaded(cfg, bin, cfg.threads, base, seeds, timeout)
    }
    to_log(LOG_INFO, "ast-fuzz: {seeds} seed(s) from {base}\n")
    var nCrash = 0
    var nClean = 0
    var nError = 0
    var nTimeout = 0
    var nResource = 0
    var nCaught = 0
    var repros : array<string>
    for (i in range(seeds)) {
        let seed = base + i
        if (!fwrite(WRAP_PATH, build_victim(cfg, seed))) {
            to_log(LOG_ERROR, "failed to write {WRAP_PATH}\n")
            return 2
        }
        var out : string
        let ec = run_wrapper(bin, cfg.verify, out, timeout)
        if (is_timeout(ec)) {
            nTimeout++
            // A mutant that outruns the timeout is the compile-time-blowup class, so it
            // needs a repro on disk just as much as a crash does.
            let slow = "{FUZZ_DIR}/__slow.seed{seed}.das"
            fwrite(slow, read_text(WRAP_PATH))
            repros |> push("seed={seed} TIMEOUT repro={slow}")
        } elif (is_resource(out)) {
            nResource++
        } elif (is_crash(ec, out)) {
            if (cfg.verify && find(out, "AST verify:") >= 0) {
                nCaught++
            } else {
                nCrash++
                let repro = "{FUZZ_DIR}/__crash.seed{seed}.das"
                fwrite(repro, read_text(WRAP_PATH))
                repros |> push("seed={seed} exit={ec} repro={repro}")
            }
        } elif (ec == 0) {
            nClean++
        } else {
            nError++
        }
        if (cfg.synth_dump || cfg.synth_report) {
            to_log(LOG_ERROR, "{out}\n")
        }
    }
    to_log(LOG_INFO, "ast-fuzz: {seeds} run(s) — {nClean} clean, {nError} compiler-error, {nCaught} verifier-caught, {nResource} resource, {nTimeout} TIMEOUT, {nCrash} CRASH\n")
    for (rp in repros) {
        to_log(LOG_INFO, "  CRASH {rp}\n")
    }
    if (nCrash > 0) {
        to_log(LOG_INFO, "reproduce with: {bin} -compile-only <repro-path>\n")
    }
    return nCrash > 0 ? 1 : 0
}
