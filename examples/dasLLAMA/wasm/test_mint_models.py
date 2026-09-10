#!/usr/bin/env python3
"""mint_models.py's refusals and its happy path, with no network and no daslang: the cache is
pre-seeded under the sha256 names the script trusts, and the converter is a stand-in that writes a
dasLLAMA image header carrying the IMAGE_VERSION the test asks for."""
import hashlib
import io
import json
import os
import stat
import struct
import sys
import tempfile
import unittest
from contextlib import redirect_stderr, redirect_stdout
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import mint_models  # noqa: E402

FAKE_CONVERTER = """#!/usr/bin/env python3
import os, struct, sys
out = sys.argv[sys.argv.index("-o") + 1]
with open(out, "wb") as f:
    f.write(struct.pack("<II", 0x314d4c44, int(os.environ["FAKE_IMAGE_VERSION"])) + b"body" * 64)
"""


def sha(data):
    return hashlib.sha256(data).hexdigest()


class Fixture:
    """an example directory with a models.json, a seeded cache, a converter stand-in and a page"""

    def __init__(self, root, image_version=35):
        self.root = Path(root)
        self.example = self.root / "example"
        self.cache = self.root / "cache"
        self.out = self.root / "out"
        self.example.mkdir()
        self.cache.mkdir()
        self.gguf = b"GGUF fake weights"
        self.pack = b"phoneme pack bytes"
        self.spec = {
            "images": [{"file": "story.gguf", "repo": "someone/models", "sha256": sha(self.gguf), "dlim": "story.dlim"}],
            "packs": [{"file": "pack.bin", "repo": "someone/models", "sha256": sha(self.pack)}],
        }
        (self.example / "models.json").write_text(json.dumps(self.spec))
        (self.cache / f"{sha(self.gguf)}-story.gguf").write_bytes(self.gguf)
        (self.cache / f"{sha(self.pack)}-pack.bin").write_bytes(self.pack)
        self.converter = self.root / "daslang"
        self.converter.write_text(FAKE_CONVERTER)
        self.converter.chmod(self.converter.stat().st_mode | stat.S_IXUSR)
        self.config = self.root / "wasm64.json"
        self.config.write_text("{}")
        self.page = self.root / "web_shell.html"
        self.page.write_text("<script>const PAGE_IMAGE_VERSION = /* @image-version */ 0;</script>")
        os.environ["FAKE_IMAGE_VERSION"] = str(image_version)

    def argv(self, *extra):
        return ["mint_models.py", "--example", str(self.example), "--config", str(self.config),
                "--daslang", str(self.converter), "--out", str(self.out), "--cache", str(self.cache), *extra]


def run_main(argv):
    """main() under argv; returns (SystemExit code or None, stdout, stderr)"""
    out, err = io.StringIO(), io.StringIO()
    saved = sys.argv
    sys.argv = argv
    try:
        with redirect_stdout(out), redirect_stderr(err):
            try:
                mint_models.main()
                code = None
            except SystemExit as e:
                code = e.code
    finally:
        sys.argv = saved
    return code, out.getvalue(), err.getvalue()


class FetchTest(unittest.TestCase):
    def test_a_cached_file_is_verified_before_it_is_trusted(self):
        with tempfile.TemporaryDirectory() as tmp:
            data = b"model bytes"
            entry = {"file": "m.gguf", "repo": "someone/models", "sha256": sha(data)}
            cached = Path(tmp) / f"{sha(data)}-m.gguf"
            cached.write_bytes(data)
            self.assertEqual(mint_models.fetch(entry, tmp), str(cached))
            cached.write_bytes(b"model bytes, changed under the same name")
            with self.assertRaises(SystemExit) as caught:
                mint_models.fetch(entry, tmp)
            self.assertIn("models.json says", str(caught.exception))
            self.assertIn(sha(data), str(caught.exception))
            self.assertFalse(cached.exists(), "a file whose hash moved is dropped from the cache, not served again")


class ImageVersionTest(unittest.TestCase):
    def test_the_version_is_read_from_the_header_and_a_non_image_is_refused(self):
        with tempfile.TemporaryDirectory() as tmp:
            image = Path(tmp) / "a.dlim"
            image.write_bytes(struct.pack("<II", mint_models.IMAGE_MAGIC, 41) + b"\0" * 56)
            self.assertEqual(mint_models.image_version_of(str(image)), 41)
            image.write_bytes(b"GGUF" + b"\0" * 60)
            with self.assertRaises(SystemExit) as caught:
                mint_models.image_version_of(str(image))
            self.assertIn("not a dasLLAMA image", str(caught.exception))


class MintTest(unittest.TestCase):
    def test_the_manifest_carries_the_minted_version_and_every_file_and_the_page_is_stamped(self):
        with tempfile.TemporaryDirectory() as tmp:
            fx = Fixture(tmp, image_version=35)
            code, out, err = run_main(fx.argv("--expect-image-version", "35", "--stamp-page", str(fx.page)))
            self.assertIsNone(code, err)
            manifest = json.loads((fx.out / "manifest.json").read_text())
            self.assertEqual(manifest["image_version"], 35)
            self.assertEqual([f["name"] for f in manifest["files"]], ["story.dlim", "pack.bin"])
            dlim = fx.out / "story.dlim"
            self.assertEqual(manifest["files"][0]["bytes"], dlim.stat().st_size)
            self.assertEqual(manifest["files"][0]["sha256"], sha(dlim.read_bytes()))
            self.assertEqual(manifest["files"][0]["source"], "someone/models/story.gguf")
            self.assertEqual(manifest["files"][1]["sha256"], sha(fx.pack))
            self.assertEqual((fx.out / "pack.bin").read_bytes(), fx.pack)
            self.assertIn("/* @image-version */ 35", fx.page.read_text())
            self.assertNotIn("/* @image-version */ 0", fx.page.read_text())
            self.assertIn("manifest.json: IMAGE_VERSION 35, 2 files", out)

    def test_a_version_the_tree_does_not_read_exits_3_before_the_manifest_is_written(self):
        with tempfile.TemporaryDirectory() as tmp:
            fx = Fixture(tmp, image_version=33)
            code, out, err = run_main(fx.argv("--expect-image-version", "35", "--stamp-page", str(fx.page)))
            self.assertEqual(code, 3)
            self.assertIn("IMAGE_VERSION 33, the tree says 35", err)
            self.assertFalse((fx.out / "manifest.json").exists(), "no manifest for a set the page would refuse")
            self.assertIn("/* @image-version */ 0", fx.page.read_text(), "the page keeps its unstamped slot")

    def test_without_an_expected_version_the_minted_one_is_taken_as_is(self):
        with tempfile.TemporaryDirectory() as tmp:
            fx = Fixture(tmp, image_version=33)
            code, out, err = run_main(fx.argv())
            self.assertIsNone(code, err)
            self.assertEqual(json.loads((fx.out / "manifest.json").read_text())["image_version"], 33)

    def test_a_page_without_the_slot_is_refused(self):
        with tempfile.TemporaryDirectory() as tmp:
            fx = Fixture(tmp)
            fx.page.write_text("<script>const PAGE_IMAGE_VERSION = 35;</script>")
            code, out, err = run_main(fx.argv("--stamp-page", str(fx.page)))
            self.assertIsInstance(code, str)
            self.assertIn("no `/* @image-version */ 0` slot", code)

    def test_a_pack_whose_hash_moved_stops_the_run(self):
        with tempfile.TemporaryDirectory() as tmp:
            fx = Fixture(tmp)
            (fx.cache / f"{sha(fx.pack)}-pack.bin").write_bytes(b"a different pack")
            code, out, err = run_main(fx.argv())
            self.assertIsInstance(code, str)
            self.assertIn("pack.bin: sha256", code)
            self.assertFalse((fx.out / "manifest.json").exists())

    def test_a_converter_that_fails_is_reported_with_its_exit(self):
        with tempfile.TemporaryDirectory() as tmp:
            fx = Fixture(tmp)
            fx.converter.write_text("#!/usr/bin/env python3\nimport sys\nprint('boom')\nsys.exit(7)\n")
            code, out, err = run_main(fx.argv())
            self.assertIsInstance(code, str)
            self.assertIn("mint of story.gguf failed (exit 7)", code)
            self.assertIn("boom", err)


if __name__ == "__main__":
    unittest.main()
