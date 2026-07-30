options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_function_arguments = false

module imgui_markdown_view shared

require imgui public
require imgui/imgui_markdown_model public
require imgui/imgui_text_flow
require imgui/imgui_fonts public
require imgui/imgui_icons
require clipboard/clipboard
require math
require strings

//! Immediate-mode rendering counterpart to `imgui_markdown_model`.
//!
//! The document and inline-run model remain UI-independent. This layer owns
//! wrapping, colors, hover/click/copy behavior, and block chrome. Link actions
//! are returned to the host; the viewer never opens a URI on its own.

struct public MarkdownTypography {
    //! Relative to the prose size supplied by FontRoleSet.  These are view
    //! policy, not font-atlas settings, so an application can tune Markdown
    //! proportions without reloading fonts or exposing user preferences.
    inline_code_scale : float = 0.86f
    code_block_scale : float = 0.90f
    heading1_scale : float = 1.55f
    heading2_scale : float = 1.35f
    heading3_scale : float = 1.18f
    heading4_scale : float = 1.05f
    heading5_scale : float = 1.00f
    heading6_scale : float = 1.00f
}

struct public MarkdownViewStyle {
    fonts : FontRoleSet = FontRoleSet()
    typography : MarkdownTypography = MarkdownTypography()
    zoom : float = 1.0f
    text_color : float4 = float4(0.86f, 0.87f, 0.89f, 1.0f)
    strong_color : float4 = float4(0.97f, 0.89f, 0.68f, 1.0f)
    emphasis_color : float4 = float4(0.77f, 0.84f, 0.94f, 1.0f)
    link_color : float4 = float4(0.36f, 0.68f, 1.0f, 1.0f)
    link_hover_color : float4 = float4(0.55f, 0.80f, 1.0f, 1.0f)
    image_color : float4 = float4(0.58f, 0.82f, 0.68f, 1.0f)
    image_hover_color : float4 = float4(0.72f, 0.94f, 0.80f, 1.0f)
    image_background : float4 = float4(0.10f, 0.16f, 0.14f, 1.0f)
    image_border : float4 = float4(0.31f, 0.55f, 0.40f, 1.0f)
    code_color : float4 = float4(0.94f, 0.80f, 0.55f, 1.0f)
    code_background : float4 = float4(0.10f, 0.11f, 0.14f, 1.0f)
    syntax_comment_color : float4 = float4(0.48f, 0.62f, 0.50f, 1.0f)
    syntax_string_color : float4 = float4(0.78f, 0.67f, 0.45f, 1.0f)
    syntax_number_color : float4 = float4(0.70f, 0.58f, 0.86f, 1.0f)
    syntax_keyword_color : float4 = float4(0.78f, 0.48f, 0.68f, 1.0f)
    syntax_type_color : float4 = float4(0.42f, 0.72f, 0.74f, 1.0f)
    syntax_function_color : float4 = float4(0.48f, 0.68f, 0.92f, 1.0f)
    syntax_variable_color : float4 = float4(0.86f, 0.87f, 0.89f, 1.0f)
    syntax_property_color : float4 = float4(0.60f, 0.76f, 0.90f, 1.0f)
    syntax_constant_color : float4 = float4(0.72f, 0.61f, 0.88f, 1.0f)
    syntax_operator_color : float4 = float4(0.76f, 0.78f, 0.82f, 1.0f)
    syntax_punctuation_color : float4 = float4(0.62f, 0.65f, 0.70f, 1.0f)
    selection_color : float4 = float4(0.24f, 0.48f, 0.78f, 0.55f)
    quote_color : float4 = float4(0.42f, 0.47f, 0.56f, 1.0f)
    marker_color : float4 = float4(0.70f, 0.73f, 0.78f, 1.0f)
    heading_color : float4 = float4(0.96f, 0.96f, 0.97f, 1.0f)
    block_spacing : float = 8.0f
    paragraph_spacing : float = 5.0f
    quote_indent : float = 16.0f
    code_padding : float = 8.0f
    copy_icon_size : float = 14.0f
    image_icon_size : float = 18.0f
    show_link_copy : bool = true
    //! Production viewers write copy actions to the system clipboard by
    //! default. Headless tests can disable the external side effect while
    //! still observing the exact copy request and payload.
    write_system_clipboard : bool = true
}

struct public MarkdownSelection {
    active : bool = false
    source : TextSourceRange = TextSourceRange(start_byte = -1, end_byte = -1)
    styles : MdSelectionStyles
    screen_valid : bool = false
    screen_bounds : float4
    screen_anchor : float2
}

struct public MarkdownViewResult {
    hovered : bool = false
    activated : bool = false
    copied : bool = false
    copy_requested : bool = false
    copy_text : string
    hovered_image : bool = false
    activated_image : bool = false
    hovered_image_resolved : bool = false
    activated_image_resolved : bool = false
    hovered_node : int = -1
    activated_node : int = -1
    copied_node : int = -1
    source : TextSourceRange = TextSourceRange(start_byte = -1, end_byte = -1)
    target : string
    selection : MarkdownSelection = MarkdownSelection()
    selection_changed : bool = false
    selection_copied : bool = false
    selection_copy_requested : bool = false
    selection_copy_text : string
    selection_copy_markdown : string
    selection_copy_html : string
    selection_copy_uri_list : string
    selection_context_requested : bool = false
    selection_context_pos : float2
    control_event : RichControlEvent = RichControlEvent()
}

struct public MarkdownImageBinding {
    //! Host-owned resolution result. The Markdown viewer performs no I/O;
    //! callers bind a texture after applying their own URI/security policy.
    document_revision : int = -1
    node_index : int = -1
    resource_revision : int = 0
    //! Assigned by the viewer when this binding changes; hosts leave it zero.
    view_revision : int = 0
    //! Backend texture handle (`ImTextureRef.GetTexID()`). The non-local
    //! `ImTextureRef` wrapper is reconstructed transiently during drawing.
    texture_id : uint64 = 0uL
    //! Display size at 100% Markdown zoom. The viewer scales it with `zoom`;
    //! texture pixels and UVs remain unchanged.
    display_size : float2
    uv0 : float2 = float2(0.0f, 0.0f)
    uv1 : float2 = float2(1.0f, 1.0f)
    tint_color : float4 = float4(1.0f, 1.0f, 1.0f, 1.0f)
}

enum public MarkdownSyntaxStyle {
    comment
    string_
    number
    keyword
    type_
    function_
    variable
    property
    constant
    operator_
    punctuation
}

struct public MarkdownSyntaxToken {
    start_byte : int
    end_byte : int
    style : MarkdownSyntaxStyle
}

struct public MarkdownSyntaxBinding {
    //! View-independent token projection for one code block. Token ranges are
    //! relative to that block's displayed text, not the outer Markdown source.
    document_revision : int = -1
    node_index : int = -1
    tokens : array<MarkdownSyntaxToken>
}

struct public MarkdownLayoutKey {
    max_width : float
    line_height : float
    font_size : float
    inline_code_scale : float = 1.0f
    icon_size : float2
    image_icon_size : float2
    forced_style : int
    show_link_copy : bool
    image_layout_revision : int = 0
    syntax_layout_revision : int = 0
    regular_id : uint64 = 0uL
    bold_id : uint64 = 0uL
    italic_id : uint64 = 0uL
    bold_italic_id : uint64 = 0uL
    monospace_id : uint64 = 0uL
    fallback_id : uint64 = 0uL
}

struct public MarkdownNodeViewCache {
    content_valid : bool = false
    content : RichInlineContent
    marker_valid : bool = false
    marker : string
    layout_valid : bool = false
    layout_key : MarkdownLayoutKey = MarkdownLayoutKey()
    layout : TextFlowLayout
    //! Last screen-space placement of this cached layout.  This is telemetry,
    //! not part of the layout key: scrolling moves the placement without
    //! invalidating or rebuilding the layout.
    screen_origin : float2
    screen_revision : int = -1
    screen_available_width : float = 0.0f
    screen_alignment : RichAlignment = RichAlignment.default_
    control_screen_valid : bool = false
    control_screen_rect : float4
}

struct public MarkdownViewState {
    //! One state belongs to one immutable document revision. Bump the caller's
    //! revision whenever the document is reparsed or otherwise replaced.
    document_revision : int = -1
    node_count : int = 0
    nodes : array<MarkdownNodeViewCache>
    image_bindings : array<MarkdownImageBinding>
    image_bindings_revision : int = 0
    syntax_bindings : array<MarkdownSyntaxBinding>
    syntax_bindings_revision : int = 0
    content_hits : int = 0
    content_misses : int = 0
    layout_hits : int = 0
    layout_misses : int = 0
    selection_anchor : RichTextPosition = RichTextPosition()
    selection_focus : RichTextPosition = RichTextPosition()
    selection_dragging : bool = false
    selection_dragged : bool = false
    selection_pressed_semantic : int = -1
    selection_changed : bool = false
    selection_click_claimed : bool = false
    selection_screen_valid : bool = false
    selection_screen_bounds : float4
    selection_screen_anchor : float2
    //! Number of continuous per-line highlight rectangles painted this frame.
    selection_paint_rect_count : int = 0
    //! Monotonic event acknowledgements used by live inspection and tests.
    //! They describe observed UI state; callers never need to wait a number
    //! of frames before issuing the next input event.
    render_revision : int = 0
    hover_revision : int = 0
    hovered_inline_node : int = -1
    hovered_fragment : int = -1
    hovered_display_byte : int = -1
    selection_revision : int = 0
    selection_commit_revision : int = 0
    selection_copy_revision : int = 0
    selection_copy_pending : bool = false
    selection_copy_text : string
    selection_copy_markdown : string
    selection_copy_html : string
    selection_copy_uri_list : string
    selection_context_revision : int = 0
    selection_context_pos : float2
    navigation_revision : int = 0
    caret_screen_valid : bool = false
    caret_screen_pos : float2
    caret_screen_height : float = 0.0f
    navigation_goal_x_valid : bool = false
    navigation_goal_x : float = 0.0f
    navigation_needs_scroll : bool = false
    navigation_scroll_revision : int = 0
    navigation_scroll_target : float = 0.0f
    viewport_top : float = 0.0f
    viewport_bottom : float = 0.0f
    control_activation_revision : int = 0
    activated_control_id : int = -1
}

let private STYLE_STRONG = 1
let private STYLE_EMPHASIS = 2
let private STYLE_CODE = 4
let private STYLE_STRIKE = 8
let private STYLE_LINK = 16
let private STYLE_HEADING = 32
let private STYLE_IMAGE = 64
let private CONTROL_COPY_URI = 1
let private CONTROL_IMAGE_PLACEHOLDER = 2

def private markdown_copy_uri_to_clipboard(target : string) : ClipboardStatus {
    var content = ClipboardContent(plain_text = target, uri_list = target + "\r\n")
    content.formats.plain_text = true
    content.formats.uri_list = true
    let status = clipboard_set_content(content)
    clipboard_dispose_content(content)
    return status
}

def private clear_node_cache(var entry : MarkdownNodeViewCache) {
    delete entry.content.runs
    delete entry.layout.lines
    delete entry.layout.fragments
    entry = MarkdownNodeViewCache()
}

def private clear_selection(var state : MarkdownViewState) {
    state.selection_anchor = RichTextPosition()
    state.selection_focus = RichTextPosition()
    state.selection_dragging = false
    state.selection_dragged = false
    state.selection_pressed_semantic = -1
    state.selection_screen_valid = false
}

def public markdown_view_clear_selection(var state : MarkdownViewState) {
    //! Clear selection explicitly and acknowledge the state transition.  Live
    //! tooling uses the revision rather than a frame delay to observe it.
    let had_state = (state.selection_anchor.valid
        || state.selection_focus.valid || state.selection_dragging)
    clear_selection(state)
    if (had_state) {
        state.selection_revision++
        state.selection_changed = true
    }
}

def public markdown_view_request_copy(var state : MarkdownViewState) {
    //! Request a semantic copy action. Keyboard/menu/toolbar binding belongs
    //! to the host application; the view only owns rich selection export.
    state.selection_copy_pending = true
}

def public markdown_view_select_all(var state : MarkdownViewState) : bool {
    var first = -1
    var last = -1
    for (index in range(length(state.nodes))) {
        if (state.nodes[index].content_valid
                && !empty(state.nodes[index].content.text)) {
            if (first < 0) {
                first = index
            }
            last = index
        }
    }
    if (first < 0 || last < 0) return false
    state.selection_anchor = md_inline_position(state.nodes[first].content,
                                                first, 0, false)
    state.selection_focus = md_inline_position(state.nodes[last].content,
        last, length(state.nodes[last].content.text), true)
    state.selection_changed = true
    state.selection_revision++
    state.selection_commit_revision++
    return state.selection_anchor.valid && state.selection_focus.valid
}

def public markdown_view_invalidate(var state : MarkdownViewState) {
    //! Deterministically release all retained strings and arrays. The outer
    //! cache capacity is retained for a same-sized replacement document.
    for (i in range(length(state.nodes))) {
        var entry & = unsafe(state.nodes[i])
        clear_node_cache(entry)
    }
    state.document_revision = -1
    state.node_count = 0
    clear_selection(state)
}

def private image_binding_index(state : MarkdownViewState const;
                                document_revision, node_index : int) : int {
    for (i in range(length(state.image_bindings))) {
        let binding = state.image_bindings[i]
        if (binding.document_revision == document_revision
                && binding.node_index == node_index) return i
    }
    return -1
}

def public markdown_view_image_bound(state : MarkdownViewState const;
                                     document_revision, node_index : int) : bool {
    return image_binding_index(state, document_revision, node_index) >= 0
}

def public markdown_view_bind_image(var state : MarkdownViewState;
                                    binding : MarkdownImageBinding const) : bool {
    //! Add or replace one host-resolved texture. `resource_revision` is the
    //! host's content identity; unchanged calls are layout-cache no-ops.
    if (binding.document_revision < 0 || binding.node_index < 0
            || binding.texture_id == 0uL
            || binding.display_size.x <= 0.0f
            || binding.display_size.y <= 0.0f) return false
    let index = image_binding_index(state, binding.document_revision,
                                    binding.node_index)
    if (index >= 0) {
        let old = state.image_bindings[index]
        let unchanged = (old.resource_revision == binding.resource_revision
            && old.texture_id == binding.texture_id
            && old.display_size.x == binding.display_size.x
            && old.display_size.y == binding.display_size.y
            && old.uv0.x == binding.uv0.x && old.uv0.y == binding.uv0.y
            && old.uv1.x == binding.uv1.x && old.uv1.y == binding.uv1.y
            && old.tint_color.x == binding.tint_color.x
            && old.tint_color.y == binding.tint_color.y
            && old.tint_color.z == binding.tint_color.z
            && old.tint_color.w == binding.tint_color.w)
        state.image_bindings[index] = binding
        if (unchanged) {
            state.image_bindings[index].view_revision = old.view_revision
            return false
        }
    } else {
        state.image_bindings |> push(binding)
    }
    state.image_bindings_revision++
    let changed_index = image_binding_index(
        state, binding.document_revision, binding.node_index)
    state.image_bindings[changed_index].view_revision = state.image_bindings_revision
    return true
}

def public markdown_view_unbind_image(var state : MarkdownViewState;
                                      document_revision, node_index : int) : bool {
    let index = image_binding_index(state, document_revision, node_index)
    if (index < 0) return false
    state.image_bindings |> erase(index)
    state.image_bindings_revision++
    return true
}

def public markdown_view_clear_images(var state : MarkdownViewState) {
    if (empty(state.image_bindings)) return
    delete state.image_bindings
    state.image_bindings_revision++
}

def private syntax_binding_index(state : MarkdownViewState const;
                                 document_revision, node_index : int) : int {
    for (i in range(length(state.syntax_bindings))) {
        let binding & = unsafe(state.syntax_bindings[i])
        if (binding.document_revision == document_revision
                && binding.node_index == node_index) return i
    }
    return -1
}

def public markdown_view_syntax_bound(state : MarkdownViewState const;
                                      document_revision, node_index : int) : bool {
    return syntax_binding_index(state, document_revision, node_index) >= 0
}

def public markdown_view_syntax_token_count(state : MarkdownViewState const;
                                            document_revision, node_index : int) : int {
    let index = syntax_binding_index(state, document_revision, node_index)
    return index >= 0 ? length(state.syntax_bindings[index].tokens) : 0
}

def public markdown_view_bind_syntax(var state : MarkdownViewState;
                                     document_revision, node_index : int;
                                     tokens : array<MarkdownSyntaxToken> const) : bool {
    //! Install a deterministic token snapshot. Parsers remain optional and
    //! external to the Markdown renderer; the renderer owns its copied view.
    if (document_revision < 0 || node_index < 0) return false
    let index = syntax_binding_index(state, document_revision, node_index)
    if (index >= 0) {
        return false
    }
    var binding = MarkdownSyntaxBinding(
        document_revision = document_revision, node_index = node_index)
    binding.tokens := tokens
    state.syntax_bindings |> emplace(binding)
    state.syntax_bindings_revision++
    return true
}

def public markdown_view_unbind_syntax(var state : MarkdownViewState;
                                       document_revision, node_index : int) : bool {
    let index = syntax_binding_index(state, document_revision, node_index)
    if (index < 0) return false
    delete state.syntax_bindings[index].tokens
    state.syntax_bindings |> erase(index)
    state.syntax_bindings_revision++
    return true
}

def public markdown_view_clear_syntax(var state : MarkdownViewState) {
    if (empty(state.syntax_bindings)) return
    for (i in range(length(state.syntax_bindings))) {
        delete state.syntax_bindings[i].tokens
    }
    delete state.syntax_bindings
    state.syntax_bindings_revision++
}

def public markdown_view_dispose(var state : MarkdownViewState) {
    //! Release a caller-owned view state during application shutdown.
    markdown_view_invalidate(state)
    unsafe { delete state.nodes }
    delete state.image_bindings
    markdown_view_clear_syntax(state)
    state = MarkdownViewState()
}

def private retain_image_bindings(var state : MarkdownViewState;
                                  document_revision : int) {
    var retained : array<MarkdownImageBinding>
    for (binding in state.image_bindings) {
        if (binding.document_revision == document_revision) {
            retained |> push(binding)
        }
    }
    if (length(retained) != length(state.image_bindings)) {
        delete state.image_bindings
        state.image_bindings <- retained
        state.image_bindings_revision++
    } else {
        delete retained
    }
}

def private retain_syntax_bindings(var state : MarkdownViewState;
                                   document_revision : int) {
    var retained : array<MarkdownSyntaxBinding>
    for (binding_index in range(length(state.syntax_bindings))) {
        let binding & = unsafe(state.syntax_bindings[binding_index])
        if (binding.document_revision == document_revision) {
            var copy = MarkdownSyntaxBinding(
                document_revision = binding.document_revision,
                node_index = binding.node_index)
            copy.tokens := binding.tokens
            retained |> emplace(copy)
        }
    }
    if (length(retained) != length(state.syntax_bindings)) {
        for (i in range(length(state.syntax_bindings))) {
            delete state.syntax_bindings[i].tokens
        }
        delete state.syntax_bindings
        state.syntax_bindings <- retained
        state.syntax_bindings_revision++
    } else {
        for (i in range(length(retained))) {
            delete retained[i].tokens
        }
        delete retained
    }
}

def private prepare_view_state(doc : RichDocument const;
                               var state : MarkdownViewState;
                               document_revision : int) {
    let count = length(doc.nodes)
    if (state.document_revision == document_revision
            && state.node_count == count && length(state.nodes) == count) return
    retain_image_bindings(state, document_revision)
    retain_syntax_bindings(state, document_revision)
    markdown_view_invalidate(state)
    if (length(state.nodes) != count) {
        unsafe {
            delete state.nodes
            state.nodes |> resize(count)
        }
    }
    state.document_revision = document_revision
    state.node_count = count
}

def private ensure_inline(doc : RichDocument const; node_index : int;
                          var state : MarkdownViewState;
                          direct_children_only : bool = false) {
    var entry & = unsafe(state.nodes[node_index])
    if (!entry.content_valid) {
        entry.content <- (direct_children_only
            ? md_inline_content_direct_children(doc, node_index)
            : md_inline_content(doc, node_index))
        entry.content_valid = true
        state.content_misses++
    } else {
        state.content_hits++
    }
}

def private view_zoom(style : MarkdownViewStyle const) : float {
    return clamp(style.zoom, 0.5f, 3.0f)
}

def private zoomed(style : MarkdownViewStyle const; value : float) : float {
    return value * view_zoom(style)
}

def private syntax_color(style_id : int; style : MarkdownViewStyle const) : float4 {
    if (style_id == int(MarkdownSyntaxStyle.comment)) return style.syntax_comment_color
    if (style_id == int(MarkdownSyntaxStyle.string_)) return style.syntax_string_color
    if (style_id == int(MarkdownSyntaxStyle.number)) return style.syntax_number_color
    if (style_id == int(MarkdownSyntaxStyle.keyword)) return style.syntax_keyword_color
    if (style_id == int(MarkdownSyntaxStyle.type_)) return style.syntax_type_color
    if (style_id == int(MarkdownSyntaxStyle.function_)) return style.syntax_function_color
    if (style_id == int(MarkdownSyntaxStyle.variable)) return style.syntax_variable_color
    if (style_id == int(MarkdownSyntaxStyle.property)) return style.syntax_property_color
    if (style_id == int(MarkdownSyntaxStyle.constant)) return style.syntax_constant_color
    if (style_id == int(MarkdownSyntaxStyle.operator_)) return style.syntax_operator_color
    return style.syntax_punctuation_color
}

def private run_style(run : RichInlineRun const) : int {
    var value = 0
    if (run.is_strong) {
        value += STYLE_STRONG
    }
    if (run.is_emphasis) {
        value += STYLE_EMPHASIS
    }
    if (run.is_code) {
        value += STYLE_CODE
    }
    if (run.is_strikethrough) {
        value += STYLE_STRIKE
    }
    if (run.is_link) {
        value += STYLE_LINK
    }
    if (run.is_image) {
        value += STYLE_IMAGE
    }
    return value
}

def private has_style(value, flag : int) : bool {
    return (value & flag) != 0
}

def private font_for_style(style_id : int; var roles : FontRoleSet;
                           var fallback : ImFont?) : ImFont? {
    if (has_style(style_id, STYLE_CODE) && roles.monospace != null) {
        return roles.monospace
    }
    let bold_role = (has_style(style_id, STYLE_STRONG)
        || has_style(style_id, STYLE_HEADING))
    if (bold_role
            && has_style(style_id, STYLE_EMPHASIS)
            && roles.bold_italic != null) {
        return roles.bold_italic
    }
    if (bold_role && roles.bold != null) {
        return roles.bold
    }
    if (has_style(style_id, STYLE_EMPHASIS) && roles.italic != null) {
        return roles.italic
    }
    if (roles.regular != null) return roles.regular
    return fallback
}

def private font_size_for_style(base_size, inline_code_scale : float;
                                style_id : int) : float {
    return base_size * (has_style(style_id, STYLE_CODE)
        ? max(inline_code_scale, 0.1f) : 1.0f)
}

def private heading_scale(typography : MarkdownTypography const;
                          level : int) : float {
    if (level == 1) return typography.heading1_scale
    if (level == 2) return typography.heading2_scale
    if (level == 3) return typography.heading3_scale
    if (level == 4) return typography.heading4_scale
    if (level == 5) return typography.heading5_scale
    return typography.heading6_scale
}

def public markdown_line_alignment_offset(entry : MarkdownNodeViewCache const;
                                          line_index : int) : float {
    if (entry.screen_alignment == RichAlignment.default_
            || entry.screen_alignment == RichAlignment.left
            || line_index < 0 || line_index >= length(entry.layout.lines)) return 0.0f
    let remaining = max(0.0f,
        entry.screen_available_width - entry.layout.lines[line_index].width)
    return entry.screen_alignment == RichAlignment.center ? remaining * 0.5f : remaining
}

def public markdown_cached_fragment_rect(entry : MarkdownNodeViewCache const;
                                         fragment : TextFlowFragment const) : float4 {
    let dx = markdown_line_alignment_offset(entry, fragment.line_index)
    return float4(
        entry.screen_origin.x + fragment.rect.x + dx,
        entry.screen_origin.y + fragment.rect.y,
        entry.screen_origin.x + fragment.rect.z + dx,
        entry.screen_origin.y + fragment.rect.w)
}

def public markdown_cached_fragment_font_size(entry : MarkdownNodeViewCache const;
                                              fragment : TextFlowFragment const) : float {
    //! Exact point size used to measure and paint a retained fragment.  Live
    //! tools can inspect typography without reaching into renderer policy.
    return font_size_for_style(entry.layout_key.font_size,
        entry.layout_key.inline_code_scale, fragment.style_id)
}

def private run_semantic_id(doc : RichDocument const; run : RichInlineRun const;
                            run_index : int) : int {
    if (!run.is_link) return -1
    return run.semantic_node >= 0 ? run.semantic_node : length(doc.nodes) + run_index
}

def private link_run_index(doc : RichDocument const; content : RichInlineContent const;
                           semantic_id : int) : int {
    for (i in range(length(content.runs))) {
        if (run_semantic_id(doc, content.runs[i], i) == semantic_id) return i
    }
    return -1
}

def private link_target(doc : RichDocument const; content : RichInlineContent const;
                        semantic_id : int) : string {
    let index = link_run_index(doc, content, semantic_id)
    if (index < 0) return ""
    return content.runs[index].destination
}

def private selected_source(state : MarkdownViewState const) : TextSourceRange {
    return md_text_selection_range(state.selection_anchor, state.selection_focus)
}

def private same_text_position(a, b : RichTextPosition const) : bool {
    return (a.valid == b.valid && a.inline_node == b.inline_node
        && a.display_byte == b.display_byte && a.source_byte == b.source_byte
        && a.trailing == b.trailing)
}

def private adjacent_text_position(var state : MarkdownViewState;
                                   from : RichTextPosition const;
                                   direction : int) : RichTextPosition {
    if (!from.valid || from.inline_node < 0
            || from.inline_node >= length(state.nodes)) return RichTextPosition()
    let content & = unsafe(state.nodes[from.inline_node].content)
    if (direction < 0 && from.display_byte > 0) {
        let at = text_previous_grapheme_boundary(content.text, from.display_byte)
        return md_inline_position(content, from.inline_node, at, false)
    }
    if (direction > 0 && from.display_byte < length(content.text)) {
        let at = text_next_grapheme_boundary(content.text, from.display_byte)
        return md_inline_position(content, from.inline_node, at, true)
    }
    var node = from.inline_node + direction
    while (node >= 0 && node < length(state.nodes)) {
        let entry & = unsafe(state.nodes[node])
        if (entry.content_valid && entry.layout_valid && !empty(entry.content.text)) {
            let at = direction < 0 ? length(entry.content.text) : 0
            return md_inline_position(entry.content, node, at, direction < 0)
        }
        node += direction
    }
    return from
}

def private position_line_index(entry : MarkdownNodeViewCache const;
                                position : RichTextPosition const) : int {
    for (fragment in entry.layout.fragments) {
        if (position_in_fragment(position, position.inline_node, fragment)) {
            return fragment.line_index
        }
    }
    return -1
}

def private line_boundary_position(var state : MarkdownViewState;
                                   from : RichTextPosition const;
                                   end_ : bool) : RichTextPosition {
    if (!from.valid || from.inline_node < 0
            || from.inline_node >= length(state.nodes)) return from
    let entry & = unsafe(state.nodes[from.inline_node])
    let line_index = position_line_index(entry, from)
    if (line_index < 0 || line_index >= length(entry.layout.lines)) return from
    let line = entry.layout.lines[line_index]
    let at = end_ ? line.source.end_byte : line.source.start_byte
    return md_inline_position(entry.content, from.inline_node, at, end_)
}

def private line_text_x_range(entry : MarkdownNodeViewCache const;
                              line_index : int) : float2 {
    var result = float2(0.0f, 0.0f)
    var valid = false
    for (fragment in entry.layout.fragments) {
        if (fragment.line_index != line_index || fragment.is_control) continue
        if (!valid) {
            result = float2(fragment.rect.x, fragment.rect.z)
            valid = true
        } else {
            result.x = min(result.x, fragment.rect.x)
            result.y = max(result.y, fragment.rect.z)
        }
    }
    return result
}

def private vertical_text_position(var state : MarkdownViewState;
                                   var style : MarkdownViewStyle;
                                   from : RichTextPosition const;
                                   direction : int) : RichTextPosition {
    if (!from.valid || from.inline_node < 0
            || from.inline_node >= length(state.nodes)) return from
    let current & = unsafe(state.nodes[from.inline_node])
    let current_line_index = position_line_index(current, from)
    if (current_line_index < 0) return from
    let current_line = current.layout.lines[current_line_index]
    let current_y = (current.screen_origin.y + current_line.y
        + current_line.height * 0.5f)
    let goal_x = (state.navigation_goal_x_valid
        ? state.navigation_goal_x : state.caret_screen_pos.x)
    var best_node = -1
    var best_line = -1
    var best_y = direction < 0 ? -1000000000.0f : 1000000000.0f
    var best_x_distance = 1000000000.0f
    for (node_index in range(length(state.nodes))) {
        let entry & = unsafe(state.nodes[node_index])
        if (!entry.content_valid || !entry.layout_valid
                || entry.screen_revision < state.render_revision - 1) continue
        for (line_index in range(length(entry.layout.lines))) {
            let line = entry.layout.lines[line_index]
            let line_y = entry.screen_origin.y + line.y + line.height * 0.5f
            if ((direction < 0 && line_y >= current_y - 0.5f)
                    || (direction > 0 && line_y <= current_y + 0.5f)) continue
            let xr = line_text_x_range(entry, line_index)
            if (xr.y <= xr.x) continue
            let align_x = markdown_line_alignment_offset(entry, line_index)
            let screen_x0 = entry.screen_origin.x + xr.x + align_x
            let screen_x1 = entry.screen_origin.x + xr.y + align_x
            let x_distance = (goal_x < screen_x0 ? screen_x0 - goal_x
                : (goal_x > screen_x1 ? goal_x - screen_x1 : 0.0f))
            let nearer_y = direction < 0 ? line_y > best_y : line_y < best_y
            let same_y = abs(line_y - best_y) < 0.5f
            if (nearer_y || (same_y && x_distance < best_x_distance)) {
                best_node = node_index
                best_line = line_index
                best_y = line_y
                best_x_distance = x_distance
            }
        }
    }
    if (best_node < 0) return from
    let target & = unsafe(state.nodes[best_node])
    let target_line = target.layout.lines[best_line]
    let xr = line_text_x_range(target, best_line)
    let align_x = markdown_line_alignment_offset(target, best_line)
    let local_x = clamp(goal_x - target.screen_origin.x - align_x, xr.x, xr.y)
    let hit = text_flow_hit_test_ranges(target.content.text, target.layout,
        float2(local_x, target_line.y + target_line.height * 0.5f)) $(text : string;
                                                                      start_byte, end_byte, style_id : int) : float {
            var fallback = GetFont()
            var font = font_for_style(style_id, style.fonts, fallback)
            let font_size = font_size_for_style(
                target.layout_key.font_size,
                target.layout_key.inline_code_scale, style_id)
            return CalcTextSizeForFontRange(font, font_size,
                text, length(text), start_byte, end_byte).x
        }
    if (!hit.valid || hit.line_index != best_line) return from
    return md_inline_position(target.content, best_node,
                              hit.byte_offset, hit.trailing)
}

def private navigate_selection_keyboard(var state : MarkdownViewState;
                                        var style : MarkdownViewStyle) {
    if (!state.selection_focus.valid
            || !IsWindowFocused(ImGuiFocusedFlags.RootAndChildWindows)
            || IsAnyItemActive()) return
    var action = 0
    if (IsKeyPressed(ImGuiKey.LeftArrow, false)) {
        action = 1
    } elif (IsKeyPressed(ImGuiKey.RightArrow, false)) {
        action = 2
    } elif (IsKeyPressed(ImGuiKey.UpArrow, false)) {
        action = 3
    } elif (IsKeyPressed(ImGuiKey.DownArrow, false)) {
        action = 4
    } elif (IsKeyPressed(ImGuiKey.Home, false)) {
        action = 5
    } elif (IsKeyPressed(ImGuiKey.End, false)) {
        action = 6
    }
    if (action == 0) return
    let io & = unsafe(GetIO())
    let selection_range = selected_source(state)
    let has_selection = selection_range.end_byte > selection_range.start_byte
    var next = state.selection_focus
    if (has_selection && !io.KeyShift && (action == 1 || action == 2)) {
        if (action == 1) {
            next = (state.selection_anchor.source_byte <= state.selection_focus.source_byte
                ? state.selection_anchor : state.selection_focus)
        } else {
            next = (state.selection_anchor.source_byte >= state.selection_focus.source_byte
                ? state.selection_anchor : state.selection_focus)
        }
        state.selection_anchor = next
    } else {
        if (action == 1 || action == 2) {
            next = adjacent_text_position(state, state.selection_focus,
                                          action == 1 ? -1 : 1)
        } elif (action == 3 || action == 4) {
            if (!state.navigation_goal_x_valid) {
                state.navigation_goal_x = state.caret_screen_pos.x
                state.navigation_goal_x_valid = true
            }
            next = vertical_text_position(state, style, state.selection_focus,
                                          action == 3 ? -1 : 1)
        } else {
            next = line_boundary_position(state, state.selection_focus, action == 6)
        }
        if (!io.KeyShift) {
            state.selection_anchor = next
        }
    }
    if (action != 3 && action != 4) {
        state.navigation_goal_x_valid = false
    }
    if (!same_text_position(state.selection_focus, next)) {
        state.selection_focus = next
        state.selection_revision++
        state.navigation_revision++
        state.selection_changed = true
        state.navigation_needs_scroll = true
    } elif (has_selection && !io.KeyShift) {
        state.selection_focus = next
        state.selection_revision++
        state.navigation_revision++
        state.selection_changed = true
        state.navigation_needs_scroll = true
    }
}

def private ensure_keyboard_caret_visible(var state : MarkdownViewState) {
    if (!state.navigation_needs_scroll) return
    state.navigation_needs_scroll = false
    if (!state.caret_screen_valid) return
    let caret_top = state.caret_screen_pos.y
    let caret_bottom = caret_top + state.caret_screen_height
    let old_scroll = GetScrollY()
    var target = old_scroll
    if (caret_top < state.viewport_top) {
        target = old_scroll - (state.viewport_top - caret_top)
    } elif (caret_bottom > state.viewport_bottom) {
        target = old_scroll + (caret_bottom - state.viewport_bottom)
    }
    target = clamp(target, 0.0f, GetScrollMaxY())
    if (abs(target - old_scroll) > 0.5f) {
        SetScrollY(target)
        state.navigation_scroll_target = target
        state.navigation_scroll_revision++
    }
}

def private include_selection_screen_rect(var state : MarkdownViewState;
                                          rect : float4) {
    if (!state.selection_screen_valid) {
        state.selection_screen_bounds = rect
        state.selection_screen_valid = true
    } else {
        state.selection_screen_bounds.x = min(state.selection_screen_bounds.x, rect.x)
        state.selection_screen_bounds.y = min(state.selection_screen_bounds.y, rect.y)
        state.selection_screen_bounds.z = max(state.selection_screen_bounds.z, rect.z)
        state.selection_screen_bounds.w = max(state.selection_screen_bounds.w, rect.w)
    }
}

def private position_in_fragment(position : RichTextPosition const; node_index : int;
                                 fragment : TextFlowFragment const) : bool {
    if (!position.valid || position.inline_node != node_index || fragment.is_control) return false
    if (position.trailing) {
        return (fragment.source.start_byte < position.display_byte
            && position.display_byte <= fragment.source.end_byte)
    }
    return (fragment.source.start_byte <= position.display_byte
        && position.display_byte < fragment.source.end_byte)
}

def private same_layout_key(a, b : MarkdownLayoutKey) : bool {
    return (a.max_width == b.max_width && a.line_height == b.line_height
        && a.font_size == b.font_size
        && a.inline_code_scale == b.inline_code_scale
        && a.icon_size.x == b.icon_size.x && a.icon_size.y == b.icon_size.y
        && a.image_icon_size.x == b.image_icon_size.x
        && a.image_icon_size.y == b.image_icon_size.y
        && a.forced_style == b.forced_style
        && a.show_link_copy == b.show_link_copy
        && a.image_layout_revision == b.image_layout_revision
        && a.syntax_layout_revision == b.syntax_layout_revision
        && a.regular_id == b.regular_id && a.bold_id == b.bold_id
        && a.italic_id == b.italic_id && a.bold_italic_id == b.bold_italic_id
        && a.monospace_id == b.monospace_id && a.fallback_id == b.fallback_id)
}

def private ensure_layout(doc : RichDocument const; node_index : int;
                          var state : MarkdownViewState;
                          key : MarkdownLayoutKey;
                          var style : MarkdownViewStyle;
                          var fallback : ImFont?) {
    var entry & = unsafe(state.nodes[node_index])
    if (entry.layout_valid && same_layout_key(entry.layout_key, key)) {
        state.layout_hits++
        return
    }
    delete entry.layout.lines
    delete entry.layout.fragments
    var spans : array<TextFlowSpan>
    let syntax_index = syntax_binding_index(
        state, state.document_revision, node_index)
    for (i in range(length(entry.content.runs))) {
        let run = entry.content.runs[i]
        let semantic_id = run_semantic_id(doc, run, i)
        let has_semantic = semantic_id >= 0
        let is_last_semantic = (has_semantic
            && (i + 1 >= length(entry.content.runs)
                || run_semantic_id(doc, entry.content.runs[i + 1], i + 1)
                    != semantic_id))
        var image_size = key.image_icon_size
        if (run.is_image) {
            let binding_index = image_binding_index(
                state, state.document_revision, run.semantic_node)
            if (binding_index >= 0) {
                let binding = state.image_bindings[binding_index]
                let zoom = view_zoom(style)
                image_size = float2(
                    binding.display_size.x * zoom + GetStyle().ItemInnerSpacing.x,
                    binding.display_size.y * zoom)
            }
        }
        if (syntax_index >= 0 && !run.is_link && !run.is_image) {
            var cursor = run.display.start_byte
            for (token in state.syntax_bindings[syntax_index].tokens) {
                let a = max(run.display.start_byte, token.start_byte)
                let b = min(run.display.end_byte, token.end_byte)
                if (b <= a || b <= cursor) continue
                let token_start = max(a, cursor)
                if (token_start > cursor) {
                    spans |> push(TextFlowSpan(
                        source = TextSourceRange(start_byte = cursor, end_byte = token_start),
                        style_id = run_style(run) | key.forced_style,
                        semantic_id = semantic_id))
                }
                spans |> push(TextFlowSpan(
                    source = TextSourceRange(start_byte = token_start, end_byte = b),
                    style_id = run_style(run) | key.forced_style,
                    semantic_id = semantic_id,
                    syntax_style_id = int(token.style)))
                cursor = max(cursor, b)
            }
            if (cursor < run.display.end_byte) {
                spans |> push(TextFlowSpan(
                    source = TextSourceRange(
                        start_byte = cursor, end_byte = run.display.end_byte),
                    style_id = run_style(run) | key.forced_style,
                    semantic_id = semantic_id))
            }
        } else {
            spans |> push(TextFlowSpan(
                source = run.display,
                style_id = run_style(run) | key.forced_style,
                semantic_id = semantic_id,
                uri_wrap = run.is_link,
                leading_control_id = run.is_image ? semantic_id : -1,
                leading_control_kind = run.is_image ? CONTROL_IMAGE_PLACEHOLDER : 0,
                leading_control_size = run.is_image
                    ? image_size : float2(0.0f, 0.0f),
                trailing_control_id = key.show_link_copy && is_last_semantic
                    ? semantic_id : -1,
                trailing_control_kind = key.show_link_copy && is_last_semantic
                    ? CONTROL_COPY_URI : 0,
                trailing_control_size = key.show_link_copy && is_last_semantic
                    ? key.icon_size : float2(0.0f, 0.0f)))
        }
    }
    entry.layout <- layout_text_flow_styled_ranges(entry.content.text, spans,
        key.max_width, key.line_height) $(text : string; start_byte : int; end_byte : int;
                                          style_id : int) : float {
            var font = font_for_style(style_id, style.fonts, fallback)
            let font_size = font_size_for_style(
                key.font_size, key.inline_code_scale, style_id)
            return CalcTextSizeForFontRange(font, font_size, text,
                                            length(text), start_byte, end_byte).x
        }
    delete spans
    entry.layout_key = key
    entry.layout_valid = true
    state.layout_misses++
}

def private render_inline(doc : RichDocument const; node_index : int;
                          var state : MarkdownViewState; max_width : float;
                          var style : MarkdownViewStyle;
                          font_scale : float; forced_style : int;
                          alignment : RichAlignment;
                          var result : MarkdownViewResult;
                          direct_children_only : bool = false) {
    let available = max(max_width, 1.0f)
    ensure_inline(doc, node_index, state, direct_children_only)
    let base_size = (style.fonts.size_px > 0.0f
        ? style.fonts.size_px : GetFontSize())
    let zoom = view_zoom(style)
    let font_size = base_size * zoom * font_scale
    let line_height = GetTextLineHeight() * font_size / max(GetFontSize(), 1.0f)
    var fallback_font = GetFont()
    let icon_full = float2(
        style.copy_icon_size * zoom + GetStyle().FramePadding.x * 2.0f,
        style.copy_icon_size * zoom + GetStyle().FramePadding.y * 2.0f)
    let image_icon_full = float2(
        style.image_icon_size * zoom + GetStyle().ItemInnerSpacing.x,
        style.image_icon_size * zoom)
    var image_layout_revision = 0
    let cached_content & = unsafe(state.nodes[node_index].content)
    for (run in cached_content.runs) {
        if (!run.is_image) continue
        let binding_index = image_binding_index(
            state, state.document_revision, run.semantic_node)
        if (binding_index >= 0) {
            image_layout_revision = (image_layout_revision * 16777619
                + state.image_bindings[binding_index].view_revision)
        }
    }
    var key = MarkdownLayoutKey(
        max_width = available, line_height = line_height, font_size = font_size,
        inline_code_scale = has_style(forced_style, STYLE_CODE)
            ? 1.0f : max(style.typography.inline_code_scale, 0.1f),
        icon_size = icon_full, image_icon_size = image_icon_full,
        forced_style = forced_style,
        show_link_copy = style.show_link_copy,
        image_layout_revision = image_layout_revision,
        syntax_layout_revision = state.syntax_bindings_revision,
        regular_id = unsafe(reinterpret<uint64>(style.fonts.regular)),
        bold_id = unsafe(reinterpret<uint64>(style.fonts.bold)),
        italic_id = unsafe(reinterpret<uint64>(style.fonts.italic)),
        bold_italic_id = unsafe(reinterpret<uint64>(style.fonts.bold_italic)),
        monospace_id = unsafe(reinterpret<uint64>(style.fonts.monospace)),
        fallback_id = unsafe(reinterpret<uint64>(fallback_font)))
    ensure_layout(doc, node_index, state, key, style, fallback_font)
    let content & = unsafe(state.nodes[node_index].content)
    let layout & = unsafe(state.nodes[node_index].layout)
    PushID(node_index)

    let origin = GetCursorScreenPos()
    var cache & = unsafe(state.nodes[node_index])
    cache.screen_origin = origin
    cache.screen_revision = state.render_revision
    cache.screen_available_width = available
    cache.screen_alignment = alignment
    Dummy(float2(available, max(layout.size.y, line_height)))

    var hovered = -1
    var hovered_fragment_index = -1
    var hovered_image_control = false
    for (fragment_index in range(length(layout.fragments))) {
        let fragment = layout.fragments[fragment_index]
        let rect = markdown_cached_fragment_rect(cache, fragment)
        if (!fragment.is_control && IsMouseHoveringRect(
                float2(rect.x, rect.y), float2(rect.z, rect.w), true)) {
            hovered_fragment_index = fragment_index
        }
        if (fragment.semantic_id >= 0 && IsMouseHoveringRect(
                float2(rect.x, rect.y), float2(rect.z, rect.w), true)) {
            hovered = fragment.semantic_id
            hovered_image_control = (fragment.is_control
                && fragment.control_kind == CONTROL_IMAGE_PLACEHOLDER)
        }
    }

    if (hovered_image_control && IsMouseClicked(ImGuiMouseButton.Left, false)) {
        state.selection_pressed_semantic = hovered
        state.selection_dragged = false
        state.selection_click_claimed = true
    }

    var hit = TextFlowHit()
    if (hovered_fragment_index >= 0) {
        let hovered_fragment = layout.fragments[hovered_fragment_index]
        let align_x = markdown_line_alignment_offset(cache, hovered_fragment.line_index)
        hit = text_flow_hit_test_ranges(content.text, layout,
            GetMousePos() - origin - float2(align_x, 0.0f)) $(text : string;
                                                              start_byte, end_byte, style_id : int) : float {
                var font = font_for_style(style_id, style.fonts, fallback_font)
                let styled_size = font_size_for_style(
                    font_size, key.inline_code_scale, style_id)
                return CalcTextSizeForFontRange(font, styled_size, text, length(text),
                                                start_byte, end_byte).x
            }
        }
    let hit_visible = hit.valid && hit.fragment_index >= 0
    if (hit_visible) {
        state.hovered_inline_node = node_index
        state.hovered_fragment = hit.fragment_index
        state.hovered_display_byte = hit.byte_offset
        let position = md_inline_position(content, node_index, hit.byte_offset, hit.trailing)
        if (position.valid && IsMouseClicked(ImGuiMouseButton.Left, false)) {
            state.selection_anchor = position
            state.selection_focus = position
            state.selection_dragging = true
            state.selection_dragged = false
            state.selection_pressed_semantic = hovered
            state.selection_click_claimed = true
            state.selection_changed = true
            state.selection_revision++
        } elif (position.valid && state.selection_dragging
                && IsMouseDown(ImGuiMouseButton.Left)) {
            if (state.selection_focus.source_byte != position.source_byte
                    || state.selection_focus.inline_node != position.inline_node
                    || state.selection_focus.display_byte != position.display_byte) {
                state.selection_focus = position
                state.selection_changed = true
                state.selection_revision++
            }
            state.selection_dragged = (state.selection_anchor.source_byte
                != state.selection_focus.source_byte)
        }
    }
    if (hovered >= 0) {
        result.hovered = true
        result.hovered_node = hovered
        let run_index = link_run_index(doc, content, hovered)
        if (run_index >= 0) {
            result.hovered_node = content.runs[run_index].semantic_node
            result.source = content.runs[run_index].source
            result.hovered_image = content.runs[run_index].is_image
            result.hovered_image_resolved = (result.hovered_image
                && markdown_view_image_bound(state, state.document_revision,
                                             result.hovered_node))
        }
        result.target := link_target(doc, content, hovered)
    }

    var dl = GetWindowDrawList()
    // Inline-code/image backgrounds sit below selection, which sits below text.
    for (fragment in layout.fragments) {
        if (fragment.is_control) continue
        let screen_rect = markdown_cached_fragment_rect(cache, fragment)
        let p0 = float2(screen_rect.x, screen_rect.y)
        let p1 = float2(screen_rect.z, screen_rect.w)
        let hot_link = fragment.semantic_id >= 0 && fragment.semantic_id == hovered
        if (has_style(fragment.style_id, STYLE_IMAGE)) {
            *dl |> AddRectFilled(p0 - float2(1.0f * zoom, 0.0f),
                                 p1 + float2(3.0f * zoom, 0.0f),
                                 GetColorU32(style.image_background), 2.0f * zoom,
                                 ImDrawFlags.None)
        }
        if (has_style(fragment.style_id, STYLE_CODE)) {
            *dl |> AddRectFilled(p0 - float2(2.0f * zoom, 0.0f),
                                 p1 + float2(2.0f * zoom, 0.0f),
                                 GetColorU32(style.code_background), 2.0f * zoom,
                                 ImDrawFlags.None)
        }
    }

    // Paint one union rect per visual line (not per run, which left transparent selected spaces).
    let selection_source = selected_source(state)
    if (selection_source.start_byte >= 0
            && selection_source.end_byte > selection_source.start_byte) {
        for (line in layout.lines) {
            var found = false
            var line_rect = float4()
            for (fragment_index in range(
                    line.first_fragment,
                    line.first_fragment + line.fragment_count)) {
                let fragment = layout.fragments[fragment_index]
                if (fragment.is_control) continue
                let screen_rect = markdown_cached_fragment_rect(cache, fragment)
                let p0 = float2(screen_rect.x, screen_rect.y)
                var font = font_for_style(
                    fragment.style_id, style.fonts, fallback_font)
                let styled_size = font_size_for_style(
                    font_size, key.inline_code_scale, fragment.style_id)
                var fragment_selected = false
                var fragment_rect = float4()
            for (run in content.runs) {
                let display = md_selected_display_range(run, selection_source)
                let a = max(display.start_byte, fragment.source.start_byte)
                let b = min(display.end_byte, fragment.source.end_byte)
                if (display.start_byte < 0 || b <= a) continue
                let prefix = CalcTextSizeForFontRange(font, styled_size, content.text,
                    length(content.text), fragment.source.start_byte, a).x
                let width = CalcTextSizeForFontRange(font, styled_size, content.text,
                    length(content.text), a, b).x
                    let selected = float4(
                        p0.x + prefix, screen_rect.y,
                        p0.x + prefix + width, screen_rect.w)
                    if (!fragment_selected) {
                        fragment_rect = selected
                        fragment_selected = true
                    } else {
                        fragment_rect.x = min(fragment_rect.x, selected.x)
                        fragment_rect.y = min(fragment_rect.y, selected.y)
                        fragment_rect.z = max(fragment_rect.z, selected.z)
                        fragment_rect.w = max(fragment_rect.w, selected.w)
                    }
                }
                if (fragment_selected) {
                    if (!found) {
                        line_rect = fragment_rect
                        found = true
                    } else {
                        line_rect.x = min(line_rect.x, fragment_rect.x)
                        line_rect.y = min(line_rect.y, fragment_rect.y)
                        line_rect.z = max(line_rect.z, fragment_rect.z)
                        line_rect.w = max(line_rect.w, fragment_rect.w)
                    }
                }
            }
            if (found) {
                *dl |> AddRectFilled(
                    float2(line_rect.x, line_rect.y),
                    float2(line_rect.z, line_rect.w),
                    GetColorU32(style.selection_color), 1.5f * zoom,
                    ImDrawFlags.None)
                include_selection_screen_rect(state, line_rect)
                state.selection_paint_rect_count++
                if (IsMouseReleased(ImGuiMouseButton.Right)
                        && IsMouseHoveringRect(
                            float2(line_rect.x, line_rect.y),
                            float2(line_rect.z, line_rect.w), true)) {
                    result.selection_context_requested = true
                    result.selection_context_pos = GetMousePos()
                    state.selection_context_pos = result.selection_context_pos
                    state.selection_context_revision++
                }
            }
        }
    }

    var copy_control_hovered = false
    for (fragment in layout.fragments) {
        if (fragment.is_control) continue
        let screen_rect = markdown_cached_fragment_rect(cache, fragment)
        let p0 = float2(screen_rect.x, screen_rect.y)
        let p1 = float2(screen_rect.z, screen_rect.w)
        let hot_link = fragment.semantic_id >= 0 && fragment.semantic_id == hovered
        var color = style.text_color
        if (has_style(fragment.style_id, STYLE_LINK)) {
            color = hot_link ? style.link_hover_color : style.link_color
        } elif (fragment.syntax_style_id >= 0) {
            color = syntax_color(fragment.syntax_style_id, style)
        } elif (has_style(fragment.style_id, STYLE_CODE)) {
            color = style.code_color
        } elif (has_style(fragment.style_id, STYLE_STRONG)) {
            color = style.strong_color
        } elif (has_style(fragment.style_id, STYLE_EMPHASIS)) {
            color = style.emphasis_color
        }
        if (has_style(fragment.style_id, STYLE_IMAGE)) {
            color = hot_link ? style.image_hover_color : style.image_color
        }
        var font = font_for_style(fragment.style_id, style.fonts, fallback_font)
        let styled_size = font_size_for_style(
            font_size, key.inline_code_scale, fragment.style_id)
        let text_y = p0.y + max(0.0f, (line_height - styled_size) * 0.5f)
        if (position_in_fragment(state.selection_focus, node_index, fragment)) {
            let caret_byte = clamp(state.selection_focus.display_byte,
                fragment.source.start_byte, fragment.source.end_byte)
            let caret_x = CalcTextSizeForFontRange(font, styled_size, content.text,
                length(content.text), fragment.source.start_byte, caret_byte).x
            let caret_top = float2(p0.x + caret_x, p0.y)
            state.selection_screen_anchor = float2(caret_top.x, p1.y)
            state.caret_screen_valid = true
            state.caret_screen_pos = caret_top
            state.caret_screen_height = p1.y - p0.y
            if (selection_source.start_byte == selection_source.end_byte
                    && IsWindowFocused(ImGuiFocusedFlags.RootAndChildWindows)) {
                *dl |> AddLine(caret_top, float2(caret_top.x, p1.y),
                               GetColorU32(style.text_color), max(1.0f, zoom))
            }
        }
        *dl |> AddTextRange(font, styled_size, float2(p0.x, text_y), GetColorU32(color),
                           content.text, length(content.text), fragment.source.start_byte,
                           fragment.source.end_byte)
        if (hot_link) {
            let y = text_y + styled_size - max(1.0f, zoom)
            *dl |> AddLine(float2(p0.x, y), float2(p1.x, y),
                           GetColorU32(color), max(1.0f, zoom))
        }
        if (has_style(fragment.style_id, STYLE_STRIKE)) {
            let y = text_y + styled_size * 0.52f
            *dl |> AddLine(float2(p0.x, y), float2(p1.x, y),
                           GetColorU32(color), max(1.0f, zoom))
        }
    }

    if (hovered >= 0) {
        SetMouseCursor(ImGuiMouseCursor.Hand)
        if (IsMouseReleased(ImGuiMouseButton.Left)
                && state.selection_pressed_semantic == hovered
                && !state.selection_dragged) {
            let run_index = link_run_index(doc, content, hovered)
            result.activated = true
            result.activated_node = (run_index >= 0
                ? content.runs[run_index].semantic_node : -1)
            if (run_index >= 0) {
                result.source = content.runs[run_index].source
                result.activated_image = content.runs[run_index].is_image
                result.activated_image_resolved = (result.activated_image
                    && markdown_view_image_bound(state, state.document_revision,
                                                 result.activated_node))
            }
            result.target := link_target(doc, content, hovered)
        }
    }
    for (fragment in layout.fragments) {
        if (!fragment.is_control) continue
        let screen_rect = markdown_cached_fragment_rect(cache, fragment)
        if (fragment.control_kind == CONTROL_IMAGE_PLACEHOLDER) {
            let p0 = float2(screen_rect.x, screen_rect.y)
            let p1 = float2(screen_rect.z, screen_rect.w)
            let hot = fragment.control_id == hovered
            let border = hot ? style.image_hover_color : style.image_border
            let binding_index = image_binding_index(
                state, state.document_revision, fragment.control_id)
            if (binding_index >= 0) {
                let binding = state.image_bindings[binding_index]
                let scaled_size = binding.display_size * zoom
                let image_p0 = float2(p0.x,
                    p0.y + max(0.0f, (p1.y - p0.y - scaled_size.y) * 0.5f))
                let image_p1 = image_p0 + scaled_size
                unsafe {
                    var texture = ImTextureRef(binding.texture_id)
                    *dl |> AddImage(texture, image_p0, image_p1,
                                    binding.uv0, binding.uv1,
                                    GetColorU32(binding.tint_color))
                }
                *dl |> AddRect(image_p0, image_p1, GetColorU32(border),
                               2.0f * zoom, ImDrawFlags.None, max(1.0f, zoom))
                continue
            }
            let pad = 2.0f * zoom
            *dl |> AddRectFilled(p0, p1, GetColorU32(style.image_background),
                                 2.0f * zoom, ImDrawFlags.None)
            *dl |> AddRect(p0 + float2(pad, pad), p1 - float2(pad, pad),
                           GetColorU32(border), 1.5f * zoom,
                           ImDrawFlags.None, max(1.0f, zoom))
            let inner0 = p0 + float2(pad * 1.7f, pad * 1.7f)
            let inner1 = p1 - float2(pad * 1.7f, pad * 1.7f)
            let sun = float2(inner1.x - (inner1.x - inner0.x) * 0.22f,
                             inner0.y + (inner1.y - inner0.y) * 0.24f)
            *dl |> AddCircleFilled(sun, max(1.0f, 1.5f * zoom),
                                   GetColorU32(style.image_color), 8)
            let ridge_y = inner1.y - (inner1.y - inner0.y) * 0.24f
            *dl |> AddLine(float2(inner0.x, ridge_y),
                           float2(inner0.x + (inner1.x - inner0.x) * 0.38f,
                                  inner0.y + (inner1.y - inner0.y) * 0.48f),
                           GetColorU32(border), max(1.0f, zoom))
            *dl |> AddLine(float2(inner0.x + (inner1.x - inner0.x) * 0.38f,
                                  inner0.y + (inner1.y - inner0.y) * 0.48f),
                           float2(inner1.x, ridge_y),
                           GetColorU32(border), max(1.0f, zoom))
            continue
        }
        if (fragment.control_kind != CONTROL_COPY_URI) continue
        let visible = fragment.control_id == hovered
        if (icon_button_draw_at("##markdown_copy_{fragment.control_id}", "duplicate",
                                float2(screen_rect.x, screen_rect.y),
                                style.copy_icon_size * zoom, 1.0f, visible)) {
            let control_id = fragment.control_id
            if (control_id >= 0) {
                let run_index = link_run_index(doc, content, control_id)
                let target = link_target(doc, content, control_id)
                result.copy_requested = true
                result.copy_text := target
                result.copied_node = (run_index >= 0
                    ? content.runs[run_index].semantic_node : -1)
                if (run_index >= 0) {
                    result.source = content.runs[run_index].source
                }
                result.target := target
                if (style.write_system_clipboard) {
                    result.copied = (markdown_copy_uri_to_clipboard(target) == ClipboardStatus.ok)
                }
            }
        }
        if (visible && IsItemHovered()) {
            copy_control_hovered = true
            SetTooltip("Copy URI")
        }
    }
    if (hovered >= 0 && !copy_control_hovered) {
        let run_index = link_run_index(doc, content, hovered)
        if (run_index >= 0) {
            let run = content.runs[run_index]
            if (run.is_image) {
                let alt = empty(run.image_alt) ? "Image" : run.image_alt
                let detail = (empty(run.title) ? run.destination
                    : "{run.title}\n{run.destination}")
                if (!empty(detail)) SetTooltip("{alt}\n{detail}")
            } elif (!empty(run.title)) {
                SetTooltip(run.title)
            }
        }
    }
    PopID()
}

def private block_spacing(var style : MarkdownViewStyle; amount : float) {
    if (amount > 0.0f) Dummy(float2(0.0f, zoomed(style, amount)))
}

def private render_children(doc : RichDocument const; node_index : int;
                            var state : MarkdownViewState;
                            var style : MarkdownViewStyle;
                            var result : MarkdownViewResult) {
    var child = doc.nodes[node_index].first_child
    while (child >= 0) {
        render_block(doc, child, state, style, result)
        child = doc.nodes[child].next_sibling
    }
}

def private render_list(doc : RichDocument const; node_index : int;
                        var state : MarkdownViewState;
                        var style : MarkdownViewStyle;
                        var result : MarkdownViewResult) {
    let list = doc.nodes[node_index]
    var item = list.first_child
    var ordinal = list.start_number > 0 ? list.start_number : 1
    while (item >= 0) {
        let node = doc.nodes[item]
        var cache & = unsafe(state.nodes[item])
        if (!cache.marker_valid) {
            cache.marker = list.kind == RichNodeKind.ordered_list ? "{ordinal}." : "•"
            cache.marker_valid = true
        }
        cache.control_screen_valid = false
        if (node.is_task) {
            let marker_pos = GetCursorScreenPos()
            // Reserve one state-independent square and draw both states ourselves, so toggling can never reflow.
            let marker_edge = max(GetTextLineHeight(), 1.0f)
            let marker_size = float2(marker_edge, marker_edge)
            PushID(item)
            let pressed = InvisibleButton("##task_toggle", marker_size)
            PopID()
            let hovered = IsItemHovered(ImGuiHoveredFlags.None)
            let marker_max = marker_pos + marker_size
            cache.control_screen_valid = true
            cache.control_screen_rect = float4(
                marker_pos.x, marker_pos.y, marker_max.x, marker_max.y)
            var dl = GetWindowDrawList()
            let box_edge = marker_edge * 0.68f
            let box_pad = (marker_edge - box_edge) * 0.5f
            let box0 = marker_pos + float2(box_pad, box_pad)
            let box1 = box0 + float2(box_edge, box_edge)
            let color = hovered ? style.link_hover_color : style.marker_color
            let thickness = max(1.0f, box_edge * 0.09f)
            *dl |> AddRect(box0, box1, GetColorU32(color),
                           max(1.0f, box_edge * 0.10f),
                           ImDrawFlags.None, thickness)
            if (node.is_checked) {
                *dl |> AddLine(
                    box0 + float2(box_edge * 0.18f, box_edge * 0.52f),
                    box0 + float2(box_edge * 0.42f, box_edge * 0.74f),
                    GetColorU32(color), thickness)
                *dl |> AddLine(
                    box0 + float2(box_edge * 0.42f, box_edge * 0.74f),
                    box0 + float2(box_edge * 0.84f, box_edge * 0.25f),
                    GetColorU32(color), thickness)
            }
            if (pressed) {
                result.control_event = RichControlEvent(
                    active = true,
                    control_id = item,
                    kind = RichControlKind.toggle,
                    requested_checked = !node.is_checked,
                    source_revision = state.document_revision)
                state.control_activation_revision++
                state.activated_control_id = item
                state.selection_click_claimed = true
            }
        } else {
            TextColored(style.marker_color, cache.marker)
        }
        SameLine(0.0f, GetStyle().ItemInnerSpacing.x)
        var has_direct_inline = false
        var direct_child = node.first_child
        while (direct_child >= 0) {
            if (md_node_kind_is_inline(doc.nodes[direct_child].kind)) {
                has_direct_inline = true
                break
            }
            direct_child = doc.nodes[direct_child].next_sibling
        }
        if (has_direct_inline) {
            render_inline(doc, item, state, GetContentRegionAvail().x,
                          style, 1.0f, 0, RichAlignment.default_, result, true)
            block_spacing(style, style.paragraph_spacing)
        }
        var child = node.first_child
        while (child >= 0) {
            let kind = doc.nodes[child].kind
            if (!md_node_kind_is_inline(kind)) {
                render_block(doc, child, state, style, result)
            }
            child = doc.nodes[child].next_sibling
        }
        item = node.next_sibling
        ordinal++
    }
    block_spacing(style, list.is_tight ? 2.0f : style.block_spacing)
}

def private render_table_section(doc : RichDocument const;
                                 section_index : int; headers : bool;
                                 var state : MarkdownViewState;
                                 var style : MarkdownViewStyle;
                                 var result : MarkdownViewResult) {
    var row = doc.nodes[section_index].first_child
    while (row >= 0) {
        TableNextRow(headers ? ImGuiTableRowFlags.Headers : ImGuiTableRowFlags.None, 0.0f)
        var cell = doc.nodes[row].first_child
        var column = 0
        while (cell >= 0) {
            if (TableSetColumnIndex(column)) {
                render_inline(doc, cell, state, GetContentRegionAvail().x,
                              style, 1.0f, 0, doc.nodes[cell].alignment, result)
            }
            column++
            cell = doc.nodes[cell].next_sibling
        }
        row = doc.nodes[row].next_sibling
    }
}

def private render_table(doc : RichDocument const; node_index : int;
                         var state : MarkdownViewState;
                         var style : MarkdownViewStyle;
                         var result : MarkdownViewResult) {
    let node = doc.nodes[node_index]
    if (node.column_count <= 0) return
    PushID(node_index)
    if (BeginTable("##markdown_table", node.column_count,
                   ImGuiTableFlags.Borders, float2(0.0f, 0.0f), 0.0f)) {
        for (column in range(node.column_count)) {
            TableSetupColumn("##column", ImGuiTableColumnFlags.None,
                             0.0f, uint(column))
        }
        var section = node.first_child
        while (section >= 0) {
            let is_head = doc.nodes[section].kind == RichNodeKind.table_head
            render_table_section(doc, section, is_head, state, style, result)
            section = doc.nodes[section].next_sibling
        }
        EndTable()
    }
    PopID()
    block_spacing(style, style.block_spacing)
}

def private render_code_block(doc : RichDocument const; node_index : int;
                              var state : MarkdownViewState;
                              var style : MarkdownViewStyle;
                              var result : MarkdownViewResult) {
    ensure_inline(doc, node_index, state)
    let content & = unsafe(state.nodes[node_index].content)
    let zoom = view_zoom(style)
    let code_scale = max(style.typography.code_block_scale, 0.1f)
    let font_size = (style.fonts.size_px > 0.0f
        ? style.fonts.size_px : GetFontSize()) * zoom * code_scale
    let padding = style.code_padding * zoom
    var mono = style.fonts.monospace != null ? style.fonts.monospace : GetFont()
    let measured = CalcTextSizeForFont(mono, font_size, content.text)
    let child_border = GetStyle().ChildBorderSize
    let window_padding = GetStyle().WindowPadding.y
    let height = max(font_size * 2.0f,
        measured.y + padding * 2.0f + child_border * 2.0f
        + window_padding * 2.0f + GetStyle().ItemSpacing.y + 2.0f * zoom)
    PushStyleColor(ImGuiCol.ChildBg, style.code_background)
    PushID(node_index)
    let open = BeginChild("##markdown_code",
        float2(0.0f, height), ImGuiChildFlags.Borders,
        ImGuiWindowFlags.NoScrollbar | ImGuiWindowFlags.NoScrollWithMouse)
    if (open) {
        SetCursorPos(GetCursorPos() + float2(padding, padding))
        var code_style := style
        code_style.text_color = style.code_color
        code_style.show_link_copy = false
        let content_width = max(measured.x, GetContentRegionAvail().x - padding)
        render_inline(doc, node_index, state, max(content_width, 1.0f),
                      code_style, code_scale, STYLE_CODE,
                      RichAlignment.default_, result)
    }
    EndChild()
    PopID()
    PopStyleColor(1)
    block_spacing(style, style.block_spacing)
}

def private render_block(doc : RichDocument const; node_index : int;
                         var state : MarkdownViewState;
                         var style : MarkdownViewStyle;
                         var result : MarkdownViewResult) {
    if (node_index < 0 || node_index >= length(doc.nodes)) return
    let node = doc.nodes[node_index]
    if (node.kind == RichNodeKind.document) {
        render_children(doc, node_index, state, style, result)
    } elif (node.kind == RichNodeKind.paragraph) {
        render_inline(doc, node_index, state, GetContentRegionAvail().x,
                      style, 1.0f, 0, RichAlignment.default_, result)
        block_spacing(style, style.paragraph_spacing)
    } elif (node.kind == RichNodeKind.heading) {
        let scale = max(heading_scale(style.typography, node.level), 0.1f)
        var heading_style := style
        heading_style.text_color = style.heading_color
        render_inline(doc, node_index, state, GetContentRegionAvail().x,
                      heading_style, scale, STYLE_HEADING, RichAlignment.default_, result)
        block_spacing(style, style.block_spacing)
    } elif (node.kind == RichNodeKind.unordered_list
            || node.kind == RichNodeKind.ordered_list) {
        render_list(doc, node_index, state, style, result)
    } elif (node.kind == RichNodeKind.quote) {
        let p0 = GetCursorScreenPos()
        let indent = zoomed(style, style.quote_indent)
        Indent(indent)
        render_children(doc, node_index, state, style, result)
        Unindent(indent)
        let p1 = GetCursorScreenPos()
        var dl = GetWindowDrawList()
        let bar_x = zoomed(style, 3.0f)
        *dl |> AddLine(p0 + float2(bar_x, 0.0f),
                       float2(p0.x + bar_x, p1.y),
                       GetColorU32(style.quote_color), zoomed(style, 2.0f))
    } elif (node.kind == RichNodeKind.thematic_break) {
        Separator()
        block_spacing(style, style.block_spacing)
    } elif (node.kind == RichNodeKind.code_block) {
        render_code_block(doc, node_index, state, style, result)
    } elif (node.kind == RichNodeKind.table_) {
        render_table(doc, node_index, state, style, result)
    } elif (node.kind == RichNodeKind.raw_html) {
        ensure_inline(doc, node_index, state)
        let content & = unsafe(state.nodes[node_index].content)
        TextColored(style.marker_color, content.text)
        block_spacing(style, style.block_spacing)
    } else {
        render_children(doc, node_index, state, style, result)
    }
}

def private finish_selection(doc : RichDocument const;
                             var state : MarkdownViewState;
                             var result : MarkdownViewResult) {
    let source = selected_source(state)
    if (source.start_byte < 0 || source.end_byte <= source.start_byte) return
    var accumulator = MdSelectionStyleAccumulator()
    for (i in range(length(state.nodes))) {
        if (state.nodes[i].content_valid) {
            let content & = unsafe(state.nodes[i].content)
            md_selection_include(accumulator, content, source)
        }
    }
    result.selection = MarkdownSelection(
        active = true,
        source = source,
        styles = md_selection_styles(accumulator),
        screen_valid = state.selection_screen_valid,
        screen_bounds = state.selection_screen_bounds,
        screen_anchor = state.selection_screen_anchor)
}

def public markdown_selection_text(state : MarkdownViewState const) : string {
    //! Flatten the selected display text on demand. Differs from `md_source_text`: Markdown
    //! delimiters are omitted and transformed entities keep the glyphs shown by the viewer;
    //! separate inline blocks are joined with a newline.
    let source = selected_source(state)
    if (source.start_byte < 0 || source.end_byte <= source.start_byte) return ""
    var value = ""
    for (i in range(length(state.nodes))) {
        if (!state.nodes[i].content_valid) continue
        let content & = unsafe(state.nodes[i].content)
        var block_text = ""
        for (run in content.runs) {
            let display = md_selected_display_range(run, source)
            if (display.start_byte >= 0 && display.end_byte > display.start_byte) {
                block_text = block_text + slice(content.text, display.start_byte, display.end_byte)
            }
        }
        if (!empty(block_text)) {
            if (!empty(value)) {
                value = value + "\n"
            }
            value = value + block_text
        }
    }
    return value
}

def public markdown_selection_export(state : MarkdownViewState const) : MdSelectionExport {
    //! Build reusable rich representations without touching the OS clipboard.
    //! Image runs become HTML `<img>` plus URI-list entries; host-owned pixels
    //! remain a separate client-supplied clipboard concern.
    var result = MdSelectionExport()
    let source = selected_source(state)
    if (source.start_byte < 0 || source.end_byte <= source.start_byte) return <- result
    for (i in range(length(state.nodes))) {
        if (!state.nodes[i].content_valid) continue
        md_selection_export_include(result, state.nodes[i].content, source)
    }
    return <- result
}

def public markdown_view(id : string; doc : RichDocument const;
                         var state : MarkdownViewState;
                         var style : MarkdownViewStyle;
                         document_revision : int = 0) : MarkdownViewResult {
    var result = MarkdownViewResult()
    if (!doc.ok || doc.root < 0) {
        TextColored(float4(1.0f, 0.35f, 0.35f, 1.0f),
                    empty(doc.error) ? "Invalid Markdown document" : doc.error)
        return <- result
    }
    prepare_view_state(doc, state, document_revision)
    state.render_revision++
    // Stable visible bounds of the scrolling window. Cursor screen Y moves
    // with ScrollY and therefore cannot serve as the viewport edge.
    state.viewport_top = GetWindowPos().y + GetFrameHeight()
    state.viewport_bottom = (GetWindowPos().y + GetWindowSize().y
        - GetStyle().WindowPadding.y)
    let old_hovered_inline_node = state.hovered_inline_node
    let old_hovered_fragment = state.hovered_fragment
    let old_hovered_display_byte = state.hovered_display_byte
    state.hovered_inline_node = -1
    state.hovered_fragment = -1
    state.hovered_display_byte = -1
    state.caret_screen_valid = false
    state.caret_screen_height = 0.0f
    state.selection_changed = false
    state.selection_click_claimed = false
    state.selection_screen_valid = false
    state.selection_paint_rect_count = 0
    if (IsMouseClicked(ImGuiMouseButton.Left, false)) {
        state.selection_pressed_semantic = -1
    }
    if (state.selection_dragging
            && IsMouseDragging(ImGuiMouseButton.Left, 2.0f)) {
        state.selection_dragged = true
    }
    navigate_selection_keyboard(state, style)
    var pushed_font = false
    if (style.fonts.regular != null) {
        let size = (style.fonts.size_px > 0.0f
            ? style.fonts.size_px : GetFontSize()) * view_zoom(style)
        PushFont(style.fonts.regular, size)
        pushed_font = true
    }
    PushID(id)
    render_block(doc, doc.root, state, style, result)
    PopID()
    ensure_keyboard_caret_visible(state)
    if (IsMouseClicked(ImGuiMouseButton.Left, false)
            && !state.selection_click_claimed) {
        let old_selection = selected_source(state)
        let had_selection = old_selection.end_byte > old_selection.start_byte
        markdown_view_clear_selection(state)
        state.selection_changed = had_selection
    }
    if (IsMouseReleased(ImGuiMouseButton.Left)) {
        if (state.selection_dragging) {
            state.selection_commit_revision++
        }
        state.selection_dragging = false
    }
    finish_selection(doc, state, result)
    result.selection_changed = state.selection_changed
    let copy_requested = state.selection_copy_pending
    state.selection_copy_pending = false
    if (result.selection.active && copy_requested) {
        var copy <- markdown_selection_export(state)
        result.selection_copy_requested = true
        result.selection_copy_text := copy.plain_text
        result.selection_copy_markdown := copy.markdown
        result.selection_copy_html := copy.html
        result.selection_copy_uri_list := copy.uri_list
        state.selection_copy_text := copy.plain_text
        state.selection_copy_markdown := copy.markdown
        state.selection_copy_html := copy.html
        state.selection_copy_uri_list := copy.uri_list
        state.selection_copy_revision++
        if (style.write_system_clipboard) {
            var content = ClipboardContent(
                plain_text = copy.plain_text,
                markdown = copy.markdown,
                html = copy.html,
                uri_list = copy.uri_list)
            content.formats.plain_text = true
            content.formats.markdown = true
            content.formats.html = true
            content.formats.uri_list = !empty(copy.uri_list)
            result.selection_copied = (clipboard_set_content(content) == ClipboardStatus.ok)
            clipboard_dispose_content(content)
        }
    }
    if (old_hovered_inline_node != state.hovered_inline_node
            || old_hovered_fragment != state.hovered_fragment
            || old_hovered_display_byte != state.hovered_display_byte) {
        state.hover_revision++
    }
    if (pushed_font) {
        PopFont()
    }
    return <- result
}

def public markdown_view(id : string; doc : RichDocument const;
                         var style : MarkdownViewStyle) : MarkdownViewResult {
    //! Compatibility form. For live applications prefer the overload taking
    //! caller-owned `MarkdownViewState`, so stable frames reuse all layout.
    var state = MarkdownViewState()
    var result <- markdown_view(id, doc, state, style, 0)
    markdown_view_dispose(state)
    return <- result
}

def private markdown_source_range_changed(source : TextSourceRange const;
                                          ranges : array<TextSourceRange> const) : bool {
    if (source.start_byte < 0 || source.end_byte <= source.start_byte
            || empty(ranges)) return false
    var lo = 0
    var hi = length(ranges)
    while (lo < hi) {
        let mid = (lo + hi) / 2
        if (ranges[mid].end_byte <= source.start_byte) {
            lo = mid + 1
        } else {
            hi = mid
        }
    }
    return (lo < length(ranges) && ranges[lo].start_byte < source.end_byte)
}

def private markdown_view_change_gutter_at(doc : RichDocument const;
                                           state : MarkdownViewState const;
                                           changed_ranges : array<TextSourceRange> const;
                                           gutter_x, width : float;
                                           color : float4) : int {
    // Paint a change marker beside every rendered block whose source overlaps a changed range (ranges must be sorted).
    if (empty(changed_ranges)) return 0
    var dl = GetWindowDrawList()
    let window_pos = GetWindowPos()
    let window_size = GetWindowSize()
    let x0 = gutter_x
    let x1 = x0 + max(1.0f, width)
    let visible_top = window_pos.y
    let visible_bottom = window_pos.y + window_size.y
    let packed_color = GetColorU32(color)
    var segments = 0
    for (index in range(min(length(doc.nodes), length(state.nodes)))) {
        let node & = unsafe(doc.nodes[index])
        let cache & = unsafe(state.nodes[index])
        if (!cache.layout_valid || cache.screen_revision != state.render_revision
                || !markdown_source_range_changed(node.source, changed_ranges)) continue
        let y0 = cache.screen_origin.y
        let y1 = y0 + max(cache.layout.size.y, GetTextLineHeight())
        if (y1 < visible_top || y0 > visible_bottom) continue
        *dl |> AddRectFilled(float2(x0, y0), float2(x1, y1), packed_color,
            0.0f, ImDrawFlags.None)
        segments++
    }
    return segments
}

def public markdown_view_with_change_gutter(id : string;
                                            doc : RichDocument const;
                                            var state : MarkdownViewState;
                                            var style : MarkdownViewStyle;
                                            document_revision : int;
                                            changed_ranges : array<TextSourceRange> const;
                                            var gutter_segments : int&;
                                            width : float = 2.0f;
                                            color : float4 = float4(0.30f, 0.76f, 0.46f, 1.0f)) : MarkdownViewResult {
    //! Reserve a dedicated strip before rich content and paint change markers
    //! only inside it, so the gutter can never cover text.
    let gutter_width = max(1.0f, width)
    let gutter_space = gutter_width + max(4.0f, GetStyle().ItemSpacing.x)
    let gutter_x = GetCursorScreenPos().x
    Indent(gutter_space)
    var result <- markdown_view(id, doc, state, style, document_revision)
    Unindent(gutter_space)
    gutter_segments = markdown_view_change_gutter_at(
        doc, state, changed_ranges, gutter_x, gutter_width, color)
    return <- result
}

def public markdown_view_current_layout_count(state : MarkdownViewState const) : int {
    var count = 0
    for (cache in state.nodes) {
        if (cache.layout_valid && cache.screen_revision == state.render_revision) count++
    }
    return count
}

def public markdown_view_changed_layout_count(doc : RichDocument const;
                                              state : MarkdownViewState const;
                                              changed_ranges : array<TextSourceRange> const) : int {
    var count = 0
    for (index in range(min(length(doc.nodes), length(state.nodes)))) {
        let node & = unsafe(doc.nodes[index])
        let cache & = unsafe(state.nodes[index])
        if (cache.layout_valid && cache.screen_revision == state.render_revision
                && markdown_source_range_changed(node.source, changed_ranges)) count++
    }
    return count
}

def public markdown_view(id : string; doc : RichDocument const) : MarkdownViewResult {
    var style = MarkdownViewStyle()
    return <- markdown_view(id, doc, style)
}

def public rich_document_view(id : string; doc : RichDocument const;
                              var state : MarkdownViewState;
                              var style : MarkdownViewStyle;
                              document_revision : int = 0) : MarkdownViewResult {
    //! Format-neutral entry point. `markdown_view` remains as the compatibility
    //! name while its renderer is shared by every RichDocument producer.
    return <- markdown_view(id, doc, state, style, document_revision)
}

def public rich_document_view(id : string; doc : RichDocument const;
                              var style : MarkdownViewStyle) : MarkdownViewResult {
    return <- markdown_view(id, doc, style)
}

def public rich_document_view(id : string;
                              doc : RichDocument const) : MarkdownViewResult {
    return <- markdown_view(id, doc)
}
