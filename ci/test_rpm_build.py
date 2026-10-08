#!/usr/bin/env python3
"""Fixture tests for ci/packaging/rpm_build.sh -- the spec it renders.

Each test stages a tiny fake bundle in a tempdir and reads the spec from
`--spec-only`, so no rpmbuild is needed; the payload itself is proven by the
release lane's post-build smoke.

Run: python3 ci/test_rpm_build.py
"""
import os
import shutil
import subprocess
import tempfile
import unittest

SCRIPT = os.path.join(os.path.dirname(os.path.abspath(__file__)), "packaging", "rpm_build.sh")


class RpmSpecTest(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.mkdtemp()
        self.bundle = os.path.join(self.tmp, "daslang_bundle")
        os.makedirs(os.path.join(self.bundle, "bin"))

    def tearDown(self):
        shutil.rmtree(self.tmp)

    def stage_exe(self, name):
        p = os.path.join(self.bundle, "bin", name)
        with open(p, "wb") as f:
            f.write(b"\x7fELF")
        os.chmod(p, 0o755)

    def spec(self, tag):
        return subprocess.run(["bash", SCRIPT, "--spec-only", self.bundle, tag],
                              check=True, capture_output=True, text=True).stdout

    def field(self, spec, name):
        return [ln for ln in spec.splitlines() if ln.startswith(name + ":")][0].split(":", 1)[1].strip()

    # --- version mapping ------------------------------------------------------

    def test_release_tag_drops_the_v(self):
        self.assertEqual(self.field(self.spec("v0.6.5"), "Version"), "0.6.5")

    def test_rc_tag_sorts_before_the_release(self):
        self.assertEqual(self.field(self.spec("v0.6.5-RC2"), "Version"), "0.6.5~rc2")

    def test_dev_tag_has_no_hyphen(self):
        # rpm rejects a hyphen inside Version; the workflow's dispatch default is 0.0.0-dev
        self.assertEqual(self.field(self.spec("0.0.0-dev"), "Version"), "0.0.0_dev")

    def test_missing_tag_defaults_to_dev(self):
        out = subprocess.run(["bash", SCRIPT, "--spec-only", self.bundle],
                             check=True, capture_output=True, text=True).stdout
        self.assertEqual(self.field(out, "Version"), "0.0.0_dev")

    # --- argument handling ----------------------------------------------------

    def test_missing_out_dir_is_usage(self):
        r = subprocess.run(["bash", SCRIPT, self.bundle, "v0.6.5"], capture_output=True, text=True)
        self.assertEqual(r.returncode, 2)
        self.assertIn("usage:", r.stderr)

    def test_missing_bundle_dir_fails(self):
        r = subprocess.run(["bash", SCRIPT, "--spec-only", os.path.join(self.tmp, "nope"), "v0.6.5"],
                           capture_output=True, text=True)
        self.assertNotEqual(r.returncode, 0)
        self.assertEqual(r.stdout, "")

    # --- payload contract -----------------------------------------------------

    def test_no_dependency_scan_and_no_rewrite_of_the_bundle(self):
        spec = self.spec("v0.6.5")
        self.assertEqual(self.field(spec, "AutoReqProv"), "no")
        self.assertIn("%global __os_install_post %{nil}", spec)
        self.assertIn("%global debug_package %{nil}", spec)
        self.assertIn("\n/opt/daslang\n", spec)

    def test_only_present_binaries_are_linked(self):
        self.stage_exe("daslang")
        spec = self.spec("v0.6.5")
        self.assertIn("ln -s /opt/daslang/bin/daslang %{buildroot}/usr/bin/daslang\n", spec)
        self.assertIn("\n/usr/bin/daslang\n", spec)
        self.assertNotIn("daslang-live", spec)

    def test_both_binaries_are_linked(self):
        self.stage_exe("daslang")
        self.stage_exe("daslang-live")
        spec = self.spec("v0.6.5")
        self.assertIn("\n/usr/bin/daslang\n", spec)
        self.assertIn("\n/usr/bin/daslang-live\n", spec)

    def test_non_executable_binary_is_not_linked(self):
        p = os.path.join(self.bundle, "bin", "daslang")
        with open(p, "wb") as f:
            f.write(b"\x7fELF")
        os.chmod(p, 0o644)
        self.assertNotIn("/usr/bin/daslang", self.spec("v0.6.5"))


class DasllamaRpmSpecTest(unittest.TestCase):
    """The dasllama profile: a flat bundle, its own prefix, declared requirements."""

    def setUp(self):
        self.tmp = tempfile.mkdtemp()
        self.bundle = os.path.join(self.tmp, "dasllama-server")
        os.makedirs(self.bundle)
        for name in ("dasllama-server.exe", "dasllama-cli.exe", "dasllama-bench.exe", "watchdog"):
            p = os.path.join(self.bundle, name)
            with open(p, "wb") as f:
                f.write(b"\x7fELF")
            os.chmod(p, 0o755)

    def tearDown(self):
        shutil.rmtree(self.tmp)

    def spec(self, tag="v0.6.5"):
        return subprocess.run(["bash", SCRIPT, "--spec-only", "--package", "dasllama", self.bundle, tag],
                              check=True, capture_output=True, text=True).stdout

    def test_own_tag_bares_to_its_version(self):
        spec = self.spec("dasllama-v0.7.0-RC1")
        self.assertIn("Version: 0.7.0~rc1\n", spec)

    def test_name_prefix_and_requirements(self):
        spec = self.spec()
        self.assertIn("Name: dasllama\n", spec)
        self.assertIn("\n/opt/dasllama\n", spec)
        self.assertIn("Requires: openssl-libs\n", spec)
        self.assertIn("Requires: curl\n", spec)

    def test_commands_drop_the_exe_suffix_and_rename_the_watchdog(self):
        spec = self.spec()
        self.assertIn("ln -s /opt/dasllama/dasllama-server.exe %{buildroot}/usr/bin/dasllama-server\n", spec)
        self.assertIn("ln -s /opt/dasllama/watchdog %{buildroot}/usr/bin/dasllama-watchdog\n", spec)
        self.assertNotIn("/usr/bin/watchdog", spec)

    def test_unknown_package_fails(self):
        r = subprocess.run(["bash", SCRIPT, "--spec-only", "--package", "nope", self.bundle, "v0.6.5"],
                           capture_output=True, text=True)
        self.assertNotEqual(r.returncode, 0)


if __name__ == "__main__":
    unittest.main()
