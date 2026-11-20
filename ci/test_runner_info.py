#!/usr/bin/env python3
"""Fixture tests for ci/runner_info.sh -- the cache-size lines it prints.

Each test stages a tiny tree in a tempdir and runs the script from there, the way a
workflow job runs it from the checkout.

Run: python3 ci/test_runner_info.py
"""
import os
import shutil
import subprocess
import tempfile
import unittest

SCRIPT = os.path.join(os.path.dirname(os.path.abspath(__file__)), "runner_info.sh")


class RunnerInfoCacheSizesTest(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.mkdtemp()

    def tearDown(self):
        shutil.rmtree(self.tmp)

    def stage(self, rel, size):
        path = os.path.join(self.tmp, rel)
        os.makedirs(os.path.dirname(path), exist_ok=True)
        with open(path, "wb") as f:
            f.write(b"\0" * size)

    def cache_lines(self, sccache_dir=None):
        env = dict(os.environ)
        env.pop("SCCACHE_DIR", None)
        if sccache_dir is not None:
            env["SCCACHE_DIR"] = sccache_dir
        out = subprocess.run(["bash", SCRIPT], cwd=self.tmp, env=env, capture_output=True, text=True, check=True).stdout
        return [line for line in out.splitlines() if line.startswith("runner: cache ")]

    def test_no_cache_directory_prints_no_cache_line(self):
        self.assertEqual(self.cache_lines(), [])

    def test_each_existing_directory_gets_its_line(self):
        self.stage("sc/obj", 2 * 1024 * 1024)
        self.stage(".cache/daslang/test_cache/a", 10)
        self.stage(".cache/daslang/_anon_1/x/a.o", 3 * 1024 * 1024)
        self.stage(".cache/daslang/_anon_2/b.dll", 2 * 1024 * 1024)
        sccache = os.path.join(self.tmp, "sc")
        lines = self.cache_lines(sccache)
        self.assertEqual(len(lines), 3, lines)
        self.assertRegex(lines[0], r"^runner: cache [23] MiB .*sc$")
        self.assertRegex(lines[1], r"^runner: cache \d+ MiB \.cache/daslang/test_cache$")
        self.assertRegex(lines[2], r"^runner: cache [5-7] MiB \.cache/daslang/_anon_\* \(2 dirs\)$")

    def test_a_cache_with_no_jit_folder_prints_no_jit_line(self):
        self.stage(".cache/daslang/lint_cache/a", 10)
        lines = self.cache_lines()
        self.assertEqual(len(lines), 1, lines)
        self.assertIn(".cache/daslang/lint_cache", lines[0])


if __name__ == "__main__":
    unittest.main()
