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
import http.server
import os
import shutil
import socketserver
import sys
import urllib.request

# Derived from this file's location, not hardcoded: the script lives in the tree it
# serves, so a checkout or worktree other than the author's would otherwise stage the
# WRONG tree's sources and say nothing about it. DASWEB_REPO still overrides.
_HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.environ.get("DASWEB_REPO") or os.path.dirname(_HERE)
SERVE = os.path.join(_HERE, "_serve")
SITE = "https://daslang.io/"

# repo path -> served path
SOURCES = [
    ("site/playground/index.html",            "playground/index.html"),
    ("site/playground/run-frame.html",        "playground/run-frame.html"),
    ("site/playground/playground-runner.js",  "playground/playground-runner.js"),
    ("site/playground/playground-init.js",    "playground/playground-init.js"),
    ("site/playground/playground-tabs.js",    "playground/playground-tabs.js"),
    ("site/playground/playground-share.js",   "playground/playground-share.js"),
    ("site/playground/playground-wasm.js",    "playground/playground-wasm.js"),
    ("site/playground/playground-splitter.js","playground/playground-splitter.js"),
    ("web/examples/ui/src/main.js",           "playground/main.js"),
    ("web/examples/ui/src/main.css",          "playground/main.css"),
    ("site/playground/forge-skin.css",        "playground/forge-skin.css"),
    ("site/coi-serviceworker.js",             "coi-serviceworker.js"),
    # The landing page, because the hero editor hands its buffer off to
    # /playground/ via #code= — that handoff is only testable with both pages
    # served from one origin.
    ("site/index.html",                       "index.html"),
]

# repo dir -> served dir, copied whole. The sample tree comes from the repo (not
# the live site) so edits to a sample are testable before they ship.
SOURCE_TREES = [
    ("web/examples/ui/samples", "playground/samples"),
    # Site assets (forge skin, CodeMirror, lz-string) live in the repo, so the
    # landing page and the playground both dress themselves from the checkout
    # rather than the live site. Staged before RUNTIME fetches, which fill only
    # what the repo does not carry.
    ("site/files", "files"),
    # what the samples' .das.assets.json sidecars fetch. The sidecar paths are
    # relative, so they resolve against the frame's URL — under /playground/.
    ("tutorials/_assets", "playground/tutorials/_assets"),
]

# fetched from the live site: build artifacts + vendored libs, keyed by the path
# the page requests them at (relative to the served root).
RUNTIME = [
    "playground/daslang_static.js",
    "playground/daslang_static.wasm",
    "playground/jquery-3.6.0.min.js",
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
    for rel, dest in SOURCE_TREES:
        src = os.path.join(REPO, rel.replace("/", os.sep))
        dst = os.path.join(SERVE, dest.replace("/", os.sep))
        if os.path.isdir(src):
            shutil.copytree(src, dst, dirs_exist_ok=True)
        else:
            print("  MISSING %s/" % rel)
    print("staged %d source file(s) + %d tree(s) from the repo"
          % (len(SOURCES), len(SOURCE_TREES)))


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


# The sample service (utils/internal/dasweb-playground) answers these in production.
# The rig has no backend, so it answers them itself with the "nothing stored,
# no builder" shape — which is what the page's fallbacks are written for: the
# dropdown drops back to the repo's data.json, and the wasm engine stays off.
#
# Answering matters beyond tidiness. On a 404 the page's handlers throw without
# reading the body, so the request never finishes as far as the browser is
# concerned and the page never reaches network idle — which hangs anything
# waiting on that, Playwright's `networkidle` included.
API_STUBS = {
    "/api/samples/listed": b"[]",
    "/api/build/info": b'{"enabled": false, "toolchain": ""}',
}


class Handler(http.server.SimpleHTTPRequestHandler):
    def do_GET(self):
        body = API_STUBS.get(self.path.split("?")[0])
        if body is None:
            return super().do_GET()
        self.send_response(200)
        self.send_header("Content-Type", "application/json")
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

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


class ThreadedServer(socketserver.ThreadingMixIn, socketserver.TCPServer):
    """One thread per connection.

    The page opens several connections at once and pulls a multi-megabyte
    runtime into each run frame. A serial server makes every other request
    wait behind those transfers, so the page never reaches network idle and
    anything waiting on that (Playwright's `networkidle`, the API fetches
    that decide which engines to offer) hangs rather than failing.
    """
    daemon_threads = True
    allow_reuse_address = True


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
    with ThreadedServer(("127.0.0.1", args.port), Handler) as httpd:
        print("serving %s at http://127.0.0.1:%d/playground/" % (SERVE, args.port))
        httpd.serve_forever()


if __name__ == "__main__":
    main()
