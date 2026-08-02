options gen2

require dastest/testing_boost public
require ../repository_core.das

// The one test that proves the changelist argv table against REAL git rather
// than merely self-consistent: a scratch repository, created and torn down
// here, runs a stage -> commit -> discard cycle through the same argv
// builders the watcher executes, asserting `git status --porcelain` after
// each act. Everything destructive happens ONLY inside the temp directory.

def private run_git(t : T?; argv : array<string>; tag : string) : string {
    var output = ""
    let exit_code = unsafe(popen_argv(argv, 30.0, $(file) {
        if (file != null) {
            output = fread(file)
        }
    }))
    t |> equal(exit_code, 0, tag)
    return output
}

def private porcelain(t : T?; root : string) : string {
    return strip(run_git(t, ["git", "--literal-pathspecs", "-C", root,
        "status", "--porcelain"], "git status runs"))
}

def private run_action(t : T?; root : string; paths : array<string>;
                       action : string) {
    var argv : array<string>
    t |> equal(repository_build_files_action_argv(root, paths, action, argv),
        "", "{action} argv builds")
    let _ = run_git(t, argv, "{action} runs")
}

[test]
def test_changelist_real_git_cycle(t : T?) {
    t |> run("stage, commit, and every discard flavor against a real repository") <| @(t : T?) {
        let temp = create_temp_directory_result("dasherd_changelist_cycle_")
        if (!(temp is value)) {
            t |> failure("could not create temporary test directory")
            return
        }
        let root = temp as value
        let _init = run_git(t, ["git", "init", "-q", root], "git init runs")
        // Commits need an identity; keep it repo-local, never touch the user's.
        let _email = run_git(t, ["git", "-C", root, "config", "user.email",
            "dasherd-test@example.com"], "git config email")
        let _name = run_git(t, ["git", "-C", root, "config", "user.name",
            "dasHerd cycle test"], "git config name")
        // Content asserts compare bytes; keep git's hands off the line
        // endings or a Windows autocrlf=true turns every restore into CRLF.
        let _crlf = run_git(t, ["git", "-C", root, "config", "core.autocrlf",
            "false"], "git config autocrlf")

        t |> success(fwrite(path_join(root, "a.txt"), "one\n"), "a.txt written")
        t |> success(fwrite(path_join(root, "b.txt"), "base\n"), "b.txt written")

        // Stage through the multi-path builder, then commit through the WIP
        // step list - the exact argvs the operation rail launches.
        run_action(t, root, ["a.txt", "b.txt"], "stage")
        t |> equal(porcelain(t, root), "A  a.txt\nA  b.txt")
        var wip_steps : array<WorktreeOperationStep>
        repository_operation_commit_wip_steps(root, "initial state", wip_steps)
        for (step in wip_steps) {
            let _ = run_git(t, step.argv, "WIP step '{step.step_label}' runs")
        }
        t |> equal(porcelain(t, root), "", "the tree is clean after the commit")

        // The MM shape: a staged edit plus a NEWER unstaged edit on one file.
        t |> success(fwrite(path_join(root, "a.txt"), "staged edit\n"),
            "staged edit written")
        run_action(t, root, ["a.txt"], "stage")
        t |> success(fwrite(path_join(root, "a.txt"), "unstaged edit\n"),
            "unstaged edit written")
        t |> equal(porcelain(t, root), "MM a.txt")

        // discard_worktree restores from the INDEX: the staged half SURVIVES.
        // A regression that spells this `checkout HEAD` destroys staged work,
        // which is why the content is asserted, not just the status code.
        run_action(t, root, ["a.txt"], "discard_worktree")
        t |> equal(porcelain(t, root), "M  a.txt")
        var restored = ""
        t |> equal(repository_read_text_file(path_join(root, "a.txt"), 1024,
            restored), "")
        t |> equal(restored, "staged edit\n",
            "the worktree now holds the staged version, not HEAD")

        // discard_all throws the change away entirely - index AND worktree.
        run_action(t, root, ["a.txt"], "discard_all")
        t |> equal(porcelain(t, root), "")
        var reverted = ""
        t |> equal(repository_read_text_file(path_join(root, "a.txt"), 1024,
            reverted), "")
        t |> equal(reverted, "one\n", "the file is back at HEAD")

        // discard_untracked deletes from disk; a dir/ row needs -d.
        var mkdir_error = ""
        t |> success(mkdir_rec(path_join(root, "junk"), mkdir_error),
            "junk/ created")
        t |> success(fwrite(path_join(root, "junk/inner.txt"), "x\n"),
            "junk/inner.txt written")
        t |> success(fwrite(path_join(root, "stray.txt"), "y\n"),
            "stray.txt written")
        t |> equal(porcelain(t, root), "?? junk/\n?? stray.txt")
        run_action(t, root, ["junk/", "stray.txt"], "discard_untracked")
        t |> equal(porcelain(t, root), "",
            "clean removed the directory and the stray file")
        t |> success(!fexist(path_join(root, "junk/inner.txt")),
            "the directory's contents are gone from disk")

        rmdir_rec(root)
    }
}
