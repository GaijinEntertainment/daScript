options gen2
options _comment_hygiene = true
options indenting = 4

module app_documents

require imgui public
require imgui/imgui_widgets_builtin public
require imgui/imgui_containers_builtin public
require imgui/imgui_scope_builtin public
require imgui/imgui_style_builtin public
require imgui/imgui_colors public
require daslib/safe_addr
require math

// Mirrors imgui_demo.cpp:8552-8900 — static void ShowExampleAppDocuments(bool* p_open).
//
// Scope: ports the Target_Tab mode (the C++ default — local tab bar with
// per-document tabs + per-tab context menu + close-confirmation popup_modal
// queue for dirty docs). Skipped: Target_None (no-op) and
// Target_DockSpaceAndWindow (would require a second flag-toggle code path
// + the dockspace-inside-window pattern + extra docking dance not worth
// the demo noise here).

// ===== MyDocument =====

struct MyDocument {
    name : string
    open : bool                 //! visible in the tab bar
    dirty : bool                //! modified flag (drives UnsavedDocument)
    want_close : bool           //! enqueued for close confirmation
    color : float4              //! per-doc text color
}

def new_doc(name : string;
            open : bool = true;
            color : float4 = float4(1.0f, 1.0f, 1.0f, 1.0f)) : MyDocument {
    return MyDocument(name = name, open = open,
                      dirty = false, want_close = false, color = color)
}

// ===== Module-scope state =====

var private DOCS : array<MyDocument>
var private DOCS_SEEDED = false
var private CLOSE_QUEUE : array<int>     //! indices into DOCS pending confirmation
var private POPUP_OPEN_LATCH = false      //! once-per-queue OpenPopup gate

// Per-doc dynamic states. The [container]/[widget] macros auto-emit a SINGLE
// global per ident; for per-row loops we explicitly declare an indexed table.
var private DOCS_TAB : table<int; TabItemState>
var private DOCS_CTX : table<int; PopupContextItemState>
var private DOCS_MODIFY_BTN : table<int; ClickState>
var private DOCS_SAVE_BTN : table<int; ClickState>
var private DOCS_CTX_SAVE_ITEM : table<int; ToggleState>
var private DOCS_CTX_CLOSE_ITEM : table<int; ToggleState>
var private DOCS_OPEN_SUBMENU_ITEM : table<int; ToggleState>
var private DOCS_OPEN_CHECKBOX : table<int; ToggleState>
var private DOCS_CLOSE_NAME : table<int; NarrativeState>
var private DOCS_DEBUG_SL : table<int; EmptyMarkerState>

// "Exit" menu sets this; ShowExampleAppDocuments returns false when set OR
// when the X-button closes the window.
var private DOCS_REQUEST_CLOSE = false

def private seed_docs() {
    DOCS |> emplace(new_doc("Lettuce", true, float4(0.4f, 0.8f, 0.4f, 1.0f)))
    DOCS |> emplace(new_doc("Eggplant", true, float4(0.8f, 0.5f, 1.0f, 1.0f)))
    DOCS |> emplace(new_doc("Carrot", true, float4(1.0f, 0.8f, 0.5f, 1.0f)))
    DOCS |> emplace(new_doc("Tomato", false, float4(1.0f, 0.3f, 0.4f, 1.0f)))
    DOCS |> emplace(new_doc("A Rather Long Title", false))
    DOCS |> emplace(new_doc("Some Document", false))
}

def private pack_color(c : float4) : uint {
    let r = uint(clamp(c.x * 255.0f, 0.0f, 255.0f))
    let g = uint(clamp(c.y * 255.0f, 0.0f, 255.0f))
    let b = uint(clamp(c.z * 255.0f, 0.0f, 255.0f))
    let a = uint(clamp(c.w * 255.0f, 0.0f, 255.0f))
    return rgba(r, g, b, a)
}

// ===== Per-doc UI helpers =====

def private display_contents(idx : int) {
    var doc & = unsafe(DOCS[idx])
    text(DOCS_TITLE, (text = "Document \"{doc.name}\""))
    with_style((ImGuiCol.Text, pack_color(doc.color))) {
        text_wrapped(DOCS_LOREM,
                     (text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua."))
    }
    if (button(DOCS_MODIFY_BTN[idx], (text = "Modify", size = float2(100.0f, 0.0f)))) {
        doc.dirty = true
    }
    same_line()
    if (button(DOCS_SAVE_BTN[idx], (text = "Save", size = float2(100.0f, 0.0f)))) {
        doc.dirty = false
    }
    edit_color_edit4(safe_addr(doc.color), (id = "doc_color", text = "color",
                                            flags = ImGuiColorEditFlags.None))
}

def private display_context_menu(idx : int) {
    var doc & = unsafe(DOCS[idx])
    popup_context_item(DOCS_CTX[idx], (str_id = "##doc_ctx_{idx}",
                                       flags = ImGuiPopupFlags.MouseButtonRight)) {
        if (menu_item(DOCS_CTX_SAVE_ITEM[idx],
                      (text = "Save {doc.name}", shortcut = "CTRL+S",
                       enabled = doc.open))) {
            doc.dirty = false
        }
        if (menu_item(DOCS_CTX_CLOSE_ITEM[idx],
                      (text = "Close", shortcut = "CTRL+W",
                       enabled = doc.open))) {
            doc.want_close = true
        }
    }
}

// ===== File menu =====

def private file_menu() {
    menu(DOCS_FILE_MENU, (text = "File", enabled = true)) {
        var open_count = 0
        for (d in DOCS) {
            if (d.open) {
                open_count++
            }
        }
        menu(DOCS_OPEN_SUBMENU,
             (text = "Open", enabled = open_count < length(DOCS))) {
            for (i in range(length(DOCS))) {
                var doc & = unsafe(DOCS[i])
                if (!doc.open) {
                    if (menu_item(DOCS_OPEN_SUBMENU_ITEM[i],
                                  (text = doc.name, shortcut = "", enabled = true))) {
                        doc.open = true
                    }
                }
            }
        }
        if (menu_item(DOCS_CLOSE_ALL,
                      (text = "Close All Documents", shortcut = "",
                       enabled = open_count > 0))) {
            for (i in range(length(DOCS))) {
                DOCS[i].want_close = true
            }
        }
        if (menu_item(DOCS_EXIT_ITEM,
                      (text = "Exit", shortcut = "Ctrl+F4", enabled = true))) {
            DOCS_REQUEST_CLOSE = true
        }
    }
}

// ===== Close-confirmation popup_modal =====

def private rebuild_close_queue() {
    if (!empty(CLOSE_QUEUE)) return
    for (i in range(length(DOCS))) {
        if (DOCS[i].want_close) {
            DOCS[i].want_close = false
            CLOSE_QUEUE |> push(i)
        }
    }
}

def private drain_close_queue_clean() {
    // No dirty docs: just close them all.
    for (i in CLOSE_QUEUE) {
        DOCS[i].open = false
        DOCS[i].dirty = false
    }
    CLOSE_QUEUE |> clear()
    POPUP_OPEN_LATCH = false
}

def private show_close_confirm() {
    if (empty(CLOSE_QUEUE)) return
    var dirty_count = 0
    for (i in CLOSE_QUEUE) {
        if (DOCS[i].dirty) {
            dirty_count++
        }
    }
    if (dirty_count == 0) {
        drain_close_queue_clean()
        return
    }
    if (!POPUP_OPEN_LATCH) {
        // Use the stateful overload — the string-id open_popup("Save?") GetID won't match the container's PushID(DOCS_CLOSE_MODAL) ID, so the modal would never display.
        open_popup(DOCS_CLOSE_MODAL)
        POPUP_OPEN_LATCH = true
    }
    popup_modal(DOCS_CLOSE_MODAL, (text = "Save?", closable = false,
                                   flags = ImGuiWindowFlags.AlwaysAutoResize)) {
        text(DOCS_CLOSE_PROMPT,
             (text = "Save changes to the following items?"))
        let item_height = GetTextLineHeightWithSpacing()
        child(DOCS_CLOSE_LIST,
              (text = "frame", size = float2(-1.0f, 6.25f * item_height),
               child_flags = ImGuiChildFlags.FrameStyle,
               window_flags = ImGuiWindowFlags.None)) {
            for (n in range(length(CLOSE_QUEUE))) {
                let idx = CLOSE_QUEUE[n]
                if (DOCS[idx].dirty) {
                    text(DOCS_CLOSE_NAME[n], (text = DOCS[idx].name))
                }
            }
        }
        let btn_sz = float2(GetFontSize() * 7.0f, 0.0f)
        if (button(DOCS_CLOSE_YES, (text = "Yes", size = btn_sz))) {
            for (idx in CLOSE_QUEUE) {
                DOCS[idx].dirty = false
                DOCS[idx].open = false
            }
            CLOSE_QUEUE |> clear()
            POPUP_OPEN_LATCH = false
            close_current_popup(DOCS_CLOSE_MODAL)
        }
        same_line()
        if (button(DOCS_CLOSE_NO, (text = "No", size = btn_sz))) {
            for (idx in CLOSE_QUEUE) {
                DOCS[idx].open = false
                DOCS[idx].dirty = false
            }
            CLOSE_QUEUE |> clear()
            POPUP_OPEN_LATCH = false
            close_current_popup(DOCS_CLOSE_MODAL)
        }
        same_line()
        if (button(DOCS_CLOSE_CANCEL, (text = "Cancel", size = btn_sz))) {
            CLOSE_QUEUE |> clear()
            POPUP_OPEN_LATCH = false
            close_current_popup(DOCS_CLOSE_MODAL)
        }
    }
}

// ===== Entry point =====

def public ShowExampleAppDocuments() : bool {
    if (!DOCS_SEEDED) {
        seed_docs()
        DOCS_SEEDED = true
    }
    // X-button closes by setting DOCS_WIN.open=false; the aggregator then
    // clears its SHOW flag. When the user re-checks the demo checkbox,
    // re-enter with stale open=false — reset so the window renders again.
    DOCS_WIN.open = true
    SetNextWindowSize(ImVec2(640.0f, 480.0f), ImGuiCond.FirstUseEver)
    window(DOCS_WIN, (text = "Example: Documents", closable = true,
                      flags = ImGuiWindowFlags.MenuBar)) {
        menu_bar(DOCS_MENUBAR) {
            file_menu()
        }

        // Debug row: per-doc visibility checkbox. Mirror doc.open into the checkbox's
        // ToggleState.value each frame so the widget matches the data; the click handler
        // routes the new value back into doc.open.
        for (i in range(length(DOCS))) {
            var doc & = unsafe(DOCS[i])
            DOCS_OPEN_CHECKBOX[i].value = doc.open
            if (i > 0) {
                same_line(DOCS_DEBUG_SL[i])
            }
            if (checkbox(DOCS_OPEN_CHECKBOX[i], (text = doc.name))) {
                doc.open = DOCS_OPEN_CHECKBOX[i].value
                if (!doc.open) {
                    doc.dirty = false
                }
            }
        }
        separator(DOCS_SEP_DEBUG)

        // Tabs: one per open document. UnsavedDocument flag drives the dot
        // + the X-button-cancels-when-dirty behavior.
        tab_bar(DOCS_TABS, (text = "##tabs",
                            flags = ImGuiTabBarFlags.Reorderable)) {
            for (i in range(length(DOCS))) {
                var doc & = unsafe(DOCS[i])
                continue if (!doc.open)
                // DOCS_TAB[i].open backs ImGui's p_open via the closable tab_item — seed it true
                // each frame so the tab can reopen after a prior X-close (else .open stays false
                // and File>Open / the debug checkbox would set only doc.open, not the tab).
                DOCS_TAB[i].open = true
                let tab_flags = (doc.dirty
                                 ? ImGuiTabItemFlags.UnsavedDocument
                                 : ImGuiTabItemFlags.None)
                tab_item(DOCS_TAB[i], (text = doc.name, closable = true,
                                       flags = tab_flags)) {
                    display_context_menu(i)
                    display_contents(i)
                }
                // Tab close handling MUST live outside the tab_item body — ImGui flips p_open=false
                // on X-click even for INACTIVE tabs whose body never runs. Inside the body, the
                // frame-top reseed flips .open back to true before it could observe the close.
                if (!DOCS_TAB[i].open) {
                    if (doc.dirty) {
                        // Cancel close attempt: requeue + reopen for the
                        // confirmation popup.
                        DOCS_TAB[i].open = true
                        doc.want_close = true
                    } else {
                        doc.open = false
                    }
                }
            }
        }

        rebuild_close_queue()
        show_close_confirm()
    }
    let menu_closed = DOCS_REQUEST_CLOSE
    DOCS_REQUEST_CLOSE = false
    return DOCS_WIN.open && !menu_closed
}
