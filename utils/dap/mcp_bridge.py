#!/usr/bin/env python3
"""Expose the daScript Debug Adapter Protocol server as MCP tools for Codex."""

from __future__ import annotations

import argparse
import copy
import json
import os
import signal
import socket
import subprocess
import sys
import threading
import time
from collections import deque
from pathlib import Path
from typing import Any, BinaryIO


MCP_PROTOCOL_VERSION = "2025-11-25"
DEFAULT_HOST = "127.0.0.1"
DEFAULT_PORT = 10000
DEFAULT_TIMEOUT = 90.0
EVENT_LIMIT = 2000
OUTPUT_LIMIT = 200
DAP_HISTORY_LIMIT = 200
DAP_HEADER_LINE_LIMIT = 8192
DAP_HEADER_LIMIT = 65536
DAP_PAYLOAD_LIMIT = 64 * 1024 * 1024


class BridgeError(RuntimeError):
    """An error suitable for returning from an MCP tool call."""


def _read_dap_frame(stream: BinaryIO) -> dict[str, Any] | None:
    headers: dict[str, str] = {}
    header_size = 0
    while True:
        line = stream.readline(DAP_HEADER_LINE_LIMIT + 1)
        if not line:
            if header_size == 0:
                return None
            raise BridgeError("truncated DAP headers")
        if len(line) > DAP_HEADER_LINE_LIMIT:
            raise BridgeError("DAP header line is too long")
        if not line.endswith(b"\n"):
            raise BridgeError("truncated DAP header line")
        header_size += len(line)
        if header_size > DAP_HEADER_LIMIT:
            raise BridgeError("DAP headers are too large")
        if line in (b"\r\n", b"\n"):
            break
        name, separator, value = line.decode("ascii").partition(":")
        if not separator:
            raise BridgeError("invalid DAP header")
        headers[name.lower()] = value.strip()
    try:
        length = int(headers["content-length"])
    except (KeyError, ValueError) as error:
        raise BridgeError("missing or invalid DAP Content-Length") from error
    if length < 0 or length > DAP_PAYLOAD_LIMIT:
        raise BridgeError(f"DAP Content-Length is outside 0..{DAP_PAYLOAD_LIMIT}")
    payload = stream.read(length)
    if len(payload) != length:
        raise BridgeError("truncated DAP payload")
    message = json.loads(payload.decode("utf-8"))
    if not isinstance(message, dict):
        raise BridgeError("DAP payload must be a JSON object")
    return message


def _write_dap_frame(sock: socket.socket, message: dict[str, Any]) -> None:
    payload = json.dumps(message, ensure_ascii=False, separators=(",", ":")).encode(
        "utf-8"
    )
    sock.sendall(f"Content-Length: {len(payload)}\r\n\r\n".encode("ascii") + payload)


class DapClient:
    def __init__(self, timeout: float) -> None:
        self.timeout = timeout
        self.sock: socket.socket | None = None
        self.stream: BinaryIO | None = None
        self.reader: threading.Thread | None = None
        self.condition = threading.Condition()
        self.write_lock = threading.Lock()
        self.responses: dict[int, dict[str, Any]] = {}
        self.events: deque[dict[str, Any]] = deque()
        self.event_history: deque[dict[str, Any]] = deque(maxlen=DAP_HISTORY_LIMIT)
        self.next_seq = 1
        self.reader_error: BaseException | None = None
        self.stopped = True
        self.closing = False
        self.endpoint: tuple[str, int] | None = None
        self.last_endpoint: tuple[str, int] | None = None
        self.capabilities: dict[str, Any] | None = None
        self.terminated_enqueued = False
        self.last_termination_body: dict[str, Any] = {}
        self.close_reason: str | None = None

    @property
    def connected(self) -> bool:
        return self.sock is not None and not self.stopped

    def connect(self, host: str, port: int, timeout: float) -> dict[str, Any]:
        if self.connected:
            if self.endpoint == (host, port):
                return {"host": host, "port": port, "already_connected": True}
            raise BridgeError(f"already connected to {self.endpoint[0]}:{self.endpoint[1]}")
        if self.sock is not None or self.stream is not None:
            self.close()

        deadline = time.monotonic() + timeout
        last_error: OSError | None = None
        while True:
            remaining = deadline - time.monotonic()
            if remaining <= 0:
                detail = f": {last_error}" if last_error is not None else ""
                raise BridgeError(f"timed out connecting to DAP at {host}:{port}{detail}")
            try:
                sock = socket.create_connection((host, port), timeout=min(0.5, remaining))
                break
            except OSError as error:
                last_error = error
                time.sleep(min(0.05, max(remaining, 0.0)))

        sock.settimeout(None)
        stream = sock.makefile("rb")
        with self.condition:
            self.sock = sock
            self.stream = stream
            self.responses.clear()
            self.events.clear()
            self.event_history.clear()
            self.next_seq = 1
            self.reader_error = None
            self.stopped = False
            self.closing = False
            self.endpoint = (host, port)
            self.last_endpoint = (host, port)
            self.capabilities = None
            self.terminated_enqueued = False
            self.last_termination_body = {}
            self.close_reason = None
        self.reader = threading.Thread(
            target=self._read_loop, name="daslang-dap-reader", daemon=True
        )
        self.reader.start()
        return {"host": host, "port": port, "already_connected": False}

    def _read_loop(self) -> None:
        try:
            assert self.stream is not None
            while True:
                message = _read_dap_frame(self.stream)
                if message is None:
                    break
                with self.condition:
                    if message.get("type") == "response":
                        request_seq = message.get("request_seq")
                        if isinstance(request_seq, (int, float)):
                            self.responses[int(request_seq)] = message
                    elif message.get("type") == "event":
                        self._enqueue_event_locked(message)
                    self.condition.notify_all()
        except BaseException as error:
            with self.condition:
                if not self.closing:
                    self.reader_error = error
                    self.close_reason = f"reader_error: {error}"
                self.condition.notify_all()
        finally:
            with self.condition:
                was_closing = self.closing
                self.stopped = True
                if not was_closing:
                    if self.close_reason is None:
                        self.close_reason = "connection_closed"
                    self._enqueue_event_locked(
                        {
                            "type": "event",
                            "event": "terminated",
                            "body": {"reason": "connection_closed", "bridgeSynthetic": True},
                        }
                    )
                self.condition.notify_all()

    def _enqueue_event_locked(self, event: dict[str, Any]) -> None:
        if event.get("event") == "terminated":
            body = event.get("body")
            if not isinstance(body, dict):
                body = {}
            merged_body = {**self.last_termination_body, **body}
            previous_reason = self.last_termination_body.get("reason")
            current_reason = body.get("reason")
            if previous_reason and current_reason and previous_reason != current_reason:
                merged_body["reasons"] = list(
                    dict.fromkeys(
                        [
                            *self.last_termination_body.get("reasons", []),
                            previous_reason,
                            current_reason,
                        ]
                    )
                )
            self.last_termination_body = merged_body
            event["body"] = merged_body
            self.event_history.append(copy.deepcopy(event))
            if self.terminated_enqueued:
                for queued in self.events:
                    if queued.get("event") == "terminated":
                        queued["body"] = dict(merged_body)
                        break
                return
            self.terminated_enqueued = True
        else:
            self.event_history.append(copy.deepcopy(event))
        if len(self.events) >= EVENT_LIMIT:
            output_index = next(
                (index for index, item in enumerate(self.events) if item.get("event") == "output"),
                None,
            )
            if output_index is not None:
                del self.events[output_index]
            else:
                self.events.popleft()
        self.events.append(event)

    def enqueue_event(self, event: str, body: dict[str, Any]) -> None:
        with self.condition:
            self._enqueue_event_locked({"type": "event", "event": event, "body": body})
            self.condition.notify_all()

    def request(
        self, command: str, arguments: dict[str, Any], timeout: float | None = None
    ) -> dict[str, Any]:
        if not self.connected or self.sock is None:
            raise BridgeError("not connected to a DAP server; call debug_connect or debug_launch")
        with self.condition:
            seq = self.next_seq
            self.next_seq += 1
        message = {
            "seq": seq,
            "type": "request",
            "command": command,
            "arguments": arguments,
        }
        try:
            with self.write_lock:
                assert self.sock is not None
                _write_dap_frame(self.sock, message)
        except OSError as error:
            raise BridgeError(f"failed to send DAP {command}: {error}") from error

        deadline = time.monotonic() + (self.timeout if timeout is None else timeout)
        with self.condition:
            while seq not in self.responses:
                if self.reader_error is not None:
                    raise BridgeError(f"DAP reader failed: {self.reader_error}")
                if self.stopped:
                    raise BridgeError(f"DAP connection closed before replying to {command}")
                remaining = deadline - time.monotonic()
                if remaining <= 0:
                    raise BridgeError(f"DAP request timed out: {command}")
                self.condition.wait(remaining)
            response = self.responses.pop(seq)
        if not response.get("success", False):
            raise BridgeError(f"DAP {command} failed: {response.get('message', response)}")
        return {
            "command": command,
            "success": True,
            "body": response.get("body"),
        }

    def initialize(self, timeout: float | None = None) -> dict[str, Any]:
        if self.capabilities is not None:
            return {
                "command": "initialize",
                "success": True,
                "body": self.capabilities,
                "cached": True,
            }
        result = self.request(
            "initialize",
            {
                "clientID": "codex",
                "clientName": "Codex daslang DAP MCP bridge",
                "adapterID": "daslang",
                "pathFormat": "path",
                "linesStartAt1": True,
                "columnsStartAt1": True,
                "supportsVariableType": True,
                "supportsVariablePaging": True,
                "supportsRunInTerminalRequest": False,
            },
            timeout=timeout,
        )
        body = result.get("body")
        self.capabilities = body if isinstance(body, dict) else {}
        return result

    def wait_event(
        self, event_name: str | None, timeout: float
    ) -> dict[str, Any]:
        deadline = time.monotonic() + timeout
        with self.condition:
            while True:
                event_index = next(
                    (
                        index
                        for index, item in enumerate(self.events)
                        if event_name is None or item.get("event") == event_name
                    ),
                    None,
                )
                if event_index is not None:
                    event = self.events[event_index]
                    del self.events[event_index]
                    return event
                if self.reader_error is not None:
                    raise BridgeError(f"DAP reader failed: {self.reader_error}")
                remaining = deadline - time.monotonic()
                if remaining <= 0 or self.stopped:
                    return {
                        "type": "event",
                        "event": None,
                        "body": {
                            "timedOut": remaining <= 0,
                            "connectionClosed": self.stopped,
                            "requestedEvent": event_name,
                        },
                    }
                self.condition.wait(remaining)

    def close(self, reason: str = "client_close") -> None:
        sock = self.sock
        stream = self.stream
        with self.condition:
            was_connected = self.connected
            self.closing = True
            if was_connected:
                self.close_reason = reason
        if sock is not None:
            try:
                sock.shutdown(socket.SHUT_RDWR)
            except OSError:
                pass
            try:
                sock.close()
            except OSError:
                pass
        if stream is not None:
            try:
                stream.close()
            except OSError:
                pass
        reader = self.reader
        if reader is not None and reader is not threading.current_thread():
            reader.join(timeout=1.0)
        with self.condition:
            self.sock = None
            self.stream = None
            self.reader = None
            self.stopped = True
            self.endpoint = None
            self.capabilities = None
            self.condition.notify_all()

    def termination_body(self) -> dict[str, Any]:
        with self.condition:
            return dict(self.last_termination_body)

    def history(self) -> list[dict[str, Any]]:
        with self.condition:
            return copy.deepcopy(list(self.event_history))


def _schema(
    properties: dict[str, Any] | None = None, required: list[str] | None = None
) -> dict[str, Any]:
    return {
        "type": "object",
        "properties": properties or {},
        "required": required or [],
        "additionalProperties": False,
    }


HOST_PORT = {
    "host": {"type": "string", "description": "DAP host; defaults to 127.0.0.1."},
    "port": {
        "type": "integer",
        "minimum": 1,
        "maximum": 65535,
        "description": "DAP TCP port; defaults to 10000.",
    },
    "timeout_sec": {"type": "number", "exclusiveMinimum": 0},
}

LAUNCH_HOST_PORT = {
    **HOST_PORT,
    "port": {
        "type": "integer",
        "minimum": 1,
        "maximum": 65535,
        "description": (
            "DAP TCP port. When omitted, debug_launch selects an available local port."
        ),
    },
}

SESSION_SETTINGS = {
    "cwd": {"type": "string"},
    "paths": {"type": "array", "items": {"type": "string"}},
    "path_aliases": {"type": "object", "additionalProperties": {"type": "string"}},
    "inline_preview_limit": {"type": "integer", "minimum": 0},
    "max_children_count": {"type": "integer", "minimum": 0},
    "collect_all_globals": {"type": "boolean"},
}

THREAD_ID = {
    "thread_id": {"type": "integer", "minimum": 1, "description": "DAP thread id."}
}

TOOLS = [
    {
        "name": "debug_connect",
        "description": "Connect to an already running daScript DAP TCP server.",
        "inputSchema": _schema(HOST_PORT),
    },
    {
        "name": "debug_initialize",
        "description": "Initialize the connected DAP session and return debugger capabilities.",
        "inputSchema": _schema(),
    },
    {
        "name": "debug_launch",
        "description": (
            "Launch a .das program with --das-wait-debugger, connect, initialize DAP, "
            "and send launch. Set breakpoints, call debug_threads, then "
            "debug_configuration_done to release startup."
        ),
        "inputSchema": _schema(
            {
                "file": {"type": "string", "description": "Script path."},
                "executable": {"type": "string", "description": "Optional daslang binary."},
                "program_args": {"type": "array", "items": {"type": "string"}},
                "stepping_debugger": {
                    "type": "boolean",
                    "description": (
                        "Use native statement stepping instead of the default instrumentation "
                        "debugger; defaults to false so callers choose the debugging mode explicitly."
                    ),
                },
                "project": {"type": "string"},
                "project_root": {"type": "string"},
                "load_modules": {"type": "array", "items": {"type": "string"}},
                **LAUNCH_HOST_PORT,
                **SESSION_SETTINGS,
            },
            ["file"],
        ),
    },
    {
        "name": "debug_attach",
        "description": "Connect if needed, initialize DAP, and attach to a running daScript runtime.",
        "inputSchema": _schema({**HOST_PORT, **SESSION_SETTINGS}),
    },
    {
        "name": "debug_set_breakpoints",
        "description": "Replace source breakpoints for one file; pass an empty lines array to clear them.",
        "inputSchema": _schema(
            {
                "file": {"type": "string"},
                "lines": {
                    "type": "array",
                    "items": {"type": "integer", "minimum": 1},
                },
                "source_modified": {"type": "boolean"},
            },
            ["file", "lines"],
        ),
    },
    {
        "name": "debug_data_breakpoint_info",
        "description": "Resolve a visible variable to a daScript hardware data-breakpoint id.",
        "inputSchema": _schema(
            {
                "variables_reference": {"type": "integer", "minimum": 1},
                "name": {"type": "string"},
            },
            ["variables_reference", "name"],
        ),
    },
    {
        "name": "debug_set_data_breakpoints",
        "description": "Replace data breakpoints; pass an empty array to clear them.",
        "inputSchema": _schema(
            {
                "breakpoints": {
                    "type": "array",
                    "items": {
                        "type": "object",
                        "properties": {
                            "data_id": {"type": "string"},
                            "access_type": {"type": "string"},
                            "condition": {"type": "string"},
                            "hit_condition": {"type": "string"},
                            "description": {"type": "string"},
                            "enabled": {"type": "boolean"},
                        },
                        "required": ["data_id"],
                        "additionalProperties": False,
                    },
                }
            },
            ["breakpoints"],
        ),
    },
    {
        "name": "debug_configuration_done",
        "description": "Tell DAP that breakpoints and startup configuration are complete.",
        "inputSchema": _schema(),
    },
    {"name": "debug_threads", "description": "Return debuggee threads/contexts.", "inputSchema": _schema()},
    {
        "name": "debug_stack_trace",
        "description": "Return the call stack of a DAP thread.",
        "inputSchema": _schema(
            {
                **THREAD_ID,
                "start_frame": {"type": "integer", "minimum": 0},
                "levels": {"type": "integer", "minimum": 1},
            },
            ["thread_id"],
        ),
    },
    {
        "name": "debug_scopes",
        "description": "Return locals, arguments, state, and globals scopes for a stack frame.",
        "inputSchema": _schema(
            {"frame_id": {"type": "integer", "minimum": 1}}, ["frame_id"]
        ),
    },
    {
        "name": "debug_variables",
        "description": "Expand a DAP variables reference.",
        "inputSchema": _schema(
            {
                "variables_reference": {"type": "integer", "minimum": 1},
                "start": {"type": "integer", "minimum": 0},
                "count": {"type": "integer", "minimum": 1},
            },
            ["variables_reference"],
        ),
    },
    {
        "name": "debug_evaluate",
        "description": "Evaluate an expression in a paused stack frame.",
        "inputSchema": _schema(
            {
                "expression": {"type": "string"},
                "frame_id": {"type": "integer", "minimum": 1},
                "context": {"type": "string"},
            },
            ["expression", "frame_id"],
        ),
    },
    {
        "name": "debug_continue",
        "description": "Continue a paused thread.",
        "inputSchema": _schema(THREAD_ID, ["thread_id"]),
    },
    {
        "name": "debug_pause",
        "description": "Request a running thread to pause.",
        "inputSchema": _schema(THREAD_ID, ["thread_id"]),
    },
    {
        "name": "debug_step_in",
        "description": "Step into the next call or statement.",
        "inputSchema": _schema(THREAD_ID, ["thread_id"]),
    },
    {
        "name": "debug_step_over",
        "description": "Step over the next statement (DAP next).",
        "inputSchema": _schema(THREAD_ID, ["thread_id"]),
    },
    {
        "name": "debug_step_out",
        "description": "Continue until the current function returns.",
        "inputSchema": _schema(THREAD_ID, ["thread_id"]),
    },
    {
        "name": "debug_terminate",
        "description": "Terminate the debuggee through DAP.",
        "inputSchema": _schema(),
    },
    {
        "name": "debug_disconnect",
        "description": "Disconnect DAP; daScript resumes threads and clears source breakpoints.",
        "inputSchema": _schema(),
    },
    {
        "name": "debug_wait_event",
        "description": "Wait for the next DAP event, optionally filtering by event name.",
        "inputSchema": _schema(
            {
                "event": {"type": "string"},
                "timeout_sec": {"type": "number", "minimum": 0, "maximum": 3600},
            }
        ),
    },
]


class DapBridge:
    def __init__(
        self,
        repo_root: Path,
        timeout: float,
        executable: Path | None = None,
    ) -> None:
        self.repo_root = repo_root.resolve()
        self.timeout = timeout
        self.executable = executable.resolve() if executable is not None else None
        self.dap = DapClient(timeout)
        self.process: subprocess.Popen[bytes] | None = None
        self.process_lock = threading.Lock()
        self.process_output: deque[str] = deque(maxlen=OUTPUT_LIMIT)
        self.process_threads: list[threading.Thread] = []
        self.process_generation = 0

    def _available_local_port(self, host: str) -> int:
        bind_host = "127.0.0.1" if host == "localhost" else host
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
            sock.bind((bind_host, 0))
            return int(sock.getsockname()[1])

    def _ensure_local_port_available(self, host: str, port: int) -> None:
        bind_host = "127.0.0.1" if host == "localhost" else host
        try:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
                sock.bind((bind_host, port))
        except OSError as error:
            raise BridgeError(
                f"DAP launch port {host}:{port} is unavailable: {error}"
            ) from error

    def _wait_owned_process(self, timeout: float) -> int | None:
        with self.process_lock:
            process = self.process
            generation = self.process_generation
            process_threads = list(self.process_threads)
        if process is None:
            return None
        try:
            return_code = process.wait(timeout=timeout)
        except subprocess.TimeoutExpired:
            return_code = process.poll()
        if return_code is not None:
            for thread in process_threads:
                if thread is not threading.current_thread():
                    thread.join(timeout=1.0)
            with self.process_lock:
                if process is not self.process or generation != self.process_generation:
                    return return_code
        return return_code

    def _session_snapshot(self) -> dict[str, Any]:
        self._wait_owned_process(0)
        with self.process_lock:
            process = self.process
            output_tail = list(self.process_output)
        endpoint = self.dap.endpoint or self.dap.last_endpoint
        return_code = process.poll() if process is not None else None
        return {
            "connected": self.dap.connected,
            "host": endpoint[0] if endpoint is not None else None,
            "port": endpoint[1] if endpoint is not None else None,
            "pid": process.pid if process is not None else None,
            "return_code": return_code,
            "close_reason": self.dap.close_reason,
            "last_dap_termination": self.dap.termination_body(),
            "recent_dap_events": self.dap.history(),
            "process_output_tail": output_tail,
        }

    def error_text(self, error: BaseException) -> str:
        snapshot = self._session_snapshot()
        if snapshot["host"] is None and snapshot["pid"] is None:
            return str(error)
        return f"{error}; session: {json.dumps(snapshot, ensure_ascii=False)}"

    def _path(self, value: Any, *, must_exist: bool = True) -> Path:
        if not isinstance(value, str) or not value:
            raise BridgeError("path must be a non-empty string")
        path = Path(value).expanduser()
        if not path.is_absolute():
            path = self.repo_root / path
        path = path.resolve()
        if must_exist and not path.exists():
            raise BridgeError(f"path does not exist: {path}")
        return path

    def _host_port_timeout(self, arguments: dict[str, Any]) -> tuple[str, int, float]:
        host = arguments.get("host", DEFAULT_HOST)
        port = arguments.get("port", DEFAULT_PORT)
        timeout = arguments.get("timeout_sec", min(self.timeout, 30.0))
        if not isinstance(host, str) or not host:
            raise BridgeError("host must be a non-empty string")
        if not isinstance(port, int) or isinstance(port, bool) or not 1 <= port <= 65535:
            raise BridgeError("port must be an integer from 1 to 65535")
        if not isinstance(timeout, (int, float)) or isinstance(timeout, bool) or timeout <= 0:
            raise BridgeError("timeout_sec must be positive")
        return host, port, float(timeout)

    def _integer(self, arguments: dict[str, Any], name: str, minimum: int = 0) -> int:
        value = arguments.get(name)
        if not isinstance(value, int) or isinstance(value, bool) or value < minimum:
            raise BridgeError(f"{name} must be an integer >= {minimum}")
        return value

    def _session_settings(self, arguments: dict[str, Any]) -> dict[str, Any]:
        result: dict[str, Any] = {}
        if "cwd" in arguments:
            result["cwd"] = str(self._path(arguments["cwd"]))
        if "paths" in arguments:
            paths = arguments["paths"]
            if not isinstance(paths, list) or not all(isinstance(item, str) for item in paths):
                raise BridgeError("paths must be an array of strings")
            result["paths"] = [str(self._path(item)) for item in paths]
        if "path_aliases" in arguments:
            aliases = arguments["path_aliases"]
            if not isinstance(aliases, dict) or not all(
                isinstance(key, str) and isinstance(value, str)
                for key, value in aliases.items()
            ):
                raise BridgeError("path_aliases must map strings to strings")
            result["pathAliases"] = aliases
        for source, target in (
            ("inline_preview_limit", "inlinePreviewLimit"),
            ("max_children_count", "maxChildrenCount"),
        ):
            if source in arguments:
                value = arguments[source]
                if not isinstance(value, int) or isinstance(value, bool) or value < 0:
                    raise BridgeError(f"{source} must be a non-negative integer")
                result[target] = value
        if "collect_all_globals" in arguments:
            value = arguments["collect_all_globals"]
            if not isinstance(value, bool):
                raise BridgeError("collect_all_globals must be a boolean")
            result["collectAllGlobals"] = value
        return result

    def _pick_executable(self, requested: Any) -> Path:
        if requested is None and self.executable is not None:
            requested = str(self.executable)
        if requested is not None:
            executable = self._path(requested)
            if not executable.is_file() or not os.access(executable, os.X_OK):
                raise BridgeError(f"not an executable file: {executable}")
            return executable
        candidates = [
            self.repo_root / "bin" / "daslang",
            self.repo_root / "build" / "daslang",
            self.repo_root / "bin" / "Release" / "daslang",
            self.repo_root / "bin" / "Release" / "daslang.exe",
        ]
        usable = [path for path in candidates if path.is_file() and os.access(path, os.X_OK)]
        if not usable:
            raise BridgeError("no daslang executable found; pass executable explicitly")
        return max(usable, key=lambda path: path.stat().st_mtime)

    def _pump_output(self, stream: BinaryIO, category: str, generation: int) -> None:
        try:
            for raw_line in iter(stream.readline, b""):
                line = raw_line.decode("utf-8", errors="replace").rstrip("\r\n")
                with self.process_lock:
                    if generation == self.process_generation:
                        self.process_output.append(f"{category}: {line}")
        finally:
            stream.close()

    def _monitor_process(
        self,
        process: subprocess.Popen[bytes],
        generation: int,
        output_threads: list[threading.Thread],
    ) -> None:
        exit_code = process.wait()
        for thread in output_threads:
            thread.join(timeout=1.0)
        with self.process_lock:
            current = process is self.process and generation == self.process_generation
        if not current:
            return
        self.dap.enqueue_event(
            "terminated",
            {"exitCode": exit_code, "reason": "process_exit", "bridgeSynthetic": True},
        )

    def _spawn(self, command: list[str], cwd: Path) -> subprocess.Popen[bytes]:
        with self.process_lock:
            if self.process is not None and self.process.poll() is None:
                raise BridgeError(f"bridge already owns running debuggee pid {self.process.pid}")
            old_threads = list(self.process_threads[:2])
        for thread in old_threads:
            thread.join(timeout=1.0)
        with self.process_lock:
            self.process_generation += 1
            generation = self.process_generation
            self.process_output.clear()
        kwargs: dict[str, Any] = {}
        if os.name == "posix":
            kwargs["start_new_session"] = True
        elif os.name == "nt":
            kwargs["creationflags"] = subprocess.CREATE_NEW_PROCESS_GROUP
        process = subprocess.Popen(
            command,
            cwd=cwd,
            stdin=subprocess.DEVNULL,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            **kwargs,
        )
        with self.process_lock:
            self.process = process
            self.process_threads = []
        output_threads: list[threading.Thread] = []
        for stream, category in ((process.stdout, "stdout"), (process.stderr, "stderr")):
            assert stream is not None
            thread = threading.Thread(
                target=self._pump_output,
                args=(stream, category, generation),
                name=f"daslang-debuggee-{category}",
                daemon=True,
            )
            thread.start()
            output_threads.append(thread)
        monitor = threading.Thread(
            target=self._monitor_process,
            args=(process, generation, output_threads),
            name="daslang-debuggee-monitor",
            daemon=True,
        )
        with self.process_lock:
            self.process_threads = [*output_threads, monitor]
        monitor.start()
        return process

    def _output_tail(self) -> list[str]:
        with self.process_lock:
            return list(self.process_output)

    def _terminate_owned(self) -> None:
        with self.process_lock:
            process = self.process
        if process is None:
            return
        if process.poll() is not None:
            self._wait_owned_process(0)
            return
        try:
            if os.name == "posix":
                os.killpg(process.pid, signal.SIGTERM)
            else:
                process.terminate()
            process.wait(timeout=3.0)
        except subprocess.TimeoutExpired:
            if os.name == "posix":
                os.killpg(process.pid, signal.SIGKILL)
            else:
                process.kill()
            process.wait(timeout=3.0)
        except ProcessLookupError:
            pass
        self._wait_owned_process(0)

    def _launch(self, arguments: dict[str, Any]) -> dict[str, Any]:
        file_path = self._path(arguments.get("file"))
        executable = self._pick_executable(arguments.get("executable"))
        host, port, connect_timeout = self._host_port_timeout(arguments)
        if host not in ("127.0.0.1", "localhost"):
            raise BridgeError("debug_launch supports only the IPv4 loopback host")
        if "port" not in arguments:
            port = self._available_local_port(host)
        else:
            self._ensure_local_port_available(host, port)
        cwd = self._path(arguments.get("cwd", str(self.repo_root)))
        command = [str(executable), "--das-wait-debugger"]
        if "project" in arguments:
            command.extend(["-project", str(self._path(arguments["project"]))])
        if "project_root" in arguments:
            command.extend(["-project_root", str(self._path(arguments["project_root"]))])
        load_modules = arguments.get("load_modules", [])
        if not isinstance(load_modules, list) or not all(
            isinstance(item, str) for item in load_modules
        ):
            raise BridgeError("load_modules must be an array of strings")
        for module in load_modules:
            command.extend(["-load_module", str(self._path(module))])
        program_args = arguments.get("program_args", [])
        if not isinstance(program_args, list) or not all(
            isinstance(item, str) for item in program_args
        ):
            raise BridgeError("program_args must be an array of strings")
        stepping_debugger = arguments.get("stepping_debugger", False)
        if not isinstance(stepping_debugger, bool):
            raise BridgeError("stepping_debugger must be a boolean")
        debugger_args = ["--das-debug-port", str(port)]
        if stepping_debugger:
            debugger_args.append("--das-stepping-debugger")
        command.extend([str(file_path), "--", *debugger_args, *program_args])

        if self.dap.connected:
            raise BridgeError("a DAP session is already connected; disconnect it first")
        process = self._spawn(command, cwd)
        try:
            connection = self.dap.connect(host, port, connect_timeout)
            if process.poll() is not None:
                raise BridgeError(f"debuggee exited before DAP initialization with {process.returncode}")
            initialized = self.dap.initialize(connect_timeout)
            session_settings = self._session_settings(arguments)
            session_settings.setdefault("cwd", str(cwd))
            launched = self.dap.request("launch", session_settings)
            initialized_event = self.dap.wait_event("initialized", min(connect_timeout, 10.0))
            if process.poll() is not None:
                raise BridgeError(f"debuggee exited during DAP launch with {process.returncode}")
        except BaseException as error:
            self.dap.close()
            self._terminate_owned()
            tail = self._output_tail()
            detail = f"; debuggee output: {tail}" if tail else ""
            raise BridgeError(f"failed to launch debug session: {error}{detail}") from error
        return {
            "pid": process.pid,
            "command": command,
            "connection": connection,
            "initialize": initialized,
            "launch": launched,
            "initialized_event": initialized_event,
        }

    def _attach(self, arguments: dict[str, Any]) -> dict[str, Any]:
        with self.process_lock:
            if self.process is not None and self.process.poll() is None:
                raise BridgeError(
                    f"bridge already owns running debuggee pid {self.process.pid}"
                )
            self.process_generation += 1
            self.process = None
            self.process_threads = []
            self.process_output.clear()
        host, port, timeout = self._host_port_timeout(arguments)
        connection = self.dap.connect(host, port, timeout)
        initialized = self.dap.initialize(timeout)
        attached = self.dap.request("attach", self._session_settings(arguments))
        initialized_event = self.dap.wait_event("initialized", min(timeout, 10.0))
        return {
            "connection": connection,
            "initialize": initialized,
            "attach": attached,
            "initialized_event": initialized_event,
        }

    def _resume(self, command: str, arguments: dict[str, Any]) -> dict[str, Any]:
        thread_id = self._integer(arguments, "thread_id", 1)
        result = self.dap.request(command, {"threadId": thread_id})
        self.dap.enqueue_event(
            "continued",
            {
                "threadId": thread_id,
                "allThreadsContinued": False,
                "bridgeSynthetic": True,
                "command": command,
            },
        )
        return result

    def call_tool(self, name: str, arguments: dict[str, Any]) -> Any:
        if not isinstance(arguments, dict):
            raise BridgeError("tool arguments must be an object")
        if name == "debug_connect":
            host, port, timeout = self._host_port_timeout(arguments)
            return self.dap.connect(host, port, timeout)
        if name == "debug_initialize":
            return self.dap.initialize()
        if name == "debug_launch":
            return self._launch(arguments)
        if name == "debug_attach":
            return self._attach(arguments)
        if name == "debug_set_breakpoints":
            file_path = self._path(arguments.get("file"))
            lines = arguments.get("lines")
            if not isinstance(lines, list) or not all(
                isinstance(line, int) and not isinstance(line, bool) and line > 0
                for line in lines
            ):
                raise BridgeError("lines must be an array of positive integers")
            source_modified = arguments.get("source_modified", False)
            if not isinstance(source_modified, bool):
                raise BridgeError("source_modified must be a boolean")
            params = {
                "source": {"name": file_path.name, "path": str(file_path)},
                "breakpoints": [{"line": line} for line in lines],
                "sourceModified": source_modified,
            }
            return self.dap.request("setBreakpoints", params)
        if name == "debug_data_breakpoint_info":
            reference = self._integer(arguments, "variables_reference", 1)
            variable_name = arguments.get("name")
            if not isinstance(variable_name, str) or not variable_name:
                raise BridgeError("name must be a non-empty string")
            return self.dap.request(
                "dataBreakpointInfo",
                {"variablesReference": reference, "name": variable_name},
            )
        if name == "debug_set_data_breakpoints":
            breakpoints = arguments.get("breakpoints")
            if not isinstance(breakpoints, list):
                raise BridgeError("breakpoints must be an array")
            converted = []
            for item in breakpoints:
                if not isinstance(item, dict) or not isinstance(item.get("data_id"), str):
                    raise BridgeError("each data breakpoint needs a string data_id")
                converted.append(
                    {
                        "dataId": item["data_id"],
                        "accessType": item.get("access_type", "write"),
                        "condition": item.get("condition", ""),
                        "hitCondition": item.get("hit_condition", ""),
                        "description": item.get("description", ""),
                        "enabled": item.get("enabled", True),
                    }
                )
            return self.dap.request("setDataBreakpoints", {"breakpoints": converted})
        if name == "debug_configuration_done":
            return self.dap.request("configurationDone", {})
        if name == "debug_threads":
            return self.dap.request("threads", {})
        if name == "debug_stack_trace":
            thread_id = self._integer(arguments, "thread_id", 1)
            start_frame = arguments.get("start_frame", 0)
            levels = arguments.get("levels", 1000)
            if not isinstance(start_frame, int) or isinstance(start_frame, bool) or start_frame < 0:
                raise BridgeError("start_frame must be a non-negative integer")
            if not isinstance(levels, int) or isinstance(levels, bool) or levels < 1:
                raise BridgeError("levels must be a positive integer")
            return self.dap.request(
                "stackTrace",
                {"threadId": thread_id, "startFrame": start_frame, "levels": levels},
            )
        if name == "debug_scopes":
            return self.dap.request(
                "scopes", {"frameId": self._integer(arguments, "frame_id", 1)}
            )
        if name == "debug_variables":
            params = {
                "variablesReference": self._integer(arguments, "variables_reference", 1)
            }
            if "start" in arguments:
                params["start"] = self._integer(arguments, "start", 0)
            if "count" in arguments:
                params["count"] = self._integer(arguments, "count", 1)
            return self.dap.request("variables", params)
        if name == "debug_evaluate":
            expression = arguments.get("expression")
            if not isinstance(expression, str) or not expression:
                raise BridgeError("expression must be a non-empty string")
            context = arguments.get("context", "repl")
            if not isinstance(context, str):
                raise BridgeError("context must be a string")
            return self.dap.request(
                "evaluate",
                {
                    "expression": expression,
                    "frameId": self._integer(arguments, "frame_id", 1),
                    "context": context,
                },
            )
        if name == "debug_continue":
            return self._resume("continue", arguments)
        if name == "debug_pause":
            return self.dap.request(
                "pause", {"threadId": self._integer(arguments, "thread_id", 1)}
            )
        if name == "debug_step_in":
            return self._resume("stepIn", arguments)
        if name == "debug_step_over":
            return self._resume("next", arguments)
        if name == "debug_step_out":
            return self._resume("stepOut", arguments)
        if name == "debug_terminate":
            result = self.dap.request("terminate", {}, timeout=min(self.timeout, 10.0))
            return {**result, "process_output_tail": self._output_tail()}
        if name == "debug_disconnect":
            if not self.dap.connected:
                self._wait_owned_process(min(self.timeout, 1.0))
                self.dap.close()
                return {
                    "command": "disconnect",
                    "success": True,
                    "already_disconnected": True,
                    "session": self._session_snapshot(),
                }
            try:
                result = self.dap.request("disconnect", {})
            except BridgeError as error:
                if self.dap.connected:
                    raise
                self.dap.close()
                self._wait_owned_process(min(self.timeout, 1.0))
                return {
                    "command": "disconnect",
                    "success": True,
                    "already_disconnected": True,
                    "disconnect_error": str(error),
                    "session": self._session_snapshot(),
                }
            self.dap.close("disconnect_request")
            self._wait_owned_process(min(self.timeout, 3.0))
            return {**result, "session": self._session_snapshot()}
        if name == "debug_wait_event":
            event_name = arguments.get("event")
            if event_name is not None and not isinstance(event_name, str):
                raise BridgeError("event must be a string")
            timeout = arguments.get("timeout_sec", min(self.timeout, 30.0))
            if not isinstance(timeout, (int, float)) or isinstance(timeout, bool) or timeout < 0:
                raise BridgeError("timeout_sec must be non-negative")
            result = self.dap.wait_event(event_name, float(timeout))
            if result.get("event") == "terminated":
                self._wait_owned_process(min(self.timeout, 1.0))
                body = result.setdefault("body", {})
                if isinstance(body, dict):
                    body.update(self.dap.termination_body())
                    body["session"] = self._session_snapshot()
            if result.get("event") is None:
                body = result.setdefault("body", {})
                if isinstance(body, dict):
                    body["processOutputTail"] = self._output_tail()
            return result
        raise BridgeError(f"unknown tool: {name}")

    def close(self) -> None:
        self.dap.close()
        self._terminate_owned()


def _tool_result(value: Any, is_error: bool = False) -> dict[str, Any]:
    text = value if isinstance(value, str) else json.dumps(value, ensure_ascii=False, indent=2)
    result: dict[str, Any] = {"content": [{"type": "text", "text": text}]}
    if is_error:
        result["isError"] = True
    return result


def _write_mcp_message(message: dict[str, Any]) -> None:
    sys.stdout.write(json.dumps(message, ensure_ascii=False, separators=(",", ":")) + "\n")
    sys.stdout.flush()


def serve(bridge: DapBridge) -> int:
    for line in sys.stdin:
        try:
            request = json.loads(line)
            if not isinstance(request, dict):
                raise ValueError("request must be an object")
        except (json.JSONDecodeError, ValueError) as error:
            _write_mcp_message(
                {"jsonrpc": "2.0", "id": None, "error": {"code": -32700, "message": str(error)}}
            )
            continue
        request_id = request.get("id")
        method = request.get("method")
        params = request.get("params") or {}
        if request_id is None:
            if method == "exit":
                break
            continue
        try:
            if method == "initialize":
                result = {
                    "protocolVersion": MCP_PROTOCOL_VERSION,
                    "capabilities": {"tools": {}},
                    "serverInfo": {"name": "daslang-dap", "version": "0.1.0"},
                    "instructions": (
                        "Stateful daScript DAP bridge. For debug_launch: set breakpoints, call "
                        "debug_threads, then debug_configuration_done. DAP ids returned by one "
                        "stop are inputs to stack/scopes/variables/evaluate tools."
                    ),
                }
            elif method == "ping":
                result = {}
            elif method == "tools/list":
                result = {"tools": TOOLS}
            elif method == "tools/call":
                name = params.get("name")
                arguments = params.get("arguments") or {}
                if not isinstance(name, str):
                    raise BridgeError("tool name must be a string")
                try:
                    result = _tool_result(bridge.call_tool(name, arguments))
                except Exception as error:
                    result = _tool_result(bridge.error_text(error), is_error=True)
            elif method == "shutdown":
                result = None
            else:
                raise KeyError(method)
            _write_mcp_message({"jsonrpc": "2.0", "id": request_id, "result": result})
        except KeyError:
            _write_mcp_message(
                {
                    "jsonrpc": "2.0",
                    "id": request_id,
                    "error": {"code": -32601, "message": f"method not found: {method}"},
                }
            )
        except BaseException as error:
            _write_mcp_message(
                {
                    "jsonrpc": "2.0",
                    "id": request_id,
                    "error": {"code": -32603, "message": str(error)},
                }
            )
    return 0


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--repo-root",
        type=Path,
        default=Path(__file__).resolve().parents[2],
        help="workspace root whose .das programs are debugged",
    )
    parser.add_argument(
        "--executable",
        type=Path,
        help="default daslang executable for debug_launch",
    )
    parser.add_argument(
        "--timeout", type=float, default=DEFAULT_TIMEOUT, help="DAP request timeout"
    )
    return parser.parse_args()


def main() -> int:
    arguments = parse_args()
    bridge = DapBridge(
        arguments.repo_root,
        arguments.timeout,
        executable=arguments.executable,
    )
    try:
        return serve(bridge)
    finally:
        bridge.close()


if __name__ == "__main__":
    raise SystemExit(main())
