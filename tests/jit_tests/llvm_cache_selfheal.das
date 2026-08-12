options gen2

require dastest/testing_boost
require daslib/module_path
require daslib/fio
require strings
require _llvm_cache_common

// The module cache's corruption self-heal, end to end and interpreter-only (no jit gate -
// this runs in every dastest sweep): a damaged record resumes (skip + reparse in place,
// later records still served) AND triggers the writeback, so the NEXT run reads clean.
// Drift-flavored partials (the dasbind case) deliberately do NOT rewrite - that arm is
// covered by llvm_ast_roundtrip's warm rounds; this file is the damage arm.

// stomp printable garbage across the first record's PAYLOAD: past the stream header +
// record header (mtime+size+filename+len - the temp-dir filename varies in length, hence
// the spread), far before the record's end (records here are >100KB), so the length field
// stays intact and the skip lands true. Many spots on purpose: a single spot could land
// inside a serialized string's CONTENT, which reads back silently - hitting structural
// bytes (lengths, tags, counts) is what forces the THROWING failure flavor
def private corrupt_first_record(path : string) : bool {
    var ok = false
    fopen(path, "rb+") $(f) {
        if (f != null) {
            var at = 300
            while (at < 8000) {
                f |> fseek(int64(at), seek_set)
                f |> fwrite("################################################################")
                at += 400
            }
            ok = true
        }
    }
    return ok
}

[test]
def test_cache_corruption_selfheal(t : T?) {
    let args <- get_command_line_arguments()  // locked view - do not delete
    let bin = get_full_file_name(args[0])
    let fixtures = get_this_module_dir()
    var terr : string
    let tmp = create_temp_directory("das_heal", terr)
    if (empty(tmp)) {
        t |> success(false, "create_temp_directory: {terr}")
        return
    }
    t |> success(copy_fixture(fixtures, tmp, "_llvm_split_modules_client.das"), "stage client fixture")
    t |> success(copy_fixture(fixtures, tmp, "_llvm_split_modules_mod_a.das"), "stage mod_a fixture")
    t |> success(copy_fixture(fixtures, tmp, "_llvm_split_modules_mod_b.das"), "stage mod_b fixture")
    // child CWD = tmp, same shape as llvm_ast_roundtrip: a popen command that STARTS with a
    // quoted path trips cmd.exe's outer-quote stripping; the cd prefix sidesteps it
    let cd_prefix = get_platform_name() == "windows" ? "cd /d \"{tmp |> replace("/", "\\")}\" && " : "cd \"{tmp}\" && "
    let base = "{cd_prefix}\"{bin}\" -dasroot \"{get_das_root()}\" -module-cache \"{tmp}/mc.bin\" -compile-only \"{tmp}/_llvm_split_modules_client.das\""
    // round 1: cold - builds the cache
    expect_round(t, "heal round 1 cold", base, ["ser: wrote"])
    // round 2: damage the first record's payload - the run must resume past it (reparse in
    // place, everything later served) AND repair the stream (corruption-flavored writeback)
    t |> success(corrupt_first_record("{tmp}/mc.bin"), "stomp record payload")
    expect_round(t, "heal round 2 corrupt", base,
        ["ser: reparsing in place", "deser: partial", "ser: wrote"])
    // rounds 3-4: the heal converges in at most TWO rounds - a record re-serialized from a
    // program that was itself served during the corrupt read (second generation) can fail
    // one more read; that round resumes it and rewrites from the fresh parse. Every round
    // still compiles correctly (rc 0) - only the verdict is allowed one extra 'partial'
    expect_round(t, "heal round 3", base, [], ["deser: clean", "deser: partial"])
    expect_round(t, "heal round 4 converged", base, ["deser: clean"])
    var err : string
    rmdir_rec(tmp, err)
}
