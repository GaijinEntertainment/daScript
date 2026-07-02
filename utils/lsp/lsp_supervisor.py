#!/usr/bin/env python3
"""daslang LSP endpoint for Claude Code (and any stdio LSP client).

Owns ALL session state: Content-Length framing, the initialize handshake, a
{uri -> text} document shadow, per-uri debounce, and request dispatch. It has
zero language knowledge — every language operation spawns a fresh daslang
running a batch subtool (utils/lsp/subtools/*.das) that prints LSP-shaped JSON
and exits. The document shadow rides along as a --overlay temp file, so
compiles see the client's buffer even when it is unsaved (Claude Code saves
before notifying; other LSP clients don't). No resident daslang, by design: no macro-state leaks across
compiles, no binary/DLL locks while builds run, per-request crash isolation.
Rationale + wave plan: utils/lsp/ROADMAP.md.

Registered via the Claude Code plugin manifest in utils/lsp/plugin/
(.claude-plugin/plugin.json). initializationOptions (all optional):
    compiler     - path to the daslang binary (else: $DASLANG_LSP_COMPILER,
                   repo-relative bin/..., then "daslang" on PATH)
    project      - .das_project file passed to subtools
    project_root - passed as `-project_root` (dynamic native modules)
    load_module  - list, each passed as `-load_module`
"""
from __future__ import annotations

import json
import os
import shutil
import subprocess
import sys
import tempfile
import threading

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
LOG = os.environ.get("DASLANG_LSP_LOG",
                     os.path.join(tempfile.gettempdir(), "daslang_lsp.log"))
DEBOUNCE_SEC = 0.1  # CC sends one didChange per Edit tool call — no keystroke bursts
VALIDATE_TIMEOUT_SEC = 60
NAV_TIMEOUT_SEC = 60


def log(tag: str, payload) -> None:
    try:
        with open(LOG, "a", encoding="utf-8") as f:
            f.write(json.dumps({"tag": tag, "payload": payload}, default=str) + "\n")
    except Exception:
        pass


def uri_to_path(uri: str) -> str:
    from urllib.parse import unquote, urlparse
    p = urlparse(uri)
    path = unquote(p.path)
    if os.name == "nt" and path.startswith("/") and len(path) > 2 and path[2] == ":":
        path = path[1:]  # /C:/x -> C:/x
    return path


def path_to_uri(path: str) -> str:
    from pathlib import Path
    try:
        return Path(path).resolve().as_uri()  # resolve(): subtool paths may be relative
    except Exception:
        return "file://" + path


def find_compiler(init_options: dict) -> str | None:
    # absolute paths only: subtools spawn with per-request cwd, so a relative
    # path that resolves here would break there
    cand = init_options.get("compiler") or os.environ.get("DASLANG_LSP_COMPILER")
    if cand and os.path.exists(cand):
        return os.path.abspath(cand)
    # repo + install-tree layouts, tried against the workspace and the
    # supervisor's own tree (bin/daslang.exe is the installed-SDK Windows spot)
    roots = [os.environ.get("CLAUDE_PROJECT_DIR", ""),
             os.path.dirname(os.path.dirname(SCRIPT_DIR))]  # utils/lsp -> utils -> root
    rels = ["bin/Release/daslang.exe", "bin/Release/daslang", "bin/daslang",
            "bin/daslang.exe", "build/daslang", "build/bin/daslang"]
    for root in roots:
        if not root:
            continue
        for rel in rels:
            p = os.path.join(root, rel)
            if os.path.exists(p):
                return os.path.abspath(p)
    return shutil.which("daslang")


class Server:
    def __init__(self):
        self.docs: dict[str, str] = {}
        self.active_uri: str | None = None  # most recent didOpen/didChange, anchors workspace/symbol
        self.init_options: dict = {}
        self.compiler: str | None = None
        self.timers: dict[str, threading.Timer] = {}
        self.inflight: dict[str, subprocess.Popen] = {}
        self.gen: dict[str, int] = {}  # per-uri validate generation, kills stale publishes
        self.lock = threading.RLock()
        self.out = sys.stdout.buffer
        self.write_lock = threading.Lock()

    # ---- wire ----------------------------------------------------------
    def write(self, msg: dict) -> None:
        body = json.dumps(msg, separators=(",", ":")).encode("utf-8")
        with self.write_lock:
            self.out.write(f"Content-Length: {len(body)}\r\n\r\n".encode("ascii"))
            self.out.write(body)
            self.out.flush()

    def reply(self, mid, result) -> None:
        self.write({"jsonrpc": "2.0", "id": mid, "result": result})

    def reply_error(self, mid, code: int, message: str) -> None:
        self.write({"jsonrpc": "2.0", "id": mid, "error": {"code": code, "message": message}})

    def publish(self, uri: str, diagnostics: list) -> None:
        self.write({"jsonrpc": "2.0", "method": "textDocument/publishDiagnostics",
                    "params": {"uri": uri, "diagnostics": diagnostics}})

    # ---- validate pipeline ---------------------------------------------
    def subtool_argv(self, subtool: str, args: list[str]) -> list[str] | None:
        if self.compiler is None:
            self.compiler = find_compiler(self.init_options)
        if self.compiler is None:
            return None
        argv = [self.compiler]
        if self.init_options.get("project_root"):
            argv += ["-project_root", self.init_options["project_root"]]
        for lm in self.init_options.get("load_module") or []:
            argv += ["-load_module", lm]
        argv += [os.path.join(SCRIPT_DIR, "subtools", subtool + ".das"), "--"] + args
        return argv

    def schedule_validate(self, uri: str) -> None:
        with self.lock:
            t = self.timers.pop(uri, None)
            if t:
                t.cancel()
            timer = threading.Timer(DEBOUNCE_SEC, self.validate, args=(uri,))
            timer.daemon = True
            self.timers[uri] = timer
            timer.start()

    def validate(self, uri: str) -> None:
        with self.lock:
            self.timers.pop(uri, None)
            self.gen[uri] = self.gen.get(uri, 0) + 1
            my_gen = self.gen[uri]
            prev = self.inflight.pop(uri, None)
            if prev and prev.poll() is None:
                prev.kill()  # newer edit supersedes the running compile

        path = uri_to_path(uri)
        args = [path]
        if self.init_options.get("project"):
            args.append(self.init_options["project"])
        argv = self.subtool_argv("validate", args)
        if argv is None:
            self.publish(uri, [self.tool_diagnostic(
                "daslang binary not found — set initializationOptions.compiler, "
                "$DASLANG_LSP_COMPILER, or put daslang on PATH")])
            return
        overlay = self.write_overlay(uri)
        if overlay:
            argv += ["--overlay", overlay]
        try:
            proc = subprocess.Popen(argv, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                    cwd=os.path.dirname(path) or None)
            with self.lock:
                self.inflight[uri] = proc
            out, err = proc.communicate(timeout=VALIDATE_TIMEOUT_SEC)
        except subprocess.TimeoutExpired:
            proc.kill()
            self.publish_if_current(uri, my_gen, [self.tool_diagnostic(
                f"daslang validate timed out after {VALIDATE_TIMEOUT_SEC}s")])
            return
        except Exception as e:
            self.publish_if_current(uri, my_gen, [self.tool_diagnostic(f"validate failed to run: {e}")])
            return
        finally:
            if overlay:
                try:
                    os.unlink(overlay)  # child has exited (or never started) by now
                except OSError:
                    pass
            with self.lock:
                if self.inflight.get(uri) is not None and self.inflight[uri].poll() is not None:
                    self.inflight.pop(uri, None)

        text = out.decode("utf-8", "replace").strip()
        if proc.returncode < 0:  # killed — a newer validate owns this uri now
            return
        try:
            payload = json.loads(text[text.index("{"):]) if "{" in text else {}
            diags = payload.get("diagnostics", [])
        except Exception:
            tail = (err.decode("utf-8", "replace").strip() or text)[-400:]
            diags = [self.tool_diagnostic(f"daslang validate crashed: {tail}")]
        self.publish_if_current(uri, my_gen, diags)

    def publish_if_current(self, uri: str, my_gen: int, diags: list) -> None:
        with self.lock:
            if self.gen.get(uri) != my_gen:
                return  # a newer edit's validate superseded this result
        self.publish(uri, diags)
        log("published", {"uri": uri, "count": len(diags)})

    @staticmethod
    def tool_diagnostic(message: str) -> dict:
        return {"range": {"start": {"line": 0, "character": 0},
                          "end": {"line": 0, "character": 1}},
                "severity": 1, "source": "daslang-lsp", "message": message}

    def write_overlay(self, uri: str | None) -> str | None:
        """Document-shadow text -> temp file for the subtool's --overlay flag.

        CC saves before it notifies, so for CC the overlay matches disk; for
        clients that send unsaved didChange text this is what makes the
        compile see the buffer. Caller unlinks the returned path."""
        text = self.docs.get(uri) if uri else None
        if text is None:
            return None
        try:
            fd, path = tempfile.mkstemp(prefix="daslang_lsp_overlay_", suffix=".das")
            with os.fdopen(fd, "w", encoding="utf-8", newline="") as f:
                f.write(text)
            return path
        except Exception as e:
            log("overlay-error", {"uri": uri, "error": str(e)})
            return None

    # ---- navigation requests ---------------------------------------------
    def nav_request(self, mid, op: str, args: list[str], cwd: str | None,
                    overlay_uri: str | None = None, data_uri: str | None = None) -> None:
        """Spawns nav.das on a worker thread so the read loop keeps consuming
        didChange while a request compiles. data_uri is the call-hierarchy
        anchor packed into each returned item's `data` (round-trips
        client->server between prepare and incoming/outgoing)."""
        if self.init_options.get("project"):
            args = args + [self.init_options["project"]]
        argv = self.subtool_argv("nav", [op] + args)
        if argv is None:
            self.reply_error(mid, -32000, "daslang binary not found — set "
                             "initializationOptions.compiler, $DASLANG_LSP_COMPILER, "
                             "or put daslang on PATH")
            return
        overlay = self.write_overlay(overlay_uri)
        if overlay:
            argv += ["--overlay", overlay]
        t = threading.Thread(target=self.run_nav, args=(mid, op, argv, cwd, overlay, data_uri),
                             daemon=True)
        t.start()

    def run_nav(self, mid, op: str, argv: list[str], cwd: str | None,
                overlay: str | None = None, data_uri: str | None = None) -> None:
        try:
            try:
                proc = subprocess.Popen(argv, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                        cwd=cwd)
                out, err = proc.communicate(timeout=NAV_TIMEOUT_SEC)
            except subprocess.TimeoutExpired:
                proc.kill()
                self.reply_error(mid, -32000, f"daslang {op} timed out after {NAV_TIMEOUT_SEC}s")
                return
            except Exception as e:
                self.reply_error(mid, -32000, f"daslang {op} failed to run: {e}")
                return
        finally:
            if overlay:
                try:
                    os.unlink(overlay)
                except OSError:
                    pass
        text = out.decode("utf-8", "replace").strip()
        try:
            payload = json.loads(text[text.index("{"):])
        except Exception:
            tail = (err.decode("utf-8", "replace").strip() or text)[-400:]
            self.reply_error(mid, -32000, f"daslang {op} crashed: {tail}")
            return
        log("nav", {"op": op, "ok": payload.get("ok"), "message": payload.get("message")})
        self.reply(mid, self.shape_nav_result(op, payload, data_uri))

    @staticmethod
    def hier_item(it: dict, data_uri: str | None) -> dict:
        item = {"name": it["name"], "kind": it["kind"], "uri": path_to_uri(it["path"]),
                "range": it["range"], "selectionRange": it["selectionRange"],
                "data": {"uri": data_uri, "file": it["file"], "line": it["line"],
                         "name": it.get("symbol") or it["name"]}}
        if it.get("detail"):
            item["detail"] = it["detail"]
        return item

    @staticmethod
    def shape_nav_result(op: str, payload: dict, data_uri: str | None = None):
        def locations():
            return [{"uri": path_to_uri(l["path"]), "range": l["range"]}
                    for l in payload.get("locations") or []]
        if op == "definition":
            locs = locations()
            return locs if locs else None
        if op in ("references", "implementation"):
            return locations()
        if op == "prepareCallHierarchy":
            items = [Server.hier_item(it, data_uri) for it in payload.get("items") or []]
            return items if items else None
        if op in ("incomingCalls", "outgoingCalls"):
            key = "from" if op == "incomingCalls" else "to"
            return [{key: Server.hier_item(c["item"], data_uri),
                     "fromRanges": c.get("fromRanges") or []}
                    for c in payload.get("calls") or []]
        if op == "hover":
            if not payload.get("contents"):
                return None
            hover = {"contents": {"kind": "markdown", "value": payload["contents"]}}
            if payload.get("hasRange"):
                hover["range"] = payload["range"]
            return hover
        if op == "documentSymbol":
            return payload.get("symbols") or []
        if op == "workspaceSymbol":
            return [{"name": s["name"], "kind": s["kind"],
                     "containerName": s.get("containerName", ""),
                     "location": {"uri": path_to_uri(s["path"]), "range": s["range"]}}
                    for s in payload.get("symbols") or []]
        return None

    def cursor_args(self, params: dict) -> tuple[list[str], str | None]:
        path = uri_to_path(params["textDocument"]["uri"])
        pos = params.get("position") or {}
        args = [path, str(pos.get("line", 0)), str(pos.get("character", 0))]
        return args, os.path.dirname(path) or None

    def active_doc_path(self) -> str | None:
        return uri_to_path(self.active_uri) if self.active_uri else None

    # ---- dispatch -------------------------------------------------------
    def handle(self, msg: dict) -> bool:
        """Returns False when the client asked to exit."""
        method = msg.get("method", "")
        mid = msg.get("id")
        params = msg.get("params") or {}

        if method == "initialize":
            self.init_options = params.get("initializationOptions") or {}
            self.reply(mid, {
                "capabilities": {
                    "textDocumentSync": {"openClose": True, "change": 1,
                                         "save": {"includeText": False}},
                    "definitionProvider": True,
                    "hoverProvider": True,
                    "referencesProvider": True,
                    "documentSymbolProvider": True,
                    "workspaceSymbolProvider": True,
                    "implementationProvider": True,
                    "callHierarchyProvider": True,
                },
                "serverInfo": {"name": "daslang-lsp", "version": "0.4.0"},
            })
        elif method == "shutdown":
            self.reply(mid, None)
        elif method == "exit":
            return False
        elif method == "textDocument/didOpen":
            doc = params["textDocument"]
            self.docs[doc["uri"]] = doc.get("text", "")
            self.active_uri = doc["uri"]
            self.schedule_validate(doc["uri"])
        elif method == "textDocument/didChange":
            uri = params["textDocument"]["uri"]
            self.active_uri = uri
            changes = params.get("contentChanges") or []
            if changes:
                # full sync (we advertise change:1); a range'd change means a
                # client ignored that — apply full-text changes only
                last = changes[-1]
                if "range" not in last:
                    self.docs[uri] = last.get("text", "")
            self.schedule_validate(uri)
        elif method == "textDocument/didSave":
            pass  # didChange already scheduled the validate
        elif method == "textDocument/didClose":
            uri = params["textDocument"]["uri"]
            self.docs.pop(uri, None)
            with self.lock:
                t = self.timers.pop(uri, None)
                if t:
                    t.cancel()
                self.gen[uri] = self.gen.get(uri, 0) + 1  # a late in-flight publish is stale now
                prev = self.inflight.pop(uri, None)
                if prev and prev.poll() is None:
                    prev.kill()
            self.publish(uri, [])
        elif method in ("textDocument/definition", "textDocument/hover",
                        "textDocument/implementation"):
            args, cwd = self.cursor_args(params)
            self.nav_request(mid, method.rsplit("/", 1)[1], args, cwd,
                             overlay_uri=params["textDocument"]["uri"])
        elif method == "textDocument/prepareCallHierarchy":
            args, cwd = self.cursor_args(params)
            self.nav_request(mid, "prepareCallHierarchy", args, cwd,
                             overlay_uri=params["textDocument"]["uri"],
                             data_uri=params["textDocument"]["uri"])
        elif method in ("callHierarchy/incomingCalls", "callHierarchy/outgoingCalls"):
            data = (params.get("item") or {}).get("data") or {}
            if not data.get("uri") or not data.get("name"):
                self.reply(mid, [])  # item didn't round-trip — nothing to resolve
            else:
                anchor = uri_to_path(data["uri"])
                op = "incomingCalls" if method.endswith("incomingCalls") else "outgoingCalls"
                self.nav_request(mid, op,
                                 [anchor, data.get("file", ""), str(data.get("line", 0)),
                                  data["name"]],
                                 os.path.dirname(anchor) or None,
                                 overlay_uri=data["uri"], data_uri=data["uri"])
        elif method == "textDocument/references":
            args, cwd = self.cursor_args(params)
            include_decl = bool((params.get("context") or {}).get("includeDeclaration"))
            self.nav_request(mid, "references", args + ["true" if include_decl else "false"], cwd,
                             overlay_uri=params["textDocument"]["uri"])
        elif method == "textDocument/documentSymbol":
            path = uri_to_path(params["textDocument"]["uri"])
            self.nav_request(mid, "documentSymbol", [path], os.path.dirname(path) or None,
                             overlay_uri=params["textDocument"]["uri"])
        elif method == "workspace/symbol":
            context = self.active_doc_path()
            if context is None:
                self.reply(mid, [])  # no open doc to anchor the compile
            else:
                self.nav_request(mid, "workspaceSymbol",
                                 [params.get("query", ""), context],
                                 os.path.dirname(context) or None,
                                 overlay_uri=self.active_uri)
        elif mid is not None:
            self.reply_error(mid, -32601, f"daslang-lsp: method not implemented: {method}")
        return True

    def serve(self) -> None:
        log("startup", {"argv": sys.argv, "cwd": os.getcwd(),
                        "project_dir": os.environ.get("CLAUDE_PROJECT_DIR")})
        stdin = sys.stdin.buffer
        while True:
            msg = read_message(stdin)
            if msg is None:
                log("shutdown", {"reason": "stdin closed"})
                return
            if not msg:
                continue
            log("received", {"method": msg.get("method"), "id": msg.get("id")})
            try:
                if not self.handle(msg):
                    log("shutdown", {"reason": "exit notification"})
                    return
            except Exception as e:
                log("handle-error", {"error": str(e), "method": msg.get("method")})
                if msg.get("id") is not None:
                    self.reply_error(msg.get("id"), -32000, f"daslang-lsp internal error: {e}")


def read_message(stdin) -> dict | None:
    headers = {}
    while True:
        line = stdin.readline()
        if not line:
            return None
        line = line.decode("ascii", "replace").strip()
        if not line:
            break
        if ":" in line:
            k, v = line.split(":", 1)
            headers[k.strip().lower()] = v.strip()
    try:
        length = int(headers.get("content-length", "0"))
    except ValueError:
        return None
    if length <= 0:
        return None
    body = stdin.read(length)
    if body is None or len(body) < length:
        return None
    try:
        return json.loads(body.decode("utf-8", "replace"))
    except Exception as e:
        log("parse-error", {"error": str(e)})
        return {}


if __name__ == "__main__":
    Server().serve()
