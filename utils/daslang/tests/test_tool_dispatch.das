options gen2

require dastest/testing_boost public
require daslib/fio
require daslib/clargs
require daslib/strings_boost
require strings

def tests_dir() : string => path_join(get_das_root(), "utils/daslang/tests")

def exe_name(name : string) : string => get_platform_name() == "windows" ? "{name}.exe" : name

struct Captured {
    rc : int
    out : string
}

//! `argv` from `cwd` ("" inherits), stdout and stderr merged
def run_daslang(argv : array<string>; cwd : string = "") : Captured {
    var res : Captured
    let noenv : array<string>
    res.rc = run_and_capture(argv, cwd, noenv, res.out, 300.0)
    return res
}

def copy_into(t : T?; src, dst : string) {
    mkdir_rec(dir_name(dst))
    var err : string
    let copied = copy_file(src, dst, true, err)
    t |> success(copied, "copied {src} to {dst}: {err}")
}

//! a scratch SDK root: daslib/builtin.das, which every compile reads; utils/echo/main.das, a tool
//! that prints its host binary and its arguments; and, when `copies` names any, copies of this
//! suite's daslang under bin/ with the runtime libraries a Windows exe loads from beside itself
def make_sdk(t : T?; name : string; copies : array<string>) : string {
    let root = path_join(test_temp_dir(), name)
    copy_into(t, path_join(get_das_root(), "daslib/builtin.das"), path_join(root, "daslib/builtin.das"))
    copy_into(t, path_join(tests_dir(), "_echo_tool.das"), path_join(root, "utils/echo/main.das"))
    let daslang = get_host_binary()
    for (c in copies) {
        copy_into(t, daslang, path_join(root, "bin/{c}"))
    }
    if (!empty(copies) && get_platform_name() == "windows") {
        let host_dir = dir_name(daslang)
        dir(host_dir) $(f) {
            if (starts_with(f, "libDaScriptDyn") && ends_with(f, ".dll")) {
                copy_into(t, path_join(host_dir, f), path_join(root, "bin/{f}"))
            }
        }
    }
    return root
}

def lines_of(out : string) : array<string> =>
    <- [for (ln in split(out, "\n")); strip(ln); where !empty(strip(ln))]

def args_of(out : string) : array<string> =>
    <- [for (ln in lines_of(out)); slice(ln, 4); where starts_with(ln, "arg=")]

def host_of(out : string) : string {
    for (ln in lines_of(out)) {
        if (starts_with(ln, "host=")) return to_generic_path(slice(ln, 5))
    }
    return ""
}

[test]
def test_tool_names(t : T?) {
    t |> run("no name lists the SDK tools, never an internal one") @(t : T?) {
        let r = run_daslang([get_host_binary(), "-tool"])
        t |> equal(r.rc, 0, r.out)
        for (name in ["lint", "dastest", "das-fmt", "tools"]) {
            t |> success(find(r.out, "    {name}\n") >= 0, "lists {name}: {r.out}")
        }
        t |> success(find(r.out, "    preflight\n") < 0, "an internal tool runs by name but is not listed: {r.out}")
    }
    t |> run("an unknown name is refused with the paths it was looked for at") @(t : T?) {
        let r = run_daslang([get_host_binary(), "-tool", "no-such-tool"])
        t |> equal(r.rc, 1, r.out)
        t |> success(find(r.out, "daslang -tool: no tool 'no-such-tool' - looked for") >= 0, r.out)
        t |> equal(length(split(r.out, "/no-such-tool/main.das")), 3, "both candidate entry files are named: {r.out}")
    }
    t |> run("a name that is a path is refused") @(t : T?) {
        for (name in ["../lint", "internal/preflight", "lint.das"]) {
            let r = run_daslang([get_host_binary(), "-tool", name])
            t |> equal(r.rc, 1, r.out)
            t |> success(find(r.out, "daslang -tool: no tool '{name}' - a tool name has no") >= 0, r.out)
        }
    }
    t |> run("das-fmt runs dasfmt.das, every argument after the name is the tool's") @(t : T?) {
        let r = run_daslang([get_host_binary(), "-tool", "das-fmt", "--path", path_join(tests_dir(), "test_tool_dispatch.das"), "--verify"])
        t |> equal(r.rc, 0, r.out)
        t |> success(find(r.out, "Verified! 1 files") >= 0, r.out)
    }
    t |> run("a name not under utils/ resolves under utils/internal/") @(t : T?) {
        let r = run_daslang([get_host_binary(), "-tool", "hygiene", "--help"])
        t |> equal(r.rc, 0, r.out)
        t |> success(find(r.out, "Usage: daslang hygiene") >= 0, r.out)
    }
}

[test]
def test_tool_arguments(t : T?) {
    let sdk = make_sdk(t, "args_sdk", array<string>())
    t |> run("-dasroot before -tool resolves the tool under that root") @(t : T?) {
        let r = run_daslang([get_host_binary(), "-dasroot", sdk, "-tool", "echo", "a", "b"], sdk)
        t |> equal(r.rc, 0, r.out)
        t |> equal(args_of(r.out), ["a", "b"], r.out)
    }
    t |> run("a -- right after the name is the tool's separator, not a second one") @(t : T?) {
        let r = run_daslang([get_host_binary(), "-dasroot", sdk, "-tool", "echo", "--", "a"], sdk)
        t |> equal(r.rc, 0, r.out)
        t |> equal(args_of(r.out), ["a"], r.out)
    }
    t |> run("a -tool after the script's -- is the script's argument") @(t : T?) {
        let r = run_daslang([get_host_binary(), "-dasroot", sdk, path_join(sdk, "utils/echo/main.das"), "--", "-tool", "lint"], sdk)
        t |> equal(r.rc, 0, r.out)
        t |> equal(args_of(r.out), ["-tool", "lint"], r.out)
    }
}

[test]
def test_tool_by_binary_name(t : T?) {
    t |> run("a copy named after a tool, alone in its bin/, hands its children itself") @(t : T?) {
        let sdk = make_sdk(t, "lone_sdk", [exe_name("echo")])
        let echo = path_join(sdk, "bin/{exe_name("echo")}")
        let r = run_daslang([echo, "a", "--", "b"], sdk)
        t |> equal(r.rc, 0, r.out)
        t |> equal(args_of(r.out), ["a", "--", "b"], r.out)
        t |> success(ends_with(host_of(r.out), "/lone_sdk/bin/{exe_name("echo")}"), r.out)
    }
    t |> run("beside a daslang, the copy hands its children that daslang") @(t : T?) {
        let sdk = make_sdk(t, "full_sdk", [exe_name("echo"), exe_name("daslang")])
        let r = run_daslang([path_join(sdk, "bin/{exe_name("echo")}"), "x"], sdk)
        t |> equal(r.rc, 0, r.out)
        t |> equal(args_of(r.out), ["x"], r.out)
        t |> success(ends_with(host_of(r.out), "/full_sdk/bin/{exe_name("daslang")}"), r.out)
    }
    if (get_platform_name() == "windows") {
        t |> run("Windows names are case-blind: ECHO.EXE is the echo tool") @(t : T?) {
            let sdk = make_sdk(t, "upper_sdk", ["ECHO.EXE"])
            let r = run_daslang([path_join(sdk, "bin/ECHO.EXE"), "y"], sdk)
            t |> equal(r.rc, 0, r.out)
            t |> equal(args_of(r.out), ["y"], r.out)
        }
    }
    t |> run("the build copies daslang as each shipped tool exe beside itself") @(t : T?) {
        let dastest_exe = path_join(dir_name(get_full_file_name(get_host_binary())), "dastest.exe")
        t |> success(fexist(dastest_exe), "the build left no {dastest_exe}")
        let r = run_daslang([dastest_exe, "--test", path_join(tests_dir(), "_host_probe.das")])
        t |> equal(r.rc, 0, r.out)
        t |> success(find(r.out, "1 passed") >= 0, r.out)
    }
}

//! the fixture project copied into the test's temp dir, so the module cache its runs write lands there
def project_copy(t : T?; name : string) : string {
    let src = path_join(tests_dir(), "_project")
    let dst = path_join(test_temp_dir(), name)
    for (rel in ["greet.das", "modules/greeter/.das_module", "modules/greeter/greeter.das"]) {
        copy_into(t, path_join(src, rel), path_join(dst, rel))
    }
    return dst
}

[test]
def test_tool_project_root(t : T?) {
    t |> run("a tool takes the project root from the directory it runs in") @(t : T?) {
        let project = project_copy(t, "project_default")
        let r = run_daslang([get_host_binary(), "-tool", "dastest", "--test", "greet.das"], project)
        t |> equal(r.rc, 0, r.out)
        t |> success(find(r.out, "1 passed") >= 0, r.out)
    }
    for (flag in ["-project_root", "-project-root"]) {
        t |> run("an explicit {flag} before -tool wins") @(t : T?) {
            let project = project_copy(t, "project{flag}")
            let r = run_daslang([get_host_binary(), flag, get_das_root(), "-tool", "dastest", "--test", "greet.das"], project)
            t |> success(r.rc != 0 && find(r.out, "missing prerequisite 'greeter/greeter'") >= 0, r.out)
        }
    }
    t |> run("an explicit -project before -tool wins") @(t : T?) {
        let project = project_copy(t, "project_file")
        fopen(path_join(project, "empty.das_project"), "wb") $(f) {
            fwrite(f, "options gen2\n")
        }
        let r = run_daslang([get_host_binary(), "-project", "empty.das_project", "-tool", "dastest", "--test", "greet.das"], project)
        t |> success(r.rc != 0 && find(r.out, "missing prerequisite 'greeter/greeter'") >= 0, r.out)
    }
}
