options gen2
//! no_aot: every test here drives the JIT or a JIT-built artifact
options no_aot

require daslib/ast
require daslib/fio
require strings
require dastest/testing_boost
require daslib/strings_boost
require daslib/clargs
require _jit_children

[test(description = "an x86 class with no F16C widens halves through the 2^112 scale, vectorized, and the half tests hold under it", timeout = 1200)]
def test_half_widen_without_f16c(t : T?) {
    if (!jit_enabled() || get_architecture_name() != "x86_64") {
        t |> skip("the x86-base class needs an x86_64 host under the JIT")
        return
    }
    let bin = get_host_binary()
    let windows = get_platform_name() == "windows"
    let baseEnv = windows ? "set DAS_JIT_BASELINE=x86-base&& " : "DAS_JIT_BASELINE=x86-base "
    var terr : string
    mkdir_rec(OUTPUT_DIR)
    let dump = path_join(OUTPUT_DIR, "half_soft_{ref_time_ticks()}")
    var out : array<string>
    spawn_in_temp("{baseEnv}\"{bin}\" {NO_DLL_CACHE_JIT} \"{WASM_LOWERING_FIXTURE}\" -- "
        + "--jit-compile-only "
        + "--jit-dump=\"{dump}.base\"", out)
    let ln <- split(fread("{dump}.base"), "\n")
    let scales = count_with(ln, "fmul ", "splat (float {TWO_POW_112})")
    t |> success(scales >= 1, "the widening loop vectorizes over the 2^112 scale ({scales} lines)")
    t |> success(count_with(ln, "x half>") == 0 && count_with(ln, "fpext") == 0,
        "no half type reaches the IR")
    remove("{dump}.base", terr)
    var run : array<string>
    let half_tests = "{get_das_root()}/tests/jit_tests/half_convert.das"
    let rc = spawn_in_temp("{baseEnv}\"{bin}\" {NO_DLL_CACHE_JIT} "
        + "\"{get_das_root()}/dastest/dastest.das\" -- --test \"{half_tests}\"", run)
    t |> equal(rc, 0, "the half tests pass under x86-base: {join(run, " | ")}")
}

let BASELINE_FMA_FIXTURE = "{get_das_root()}/tests/jit_tests/_baseline_fma_fixture.das"

[test(description = "under x86-avx2 the uncached JIT emits FMA: float4 sin reduces fused", timeout = 1200)]
def test_baseline_class_reaches_uncached_jit(t : T?) {
    if (!jit_enabled() || get_architecture_name() != "x86_64" || !cpu_supports("fma")) {
        t |> skip("the x86-avx2 class runs on an x86_64 host with FMA under the JIT")
        return
    }
    let bin = get_host_binary()
    let windows = get_platform_name() == "windows"
    let avx2Env = windows ? "set DAS_JIT_BASELINE=x86-avx2&& " : "DAS_JIT_BASELINE=x86-avx2 "
    var out : array<string>
    let rc = spawn_in_temp("{avx2Env}\"{bin}\" {NO_DLL_CACHE_JIT} \"{BASELINE_FMA_FIXTURE}\"", out)
    t |> equal(rc, 0, "the fixture runs under x86-avx2: {join(out, " | ")}")
    t |> success(count_with(out, "fused") >= 1 && count_with(out, "unfused") == 0,
        "the uncached JIT builds for the class, FMA included: {join(out, " | ")}")
}

[test(description = "a break inside a try arm of a loop whose recover arm can panic leaves the loop under -jit on a cold cache and again on the warm one", timeout = 1200)]
def test_break_inside_a_try_arm_leaves_the_loop_under_jit(t : T?) {
    if (!jit_enabled()) {
        t |> skip("the child runs under -jit, which this build does not carry")
        return
    }
    let bin = get_host_binary()
    let cwd = "{OUTPUT_DIR}/break_in_try_{ref_time_ticks()}"   // fresh: round one's cache is cold
    mkdir_rec(cwd)
    let fixture = "{FIXTURE_DIR}/_fixture_break_in_try.das"
    let root = get_das_root()
    for (round in range(2)) {
        var inscope lines : array<string>
        let rc = spawn("{cd_to(cwd)} && \"{bin}\" -dasroot \"{root}\" -jit \"{fixture}\"", lines)
        let out = join(lines, "\n")
        t |> equal(0, rc, "round {round}: the child runs under -jit, got:\n{out}")
        t |> success(lines |> has("BREAK_IN_TRY 2"),
                     "round {round}: the loop leaves at i == 2, got:\n{out}")
    }
}

[test(description = "a load through an odd int? that a parity test guards is skipped under -jit: the pointer parameter carries no alignment the fold could use", timeout = 1200)]
def test_guarded_load_through_an_odd_pointer_is_skipped(t : T?) {
    if (!jit_enabled()) {
        t |> skip("the child runs under -jit, which this build does not carry")
        return
    }
    let bin = get_host_binary()
    let fixture = "{FIXTURE_DIR}/_fixture_misaligned_pointer.das"
    var lines : array<string>
    let rc = spawn_in_temp("\"{bin}\" {NO_DLL_CACHE_JIT} \"{fixture}\"", lines)
    let out = join(lines, "\n")
    t |> equal(0, rc, "the guarded load through an odd pointer is skipped, got:\n{out}")
}

def private launder_calls(dump : string) : int {
    let ln <- split(fread(dump), "\n")
    return count_with(ln, "call ptr @llvm.launder.invariant.group")
}

//! the launder calls of a one-module and a split build of `fixture`
def private hidden_bases(t : T?; fixture : string) : tuple<one : int; split : int> {
    let bin = get_host_binary()
    let root = get_das_root()
    let cwd = "{OUTPUT_DIR}/solid_base_{ref_time_ticks()}"   // fresh: a cold cache
    mkdir_rec(cwd)
    var out : array<string>
    let rc_one = spawn_in_temp("\"{bin}\" {NO_DLL_CACHE_JIT} \"{fixture}\" "
        + "-- --jit-dump=\"{cwd}/one.ll\"", out)
    t |> equal(rc_one, 0, "the one-module child runs, got:\n{join(out, "\n")}")
    var split_out : array<string>
    let rc_split = spawn("{cd_to(cwd)} && \"{bin}\" -dasroot \"{root}\" -no-module-cache "
        + "-jit \"{fixture}\" -- --jit-dump=split.ll", split_out)
    t |> equal(rc_split, 0, "the split child runs, got:\n{join(split_out, "\n")}")
    let res = (one = launder_calls("{cwd}/one.ll"), split = launder_calls("{cwd}/split.ll"))
    rmdir_rec_result(cwd)
    return res
}

[test(description = "under solid_context the address of a global array a loop indexes - shared or not, the first global of its block or a later one - reaches code generation hidden in the one-module and the split build, and a scalar global's does not", timeout = 1200)]
def test_solid_array_base_is_hidden_from_codegen(t : T?) {
    if (!jit_enabled()) {
        t |> skip("the child runs under -jit, which this build does not carry")
        return
    }
    let hidden = hidden_bases(t, "{FIXTURE_DIR}/_fixture_solid_array_base.das")
    t |> equal(hidden.one, 3, "the one-module build hides the three arrays alone")
    t |> equal(hidden.split, 3, "the split build hides them too")
}

[test(description = "under solid_context a function that only reads the block gets it as an argument, and the array it indexes through that argument is hidden too", timeout = 1200)]
def test_solid_array_base_is_hidden_behind_a_promoted_argument(t : T?) {
    if (!jit_enabled()) {
        t |> skip("the child runs under -jit, which this build does not carry")
        return
    }
    let hidden = hidden_bases(t, "{FIXTURE_DIR}/_fixture_solid_promoted_block.das")
    t |> equal(hidden.one, 1, "the one-module build hides the array")
    t |> equal(hidden.split, 1, "the split build hides it too")
}
