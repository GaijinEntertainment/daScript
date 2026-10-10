#!/usr/bin/env python3
"""Fixture tests for ci/tree_sitter_parse_sources.sh -- which files it hands to the parser.

Each test stages a tiny git repository with the script, a rejected-sources list and .das files, and puts a stub
`tree-sitter` on PATH that records the --paths file it receives and exits with a chosen status, so no grammar build
is needed; the real parse runs in the tree_sitter_daslang_sources workflow.

Run: python3 ci/test_tree_sitter_parse_sources.py
"""
import os
import shutil
import stat
import subprocess
import tempfile
import unittest

SCRIPT = os.path.join(os.path.dirname(os.path.abspath(__file__)), "tree_sitter_parse_sources.sh")
LIST = "ci/tree_sitter_compiler_rejected_sources.txt"

STUB = """#!/usr/bin/env bash
while (( $# )); do
  if [[ "$1" == --paths ]]; then cp "$2" "$STUB_OUT"; fi
  shift
done
exit "${STUB_RC:-0}"
"""


class ParseSourcesTest(unittest.TestCase):
    def setUp(self):
        self.repo = tempfile.mkdtemp()
        self.bin = os.path.join(self.repo, "stub-bin")
        os.makedirs(os.path.join(self.repo, "ci"))
        os.makedirs(self.bin)
        shutil.copy(SCRIPT, os.path.join(self.repo, "ci", "tree_sitter_parse_sources.sh"))
        stub = os.path.join(self.bin, "tree-sitter")
        with open(stub, "w") as f:
            f.write(STUB)
        os.chmod(stub, os.stat(stub).st_mode | stat.S_IEXEC)
        self.out = os.path.join(self.repo, "stub-bin", "paths.txt")
        self.git("init", "-q")

    def tearDown(self):
        shutil.rmtree(self.repo)

    def git(self, *args):
        subprocess.run(["git", *args], cwd=self.repo, check=True, capture_output=True)

    def write(self, rel, data):
        path = os.path.join(self.repo, rel)
        os.makedirs(os.path.dirname(path), exist_ok=True)
        with open(path, "wb") as f:
            f.write(data)

    def run_script(self, rc=0):
        self.git("add", "-A", "--", ".", ":!stub-bin")
        env = dict(os.environ, PATH=self.bin + os.pathsep + os.environ["PATH"], STUB_OUT=self.out,
                   STUB_RC=str(rc))
        return subprocess.run(["bash", "ci/tree_sitter_parse_sources.sh"], cwd=self.repo, env=env,
                              capture_output=True, text=True)

    def parsed(self):
        with open(self.out) as f:
            return sorted(line.strip() for line in f if line.strip())

    def test_selects_gen2_files_by_their_first_marker(self):
        self.write("gen2.das", b"options gen2\ndef f() {\n}\n")
        self.write("no_marker.das", b"def f() {\n}\n")
        self.write("gen1.das", b"options gen2 = false\n")
        self.write("gen1_tight.das", b"  options gen2=false\n")
        self.write("gen1_bom.das", b"\xef\xbb\xbfoptions gen2 = false\n")
        self.write("first_marker_wins.das", b"options gen2\n//fmt:ignore-file\noptions gen2 = false\n")
        self.write("other_option.das", b"options gen2_extra = false\n")
        self.write("rejected.das", b"options gen2\ndef (\n")
        self.write(LIST, b"# comment\n\nrejected.das\n")
        result = self.run_script()
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(self.parsed(), ["first_marker_wins.das", "gen2.das", "no_marker.das", "other_option.das"])

    def test_fails_when_the_parse_fails(self):
        self.write("gen2.das", b"options gen2\n")
        self.write(LIST, b"")
        result = self.run_script(rc=1)
        self.assertNotEqual(result.returncode, 0)

    def test_fails_on_a_list_entry_git_does_not_track(self):
        self.write("gen2.das", b"options gen2\n")
        self.write(LIST, b"gone.das\n")
        result = self.run_script()
        self.assertNotEqual(result.returncode, 0)
        self.assertIn("gone.das", result.stderr)
        self.assertFalse(os.path.exists(self.out), "the parse does not run")


if __name__ == "__main__":
    unittest.main()
