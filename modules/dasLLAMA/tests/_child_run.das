options gen2
options _dasllama_internal = true

module _child_run shared public

require daslib/fio
require daslib/strings_boost
require strings

// a test's child process: this binary on a root fixture of this folder, under named environment
// settings, its stdout and stderr read whole - the instrument of every once-per-process contract

//! the running daslang binary, as the command line names it
def das_exe() : string {
    let args <- get_command_line_arguments()
    return empty(args) ? "" : args[0]
}

//! run `cmd` to its end (five minutes at most); `output` is everything it printed, the result its exit code
def run_child(cmd : string; var output : string&) : int => run_child_within(cmd, 300.0, output)

//! a verb of the test child's wall in seconds: under the wall of the test that spawns it
let CHILD_VERB_SECONDS = 25.0

//! `run_child` for a verb of the test child (`child_verb_cmd`): killed, with everything it spawned, past `CHILD_VERB_SECONDS`
def run_verb_child(cmd : string; var output : string&) : int => run_child_within(cmd, CHILD_VERB_SECONDS, output)

//! `run_verb_child` for a verb whose own work outlasts `CHILD_VERB_SECONDS`: killed past `seconds`
def run_verb_child(cmd : string; seconds : float; var output : string&) : int => run_child_within(cmd, seconds, output)

def private run_child_within(cmd : string; seconds : float; var output : string&) : int {
    return unsafe(popen_timeout(cmd, seconds, $(f) {
        if (f != null) {
            output = fread(f)
        }
    }))
}

//! the command that runs `root` (a file of this folder) under `envs` on this platform's shell, stderr folded into stdout;
//! an empty value unsets the variable on both shells; `args` follow the root after `--`, and `jit` runs it under -jit
def child_cmd(root : string; envs : array<tuple<name : string; value : string>>; args : string = ""; jit : bool = false) : string => child_cmd_at("modules/dasLLAMA/tests/{root}", envs, args, jit)

//! `child_cmd` for a root at `path`, relative to the das root - dastest's own, to run a test file of this folder whole
def child_cmd_at(path : string; envs : array<tuple<name : string; value : string>>; args : string = ""; jit : bool = false) : string
    => child_cmd_root("{get_das_root()}/{path}", envs, args, jit ? "-jit" : "")

//! `child_cmd` for a root at the full path `root`, `host` the host flags ahead of it (`-jit`, `-module-cache <path>`)
def child_cmd_root(root : string; envs : array<tuple<name : string; value : string>>; args : string = ""; host : string = "") : string {
    let flags = empty(host) ? "" : " {host}"
    let tail = empty(args) ? "" : " -- {args}"
    return shell_line(envs, "\"{das_exe()}\"{flags} -dasroot \"{get_das_root()}\" \"{root}\"{tail}")
}

//! the shell line that runs `core` - a command opening with a quoted program - under `envs`, stderr folded into stdout
def private shell_line(envs : array<tuple<name : string; value : string>>; core : string) : string {
    let win = get_platform_name() == "windows"
    let prefix = build_string() $(w) {
        for (e in envs) {
            w |> write(win ? "set \"{e.name}={e.value}\"&& " : "{e.name}={e.value} ")
        }
    }
    let line = "{prefix}{core} 2>&1"
    // cmd.exe strips the first and last quote of a command line that opens with one, which breaks every quoted path after
    // the exe's; an env prefix opens the line with `set`, and a line with none is wrapped in one more pair cmd strips instead
    return (win && empty(envs)) ? "\"{line}\"" : line
}

// ===== the test child: one program, a verb a contract =====

let private CHILD_ROOT = "modules/dasLLAMA/tests/_child_root.das"
let private CHILD_EXE_DIR = ".cache/daslang/dasllama_child"
let private CHILD_BUILD_SECONDS = 1800l   //! the two builds' cap: an engine compile on the slowest box, several times over
//! the source folders the test child's build reads, by their `.das` files: an edit under one re-keys the exe
let private CHILD_SOURCE_DIRS = fixed_array("daslib", "dastest", "modules/dasLLAMA/dasllama", "modules/dasLLAMA/tests",
    "modules/dasLLVM/daslib", "modules/dasMetal/metal", "modules/dasVulkan/daslib", "modules/dasCUDA/daslib")
//! the environment a build bakes into the exe: the tune and JIT settings macros read at compile time
let private CHILD_BUILD_ENVS = fixed_array("DAS_TUNE_MANIFEST", "DAS_TUNE_FORCE", "DAS_TUNE_POLICY", "DAS_TUNE_MODE", "DAS_JIT_BASELINE")

//! The key of the test child's exe for this tree, binary and build environment: the size and time of the libraries the
//! binary is built of (the binary's own where the tree has no `lib/`) and of every `.das` the build can read - a tests-folder file counts where it is the child's own (`_child_*.das`) - and the
//! values of the compile-time variables. A changed key is a stale exe
def child_exe_key() : string {
    let root = get_das_root()
    var h = FNV1A_SEED
    var libs : array<string>
    dir("{root}/lib") $(name) {   //! the compiler and runtime libraries: the binary by what it is built of, whatever launcher names it
        libs |> push(clone_string(name))
    }
    sort(libs)
    for (name in libs) {
        let st = stat("{root}/lib/{name}")
        h = fnv1a("lib/{name}:{st.size}:{st.mtime}", h)
    }
    if (empty(libs)) {
        let exe = stat(das_exe())
        h = fnv1a("{exe.size}:{exe.mtime}", h)
    }
    for (name in CHILD_BUILD_ENVS) {
        h = fnv1a("{name}={get_env_variable(name)}", h)
    }
    for (rel in CHILD_SOURCE_DIRS) {
        let own = rel == "modules/dasLLAMA/tests"
        var inscope names : array<string>
        dir("{root}/{rel}") $(name) {
            if (name |> ends_with(".das") && (!own || name |> starts_with("_child_"))) {
                names |> push(clone_string(name))
            }
        }
        sort(names)
        for (name in names) {
            let st = stat("{root}/{rel}/{name}")
            h = fnv1a("{rel}/{name}:{st.size}:{st.mtime}", h)
        }
    }
    return "{h}"
}

//! the engine's public entry modules: what a program with no escape line may require
let ENTRY_REQUIRES = fixed_array("require dasllama/dasllama", "require dasllama/dasllama_scheduler",
    "require dasllama/dasllama_exchange_schema", "require dasllama/dasllama_bench")

//! a one-function program requiring `requires` (one a line), no escape line
def fixture_source(requires : array<string>) : string =>
    join(requires, "\n") + "\n\n[export]\ndef main() " + "\{\n    print(\"ok\\n\")\n\}\n"

//! The module-cache record of the entry program's compile (`fixture_source` of `ENTRY_REQUIRES`) where the one for this
//! tree is built, else "": the record is one program's, read and rewritten by every compile handed it, so the entry
//! program's compile alone takes it - an engine parse the runner already paid
def child_compile_cache() : string {
    let path = child_cache_path(child_exe_key())
    return stat(path).is_valid ? path : ""
}

//! the entry program's file beside its record (`child_compile_cache`): a record serves the root it was written from, by
//! path, and a copy of the same text elsewhere reparses the engine
def child_entry_program() : string => "{dir_name(child_cache_path(child_exe_key()))}/entries.das"

def private child_cache_path(key : string) : string => "{get_das_root()}/{CHILD_EXE_DIR}/{key}/engine.dascache"

//! where the test child's exe for `key` sits under the das root
def child_exe_path(key : string) : string
    => "{get_das_root()}/{CHILD_EXE_DIR}/{key}/dasllama_test_child{get_platform_name() == "windows" ? ".exe" : ""}"

//! one build of the pair `child_exe_ensure` runs: its process, what it printed so far, and its exit code once it ended
struct private ChildBuild {
    proc : process
    out : array<string>
    rc : int = process_running
}

//! takes the lines `b` has ready and its exit code where it ended; true while it runs
def private build_step(var b : ChildBuild) : bool {
    return false if (b.rc != process_running)
    unsafe {
        while (process_drain(b.proc) $(line) { b.out |> push(clone_string(line)) }) {
        }
        b.rc = process_poll(b.proc)
    }
    return b.rc == process_running
}

//! Builds the test child's exe and the entry program's compile record for this tree where either is not current, those
//! of earlier keys removed first - the two builds side by side, each an engine compile. Returns a failed build's output,
//! "" on success or where both were current. The runner calls it before its first child of a run: no test's wall admits
//! an engine compile
[arch(at = "../ARCHITECTURE_ENGINE_SUPPORT.md#test-child")]
def child_exe_ensure() : string {
    let key = child_exe_key()
    let path = child_exe_path(key)
    let cache = child_cache_path(key)
    return "" if (stat(path).is_valid && stat(cache).is_valid)
    let root = get_das_root()
    rmdir_rec("{root}/{CHILD_EXE_DIR}")
    mkdir_rec(dir_name(path))
    let entries = child_entry_program()
    return "the entry program was not written: {entries}" if (!fwrite(entries, fixture_source([for (line in ENTRY_REQUIRES); line])))
    var exe = ChildBuild()
    var parse = ChildBuild()
    let noenv : array<string>
    unsafe {
        exe.proc = spawn_process([das_exe(), "-dasroot", root, "-exe", "-output", path, "{root}/{CHILD_ROOT}", "--", "--jit-split-modules=-1"], "", noenv)
        parse.proc = spawn_process([das_exe(), "-dasroot", root, "-module-cache", cache, "-compile-only", entries], "", noenv)
    }
    let t0 = ref_time_ticks()
    var late = false
    while (!late) {
        let building = build_step(exe)
        let parsing = build_step(parse)
        break if (!building && !parsing)
        sleep(50u)
        late = get_time_nsec(t0) / 1000000000l > CHILD_BUILD_SECONDS
    }
    unsafe {
        close_process(exe.proc)
        close_process(parse.proc)
    }
    return "the test child's builds did not end in {CHILD_BUILD_SECONDS} s" if (late)
    return "the test child did not build (exit {exe.rc}):\n{join(exe.out, "\n")}" if (exe.rc != 0 || !stat(path).is_valid)
    return "the entry program did not compile (exit {parse.rc}):\n{join(parse.out, "\n")}" if (parse.rc != 0 || !stat(cache).is_valid)
    //! a new exe's first start is slow on some hosts: paid here, not inside a test's wall
    var out : string
    let warm = run_and_capture([path, "ping"], out, 300.0)
    return warm == 0 ? "" : "the test child built and its first start failed (exit {warm}):\n{out}"
}

//! The command that runs the test child's `verb` under `envs`, stderr folded into stdout: the standalone exe where the
//! one for this tree is built, else the root under daslang - the same program, an engine compile slower
def child_verb_cmd(verb : string; envs : array<tuple<name : string; value : string>>; args : string = "") : string {
    let path = child_exe_path(child_exe_key())
    let tail = empty(args) ? verb : "{verb} {args}"
    return child_cmd(slice(CHILD_ROOT, length("modules/dasLLAMA/tests/")), envs, tail) if (!stat(path).is_valid)
    return shell_line(envs, "\"{path}\" {tail}")
}
