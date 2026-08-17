#!/usr/bin/env python3
"""Local preview rig for dasllama.io — serves site-dasllama/ with the shared files
staged the same way the deploy does: forge.css / nav-dropdown.css / dasllama-table.css /
github-star.js come from ../site/files (single source of truth in the repo), everything
else from here.

/api/* is proxied to a locally running ladder service (utils/internal/dasllama-ladder on :8201),
mirroring the Caddy vhost — start one with real data to preview the live pages:

    bin/Release/daslang.exe utils/internal/dasllama-ladder/main.das -- --db ladder.db \
        --official-dir modules/dasLLAMA/performance/records
    py -3 serve.py            # http://127.0.0.1:8932/
"""
import http.server
import os
import socketserver
import urllib.error
import urllib.request

HERE = os.path.dirname(os.path.abspath(__file__))
SITE_FILES = os.path.normpath(os.path.join(HERE, "..", "site", "files"))
SHARED = {"forge.css", "nav-dropdown.css", "dasllama-table.css", "github-star.js"}
PORT = 8932
LADDER = "http://127.0.0.1:8201"


class Handler(http.server.SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=HERE, **kwargs)

    def do_GET(self):
        if self.path.startswith("/api/"):
            return self.proxy_api()
        return super().do_GET()

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
        return super().translate_path(path)


class Server(socketserver.ThreadingTCPServer):
    allow_reuse_address = True


if __name__ == "__main__":
    with Server(("127.0.0.1", PORT), Handler) as httpd:
        print(f"dasllama.io preview: http://127.0.0.1:{PORT}/  (api -> {LADDER})")
        httpd.serve_forever()
