#!/usr/bin/env python3
"""The packages the release workflows build - one profile per package, read by every
builder in this folder (deb_build.sh, rpm_build.sh, wheel_build.py, render_manifests.py).
`manifests` maps each package-manager repo (under github.com/borisbat) to the files it
carries for the package and the template each renders from.

Usage: packages.py shell <package> <bundle_dir> <tag>   (bash assignments for the shell builders)

stdlib only: runs on every release runner.
"""
import os
import re
import shlex
import sys

# A command maps its user-facing name to the candidate paths of its executable, relative
# to the directory the builder is handed; the first one present wins. The linux packages
# link each command into /usr/bin, the wheel gets one console_scripts shim per command.
PROFILES = {
    "daslang": {
        "summary": "High-performance statically-typed scripting language for games and real-time applications",
        "description": ("High-performance statically-typed scripting language for games and\n"
                        "real-time applications: compiler, JIT, standard library, test framework,\n"
                        "tools, and modules. Installed under /opt/daslang."),
        "deb_summary": "daslang programming language SDK",
        "deb_section": "devel",
        "deb_depends": "",
        "rpm_requires": [],
        "prefix": "/opt/daslang",
        "links": ["daslang", "daslang-live"],
        "commands": {t: [f"bin/{t}", f"bin/{t}.exe"] for t in
                     ["daslang", "daslang-live", "lint", "daspkg", "dascov",
                      "detect-dupe", "benchctl", "dastest", "das-fmt"]},
        "main_command": "daslang",
        "license": "bundle",
        "probe_prefixes": ("bin/", "lib/"),
        "classifiers": ["Topic :: Software Development :: Compilers",
                        "Topic :: Software Development :: Interpreters"],
        "manifests": {
            "homebrew-daslang": {"Formula/daslang.rb": "homebrew-daslang.rb.template"},
            "scoop-daslang": {"bucket/daslang.json": "scoop-daslang.json.template"},
        },
    },
    "dasllama": {
        "summary": "OpenAI-compatible local LLM server, CLI and benchmark over dasLLAMA",
        "description": ("dasllama-server, an OpenAI-compatible HTTP server for local GGUF models,\n"
                        "with its supervisor (dasllama-watchdog), the dasllama-cli shell front\n"
                        "end and dasllama-bench. Installed under /opt/dasllama."),
        "deb_summary": "OpenAI-compatible local LLM server over dasLLAMA",
        "deb_section": "net",
        # Ubuntu 24.04 renamed the OpenSSL 3 runtime to libssl3t64
        "deb_depends": "libssl3 | libssl3t64, curl",
        "rpm_requires": ["openssl-libs", "curl"],
        "prefix": "/opt/dasllama",
        "links": ["dasllama-server", "dasllama-cli", "dasllama-bench", "dasllama-watchdog"],
        # daspkg keeps .exe on every flat bundle; the mac .app carries the bare names
        "commands": {
            "dasllama-server": ["dasllama-server.exe", "dasllama-server.app/Contents/MacOS/dasllama-server"],
            "dasllama-cli": ["dasllama-cli.exe", "dasllama-server.app/Contents/MacOS/dasllama-cli"],
            "dasllama-bench": ["dasllama-bench.exe", "dasllama-server.app/Contents/MacOS/dasllama-bench"],
            # the Debian `watchdog` package owns /usr/sbin/watchdog
            "dasllama-watchdog": ["watchdog", "watchdog.exe", "dasllama-server.app/Contents/MacOS/watchdog"],
        },
        "main_command": "dasllama-cli",
        "license": "repo",
        "probe_prefixes": ("",),
        "classifiers": ["Topic :: Scientific/Engineering :: Artificial Intelligence",
                        "Environment :: Console"],
        "manifests": {
            "homebrew-daslang": {"Formula/dasllama.rb": "homebrew-dasllama.rb.template",
                                 "Casks/dasllama.rb": "homebrew-dasllama-cask.rb.template"},
            "scoop-daslang": {"bucket/dasllama.json": "scoop-dasllama.json.template"},
        },
    },
}


# --- versions: one release tag, the spelling each package format wants ------------------------

def bare_tag(tag):
    """The tag's version: the leading v dropped, and before it the package prefix a product with
    its own tags carries (dasllama-v0.7.0-RC1 -> 0.7.0-RC1, v0.6.5 -> 0.6.5)."""
    tag = re.sub(r"^[A-Za-z][A-Za-z0-9]*-(?=[vV]\d)", "", tag)
    return tag[1:] if tag.startswith(("v", "V")) else tag


def deb_version(tag):
    """v0.6.5-RC1 -> 0.6.5~rc1: tilde sorts BEFORE the release, so the RC upgrades cleanly."""
    return re.sub(r"-[Rr][Cc](\d+)", r"~rc\1", bare_tag(tag))


def rpm_version(tag):
    """deb_version with no hyphen left - rpm rejects one inside Version."""
    return deb_version(tag).replace("-", "_")


def pep440(tag):
    """v0.6.4-RC1 -> 0.6.4rc1, v0.6.4 -> 0.6.4, 0.0.0-dev -> 0.0.0.dev0 (the release pattern is
    plain + RC + dev; anything else is an error, not a guess)."""
    m = re.fullmatch(r"(\d+\.\d+\.\d+)(?:-rc(\d+)|(-dev)\d*)?", bare_tag(tag), re.I)
    if not m:
        sys.exit(f"packages: cannot map tag {tag!r} to a PEP 440 version")
    base, rc, dev = m.groups()
    if rc:
        return base + "rc" + rc
    return base + ".dev0" if dev else base


def manifest_version(tag):
    """v0.6.4-RC4 -> 0.6.4-rc4: the tag without its v, lowercased (the tap's convention)."""
    return bare_tag(tag).lower()


# --- the executable of each command on each platform, from its candidates ---------------------

def mac_exe(p, command):
    """The command's executable inside the macOS .app, or None."""
    return next((c for c in p["commands"][command] if ".app/" in c), None)


def windows_exe(p, command):
    """The command's `.exe`, or None."""
    return next((c for c in p["commands"][command] if c.endswith(".exe")), None)


def unix_exe(p, command):
    """The command's executable in a linux bundle: its first candidate outside the .app."""
    return next((c for c in p["commands"][command] if ".app/" not in c), None)


def profile(name):
    if name not in PROFILES:
        sys.exit(f"packages: no package {name!r} (known: {sorted(PROFILES)})")
    return PROFILES[name]


def resolve(p, command, bundle):
    """The first candidate of `command` present and executable under `bundle`, or None."""
    for rel in p["commands"][command]:
        full = os.path.join(bundle, rel)
        if os.path.isfile(full) and os.access(full, os.X_OK):
            return rel
    return None


def shell(name, bundle, tag):
    p = profile(name)
    q = shlex.quote
    lines = [
        f"PKG_NAME={q(name)}",
        f"PKG_DEB_VERSION={q(deb_version(tag))}",
        f"PKG_RPM_VERSION={q(rpm_version(tag))}",
        f"PKG_PREFIX={q(p['prefix'])}",
        f"PKG_SUMMARY={q(p['deb_summary'])}",
        f"PKG_DESCRIPTION={q(p['description'])}",
        f"PKG_DEB_SECTION={q(p['deb_section'])}",
        f"PKG_DEB_DEPENDS={q(p['deb_depends'])}",
        "PKG_RPM_REQUIRES=(" + " ".join(q(r) for r in p["rpm_requires"]) + ")",
    ]
    # command=path for each linked command the bundle carries; the rest are not linked
    links = [(c, resolve(p, c, bundle)) for c in p["links"]]
    lines.append("PKG_LINKS=(" + " ".join(q(f"{c}={rel}") for c, rel in links if rel) + ")")
    return "\n".join(lines) + "\n"


def main():
    if len(sys.argv) != 5 or sys.argv[1] != "shell":
        sys.exit("usage: packages.py shell <package> <bundle_dir> <tag>")
    sys.stdout.write(shell(sys.argv[2], sys.argv[3], sys.argv[4]))


if __name__ == "__main__":
    main()
