#!/usr/bin/env python3
"""Tests for comment_guard.py — unit level (scanner, header, kept sets, edit
diff) and end-to-end (subprocess with hook JSON on stdin, asserting exit code,
stderr, and empty stdout). The grammar is finite; the matrix walks it:
comment forms x string/char forms x header positions x edit modes x extensions."""

import json
import os
import subprocess
import sys
import unittest

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)

import comment_guard as cg

GUARD = os.path.join(HERE, "comment_guard.py")


def run_hook(payload):
    raw = payload if isinstance(payload, str) else json.dumps(payload)
    proc = subprocess.run([sys.executable, GUARD], input=raw,
                          capture_output=True, text=True, timeout=30)
    return proc.returncode, proc.stderr, proc.stdout


def edit_payload(path, old, new):
    return {"tool_name": "Edit", "tool_input": {"file_path": path, "old_string": old, "new_string": new}}


def write_payload(path, content):
    return {"tool_name": "Write", "tool_input": {"file_path": path, "content": content}}


def comments(text, is_das=True):
    return cg.scan(text, is_das)[0]


def hdr(text, is_das):
    return cg.header_extent(text, is_das, cg.scan(text, is_das)[1])


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

    def test_lone_apostrophe_does_not_mask(self):
        got = comments("its = 1 // don't and won't")
        self.assertEqual(got, [(1, "// don't and won't", False)])

    def test_apostrophe_before_block_close(self):
        got = comments("/* it's */ let c = 'x' // note\n// second", is_das=True)
        self.assertEqual([t for _, t, _ in got], ["/* it's */", "// note", "// second"])

    def test_digit_separators(self):
        self.assertEqual(comments("int n = 1'000'000; // big", is_das=False),
                         [(1, "// big", False)])

    def test_multi_line_string_das(self):
        src = 'let AOT = "\n// unreferenced formal parameter\n#pragma warning\n"\nx = 1 // real'
        self.assertEqual(comments(src, is_das=True), [(5, "// real", False)])

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

    def test_url_in_code_fragment_not_comment(self):
        self.assertEqual(comments("see http://old.example.com", is_das=False), [])

    def test_url_at_line_start_is_comment(self):
        self.assertEqual(comments("// see notes"), [(1, "// see notes", True)])


class TestHeader(unittest.TestCase):
    def test_plain_header(self):
        self.assertEqual(hdr("// hdr\n// hdr2\n\ndef f() {}", True), 3)

    def test_header_below_options_require(self):
        src = "options gen2\nrequire daslib/fio\n// header prose\ndef f() {}"
        self.assertEqual(hdr(src, True), 3)

    def test_header_below_module(self):
        src = "module foo shared\n// header prose\ndef f() {}"
        self.assertEqual(hdr(src, True), 2)

    def test_c_header_stops_at_code(self):
        self.assertEqual(hdr("// copyright\nint x;\n// not header", False), 1)

    def test_c_header_below_preprocessor(self):
        src = "#pragma once\n// copyright\n#include <x.h>\nint x;"
        self.assertEqual(hdr(src, False), 3)

    def test_c_include_guard(self):
        src = "#ifndef X_H\n#define X_H\n// copyright\nint x;"
        self.assertEqual(hdr(src, False), 3)

    def test_cpp20_module_is_not_preamble(self):
        self.assertEqual(hdr("module;\n// after\nint x;", False), 0)

    def test_das_preamble_not_granted_in_c(self):
        self.assertEqual(hdr("options gen2\n// x", False), 0)

    def test_block_header(self):
        self.assertEqual(hdr("/* copyright\n   notice */\nint x;", False), 2)

    def test_self_closing_block_header(self):
        self.assertEqual(hdr("/* copyright */\nint x;", False), 1)

    def test_code_after_block_close_ends_header(self):
        self.assertEqual(hdr("/* header */ int x;\n// narr", False), 0)

    def test_code_after_multiline_block_close_ends_header(self):
        src = "/* header\nmore */ int x;\n// narr"
        self.assertEqual(hdr(src, False), 1)
        self.assertEqual([t for _, t in cg.violations(src, False, True)], ["// narr"])


class TestKept(unittest.TestCase):
    def test_das_kept_set(self):
        src = "//! doc\nfoo() // nolint: STYLE037 short why\n//fmt: ignore-file\n// @nolint tag\n"
        self.assertEqual(cg.violations(src, True, False), [])

    def test_das_fmt_space_flagged(self):
        self.assertEqual(len(cg.violations("// fmt: off\n", True, False)), 1)

    def test_das_bare_nolint_no_colon_flagged(self):
        self.assertEqual(len(cg.violations("// nolint missing colon\n", True, False)), 1)

    def test_das_nolint_case_sensitive(self):
        self.assertEqual(len(cg.violations("// NOLINT: X\n", True, False)), 1)

    def test_das_clang_format_flagged(self):
        self.assertEqual(len(cg.violations("// clang-format off\n", True, False)), 1)

    def test_c_kept_set(self):
        src = ("//! doc\nint x; // NOLINT(readability)\n// NOLINTNEXTLINE(bugprone-x)\n"
              "// NOLINTBEGIN\n// nolint: lower\n// clang-format off\n/*! dox */\n/** dox */\n")
        self.assertEqual(cg.violations(src, False, False), [])

    def test_c_fmt_directive_flagged(self):
        self.assertEqual(len(cg.violations("//fmt: off\n", False, False)), 1)

    def test_c_empty_block_flagged(self):
        self.assertEqual(len(cg.violations("int x;\n/**/\n", False, False)), 1)

    def test_das_doxygen_block_flagged(self):
        self.assertEqual(len(cg.violations("def f() {}\n/*! doc-ish */\n", True, True)), 1)

    def test_narration_flagged(self):
        self.assertEqual(len(cg.violations("x = 1 // bump it\n", True, False)), 1)


class TestWholeFile(unittest.TestCase):
    def test_header_exempt(self):
        src = "// header\noptions gen2\n// still header\ndef f() {}\n"
        self.assertEqual(cg.violations(src, True, True), [])

    def test_trailing_comment_on_preamble_line_flagged(self):
        src = "options gen2\nrequire daslib/fio // for fopen\ndef f() {}\n"
        self.assertEqual([t for _, t in cg.violations(src, True, True)], ["// for fopen"])

    def test_bom_does_not_kill_header(self):
        self.assertEqual(cg.violations("﻿// copyright\nint x;\n", False, True), [])

    def test_comment_after_header_code_line_flagged(self):
        src = "/* header */ int x;\n// narration\n"
        self.assertEqual(len(cg.violations(src, False, True)), 2)

    def test_crlf_input(self):
        src = "// header\r\nint x;\r\nint y; // narr\r\n"
        self.assertEqual([t for _, t in cg.violations(src, False, True)], ["// narr"])

    def test_empty_input(self):
        self.assertEqual(cg.violations("", True, True), [])


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

    def test_das_edit_move_silent(self):
        rc, _, _ = run_hook(edit_payload("daslib/foo.das", "// old note\nfoo()", "foo()\n// old note"))
        self.assertEqual(rc, 0)

    def test_cpp_write_slapped_with_c_message(self):
        rc, err, _ = run_hook(write_payload("src/ast/x.cpp", "int f() {\n    return 1; // one\n}\n"))
        self.assertEqual(rc, 2)
        self.assertIn("no new C/C++ comments", err)
        self.assertNotIn("formatter", err)

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

    def test_unicode_content(self):
        rc, err, _ = run_hook(write_payload("a.das", "x = 1 // от так\n"))
        self.assertEqual(rc, 2)


if __name__ == "__main__":
    unittest.main(verbosity=1)
