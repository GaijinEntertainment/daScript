options gen2

require dastest/testing_boost public
require ../text_lines.das
require ../parallel_workers.das
require ../cmake_cache.das
require strings

[test(description = "non_empty_lines keeps the non-blank lines of a text")]
def test_non_empty_lines(t : T?) {
    t |> equal(non_empty_lines("a.txt\r\nb/c.txt\n"), ["a.txt", "b/c.txt"],
        "CR and the trailing newline dropped")
    t |> equal(length(non_empty_lines("")), 0, "empty output is no lines")
    t |> equal(length(non_empty_lines("\n\r\n   \n")), 0, "blank and whitespace-only lines dropped")
}

[test(description = "cmake_cache_value reads an entry of any type, a CRLF line end included, and answers empty for a missing entry or a longer name")]
def test_cmake_cache_value(t : T?) {
    let cache_text = ("CMAKE_GENERATOR_INSTANCE:INTERNAL=C:/VS\n" +
        "CMAKE_GENERATOR:INTERNAL=Ninja Multi-Config\r\n" +
        "DAS_HOST:UNINITIALIZED=C:/sdk/bin/daslang.exe\nBROKEN:BOOL\n")
    t |> equal(cmake_cache_value(cache_text, "CMAKE_GENERATOR"), "Ninja Multi-Config",
        "the entry, not the longer name before it")
    t |> equal(cmake_cache_value(cache_text, "DAS_HOST"), "C:/sdk/bin/daslang.exe",
        "an entry of another type")
    t |> equal(cmake_cache_value(cache_text, "DAS_OTHER"), "", "no such entry")
    t |> equal(cmake_cache_value(cache_text, "BROKEN"), "", "a line with no value")
    t |> equal(cmake_cache_value("", "CMAKE_GENERATOR"), "", "an empty cache")
}

[test(description = "cmake_build_generator reads the generator a build directory was configured with, answers empty without a cache, and only the multi-config generators are told apart")]
def test_cmake_build_generator(t : T?) {
    var err : string
    let build_dir = create_temp_directory("das_cmake_cache", err)
    if (empty(build_dir)) {
        t |> failure("create_temp_directory: {err}")
        return
    }
    t |> equal(cmake_build_generator(build_dir), "", "a directory with no cache")
    fwrite(path_join(build_dir, "CMakeCache.txt"),
        "CMAKE_BUILD_TYPE:STRING=Release\nCMAKE_GENERATOR:INTERNAL=Visual Studio 18 2026\n")
    t |> equal(cmake_build_generator(build_dir), "Visual Studio 18 2026", "the cache's generator")
    for (multi in ["Visual Studio 18 2026", "Xcode", "Ninja Multi-Config"]) {
        t |> success(is_multi_config_generator(multi), "{multi} builds a folder per configuration")
    }
    for (single in ["Ninja", "Unix Makefiles", ""]) {
        t |> success(!is_multi_config_generator(single), "'{single}' builds one configuration")
    }
    remove(path_join(build_dir, "CMakeCache.txt"))
    rmdir(build_dir)
}

[test(description = "split_trimmed splits a text on a separator and trims each piece")]
def test_split_trimmed(t : T?) {
    t |> equal(split_trimmed(" a.bin, ,b.gguf ,", ","), ["a.bin", "b.gguf"],
        "pieces stripped, empty ones dropped, in order")
    t |> equal(length(split_trimmed("", ",")), 0, "an empty text is no pieces")
    t |> equal(split_trimmed("x", ","), ["x"], "no separator is one piece")
}

[test(description = "run_chunk_workers runs every argv as a child at once and returns each one's exit code, output and wall seconds in input order")]
def test_run_chunk_workers(t : T?) {
    var err : string
    let tmp = create_temp_directory("das_chunk_workers", err)
    if (empty(tmp)) {
        t |> failure("create_temp_directory: {err}")
        return
    }
    let script = path_join(tmp, "child.das")
    let body = ("options gen2\nrequire strings\n[export]\ndef main() : int \{\n"
        + "    let args <- get_command_line_arguments()\n"
        + "    let last = args[length(args) - 1]\n"
        + "    print(\"child \{last\}\\n\")\n    return to_int(last)\n\}\n")
    fwrite(script, body)
    let exe = get_command_line_arguments()[0]
    let runs <- run_chunk_workers([[exe, script, "--", "0"], [exe, script, "--", "3"]])
    t |> equal(length(runs), 2, "one result an argv")
    t |> equal(runs[0].exit_code, 0, "the first child's exit code")
    t |> equal(runs[1].exit_code, 3, "the second child's, in input order")
    t |> success(find(runs[0].stdout, "child 0") >= 0, "the first child's output")
    t |> success(find(runs[1].stdout, "child 3") >= 0, "the second child's output")
    let timed = runs[0].seconds > 0.0 && runs[1].seconds > 0.0
    t |> success(timed, "each child's wall time is measured")
    rmdir_rec(tmp, err)
}
