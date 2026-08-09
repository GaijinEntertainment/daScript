#!/usr/bin/env python3
"""Dummy supervised program for exercising the watchdog's desktop surfaces (tray icon, menu,
notifications) without a real service. Serves the health endpoint the watchdog polls, a hello
page for the tray's Open item, a /crash link to trip the crash path on demand, and the
POST /shutdown the tray's Shutdown routes through.

Rig: put watchdog.py beside this file with a watchdog.json of
  {"name": "dummy", "tray": true, "program": "dummy_server.py",
   "health_url": "http://127.0.0.1:8765/v1/models",
   "shutdown_url": "http://127.0.0.1:8765/shutdown", "stable_seconds": 15}
then run `python3 watchdog.py`.
"""
import os
import threading
import time
from http.server import BaseHTTPRequestHandler, HTTPServer

PORT = 8765

PAGE = """<!doctype html><meta charset="utf-8"><title>dummy server</title>
<body style="font: 16px system-ui; padding: 2em; max-width: 40em">
<h1>dummy server</h1>
<p>Watchdog desktop-surface test target. The tray's Open item brought you here.</p>
<p><a href="/crash">crash me</a> — exits 139 so the watchdog's crash notification,
bundle, backoff, and recovery notification can all be watched in one click.</p>
</body>"""


def exit_soon(code: int) -> None:
    # let the in-flight response land before the process dies
    threading.Thread(target=lambda: (time.sleep(0.2), os._exit(code)), daemon=True).start()


class Handler(BaseHTTPRequestHandler):
    def reply(self, code: int, body: str, ctype: str = "text/plain") -> None:
        data = body.encode("utf-8")
        self.send_response(code)
        self.send_header("Content-Type", f"{ctype}; charset=utf-8")
        self.send_header("Content-Length", str(len(data)))
        self.end_headers()
        self.wfile.write(data)

    def do_GET(self) -> None:
        if self.path == "/v1/models":
            self.reply(200, '{"object":"list","data":[]}', "application/json")
        elif self.path == "/crash":
            print("dummy: crashing on request", flush=True)
            self.reply(200, "crashing\n")
            exit_soon(139)
        else:
            self.reply(200, PAGE, "text/html")

    def do_POST(self) -> None:
        if self.path == "/shutdown":
            print("dummy: shutdown requested", flush=True)
            self.reply(200, "bye\n")
            exit_soon(0)
        else:
            self.reply(404, "no such endpoint\n")

    def log_message(self, *_args) -> None:
        pass  # the watchdog logs every stdout line; per-request spam adds nothing


def main() -> None:
    print("dummy: warming up", flush=True)
    time.sleep(3)  # long enough for the tray to visibly show "booting"
    server = HTTPServer(("127.0.0.1", PORT), Handler)
    # this exact phrase trips the watchdog's "ready" startup stage, like the real server
    print(f"listening on http://127.0.0.1:{PORT}", flush=True)
    server.serve_forever()


if __name__ == "__main__":
    main()
