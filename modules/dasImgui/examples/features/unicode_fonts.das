options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require imgui/imgui_theme_daslang
require imgui/imgui_fonts
require imgui/imgui_style_builtin
require imgui/imgui_table_builtin
require strings

// =============================================================================
// FEATURE: unicode_fonts — UTF-8 text, explicit font fallback, and colored glyphs.
//
// Windows Terminal uses DirectWrite font fallback. Dear ImGui does not: every
// fallback face has to be merged into the atlas explicitly. This probe uses the
// bundled JetBrains Mono as its base and merges common platform symbol/CJK
// faces when they are available.
//
// STANDALONE: daslang.exe modules/dasImgui/examples/features/unicode_fonts.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/unicode_fonts.das -- --headless --headless-frames=60
// =============================================================================

var private g_font : ImFont?
var private g_loaded_sources : array<string>
var private g_sources_text : string
var private g_has_grinning_face : bool

[export]
def init() {
    harness_init("dasImgui Unicode font probe", 1000, 760)
    g_font = load_unicode_ui_font(20.0f, g_loaded_sources)
    g_has_grinning_face = font_has_glyph(g_font, 0x1F600u)
    g_sources_text = build_string() $(var writer) {
        for (source in g_loaded_sources) {
            write(writer, "  • {source}\n")
        }
    }
    GetStyle().FontScaleMain = 1.0f
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowPos(ImVec2(20.0f, 20.0f), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(960.0f, 720.0f), ImGuiCond.Always)
    window(UNICODE_WIN, (text = "Unicode font probe", closable = false,
                         flags = ImGuiWindowFlags.None)) {
        with_font(g_font, 20.0f) {
            text("UTF-8 text with explicit Dear ImGui font fallback")
            separator()

            text("Latin:     déjà vu · Straße · smørrebrød · naïve · ĀČĘ")
            text("Greek:     Ελληνικά · Καλημέρα κόσμε")
            text("Cyrillic:  Русский · Привет, мир")
            text("CJK:       日本語 · 简体中文 · 한국어")
            text("Hebrew:    עברית · שלום עולם  (glyph check; bidi is separate)")
            text("Arabic:    العربية · مرحبا بالعالم  (glyph check; shaping is separate)")
            text("Combining: é / é · Å / Å")

            separator()
            text("Unicode box drawing (joins should be continuous):")
            // Consolas' box glyph is one pixel taller than ImGui's multiline
            // advance at this size. A 3 px inter-item advance makes rows meet
            // without the overlap of a multiline string or normal UI spacing.
            with_style((ImGuiStyleVar.ItemSpacing, float2(GetStyle().ItemSpacing.x, 3.0f))) {
                text("┌───────┬───────┐   ╭─────╮   ╔═════╦═════╗")
                text("│ Alpha │ Beta  │   │  42 │   ║  A  ║  B  ║")
                text("├───────┼───────┤   ├─────┤   ╠═════╬═════╣")
                text("│   42  │ ready │   │  ok │   ║ yes ║  no ║")
                text("└───────┴───────┘   ╰─────╯   ╚═════╩═════╝")
            }
            text("Wide glyphs use natural UI advances here: 中文 · 😀 🚀")

            separator()
            text("Terminal symbols:  ✓  ✗  ⚠  ⓘ  →  ←  •  ◆  ▶  ⏱  ⌁")
            text("Powerline PUA:      prompt     branch")

            text("Tinted glyphs:"); same_line()
            text_colored((color = float4(0.35f, 0.90f, 0.45f, 1.0f), text = "✓ success")); same_line()
            text_colored((color = float4(1.00f, 0.72f, 0.25f, 1.0f), text = "⚠ warning")); same_line()
            text_colored((color = float4(1.00f, 0.35f, 0.35f, 1.0f), text = "✗ error")); same_line()
            text_colored((color = float4(0.45f, 0.75f, 1.00f, 1.0f), text = "ⓘ info"))

            separator()
            text("Supplementary plane: 😀  🚀  🧪  🎨")
            if (g_has_grinning_face) {
                text_colored((color = float4(0.35f, 0.90f, 0.45f, 1.0f), text = "WCHAR32 check: U+1F600 is present in the merged font."))
            } else {
                text_colored((color = float4(1.00f, 0.72f, 0.25f, 1.0f), text = "WCHAR32 check passed; the merged fonts do not provide U+1F600."))
            }

            separator()
            text("ImGui table with mixed scripts and color glyphs:")
            data_table(UNICODE_TABLE, (text = "unicode_table", columns = 4,
                                       flags = ImGuiTableFlags.Borders |
                                               ImGuiTableFlags.RowBg |
                                               ImGuiTableFlags.SizingStretchProp,
                                       outer_size = float2(0.0f, 0.0f), inner_width = 0.0f)) {
                table_setup_column("Script")
                table_setup_column("Sample")
                table_setup_column("Status")
                table_setup_column("Emoji")
                table_headers_row()

                table_next_row()
                if (table_set_column_index(0)) text("Latin")
                if (table_next_column()) text("déjà vu")
                if (table_next_column()) text_colored((color = float4(0.35f, 0.90f, 0.45f, 1.0f), text = "✓ ready"))
                if (table_next_column()) text("😀 🎨")

                table_next_row()
                if (table_set_column_index(0)) text("CJK")
                if (table_next_column()) text("日本語 · 简体中文")
                if (table_next_column()) text_colored((color = float4(1.00f, 0.72f, 0.25f, 1.0f), text = "⚠ check"))
                if (table_next_column()) text("🚀 🧪")

                table_next_row()
                if (table_set_column_index(0)) text("RTL glyphs")
                if (table_next_column()) text("שלום · مرحبا")
                if (table_next_column()) text_colored((color = float4(0.45f, 0.75f, 1.00f, 1.0f), text = "ⓘ glyphs"))
                if (table_next_column()) text("🌍 💡")
            }

            separator()
            text("Merged font sources:")
            text_wrapped((text = g_sources_text))
        }
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
