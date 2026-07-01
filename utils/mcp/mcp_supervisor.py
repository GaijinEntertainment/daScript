#!/usr/bin/env python3
"""daslang MCP stdio supervisor.

Claude Code spawns THIS over stdio (so CC's HTTP-transport OAuth probe never
happens — see anthropics/claude-code#46879). It forwards MCP JSON-RPC to a
daslang child (spawned via daslang-mcp-msvc.cmd, which sets up vcvars), and
respawns that child on death/rebuild, replaying the MCP `initialize` handshake.

Result: daslang can be killed (e.g. to release DLL locks before a rebuild) or
crash, and CC never sees a disconnect — CC's pipe is to this supervisor, which
does not die when the child does. The supervisor answers `initialize` and
`ping` itself and spawns the daslang child lazily on the first real `tools/*`
call, so idle keepalives during a build don't respawn a deliberately-killed
child and re-lock the DLLs.

Per-worktree / per-session isolation is automatic: every CC session spawns its
own supervisor, which spawns that worktree's own daslang. No ports, no config.

.mcp.json (written by --emit-config / setup.das):
    "daslang": { "command": "python", "args": ["utils/mcp/mcp_supervisor.py"] }

Pre-build guard: kill the daslang child (a `daslang.exe` running mcp\\main.das)
to release its DLL locks; build; the next tool call respawns the fresh binary.
The supervisor stays up throughout, so CC never disconnects.
"""
from __future__ import annotations

import argparse
import json
import os
import subprocess
import sys
import tempfile
import threading

IS_WINDOWS = os.name == "nt"
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
REPO_ROOT = os.path.dirname(os.path.dirname(SCRIPT_DIR))  # utils/mcp -> utils -> repo

# The daslang MCP server's InitializeResult is a fixed constant
# (utils/mcp/protocol_core.das: handle_initialize). Answering it here lets CC
# connect instantly while the daslang child stays unspawned until first use.
INIT_RESULT = {
    "protocolVersion": "2025-11-25",
    "capabilities": {"tools": {}},
    "serverInfo": {"name": "daslang", "version": "0.1.0"},
}

SUPERVISOR_LOG = os.path.join(tempfile.gettempdir(), "daslang_mcp_supervisor.log")


def _log(msg: str):
    try:
        with open(SUPERVISOR_LOG, "a", encoding="utf-8") as f:
            f.write(msg + "\n")
    except Exception:
        pass


class ChildDead(Exception):
    """The daslang child exited / its pipe broke mid-exchange."""


class DaslangChild:
    """Supervises the stdio daslang MCP child: lazy spawn, transparent respawn
    with handshake replay, serialized forwarding."""

    def __init__(self, launcher: list[str], stderr_log: str, cwd: str):
        self.launcher = launcher
        self.stderr_log = stderr_log
        self.cwd = cwd
        self.proc: subprocess.Popen | None = None
        self.init_request: dict | None = None   # cached client `initialize`, replayed on spawn
        self.initialized_seen = False
        self.lock = threading.RLock()
        self._stderr_fh = None

    def cache_init(self, msg: dict):
        with self.lock:
            self.init_request = msg

    def mark_initialized(self):
        with self.lock:
            self.initialized_seen = True

    # ---- lifecycle ------------------------------------------------------
    def _kill_proc(self):
        """Terminate (if alive), reap, and close the pipes of the current child.
        Run before every respawn and on stop() so a long kill-rebuild session
        never orphans a live daslang or leaks its pipe fds."""
        p = self.proc
        self.proc = None
        if p is None:
            return
        try:
            if p.poll() is None:
                if IS_WINDOWS:
                    subprocess.run(["taskkill", "/F", "/T", "/PID", str(p.pid)],
                                   stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
                else:
                    p.terminate()
                p.wait(timeout=10)
        except Exception:
            pass
        for pipe in (p.stdin, p.stdout):
            try:
                if pipe is not None:
                    pipe.close()
            except Exception:
                pass

    def _spawn_and_replay(self):
        self._kill_proc()   # clean up any prior child before respawning
        if self._stderr_fh is None:
            self._stderr_fh = open(self.stderr_log, "ab", buffering=0)
        self.proc = subprocess.Popen(
            self.launcher,
            stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=self._stderr_fh,
            cwd=self.cwd, text=True, encoding="utf-8", errors="replace", bufsize=1,
        )
        _log(f"spawned daslang child pid={self.proc.pid}")
        if self.init_request is not None:
            self._write_line(json.dumps(self.init_request, separators=(",", ":")))
            self._read_line()  # consume & discard the replayed initialize result
            if self.initialized_seen:
                self._write_line('{"jsonrpc":"2.0","method":"notifications/initialized"}')

    def _ensure_alive(self):
        if self.proc is None or self.proc.poll() is not None:
            self._spawn_and_replay()

    def _write_line(self, line: str):
        try:
            self.proc.stdin.write(line + "\n")
            self.proc.stdin.flush()
        except (BrokenPipeError, OSError) as e:
            raise ChildDead(str(e))

    def _read_line(self) -> str:
        while True:
            try:
                line = self.proc.stdout.readline()
            except (OSError, ValueError) as e:
                raise ChildDead(str(e))
            if line == "":
                raise ChildDead("eof")
            line = line.strip()
            if line:
                return line

    # ---- forwarding -----------------------------------------------------
    def request(self, msg: dict) -> str:
        """Forward a request (has id) and return the daslang response line.
        The retry covers a child that's dead / dies *before* the request is
        delivered (the build-guard case). Once the write succeeds we do NOT
        re-send on a later failure — a retry could double-execute a
        side-effecting tool (run_test, live_*, format_file); a child that dies
        while we read the response surfaces as an error instead."""
        with self.lock:
            line = json.dumps(msg, separators=(",", ":"))
            try:
                self._ensure_alive()
                self._write_line(line)
            except ChildDead:
                self._spawn_and_replay()        # not yet delivered -> respawn + resend once
                self._write_line(line)
            return self._read_line()            # ChildDead here propagates (no re-send)

    def notify(self, msg: dict):
        """Forward a notification (no id) to a live child; skip if dead (state
        is re-established via handshake replay on the next spawn)."""
        with self.lock:
            if self.proc is None or self.proc.poll() is not None:
                return
            try:
                self._write_line(json.dumps(msg, separators=(",", ":")))
            except ChildDead:
                pass

    def stop(self):
        with self.lock:
            self._kill_proc()
            if self._stderr_fh is not None:
                try:
                    self._stderr_fh.close()
                except Exception:
                    pass
                self._stderr_fh = None   # reopened on the next spawn


def handle(child: DaslangChild, msg: dict) -> str | None:
    """Return a response line for a request, or None for a notification.
    initialize/ping are answered locally; tools/* are forwarded (spawning the
    child on first use)."""
    method = msg.get("method")
    has_id = "id" in msg
    mid = msg.get("id")

    if method == "initialize":
        child.cache_init(msg)   # replayed to the child when it first spawns
        return json.dumps({"jsonrpc": "2.0", "id": mid, "result": INIT_RESULT})
    if method in ("notifications/initialized", "initialized"):
        child.mark_initialized()
        child.notify(msg)
        return None
    if method == "ping" and has_id:
        return json.dumps({"jsonrpc": "2.0", "id": mid, "result": {}})
    if not has_id:
        child.notify(msg)
        return None
    return child.request(msg)   # tools/list, tools/call, anything else


def _default_launcher() -> list[str]:
    if IS_WINDOWS:
        return ["cmd", "/c", os.path.join(SCRIPT_DIR, "daslang-mcp-msvc.cmd")]
    # POSIX: run the built binary directly, trying the usual single/multi-config
    # output locations (bin/ and build/, cf. setup.das locate_binary) — first
    # existing wins.
    main_das = os.path.join(SCRIPT_DIR, "main.das")
    for rel in ("bin/Release/daslang", "bin/daslang", "build/daslang", "build/bin/daslang"):
        cand = os.path.join(REPO_ROOT, rel)
        if os.path.exists(cand):
            return [cand, main_das]
    return [os.path.join(REPO_ROOT, "bin", "daslang"), main_das]  # best-guess fallback


def write_mcp_json(repo_root: str) -> bool:
    """Set mcpServers.daslang to spawn this supervisor over stdio, preserving
    every other server (github, …). Atomic; never clobbers an unparseable file.
    Returns True if the file was (re)written, False if left untouched."""
    path = os.path.join(repo_root, ".mcp.json")
    data = {"mcpServers": {}}
    if os.path.exists(path):
        try:
            with open(path, encoding="utf-8") as f:
                data = json.load(f)
        except Exception as e:
            print(f"  WARNING: {path} did not parse ({e}); leaving it untouched", flush=True)
            return False
        if not isinstance(data, dict):
            print(f"  WARNING: {path} is not a JSON object; leaving it untouched", flush=True)
            return False
    servers = data.get("mcpServers")
    if servers is None:
        servers = {}
        data["mcpServers"] = servers
    elif not isinstance(servers, dict):
        print(f"  WARNING: {path} has a non-object 'mcpServers'; leaving it untouched", flush=True)
        return False
    prev = servers.get("daslang", {})
    entry = {"command": "python", "args": ["utils/mcp/mcp_supervisor.py"]}
    if isinstance(prev, dict) and "defer_loading" in prev:
        entry["defer_loading"] = prev["defer_loading"]
    servers["daslang"] = entry
    tmp = path + ".tmp"
    with open(tmp, "w", encoding="utf-8") as f:
        json.dump(data, f, indent=2)
        f.write("\n")
    os.replace(tmp, path)
    return True


def serve(repo_root: str, stderr_log: str):
    child = DaslangChild(_default_launcher(), stderr_log, cwd=repo_root)
    _log(f"supervisor up (repo={repo_root})")
    out = sys.stdout.buffer            # binary: avoid Windows \r\n translation on the JSON-RPC pipe
    try:
        for raw in sys.stdin.buffer:   # newline-delimited JSON-RPC from Claude Code
            text = raw.decode("utf-8", "replace").strip()
            if not text:
                continue
            try:
                msg = json.loads(text)
            except Exception as e:
                _log(f"parse error: {e}: {text[:200]}")
                continue
            if not isinstance(msg, dict):   # MCP 2025-06-18 removed JSON-RPC batching
                _log(f"ignoring non-object message: {text[:120]}")
                continue
            try:
                resp = handle(child, msg)
            except Exception as e:
                _log(f"handle error: {e}")
                # Respond whenever the request carried an `id` field (same test
                # handle() uses to tell requests from notifications) — including
                # an explicit null id — so the client isn't left hanging.
                resp = (json.dumps({"jsonrpc": "2.0", "id": msg.get("id"),
                        "error": {"code": -32000, "message": f"supervisor error: {e}"}})
                        if "id" in msg else None)
            if resp is not None:
                out.write(resp.encode("utf-8") + b"\n")
                out.flush()
    finally:
        child.stop()
        _log("supervisor down (stdin closed)")


def main():
    ap = argparse.ArgumentParser(description="daslang MCP stdio supervisor with auto-respawn")
    ap.add_argument("--repo-root", default=REPO_ROOT)
    ap.add_argument("--stderr-log", default=os.environ.get(
        "DASLANG_MCP_CHILD_LOG",
        os.path.join(tempfile.gettempdir(), "daslang_mcp_child_stderr.log")))
    ap.add_argument("--emit-config", action="store_true",
                    help="write the worktree's .mcp.json daslang entry and exit")
    args = ap.parse_args()
    repo_root = os.path.abspath(args.repo_root)

    if args.emit_config:
        if write_mcp_json(repo_root):
            print(f"wrote {os.path.join(repo_root, '.mcp.json')}: daslang -> stdio supervisor (utils/mcp/mcp_supervisor.py)")
            return
        sys.exit(1)   # left an existing malformed .mcp.json untouched -> fail so callers surface it
    serve(repo_root, args.stderr_log)


if __name__ == "__main__":
    main()
