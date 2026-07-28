#!/usr/bin/env python3
"""Supervise dasllama-server under JIT, or a standalone program such as Cadmus.

The first untuned invocation exits with code 3 after writing the tune sidecar;
that is an expected bootstrap event and is relaunched immediately. Exit 4 is a
config restart (the control page saved a new config and asked for a relaunch)
and is also relaunched immediately. Exit 0 is an intentional shutdown. Other
exits are reported, notified, and restarted with bounded exponential backoff. Standalone programs can use a stop-file handshake
to finish their current update and run shutdown before exiting.
"""
from __future__ import annotations

import argparse
import ctypes
import ctypes.wintypes as wt
import importlib.util
import json
import logging
from logging.handlers import RotatingFileHandler
import os
from pathlib import Path
import re
import shutil
import signal
import subprocess
import sys
import threading
import time
from urllib.error import URLError
from urllib.request import Request, urlopen


IS_WINDOWS = os.name == "nt"
SCRIPT_DIR = Path(__file__).resolve().parent
SOURCE_REPO_ROOT = SCRIPT_DIR.parent.parent
# In a deployed bundle the watchdog sits beside what it supervises, so the bundle dir is the right
# default. In-repo it lives in utils/watchdog/ next to nothing runnable, so those runs pass --cwd.
DEFAULT_CWD = SCRIPT_DIR
TUNE_RESTART_EXIT = 3
CONFIG_RESTART_EXIT = 4
# Per-program wiring shipped beside watchdog.py by `daspkg release`. Every key sets the DEFAULT for
# the flag of the same name, so an explicit flag still wins. Absent config = discovery (below).
CONFIG_NAME = "watchdog.json"
# Optional per-program control surface (the dictation bot's settings page, say). Supervision stays
# generic; anything that knows about a specific program's config lives in this sibling module.
CONTROL_MODULE = "watchdog_control.py"
CONFIG_PATH_KEYS = frozenset(
    {"program", "daslang", "script", "cwd", "log", "pid_file", "stop_file",
     "dump_dir", "crash_dir", "jit_dir", "control_page"}
)
# How often to restate "still healthy" once health has settled. Health is otherwise logged only on
# transition, so this is the only proof-of-life in a long quiet run.
HEALTH_HEARTBEAT_SECONDS = 300.0
JIT_ARTIFACT_SUFFIXES = {".dll", ".exp", ".lib", ".map", ".o", ".obj", ".pdb"}
WER_LOCAL_DUMPS = r"SOFTWARE\Microsoft\Windows\Windows Error Reporting\LocalDumps"


# Supervision state, published for the control plugin (which runs on HTTP threads) to read.
STATE_LOCK = threading.Lock()
STATE: dict[str, object] = {
    "watchdog_started_at": time.time(),
    "child_pid": 0,
    "child_started_at": 0.0,
    "child_exit_code": None,
    "restart_delay": 0.0,
    "stage": None,
    # Progress of an in-flight tune, folded from llvm_tune's @tune events. Counters only —
    # nothing here is an estimate, because the tuner cannot know how long it has left.
    "tune": {},
}


def set_state(**fields: object) -> None:
    with STATE_LOCK:
        STATE.update(fields)


def read_state() -> dict[str, object]:
    with STATE_LOCK:
        return dict(STATE)


def find_daslang() -> Path:
    # dasllama-server is JIT-only (per-box [tune]/[llvm_code] kernels), so the watchdog always
    # runs `daslang -jit main.das`, never a baked exe. Prefer a daslang inside a deployed JIT
    # bundle (bin/Release beside this script — the layout getDasRoot() resolves the bundle from),
    # then the source-repo build tree, then bare name on PATH.
    rel = (
        "bin/Release/daslang.exe",
        "bin/daslang.exe",
        "bin/Release/daslang",
        "bin/daslang",
        "build/daslang",
    )
    for root in (SCRIPT_DIR, SOURCE_REPO_ROOT):
        for name in rel:
            candidate = root / name
            if candidate.is_file():
                return candidate
    return Path("daslang.exe" if IS_WINDOWS else "daslang")


def resolve_from(path: Path, base: Path) -> Path:
    if path.is_absolute():
        return path.resolve()
    return (base / path).resolve()


def make_logger(path: Path, name: str = "watchdog") -> logging.Logger:
    path.parent.mkdir(parents=True, exist_ok=True)
    logger = logging.getLogger(f"{name}-watchdog")
    logger.setLevel(logging.INFO)
    logger.handlers.clear()
    formatter = logging.Formatter("%(message)s")
    rotating = RotatingFileHandler(
        path, maxBytes=20 * 1024 * 1024, backupCount=5, encoding="utf-8"
    )
    rotating.setFormatter(formatter)
    logger.addHandler(rotating)
    console = logging.StreamHandler(sys.stdout)
    console.setFormatter(formatter)
    logger.addHandler(console)
    return logger


def emit(logger: logging.Logger, event: str, **fields) -> None:
    payload = {
        "ts": time.strftime("%Y-%m-%dT%H:%M:%S%z"),
        "event": event,
        **fields,
    }
    logger.info(json.dumps(payload, ensure_ascii=False, separators=(",", ":")))


def windows_balloon(title: str, message: str, error: bool = False) -> None:
    """Show a dependency-free notification in the interactive Windows session."""
    if not IS_WINDOWS:
        return
    icon = "Error" if error else "Info"
    safe_title = title.replace("'", "''")
    safe_message = message.replace("'", "''")
    script = (
        "Add-Type -AssemblyName System.Windows.Forms;"
        "Add-Type -AssemblyName System.Drawing;"
        "$n=New-Object System.Windows.Forms.NotifyIcon;"
        f"$n.Icon=[System.Drawing.SystemIcons]::{icon};"
        "$n.Visible=$true;"
        f"$n.ShowBalloonTip(10000,'{safe_title}','{safe_message}',"
        f"[System.Windows.Forms.ToolTipIcon]::{icon});"
        "Start-Sleep -Seconds 11;"
        "$n.Dispose()"
    )
    creationflags = getattr(subprocess, "CREATE_NO_WINDOW", 0)
    try:
        subprocess.Popen(
            ["powershell.exe", "-NoProfile", "-NonInteractive", "-Command", script],
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL,
            creationflags=creationflags,
        )
    except OSError:
        pass


class PROCESS_MEMORY_COUNTERS_EX(ctypes.Structure):
    _fields_ = [
        ("cb", wt.DWORD),
        ("PageFaultCount", wt.DWORD),
        ("PeakWorkingSetSize", ctypes.c_size_t),
        ("WorkingSetSize", ctypes.c_size_t),
        ("QuotaPeakPagedPoolUsage", ctypes.c_size_t),
        ("QuotaPagedPoolUsage", ctypes.c_size_t),
        ("QuotaPeakNonPagedPoolUsage", ctypes.c_size_t),
        ("QuotaNonPagedPoolUsage", ctypes.c_size_t),
        ("PagefileUsage", ctypes.c_size_t),
        ("PeakPagefileUsage", ctypes.c_size_t),
        ("PrivateUsage", ctypes.c_size_t),
    ]


class FILETIME(ctypes.Structure):
    _fields_ = [("low", wt.DWORD), ("high", wt.DWORD)]

    def ticks(self) -> int:
        return (int(self.high) << 32) | int(self.low)


class THREADENTRY32(ctypes.Structure):
    _fields_ = [
        ("dwSize", wt.DWORD),
        ("cntUsage", wt.DWORD),
        ("th32ThreadID", wt.DWORD),
        ("th32OwnerProcessID", wt.DWORD),
        ("tpBasePri", wt.LONG),
        ("tpDeltaPri", wt.LONG),
        ("dwFlags", wt.DWORD),
    ]


def windows_thread_count(pid: int) -> int | None:
    if not IS_WINDOWS:
        return None
    kernel32 = ctypes.WinDLL("kernel32", use_last_error=True)
    kernel32.CreateToolhelp32Snapshot.argtypes = [wt.DWORD, wt.DWORD]
    kernel32.CreateToolhelp32Snapshot.restype = wt.HANDLE
    kernel32.Thread32First.argtypes = [wt.HANDLE, ctypes.POINTER(THREADENTRY32)]
    kernel32.Thread32First.restype = wt.BOOL
    kernel32.Thread32Next.argtypes = [wt.HANDLE, ctypes.POINTER(THREADENTRY32)]
    kernel32.Thread32Next.restype = wt.BOOL
    kernel32.CloseHandle.argtypes = [wt.HANDLE]
    kernel32.CloseHandle.restype = wt.BOOL
    snapshot = kernel32.CreateToolhelp32Snapshot(0x00000004, 0)
    invalid = ctypes.c_void_p(-1).value
    if snapshot == invalid:
        return None
    entry = THREADENTRY32()
    entry.dwSize = ctypes.sizeof(entry)
    count = 0
    try:
        ok = kernel32.Thread32First(snapshot, ctypes.byref(entry))
        while ok:
            if entry.th32OwnerProcessID == pid:
                count += 1
            ok = kernel32.Thread32Next(snapshot, ctypes.byref(entry))
    finally:
        kernel32.CloseHandle(snapshot)
    return count


def windows_process_metrics(pid: int) -> dict[str, int | float] | None:
    if not IS_WINDOWS:
        return None
    kernel32 = ctypes.WinDLL("kernel32", use_last_error=True)
    psapi = ctypes.WinDLL("psapi", use_last_error=True)
    kernel32.OpenProcess.argtypes = [wt.DWORD, wt.BOOL, wt.DWORD]
    kernel32.OpenProcess.restype = wt.HANDLE
    kernel32.CloseHandle.argtypes = [wt.HANDLE]
    kernel32.CloseHandle.restype = wt.BOOL
    kernel32.GetProcessHandleCount.argtypes = [wt.HANDLE, ctypes.POINTER(wt.DWORD)]
    kernel32.GetProcessHandleCount.restype = wt.BOOL
    kernel32.GetProcessTimes.argtypes = [
        wt.HANDLE,
        ctypes.POINTER(FILETIME),
        ctypes.POINTER(FILETIME),
        ctypes.POINTER(FILETIME),
        ctypes.POINTER(FILETIME),
    ]
    kernel32.GetProcessTimes.restype = wt.BOOL
    psapi.GetProcessMemoryInfo.argtypes = [
        wt.HANDLE,
        ctypes.POINTER(PROCESS_MEMORY_COUNTERS_EX),
        wt.DWORD,
    ]
    psapi.GetProcessMemoryInfo.restype = wt.BOOL
    handle = kernel32.OpenProcess(0x1000 | 0x0010, False, pid)
    if not handle:
        return None
    try:
        memory = PROCESS_MEMORY_COUNTERS_EX()
        memory.cb = ctypes.sizeof(memory)
        if not psapi.GetProcessMemoryInfo(
            handle, ctypes.byref(memory), ctypes.sizeof(memory)
        ):
            return None
        handles = wt.DWORD()
        kernel32.GetProcessHandleCount(handle, ctypes.byref(handles))
        created = FILETIME()
        exited = FILETIME()
        kernel = FILETIME()
        user = FILETIME()
        kernel32.GetProcessTimes(
            handle,
            ctypes.byref(created),
            ctypes.byref(exited),
            ctypes.byref(kernel),
            ctypes.byref(user),
        )
        return {
            "working_set": int(memory.WorkingSetSize),
            "peak_working_set": int(memory.PeakWorkingSetSize),
            "private_bytes": int(memory.PrivateUsage),
            "peak_pagefile": int(memory.PeakPagefileUsage),
            "page_faults": int(memory.PageFaultCount),
            "handles": int(handles.value),
            "threads": windows_thread_count(pid) or 0,
            "cpu_seconds": (kernel.ticks() + user.ticks()) / 10_000_000.0,
        }
    finally:
        kernel32.CloseHandle(handle)


def latest_wer_report(started_at: float) -> str | None:
    if not IS_WINDOWS:
        return None
    root = Path(os.environ.get("ProgramData", r"C:\ProgramData")) / "Microsoft/Windows/WER"
    candidates: list[Path] = []
    for directory in (root / "ReportArchive", root / "ReportQueue"):
        if not directory.is_dir():
            continue
        try:
            for path in directory.glob("AppCrash_*"):
                if path.stat().st_mtime >= started_at - 5.0:
                    candidates.append(path)
        except OSError:
            continue
    if not candidates:
        return None
    return str(max(candidates, key=lambda path: path.stat().st_mtime))


def windows_local_dump_config(exe_name: str) -> tuple[int | None, Path | None]:
    """Return the effective WER dump type/folder for an executable."""
    if not IS_WINDOWS:
        return None, None
    import winreg

    for subkey, app_specific in (
        (f"{WER_LOCAL_DUMPS}\\{exe_name}", True),
        (WER_LOCAL_DUMPS, False),
    ):
        try:
            with winreg.OpenKey(
                winreg.HKEY_LOCAL_MACHINE,
                subkey,
                0,
                winreg.KEY_READ | winreg.KEY_WOW64_64KEY,
            ) as key:
                # A root LocalDumps key that only contains application subkeys is not a global
                # policy. Treat it as configured only when it carries at least one policy value.
                if not app_specific:
                    policy_value = False
                    for name in ("DumpFolder", "DumpCount", "DumpType", "CustomDumpFlags"):
                        try:
                            winreg.QueryValueEx(key, name)
                            policy_value = True
                            break
                        except FileNotFoundError:
                            pass
                    if not policy_value:
                        continue
                try:
                    dump_type = int(winreg.QueryValueEx(key, "DumpType")[0])
                except FileNotFoundError:
                    dump_type = 1
                try:
                    folder_text = str(winreg.QueryValueEx(key, "DumpFolder")[0])
                    folder = Path(os.path.expandvars(folder_text))
                except FileNotFoundError:
                    folder = Path(os.environ["LOCALAPPDATA"]) / "CrashDumps"
                return dump_type, folder
        except FileNotFoundError:
            continue
        except OSError:
            return None, None
    return None, None


def install_windows_local_dumps(exe_name: str, dump_dir: Path, count: int) -> None:
    """Install an application-specific normal-minidump policy (requires elevation)."""
    if not IS_WINDOWS:
        raise OSError("WER LocalDumps is available only on Windows")
    import winreg

    dump_dir = dump_dir.resolve()
    dump_dir.mkdir(parents=True, exist_ok=True)
    subkey = f"{WER_LOCAL_DUMPS}\\{exe_name}"
    with winreg.CreateKeyEx(
        winreg.HKEY_LOCAL_MACHINE,
        subkey,
        0,
        winreg.KEY_SET_VALUE | winreg.KEY_WOW64_64KEY,
    ) as key:
        winreg.SetValueEx(key, "DumpFolder", 0, winreg.REG_EXPAND_SZ, str(dump_dir))
        winreg.SetValueEx(key, "DumpCount", 0, winreg.REG_DWORD, max(count, 1))
        winreg.SetValueEx(key, "DumpType", 0, winreg.REG_DWORD, 1)


def wait_for_recent_dump(
    dump_dir: Path,
    exe_name: str,
    started_at: float,
    timeout: float = 15.0,
) -> Path | None:
    """Wait for WER to finish publishing the newest dump for this child."""
    deadline = time.monotonic() + timeout
    exe_prefix = exe_name.lower()
    while time.monotonic() < deadline:
        candidates: list[Path] = []
        try:
            for path in dump_dir.glob("*.dmp"):
                if path.name.lower().startswith(exe_prefix) and path.stat().st_mtime >= started_at - 5.0:
                    candidates.append(path)
        except OSError:
            candidates = []
        if candidates:
            newest = max(candidates, key=lambda path: path.stat().st_mtime)
            try:
                with newest.open("rb"):
                    pass
                return newest
            except OSError:
                pass
        time.sleep(0.25)
    return None


def prune_crash_bundles(root: Path, name: str, keep: int) -> None:
    if keep <= 0 or not root.is_dir():
        return
    bundles = sorted(
        (path for path in root.glob(f"{name}-*") if path.is_dir()),
        key=lambda path: path.stat().st_mtime,
        reverse=True,
    )
    for path in bundles[keep:]:
        shutil.rmtree(path, ignore_errors=True)


def collect_crash_bundle(
    args: argparse.Namespace,
    command: list[str],
    child_pid: int,
    return_code: int,
    started_at: float,
    wer_report: str | None,
    dump_path: Path | None,
    jit_dlls: set[Path],
) -> Path:
    stamp = time.strftime("%Y%m%d-%H%M%S")
    root = args.crash_dir.resolve()
    bundle = root / f"{args.name}-{stamp}-pid{child_pid}"
    bundle.mkdir(parents=True, exist_ok=False)
    program_artifacts: list[Path] = []
    if args.program is not None:
        for source in (
            args.program,
            args.program.with_suffix(".map"),
            args.program.with_suffix(".pdb"),
        ):
            if source.is_file():
                program_artifacts.append(source)
    metadata = {
        "timestamp": time.strftime("%Y-%m-%dT%H:%M:%S%z"),
        "pid": child_pid,
        "exit_code": return_code,
        "exit_code_hex": f"0x{return_code & 0xFFFFFFFF:08X}",
        "started_at": started_at,
        "command": command,
        "cwd": str(args.cwd.resolve()),
        "wer_report": wer_report or "",
        "dump": dump_path.name if dump_path is not None else "",
        "jit_dlls": [str(path) for path in sorted(jit_dlls)],
        "program_artifacts": [path.name for path in program_artifacts],
    }
    (bundle / "crash.json").write_text(
        json.dumps(metadata, indent=2, ensure_ascii=False) + "\n",
        encoding="utf-8",
    )
    if args.log.is_file():
        shutil.copy2(args.log, bundle / args.log.name)
    if dump_path is not None and dump_path.is_file():
        shutil.copy2(dump_path, bundle / dump_path.name)
    for source in program_artifacts:
        shutil.copy2(source, bundle / source.name)
    tune_file = (
        args.program.with_suffix(".tune.json")
        if args.program is not None
        else args.script.with_suffix(".tune.json")
    )
    if tune_file.is_file():
        shutil.copy2(tune_file, bundle / tune_file.name)
    jit_dir = args.jit_dir.resolve()
    if jit_dir.is_dir():
        artifact_root = bundle / "jitted_scripts"
        resolved_dlls: set[Path] = set()
        for path in jit_dlls:
            source = path if path.is_absolute() else args.cwd / path
            source = source.resolve()
            try:
                source.relative_to(jit_dir)
            except ValueError:
                continue
            if source.is_file():
                resolved_dlls.add(source)
        if not resolved_dlls:
            cached = [path for path in jit_dir.rglob("*.dll") if path.is_file()]
            if cached:
                resolved_dlls.add(max(cached, key=lambda path: path.stat().st_mtime))
        for dll in resolved_dlls:
            for suffix in JIT_ARTIFACT_SUFFIXES:
                source = dll.with_suffix(suffix)
                if not source.is_file():
                    continue
                target = artifact_root / source.relative_to(jit_dir)
                target.parent.mkdir(parents=True, exist_ok=True)
                shutil.copy2(source, target)
    prune_crash_bundles(root, args.name, args.crash_bundles)
    return bundle


def health_ok(url: str) -> bool:
    try:
        with urlopen(url, timeout=2.0) as response:
            return 200 <= response.status < 300
    except (OSError, URLError):
        return False


def request_shutdown(url: str, logger: logging.Logger) -> None:
    try:
        request = Request(url, data=b"", method="POST")
        with urlopen(request, timeout=5.0) as response:
            emit(logger, "shutdown_requested", status=response.status)
    except (OSError, URLError) as error:
        emit(logger, "shutdown_request_failed", error=str(error))


# Startup is several minutes of work with no single progress signal, and a cold start after a JIT
# cache invalidation re-tunes every kernel family. Without these the log is only `health ok:false`
# repeated for the whole window, which says the server is down but never what it is doing.
#
# Ranked and monotonic: a tune codegens each kernel variant, so the JIT lines fire hundreds of times
# mid-tune and would otherwise flap the stage back and forth once a second. Only a forward move is
# reported. `tune_restart` is the one legitimate rewind — the process restarts to apply the winners
# — so it resets the rank and the sequence starts over.
STARTUP_STAGES: list[tuple[str, int, str]] = [
    ("jit_cached",   1, r"LLVM JIT: DLL cache hit"),
    ("jit_codegen",  1, r"LLVM JIT: DLL cache miss, codegen for"),
    ("jit_linked",   2, r"Library .+ linked - ok"),
    ("tuning",       3, r"llvm_tune: tuning scope"),
    ("tune_restart", 4, r"llvm_tune: restart to apply the winners"),
    ("model_load",   5, r"dasLLAMA: prepared image mapped"),
    ("asr_init",     6, r"starting \d+ asynchronous ASR worker"),
    ("ready",        7, r"listening on http"),
]
TUNE_RESTART_RANK = 4


def detect_stage(message: str) -> tuple[str, int] | None:
    for name, rank, pattern in STARTUP_STAGES:
        if re.search(pattern, message):
            return name, rank
    return None


def advance_stage(stage: dict[str, object], message: str, now: float) -> dict[str, object] | None:
    """Return the fields to log for a forward stage move, or None. Mutates `stage` on a move.

    Split out so it can be replayed against a real startup log — the rank/reset rule is the part
    that is easy to get wrong, and a naive version flaps once per kernel variant during a tune.
    """
    found = detect_stage(message)
    if found is None:
        return None
    name, rank = found
    if rank <= int(stage.get("rank", 0)):
        return None
    fields: dict[str, object] = {"stage": name}
    if stage.get("name") is not None:
        fields["after"] = stage["name"]
        began = stage.get("since")
        if isinstance(began, float):
            fields["elapsed_s"] = round(now - began, 1)
    stage["name"] = name
    # The restart re-runs codegen from scratch, so let the sequence replay.
    stage["rank"] = 0 if rank == TUNE_RESTART_RANK else rank
    stage["since"] = now
    return fields


# llvm_tune emits structured progress as `@tune <kind> k=v ...` lines. A supervised child never
# owns a terminal, so it forwards these instead of drawing a bar — that is exactly what makes
# them ours to consume. Values never contain spaces (llvm_tune's contract), so a plain split is
# enough. Consuming these is why the log no longer needs to infer structure from prose.
TUNE_EVENT_PREFIX = "@tune "


def parse_tune_event(message: str) -> tuple[str, dict[str, str]] | None:
    """Return (kind, fields) for a `@tune ...` line, or None. Never raises on malformed input."""
    if not message.startswith(TUNE_EVENT_PREFIX):
        return None
    parts = message[len(TUNE_EVENT_PREFIX):].split()
    if not parts:
        return None
    fields = {}
    for token in parts[1:]:
        key, sep, value = token.partition("=")
        if sep:
            fields[key] = value
    return parts[0], fields


def apply_tune_event(tune: dict[str, object], kind: str, fields: dict[str, str]) -> None:
    """Fold one progress event into the published tune state. Counters only; no prediction."""
    def as_int(key: str) -> int:
        try:
            return int(fields.get(key, ""))
        except ValueError:
            return 0
    if kind == "plan":
        tune.clear()
        tune.update(scope=fields.get("scope", ""), done=0, total=as_int("total"))
    elif kind == "begin":
        tune.update(kernel=fields.get("name", ""), round=0, rounds=as_int("total"),
                    phase="", live=0)
    elif kind == "step":
        tune.update(round=as_int("i"), phase=fields.get("phase", ""), live=as_int("live"))
    elif kind == "end":
        # the per-round fields describe an in-flight kernel; leaving them set between kernels
        # publishes an incoherent state (round=47 against rounds=0) that a status page renders
        tune["done"] = int(tune.get("done", 0)) + 1
        tune.update(round=0, rounds=0, phase="", live=0)


def stream_child(
    proc: subprocess.Popen[str],
    logger: logging.Logger,
    tune_restart_seen: threading.Event,
    jit_dlls: set[Path],
    stage: dict[str, object],
) -> None:
    assert proc.stdout is not None
    # Per child, and published immediately: the tune-bootstrap restart is the normal first-run
    # path, and the relaunched child emits no tune events at all. Without clearing here, the
    # control page would keep showing the finished tune's counters for the rest of the run.
    tune: dict[str, object] = {}
    set_state(tune={})
    for line in proc.stdout:
        message = line.rstrip("\r\n")
        event = parse_tune_event(message)
        if event is not None:
            kind, fields = event
            apply_tune_event(tune, kind, fields)
            # A tune is hundreds of steps; publish every one for the control page but log only
            # the kernel boundaries, or the log becomes the flood these events exist to replace.
            set_state(tune=dict(tune))
            if kind in ("plan", "end"):
                emit(logger, "tune", pid=proc.pid, kind=kind, **fields)
            continue
        if "llvm_tune: restart to apply the winners" in message:
            tune_restart_seen.set()
        match = re.search(
            r"LLVM JIT: DLL cache (?:hit |miss, codegen for )(.+\.dll)$",
            message,
        )
        if match is not None:
            jit_dlls.add(Path(match.group(1)))
        moved = advance_stage(stage, message, time.monotonic())
        if moved is not None:
            emit(logger, "stage", pid=proc.pid, **moved)
            set_state(stage=moved["stage"])
        emit(logger, "child", pid=proc.pid, message=message)


def start_control_plugin(script_dir: Path, logger: logging.Logger, args: argparse.Namespace):
    """Start the sibling control module if the program ships one. It must expose
    start(logger, args). A control surface that fails to load is reported and skipped, never
    fatal — keeping the program alive outranks being able to reconfigure it."""
    path = script_dir / CONTROL_MODULE
    if not path.is_file():
        return None
    try:
        spec = importlib.util.spec_from_file_location("watchdog_control", path)
        module = importlib.util.module_from_spec(spec)
        # Injected before exec so the plugin can reuse emit() and friends. Sharing the host beats
        # re-declaring emit in every plugin, where the log envelope would drift.
        module.host = sys.modules[__name__]
        spec.loader.exec_module(module)
        handle = module.start(logger, args)
    except Exception as exc:  # noqa: BLE001 - a broken plugin must not take supervision down
        emit(logger, "control_failed", path=str(path), error=repr(exc))
        return None
    emit(logger, "control_started", path=str(path))
    return handle


def load_config(script_dir: Path) -> dict[str, object]:
    """Read watchdog.json beside this script. Missing is fine; malformed is fatal."""
    path = script_dir / CONFIG_NAME
    if not path.is_file():
        return {}
    try:
        data = json.loads(path.read_text(encoding="utf-8"))
    except OSError as exc:
        raise SystemExit(f"watchdog: cannot read {path}: {exc}")
    except ValueError as exc:
        raise SystemExit(f"watchdog: {path} is not valid JSON: {exc}")
    if not isinstance(data, dict):
        raise SystemExit(f"watchdog: {path} must hold a JSON object, got {type(data).__name__}")
    return data


def apply_config(parser: argparse.ArgumentParser, config: dict[str, object], where: Path) -> None:
    """Fold config into the parser's defaults. An unknown key is fatal: silently ignoring a typo
    would supervise the program with wrong wiring and nothing would say so."""
    if not config:
        return
    valid = set(vars(parser.parse_args([])))
    unknown = sorted(set(config) - valid)
    if unknown:
        raise SystemExit(
            f"watchdog: {where} has unknown key(s): {', '.join(unknown)}\n"
            f"  known keys: {', '.join(sorted(valid - {'server_args'}))}"
        )
    # set_defaults bypasses each action's type=, so path-valued keys convert here.
    resolved = {k: (Path(str(v)) if k in CONFIG_PATH_KEYS and v is not None else v)
                for k, v in config.items()}
    parser.set_defaults(**resolved)


def discover_target(script_dir: Path) -> dict[str, object]:
    """Infer what to supervise from the bundle layout, so a deployed `python watchdog.py` with no
    config and no flags just runs. Ambiguity is reported, never guessed."""
    script = script_dir / "main.das"
    daslang = script_dir / "bin/Release/daslang.exe"
    if script.is_file() and daslang.is_file():
        return {"script": script, "daslang": daslang}
    exes = sorted(p for p in script_dir.glob("*.exe") if p.is_file())
    if len(exes) == 1:
        return {"program": exes[0], "name": exes[0].stem}
    if len(exes) > 1:
        raise SystemExit(
            f"watchdog: {script_dir} holds {len(exes)} executables "
            f"({', '.join(p.name for p in exes)}); pick one with --program or {CONFIG_NAME}"
        )
    raise SystemExit(
        f"watchdog: nothing to supervise in {script_dir} — expected a single *.exe, or "
        f"main.das beside bin/Release/daslang.exe. Use --program/--script or ship {CONFIG_NAME}."
    )


def parse_cli() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    # None means "derive from the discovered program"; see resolve_identity.
    parser.add_argument("--name", default=None)
    parser.add_argument(
        "--program",
        type=Path,
        help="Run this standalone program instead of daslang -jit <script>; relative paths use --cwd",
    )
    parser.add_argument("--daslang", type=Path, default=None)
    parser.add_argument("--script", type=Path, default=None)
    parser.add_argument("--cwd", type=Path, default=DEFAULT_CWD)
    parser.add_argument("--log", type=Path, default=None)
    parser.add_argument("--pid-file", type=Path, default=None)
    parser.add_argument("--health-url", default="http://127.0.0.1:8080/v1/models")
    parser.add_argument("--shutdown-url", default="http://127.0.0.1:8080/shutdown")
    parser.add_argument("--no-health", action="store_true")
    parser.add_argument("--no-shutdown", action="store_true")
    parser.add_argument(
        "--stop-file",
        type=Path,
        help="Create this file to request a lifecycle-safe standalone shutdown",
    )
    parser.add_argument("--stop-env", default="CADMUS_STOP_FILE")
    parser.add_argument("--stop-timeout", type=float, default=1200.0)
    parser.add_argument("--memory-interval", type=float, default=60.0)
    parser.add_argument("--health-interval", type=float, default=5.0)
    parser.add_argument("--stable-seconds", type=float, default=300.0)
    parser.add_argument("--max-restart-delay", type=float, default=60.0)
    parser.add_argument(
        "--jit-stack",
        action="store_true",
        help="Pass pre-compile -jit-stack to daslang so every generated call has a logical stack frame",
    )
    parser.add_argument(
        "--dump-dir",
        type=Path,
        default=Path("logs/dumps"),
        help="Expected WER LocalDumps output folder",
    )
    parser.add_argument(
        "--install-local-dumps",
        action="store_true",
        help="Install an app-specific WER normal-minidump policy and exit (Windows, elevated)",
    )
    parser.add_argument("--dump-count", type=int, default=10)
    parser.add_argument(
        "--require-dumps",
        action="store_true",
        help="Refuse to start unless an app-specific/global WER normal-minidump policy is active",
    )
    parser.add_argument(
        "--crash-dir",
        type=Path,
        default=Path("logs/crashes"),
        help="Crash bundle destination",
    )
    parser.add_argument("--crash-bundles", type=int, default=10)
    parser.add_argument(
        "--jit-dir",
        type=Path,
        default=Path(".jitted_scripts"),
        help="JIT artifact cache copied into each crash bundle",
    )
    parser.add_argument("--no-crash-bundle", action="store_true")
    parser.add_argument("server_args", nargs=argparse.REMAINDER)
    apply_config(parser, load_config(SCRIPT_DIR), SCRIPT_DIR / CONFIG_NAME)
    args = parser.parse_args()
    if args.server_args[:1] == ["--"]:
        args.server_args = args.server_args[1:]
    resolve_identity(args)
    return args


def resolve_identity(args: argparse.Namespace) -> None:
    """Fill in whatever neither the CLI nor watchdog.json pinned down: what to run, and the
    name that the log, pid file and notifications are keyed on."""
    if args.program is None and args.script is None:
        for key, value in discover_target(args.cwd.resolve()).items():
            if getattr(args, key) is None:
                setattr(args, key, value)
    if args.name is None:
        # An executable names itself; a script is almost always main.das, so the bundle directory
        # is the meaningful identity there.
        args.name = (
            Path(args.program).stem if args.program is not None else args.cwd.resolve().name
        )
    if args.daslang is None:
        args.daslang = find_daslang()
    if args.log is None:
        args.log = Path(f"logs/{args.name}-watchdog.log")
    if args.pid_file is None:
        args.pid_file = Path(f"logs/{args.name}-watchdog.pid")


def main() -> int:
    args = parse_cli()
    args.cwd = args.cwd.resolve()
    args.log = resolve_from(args.log, args.cwd)
    args.pid_file = resolve_from(args.pid_file, args.cwd)
    if args.script is not None:
        args.script = resolve_from(args.script, args.cwd)
    args.dump_dir = resolve_from(args.dump_dir, args.cwd)
    args.crash_dir = resolve_from(args.crash_dir, args.cwd)
    args.jit_dir = resolve_from(args.jit_dir, args.cwd)
    if args.program is not None:
        args.program = resolve_from(args.program, args.cwd)
    if args.stop_file is not None:
        args.stop_file = resolve_from(args.stop_file, args.cwd)
    logger = make_logger(args.log, args.name)
    start_control_plugin(SCRIPT_DIR, logger, args)
    daslang_candidate = resolve_from(args.daslang, args.cwd)
    daslang = daslang_candidate if daslang_candidate.is_file() else args.daslang
    if args.program is not None:
        command = [str(args.program)]
    else:
        command = [str(daslang), "-jit"]
        if args.jit_stack:
            command.append("-jit-stack")
        command.extend((str(args.script), "--"))
    command.extend(args.server_args)
    child_exe_name = Path(command[0]).name

    if args.install_local_dumps:
        try:
            install_windows_local_dumps(child_exe_name, args.dump_dir, args.dump_count)
        except OSError as error:
            emit(
                logger,
                "wer_install_failed",
                executable=child_exe_name,
                dump_dir=str(args.dump_dir),
                error=str(error),
            )
            windows_balloon(
                f"{args.name} dump setup failed",
                f"Run the LocalDumps install from an elevated terminal: {error}",
                True,
            )
            return 2
        emit(
            logger,
            "wer_installed",
            executable=child_exe_name,
            dump_dir=str(args.dump_dir),
            dump_type="minidump",
            dump_count=max(args.dump_count, 1),
        )
        return 0

    dump_type, configured_dump_dir = windows_local_dump_config(child_exe_name)
    if configured_dump_dir is not None:
        args.dump_dir = configured_dump_dir
    if dump_type == 1:
        emit(
            logger,
            "wer_ready",
            executable=child_exe_name,
            dump_dir=str(args.dump_dir),
            dump_type="minidump",
        )
    else:
        reason = "not configured" if dump_type is None else f"unsafe dump type {dump_type}"
        emit(
            logger,
            "wer_not_ready",
            executable=child_exe_name,
            dump_dir=str(args.dump_dir),
            reason=reason,
        )
        if args.require_dumps:
            windows_balloon(
                f"{args.name} watchdog not started",
                f"WER normal minidumps are {reason}; run --install-local-dumps elevated",
                True,
            )
            return 2

    args.pid_file.parent.mkdir(parents=True, exist_ok=True)
    args.pid_file.write_text(str(os.getpid()), encoding="ascii")
    stopping = threading.Event()
    child: subprocess.Popen[str] | None = None

    def stop_handler(_signum, _frame) -> None:
        stopping.set()

    signal.signal(signal.SIGINT, stop_handler)
    signal.signal(signal.SIGTERM, stop_handler)
    failures = 0
    recovery_pending = False
    stop_file = args.stop_file
    child_env = os.environ.copy()
    if stop_file is not None:
        child_env[args.stop_env] = str(stop_file)
    emit(logger, "watchdog_started", pid=os.getpid(), command=command, cwd=str(args.cwd))

    try:
        while not stopping.is_set():
            if stop_file is not None:
                try:
                    stop_file.unlink()
                except FileNotFoundError:
                    pass
            started_at = time.time()
            creationflags = getattr(subprocess, "CREATE_NEW_PROCESS_GROUP", 0) if IS_WINDOWS else 0
            try:
                child = subprocess.Popen(
                    command,
                    cwd=args.cwd,
                    stdout=subprocess.PIPE,
                    stderr=subprocess.STDOUT,
                    text=True,
                    encoding="utf-8",
                    errors="replace",
                    bufsize=1,
                    creationflags=creationflags,
                    env=child_env,
                )
            except OSError as error:
                emit(logger, "spawn_failed", error=str(error))
                windows_balloon(f"{args.name} watchdog", f"Could not start child: {error}", True)
                return 2

            emit(logger, "child_started", pid=child.pid)
            set_state(child_pid=child.pid, child_started_at=started_at,
                      child_exit_code=None, restart_delay=0.0, stage=None)
            tune_restart_seen = threading.Event()
            jit_dlls: set[Path] = set()
            stage: dict[str, object] = {"name": None, "rank": 0, "since": time.monotonic()}
            output = threading.Thread(
                target=stream_child,
                args=(child, logger, tune_restart_seen, jit_dlls, stage),
                daemon=True,
            )
            output.start()
            next_memory = time.monotonic()
            next_health = time.monotonic()
            last_health: bool | None = None
            last_health_change = time.monotonic()
            next_heartbeat = time.monotonic() + HEALTH_HEARTBEAT_SECONDS
            shutdown_sent = False
            shutdown_requested_at = 0.0
            while child.poll() is None:
                now = time.monotonic()
                if stopping.is_set() and not shutdown_sent:
                    if stop_file is not None:
                        stop_file.parent.mkdir(parents=True, exist_ok=True)
                        stop_file.write_text("stop\n", encoding="ascii")
                        emit(logger, "stop_file_requested", path=str(stop_file))
                    elif args.no_shutdown:
                        child.terminate()
                        emit(logger, "terminate_requested")
                    else:
                        request_shutdown(args.shutdown_url, logger)
                    shutdown_sent = True
                    shutdown_requested_at = time.monotonic()
                if (stopping.is_set() and shutdown_sent and
                        time.monotonic() - shutdown_requested_at > args.stop_timeout):
                    child.terminate()
                if now >= next_memory:
                    metrics = windows_process_metrics(child.pid)
                    if metrics is not None:
                        emit(logger, "memory", pid=child.pid, **metrics)
                    next_memory = now + max(args.memory_interval, 1.0)
                if not args.no_health and now >= next_health:
                    healthy = health_ok(args.health_url)
                    # Log transitions, not every poll. A cold start is minutes of `ok:false` that
                    # says nothing the `stage` events do not say better; a steady-state run is
                    # hours of `ok:true`. Both drown the events that matter. The periodic
                    # heartbeat keeps "still alive and healthy" observable at a readable rate.
                    if healthy != last_health:
                        fields: dict[str, object] = {"pid": child.pid, "ok": healthy}
                        if last_health is not None:
                            fields["was"] = last_health
                            fields["after_s"] = round(now - last_health_change, 1)
                        if not healthy and stage.get("name") is not None:
                            fields["stage"] = stage["name"]
                        emit(logger, "health", **fields)
                        last_health = healthy
                        last_health_change = now
                        next_heartbeat = now + HEALTH_HEARTBEAT_SECONDS
                    elif healthy and now >= next_heartbeat:
                        emit(logger, "health_heartbeat", pid=child.pid,
                             ok=True, healthy_for_s=round(now - last_health_change))
                        next_heartbeat = now + HEALTH_HEARTBEAT_SECONDS
                    if healthy and recovery_pending:
                        windows_balloon(f"{args.name} recovered", f"Server is healthy (pid {child.pid})")
                        emit(logger, "recovered", pid=child.pid)
                        recovery_pending = False
                    next_health = now + max(args.health_interval, 1.0)
                elif (args.no_health and recovery_pending and
                      time.time() - started_at >= min(args.stable_seconds, 10.0)):
                    windows_balloon(f"{args.name} recovered", f"Process is running (pid {child.pid})")
                    emit(logger, "recovered", pid=child.pid)
                    recovery_pending = False
                time.sleep(0.25)

            child_pid = child.pid
            return_code = child.wait()
            output.join(timeout=2.0)
            uptime = time.time() - started_at
            emit(logger, "child_exited", pid=child_pid, code=return_code, uptime_seconds=uptime)
            set_state(child_pid=0, child_exit_code=return_code)
            child = None
            if stopping.is_set():
                return 0
            if return_code == 0:
                emit(logger, "intentional_shutdown")
                return 0
            if (args.program is None and return_code == TUNE_RESTART_EXIT and
                    tune_restart_seen.is_set()):
                emit(logger, "tune_bootstrap_complete")
                failures = 0
                continue
            if return_code == CONFIG_RESTART_EXIT:
                emit(logger, "config_restart_relaunch")
                failures = 0
                continue

            wer = latest_wer_report(started_at)
            dump_path = (
                wait_for_recent_dump(args.dump_dir, child_exe_name, started_at)
                if dump_type == 1
                else None
            )
            failures = 0 if uptime >= args.stable_seconds else failures + 1
            delay = min(2.0 ** min(failures, 6), args.max_restart_delay)
            emit(
                logger,
                "crash",
                code=return_code,
                uptime_seconds=uptime,
                restart_delay_seconds=delay,
                wer=wer or "",
                dump=str(dump_path) if dump_path is not None else "",
            )
            set_state(restart_delay=delay)
            bundle_path: Path | None = None
            if not args.no_crash_bundle:
                try:
                    bundle_path = collect_crash_bundle(
                        args,
                        command,
                        child_pid,
                        return_code,
                        started_at,
                        wer,
                        dump_path,
                        jit_dlls,
                    )
                    emit(logger, "crash_bundle", path=str(bundle_path))
                except OSError as error:
                    emit(logger, "crash_bundle_failed", error=str(error))
            windows_balloon(
                f"{args.name} crashed",
                f"Exit {return_code}; restarting in {delay:.0f}s"
                + (f"\nDump: {dump_path}" if dump_path is not None else "\nNo minidump produced")
                + (f"\nBundle: {bundle_path}" if bundle_path is not None else ""),
                True,
            )
            recovery_pending = True
            stopping.wait(delay)
    finally:
        if child is not None and child.poll() is None:
            child.terminate()
            try:
                child.wait(timeout=10.0)
            except subprocess.TimeoutExpired:
                child.kill()
                child.wait(timeout=10.0)
        try:
            # Only clear the file if it is still ours. A second watchdog that took over the slot
            # owns it now, and deleting its claim would let a third start alongside it.
            if args.pid_file.read_text(encoding="ascii").strip() == str(os.getpid()):
                args.pid_file.unlink()
        except OSError:
            pass
        if stop_file is not None:
            try:
                stop_file.unlink()
            except OSError:
                pass
        emit(logger, "watchdog_stopped")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
