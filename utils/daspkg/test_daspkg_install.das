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

[test(description = "a native build is blocked on an SDK without daScript.h and unblocked once the header exists")]
def test_native_build_blocker(t : T?) {
    t |> run("a native build is refused on a headerless SDK, allowed with daScript.h") @(t : T?) {
        let tmp = create_temp_directory_result("daspkg headers ")
        if (!(tmp is value)) {
            t |> failure("could not create temp directory: {tmp as error}")
            return
        }
        let root = tmp as value
        let blocker = native_build_blocker(root)
        t |> success(!empty(blocker), "a headerless SDK is a blocker")
        t |> success(find(blocker, "include/daScript") >= 0, "the message names the missing tree")
        t |> success(find(blocker, root) >= 0, "the message names the SDK it looked at")
        mkdir(path_join(root, "include"))
        mkdir(path_join(root, "include/daScript"))
        fwrite(path_join(root, "include/daScript/daScript.h"), "#pragma once")
        t |> equal(native_build_blocker(root), "", "the header set clears the blocker")
        let cleanup = rmdir_rec_result(root)
        t |> success(cleanup is value && cleanup as value, "temp dir removed")
    }
}

[test(description = "copy_dir_rec copies real content and skips links, .git and build directories", timeout = 1200)]
def test_copy_dir_rec_skips_links_and_build_residue(t : T?) {
    t |> run("junction/symlink, .git, and build dirs stay out of the copy") @(t : T?) {
        let tmp = "{get_das_root()}/_daspkg_test_copyskip"
        let src = "{tmp}/src"
        mkdir(tmp)
        mkdir(src)
        mkdir("{src}/real")
        fopen("{src}/real/a.txt", "wb") $(f) {
            fprint(f, "content")
        }
        mkdir("{src}/_build")
        fopen("{src}/_build/CMakeCache.txt", "wb") $(f) {
            fprint(f, "stale")
        }
        mkdir("{src}/build")
        mkdir("{src}/.git")
        // a link pointing at src itself - copying it recurses forever unless copy_dir_rec skips links
        let link = "{src}/loop"
        var out : string
        var link_cmd = "ln -s \"{src}\" \"{link}\""
        if (get_platform_name() == "windows") {
            link_cmd = "cmd /c mklink /J \"{replace(link, "/", "\\")}\" \"{replace(src, "/", "\\")}\""
        }
        let linked = run_cmd(link_cmd, out) == 0
        let dst = "{tmp}/dst"
        t |> success(copy_dir_rec(src, dst), "copy_dir_rec({src} -> {dst}) returned false")
        t |> success(fexist("{dst}/real/a.txt"), "real content missing from the copy")
        t |> success(!fexist("{dst}/_build"), "_build was copied")
        t |> success(!fexist("{dst}/build"), "build was copied")
        t |> success(!fexist("{dst}/.git"), ".git was copied")
        if (linked) {
            var link_err : string
            t |> success(is_reparse_point(link, link_err), "is_reparse_point does not see the link")
            t |> success(!is_reparse_point(src, link_err), "is_reparse_point flags a plain dir")
            t |> success(!fexist("{dst}/loop"), "the link was followed into the copy")
        } else {
            to_log(LOG_INFO, "test_copy_dir_rec_skips_links_and_build_residue: could not create a junction/symlink here; skipping the link half\n")
        }
        force_rmdir(tmp)
    }
}

[test(description = "installing an in-tree module is a no-op that creates no shadow clone")]
def test_in_tree_module_satisfies_install(t : T?) {
    t |> run("explicit install of an in-tree module is a no-op") @(t : T?) {
        // dasGlfw is a tracked in-tree module: present at <das_root>/modules/
        // and never in the global lock file. cmd_install must recognize it,
        // return 0 without touching the index or the network, and create no
        // project-local shadow clone.
        let tmp_root = "{get_das_root()}/_daspkg_test_intree"
        mkdir(tmp_root)
        let rc = cmd_install(tmp_root, "dasGlfw", false)
        t |> equal(rc, 0)
        t |> success(!fexist("{tmp_root}/modules/dasGlfw"))
        // cleanup (no lock file is written on the no-op path)
        rmdir(tmp_root)
    }
}

[test(description = "install_local copies the package, records it in the lock file as local and root, and writes the lock file")]
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
        t |> success(has_package(lf, "hello"))
        let hello = get_package(lf, "hello")
        t |> success(hello.local)
        t |> success(hello.root)
        // verify files copied
        t |> success(fexist("{tmp_root}/modules/hello/daslib/hello.das"))
        // verify lock file was written to disk
        t |> success(fexist("{tmp_root}/{LOCK_FILE_NAME}"))
        // cleanup
        force_rmdir(tmp_root)
        force_rmdir(tmp_src)
    }
}

// Writes a .das_package + CMakeLists.txt that always FATAL_ERRORs on configure.
def private write_failing_pkg(src_dir, pkg_name : string) {
    mkdir(src_dir)
    let manifest = "options gen2\nrequire daslib/daspkg\n[export]\ndef package() \{\n    package_name(\"{pkg_name}\")\n\}\n[export]\ndef build() \{\n    cmake_build()\n\}\n"
    fwrite("{src_dir}/.das_package", manifest)
    fwrite("{src_dir}/CMakeLists.txt", "cmake_minimum_required(VERSION 3.16)\nproject({pkg_name})\nmessage(FATAL_ERROR \"forced test failure\")\n")
}

[test(description = "a package whose build fails is neither installed nor recorded, and no lock file appears", timeout = 1200)]
def test_install_local_build_failure_not_recorded(t : T?) {
    t |> run("failed build does not stay installed") @(t : T?) {
        let tmp_root = "{get_das_root()}/_daspkg_test_fail_root"
        let tmp_src = "{get_das_root()}/_daspkg_test_fail_src"
        force_rmdir(tmp_root)
        force_rmdir(tmp_src)
        mkdir(tmp_root)
        write_failing_pkg(tmp_src, "failpkg")

        var lf : LockFile
        let rc = install_local(tmp_root, "failpkg", tmp_src, lf)
        t |> success(rc != 0)
        t |> success(!has_package(lf, "failpkg"))
        t |> success(!fexist("{tmp_root}/modules/failpkg"))
        // failure with no prior install must NOT create an empty lock file
        t |> success(!fexist("{tmp_root}/{LOCK_FILE_NAME}"))

        force_rmdir(tmp_root)
        force_rmdir(tmp_src)
    }
}

[test(description = "a failed reinstall over a healthy package removes both its directory and its lock entry", timeout = 1200)]
def test_install_local_reinstall_failure_clears_entry(t : T?) {
    // First install a healthy pkg, then reinstall in place with a failing source.
    // The reinstall must remove both the directory AND the prior lockfile entry,
    // so the lockfile is consistent with the on-disk state.
    t |> run("reinstall over healthy pkg with failing src clears lock entry") @(t : T?) {
        let tmp_root = "{get_das_root()}/_daspkg_test_reinst_root"
        let tmp_src_ok = "{get_das_root()}/_daspkg_test_reinst_ok_src"
        let tmp_src_bad = "{get_das_root()}/_daspkg_test_reinst_bad_src"
        force_rmdir(tmp_root)
        force_rmdir(tmp_src_ok)
        force_rmdir(tmp_src_bad)
        mkdir(tmp_root)
        write_passing_pkg(tmp_src_ok, "reinstpkg")
        write_failing_pkg(tmp_src_bad, "reinstpkg")

        var lf : LockFile
        let rc1 = install_local(tmp_root, "reinstpkg", tmp_src_ok, lf)
        t |> success(rc1 == 0)
        t |> success(has_package(lf, "reinstpkg"))
        t |> success(fexist("{tmp_root}/modules/reinstpkg"))

        let rc2 = install_local(tmp_root, "reinstpkg", tmp_src_bad, lf)
        t |> success(rc2 != 0)
        t |> success(!has_package(lf, "reinstpkg"))
        t |> success(!fexist("{tmp_root}/modules/reinstpkg"))

        // disk lock file must agree with in-memory lf
        var lf2 : LockFile
        if (read_lock_file(tmp_root, lf2)) {
            t |> success(!has_package(lf2, "reinstpkg"))
        }

        force_rmdir(tmp_root)
        force_rmdir(tmp_src_ok)
        force_rmdir(tmp_src_bad)
    }
}

[test(description = "a failing dependency rolls back the parent and itself", timeout = 1200)]
def test_install_local_failing_transitive_dep(t : T?) {
    // Parent depends on a failing local child. Both should be rolled back.
    // Pkg names must match the source-dir basenames — install_dependencies
    // derives dep names from the source path, not the .das_package manifest.
    t |> run("failing transitive dep rolls back parent") @(t : T?) {
        let fix = "{get_das_root()}/_daspkg_test_dep_fix"
        force_rmdir(fix)
        mkdir(fix)
        let proj = "{fix}/proj"
        let parent_src = "{fix}/depparent"
        let child_src = "{fix}/depchild"
        mkdir(proj)
        write_failing_pkg(child_src, "depchild")
        let parent_deps <- [child_src]
        write_passing_pkg(parent_src, "depparent", parent_deps)

        var lf : LockFile
        let rc = install_local(proj, "depparent", parent_src, lf)
        t |> success(rc != 0)
        t |> success(!has_package(lf, "depparent"))
        t |> success(!has_package(lf, "depchild"))
        t |> success(!fexist("{proj}/modules/depparent"))
        t |> success(!fexist("{proj}/modules/depchild"))

        force_rmdir(fix)
    }
}

[test(description = "a dependency cycle between healthy packages installs each once without recursing forever")]
def test_install_local_cycle(t : T?) {
    // A depends on B, B depends on A. Both passing — should install once each
    // without infinite recursion or directory wipes.
    t |> run("cycle between two healthy packages installs both") @(t : T?) {
        let fix = "{get_das_root()}/_daspkg_test_cycle_fix"
        force_rmdir(fix)
        mkdir(fix)
        let proj = "{fix}/proj"
        let a_src = "{fix}/cycleA"
        let b_src = "{fix}/cycleB"
        mkdir(proj)
        let a_deps <- [b_src]
        let b_deps <- [a_src]
        write_passing_pkg(a_src, "cycleA", a_deps)
        write_passing_pkg(b_src, "cycleB", b_deps)

        var lf : LockFile
        let rc = install_local(proj, "cycleA", a_src, lf)
        t |> success(rc == 0)
        t |> success(has_package(lf, "cycleA"))
        t |> success(has_package(lf, "cycleB"))
        t |> success(fexist("{proj}/modules/cycleA"))
        t |> success(fexist("{proj}/modules/cycleB"))
        // root vs transitive flags
        let a_entry = get_package(lf, "cycleA")
        let b_entry = get_package(lf, "cycleB")
        t |> success(a_entry.root)
        t |> success(!b_entry.root)

        force_rmdir(fix)
    }
}

[test(description = "a dependency shared by two siblings installs once through the already-installed path")]
def test_install_local_diamond_dep(t : T?) {
    // P depends on A and B; both A and B depend on shared C.
    // C should install once; the second sibling's iteration must take the
    // has_package path, NOT the cycle-detection path. Without erasing
    // visiting on unwind, C would still be in `visiting` when B is
    // processed and would emit a misleading "cycle detected" log.
    t |> run("shared dep installs once via has_package, not cycle path") @(t : T?) {
        let fix = "{get_das_root()}/_daspkg_test_diamond_fix"
        force_rmdir(fix)
        mkdir(fix)
        let proj = "{fix}/proj"
        let p_src = "{fix}/diamondP"
        let a_src = "{fix}/diamondA"
        let b_src = "{fix}/diamondB"
        let c_src = "{fix}/diamondC"
        mkdir(proj)
        write_passing_pkg(c_src, "diamondC")
        let a_deps <- [c_src]
        let b_deps <- [c_src]
        write_passing_pkg(a_src, "diamondA", a_deps)
        write_passing_pkg(b_src, "diamondB", b_deps)
        let p_deps <- [a_src, b_src]
        write_passing_pkg(p_src, "diamondP", p_deps)

        var lf : LockFile
        let rc = install_local(proj, "diamondP", p_src, lf)
        t |> success(rc == 0)
        t |> success(has_package(lf, "diamondP"))
        t |> success(has_package(lf, "diamondA"))
        t |> success(has_package(lf, "diamondB"))
        t |> success(has_package(lf, "diamondC"))
        t |> success(fexist("{proj}/modules/diamondC"))

        force_rmdir(fix)
    }
}

[test(description = "a cycle with a failing member rolls back every package in it")]
def test_install_local_cycle_with_failure(t : T?) {
    // A depends on B, B depends on A — but B's build fails.
    // Both must be rolled back; nothing left in lockfile or on disk.
    t |> run("cycle with failing dep rolls back both") @(t : T?) {
        let fix = "{get_das_root()}/_daspkg_test_cyclefail_fix"
        force_rmdir(fix)
        mkdir(fix)
        let proj = "{fix}/proj"
        let a_src = "{fix}/cyclefailA"
        let b_src = "{fix}/cyclefailB"
        mkdir(proj)
        let a_deps <- [b_src]
        write_passing_pkg(a_src, "cyclefailA", a_deps)
        // failing B with a back-edge to A to form the cycle
        mkdir(b_src)
        let b_manifest = "options gen2\nrequire daslib/daspkg\n[export]\ndef package() \{\n    package_name(\"cyclefailB\")\n\}\n[export]\ndef dependencies(version : string) \{\n    require_package(\"{a_src}\")\n\}\n[export]\ndef build() \{\n    cmake_build()\n\}\n"
        fwrite("{b_src}/.das_package", b_manifest)
        fwrite("{b_src}/CMakeLists.txt", "cmake_minimum_required(VERSION 3.16)\nproject(cyclefailB)\nmessage(FATAL_ERROR \"forced test failure\")\n")

        var lf : LockFile
        let rc = install_local(proj, "cyclefailA", a_src, lf)
        t |> success(rc != 0)
        t |> success(!has_package(lf, "cyclefailA"))
        t |> success(!has_package(lf, "cyclefailB"))
        t |> success(!fexist("{proj}/modules/cyclefailA"))
        t |> success(!fexist("{proj}/modules/cyclefailB"))

        force_rmdir(fix)
    }
}

let private BROKEN_MANIFEST = "options gen2\nrequire daslib/daspkg\n[export]\ndef package() \{\n    package_from_a_newer_sdk(\"x\")\n\}\n"

[test(description = "a manifest this SDK cannot compile is refused before anything lands, and a throwing build() fails and rolls back the install")]
def test_install_local_manifest_this_sdk_cannot_run(t : T?) {
    t |> run("a manifest that does not compile is refused before anything lands") @(t : T?) {
        let tmp = scratch_dir(t, "daspkg-nocompile-")
        let tmp_root = "{tmp}/root"
        let tmp_src = "{tmp}/src"
        mkdir_rec("{tmp_root}/modules/nocompile")
        fwrite("{tmp_root}/modules/nocompile/keep.txt", "an earlier install")
        mkdir(tmp_src)
        fwrite("{tmp_src}/.das_package", BROKEN_MANIFEST)
        var lf : LockFile
        t |> equal(1, install_local(tmp_root, "nocompile", tmp_src, lf, [force = true]))
        t |> success(!has_package(lf, "nocompile"))
        t |> success(fexist("{tmp_root}/modules/nocompile/keep.txt"))
        force_rmdir(tmp)
    }
    t |> run("a build() that throws fails the install and rolls it back") @(t : T?) {
        let tmp = scratch_dir(t, "daspkg-throwbuild-")
        let tmp_root = "{tmp}/root"
        let tmp_src = "{tmp}/src"
        mkdir(tmp_root)
        mkdir(tmp_src)
        fwrite("{tmp_src}/.das_package", "options gen2\nrequire daslib/daspkg\n[export]\ndef package() \{\n    package_name(\"throwbuild\")\n\}\n[export]\ndef build() \{\n    panic(\"build hook sentinel\")\n\}\n")
        var lf : LockFile
        t |> equal(1, install_local(tmp_root, "throwbuild", tmp_src, lf))
        t |> success(!has_package(lf, "throwbuild"))
        t |> success(!fexist("{tmp_root}/modules/throwbuild"))
        force_rmdir(tmp)
    }
}

[test(description = "an install whose target has a file held open fails the reinstall instead of the copy")]
def test_install_local_existing_target_held(t : T?) {
    t |> run("an existing install a process holds a file of fails the reinstall, not the copy") @(t : T?) {
        let tmp = scratch_dir(t, "daspkg-held-target-")
        let tmp_root = "{tmp}/root"
        let tmp_src = "{tmp}/src"
        mkdir(tmp_root)
        write_passing_pkg(tmp_src, "heldpkg")
        var lf : LockFile
        t |> equal(0, install_local(tmp_root, "heldpkg", tmp_src, lf))
        // Windows cannot delete a file open without share-delete; elsewhere the unlink succeeds
        let expected = get_platform_name() == "windows" ? 1 : 0
        fopen("{tmp_root}/modules/heldpkg/.das_package", "rb") $(held) {
            t |> success(held != null)
            t |> equal(expected, install_local(tmp_root, "heldpkg", tmp_src, lf))
        }
        force_rmdir(tmp)
    }
}

[test(description = "move_into_place moves a tree whole, survives a held file, fails on a link, and leaves nothing behind when the destination cannot be made", timeout = 1200)]
def test_move_into_place(t : T?) {
    t |> run("a tree moves") @(t : T?) {
        let tmp = scratch_dir(t, "daspkg-move-")
        let src = "{tmp}/src"
        let dst = "{tmp}/dst"
        mkdir(src)
        mkdir("{src}/sub")
        fwrite("{src}/sub/b.txt", "b")
        t |> success(move_into_place(src, dst))
        t |> success(fexist("{dst}/sub/b.txt"))
        t |> success(!fexist(src))
        force_rmdir(tmp)
    }
    t |> run("a file held open under the source still lands the whole tree") @(t : T?) {
        let tmp = scratch_dir(t, "daspkg-move-held-")
        let src = "{tmp}/src"
        let dst = "{tmp}/dst"
        mkdir_rec("{src}/sub")
        mkdir_rec("{src}/.git")
        mkdir_rec("{src}/build")
        fwrite("{src}/a.txt", "a")
        fwrite("{src}/sub/b.txt", "b")
        fwrite("{src}/.git/HEAD", "ref: refs/heads/main\n")
        fwrite("{src}/build/c.txt", "c")
        fopen("{src}/a.txt", "rb") $(held) {
            t |> success(held != null)
            t |> success(move_into_place(src, dst))
        }
        t |> success(fexist("{dst}/a.txt"))
        t |> success(fexist("{dst}/sub/b.txt"))
        t |> success(fexist("{dst}/.git/HEAD"))
        t |> success(fexist("{dst}/build/c.txt"))
        force_rmdir(tmp)
    }
    t |> run("a link in the source fails the copy instead of dropping it") @(t : T?) {
        let tmp = scratch_dir(t, "daspkg-move-link-")
        let src = "{tmp}/src"
        let target = "{tmp}/target"
        mkdir(src)
        mkdir(target)
        fwrite("{src}/a.txt", "a")
        let link = "{src}/linked"
        var link_cmd = "ln -s \"{target}\" \"{link}\""
        if (get_platform_name() == "windows") {
            link_cmd = "cmd /c mklink /J \"{replace(link, "/", "\\")}\" \"{replace(target, "/", "\\")}\""
        }
        var out : string
        if (run_cmd(link_cmd, out) != 0) {
            t |> skip("could not create a junction/symlink here")
            force_rmdir(tmp)
            return
        }
        t |> success(!copy_dir_rec(src, "{tmp}/whole", true))
        t |> success(copy_dir_rec(src, "{tmp}/content"))
        force_rmdir(tmp)
    }
    t |> run("a destination that cannot be made fails and leaves nothing there") @(t : T?) {
        let tmp = scratch_dir(t, "daspkg-move-nodst-")
        let src = "{tmp}/src"
        let dst = "{tmp}/no/such/parent/dst"
        mkdir(src)
        fwrite("{src}/a.txt", "a")
        t |> success(!move_into_place(src, dst))
        t |> success(!fexist(dst))
        force_rmdir(tmp)
    }
}

[test(description = "a failed reinstall restores the previous copy and its lock entry, and a successful one drops the backup")]
def test_reinstall_with_backup(t : T?) {
    t |> run("a failed install restores the installed copy and its lock entry") @(t : T?) {
        let root = scratch_dir(t, "daspkg-reinstall-")
        mkdir_rec("{root}/modules/pkg")
        fwrite("{root}/modules/pkg/old.txt", "old")
        var lf : LockFile
        let entry = PackageEntry(name = "pkg", source = "github.com/nobody/pkg", tag = "v1.0")
        set_package(lf, entry)
        let rc = reinstall_with_backup(root, entry, lf, false) $(var into_lf : LockFile) {
            mkdir("{root}/modules/pkg")
            fwrite("{root}/modules/pkg/partial.txt", "partial")
            return 1
        }
        t |> equal(1, rc)
        t |> success(fexist("{root}/modules/pkg/old.txt"))
        t |> success(!fexist("{root}/modules/pkg/partial.txt"))
        t |> success(has_package(lf, "pkg"))
        var on_disk : LockFile
        t |> success(read_lock_file(root, on_disk))
        t |> equal("v1.0", get_package(on_disk, "pkg").tag)
        force_rmdir(root)
    }
    t |> run("a successful install replaces the copy and drops the backup") @(t : T?) {
        let root = scratch_dir(t, "daspkg-reinstall-")
        mkdir_rec("{root}/modules/pkg")
        fwrite("{root}/modules/pkg/old.txt", "old")
        var lf : LockFile
        let entry = PackageEntry(name = "pkg", source = "github.com/nobody/pkg", tag = "v1.0")
        set_package(lf, entry)
        let rc = reinstall_with_backup(root, entry, lf, false) $(var into_lf : LockFile) {
            mkdir("{root}/modules/pkg")
            fwrite("{root}/modules/pkg/new.txt", "new")
            set_package(into_lf, PackageEntry(name = "pkg", source = "github.com/nobody/pkg", tag = "v2.0"))
            return 0
        }
        t |> equal(0, rc)
        t |> success(fexist("{root}/modules/pkg/new.txt"))
        t |> success(!fexist("{root}/modules/pkg/old.txt"))
        t |> success(!fexist("{root}/modules/.daspkg_backup/pkg"))
        t |> equal("v2.0", get_package(lf, "pkg").tag)
        force_rmdir(root)
    }
}

[test(description = "update from a local source whose manifest this daslang cannot run leaves the installed copy untouched")]
def test_update_local_gates_the_manifest(t : T?) {
    t |> run("a local source whose manifest this daslang cannot run leaves the installed copy") @(t : T?) {
        let tmp = scratch_dir(t, "daspkg-update-local-")
        let tmp_root = "{tmp}/root"
        let tmp_src = "{tmp}/src"
        mkdir(tmp_root)
        write_passing_pkg(tmp_src, "localgated")
        var lf : LockFile
        t |> equal(0, install_local(tmp_root, "localgated", tmp_src, lf))
        fwrite("{tmp_src}/.das_package", BROKEN_MANIFEST)
        t |> equal(1, cmd_update(tmp_root, "localgated"))
        let kept = fread("{tmp_root}/modules/localgated/.das_package")
        t |> success(find(kept, "package_from_a_newer_sdk") < 0)
        force_rmdir(tmp)
    }
}

[test(description = "land_clone replaces the install on success and removes the clone on a refused manifest, a held file or a failed move")]
def test_land_clone(t : T?) {
    t |> run("a clone replaces the existing install") @(t : T?) {
        let tmp = scratch_dir(t, "daspkg-land-")
        let clone = "{tmp}/clone"
        let target = "{tmp}/modules/pkg"
        write_passing_pkg(clone, "pkg")
        fwrite("{clone}/new.txt", "new")
        mkdir_rec(target)
        fwrite("{target}/old.txt", "old")
        t |> equal(0, land_clone(clone, target, "pkg", false))
        t |> success(fexist("{target}/new.txt"))
        t |> success(!fexist("{target}/old.txt"))
        force_rmdir(tmp)
    }
    t |> run("a refused manifest removes the clone and leaves the existing install") @(t : T?) {
        let tmp = scratch_dir(t, "daspkg-land-")
        let clone = "{tmp}/clone"
        let target = "{tmp}/modules/pkg"
        mkdir(clone)
        fwrite("{clone}/.das_package", BROKEN_MANIFEST)
        mkdir_rec(target)
        fwrite("{target}/old.txt", "old")
        t |> equal(1, land_clone(clone, target, "pkg", true))
        t |> success(!fexist(clone))
        t |> success(fexist("{target}/old.txt"))
        force_rmdir(tmp)
    }
    t |> run("an existing install a process holds a file of fails the landing") @(t : T?) {
        let tmp = scratch_dir(t, "daspkg-land-")
        let clone = "{tmp}/clone"
        let target = "{tmp}/modules/pkg"
        write_passing_pkg(clone, "pkg")
        mkdir_rec(target)
        fwrite("{target}/held.txt", "held")
        // Windows cannot delete a file open without share-delete; elsewhere the unlink succeeds
        let expected = get_platform_name() == "windows" ? 1 : 0
        fopen("{target}/held.txt", "rb") $(held) {
            t |> success(held != null)
            t |> equal(expected, land_clone(clone, target, "pkg", false))
        }
        t |> success(!fexist(clone))
        force_rmdir(tmp)
    }
    t |> run("a move that fails removes the clone") @(t : T?) {
        let tmp = scratch_dir(t, "daspkg-land-")
        let clone = "{tmp}/clone"
        let target = "{tmp}/no/such/parent/pkg"
        write_passing_pkg(clone, "pkg")
        t |> equal(1, land_clone(clone, target, "pkg", false))
        t |> success(!fexist(clone))
        t |> success(!fexist(target))
        force_rmdir(tmp)
    }
}

[test(description = "an install is refused for an unsupported platform, forced by --force, and the gate applies to dependencies too")]
def test_install_local_platform_gate(t : T?) {
    let other = other_platform()
    t |> run("an unsupported package is refused before anything lands") @(t : T?) {
        let tmp_root = "{get_das_root()}/_daspkg_test_plat_root"
        let tmp_src = "{get_das_root()}/_daspkg_test_plat_src"
        mkdir(tmp_root)
        write_platform_pkg(tmp_src, "foreign", other)
        var lf : LockFile
        t |> equal(RC_UNSUPPORTED_PLATFORM, install_local(tmp_root, "foreign", tmp_src, lf))
        t |> success(!has_package(lf, "foreign"))
        t |> success(!fexist("{tmp_root}/modules/foreign"))
        force_rmdir(tmp_root)
        force_rmdir(tmp_src)
    }
    t |> run("--force installs it") @(t : T?) {
        let tmp_root = "{get_das_root()}/_daspkg_test_plat_root"
        let tmp_src = "{get_das_root()}/_daspkg_test_plat_src"
        mkdir(tmp_root)
        write_platform_pkg(tmp_src, "foreign", other)
        var lf : LockFile
        t |> equal(0, install_local(tmp_root, "foreign", tmp_src, lf, [force = true]))
        t |> success(has_package(lf, "foreign"))
        t |> success(fexist("{tmp_root}/modules/foreign/.das_package"))
        force_rmdir(tmp_root)
        force_rmdir(tmp_src)
    }
    t |> run("a supported package installs") @(t : T?) {
        let tmp_root = "{get_das_root()}/_daspkg_test_plat_root"
        let tmp_src = "{get_das_root()}/_daspkg_test_plat_src"
        mkdir(tmp_root)
        write_platform_pkg(tmp_src, "native", get_platform_name())
        var lf : LockFile
        t |> equal(0, install_local(tmp_root, "native", tmp_src, lf))
        t |> success(has_package(lf, "native"))
        force_rmdir(tmp_root)
        force_rmdir(tmp_src)
    }
    t |> run("an unsupported dependency refuses the parent and rolls it back") @(t : T?) {
        let tmp_root = "{get_das_root()}/_daspkg_test_plat_root"
        let tmp_src = "{get_das_root()}/_daspkg_test_plat_src"
        let dep_src = "{get_das_root()}/_daspkg_test_plat_dep"
        mkdir(tmp_root)
        write_platform_pkg(dep_src, "foreigndep", other)
        write_passing_pkg(tmp_src, "parent", [dep_src])
        var lf : LockFile
        t |> equal(RC_UNSUPPORTED_PLATFORM, install_local(tmp_root, "parent", tmp_src, lf))
        t |> success(!has_package(lf, "parent"))
        t |> success(!has_package(lf, "_daspkg_test_plat_dep"))
        t |> success(!fexist("{tmp_root}/modules/parent"))
        force_rmdir(tmp_root)
        force_rmdir(tmp_src)
        force_rmdir(dep_src)
    }
    t |> run("--force reaches the dependency") @(t : T?) {
        let tmp_root = "{get_das_root()}/_daspkg_test_plat_root"
        let tmp_src = "{get_das_root()}/_daspkg_test_plat_src"
        let dep_src = "{get_das_root()}/_daspkg_test_plat_dep"
        mkdir(tmp_root)
        write_platform_pkg(dep_src, "foreigndep", other)
        write_passing_pkg(tmp_src, "parent", [dep_src])
        var lf : LockFile
        t |> equal(0, install_local(tmp_root, "parent", tmp_src, lf, [force = true]))
        t |> success(has_package(lf, "parent"))
        t |> success(has_package(lf, "_daspkg_test_plat_dep"))
        force_rmdir(tmp_root)
        force_rmdir(tmp_src)
        force_rmdir(dep_src)
    }
}
