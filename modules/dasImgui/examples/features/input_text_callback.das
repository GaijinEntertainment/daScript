options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require strings

// =============================================================================
// FEATURE: input_text_callback — InputText with user-supplied event callback.
// SHOWS:   `input_text_callback(STATE, text, flags, cb)` — the lambda fires
//          synchronously from ImGui::InputText whenever a callback-event flag
//          (CallbackCompletion = TAB, CallbackHistory = Up/Down, etc.) is
//          tripped. The `data` pointer is valid only inside the call. This
//          demo wires CallbackCompletion to a tiny dictionary: pressing TAB
//          while typing prefix-completes the longest unique match.
// STANDALONE: daslang.exe modules/dasImgui/examples/features/input_text_callback.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/input_text_callback.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/input_text_callback.das
// =============================================================================

let private COMMANDS <- ["help", "history", "clear", "classify", "echo", "exit"]

def private completion_cb(var data : ImGuiInputTextCallbackData) : int {
    if (data.EventFlag == ImGuiInputTextFlags.CallbackCompletion) {
        let prefix = clone_string(data.Buf)
        var matches <- [for (c in COMMANDS); c; where c |> starts_with(prefix)]
        if (length(matches) == 1) {
            data |> DeleteChars(0, data.BufTextLen)
            data |> InsertChars(0, matches[0])
        }
        // Local arrays don't finalize on scope exit — without this delete each TAB-complete leaks the buffer.
        delete matches
    }
    return 0
}

[export]
def init() {
    harness_init("input_text_callback — TAB-complete", 520, 240)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(480.0, 200.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "input_text_callback", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text(LEAD, (text = "Type a prefix (e.g. 'he') and press TAB."))

        if (input_text_callback(CMD, "command",
                                ImGuiInputTextFlags.CallbackCompletion,
                                @(var data : ImGuiInputTextCallbackData) =>
                                    completion_cb(data))) {}

        separator(SEP)
        text(STATIC, (text = "Dictionary: help / history / clear / classify / echo / exit"))
    }

    harness_end_frame()
}

[export]
def shutdown() {
    harness_shutdown()
}

[export]
def main() {
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
