options gen2

require dastest/testing_boost public
require ../file_inspection.das
require daslib/strings_boost

[test]
def test_inspector_language_resolution(t : T?) {
    t |> run("auto syntax resolves compound local filenames") <| @(tt : T?) {
        tt |> equal(inspector_language_for_path("notes.md.local"), "markdown")
        tt |> equal(inspector_language_for_path("DATA.JSON.LOCAL"), "json")
        tt |> equal(inspector_language_for_path("src/widget.cpp"), "cpp")
        tt |> equal(inspector_language_for_path("script.das"), "daslang")
        tt |> equal(inspector_language_for_path("README"), "")
    }
    t |> run("explicit syntax overrides auto and supports plain text") <| @(tt : T?) {
        tt |> equal(inspector_language_for_path("wrong.bin", "C++"), "cpp")
        tt |> equal(inspector_language_for_path("sample.das", "plain"), "")
        tt |> equal(inspector_language_for_path("sample.txt", "markdown"), "markdown")
    }
}

[test]
def test_unified_patch_projection(t : T?) {
    t |> run("unified hunks become aligned Diff and resulting View text") <| @(t : T?) {
        let source = "diff --git a/sample.das b/sample.das\n--- a/sample.das\n+++ b/sample.das\n@@ -1,4 +1,5 @@\n options gen2\n-let old = 1\n+let next = 2\n+let added = 3\n keep()\n-last()\n"
        var inscope parsed <- git_parse_unified_patch(source)
        t |> equal(parsed.error, "")
        t |> equal(parsed.old_text, "options gen2\nlet old = 1\nkeep()\nlast()")
        t |> equal(parsed.new_text, "options gen2\nlet next = 2\nlet added = 3\nkeep()")
        t |> equal(length(parsed.rows), 5)
        t |> equal(length(parsed.hunks), 1)
        t |> equal(parsed.hunks[0].row_start, 0)
        t |> equal(parsed.hunks[0].row_count, 5)
        t |> equal(parsed.rows[1].old_kind, GitDiffLineKind.removed)
        t |> equal(parsed.rows[1].new_kind, GitDiffLineKind.added)
        t |> equal(parsed.rows[2].old_kind, GitDiffLineKind.empty)
        t |> equal(parsed.rows[2].new_kind, GitDiffLineKind.added)
        t |> equal(parsed.rows[4].old_kind, GitDiffLineKind.removed)
        t |> equal(parsed.rows[4].new_kind, GitDiffLineKind.empty)
        t |> equal(length(parsed.new_changed_lines), 2)
        t |> equal(length(parsed.new_deleted_anchors), 1)
        t |> equal(parsed.new_deleted_anchors[0], 4,
            "EOF deletion is anchored after the fourth resulting line")
    }
    t |> run("separated hunks retain navigation boundaries") <| @(tt : T?) {
        let source = "diff --git a/a.das b/a.das\n--- a/a.das\n+++ b/a.das\n@@ -2,2 +2,2 @@\n-old one\n+new one\n keep\n@@ -20 +20 @@\n-old two\n+new two\n"
        var inscope parsed <- git_parse_unified_patch(source)
        tt |> equal(parsed.error, "")
        tt |> equal(length(parsed.hunks), 2)
        tt |> equal(parsed.hunks[0].row_start, 0)
        tt |> equal(parsed.hunks[0].row_count, 2)
        tt |> equal(parsed.hunks[0].old_start, 2)
        tt |> equal(parsed.hunks[0].new_start, 2)
        tt |> equal(parsed.hunks[1].row_start, 2)
        tt |> equal(parsed.hunks[1].row_count, 1)
        tt |> equal(parsed.hunks[1].old_start, 20)
        tt |> equal(parsed.hunks[1].new_start, 20)
    }
    t |> run("header-only Git diffs remain valid") <| @(tt : T?) {
        let source = "diff --git a/old.das b/new.das\nsimilarity index 100%\nrename from old.das\nrename to new.das\n"
        var inscope parsed <- git_parse_unified_patch(source)
        tt |> equal(parsed.error, "")
        tt |> equal(parsed.binary, false)
        tt |> equal(length(parsed.hunks), 0)
        tt |> equal(length(parsed.rows), 0)

        var inscope invalid <- git_parse_unified_patch("not a Git diff")
        tt |> equal(invalid.error, "diff contains no text hunks")
    }
}

[test]
def test_patch_context_injection(t : T?) {
    //! Find reveals hidden matches IN the diff: injected windows must display
    //! like any other area - correct line numbers both sides, hunk offsets
    //! intact, aligned texts rebuilt.
    let patch = "diff --git a/a.das b/a.das\n--- a/a.das\n+++ b/a.das\n@@ -2,3 +2,4 @@\n keep_a\n-old\n+new\n+extra\n keep_b\n"
    let source = "one\nkeep_a\nnew\nextra\nkeep_b\nsix\nseven\neight\nnine\nten\n"
    t |> run("a window after the hunk inserts shifted context rows") <| @(tt : T?) {
        var inscope parsed <- git_parse_unified_patch(patch)
        tt |> equal(parsed.error, "")
        let before_rows = length(parsed.rows)
        git_patch_inject_context(parsed, source, [int2(7, 9)])
        tt |> equal(length(parsed.rows), before_rows + 3)
        let first = parsed.rows[before_rows]
        tt |> equal(first.new_line, 7)
        tt |> equal(first.old_line, 6,
            "the old side is one line behind after the net insertion")
        tt |> equal(first.new_text, "seven")
        tt |> equal(first.old_kind, GitDiffLineKind.context)
        tt |> equal(first.new_kind, GitDiffLineKind.context)
        tt |> success(ends_with(parsed.new_aligned_text, "seven\neight\nnine"),
            "the AFTER pane displays the window")
        tt |> success(ends_with(parsed.old_aligned_text, "seven\neight\nnine"),
            "the BEFORE pane displays the window")
        tt |> equal(parsed.hunks[0].row_start, 0)
    }
    t |> run("a window before the hunk shifts the hunk and skips displayed lines") <| @(tt : T?) {
        var inscope parsed <- git_parse_unified_patch(patch)
        let before_rows = length(parsed.rows)
        git_patch_inject_context(parsed, source, [int2(1, 3)])
        tt |> equal(length(parsed.rows), before_rows + 1,
            "lines 2 and 3 are already displayed")
        tt |> equal(parsed.rows[0].new_line, 1)
        tt |> equal(parsed.rows[0].old_line, 1)
        tt |> equal(parsed.rows[0].new_text, "one")
        tt |> equal(parsed.hunks[0].row_start, 1)
    }
    t |> run("fully displayed windows are a no-op; out-of-range clamps") <| @(tt : T?) {
        var inscope parsed <- git_parse_unified_patch(patch)
        let before_rows = length(parsed.rows)
        git_patch_inject_context(parsed, source, [int2(2, 5)])
        tt |> equal(length(parsed.rows), before_rows)
        git_patch_inject_context(parsed, source, [int2(9, 99)])
        tt |> equal(length(parsed.rows), before_rows + 2,
            "only the real lines nine and ten exist")
        tt |> equal(parsed.rows[length(parsed.rows) - 1].new_text, "ten")
        tt |> equal(parsed.rows[length(parsed.rows) - 1].new_line, 10)
    }
    t |> run("a window between two hunks lands between them") <| @(tt : T?) {
        let two = "diff --git a/a.das b/a.das\n--- a/a.das\n+++ b/a.das\n@@ -2,2 +2,2 @@\n-old one\n+new one\n keep\n@@ -20 +20 @@\n-old two\n+new two\n"
        var inscope lines : array<string>
        for (index in range(1, 23)) {
            lines |> push(index == 2 ? "new one"
                : (index == 3 ? "keep" : (index == 20 ? "new two" : "line {index}")))
        }
        let big_source = join(lines, "\n")
        var inscope parsed <- git_parse_unified_patch(two)
        tt |> equal(parsed.error, "")
        git_patch_inject_context(parsed, big_source, [int2(10, 12)])
        tt |> equal(length(parsed.rows), 6)
        tt |> equal(parsed.rows[2].new_line, 10)
        tt |> equal(parsed.rows[2].old_line, 10,
            "both hunks replace 1:1 - zero offset between them")
        tt |> equal(parsed.hunks[0].row_start, 0)
        tt |> equal(parsed.hunks[1].row_start, 5,
            "the second hunk shifts past the window")
    }
}

[test]
def test_binary_patch_detection(t : T?) {
    t |> run("binary markers are metadata lines, not text payload substrings") <| @(t : T?) {
        let text_patch = "diff --git a/note.txt b/note.txt\n@@ -1 +1 @@\n-old\n+Binary files are discussed here\n+GIT binary patch\n"
        t |> equal(git_patch_is_binary(text_patch), false)
        let binary_patch = "diff --git a/image.png b/image.png\nBinary files a/image.png and b/image.png differ\n"
        t |> equal(git_patch_is_binary(binary_patch), true)
        let literal_patch = "diff --git a/image.png b/image.png\nGIT binary patch\nliteral 0\n"
        t |> equal(git_patch_is_binary(literal_patch), true)
        let deleted_patch = "diff --git a/old.txt b/old.txt\ndeleted file mode 100644\n--- a/old.txt\n+++ /dev/null\n"
        t |> equal(git_patch_result_is_deleted(deleted_patch), true)
        t |> equal(git_patch_result_is_deleted(text_patch), false)
    }
}

[test]
def test_inspector_load_state(t : T?) {
    t |> run("replacement retains ready content until the matching result arrives") <| @(t : T?) {
        var state = InspectorLoadState()
        let first = inspector_load_queue(state, "Queued first")
        t |> equal(first, 1)
        t |> equal(state.request, InspectorRequestState.queued)
        t |> equal(inspector_load_start(state, first), true)
        t |> equal(inspector_load_succeed(state, first), true)
        t |> equal(state.has_retained_content, true)
        t |> equal(state.retained_generation, first)

        let replacement = inspector_load_queue(state, "Queued replacement")
        t |> equal(state.request, InspectorRequestState.queued)
        t |> equal(state.has_retained_content, true)
        t |> equal(state.retained_generation, first)
        t |> equal(inspector_load_start(state, replacement), true)
        t |> equal(inspector_load_fail(state, replacement, "parse failed"), true)
        t |> equal(state.latest_outcome, InspectorContentState.failed)
        t |> equal(state.has_retained_content, true)
        t |> equal(state.retained_generation, first)
        t |> equal(state.error, "parse failed")
    }

    t |> run("superseded worker events cannot mutate the current generation") <| @(t : T?) {
        var state = InspectorLoadState()
        let stale = inspector_load_queue(state)
        let current = inspector_load_queue(state)
        t |> equal(inspector_load_start(state, stale), false)
        t |> equal(inspector_load_succeed(state, stale), false)
        t |> equal(inspector_load_fail(state, stale, "stale failure"), false)
        t |> equal(state.generation, current)
        t |> equal(state.request, InspectorRequestState.queued)
        t |> equal(state.error, "")
        t |> equal(inspector_load_start(state, current), true)
        t |> equal(inspector_load_succeed(state, current), true)
        t |> equal(state.retained_generation, current)
    }

    t |> run("reset invalidates in-flight events and removes retained content") <| @(t : T?) {
        var state = InspectorLoadState()
        let generation = inspector_load_queue(state)
        t |> equal(inspector_load_start(state, generation), true)
        inspector_load_reset(state)
        t |> equal(inspector_load_succeed(state, generation), false)
        t |> equal(state.request, InspectorRequestState.idle)
        t |> equal(state.latest_outcome, InspectorContentState.absent)
        t |> equal(state.has_retained_content, false)
    }

    t |> run("superseding a refresh invalidates work but retains installed content") <| @(t : T?) {
        var state = InspectorLoadState()
        let ready = inspector_load_queue(state)
        let started = inspector_load_start(state, ready)
        let succeeded = inspector_load_succeed(state, ready)
        t |> equal(started && succeeded, true)
        inspector_load_supersede(state, "Requesting diff...")
        t |> equal(state.generation, ready + 1)
        t |> equal(state.request, InspectorRequestState.idle)
        t |> equal(state.has_retained_content, true)
        t |> equal(state.retained_generation, ready)
        t |> equal(state.status, "Requesting diff...")
        t |> equal(inspector_load_fail(state, ready, "stale"), false)
    }
}
