options gen2

require daslib/clargs
require daslib/fio
require daslib/strings_boost
require math

let FUZZ_DIR = "utils/ast-fuzz"
let ALL_PATH = "utils/ast-fuzz/_ast_synth_all.das"
var WRAP_PATH = "utils/ast-fuzz/__wrap.das"
var MEMCAP_KB = 4194304

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

def build_victim(cfg : Config; seed : int) : string {
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
    return find(out, "CRASH:") >= 0 || find(out, "Segmentation fault") >= 0
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
            w |> write(" > {FUZZ_DIR}/__worker{i + 1}.log 2>&1 & ")
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
        let log = read_text("{FUZZ_DIR}/__worker{i + 1}.log")
        for (ln in split(log, "\n")) {
            if (find(ln, "run(s) —") >= 0) {
                to_log(LOG_INFO, "  worker {i + 1}: {strip(slice(ln, find(ln, "run(s) —")))}\n")
            } elif (find(ln, "CRASH seed=") >= 0) {
                to_log(LOG_INFO, "  {slice(ln, find(ln, "CRASH seed="))}\n")
                crashes++
            }
        }
    }
    to_log(LOG_INFO, "ast-fuzz: {crashes} crash(es) across {threads} worker(s); logs in {FUZZ_DIR}/__worker*.log\n")
    return crashes > 0 ? 1 : 0
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
    if (cfg.worker > 0) {
        WRAP_PATH = "{FUZZ_DIR}/__wrap{cfg.worker}.das"
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
