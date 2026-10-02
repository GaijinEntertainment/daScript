options gen2
options no_aot

require dastest/testing_boost public
require daslib/fio
require strings

[test]
def test_get_das_exe_host_not_named_daslang(t : T?) {
    if (get_platform_name() == "windows") {
        t |> skip("a copied host would lose the DLLs next to it")
        return
    }
    let host = path_join(test_temp_dir(), "das_host")
    var err : string
    let copied = copy_file(get_command_line_arguments()[0], host, true, err)
    t |> success(copied, "copied the host to '{host}': {err}")
    let rel = relative(host, getcwd(), err)
    if (!empty(err) || is_absolute(rel)) {
        t |> skip("the temp directory has no path relative to the current one: {err}")
        return
    }
    var output : string
    let root = get_das_root()
    let rc = unsafe(popen_argv([rel, "-dasroot", root, path_join(root, "tests/daslib/_das_exe_probe.das")], 0.0, $(f) {
        if (f != null) {
            output = fread(f)
        }
    }))
    t |> equal(rc, 0, "the host starts by '{rel}': {output}")
    let child = strip(output)
    t |> success(is_absolute(child) && fexist(child), "a host with no daslang next to it finds itself by an absolute path, not '{child}'")
}
