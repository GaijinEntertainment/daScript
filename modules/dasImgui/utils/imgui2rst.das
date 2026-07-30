options gen2
options _comment_hygiene = true
options rtti = true

require daslib/fio
require daslib/rtti
require daslib/ast
require daslib/rst_comment
require daslib/rst
require daslib/regex
require daslib/regex_boost
require daslib/profiler_boost

// pull in every v2 module so RTTI can see them. (v1 daslib/imgui_boost is excluded.)
require imgui/imgui_boost_v2
require imgui/imgui_boost_runtime
require imgui/imgui_containers_builtin
require imgui/imgui_id_builtin
require imgui/imgui_layout_builtin
require imgui/imgui_live
require imgui/imgui_live_core
require imgui/imgui_playwright
require imgui/imgui_style_builtin
require imgui/imgui_transport
require imgui/imgui_visual_aids
require imgui/imgui_widgets_builtin
require imgui/imgui_icons


def document_module_imgui_boost_v2() {
    var mod = find_module("imgui_boost_v2")
    var groups <- array<DocGroup>(
        group_by_regex("Container macros",       mod, %regex~^(window|child|group|tab_bar|tab_item|wrap_tab_bar|wrap_tab_item|menu_bar|menu|popup|modal|tree_node|collapsing_header)$%%),
        group_by_regex("Widget annotation macros", mod, %regex~^(define_widget|widget|container)$%%),
        group_by_regex("ID scope macros",        mod, %regex~^(with_id|with_path)$%%),
        group_by_regex("Style scope macros",     mod, %regex~^(with_style|with_color|with_var)$%%),
        group_by_regex("Layout macros",          mod, %regex~^(split_h|split_v|columns|dock_left|dock_right|dock_top|dock_bottom)$%%),
        hide_group(group_by_regex("Internal",    mod, %regex~^(_|priv_).*%%))
    )
    document("Boost macro layer — DSL for declarative widget composition", mod, "imgui_boost_v2.rst", groups)
}

def document_module_imgui_boost_runtime() {
    var mod = find_module("imgui_boost_runtime")
    var groups <- array<DocGroup>(
        group_by_regex("Widget state types",      mod, %regex~.*State(Float|Int|Float2|Float3|Float4|Int2|Int3|Int4)?$%%),
        group_by_regex("Per-frame registry",      mod, %regex~^(WidgetEntry|WidgetMeta|begin_frame|end_frame|end_of_frame|register_widget|register_focusable|register_radio_site_alias|register_handle_bbox|register_clickable_bbox|register_clickable_state_bbox|register_end_of_frame_hook|register_post_command_hook|lookup_widget|lookup_state_addr|clear_module_widgets|widget_registered|widget_prelude|widget_rect|with_state|id_to_path|id_to_bbox).*%%),
        group_by_regex("Path machinery",          mod, %regex~^(container_path_push|container_path_pop|widget_path_key|container_path_key|active_widget_path)$%%),
        group_by_regex("Coroutines",              mod, %regex~^(advance_coroutines|spawn_coroutine)$%%),
        group_by_regex("Window control",          mod, %regex~^imgui_(dock|undock|dock_reset|raise|set_window_pos|set_window_size)$%%),
        group_by_regex("Live commands",           mod, %regex~^imgui_(snapshot|click|force_set|open|close|select|focus|await|mouse_pos|mouse_click|mouse_scroll|key_press|key_release|key_char)$%%),
        group_by_regex("Synthetic input",         mod, %regex~^(synth_mouse_move|synth_mouse_button|synth_mouse_wheel|synth_key_press|synth_key_release|synth_char|release_held_buttons|release_held_keys|get_synth_cursor|get_synth_keys|imgui_synth_tick|user_control_enabled|set_user_control_enabled|set_user_control|register_real_input_toggle|register_synth_frame_hook|register_synth_stop_hook)$%%),
        group_by_regex("Debug logging",           mod, %regex~^(log_section|log_section_on|imgui_log_section|imgui_log_drain)$%%),
        group_by_regex("Memory debugging",         mod, %regex~^(imgui_debug_memory|imgui_reset_memory_baseline|imgui_memory_report)$%%),
        group_by_regex("Snapshot transform",      mod, %regex~^(push_snapshot_item_transform|pop_snapshot_item_transform|capture_item_bbox|capture_current_item_rect)$%%),
        group_by_regex("Await modifiers",         mod, %regex~^(AwaitModifiers|AwaitArgs|AwaitResult|with_await)$%%),
        group_by_regex("JSON serialization",      mod, %regex~^(widget_entry_jv|state_jv|to_JV|from_JV|JV|serialize).*%%),
        hide_group(group_by_regex("Finalizers",   mod, %regex~^.*_finalize(_vec|_subs)?$%%)),
        hide_group(group_by_regex("Internal",     mod, %regex~^(_|priv_).*%%))
    )
    document("Boost runtime — widget state structs, registry, and dispatcher", mod, "imgui_boost_runtime.rst", groups)
}

def document_module_imgui_widgets_builtin() {
    var mod = find_module("imgui_widgets_builtin")
    var groups <- array<DocGroup>(
        group_by_regex("Buttons",         mod, %regex~^(button|small_button|arrow_button|image_button|invisible_button|checkbox|radio_button)$%%),
        group_by_regex("Images",          mod, %regex~^image_texture_id$%%),
        group_by_regex("Sliders",         mod, %regex~^(slider|slider_int|slider_float|slider_int2|slider_int3|slider_int4|slider_float2|slider_float3|slider_float4|slider_angle)$%%),
        group_by_regex("Drag inputs",     mod, %regex~^drag.*%%),
        group_by_regex("Text input",      mod, %regex~^(input_text|input_text_multiline|input_int|input_float|input_int2|input_int3|input_int4|input_float2|input_float3|input_float4)$%%),
        group_by_regex("Color pickers",   mod, %regex~^(color_edit|color_picker|color_button)$%%),
        group_by_regex("Selection",       mod, %regex~^(combo|list_box|selectable|passes_filter)$%%),
        group_by_regex("Plot helpers",    mod, %regex~^(plot_lines|plot_histogram|progress_bar)$%%),
        group_by_regex("Output",          mod, %regex~^(text_show|text_input|label_text|bullet_text|help_marker)$%%),
        group_by_regex("Focus & scrolling", mod, %regex~^(set_scroll_here_y|set_keyboard_focus_here|set_item_default_focus)$%%),
        group_by_regex("Logging",         mod, %regex~^(log_to_clipboard|log_finish|log_text)$%%),
        group_by_regex("Status queries",  mod, %regex~^is_active$%%),
        hide_group(group_by_regex("Finalizers", mod, %regex~^.*_finalize(_vec|_subs)?$%%)),
        hide_group(group_by_regex("Internal", mod, %regex~^(_|priv_).*%%))
    )
    document("Built-in widgets — inputs, selection, color, plots, output", mod, "imgui_widgets_builtin.rst", groups)
}

def document_module_imgui_containers_builtin() {
    var mod = find_module("imgui_containers_builtin")
    var groups <- array<DocGroup>(
        group_by_regex("Windows",       mod, %regex~^(window|child|group|child_scroll_reenter|set_window_size|viewport_center|is_mouse_pos_valid)$%%),
        group_by_regex("Tabs",          mod, %regex~^(tab_bar|tab_item|wrap_tab_bar|wrap_tab_item)$%%),
        group_by_regex("Trees",         mod, %regex~^(tree_node|collapsing_header)$%%),
        group_by_regex("Menus",         mod, %regex~^(menu_bar|menu|menu_item)$%%),
        group_by_regex("Popups",        mod, %regex~^(popup|modal|tooltip|open_popup|open_popup_on_item_click|close_current_popup|is_popup_open)$%%),
        hide_group(group_by_regex("Finalizers", mod, %regex~^.*_finalize(_vec|_subs)?$%%)),
        hide_group(group_by_regex("Internal", mod, %regex~^(_|priv_).*%%))
    )
    document("Built-in containers — windows, child regions, tabs, menus, popups", mod, "imgui_containers_builtin.rst", groups)
}

def document_module_imgui_id_builtin() {
    var mod = find_module("imgui_id_builtin")
    var groups <- array<DocGroup>(
        group_by_regex("ID stack",      mod, %regex~^(push_id|pop_id|with_id|with_override_id|with_path)$%%),
        group_by_regex("Hashing",       mod, %regex~^(hash_id|get_id).*%%),
        hide_group(group_by_regex("Internal", mod, %regex~^(_|priv_).*%%))
    )
    document("ID stack — push/pop, scoped, hashing", mod, "imgui_id_builtin.rst", groups)
}

def document_module_imgui_layout_builtin() {
    var mod = find_module("imgui_layout_builtin")
    var groups <- array<DocGroup>(
        group_by_regex("Splits",        mod, %regex~^split_.*%%),
        group_by_regex("Columns",       mod, %regex~^(columns|columns_open|columns_close|next_col)$%%),
        group_by_regex("Dock helpers",  mod, %regex~^dock_.*%%),
        group_by_regex("Spacing",       mod, %regex~^(same_line|spacing|new_line|separator|indent|unindent)$%%),
        group_by_regex("Trees",         mod, %regex~^(tree_pop|tree_node_open)$%%),
        group_by_regex("List clipping", mod, %regex~^list_clipper$%%),
        hide_group(group_by_regex("Internal", mod, %regex~^(_|priv_).*%%))
    )
    document("Layout helpers — splits, columns, docking, spacing", mod, "imgui_layout_builtin.rst", groups)
}

def document_module_imgui_style_builtin() {
    var mod = find_module("imgui_style_builtin")
    var groups <- array<DocGroup>(
        group_by_regex("Style scope",   mod, %regex~^(with_style|with_color|with_var)$%%),
        group_by_regex("Push/pop",      mod, %regex~^(push_style|pop_style).*%%),
        group_by_regex("Themes",        mod, %regex~^(style_colors_dark|style_colors_light|style_colors_classic)$%%),
        hide_group(group_by_regex("Internal", mod, %regex~^(_|priv_).*%%))
    )
    document("Style helpers — colors, vars, push/pop, themes", mod, "imgui_style_builtin.rst", groups)
}

def document_module_imgui_visual_aids() {
    var mod = find_module("imgui_visual_aids")
    var groups <- array<DocGroup>(
        group_by_regex("Highlight overlay", mod, %regex~^(imgui_highlight|imgui_auto_highlight|highlight|paint_highlight).*%%),
        group_by_regex("Mouse trail",       mod, %regex~^(imgui_mouse_trail|mouse_trail|paint_mouse_trail).*%%),
        group_by_regex("Narration",         mod, %regex~^(narrate|imgui_narrate|paint_narrate|paint_callout|compute_narrate_anchor|compute_connector_).*%%),
        group_by_regex("HUD",               mod, %regex~^(paint_key_hud|glyph_for_imgui_key|paint_hud|imgui_key_hud|imgui_focus_rect).*%%),
        group_by_regex("Cursor sprite",     mod, %regex~^(cursor_sprite|imgui_cursor_sprite)$%%),
        group_by_regex("Override hooks",    mod, %regex~^apply_synth_io_override$%%),
        group_by_regex("Memory debugging",  mod, %regex~^imgui_memory_debug$%%),
        group_by_regex("Color utilities",   mod, %regex~^rgba$%%),
        group_by_regex("Serve / lifecycle", mod, %regex~^(imgui_visual_aids_serve|visual_aids_init|render_visual_aids|effective_).*%%),
        hide_group(group_by_regex("Internal", mod, %regex~^(_|priv_).*%%))
    )
    document("Visual aids — tutorial-mode overlays, mouse trail, narration, key HUD", mod, "imgui_visual_aids.rst", groups)
}

def document_module_imgui_playwright() {
    var mod = find_module("imgui_playwright")
    var groups <- array<DocGroup>(
        group_by_regex("App lifecycle",     mod, %regex~^(with_imgui_app|launch_imgui_app|shutdown|with_recording_app).*%%),
        group_by_regex("Locators / queries", mod, %regex~^(find_widget|widget_exists|widget_payload_field|widget_rendered|widget_click_point|widget_component_point|get_text)$%%),
        group_by_regex("Actions",           mod, %regex~^(click|double_click|click_at|right_click|type_text|key_tap|force_set_value|force_set_verified|drag|drag_along|drag_to|focus|open_widget|close_widget|select_widget|reload|reset|move_to|reset_cursor_pos|resize_window)$%%),
        group_by_regex("Recording / narration", mod, %regex~^(say|say_begin|say_hash|hold_through_voice|drag_through_voice|type_into_voice|combo_pick_voice|toggle_tree_voice|close_button_voice|click_render_voice|menu_pick_voice|record_check_value|record_check_changed|record_check_unchanged|record_check_rendered|record_check_kind_count|record_check)$%%),
        group_by_regex("Content-time gating", mod, %regex~^(wait_content_frames|hold_content|record_frame_count|hold_remainder_content)$%%),
        group_by_regex("Snapshots",         mod, %regex~^(snapshot|log_snapshot|expect_value|expect_render).*%%),
        group_by_regex("Polling / await",   mod, %regex~^(wait_until.*|wait_for_.*|wait_cond|await_quiescent|await_probe|set_auto_dump_on_timeout|auto_dump_on_timeout_enabled)$%%),
        group_by_regex("Pause control",     mod, %regex~^(pause|unpause|with_paused)$%%),
        group_by_regex("Transport plumbing", mod, %regex~^(post_command|send_imgui_command|receive_imgui_command).*%%),
        hide_group(group_by_regex("Internal", mod, %regex~^(_|priv_|dasimgui_module_root).*%%))
    )
    document("Playwright — block-form test harness for daslang-live + dasImgui apps", mod, "imgui_playwright.rst", groups)
}

def document_module_imgui_live() {
    var mod = [find_module("imgui_live"), find_module("imgui_live_core")]
    var groups <- array<DocGroup>(
        group_by_regex("Lifecycle",           mod, %regex~^(live_imgui_init|live_imgui_shutdown|live_imgui_.*)$%%),
        group_by_regex("Synth IO drivers",    mod, %regex~^(synth_|imgui_mouse_|imgui_key_|imgui_synth_|get_synth_|warn_if_synth_|reset_synth_).*%%),
        group_by_regex("User input control",  mod, %regex~^(set_user_control.*|user_control_enabled)$%%),
        group_by_regex("Event timelines",     mod, %regex~^(MouseEventWire|KeyEventWire|sort_mouse_play_events|sort_key_play_events|.*_play)$%%),
        group_by_regex("Override hooks",      mod, %regex~^(apply_synth_io_override|effective_cursor_pos|effective_synth_click)$%%),
        group_by_regex("Status / introspection", mod, %regex~^(.*_status|.*Status).*%%),
        hide_group(group_by_regex("Internal", mod, %regex~^(_|priv_).*%%))
    )
    documents("Live-reload lifecycle and synthetic-IO bypass driver",
              mod, "imgui_live.rst", groups)
}

def document_module_imgui_transport() {
    var mod = find_module("imgui_transport")
    var groups <- array<DocGroup>(
        group_by_regex("Transport construction", mod, %regex~^(live_api_transport|make_transport|with_transport)$%%),
        group_by_regex("Send / receive",         mod, %regex~^(send_imgui_command|receive_imgui_command|receive_imgui_snapshot|post_command)$%%),
        group_by_regex("Awaiting",               mod, %regex~^(await_imgui|await_imgui_frame|send_with_await)$%%),
        group_by_regex("Framing / encoding",     mod, %regex~^(encode_|decode_|frame_).*%%),
        group_by_regex("Raw HTTP",               mod, %regex~^(http_get|http_post|HttpReply)$%%),
        hide_group(group_by_regex("Internal",    mod, %regex~^(_|priv_).*%%))
    )
    document("Transport — pluggable lambda-based IPC for imgui commands", mod, "imgui_transport.rst", groups)
}


// dasImgui icon catalog — a hand-built visual reference page. imgui_icons draws runtime vector
// geometry (no asset files), so catalog images are rendered offline by utils/make_icon_doc.das into
// _static/icons/ (committed); enumeration uses the module's catalog accessors so the page can't drift.
let RULE = "----------------------------------------"

def generate_icon_catalog() {
    let path = "{topic_root}/imgui_icons.rst"
    mkdir_rec(dir_name(path))
    var f = fopen(path, "wb")
    if (f == null) {
        panic("can't open {path}")
    }
    fwrite(f, "\n")
    fwrite(f, "Icon set\n========\n\n")
    fwrite(f, "The dasImgui icon set is a growing collection of asset-free vector glyphs ")
    fwrite(f, "drawn directly onto the ImGui draw list -- no SVG, no texture atlas. Every ")
    fwrite(f, "glyph is geometry on a logical 24x24 grid, tinted from the daslang amber accent.\n\n")
    fwrite(f, "Draw a glyph at the cursor with ``icon(name, size)``; for a clickable, ")
    fwrite(f, "snapshot-visible button use the ``icon_button`` widget (see ")
    fwrite(f, ":doc:`imgui_widgets_builtin`). Enumerate the set programmatically with ")
    fwrite(f, "``icon_count`` / ``icon_name`` / ``icon_category`` -- e.g. to build a picker. ")
    fwrite(f, "Reference a glyph inline in prose with the ``:icon:`name``` role, e.g. the ")
    fwrite(f, ":icon:`play` button.\n\n")

    var cur = ""
    var grid_open = false
    for (i in range(icon_count())) {
        let nm = icon_name(i)
        let cat = icon_category(i)
        if (cat != cur) {
            if (grid_open) {
                fwrite(f, "   </div>\n\n")
            }
            cur = cat
            fwrite(f, "{cat}\n{RULE}\n\n")
            fwrite(f, ".. raw:: html\n\n")
            fwrite(f, "   <div class=\"icon-catalog\">\n")
            grid_open = true
        }
        fwrite(f, "   <figure class=\"icon-card\"><img src=\"../../_static/icons/{nm}.png\" alt=\"{nm}\"><figcaption>{nm}</figcaption></figure>\n")
    }
    if (grid_open) {
        fwrite(f, "   </div>\n\n")
    }

    fwrite(f, "API\n{RULE}\n\n")
    fwrite(f, "- ``icon(name : string; size : float = 20.0; weight : float = 1.0)`` -- draw a glyph at the cursor (no interaction); advances the cursor.\n")
    fwrite(f, "- ``icon_button_draw(id, glyph : string; size, weight : float)`` -- low-level clickable icon button; returns true on click. Prefer the ``icon_button`` widget for snapshot / playwright support.\n")
    fwrite(f, "- ``icon_count() : int`` / ``icon_name(i) : string`` / ``icon_category(i) : string`` -- enumerate the catalog (the source of this page).\n\n")
    fclose(f)
}

[export]
def main {
    disable_profiler(this_context())
    disable_profiler_log(this_context())
    ast_gc_guard() {
        // dasImgui docs live in the main doc tree; generated pages join
        // das2rst's output under doc/source/stdlib/generated/.
        let doc_stdlib = get_das_root() + "/doc/source/stdlib"
        topic_root = "{doc_stdlib}/generated"
        handmade_root = doc_stdlib

        document_module_imgui_boost_v2()
        document_module_imgui_boost_runtime()
        document_module_imgui_widgets_builtin()
        document_module_imgui_containers_builtin()
        document_module_imgui_id_builtin()
        document_module_imgui_layout_builtin()
        document_module_imgui_style_builtin()
        document_module_imgui_visual_aids()
        document_module_imgui_playwright()
        document_module_imgui_live()
        document_module_imgui_transport()
        generate_icon_catalog()
    }
}
