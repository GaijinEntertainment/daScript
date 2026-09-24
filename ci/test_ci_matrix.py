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
    # the fast-math cell is a Release/none cell too, so its flag has to reach the name; a
    # sanitizer cell is one job per phase, so the phase does too
    return sorted("%s-%s-%s-%s%s%s" % (c["target"], c["architecture"], c["cmake_preset"], c["sanitizers"],
                                       "-fastmath" if c.get("fast_math") == "ON" else "",
                                       "" if c.get("phase", "all") == "all" else "-" + c["phase"])
                  for c in cells)


PR_BUILD = sorted([
    "linux-64-Debug-none", "linux-64-Release-none",
    "linux_arm-64-Debug-none", "linux_arm-64-Release-none",
    "darwin15-arm64-Debug-none", "darwin15-arm64-Release-none",
    "darwin26-arm64-Debug-none", "darwin26-arm64-Release-none",
    "windows-32-Release-none", "windows-64-Release-none",
])
NIGHTLY_ONLY_BUILD = sorted(
    ["windows-64-Debug-none", "linux-64-Release-none-fastmath"]
    + ["linux-64-Release-%s-%s" % (san, phase) for san in ("asan", "tsan", "ubsan")
       for phase in ("tests", "slow", "backend")])


class BuildMatrix(unittest.TestCase):
    def test_per_pr_cells(self):
        self.assertEqual(names(ci_matrix.build_cells(False)), PR_BUILD)

    def test_nightly_adds_exactly_the_slow_cells(self):
        release = [n for n in PR_BUILD if "-Release-" in n]
        self.assertEqual(names(ci_matrix.build_cells(True)), sorted(release + NIGHTLY_ONLY_BUILD))

    def test_nightly_only_cells_are_marked(self):
        marked = names(c for c in ci_matrix.build_cells(True) if c.get("nightly_only") == "ON")
        self.assertEqual(marked, NIGHTLY_ONLY_BUILD)
        self.assertFalse(any(c.get("nightly_only") for c in ci_matrix.build_cells(False)))

    def test_every_cell_names_a_runner_and_a_generator(self):
        for cell in ci_matrix.build_cells(True):
            self.assertTrue(cell.get("runner"), cell)
            self.assertEqual(cell.get("cmake_generator"), "Ninja", cell)
            self.assertEqual(cell.get("build_system"), "cmake", cell)

    def test_windows_release_64_is_the_llvm_free_gate(self):
        cell = [c for c in ci_matrix.build_cells(False)
                if c["target"] == "windows" and c["architecture"] == 64 and c["cmake_preset"] == "Release"][0]
        self.assertEqual((cell["llvm_disabled"], cell["jit_disabled"]), ("ON", "ON"))

    def test_release_cells_carry_archive_fields(self):
        for cell in ci_matrix.build_cells(False):
            self.assertIn("release_target", cell, cell)
            self.assertIn("release_arch", cell, cell)
            self.assertIn("archive_ext", cell, cell)

    def test_sanitizer_cells_carry_no_archive_fields(self):
        for cell in ci_matrix.build_cells(True):
            if cell["sanitizers"] != "none":
                self.assertNotIn("release_target", cell, cell)
                self.assertEqual(cell["build_name"], "linux_" + cell["sanitizers"])

    def test_only_sanitizer_cells_split_into_phases(self):
        # build.yml gates its three test steps on the phase; a cell that ran none of them would be
        # a build with no signal, and a non-sanitizer cell split would multiply the per-PR matrix
        for cell in ci_matrix.build_cells("schedule"):
            if cell["sanitizers"] == "none":
                self.assertEqual(cell["phase"], "all", cell)
            else:
                self.assertIn(cell["phase"], ("tests", "slow", "backend"), cell)


class ExtendedMatrix(unittest.TestCase):
    def test_per_pr_is_two_darwin_roles(self):
        cells = ci_matrix.extended_cells(False)
        self.assertEqual([(c["target"], c["role"]) for c in cells], [("darwin15", "core"), ("darwin15", "modules")])

    def test_nightly_is_one_full_job_per_platform(self):
        cells = ci_matrix.extended_cells(True)
        self.assertEqual([(c["target"], c["role"]) for c in cells],
                         [("linux", "all"), ("darwin15", "all"), ("windows", "all")])

    def test_every_cell_names_a_runner(self):
        for nightly in (False, True):
            for cell in ci_matrix.extended_cells(nightly):
                self.assertTrue(cell.get("runner"), cell)
                self.assertIn("architecture", cell, cell)


class WorkflowShapes(unittest.TestCase):
    """The workflows consume the matrices through pre_job; the role split must keep the nightly
    `all` role running every step."""

    def read(self, name):
        with open(os.path.join(WORKFLOWS, name), encoding="utf-8") as f:
            return f.read()

    def test_both_workflows_read_their_matrix_from_pre_job(self):
        for name, job in (("build_matrix.yml", "matrix"), ("extended_checks.yml", "pre_job")):
            text = self.read(name)
            self.assertIn("matrix: ${{ fromJSON(needs.%s.outputs.matrix) }}" % job, text, name)
            self.assertIn("ci/ci_matrix.py ", text, name)

    def test_role_conditions_exclude_one_role_only(self):
        # `matrix.role == 'core'` would drop the step from the nightly `all` job; the only admitted
        # spelling names the role a step does NOT run in
        text = self.read("extended_checks.yml")
        refs = re.findall(r"matrix\.role\s*(==|!=)\s*'([a-z]+)'", text)
        self.assertTrue(refs, "extended_checks.yml carries no role conditions")
        for op, role in refs:
            self.assertEqual(op, "!=", "matrix.role %s '%s'" % (op, role))
            self.assertIn(role, ("core", "modules"), "matrix.role %s '%s'" % (op, role))

    def test_every_cache_restore_runs_on_the_cron_too(self):
        # the nightly cells are the slow ones; a restore gated off `schedule` is the old cold-build
        # policy coming back
        for name in ("build.yml", "build_matrix.yml", "nightly.yml", "extended_checks.yml"):
            text = self.read(name)
            for block in re.findall(r"- name: \"(?:Restore|Cache) [^\"]*\"\n((?:      .*\n)+)", text):
                self.assertNotIn("github.event_name != 'schedule'", block, name)

    def test_one_sanitizer_phase_writes_the_shared_slot(self):
        text = self.read("build_matrix.yml")
        self.assertIn("github.ref == 'refs/heads/master' && matrix.phase != 'slow' && matrix.phase != 'backend'", text)
        self.assertNotIn("matrix.nightly_only != 'ON'", text)

    def test_the_three_test_steps_gate_on_the_phase(self):
        text = self.read("build_matrix.yml")
        self.assertIn("(matrix.phase == 'all' || matrix.phase == 'tests')", text)
        self.assertIn("inputs.lane == 'build_nightly' && matrix.phase == 'slow'", text)
        self.assertIn("inputs.lane == 'build_nightly' && matrix.phase == 'backend'", text)

    def test_the_slot_is_saved_after_the_lane_compiled(self):
        # Build only configures; the lane targets compile, so a save placed before them lands
        # an empty slot
        text = self.read("build_matrix.yml")
        self.assertLess(text.index('- name: "Nightly Backend Sweeps"'), text.index('- name: "Save sccache objects"'))

    # the checks that run only on the nightly cron, pinned by name: moving another off the
    # per-PR path is a deliberate edit here, with its preflight mirror or platform reason stated
    NIGHTLY_ONLY = {
        "run_examples",
        "dry_run_tutorials",
        "check_doc_verify",
        "check_nano_arm",
        "check_ast_verify_tree",
        "run_coverage",
    }

    def test_nightly_only_checks_are_exactly_the_pinned_set(self):
        """extended_checks.yml runs one group target per role; ci/CMakeLists.txt assigns the
        checks, so the nightly-only set is read there rather than from step conditions."""
        text = open(os.path.join(os.path.dirname(HERE), "ci", "CMakeLists.txt"), encoding="utf-8").read()
        body = text.split("set(DAS_EXTENDED_NIGHTLY", 1)[1].split("das_lane_group", 1)[0]
        named = set(re.findall(r"\b(?:check|run|dry)_[a-z_0-9]+", body))
        self.assertEqual(named, self.NIGHTLY_ONLY)


class CommandLine(unittest.TestCase):
    def run_tool(self, *args):
        tool = os.path.join(HERE, "ci_matrix.py")
        return subprocess.run([sys.executable, tool, *args], capture_output=True, text=True)

    def test_emits_one_json_line_the_workflow_can_fromjson(self):
        for kind in ("build", "extended"):
            out = self.run_tool(kind)
            self.assertEqual(out.returncode, 0, out.stderr)
            self.assertEqual(out.stdout.count("\n"), 1)
            self.assertIn("include", json.loads(out.stdout))

    def test_rejects_an_unknown_matrix(self):
        self.assertEqual(self.run_tool("release").returncode, 2)

    def test_rejects_a_missing_lane(self):
        self.assertEqual(self.run_tool().returncode, 2)

    def test_each_kind_emits_its_own_cell_shape(self):
        build = json.loads(self.run_tool("build").stdout)["include"]
        extended = json.loads(self.run_tool("extended").stdout)["include"]
        self.assertTrue(all("sanitizers" in c and "role" not in c for c in build))
        self.assertTrue(all("role" in c and "sanitizers" not in c for c in extended))


if __name__ == "__main__":
    unittest.main()
