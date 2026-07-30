options gen2
options _comment_hygiene = true

require imgui/imgui_boost_v2
require imgui/imgui_widgets_builtin

//! Negative test — verbose named-tuple form `text((text=...))` must trip
//! STYLE001 (macro_error code 50503). Lint is default-on and bundled via
//! `imgui/imgui_boost_v2`; no opt-in line required.

expect 50503

[export]
def main() {
    text((text = "must fail with STYLE001"))
}
