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
def test_read_pkg_dependencies(t : T?) {
    t |> run("parses daspkg.json dependencies") @(t : T?) {
        let tmp_dir = "{get_das_root()}/_daspkg_test_tmp"
        mkdir(tmp_dir)
        let manifest = "{tmp_dir}/daspkg.json"
        fwrite(manifest, "\{\"name\":\"test\",\"dependencies\":\{\"foo\":\"github.com/user/foo\",\"bar\":\"github.com/user/bar\"\}\}")
        var deps : table<string; string>
        t |> success(read_pkg_dependencies(tmp_dir, deps))
        t |> equal(length(deps), 2)
        t |> success(deps |> key_exists("foo"))
        t |> success(deps |> key_exists("bar"))
        unsafe {
            t |> equal(deps["foo"], "github.com/user/foo")
            t |> equal(deps["bar"], "github.com/user/bar")
        }
        // cleanup
        remove(manifest)
        rmdir(tmp_dir)
    }
    t |> run("no manifest returns false") @(t : T?) {
        var deps : table<string; string>
        t |> success(!read_pkg_dependencies("{get_das_root()}/_daspkg_nonexistent", deps))
    }
    t |> run("no dependencies field returns false") @(t : T?) {
        let tmp_dir = "{get_das_root()}/_daspkg_test_tmp2"
        mkdir(tmp_dir)
        fwrite("{tmp_dir}/daspkg.json", "\{\"name\":\"test\"\}")
        var deps : table<string; string>
        t |> success(!read_pkg_dependencies(tmp_dir, deps))
        remove("{tmp_dir}/daspkg.json")
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
    t |> run("valid manifest") @(t : T?) {
        let tmp_dir = "{get_das_root()}/_daspkg_test_manifest"
        mkdir(tmp_dir)
        fwrite("{tmp_dir}/daspkg.json", "\{\"name\":\"mypkg\",\"description\":\"A test package\"\}")
        var manifest : PackageManifest
        t |> success(read_manifest(tmp_dir, manifest))
        t |> equal(manifest.name, "mypkg")
        t |> equal(manifest.description, "A test package")
        remove("{tmp_dir}/daspkg.json")
        rmdir(tmp_dir)
    }
    t |> run("missing name") @(t : T?) {
        let tmp_dir = "{get_das_root()}/_daspkg_test_manifest2"
        mkdir(tmp_dir)
        fwrite("{tmp_dir}/daspkg.json", "\{\"description\":\"no name\"\}")
        var manifest : PackageManifest
        t |> success(!read_manifest(tmp_dir, manifest))
        remove("{tmp_dir}/daspkg.json")
        rmdir(tmp_dir)
    }
    t |> run("missing description") @(t : T?) {
        let tmp_dir = "{get_das_root()}/_daspkg_test_manifest3"
        mkdir(tmp_dir)
        fwrite("{tmp_dir}/daspkg.json", "\{\"name\":\"mypkg\"\}")
        var manifest : PackageManifest
        t |> success(!read_manifest(tmp_dir, manifest))
        remove("{tmp_dir}/daspkg.json")
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
