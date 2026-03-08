options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_global_variables = false

require dastest/testing_boost public

require fio
require strings

require daslib/json
require daslib/json_boost

require lockfile
require utils
require index
require commands
require package_runner public

[test]
def test_package_name_from_source(t : T?) {
    t |> run("github URL") @(t : T?) {
        t |> equal(package_name_from_source("github.com/user/dasImgui"), "dasImgui")
    }
    t |> run("nested path") @(t : T?) {
        t |> equal(package_name_from_source("github.com/org/sub/mymod"), "mymod")
    }
    t |> run("local absolute path") @(t : T?) {
        t |> equal(package_name_from_source("/home/user/mymod"), "mymod")
    }
    t |> run("bare name") @(t : T?) {
        t |> equal(package_name_from_source("dasImgui"), "dasImgui")
    }
    t |> run("trailing slash stripped") @(t : T?) {
        t |> equal(package_name_from_source("github.com/user/repo/"), "")
    }
    t |> run("windows backslash path") @(t : T?) {
        t |> equal(package_name_from_source("C:\\Users\\mymod"), "mymod")
    }
    t |> run("mixed separators") @(t : T?) {
        t |> equal(package_name_from_source("C:/Users\\mymod"), "mymod")
    }
}

[test]
def test_is_local_path(t : T?) {
    t |> run("absolute unix path") @(t : T?) {
        t |> success(is_local_path("/home/user/mod"))
    }
    t |> run("relative dot path") @(t : T?) {
        t |> success(is_local_path("./mymod"))
    }
    t |> run("relative parent path") @(t : T?) {
        t |> success(is_local_path("../mymod"))
    }
    t |> run("windows drive letter") @(t : T?) {
        t |> success(is_local_path("C:/Users/mod"))
    }
    t |> run("github URL is not local") @(t : T?) {
        t |> success(!is_local_path("github.com/user/repo"))
    }
    t |> run("bare name is not local") @(t : T?) {
        t |> success(!is_local_path("dasImgui"))
    }
    t |> run("windows backslash dot path") @(t : T?) {
        t |> success(is_local_path(".\\mymod"))
    }
    t |> run("windows backslash parent path") @(t : T?) {
        t |> success(is_local_path("..\\mymod"))
    }
}

[test]
def test_is_index_name(t : T?) {
    t |> run("bare name is index name") @(t : T?) {
        t |> success(is_index_name("dasImgui"))
    }
    t |> run("github URL is not index name") @(t : T?) {
        t |> success(!is_index_name("github.com/user/repo"))
    }
    t |> run("local path is not index name") @(t : T?) {
        t |> success(!is_index_name("./mymod"))
    }
    t |> run("backslash path is not index name") @(t : T?) {
        t |> success(!is_index_name("path\\to\\mod"))
    }
}

[test]
def test_parse_args(t : T?) {
    t |> run("install command") @(t : T?) {
        let parsed = parse_args(["daslang.exe", "main.das", "--", "install", "dasImgui"])
        t |> equal(parsed.command, "install")
        t |> equal(parsed.command_arg, "dasImgui")
        t |> success(!parsed.force)
    }
    t |> run("install with force") @(t : T?) {
        let parsed = parse_args(["daslang.exe", "main.das", "--", "install", "dasImgui", "--force"])
        t |> equal(parsed.command, "install")
        t |> equal(parsed.command_arg, "dasImgui")
        t |> success(parsed.force)
    }
    t |> run("install with root") @(t : T?) {
        let parsed = parse_args(["daslang.exe", "main.das", "--", "--root", "/tmp/proj", "install", "foo"])
        t |> equal(parsed.command, "install")
        t |> equal(parsed.command_arg, "foo")
        t |> equal(parsed.root, "/tmp/proj")
    }
    t |> run("list command no arg") @(t : T?) {
        let parsed = parse_args(["daslang.exe", "main.das", "--", "list"])
        t |> equal(parsed.command, "list")
        t |> success(empty(parsed.command_arg))
    }
    t |> run("no command") @(t : T?) {
        let parsed = parse_args(["daslang.exe", "main.das", "--"])
        t |> success(empty(parsed.command))
    }
    t |> run("default root is dot") @(t : T?) {
        let parsed = parse_args(["daslang.exe", "main.das", "--", "list"])
        t |> equal(parsed.root, ".")
    }
}

[test]
def test_lockfile_roundtrip(t : T?) {
    t |> run("write and read back") @(t : T?) {
        let tmp_dir = "{get_das_root()}/_daspkg_test_tmp"
        mkdir(tmp_dir)
        var lf : LockFile
        lf.sdk_version = "0.4.0"
        var entry = PackageEntry()
        entry.source = "github.com/user/foo"
        entry.version = "v1.0"
        entry.tag = "v1.0"
        entry.root = true
        lf.packages |> insert("foo", entry)
        var dep = PackageEntry()
        dep.source = "github.com/user/bar"
        dep.version = "v2.0"
        dep.tag = "v2.0"
        dep.root = false
        lf.packages |> insert("bar", dep)
        t |> success(write_lock_file(tmp_dir, lf))
        var lf2 : LockFile
        t |> success(read_lock_file(tmp_dir, lf2))
        t |> equal(lf2.sdk_version, "0.4.0")
        t |> equal(length(lf2.packages), 2)
        t |> success(lf2.packages |> key_exists("foo"))
        t |> success(lf2.packages |> key_exists("bar"))
        unsafe {
            t |> equal(lf2.packages["foo"].source, "github.com/user/foo")
            t |> equal(lf2.packages["foo"].version, "v1.0")
            t |> success(lf2.packages["foo"].root)
            t |> equal(lf2.packages["bar"].source, "github.com/user/bar")
            t |> success(!lf2.packages["bar"].root)
        }
        // cleanup
        remove("{tmp_dir}/{LOCK_FILE_NAME}")
        rmdir(tmp_dir)
    }
}

[test]
def test_install_local(t : T?) {
    t |> run("installs and records in lockfile") @(t : T?) {
        let tmp_root = "{get_das_root()}/_daspkg_test_root"
        let tmp_src = "{get_das_root()}/_daspkg_test_src"
        mkdir(tmp_root)
        mkdir(tmp_src)
        // create a fake package source
        mkdir("{tmp_src}/daslib")
        fwrite("{tmp_src}/daslib/hello.das", "module hello\n")
        var lf : LockFile
        install_local(tmp_root, "hello", tmp_src, lf)
        // verify lock file entry
        t |> success(lf.packages |> key_exists("hello"))
        unsafe {
            t |> success(lf.packages["hello"].local)
            t |> success(lf.packages["hello"].root)
        }
        // verify files copied
        t |> success(fexist("{tmp_root}/modules/hello/daslib/hello.das"))
        // verify lock file was written to disk
        t |> success(fexist("{tmp_root}/{LOCK_FILE_NAME}"))
        // cleanup
        rmdir_rec(tmp_root)
        rmdir_rec(tmp_src)
    }
}

[test]
def test_cmd_remove(t : T?) {
    t |> run("removes package and updates lockfile") @(t : T?) {
        let tmp_root = "{get_das_root()}/_daspkg_test_remove"
        mkdir(tmp_root)
        mkdir("{tmp_root}/modules")
        mkdir("{tmp_root}/modules/testpkg")
        fwrite("{tmp_root}/modules/testpkg/dummy.txt", "hello")
        // write a lock file with the package
        var lf : LockFile
        var entry = PackageEntry()
        entry.source = "github.com/user/testpkg"
        entry.root = true
        lf.packages |> insert("testpkg", entry)
        write_lock_file(tmp_root, lf)
        // remove
        cmd_remove(tmp_root, "testpkg")
        // verify directory removed
        t |> success(!fexist("{tmp_root}/modules/testpkg"))
        // verify lock file updated
        var lf2 : LockFile
        t |> success(read_lock_file(tmp_root, lf2))
        t |> success(!lf2.packages |> key_exists("testpkg"))
        // cleanup
        rmdir_rec(tmp_root)
    }
}

[test]
def test_read_manifest(t : T?) {
    t |> run("valid manifest from .das_package") @(t : T?) {
        let tmp_dir = "{get_das_root()}/_daspkg_test_manifest"
        mkdir(tmp_dir)
        fwrite("{tmp_dir}/.das_package", "options gen2\nrequire daslib/daspkg\n[export]\ndef package() \{\n    package_name(\"mypkg\")\n    package_description(\"A test package\")\n\}\n")
        var manifest : PackageManifest
        t |> success(read_manifest(tmp_dir, manifest))
        t |> equal(manifest.name, "mypkg")
        t |> equal(manifest.description, "A test package")
        remove("{tmp_dir}/.das_package")
        rmdir(tmp_dir)
    }
    t |> run("missing name") @(t : T?) {
        let tmp_dir = "{get_das_root()}/_daspkg_test_manifest2"
        mkdir(tmp_dir)
        fwrite("{tmp_dir}/.das_package", "options gen2\nrequire daslib/daspkg\n[export]\ndef package() \{\n    package_description(\"no name\")\n\}\n")
        var manifest : PackageManifest
        t |> success(!read_manifest(tmp_dir, manifest))
        remove("{tmp_dir}/.das_package")
        rmdir(tmp_dir)
    }
    t |> run("missing description") @(t : T?) {
        let tmp_dir = "{get_das_root()}/_daspkg_test_manifest3"
        mkdir(tmp_dir)
        fwrite("{tmp_dir}/.das_package", "options gen2\nrequire daslib/daspkg\n[export]\ndef package() \{\n    package_name(\"mypkg\")\n\}\n")
        var manifest : PackageManifest
        t |> success(!read_manifest(tmp_dir, manifest))
        remove("{tmp_dir}/.das_package")
        rmdir(tmp_dir)
    }
    t |> run("no manifest file") @(t : T?) {
        var manifest : PackageManifest
        t |> success(!read_manifest("{get_das_root()}/_daspkg_nonexistent", manifest))
    }
}

[test]
def test_add_to_index_json(t : T?) {
    t |> run("adds new package") @(t : T?) {
        let result = add_to_index_json("[]", "mypkg", "github.com/user/mypkg", "A cool package")
        t |> success(!empty(result))
        var error : string
        var js = read_json(result, error)
        t |> success(js != null)
        unsafe {
            t |> equal(length(js.value as _array), 1)
            t |> equal((js.value as _array)[0]?.name ?? "", "mypkg")
            t |> equal((js.value as _array)[0]?.url ?? "", "github.com/user/mypkg")
            t |> equal((js.value as _array)[0]?.description ?? "", "A cool package")
        }
    }
    t |> run("preserves existing and sorts") @(t : T?) {
        let input = add_to_index_json("[]", "pkg2", "github.com/user/pkg2", "Second")
        let result = add_to_index_json(input, "pkg1", "github.com/user/pkg1", "First")
        var error : string
        var js = read_json(result, error)
        t |> success(js != null)
        unsafe {
            t |> equal(length(js.value as _array), 2)
            // pkg1 should come before pkg2 (sorted)
            t |> equal((js.value as _array)[0]?.name ?? "", "pkg1")
            t |> equal((js.value as _array)[1]?.name ?? "", "pkg2")
        }
    }
    t |> run("invalid JSON returns empty") @(t : T?) {
        let result = add_to_index_json("not json", "pkg", "url", "desc")
        t |> success(empty(result))
    }
}

[test]
def test_remove_from_index_json(t : T?) {
    t |> run("removes existing package") @(t : T?) {
        let input = add_to_index_json("[]", "mypkg", "github.com/user/mypkg", "desc")
        let result = remove_from_index_json(input, "mypkg")
        t |> success(!empty(result))
        var error : string
        var js = read_json(result, error)
        t |> success(js != null)
        unsafe {
            t |> equal(length(js.value as _array), 0)
        }
    }
    t |> run("preserves other entries") @(t : T?) {
        var input = add_to_index_json("[]", "pkg1", "github.com/user/pkg1", "First")
        input = add_to_index_json(input, "pkg2", "github.com/user/pkg2", "Second")
        let result = remove_from_index_json(input, "pkg1")
        var error : string
        var js = read_json(result, error)
        t |> success(js != null)
        unsafe {
            t |> equal(length(js.value as _array), 1)
            t |> equal((js.value as _array)[0]?.name ?? "", "pkg2")
        }
    }
    t |> run("removing nonexistent is no-op") @(t : T?) {
        let input = add_to_index_json("[]", "pkg1", "github.com/user/pkg1", "desc")
        let result = remove_from_index_json(input, "nonexistent")
        var error : string
        var js = read_json(result, error)
        t |> success(js != null)
        unsafe {
            t |> equal(length(js.value as _array), 1)
            t |> equal((js.value as _array)[0]?.name ?? "", "pkg1")
        }
    }
    t |> run("invalid JSON returns empty") @(t : T?) {
        let result = remove_from_index_json("bad", "pkg")
        t |> success(empty(result))
    }
}

[test]
def test_cmd_check(t : T?) {
    let test_root = get_full_file_name("_daspkg_test_check")
    let modules_dir = "{test_root}/modules"
    let pkg_dir = "{modules_dir}/mypkg"

    // setup: create module dir with .das_module
    mkdir(test_root)
    mkdir(modules_dir)
    mkdir(pkg_dir)
    fopen("{pkg_dir}/.das_module", "wb") <| $(f) {
        fwrite(f, "options gen2\n")
    }

    // write lockfile with two entries: one present, one missing
    var lf = LockFile()
    var e1 = PackageEntry()
    e1.source = "github.com/user/mypkg"
    lf.packages |> insert("mypkg", e1)
    var e2 = PackageEntry()
    e2.source = "github.com/user/ghost"
    lf.packages |> insert("ghost", e2)
    write_lock_file(test_root, lf)

    t |> run("detects present and missing packages") @(t : T?) {
        // cmd_check prints output — just verify it doesn't crash
        cmd_check(test_root)
        // verify the package dir exists
        t |> success(fexist(pkg_dir))
        // verify the ghost dir does NOT exist
        t |> success(!fexist("{modules_dir}/ghost"))
    }

    t |> run("warns about missing .das_module") @(t : T?) {
        let pkg2_dir = "{modules_dir}/nopkg"
        mkdir(pkg2_dir)
        var lf2 = LockFile()
        var e = PackageEntry()
        e.source = "github.com/user/nopkg"
        lf2.packages |> insert("nopkg", e)
        write_lock_file(test_root, lf2)
        cmd_check(test_root)  // should print WARNING, not crash
        t |> success(fexist(pkg2_dir))
        rmdir_rec(pkg2_dir)
    }

    // cleanup
    rmdir_rec(test_root)
}

[test]
def test_cmd_check_no_lockfile(t : T?) {
    t |> run("no lockfile prints message") @(t : T?) {
        cmd_check("_daspkg_nonexistent_dir")
        t |> success(true)  // just verify no crash
    }
}

[test]
def test_check_tool(t : T?) {
    t |> run("detects git") @(t : T?) {
        t |> success(check_tool("git", "git --version"))
    }
    t |> run("detects cmake") @(t : T?) {
        t |> success(check_tool("cmake", "cmake --version"))
    }
    t |> run("fails on nonexistent tool") @(t : T?) {
        t |> success(!check_tool("nonexistent", "nonexistent_tool_xyz --version"))
    }
}

[test]
def test_cmd_doctor(t : T?) {
    t |> run("runs without crash") @(t : T?) {
        cmd_doctor()
        t |> success(true)
    }
}

// --- package_runner tests ---

let PKG_FIXTURE = "utils/daspkg/fixtures/test.das_package"
let PKG_BRANCH_FIXTURE = "utils/daspkg/fixtures/test_branch.das_package"
let PKG_REDIRECT_FIXTURE = "utils/daspkg/fixtures/test_redirect.das_package"
let PKG_SDK_AWARE_FIXTURE = "utils/daspkg/fixtures/test_sdk_aware.das_package"

[test]
def test_run_das_package_meta(t : T?) {
    t |> run("reads package metadata") @(t : T?) {
        var meta : PackageMeta
        let ok = run_das_package_meta(PKG_FIXTURE, meta)
        t |> success(ok)
        t |> equal("test-pkg", meta.pkg_name)
        t |> equal("Test Author", meta.author)
        t |> equal("A test package", meta.description)
        t |> equal("https://github.com/test/test-pkg", meta.source)
    }
}

[test]
def test_run_das_package_resolve_tag(t : T?) {
    t |> run("resolves tag version") @(t : T?) {
        var res : ResolveResult
        let ok = run_das_package_resolve(PKG_FIXTURE, "0.5", "1.2.3", res)
        t |> success(ok)
        t |> equal("v1.2.3", res.tag)
        t |> equal("", res.branch)
    }
}

[test]
def test_run_das_package_resolve_branch(t : T?) {
    t |> run("resolves branch download") @(t : T?) {
        var res : ResolveResult
        let ok = run_das_package_resolve(PKG_BRANCH_FIXTURE, "0.5", "1.0", res)
        t |> success(ok)
        t |> equal("", res.tag)
        t |> equal("main", res.branch)
    }
}

[test]
def test_run_das_package_deps(t : T?) {
    t |> run("reads dependencies") @(t : T?) {
        var deps : array<PackageDependency>
        let ok = run_das_package_deps(PKG_FIXTURE, "1.0", deps)
        t |> success(ok)
        t |> equal(2, length(deps))
        t |> equal("https://github.com/test/dep-a", deps[0].source)
        t |> equal(">=1.0", deps[0].version_constraint)
        t |> equal("https://github.com/test/dep-b", deps[1].source)
        t |> equal("", deps[1].version_constraint)
    }
}

[test]
def test_run_das_package_build(t : T?) {
    t |> run("reads cmake build info") @(tt : T?) {
        var info : PackageBuildInfo
        let ok = run_das_package_build(PKG_FIXTURE, info)
        tt |> success(ok)
        tt |> equal(true, info.is_cmake)
    }
}

[test]
def test_run_das_package_missing_file(t : T?) {
    t |> run("returns false for missing file") @(t : T?) {
        var meta : PackageMeta
        let ok = run_das_package_meta("nonexistent.das_package", meta)
        t |> equal(false, ok)
    }
}

[test]
def test_run_das_package_optional_functions(t : T?) {
    t |> run("succeeds when function is missing") @(tt : T?) {
        // test_branch.das_package has no build() function — should still succeed
        var info : PackageBuildInfo
        let ok = run_das_package_build(PKG_BRANCH_FIXTURE, info)
        tt |> success(ok)
        tt |> equal(false, info.is_cmake)
        tt |> equal(false, info.is_custom)
    }
}

[test]
def test_run_das_package_resolve_redirect(t : T?) {
    t |> run("resolves redirect to different repo") @(t : T?) {
        var res : ResolveResult
        let ok = run_das_package_resolve(PKG_REDIRECT_FIXTURE, "", "", res)
        t |> success(ok)
        t |> equal("github.com/borisbat/daspkg-test-pure", res.source)
        t |> equal("v1.0", res.tag)
        t |> equal("", res.branch)
    }
}

[test]
def test_run_das_package_resolve_sdk_aware(t : T?) {
    t |> run("resolves latest to default tag") @(t : T?) {
        var res : ResolveResult
        let ok = run_das_package_resolve(PKG_SDK_AWARE_FIXTURE, "0.5", "", res)
        t |> success(ok)
        t |> equal("v2.0", res.tag)
    }
    t |> run("resolves 1.x to v1.5") @(t : T?) {
        var res : ResolveResult
        let ok = run_das_package_resolve(PKG_SDK_AWARE_FIXTURE, "0.5", "1.x", res)
        t |> success(ok)
        t |> equal("v1.5", res.tag)
    }
    t |> run("resolves explicit version") @(t : T?) {
        var res : ResolveResult
        let ok = run_das_package_resolve(PKG_SDK_AWARE_FIXTURE, "0.5", "3.0", res)
        t |> success(ok)
        t |> equal("v3.0", res.tag)
    }
}
