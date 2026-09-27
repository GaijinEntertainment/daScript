#!/usr/bin/env python3
"""Render a package's Homebrew and scoop manifests for one release tag.

Usage: render_manifests.py <package> <tag> <sha_dir> <out_dir>

- <sha_dir> holds the release's `<asset>.sha256` files (`gh release download <tag> -p '*.sha256'`)
- writes <out_dir>/<repo>/<path> for every manifest the package's profile names, ready to
  copy over a checkout of that repo
- a template names each asset it needs as @SHA:<asset>@; an asset with no .sha256 is fatal,
  so a manifest never points at a file the release does not carry
- a .rb template's leading comment block is the template's own header and is dropped

stdlib only: runs on every release runner.
"""
import os
import re
import sys

from packages import mac_exe, manifest_version, profile, unix_exe, windows_exe

HERE = os.path.dirname(os.path.abspath(__file__))
SHA_REF = re.compile(r"@SHA:([^@]+)@")


def read_sha(sha_dir, asset):
    path = os.path.join(sha_dir, asset + ".sha256")
    if not os.path.isfile(path):
        sys.exit(f"render_manifests: {asset}.sha256 not in {sha_dir} - the release does not carry {asset}")
    with open(path) as f:
        words = f.read().split()
    if not words or not re.fullmatch(r"[0-9a-f]{64}", words[0]):
        sys.exit(f"render_manifests: {path} does not start with a sha256")
    return words[0]


def stem(path):
    return os.path.splitext(os.path.basename(path))[0]


def command_tables(p):
    """The per-manager command lists, rendered from the profile: @SCOOP_BIN@ (every command, a
    renamed one as [exe, alias]), @BREW_EXES_MAC@ / @BREW_EXES_LINUX@ (the linked commands as a
    Ruby hash body, command => executable) and @CASK_BINARIES@ (one `binary` line per link, for a
    package that ships a macOS .app)."""
    scoop = []
    for c in p["commands"]:
        rel = windows_exe(p, c)
        exe = rel.replace("/", "\\\\")
        scoop.append(f'"{exe}"' if stem(rel) == c else f'["{exe}", "{c}"]')
    tables = {
        "@SCOOP_BIN@": ",\n".join("        " + e for e in scoop),
        "@BREW_EXES_LINUX@": ", ".join(f'"{c}" => "{unix_exe(p, c)}"' for c in p["links"]),
    }
    if all(mac_exe(p, c) for c in p["links"]):
        macs = {c: mac_exe(p, c) for c in p["links"]}
        tables["@BREW_EXES_MAC@"] = ", ".join(f'"{c}" => "{os.path.basename(e)}"' for c, e in macs.items())
        cask = []
        for c, e in macs.items():
            line = f'  binary "#{{appdir}}/{e}"'
            cask.append(line if os.path.basename(e) == c else f'{line}, target: "{c}"')
        tables["@CASK_BINARIES@"] = "\n".join(cask)
    return tables


def render(text, tag, sha_dir, rb=False, p=None):
    if p is not None:
        for k, v in command_tables(p).items():
            text = text.replace(k, v)
    if rb:
        lines = text.splitlines(keepends=True)
        while lines and lines[0].startswith("#"):
            lines.pop(0)
        text = "".join(lines)
    text = text.replace("@TAG@", tag).replace("@VERSION@", manifest_version(tag))
    text = SHA_REF.sub(lambda m: read_sha(sha_dir, m.group(1)), text)
    left = re.findall(r"@[A-Z_:]+@", text)
    if left:
        sys.exit(f"render_manifests: unfilled placeholders {sorted(set(left))}")
    return text


def render_package(package, tag, sha_dir, out_dir):
    written = []
    for repo, files in profile(package)["manifests"].items():
        for rel, template in files.items():
            with open(os.path.join(HERE, template)) as f:
                text = render(f.read(), tag, sha_dir, rb=template.endswith(".rb.template"), p=profile(package))
            out = os.path.join(out_dir, repo, rel)
            os.makedirs(os.path.dirname(out), exist_ok=True)
            with open(out, "w") as f:
                f.write(text)
            written.append(out)
    return written


def main():
    if len(sys.argv) != 5:
        sys.exit("usage: render_manifests.py <package> <tag> <sha_dir> <out_dir>")
    for path in render_package(*sys.argv[1:]):
        print(f"rendered: {path}")


if __name__ == "__main__":
    main()
