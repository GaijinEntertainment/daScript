// DAS_JIT_BASELINE=<class> builds for a CPU class instead of the running box: the build
// announces the class, a kernel-carrying -exe no longer targets the box, and the artifact bakes
// the class's tier gates (the tier_bits selftest generator) on the generic feature rail. A class
// of another architecture, or a name no class carries, refuses the build by name; a cross
// target under a baseline is admitted when the class is the triple's arch and refused otherwise.
// Every file the baseline tests write lands in a per-process temp
// directory they remove.
options gen2
options no_aot

require dastest/testing_boost
require daslib/ast
require daslib/fio
require daslib/strings_boost
require strings
require _spawn_common
require _llvm_cache_common

let HOST_CPU_LINE = "targeting the current box"
let BASELINE_LINE = "DAS_JIT_BASELINE="

// the lowest class of this arch's ladder that every CI box of the arch runs, and the tier bits
// the class's gates bake: avx2 + f16c on x86, dotprod + fullfp16 on arm
def private lowest_class() : string => get_architecture_name() == "x86_64" ? "x86-avx2" : "arm-neon"

def private lowest_class_bits() : int => get_architecture_name() == "x86_64" ? 3 : 1280

def private foreign_class() : string =>
    get_architecture_name() == "x86_64" ? "arm-neon" : "x86-avx2"

// a linux triple of this arch and of the other one - the cross-target cases compile only
def private own_arch_triple() : string =>
    get_architecture_name() == "x86_64" ? "x86_64-unknown-linux-gnu" : "aarch64-unknown-linux-gnu"

def private foreign_arch_triple() : string =>
    get_architecture_name() == "x86_64" ? "aarch64-unknown-linux-gnu" : "x86_64-unknown-linux-gnu"

def private write_probe_script(script : string) : bool {
    var ok = false
    fopen(script, "w") $(f) {
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
def private env_prefix(baseline : string) : string =>
    child_env([("DAS_TUNE_MODE", ""), ("DAS_JIT_BASELINE", baseline), ("DAS_LOG_LEVEL", "info")])

def private has(lines : array<string>; needle : string) : bool {
    for (ln in lines) {
        if (find(ln, needle) >= 0) return true
    }
    return false
}

// the per-process work directory with the probe script in it, "" when it could not be made
def private make_work(t : T?) : string {
    let dirRes = create_temp_directory_result("llvm_jit_baseline_")
    if (dirRes is error) {
        t |> failure("could not create a temp dir: {dirRes as error}")
        return ""
    }
    let work = dirRes as value
    t |> success(write_probe_script(path_join(work, "kernel.das")), "failed to write the probe script")
    return work
}

[test(description = "under DAS_JIT_BASELINE a kernel-carrying -exe and a -jit run announce the class, never target the box, and bake the class's tier gates on the generic rail", timeout = 1200)]
def test_baseline_class_replaces_the_box(t : T?) {
    if (!jit_enabled()) {
        t |> success(true)
        return
    }
    let args <- get_command_line_arguments()  // locked view — do not delete
    let bin = args[0]
    let klass = lowest_class()
    let work = make_work(t)
    return if (empty(work))
    let script = path_join(work, "kernel.das")
    let exe = path_join(work, "kernel")

    var lb : array<string>
    let rc = spawn_child("{env_prefix(klass)}\"{bin}\" -exe -output \"{exe}.exe\" \"{script}\"", lb)
    t |> equal(0, rc, "the baseline exe must build: {join(lb, "\n")}")
    t |> success(lb |> has("{BASELINE_LINE}{klass}"), "the build announces the baseline class it targets")
    t |> success(!(lb |> has(HOST_CPU_LINE)), "a kernel-carrying exe under a baseline never targets the box")

    // the DLL rail honors the class the same way - one truth on every rail
    var ld : array<string>
    spawn_child("{env_prefix(klass)}\"{bin}\" -jit -jit-no-cache \"{script}\"", ld)
    t |> success(ld |> has("{BASELINE_LINE}{klass}"), "a -jit run under a baseline announces the class too")
    t |> success(ld |> has("HOSTF=[0] TIER=[{lowest_class_bits()}]"), "the jitted kernels bake the class's gates on the generic rail: {join(ld, "\n")}")

    // run the built exe where the platform can load the shared lib
    // (the jit_artifacts.das gates)
    let can_run = get_platform_name() != "windows" && das_is_dll_build()
    if (can_run && fexist("{exe}.exe")) {
        var lr : array<string>
        spawn_child("\"{exe}.exe\"", lr)
        t |> success(lr |> has("HOSTF=[0] TIER=[{lowest_class_bits()}]"), "the exe bakes the class's gates, not the box's: {join(lr, "\n")}")
    }
    rmdir_rec(work)
}

[test(description = "DAS_JIT_BASELINE naming another architecture's class, or a name no class carries, refuses the build and names the reason", timeout = 1200)]
def test_baseline_refuses_a_foreign_or_unknown_class(t : T?) {
    if (!jit_enabled()) {
        t |> success(true)
        return
    }
    let args <- get_command_line_arguments()  // locked view — do not delete
    let bin = args[0]
    let work = make_work(t)
    return if (empty(work))
    let script = path_join(work, "kernel.das")
    let exe = path_join(work, "kernel")

    var lf : array<string>
    let exe_args = "-exe -output \"{exe}.exe\" \"{script}\""
    let rcf = spawn_child("{env_prefix(foreign_class())}\"{bin}\" {exe_args}", lf)
    t |> success(rcf != 0, "a class of another architecture refuses the build")
    t |> success(lf |> has("never crosses architectures"), "the refusal names the architecture mismatch: {join(lf, "\n")}")

    var lu : array<string>
    let rcu = spawn_child("{env_prefix("x86-zen9")}\"{bin}\" {exe_args}", lu)
    t |> success(rcu != 0, "a name no class carries refuses the build")
    t |> success(lu |> has("names no CPU class") && lu |> has("x86-avx2"), "the refusal lists the classes: {join(lu, "\n")}")
    rmdir_rec(work)
}

// the target-triple branch: a baseline is checked against the TRIPLE's arch, not the box's
[test(description = "under DAS_JIT_BASELINE a cross target of the class's own arch compiles, and one of the other arch is refused against the triple's arch", timeout = 1200)]
def test_baseline_under_a_cross_target(t : T?) {
    if (!jit_enabled()) {
        t |> success(true)
        return
    }
    let args <- get_command_line_arguments()  // locked view — do not delete
    let bin = args[0]
    let klass = lowest_class()
    let work = make_work(t)
    return if (empty(work))
    let script = path_join(work, "kernel.das")
    let exe = path_join(work, "kernel")
    let exe_args = "-exe -output \"{exe}.exe\" \"{script}\""

    var lo : array<string>
    let rco = spawn_child("{env_prefix(klass)}\"{bin}\" {exe_args} "
        + "-- --jit-target {own_arch_triple()} --jit-compile-only", lo)
    t |> equal(0, rco, "a cross target of the class's own arch compiles under the baseline: {join(lo, "\n")}")
    t |> success(lo |> has("{BASELINE_LINE}{klass}"), "the cross build announces the class")

    var lx : array<string>
    let rcx = spawn_child("{env_prefix(klass)}\"{bin}\" {exe_args} "
        + "-- --jit-target {foreign_arch_triple()} --jit-compile-only", lx)
    t |> success(rcx != 0, "a cross target of the other arch refuses the class")
    t |> success(lx |> has("never crosses architectures"), "the refusal names the arch mismatch against the triple: {join(lx, "\n")}")
    rmdir_rec(work)
}

// A standalone -exe carrying [llvm_code] kernels must target the build box (CPU + features),
// decided BEFORE the target-flag pass runs — generic-CPU flags make every feature-gated
// generator decline to its reference body, which is how release exes shipped 11-15x-slower
// scalar kernels while -jit ran the tuned tiers. A kernel-free exe stays generic /
// redistributable. Two witnesses: the BUILD log decision line (prints before the link step,
// so sanitizer-lane link failures cannot flake it), and the selftest host_features probe —
// the generator bakes the resolved feature-rail truth into the exe as a constant, so running
// it distinguishes host (1) from generic (0) from not-generated (-1).

let BASELINE_SCRIPT = "{get_das_root()}/modules/dasLLVM/tests/_exe_baseline_intrin.das"
let BASELINE_ANSWER = "INTRIN=[1496,2480,1496]"   //! the fallback bodies over w = x = 1..16: sum of squares per lane group, the 2x2 block products, the same dot again

def private write_host_probe_script(path : string; with_kernel : bool) : bool {
    var ok = false
    fopen(path, "w") $(f) {
        if (f != null) {
            fprint(f, "options gen2\n")
            if (with_kernel) {
                fprint(f, "require daslib/tune\n")
                fprint(f, "[llvm_code(name = \"llvm_code_selftest::host_features\")]\n")
                fprint(f, "def host_rail() : int \{ return -1 \}\n")
                fprint(f, "[export] def main() \{ print(\"HOSTF=[\{host_rail()}]\\n\"); \}\n")
            } else {
                fprint(f, "[export] def main() \{ print(\"R=5\\n\"); \}\n")
            }
            ok = true
        }
    }
    return ok
}

// env prefix leads, so no sacrificial-quote wrap is needed (cmd.exe only strips a LEADING quote)
def private neutral_env() : string {
    return (get_platform_name() == "windows" ?
        "set DAS_TUNE_MODE=&& " :
        "DAS_TUNE_MODE= ")
}

def private plan_targets_host(script : string) : bool {
    var res = false
    compile_for_jit(script, true) $(var program) {
        simulate(program) $(sok, context, _serrors) {
            if (sok) {
                let cfg : JitCliOptions
                let plan <- make_jit_plan(get_ptr(program), get_ptr(context), false, cfg)
                res = plan.use_host_cpu
            }
        }
    }
    return res
}

[test(description = "a kernel-carrying -exe targets the build box and bakes the host feature rail, while a kernel-free exe and one that only calls an intrinsic stay on the generic baseline", timeout = 1200)]
def test_exe_host_cpu(t : T?) {
    if (!jit_enabled()) {
        // the decision under test lives in the LLVM backend's exe rail
        t |> success(true)
        return
    }
    let args <- get_command_line_arguments()  // locked view — do not delete
    let bin = args[0]
    let out = test_temp_dir()
    let kernel_script = "{out}/exe_host_cpu_kernel.das"
    let kernel_exe = "{out}/exe_host_cpu_kernel"
    let plain_script = "{out}/exe_host_cpu_plain.das"
    let baseline_exe = "{out}/exe_baseline_intrin"

    t |> success(write_host_probe_script(kernel_script, true),
        "failed to write kernel probe script")
    t |> success(plan_targets_host(kernel_script),
        "kernel-carrying exe build must target the host cpu")
    var lk : array<string>
    spawn_child("{neutral_env()}\"{bin}\" -exe -output \"{kernel_exe}.exe\" "
        + "\"{kernel_script}\"", lk)

    t |> success(write_host_probe_script(plain_script, false), "failed to write plain probe script")
    t |> success(!plan_targets_host(plain_script), "kernel-free exe build must stay generic")

    //! an intrinsic call alone must not promote the exe off the generic baseline - the gates decline to the fallback bodies, so the build emits nothing that machine cannot select
    t |> success(!plan_targets_host(BASELINE_SCRIPT), "an intrinsic call alone never promotes the exe to the host cpu")
    var lb : array<string>
    let rcb = spawn_child("{neutral_env()}\"{bin}\" -exe -output \"{baseline_exe}.exe\" "
        + "\"{BASELINE_SCRIPT}\"", lb)
    t |> equal(0, rcb, "the intrinsic-calling kernel-free exe must build on the generic baseline: {join(lb, "\n")}")

    // run the built exes where the platform can load the shared lib
    // (same gates as jit_artifacts.das: Windows lacks RPATH; static
    // builds ship no runtime lib for the exe to call into)
    let can_run = get_platform_name() != "windows" && das_is_dll_build()
    if (can_run && fexist("{kernel_exe}.exe")) {
        var lr : array<string>
        spawn_child("\"{kernel_exe}.exe\"", lr)
        t |> success(lr |> has("HOSTF=[1]"), "the exe must bake the HOST feature rail, not generic (0) or reference (-1)")
    }
    if (can_run && fexist("{baseline_exe}.exe")) {
        var lr : array<string>
        spawn_child("\"{baseline_exe}.exe\"", lr)
        t |> success(lr |> has(BASELINE_ANSWER), "the baseline exe computes the fallback bodies' answers: {join(lr, "\n")}")
    }
}
