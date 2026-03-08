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
        t |> success(fexist("{tmp_root}/modules/daspkg-test-pure/.das_package"))
        t |> success(fexist("{tmp_root}/modules/daspkg-test-pure/daslib/test_pure.das"))
        // cleanup
        rmdir_rec(tmp_root)
    }
}

[test]
def test_install_git_version_v1(t : T?) {
    t |> run("installs v1.0 via resolve — no new_in_v2") @(t : T?) {
        let tmp_root = "{get_das_root()}/_daspkg_test_v1"
        mkdir(tmp_root)
        var lf : LockFile
        install_git(tmp_root, "daspkg-test-versions", "github.com/borisbat/daspkg-test-versions", "1.0", lf)
        t |> success(lf.packages |> key_exists("daspkg-test-versions"))
        unsafe {
            t |> equal(lf.packages["daspkg-test-versions"].tag, "v1.0")
        }
        // v1.0 has get_version returning "1.0" and no new_in_v2
        let src = fread("{tmp_root}/modules/daspkg-test-versions/daslib/test_versions.das")
        t |> success(find(src, "return \"1.0\"") >= 0)
        t |> success(find(src, "new_in_v2") < 0)
        rmdir_rec(tmp_root)
    }
}

[test]
def test_install_git_version_v2(t : T?) {
    t |> run("installs v2.0 via resolve — has new_in_v2") @(t : T?) {
        let tmp_root = "{get_das_root()}/_daspkg_test_v2"
        mkdir(tmp_root)
        var lf : LockFile
        install_git(tmp_root, "daspkg-test-versions", "github.com/borisbat/daspkg-test-versions", "2.0", lf)
        t |> success(lf.packages |> key_exists("daspkg-test-versions"))
        unsafe {
            t |> equal(lf.packages["daspkg-test-versions"].tag, "v2.0")
        }
        // v2.0 has get_version returning "2.0" and new_in_v2
        let src = fread("{tmp_root}/modules/daspkg-test-versions/daslib/test_versions.das")
        t |> success(find(src, "return \"2.0\"") >= 0)
        t |> success(find(src, "new_in_v2") >= 0)
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
def test_cmd_update_recopies_local(t : T?) {
    t |> run("re-copies local packages on update") @(t : T?) {
        let tmp_root = "{get_das_root()}/_daspkg_test_upd_local"
        let src_dir = "{get_das_root()}/_daspkg_test_upd_src"
        mkdir(tmp_root)
        mkdir(src_dir)
        // create a dummy source
        fopen("{src_dir}/hello.das", "wb") <| $(f) {
            fwrite(f, "// hello v1\n")
        }
        // install locally
        var lf : LockFile
        install_local(tmp_root, "localpkg", src_dir, lf)
        t |> success(lf.packages |> key_exists("localpkg"))
        // modify source
        fopen("{src_dir}/hello.das", "wb") <| $(f) {
            fwrite(f, "// hello v2\n")
        }
        // update should re-copy
        cmd_update(tmp_root, "localpkg")
        t |> success(fexist("{tmp_root}/modules/localpkg/hello.das"))
        // verify updated content
        let content = fread("{tmp_root}/modules/localpkg/hello.das")
        t |> equal(content, "// hello v2\n")
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
    t |> run("installs all dependencies from .das_package") @(t : T?) {
        let tmp_root = "{get_das_root()}/_daspkg_test_inst_all"
        mkdir(tmp_root)
        // write a .das_package with a dependency
        fwrite("{tmp_root}/.das_package", "options gen2\nrequire daslib/daspkg\n[export]\ndef package() \{\n    package_name(\"test-project\")\n    package_description(\"test\")\n\}\n[export]\ndef dependencies(version : string) \{\n    require_package(\"github.com/borisbat/daspkg-test-pure\")\n\}\n")
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

[test]
def test_install_local_auto_build(t : T?) {
    t |> run("auto-builds package with CMakeLists.txt on install") @(t : T?) {
        let tmp_root = "{get_das_root()}/_daspkg_test_autobuild"
        let src_dir = get_full_file_name("examples/daspkg/packages/daspkg-example-c")
        mkdir(tmp_root)
        // install local package that has CMakeLists.txt
        var lf : LockFile
        install_local(tmp_root, "daspkg-example-c", src_dir, lf)
        t |> success(lf.packages |> key_exists("daspkg-example-c"))
        // verify CMakeLists.txt was copied
        t |> success(fexist("{tmp_root}/modules/daspkg-example-c/CMakeLists.txt"))
        // verify build was triggered — _build directory should exist
        t |> success(fexist("{tmp_root}/modules/daspkg-example-c/_build"))
        // verify shared module was produced
        t |> success(fexist("{tmp_root}/modules/daspkg-example-c/dasModuleFastMath.shared_module"))
        // cleanup
        rmdir_rec(tmp_root)
    }
}

[test]
def test_update_local_auto_build(t : T?) {
    t |> run("auto-builds package with CMakeLists.txt on update") @(t : T?) {
        let tmp_root = "{get_das_root()}/_daspkg_test_autobuild_upd"
        let src_dir = get_full_file_name("examples/daspkg/packages/daspkg-example-c")
        mkdir(tmp_root)
        // install first
        var lf : LockFile
        install_local(tmp_root, "daspkg-example-c", src_dir, lf)
        t |> success(lf.packages |> key_exists("daspkg-example-c"))
        // remove the built module to verify rebuild on update
        let module_file = "{tmp_root}/modules/daspkg-example-c/dasModuleFastMath.shared_module"
        if (fexist(module_file))
            remove(module_file)
        t |> success(!fexist(module_file))
        // update should re-copy and rebuild
        cmd_update(tmp_root, "daspkg-example-c")
        t |> success(fexist(module_file))
        // cleanup
        rmdir_rec(tmp_root)
    }
}

[test]
def test_full_workflow_local(t : T?) {
    t |> run("install, run, update, run — pure das package") @(t : T?) {
        let das_root = get_das_root()
        let daslang = "{das_root}/bin/Release/daslang.exe"
        let tmp_root = "{das_root}/_daspkg_test_workflow"
        let pkg_src = "{das_root}/_daspkg_test_workflow_src"

        // create a simple das package source with .das_module
        mkdir(pkg_src)
        mkdir("{pkg_src}/daslib")
        fopen("{pkg_src}/daslib/greeter.das", "wb") <| $(f) {
            fwrite(f, "options gen2\nmodule greeter\ndef greet(name : string) : string \{\n    return \"hello, \" + name\n\}\n")
        }
        fopen("{pkg_src}/.das_module", "wb") <| $(f) {
            fwrite(f, "options gen2\nrequire fio\n[export]\ndef initialize(project_path : string) \{\n    register_native_path(\"greeter\", \"greeter\", \"\{project_path\}/daslib/greeter.das\")\n\}\n")
        }

        // create project dir
        mkdir(tmp_root)

        // step 1: install local package
        var lf : LockFile
        install_local(tmp_root, "greeter", pkg_src, lf)
        t |> success(fexist("{tmp_root}/modules/greeter/daslib/greeter.das"), "package installed")
        t |> success(fexist("{tmp_root}/modules/greeter/.das_module"), ".das_module present")

        // step 2: write a script that uses the package
        fopen("{tmp_root}/test_script.das", "wb") <| $(f) {
            fwrite(f, "options gen2\nrequire greeter/greeter\n[export]\ndef main() \{\n    print(greet(\"world\"))\n\}\n")
        }

        // step 3: run the script with -project_root, capture output to file
        let out_file = "{tmp_root}/_output.txt"
        var output : string
        var exit_code = run_cmd("{daslang} -project_root {tmp_root} {tmp_root}/test_script.das > {out_file} 2>&1", output)
        let result_v1 = strip(fread(out_file))
        t |> success(exit_code == 0, "script runs successfully (exit={exit_code})")
        t |> equal("hello, world", result_v1, "correct output v1")

        // step 4: modify the source package
        fopen("{pkg_src}/daslib/greeter.das", "wb") <| $(f) {
            fwrite(f, "options gen2\nmodule greeter\ndef greet(name : string) : string \{\n    return \"hi, \" + name\n\}\n")
        }

        // step 5: update
        cmd_update(tmp_root, "greeter")

        // step 6: run again — should reflect the update
        exit_code = run_cmd("{daslang} -project_root {tmp_root} {tmp_root}/test_script.das > {out_file} 2>&1", output)
        let result_v2 = strip(fread(out_file))
        t |> success(exit_code == 0, "script runs after update (exit={exit_code})")
        t |> equal("hi, world", result_v2, "correct output v2")

        // cleanup
        rmdir_rec(tmp_root)
        rmdir_rec(pkg_src)
    }
}

[test]
def test_cmd_build_integration(t : T?) {
    t |> run("builds all packages with CMakeLists.txt") @(t : T?) {
        let tmp_root = "{get_das_root()}/_daspkg_test_build_all"
        let src_dir = get_full_file_name("examples/daspkg/packages/daspkg-example-c")
        mkdir(tmp_root)
        // manually copy package without auto-build
        let modules_dir = "{tmp_root}/modules"
        let pkg_dir = "{modules_dir}/daspkg-example-c"
        mkdir(modules_dir)
        copy_dir_rec(src_dir, pkg_dir)
        // remove any pre-existing build artifacts
        let module_file = "{pkg_dir}/dasModuleFastMath.shared_module"
        if (fexist(module_file))
            remove(module_file)
        if (fexist("{pkg_dir}/_build"))
            rmdir_rec("{pkg_dir}/_build")
        // run cmd_build
        cmd_build(tmp_root)
        // verify build happened
        t |> success(fexist("{pkg_dir}/_build"))
        t |> success(fexist(module_file))
        // cleanup
        rmdir_rec(tmp_root)
    }
}
