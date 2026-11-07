options gen2

require dastest/testing_boost public
require daslib/fio
require daslib/clargs
require daslib/strings_boost
require strings
require _host_fixture

//! the folder holding a copied daslang and, if not "", its `sidecar`
def relocate(t : T?; name, folder, sidecar : string; exe : string = "daslang") : string {
    let dir = path_join(test_temp_dir(), name)
    let bin = path_join(dir, folder)
    copy_into(t, get_host_binary(), path_join(bin, exe_name(exe)))
    if (!empty(sidecar)) {
        fwrite(path_join(bin, "daslang.root"), sidecar)
    }
    return bin
}

def make_sdk_with_libraries(t : T?; name : string) : string {
    let sdk = make_sdk(t, name, array<string>())
    copy_daslang_libraries(t, sdk)
    return sdk
}

def probe() : string => path_join(tests_dir(), "_root_probe.das")

[test]
def test_sidecar_names_the_root(t : T?) {
    let sdk = make_sdk_with_libraries(t, "reloc/sdk")
    t |> run("a relative daslang.root is joined to the daslang's folder, not the cwd") @(t : T?) {
        let bin = relocate(t, "reloc", "venv/bin", "../../sdk")
        let elsewhere = test_temp_dir()
        let daslang = path_join(bin, exe_name("daslang"))
        let r = run_daslang([daslang, "-tool", "echo", "a", "b"], elsewhere)
        t |> equal(r.rc, 0, r.out)
        t |> equal(args_of(r.out), ["a", "b"], r.out)
        let p = run_daslang([path_join(bin, exe_name("daslang")), probe()], elsewhere)
        t |> equal(p.rc, 0, p.out)
        t |> success(ends_with(root_of(p.out), "/reloc/sdk"),
            "root resolved through the sidecar: {p.out}")
    }
    t |> run("an absolute daslang.root is taken as it is") @(t : T?) {
        let bin = relocate(t, "reloc_abs", "Scripts", sdk)
        let p = run_daslang([path_join(bin, exe_name("daslang")), probe()], bin)
        t |> equal(p.rc, 0, p.out)
        t |> equal(root_of(p.out), to_generic_path(sdk), p.out)
    }
    t |> run("the first line naming an existing folder wins") @(t : T?) {
        let bin = relocate(t, "reloc_lines", "bin", "../no_such_sdk\n{sdk}\n")
        let p = run_daslang([path_join(bin, exe_name("daslang")), probe()], bin)
        t |> equal(p.rc, 0, p.out)
        t |> equal(root_of(p.out), to_generic_path(sdk), p.out)
    }
    t |> run("surrounding spaces, tabs and a CRLF line end are not part of the root") @(t : T?) {
        let bin = relocate(t, "reloc_crlf", "bin", "\t {sdk} \t\r\n")
        let p = run_daslang([path_join(bin, exe_name("daslang")), probe()], bin)
        t |> equal(p.rc, 0, p.out)
        t |> equal(root_of(p.out), to_generic_path(sdk), p.out)
    }
    t |> run("the sidecar wins over the bin/.. derivation") @(t : T?) {
        let bin = relocate(t, "reloc_over/other/bin", "", sdk)
        copy_into(t, path_join(get_das_root(), "daslib/builtin.das"),
            path_join(test_temp_dir(), "reloc_over/other/daslib/builtin.das"))
        let p = run_daslang([path_join(bin, exe_name("daslang")), probe()], bin)
        t |> equal(p.rc, 0, p.out)
        t |> equal(root_of(p.out), to_generic_path(sdk), p.out)
    }
    t |> run("a tool copy beside the sidecar runs its tool from that root") @(t : T?) {
        let bin = relocate(t, "reloc_tool", "bin", sdk, "echo")
        let echo = path_join(bin, exe_name("echo"))
        let r = run_daslang([echo, "x", "--", "y"], bin)
        t |> equal(r.rc, 0, r.out)
        t |> equal(args_of(r.out), ["x", "--", "y"], r.out)
        t |> success(ends_with(host_of(r.out), "/reloc_tool/bin/{exe_name("echo")}"), r.out)
    }
    t |> run("started by its bare name through PATH, the daslang still finds itself") @(t : T?) {
        if (get_platform_name() == "windows") {
            t |> skip("CreateProcess resolves a bare name in the parent's own folder before PATH")
            return
        }
        let bin = relocate(t, "reloc_path", "bin", sdk)
        let sep = get_platform_name() == "windows" ? ";" : ":"
        let env <- ["PATH={bin}{sep}{get_env_variable("PATH")}"]
        var out : string
        let rc = run_and_capture(["daslang", probe()], test_temp_dir(), env, out, 300.0)
        t |> equal(rc, 0, out)
        t |> equal(root_of(out), to_generic_path(sdk), out)
    }
}

[test]
def test_root_without_a_sidecar(t : T?) {
    t |> run("a root named outside ASCII works, on Windows outside the code page too") @(t : T?) {
        let own = make_sdk_with_libraries(t, "reloc_ünï_日本")
        copy_into(t, get_host_binary(), path_join(own, "bin/{exe_name("daslang")}"))
        let p = run_daslang([path_join(own, "bin/{exe_name("daslang")}"), probe()], own)
        t |> equal(p.rc, 0, p.out)
        t |> success(ends_with(root_of(p.out), "/reloc_ünï_日本"), "root under the name: {p.out}")
    }
    t |> run("a script path and an argument outside ASCII reach the host as typed") @(t : T?) {
        let own = make_sdk_with_libraries(t, "reloc_argv_日本")
        let daslang = path_join(own, "bin/{exe_name("daslang")}")
        copy_into(t, get_host_binary(), daslang)
        let script = path_join(own, "probe_値.das")
        copy_into(t, probe(), script)
        let p = run_daslang([daslang, script], own)
        t |> equal(p.rc, 0, p.out)
        t |> success(ends_with(root_of(p.out), "/reloc_argv_日本"), "root under the name: {p.out}")
        let r = run_daslang([daslang, "-tool", "echo", "値", "--", "ünï"], own)
        t |> equal(r.rc, 0, r.out)
        t |> equal(args_of(r.out), ["値", "--", "ünï"], r.out)
    }
    t |> run("a blank sidecar is no sidecar: the root is derived from bin/..") @(t : T?) {
        let own = make_sdk_with_libraries(t, "reloc_blank")
        copy_into(t, get_host_binary(), path_join(own, "bin/{exe_name("daslang")}"))
        fwrite(path_join(own, "bin/daslang.root"), " \n\t\n")
        let p = run_daslang([path_join(own, "bin/{exe_name("daslang")}"), probe()], own)
        t |> equal(p.rc, 0, p.out)
        t |> success(ends_with(root_of(p.out), "/reloc_blank"), "root derived from bin/..: {p.out}")
    }
    t |> run("a daslang in bin/<config>/ steps up to its tree, the multi-config layout") @(t : T?) {
        let own = make_sdk_with_libraries(t, "reloc_cfg")
        let exe = path_join(own, "bin/Release/{exe_name("daslang")}")
        copy_into(t, get_host_binary(), exe)
        let p = run_daslang([exe, probe()], own)
        t |> equal(p.rc, 0, p.out)
        t |> success(ends_with(root_of(p.out), "/reloc_cfg"), "root past the config: {p.out}")
    }
}

[test]
def test_symlinked_host(t : T?) {
    if (get_platform_name() == "windows") {
        t |> skip("a package manager links through junctions there, which the exe path resolves")
        return
    }
    t |> run("a symlink to daslang, as brew installs it, runs the real binary's tree") @(t : T?) {
        let bin = path_join(test_temp_dir(), "linked/bin")
        mkdir_rec(bin)
        let link = run_daslang(["ln", "-s", get_host_binary(), path_join(bin, "daslang")])
        t |> equal(link.rc, 0, link.out)
        let p = run_daslang([path_join(bin, "daslang"), probe()], bin)
        t |> equal(p.rc, 0, p.out)
        t |> equal(root_of(p.out), get_das_root(), p.out)
    }
}

[test]
def test_missing_host_library(t : T?) {
    t |> run("no sidecar and no library: every path looked at is named") @(t : T?) {
        let bin = relocate(t, "lost", "bin", "")
        let r = run_daslang([path_join(bin, exe_name("daslang")), probe()], bin)
        t |> equal(r.rc, 1, r.out)
        t |> success(find(r.out, "looked for") >= 0, r.out)
        let named = find(r.out, "lost/lib/") >= 0 || find(r.out, "lost\\lib\\") >= 0
        t |> success(named, "names <root>/lib: {r.out}")
        t |> success(find(r.out, "daslang.root") >= 0, r.out)
    }
    t |> run("a -dasroot after the script's -- belongs to the script, not the stub") @(t : T?) {
        let bin = relocate(t, "lost_after_sep", "bin", "")
        let daslang = path_join(bin, exe_name("daslang"))
        let r = run_daslang([daslang, probe(), "--", "-dasroot", get_das_root()], bin)
        t |> equal(r.rc, 1, r.out)
        t |> success(find(r.out, "looked for") >= 0, r.out)
    }
    t |> run("a trailing -dasroot with no value is ignored, not read past argv") @(t : T?) {
        let bin = relocate(t, "lost_trailing", "bin", "")
        let r = run_daslang([path_join(bin, exe_name("daslang")), "-dasroot"], bin)
        t |> equal(r.rc, 1, r.out)
        t |> success(find(r.out, "looked for") >= 0, r.out)
    }
    t |> run("a library file that does not load is refused with the loader's reason") @(t : T?) {
        let bin = relocate(t, "broken", "bin", "")
        var lib = "libdaslang_main.so"
        if (get_platform_name() == "windows") {
            lib = "daslang_main.dll"
        } elif (get_platform_name() == "darwin") {
            lib = "libdaslang_main.dylib"
        }
        mkdir_rec(path_join(test_temp_dir(), "broken/lib"))
        fwrite(path_join(test_temp_dir(), "broken/lib/{lib}"), "not a library\n")
        let r = run_daslang([path_join(bin, exe_name("daslang")), probe()], bin)
        t |> equal(r.rc, 1, r.out)
        t |> success(find(r.out, "cannot load") >= 0, r.out)
    }
    t |> run("a sidecar naming a root with no libraries is refused the same way") @(t : T?) {
        let empty_root = path_join(test_temp_dir(), "lost_root")
        mkdir_rec(empty_root)
        let bin = relocate(t, "lost_sidecar", "bin", empty_root)
        let r = run_daslang([path_join(bin, exe_name("daslang")), probe()], bin)
        t |> equal(r.rc, 1, r.out)
        t |> success(find(r.out, "lost_root") >= 0, r.out)
    }
    t |> run("a sidecar root without libraries is refused even beside a tree with them") @(t : T?) {
        let own = make_sdk_with_libraries(t, "mixed")
        let empty_root = path_join(test_temp_dir(), "mixed_root")
        mkdir_rec(path_join(empty_root, "daslib"))
        copy_into(t, get_host_binary(), path_join(own, "bin/{exe_name("daslang")}"))
        fwrite(path_join(own, "bin/daslang.root"), empty_root)
        let r = run_daslang([path_join(own, "bin/{exe_name("daslang")}"), probe()], own)
        t |> equal(r.rc, 1, "own library never pairs with the sidecar's daslib: {r.out}")
    }
}
