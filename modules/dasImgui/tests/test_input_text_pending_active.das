options gen2
options _allow_imgui_legacy = true // Headless regression feeds native input events without a platform backend.
options indenting = 4
require dastest/testing_boost public
require imgui/imgui_boost_v2
require imgui/imgui_boost_runtime
require imgui/imgui_containers_builtin
require imgui/imgui_widgets_builtin

var private EDITORS : table<int; InputTextState>

[test]
def pending_value_replaces_focused_editors(t : T?) {
    for (kind in range(5)) {
        let context = CreateContext(null)
        var io & = unsafe(GetIO())
        io.DisplaySize = float2(640, 480)
        io.BackendFlags |= ImGuiBackendFlags.RendererHasTextures
        for (frame in range(8)) {
            NewFrame()
            imgui_boost_runtime::begin_frame()
            window(PENDING_WINDOW, (text = "Pending text", closable = false, flags = ImGuiWindowFlags.None)) {
                if (frame == 0) { EDITORS[kind].pending_value = "old text"; EDITORS[kind].has_pending = true }
                if (frame == 1) { SetKeyboardFocusHere(0) }
                if (frame == 4) { EDITORS[kind].pending_value = "replacement 世界"; EDITORS[kind].has_pending = true } // nolint:STYLE039 UTF-8 replacement is the regression input.
                if (frame == 6) { EDITORS[kind].pending_value = ""; EDITORS[kind].has_pending = true }
                if (kind == 0) { input_text(EDITORS[kind], (text = "Editor")) }
                elif (kind == 1) { input_text_with_hint(EDITORS[kind], (text = "Editor", hint = "Hint")) }
                elif (kind == 2) { input_text_multiline(EDITORS[kind], (text = "Editor", size = float2(300, 120))) }
                elif (kind == 3) { input_text_growable(EDITORS[kind], (text = "Editor")) }
                else { input_text_callback(EDITORS[kind], "Editor", ImGuiInputTextFlags.CallbackCompletion, @(var _data : ImGuiInputTextCallbackData) : int { return 0 }) }
                if (frame == 3) { t |> success(IsItemActive(), "editor has focus before replacement") }
                if (frame == 5) { t |> equal(EDITORS[kind].value, "replacement 世界"); t |> success(IsItemActive(), "replacement preserves focus") } // nolint:STYLE039 UTF-8 replacement is the regression input.
                if (frame == 7) { t |> equal(EDITORS[kind].value, ""); t |> success(IsItemActive(), "clear preserves focus") }
            }
            EndFrame()
        }
        clear(g_registry); clear(g_widgets); clear(EDITORS)
        DestroyContext(context)
    }
}

[test]
def pending_edit_keeps_other_editor_cursor_and_undo_semantics(t : T?) {
    let context = CreateContext(null)
    var io & = unsafe(GetIO())
    io.DisplaySize = float2(640, 480)
    io.BackendFlags |= ImGuiBackendFlags.RendererHasTextures
    io.ConfigMacOSXBehaviors = false
    for (frame in range(10)) {
        if (frame == 3) { io |> AddKeyEvent(ImGuiKey.Home, true) }
        if (frame == 4) { io |> AddKeyEvent(ImGuiKey.Home, false); io |> AddInputCharactersUTF8("X") }
        if (frame == 7) { io |> AddKeyEvent(ImGuiKey.ImGuiMod_Ctrl, true); io |> AddKeyEvent(ImGuiKey.Z, true) }
        if (frame == 8) { io |> AddKeyEvent(ImGuiKey.Z, false); io |> AddKeyEvent(ImGuiKey.ImGuiMod_Ctrl, false) }
        NewFrame(); imgui_boost_runtime::begin_frame()
        window(OWNERS_WINDOW, (text = "Editor owners", closable = false, flags = ImGuiWindowFlags.None)) {
            if (frame == 0) { FIRST.pending_value = "original"; FIRST.has_pending = true }
            if (frame == 1) { SetKeyboardFocusHere(0) }
            if (frame == 6) { FIRST.pending_value = "replacement"; FIRST.has_pending = true }
            input_text(FIRST, (text = "First"))
            if (frame == 5) { t |> equal(FIRST.value, "Xoriginal", "inactive pending update preserves the focused cursor") }
            if (frame == 9) { t |> equal(FIRST.value, "Xoriginal", "ordinary external edits participate in undo") }
            if (frame == 3) { SECOND.pending_value = "Other editor"; SECOND.has_pending = true }
            input_text(SECOND, (text = "Second"))
        }
        EndFrame()
    }
    clear(g_registry); clear(g_widgets)
    DestroyContext(context)
}

[test]
def enter_only_result_preserves_live_text_state(t : T?) {
    for (kind in range(5)) {
        let context = CreateContext(null)
        var io & = unsafe(GetIO())
        io.DisplaySize = float2(640, 480)
        io.BackendFlags |= ImGuiBackendFlags.RendererHasTextures
        io.ConfigMacOSXBehaviors = false
        for (frame in range(8)) {
            if (frame == 3) { io |> AddInputCharactersUTF8("draft") }
            if (frame == 5) {
                if (kind == 2) { io |> AddKeyEvent(ImGuiKey.ImGuiMod_Ctrl, true) }
                io |> AddKeyEvent(ImGuiKey.Enter, true)
            }
            if (frame == 6) { io |> AddKeyEvent(ImGuiKey.Enter, false); io |> AddKeyEvent(ImGuiKey.ImGuiMod_Ctrl, false) }
            NewFrame(); imgui_boost_runtime::begin_frame()
            window(ENTER_WINDOW, (text = "Enter-only input", closable = false, flags = ImGuiWindowFlags.None)) {
                if (frame == 1) { SetKeyboardFocusHere(0) }
                var result = false
                let flags = ImGuiInputTextFlags.EnterReturnsTrue
                if (kind == 0) { result = input_text(EDITORS[kind], (text = "Editor", flags = flags)) }
                elif (kind == 1) { result = input_text_with_hint(EDITORS[kind], (text = "Editor", hint = "Hint", flags = flags)) }
                elif (kind == 2) { result = input_text_multiline(EDITORS[kind], (text = "Editor", size = float2(300, 120), flags = flags)) }
                elif (kind == 3) { result = input_text_growable(EDITORS[kind], (text = "Editor", flags = flags)) }
                else { result = input_text_callback(EDITORS[kind], "Editor", flags, @(var _data : ImGuiInputTextCallbackData) : int { return 0 }) }
                if (frame == 3) {
                    t |> equal(EDITORS[kind].value, "draft", "value follows typing before submission")
                    t |> success(EDITORS[kind].changed, "typing is an edit")
                    t |> success(!result, "typing does not submit")
                }
                if (frame == 4) { t |> success(!EDITORS[kind].changed, "idle is not another edit") }
                if (frame == 5) {
                    t |> success(result, "Enter retains its submission result")
                    t |> equal(EDITORS[kind].value, "draft")
                    t |> success(!EDITORS[kind].changed, "submitting unchanged text is not an edit")
                }
            }
            EndFrame()
        }
        clear(g_registry); clear(g_widgets); clear(EDITORS)
        DestroyContext(context)
    }
}
