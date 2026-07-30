options gen2
options _comment_hygiene = true

require imgui
require imgui/imgui_lint

//! Negative test — default-on lint + raw `imgui::Button` must trip
//! IMGUI002 (macro_error code 50503). No opt-in line required.

expect 50503

[export]
def main() {
    let _ = Button("must fail with IMGUI002", float2(0.0f, 0.0f))
}
