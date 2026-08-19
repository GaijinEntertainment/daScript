#!/usr/bin/env python3
"""Fixture tests for ci/packaging/wheel_build.py -- the pip wheel repack.

Each test stages a tiny fake bundle in a tempdir and checks one contract: the
tag->PEP 440 mapping, what the payload filter keeps and drops (the JIT import
libs stay, the 2 GB of static libs go), the platform tag derived from the
binaries, the exec bits pip will restore, and the entry-point surface.

Run: python3 ci/test_wheel_build.py
"""
import os
import shutil
import struct
import sys
import tempfile
import unittest
import zipfile

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "packaging"))
import wheel_build as wb


def elf_with(*versions):
    return b"\x7fELF" + b"\0" * 60 + b"".join(f"GLIBC_{v}\0".encode() for v in versions)


def macho_with(minos_major, minos_minor):
    # header (32 bytes, ncmds=1) + one LC_BUILD_VERSION (cmdsize 24)
    hdr = struct.pack("<IIIIIIII", 0xFEEDFACF, 0x0100000C, 0, 2, 1, 24, 0, 0)
    mo = (minos_major << 16) | (minos_minor << 8)
    cmd = struct.pack("<IIIIII", 0x32, 24, 1, mo, mo, 0)
    return hdr + cmd


class WheelBuildTest(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.mkdtemp()
        self.bundle = os.path.join(self.tmp, "daslang_bundle")
        self.out = os.path.join(self.tmp, "out")

    def tearDown(self):
        shutil.rmtree(self.tmp)

    def stage(self, files):
        for rel, data in files.items():
            p = os.path.join(self.bundle, rel)
            os.makedirs(os.path.dirname(p), exist_ok=True)
            with open(p, "wb") as f:
                f.write(data)

    def minimal(self, extra=None):
        files = {
            "LICENSE": b"BSD",
            "README.md": b"# daslang",
            "bin/daslang.exe": b"MZ",
            "bin/dastest.exe": b"MZ",
            "daslib/strings.das": b"// das",
        }
        files.update(extra or {})
        self.stage(files)

    def names(self, whl):
        with zipfile.ZipFile(whl) as z:
            return set(z.namelist())

    # --- version mapping ------------------------------------------------------

    def test_pep440_mapping(self):
        self.assertEqual(wb.pep440("v0.6.4-RC1"), "0.6.4rc1")
        self.assertEqual(wb.pep440("v0.6.4-rc2"), "0.6.4rc2")
        self.assertEqual(wb.pep440("v0.6.4"), "0.6.4")
        self.assertEqual(wb.pep440("0.6.4"), "0.6.4")
        self.assertEqual(wb.pep440("0.0.0-dev"), "0.0.0.dev0")

    def test_pep440_rejects_garbage(self):
        with self.assertRaises(SystemExit):
            wb.pep440("nightly-2026-08-18")

    # --- payload filter -------------------------------------------------------

    def test_filter_drops_embedding_payload_and_media_keeps_jit_import_libs(self):
        keep = ["bin/daslang", "lib/liblibDaScriptDyn.so", "lib/LLVM.dll",
                "lib/libDaScriptDyn_runtime.lib", "lib/libDaScriptDyn.lib",
                "daslib/x.das", "modules/dasHV/x.das", "tutorials/language/01.das",
                "skills/daslang/SKILL.md", "utils/mcp/main.das", "sgconfig.yml", "LICENSE"]
        drop = ["include/daScript/daScript.h", "lib/libDaScript.lib", "lib/liblibDaScript.a",
                "lib/libDasModuleVulkan.lib", "lib/cmake/daslang/config.cmake",
                "lib/pkgconfig/daslang.pc", "examples/gltf/media/BoomBox.glb",
                "tutorials/_assets/gltf/BoomBox.glb", "doc/index.html", "logs/x.log"]
        for rel in keep:
            self.assertTrue(wb.wants(rel), rel)
        for rel in drop:
            self.assertFalse(wb.wants(rel), rel)

    # --- platform tag ---------------------------------------------------------

    def test_linux_tag_is_the_highest_glibc_any_binary_needs(self):
        self.stage({"bin/daslang": elf_with("2.17", "2.34"),
                    "lib/liblibDaScriptDyn_runtime.so": elf_with("2.38", "2.2.5"),
                    "daslib/x.das": b"GLIBC_9.9 in a text file must not count"})
        files = list(wb.bundle_files(self.bundle))
        self.assertEqual(wb.detect_platform_tag(files, "Linux", "x86_64"), "manylinux_2_38_x86_64")
        self.assertEqual(wb.detect_platform_tag(files, "Linux", "aarch64"), "manylinux_2_38_aarch64")

    def test_macos_tag_is_the_highest_minos_major_zero(self):
        self.stage({"bin/daslang": macho_with(26, 0), "lib/LLVM.dll": macho_with(14, 0)})
        files = list(wb.bundle_files(self.bundle))
        self.assertEqual(wb.detect_platform_tag(files, "Darwin", "arm64"), "macosx_26_0_arm64")

    def test_windows_tag(self):
        self.assertEqual(wb.detect_platform_tag([], "Windows", "AMD64"), "win_amd64")

    def test_no_binaries_is_an_error_not_a_guess(self):
        self.stage({"daslib/x.das": b"//"})
        with self.assertRaises(SystemExit):
            wb.detect_platform_tag(list(wb.bundle_files(self.bundle)), "Linux", "x86_64")

    # --- the wheel itself -----------------------------------------------------

    def test_wheel_layout_entry_points_and_exec_bits(self):
        self.minimal({"lib/libDaScript.lib": b"x" * 10, "include/a.h": b"//",
                      "bin/daslang-live": b"\x7fELF"})
        whl = wb.build(self.bundle, "v0.6.4-RC1", self.out, platform_tag="win_amd64")
        self.assertTrue(whl.endswith("daslang-0.6.4rc1-py3-none-win_amd64.whl"))
        names = self.names(whl)
        for n in ("daslang/__init__.py", "daslang/_cli.py", "daslang/__main__.py",
                  "daslang/_sdk/bin/daslang.exe", "daslang/_sdk/daslib/strings.das",
                  "daslang-0.6.4rc1.dist-info/METADATA", "daslang-0.6.4rc1.dist-info/WHEEL",
                  "daslang-0.6.4rc1.dist-info/RECORD", "daslang-0.6.4rc1.dist-info/LICENSE",
                  "daslang-0.6.4rc1.dist-info/entry_points.txt"):
            self.assertIn(n, names)
        self.assertNotIn("daslang/_sdk/lib/libDaScript.lib", names)
        self.assertNotIn("daslang/_sdk/include/a.h", names)
        with zipfile.ZipFile(whl) as z:
            ep = z.read("daslang-0.6.4rc1.dist-info/entry_points.txt").decode()
            for tool in wb.TOOLS:
                self.assertIn(f"{tool} = daslang._cli:{tool.replace('-', '_')}", ep)
            wheel = z.read("daslang-0.6.4rc1.dist-info/WHEEL").decode()
            self.assertIn("Tag: py3-none-win_amd64", wheel)
            self.assertIn("Root-Is-Purelib: false", wheel)
            meta = z.read("daslang-0.6.4rc1.dist-info/METADATA").decode()
            self.assertIn("Version: 0.6.4rc1", meta)
            self.assertIn("# daslang", meta)
            for name in ("daslang/_sdk/bin/daslang.exe", "daslang/_sdk/bin/daslang-live"):
                mode = (z.getinfo(name).external_attr >> 16) & 0o777
                self.assertTrue(mode & 0o111, f"{name} not executable: {oct(mode)}")
            record = z.read("daslang-0.6.4rc1.dist-info/RECORD").decode().splitlines()
            listed = {line.split(",")[0] for line in record}
            self.assertEqual(listed, names)
            # the shims are importable python
            compile(z.read("daslang/_cli.py"), "_cli.py", "exec")
            compile(z.read("daslang/__init__.py"), "__init__.py", "exec")

    def test_missing_license_is_fatal(self):
        self.stage({"bin/daslang.exe": b"MZ"})
        with self.assertRaises(SystemExit):
            wb.build(self.bundle, "v0.6.4", self.out, platform_tag="win_amd64")


if __name__ == "__main__":
    unittest.main()
