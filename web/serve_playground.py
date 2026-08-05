#!/usr/bin/env python3
"""Local playground rig: serve the repo's playground sources over the same
COOP/COEP headers production uses, so pthreads + SharedArrayBuffer behave.

The wasm runtime itself (daslang_static.{js,wasm}) is NOT in the repo — it is a
build artifact. Fetch it once from the live site into the serve dir:

    python serve_playground.py --fetch-runtime

Then just:

    python serve_playground.py            # http://127.0.0.1:8791/playground/
"""
import argparse
import functools
import http.server
import os
import shutil
import socketserver
import sys
import urllib.request

REPO = os.environ.get("DASWEB_REPO", r"D:\Work\daScript-dasweb")
SERVE = os.path.join(os.path.dirname(os.path.abspath(__file__)), "_serve")
SITE = "https://daslang.io/"

# repo path -> served path
SOURCES = [
    ("site/playground/index.html",            "playground/index.html"),
    ("site/playground/run-frame.html",        "playground/run-frame.html"),
    ("site/playground/playground-runner.js",  "playground/playground-runner.js"),
    ("site/playground/playground-init.js",    "playground/playground-init.js"),
    ("site/playground/playground-tabs.js",    "playground/playground-tabs.js"),
    ("site/playground/playground-share.js",   "playground/playground-share.js"),
    ("site/playground/playground-splitter.js","playground/playground-splitter.js"),
    ("web/examples/ui/src/main.js",           "playground/main.js"),
]

# fetched from the live site: build artifacts + vendored libs, keyed by the path
# the page requests them at (relative to the served root).
RUNTIME = [
    "playground/daslang_static.js",
    "playground/daslang_static.wasm",
    "playground/jquery-3.6.0.min.js",
    "playground/samples/data.json",
    "files/cm/codemirror.min.js",
    "files/cm/codemirror.min.css",
    "files/cm/simple-mode.js",
    "files/cm/daslang-keywords.js",
    "files/cm/daslang-mode.js",
    "files/lz-string.min.js",
]


def stage():
    for rel, dest in SOURCES:
        src = os.path.join(REPO, rel.replace("/", os.sep))
        dst = os.path.join(SERVE, dest.replace("/", os.sep))
        os.makedirs(os.path.dirname(dst), exist_ok=True)
        if os.path.exists(src):
            shutil.copy2(src, dst)
        else:
            print("  MISSING %s" % rel)
    print("staged %d source file(s) from the repo" % len(SOURCES))


def fetch_runtime():
    for rel in RUNTIME:
        dst = os.path.join(SERVE, rel.replace("/", os.sep))
        if os.path.exists(dst) and os.path.getsize(dst) > 0:
            print("  have %s" % rel)
            continue
        os.makedirs(os.path.dirname(dst), exist_ok=True)
        url = SITE + rel
        print("  fetch %s" % url)
        with urllib.request.urlopen(url) as r, open(dst, "wb") as f:
            shutil.copyfileobj(r, f)
    print("runtime staged")


class Handler(http.server.SimpleHTTPRequestHandler):
    def end_headers(self):
        # Production serves these on /playground*; pthreads need the isolation.
        self.send_header("Cross-Origin-Opener-Policy", "same-origin")
        self.send_header("Cross-Origin-Embedder-Policy", "require-corp")
        self.send_header("Cross-Origin-Resource-Policy", "same-origin")
        self.send_header("Cache-Control", "no-store")
        super().end_headers()

    def log_message(self, fmt, *args):
        if "404" in (fmt % args):
            sys.stderr.write("404 %s\n" % (args[0] if args else ""))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--port", type=int, default=8791)
    ap.add_argument("--fetch-runtime", action="store_true")
    args = ap.parse_args()

    os.makedirs(SERVE, exist_ok=True)
    stage()
    if args.fetch_runtime:
        fetch_runtime()

    os.chdir(SERVE)
    socketserver.TCPServer.allow_reuse_address = True
    with socketserver.TCPServer(("127.0.0.1", args.port), Handler) as httpd:
        print("serving %s at http://127.0.0.1:%d/playground/" % (SERVE, args.port))
        httpd.serve_forever()


if __name__ == "__main__":
    main()
