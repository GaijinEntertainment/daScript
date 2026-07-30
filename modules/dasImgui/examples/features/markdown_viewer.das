options gen2
options _comment_hygiene = true
options gc
options persistent_heap

require imgui/imgui_harness
require imgui/imgui_commands
require imgui/imgui_fonts
require imgui/imgui_markdown_tree_sitter
require imgui/imgui_theme_daslang
require imgui/imgui_style_builtin
require daslib/safe_addr
require daslib/stringify
require daslib/json
require daslib/json_boost
require live/live_commands
require math
require strings

// =============================================================================
// FEATURE: markdown_viewer — typed MD4C model + source-mapped ImGui rendering.
//
// Link activation is reported to the host status line; this probe deliberately
// does not launch a browser. Drag-select any rendered inline text, then
// right-click the highlight to exercise source/style/screen comment anchors.
//
// STANDALONE: daslang.exe modules/dasImgui/examples/features/markdown_viewer.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/markdown_viewer.das -- --headless --headless-frames=60
// =============================================================================

var private g_fonts = FontRoleSet()
var private g_font_sources : array<string>
var private g_doc = RichDocument()
var private g_view = MarkdownViewState()
var private g_tree_sitter = MarkdownTreeSitterState()
var private g_status = "Drag to select text; right-click a selection for its comment anchor."
var private g_zoom_percent = 100
var private g_context_source = TextSourceRange(start_byte = -1, end_byte = -1)
var private g_last_selection = MarkdownSelection()
var private g_add_comment_hovered = false
var private g_last_hovered_node = -1
var private g_last_hovered_image = false
var private g_last_hovered_target = ""
var private g_commands = CommandRegistry()

let SYNTAX_DEFAULT_LABELS : array<string> <- [
    "Auto (fence)", "Plain text", "daScript", "C", "C++", "Markdown"
]
let SYNTAX_DEFAULT_VALUES : array<string> <- [
    "auto", "none", "das", "c", "cpp", "markdown"
]
let SYNTAX_BLOCK_LABELS : array<string> <- [
    "Inherit default", "Auto (fence)", "Plain text",
    "daScript", "C", "C++", "Markdown"
]
let SYNTAX_BLOCK_VALUES : array<string> <- [
    "inherit", "auto", "none", "das", "c", "cpp", "markdown"
]

struct MarkdownProbeFragmentArgs {
    text : string
}

struct MarkdownProbeZoomArgs {
    percent : int
}

struct MarkdownProbeBindImageArgs {
    width : float = 48.0f
    height : float = 36.0f
    resource_revision : int = 1
}

def private first_image_node() : int {
    for (node_index in range(length(g_doc.nodes))) {
        if (g_doc.nodes[node_index].kind == RichNodeKind.image) return node_index
    }
    return -1
}

def private first_code_block_node() : int {
    for (node_index in range(length(g_doc.nodes))) {
        if (g_doc.nodes[node_index].kind == RichNodeKind.code_block) return node_index
    }
    return -1
}

def private bind_demo_image(width, height : float;
                            resource_revision : int) : bool {
    let node_index = first_image_node()
    if (node_index < 0) return false
    let atlas_id = image_texture_id(unsafe(GetIO()).Fonts.TexRef)
    return markdown_view_bind_image(g_view, MarkdownImageBinding(
        document_revision = 0,
        node_index = node_index,
        resource_revision = resource_revision,
        texture_id = atlas_id == 0uL ? 1uL : atlas_id,
        display_size = float2(max(width, 1.0f), max(height, 1.0f))))
}

[live_command(description = "Inspect Markdown render, hover, selection, and event revisions without a screenshot.")]
def markdown_probe_state(_input : JsonValue?) : JsonValue? {
    let s = g_last_selection
    let io & = unsafe(GetIO())
    let copy_modifier_down = io.ConfigMacOSXBehaviors ? io.KeySuper : io.KeyCtrl
    return JV((
        ok = g_doc.ok,
        render_revision = g_view.render_revision,
        hover_revision = g_view.hover_revision,
        hovered_inline_node = g_view.hovered_inline_node,
        hovered_fragment = g_view.hovered_fragment,
        hovered_display_byte = g_view.hovered_display_byte,
        selection = (
            active = s.active,
            dragging = g_view.selection_dragging,
            revision = g_view.selection_revision,
            commit_revision = g_view.selection_commit_revision,
            copy_revision = g_view.selection_copy_revision,
            copy_text = g_view.selection_copy_text,
            copy_markdown = g_view.selection_copy_markdown,
            copy_html = g_view.selection_copy_html,
            copy_uri_list = g_view.selection_copy_uri_list,
            context_revision = g_view.selection_context_revision,
            navigation_revision = g_view.navigation_revision,
            navigation_scroll_revision = g_view.navigation_scroll_revision,
            navigation_scroll_target = g_view.navigation_scroll_target,
            source_start = s.source.start_byte,
            source_end = s.source.end_byte,
            display_text = markdown_selection_text(g_view),
            source_text = s.active ? md_source_text(g_doc, s.source) : "",
            all_regular = s.styles.all_regular,
            has_strong = s.styles.has_strong,
            has_emphasis = s.styles.has_emphasis,
            has_code = s.styles.has_code,
            has_strikethrough = s.styles.has_strikethrough,
            has_links = s.styles.has_links,
            has_images = s.styles.has_images,
            has_mixed_styles = s.styles.has_mixed_styles,
            screen_valid = s.screen_valid,
            screen_x0 = s.screen_bounds.x,
            screen_y0 = s.screen_bounds.y,
            screen_x1 = s.screen_bounds.z,
            screen_y1 = s.screen_bounds.w,
            paint_rect_count = g_view.selection_paint_rect_count,
            anchor_x = s.screen_anchor.x,
            anchor_y = s.screen_anchor.y),
        caret = (
            valid = g_view.selection_focus.valid && g_view.caret_screen_valid,
            inline_node = g_view.selection_focus.inline_node,
            display_byte = g_view.selection_focus.display_byte,
            source_byte = g_view.selection_focus.source_byte,
            x = g_view.caret_screen_pos.x,
            y = g_view.caret_screen_pos.y,
            height = g_view.caret_screen_height),
        viewport_top = g_view.viewport_top,
        viewport_bottom = g_view.viewport_bottom,
        context_x = g_view.selection_context_pos.x,
        context_y = g_view.selection_context_pos.y,
        context_source_start = g_context_source.start_byte,
        context_source_end = g_context_source.end_byte,
        add_comment_hovered = g_add_comment_hovered,
        input = (
            right_down = IsMouseDown(ImGuiMouseButton.Right),
            copy_modifier_down = copy_modifier_down,
            key_c_down = IsKeyDown(ImGuiKey.C),
            shift_down = io.KeyShift,
            left_down = IsKeyDown(ImGuiKey.LeftArrow),
            right_arrow_down = IsKeyDown(ImGuiKey.RightArrow),
            up_down = IsKeyDown(ImGuiKey.UpArrow),
            down_down = IsKeyDown(ImGuiKey.DownArrow),
            home_down = IsKeyDown(ImGuiKey.Home),
            end_down = IsKeyDown(ImGuiKey.End),
            macos_behaviors = io.ConfigMacOSXBehaviors),
        layout_hits = g_view.layout_hits,
        layout_misses = g_view.layout_misses,
        image_bindings_revision = g_view.image_bindings_revision,
        semantic_hover = (
            node = g_last_hovered_node,
            image = g_last_hovered_image,
            target = g_last_hovered_target),
        zoom_percent = g_zoom_percent))
}

[live_command(description = "Find a currently rendered Markdown text fragment and return its exact screen rectangle.")]
def markdown_probe_fragment(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<MarkdownProbeFragmentArgs>)
    for (node_index in range(length(g_view.nodes))) {
        let entry & = unsafe(g_view.nodes[node_index])
        if (!entry.content_valid || !entry.layout_valid
                || entry.screen_revision != g_view.render_revision) continue
        for (fragment_index in range(length(entry.layout.fragments))) {
            let fragment = entry.layout.fragments[fragment_index]
            if (fragment.is_control) continue
            let fragment_text = slice(entry.content.text,
                fragment.source.start_byte, fragment.source.end_byte)
            if (find(fragment_text, args.text) < 0) continue
            let screen_rect = markdown_cached_fragment_rect(entry, fragment)
            let align_x = markdown_line_alignment_offset(entry, fragment.line_index)
            let font_size = markdown_cached_fragment_font_size(entry, fragment)
            return JV((
                ok = true,
                render_revision = g_view.render_revision,
                node = node_index,
                fragment = fragment_index,
                text = fragment_text,
                display_start = fragment.source.start_byte,
                display_end = fragment.source.end_byte,
                style_id = fragment.style_id,
                font_size = font_size,
                base_font_size = entry.layout_key.font_size,
                inline_code_scale = entry.layout_key.inline_code_scale,
                semantic_id = fragment.semantic_id,
                alignment = int(entry.screen_alignment),
                alignment_offset = align_x,
                x0 = screen_rect.x, y0 = screen_rect.y,
                x1 = screen_rect.z, y1 = screen_rect.w))
        }
    }
    return JV((ok = false, render_revision = g_view.render_revision,
               error = "rendered fragment not found", text = args.text))
}

[live_command(description = "Inspect the first fenced Markdown code block and its retained metadata.")]
def markdown_probe_code_block(_input : JsonValue?) : JsonValue? {
    for (node_index in range(length(g_doc.nodes))) {
        let node = g_doc.nodes[node_index]
        if (node.kind != RichNodeKind.code_block) continue
        let entry & = unsafe(g_view.nodes[node_index])
        return JV((
            ok = true,
            node = node_index,
            info = node.info,
            language = node.language,
            syntax_default = g_tree_sitter.default_language,
            syntax_override = markdown_tree_sitter_override_language(
                g_tree_sitter, 0, node_index),
            syntax_effective = markdown_tree_sitter_effective_language(
                g_doc, g_tree_sitter, 0, node_index),
            font_size = entry.layout_key.font_size,
            base_font_size = g_fonts.size_px * float(g_zoom_percent) / 100.0f,
            rendered = entry.content_valid && entry.layout_valid
                && entry.screen_revision >= 0))
    }
    return JV((ok = false, error = "code block not found"))
}

[live_command(description = "Inspect the first Markdown image's atomic model and placeholder geometry.")]
def markdown_probe_image(_input : JsonValue?) : JsonValue? {
    for (node_index in range(length(g_view.nodes))) {
        let entry & = unsafe(g_view.nodes[node_index])
        if (!entry.content_valid || !entry.layout_valid
                || entry.screen_revision != g_view.render_revision) continue
        for (run in entry.content.runs) {
            if (!run.is_image) continue
            var binding_size = float2()
            var binding_view_revision = 0
            for (binding in g_view.image_bindings) {
                if (binding.document_revision == g_view.document_revision
                        && binding.node_index == run.semantic_node) {
                    binding_size = binding.display_size
                    binding_view_revision = binding.view_revision
                    break
                }
            }
            var placeholder_valid = false
            var placeholder_rect = float4()
            for (fragment in entry.layout.fragments) {
                if (!fragment.is_control || fragment.control_id != run.semantic_node
                        || fragment.source.start_byte != run.display.start_byte) continue
                placeholder_rect = markdown_cached_fragment_rect(entry, fragment)
                placeholder_valid = true
                break
            }
            return JV((
                ok = true,
                render_revision = g_view.render_revision,
                node = run.semantic_node,
                alt = run.image_alt,
                display = slice(entry.content.text,
                    run.display.start_byte, run.display.end_byte),
                destination = run.destination,
                title = run.title,
                source_exact = run.source_exact,
                source_text = md_source_text(g_doc, run.source),
                resolved = markdown_view_image_bound(
                    g_view, g_view.document_revision, run.semantic_node),
                image_bindings_revision = g_view.image_bindings_revision,
                binding_width = binding_size.x,
                binding_height = binding_size.y,
                binding_view_revision = binding_view_revision,
                layout_image_revision = entry.layout_key.image_layout_revision,
                placeholder_valid = placeholder_valid,
                x0 = placeholder_rect.x, y0 = placeholder_rect.y,
                x1 = placeholder_rect.z, y1 = placeholder_rect.w))
        }
    }
    return JV((ok = false, error = "rendered image not found"))
}

[live_command(description = "Bind the font atlas as a deterministic stand-in for a host-resolved Markdown texture.")]
def markdown_probe_bind_image(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<MarkdownProbeBindImageArgs>)
    let node_index = first_image_node()
    if (node_index < 0) return JV((ok = false, error = "image node not found"))
    let changed = bind_demo_image(args.width, args.height,
                                  args.resource_revision)
    return JV((ok = true, changed = changed, node = node_index,
               image_bindings_revision = g_view.image_bindings_revision,
               prior_render_revision = g_view.render_revision))
}

[live_command(description = "Remove the deterministic Markdown image binding and restore its fallback placeholder.")]
def markdown_probe_unbind_image(_input : JsonValue?) : JsonValue? {
    let node_index = first_image_node()
    if (node_index < 0) return JV((ok = false, error = "image node not found"))
    let changed = markdown_view_unbind_image(g_view, 0, node_index)
    return JV((ok = true, changed = changed, node = node_index,
               image_bindings_revision = g_view.image_bindings_revision,
               prior_render_revision = g_view.render_revision))
}

[live_command(description = "Clear the Markdown selection and return its acknowledgement revision.")]
def markdown_probe_clear_selection(_input : JsonValue?) : JsonValue? {
    markdown_view_clear_selection(g_view)
    g_last_selection = MarkdownSelection()
    return JV((ok = true, selection_revision = g_view.selection_revision))
}

[live_command(description = "Set Markdown zoom for deterministic inspection and return the prior render revision.")]
def markdown_probe_set_zoom(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<MarkdownProbeZoomArgs>)
    g_zoom_percent = clamp(args.percent, 50, 300)
    return JV((ok = true, zoom_percent = g_zoom_percent,
               prior_render_revision = g_view.render_revision))
}

def private selection_style_summary(styles : MdSelectionStyles) : string {
    if (styles.all_regular) return "regular"
    var value = ""
    if (styles.has_strong) {
        value = value + (empty(value) ? "strong" : ", strong")
    }
    if (styles.has_emphasis) {
        value = value + (empty(value) ? "emphasis" : ", emphasis")
    }
    if (styles.has_code) {
        value = value + (empty(value) ? "code" : ", code")
    }
    if (styles.has_strikethrough) {
        value = value + (empty(value) ? "strike" : ", strike")
    }
    if (styles.has_links) {
        value = value + (empty(value) ? "links" : ", links")
    }
    if (styles.has_images) {
        value = value + (empty(value) ? "images" : ", images")
    }
    if (styles.has_mixed_styles) {
        value = value + (empty(value) ? "mixed" : ", mixed")
    }
    return empty(value) ? "none" : value
}

[export]
def init() {
    harness_init("dasImgui Markdown viewer", 1040, 820)
    g_fonts = load_unicode_font_roles(18.0f, g_font_sources)
    apply_daslang_theme(GetStyle())
    command_define(g_commands, CommandDefinition(
        id = "edit.copy", title = "Copy", category = "Edit",
        scope = CommandScope.component),
        command_binding("edit.copy", ImGuiKey.C, primary = true,
                        scope = CommandScope.component))
    let source = %stringify~# Markdown + Unicode 🎨

This is a source-mapped **Markdown viewer** with *emphasis*, `inline code`, ~~deleted text~~, named [documentation](../docs/editor.md "Editor docs"), and a long autolink https://example.com/文档/naïve/é/terminal-and-editor?mode=full&emoji=👩‍💻#résumé.

The shared plain-text pass also recognizes custom editor://workspace/文档 and WWW.Example.org/Unicode/文档 outside Markdown link syntax.

Image fallback and host URI semantics: ![Unicode terminal preview](editor://assets/terminal-preview.png "Terminal preview").

> Font fallback, grapheme-safe wrapping, semantic links, and the Markdown model are separate layers.

## Work list

- [x] Parse CommonMark and GFM
- [x] Preserve UTF-8 byte ranges
- [x] Source-mapped rich text selection and comment anchors
- [ ] Add keyboard navigation later

## Follow-up: shared text infrastructure

- [x] Syntax token model independent of Markdown, ImGui, and the future editor
- [x] Language-aware syntax highlighting for fenced code blocks
- [ ] Reuse the same tokenizer/theme pipeline in the text editor
- [ ] Incremental tokenization for large editable documents

## Table test

| Script | Sample | State |
| :--- | :---: | ---: |
| Cyrillic | Привет | ✓ |
| CJK | 日本語 · 中文 | 🎨 |
| Combining | é / é | ready |

## Code

```das
let greeting = "Привет, 世界 👋"
print("{greeting}\n")
```

Entities decode for display while retaining source bytes: &amp; &copy; &#x1f680;.
%%
    g_doc <- md_parse(source)
    if (!g_doc.ok) {
        g_status = g_doc.error
    }
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()
    command_begin_frame(g_commands)

    SetNextWindowPos(ImVec2(20.0f, 20.0f), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(1000.0f, 780.0f), ImGuiCond.Always)
    window(MARKDOWN_VIEW_WIN, (text = "Markdown viewer", closable = false,
                               flags = ImGuiWindowFlags.None)) {
        command_set_enabled(g_commands, "edit.copy", g_last_selection.active)
        command_poll_scope(g_commands, CommandScope.component, "markdown")
        var invocation = CommandInvocation()
        while (command_take(g_commands, invocation)) {
            if (invocation.command_id == "edit.copy") {
                markdown_view_request_copy(g_view)
            }
        }
        let io & = unsafe(GetIO())
        if (IsWindowHovered(ImGuiHoveredFlags.RootAndChildWindows) && io.KeyCtrl) {
            if (io.MouseWheel > 0.0f) {
                g_zoom_percent = min(g_zoom_percent + 10, 300)
            } elif (io.MouseWheel < 0.0f) {
                g_zoom_percent = max(g_zoom_percent - 10, 50)
            }
        }
        text("Zoom")
        same_line()
        SetNextItemWidth(180.0f)
        if (edit_slider_int(safe_addr(g_zoom_percent),
                (id = "MARKDOWN_ZOOM", text = "##zoom", v_min = 50,
                 v_max = 300, format = "%d%%"))) {
            g_zoom_percent = clamp(((g_zoom_percent + 5) / 10) * 10, 50, 300)
        }
        same_line()
        if (small_button(ZOOM_RESET, (text = "100%"))) {
            g_zoom_percent = 100
        }
        same_line()
        let image_node = first_image_node()
        let image_bound = (image_node >= 0
            && markdown_view_image_bound(g_view, 0, image_node))
        if (small_button(IMAGE_RESOLVE,
                (text = image_bound ? "Use placeholder" : "Resolve demo image"))) {
            if (image_bound) {
                let _ = markdown_view_unbind_image(g_view, 0, image_node)
                g_status = "Host image binding removed; alt placeholder restored."
            } else {
                let _ = bind_demo_image(160.0f, 96.0f, 1)
                g_status = "Host bound a 160x96 texture; Markdown performed no I/O."
            }
        }
        same_line()
        text_disabled("Ctrl+wheel")
        text("Syntax")
        same_line()
        SetNextItemWidth(150.0f)
        if (combo(SYNTAX_DEFAULT,
                (text = "##syntax_default", items <- SYNTAX_DEFAULT_LABELS))) {
            let index = clamp(SYNTAX_DEFAULT.value, 0,
                              length(SYNTAX_DEFAULT_VALUES) - 1)
            let _ = markdown_tree_sitter_set_default(
                g_tree_sitter, SYNTAX_DEFAULT_VALUES[index])
        }
        let code_node = first_code_block_node()
        if (code_node >= 0) {
            same_line()
            text("First block")
            same_line()
            SetNextItemWidth(160.0f)
            if (combo(SYNTAX_BLOCK,
                    (text = "##syntax_block", items <- SYNTAX_BLOCK_LABELS))) {
                let index = clamp(SYNTAX_BLOCK.value, 0,
                                  length(SYNTAX_BLOCK_VALUES) - 1)
                let value = SYNTAX_BLOCK_VALUES[index]
                if (value == "inherit") {
                    let _ = markdown_tree_sitter_clear_override(
                        g_tree_sitter, 0, code_node)
                } else {
                    let _ = markdown_tree_sitter_set_override(
                        g_tree_sitter, 0, code_node, value)
                }
            }
        }
        separator()
        with_font(g_fonts.regular, 18.0f) {
            var markdown_style = MarkdownViewStyle(
                fonts = g_fonts,
                // Deliberately non-default: the integration probe verifies
                // that host typography policy reaches measure + paint.
                typography = MarkdownTypography(
                    inline_code_scale = 0.80f,
                    code_block_scale = 0.88f),
                zoom = float(g_zoom_percent) / 100.0f,
                write_system_clipboard = !harness_is_headless())
            let result = markdown_view_tree_sitter(
                "probe", g_doc, g_tree_sitter, g_view, markdown_style)
            g_last_selection = result.selection
            g_last_hovered_node = result.hovered_node
            g_last_hovered_image = result.hovered_image
            g_last_hovered_target = result.target
            if (result.copied) {
                g_status = "Copied: {result.target}"
            } elif (result.selection_copied) {
                g_status = "Copied selected display text."
            } elif (result.activated) {
                g_status = "Open requested: {result.target}"
            } elif (result.selection_context_requested) {
                g_context_source = result.selection.source
                g_status = "Comment requested at ({int(result.selection_context_pos.x)}, {int(result.selection_context_pos.y)})"
                SetNextWindowPos(result.selection_context_pos, ImGuiCond.Appearing)
                open_popup("markdown_selection_comment")
            } elif (result.selection_changed && result.selection.active) {
                let s = result.selection
                g_status = "Selected source bytes {s.source.start_byte}..{s.source.end_byte}; {selection_style_summary(s.styles)}; anchor ({int(s.screen_anchor.x)}, {int(s.screen_anchor.y)})"
            } elif (result.selection_changed) {
                g_status = "Selection cleared."
            }
            g_add_comment_hovered = false
            popup_window(SELECTION_COMMENT_POPUP,
                         (str_id = "markdown_selection_comment",
                          flags = ImGuiWindowFlags.AlwaysAutoResize)) {
                text("Comment anchor")
                separator()
                text("Source bytes {g_context_source.start_byte}..{g_context_source.end_byte}")
                text_wrapped((text = md_source_text(g_doc, g_context_source)))
                let add_comment_clicked = button(ADD_COMMENT, (text = "Add comment"))
                g_add_comment_hovered = IsItemHovered()
                if (add_comment_clicked) {
                    g_status = "Host accepted comment anchor {g_context_source.start_byte}..{g_context_source.end_byte}"
                    close_current_popup()
                }
            }
            separator()
            text_colored((color = float4(0.95f, 0.72f, 0.30f, 1.0f), text = g_status))
        }
    }

    harness_end_frame()
}

[export]
def shutdown() {
    markdown_tree_sitter_dispose(g_tree_sitter)
    markdown_view_dispose(g_view)
    command_registry_dispose(g_commands)
    harness_shutdown()
}

[export]
def main() {
    init()
    while (!exit_requested()) {
        update()
        harness_maybe_collect_gc()
    }
    shutdown()
}
