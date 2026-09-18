#!/usr/bin/env python3
"""Local preview rig for dasllama.io — serves site-dasllama/ with the shared files
staged the same way the deploy does: forge.css / nav-dropdown.css / dasllama-table.css /
github-star.js come from ../site/files (single source of truth in the repo), everything
else from here.

/examples/<id>/ is served from ../web/output64/examples/<id>/ - where `daspkg release wasm`
writes a browser example - with the two cross-origin-isolation headers the Caddy vhost sends
there (the -pthread wasm64 builds need SharedArrayBuffer). Build one, stage its model set into
<id>/models/ the way the deploy does - `examples/dasLLAMA/wasm/mint_models.py --example
examples/dasLLAMA/<id> --config <the wasm64 build's DlimConfiguration JSON> --daslang bin/daslang
--out web/output64/examples/<id>/models --stamp-page web/output64/examples/<id>/<id>.html` - and
the card works here as deployed. With a `devlog.on` marker file beside the examples (or
`?devlog=1` on the page) a served example page also carries an injected console/touch/audio
tap that POSTs to /api/devlog, so that page is not the deployed one.

/api/* is proxied to a locally running ladder service (utils/internal/dasllama-ladder on :8201),
mirroring the Caddy vhost - except /api/devlog, which this server appends to
web/output64/examples/devlog.jsonl itself, one JSON line per page event — start one with real data to preview the live pages:

    bin/Release/daslang.exe utils/internal/dasllama-ladder/main.das -- --db ladder.db \
        --official-dir modules/dasLLAMA/performance/records
    py -3 serve.py            # http://127.0.0.1:8932/
"""
import http.server
import os
import socketserver
import threading
import urllib.error
import urllib.request

HERE = os.path.dirname(os.path.abspath(__file__))
SITE_FILES = os.path.normpath(os.path.join(HERE, "..", "site", "files"))
EXAMPLES_OUT = os.path.realpath(os.path.join(HERE, "..", "web", "output64", "examples"))
SHARED = {"forge.css", "nav-dropdown.css", "dasllama-table.css", "github-star.js"}
PORT = 8932
LADDER = "http://127.0.0.1:8201"
DEVLOG = os.path.join(EXAMPLES_OUT, "devlog.jsonl")
DEVLOG_MAX_BODY = 64 * 1024        # the page caps a line at 4000 chars; anything past this is not a devlog line
DEVLOG_LOCK = threading.Lock()
DEVLOG_ON = os.path.join(EXAMPLES_OUT, "devlog.on")
DEVLOG_SCRIPT = b"""<script>
(function () {
  var page = location.pathname, seq = 0;
  function post(kind, text) {
    try {
      var body = JSON.stringify({ t: Date.now(), page: page, seq: seq++, kind: kind, text: String(text).slice(0, 4000) });
      if (!(navigator.sendBeacon && navigator.sendBeacon('/api/devlog', body)))
        fetch('/api/devlog', { method: 'POST', body: body, keepalive: true }).catch(function () {});
    } catch (e) {}
  }
  ['log', 'info', 'warn', 'error'].forEach(function (k) {
    var orig = console[k];
    console[k] = function () { post(k, Array.prototype.map.call(arguments, String).join(' ')); return orig.apply(console, arguments); };
  });
  window.addEventListener('error', function (e) { post('window-error', e.message + ' @ ' + e.filename + ':' + e.lineno); });
  window.addEventListener('unhandledrejection', function (e) { post('rejection', e.reason && (e.reason.stack || e.reason)); });
  var fingers = 0;
  document.addEventListener('touchstart', function (e) { fingers = Math.max(fingers, e.touches.length); }, true);
  var lastTick = 0;
  function tick(now) {
    requestAnimationFrame(tick);
    if (now - lastTick < 5000) return;
    lastTick = now;
   try {
    var c = document.getElementById('canvas');
    post('tick', JSON.stringify({ ua: navigator.userAgent.slice(0, 80), coarse: matchMedia('(pointer: coarse)').matches, hover: matchMedia('(hover: none)').matches,
      points: navigator.maxTouchPoints, dpr: devicePixelRatio, w: innerWidth, h: innerHeight,
      canvas: c ? [c.width, c.height, c.clientWidth, c.clientHeight] : null, maxFingers: fingers,
      dasTouch: (window.Module && Module.dasTouch) ? Module.dasTouch.list.length : -1, run: !!(window.Module && Module.calledRun),
      audio: (window.__dasAudioStates || []).join(',') }));
   } catch (e) { post('tick-error', e && (e.stack || e)); }
  }
  requestAnimationFrame(tick);
  var OrigAC = window.AudioContext || window.webkitAudioContext;
  if (OrigAC) {
    var states = window.__dasAudioStates = [];
    var Wrapped = function () { var ctx = new OrigAC(...arguments); states.push(ctx); ctx.addEventListener('statechange', function () { post('audio', 'AudioContext ' + ctx.state); }); post('audio', 'AudioContext created ' + ctx.state + ' rate ' + ctx.sampleRate); return ctx; };
    Wrapped.prototype = OrigAC.prototype;
    window.AudioContext = Wrapped; if (window.webkitAudioContext) window.webkitAudioContext = Wrapped;
    window.__dasAudioStates = { join: function () { return states.map(function (c) { return c.state; }).join(','); } };
  }
  window.addEventListener('DOMContentLoaded', function () { post('open', 'devlog on, build ' + ((document.getElementById('build') || {}).textContent || 'unstamped')); });
})();
</script>"""


class Handler(http.server.SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=HERE, **kwargs)

    def do_GET(self):
        if self.path.startswith("/api/"):
            return self.proxy_api()
        if self.path.startswith("/examples/") and self.path.split("?", 1)[0].endswith(".html") \
                and ("devlog=1" in self.path or os.path.exists(DEVLOG_ON)):
            return self.serve_with_devlog()
        return super().do_GET()

    def do_POST(self):
        # a page's console lines and errors land in DEVLOG, one JSON line each, so a phone's run reads on the box that serves it
        if self.path.startswith("/api/devlog"):
            try:
                n = int(self.headers.get("Content-Length", "0"))
            except ValueError:
                n = 0
            n = min(n, DEVLOG_MAX_BODY)
            body = self.rfile.read(n) if n else b""
            if body.strip():
                with DEVLOG_LOCK, open(DEVLOG, "ab", buffering=0) as f:   # handlers run on threads: one writer at a time, one write per line
                    f.write(body.rstrip(b"\n") + b"\n")
            self.send_response(204)
            self.end_headers()
            return
        self.send_response(404)
        self.end_headers()

    def serve_with_devlog(self):
        path = self.translate_path(self.path)
        try:
            with open(path, "rb") as f:
                html = f.read()
        except OSError:
            self.send_response(404)
            self.end_headers()
            return
        html = html.replace(b"<head>", b"<head>" + DEVLOG_SCRIPT, 1)
        self.send_response(200)
        self.send_header("Content-Type", "text/html")
        self.send_header("Cache-Control", "no-store")
        self.send_header("Content-Length", str(len(html)))
        self.end_headers()
        self.wfile.write(html)

    def end_headers(self):
        # mirrors the `header /examples/*` block of utils/internal/dasllama-ladder/caddy.snippet
        if self.path.startswith("/examples/"):
            self.send_header("Cross-Origin-Opener-Policy", "same-origin")
            self.send_header("Cross-Origin-Embedder-Policy", "require-corp")
        super().end_headers()

    def proxy_api(self):
        try:
            with urllib.request.urlopen(LADDER + self.path, timeout=30) as up:
                body = up.read()
                self.send_response(up.status)
                for k in ("Content-Type", "Content-Disposition", "Cache-Control"):
                    if up.headers.get(k):
                        self.send_header(k, up.headers[k])
                self.send_header("Content-Length", str(len(body)))
                self.end_headers()
                self.wfile.write(body)
        except urllib.error.HTTPError as e:
            body = e.read()
            self.send_response(e.code)
            self.send_header("Content-Type", e.headers.get("Content-Type", "application/json"))
            self.send_header("Content-Length", str(len(body)))
            self.end_headers()
            self.wfile.write(body)
        except OSError:
            self.send_response(502)
            self.send_header("Content-Type", "application/json")
            self.end_headers()
            self.wfile.write(b'{"error":"ladder service not running on 8201"}')

    def translate_path(self, path):
        clean = path.split("?", 1)[0].split("#", 1)[0]
        if clean.startswith("/files/"):
            name = os.path.basename(clean)
            if name in SHARED:
                return os.path.join(SITE_FILES, name)
        if clean.startswith("/examples/"):
            rel = clean[len("/examples/"):]
            # containment, not pattern checks: `..`, an absolute remainder (a doubled slash) and a
            # drive-qualified one on Windows (`C:foo`) all resolve outside EXAMPLES_OUT and fall
            # through to the site tree, where no such path exists
            full = os.path.realpath(os.path.join(EXAMPLES_OUT, rel))
            if rel and full.startswith(EXAMPLES_OUT + os.sep):
                return full
        return super().translate_path(path)


class Server(socketserver.ThreadingTCPServer):
    allow_reuse_address = True


if __name__ == "__main__":
    with Server(("127.0.0.1", PORT), Handler) as httpd:
        print(f"dasllama.io preview: http://127.0.0.1:{PORT}/  (api -> {LADDER})")
        httpd.serve_forever()
