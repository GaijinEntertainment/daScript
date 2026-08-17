#!/usr/bin/env python3
"""Fixture tests for ci/check_shipped_skills.py -- the shipped-skills gate.

The gate's rejection branches are green-on-good-input by nature: a regex typo
mutes one permanently while CI stays green. Each test here builds a tiny bundle
in a tempdir, injects exactly one violation, and asserts the gate fires -- with
a clean twin asserting it stays silent.

Run: python3 ci/test_check_shipped_skills.py
"""
import contextlib
import io
import os
import shutil
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import check_shipped_skills as gate


def write(root, rel, text):
    path = os.path.join(root, rel)
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w", encoding="utf-8") as fh:
        fh.write(text)


class GateFixture(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.mkdtemp(prefix="skills_gate_")
        self.bundle = os.path.join(self.tmp, "bundle")
        self.repo = os.path.join(self.tmp, "repo")
        # minimal clean bundle: one root skill with a trigger row, the language
        # skill, and a repo twin that agrees with it
        write(self.bundle, "CLAUDE.md", "| `skills/good.md` | row |\n")
        write(self.bundle, "skills/good.md", "# Good\n\nplain prose\n")
        write(self.bundle, "skills/daslang/SKILL.md", "# daslang\n\nclean\n")
        write(self.bundle, "skills/daslang/references/ok.md", "# ok\n\nclean\n")
        write(self.repo, "skills/good.md", "# Good\n\nplain prose\n")

    def tearDown(self):
        shutil.rmtree(self.tmp, ignore_errors=True)

    def run_gate(self):
        out = io.StringIO()
        with contextlib.redirect_stdout(out):
            rc = gate.check(self.bundle, repo_root=self.repo)
        return rc, out.getvalue()

    def assert_fires(self, kind, detail_part=None):
        rc, out = self.run_gate()
        self.assertEqual(rc, 1, "gate stayed green:\n" + out)
        self.assertIn(kind, out)
        if detail_part:
            self.assertIn(detail_part, out)

    def test_clean_bundle_passes(self):
        rc, out = self.run_gate()
        self.assertEqual(rc, 0, out)

    def test_purity_mcp(self):
        write(self.bundle, "skills/daslang/references/bad.md",
              "# bad\n\nuse the MCP tool for this\n")
        self.assert_fires("bundle purity", "MCP")

    def test_purity_ci(self):
        write(self.bundle, "skills/daslang/references/bad.md",
              "# bad\n\nthe CI lane runs it nightly\n")
        self.assert_fires("bundle purity", "CI")

    def test_purity_lint_id(self):
        write(self.bundle, "skills/daslang/references/bad.md",
              "# bad\n\nPERF018 fires on this shape\n")
        self.assert_fires("bundle purity", "PERF018")

    def test_no_marker_escape_under_daslang(self):
        # the repo-only marker must NOT exempt the language skill
        write(self.bundle, "skills/daslang/references/bad.md",
              "# bad\n\nsee src/ast/ast.cpp repo-only\n")
        self.assert_fires("not in bundle", "src/ast/ast.cpp")

    def test_repo_path_unmarked_fires_marked_passes(self):
        write(self.bundle, "skills/good.md",
              "# Good\n\nsee src/ast/ast.cpp for detail\n")
        self.assert_fires("not in bundle", "src/ast/ast.cpp")
        write(self.bundle, "skills/good.md",
              "# Good\n\nsee src/ast/ast.cpp for detail (repo-only)\n")
        rc, out = self.run_gate()
        self.assertEqual(rc, 0, out)

    def test_internal_ref_unmarked_fires_marked_passes(self):
        write(self.bundle, "skills/good.md",
              "# Good\n\nsee skills/internal/secret.md for the rest\n")
        self.assert_fires("skill not shipped", "internal/secret.md")
        write(self.bundle, "skills/good.md",
              "# Good\n\nsee skills/internal/secret.md (repo-only)\n")
        rc, out = self.run_gate()
        self.assertEqual(rc, 0, out)

    def test_missing_skill_ref(self):
        write(self.bundle, "skills/good.md", "# Good\n\nsee skills/nope.md\n")
        self.assert_fires("skill not shipped", "nope.md")

    def test_trigger_row_missing(self):
        write(self.bundle, "skills/norow.md", "# NoRow\n\nprose\n")
        write(self.repo, "skills/norow.md", "# NoRow\n\nprose\n")
        self.assert_fires("no trigger row", "norow.md")

    def test_dead_link_fires_letterless_skipped(self):
        write(self.bundle, "skills/daslang/references/ok.md",
              "# ok\n\n[gone](missing.md) but `arr[0](7)` is das syntax\n")
        rc, out = self.run_gate()
        self.assertEqual(rc, 1, out)
        self.assertIn("missing.md", out)
        self.assertNotIn("dead link (2)", out)

    def test_internal_walk_dead_link_only(self):
        # a dead link in an internal skill fires; its src/ mention does not
        write(self.repo, "skills/internal/x.md",
              "# x\n\nsee src/ast/ast.cpp and [gone](missing.md)\n")
        rc, out = self.run_gate()
        self.assertEqual(rc, 1, out)
        self.assertIn("missing.md", out)
        self.assertNotIn("not in bundle", out)

    def test_repo_skill_missing_from_bundle(self):
        # the daslang_live regression class: repo has it, install rules dropped it
        write(self.repo, "skills/dropped.md", "# Dropped\n\nprose\n")
        self.assert_fires("missing from bundle", "dropped.md")


if __name__ == "__main__":
    unittest.main(verbosity=1)
