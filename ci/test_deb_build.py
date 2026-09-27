#!/usr/bin/env python3
"""Fixture tests for ci/packaging/deb_build.sh -- the control file and links it renders.

Each test stages a tiny fake bundle in a tempdir and reads `--control-only`, so no
dpkg-deb is needed; the payload itself is proven by the release lanes' post-build smoke.

Run: python3 ci/test_deb_build.py
"""
import os
import shutil
import subprocess
import tempfile
import unittest

SCRIPT = os.path.join(os.path.dirname(os.path.abspath(__file__)), "packaging", "deb_build.sh")


class DebControlTest(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.mkdtemp()

    def tearDown(self):
        shutil.rmtree(self.tmp)

    def stage(self, rels):
        bundle = os.path.join(self.tmp, "bundle")
        for rel in rels:
            p = os.path.join(bundle, rel)
            os.makedirs(os.path.dirname(p), exist_ok=True)
            with open(p, "wb") as f:
                f.write(b"\x7fELF")
            os.chmod(p, 0o755)
        return bundle

    def control(self, bundle, tag, *pre):
        return subprocess.run(["bash", SCRIPT, "--control-only", *pre, bundle, tag],
                              check=True, capture_output=True, text=True).stdout

    def field(self, text, name):
        return [ln for ln in text.splitlines() if ln.startswith(name + ":")][0].split(":", 1)[1].strip()

    def test_rc_tag_sorts_before_the_release(self):
        out = self.control(self.stage(["bin/daslang"]), "v0.6.5-RC2")
        self.assertEqual(self.field(out, "Version"), "0.6.5~rc2")

    def test_daslang_declares_no_dependencies(self):
        out = self.control(self.stage(["bin/daslang", "bin/daslang-live"]), "v0.6.5")
        self.assertEqual(self.field(out, "Package"), "daslang")
        self.assertNotIn("Depends:", out)
        self.assertIn("link: /usr/bin/daslang -> /opt/daslang/bin/daslang\n", out)
        self.assertIn("link: /usr/bin/daslang-live -> /opt/daslang/bin/daslang-live\n", out)

    def test_description_continuation_lines_are_indented(self):
        out = self.control(self.stage(["bin/daslang"]), "v0.6.5")
        after = out.split("Description:", 1)[1].splitlines()[1:]
        self.assertTrue(after and all(ln.startswith(" ") for ln in after if not ln.startswith("link:")))

    def test_missing_out_dir_is_usage(self):
        r = subprocess.run(["bash", SCRIPT, self.stage(["bin/daslang"]), "v0.6.5"], capture_output=True, text=True)
        self.assertEqual(r.returncode, 2)
        self.assertIn("usage:", r.stderr)

    def test_dasllama_depends_and_links(self):
        bundle = self.stage(["dasllama-server.exe", "dasllama-cli.exe", "dasllama-bench.exe", "watchdog"])
        out = self.control(bundle, "v0.6.5", "--package", "dasllama")
        self.assertEqual(self.field(out, "Package"), "dasllama")
        self.assertEqual(self.field(out, "Depends"), "libssl3 | libssl3t64, curl")
        self.assertIn("link: /usr/bin/dasllama-server -> /opt/dasllama/dasllama-server.exe\n", out)
        self.assertIn("link: /usr/bin/dasllama-watchdog -> /opt/dasllama/watchdog\n", out)


if __name__ == "__main__":
    unittest.main()
