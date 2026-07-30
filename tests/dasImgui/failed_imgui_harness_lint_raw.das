options gen2
options _comment_hygiene = true

require imgui/imgui_harness
// Explicit backend require — bypasses the structural private-require gate
// the harness sets up. The HARNESS001 lint is the second line of defense.
require live/glfw_live

//! Negative test — default-on harness lint must trip HARNESS001
//! (macro_error code 50503) when a harness-using file forces a backend
//! require to bypass the structural gate, then calls a backend symbol
//! directly (here: ``live_create_window`` from ``glfw_live``).
//! No opt-in line required; lint is bundled via ``imgui/imgui_harness``
//! transitively through ``require imgui/imgui_harness_lint public``.

expect 50503

[export]
def main() {
    live_create_window("must fail with HARNESS001", 320, 240)
}
