// DAS_JIT_BASELINE=<class> builds for a CPU class instead of the running box: the build
// announces the class, a kernel-carrying -exe no longer targets the box, and the artifact bakes
// the class's tier gates (the tier_bits selftest generator) on the generic feature rail. A class
// of another architecture, or a name no class carries, refuses the build by name.
options gen2
options no_aot

require dastest/testing_boost
require daslib/fio
require daslib/strings_boost
require strings

let OUTPUT_DIR = "{get_das_root()}/build/tests"
let SCRIPT = "{OUTPUT_DIR}/llvm_jit_baseline_kernel.das"
let EXE = "{OUTPUT_DIR}/llvm_jit_baseline_kernel"
let HOST_CPU_LINE = "targeting the current box"
let BASELINE_LINE = "DAS_JIT_BASELINE="

// the lowest class of this arch's ladder that every CI box of the arch runs, and the tier bits
// the class's gates bake: avx2 + f16c on x86, dotprod + fullfp16 on arm
def private lowest_class() : string {
    return get_architecture_name() == "x86_64" ? "x86-avx2" : "arm-neon"
}

def private lowest_class_bits() : int {
    return get_architecture_name() == "x86_64" ? 3 : 1280
}

def private foreign_class() : string {
    return get_architecture_name() == "x86_64" ? "arm-neon" : "x86-avx2"
}

def private write_probe_script() : bool {
    var ok = false
    mkdir_rec(OUTPUT_DIR)
    fopen(SCRIPT, "w") $(f) {
        if (f != null) {
            fprint(f, "options gen2\n")
            fprint(f, "require daslib/tune\n")
            fprint(f, "[llvm_code(name = \"llvm_code_selftest::host_features\")]\n")
            fprint(f, "def host_rail() : int \{ return -1 \}\n")
            fprint(f, "[llvm_code(name = \"llvm_code_selftest::tier_bits\")]\n")
            fprint(f, "def tier_bits() : int \{ return -1 \}\n")
            fprint(f, "[export] def main() \{ print(\"HOSTF=[\{host_rail()}] TIER=[\{tier_bits()}]\\n\"); \}\n")
            ok = true
        }
    }
    return ok
}

// env prefix leads, so no sacrificial-quote wrap is needed (cmd.exe only strips a LEADING quote)
def private env_prefix(baseline : string) : string {
    return (get_platform_name() == "windows" ?
        "set DAS_TUNE_MODE=&& set DAS_JIT_BASELINE={baseline}&& " :
        "DAS_TUNE_MODE= DAS_JIT_BASELINE={baseline} ")
}

def private spawn_child(cmd : string; var lines : array<string>) : int {
    var rc : int
    unsafe {
        rc = popen_timeout("{cmd} 2>&1", 300.0) $(f) {
            if (f == null) {
                return
            }
            while (!feof(f)) {
                let ln = strip(fgets(f))
                if (!empty(ln)) {
                    lines |> push("{ln}")
                }
            }
        }
    }
    return rc
}

def private has(lines : array<string>; needle : string) : bool {
    for (ln in lines) {
        if (find(ln, needle) >= 0) return true
    }
    return false
}

[test]
def test_baseline_class_replaces_the_box(t : T?) {
    if (!jit_enabled()) {
        t |> success(true)
        return
    }
    let args <- get_command_line_arguments()  // locked view — do not delete
    let bin = args[0]
    let klass = lowest_class()
    t |> success(write_probe_script(), "failed to write the probe script")

    var lb : array<string>
    let rc = spawn_child("{env_prefix(klass)}\"{bin}\" -exe -output \"{EXE}\" \"{SCRIPT}\"", lb)
    t |> equal(0, rc, "the baseline exe must build: {join(lb, "\n")}")
    t |> success(lb |> has("{BASELINE_LINE}{klass}"), "the build announces the baseline class it targets")
    t |> success(!(lb |> has(HOST_CPU_LINE)), "a kernel-carrying exe under a baseline never targets the box")

    // the DLL rail honors the class the same way - one truth on every rail
    var ld : array<string>
    spawn_child("{env_prefix(klass)}\"{bin}\" -jit -jit-no-cache \"{SCRIPT}\"", ld)
    t |> success(ld |> has("{BASELINE_LINE}{klass}"), "a -jit run under a baseline announces the class too")
    t |> success(ld |> has("HOSTF=[0] TIER=[{lowest_class_bits()}]"), "the jitted kernels bake the class's gates on the generic rail: {join(ld, "\n")}")

    // run the built exe where the platform can load the shared lib (the jit_exe.das gates)
    let can_run = get_platform_name() != "windows" && das_is_dll_build()
    if (can_run && fexist("{EXE}.exe")) {
        var lr : array<string>
        spawn_child("\"{EXE}.exe\"", lr)
        t |> success(lr |> has("HOSTF=[0] TIER=[{lowest_class_bits()}]"), "the exe bakes the class's gates, not the box's: {join(lr, "\n")}")
    }
}

[test]
def test_baseline_refuses_a_foreign_or_unknown_class(t : T?) {
    if (!jit_enabled()) {
        t |> success(true)
        return
    }
    let args <- get_command_line_arguments()  // locked view — do not delete
    let bin = args[0]
    t |> success(write_probe_script(), "failed to write the probe script")

    var lf : array<string>
    let rcf = spawn_child("{env_prefix(foreign_class())}\"{bin}\" -exe -output \"{EXE}\" \"{SCRIPT}\"", lf)
    t |> success(rcf != 0, "a class of another architecture refuses the build")
    t |> success(lf |> has("never crosses architectures"), "the refusal names the architecture mismatch: {join(lf, "\n")}")

    var lu : array<string>
    let rcu = spawn_child("{env_prefix("x86-zen9")}\"{bin}\" -exe -output \"{EXE}\" \"{SCRIPT}\"", lu)
    t |> success(rcu != 0, "a name no class carries refuses the build")
    t |> success(lu |> has("names no CPU class") && lu |> has("x86-avx2"), "the refusal lists the classes: {join(lu, "\n")}")
}
