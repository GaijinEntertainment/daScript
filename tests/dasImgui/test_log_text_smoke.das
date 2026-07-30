options gen2
options _comment_hygiene = true

require imgui
require imgui/imgui_widgets_builtin
require imgui/imgui_lint

//! Resolution smoke for the imperative-clipboard trio. LogText outside an
//! active LogTo* bracket is harmless (ImGui drops it); we only assert the
//! symbol resolves and lint accepts the snake_case form.

[export]
def main() {
    log_to_clipboard()
    log_text("hello")
    log_finish()
}
