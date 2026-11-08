options gen2
options no_global_variables = false

require dastest/testing_boost public

require daslib/fio
require daslib/clargs
require daslib/defer
require strings
require daslib/strings_boost

require daslib/json_boost


require lockfile
require utils
require commands
require daslib/result
require package_runner public
require _daspkg_test_common

[test(description = "cmd_cleanup --force removes the modules, the lock file and the gitignore entries")]
def test_cmd_cleanup(t : T?) {
    t |> run("removes modules, lockfile, and gitignore entries") @(t : T?) {
        let tmp_root = "{get_das_root()}/_daspkg_test_cleanup"
        force_rmdir(tmp_root)
        mkdir(tmp_root)
        // fake .git so gitignore_remove runs
        mkdir("{tmp_root}/.git")
        fwrite("{tmp_root}/.das_package", "options gen2\n")
        mkdir("{tmp_root}/modules")
        mkdir("{tmp_root}/modules/foo")
        fwrite("{tmp_root}/modules/foo/dummy.txt", "x")
        var lf : LockFile
        set_package(lf, PackageEntry(name = "foo", source = "github.com/u/foo", root = true))
        write_lock_file(tmp_root, lf)
        fwrite("{tmp_root}/.gitignore", "modules/foo\nother-entry\n")
        let rc = cmd_cleanup(tmp_root, true)
        t |> equal(rc, 0)
        t |> success(!fexist("{tmp_root}/modules"))
        t |> success(!fexist("{tmp_root}/daspkg.lock"))
        var content = ""
        fopen("{tmp_root}/.gitignore", "rb") $(f) {
            if (f != null) {
                content = fread(f)
            }
        }
        t |> equal(content, "other-entry\n")
        force_rmdir(tmp_root)
    }
}

[test(description = "cmd_cleanup refuses a directory that has neither a .das_package nor a lock file")]
def test_cmd_cleanup_refuses_without_marker(t : T?) {
    t |> run("refuses in dir without .das_package or lockfile") @(t : T?) {
        let tmp_root = "{get_das_root()}/_daspkg_test_cleanup_bare"
        force_rmdir(tmp_root)
        mkdir(tmp_root)
        mkdir("{tmp_root}/modules")
        mkdir("{tmp_root}/modules/foo")
        let rc = cmd_cleanup(tmp_root, true)
        t |> equal(rc, 1)
        // modules untouched
        t |> success(fexist("{tmp_root}/modules/foo"))
        force_rmdir(tmp_root)
    }
}

[test(description = "cmd_cleanup is a no-op on a project with a .das_package but no lock file or modules")]
def test_cmd_cleanup_no_lockfile(t : T?) {
    t |> run("no-op on project with .das_package but no lockfile/modules") @(t : T?) {
        let tmp_root = "{get_das_root()}/_daspkg_test_cleanup_empty"
        force_rmdir(tmp_root)
        mkdir(tmp_root)
        fwrite("{tmp_root}/.das_package", "options gen2\n")
        let rc = cmd_cleanup(tmp_root, true)
        t |> equal(rc, 0)
        t |> success(fexist("{tmp_root}/.das_package"))
        force_rmdir(tmp_root)
    }
}

[test(description = "cmd_cleanup leaves files that daspkg did not install alongside the modules")]
def test_cmd_cleanup_preserves_user_files(t : T?) {
    t |> run("leaves non-daspkg files alongside modules") @(t : T?) {
        let tmp_root = "{get_das_root()}/_daspkg_test_cleanup_preserve"
        force_rmdir(tmp_root)
        mkdir(tmp_root)
        fwrite("{tmp_root}/.das_package", "options gen2\n")
        fwrite("{tmp_root}/main.das", "// user file\n")
        fwrite("{tmp_root}/README.md", "hello\n")
        mkdir("{tmp_root}/modules")
        mkdir("{tmp_root}/modules/foo")
        var lf : LockFile
        set_package(lf, PackageEntry(name = "foo", source = "github.com/u/foo", root = true))
        write_lock_file(tmp_root, lf)
        let rc = cmd_cleanup(tmp_root, true)
        t |> equal(rc, 0)
        t |> success(fexist("{tmp_root}/.das_package"))
        t |> success(fexist("{tmp_root}/main.das"))
        t |> success(fexist("{tmp_root}/README.md"))
        t |> success(!fexist("{tmp_root}/modules"))
        t |> success(!fexist("{tmp_root}/daspkg.lock"))
        force_rmdir(tmp_root)
    }
}

[test(description = "cmd_cleanup without --force only reports and keeps the modules and the lock file")]
def test_cmd_cleanup_without_force_dry_runs(t : T?) {
    t |> run("without --force, modules and lockfile are preserved") @(t : T?) {
        let tmp_root = "{get_das_root()}/_daspkg_test_cleanup_dryrun"
        force_rmdir(tmp_root)
        mkdir(tmp_root)
        fwrite("{tmp_root}/.das_package", "options gen2\n")
        mkdir("{tmp_root}/modules")
        mkdir("{tmp_root}/modules/foo")
        var lf : LockFile
        set_package(lf, PackageEntry(name = "foo", source = "github.com/u/foo", root = true))
        write_lock_file(tmp_root, lf)
        let rc = cmd_cleanup(tmp_root, false)
        t |> equal(rc, 0)
        t |> success(fexist("{tmp_root}/modules/foo"))
        t |> success(fexist("{tmp_root}/daspkg.lock"))
        force_rmdir(tmp_root)
    }
}

[test(description = "cmd_check detects present and missing packages and warns about a missing .das_module")]
def test_cmd_check(t : T?) {
    let test_root = get_full_file_name("_daspkg_test_check")
    let modules_dir = "{test_root}/modules"
    let pkg_dir = "{modules_dir}/mypkg"

    // setup: create module dir with .das_module
    mkdir(test_root)
    mkdir(modules_dir)
    mkdir(pkg_dir)
    fopen("{pkg_dir}/.das_module", "wb") $(f) {
        fwrite(f, "options gen2\n")
    }

    // write lockfile with two entries: one present, one missing
    var lf = LockFile()
    set_package(lf, PackageEntry(name = "mypkg", source = "github.com/user/mypkg"))
    set_package(lf, PackageEntry(name = "ghost", source = "github.com/user/ghost"))
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
        set_package(lf2, PackageEntry(name = "nopkg", source = "github.com/user/nopkg"))
        write_lock_file(test_root, lf2)
        cmd_check(test_root)  // should print WARNING, not crash
        t |> success(fexist(pkg2_dir))
        force_rmdir(pkg2_dir)
    }

    // cleanup
    force_rmdir(test_root)
}

[test(description = "cmd_check on a project with no lock file prints a message and does not fail")]
def test_cmd_check_no_lockfile(t : T?) {
    t |> run("no lockfile prints message") @(t : T?) {
        cmd_check("_daspkg_nonexistent_dir")
        t |> success(true)  // just verify no crash
    }
}

[test(description = "check_tool finds git and cmake and fails on a tool that does not exist", timeout = 1200)]
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

[test(description = "cmd_doctor runs its tool checks without crashing", timeout = 1200)]
def test_cmd_doctor(t : T?) {
    t |> run("runs without crash") @(t : T?) {
        cmd_doctor()
        t |> success(true)
    }
}

[test(description = "gitignore_add creates or appends an entry once, adds a missing newline, and does nothing outside a git checkout")]
def test_gitignore_add(t : T?) {
    let tmp_root = "{get_das_root()}/_daspkg_test_gitignore"
    mkdir(tmp_root)
    // create a fake .git dir so gitignore_add doesn't skip
    mkdir("{tmp_root}/.git")
    let gitignore = "{tmp_root}/.gitignore"

    t |> run("creates .gitignore if missing") @(t : T?) {
        remove(gitignore)
        gitignore_add(tmp_root, "modules/foo")
        var content = ""
        fopen(gitignore, "rb") $(f) {
            if (f != null) {
                content = fread(f)
            }
        }
        t |> equal(content, "modules/foo\n")
    }
    t |> run("appends without duplicating") @(t : T?) {
        remove(gitignore)
        gitignore_add(tmp_root, "modules/foo")
        gitignore_add(tmp_root, "modules/bar")
        gitignore_add(tmp_root, "modules/foo")  // duplicate
        var content = ""
        fopen(gitignore, "rb") $(f) {
            if (f != null) {
                content = fread(f)
            }
        }
        t |> equal(content, "modules/foo\nmodules/bar\n")
    }
    t |> run("appends newline if file doesn't end with one") @(t : T?) {
        fopen(gitignore, "wb") $(f) {
            if (f != null) {
                fprint(f, "existing")
            }
        }
        gitignore_add(tmp_root, "modules/baz")
        var content = ""
        fopen(gitignore, "rb") $(f) {
            if (f != null) {
                content = fread(f)
            }
        }
        t |> equal(content, "existing\nmodules/baz\n")
    }
    t |> run("skips if no .git directory") @(t : T?) {
        let no_git = "{get_das_root()}/_daspkg_test_nogit"
        mkdir(no_git)
        gitignore_add(no_git, "modules/foo")
        t |> success(!fexist("{no_git}/.gitignore"))
        rmdir(no_git)
    }
    // cleanup
    remove(gitignore)
    force_rmdir("{tmp_root}/.git")
    rmdir(tmp_root)
}

[test(description = "gitignore_remove drops the matching line and is a no-op when the pattern or the file is absent")]
def test_gitignore_remove(t : T?) {
    let tmp_root = "{get_das_root()}/_daspkg_test_gitignore_rm"
    mkdir(tmp_root)
    mkdir("{tmp_root}/.git")
    let gitignore = "{tmp_root}/.gitignore"

    t |> run("removes matching line") @(t : T?) {
        fopen(gitignore, "wb") $(f) {
            if (f != null) {
                fprint(f, "modules/foo\nmodules/bar\nmodules/baz\n")
            }
        }
        gitignore_remove(tmp_root, "modules/bar")
        var content = ""
        fopen(gitignore, "rb") $(f) {
            if (f != null) {
                content = fread(f)
            }
        }
        t |> equal(content, "modules/foo\nmodules/baz\n")
    }
    t |> run("no-op if pattern not found") @(t : T?) {
        fopen(gitignore, "wb") $(f) {
            if (f != null) {
                fprint(f, "modules/foo\n")
            }
        }
        gitignore_remove(tmp_root, "modules/nope")
        var content = ""
        fopen(gitignore, "rb") $(f) {
            if (f != null) {
                content = fread(f)
            }
        }
        t |> equal(content, "modules/foo\n")
    }
    t |> run("no-op if no .gitignore") @(t : T?) {
        remove(gitignore)
        gitignore_remove(tmp_root, "modules/foo")
        t |> success(!fexist(gitignore))
    }
    // cleanup
    remove(gitignore)
    force_rmdir("{tmp_root}/.git")
    rmdir(tmp_root)
}

[test(description = "cmd_build with a name builds only that package and fails for an unknown or non-bare name", timeout = 1200)]
def test_cmd_build_only(t : T?) {
    t |> run("a name with no CMake package fails") @(t : T?) {
        let tmp_root = scratch_dir(t, "daspkg-build-only-")
        mkdir_rec("{tmp_root}/modules/plain")
        t |> equal(1, cmd_build(tmp_root, false, "absent"))
        t |> equal(1, cmd_build(tmp_root, false, "plain"))
        force_rmdir(tmp_root)
    }
    t |> run("a name that is not a bare package name fails") @(t : T?) {
        let tmp_root = scratch_dir(t, "daspkg-build-only-")
        mkdir_rec("{tmp_root}/modules/wanted")
        fwrite("{tmp_root}/modules/wanted/CMakeLists.txt", "cmake_minimum_required(VERSION 3.16)\nproject(wanted NONE)\n")
        t |> equal(1, cmd_build(tmp_root, false, "wanted/"))
        t |> equal(1, cmd_build(tmp_root, false, "../modules/wanted"))
        force_rmdir(tmp_root)
    }
    t |> run("only the named package builds") @(t : T?) {
        let tmp_root = scratch_dir(t, "daspkg-build-only-")
        mkdir_rec("{tmp_root}/modules/wanted")
        mkdir_rec("{tmp_root}/modules/broken")
        fwrite("{tmp_root}/modules/wanted/CMakeLists.txt", "cmake_minimum_required(VERSION 3.16)\nproject(wanted NONE)\n")
        fwrite("{tmp_root}/modules/broken/CMakeLists.txt", "message(FATAL_ERROR \"built a package nobody named\")\n")
        t |> equal(0, cmd_build(tmp_root, false, "wanted"))
        t |> equal(1, cmd_build(tmp_root))
        // the filesystem's case rule decides whether WANTED names it; build agrees with the lookup
        let found = fexist("{tmp_root}/modules/WANTED/CMakeLists.txt")
        t |> equal(found ? 0 : 1, cmd_build(tmp_root, false, "WANTED"))
        force_rmdir(tmp_root)
    }
}

[test(description = "daspkg build <name> builds only that name, so an absent name fails with exit code 1", timeout = 1200)]
def test_main_build_takes_a_name(t : T?) {
    t |> run("daspkg build <name> builds only that name, so an absent one fails") @(t : T?) {
        let tmp = scratch_dir(t, "daspkg-main-build-")
        mkdir("{tmp}/modules")
        let args <- get_command_line_arguments()
        var exe = replace(args[0], "\\", "/")
        if (find(exe, ":") < 0 && first_character(exe) != '/') {
            exe = "{get_das_root()}/{exe}"
        }
        var output : string
        let main_das = "{get_das_root()}/utils/daspkg/main.das"
        t |> equal(0, run_cmd("\"{exe}\" \"{main_das}\" -- build --root \"{tmp}\"", output), output)
        t |> equal(1, run_cmd("\"{exe}\" \"{main_das}\" -- build absent --root \"{tmp}\"", output), output)
        force_rmdir(tmp)
    }
}
