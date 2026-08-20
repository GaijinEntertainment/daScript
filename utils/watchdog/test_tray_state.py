"""tray_state() wording: a child that has never been healthy is "starting up" (amber, with
plain-language stage labels); "unhealthy" (red) is reserved for a server that served and then
stopped answering. Run: python -m pytest utils/watchdog/test_tray_state.py"""

import importlib.util
from pathlib import Path

spec = importlib.util.spec_from_file_location("wd", Path(__file__).with_name("watchdog.py"))
wd = importlib.util.module_from_spec(spec)
spec.loader.exec_module(wd)


def state(**kv):
    base = {"child_pid": 1, "healthy": None, "serving_since": 0.0, "stage": None}
    base.update(kv)
    return base


def tray(monkeypatch, **kv):
    monkeypatch.setattr(wd, "read_state", lambda: state(**kv))
    return wd.tray_state()


def test_cold_start_is_starting_up_not_unhealthy(monkeypatch):
    line, cls = tray(monkeypatch, healthy=False, stage="jit_codegen")
    assert cls == "amber"
    assert line.startswith("starting up — ")
    assert "unhealthy" not in line


def test_every_stage_gets_a_plain_language_label(monkeypatch):
    for stage, label in wd.STAGE_LABELS.items():
        line, cls = tray(monkeypatch, healthy=False, stage=stage)
        assert cls == "amber"
        assert line == f"starting up — {label}"


def test_unknown_stage_passes_through_and_no_stage_is_bare(monkeypatch):
    line, _ = tray(monkeypatch, stage="model_warmup")
    assert line == "starting up — model_warmup"
    line, _ = tray(monkeypatch, stage=None)
    assert line == "starting up"


def test_unhealthy_requires_having_served(monkeypatch):
    line, cls = tray(monkeypatch, healthy=False, serving_since=123.0, stage="ready")
    assert cls == "red"
    assert line == "unhealthy (ready)"


def test_serving_line_untouched(monkeypatch):
    line, cls = tray(monkeypatch, healthy=True, serving_since=1.0)
    assert cls == "base"
    assert line.startswith("serving · healthy")
