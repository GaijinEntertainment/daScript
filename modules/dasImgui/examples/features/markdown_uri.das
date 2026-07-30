options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require imgui/imgui_fonts
require imgui/imgui_markdown_text
require imgui/imgui_theme_daslang
require imgui/imgui_style_builtin

// =============================================================================
// FEATURE: markdown_uri — source-mapped Unicode flow + one semantic URI.
//
// Hover any visual fragment of a wrapped URI: all fragments underline and a
// pre-reserved copy control appears without reflowing the paragraph.
// Activating the URI is reported to the host; the component never launches it.
//
// STANDALONE: daslang.exe modules/dasImgui/examples/features/markdown_uri.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/markdown_uri.das -- --headless --headless-frames=60
// =============================================================================

var private g_font : ImFont?
var private g_font_sources : array<string>
var private g_status = "Hover a URI, copy it, or click it."

[export]
def init() {
    harness_init("dasImgui Markdown URI probe", 860, 620)
    g_font = load_unicode_ui_font(19.0f, g_font_sources)
    apply_daslang_theme(GetStyle())
}

def private report_result(name, target : string; result : MarkdownUriResult) {
    if (result.copied) {
        g_status = "Copied {name}: {target}"
    } elif (result.activated) {
        // No ShellExecute/open here — the containing app owns navigation policy.
        g_status = "Open requested for {name}: {target}"
    }
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowPos(ImVec2(20.0f, 20.0f), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(820.0f, 580.0f), ImGuiCond.Always)
    window(MARKDOWN_URI_WIN, (text = "Markdown URI flow probe", closable = false,
                              flags = ImGuiWindowFlags.None)) {
        with_font(g_font, 19.0f) {
            text("Markdown inline-flow foundation")
            text_disabled("One semantic URI → many visual fragments → one hover/click/copy action")
            separator()

            let web_target = "https://example.com/文档/naïve/é/very-long-resource-name?mode=full&emoji=👩‍💻#résumé"
            let web = markdown_uri_paragraph(
                "web", "A wrapped Unicode URI: ", web_target,
                " — followed by ordinary paragraph text.", web_target, 620.0f)
            report_result("web", web_target, web)

            spacing()
            let relative_target = "../guides/terminal-and-editor.md#font-fallback"
            let relative = markdown_uri_paragraph(
                "relative", "Relative Markdown target: ", relative_target,
                " (the host will resolve this against the document URI).",
                relative_target, 520.0f)
            report_result("relative", relative_target, relative)

            spacing()
            let mail_target = "mailto:unicode@example.org?subject=Привет-мир"
            let mail = markdown_uri_paragraph(
                "mail", "Custom display text still keeps its target: ",
                "contact the Unicode team", ".", mail_target, 520.0f)
            report_result("mail", mail_target, mail)

            separator()
            text_colored((color = float4(0.95f, 0.72f, 0.30f, 1.0f), text = g_status))
            text_disabled("The copy icon's slot is present in layout even while the icon is hidden.")
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
