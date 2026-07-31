options gen2
options gc
options persistent_heap

require dastest/testing_boost public
require ../file_inspection.das
require ../inspection_worker.das
require daslib/jobque_boost
require daslib/utf8_utils
require daslib/base64
require daslib/fio
require imgui/imgui_text_tree_sitter
require daslib/strings_boost

[test]
def test_inspection_prepare_worker(t : T?) { // nolint:STYLE038
    t |> run("installed syntax providers highlight representative Diff sources") <| @(tt : T?) {
        var cpp <- tree_sitter_source_document(
            "#include <string>\n\nstatic std::string greeting() \{\n    const auto value = std::string\{\"working tree\"\};\n    return value;\n\}\n",
            "cpp", 1)
        tt |> equal(length(cpp.spans) > 1, true)
        text_source_document_dispose(cpp)
    }
    t |> run("compact Diff projects syntax from complete files") <| @(tt : T?) {
        let complete_new = "options gen2\n\nstruct Sample \{\n    value : int\n\}\n\ndef answer(sample : Sample) : int \{\n    return sample.value + 1\n\}\n"
        let compact_patch = "diff --git a/a.das b/a.das\n--- a/a.das\n+++ b/a.das\n@@ -8 +8 @@\n-    return sample.value\n+    return sample.value + 1\n"
        var parsed <- git_parse_unified_patch(compact_patch)
        let complete_old = inspector_reconstruct_old_source(complete_new, parsed)
        tt |> equal(complete_old,
            "options gen2\n\nstruct Sample \{\n    value : int\n\}\n\ndef answer(sample : Sample) : int \{\n    return sample.value\n\}\n")
        var full_old <- tree_sitter_source_document(complete_old, "daslang", 21)
        var full_new <- tree_sitter_source_document(complete_new, "daslang", 21)
        var compact_old <- inspector_project_full_syntax(full_old,
            parsed.old_aligned_text, parsed.rows, true, 21)
        var compact_new <- inspector_project_full_syntax(full_new,
            parsed.new_aligned_text, parsed.rows, false, 21)
        tt |> success(length(compact_old.spans) > 1,
            "old compact row retains complete-file syntax")
        tt |> success(length(compact_new.spans) > 1,
            "new compact row retains complete-file syntax")
        var saw_keyword = false
        for (span in compact_new.spans) {
            if (span.syntax_style == TextSourceSyntaxStyle.keyword) {
                saw_keyword = true
            }
        }
        tt |> success(saw_keyword,
            "the return keyword remains highlighted outside full context")
        text_source_document_dispose(compact_old)
        text_source_document_dispose(compact_new)
        text_source_document_dispose(full_old)
        text_source_document_dispose(full_new)
        git_parsed_patch_dispose(parsed)
    }
    t |> run("worker prepares syntax off-thread and shuts down cleanly") <| @(t : T?) {
        t |> equal(is_utf8_string_valid("Καλημέρα, мир, こんにちは, 🐑"), true)
        create_job_que()
        var worker = InspectorPrepareWorker()
        t |> equal(inspector_prepare_worker_start(worker), true)
        t |> equal(inspector_prepare_worker_queue(worker, 7,
            "diff --git a/a.das b/a.das\n--- a/a.das\n+++ b/a.das\n@@ -1 +1 @@\n-let answer = 41\n+let answer = \"🐑\"\n",
            "options gen2\nlet answer = \"🐑\"\n", "das"), true)

        var ready = false
        var started = false
        var completed = false
        var span_count = 0
        let wait_started = ref_time_ticks()
        while (!completed && int64(get_time_usec(wait_started)) < 5000000l) {
            var event : InspectorPrepareEvent
            if (inspector_prepare_worker_take_event(worker, event)) {
                if (event.kind == InspectorPrepareEventKind.ready) {
                    ready = true
                } elif (event.kind == InspectorPrepareEventKind.started) {
                    started = true
                } elif (event.kind == InspectorPrepareEventKind.completed) {
                    completed = true
                    span_count = length(event.new_document.spans)
                    t |> equal(event.generation, 7)
                    t |> equal(length(event.parsed.rows), 1)
                    t |> equal(length(event.old_document.line_numbers), 1)
                    t |> equal(length(event.new_document.line_numbers), 1)
                    t |> equal(event.old_document.line_numbers[0], 1)
                    t |> equal(event.new_document.line_numbers[0], 1)
                    t |> equal(event.view_source,
                        "options gen2\nlet answer = \"🐑\"\n")
                    t |> success(length(event.view_document.spans) > 1,
                        "full View syntax is prepared by the worker")
                    t |> equal(length(event.view_document.line_marks), 1,
                        "full View change marks are prepared by the worker")
                }
            }
        }
        t |> equal(ready, true)
        t |> equal(started, true)
        t |> equal(completed, true)
        t |> equal(span_count > 1, true)
        t |> equal(inspector_prepare_worker_shutdown(worker), true)
        destroy_job_que()
    }
    t |> run("worker prepares Markdown View parsing and fenced syntax") <| @(tt : T?) {
        create_job_que()
        var worker = InspectorPrepareWorker()
        tt |> equal(inspector_prepare_worker_start(worker), true)
        let patch = "diff --git a/readme.md b/readme.md\n--- a/readme.md\n+++ b/readme.md\n@@ -1,5 +1,5 @@\n-# Old\n+# New\n \n ```das\n let answer = 42\n ```\n"
        let view = "# New\n\n```das\nlet answer = 42\n```\n"
        tt |> equal(inspector_prepare_worker_queue(
            worker, 9, patch, view, "markdown"), true)
        var completed = false
        let wait_started = ref_time_ticks()
        while (!completed && int64(get_time_usec(wait_started)) < 5000000l) {
            inspector_prepare_worker_try_event(worker) $(event : InspectorPrepareEvent const) {
                if (event.kind == InspectorPrepareEventKind.completed) {
                    completed = true
                    tt |> success(event.markdown_document.ok,
                        "Markdown View parses off-thread")
                    tt |> success(length(event.view_document.spans) > 1,
                        "Markdown source View is styled off-thread")
                    tt |> equal(length(event.changed_source_ranges), 1,
                        "Markdown change gutter ranges are prepared off-thread")
                    tt |> success(!empty(event.markdown_view.syntax_bindings),
                        "fenced code syntax is prepared off-thread")
                }
            }
        }
        tt |> equal(completed, true)
        tt |> equal(inspector_prepare_worker_shutdown(worker), true)
        destroy_job_que()
    }
    t |> run("worker reports malformed patches and still shuts down") <| @(tt : T?) {
        create_job_que()
        var worker = InspectorPrepareWorker()
        tt |> equal(inspector_prepare_worker_start(worker), true)
        tt |> equal(inspector_prepare_worker_queue(worker, 11,
            "not a unified patch", "", "das"), true)
        var failed = false
        var failure = ""
        let wait_started = ref_time_ticks()
        while (!failed && int64(get_time_usec(wait_started)) < 5000000l) {
            inspector_prepare_worker_try_event(worker) $(event : InspectorPrepareEvent const) {
                if (event.kind == InspectorPrepareEventKind.failed) {
                    failed = true
                    failure = clone_string(event.error)
                    tt |> equal(event.generation, 11)
                }
            }
        }
        tt |> equal(failed, true)
        tt |> equal(empty(failure), false)
        tt |> equal(inspector_prepare_worker_shutdown(worker), true)
        destroy_job_que()
    }
    t |> run("binary image bytes decode off-thread with exact transport size") <| @(tt : T?) {
        var encoded = ""
        var byte_count = 0
        fopen("3rdparty/fmt/doc/html/_static/file.png", "rb") $(file) {
            if (file != null) {
                fmap(file) $(var data : array<uint8>#) {
                    byte_count = length(data)
                    encoded = base64_encode(data)
                }
            }
        }
        tt |> success(byte_count > 0, "tracked PNG fixture is readable")
        create_job_que()
        var worker = InspectorPrepareWorker()
        tt |> equal(inspector_prepare_worker_start(worker), true)
        tt |> equal(inspector_prepare_worker_queue(worker, 13,
            "diff --git a/file.png b/file.png\nBinary files a/file.png and b/file.png differ\n",
            "", "", encoded, byte_count), true)
        var completed = false
        let wait_started = ref_time_ticks()
        while (!completed && int64(get_time_usec(wait_started)) < 5000000l) {
            inspector_prepare_worker_try_event(worker) $(event : InspectorPrepareEvent const) {
                if (event.kind == InspectorPrepareEventKind.completed) {
                    completed = true
                    tt |> equal(event.parsed.binary, true)
                    tt |> equal(event.preview_byte_count, byte_count)
                    tt |> equal(event.preview_error, "")
                    tt |> success(event.preview_width > 0 && event.preview_height > 0,
                        "stb_image reports image dimensions")
                    tt |> equal(length(event.preview_pixels),
                        event.preview_width * event.preview_height * 4)
                }
            }
        }
        tt |> equal(completed, true)
        tt |> equal(inspector_prepare_worker_shutdown(worker), true)
        destroy_job_que()
    }
    t |> run("unsupported binary completes with an unavailable-preview reason") <| @(tt : T?) {
        let bytes : array<uint8> <- [uint8(0), uint8(1), uint8(2), uint8(3)]
        let encoded = base64_encode(bytes)
        create_job_que()
        var worker = InspectorPrepareWorker()
        tt |> equal(inspector_prepare_worker_start(worker), true)
        tt |> equal(inspector_prepare_worker_queue(worker, 14,
            "diff --git a/file.bin b/file.bin\nBinary files a/file.bin and b/file.bin differ\n",
            "", "", encoded, length(bytes)), true)
        var completed = false
        let wait_started = ref_time_ticks()
        while (!completed && int64(get_time_usec(wait_started)) < 5000000l) {
            inspector_prepare_worker_try_event(worker) $(event : InspectorPrepareEvent const) {
                if (event.kind == InspectorPrepareEventKind.completed) {
                    completed = true
                    tt |> equal(event.parsed.binary, true)
                    tt |> equal(empty(event.preview_pixels), true)
                    tt |> equal(empty(event.preview_error), false)
                }
            }
        }
        tt |> equal(completed, true)
        tt |> equal(inspector_prepare_worker_shutdown(worker), true)
        destroy_job_que()
    }
    t |> run("empty binary preview completes without decoding") <| @(tt : T?) {
        create_job_que()
        var worker = InspectorPrepareWorker()
        tt |> equal(inspector_prepare_worker_start(worker), true)
        tt |> equal(inspector_prepare_worker_queue(worker, 15,
            "diff --git a/empty.bin b/empty.bin\nBinary files a/empty.bin and b/empty.bin differ\n",
            "", "", "", 0), true)
        var completed = false
        let wait_started = ref_time_ticks()
        while (!completed && int64(get_time_usec(wait_started)) < 5000000l) {
            inspector_prepare_worker_try_event(worker) $(event : InspectorPrepareEvent const) {
                if (event.kind == InspectorPrepareEventKind.completed) {
                    completed = true
                    tt |> equal(event.parsed.binary, true)
                    tt |> equal(event.preview_error, "Binary preview is empty")
                    tt |> equal(empty(event.preview_pixels), true)
                }
            }
        }
        tt |> equal(completed, true)
        tt |> equal(inspector_prepare_worker_shutdown(worker), true)
        destroy_job_que()
    }
    t |> run("explicit empty full View does not fall back to diff context") <| @(tt : T?) {
        create_job_que()
        var worker = InspectorPrepareWorker()
        tt |> equal(inspector_prepare_worker_start(worker), true)
        tt |> equal(inspector_prepare_worker_queue(worker, 16,
            "diff --git a/old.txt b/old.txt\ndeleted file mode 100644\n--- a/old.txt\n+++ /dev/null\n@@ -1 +0,0 @@\n-old\n",
            "", "plain", "", 0, true), true)
        var completed = false
        let wait_started = ref_time_ticks()
        while (!completed && int64(get_time_usec(wait_started)) < 5000000l) {
            inspector_prepare_worker_try_event(worker) $(event : InspectorPrepareEvent const) {
                if (event.kind == InspectorPrepareEventKind.completed) {
                    completed = true
                    tt |> equal(event.view_source, "")
                    tt |> equal(event.view_document.source, "")
                }
            }
        }
        tt |> equal(completed, true)
        tt |> equal(inspector_prepare_worker_shutdown(worker), true)
        destroy_job_que()
    }
    t |> run("no-hunk patch still prepares the selected full View") <| @(tt : T?) {
        let source = "int generated_value = 42;\n"
        create_job_que()
        var worker = InspectorPrepareWorker()
        tt |> equal(inspector_prepare_worker_start(worker), true)
        tt |> equal(inspector_prepare_worker_queue(worker, 17,
            "warning: diff emitted no file patch\n", source,
            "cpp", "", 0, true), true)
        var completed = false
        let wait_started = ref_time_ticks()
        while (!completed && int64(get_time_usec(wait_started)) < 5000000l) {
            inspector_prepare_worker_try_event(worker) $(event : InspectorPrepareEvent const) {
                if (event.kind == InspectorPrepareEventKind.completed) {
                    completed = true
                    tt |> equal(event.view_text_ready, true)
                    tt |> equal(event.view_source, source)
                    tt |> equal(event.view_document.source, source)
                    tt |> equal(length(event.parsed.hunks), 0)
                    tt |> success(length(event.view_document.spans) > 1,
                        "zero-hunk View is syntax highlighted")
                }
            }
        }
        tt |> equal(completed, true)
        tt |> equal(inspector_prepare_worker_shutdown(worker), true)
        destroy_job_que()
    }
    t |> run("View documents carry fold regions; aligned diff panes never do") <| @(tt : T?) {
        create_job_que()
        var worker = InspectorPrepareWorker()
        tt |> equal(inspector_prepare_worker_start(worker), true)
        let view = "options gen2\n\ndef answer(sample : int) : int \{\n    var total = sample\n    total += 1\n    return total\n\}\n"
        let patch = "diff --git a/a.das b/a.das\n--- a/a.das\n+++ b/a.das\n@@ -5 +5 @@\n-    total += 2\n+    total += 1\n"
        tt |> equal(inspector_prepare_worker_queue(worker, 19, patch, view, "das"),
            true)
        var completed = false
        let wait_started = ref_time_ticks()
        while (!completed && int64(get_time_usec(wait_started)) < 5000000l) {
            inspector_prepare_worker_try_event(worker) $(event : InspectorPrepareEvent const) {
                if (event.kind == InspectorPrepareEventKind.completed) {
                    completed = true
                    tt |> success(length(event.view_document.folds) > 0,
                        "the View document lists foldable regions")
                    tt |> equal(length(event.old_document.folds), 0,
                        "the BEFORE pane stays fold-free")
                    tt |> equal(length(event.new_document.folds), 0,
                        "the AFTER pane stays fold-free")
                }
            }
        }
        tt |> equal(completed, true)
        tt |> equal(inspector_prepare_worker_shutdown(worker), true)
        destroy_job_que()
    }
    t |> run("Git-binary UTF-8 bytes prepare as source View") <| @(tt : T?) {
        let source = "// generated source\nint answer = 42;\n"
        let encoded = base64_encode(source)
        create_job_que()
        var worker = InspectorPrepareWorker()
        tt |> equal(inspector_prepare_worker_start(worker), true)
        tt |> equal(inspector_prepare_worker_queue(worker, 18,
            "diff --git a/generated.cpp b/generated.cpp\nBinary files a/generated.cpp and b/generated.cpp differ\n",
            "", "cpp", encoded, length(source), true), true)
        var completed = false
        let wait_started = ref_time_ticks()
        while (!completed && int64(get_time_usec(wait_started)) < 5000000l) {
            inspector_prepare_worker_try_event(worker) $(event : InspectorPrepareEvent const) {
                if (event.kind == InspectorPrepareEventKind.completed) {
                    completed = true
                    tt |> equal(event.parsed.binary, true)
                    tt |> equal(event.view_text_ready, true)
                    tt |> equal(event.view_source, source)
                    tt |> equal(event.view_document.source, source)
                    tt |> equal(event.preview_error, "")
                    tt |> equal(empty(event.preview_pixels), true)
                    tt |> success(length(event.view_document.spans) > 1,
                        "Git-binary source is syntax highlighted")
                }
            }
        }
        tt |> equal(completed, true)
        tt |> equal(inspector_prepare_worker_shutdown(worker), true)
        destroy_job_que()
    }
}

[test]
def test_inspector_capped_language(t : T?) {
    //! The highlight cap and the VIEW's renderer choice must be one decision.
    //! When they drifted apart, a .md past the cap routed to the markdown
    //! renderer with no document built for it and drew "Invalid Markdown
    //! document" - a file that rendered fine as plain text one byte earlier.
    var inscope pieces : array<string>
    pieces |> reserve(200)
    for (_index in range(200)) {
        pieces |> push("0123456789012345678901234567890123456789" +
            "012345678901234567890123456789012345678901234567890123456789")
    }
    let long_line = join(pieces, "")
    t |> run("a line past the shape cap drops the language") <| @(tt : T?) {
        tt |> equal(length(long_line), 20000)
        tt |> success(!inspector_syntax_worth_highlighting(long_line))
        tt |> equal(inspector_highlight_language("markdown",
            "# Title\n{long_line}\n", "# Old\n"), "",
            "either side over the cap drops it")
        tt |> equal(inspector_highlight_language("markdown",
            "# New\n", "# Old\n{long_line}\n"), "")
        tt |> equal(inspector_highlight_language("markdown",
            "# New\n", "# Old\n"), "markdown")
    }
    t |> run("the renderer follows the same capped answer") <| @(tt : T?) {
        tt |> equal(inspector_resolved_language("markdown", true), "",
            "a capped .md renders as PLAIN TEXT, not as an empty markdown view")
        tt |> equal(inspector_resolved_language("markdown", false), "markdown")
        tt |> equal(inspector_resolved_language("", true), "")
    }
    t |> run("a capped Markdown job builds no document and reports capped") <| @(tt : T?) {
        create_job_que()
        var worker = InspectorPrepareWorker()
        tt |> equal(inspector_prepare_worker_start(worker), true)
        let patch = "diff --git a/big.md b/big.md\n--- a/big.md\n+++ b/big.md\n@@ -1 +1 @@\n-# Old\n+# New\n"
        let view = "# New\n{long_line}\n"
        tt |> equal(inspector_prepare_worker_queue(
            worker, 31, patch, view, "markdown"), true)
        var completed = false
        let wait_started = ref_time_ticks()
        while (!completed && int64(get_time_usec(wait_started)) < 5000000l) {
            inspector_prepare_worker_try_event(worker) $(event : InspectorPrepareEvent const) {
                if (event.kind == InspectorPrepareEventKind.completed) {
                    completed = true
                    tt |> success(event.syntax_capped,
                        "the shape gate dropped the language")
                    tt |> success(!event.markdown_document.ok,
                        "no markdown document is built past the cap")
                    tt |> equal(inspector_resolved_language("markdown",
                        event.syntax_capped), "",
                        "so the view must not choose the markdown renderer")
                }
            }
        }
        tt |> equal(completed, true)
        tt |> equal(inspector_prepare_worker_shutdown(worker), true)
        destroy_job_que()
    }
}
