options gen2
options _comment_hygiene = true
options _allow_imgui_legacy = true

require imgui
require imgui/imgui_lint

//! Opt-out smoke — `imgui_lint` is loaded and default-on, but
//! `_allow_imgui_legacy = true` disables it. Raw imgui::Button compiles cleanly.

[export]
def main() {
    let _ = Button("opt-out test", float2(0.0f, 0.0f))
}
