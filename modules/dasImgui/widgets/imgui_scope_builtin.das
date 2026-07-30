options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false

module imgui_scope_builtin shared

require imgui public
require imgui/imgui_boost_runtime public
require imgui/imgui_enums public

def public with_indent(amount : float; blk : block) {
    //! Run ``blk`` with ``Indent(amount)`` applied; matching ``Unindent(amount)`` on return.
    //! Pass ``0.0f`` to defer to ImGui's ``IndentSpacing`` style default.
    Indent(amount)
    invoke(blk)
    Unindent(amount)
}

def public with_item_width(item_width : float; blk : block) {
    //! Run ``blk`` with ``PushItemWidth(item_width)`` applied; matching ``PopItemWidth()`` on return.
    //! Negative values are "right edge minus N pixels"; positive are absolute widget width.
    PushItemWidth(item_width)
    invoke(blk)
    PopItemWidth()
}

def public with_text_wrap_pos(wrap_local_pos_x : float; blk : block) {
    //! Run ``blk`` with ``PushTextWrapPos(wrap_local_pos_x)`` applied; matching ``PopTextWrapPos()`` on return.
    //! Pass ``0.0f`` to wrap at the window's right edge; positive is an x-coord in window-local space.
    PushTextWrapPos(wrap_local_pos_x)
    invoke(blk)
    PopTextWrapPos()
}

def public with_disabled(disabled : bool; blk : block) {
    //! Run ``blk`` with ``BeginDisabled(disabled)`` applied; matching ``EndDisabled()`` on return.
    //! While ``disabled=true`` every widget inside ``blk`` is rendered greyed-out and ignores input.
    BeginDisabled(disabled)
    invoke(blk)
    EndDisabled()
}

def public with_font(font : ImFont?; size : float = 0.0f; blk : block) {
    //! Run ``blk`` with ``PushFont(font, size)`` applied; matching ``PopFont()`` on return.
    //! ``font=null`` keeps the current font (re-size only); ``size`` is the rasterization pixel height,
    //! ``0.0f`` (default) keeps the current size — so ``with_font(font) { ... }`` is a pure font swap.
    PushFont(font, size)
    invoke(blk)
    PopFont()
}

def public with_button_repeat(repeat : bool; blk : block) {
    //! Run ``blk`` with the button-repeat item flag applied; cleared on return.
    //! When ``repeat=true``, held-down buttons fire repeated click events at the io.KeyRepeatRate cadence.
    PushItemFlag(int(ImGuiItemFlags.ButtonRepeat), repeat)
    invoke(blk)
    PopItemFlag()
}

def public with_clip_rect(clip_rect_min : float2; clip_rect_max : float2;
                          intersect_with_current : bool; blk : block) {
    //! Run ``blk`` with ``PushClipRect(min, max, intersect)`` applied; matching ``PopClipRect()`` on return.
    //! When ``intersect_with_current=true`` the new rect is intersected with the existing clip rect;
    //! otherwise it replaces it. Coordinates are in screen space.
    PushClipRect(clip_rect_min, clip_rect_max, intersect_with_current)
    invoke(blk)
    PopClipRect()
}

def public with_tab_stop(tab_stop : bool; blk : block) {
    //! Run ``blk`` with the tab-stop item flag applied; cleared on return.
    //! When ``tab_stop=false`` every widget inside ``blk`` is skipped by TAB / Shift+Tab focus cycling.
    PushItemFlag(int(ImGuiItemFlags.NoTabStop), !tab_stop)
    invoke(blk)
    PopItemFlag()
}

def public with_item_flag(flag : ImGuiItemFlags; enabled : bool; blk : block) {
    //! Run ``blk`` with ``PushItemFlag(flag, enabled)`` applied; matching ``PopItemFlag()`` on return.
    //! For internal item flags lacking a dedicated guard (``AllowOverlap``, ``NoNav``, ``MixedValue``, ``ReadOnly``).
    PushItemFlag(int(flag), enabled)
    invoke(blk)
    PopItemFlag()
}

def public with_focus_scope(id : uint; blk : block) {
    //! Run ``blk`` inside focus scope ``id`` (``PushFocusScope``); matching ``PopFocusScope()`` on return.
    //! Focus scopes group items for keyboard/gamepad nav and multi-select; ``id`` is an ImGuiID (uint), e.g. ``GetID("list")``.
    PushFocusScope(id)
    invoke(blk)
    PopFocusScope()
}

def public with_popup_ex(id : uint; window_flags : ImGuiWindowFlags; blk : block) : bool {
    //! `BeginPopupEx(id, window_flags)` / `EndPopup()` — only-on-true. Lower-level
    //! `popup` for a precomputed ImGuiID (e.g. from ``with_override_id``/``GetID``);
    //! the popup must already be open (``OpenPopupEx(id)``). Returns whether it is open.
    let opened = BeginPopupEx(id, window_flags)
    if (opened) {
        invoke(blk)
        EndPopup()
    }
    return opened
}

def public with_tooltip_ex(tooltip_flags : ImGuiTooltipFlags; window_flags : ImGuiWindowFlags; blk : block) : bool {
    //! `BeginTooltipEx(tooltip_flags, window_flags)` / `EndTooltip()` — only-on-true. ``ImGuiTooltipFlags.OverridePrevious``
    //! replaces an already-submitted tooltip instead of appending. Returns whether it opened.
    // ImGuiTooltipFlags is an internal enum (no arg-remap) so it binds as int, hence the ``int()`` cast.
    let opened = BeginTooltipEx(int(tooltip_flags), window_flags)
    if (opened) {
        invoke(blk)
        EndTooltip()
    }
    return opened
}

def public with_tooltip_hidden(blk : block) : bool {
    //! `BeginTooltipHidden()` / `EndTooltip()` — only-on-true. Begins an off-screen
    //! tooltip used to MEASURE content (its rect) without showing it. Returns whether it opened.
    let opened = BeginTooltipHidden()
    if (opened) {
        invoke(blk)
        EndTooltip()
    }
    return opened
}

def public with_menu_ex(lbl : string; icon : string; enabled : bool; blk : block) : bool {
    //! `BeginMenuEx(lbl, icon, enabled)` / `EndMenu()` — only-on-true. Like `menu`
    //! but with an ``icon`` column (pass ``""`` for none); ``blk`` is the dropdown body.
    //! Returns whether the dropdown is open.
    let opened = BeginMenuEx(lbl, icon, enabled)
    if (opened) {
        invoke(blk)
        EndMenu()
    }
    return opened
}

def public with_combo_popup(popup_id : uint; bb : float4; flags : ImGuiComboFlags; blk : block) : bool {
    //! `BeginComboPopup(popup_id, bb, flags)` / `EndCombo()` — only-on-true. Low-level combo dropdown the
    //! public `combo` is built on: open the popup first (``OpenPopupEx(popup_id)``), pass the combo box's
    //! rect as ``bb``, render selectables in ``blk``. Returns whether the dropdown is open.

    // Close with EndCombo, NOT EndPopup: BeginComboPopup increments g.BeginComboDepth and only EndCombo
    // decrements it (EndPopup would leak the depth, corrupting the recycled ##Combo_NN popup on next open).
    let opened = BeginComboPopup(popup_id, bb, flags)
    if (opened) {
        invoke(blk)
        EndCombo()
    }
    return opened
}

def public with_custom_preview_combo(str_id : string; flags : ImGuiComboFlags; blk : block) : bool {
    //! `BeginCombo(str_id, "", flags + CustomPreview)` / `EndCombo()` — only-on-true. Opens a combo whose
    //! closed-state preview is custom-drawn: pair it with a ``with_combo_preview`` block immediately after.
    //! ``blk`` renders the dropdown selectables. Returns whether the dropdown is open.

    // CustomPreview is the lone internal ImGuiComboFlagsPrivate flag; fold it into the public flags by
    // value (the two are distinct das enums over one int flag set).
    let cflags = unsafe(reinterpret<ImGuiComboFlags>(int(flags) | int(ImGuiComboFlagsPrivate.CustomPreview)))
    let opened = BeginCombo(str_id, "", cflags)
    if (opened) {
        invoke(blk)
        EndCombo()
    }
    return opened
}

def public with_combo_preview(blk : block) : bool {
    //! `BeginComboPreview()` / `EndComboPreview()` — only-on-true. Renders the custom preview content
    //! (``blk``) into the preview box of a ``with_custom_preview_combo`` combo — call it immediately after
    //! that combo's block. Returns whether the preview began.
    let opened = BeginComboPreview()
    if (opened) {
        invoke(blk)
        EndComboPreview()
    }
    return opened
}

def public with_log(log_type : ImGuiLogFlags; auto_open_depth : int; blk : block) {
    //! `LogBegin(log_type, auto_open_depth)` / `LogFinish()` — captures rendered text of every widget in ``blk``
    //! to the target flag (``OutputTTY`` / ``OutputFile`` / ``OutputClipboard`` / ``OutputBuffer``); ``auto_open_depth``
    //! is the tree depth to auto-expand (-1 = default). Do NOT nest: LogBegin asserts logging is not already active.
    LogBegin(int(log_type), auto_open_depth)
    invoke(blk)
    LogFinish()
}
