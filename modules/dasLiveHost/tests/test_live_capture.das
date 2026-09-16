options gen2
require dastest/testing_boost public
require live/live_capture

var value = 7
var unwanted_calls = 0
def read_fixture(_args : JsonValue?) : JsonValue? {return JV((number = value))}
def validate_fixture(args : JsonValue?; var error : string&) : bool {
    if ((args?.number ?? -1) < 0 || (args?.number ?? -1) > 100){error = "fixture out of range"; return false}
    return true
}
def apply_fixture(args : JsonValue?){value = args?.number ?? 0}
[live_command]
def unrelated_mutation(_args : JsonValue?) : JsonValue? {unwanted_calls++; return JV(true)}
def setup_capture_fixture() {
    clear(capture_providers); clear(capture_diagnostics); clear(capture_history)
    capture_application = "fixture"; capture_build = "build-A"; value = 7; unwanted_calls = 0
    live_capture_provider(LiveCaptureProvider(name = "state", version = 1, read = @@read_fixture, validate = @@validate_fixture, apply = @@apply_fixture))
}
[test]
def test_capture_is_opt_in_and_restores(t : T?) {
    setup_capture_fixture()
    for (_i in range(300)){live_capture_event("input", "event")}
    var snapshot = live_capture(null)
    t |> equal(length(capture_history), 256)
    t |> equal(unwanted_calls, 0, "capture must not call unrelated live commands")
    value = 90
    var result = live_capture_restore(snapshot)
    t |> success(result?.ok ?? false, "matching snapshot restored")
    t |> equal(value, 7)
    delete_json(snapshot); delete_json(result)
}
[test]
def test_capture_validation_precedes_mutation(t : T?) {
    setup_capture_fixture(); value = 101
    var invalid = live_capture(null); value = 42
    var result = live_capture_restore(invalid)
    t |> success(!(result?.ok ?? true), "invalid provider rejected")
    t |> equal(value, 42, "rejected restore leaves state intact")
    delete_json(result); delete_json(invalid)
    var snapshot = live_capture(null)
    capture_build = "build-B"; value = 77
    result = live_capture_restore(snapshot)
    t |> success(!(result?.ok ?? true), "wrong build rejected")
    t |> equal(value, 77)
    delete_json(snapshot); delete_json(result)
}
