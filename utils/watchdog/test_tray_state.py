"""tray_state() wording: a child that has never been healthy is "starting up" (amber, with
plain-language stage labels); "unhealthy" (red) is reserved for a server that served and then
stopped answering. Self-runnable: python3 utils/watchdog/test_tray_state.py (pytest also works)."""

import importlib.util
from pathlib import Path

spec = importlib.util.spec_from_file_location("wd", Path(__file__).with_name("watchdog.py"))
wd = importlib.util.module_from_spec(spec)
spec.loader.exec_module(wd)


def tray(**kv):
    state = {"child_pid": 1, "healthy": None, "serving_since": 0.0, "stage": None}
    state.update(kv)
    saved = wd.read_state
    wd.read_state = lambda: state
    try:
        return wd.tray_state()
    finally:
        wd.read_state = saved


def test_cold_start_is_starting_up_not_unhealthy():
    line, cls = tray(healthy=False, stage="jit_codegen")
    assert cls == "amber"
    assert line.startswith("starting up — ")
    assert "unhealthy" not in line


def test_every_stage_gets_a_plain_language_label():
    for stage, label in wd.STAGE_LABELS.items():
        line, cls = tray(healthy=False, stage=stage)
        assert cls == "amber"
        assert line == f"starting up — {label}"


def test_unknown_stage_passes_through_and_no_stage_is_bare():
    line, _ = tray(stage="model_warmup")
    assert line == "starting up — model_warmup"
    line, _ = tray(stage=None)
    assert line == "starting up"


def test_unhealthy_requires_having_served():
    line, cls = tray(healthy=False, serving_since=123.0, stage="ready")
    assert cls == "red"
    assert line == "unhealthy (ready)"


def test_serving_line_untouched():
    line, cls = tray(healthy=True, serving_since=1.0)
    assert cls == "base"
    assert line.startswith("serving · healthy")


if __name__ == "__main__":
    failed = 0
    for name, fn in sorted(globals().items()):
        if name.startswith("test_") and callable(fn):
            try:
                fn()
                print(f"PASS {name}")
            except AssertionError as e:
                failed += 1
                print(f"FAIL {name}: {e}")
    raise SystemExit(1 if failed else 0)
