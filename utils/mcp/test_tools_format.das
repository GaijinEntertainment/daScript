options gen2
options rtti

require dastest/testing_boost public
require daslib/json
require daslib/json_boost
require strings
require daslib/fio
require _tools_common

require tools/format_file public
require daslib/das_source_formatter public

def private parse_json_array(text : string; var parsed : JsonValue?&) : bool {
    var jerr : string
    parsed = read_json(text, jerr)
    return parsed != null && parsed.value is _array
}

def private find_format_result(arr : JsonValue?; file_substr : string; var status : string&; var message : string&) : bool {
    if (arr == null || !(arr.value is _array)) return false
    for (entry in arr.value as _array) {
        if (entry == null) continue
        let file_val = entry?["file"]
        if (file_val == null || !(file_val.value is _string)) continue
        let file = file_val.value as _string
        if (find(file, file_substr) < 0) continue
        let status_val = entry?["status"]
        let msg_val = entry?["message"]
        if (status_val != null && status_val.value is _string) {
            status = status_val.value as _string
        }
        if (msg_val != null && msg_val.value is _string) {
            message = msg_val.value as _string
        }
        return true
    }
    return false
}

[test(description = "format_file returns a JSON array of per-file results with file, status and message for one file and for a comma-separated list.", timeout = 1200)]
def test_format_file(t : T?) {
    t |> run("single file returns JSON array with required fields") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_format_file(fixture_path("_fixture_valid.das")), text, is_error)
        t |> success(!is_error, "should not be error")

        var parsed : JsonValue?
        let ok = parse_json_array(text, parsed)
        t |> success(ok, "inner payload should be a JSON array")
        if (!ok) {
            unsafe { delete parsed; }
            return
        }

        t |> success(!empty(parsed.value as _array), "array should contain at least one result")
        let first = (parsed.value as _array)[0]
        t |> success(first != null, "first result should exist")
        if (first != null) {
            t |> success(first?["file"] != null, "result has file")
            t |> success(first?["status"] != null, "result has status")
            t |> success(first?["message"] != null, "result has message")
            let st = first?["status"]
            if (st != null && st.value is _string) {
                let status = st.value as _string
                t |> success(status == "formatted" || status == "already_formatted", "status is expected")
            } else {
                t |> failure("status should be a string")
            }
        }
        unsafe { delete parsed; }
    }

    t |> run("comma-separated list returns one result per file") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_format_file("{fixture_path("_fixture_valid.das")},{fixture_path("_fixture_error.das")}"), text, is_error)
        t |> success(!is_error, "both fixture files should be processable")

        var parsed : JsonValue?
        let ok = parse_json_array(text, parsed)
        t |> success(ok, "inner payload should be a JSON array")
        if (!ok) {
            unsafe { delete parsed; }
            return
        }
        t |> equal(length(parsed.value as _array), 2, "comma-separated list should yield two results")

        var st1 = ""
        var msg1 = ""
        let has_valid = find_format_result(parsed, "_fixture_valid.das", st1, msg1)
        t |> success(has_valid, "valid fixture result present")
        t |> success(!empty(st1), "valid fixture has non-empty status")
        t |> success(find(msg1, "fixture_valid") >= 0, "valid fixture message references file")

        var st2 = ""
        var msg2 = ""
        let has_error_fixture = find_format_result(parsed, "_fixture_error.das", st2, msg2)
        t |> success(has_error_fixture, "error fixture result present")
        t |> success(!empty(st2), "error fixture has non-empty status")
        t |> success(find(msg2, "fixture_error") >= 0, "error fixture message references file")
        unsafe { delete parsed; }
    }

}

[test(description = "format_file expands globs and reports each file of a mixed batch, marking the batch an error when one file is missing.", timeout = 1200)]
def test_format_file_batches(t : T?) {
    t |> run("glob pattern expands and returns matching file entry") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_format_file("utils/mcp/tests/_fixture_v*.das"), text, is_error)
        t |> success(!is_error, "glob with matches should not be error")

        var parsed : JsonValue?
        let ok = parse_json_array(text, parsed)
        t |> success(ok, "inner payload should be a JSON array")
        if (!ok) {
            unsafe { delete parsed; }
            return
        }
        var st = ""
        var msg = ""
        let has_valid = find_format_result(parsed, "_fixture_valid.das", st, msg)
        t |> success(has_valid, "glob should include _fixture_valid.das")
        t |> success(st == "formatted" || st == "already_formatted", "glob result status is expected")
        unsafe { delete parsed; }
    }

    t |> run("mixed valid + missing sets isError and reports per-file statuses") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_format_file("{fixture_path("_fixture_valid.das")},utils/mcp/tests/_fixture_does_not_exist_123.das"), text, is_error)
        t |> success(is_error, "missing file in batch should set isError")

        var parsed : JsonValue?
        let ok = parse_json_array(text, parsed)
        t |> success(ok, "inner payload should be a JSON array")
        if (!ok) {
            unsafe { delete parsed; }
            return
        }
        t |> equal(length(parsed.value as _array), 2, "mixed batch should still return two result entries")

        var st_ok = ""
        var msg_ok = ""
        let has_valid = find_format_result(parsed, "_fixture_valid.das", st_ok, msg_ok)
        t |> success(has_valid, "valid file result present")
        t |> success(st_ok == "formatted" || st_ok == "already_formatted", "valid file status is expected")

        var st_missing = ""
        var msg_missing = ""
        let has_missing = find_format_result(parsed, "_fixture_does_not_exist_123.das", st_missing, msg_missing)
        t |> success(has_missing, "missing file result present")
        t |> equal(st_missing, "error")
        t |> success(find(msg_missing, "Cannot open file") >= 0, "missing file reports open error")
        unsafe { delete parsed; }
    }
}

[test(description = "Comment stripping keeps headers, doc comments, directives, nolint whys and licenses and drops narration, with kept comments continuing only as whole chains.")]
def test_format_strip_comments_source(t : T?) {
    t |> run("keeps header, docs, fmt directives, nolint; strips the rest") <| @(t : T?) {
        let src = fread(fixture_path("_fixture_comments.das"))
        t |> success(!empty(src), "fixture readable")
        let stripped = format_source_string(src, true)
        t |> success(find(stripped, "// header block stays") >= 0, "header block kept")
        t |> success(find(stripped, "//! doc comment survives") >= 0, "doc comment kept")
        t |> success(find(stripped, "nolint:STYLE005") >= 0, "nolint suppression kept with its why")
        t |> success(find(stripped, "@nolint") >= 0, "@nolint form kept")
        t |> success(find(stripped, "narration") < 0, "narration stripped")
        t |> success(find(stripped, "trailing dies") < 0, "trailing comment stripped")
        t |> success(find(stripped, "block dies") < 0, "block comment stripped")
        t |> success(find(stripped, "let z = x + y") >= 0, "multi-line inline comment joins its expression")
    }
    t |> run("license comments survive the strip anywhere in the file") <| @(t : T?) {
        let src = "options gen2\n\ndef mid \{\n    return 1\n\}\n\n// Copyright (c) 2008 Bjoern Hoehrmann\n// SPDX-License-Identifier: MIT\n// COPYRIGHT 2026 SHOUTING NOTICES TOO\ndef tail \{\n    return 2\n\}\n"
        let stripped = format_source_string(src, true)
        t |> success(find(stripped, "Copyright (c) 2008") >= 0, "Copyright line kept mid-file")
        t |> success(find(stripped, "SPDX-License-Identifier: MIT") >= 0, "SPDX line kept mid-file")
        t |> success(find(stripped, "COPYRIGHT 2026") >= 0, "match is case-insensitive")
    }
    t |> run("a fmt directive ends a kept continuation chain") <| @(t : T?) {
        let src = "options gen2\n\ndef head \{\n    return 1\n\}\n\n//! documented contract\n//fmt:keep-blank-lines\n// narration dies here\ndef tail \{\n    return 2\n\}\n"
        let stripped = format_source_string(src, true)
        t |> success(find(stripped, "documented contract") >= 0, "doc kept")
        t |> success(find(stripped, "fmt:keep-blank-lines") >= 0, "directive kept")
        t |> success(find(stripped, "narration dies") < 0, "narration after the directive is not a continuation")
    }
    t |> run("strip-disabled path leaves every comment in place") <| @(t : T?) {
        let src = fread(fixture_path("_fixture_comments.das"))
        let formatted = format_source_string(src, false)
        t |> success(find(formatted, "narration dies") >= 0, "narration kept when not stripping")
        t |> success(find(formatted, "block dies") >= 0, "block comment kept when not stripping")
    }
    t |> run("//fmt:ignore-file opts out of stripping too") <| @(t : T?) {
        let src = "//fmt:ignore-file\nlet X = 1\n// narration survives ignore\nlet Y = 2\n"
        let out = format_source_string(src, true)
        t |> success(find(out, "narration survives ignore") >= 0, "ignore-file blocks the strip")
    }
    t |> run("header under options survives; a stripped banner leaves no blank-line scar") <| @(t : T?) {
        let src = "options gen2\n\n// header under options stays\n\nrequire strings // trailing narration dies\n\ndef a() \{\n    return\n\}\n\n\n// banner dies\n\n\ndef b() \{\n    return\n\}\n"
        let out = format_source_string(src, true)
        t |> success(find(out, "header under options stays") >= 0, "header kept below the options line")
        t |> success(find(out, "trailing narration") < 0, "trailing narration on a require line is not header")
        t |> success(find(out, "banner") < 0, "banner stripped")
        t |> success(find(out, "\n\n\n\n") < 0, "no 3+ blank run at the strip site")
    }
    t |> run("//fmt: directives inside code survive a strip") <| @(t : T?) {
        let src = "options gen2\n\ndef a() \{\n    //fmt:keep-alignment\n    // narration dies\n    return\n\}\n"
        let out = format_source_string(src, true)
        t |> success(find(out, "//fmt:keep-alignment") >= 0, "fmt directive kept")
        t |> success(find(out, "narration") < 0, "narration stripped")
    }
    t |> run("a kept comment keeps its attached continuation lines, whole or not at all") <| @(t : T?) {
        let src = "options gen2\n\ndef a() \{\n    // nolint:STYLE014 — the why starts here and\n    // continues on a second line without truncation\n    return\n\}\n\ndef b() \{\n    //! doc head\n    // doc continuation line\n    return\n\}\n"
        let out = format_source_string(src, true)
        t |> success(find(out, "continues on a second line") >= 0, "nolint why kept whole")
        t |> success(find(out, "doc continuation line") >= 0, "//! continuation kept")
    }
    t |> run("the chain extends through a third line, and a block comment never continues it") <| @(t : T?) {
        let src = "options gen2\n\ndef a() \{\n    // nolint:STYLE014 — line one of the why,\n    // line two of the why,\n    // line three of the why\n    return\n\}\n\ndef b() \{\n    //! doc head\n    /* block narration dies */\n    return\n\}\n"
        let out = format_source_string(src, true)
        t |> success(find(out, "line three of the why") >= 0, "third continuation line kept")
        t |> success(find(out, "block narration") < 0, "a /* */ under a kept head is not a continuation")
    }
    t |> run("a blank line or code breaks the continuation chain") <| @(t : T?) {
        let src = "options gen2\n\ndef a() \{\n    // nolint:STYLE014 — one-line why\n\n    // separate narration dies\n    return  // nolint:STYLE014 — trailing why\n    // not a continuation of a trailing head, dies\n\}\n"
        let out = format_source_string(src, true)
        t |> success(find(out, "one-line why") >= 0, "head kept")
        t |> success(find(out, "separate narration") < 0, "blank line breaks the chain")
        t |> success(find(out, "trailing why") >= 0, "trailing nolint kept")
        t |> success(find(out, "not a continuation") < 0, "trailing head opens no chain")
    }
    t |> run("!is-prefixed identifiers survive formatting untouched") <| @(t : T?) {
        let src = "options gen2\n\ndef check(f : bool) : bool \{\n    return !is_unary(f)\n\}\n"
        let out = format_source_string(src, false)
        t |> success(find(out, "!is_unary(f)") >= 0,
            "template matching has no word boundary: a '!is'/'!as' entry in greedy_match_safe_tokens would eat the prefix of !is_unary-style unary-not calls - those two tokens must stay OUT of the list")
    }
}

[test(description = "format_file strips comments only in folders that opt in, restores the original bytes when the stripped file no longer compiles, and raises the rescue advisory only when something was stripped.", timeout = 1200)]
def test_format_file_comment_policy(t : T?) {
    t |> run("force_clean_comments folder strips, and the compile verify passes") <| @(t : T?) {
        var terr : string
        let dir = make_policy_dir("mcp_strip_test", "[format]\nforce_clean_comments = true\n", terr)
        if (empty(dir)) {
            t |> failure("temp dir: {terr}")
            return
        }
        let dst = path_join(dir, "victim.das")
        let original = fread(fixture_path("_fixture_comments.das"))
        t |> success(fwrite(dst, original), "fixture copied")
        var text : string
        var is_error = false
        let ok = parse_result(do_format_file(dst), text, is_error)
        t |> success(ok, "parse result JSON")
        t |> success(find(text, "comments stripped") >= 0, "opted-in folder stripped -- text={text}")
        t |> success(find(text, "rescue_advisory") >= 0, "strip batch appends the rescue advisory -- text={text}")
        t |> success(find(text, "harvester") >= 0, "advisory names the harvester agent")
        let after = fread(dst)
        t |> success(find(after, "narration") < 0, "comments removed on disk")
        t |> success(find(after, "//! doc comment survives") >= 0, "docs kept on disk")
        rmdir_rec(dir)
    }
    t |> run("a reformat that strips no comments raises no advisory") <| @(t : T?) {
        var terr : string
        let dir = make_policy_dir("mcp_nostrip_test", "[format]\nforce_clean_comments = true\n", terr)
        if (empty(dir)) {
            t |> failure("temp dir: {terr}")
            return
        }
        let dst = path_join(dir, "spacing.das")
        fwrite(dst, "options gen2\n\ndef f() \{\n    let x=1\n    return x\n\}\n")
        var text : string
        var is_error = false
        parse_result(do_format_file(dst), text, is_error)
        t |> success(find(text, "\"status\":\"formatted\"") >= 0, "spacing-only file was reformatted -- text={text}")
        t |> success(find(text, "rescue_advisory") < 0, "no comments deleted, no advisory")
        rmdir_rec(dir)
    }
    t |> run("fail-closed: a strip whose compile_check fails restores the original bytes") <| @(t : T?) {
        var terr : string
        let dir = make_policy_dir("mcp_revert_test", "[format]\nforce_clean_comments = true\n", terr)
        if (empty(dir)) {
            t |> failure("temp dir: {terr}")
            return
        }
        let dst = path_join(dir, "broken.das")
        let src = "options gen2\n\ndef broken() \{\n    // narration to strip\n    return undefined_symbol_xyz\n\}\n"
        t |> success(fwrite(dst, src), "broken source written")
        var text : string
        var is_error = false
        let ok = parse_result(do_format_file(dst), text, is_error)
        t |> success(ok, "parse result JSON")
        t |> success(is_error, "reverted outcome reports as error")
        t |> success(find(text, "reverted") >= 0, "status is reverted -- text={text}")
        t |> success(find(text, "rescue_advisory") < 0, "a reverted strip keeps its comments — no rescue advisory")
        t |> equal(fread(dst), src, "original bytes restored")
        rmdir_rec(dir)
    }
    t |> run("no config: formats, comments stay") <| @(t : T?) {
        var terr : string
        let dir = make_policy_dir("mcp_keep_test", "", terr)
        if (empty(dir)) {
            t |> failure("temp dir: {terr}")
            return
        }
        let dst = path_join(dir, "victim.das")
        fwrite(dst, fread(fixture_path("_fixture_comments.das")))
        var text : string
        var is_error = false
        parse_result(do_format_file(dst), text, is_error)
        t |> success(!is_error, "default run should not error")
        t |> success(find(text, "rescue_advisory") < 0, "nothing stripped — no rescue advisory")
        let after = fread(dst)
        t |> success(find(after, "narration dies") >= 0, "comments intact on disk")
        rmdir_rec(dir)
    }
}

[test(description = "format_file skips a folder that disables formatting and lets the nearest lint config win per key.", timeout = 1200)]
def test_format_file_policy_scoping(t : T?) {
    t |> run("format_enabled = false skips the file untouched") <| @(t : T?) {
        var terr : string
        let dir = make_policy_dir("mcp_skip_test", "[format]\nformat_enabled = false\n", terr)
        if (empty(dir)) {
            t |> failure("temp dir: {terr}")
            return
        }
        let dst = path_join(dir, "corpus.das")
        let src = "options gen2\ndef f() \{\n    let x=1\n\}\n"
        fwrite(dst, src)
        var text : string
        var is_error = false
        parse_result(do_format_file(dst), text, is_error)
        t |> success(find(text, "skipped") >= 0, "status is skipped -- text={text}")
        t |> equal(fread(dst), src, "bytes untouched")
        rmdir_rec(dir)
    }
    t |> run("nearest .lint_config wins per key") <| @(t : T?) {
        var terr : string
        let dir = make_policy_dir("mcp_cascade_test", "[format]\nforce_clean_comments = true\n", terr)
        if (empty(dir)) {
            t |> failure("temp dir: {terr}")
            return
        }
        let sub = path_join(dir, "teaching")
        mkdir(sub)
        fwrite(path_join(sub, ".lint_config"), "[format]\nforce_clean_comments = false\n")
        let dst = path_join(sub, "victim.das")
        fwrite(dst, fread(fixture_path("_fixture_comments.das")))
        var text : string
        var is_error = false
        parse_result(do_format_file(dst), text, is_error)
        t |> success(!is_error, "cascade run should not error")
        let after = fread(dst)
        t |> success(find(after, "narration dies") >= 0, "child override kept comments")
        rmdir_rec(dir)
    }
}
