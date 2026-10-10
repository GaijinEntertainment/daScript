options gen2

require dastest/testing_boost public
require daslib/fio
require daslib/clargs
require strings
require ../watchdog.das

def host(is_windows : bool) : ToolHost =>
    ToolHost(daslang = "/sdk/bin/daslang", cwd = "/work/proj", das_root = "/sdk", is_windows = is_windows, cmd_exe = "C:/Windows/System32/cmd.exe")

[test]
def test_tool_front(t : T?) {
    t |> run("no -tool: the command line comes back as it was") @(t : T?) {
        let front = tool_front(["--cwd", "x", "--", "-tool", "mcp"], host(false))
        t |> equal(front.error, "")
        t |> equal(front.args, ["--cwd", "x", "--", "-tool", "mcp"])
    }
    t |> run("lsp is the --lsp front, options before and arguments after kept") @(t : T?) {
        let front = tool_front(["--log", "l.txt", "-tool", "lsp", "--cwd", "w"], host(false))
        t |> equal(front.args, ["--lsp", "--log", "l.txt", "--cwd", "w"])
    }
    t |> run("mcp: the stdio front over daslang -tool mcp, rooted at the client's directory") @(t : T?) {
        let front = tool_front(["-tool", "mcp", "--verbose"], host(false))
        t |> equal(front.args, ["--stdio", "--name", "daslang-mcp", "--cwd", "/work/proj", "--program", "/sdk/bin/daslang",
                                "--", "-ignore-manifest", "-tool", "mcp", "--verbose"])
    }
    t |> run("mcp on Windows: the MSVC launcher, the arguments behind its script selector") @(t : T?) {
        let launcher = path_join("/sdk", "utils/mcp/daslang-mcp-msvc.cmd")
        let bare = tool_front(["-tool", "mcp"], host(true))
        t |> equal(bare.args, ["--stdio", "--name", "daslang-mcp", "--cwd", "/work/proj", "--program", "C:/Windows/System32/cmd.exe",
                               "--", "/c", launcher])
        let with_args = tool_front(["-tool", "mcp", "--verbose"], host(true))
        t |> equal(with_args.args, ["--stdio", "--name", "daslang-mcp", "--cwd", "/work/proj", "--program", "C:/Windows/System32/cmd.exe",
                                    "--", "/c", launcher, "main.das", "--", "--verbose"])
    }
    t |> run("dap: debuggee and repo root follow an explicit --cwd") @(t : T?) {
        let front = tool_front(["--cwd", "/other", "-tool", "dap"], host(true))
        t |> equal(front.args, ["--stdio", "--name", "daslang-dap", "--cwd", "/other", "--program", "/sdk/bin/daslang",
                                "--", "-tool", "dap", "--repo-root", "/other", "--executable", "/sdk/bin/daslang"])
    }
    t |> run("an explicit --cwd=DIR is the project root too, never joined by a second --cwd") @(t : T?) {
        let front = tool_front(["--cwd=/other", "-tool", "dap"], host(false))
        t |> equal(front.args, ["--stdio", "--name", "daslang-dap", "--cwd=/other", "--program", "/sdk/bin/daslang",
                                "--", "-tool", "dap", "--repo-root", "/other", "--executable", "/sdk/bin/daslang"])
    }
    t |> run("a --cwd with no value leaves the client's directory as the project root") @(t : T?) {
        let front = tool_front(["--cwd", "-tool", "dap"], host(false))
        t |> equal(front.args, ["--stdio", "--name", "daslang-dap", "--cwd", "--program", "/sdk/bin/daslang",
                                "--", "-tool", "dap", "--repo-root", "/work/proj", "--executable", "/sdk/bin/daslang"])
    }
    t |> run("errors: no name, an unknown name, no daslang to spawn") @(t : T?) {
        t |> equal(tool_front(["-tool"], host(false)).error, "watchdog -tool: name the tool - lsp, mcp or dap")
        t |> equal(tool_front(["-tool", "lint"], host(false)).error, "watchdog -tool: no front for 'lint' - the tools are lsp, mcp and dap")
        var no_daslang = host(false)
        no_daslang.daslang = ""
        t |> equal(tool_front(["-tool", "dap"], no_daslang).error, "watchdog -tool dap: no daslang beside the watchdog")
    }
}

def touch(t : T?; path : string) {
    mkdir_rec(dir_name(path))
    fopen(path, "wb") $(f) {
        t |> success(f != null, "created {path}")
    }
}

[test]
def test_daslang_beside_watchdog(t : T?) {
    let exe = get_platform_name() == "windows" ? ".exe" : ""
    let root = path_join(test_temp_dir(), "beside")
    t |> run("the daslang in the watchdog's own directory") @(t : T?) {
        let flat = path_join(root, "flat")
        touch(t, path_join(flat, "daslang{exe}"))
        t |> equal(daslang_beside_watchdog(flat), path_join(flat, "daslang{exe}"))
    }
    t |> run("one level up: bin/watchdog finds bin/Release/daslang through its parent") @(t : T?) {
        let tree = path_join(root, "tree")
        touch(t, path_join(tree, "bin/Release/daslang{exe}"))
        mkdir_rec(path_join(tree, "bin"))
        t |> equal(daslang_beside_watchdog(path_join(tree, "bin")), path_join(tree, "bin/Release/daslang{exe}"))
    }
    t |> run("no daslang in reach") @(t : T?) {
        let empty_dir = path_join(root, "empty/sub")
        mkdir_rec(empty_dir)
        t |> equal(daslang_beside_watchdog(empty_dir), "")
    }
}

struct Captured {
    rc : int
    out : string
}

def run_captured(argv : array<string>) : Captured {
    var out : string
    let rc = run_and_capture(argv, out)
    return Captured(rc = rc, out = out)
}

[test]
def test_tool_front_hosts(t : T?) {
    let daslang = get_host_binary()
    t |> run("the interpreter host refuses an unknown front with exit 2") @(t : T?) {
        let r = run_captured([daslang, path_join(get_das_root(), "utils/watchdog/main.das"), "--", "-tool", "bogus"])
        t |> equal(r.rc, 2, r.out)
        t |> success(find(r.out, "watchdog -tool: no front for 'bogus'") >= 0, r.out)
    }
    let watchdog = path_join(dir_name(get_full_file_name(daslang)), get_platform_name() == "windows" ? "watchdog.exe" : "watchdog")
    if (!fexist(watchdog)) {
        t |> skip("no watchdog exe beside {daslang}")
        return
    }
    t |> run("the exe host refuses an unknown front with exit 2") @(t : T?) {
        let r = run_captured([watchdog, "-tool", "bogus"])
        t |> equal(r.rc, 2, r.out)
        t |> success(find(r.out, "watchdog -tool: no front for 'bogus'") >= 0, r.out)
    }
}
