#!/usr/bin/env python3
"""Fixture tests for ci/packaging/wheel_build.py -- the pip wheel repack.

Each test stages a tiny fake bundle in a tempdir and checks one contract.

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


def macho_with(minos_major, minos_minor, legacy=False):
    """One load command: LC_BUILD_VERSION, or LC_VERSION_MIN_MACOSX when legacy."""
    hdr = struct.pack("<IIIIIIII", wb.MH_MAGIC_64, 0x0100000C, 0, 2, 1, 24 if not legacy else 16, 0, 0)
    mo = (minos_major << 16) | (minos_minor << 8)
    if legacy:
        return hdr + struct.pack("<IIII", wb.LC_VERSION_MIN_MACOSX, 16, mo, mo)
    return hdr + struct.pack("<IIIIII", wb.LC_BUILD_VERSION, 24, 1, mo, mo, 0)


class BundleFixture(unittest.TestCase):
    """A tempdir with the bundle under BUNDLE_DIR and an out dir; stage() writes files into it."""
    BUNDLE_DIR = "daslang_bundle"

    def setUp(self):
        self.tmp = tempfile.mkdtemp()
        self.bundle = os.path.join(self.tmp, self.BUNDLE_DIR)
        self.out = os.path.join(self.tmp, "out")

    def tearDown(self):
        shutil.rmtree(self.tmp)

    def stage(self, files):
        for rel, data in files.items():
            p = os.path.join(self.bundle, rel)
            os.makedirs(os.path.dirname(p), exist_ok=True)
            with open(p, "wb") as f:
                f.write(data)


class WheelBuildTest(BundleFixture):

    def windows_exes(self):
        """Every command's Windows executable - the last candidate of each."""
        return {wb.profile("daslang")["commands"][t][-1]: b"MZ" for t in wb.TOOLS}

    def stage_minimal(self, extra=None):
        files = {"LICENSE": b"BSD", "README.md": b"# daslang", "daslib/strings.das": b"// das"}
        files.update(self.windows_exes())
        files.update(extra or {})
        self.stage(files)

    def zip_names(self, whl):
        with zipfile.ZipFile(whl) as z:
            return set(z.namelist())

    # --- version mapping ------------------------------------------------------

    def test_pep440_mapping(self):
        self.assertEqual(wb.pep440("v0.6.4-RC1"), "0.6.4rc1")
        self.assertEqual(wb.pep440("v0.6.4-rc2"), "0.6.4rc2")
        self.assertEqual(wb.pep440("v0.6.4"), "0.6.4")
        self.assertEqual(wb.pep440("0.6.4"), "0.6.4")
        self.assertEqual(wb.pep440("0.0.0-dev"), "0.0.0.dev0")
        self.assertEqual(wb.pep440("dasllama-v0.7.0-RC1"), "0.7.0rc1")
        self.assertEqual(wb.pep440("dasllama-v0.7.0"), "0.7.0")

    def test_pep440_rejects_garbage_and_unhandled_prerelease_shapes(self):
        for tag in ("nightly-2026-08-18", "v0.6.4-beta1", "v0.6.4-RC", "v0.6"):
            with self.assertRaises(SystemExit, msg=tag):
                wb.pep440(tag)

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
            self.assertTrue(wb.is_shipped(rel), rel)
        for rel in drop:
            self.assertFalse(wb.is_shipped(rel), rel)

    # --- platform tag ---------------------------------------------------------

    def test_linux_tag_is_the_highest_glibc_any_binary_needs(self):
        self.stage({"bin/daslang": elf_with("2.17", "2.34"),
                    "lib/liblibDaScriptDyn_runtime.so": elf_with("2.38", "2.2.5"),
                    "bin/README.txt": b"GLIBC_9.9 in a non-ELF file under bin/ must not count",
                    "daslib/tool.so": elf_with("9.8") + b" an ELF outside bin/ and lib/ must not count"})
        files = list(wb.bundle_files(self.bundle))
        self.assertEqual(wb.detect_platform_tag(files, "Linux", "x86_64"), "manylinux_2_38_x86_64")
        self.assertEqual(wb.detect_platform_tag(files, "Linux", "aarch64"), "manylinux_2_38_aarch64")

    def test_macos_tag_is_the_highest_minos_major_zero(self):
        self.stage({"bin/daslang": macho_with(26, 0), "lib/LLVM.dll": macho_with(14, 0)})
        files = list(wb.bundle_files(self.bundle))
        self.assertEqual(wb.detect_platform_tag(files, "Darwin", "arm64"), "macosx_26_0_arm64")

    def test_macos_tag_pins_minor_to_zero_from_11_and_reads_the_legacy_command(self):
        self.stage({"bin/daslang": macho_with(14, 5)})
        self.assertEqual(wb.detect_platform_tag(list(wb.bundle_files(self.bundle)), "Darwin", "arm64"), "macosx_14_0_arm64")
        self.stage({"bin/daslang": macho_with(10, 15, legacy=True)})
        self.assertEqual(wb.detect_platform_tag(list(wb.bundle_files(self.bundle)), "Darwin", "x86_64"), "macosx_10_15_x86_64")

    def test_windows_tag(self):
        self.assertEqual(wb.detect_platform_tag([], "Windows", "AMD64"), "win_amd64")
        self.assertEqual(wb.detect_platform_tag([], "Windows", "ARM64"), "win_arm64")

    def test_unknown_machine_is_an_error_not_a_traceback(self):
        with self.assertRaises(SystemExit):
            wb.detect_platform_tag([], "Windows", "mips")
        self.stage({"bin/daslang": elf_with("2.34")})
        with self.assertRaises(SystemExit):
            wb.detect_platform_tag(list(wb.bundle_files(self.bundle)), "Linux", "riscv64")

    def test_symlinks_stay_inside_the_bundle(self):
        self.stage({"lib/libglfw.3.4.dylib": b"real", "daslib/x.das": b"//"})
        outside = os.path.join(self.tmp, "outside.txt")
        with open(outside, "wb") as f:
            f.write(b"secret")
        try:
            os.symlink(os.path.join(self.bundle, "lib", "libglfw.3.4.dylib"), os.path.join(self.bundle, "lib", "libglfw.dylib"))
        except (OSError, NotImplementedError):
            self.skipTest("symlinks unavailable on this host")
        files = dict(wb.bundle_files(self.bundle))
        self.assertIn(os.path.join(self.bundle, "lib", "libglfw.dylib"), files)  # in-bundle alias ships
        os.symlink(outside, os.path.join(self.bundle, "lib", "escape.so"))
        with self.assertRaises(SystemExit):
            list(wb.bundle_files(self.bundle))

    def test_no_binaries_is_an_error_not_a_guess(self):
        self.stage({"daslib/x.das": b"//"})
        with self.assertRaises(SystemExit):
            wb.detect_platform_tag(list(wb.bundle_files(self.bundle)), "Linux", "x86_64")

    # --- the wheel itself -----------------------------------------------------

    def test_wheel_layout_entry_points_and_exec_bits(self):
        self.stage_minimal({"lib/libDaScript.lib": b"x" * 10, "include/a.h": b"//",
                      "bin/daslang-live": b"\x7fELF"})
        whl = wb.build(self.bundle, "v0.6.4-RC1", self.out, platform_tag="win_amd64")
        self.assertTrue(whl.endswith("daslang-0.6.4rc1-py3-none-win_amd64.whl"))
        names = self.zip_names(whl)
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
            for line in record:
                name, digest, size = line.split(",")
                if name.endswith("/RECORD"):
                    continue
                data = z.read(name)
                self.assertEqual(int(size), len(data), name)
                self.assertEqual(digest, wb.urlsafe_sha256(data), name)
            compile(z.read("daslang/_cli.py"), "_cli.py", "exec")
            compile(z.read("daslang/__init__.py"), "__init__.py", "exec")
            init = z.read("daslang/__init__.py").decode()
            self.assertIn("'daslang-watchdog': ['bin/watchdog', 'bin/watchdog.exe']", init,
                          "the watchdog keeps its bundle name behind the package's command")
            self.assertIn("'lint': ['bin/lint.exe']", init, "a tool's exe keeps .exe on every platform")

    def test_missing_license_is_fatal(self):
        self.stage(self.windows_exes())
        with self.assertRaises(SystemExit):
            wb.build(self.bundle, "v0.6.4", self.out, platform_tag="win_amd64")

    def test_readme_absent_falls_back_to_the_summary(self):
        self.stage_minimal()
        os.remove(os.path.join(self.bundle, "README.md"))
        whl = wb.build(self.bundle, "v0.6.4", self.out, platform_tag="win_amd64")
        with zipfile.ZipFile(whl) as z:
            self.assertIn(wb.profile("daslang")["summary"], z.read("daslang-0.6.4.dist-info/METADATA").decode())

    def test_over_the_size_cap_is_fatal(self):
        self.stage_minimal()
        saved = wb.MAX_WHEEL_MB
        wb.MAX_WHEEL_MB = 0
        try:
            with self.assertRaises(SystemExit):
                wb.build(self.bundle, "v0.6.4", self.out, platform_tag="win_amd64")
        finally:
            wb.MAX_WHEEL_MB = saved

    def test_missing_tool_binary_is_fatal(self):
        self.stage_minimal()
        os.remove(os.path.join(self.bundle, "bin", "dastest.exe"))
        with self.assertRaises(SystemExit):
            wb.build(self.bundle, "v0.6.4", self.out, platform_tag="win_amd64")


class DasllamaWheelTest(BundleFixture):
    """The dasllama profile: a flat bundle (or the mac .app), the repo LICENSE, no trimming."""
    BUNDLE_DIR = "dist"

    def build(self, **kw):
        return wb.build(self.bundle, "v0.6.5-RC1", self.out, package="dasllama", **kw)

    def test_flat_bundle_shims_every_command(self):
        self.stage({"dasllama-server.exe": elf_with("2.35"), "dasllama-cli.exe": elf_with("2.35"),
                    "dasllama-bench.exe": elf_with("2.34"), "watchdog": elf_with("2.35"),
                    "control.html": b"<html>", "include/x.h": b"kept"})
        whl = self.build(system="Linux", machine="x86_64")
        self.assertTrue(whl.endswith("dasllama-0.6.5rc1-py3-none-manylinux_2_35_x86_64.whl"))
        with zipfile.ZipFile(whl) as z:
            names = set(z.namelist())
            eps = z.read("dasllama-0.6.5rc1.dist-info/entry_points.txt").decode()
            mode = z.getinfo("dasllama/_sdk/watchdog").external_attr >> 16
            init = z.read("dasllama/__init__.py").decode()
            main = z.read("dasllama/__main__.py").decode()
            meta = z.read("dasllama-0.6.5rc1.dist-info/METADATA").decode()
        self.assertIn("dasllama/_sdk/include/x.h", names)
        self.assertIn("dasllama-0.6.5rc1.dist-info/LICENSE", names)
        self.assertIn("dasllama-watchdog = dasllama._cli:dasllama_watchdog", eps)
        self.assertIn("dasllama-server = dasllama._cli:dasllama_server", eps)
        self.assertTrue(mode & 0o111, "an extensionless executable keeps its exec bit")
        self.assertIn("'dasllama-watchdog': ['watchdog'", init)
        self.assertIn('run("dasllama-cli")', main, "python -m dasllama runs the profile's main command")
        self.assertIn(wb.profile("dasllama")["description"], meta, "no README: the summary and the description")

    def test_mac_app_resolves_inside_the_bundle(self):
        macos = "dasllama-server.app/Contents/MacOS/"
        self.stage({macos + n: macho_with(14, 0) for n in
                    ("dasllama-server", "dasllama-cli", "dasllama-bench", "watchdog")})
        whl = self.build(system="Darwin", machine="arm64")
        self.assertTrue(whl.endswith("py3-none-macosx_14_0_arm64.whl"))

    def test_missing_command_is_fatal(self):
        self.stage({"dasllama-server.exe": b"MZ", "dasllama-cli.exe": b"MZ", "watchdog.exe": b"MZ"})
        with self.assertRaises(SystemExit):
            self.build(platform_tag="win_amd64")


if __name__ == "__main__":
    unittest.main()
