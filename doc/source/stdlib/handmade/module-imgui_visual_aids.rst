Tutorial-mode overlays — highlight rectangles, mouse-cursor sprite + trail,
narration text, key HUD, click flash — painted over the live ImGui surface to
guide the viewer through scripted recordings. Every overlay is opt-in and
keyed off the boost registry's widget path, so highlighting
``MAIN_WIN/SETTINGS_TAB/RPS`` resolves to the exact rect the renderer drew.
Painters are registered as end-of-frame hooks so the host's render loop is
unchanged.

Auto-highlight (``imgui_auto_highlight_on_command``) makes every accepted
``imgui_*`` command auto-flash its target — useful when recording with
``imgui_playwright`` so each programmatic action is visually narrated
without explicit ``imgui_highlight`` calls.
