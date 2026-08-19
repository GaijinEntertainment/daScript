#!/usr/bin/env python3
"""Build a pip wheel from an installed daslang bundle directory.

Usage: wheel_build.py <bundle_dir> <tag_or_version> <out_dir> [--platform-tag TAG]

- a repack, not a compile: the bundle tree lands under daslang/_sdk/, console_scripts
  shims exec the binaries there
- payload: the toolchain minus what EXCLUDE_* below names - keeps every wheel under
  PyPI's 100 MB per-file cap; the release zip carries the rest
- platform tag derived from the binaries (highest GLIBC symbol / Mach-O minos), never
  assumed
- stdlib only: runs on every release runner
"""
import argparse
import base64
import hashlib
import io
import os
import platform
import re
import struct
import sys
import zipfile

PACKAGE = "daslang"
SUMMARY = "High-performance statically-typed scripting language for games and real-time applications"

# console_scripts shim per shipped executable; the scoop bucket exposes the same set
TOOLS = ["daslang", "daslang-live", "gen1_to_gen2", "lint", "daspkg", "dascov",
         "detect-dupe", "benchctl", "dastest", "das-fmt"]

EXCLUDE_TOP = ("include", "examples", "doc", "logs")
# the tutorial sources ship (0.6 MB, and the skills link into them); the 30 MB of
# media under tutorials/_assets and the media-heavy examples tree stay in the zip
EXCLUDE_DIRS = ("tutorials/_assets",)
EXCLUDE_LIB_SUB = ("cmake", "pkgconfig")
EXCLUDE_LIB_EXT = (".a", ".lib")
# JIT on Windows links its DLLs against these import libs (module_jit.cpp
# get_real_lib_linker_paths); POSIX links the .so/.dylib directly, which stay anyway
KEEP_LIB_PREFIX = "libDaScriptDyn"

ZIP_EPOCH = (1980, 1, 1, 0, 0, 0)  # reproducible: no mtimes in the archive
MAX_WHEEL_MB = 100  # PyPI's per-file limit


def pep440(raw):
    """v0.6.4-RC1 -> 0.6.4rc1, v0.6.4 -> 0.6.4, 0.0.0-dev -> 0.0.0.dev0 (the release
    pattern is plain + RC + dev; anything else is a wheel_build error, not a guess)."""
    v = raw[1:] if raw.startswith(("v", "V")) else raw
    m = re.fullmatch(r"(\d+\.\d+\.\d+)(?:-rc(\d+)|(-dev)\d*)?", v, re.I)
    if not m:
        sys.exit(f"wheel_build: cannot map tag {raw!r} to a PEP 440 version")
    base, rc, dev = m.groups()
    if rc:
        return base + "rc" + rc
    return base + ".dev0" if dev else base


def is_shipped(rel):
    parts = rel.replace(os.sep, "/").split("/")
    if parts[0] in EXCLUDE_TOP:
        return False
    if any(rel.replace(os.sep, "/").startswith(d + "/") for d in EXCLUDE_DIRS):
        return False
    if parts[0] == "lib":
        if len(parts) > 1 and parts[1] in EXCLUDE_LIB_SUB:
            return False
        if parts[-1].endswith(EXCLUDE_LIB_EXT) and not parts[-1].startswith(KEEP_LIB_PREFIX):
            return False
    return True


def bundle_files(bundle):
    for d, _, fs in os.walk(bundle):
        for f in fs:
            full = os.path.join(d, f)
            rel = os.path.relpath(full, bundle)
            if is_shipped(rel):
                yield full, rel.replace(os.sep, "/")


# --- platform tag -------------------------------------------------------------

MH_MAGIC_64 = 0xFEEDFACF
LC_VERSION_MIN_MACOSX = 0x24
LC_BUILD_VERSION = 0x32


def read_bytes(path):
    with open(path, "rb") as f:
        return f.read()


def elf_glibc_max(path):
    data = read_bytes(path)
    if data[:4] != b"\x7fELF":
        return None
    best = None
    for a, b in re.findall(rb"GLIBC_(\d+)\.(\d+)", data):
        v = (int(a), int(b))
        best = v if best is None or v > best else best
    return best


def macho_minos(path):
    data = read_bytes(path)
    if len(data) < 32 or struct.unpack("<I", data[:4])[0] != MH_MAGIC_64:
        return None
    ncmds = struct.unpack("<I", data[16:20])[0]
    off, best = 32, None
    for _ in range(ncmds):
        cmd, size = struct.unpack("<II", data[off:off + 8])
        if cmd == LC_BUILD_VERSION:  # platform, minos, sdk
            mo = struct.unpack("<I", data[off + 12:off + 16])[0]
            best = (mo >> 16, (mo >> 8) & 0xFF)
        elif cmd == LC_VERSION_MIN_MACOSX:
            mo = struct.unpack("<I", data[off + 8:off + 12])[0]
            best = (mo >> 16, (mo >> 8) & 0xFF)
        off += size
    return best


def arch_word(machine, table):
    if machine not in table:
        sys.exit(f"wheel_build: no wheel arch for machine {machine!r} (known: {sorted(table)}); pass --platform-tag")
    return table[machine]


def detect_platform_tag(files, system=None, machine=None):
    system = system or platform.system()
    machine = (machine or platform.machine()).lower()
    if system == "Windows":
        return "win_" + arch_word(machine, {"amd64": "amd64", "x86_64": "amd64", "arm64": "arm64"})
    probe = macho_minos if system == "Darwin" else elf_glibc_max
    best = None
    for full, rel in files:
        if not (rel.startswith("bin/") or rel.startswith("lib/")):
            continue
        v = probe(full)
        if v and (best is None or v > best):
            best = v
    if best is None:
        sys.exit("wheel_build: no binaries found under bin/ or lib/ to derive a platform tag from")
    if system == "Darwin":
        major, minor = best
        minor = 0 if major >= 11 else minor  # macOS 11+ wheel tags pin the minor to 0
        arch = "arm64" if machine in ("arm64", "aarch64") else "x86_64"
        return f"macosx_{major}_{minor}_{arch}"
    arch = arch_word(machine, {"x86_64": "x86_64", "amd64": "x86_64", "aarch64": "aarch64", "arm64": "aarch64"})
    return f"manylinux_{best[0]}_{best[1]}_{arch}"


# --- package sources ----------------------------------------------------------

INIT_PY = '''"""daslang SDK — the toolchain is under `sdk_root()`; console scripts exec it."""
import os

__version__ = "@VERSION@"


def sdk_root():
    """Root of the bundled SDK (bin/, daslib/, modules/, dastest/, utils/, skills/)."""
    return os.path.join(os.path.dirname(os.path.abspath(__file__)), "_sdk")


def bin_dir():
    return os.path.join(sdk_root(), "bin")


def tool_path(name):
    """Absolute path of a shipped executable (`daslang`, `dastest`, ...); the
    `.exe` suffix is resolved here so callers never spell it."""
    for candidate in (name, name + ".exe"):
        p = os.path.join(bin_dir(), candidate)
        if os.path.isfile(p):
            return p
    raise FileNotFoundError(f"daslang SDK has no tool {name!r} in {bin_dir()}")
'''

CLI_PY = '''"""console_scripts entry points: one shim per shipped executable."""
import os
import subprocess
import sys

from . import tool_path


def run(name):
    exe = tool_path(name)
    args = [exe] + sys.argv[1:]
    if os.name == "nt":  # no execv semantics on Windows: spawn, forward the exit code
        try:
            raise SystemExit(subprocess.call(args))
        except KeyboardInterrupt:
            raise SystemExit(130)
    os.execv(exe, args)


@SHIMS@'''

MAIN_PY = '''"""`python -m daslang file.das` == `daslang file.das`."""
from ._cli import run

run("daslang")
'''


def shim_name(tool):
    return tool.replace("-", "_")


def render_cli():
    shims = "\n\n".join(f"def {shim_name(t)}():\n    run({t!r})\n" for t in TOOLS)
    return CLI_PY.replace("@SHIMS@", shims)


def render_metadata(version, readme):
    lines = [
        "Metadata-Version: 2.1",
        f"Name: {PACKAGE}",
        f"Version: {version}",
        f"Summary: {SUMMARY}",
        "Home-page: https://daslang.io",
        "Author: Gaijin Entertainment",
        "License: BSD-3-Clause",
        "License-File: LICENSE",
        "Project-URL: Homepage, https://daslang.io",
        "Project-URL: Source, https://github.com/GaijinEntertainment/daScript",
        "Project-URL: Documentation, https://daslang.io/docs",
        "Requires-Python: >=3.8",
        "Classifier: Programming Language :: Other",
        "Classifier: License :: OSI Approved :: BSD License",
        "Classifier: Topic :: Software Development :: Compilers",
        "Classifier: Topic :: Software Development :: Interpreters",
        "Classifier: Operating System :: Microsoft :: Windows",
        "Classifier: Operating System :: POSIX :: Linux",
        "Classifier: Operating System :: MacOS",
        "Description-Content-Type: text/markdown",
        "",
        readme,
    ]
    return "\n".join(lines)


def render_entry_points():
    body = "\n".join(f"{t} = {PACKAGE}._cli:{shim_name(t)}" for t in TOOLS)
    return f"[console_scripts]\n{body}\n"


# --- wheel writer -------------------------------------------------------------

def urlsafe_sha256(data):
    return "sha256=" + base64.urlsafe_b64encode(hashlib.sha256(data).digest()).rstrip(b"=").decode()


class WheelWriter:
    def __init__(self, path):
        self.zf = zipfile.ZipFile(path, "w", zipfile.ZIP_DEFLATED, compresslevel=9)
        self.record = []

    def add_bytes(self, arcname, data, mode=0o644):
        info = zipfile.ZipInfo(arcname, date_time=ZIP_EPOCH)
        info.compress_type = zipfile.ZIP_DEFLATED
        info.external_attr = ((0o100000 | mode) & 0xFFFF) << 16
        self.zf.writestr(info, data)
        self.record.append((arcname, urlsafe_sha256(data), len(data)))

    def add_file(self, arcname, path, executable=False):
        mode = os.stat(path).st_mode & 0o777 or 0o644
        if executable:
            mode |= 0o111
        self.add_bytes(arcname, read_bytes(path), mode)

    def finish(self, dist_info):
        buf = io.StringIO()
        for name, digest, size in self.record:
            buf.write(f"{name},{digest},{size}\n")
        rec = f"{dist_info}/RECORD"
        buf.write(f"{rec},,\n")
        info = zipfile.ZipInfo(rec, date_time=ZIP_EPOCH)
        info.compress_type = zipfile.ZIP_DEFLATED
        info.external_attr = (0o100644 & 0xFFFF) << 16
        self.zf.writestr(info, buf.getvalue())
        self.zf.close()


def build(bundle, tag, out_dir, platform_tag=None, system=None, machine=None):
    bundle = os.path.abspath(bundle)
    version = pep440(tag)
    files = sorted(bundle_files(bundle), key=lambda x: x[1])
    present = {rel for _, rel in files}
    missing = [t for t in TOOLS if not ({f"bin/{t}", f"bin/{t}.exe"} & present)]
    if missing:
        sys.exit(f"wheel_build: bundle has no binary for console_scripts {missing} - every entry point must resolve")
    plat = platform_tag or detect_platform_tag(files, system, machine)
    dist_info = f"{PACKAGE}-{version}.dist-info"
    wheel_name = f"{PACKAGE}-{version}-py3-none-{plat}.whl"
    os.makedirs(out_dir, exist_ok=True)
    out = os.path.join(out_dir, wheel_name)

    readme_path = os.path.join(bundle, "README.md")
    readme = read_bytes(readme_path).decode("utf-8") if os.path.isfile(readme_path) else SUMMARY
    license_path = os.path.join(bundle, "LICENSE")
    if not os.path.isfile(license_path):
        sys.exit(f"wheel_build: {license_path} missing — the bundle must carry its LICENSE")

    w = WheelWriter(out)
    w.add_bytes(f"{PACKAGE}/__init__.py", INIT_PY.replace("@VERSION@", version).encode())
    w.add_bytes(f"{PACKAGE}/_cli.py", render_cli().encode())
    w.add_bytes(f"{PACKAGE}/__main__.py", MAIN_PY.encode())
    for full, rel in files:
        # pip restores mode bits from the archive - a bundle staged on a modeless filesystem must not lose them
        w.add_file(f"{PACKAGE}/_sdk/{rel}", full, executable=rel.startswith("bin/"))
    w.add_file(f"{dist_info}/LICENSE", license_path)
    w.add_bytes(f"{dist_info}/METADATA", render_metadata(version, readme).encode())
    w.add_bytes(f"{dist_info}/WHEEL",
                f"Wheel-Version: 1.0\nGenerator: daslang-wheel-build\nRoot-Is-Purelib: false\nTag: py3-none-{plat}\n".encode())
    w.add_bytes(f"{dist_info}/entry_points.txt", render_entry_points().encode())
    w.add_bytes(f"{dist_info}/top_level.txt", f"{PACKAGE}\n".encode())
    w.finish(dist_info)
    size_mb = os.path.getsize(out) / 2**20
    print(f"built: {out} ({len(files)} SDK files, {size_mb:.1f} MB, tag py3-none-{plat})")
    if size_mb > MAX_WHEEL_MB:
        sys.exit(f"wheel_build: {out} is {size_mb:.1f} MB, over PyPI's {MAX_WHEEL_MB} MB per-file limit - trim EXCLUDE_* or request a size raise")
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("bundle_dir")
    ap.add_argument("tag", help="release tag (v0.6.4-RC1) or version; 0.0.0-dev for dispatch runs")
    ap.add_argument("out_dir")
    ap.add_argument("--platform-tag", help="override the derived wheel platform tag")
    ap.add_argument("--system", choices=["Linux", "Darwin", "Windows"],
                    help="derive the tag as if on this OS (cross-checking a foreign bundle)")
    ap.add_argument("--machine", help="machine word for --system (x86_64, aarch64, arm64)")
    a = ap.parse_args()
    build(a.bundle_dir, a.tag, a.out_dir, a.platform_tag, a.system, a.machine)


if __name__ == "__main__":
    main()
