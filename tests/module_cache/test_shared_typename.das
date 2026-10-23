options gen2
options indenting = 4

require dastest/testing_boost public

require strings
require daslib/fio
require _mc_common

[test, arch(at="ARCHITECTURE.md#process-startup")]
def test_shared_typename_cache(t : T?) {
    let tmp = test_temp_dir()
    let root = get_das_root()
    let fixture = path_join(root,
        "tests/module_cache/_fixtures/mc_shared_typename.das")
    let cache = path_join(tmp, "shared_typename.bin")
    let win = get_platform_name() == "windows"
    let args <- get_command_line_arguments()
    let uses_aot = find_index(args, "--use-aot") >= 0
    let host = get_executable_path()
    let exe = (uses_aot
        ? path_join(dir_name(host), win ? "daslang.exe" : "daslang") : host)
    let enter = win ? "cd /d" : "cd"
    let cmd = ("{enter} \"{tmp}\" && \"{exe}\" -dasroot \"{root}\""
        + " -module-cache \"{cache}\" \"{fixture}\"")
    let type_name = "tuple<a:float;b:float;c:float>"
    let marker = "SHARED_TYPENAME={type_name}\n"
    let expected_dll = uses_aot || das_is_dll_build()
    let host_marker = "SHARED_TYPENAME_DLL={expected_dll}\n"
    let clean = "deser: clean - all modules from cache\n"
    for (phase, verdict in ["cold", "warm"], ["ser: wrote", clean]) {
        var out : string
        let ok = run_child_reported(t, phase, cmd, out, marker)
        t |> success(ok && find(out, verdict) >= 0
            && find(out, host_marker) >= 0, "{phase} cache result:\n{out}")
    }
    var bytes : array<uint8>
    fopen(cache, "rb") $(f) {
        t |> strictEqual(f != null, true)
        bytes |> resize(int64(fstat(f).size))
        let read_size = long_fread(f, bytes)
        t |> strictEqual(read_size, long_length(bytes))
    }
    let stream_magic = "\xc4\x82\xcd\xea\x04"
    let version_232 = [0xe8u8, 0x01u8]
    let version_offset = length(stream_magic)
    t |> strictEqual(length(bytes) >= version_offset + 2, true)
    t |> strictEqual(find(bytes, stream_magic), 0)
    t |> strictEqual(int(bytes[version_offset]) & 128, 128)
    t |> strictEqual(int(bytes[version_offset + 1]) & 128, 0)
    let name_offset = find(bytes, type_name)
    t |> strictEqual(name_offset >= 0, true)
    t |> strictEqual(find(bytes, type_name, name_offset + 1), -1)
    let separator = find(type_name, ';')
    bytes[name_offset + separator] = uint8(' ')
    let bad_name = modify_data(type_name) $(var data) {
        data[separator] = uint8(' ')
    }
    let bad_marker = "SHARED_TYPENAME={bad_name}\n"
    for (phase, expected, verdict in ["poisoned", "legacy", "rewritten"],
            [bad_marker, marker, marker],
            [clean, "deser: FALLBACK", clean]) {
        if (phase == "legacy") {
            for (b, i in version_232, count()) {
                bytes[version_offset + i] = b
            }
        }
        if (phase != "rewritten") {
            fopen(cache, "wb") $(f) {
                t |> strictEqual(f != null, true)
                let written = long_fwrite(f, bytes)
                t |> strictEqual(written, long_length(bytes))
            }
        }
        var out : string
        let ok = run_child_reported(t, phase, cmd, out, expected)
        t |> success(ok && find(out, verdict) >= 0
            && find(out, host_marker) >= 0, "{phase} cache result:\n{out}")
    }
}
