options gen2

require dastest/testing_boost public
require daslib/fio
require strings

def private run_protocol_test(var output : string&) : int {
    let root = get_das_root()
    var compiler = path_join(root, "bin/Release/daslang.exe")
    if (!fexist(compiler)) {
        let args <- get_command_line_arguments()
        compiler = empty(args) ? "daslang.exe" : args[0]
    }
    let script = path_join(root,
        "utils/dasHerd/watcher/tests/test_watcher_protocol.ps1")
    var argv <- ["powershell.exe", "-NoLogo", "-NoProfile",
        "-ExecutionPolicy", "Bypass", "-File", script,
        "-DaslangPath", compiler, "-Root", root]
    return unsafe(popen_argv(argv, 0.0, $(file) {
        if (file != null) {
            output = unsafe(fread_to_eof(file))
        }
    }))
}

[test]
def test_watcher_protocol_integration(t : T?) {
    if (get_platform_name() != "windows") return
    t |> run("two-repository session survives attach, detach, and controller handoff") <| @(t : T?) {
        var output : string
        let exit_code = run_protocol_test(output)
        t |> equal(exit_code, 0, output)
        t |> success(find(output, "PASS watcher protocol integration:") >= 0,
            "protocol harness did not report success: {output}")
    }
}
