#!/usr/bin/env python3
"""Tests for ci/ninja_log_report.py -- the per-invocation tables it prints from a .ninja_log.

Run: python3 ci/test_ninja_log_report.py
"""
import os
import subprocess
import sys
import tempfile
import unittest

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import ninja_log_report  # noqa: E402

LOG = "\n".join([
    "# ninja log v6",
    "0\t60000\t0\tCMakeFiles/libDaScript.dir/src/a.cpp.obj\tabc",
    "0\t120000\t0\tCMakeFiles/libDaScript.dir/src/b.cpp.obj\tabd",
    "120000\t180000\t0\tbin\\daslang.dll\tabe",
    "120000\t180000\t0\tlib\\daslang.lib\tabe",
    "0\t1000\t0\tdas_stamps/check_md_ascii.stamp\tabf",
    "0\t300000\t0\tdas_stamps/lint_tree_chunk_7.stamp\tac0",
    "0\t600000\t0\tdas_stamps/lint_tree_chunk_12.stamp\tac1",
    "600000\t660000\t0\tdas_stamps/run_utils_tests.stamp\tac2",
    "600000\t660000\t0\tD:/a/daScript/build/das_stamps/run_utils_tests.stamp\tac2",
    "660000\tgarbage\t0\tdas_stamps/half_written.stamp\tac3",
    "660000\t6600",
])


class NinjaLogReportTest(unittest.TestCase):
    def write(self, text):
        handle, path = tempfile.mkstemp(suffix=".ninja_log")
        with os.fdopen(handle, "w", encoding="utf-8") as log:
            log.write(text)
        self.addCleanup(os.remove, path)
        return path

    def test_an_end_that_goes_back_starts_a_new_invocation(self):
        invocations = ninja_log_report.read_invocations(self.write(LOG))
        self.assertEqual([len(i) for i in invocations], [3, 4])

    def test_the_lines_of_one_edge_are_one_edge(self):
        both = "0\t1000\t0\tbin/x.dll\taaa\n0\t1000\t0\tlib/x.lib\taaa\n"
        self.assertEqual(len(ninja_log_report.read_invocations(self.write(both))[0]), 1)
        two_commands = "0\t1000\t0\tdir_a/x.obj\taaa\n0\t1000\t0\tdir_b/x.obj\tbbb\n"
        self.assertEqual(len(ninja_log_report.read_invocations(self.write(two_commands))[0]), 2)
        no_hash = "0\t1000\t0\tdir_a/x.stamp\n0\t1000\t0\tD:/build/dir_a/x.stamp\n0\t1000\t0\ty.stamp\n"
        self.assertEqual(len(ninja_log_report.read_invocations(self.write(no_hash))[0]), 2)

    def test_an_edge_seen_in_one_invocation_is_counted_again_in_the_next(self):
        again = "0\t1000\t0\ta.stamp\taaa\n0\t500\t0\tb.stamp\tbbb\n0\t1000\t0\ta.stamp\taaa\n"
        self.assertEqual([len(i) for i in ninja_log_report.read_invocations(self.write(again))], [1, 2])

    def test_a_row_is_a_kind_or_an_output_with_its_numbers_folded(self):
        self.assertEqual(ninja_log_report.row_key("CMakeFiles/x.dir/a.cpp.obj"), "(compile)")
        self.assertEqual(ninja_log_report.row_key("lib/libDaScript.a"), "(link)")
        self.assertEqual(ninja_log_report.row_key("das_stamps/lint_tree_chunk_12.stamp"), "lint_tree_chunk_N")
        self.assertEqual(ninja_log_report.row_key("jit_chunk_3"), "jit_chunk_N")

    def test_a_table_sums_a_row_and_orders_by_time(self):
        invocations = ninja_log_report.read_invocations(self.write(LOG))
        build = ninja_log_report.report(invocations[0], 25)
        self.assertEqual(build[0], "wall 3.0 min, 3 edges, 4.0 edge-minutes")
        self.assertRegex(build[2], r"^\s+3\.0\s+2\s+2\.0  \(compile\)$")
        self.assertRegex(build[3], r"^\s+1\.0\s+1\s+1\.0  \(link\)$")
        self.assertEqual(build[4], "slowest single compiles and links:")
        self.assertRegex(build[5], r"^\s+2\.0  CMakeFiles/libDaScript.dir/src/b.cpp.obj$")
        self.assertRegex(build[6], r"^\s+1\.0  bin/daslang.dll$")
        self.assertEqual(len(build), 8)
        checks = ninja_log_report.report(invocations[1], 1)
        self.assertEqual(len(checks), 3)
        self.assertRegex(checks[2], r"^\s+15\.0\s+2\s+10\.0  lint_tree_chunk_N$")

    def test_only_a_slow_compile_or_link_is_listed_and_no_more_than_the_cap(self):
        quick = [(0, ninja_log_report.SLOW_SINGLE_MIN_MS - 1, "a.obj")]
        self.assertNotIn("slowest single compiles and links:", ninja_log_report.report(quick, 25))
        many = [(0, ninja_log_report.SLOW_SINGLE_MIN_MS + n, f"f{n}.obj")
                for n in range(ninja_log_report.SLOW_SINGLES_SHOWN + 3)]
        listed = ninja_log_report.report(many, 25)
        at = listed.index("slowest single compiles and links:")
        self.assertEqual(len(listed) - at - 1, ninja_log_report.SLOW_SINGLES_SHOWN)
        slow_stamp = [(0, 10 * ninja_log_report.SLOW_SINGLE_MIN_MS, "das_stamps/run_x.stamp")]
        self.assertNotIn("slowest single compiles and links:", ninja_log_report.report(slow_stamp, 25))

    def test_the_tool_prints_each_invocation_and_survives_a_missing_log(self):
        script = os.path.join(HERE, "ninja_log_report.py")
        out = subprocess.run([sys.executable, script, self.write(LOG)], capture_output=True, text=True, check=True).stdout
        self.assertIn("ninja log: invocation 1", out)
        self.assertIn("ninja log: invocation 2", out)
        self.assertIn("run_utils_tests", out)
        top_one = subprocess.run([sys.executable, script, self.write(LOG), "1"], capture_output=True, text=True, check=True).stdout
        self.assertNotIn("run_utils_tests", top_one)
        missing = subprocess.run([sys.executable, script, os.path.join(HERE, "no_such_dir", ".ninja_log")],
                                 capture_output=True, text=True)
        self.assertEqual(missing.returncode, 0)
        self.assertIn("did not run under ninja", missing.stdout)
        usage = subprocess.run([sys.executable, script], capture_output=True, text=True)
        self.assertEqual(usage.returncode, 2)


if __name__ == "__main__":
    unittest.main()
