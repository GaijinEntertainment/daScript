options gen2
require dastest/testing_boost public
require imgui/imgui_boost_runtime

def fixture_payload(_ctx : void?; _id : int) : JsonValue? {return JV((text = "snapshot payload"))}
[test]
def test_snapshot_transfers_payload_ownership(t : T?) {
    let context = CreateContext(null)
    g_registry["capture_fixture"] <- WidgetEntry(kind = "fixture", serialize = @@fixture_payload)
    var snapshot = imgui_snapshot(null)
    t |> success(g_registry["capture_fixture"].payload == null, "registry must not retain a pointer owned by returned JSON")
    t |> equal(snapshot?.globals?.capture_fixture?.payload?.text ?? "", "snapshot payload")
    delete_json(snapshot)
    clear(g_registry)
    DestroyContext(context)
}
