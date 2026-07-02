#!/usr/bin/env python3
"""Wave-0 probe LSP server for the daslang Claude Code plugin (utils/lsp/ROADMAP.md).

Answers the LSP handshake, publishes one hardcoded PROBE-DIAGNOSTIC on every
didOpen/didChange/didSave, returns marker payloads for every navigation request,
and logs EVERY message verbatim to $PROBE_LOG. The log is the deliverable: it
answers the six wave-0 probe questions (what CC sends, when, and in what shape).
"""
from __future__ import annotations

import json
import os
import sys

LOG = os.environ.get("PROBE_LOG", "/tmp/daslang_lsp_probe.log")
_seq = 0


def log(tag: str, payload) -> None:
    try:
        with open(LOG, "a", encoding="utf-8") as f:
            f.write(json.dumps({"tag": tag, "payload": payload}) + "\n")
    except Exception:
        pass


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
        log("parse-error", {"error": str(e), "body": body[:400].decode("utf-8", "replace")})
        return {}


def write_message(stdout, msg: dict) -> None:
    body = json.dumps(msg, separators=(",", ":")).encode("utf-8")
    stdout.write(f"Content-Length: {len(body)}\r\n\r\n".encode("ascii"))
    stdout.write(body)
    stdout.flush()
    log("sent", msg)


def publish_probe_diagnostic(stdout, uri: str, reason: str) -> None:
    global _seq
    _seq += 1
    write_message(stdout, {
        "jsonrpc": "2.0",
        "method": "textDocument/publishDiagnostics",
        "params": {
            "uri": uri,
            "diagnostics": [{
                "range": {"start": {"line": 0, "character": 0},
                          "end": {"line": 0, "character": 1}},
                "severity": 1,
                "source": "daslang-probe",
                "message": f"PROBE-DIAGNOSTIC-{_seq} (trigger={reason})",
            }],
        },
    })


CAPABILITIES = {
    "textDocumentSync": {"openClose": True, "change": 1, "save": {"includeText": True}},
    "hoverProvider": True,
    "definitionProvider": True,
    "referencesProvider": True,
    "documentSymbolProvider": True,
    "workspaceSymbolProvider": True,
    "implementationProvider": True,
    "callHierarchyProvider": True,
}


def probe_range():
    return {"start": {"line": 0, "character": 0}, "end": {"line": 0, "character": 5}}


def handle_request(stdout, msg: dict) -> None:
    method = msg.get("method", "")
    mid = msg.get("id")
    params = msg.get("params") or {}
    uri = (params.get("textDocument") or {}).get("uri", "file:///probe-unknown")

    if method == "initialize":
        result = {"capabilities": CAPABILITIES,
                  "serverInfo": {"name": "daslang-probe", "version": "0.0.1"}}
    elif method == "shutdown":
        result = None
    elif method == "textDocument/hover":
        result = {"contents": {"kind": "markdown", "value": "PROBE-HOVER: daslang probe hover payload"}}
    elif method == "textDocument/definition":
        result = [{"uri": uri, "range": probe_range()}]
    elif method == "textDocument/references":
        result = [{"uri": uri, "range": probe_range()}]
    elif method == "textDocument/documentSymbol":
        result = [{"name": "PROBE_SYMBOL", "kind": 12, "range": probe_range(),
                   "selectionRange": probe_range()}]
    elif method == "workspace/symbol":
        result = [{"name": "PROBE_WORKSPACE_SYMBOL", "kind": 12,
                   "location": {"uri": "file:///probe.das", "range": probe_range()}}]
    else:
        write_message(stdout, {"jsonrpc": "2.0", "id": mid,
                               "error": {"code": -32601, "message": f"probe: method not found: {method}"}})
        return
    write_message(stdout, {"jsonrpc": "2.0", "id": mid, "result": result})


def main() -> None:
    log("startup", {"argv": sys.argv, "cwd": os.getcwd(),
                    "env": {k: v for k, v in os.environ.items()
                            if "CLAUDE" in k or "PLUGIN" in k or "WORKSPACE" in k}})
    stdin = sys.stdin.buffer
    stdout = sys.stdout.buffer
    while True:
        msg = read_message(stdin)
        if msg is None:
            log("shutdown", {"reason": "stdin closed"})
            return
        if not msg:
            continue
        log("received", msg)
        method = msg.get("method", "")
        if "id" in msg and method:
            handle_request(stdout, msg)
        elif method == "exit":
            log("shutdown", {"reason": "exit notification"})
            return
        elif method in ("textDocument/didOpen", "textDocument/didChange", "textDocument/didSave"):
            uri = ((msg.get("params") or {}).get("textDocument") or {}).get("uri", "file:///probe-unknown")
            publish_probe_diagnostic(stdout, uri, method)


if __name__ == "__main__":
    main()
