options gen2
options rtti

require dastest/testing_boost public
require ../repository_core.das
require strings

[test]
def test_repository_porcelain_parsing(t : T?) {
    t |> run("Git 2.17 worktree and status porcelain become structured state") <| @(t : T?) {
        let inventory = "worktree D:/Work/daScript\r\nHEAD 0123456789012345678901234567890123456789\r\nbranch refs/heads/main-feature\r\n\r\nworktree D:/Work/daScript/.codex/worktrees/space tree\r\nHEAD abcdefabcdefabcdefabcdefabcdefabcdefabcd\r\ndetached\r\nlocked reason\r\nprunable reason\r\n\r\n"
        var worktrees : array<WorktreeState>
        repository_parse_inventory(inventory, worktrees)
        t |> equal(length(worktrees), 2)
        t |> success(worktrees[0].is_main, "first porcelain entry is the visually distinct main checkout")
        t |> equal(worktrees[0].branch, "main-feature")
        t |> equal(worktrees[1].path, "D:/Work/daScript/.codex/worktrees/space tree")
        t |> success(worktrees[1].detached && worktrees[1].locked && worktrees[1].prunable,
            "detached, locked, and prunable markers are retained")

        var files : array<GitFileState>
        repository_parse_status("## main-feature...origin/main [ahead 2, behind 3]\r\nM  staged file.das\r\n M unstaged file.das\r\n?? new file.txt\r\nUU conflict.txt\r\nR  old name -> new name\r\n M literal -> name.txt\r\n", worktrees[0], files)
        t |> equal(worktrees[0].ahead, 2)
        t |> equal(worktrees[0].behind, 3)
        t |> equal(worktrees[0].staged, 3)
        t |> equal(worktrees[0].unstaged, 3)
        t |> equal(worktrees[0].untracked, 1)
        t |> equal(worktrees[0].conflicted, 1)
        t |> equal(length(files), 6)
        t |> equal(files[4].old_path, "old name")
        t |> equal(files[4].path, "new name")
        t |> equal(files[5].old_path, "")
        t |> equal(files[5].path, "literal -> name.txt")
        t |> equal(files[0].worktree_path, "D:/Work/daScript")
    }
    t |> run("unquoted porcelain retains UTF-8 file paths") <| @(t : T?) {
        var worktree = WorktreeState(path = "D:/Work/fixture")
        var files : array<GitFileState>
        repository_parse_status("## fixture\r\n M unicode-Привет.txt\r\n",
            worktree, files)
        t |> equal(length(files), 1)
        t |> equal(files[0].path, "unicode-Привет.txt")
    }
    t |> run("the status branch line carries the upstream, or says there is none") <| @(t : T?) {
        var worktree = WorktreeState(path = "D:/Work/fixture")
        var files : array<GitFileState>
        repository_parse_status("## topic...borisbat/topic [ahead 2]\r\n",
            worktree, files)
        t |> equal(worktree.upstream_ref, "borisbat/topic")
        // Measured live 2026-07-27 on codex/dogfood-round-2: a branch whose
        // upstream IS the base, 9 ahead. The same worktree reads 81 unmerged
        // against local master and 9 against origin/master, because local
        // master sits 108 behind — the trap this whole design exists for.
        repository_parse_status(
            "## codex/dogfood-round-2...origin/master [ahead 9, behind 36]\r\n",
            worktree, files)
        t |> equal(worktree.upstream_ref, "origin/master")
        t |> equal(worktree.ahead, 9)
        t |> equal(worktree.behind, 36)
        repository_parse_status("## topic...origin/topic\r\n", worktree, files)
        t |> equal(worktree.upstream_ref, "origin/topic",
            "an in-sync branch prints no bracket")
        repository_parse_status("## local-only\r\n", worktree, files)
        t |> equal(worktree.upstream_ref, "",
            "no upstream means nothing else has a copy")
        repository_parse_status("## HEAD (no branch)\r\n", worktree, files)
        t |> equal(worktree.upstream_ref, "")
    }
}

[test]
def test_repository_delete_safety_verdict(t : T?) {
    t |> run("the verdict ladder blocks on the working tree, warns on redundancy") <| @(t : T?) {
        let clean = WorktreeState(path = "D:/Work/tree", branch = "topic",
            upstream_ref = "origin/topic", unmerged_known = true,
            unmerged_base_ref = "origin/master")
        t |> equal(repository_worktree_delete_tier(clean, ""),
            WORKTREE_TIER_GREEN)
        t |> equal(repository_worktree_delete_tier(clean, "dictation"),
            WORKTREE_TIER_IN_USE, "a live session outranks every other fact")

        let main_checkout <- WorktreeState(path = "D:/Work/daScript",
            is_main = true)
        t |> equal(repository_worktree_delete_tier(main_checkout, ""),
            WORKTREE_TIER_MAIN)

        // Untracked alone blocks: those files exist nowhere but this folder.
        let junk <- WorktreeState(path = "D:/Work/tree", untracked = 3,
            unmerged_known = true)
        t |> equal(repository_worktree_delete_tier(junk, ""),
            WORKTREE_TIER_DIRTY)
    }
    t |> run("only-here needs BOTH unmerged and unpushed, and never guesses") <| @(t : T?) {
        // Pushed: the remote has every commit, so removing the folder loses
        // nothing and duplicates nothing.
        var pushed = WorktreeState(unmerged_known = true, unmerged_commits = 3,
            upstream_ref = "origin/topic", ahead = 0)
        t |> success(!repository_worktree_only_here(pushed),
            "commits on an upstream are not 'only here'")
        pushed.ahead = 1
        t |> success(repository_worktree_only_here(pushed),
            "commits ahead of the upstream exist only on this machine")

        let no_upstream <- WorktreeState(unmerged_known = true,
            unmerged_commits = 2)
        t |> success(repository_worktree_only_here(no_upstream),
            "no upstream at all means nothing else has a copy")
        t |> equal(repository_worktree_delete_tier(no_upstream, ""),
            WORKTREE_TIER_WARN, "redundancy warns, it never blocks")

        // The count that could not be measured must not read as a warning OR
        // as a clean bill of health.
        let unknown <- WorktreeState(unmerged_commits = 7)
        t |> success(!repository_worktree_only_here(unknown),
            "an unresolved base never manufactures a warning")
    }
    t |> run("a recognised failure explains itself and says whether retrying helps") <| @(t : T?) {
        // The exact line git produced live on 2026-07-27, retiring
        // D:/Work/daScript-ci-30233791631: it emptied the folder, could not
        // remove the folder itself, and asked an interactive question. Proven
        // non-transient — a second delete attempt failed the same way.
        var hint : string
        var retryable = false
        repository_classify_failure(
            "Deletion of directory 'D:/Work/daScript-ci-30233791631' failed. " +
            "Should I try again? (y/n)", hint, retryable)
        t |> success(!empty(hint), "the locked-folder failure must explain itself")
        t |> success(find(hint, "locked") >= 0,
            "the explanation names the cause, not git's question")
        t |> success(retryable, "a lock can be released, so retry is offered")

        var unknown_hint : string
        var unknown_retryable = true
        repository_classify_failure("something nobody has ever seen",
            unknown_hint, unknown_retryable)
        t |> equal(unknown_hint, "",
            "an unrecognised failure invents no explanation")
        t |> success(!unknown_retryable,
            "and never claims a retry will help when we do not know")
    }
    t |> run("the base ref is the authoritative one, never local master") <| @(t : T?) {
        t |> equal(repository_default_base_ref(RepositoryRecord()),
            "origin/master")
        t |> equal(repository_default_base_ref(
            RepositoryRecord(base_ref = "origin/trunk")), "origin/trunk")
    }
}

[test]
def test_repository_untracked_diff_contract(t : T?) { // nolint:STYLE038
    t |> run("Untracked diffs accept git --no-index exit 1") <| @(t : T?) {
        let file = GitFileState(code = "??", path = "new file.das")
        var argv : array<string>
        var expected_exit_code = 0l
        let error = repository_build_file_diff_argv(
            "D:/Work/tree", file, "working", argv, expected_exit_code)
        t |> equal(error, "")
        t |> equal(expected_exit_code, 1l)
        t |> equal(argv[1], "--literal-pathspecs")
        t |> equal(argv[8], "--no-index")
        t |> equal(argv[10], "--unified=3")
        var forces_text = false
        for (argument in argv) {
            if (argument == "--text") {
                forces_text = true
            }
        }
        t |> equal(forces_text, false, "Git retains native binary detection")
        let null_device = get_platform_name() == "windows" ? "NUL" : "/dev/null"
        t |> equal(argv[length(argv) - 2], null_device)
        t |> equal(argv[length(argv) - 1], "new file.das")

        let staged_file = GitFileState(code = "M ", path = "staged.das")
        expected_exit_code = -1l
        let staged_error = repository_build_file_diff_argv(
            "D:/Work/tree", staged_file, "staged", argv, expected_exit_code)
        t |> equal(staged_error, "")
        t |> equal(expected_exit_code, 0l)
        t |> equal(argv[8], "--cached")
        t |> equal(argv[10], "--unified=3")
        t |> equal(argv[length(argv) - 1], "staged.das")

        let clean_file = GitFileState(code = "  ", path = "clean.das")
        let clean_error = repository_build_file_diff_argv(
            "D:/Work/tree", clean_file, "working", argv,
            expected_exit_code)
        t |> equal(clean_error, "",
            "working inspection permits files with an empty diff")
        t |> equal(expected_exit_code, 0l)
        t |> equal(argv[length(argv) - 1], "clean.das")

        let full_error = repository_build_file_diff_argv(
            "D:/Work/tree", staged_file, "staged", argv,
            expected_exit_code, "full")
        t |> equal(full_error, "")
        t |> equal(argv[10], "--unified=1000000")

        let expanded_error = repository_build_file_diff_argv(
            "D:/Work/tree", staged_file, "staged", argv,
            expected_exit_code, "expanded")
        t |> equal(expanded_error, "")
        t |> equal(argv[10], "--unified=20")

        let magic_file = GitFileState(code = " M", path = ":(glob)**.das")
        let magic_error = repository_build_file_diff_argv(
            "D:/Work/tree", magic_file, "working", argv, expected_exit_code)
        t |> equal(magic_error, "")
        t |> equal(argv[1], "--literal-pathspecs")
        t |> equal(argv[length(argv) - 1], ":(glob)**.das")

        let staged_view_error = repository_staged_file_view_argv(
            "D:/Work/tree", "staged.das", "D:/Temp/staged-view.raw", argv)
        t |> equal(staged_view_error, "")
        t |> success(ends_with(to_lower(argv[0]),
            get_platform_name() == "windows" ? "daslang.exe" : "daslang"))
        t |> success(ends_with(replace(argv[1], "\\", "/"),
            "utils/dasHerd/watcher/git_blob_capture.das"))
        t |> equal(argv[length(argv) - 3], "--revision=:0")
        t |> equal(argv[length(argv) - 2], "--file=staged.das")
        t |> equal(argv[length(argv) - 1],
            "--output=D:/Temp/staged-view.raw")

        let absolute_worktree = (get_platform_name() == "windows"
            ? "D:/Work/trees/debug" : "/tmp/work/trees/debug")
        var create_argv : array<string>
        let create_error = repository_build_worktree_create_argv(
            "D:/Work/main", absolute_worktree, "codex/debug-fix",
            "origin/main", create_argv)
        t |> equal(create_error, "")
        t |> equal(create_argv[0], "git")
        t |> equal(create_argv[4], "add")
        t |> equal(create_argv[5], "-b")
        t |> equal(create_argv[6], "codex/debug-fix")
        t |> equal(create_argv[7], "--")
        t |> equal(create_argv[8], absolute_worktree)
        t |> equal(create_argv[9], "origin/main")
        let normalized_create_error = repository_build_worktree_create_argv(
            "D:/Work/main", absolute_worktree, " codex/debug-fix ",
            " origin/main ", create_argv)
        t |> equal(normalized_create_error, "")
        t |> equal(create_argv[6], "codex/debug-fix")
        t |> equal(create_argv[9], "origin/main")
        let default_base_error = repository_build_worktree_create_argv(
            "D:/Work/main", absolute_worktree, "codex/debug-fix",
            " \r\n ", create_argv)
        t |> success(!empty(default_base_error),
            "base refs containing CR/LF remain invalid after whitespace normalization")
        let blank_base_error = repository_build_worktree_create_argv(
            "D:/Work/main", absolute_worktree, "codex/debug-fix",
            "   ", create_argv)
        t |> equal(blank_base_error, "")
        t |> equal(create_argv[9], "HEAD",
            "a whitespace-only optional base ref keeps the default HEAD contract")
        t |> success(!empty(repository_build_worktree_create_argv(
            "D:/Work/main", absolute_worktree, "   ", "HEAD",
            create_argv)), "whitespace-only worktree branches are rejected before Git")
        t |> success(!empty(repository_build_worktree_create_argv(
            "D:/Work/main", "relative/tree", "codex/debug-fix", "HEAD",
            create_argv)), "dedicated worktree creation requires an exact absolute target")
    }
}

[test]
def test_repository_review_parsing(t : T?) { // nolint:STYLE038
    t |> run("worktree PR commits retain descriptions and identity") <| @(t : T?) {
        var commits : array<GitCommitState>
        repository_parse_review_commits(
            "0123456789abcdef\t01234567\t1721577600\tAgent One\tFirst change\r\n" +
            "abcdef0123456789\tabcdef01\t1721577700\tAgent Two\tSubject\twith tab\n",
            "D:/Work/tree", commits)
        t |> equal(length(commits), 2)
        t |> equal(commits[0].worktree_path, "D:/Work/tree")
        t |> equal(commits[0].short_hash, "01234567")
        t |> equal(commits[0].authored_at, 1721577600l)
        t |> equal(commits[1].subject, "Subject\twith tab")
        var graph_commits : array<GitCommitState>
        repository_parse_review_commits(
            "feedface<|dasHerd-field|>feedface<|dasHerd-field|>1721577800<|dasHerd-field|>Agent<|dasHerd-field|>Merge work<|dasHerd-field|>parent-one parent-two\n",
            "D:/Work/tree", graph_commits)
        t |> equal(length(graph_commits), 1)
        t |> equal(graph_commits[0].subject, "Merge work")
        t |> equal(graph_commits[0].parents, "parent-one parent-two")
        var collision_commits : array<GitCommitState>
        repository_parse_review_commits(
            "deadbeef<|dasHerd-field|>deadbeef<|dasHerd-field|>1721577900<|dasHerd-field|>Agent<|dasHerd-field|>Mention <|dasHerd-field|> literally<|dasHerd-field|>parent-one\n",
            "D:/Work/tree", collision_commits)
        t |> equal(length(collision_commits), 1)
        t |> equal(collision_commits[0].subject,
            "Mention <|dasHerd-field|> literally")
        t |> equal(collision_commits[0].parents, "parent-one")
    }
    t |> run("Git graph history preserves commit and transition rows") <| @(t : T?) {
        var commits : array<GitCommitState>
        var rows : array<GitGraphRowState>
        repository_parse_graph_history(
            "* <|dasHerd-commit|>mergehash<|dasHerd-field|>merge<|dasHerd-field|>1721578000<|dasHerd-field|>Agent One<|dasHerd-field|>Merge topic<|dasHerd-field|>mainhash sidehash\r\n" +
            "|\\  \r\n" +
            "| * <|dasHerd-commit|>sidehash<|dasHerd-field|>side<|dasHerd-field|>1721577900<|dasHerd-field|>Agent Two<|dasHerd-field|>Mention <|dasHerd-field|> literally<|dasHerd-field|>mainhash\n" +
            "|/  \n" +
            "* <|dasHerd-commit|>mainhash<|dasHerd-field|>main<|dasHerd-field|>1721577800<|dasHerd-field|>Agent One<|dasHerd-field|>Main line<|dasHerd-field|>\n",
            "D:/Work/tree", commits, rows)
        t |> equal(length(commits), 3)
        t |> equal(length(rows), 5)
        t |> equal(rows[0].topology, "* ")
        t |> equal(rows[0].commit_hash, "mergehash")
        t |> equal(rows[1].topology, "|\\  ")
        t |> equal(rows[1].commit_hash, "")
        t |> equal(rows[2].topology, "| * ")
        t |> equal(rows[3].topology, "|/  ")
        t |> equal(rows[4].commit_hash, "mainhash")
        t |> equal(commits[1].subject,
            "Mention <|dasHerd-field|> literally")
        t |> equal(commits[0].parents, "mainhash sidehash")
    }
    t |> run("aggregate PR files retain rename provenance") <| @(t : T?) {
        var files : array<GitReviewFileState>
        repository_parse_review_files(
            "M\tsrc/main.das\r\nA\tdocs/new.md\nR095\told name.cpp\tnew name.cpp\n",
            "D:/Work/tree", files)
        t |> equal(length(files), 3)
        t |> equal(files[0].code, "M")
        t |> equal(files[1].path, "docs/new.md")
        t |> equal(files[2].old_path, "old name.cpp")
        t |> equal(files[2].path, "new name.cpp")
        var stats : array<GitReviewFileState>
        repository_parse_review_files(
            "12  3  src/main.das\n0  0  old.cpp => src/new.cpp\n-  -  assets/image.png\n",
            "D:/Work/tree", stats)
        t |> equal(length(stats), 3)
        t |> equal(stats[0].added_lines, 12)
        t |> equal(stats[0].deleted_lines, 3)
        t |> success(stats[0].stats_known)
        t |> equal(stats[1].old_path, "old.cpp")
        t |> equal(stats[1].path, "src/new.cpp")
        t |> success(stats[2].binary)
        t |> success(!stats[2].stats_known)
        stats |> clear()
        repository_parse_review_files(
            "0  0  src/\{old => new\}.cpp\n",
            "D:/Work/tree", stats)
        t |> equal(stats[0].old_path, "src/old.cpp")
        t |> equal(stats[0].path, "src/new.cpp")
    }
    t |> run("aggregate PR diffs compare the merge base with the index") <| @(t : T?) {
        var argv : array<string>
        let error = repository_build_review_file_diff_argv(
            "D:/Work/tree", "0123456789abcdef", "src/main.das", argv, "expanded")
        t |> equal(error, "")
        t |> equal(argv[1], "--literal-pathspecs")
        t |> equal(argv[8], "--cached")
        t |> equal(argv[10], "--unified=20")
        t |> equal(argv[11], "0123456789abcdef")
        t |> equal(argv[length(argv) - 1], "src/main.das")
    }
    t |> run("commit files and refs retain review identity") <| @(t : T?) {
        var files : array<GitCommitFileState>
        repository_parse_commit_files(
            "M\tsrc/main.das\r\nR090\told.md\tnew.md\n",
            "D:/Work/tree", "abcdef0123456789", files)
        t |> equal(length(files), 2)
        t |> equal(files[0].commit_hash, "abcdef0123456789")
        t |> equal(files[1].old_path, "old.md")
        t |> equal(files[1].path, "new.md")
        files |> clear()
        let esc = to_char(27)
        repository_parse_commit_files(watcher_strip_terminal_controls(
            "1{esc}[7C2{esc}[6Csrc/main.das\r\n"),
            "D:/Work/tree", "abcdef0123456789", files)
        t |> equal(length(files), 1)
        t |> equal(files[0].added_lines, 1)
        t |> equal(files[0].deleted_lines, 2)
        t |> equal(files[0].path, "src/main.das")
        files |> clear()
        repository_parse_commit_files(
            "0  0  docs/\{before => after\}.md\n",
            "D:/Work/tree", "abcdef0123456789", files)
        t |> equal(files[0].old_path, "docs/before.md")
        t |> equal(files[0].path, "docs/after.md")
        var refs : array<GitRefState>
        repository_parse_refs(
            "refs/heads/main\tmain\tabcdef\t*\t\torigin/main\n" +
            "refs/remotes/origin/main\torigin/main\tabcdef\t \t\t\n" +
            "refs/remotes/origin/HEAD\torigin/HEAD\tabcdef\t \trefs/remotes/origin/main\n",
            "D:/Work/tree", refs)
        t |> equal(length(refs), 2)
        t |> success(refs[0].current)
        t |> success(!refs[0].remote)
        t |> equal(refs[0].upstream, "origin/main")
        t |> success(refs[1].remote)
    }
    t |> run("working numstat updates only matching worktree files") <| @(t : T?) {
        var files : array<GitFileState> <- [
            GitFileState(worktree_path = "D:/Work/tree", code = " M",
                path = "src/main.das"),
            GitFileState(worktree_path = "D:/Work/other", code = " M",
                path = "src/main.das")]
        repository_apply_working_numstat(
            "9  4  src/main.das\n", "D:/Work/tree", files)
        t |> equal(files[0].added_lines, 9)
        t |> equal(files[0].deleted_lines, 4)
        t |> success(files[0].stats_known)
        t |> success(!files[1].stats_known)
    }
    t |> run("commit inspection uses one observed revision") <| @(t : T?) {
        var argv : array<string>
        var expected_exit_code = 0l
        t |> equal(repository_file_diff_argv(
            "missing", "D:/Work/tree", "src/main.das", "commit",
            argv, expected_exit_code, "compact", ""),
            "commit revision is required")
        t |> equal(repository_build_commit_file_diff_argv(
            "D:/Work/tree", "abcdef0123456789", "src/main.das",
            argv, "expanded"), "")
        t |> equal(argv[1], "--literal-pathspecs")
        t |> equal(argv[7], "show")
        t |> equal(argv[12], "--unified=20")
        t |> equal(argv[13], "abcdef0123456789")
        t |> equal(argv[length(argv) - 1], "src/main.das")
        t |> equal(repository_commit_file_view_argv(
            "D:/Work/tree", "abcdef0123456789", "src/main.das",
            "D:/Temp/view.raw", argv), "")
        t |> equal(argv[length(argv) - 3],
            "--revision=abcdef0123456789")
        t |> equal(argv[length(argv) - 2], "--file=src/main.das")
        t |> equal(argv[length(argv) - 1], "--output=D:/Temp/view.raw")
    }
    t |> run("file staging actions use literal Git paths") <| @(t : T?) {
        var argv : array<string>
        t |> equal(repository_build_file_action_argv(
            "D:/Work/tree", ":(glob)**.das", "stage", argv), "")
        t |> equal(argv[1], "--literal-pathspecs")
        t |> equal(argv[4], "add")
        t |> equal(argv[length(argv) - 1], ":(glob)**.das")
        t |> equal(repository_build_file_action_argv(
            "D:/Work/tree", "src/main.das", "unstage", argv), "")
        t |> equal(argv[4], "reset")
        t |> equal(argv[5], "--quiet")
        t |> equal(argv[6], "HEAD")
        t |> success(!empty(repository_build_file_action_argv(
            "D:/Work/tree", "../outside.txt", "stage", argv)))
    }
}

[test]
def test_repository_operation_step_lists(t : T?) { // nolint:STYLE038
    t |> run("retire builds the precheck/remove list with its recovery graph") <| @(t : T?) {
        var steps : array<WorktreeOperationStep>
        var recoveries : table<string; array<WorktreeOperationStep>>
        repository_operation_remove_steps("D:/Work/main", "D:/Work/tree",
            false, steps, recoveries)
        t |> equal(length(steps), 2)
        t |> equal(steps[0].argv[1], "-C")
        t |> equal(steps[0].argv[2], "D:/Work/tree",
            "the dirty precheck runs in the WORKTREE, not the checkout")
        t |> equal(steps[0].argv[3], "status")
        t |> equal(steps[0].expectation, "empty_output")
        t |> equal(steps[0].expect_message,
            "uncommitted or untracked work is present")
        t |> equal(steps[0].recover_match, "not a git repository")
        t |> equal(steps[0].recover_name, "force_remove")
        t |> equal(steps[1].argv[2], "D:/Work/main",
            "the removal runs from the main checkout")
        t |> equal(steps[1].argv[4], "remove")
        t |> equal(steps[1].argv[length(steps[1].argv) - 1], "D:/Work/tree")
        t |> equal(steps[1].recover_match, "validation failed")
        t |> equal(steps[1].recover_name, "prune")
        t |> success(key_exists(recoveries, "force_remove"))
        t |> success(key_exists(recoveries, "prune"))
        t |> equal(length(recoveries["force_remove"]), 1)
        t |> equal(recoveries["force_remove"][0].argv[5], "--force")
        t |> equal(recoveries["force_remove"][0].recover_name, "prune",
            "the forced fallback still recovers into prune")
        t |> equal(length(recoveries["prune"]), 1)
        t |> equal(recoveries["prune"][0].argv[4], "prune")
        t |> success(recoveries["prune"][0].require_folder_gone,
            "prune success still fails honestly when the folder survives")
        delete steps
        delete recoveries
    }
    t |> run("forced retire is one step and keeps the prune fallback") <| @(t : T?) {
        var steps : array<WorktreeOperationStep>
        var recoveries : table<string; array<WorktreeOperationStep>>
        repository_operation_remove_steps("D:/Work/main", "D:/Work/tree",
            true, steps, recoveries)
        t |> equal(length(steps), 1)
        t |> equal(steps[0].argv[5], "--force")
        t |> equal(steps[0].recover_name, "prune")
        t |> success(!key_exists(recoveries, "force_remove"),
            "an already-forced removal has no forced fallback")
        t |> success(key_exists(recoveries, "prune"))
        delete steps
        delete recoveries
    }
    t |> run("multi-path actions build one literal argv or none at all") <| @(t : T?) {
        var argv : array<string>
        let paths <- ["src/main.das", "with space.txt", ":(glob)**.das"]
        t |> equal(repository_build_files_action_argv(
            "D:/Work/tree", paths, "stage", argv), "")
        t |> equal(argv[1], "--literal-pathspecs")
        t |> equal(argv[4], "add")
        t |> equal(argv[5], "--")
        t |> equal(argv[6], "src/main.das")
        t |> equal(argv[7], "with space.txt")
        t |> equal(argv[8], ":(glob)**.das")
        t |> equal(repository_build_files_action_argv(
            "D:/Work/tree", paths, "unstage", argv), "")
        t |> equal(argv[4], "reset")
        t |> equal(argv[6], "HEAD")
        t |> equal(argv[7], "--")
        t |> equal(argv[length(argv) - 1], ":(glob)**.das")
        // One bad path fails the WHOLE action — never a partial argv.
        let poisoned <- ["src/main.das", "../outside.txt"]
        t |> success(!empty(repository_build_files_action_argv(
            "D:/Work/tree", poisoned, "stage", argv)))
        t |> equal(length(argv), 0, "a rejected action leaves argv empty")
        let no_paths : array<string>
        t |> success(!empty(repository_build_files_action_argv(
            "D:/Work/tree", no_paths, "stage", argv)))
        t |> success(!empty(repository_build_files_action_argv(
            "D:/Work/tree", paths, "sideload", argv)),
            "unknown actions still error rather than building an argv")
    }
    t |> run("changelist actions are one rail step with a counted label") <| @(t : T?) {
        var steps : array<WorktreeOperationStep>
        let paths <- ["a.das", "b.das"]
        t |> equal(repository_operation_changelist_steps(
            "D:/Work/tree", "stage", paths, steps), "")
        t |> equal(length(steps), 1)
        t |> equal(steps[0].step_label, "staging 2 files")
        t |> equal(steps[0].kind, "git-changelist-stage")
        t |> equal(steps[0].argv[4], "add")
        let single <- ["a.das"]
        t |> equal(repository_operation_changelist_steps(
            "D:/Work/tree", "unstage", single, steps), "")
        t |> equal(steps[0].step_label, "unstaging 1 file")
        t |> equal(steps[0].argv[4], "reset")
        let poisoned <- ["../outside.txt"]
        t |> success(!empty(repository_operation_changelist_steps(
            "D:/Work/tree", "stage", poisoned, steps)))
        t |> equal(length(steps), 0)
        delete steps
    }
    t |> run("the three discard spellings stay distinct - confusing them destroys staged work") <| @(t : T?) {
        var argv : array<string>
        let paths <- ["src/main.das"]
        t |> equal(repository_build_files_action_argv(
            "D:/Work/tree", paths, "discard_worktree", argv), "")
        t |> equal(argv[4], "checkout")
        t |> equal(argv[5], "--",
            "discard_worktree restores from the INDEX - a staged part survives")
        t |> equal(repository_build_files_action_argv(
            "D:/Work/tree", paths, "discard_all", argv), "")
        t |> equal(argv[4], "checkout")
        t |> equal(argv[5], "HEAD",
            "discard_all resets index AND worktree to HEAD")
        t |> equal(argv[6], "--")
        t |> equal(repository_build_files_action_argv(
            "D:/Work/tree", paths, "discard_untracked", argv), "")
        t |> equal(argv[4], "clean")
        t |> equal(argv[5], "-f")
        t |> equal(argv[6], "--", "no directory row, no -d")
        let with_dir <- ["stray.txt", "junk/"]
        t |> equal(repository_build_files_action_argv(
            "D:/Work/tree", with_dir, "discard_untracked", argv), "")
        t |> equal(argv[6], "-d",
            "a dir/ row adds -d - clean refuses directories without it")
        t |> equal(argv[7], "--")
    }
    t |> run("discard builds one operation with a step per class, staged first") <| @(t : T?) {
        var steps : array<WorktreeOperationStep>
        let modified <- ["m.das"]
        let staged <- ["s.das"]
        let untracked <- ["u.txt"]
        t |> equal(repository_operation_discard_steps("D:/Work/tree",
            modified, staged, untracked, steps), "")
        t |> equal(length(steps), 3)
        t |> equal(steps[0].step_label, "discarding 1 staged file")
        t |> equal(steps[0].argv[5], "HEAD")
        t |> equal(steps[1].step_label, "discarding 1 edited file")
        t |> equal(steps[1].argv[5], "--")
        t |> equal(steps[2].step_label, "deleting 1 untracked file")
        t |> equal(steps[2].argv[4], "clean")
        let no_paths : array<string>
        t |> equal(repository_operation_discard_steps("D:/Work/tree",
            no_paths, staged, no_paths, steps), "")
        t |> equal(length(steps), 1, "empty classes contribute no step")
        t |> success(!empty(repository_operation_discard_steps("D:/Work/tree",
            no_paths, no_paths, no_paths, steps)),
            "nothing to discard is an error, not an empty operation")
        delete steps
    }
    t |> run("a real commit takes the staged set only and runs the hooks") <| @(t : T?) {
        var steps : array<WorktreeOperationStep>
        t |> equal(repository_operation_commit_steps("D:/Work/tree",
            "a real message", steps), "")
        t |> equal(length(steps), 1)
        t |> equal(steps[0].argv[3], "commit")
        t |> equal(steps[0].argv[4], "-m")
        t |> equal(steps[0].argv[5], "a real message")
        for (argument in steps[0].argv) {
            t |> success(argument != "--no-verify",
                "a real commit must run the repository's hooks")
            t |> success(argument != "-a",
                "commit never silently widens beyond what is staged")
        }
        t |> success(!empty(repository_operation_commit_steps("D:/Work/tree",
            "   ", steps)), "a blank message is refused before git sees it")
        delete steps
    }
    t |> run("network operations build guarded steps: ff-only pull, explicit publish") <| @(t : T?) {
        var steps : array<WorktreeOperationStep>
        t |> equal(repository_operation_network_steps("D:/Work/tree", "sync",
            "topic", "origin/topic", steps), "")
        t |> equal(length(steps), 3, "sync is fetch, then pull, then push")
        t |> equal(steps[0].argv[3], "fetch")
        t |> equal(steps[0].argv[4], "--prune")
        t |> equal(steps[1].argv[3], "pull")
        t |> equal(steps[1].argv[4], "--ff-only",
            "NEVER a bare pull - a surprise merge commit is a silent lie")
        t |> equal(steps[2].argv[3], "push")
        t |> equal(length(steps[2].argv), 4,
            "an existing upstream gets a plain push")
        for (step in steps) {
            t |> equal(step.timeout_ms, 120000l,
                "network git gets its own budget, not the local 15s")
            t |> equal(length(step.env), 1)
            t |> equal(step.env[0], "GIT_TERMINAL_PROMPT=0",
                "a credential prompt must fail fast, not eat the timeout")
        }
        t |> equal(repository_operation_network_steps("D:/Work/tree", "push",
            "topic", "", steps), "")
        t |> equal(length(steps), 1)
        t |> equal(steps[0].argv[4], "-u")
        t |> equal(steps[0].argv[5], "origin")
        t |> equal(steps[0].argv[6], "topic",
            "no upstream means an explicit publish naming the remote")
        t |> success(!empty(repository_operation_network_steps("D:/Work/tree",
            "push", "", "", steps)), "detached HEAD cannot push")
        t |> success(!empty(repository_operation_network_steps("D:/Work/tree",
            "teleport", "topic", "origin/topic", steps)),
            "unknown network actions error")
        delete steps
    }
    t |> run("network failures classify: what happened, and whether retry helps") <| @(t : T?) {
        var hint : string
        var retryable = true
        repository_classify_failure(
            "! [rejected] topic -> topic (non-fast-forward)", hint, retryable)
        t |> success(!empty(hint), "non-fast-forward explains itself")
        t |> success(!retryable,
            "retry fails identically until the user rebases - not retryable")
        retryable = true
        repository_classify_failure(
            "fatal: Not possible to fast-forward, aborting.", hint, retryable)
        t |> success(!empty(hint), "the ff-only pull refusal is recognised")
        t |> success(!retryable)
        retryable = true
        repository_classify_failure(
            "fatal: The current branch topic has no upstream branch.",
            hint, retryable)
        t |> success(!empty(hint))
        t |> success(find(hint, "ublish") >= 0,
            "the hint points at Publish, the button that fixes it")
        t |> success(!retryable)
        retryable = true
        repository_classify_failure(
            "fatal: could not read Username for 'https://github.com': " +
            "terminal prompts disabled", hint, retryable)
        t |> success(!empty(hint), "auth failures explain themselves")
        t |> success(!retryable, "auth retries fail identically")
        retryable = false
        repository_classify_failure(
            "fatal: unable to access 'https://github.com/x.git/': " +
            "Could not resolve host", hint, retryable)
        t |> success(!empty(hint))
        t |> success(retryable, "a network blip is worth retrying")
    }
    t |> run("the WIP rescue stages everything then commits, hooks bypassed") <| @(t : T?) {
        var steps : array<WorktreeOperationStep>
        repository_operation_commit_wip_steps("D:/Work/tree", "rescue message",
            steps)
        t |> equal(length(steps), 2)
        t |> equal(steps[0].argv[3], "add")
        t |> equal(steps[0].argv[4], "-A")
        t |> equal(steps[1].argv[3], "commit")
        t |> equal(steps[1].argv[4], "--no-verify",
            "the rescue must not be blocked by a hook")
        t |> equal(steps[1].argv[length(steps[1].argv) - 1], "rescue message")
        t |> equal(steps[0].timeout_ms, 0l,
            "local git keeps the default timeout")
        delete steps
    }
}

[test]
def test_repository_relative_file_path_contract(t : T?) {
    t |> run("file previews cannot escape their observed worktree") <| @(t : T?) {
        t |> success(repository_valid_relative_file_path("src/main.das"))
        t |> success(!repository_valid_relative_file_path("../outside.txt"))
        t |> success(!repository_valid_relative_file_path("src/../../outside.txt"))
        t |> success(!repository_valid_relative_file_path("/etc/passwd"))
        t |> success(!repository_valid_relative_file_path("\\\\server\\share\\file.txt"))
        if (get_platform_name() == "windows") {
            t |> success(!repository_valid_relative_file_path("C:\\Windows\\win.ini"))
            t |> success(!repository_valid_relative_file_path("C:drive-relative.txt"))
        }
    }
}

[test]
def test_repository_async_lifecycle_contract(t : T?) {
    t |> run("working-numstat launch failures remain refresh failures") <| @(t : T?) {
        t |> equal(repository_phase_family(1), "refresh")
        t |> equal(repository_phase_family(2), "refresh")
        t |> equal(repository_phase_family(10), "refresh")
        t |> equal(repository_phase_family(3), "review")
        t |> equal(repository_phase_family(6), "review")
        t |> equal(repository_phase_family(7), "history")
        t |> equal(repository_phase_family(9), "history")
    }
    t |> run("inventory replacement releases removed-worktree payloads") <| @(t : T?) {
        let kept = "D:/Work/main"
        let removed = "D:/Work/removed"
        var snapshot = RepositorySnapshot()
        snapshot.files <- [
            GitFileState(worktree_path = kept, path = "keep.txt"),
            GitFileState(worktree_path = removed, path = "drop.txt")]
        snapshot.review_commits <- [
            GitCommitState(worktree_path = kept, hash = "keep"),
            GitCommitState(worktree_path = removed, hash = "drop")]
        snapshot.review_files <- [
            GitReviewFileState(worktree_path = kept, path = "keep.txt"),
            GitReviewFileState(worktree_path = removed, path = "drop.txt")]
        snapshot.history_commits <- [
            GitCommitState(worktree_path = kept, hash = "keep"),
            GitCommitState(worktree_path = removed, hash = "drop")]
        snapshot.graph_rows <- [
            GitGraphRowState(worktree_path = kept, commit_hash = "keep"),
            GitGraphRowState(worktree_path = removed, commit_hash = "drop")]
        snapshot.commit_files <- [
            GitCommitFileState(worktree_path = kept, path = "keep.txt"),
            GitCommitFileState(worktree_path = removed, path = "drop.txt")]
        snapshot.refs <- [
            GitRefState(worktree_path = kept, name = "main"),
            GitRefState(worktree_path = removed, name = "old")]
        let observed : array<WorktreeState> <- [WorktreeState(path = kept)]

        repository_prune_removed_worktree_data(snapshot, observed)

        t |> equal(length(snapshot.files), 1)
        t |> equal(length(snapshot.review_commits), 1)
        t |> equal(length(snapshot.review_files), 1)
        t |> equal(length(snapshot.history_commits), 1)
        t |> equal(length(snapshot.graph_rows), 1)
        t |> equal(length(snapshot.commit_files), 1)
        t |> equal(length(snapshot.refs), 1)
        t |> equal(snapshot.files[0].worktree_path, kept)
        t |> equal(snapshot.refs[0].worktree_path, kept)

        delete snapshot.files
        delete snapshot.review_commits
        delete snapshot.review_files
        delete snapshot.history_commits
        delete snapshot.graph_rows
        delete snapshot.commit_files
        delete snapshot.refs
    }
}

[test]
def test_repository_worktree_identity(t : T?) {
    t |> run("main-checkout identity folds case on Windows only") <| @(t : T?) {
        let temp = create_temp_directory_result("dasherd_repo_identity_")
        if (!(temp is value)) {
            t |> failure("could not create temporary test directory")
            return
        }
        let root = temp as value
        t |> success(repository_same_worktree_identity(root, root),
            "a path is its own identity")
        // Case-only difference: equal under Windows' case-insensitive
        // filesystem, distinct everywhere else (the #3574 review fix - an
        // unconditional fold refused legitimate deletions as \"main\").
        let probe_upper = path_join(root, "CaseProbe")
        let probe_lower = path_join(root, "caseprobe")
        let case_folded = get_platform_name() == "windows"
        t |> equal(repository_same_worktree_identity(probe_upper, probe_lower), case_folded)
        t |> equal(repository_same_worktree_identity(probe_lower, probe_upper), case_folded)
        // Slash direction never separates identities (normalized_path).
        t |> success(repository_same_worktree_identity(
                replace(root, "\\", "/"), root),
            "separator spelling does not split the identity")
        t |> success(!repository_same_worktree_identity(
                path_join(root, "elsewhere"), root),
            "distinct paths stay distinct")
        rmdir_rec(root)
    }
}

[test]
def test_repository_text_read_contract(t : T?) {
    t |> run("empty files remain distinct from read failures") <| @(t : T?) {
        let temp = create_temp_directory_result("dasherd_repository_text_")
        if (!(temp is value)) {
            t |> failure("could not create temporary test directory")
            return
        }
        let root = temp as value
        let empty_path = path_join(root, "empty.txt")
        t |> success(fwrite(empty_path, ""), "empty fixture is written")

        var text : string
        t |> equal(repository_read_text_file(empty_path, 1024, text), "")
        t |> equal(text, "", "an opened empty file is a successful preview")
        t |> success(!empty(repository_read_text_file(root, 1024, text)),
            "a path that cannot be read as a regular file reports an error")

        let bytes_path = path_join(root, "bytes.bin")
        t |> success(fwrite(bytes_path, "abc"), "byte fixture is written")
        var preview_path = ""
        t |> equal(repository_working_preview_path(
            root, "bytes.bin", preview_path), "")
        t |> equal(preview_path, bytes_path)
        let unicode_name = "unicode-Привет.txt"
        let unicode_path = path_join(root, unicode_name)
        t |> success(fwrite(unicode_path, "hello"),
            "UTF-8 path fixture is written")
        t |> equal(repository_working_preview_path(
            root, unicode_name, preview_path), "")
        t |> equal(preview_path, unicode_path)
        t |> equal(repository_read_text_file(bytes_path, 2, text),
            "file exceeds the raw-byte View limit")
        t |> equal(text, "", "oversized mapped text is never materialized")
        t |> equal(repository_read_text_file(bytes_path, 3, text), "")
        t |> equal(text, "abc", "mapped text at the raw-byte limit is materialized")
        var bytes : array<uint8>
        var byte_count = 0
        t |> equal(repository_read_file_bytes(
            bytes_path, 3, bytes, byte_count), "")
        t |> equal(byte_count, 3)
        t |> equal(string(bytes), "abc")

        rmdir_rec(root)
    }
}
