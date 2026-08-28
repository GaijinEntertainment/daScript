"""The consent surface: the `@sidecar consent` fold, the non-sticky "consent" stop mode, and
handle_consent_needed's write-then-stop contract (dialog mocked at the show_consent_dialog
seam — the native call itself is not unit-testable). Self-runnable:
python3 utils/watchdog/test_consent.py (pytest also works)."""

import argparse
import importlib.util
import logging
from pathlib import Path
import tempfile
import threading
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
    whether a tune stop was requested."""
    args = argparse.Namespace(name="t", log=consent_path.parent / "t.log")
    stopped = []
    saved = (wd.show_consent_dialog, wd.tune_in_flight, wd.request_tune_stop,
             wd.threading.Thread, wd.notify)
    wd.show_consent_dialog = lambda: answer
    wd.tune_in_flight = lambda: tune_running
    wd.request_tune_stop = lambda a, lg, mode: stopped.append(mode)
    wd.notify = lambda *a, **k: None

    class SyncThread:
        def __init__(self, target=None, **_kw):
            self._target = target

        def start(self):
            self._target()

    wd.threading.Thread = SyncThread
    try:
        wd.handle_consent_needed(args, logging.getLogger("t"), quote(str(consent_path)))
    finally:
        (wd.show_consent_dialog, wd.tune_in_flight, wd.request_tune_stop,
         wd.threading.Thread, wd.notify) = saved
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


if __name__ == "__main__":
    for name, fn in sorted(globals().items()):
        if name.startswith("test_"):
            fn()
            print(f"PASS {name}")
