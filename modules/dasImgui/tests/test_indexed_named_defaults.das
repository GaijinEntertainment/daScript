options gen2
options indenting = 4
require dastest/testing_boost public
require imgui/imgui_boost_v2
require imgui/imgui_boost_runtime
require imgui/imgui_containers_builtin

struct NamedDefaultsState {
    first : int
    caption : string
    extent : float2
}
var private NAMED_ROWS : table<int; NamedDefaultsState>
[widget]
def named_defaults_probe(var state : NamedDefaultsState; first : int = 7; caption : string = "default"; extent : float2 = float2(1, 2)) {
    state.first = first; state.caption = caption; state.extent = extent
    PopID()
}
[test]
def indexed_named_arguments_skip_defaults_and_follow_names(t : T?) {
    let context = CreateContext(null)
    var io & = unsafe(GetIO())
    io.DisplaySize = float2(640, 480)
    io.BackendFlags |= ImGuiBackendFlags.RendererHasTextures
    NewFrame()
    window(TEST_WINDOW, (text = "test", closable = false, flags = ImGuiWindowFlags.None)) {
        named_defaults_probe(NAMED_ROWS[0], (extent = float2(30, 40)))
        named_defaults_probe(NAMED_ROWS[1], (caption = "second", first = 19))
        named_defaults_probe(NAMED_ROWS[2], 23, (extent = float2(50, 60)))
        named_defaults_probe(NAMED_ROWS[3], (caption = "named"), 31, float2(70, 80))
        named_defaults_probe(MIXED_SINGLE, 29, (extent = float2(90, 100)))
        t |> equal(NAMED_ROWS[2].first, 23)
        t |> equal(NAMED_ROWS[2].caption, "default")
        t |> equal(NAMED_ROWS[2].extent, float2(50, 60))
        t |> equal(NAMED_ROWS[3].first, 31)
        t |> equal(NAMED_ROWS[3].caption, "named")
        t |> equal(NAMED_ROWS[3].extent, float2(70, 80))
        t |> equal(MIXED_SINGLE.first, 29)
        t |> equal(MIXED_SINGLE.caption, "default")
        t |> equal(MIXED_SINGLE.extent, float2(90, 100))
        t |> equal(NAMED_ROWS[0].first, 7)
        t |> equal(NAMED_ROWS[0].caption, "default")
        t |> equal(NAMED_ROWS[0].extent, float2(30, 40))
        t |> equal(NAMED_ROWS[1].first, 19)
        t |> equal(NAMED_ROWS[1].caption, "second")
        t |> equal(NAMED_ROWS[1].extent, float2(1, 2))
    }
    EndFrame()
    clear(g_registry); clear(g_widgets); clear(NAMED_ROWS)
    DestroyContext(context)
}
