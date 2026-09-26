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

[test]
def test_capture_flush_roundtrip_and_timings(t : T?) {
    setup_capture_fixture()
    let directory = "/tmp/das-capture-timing-test"
    live_capture_configure("fixture", "build-A", directory)
    capture_request_id = "quoted-\"-identifier"
    capture_pending = true
    let before = capture_completed
    live_capture_flush(123)
    var error : string
    let response_text = fread("{directory}/response.json")
    var response = read_json(response_text, error)
    t |> equal(error, "", "transport JSON remains valid with escaped metadata")
    t |> equal(response?.id ?? "", "quoted-\"-identifier")
    t |> equal(response?.snapshot?.frame ?? 0, 123)
    t |> equal(response?.snapshot?.providers?.state?.state?.number ?? 0, 7)
    t |> success(key_exists(response, "timings"))
    t |> success(capture_timings.total_ms >= capture_timings.snapshot_ms)
    t |> equal(capture_completed, before + 1)
    var saved = read_json(fread(capture_last_file), error)
    value = 99
    var result = live_capture_restore(saved)
    t |> success(result?.ok ?? false)
    t |> equal(value, 7, "saved and transported snapshot retain restoration state")
    delete_json(saved); delete_json(result); delete_json(response)
    remove(capture_last_file); remove("{directory}/response.json")
    capture_last_file = ""
}

def custom_capture_artifact(_file : string) : bool {
    capture_artifact_file = "custom.rgba"; capture_artifact_format = "rgba"
    capture_artifact_width = 17; capture_artifact_height = 9
    return true
}
def failed_capture_artifact(_file : string) : bool { return false }
[test]
def test_capture_artifact_metadata_and_failure(t : T?) {
    setup_capture_fixture()
    let directory = "/tmp/das-capture-artifact-test"
    live_capture_configure("fixture", "build-A", directory)
    let previous_artifact = capture_artifact
    capture_artifact = @@custom_capture_artifact
    live_capture_request(); live_capture_flush(124)
    let first_file = clone(capture_last_file)
    var error : string
    var response = read_json(fread("{directory}/response.json"), error)
    t |> equal(error, "")
    t |> equal(response?.image ?? "", "custom.rgba")
    t |> equal(response?.image_format ?? "", "rgba")
    t |> equal(response?.image_width ?? 0, 17)
    t |> equal(response?.image_height ?? 0, 9)
    delete_json(response)
    capture_artifact = @@failed_capture_artifact
    live_capture_request(); live_capture_flush(125)
    response = read_json(fread("{directory}/response.json"), error)
    t |> equal(error, "")
    t |> equal(response?.image ?? "", "", "failed image capture still yields a usable report")
    t |> equal(response?.image_width ?? -1, 0, "metadata from earlier capture is cleared")
    t |> equal(response?.snapshot?.frame ?? 0, 125)
    t |> equal(fexist(first_file), get_running_platform_name() != "emscripten", "browser keeps only the latest file; native retains earlier captures")
    delete_json(response)
    remove(first_file); remove(capture_last_file); remove("{directory}/response.json")
    capture_last_file = ""; capture_artifact = previous_artifact
}
