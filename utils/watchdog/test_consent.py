"""The consent surface: the `@sidecar consent` fold, the non-sticky "consent" stop mode, and
handle_consent_needed's write-then-stop contract (dialog mocked at the show_consent_dialog
seam — the native call itself is not unit-testable). Self-runnable:
python3 utils/watchdog/test_consent.py (pytest also works)."""

import argparse
import importlib.util
import logging
from pathlib import Path
import tempfile
from types import SimpleNamespace
from urllib.parse import quote

spec = importlib.util.spec_from_file_location("wd", Path(__file__).with_name("watchdog.py"))
wd = importlib.util.module_from_spec(spec)
spec.loader.exec_module(wd)


def test_consent_event_folds_into_sidecar_state():
    sidecar = {}
    wd.apply_sidecar_event(sidecar, "consent", {"state": "needed", "path": "x"})
    assert sidecar["consent"] == "needed"
    wd.apply_sidecar_event(sidecar, "consent", {"state": "accepted"})
    assert sidecar["consent"] == "accepted"


def test_consent_stop_mode_is_one_shot_and_env_free():
    # "consent" must relaunch CLEAN: no DASLLAMA_EXCHANGE_ACCEPT widening (that is
    # use_sidecar's one-shot), no sticky DAS_TUNE_POLICY hold (that is run-untuned's)
    with tempfile.TemporaryDirectory() as td:
        args = argparse.Namespace(name="t", log=Path(td) / "t.log")
        saved_notify, saved_set = wd.notify, wd.set_state
        armed = {}
        wd.notify = lambda *a, **k: None
        wd.set_state = lambda **kv: armed.update(kv)
        try:
            wd.request_tune_stop(args, logging.getLogger("t"), "consent")
        finally:
            wd.notify, wd.set_state = saved_notify, saved_set
        action = armed["tune_stop"]
        assert action["mode"] == "consent"
        assert action["env"] == {}
        assert action["sticky"] is False
        assert (Path(td) / "t.tune-stop").read_text() == "stop\n"


def run_handler(answer, tune_running, consent_path):
    """Drive handle_consent_needed with the dialog seam and thread spawn mocked; returns
    the list of stop modes requested."""
    args = argparse.Namespace(name="t", log=consent_path.parent / "t.log")
    stopped = []
    saved = (wd.show_consent_dialog, wd.tune_in_flight, wd.request_tune_stop,
             wd.threading, wd.notify)
    wd.show_consent_dialog = lambda: answer
    wd.tune_in_flight = lambda: tune_running
    wd.request_tune_stop = lambda a, lg, mode: stopped.append(mode)
    wd.notify = lambda *a, **k: None

    class SyncThread:
        def __init__(self, target=None, **_kw):
            self._target = target

        def start(self):
            self._target()

    # rebind the MODULE's threading attribute - never mutate the shared stdlib module
    wd.threading = SimpleNamespace(Thread=SyncThread)
    try:
        wd.handle_consent_needed(args, logging.getLogger("t"), quote(str(consent_path)))
    finally:
        (wd.show_consent_dialog, wd.tune_in_flight, wd.request_tune_stop,
         wd.threading, wd.notify) = saved
    return stopped


def test_accept_writes_file_and_stops_a_running_tune():
    with tempfile.TemporaryDirectory() as td:
        p = Path(td) / "main.tune.consent"
        stopped = run_handler(True, True, p)
        assert p.read_text() == "accepted\n"
        assert stopped == ["consent"]


def test_accept_with_no_tune_in_flight_only_records():
    with tempfile.TemporaryDirectory() as td:
        p = Path(td) / "main.tune.consent"
        stopped = run_handler(True, False, p)
        assert p.read_text() == "accepted\n"
        assert stopped == []


def test_decline_records_and_never_stops_the_tune():
    with tempfile.TemporaryDirectory() as td:
        p = Path(td) / "main.tune.consent"
        stopped = run_handler(False, True, p)
        assert p.read_text() == "declined\n"
        assert stopped == []


def test_walkaway_answers_nothing():
    # None (timeout / closed / no surface) must leave the question open: no file, no stop
    with tempfile.TemporaryDirectory() as td:
        p = Path(td) / "main.tune.consent"
        stopped = run_handler(None, True, p)
        assert not p.exists()
        assert stopped == []


def test_stream_child_dispatches_the_dialog_once_per_path():
    # the @sidecar consent event must reach handle_consent_needed exactly once per consent
    # file for the watchdog's lifetime - a noise/untuned boot loop re-emits it every restart
    class FakeProc:
        def __init__(self, lines):
            self.stdout = iter(lines)
            self.pid = 42
    lines = [
        "@sidecar consent state=needed path=a%2Fb.consent\n",
        "@sidecar consent state=needed path=a%2Fb.consent\n",
        "@sidecar consent state=needed path=other.consent\n",
    ]
    calls = []
    args = argparse.Namespace(name="t", log=Path("/tmp/t.log"))
    saved = (wd.handle_consent_needed, wd.notify)
    wd.handle_consent_needed = lambda a, lg, path: calls.append(path)
    wd.notify = lambda *a, **k: None
    wd._BALLOONED.clear()
    try:
        wd.stream_child(FakeProc(lines), logging.getLogger("t"),
                        __import__("threading").Event(), set(), {}, "t", args)
    finally:
        wd.handle_consent_needed, wd.notify = saved
        wd._BALLOONED.clear()
    assert calls == ["a%2Fb.consent", "other.consent"]


def test_darwin_dialog_parse_arms():
    # the osascript OUTPUT parse is unit-testable even though showing the dialog is not:
    # Accept/Decline must map to True/False, gave-up and Escape must leave the question open
    if not hasattr(wd, "show_consent_dialog") or wd.IS_WINDOWS or wd.sys.platform != "darwin":
        return  # the parse under test is the darwin arm
    def fake_run(stdout):
        return SimpleNamespace(stdout=stdout)
    saved = wd.subprocess
    outs = {}
    wd.subprocess = SimpleNamespace(
        run=lambda *a, **k: fake_run(outs["v"]), TimeoutExpired=Exception)
    try:
        for text, want in [("button returned:Accept", True),
                           ("button returned:Decline", False),
                           ("button returned:Accept, gave up:true", None),
                           ("", None)]:
            outs["v"] = text
            assert wd.show_consent_dialog() is want, (text, want)
    finally:
        wd.subprocess = saved


if __name__ == "__main__":
    for name, fn in sorted(globals().items()):
        if name.startswith("test_"):
            fn()
            print(f"PASS {name}")
