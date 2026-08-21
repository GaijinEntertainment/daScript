#!/usr/bin/env python3
"""Tests for comment_guard.py — unit level (scanner, header exemption, kept
sets, edit diff) and end-to-end (subprocess with hook JSON on stdin, asserting
exit code, stderr, and empty stdout). The grammar is finite; the matrix walks
it: comment forms x string/char/reader forms x header positions x edit modes x
extensions, plus a bounded corpus invariant over known-tricky repo files."""

import json
import os
import subprocess
import sys
import unittest

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(os.path.dirname(HERE))
sys.path.insert(0, HERE)

import comment_guard as cg

GUARD = os.path.join(HERE, "comment_guard.py")


def run_hook(payload):
    raw = payload if isinstance(payload, str) else json.dumps(payload)
    proc = subprocess.run([sys.executable, GUARD], input=raw,
                          capture_output=True, text=True, timeout=30,
                          encoding="utf-8")
    return proc.returncode, proc.stderr, proc.stdout


def edit_payload(path, old, new):
    return {"tool_name": "Edit", "tool_input": {"file_path": path, "old_string": old, "new_string": new}}


def write_payload(path, content):
    return {"tool_name": "Write", "tool_input": {"file_path": path, "content": content}}


def comments(text, is_das=True):
    return cg.scan(text, is_das)[0]


class TestScanner(unittest.TestCase):
    def test_line_comment(self):
        self.assertEqual(comments("let x = 1 // note"), [(1, "// note", False)])

    def test_full_line_comment(self):
        self.assertEqual(comments("    // note"), [(1, "// note", True)])

    def test_block_single_line(self):
        self.assertEqual(comments("a /* mid */ b"), [(1, "/* mid */", False)])

    def test_block_multi_line_full_text(self):
        got = comments("/* one\ntwo\nthree */ let x = 1\nlet y = 2 // tail")
        self.assertEqual(got, [(1, "/* one\ntwo\nthree */", True), (4, "// tail", False)])

    def test_two_blocks_one_line(self):
        got = comments("a /* x */ b /* y */ c")
        self.assertEqual([t for _, t, _ in got], ["/* x */", "/* y */"])

    def test_comment_after_block_close_line(self):
        got = comments("/* a\nb */ code() // trail")
        self.assertEqual(got, [(1, "/* a\nb */", True), (2, "// trail", False)])

    def test_line_before_block_precedence(self):
        self.assertEqual(comments("x = 1 // note /* text */"), [(1, "// note /* text */", False)])

    def test_nested_block_das(self):
        got = comments("/* outer /* inner */ still */ x() // t", is_das=True)
        self.assertEqual([t for _, t, _ in got], ["/* outer /* inner */ still */", "// t"])

    def test_nested_block_multi_line_das(self):
        got = comments("/* a\n/* b */\nc */\nx = 1 // t", is_das=True)
        self.assertEqual(got, [(1, "/* a\n/* b */\nc */", True), (4, "// t", False)])

    def test_block_not_nested_c(self):
        got = comments("/* outer /* inner */ rest();", is_das=False)
        self.assertEqual([t for _, t, _ in got], ["/* outer /* inner */"])

    def test_unterminated_block_at_eof(self):
        got = comments("int x;\n/* dangling\nmore")
        self.assertEqual(got, [(2, "/* dangling\nmore", True)])

    def test_unterminated_nested_block_at_eof_das(self):
        got = comments("/* a /* b\nc", is_das=True)
        self.assertEqual(got, [(1, "/* a /* b\nc", True)])

    def test_no_comment_inside_block_reported_twice(self):
        got = comments("/* head\n// inner narration\n*/\ncode()")
        self.assertEqual(len(got), 1)

    def test_comment_marker_inside_string(self):
        self.assertEqual(comments('let url = "https://x.io"'), [])
        self.assertEqual(comments('let s = "/* not a comment */"'), [])
        self.assertEqual(comments('let s = "a//b"'), [])

    def test_comment_after_string(self):
        self.assertEqual(comments('let s = "a//b" // real'), [(1, "// real", False)])

    def test_escaped_quote_in_string(self):
        self.assertEqual(comments('let s = "a\\" // x"'), [])

    def test_char_literal_slash(self):
        self.assertEqual(comments("if (b == '/') {}"), [])

    def test_char_literal_double_quote(self):
        self.assertEqual(comments("if (c == '\"') {} // after"), [(1, "// after", False)])

    def test_char_literal_escaped_quote(self):
        self.assertEqual(comments("if (c == '\\'') {} // t"), [(1, "// t", False)])

    def test_lone_apostrophe_does_not_mask(self):
        got = comments("who's = 1 // don't and won't")
        self.assertEqual(got, [(1, "// don't and won't", False)])

    def test_char_span_bound_keeps_comment_after_apostrophe(self):
        got = comments("a = b'c // note about d'", is_das=False)
        self.assertEqual(got, [(1, "// note about d'", False)])

    def test_apostrophe_before_block_close(self):
        got = comments("/* it's */ let c = 'x' // note\n// second", is_das=True)
        self.assertEqual([t for _, t, _ in got], ["/* it's */", "// note", "// second"])

    def test_digit_separators(self):
        self.assertEqual(comments("int n = 1'000'000; // big", is_das=False),
                         [(1, "// big", False)])

    def test_multi_line_string_das(self):
        src = 'let AOT = "\n// unreferenced formal parameter\n#pragma warning\n"\nx = 1 // real'
        self.assertEqual(comments(src, is_das=True), [(5, "// real", False)])

    def test_interpolated_nested_string_das(self):
        self.assertEqual(comments('let s = "{foo("a//b")}"', is_das=True), [])

    def test_interpolated_multi_line_das(self):
        src = 'let s = "{a}\ntext // not comment\n{b("q//r")} more"\nx() // real'
        self.assertEqual(comments(src, is_das=True), [(4, "// real", False)])

    def test_interpolation_brace_depth(self):
        self.assertEqual(comments('let s = "{ {a=1} }" // t', is_das=True),
                         [(1, "// t", False)])

    def test_braces_literal_in_c_string(self):
        self.assertEqual(comments('printf("{//}"); // t', is_das=False),
                         [(1, "// t", False)])

    def test_reader_macro_single_line(self):
        got = comments("var RE <- %regex~//.*%%\nx = 1 // real", is_das=True)
        self.assertEqual(got, [(2, "// real", False)])

    def test_reader_macro_unbalanced_quote_no_derail(self):
        src = '%regex~"((LINT|PERF|STYLE)[0-9][0-9][0-9]):%%\ny = 2 // after'
        self.assertEqual(comments(src, is_das=True), [(2, "// after", False)])

    def test_reader_macro_multi_line(self):
        src = '%peg~\nrule <- "a" // not a comment\n%%\nz() // yes'
        self.assertEqual(comments(src, is_das=True), [(4, "// yes", False)])

    def test_modulo_is_not_reader(self):
        self.assertEqual(comments("x = a % b // mod", is_das=True), [(1, "// mod", False)])

    def test_reader_not_special_in_c(self):
        got = comments("int p = a %regex~ 2; // c", is_das=False)
        self.assertEqual(got, [(1, "// c", False)])

    def test_string_backslash_continuation_c(self):
        src = 'const char* s = "abc \\\n def // xyz";'
        self.assertEqual(comments(src, is_das=False), [])

    def test_unterminated_string_c_resets(self):
        src = 'printf("oops;\n// next line comment'
        self.assertEqual(comments(src, is_das=False), [(2, "// next line comment", True)])

    def test_raw_string_single_line(self):
        self.assertEqual(comments('auto p = R"delim(a" // b)delim";', is_das=False), [])

    def test_raw_string_multi_line(self):
        src = 'const char* sh = R"(\n// GLSL comment\n)";'
        self.assertEqual(comments(src, is_das=False), [])

    def test_raw_string_prefixed(self):
        self.assertEqual(comments('auto s = u8R"(a // b)";', is_das=False), [])
        self.assertEqual(comments('auto s = LR"x(a // b)x";', is_das=False), [])

    def test_raw_string_overlong_delimiter_falls_back(self):
        d = "a" * 20
        src = 'auto s = R"{}(x " y){}"; // tail'.format(d, d)
        self.assertEqual(comments(src, is_das=False), [])

    def test_reader_bang_form(self):
        self.assertEqual(comments("var x <- %foo!a//b%%", is_das=True), [])

    def test_c_string_escaped_quote_then_int(self):
        src = 'const char* s = "a\\" // x"; int y;'
        self.assertEqual(comments(src, is_das=False), [])

    def test_das_string_close_brace_literal(self):
        self.assertEqual(comments('let s = "a}b" // t', is_das=True), [(1, "// t", False)])

    def test_interpolation_depth_across_lines(self):
        src = 'let s = "{ f(\n"a//b") } tail // not comment"\nz() // real'
        self.assertEqual(comments(src, is_das=True), [(3, "// real", False)])

    def test_interpolation_nested_across_lines(self):
        src = 'let s = "{f("abc\ndef")} tail"\nx() // real'
        self.assertEqual(comments(src, is_das=True), [(3, "// real", False)])

    def test_c_string_open_line_start_resets(self):
        src = 'printf("oops \\\nstill open\n// next comment'
        self.assertEqual(comments(src, is_das=False), [(3, "// next comment", True)])

    def test_c_string_three_line_continuation(self):
        src = '"a \\\nb // not comment \\\nc // still not";'
        self.assertEqual(comments(src, is_das=False), [])

    def test_lone_cr_is_a_line_break(self):
        src = "// header\rint x;\rint y; // narr\r"
        self.assertEqual([t for _, t in cg.violations(src, False, True)], ["// narr"])

    def test_control_chars_do_not_split_lines(self):
        got = comments("// a\x0cfeed /* block\n}", is_das=False)
        self.assertEqual(got, [(1, "// a\x0cfeed /* block", True)])
        got = comments("x = 1 // note", is_das=False)
        self.assertEqual(got, [(1, "// note", False)])

    def test_url_skipped_in_code_fragment(self):
        self.assertEqual(comments("see http://old.example.com", is_das=False), [])
        self.assertEqual(comments("ftp://host/path", is_das=False), [])

    def test_case_label_comment_not_url(self):
        self.assertEqual(comments("case 1://Pure RLE", is_das=False),
                         [(1, "//Pure RLE", False)])

    def test_url_needs_nonspace_tail(self):
        self.assertEqual(comments("done:// cleanup", is_das=False),
                         [(1, "// cleanup", False)])

    def test_url_candidate_at_eol_is_comment(self):
        self.assertEqual(comments("x = ab://", is_das=False), [(1, "//", False)])

    def test_no_colon_before_slashes_is_comment(self):
        self.assertEqual(comments("value//keep", is_das=False), [(1, "//keep", False)])


class TestHeaderExemption(unittest.TestCase):
    def test_plain_header(self):
        self.assertEqual(cg.violations("// hdr\n// hdr2\n\ndef f() {}", True, True), [])

    def test_header_below_options_require(self):
        src = "options gen2\nrequire daslib/fio\n// header prose\ndef f() {}"
        self.assertEqual(cg.violations(src, True, True), [])

    def test_header_below_module(self):
        self.assertEqual(cg.violations("module foo shared\n// header prose\ndef f() {}", True, True), [])

    def test_das_license_before_same_line_code(self):
        self.assertEqual(cg.violations("/* license */ def f() {}", True, True), [])

    def test_c_header_stops_at_code(self):
        got = cg.violations("// copyright\nint x;\n// not header", False, True)
        self.assertEqual([t for _, t in got], ["// not header"])

    def test_c_header_below_preprocessor(self):
        src = '#line 1 "gen.cpp"\n#ifdef X\n#else\n#undef Y\n/* flex banner */\nint x;'
        self.assertEqual(cg.violations(src, False, True), [])

    def test_c_include_guard(self):
        src = "#ifndef X_H\n#define X_H\n// copyright\nint x;"
        self.assertEqual(cg.violations(src, False, True), [])

    def test_cpp20_module_is_not_preamble(self):
        got = cg.violations("module;\n// after\nint x;", False, True)
        self.assertEqual([t for _, t in got], ["// after"])

    def test_das_preamble_not_granted_in_c(self):
        got = cg.violations("options gen2\n// x", False, True)
        self.assertEqual([t for _, t in got], ["// x"])

    def test_block_header(self):
        self.assertEqual(cg.violations("/* copyright\n   notice */\nint x;", False, True), [])

    def test_self_closing_block_header(self):
        self.assertEqual(cg.violations("/* copyright */\nint x;", False, True), [])

    def test_comment_after_header_code_line_flagged(self):
        got = cg.violations("/* header */ int x;\n// narration\n", False, True)
        self.assertEqual([t for _, t in got], ["// narration"])

    def test_multiline_block_close_with_code_exempts_block_only(self):
        got = cg.violations("/* header\nmore */ int x;\n// narr", False, True)
        self.assertEqual([t for _, t in got], ["// narr"])

    def test_comment_only_file_is_all_header(self):
        self.assertEqual(cg.violations("// a\n\n// b\n/* c */\n", True, True), [])

    def test_indented_header_stays_exempt(self):
        self.assertEqual(cg.violations("    // a\n    // b\ndef f() {}", True, True), [])

    def test_string_interior_line_counts_as_code(self):
        src = 'options gen2 = "abc\ndef\nrequire x"\n// after'
        self.assertEqual([t for _, t in cg.violations(src, True, True)], ["// after"])

    def test_trailing_comment_on_preamble_line_flagged(self):
        src = "options gen2\nrequire daslib/fio // for fopen\ndef f() {}\n"
        self.assertEqual([t for _, t in cg.violations(src, True, True)], ["// for fopen"])

    def test_bom_does_not_kill_header(self):
        self.assertEqual(cg.violations("﻿// copyright\nint x;\n", False, True), [])

    def test_crlf_input(self):
        src = "// header\r\nint x;\r\nint y; // narr\r\n"
        self.assertEqual([t for _, t in cg.violations(src, False, True)], ["// narr"])

    def test_empty_input(self):
        self.assertEqual(cg.violations("", True, True), [])


class TestKept(unittest.TestCase):
    def test_das_kept_set(self):
        src = "//! doc\nfoo() // nolint: STYLE037 short why\n//fmt: ignore-file\n// @nolint tag\n"
        self.assertEqual(cg.violations(src, True, False), [])

    def test_das_fmt_space_flagged(self):
        self.assertEqual(len(cg.violations("// fmt: off\n", True, False)), 1)

    def test_das_ignore_file_opts_out_whole_write(self):
        src = "//fmt:ignore-file\ndef f() {}\nx = 1 // narration stays\n"
        self.assertEqual(cg.violations(src, True, True), [])

    def test_das_ignore_file_opts_out_edit_fragment(self):
        self.assertEqual(cg.edit_violations("x()\n", "//fmt:ignore-file\nx() // narr\n", True), [])

    def test_das_ignore_file_with_space_is_not_the_token(self):
        src = "//fmt: ignore-file\ndef f() {}\nx = 1 // narr\n"
        self.assertEqual([t for _, t in cg.violations(src, True, True)], ["// narr"])

    def test_c_ignore_file_not_special(self):
        src = "int x;\n//fmt:ignore-file\nint y; // narr\n"
        self.assertEqual(len(cg.violations(src, False, False)), 2)

    def test_das_bare_nolint_no_colon_flagged(self):
        self.assertEqual(len(cg.violations("// nolint missing colon\n", True, False)), 1)

    def test_das_nolint_case_sensitive(self):
        self.assertEqual(len(cg.violations("// NOLINT: X\n", True, False)), 1)

    def test_das_nbsp_after_slashes_flagged(self):
        self.assertEqual(len(cg.violations("// nolint: X\n", True, False)), 1)

    def test_das_clang_format_flagged(self):
        self.assertEqual(len(cg.violations("// clang-format off\n", True, False)), 1)

    def test_c_kept_set(self):
        src = ("//! doc\nint x; // NOLINT(readability)\n// NOLINTNEXTLINE(bugprone-x)\n"
              "// NOLINTBEGIN\n// nolint: lower\n// clang-format off\n/*! dox */\n/** dox */\n")
        self.assertEqual(cg.violations(src, False, False), [])

    def test_c_doxygen_multiline_kept(self):
        self.assertEqual(cg.violations("int y;\n/**\n * doc\n */\n", False, False), [])

    def test_c_star_banner_flagged(self):
        self.assertEqual(len(cg.violations("int y;\n/***** section *****/\n", False, False)), 1)

    def test_c_fmt_directive_flagged(self):
        self.assertEqual(len(cg.violations("//fmt: off\n", False, False)), 1)

    def test_c_empty_block_flagged(self):
        self.assertEqual(len(cg.violations("int x;\n/**/\n", False, False)), 1)

    def test_c_bare_doxygen_open_at_eof_kept_no_crash(self):
        self.assertEqual(cg.violations("int x;\n/**", False, False), [])

    def test_das_doxygen_block_flagged(self):
        self.assertEqual(len(cg.violations("def f() {}\n/*! doc-ish */\n", True, True)), 1)

    def test_narration_flagged(self):
        self.assertEqual(len(cg.violations("x = 1 // bump it\n", True, False)), 1)


class TestEditDiff(unittest.TestCase):
    def test_move_silent(self):
        old = "    // legacy narration\n    foo()\n"
        new = "// legacy narration\nfoo()\nbar()\n"
        self.assertEqual(cg.edit_violations(old, new, True), [])

    def test_addition_flagged(self):
        self.assertEqual(len(cg.edit_violations("foo()\n", "foo() // call foo\n", True)), 1)

    def test_reword_silent(self):
        self.assertEqual(cg.edit_violations("// note about 1\n", "// note about 2\n", True), [])

    def test_block_reword_silent(self):
        self.assertEqual(cg.edit_violations("/* a\nold */\n", "/* a\nnew */\n", False), [])

    def test_reindented_block_not_quoted_as_new(self):
        old = "/* keep\nthis */\nx()\n"
        new = "    /* keep\n    this */\nx()\ny() // fresh\n"
        got = cg.edit_violations(old, new, False)
        self.assertEqual([t for _, t in got], ["// fresh"])

    def test_duplicate_addition_flagged(self):
        old = "x()\n// note\n"
        new = "// note\nx()\n// note\n"
        self.assertEqual(len(cg.edit_violations(old, new, True)), 1)

    def test_net_decrease_silent(self):
        old = "// a\n// b\nx()\n"
        new = "x()\n// c\n"
        self.assertEqual(cg.edit_violations(old, new, True), [])

    def test_kept_demotion_flagged(self):
        self.assertEqual(len(cg.edit_violations("//! doc\n", "// doc\n", True)), 1)

    def test_promotion_silent(self):
        self.assertEqual(cg.edit_violations("// doc\n", "//! doc\n", True), [])

    def test_block_interior_fragment_with_url(self):
        old = "  see http://old.example.com\n"
        new = "  see http://new.example.com\n"
        self.assertEqual(cg.edit_violations(old, new, False), [])

    def test_empty_old_string(self):
        self.assertEqual(len(cg.edit_violations("", "// fresh\n", True)), 1)


class TestEndToEnd(unittest.TestCase):
    def test_non_source_file_silent(self):
        rc, _, out = run_hook(write_payload("readme.md", "# // not code"))
        self.assertEqual((rc, out), (0, ""))

    def test_other_tool_silent(self):
        rc, _, _ = run_hook({"tool_name": "Bash", "tool_input": {"command": "ls"}})
        self.assertEqual(rc, 0)

    def test_bad_json_silent(self):
        rc, err, out = run_hook("not json")
        self.assertEqual((rc, err, out), (0, "", ""))

    def test_non_dict_roots_silent(self):
        for raw in ("[1, 2]", '"x"', "null", "42"):
            rc, err, out = run_hook(raw)
            self.assertEqual((rc, err, out), (0, "", ""), raw)

    def test_missing_fields_silent(self):
        rc, err, _ = run_hook({"tool_name": "Edit"})
        self.assertEqual((rc, err), (0, ""))

    def test_non_dict_tool_input_silent(self):
        rc, _, _ = run_hook({"tool_name": "Edit", "tool_input": "oops"})
        self.assertEqual(rc, 0)

    def test_null_content_silent(self):
        rc, _, _ = run_hook(write_payload("a.das", None))
        self.assertEqual(rc, 0)

    def test_wrong_event_silent(self):
        p = write_payload("a.das", "x = 1 // narr\n")
        p["hook_event_name"] = "PreToolUse"
        rc, err, _ = run_hook(p)
        self.assertEqual((rc, err), (0, ""))

    def test_post_tool_use_event_accepted(self):
        p = write_payload("a.das", "x = 1 // narr\n")
        p["hook_event_name"] = "PostToolUse"
        rc, _, _ = run_hook(p)
        self.assertEqual(rc, 2)

    def test_failed_tool_call_silent(self):
        p = write_payload("a.das", "x = 1 // narr\n")
        p["tool_response"] = {"success": False}
        rc, _, _ = run_hook(p)
        self.assertEqual(rc, 0)

    def test_das_write_narration_slapped(self):
        rc, err, out = run_hook(write_payload("daslib/foo.das", "def f() {\n    // walk members\n}\n"))
        self.assertEqual(rc, 2)
        self.assertEqual(out, "")
        self.assertIn("NOT survive the formatter", err)
        self.assertIn("// walk members", err)
        self.assertIn("REVIEW.md", err)
        self.assertIn("PRE-EXISTING", err)

    def test_das_write_clean_silent(self):
        src = "options gen2\n// header block\nrequire daslib/fio\n\n//! doc\ndef f() {}\n"
        rc, err, _ = run_hook(write_payload("daslib/foo.das", src))
        self.assertEqual(rc, 0, err)

    def test_das_edit_addition_slapped(self):
        rc, err, _ = run_hook(edit_payload("daslib/foo.das", "foo()", "// explain\nfoo()"))
        self.assertEqual(rc, 2)
        self.assertIn("comment guard", err)
        self.assertNotIn("PRE-EXISTING", err)
        self.assertIn("replacement text", err)

    def test_das_edit_move_silent(self):
        rc, _, _ = run_hook(edit_payload("daslib/foo.das", "// old note\nfoo()", "foo()\n// old note"))
        self.assertEqual(rc, 0)

    def test_cpp_write_slapped_with_c_message(self):
        rc, err, _ = run_hook(write_payload("src/ast/x.cpp", "int f() {\n    return 1; // one\n}\n"))
        self.assertEqual(rc, 2)
        self.assertIn("no new C/C++ comments", err)
        self.assertNotIn("formatter", err)
        self.assertNotIn("new comment(s)", err)

    def test_cpp_edit_says_new(self):
        rc, err, _ = run_hook(edit_payload("src/x.cpp", "int a;", "int a; // narr"))
        self.assertEqual(rc, 2)
        self.assertIn("new comment(s)", err)

    def test_all_c_extensions_guarded(self):
        for ext in cg.C_EXTS:
            rc, err, _ = run_hook(edit_payload("x" + ext, "int a;", "int a; // narr"))
            self.assertEqual(rc, 2, ext)
            self.assertIn("no new C/C++ comments", err, ext)

    def test_h_edit_nolint_silent(self):
        rc, _, _ = run_hook(edit_payload("include/x.h", "int a;", "int a; // NOLINT"))
        self.assertEqual(rc, 0)

    def test_message_caps_quoted_lines(self):
        body = "".join("int v{}; // c{}\n".format(i, i) for i in range(9))
        rc, err, _ = run_hook(write_payload("a.cpp", body))
        self.assertEqual(rc, 2)
        self.assertIn("and 4 more", err)

    def test_single_violation_has_no_more_tail(self):
        rc, err, _ = run_hook(write_payload("a.cpp", "int v; // one\n"))
        self.assertEqual(rc, 2)
        self.assertNotIn(" more", err)

    def test_multiline_block_quoted_first_line_only(self):
        rc, err, _ = run_hook(write_payload("a.cpp", "int x;\n/* line1\nline2 */\n"))
        self.assertEqual(rc, 2)
        self.assertIn("/* line1", err)
        self.assertNotIn("line2", err)

    def test_case_insensitive_extension(self):
        rc, _, _ = run_hook(write_payload("X.DAS", "x = 1 // hm\n"))
        self.assertEqual(rc, 2)

    def test_unicode_survives_to_stderr(self):
        rc, err, _ = run_hook(write_payload("a.das", "x = 1 // от так\n"))
        self.assertEqual(rc, 2)
        self.assertIn("от так", err)

    def test_unicode_survives_legacy_codepage(self):
        env = dict(os.environ, PYTHONIOENCODING="cp1252")
        proc = subprocess.run(
            [sys.executable, GUARD],
            input=json.dumps(write_payload("a.das", "x = 1 // от так\n")).encode("utf-8"),
            capture_output=True, timeout=30, env=env)
        self.assertEqual(proc.returncode, 2)
        self.assertIn("от так".encode("utf-8"), proc.stderr)


class TestCorpus(unittest.TestCase):
    """Known-tricky tracked files: appending a probe comment must still be
    detected — proves no scanner derail (reader macros, huge strings, flex
    output). Files missing in a partial checkout are skipped."""

    DAS_FILES = [
        "utils/lint/REVIEW.das",
        "modules/dasPEG/tests/csv.das",
        "modules/dasPEG/tests/json.das",
        "utils/internal/hygiene/rule_private_docs.das",
        "daslib/aot_constants.das",
        "daslib/das_source_formatter.das",
    ]
    C_FILES = [
        "src/parser/ds2_lexer.cpp",
        "modules/dasStbImage/src/stb_image.h",
    ]

    def _probe(self, rel, is_das):
        path = os.path.join(REPO, rel)
        if not os.path.isfile(path):
            self.skipTest("not in this checkout: " + rel)
        with open(path, encoding="utf-8", errors="replace") as f:
            src = f.read()
        probed = src + "\n__probe__() // __probe_comment__\n"
        found = [t for _, t in cg.violations(probed, is_das, True) if "__probe_comment__" in t]
        self.assertEqual(len(found), 1, rel + ": scanner derailed, probe comment invisible")

    def test_das_corpus(self):
        for rel in self.DAS_FILES:
            with self.subTest(rel):
                self._probe(rel, True)

    def test_c_corpus(self):
        for rel in self.C_FILES:
            with self.subTest(rel):
                self._probe(rel, False)


if __name__ == "__main__":
    unittest.main(verbosity=1)
