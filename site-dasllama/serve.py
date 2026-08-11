#!/usr/bin/env python3
"""Local preview rig for dasllama.io — serves site-dasllama/ with the shared css
staged the same way the deploy does: forge.css / nav-dropdown.css / dasllama-table.css
come from ../site/files (single source of truth in the repo), everything else from here.

    py -3 serve.py            # http://127.0.0.1:8932/
"""
import http.server
import os
import socketserver

HERE = os.path.dirname(os.path.abspath(__file__))
SITE_FILES = os.path.normpath(os.path.join(HERE, "..", "site", "files"))
SHARED = {"forge.css", "nav-dropdown.css", "dasllama-table.css"}
PORT = 8932


class Handler(http.server.SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=HERE, **kwargs)

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
        print(f"dasllama.io preview: http://127.0.0.1:{PORT}/")
        httpd.serve_forever()
