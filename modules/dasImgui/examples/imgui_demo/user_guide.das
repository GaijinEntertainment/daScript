options gen2
options _comment_hygiene = true
options indenting = 4

module user_guide

require imgui
require imgui/imgui_widgets_builtin
require imgui/imgui_scope_builtin

// Mirrors imgui_demo.cpp:6968-6997 — void ImGui::ShowUserGuide().
def ShowUserGuide() {
    let io & = unsafe(GetIO())
    bullet_text("Double-click on title bar to collapse window.")
    bullet_text((text = "Click and drag on lower corner to resize window\n" +
                        "(double-click to auto fit window to its contents)."))
    bullet_text("CTRL+Click on a slider or drag box to input value as text.")
    bullet_text("TAB/SHIFT+TAB to cycle through keyboard editable fields.")
    bullet_text("CTRL+Tab to select a window.")
    if (io.FontAllowUserScaling) {
        bullet_text("CTRL+Mouse Wheel to zoom window contents.")
    }
    bullet_text("While inputing text:\n")
    with_indent(0.0f) {
        bullet_text("CTRL+Left/Right to word jump.")
        bullet_text("CTRL+A or double-click to select all.")
        bullet_text("CTRL+X/C/V to use clipboard cut/copy/paste.")
        bullet_text("CTRL+Z,CTRL+Y to undo/redo.")
        bullet_text("ESCAPE to revert.")
    }
    bullet_text("With keyboard navigation enabled:")
    with_indent(0.0f) {
        bullet_text("Arrow keys to navigate.")
        bullet_text("Space to activate a widget.")
        bullet_text("Return to input text into a widget.")
        bullet_text("Escape to deactivate a widget, close popup, exit child window.")
        bullet_text("Alt to jump to the menu layer of a window.")
    }
}
