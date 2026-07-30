options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: internal_separator_text — the imgui_internal.h separator / low-level
//          text primitives below the public Separator / Text API. Cherry-picked
//          into the v2 binding: SeparatorEx (raw, internal_allow_func — its
//          ImGuiSeparatorFlags binds as int, cast at the call site); TextEx
//          (nullable text_end) and SeparatorTextEx (nullable label_end) are manual
//          C++ wrappers (dasIMGUI.main.cpp) that pin the terminus to nullptr. New
//          internal enums ImGuiSeparatorFlags (Horizontal / Vertical / SpanAllColumns)
//          and ImGuiTextFlags (NoWidthForLargeClippedText). This is the "binding is
//          usable" gate: it drives the real bound API at runtime.
// SHOWS:   SeparatorEx with varying thickness + a vertical separator between
//          same-line items; TextEx (the low-level text renderer, with / without its
//          layout flag); and SeparatorTextEx — a separator carrying an inline label.
// STANDALONE: daslang.exe modules/dasImgui/examples/features/internal_separator_text.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/internal_separator_text.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/internal_separator_text.das
// =============================================================================

[export]
def init() {
    harness_init("internal_separator_text - SeparatorEx / TextEx / SeparatorTextEx", 760, 540)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.25
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(720.0, 500.0), ImGuiCond.Always)
    window(ST_WIN, (text = "internal_separator_text", closable = false,
                    flags = ImGuiWindowFlags.None)) {

        // ----- SeparatorEx: axis + thickness -----
        text("SeparatorEx(flags, thickness): horizontal separators of varying thickness.")
        text("thin (1px):")
        SeparatorEx(int(ImGuiSeparatorFlags.Horizontal), 1.0)
        text("medium (3px):")
        SeparatorEx(int(ImGuiSeparatorFlags.Horizontal), 3.0)
        text("thick (6px):")
        SeparatorEx(int(ImGuiSeparatorFlags.Horizontal), 6.0)
        text("a vertical SeparatorEx between two same-line items:")
        text("left")
        same_line()
        SeparatorEx(int(ImGuiSeparatorFlags.Vertical), 2.0)
        same_line()
        text("right")
        separator()

        // ----- TextEx: the low-level text renderer with layout flags -----
        text("TextEx(text, flags): the low-level text renderer (ImGuiTextFlags).")
        TextEx("plain TextEx, no flags")
        TextEx("TextEx with NoWidthForLargeClippedText", int(ImGuiTextFlags.NoWidthForLargeClippedText))
        separator()

        // ----- SeparatorTextEx: a separator carrying an inline label -----
        text("SeparatorTextEx(id, label, extra_width): a separator with an inline label.")
        SeparatorTextEx(GetID("sec1"), "Section One", 0.0)
        text("content under section one")
        SeparatorTextEx(GetID("sec2"), "Section Two (extra_width 40)", 40.0)
        text("content under section two")
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
