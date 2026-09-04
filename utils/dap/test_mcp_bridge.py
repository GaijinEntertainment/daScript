#!/usr/bin/env python3
"""End-to-end smoke test for the daScript DAP-to-MCP bridge."""

from __future__ import annotations

import errno
import io
import json
import os
import signal
import socket
import subprocess
import sys
import tempfile
from pathlib import Path
from typing import Any

from mcp_bridge import BridgeError, DapBridge, _read_dap_frame


ROOT = Path(__file__).resolve().parents[2]
BRIDGE = ROOT / "utils" / "dap" / "mcp_bridge.py"
FIXTURE = ROOT / "utils" / "dap" / "_fixture.das"
DASLANG = Path(os.environ.get("DASLANG", ROOT / "bin" / "daslang")).resolve()
STEPPING_OVERRIDE = os.environ.get("DAS_TEST_STEPPING")
STEPPING_DEBUGGER = (
    STEPPING_OVERRIDE == "1" if STEPPING_OVERRIDE is not None else False
)


def stepping_arguments() -> dict[str, bool]:
    if STEPPING_OVERRIDE is None:
        return {}
    return {"stepping_debugger": STEPPING_DEBUGGER}


class McpClient:
    def __init__(self) -> None:
        environment = os.environ.copy()
        environment["PYTHONDONTWRITEBYTECODE"] = "1"
        self.process = subprocess.Popen(
            [
                sys.executable,
                str(BRIDGE),
                "--repo-root",
                str(ROOT),
                "--executable",
                str(DASLANG),
            ],
            cwd=ROOT,
            env=environment,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            bufsize=1,
        )
        self.next_id = 1

    def request(self, method: str, params: dict[str, Any] | None = None) -> Any:
        assert self.process.stdin is not None and self.process.stdout is not None
        request_id = self.next_id
        self.next_id += 1
        message: dict[str, Any] = {"jsonrpc": "2.0", "id": request_id, "method": method}
        if params is not None:
            message["params"] = params
        self.process.stdin.write(json.dumps(message, separators=(",", ":")) + "\n")
        self.process.stdin.flush()
        line = self.process.stdout.readline()
        if not line:
            stderr = self.process.stderr.read() if self.process.stderr is not None else ""
            raise AssertionError(
                f"MCP bridge exited with {self.process.poll()} while handling {method}: {stderr}"
            )
        response = json.loads(line)
        assert response.get("id") == request_id, response
        assert "error" not in response, response
        return response.get("result")

    def tool(self, name: str, arguments: dict[str, Any]) -> Any:
        result = self.request("tools/call", {"name": name, "arguments": arguments})
        assert not result.get("isError"), (name, result)
        content = result.get("content")
        assert isinstance(content, list) and content, result
        return json.loads(content[0]["text"])

    def tool_error(self, name: str, arguments: dict[str, Any]) -> str:
        result = self.request("tools/call", {"name": name, "arguments": arguments})
        assert result.get("isError"), (name, result)
        content = result.get("content")
        assert isinstance(content, list) and content, result
        return content[0]["text"]

    def close(self) -> None:
        if self.process.poll() is None:
            try:
                self.request("shutdown")
            except (AssertionError, BrokenPipeError):
                pass
        if self.process.stdin is not None:
            self.process.stdin.close()
        try:
            self.process.wait(timeout=5.0)
        except subprocess.TimeoutExpired:
            self.process.terminate()
            self.process.wait(timeout=5.0)


def free_port() -> int:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.bind(("127.0.0.1", 0))
        return int(sock.getsockname()[1])


def source_line(needle: str) -> int:
    lines = FIXTURE.read_text(encoding="utf-8").splitlines()
    return next(index for index, line in enumerate(lines, start=1) if needle in line)


def response_body(value: dict[str, Any]) -> dict[str, Any]:
    body = value.get("body")
    assert isinstance(body, dict), value
    return body


def test_dap_frame_limits() -> None:
    invalid_frames = (
        (b"Content-Length: 10", "truncated DAP header line"),
        (b"Broken\r\n\r\n", "invalid DAP header"),
        (b"X-Test: value\r\n\r\n", "missing or invalid DAP Content-Length"),
        (b"Content-Length: nope\r\n\r\n", "missing or invalid DAP Content-Length"),
        (b"Content-Length: -1\r\n\r\n", "DAP Content-Length is outside"),
        (b"X-Long: " + b"x" * 8192 + b"\r\n\r\n", "DAP header line is too long"),
        ((b"X-Many: " + b"x" * 8170 + b"\r\n") * 9 + b"\r\n", "DAP headers are too large"),
        (b"Content-Length: 67108865\r\n\r\n", "DAP Content-Length is outside"),
        (b"Content-Length: 4\r\n\r\n{}", "truncated DAP payload"),
        (b"Content-Length: 1\r\n\r\n1", "DAP payload must be a JSON object"),
    )
    for frame, expected_error in invalid_frames:
        try:
            _read_dap_frame(io.BytesIO(frame))
        except BridgeError as error:
            assert expected_error in str(error), (expected_error, str(error))
            continue
        raise AssertionError(f"invalid DAP frame was accepted: {frame[:80]!r}")


def launch_external(port: int) -> subprocess.Popen[bytes]:
    command = [
        str(DASLANG),
        "--das-wait-debugger",
        str(FIXTURE),
        "--",
        "--das-debug-port",
        str(port),
    ]
    if STEPPING_DEBUGGER:
        command.append("--das-stepping-debugger")
    return subprocess.Popen(
        command,
        cwd=ROOT,
        stdin=subprocess.DEVNULL,
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL,
    )


def test_waiting_worker_shutdown() -> None:
    fixture = ROOT / "utils" / "dap" / "_fixture_cancel.das"
    completed = subprocess.run(
        [str(DASLANG), str(fixture)],
        cwd=ROOT,
        stdin=subprocess.DEVNULL,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        timeout=10,
        check=False,
    )
    assert completed.returncode == 0, completed.stdout
    assert "DAP_CANCEL_READY" in completed.stdout, completed.stdout
    assert "cancelled debugger worker ran" not in completed.stdout, completed.stdout


def test_callback_without_threadlock() -> None:
    fixture = ROOT / "utils" / "dap" / "_fixture_callback_no_threadlock.das"
    completed = subprocess.run(
        [str(DASLANG), str(fixture)],
        cwd=ROOT,
        stdin=subprocess.DEVNULL,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        timeout=10,
        check=False,
    )
    assert completed.returncode == 0, completed.stdout
    assert "DAP_CALLBACK_NO_THREADLOCK=1" in completed.stdout, completed.stdout


def test_waiting_worker_context_destroyed() -> None:
    fixture = ROOT / "utils" / "dap" / "_fixture_context_destroyed.das"
    with tempfile.TemporaryDirectory(prefix="das-dap-destroy-") as temp_dir:
        completed = subprocess.run(
            [str(DASLANG), str(fixture), "--", f"{temp_dir}/done"],
            cwd=ROOT,
            stdin=subprocess.DEVNULL,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
            timeout=10,
            check=False,
        )
    assert completed.returncode == 0, completed.stdout
    assert "destroyed context worker ran" not in completed.stdout, completed.stdout
    assert "DAP_CONTEXT_DESTROY_RECOVERED=1" in completed.stdout, completed.stdout


def test_repeated_worker_lifecycle() -> None:
    fixture = ROOT / "utils" / "dap" / "_fixture_lifecycle.das"
    with tempfile.TemporaryDirectory(prefix="das-dap-lifecycle-") as temp_dir:
        completed = subprocess.run(
            [
                str(DASLANG),
                str(fixture),
                "--",
                f"{temp_dir}/first",
                f"{temp_dir}/second",
            ],
            cwd=ROOT,
            stdin=subprocess.DEVNULL,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
            timeout=10,
            check=False,
        )
    assert completed.returncode == 0, completed.stdout
    assert "DAP_LIFECYCLE_WORKER=1" in completed.stdout, completed.stdout
    assert "DAP_LIFECYCLE_WORKER=2" in completed.stdout, completed.stdout
    assert "DAP_LIFECYCLE=2" in completed.stdout, completed.stdout


def test_duplicate_worker_rejected() -> None:
    fixture = ROOT / "utils" / "dap" / "_fixture_duplicate.das"
    completed = subprocess.run(
        [str(DASLANG), str(fixture)],
        cwd=ROOT,
        stdin=subprocess.DEVNULL,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        timeout=10,
        check=False,
    )
    assert completed.returncode != 0, completed.stdout
    assert "debugger thread is already active" in completed.stdout, completed.stdout


class RecordingDap:
    def __init__(self) -> None:
        self.commands: list[str] = []

    def request(self, command: str, arguments: dict[str, Any]) -> dict[str, Any]:
        self.commands.append(command)
        return {"command": command, "success": True, "body": arguments}

    def enqueue_event(self, _event: str, _body: dict[str, Any]) -> None:
        pass


def test_execution_command_mapping() -> None:
    bridge = DapBridge(ROOT, 1.0, DASLANG)
    recorder = RecordingDap()
    bridge.dap = recorder
    expected = {
        "debug_pause": "pause",
        "debug_step_in": "stepIn",
        "debug_step_over": "next",
        "debug_step_out": "stepOut",
    }
    for tool, command in expected.items():
        bridge.call_tool(tool, {"thread_id": 1})
        assert recorder.commands[-1] == command, (tool, recorder.commands)


def main() -> int:
    test_dap_frame_limits()
    test_waiting_worker_shutdown()
    test_callback_without_threadlock()
    test_waiting_worker_context_destroyed()
    test_repeated_worker_lifecycle()
    test_duplicate_worker_rejected()
    test_execution_command_mapping()
    client = McpClient()
    external_processes: list[subprocess.Popen[bytes]] = []
    try:
        initialized = client.request(
            "initialize",
            {
                "protocolVersion": "2025-11-25",
                "capabilities": {},
                "clientInfo": {"name": "test-dap-mcp-bridge", "version": "0"},
            },
        )
        assert initialized["serverInfo"]["name"] == "daslang-dap", initialized

        listed = client.request("tools/list")
        names = [tool["name"] for tool in listed["tools"]]
        assert names == [
            "debug_connect",
            "debug_initialize",
            "debug_launch",
            "debug_attach",
            "debug_set_breakpoints",
            "debug_data_breakpoint_info",
            "debug_set_data_breakpoints",
            "debug_configuration_done",
            "debug_threads",
            "debug_stack_trace",
            "debug_scopes",
            "debug_variables",
            "debug_evaluate",
            "debug_continue",
            "debug_pause",
            "debug_step_in",
            "debug_step_over",
            "debug_step_out",
            "debug_terminate",
            "debug_disconnect",
            "debug_wait_event",
        ], names

        occupied_port_guard = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            occupied_port_guard.bind(("127.0.0.1", 0))
            occupied_port = int(occupied_port_guard.getsockname()[1])
            occupied_port_guard.listen(1)
            launch_error = client.tool_error(
                "debug_launch",
                {"file": str(FIXTURE), "port": occupied_port, "timeout_sec": 2},
            )
            assert "is unavailable" in launch_error, launch_error
        finally:
            occupied_port_guard.close()

        ipv6_error = client.tool_error(
            "debug_launch",
            {"file": str(FIXTURE), "host": "::1", "timeout_sec": 2},
        )
        assert "IPv4 loopback" in ipv6_error, ipv6_error

        port = free_port()
        launch = client.tool(
            "debug_launch",
            {
                "file": str(FIXTURE),
                "port": port,
                "timeout_sec": 20,
                **stepping_arguments(),
            },
        )
        assert launch["initialize"]["body"]["supportsDataBreakpoints"], launch
        assert launch["initialized_event"]["event"] == "initialized", launch

        early_breakpoint_line = source_line("var result = value + 1")
        breakpoint_line = source_line("guard += 1")
        breakpoint_lines = [early_breakpoint_line, breakpoint_line]
        breakpoints = response_body(
            client.tool(
                "debug_set_breakpoints",
                {"file": str(FIXTURE), "lines": breakpoint_lines},
            )
        )["breakpoints"]
        assert len(breakpoints) == len(breakpoint_lines), breakpoints
        assert all(
            item["verified"] == STEPPING_DEBUGGER for item in breakpoints
        ), breakpoints

        initial_threads = response_body(client.tool("debug_threads", {}))["threads"]
        assert initial_threads, initial_threads
        configured = client.tool("debug_configuration_done", {})
        assert configured["success"], configured
        stopped = client.tool(
            "debug_wait_event", {"event": "stopped", "timeout_sec": 20}
        )
        assert stopped["event"] == "stopped", stopped
        thread_id = int(stopped["body"]["threadId"])
        early_stack = response_body(
            client.tool("debug_stack_trace", {"thread_id": thread_id, "levels": 1})
        )["stackFrames"]
        expected_first_line = breakpoint_line if STEPPING_DEBUGGER else early_breakpoint_line
        assert early_stack and early_stack[0]["line"] == expected_first_line, early_stack
        if not STEPPING_DEBUGGER:
            client.tool("debug_continue", {"thread_id": thread_id})
            stopped = client.tool(
                "debug_wait_event", {"event": "stopped", "timeout_sec": 20}
            )
            assert stopped["event"] == "stopped", stopped
            thread_id = int(stopped["body"]["threadId"])

        changed_breakpoints = response_body(
            client.tool(
                "debug_set_breakpoints",
                {
                    "file": str(FIXTURE),
                    "lines": [breakpoint_line, source_line("sleep(1u)")],
                },
            )
        )["breakpoints"]
        assert len(changed_breakpoints) == 2, changed_breakpoints
        if STEPPING_DEBUGGER:
            client.tool("debug_terminate", {})
            terminated = client.tool(
                "debug_wait_event", {"event": "terminated", "timeout_sec": 20}
            )
            assert terminated["event"] == "terminated", terminated
            assert terminated["body"]["session"]["return_code"] is not None, terminated
            assert "exitCode" in terminated["body"], terminated
            assert not any(
                "locked table" in line
                for line in terminated["body"]["session"]["process_output_tail"]
            ), terminated
            print("daslang DAP MCP stepping breakpoint regression passed")
            return 0

        stack = response_body(
            client.tool("debug_stack_trace", {"thread_id": thread_id, "levels": 100})
        )["stackFrames"]
        assert stack, stack
        frame_id = int(stack[0]["id"])
        scopes = response_body(client.tool("debug_scopes", {"frame_id": frame_id}))[
            "scopes"
        ]
        local_groups = []
        for scope in scopes:
            if scope["name"].startswith("Locals"):
                variables = response_body(
                    client.tool(
                        "debug_variables",
                        {"variables_reference": int(scope["variablesReference"])},
                    )
                )["variables"]
                local_groups.append((scope, variables))
        guard_scope, guard_variables = next(
            group
            for group in local_groups
            if any(item["name"] == "guard" for item in group[1])
        )
        guard_variable = next(item for item in guard_variables if item["name"] == "guard")
        guard_value = int(guard_variable["value"])
        assert 0 <= guard_value < 1000, local_groups
        evaluated = response_body(
            client.tool(
                "debug_evaluate",
                {"expression": "guard", "frame_id": frame_id},
            )
        )
        assert str(guard_value) in evaluated["result"], evaluated

        data_info = response_body(
            client.tool(
                "debug_data_breakpoint_info",
                {
                    "variables_reference": int(guard_scope["variablesReference"]),
                    "name": "guard",
                },
            )
        )
        assert data_info["dataId"], data_info
        cleared = response_body(
            client.tool(
                "debug_set_data_breakpoints",
                {"breakpoints": [{"data_id": data_info["dataId"]}]},
            )
        )
        assert len(cleared["breakpoints"]) == 1, cleared
        assert int(cleared["breakpoints"][0]["id"]) != 0, cleared
        cleared = response_body(
            client.tool("debug_set_data_breakpoints", {"breakpoints": []})
        )
        assert cleared["breakpoints"] == [], cleared

        client.tool("debug_set_breakpoints", {"file": str(FIXTURE), "lines": []})
        client.tool("debug_continue", {"thread_id": thread_id})
        client.tool(
            "debug_wait_event", {"event": "continued", "timeout_sec": 1}
        )
        client.tool("debug_pause", {"thread_id": thread_id})
        paused = client.tool(
            "debug_wait_event", {"event": "stopped", "timeout_sec": 20}
        )
        assert paused["event"] == "stopped", paused
        assert paused["body"]["reason"] == "pause", paused
        client.tool("debug_step_in", {"thread_id": thread_id})
        continued = client.tool(
            "debug_wait_event", {"event": "continued", "timeout_sec": 1}
        )
        assert continued["event"] == "continued", continued
        stepped = client.tool(
            "debug_wait_event", {"event": "stopped", "timeout_sec": 20}
        )
        assert stepped["event"] == "stopped", stepped
        assert stepped["body"]["reason"] == "step", stepped
        client.tool("debug_step_over", {"thread_id": thread_id})
        stepped = client.tool(
            "debug_wait_event", {"event": "stopped", "timeout_sec": 20}
        )
        assert stepped["event"] == "stopped", stepped
        assert stepped["body"]["reason"] == "step", stepped
        client.tool("debug_step_out", {"thread_id": thread_id})
        stepped = client.tool(
            "debug_wait_event", {"event": "stopped", "timeout_sec": 20}
        )
        assert stepped["event"] == "stopped", stepped
        assert stepped["body"]["reason"] == "step", stepped
        client.tool("debug_continue", {"thread_id": thread_id})
        terminated = client.tool(
            "debug_wait_event", {"event": "terminated", "timeout_sec": 20}
        )
        assert terminated["event"] == "terminated", terminated
        assert any(
            "DAP_RESULT=42" in line
            for line in terminated["body"]["session"]["process_output_tail"]
        ), terminated
        assert any(
            event.get("event") == "stopped"
            for event in terminated["body"]["session"]["recent_dap_events"]
        ), terminated

        attach_port = free_port()
        attached_process = launch_external(attach_port)
        external_processes.append(attached_process)
        connected = client.tool(
            "debug_connect", {"port": attach_port, "timeout_sec": 20}
        )
        assert connected["port"] == attach_port, connected
        dap_initialized = client.tool("debug_initialize", {})
        assert dap_initialized["body"]["supportsConfigurationDoneRequest"], dap_initialized
        attached = client.tool(
            "debug_attach", {"port": attach_port, "cwd": str(ROOT), "timeout_sec": 20}
        )
        assert attached["attach"]["success"], attached
        client.tool("debug_threads", {})
        client.tool("debug_configuration_done", {})
        client.tool("debug_terminate", {})
        terminated = client.tool(
            "debug_wait_event", {"event": "terminated", "timeout_sec": 20}
        )
        assert terminated["event"] == "terminated", terminated
        attached_process.wait(timeout=10)

        disconnect_port = free_port()
        disconnected_process = launch_external(disconnect_port)
        external_processes.append(disconnected_process)
        client.tool("debug_connect", {"port": disconnect_port, "timeout_sec": 20})
        client.tool("debug_initialize", {})
        client.tool(
            "debug_attach",
            {"port": disconnect_port, "cwd": str(ROOT), "timeout_sec": 20},
        )
        client.tool("debug_threads", {})
        client.tool("debug_configuration_done", {})
        disconnected = client.tool("debug_disconnect", {})
        assert disconnected["success"], disconnected
        disconnected_process.wait(timeout=10)

        disconnected_again = client.tool("debug_disconnect", {})
        assert disconnected_again["already_disconnected"], disconnected_again
        assert disconnected_again["session"]["return_code"] is None, disconnected_again

        default_port_guard = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            try:
                default_port_guard.bind(("127.0.0.1", 10000))
            except OSError as error:
                if error.errno != errno.EADDRINUSE:
                    raise
            auto_launch = client.tool(
                "debug_launch",
                {
                    "file": str(FIXTURE),
                    "timeout_sec": 20,
                    **stepping_arguments(),
                },
            )
        finally:
            default_port_guard.close()
        auto_port = int(auto_launch["connection"]["port"])
        assert auto_port != 10000, auto_launch

        breakpoint_line = source_line("guard += 1")
        client.tool(
            "debug_set_breakpoints",
            {"file": str(FIXTURE), "lines": [breakpoint_line]},
        )
        client.tool("debug_threads", {})
        client.tool("debug_configuration_done", {})
        stopped = client.tool(
            "debug_wait_event", {"event": "stopped", "timeout_sec": 20}
        )
        assert stopped["event"] == "stopped", stopped
        disconnected = client.tool("debug_disconnect", {})
        assert disconnected["success"], disconnected
        disconnected_again = client.tool("debug_disconnect", {})
        assert disconnected_again["already_disconnected"], disconnected_again
        disconnect_session = disconnected_again["session"]
        assert disconnect_session["return_code"] == 0, disconnect_session
        assert not any(
            "locked table" in line
            for line in disconnect_session["process_output_tail"]
        ), disconnect_session

        killed_launch = client.tool(
            "debug_launch",
            {
                "file": str(FIXTURE),
                "timeout_sec": 20,
                **stepping_arguments(),
            },
        )
        auto_pid = int(killed_launch["pid"])
        if os.name == "posix":
            os.killpg(auto_pid, signal.SIGKILL)
        else:
            os.kill(auto_pid, signal.SIGTERM)
        terminated = client.tool(
            "debug_wait_event", {"event": "terminated", "timeout_sec": 20}
        )
        assert terminated["event"] == "terminated", terminated
        assert terminated["body"]["exitCode"] != 0, terminated
        assert terminated["body"]["session"]["return_code"] != 0, terminated
        assert isinstance(
            terminated["body"]["session"]["process_output_tail"], list
        ), terminated

        cleanup = client.tool("debug_disconnect", {})
        assert cleanup["already_disconnected"], cleanup
        assert cleanup["session"]["return_code"] != 0, cleanup
        initialize_error = client.tool_error("debug_initialize", {})
        assert "not connected" in initialize_error, initialize_error
    finally:
        for process in external_processes:
            if process.poll() is None:
                process.terminate()
                try:
                    process.wait(timeout=3)
                except subprocess.TimeoutExpired:
                    process.kill()
                    process.wait(timeout=3)
        client.close()

    print("daslang DAP MCP bridge test passed")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
