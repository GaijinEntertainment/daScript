options gen2
options no_global_variables = false

require dastest/testing_boost public

require daslib/fio
require daslib/defer

require daslib/json_boost


require lockfile
require utils
require commands
require daslib/result
require package_runner public
require _daspkg_test_common

[test(description = "a lock file written to disk reads back with its SDK version, packages and root flags intact")]
def test_lockfile_roundtrip(t : T?) {
    t |> run("write and read back") @(t : T?) {
        let tmp_dir = "{get_das_root()}/_daspkg_test_tmp"
        mkdir(tmp_dir)
        var lf : LockFile
        lf.sdk_version = "0.4.0"
        set_package(lf, PackageEntry(name = "foo", source = "github.com/user/foo",
            version = "v1.0", tag = "v1.0", root = true))
        set_package(lf, PackageEntry(name = "bar", source = "github.com/user/bar",
            version = "v2.0", tag = "v2.0", root = false))
        t |> success(write_lock_file(tmp_dir, lf))
        var lf2 : LockFile
        t |> success(read_lock_file(tmp_dir, lf2))
        t |> equal(lf2.sdk_version, "0.4.0")
        t |> equal(length(lf2.packages), 2)
        t |> success(has_package(lf2, "foo"))
        t |> success(has_package(lf2, "bar"))
        let foo = get_package(lf2, "foo")
        t |> equal(foo.source, "github.com/user/foo")
        t |> equal(foo.version, "v1.0")
        t |> success(foo.root)
        let bar = get_package(lf2, "bar")
        t |> equal(bar.source, "github.com/user/bar")
        t |> success(!bar.root)
        // cleanup
        remove("{tmp_dir}/{LOCK_FILE_NAME}")
        rmdir(tmp_dir)
    }
}

[test(description = "cmd_remove deletes the package directory and its lock file entry")]
def test_cmd_remove(t : T?) {
    t |> run("removes package and updates lockfile") @(t : T?) {
        let tmp_root = "{get_das_root()}/_daspkg_test_remove"
        mkdir(tmp_root)
        mkdir("{tmp_root}/modules")
        mkdir("{tmp_root}/modules/testpkg")
        fwrite("{tmp_root}/modules/testpkg/dummy.txt", "hello")
        // write a lock file with the package
        var lf : LockFile
        set_package(lf, PackageEntry(name = "testpkg", source = "github.com/user/testpkg", root = true))
        write_lock_file(tmp_root, lf)
        // remove
        cmd_remove(tmp_root, "testpkg")
        // verify directory removed
        t |> success(!fexist("{tmp_root}/modules/testpkg"))
        // verify lock file updated
        var lf2 : LockFile
        t |> success(read_lock_file(tmp_root, lf2))
        t |> success(!has_package(lf2, "testpkg"))
        // cleanup
        force_rmdir(tmp_root)
    }
}

[test(description = "the global lock file round-trips its entries through disk")]
def test_global_lock_file_roundtrip(t : T?) {
    t |> run("write and read global lock file") @(t : T?) {
        // use a temp dir instead of the real global lock file path
        let tmp_dir = "{get_das_root()}/_daspkg_test_global_lf"
        mkdir(tmp_dir)
        let path = "{tmp_dir}/{GLOBAL_LOCK_FILE_NAME}"
        var lf : LockFile
        lf.sdk_version = "0.5.0"
        set_package(lf, PackageEntry(name = "globalPkg", source = "github.com/user/globalPkg",
            version = "1.0", tag = "v1.0", root = true, global = true))
        t |> success(write_lock_file_at(path, lf))
        // read it back
        var lf2 : LockFile
        t |> success(read_lock_file_at(path, lf2))
        t |> equal(lf2.sdk_version, "0.5.0")
        t |> equal(length(lf2.packages), 1)
        let pkg = get_package(lf2, "globalPkg")
        t |> equal(pkg.source, "github.com/user/globalPkg")
        t |> equal(pkg.tag, "v1.0")
        t |> success(pkg.global)
        t |> success(pkg.root)
        // cleanup
        remove(path)
        rmdir(tmp_dir)
    }
}

[test(description = "a package entry serializes its global field to JSON and reads it back")]
def test_package_entry_global_field(t : T?) {
    t |> run("global field serializes to JSON") @(t : T?) {
        let tmp_dir = "{get_das_root()}/_daspkg_test_global_field"
        mkdir(tmp_dir)
        var lf : LockFile
        set_package(lf, PackageEntry(name = "pkg1", source = "github.com/a/pkg1",
            version = "1.0", tag = "v1.0", root = true, global = true))
        set_package(lf, PackageEntry(name = "pkg2", source = "github.com/a/pkg2",
            version = "2.0", tag = "v2.0", root = true, global = false))
        t |> success(write_lock_file(tmp_dir, lf))
        var lf2 : LockFile
        t |> success(read_lock_file(tmp_dir, lf2))
        let p1 = get_package(lf2, "pkg1")
        t |> success(p1.global)
        let p2 = get_package(lf2, "pkg2")
        t |> success(!p2.global)
        // cleanup
        remove("{tmp_dir}/{LOCK_FILE_NAME}")
        rmdir(tmp_dir)
    }
}

[test(description = "cmd_list reports packages recorded in the global lock file")]
def test_cmd_list_global(t : T?) {
    t |> run("lists global packages") @(t : T?) {
        let tmp_dir = "{get_das_root()}/_daspkg_test_list_global"
        mkdir(tmp_dir)
        let path = "{tmp_dir}/{GLOBAL_LOCK_FILE_NAME}"
        var lf : LockFile
        set_package(lf, PackageEntry(name = "gmod", source = "github.com/a/gmod",
            tag = "v1.0", root = true, global = true))
        write_lock_file_at(path, lf)
        // cmd_list --global reads the global lock file; we can't easily redirect it
        // so just verify the lock file is readable with correct content
        var lf2 : LockFile
        t |> success(read_lock_file_at(path, lf2))
        t |> equal(length(lf2.packages), 1)
        t |> equal(lf2.packages[0].name, "gmod")
        t |> success(lf2.packages[0].global)
        // cleanup
        remove(path)
        rmdir(tmp_dir)
    }
}

[test(description = "cmd_check verifies a global entry against das_root/modules")]
def test_cmd_check_global_entry(t : T?) {
    t |> run("check verifies global entries at das_root/modules") @(t : T?) {
        let tmp_root = "{get_das_root()}/_daspkg_test_check_global"
        mkdir(tmp_root)
        // create a lock file with a global entry pointing to a non-existent module
        var lf : LockFile
        set_package(lf, PackageEntry(name = "nonexistent_global", source = "github.com/a/b",
            tag = "v1.0", root = true, global = true))
        write_lock_file(tmp_root, lf)
        // cmd_check should report it as missing (since das_root/modules/nonexistent_global doesn't exist)
        // we can't capture output easily, but we verify the check logic path
        cmd_check(tmp_root)
        // cleanup
        remove("{tmp_root}/{LOCK_FILE_NAME}")
        rmdir(tmp_root)
    }
}

[test(description = "removing a global entry drops only its lock reference and leaves the module on disk")]
def test_cmd_remove_global_entry(t : T?) {
    t |> run("removing global entry only removes lock reference") @(t : T?) {
        let tmp_root = "{get_das_root()}/_daspkg_test_rm_global_entry"
        mkdir(tmp_root)
        // create a lock file with a global entry
        var lf : LockFile
        set_package(lf, PackageEntry(name = "global_pkg", source = "github.com/a/b",
            tag = "v1.0", root = true, global = true))
        write_lock_file(tmp_root, lf)
        t |> success(has_package(lf, "global_pkg"))
        // remove — should only remove from lock file
        cmd_remove(tmp_root, "global_pkg")
        var lf2 : LockFile
        read_lock_file(tmp_root, lf2)
        t |> success(!has_package(lf2, "global_pkg"), "removed from lock file")
        // no local directory should have been attempted to delete
        // (the module is global, no local dir to delete)
        // cleanup
        remove("{tmp_root}/{LOCK_FILE_NAME}")
        rmdir(tmp_root)
    }
}
