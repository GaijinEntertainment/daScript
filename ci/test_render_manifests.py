#!/usr/bin/env python3
"""Fixture tests for ci/packaging/render_manifests.py -- the Homebrew and scoop manifests.

Each test writes fake `<asset>.sha256` files into a tempdir and renders the real
templates, so no release or network is needed.

Run: python3 ci/test_render_manifests.py
"""
import hashlib
import json
import os
import shutil
import subprocess
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "packaging"))
import render_manifests as rm
from packages import PROFILES

ASSETS = {
    "daslang": ["daslang-bundle-darwin26-arm64.zip", "daslang-bundle-linux-x86_64.zip",
                "daslang-bundle-linux-arm64.zip", "daslang-bundle-windows-x86_64.zip"],
    "dasllama": ["dasllama-darwin-arm64.zip", "dasllama-linux-x86_64.tar.gz",
                 "dasllama-linux-arm64.tar.gz", "dasllama-windows-x64.zip"],
}


def fake_sha(asset):
    return hashlib.sha256(asset.encode()).hexdigest()


def git_bash():
    """The bash the release workflows' `shell: bash` runs - Git's on Windows, where a bare
    `bash` from Python resolves to System32's WSL launcher first."""
    if os.name != "nt":
        return shutil.which("bash")
    git = shutil.which("git")
    root = os.path.dirname(git) if git else ""
    while root and os.path.dirname(root) != root:
        candidate = os.path.join(root, "bin", "bash.exe")
        if os.path.isfile(candidate):
            return candidate
        root = os.path.dirname(root)
    return None


class RenderManifestsTest(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.mkdtemp()
        self.sha = os.path.join(self.tmp, "sha")
        self.out = os.path.join(self.tmp, "out")
        os.makedirs(self.sha)

    def tearDown(self):
        shutil.rmtree(self.tmp)

    def stage(self, assets):
        for a in assets:
            with open(os.path.join(self.sha, a + ".sha256"), "w") as f:
                f.write(f"{fake_sha(a)}  {a}\n")

    def read(self, rel):
        with open(os.path.join(self.out, rel)) as f:
            return f.read()

    def test_version_is_the_tag_without_v_lowercased(self):
        self.assertEqual(rm.manifest_version("v0.6.4-RC4"), "0.6.4-rc4")
        self.assertEqual(rm.manifest_version("v0.6.5"), "0.6.5")
        self.assertEqual(rm.manifest_version("0.6.5-RC1"), "0.6.5-rc1")
        self.assertEqual(rm.manifest_version("V0.6.5"), "0.6.5")
        self.assertEqual(rm.manifest_version("dasllama-v0.7.0-RC1"), "0.7.0-rc1")
        self.assertEqual(rm.manifest_version("dasllama-v0.7.0"), "0.7.0")

    def test_dasllama_own_tag_keeps_its_prefix_in_every_url(self):
        self.stage(ASSETS["dasllama"])
        rm.render_package("dasllama", "dasllama-v0.7.0-RC1", self.sha, self.out)
        formula = self.read("homebrew-daslang/Formula/dasllama.rb")
        cask = self.read("homebrew-daslang/Casks/dasllama.rb")
        bucket = json.loads(self.read("scoop-daslang/bucket/dasllama.json"))
        self.assertIn('version "0.7.0-rc1"', formula)
        self.assertIn("/download/dasllama-v0.7.0-RC1/dasllama-linux-x86_64.tar.gz", formula)
        self.assertIn("/download/dasllama-v0.7.0-RC1/dasllama-darwin-arm64.zip", cask)
        self.assertEqual(bucket["version"], "0.7.0-rc1")
        self.assertIn("/download/dasllama-v0.7.0-RC1/dasllama-windows-x64.zip", bucket["url"])
        self.assertIn("/download/dasllama-v$version/", bucket["autoupdate"]["url"])
        self.assertIn("dasllama-v", bucket["checkver"]["regex"], "the bucket's version check reads dasllama's tags, not daslang's")

    def test_daslang_bucket_version_check_ignores_dasllama_tags(self):
        self.stage(ASSETS["daslang"])
        rm.render_package("daslang", "v0.6.5", self.sha, self.out)
        bucket = json.loads(self.read("scoop-daslang/bucket/daslang.json"))
        import re
        rx = re.compile(bucket["checkver"]["regex"])
        self.assertEqual(rx.search('href="/GaijinEntertainment/daScript/releases/tag/v0.6.5"').group(1), "0.6.5")
        self.assertIsNone(rx.search('href="/GaijinEntertainment/daScript/releases/tag/dasllama-v0.7.0"'))

    def test_every_package_renders_every_manifest(self):
        for package, assets in ASSETS.items():
            self.stage(assets)
            written = rm.render_package(package, "v0.6.5-RC1", self.sha, self.out)
            want = sum(len(files) for files in PROFILES[package]["manifests"].values())
            self.assertEqual(len(written), want, package)

    def test_dasllama_formula_cask_and_bucket(self):
        self.stage(ASSETS["dasllama"])
        rm.render_package("dasllama", "v0.6.5-RC1", self.sha, self.out)
        formula = self.read("homebrew-daslang/Formula/dasllama.rb")
        cask = self.read("homebrew-daslang/Casks/dasllama.rb")
        bucket = json.loads(self.read("scoop-daslang/bucket/dasllama.json"))
        self.assertTrue(formula.startswith("class Dasllama < Formula"), "the template header is dropped")
        self.assertIn('version "0.6.5-rc1"', formula)
        self.assertIn("/download/v0.6.5-RC1/dasllama-linux-arm64.tar.gz", formula)
        self.assertIn(fake_sha("dasllama-linux-arm64.tar.gz"), formula)
        self.assertTrue(cask.startswith('cask "dasllama" do'))
        self.assertIn(f'sha256 "{fake_sha("dasllama-darwin-arm64.zip")}"', cask)
        self.assertEqual(bucket["hash"], fake_sha("dasllama-windows-x64.zip"))
        self.assertEqual(bucket["version"], "0.6.5-rc1")
        self.assertNotIn("@", json.dumps(bucket["url"]))
        self.assertIn(["watchdog.exe", "dasllama-watchdog"], bucket["bin"], "a renamed command is [exe, alias]")
        self.assertIn("dasllama-cli.exe", bucket["bin"], "a same-named command is its exe alone")
        self.assertIn('"dasllama-watchdog" => "watchdog"', formula, "the mac and linux link tables")
        self.assertIn('"dasllama-cli" => "dasllama-cli.exe"', formula)
        self.assertIn('Contents/MacOS/watchdog", target: "dasllama-watchdog"', cask)
        self.assertIn('Contents/MacOS/dasllama-cli"\n', cask, "a same-named binary needs no target")

    def test_daslang_bucket_lists_every_tool_by_path(self):
        self.stage(ASSETS["daslang"])
        rm.render_package("daslang", "v0.6.5", self.sha, self.out)
        bucket = json.loads(self.read("scoop-daslang/bucket/daslang.json"))
        self.assertEqual(bucket["bin"][0], "bin\\daslang.exe")
        self.assertEqual(len(bucket["bin"]), 10)
        self.assertIn(["bin\\watchdog.exe", "daslang-watchdog"], bucket["bin"], "the watchdog under the package's name")
        formula = self.read("homebrew-daslang/Formula/daslang.rb")
        self.assertIn('"lint" => "bin/lint.exe"', formula, "a tool links to its exe in the bundle")
        self.assertIn('"daslang-watchdog" => "bin/watchdog"', formula)

    def test_missing_asset_is_fatal(self):
        self.stage(ASSETS["dasllama"][:-1])
        with self.assertRaises(SystemExit) as cm:
            rm.render_package("dasllama", "v0.6.5", self.sha, self.out)
        self.assertIn("dasllama-windows-x64.zip", str(cm.exception))

    def test_malformed_sha_is_fatal(self):
        self.stage(ASSETS["daslang"])
        with open(os.path.join(self.sha, ASSETS["daslang"][0] + ".sha256"), "w") as f:
            f.write("not-a-hash  x\n")
        with self.assertRaises(SystemExit):
            rm.render_package("daslang", "v0.6.5", self.sha, self.out)

    @unittest.skipUnless(git_bash(), "no bash to run checksum_assets.sh")
    def test_checksum_script_writes_what_the_render_reads(self):
        here = os.path.join(os.path.dirname(os.path.abspath(__file__)), "packaging")
        for a in ASSETS["dasllama"]:
            with open(os.path.join(self.sha, a), "w") as f:
                f.write(a)
        with open(os.path.join(self.sha, "stale.sha256"), "w") as f:
            f.write("left alone\n")
        subprocess.run([git_bash(), os.path.join(here, "checksum_assets.sh"), self.sha], check=True, capture_output=True)
        self.assertFalse(os.path.exists(os.path.join(self.sha, "stale.sha256.sha256")), "a .sha256 is never summed")
        rm.render_package("dasllama", "v0.6.5", self.sha, self.out)
        bucket = json.loads(self.read("scoop-daslang/bucket/dasllama.json"))
        self.assertEqual(bucket["hash"], hashlib.sha256(b"dasllama-windows-x64.zip").hexdigest())

    def test_bad_arguments_are_usage(self):
        here = os.path.join(os.path.dirname(os.path.abspath(__file__)), "packaging")
        for script in ("render_manifests.py", "packages.py"):
            r = subprocess.run([sys.executable, os.path.join(here, script), "only-one"], capture_output=True, text=True)
            self.assertNotEqual(r.returncode, 0, script)
            self.assertIn("usage:", r.stderr, script)

    def test_unknown_placeholder_is_fatal(self):
        with self.assertRaises(SystemExit):
            rm.render("url @TAG@ @OOPS@", "v1.0.0", self.sha)


if __name__ == "__main__":
    unittest.main()
