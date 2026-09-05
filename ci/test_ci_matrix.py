#!/usr/bin/env python3
"""Pins the per-PR and nightly job matrices ci_matrix.py emits, and the workflow shapes that
consume them."""
import json
import os
import re
import subprocess
import sys
import unittest

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import ci_matrix  # noqa: E402

WORKFLOWS = os.path.join(os.path.dirname(HERE), ".github", "workflows")


def names(cells):
    return sorted("%s-%s-%s-%s" % (c["target"], c["architecture"], c["cmake_preset"], c["sanitizers"]) for c in cells)


PR_BUILD = sorted([
    "linux-64-Debug-none", "linux-64-Release-none",
    "linux_arm-64-Debug-none", "linux_arm-64-Release-none",
    "darwin15-arm64-Debug-none", "darwin15-arm64-Release-none",
    "darwin26-arm64-Debug-none", "darwin26-arm64-Release-none",
    "windows-32-Release-none", "windows-64-Release-none",
])
NIGHTLY_ONLY_BUILD = sorted([
    "windows-64-Debug-none", "linux-64-Release-asan", "linux-64-Release-tsan", "linux-64-Release-ubsan",
])


class BuildMatrix(unittest.TestCase):
    def test_per_pr_cells(self):
        for event in ("pull_request", "push"):
            self.assertEqual(names(ci_matrix.build_cells(event)), PR_BUILD, event)

    def test_nightly_adds_exactly_the_slow_cells(self):
        for event in ("schedule", "workflow_dispatch"):
            self.assertEqual(names(ci_matrix.build_cells(event)), sorted(PR_BUILD + NIGHTLY_ONLY_BUILD), event)

    def test_nightly_only_cells_are_marked(self):
        marked = names(c for c in ci_matrix.build_cells("schedule") if c.get("nightly_only") == "ON")
        self.assertEqual(marked, NIGHTLY_ONLY_BUILD)
        self.assertFalse(any(c.get("nightly_only") for c in ci_matrix.build_cells("pull_request")))

    def test_every_cell_names_a_runner_and_a_generator(self):
        for cell in ci_matrix.build_cells("schedule"):
            self.assertTrue(cell.get("runner"), cell)
            self.assertEqual(cell.get("cmake_generator"), "Ninja", cell)
            self.assertEqual(cell.get("build_system"), "cmake", cell)

    def test_windows_release_64_is_the_llvm_free_gate(self):
        cell = [c for c in ci_matrix.build_cells("pull_request")
                if c["target"] == "windows" and c["architecture"] == 64 and c["cmake_preset"] == "Release"][0]
        self.assertEqual((cell["llvm_disabled"], cell["jit_disabled"]), ("ON", "ON"))

    def test_release_cells_carry_archive_fields(self):
        for cell in ci_matrix.build_cells("pull_request"):
            self.assertIn("release_target", cell, cell)
            self.assertIn("release_arch", cell, cell)
            self.assertIn("archive_ext", cell, cell)

    def test_sanitizer_cells_carry_no_archive_fields(self):
        for cell in ci_matrix.build_cells("schedule"):
            if cell["sanitizers"] != "none":
                self.assertNotIn("release_target", cell, cell)
                self.assertEqual(cell["build_name"], "linux_" + cell["sanitizers"])


class ExtendedMatrix(unittest.TestCase):
    def test_per_pr_is_two_darwin_roles(self):
        for event in ("pull_request", "push"):
            cells = ci_matrix.extended_cells(event)
            self.assertEqual([(c["target"], c["role"]) for c in cells], [("darwin15", "core"), ("darwin15", "modules")], event)

    def test_nightly_is_one_full_job_per_platform(self):
        for event in ("schedule", "workflow_dispatch"):
            cells = ci_matrix.extended_cells(event)
            self.assertEqual([(c["target"], c["role"]) for c in cells],
                             [("linux", "all"), ("darwin15", "all"), ("windows", "all")], event)

    def test_every_cell_names_a_runner(self):
        for event in ("pull_request", "schedule"):
            for cell in ci_matrix.extended_cells(event):
                self.assertTrue(cell.get("runner"), cell)
                self.assertIn("architecture", cell, cell)


class WorkflowShapes(unittest.TestCase):
    """The workflows consume the matrices through pre_job; the role split must keep the nightly
    `all` role running every step."""

    def read(self, name):
        with open(os.path.join(WORKFLOWS, name), encoding="utf-8") as f:
            return f.read()

    def test_both_workflows_read_their_matrix_from_pre_job(self):
        for name, kind in (("build.yml", "build"), ("extended_checks.yml", "extended")):
            text = self.read(name)
            self.assertIn("matrix: ${{ fromJSON(needs.pre_job.outputs.matrix) }}", text, name)
            self.assertIn("ci/ci_matrix.py %s" % kind, text, name)

    def test_role_conditions_exclude_one_role_only(self):
        # `matrix.role == 'core'` would drop the step from the nightly `all` job; the only admitted
        # spelling names the role a step does NOT run in
        text = self.read("extended_checks.yml")
        refs = re.findall(r"matrix\.role\s*(==|!=)\s*'([a-z]+)'", text)
        self.assertTrue(refs, "extended_checks.yml carries no role conditions")
        for op, role in refs:
            self.assertEqual(op, "!=", "matrix.role %s '%s'" % (op, role))
            self.assertIn(role, ("core", "modules"), "matrix.role %s '%s'" % (op, role))

    def test_nightly_only_build_cells_save_no_sccache_slot(self):
        text = self.read("build.yml")
        self.assertIn("if: github.ref == 'refs/heads/master' && matrix.nightly_only != 'ON'", text)


class CommandLine(unittest.TestCase):
    def run_tool(self, *args):
        tool = os.path.join(HERE, "ci_matrix.py")
        return subprocess.run([sys.executable, tool, *args], capture_output=True, text=True)

    def test_emits_one_json_line_the_workflow_can_fromjson(self):
        for kind in ("build", "extended"):
            out = self.run_tool(kind, "pull_request")
            self.assertEqual(out.returncode, 0, out.stderr)
            self.assertEqual(out.stdout.count("\n"), 1)
            self.assertIn("include", json.loads(out.stdout))

    def test_rejects_an_unknown_matrix(self):
        self.assertEqual(self.run_tool("release", "push").returncode, 2)


if __name__ == "__main__":
    unittest.main()
