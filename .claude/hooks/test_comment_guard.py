#!/usr/bin/env python3
"""Tests for comment_guard.py — unit level (scanner, header, diff) and
end-to-end (subprocess with hook JSON on stdin, asserting exit code + stderr)."""

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
    proc = subprocess.run([sys.executable, GUARD], input=json.dumps(payload),
                          capture_output=True, text=True, timeout=30)
    return proc.returncode, proc.stderr


def edit_payload(path, old, new):
    return {"tool_name": "Edit", "tool_input": {"file_path": path, "old_string": old, "new_string": new}}


def write_payload(path, content):
    return {"tool_name": "Write", "tool_input": {"file_path": path, "content": content}}


class TestScanner(unittest.TestCase):
    def test_line_comment(self):
        self.assertEqual(cg.scan_comments("let x = 1 // note"), [(1, "// note", False)])

    def test_full_line_comment(self):
        self.assertEqual(cg.scan_comments("    // note"), [(1, "// note", True)])

    def test_block_single_line(self):
        self.assertEqual(cg.scan_comments("a /* mid */ b"), [(1, "/* mid */", False)])

    def test_block_multi_line(self):
        got = cg.scan_comments("/* one\ntwo\nthree */ let x = 1\nlet y = 2 // tail")
        self.assertEqual(got, [(1, "/* one", True), (4, "// tail", False)])

    def test_two_blocks_one_line(self):
        got = cg.scan_comments("a /* x */ b /* y */ c")
        self.assertEqual([t for _, t, _ in got], ["/* x */", "/* y */"])

    def test_comment_marker_inside_string(self):
        self.assertEqual(cg.scan_comments('let url = "https://x.io"'), [])
        self.assertEqual(cg.scan_comments('let s = "/* not a comment */"'), [])

    def test_comment_after_string(self):
        got = cg.scan_comments('let s = "a//b" // real')
        self.assertEqual(got, [(1, "// real", False)])

    def test_char_literal(self):
        self.assertEqual(cg.scan_comments("if (b == '/') {}"), [])


class TestHeader(unittest.TestCase):
    def test_plain_header(self):
        self.assertEqual(cg.header_extent("// hdr\n// hdr2\n\ndef f() {}", True), 3)

    def test_header_below_preamble(self):
        src = "options gen2\nrequire daslib/fio\n// header prose\ndef f() {}"
        self.assertEqual(cg.header_extent(src, True), 3)

    def test_c_header_stops_at_include(self):
        src = "// copyright\n#include <x.h>\n// not header"
        self.assertEqual(cg.header_extent(src, False), 1)

    def test_block_header(self):
        src = "/* copyright\n   notice */\nint x;"
        self.assertEqual(cg.header_extent(src, False), 2)


class TestViolations(unittest.TestCase):
    def test_kept_set_das(self):
        src = "//! doc\ndef f() {} // nolint: STYLE037 short why\n//fmt: ignore-file\n// fmt: off\n"
        self.assertEqual(cg.violations(src, True, False), [])

    def test_kept_set_c(self):
        src = "//! doc\nint x; // NOLINT(readability)\n"
        self.assertEqual(cg.violations(src, False, False), [])

    def test_fmt_not_kept_in_c(self):
        self.assertEqual(len(cg.violations("//fmt: off\n", False, False)), 1)

    def test_narration_flagged(self):
        self.assertEqual(len(cg.violations("x = 1 // bump it\n", True, False)), 1)

    def test_block_never_kept_outside_header(self):
        src = "def f() {}\n/*! doc-ish */\n"
        self.assertEqual(len(cg.violations(src, True, True)), 1)

    def test_header_exempt_in_whole_file(self):
        src = "// header\noptions gen2\n// still header\ndef f() {}\n"
        self.assertEqual(cg.violations(src, True, True), [])

    def test_trailing_comment_on_preamble_line_flagged(self):
        src = "options gen2\nrequire daslib/fio // for fopen\ndef f() {}\n"
        got = cg.violations(src, True, True)
        self.assertEqual([t for _, t in got], ["// for fopen"])

    def test_edit_move_is_silent(self):
        old = "    // legacy narration\n    foo()\n"
        new = "// legacy narration\nfoo()\nbar()\n"
        self.assertEqual(cg.edit_violations(old, new, True), [])

    def test_edit_addition_flagged(self):
        old = "foo()\n"
        new = "foo() // call foo\n"
        self.assertEqual(len(cg.edit_violations(old, new, True)), 1)


class TestEndToEnd(unittest.TestCase):
    def test_non_source_file_silent(self):
        rc, _ = run_hook(write_payload("readme.md", "# // not code"))
        self.assertEqual(rc, 0)

    def test_other_tool_silent(self):
        rc, _ = run_hook({"tool_name": "Bash", "tool_input": {"command": "ls"}})
        self.assertEqual(rc, 0)

    def test_bad_json_silent(self):
        proc = subprocess.run([sys.executable, GUARD], input="not json",
                              capture_output=True, text=True, timeout=30)
        self.assertEqual(proc.returncode, 0)

    def test_missing_fields_silent(self):
        rc, _ = run_hook({"tool_name": "Edit"})
        self.assertEqual(rc, 0)

    def test_das_write_narration_slapped(self):
        rc, err = run_hook(write_payload("daslib/foo.das", "def f() {\n    // walk members\n}\n"))
        self.assertEqual(rc, 2)
        self.assertIn("NOT survive the formatter", err)
        self.assertIn("// walk members", err)
        self.assertIn("REVIEW.md", err)

    def test_das_write_clean_silent(self):
        src = "options gen2\n// header block\nrequire daslib/fio\n\n//! doc\ndef f() {}\n"
        rc, err = run_hook(write_payload("daslib/foo.das", src))
        self.assertEqual(rc, 0, err)

    def test_das_edit_addition_slapped(self):
        rc, err = run_hook(edit_payload("daslib/foo.das", "foo()", "// explain\nfoo()"))
        self.assertEqual(rc, 2)
        self.assertIn("comment guard", err)

    def test_das_edit_move_silent(self):
        rc, _ = run_hook(edit_payload("daslib/foo.das", "// old note\nfoo()", "foo()\n// old note"))
        self.assertEqual(rc, 0)

    def test_cpp_write_slapped_with_c_message(self):
        rc, err = run_hook(write_payload("src/ast/x.cpp", "int f() {\n    return 1; // one\n}\n"))
        self.assertEqual(rc, 2)
        self.assertIn("no new C/C++ comments", err)
        self.assertNotIn("formatter", err)

    def test_h_edit_nolint_silent(self):
        rc, _ = run_hook(edit_payload("include/x.h", "int a;", "int a; // NOLINT"))
        self.assertEqual(rc, 0)

    def test_message_caps_quoted_lines(self):
        body = "".join("int v{}; // c{}\n".format(i, i) for i in range(9))
        rc, err = run_hook(write_payload("a.cpp", body))
        self.assertEqual(rc, 2)
        self.assertIn("and 4 more", err)

    def test_case_insensitive_extension(self):
        rc, _ = run_hook(write_payload("X.DAS", "x = 1 // hm\n"))
        self.assertEqual(rc, 2)


if __name__ == "__main__":
    unittest.main(verbosity=2)
