options gen2

require dastest/testing_boost
require daslib/module_path
require daslib/fio
require strings
require _llvm_cache_common

// AST serializer round trip scored by the jit obj cache: -ser captures every parsed module
// (the env serializer rail) into a cache file, -deser feeds them back instead of parsing,
// and the per-partition obj cache is the fidelity oracle - a deserialized AST that
// reproduces the per-function AOT hashes links every cached object (N/N), one that doesn't
// re-emits. The verdict line guards the vacuous pass: a stale or unreadable cache silently
// falls back to reparse, which would score N/N without testing serialization at all.
// Under -jit the verdict is 'deser: partial' TODAY: llvm_func.das registers its
// [dasbind] externs into the dasbind BUILTIN module at compile time, so its record can
// never deserialize in a cold process - the reader skips it (per-record length), reparses
// it in place (replaying the registrations), and keeps serving every later record. The
// warm rounds accept 'partial' OR 'clean' so fixing dasbind (a strict improvement, an
// agreed future arc) does not red this test.

[test]
def test_jit_ast_roundtrip(t : T?) {
    if (!jit_enabled() || !das_is_dll_build()) {
        // the oracle rides the LLVM JIT obj cache; the child needs -jit too
        t |> skip("needs -jit and a DLL daslang build")
        return
    }
    let args <- get_command_line_arguments()  // locked view — do not delete
    // absolutize: the child cd's away, and argv[0] is shell-dependent (cmd.exe/sh keep it relative)
    let bin = get_full_file_name(args[0])
    let fixtures = get_this_module_dir()
    // unique per run: round 1 is guaranteed cold and parallel workers can't collide
    var terr : string
    let tmp = create_temp_directory("das_astrt", terr)
    if (empty(tmp)) {
        t |> success(false, "create_temp_directory: {terr}")
        return
    }
    t |> success(copy_fixture(fixtures, tmp, "_llvm_split_modules_client.das"), "stage client fixture")
    t |> success(copy_fixture(fixtures, tmp, "_llvm_split_modules_mod_a.das"), "stage mod_a fixture")
    t |> success(copy_fixture(fixtures, tmp, "_llvm_split_modules_mod_b.das"), "stage mod_b fixture")
    // child CWD = tmp so .jitted_scripts lands inside it; windows cd needs /d for cross-drive
    let cd_prefix = get_platform_name() == "windows" ? "cd /d \"{tmp |> replace("/", "\\")}\" && " : "cd \"{tmp}\" && "
    let base = "{cd_prefix}\"{bin}\" -dasroot \"{get_das_root()}\""
    let script = "\"{tmp}/_llvm_split_modules_client.das\" -- --jit-split-modules=4"
    // round 0: no jit chain in the program - the clean end-to-end path
    expect_round(t, "round 0 ser interp", "{base} -ser \"{tmp}/ast0.bin\" -compile-only \"{tmp}/_llvm_split_modules_client.das\"",
        ["ser: wrote"])
    expect_round(t, "round 0 deser interp", "{base} -deser \"{tmp}/ast0.bin\" -compile-only \"{tmp}/_llvm_split_modules_client.das\"",
        ["deser: clean"])
    // round 1: cold -jit compile writes the AST cache; every partition emits
    expect_round(t, "round 1 ser", "{base} -ser \"{tmp}/ast.bin\" -jit {script}",
        ["ser: wrote", "obj cache - 0/3 partitions cached", "SPLIT_RESULT 340 19 36 33 -1"])
    // round 2: a fresh child compiles FROM the cache; the DLL is deleted so the link goes
    // through the obj probes - 3/3 proves the deserialized AST reproduced every partition
    // key. 'partial' = the dasbind registrar(s) reparsed in place, everything else served
    let dlls = delete_dlls_under("{tmp}/.jitted_scripts")
    t |> success(dlls > 0, "round 2 precondition: found a DLL to delete")
    expect_round(t, "round 2 deser", "{base} -deser \"{tmp}/ast.bin\" -jit {script}",
        ["obj cache - 3/3 partitions cached", "SPLIT_RESULT 340 19 36 33 -1"],
        ["deser: partial", "deser: clean"])
    // round 3: edit mod_a so the cache is stale - the rail must FALL BACK (mtime mismatch),
    // and the mixed program (mod_b deserialized, mod_a + main parsed) still keys mod_b's .o
    t |> success(copy_fixture(fixtures, tmp, "_llvm_split_modules_mod_a.das",
        "=> helper_b(x) + slow_b(x)", "=> helper_b(x) + slow_b(x) + 1"), "round 3 edit mod_a")
    expect_round(t, "round 3 stale", "{base} -deser \"{tmp}/ast.bin\" -jit {script}",
        ["deser: FALLBACK", "obj cache - 1/3 partitions cached", "SPLIT_RESULT 350 19 36 33 -1"])
    // round 4: -module-cache auto mode - a cold run misses and writes the cache, a warm run
    // serves from it (partial: the dasbind registrar reparses) and rewrites nothing
    expect_round(t, "round 4 mc cold", "{base} -module-cache \"{tmp}/mc.bin\" -jit {script}",
        ["ser: wrote", "SPLIT_RESULT 350 19 36 33 -1"])
    expect_round(t, "round 4 mc warm", "{base} -module-cache \"{tmp}/mc.bin\" -jit {script}",
        ["SPLIT_RESULT 350 19 36 33 -1"],
        ["deser: partial", "deser: clean"])
    var err : string
    rmdir_rec(tmp, err)
}
