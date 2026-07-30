options gen2
options _comment_hygiene = true
options indenting = 4

module imgui_markdown_text shared

require imgui public
require imgui/imgui_text_flow public
require imgui/imgui_icons
require clipboard/clipboard
require daslib/strings_boost
require math

//! First Markdown UI vertical slice: a mixed paragraph containing one semantic
//! URI. The function exercises the reusable flow layout before a Markdown
//! parser is introduced. One URI may produce many visual fragments, while
//! hover/click/copy behavior remains attached to its single semantic id.

struct public MarkdownUriStyle {
    body_color : float4 = float4(0.86f, 0.86f, 0.86f, 1.0f)
    link_color : float4 = float4(0.36f, 0.68f, 1.0f, 1.0f)
    link_hover_color : float4 = float4(0.55f, 0.80f, 1.0f, 1.0f)
    underline_thickness : float = 1.0f
    copy_icon_size : float = 14.0f
}

struct public MarkdownUriResult {
    hovered : bool
    activated : bool
    copied : bool
    line_count : int
}

def private markdown_uri_copy(target : string) : ClipboardStatus {
    var content = ClipboardContent(plain_text = target, uri_list = target + "\r\n")
    content.formats.plain_text = true
    content.formats.uri_list = true
    let status = clipboard_set_content(content)
    clipboard_dispose_content(content)
    return status
}

def private fragment_min(origin : float2; fragment : TextFlowFragment) : float2 {
    return origin + float2(fragment.rect.x, fragment.rect.y)
}

def private fragment_max(origin : float2; fragment : TextFlowFragment) : float2 {
    return origin + float2(fragment.rect.z, fragment.rect.w)
}

def public markdown_uri_paragraph(id : string; prefix, display_text, suffix, target : string;
                                  max_width : float = 0.0f;
                                  style : MarkdownUriStyle = MarkdownUriStyle()) : MarkdownUriResult {
    //! Draw one source-mapped inline paragraph. The copy button is only visible
    //! while the URI/control is hovered, but its inline slot is always reserved
    //! by ``layout_text_flow`` so hover never changes wrapping.
    let source = prefix + display_text + suffix
    let uri_start = length(prefix)
    let uri_end = uri_start + length(display_text)
    let available = max_width > 0.0f ? max_width : GetContentRegionAvail().x
    let icon_full = float2(
        style.copy_icon_size + GetStyle().FramePadding.x * 2.0f,
        style.copy_icon_size + GetStyle().FramePadding.y * 2.0f)

    var spans = [
        TextFlowSpan(
            source = TextSourceRange(start_byte = 0, end_byte = uri_start),
            style_id = 0),
        TextFlowSpan(
            source = TextSourceRange(start_byte = uri_start, end_byte = uri_end),
            style_id = 1, semantic_id = 1, uri_wrap = true,
            trailing_control_id = 1,
            trailing_control_size = icon_full),
        TextFlowSpan(
            source = TextSourceRange(start_byte = uri_end, end_byte = length(source)),
            style_id = 0)
    ]
    let layout <- layout_text_flow(source, spans, available, GetTextLineHeight(),
        $(s : string) : float {
            return CalcTextSize(s).x
        })

    let origin = GetCursorScreenPos()
    Dummy(float2(available, max(layout.size.y, GetTextLineHeight())))

    var result = MarkdownUriResult(line_count = length(layout.lines))
    var uri_text_hovered = false
    var control_hovered = false
    var control_fragment = TextFlowFragment()
    var has_control = false
    for (fragment in layout.fragments) {
        if (fragment.semantic_id != 1) continue
        let hot = IsMouseHoveringRect(fragment_min(origin, fragment),
                                     fragment_max(origin, fragment), true)
        if (fragment.is_control) {
            control_hovered = control_hovered || hot
            control_fragment = fragment
            has_control = true
        } else {
            uri_text_hovered = uri_text_hovered || hot
        }
    }
    result.hovered = uri_text_hovered || control_hovered

    var dl = GetWindowDrawList()
    let body_col = GetColorU32(style.body_color)
    let link_col = GetColorU32(result.hovered ? style.link_hover_color : style.link_color)
    for (fragment in layout.fragments) {
        if (fragment.is_control) continue
        let fragment_text = slice(source, fragment.source.start_byte,
                                  fragment.source.end_byte)
        let p0 = fragment_min(origin, fragment)
        *dl |> AddText(p0, fragment.semantic_id == 1 ? link_col : body_col, fragment_text)
        if (result.hovered && fragment.semantic_id == 1) {
            let underline_y = p0.y + GetTextLineHeight() - 1.0f
            *dl |> AddLine(float2(p0.x, underline_y),
                           float2(fragment_max(origin, fragment).x, underline_y),
                           link_col, style.underline_thickness)
        }
    }

    if (uri_text_hovered) {
        SetMouseCursor(ImGuiMouseCursor.Hand)
        result.activated = IsMouseClicked(ImGuiMouseButton.Left, false)
    }

    if (has_control) {
        if (icon_button_draw_at("##markdown_uri_copy_{id}", "duplicate",
                                fragment_min(origin, control_fragment),
                                style.copy_icon_size, 1.0f, result.hovered)) {
            let copy_status = markdown_uri_copy(empty(target) ? display_text : target)
            result.copied = copy_status == ClipboardStatus.ok
        }
        if (result.hovered && IsItemHovered()) {
            SetTooltip("Copy URI")
        }
    }
    return result
}
