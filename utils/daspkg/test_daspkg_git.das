options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_global_variables = false
options persistent_heap

require dastest/testing_boost public

require fio
require strings

require lockfile
require utils
require index
require commands

[test]
def test_install_git_basic(t : T?) {
    t |> run("clones a package from github") @(t : T?) {
        let tmp_root = "{get_das_root()}/_daspkg_test_git"
        mkdir(tmp_root)
        var lf : LockFile
        install_git(tmp_root, "daspkg-test-pure", "github.com/borisbat/daspkg-test-pure", "", lf)
        // verify lock file entry
        t |> success(lf.packages |> key_exists("daspkg-test-pure"))
        unsafe {
            t |> equal(lf.packages["daspkg-test-pure"].source, "github.com/borisbat/daspkg-test-pure")
            t |> success(lf.packages["daspkg-test-pure"].root)
        }
        // verify files exist
        t |> success(fexist("{tmp_root}/modules/daspkg-test-pure/daspkg.json"))
        t |> success(fexist("{tmp_root}/modules/daspkg-test-pure/daslib/test_pure.das"))
        // cleanup
        rmdir_rec(tmp_root)
    }
}

[test]
def test_install_git_version(t : T?) {
    t |> run("checks out specific tag") @(t : T?) {
        let tmp_root = "{get_das_root()}/_daspkg_test_ver"
        mkdir(tmp_root)
        var lf : LockFile
        install_git(tmp_root, "daspkg-test-versions", "github.com/borisbat/daspkg-test-versions", "v1.0", lf)
        t |> success(lf.packages |> key_exists("daspkg-test-versions"))
        unsafe {
            t |> equal(lf.packages["daspkg-test-versions"].tag, "v1.0")
        }
        // v1.0 should have the module file
        t |> success(fexist("{tmp_root}/modules/daspkg-test-versions/daslib/test_versions.das"))
        // cleanup
        rmdir_rec(tmp_root)
    }
}

[test]
def test_install_with_dependencies(t : T?) {
    t |> run("installs transitive dependencies") @(t : T?) {
        let tmp_root = "{get_das_root()}/_daspkg_test_deps"
        mkdir(tmp_root)
        var lf : LockFile
        // daspkg-test-deps depends on daspkg-test-pure
        install_git(tmp_root, "daspkg-test-deps", "github.com/borisbat/daspkg-test-deps", "", lf)
        // both packages should be installed
        t |> success(lf.packages |> key_exists("daspkg-test-deps"), "main package installed")
        t |> success(lf.packages |> key_exists("daspkg-test-pure"), "dependency installed")
        // dependency should be marked as non-root
        unsafe {
            t |> success(lf.packages["daspkg-test-deps"].root, "main is root")
            t |> success(!lf.packages["daspkg-test-pure"].root, "dep is not root")
        }
        // verify both module dirs exist
        t |> success(fexist("{tmp_root}/modules/daspkg-test-deps/daslib/test_deps.das"))
        t |> success(fexist("{tmp_root}/modules/daspkg-test-pure/daslib/test_pure.das"))
        // cleanup
        rmdir_rec(tmp_root)
    }
}

[test]
def test_fetch_index(t : T?) {
    t |> run("fetches and parses package index") @(t : T?) {
        let tmp_root = "{get_das_root()}/_daspkg_test_idx"
        mkdir(tmp_root)
        var index : table<string; IndexEntry>
        t |> success(fetch_index(tmp_root, index), "fetch succeeded")
        t |> success(length(index) > 0, "index has entries")
        t |> success(index |> key_exists("daspkg-test-pure"), "test-pure in index")
        // cleanup
        rmdir_rec(tmp_root)
    }
}

[test]
def test_cmd_install_by_name(t : T?) {
    t |> run("installs from index by bare name") @(t : T?) {
        let tmp_root = "{get_das_root()}/_daspkg_test_byname"
        mkdir(tmp_root)
        cmd_install(tmp_root, "daspkg-test-pure", false)
        // verify installed
        var lf : LockFile
        t |> success(read_lock_file(tmp_root, lf))
        t |> success(lf.packages |> key_exists("daspkg-test-pure"))
        t |> success(fexist("{tmp_root}/modules/daspkg-test-pure/daslib/test_pure.das"))
        // cleanup
        rmdir_rec(tmp_root)
    }
}

[test]
def test_cmd_update_single(t : T?) {
    t |> run("updates a single git package") @(t : T?) {
        let tmp_root = "{get_das_root()}/_daspkg_test_upd"
        mkdir(tmp_root)
        // install first
        var lf : LockFile
        install_git(tmp_root, "daspkg-test-pure", "github.com/borisbat/daspkg-test-pure", "", lf)
        t |> success(lf.packages |> key_exists("daspkg-test-pure"))
        // update it
        cmd_update(tmp_root, "daspkg-test-pure")
        // verify still present after update
        t |> success(fexist("{tmp_root}/modules/daspkg-test-pure/daslib/test_pure.das"))
        // cleanup
        rmdir_rec(tmp_root)
    }
}

[test]
def test_cmd_update_skips_local(t : T?) {
    t |> run("skips local packages") @(t : T?) {
        let tmp_root = "{get_das_root()}/_daspkg_test_upd_local"
        let src_dir = "{get_das_root()}/_daspkg_test_upd_src"
        mkdir(tmp_root)
        mkdir(src_dir)
        // create a dummy source
        fopen("{src_dir}/hello.das", "wb") <| $(f) {
            fwrite(f, "// hello\n")
        }
        // install locally
        var lf : LockFile
        install_local(tmp_root, "localpkg", src_dir, lf)
        t |> success(lf.packages |> key_exists("localpkg"))
        // update should skip without error
        cmd_update(tmp_root, "localpkg")
        t |> success(fexist("{tmp_root}/modules/localpkg/hello.das"))
        // cleanup
        rmdir_rec(tmp_root)
        rmdir_rec(src_dir)
    }
}

[test]
def test_cmd_update_nonexistent(t : T?) {
    t |> run("error on nonexistent package") @(t : T?) {
        let tmp_root = "{get_das_root()}/_daspkg_test_upd_noex"
        mkdir(tmp_root)
        var lf = LockFile()
        write_lock_file(tmp_root, lf)
        // should print error, not crash
        cmd_update(tmp_root, "nonexistent")
        t |> success(true)
        // cleanup
        rmdir_rec(tmp_root)
    }
}

[test]
def test_cmd_update_all_integration(t : T?) {
    t |> run("updates all installed packages") @(t : T?) {
        let tmp_root = "{get_das_root()}/_daspkg_test_upd_all"
        mkdir(tmp_root)
        // install two packages
        var lf : LockFile
        install_git(tmp_root, "daspkg-test-pure", "github.com/borisbat/daspkg-test-pure", "", lf)
        install_git(tmp_root, "daspkg-test-deps", "github.com/borisbat/daspkg-test-deps", "", lf)
        write_lock_file(tmp_root, lf)
        // update all
        cmd_update_all(tmp_root)
        // both should still be present
        t |> success(fexist("{tmp_root}/modules/daspkg-test-pure/daslib/test_pure.das"))
        t |> success(fexist("{tmp_root}/modules/daspkg-test-deps/daslib/test_deps.das"))
        // cleanup
        rmdir_rec(tmp_root)
    }
}

[test]
def test_cmd_install_all_integration(t : T?) {
    t |> run("installs all dependencies from daspkg.json") @(t : T?) {
        let tmp_root = "{get_das_root()}/_daspkg_test_inst_all"
        mkdir(tmp_root)
        // write a daspkg.json with two deps
        fopen("{tmp_root}/daspkg.json", "wb") <| $(f) {
            fwrite(f, "\{\"name\":\"test\",\"description\":\"test\",\"dependencies\":\{\"daspkg-test-pure\":\"github.com/borisbat/daspkg-test-pure\"\}\}\n")
        }
        cmd_install_all(tmp_root, false)
        // verify installed
        var lf : LockFile
        t |> success(read_lock_file(tmp_root, lf))
        t |> success(lf.packages |> key_exists("daspkg-test-pure"))
        t |> success(fexist("{tmp_root}/modules/daspkg-test-pure/daslib/test_pure.das"))
        // cleanup
        rmdir_rec(tmp_root)
    }
}

[test]
def test_cmd_check_integration(t : T?) {
    t |> run("check reports OK for installed packages") @(t : T?) {
        let tmp_root = "{get_das_root()}/_daspkg_test_chk"
        mkdir(tmp_root)
        // install a package with .das_module
        var lf : LockFile
        install_git(tmp_root, "daspkg-test-pure", "github.com/borisbat/daspkg-test-pure", "", lf)
        // check should report OK
        cmd_check(tmp_root)
        t |> success(fexist("{tmp_root}/modules/daspkg-test-pure/.das_module"))
        // cleanup
        rmdir_rec(tmp_root)
    }
}

[test]
def test_cmd_install_already_installed(t : T?) {
    t |> run("skip if already installed without force") @(t : T?) {
        let tmp_root = "{get_das_root()}/_daspkg_test_skip"
        mkdir(tmp_root)
        // install once
        cmd_install(tmp_root, "github.com/borisbat/daspkg-test-pure", false)
        var lf : LockFile
        t |> success(read_lock_file(tmp_root, lf))
        t |> success(lf.packages |> key_exists("daspkg-test-pure"))
        // install again without force — should skip
        cmd_install(tmp_root, "github.com/borisbat/daspkg-test-pure", false)
        t |> success(fexist("{tmp_root}/modules/daspkg-test-pure/daslib/test_pure.das"))
        // cleanup
        rmdir_rec(tmp_root)
    }
}
