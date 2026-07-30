options gen2
options _comment_hygiene = true
options indenting = 4

module widgets

require imgui
require imgui/imgui_widgets_builtin
require imgui/imgui_containers_builtin
require imgui/imgui_table_builtin        // data_table + table_next_row/column
require imgui/imgui_id_builtin
require imgui/imgui_layout_builtin       // tree_pop pass-through
require imgui/imgui_scope_builtin
require imgui/imgui_drawlist_builtin     // with_window_drawlist + add_rect / add_rect_filled
require imgui/imgui_style_builtin        // with_style call_macro
require imgui/imgui_colors               // rgba packer (formerly v1 IM_COL32)
require daslib/safe_addr
require math

// =============================================================================
// Mirrors imgui_demo.cpp:651-2835 — static void ShowDemoWindowWidgets().
//
// Ported subsections:
//   Basic, Tooltips, Tree Nodes, Collapsing Headers, Bullets, Text, Images
//   (cpp:651-1276) + Tier-A quick-win subsections:
//   Combo (1279), List boxes (1347), Plotting (1898), Progress Bars (1961),
//   Range Widgets (2217), Multi-component (2352), Vertical Sliders (2385),
//   Disable block (2808), Text Filter (2816)
//   + Tier-B: Selectables (1396), Text Input (1558), Tabs (1752), Drag and Drop
//   (2450).
//   + Tier-C: Drag/Slider Flags (2183), Querying Item Status (2594),
//     Querying Window Status (2706), Color/Picker (1990), Data Types (2228).
//
// All sub-sections of `ShowDemoWindowWidgets` are now ported. No stubs.
//
// Boost-surface conventions for this port (masterplan §"Static-state pattern"):
//   - Value-owning widgets — checkbox / radio_button_int / slider_* / drag_* /
//     input_* / color_edit* / combo / button — go through `[widget]`. The first
//     argument is the auto-emitted state global's identifier; `init=` /
//     `bounds=` seed the one-shot state initializer. The value lives in
//     `IDENT.value` and persists across reloads via `@live`.
//   - Indexed widgets — `IDENT[i]` — used for the seven coloured-button and
//     eight textured-button loops; each index gets its own state global.
//   - Containers — `tree_node` / `collapsing_header` / `item_tooltip` /
//     `drag_drop_source` (Phase 1) — replace raw `Begin*/End*` pairs.
//   - Leaf widgets — `tree_node_ex` returns the open bool without claiming
//     `TreePop()`, used where post-header item-state queries are needed
//     (Tree/Advanced node + leaf rows).
//   - Scope wrappers — `with_id` / `with_style` (color + var) / `with_text_wrap_pos`
//     / `with_disabled` (Phase 1) — replace raw `Push*/Pop*` and the
//     `BeginDisabled/EndDisabled` pair.
//   - External-pointer cases — `edit_checkbox_flags` (Tree Nodes/Advanced),
//     `edit_slider_angle` (Basic), `edit_collapsing_header` (Collapsing
//     Headers / closable bool) — `[edit_widget]` rail taking `safe_addr(...)`
//     against module-scope plain vars or `unsafe(addr(struct.field))`.
//   - Hover-gated tooltips — `set_item_tooltip(IDENT, text, flags=...)` for the
//     `if (IsItemHovered(flags)) SetTooltip(text)` idiom. Default `flags =
//     ForTooltip`; override per call (DelayNone / Stationary / etc.).
//   - `IMGUI_DEMO_MARKER` test-engine markers dropped (off-limits per CLAUDE.md).
//   - `SetItemTooltip(s)` → `item_tooltip(IDENT) { Text(s) }` (binding gap).
// =============================================================================

// ----- App-state plain vars (NOT widget state) -----
// Flipped by the "Disable block" subsection's WIDGETS_DIS_CB checkbox
// (cpp:2807); read by upstream sections that wrap their contents in
// with_disabled(WIDGETS_DISABLE_ALL) to gate input. Stays a plain var
// (not [widget] state) because the *read* side doesn't need telemetry.
var WIDGETS_DISABLE_ALL = false

// ----- Tree Nodes/Advanced — external-pointer state -----
var WIDGETS_TREE_BASE_FLAGS = (ImGuiTreeNodeFlags.OpenOnArrow |
                               ImGuiTreeNodeFlags.OpenOnDoubleClick |
                               ImGuiTreeNodeFlags.SpanAvailWidth)
var WIDGETS_TREE_SELECTION_MASK = 1 << 2

// ----- Basic/SliderAngle external-pointer -----
var WIDGETS_BASIC_ANGLE = 0.0f

// ----- Plain counters (not widget state) -----
// C++ holds these as `static int counter`; a plain var keeps the imperative ++/-- semantics.
var WIDGETS_BASIC_ARROW_COUNTER = 0
var WIDGETS_IMAGES_PRESSED_COUNT = 0

// ----- Indexed widget tables -----
// The [widget] indexed form IDENT[k] needs an upfront module-scope table<K; ST>
// (the macro doesn't auto-emit indexed tables — see widgets/imgui_boost_v2.das).
var private WIDGETS_BASIC_COLOR_BTN : table<int; ClickState>
var private WIDGETS_TREE_BASIC : table<int; TreeNodeState>
var private WIDGETS_TREE_BASIC_BTN : table<int; ClickState>
var private WIDGETS_TREE_DRAG_SRC : table<int; DragDropSourceState>
var private WIDGETS_IMAGES_BTN : table<int; ClickState>
// In-loop narrative/marker widgets — one state global per iteration to avoid
// `check_unique_render` (same ident rendered N times per frame).
var private WIDGETS_LOOP_BASIC_COLOR_SL : table<int; EmptyMarkerState>
var private WIDGETS_LOOP_TREE_BASIC_TEXT : table<int; NarrativeState>
var private WIDGETS_LOOP_TREE_BASIC_SL : table<int; EmptyMarkerState>
var private WIDGETS_LOOP_TREE_DRAG_TEXT : table<int; NarrativeState>
var private WIDGETS_LOOP_TREE_BULLETTEXT : table<int; NarrativeState>
var private WIDGETS_LOOP_CH_TEXT_INNER : table<int; NarrativeState>
var private WIDGETS_LOOP_CH_TEXT_INNER2 : table<int; NarrativeState>
var private WIDGETS_LOOP_TEXT_PARA : table<int; NarrativeState>
var private WIDGETS_LOOP_IMG_SL : table<int; EmptyMarkerState>
var private WIDGETS_BASIC_LB_ROW : table<int; ClickState>
var private WIDGETS_TREE_LOOP_NODE : table<int; TreeNodeState>
var private WIDGETS_TREE_LOOP_LEAF : table<int; TreeNodeState>

// ----- Tier-A subsection state -----
// Combo: shared item list (cpp:1304) + ComboFlags via edit_checkbox_flags.
let private WIDGETS_COMBO_ITEMS : array<string> <- ["AAAA", "BBBB", "CCCC", "DDDD",
                                                    "EEEE", "FFFF", "GGGG", "HHHH",
                                                    "IIII", "JJJJ", "KKKK", "LLLLLLL",
                                                    "MMMM", "OOOOOOO"]
var private WIDGETS_COMBO_FLAGS = int(ImGuiComboFlags.None)
var private WIDGETS_COMBO_LB_ROW : table<int; ClickState>

// List Boxes: separate per-box row tables because every Selectable inside a
// container needs its own state global (path key disambiguation).
var private WIDGETS_LB_ROW1 : table<int; ClickState>
var private WIDGETS_LB_ROW2 : table<int; ClickState>

// Text Filter: per-line bullet idents (cpp:2828, 8 entries — kept generic).
var private WIDGETS_TF_LINE : table<int; NarrativeState>

// Plotting: animated values buffer (cpp:1912) + cosine-phase scratch.
var private WIDGETS_PLOT_VALUES : array<float>
var private WIDGETS_PLOT_VALUES_OFFSET = 0
var private WIDGETS_PLOT_REFRESH_TIME = 0.0lf
var private WIDGETS_PLOT_PHASE = 0.0f
let private WIDGETS_PLOT_FRAMETIMES : array<float> <- [0.6f, 0.1f, 1.0f, 0.5f,
                                                       0.92f, 0.1f, 0.2f]
let private WIDGETS_PLOT_FUNC_ITEMS : array<string> <- ["Sin", "Saw"]

// Progress Bars: animation state (cpp:1964); ping-pongs between -0.1 and 1.1.
var private WIDGETS_PROGRESS = 0.0f
var private WIDGETS_PROGRESS_DIR = 1.0f

// Combo "one-liner" items (cpp:1332) — hoisted to module-scope to avoid the
// per-frame heap allocation of the inline form.
let private WIDGETS_COMBO_ONELINER_ITEMS : array<string> <- ["aaaa", "bbbb",
                                                             "cccc", "dddd", "eeee"]

// Text Filter lines (cpp:2828) — hoisted to module-scope, same reason.
let private WIDGETS_TF_LINES : array<string> <- ["aaa1.c", "bbb1.c", "ccc1.c",
                                                 "aaa2.cpp", "bbb2.cpp", "ccc2.cpp",
                                                 "abc.h", "hello, world"]

// Vertical Sliders: three groups of state (cpp:2390 / 2394 / 2414).
// INT_PTR/VALUES fed to edit_vslider_*; SET1-3 per-iter same_line markers;
// GROUP the per-column BeginGroup; TIP1/2 per-iter tooltips.
var private WIDGETS_VS_INT_PTR = 0
var private WIDGETS_VS_VALUES : array<float> <- [0.0f, 0.60f, 0.35f, 0.9f,
                                                 0.70f, 0.20f, 0.0f]
var private WIDGETS_VS_VALUES2 : array<float> <- [0.20f, 0.80f, 0.40f, 0.25f]
var private WIDGETS_VS_SET1 : table<int; EmptyMarkerState>
var private WIDGETS_VS_SET2 : table<int; EmptyMarkerState>
var private WIDGETS_VS_SET3 : table<int; EmptyMarkerState>
var private WIDGETS_VS_GROUP : table<int; GroupState>
var private WIDGETS_VS_TIP1 : table<int; NarrativeState>
var private WIDGETS_VS_TIP2 : table<int; NarrativeState>

// Tabs/Advanced & Close: ImGuiTabBarFlags bit-bag from 6 checkboxes;
// WIDGETS_TABS_ADV_OPENED bool[] threads through both the checkbox row and the
// tab X-button (mirrors cpp's `bool opened[4]` shared across two UI surfaces).
var private WIDGETS_TABS_ADV_FLAGS = int(ImGuiTabBarFlags.Reorderable)
var private WIDGETS_TABS_ADV_OPENED : array<bool> <- [true, true, true, true]
var private WIDGETS_TABS_ADV_TOGGLE : table<int; EmptyMarkerState>
var private WIDGETS_TABS_ADV_BODY : table<int; NarrativeState>
let private WIDGETS_TABS_ADV_NAMES : array<string> <- ["Artichoke", "Beetroot",
                                                       "Celery", "Daikon"]

// Tabs/TabItemButton & Leading/Trailing: dynamic tab list driven by the "+"
// trailing button. `NEXT_ID == 0` is the seed sentinel — the section function
// fills 3 initial tabs on first frame (mirrors cpp:1828-1831).
var private WIDGETS_TABS_LT_FLAGS = int(ImGuiTabBarFlags.AutoSelectNewTabs |
                                        ImGuiTabBarFlags.Reorderable |
                                        ImGuiTabBarFlags.FittingPolicyShrink)
var private WIDGETS_TABS_LT_ACTIVE : array<int>
var private WIDGETS_TABS_LT_NEXT_ID = 0
var private WIDGETS_TABS_LT_DYN : table<int; TabItemState>

// Selectables (cpp:1396) — seven sub-trees. Most sub-trees are caller-owned
// bool/int selection state + indexed per-row `[widget]` tables; only "Basic"
// and "Same Line" / "Alignment" use self-toggling `selectable` (ToggleState).
var private WIDGETS_SEL_BASIC_TOGGLE : table<int; ToggleState>
var private WIDGETS_SEL_SINGLE_IDX = -1
var private WIDGETS_SEL_SINGLE_ROW : table<int; ClickState>
var private WIDGETS_SEL_MULTI_SEL : array<bool> <- [false, false, false, false, false]
var private WIDGETS_SEL_MULTI_ROW : table<int; ClickState>
var private WIDGETS_SEL_SL_ITEM : table<int; ToggleState>
var private WIDGETS_SEL_SL_LINK : table<int; ClickState>
var private WIDGETS_SEL_SL_SL : table<int; EmptyMarkerState>
let private WIDGETS_SEL_SL_LABELS : array<string> <- ["main.c", "Hello.cpp", "Hello.h"]
var private WIDGETS_SEL_COL1_ROW : table<int; ToggleState>
var private WIDGETS_SEL_COL2_ROW : table<int; ToggleState>
var private WIDGETS_SEL_COL2_TXT1 : table<int; NarrativeState>
var private WIDGETS_SEL_COL2_TXT2 : table<int; NarrativeState>
var private WIDGETS_SEL_GRID_SEL : array<bool> <- [true, false, false, false,
                                                   false, true, false, false,
                                                   false, false, true, false,
                                                   false, false, false, true]
var private WIDGETS_SEL_GRID_ROW : table<int; ClickState>
var private WIDGETS_SEL_GRID_SL : table<int; EmptyMarkerState>
var private WIDGETS_SEL_ALIGN_ROW : table<int; ToggleState>
var private WIDGETS_SEL_ALIGN_SL : table<int; EmptyMarkerState>

// Text Input (cpp:1558) — six sub-trees: Multi-line (persistent 16K string),
// Filtered (CallbackCharFilter), Password (shared buffer), Completion/History/Edit
// Callbacks (dispatch by data.EventFlag), Resize (CallbackResize), Misc (flag toggles).
let private WIDGETS_TI_MULTILINE_INIT = "/*\n The Pentium F00F bug, shorthand for F0 0F C7 C8,\n the hexadecimal encoding of one offending instruction,\n more formally, the invalid operand with locked CMPXCHG8B\n instruction bug, is a design flaw in the majority of\n Intel Pentium, Pentium MMX, and Pentium OverDrive\n processors (all in the P5 microarchitecture).\n*/\n\nlabel:\n\tlock cmpxchg8b eax\n"
var private WIDGETS_TI_ML_FLAGS = int(ImGuiInputTextFlags.AllowTabInput)
var private WIDGETS_TI_EDIT_COUNT = 0
var private WIDGETS_TI_MISC_FLAGS = int(ImGuiInputTextFlags.EscapeClearsAll)

// Drag and Drop (cpp:2450) — four sub-trees. Mode picker for the copy/swap
// sub-tree is an exclusive int selection (0=Copy / 1=Move / 2=Swap). NAMES is
// per-frame mutable (Copy/Move/Swap rewrite slots in place).
var private WIDGETS_DD_COPY_MODE = 0
var private WIDGETS_DD_COPY_NAMES : array<string> <- ["Bobby", "Beatrice", "Betty",
                                                      "Brianna", "Barry", "Bernard",
                                                      "Bibi", "Blaine", "Bryn"]
var private WIDGETS_DD_COPY_BTN : table<int; ClickState>
var private WIDGETS_DD_COPY_SRC : table<int; DragDropSourceState>
var private WIDGETS_DD_COPY_TGT : table<int; DragDropTargetState>
var private WIDGETS_DD_COPY_SL : table<int; EmptyMarkerState>
var private WIDGETS_DD_COPY_PREVIEW : table<int; NarrativeState>
var private WIDGETS_DD_REORDER_NAMES : array<string> <- ["Item One", "Item Two",
                                                          "Item Three", "Item Four",
                                                          "Item Five"]
var private WIDGETS_DD_REORDER_ROW : table<int; ToggleState>
var private WIDGETS_DD_TIP_BTN : table<int; ClickState>
var private WIDGETS_DD_TIP_TGT : table<int; DragDropTargetState>
var private WIDGETS_DD_TIP_NO : table<int; NarrativeState>

// Data Types (cpp:2228) — one persistent value per scalar type driving 50
// edit_drag/slider/input_scalar widgets (ImGuiDataType picked at compile time via
// typeinfo stripped_typename). Init values hit sign edges (S8 127, U8 255, S32 -1).
var private WIDGETS_DT_S8 : int8 = int8(127)
var private WIDGETS_DT_U8 : uint8 = uint8(255)
var private WIDGETS_DT_S16 : int16 = int16(32767)
var private WIDGETS_DT_U16 : uint16 = uint16(65535)
var private WIDGETS_DT_S32 : int = -1
var private WIDGETS_DT_U32 : uint = 0xffffffffu
var private WIDGETS_DT_S64 : int64 = -1l
var private WIDGETS_DT_U64 : uint64 = 0xfffffffffffffffful
var private WIDGETS_DT_F32 : float = 0.123f
var private WIDGETS_DT_F64 : double = 90000.01234567890123456789lf

// Color/Picker Widgets (cpp:1990) — one shared float4 viewed as RGBA and RGB-only
// (reinterpret<float3?>, mirroring cpp's (float*)&color). 32-swatch palette lazy-built
// via hsv_to_rgb; each swatch is a ColorButton drag source + drop target (COL3F/COL4F).
var private WIDGETS_CP_COLOR : float4 = float4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f)
var private WIDGETS_CP_BACKUP : float4 = float4(0.0f, 0.0f, 0.0f, 0.0f)
var private WIDGETS_CP_REF_COLOR_V : float4 = float4(1.0f, 0.0f, 1.0f, 0.5f)
var private WIDGETS_CP_COLOR_HSV : float4 = float4(0.23f, 1.0f, 1.0f, 1.0f)
var private WIDGETS_CP_PALETTE : array<float4>
var private WIDGETS_CP_PALETTE_INIT = false
var private WIDGETS_CP_PALETTE_BTN : table<int; ClickState>
var private WIDGETS_CP_PALETTE_TGT : table<int; DragDropTargetState>
var private WIDGETS_CP_PALETTE_SL : table<int; EmptyMarkerState>
let private WIDGETS_CP_DISPLAY_ITEMS : array<string> <- ["Auto/Current",
                                                          "None",
                                                          "RGB Only",
                                                          "HSV Only",
                                                          "Hex Only"]

// Drag/Slider Flags (cpp:2183) — one ImGuiSliderFlags bitset (from 4 edit_checkbox_flags)
// shared by all 5 drag + 2 slider widgets via `flags=`. drag_f/slider_f/drag_i/slider_i
// route through edit_* against a plain `?` ptr so entries hitting the same value share it.
var private WIDGETS_DSF_FLAGS = int(ImGuiSliderFlags.None)
var private WIDGETS_DSF_DRAG_F = 0.5f
var private WIDGETS_DSF_DRAG_I = 50
var private WIDGETS_DSF_SLIDER_F = 0.5f
var private WIDGETS_DSF_SLIDER_I = 50

// Querying Item Status (cpp:2594) — Combo-driven dispatcher over 16 item kinds.
// The 4 InputText/scalar/color widgets share one col4f array + str buffer via edit_*;
// the 12 self-contained widgets each get their own state global per branch.
var private WIDGETS_QIS_DISABLED = false
var private WIDGETS_QIS_F1 = 1.0f
var private WIDGETS_QIS_F1B = 0.5f
var private WIDGETS_QIS_F3 : float3 = float3(1.0f, 0.5f, 0.0f)
var private WIDGETS_QIS_COL4F : float4 = float4(1.0f, 0.5f, 0.0f, 1.0f)
let private WIDGETS_QIS_TYPES : array<string> <- ["Text", "Button", "Button (w/ repeat)",
                                                  "Checkbox", "SliderFloat", "InputText",
                                                  "InputTextMultiline", "InputFloat",
                                                  "InputFloat3", "ColorEdit4", "Selectable",
                                                  "MenuItem", "TreeNode",
                                                  "TreeNode (w/ double-click)",
                                                  "Combo", "ListBox"]
let private WIDGETS_QIS_FRUITS : array<string> <- ["Apple", "Banana", "Cherry", "Kiwi"]
// Per-item-type widget states are auto-emitted by the [widget] macro from the
// IDENT name; only the ListBox row table needs an explicit declaration (the
// macro can't infer indexed IDENT[k] shapes).
var private WIDGETS_QIS_LB_ROW : table<int; ToggleState>

// Querying Window Status (cpp:2706) — one outer-child toggle + one
// title-bar test window (secondary Begin/End). The cpp inner child uses a
// fixed 50-px tall scroll region; we mirror via the boost `child` rail.
var private WIDGETS_QWS_EMBED = false
var private WIDGETS_QWS_TEST = false


// =============================================================================
// Helpers
// =============================================================================

// =============================================================================
// ShowDemoWindowWidgets — dispatcher
// =============================================================================

def ShowDemoWindowWidgets() {
    collapsing_header(WIDGETS_SECTION,
        (text = "Widgets", closable = false, flags = ImGuiTreeNodeFlags.None)) {
            with_disabled(WIDGETS_DISABLE_ALL) {
                BasicSection()
                TooltipsSection()
                TreeNodesSection()
                CollapsingHeadersSection()
                BulletsSection()
                TextSection()
                ImagesSection()
                ComboSection()
                ListBoxesSection()
                SelectablesSection()
                TextInputSection()
                TabsSection()
                PlottingSection()
                ProgressBarsSection()
                RangeWidgetsSection()
                MultiComponentSection()
                VerticalSlidersSection()
                DragDropSection()
                ColorPickerSection()
                DataTypesSection()
                DragSliderFlagsSection()
                QueryingItemStatusSection()
                QueryingWindowStatusSection()
            }
            // "Disable block" exposes the WIDGETS_DISABLE_ALL checkbox itself; it would be
            // hidden by its own gate, so it sits OUTSIDE with_disabled (matches cpp:2807
            // placement after the with_disabled-equivalent block).
            DisableBlockSection()
            TextFilterSection()
    }
}


// =============================================================================
// Basic — cpp:662-855
// =============================================================================

def private BasicSection() {
    tree_node(WIDGETS_BASIC_SECTION,
        (text = "Basic", flags = ImGuiTreeNodeFlags.None)) {

        separator_text("General")

        // Button + click counter
        if (button(WIDGETS_BASIC_BUTTON, (text = "Button"))) {
            WIDGETS_BASIC_BUTTON.click_count++
        }
        if ((WIDGETS_BASIC_BUTTON.click_count & 1) != 0) {
            same_line(WIDGETS_SL_1)
            text("Thanks for clicking me!")
        }

        // Checkbox
        checkbox(WIDGETS_BASIC_CHECK, (text = "checkbox", init = true))

        // RadioButton — three buttons sharing one int state (radio_button_int).
        radio_button_int(WIDGETS_BASIC_RADIO, (text = "radio a", v_button = 0)); same_line(WIDGETS_SL_2)
        radio_button_int(WIDGETS_BASIC_RADIO, (text = "radio b", v_button = 1)); same_line(WIDGETS_SL_3)
        radio_button_int(WIDGETS_BASIC_RADIO, (text = "radio c", v_button = 2))

        // Coloured Buttons — with_style scopes the per-button color stack; indexed WIDGETS_BASIC_COLOR_BTN[i] gives each iteration its own state global.
        for (i in range(7)) {
            if (i > 0) same_line(WIDGETS_LOOP_BASIC_COLOR_SL[i])
            let hue = float(i) / 7.0f
            with_style((ImGuiCol.Button, HSV(hue, 0.6f, 0.6f)),
                       (ImGuiCol.ButtonHovered, HSV(hue, 0.7f, 0.7f)),
                       (ImGuiCol.ButtonActive, HSV(hue, 0.8f, 0.8f))) {
                button(WIDGETS_BASIC_COLOR_BTN[i], (text = "Click"))
            }
        }

        // Arrow buttons with the Repeater flag, wrapped by the with_button_repeat scope; the arrow buttons go through boost.
        align_text_to_frame_padding(WIDGETS_BASIC_ATFP)
        text("Hold to repeat:")
        same_line(WIDGETS_SL_4)
        let spacing = GetStyle().ItemInnerSpacing.x
        with_button_repeat(true) {
            if (arrow_button(WIDGETS_BASIC_ARROW_L, (text = "##left", dir = ImGuiDir.Left))) {
                WIDGETS_BASIC_ARROW_COUNTER--
            }
            same_line((offset_from_start_x = 0.0f, spacing = spacing))
            if (arrow_button(WIDGETS_BASIC_ARROW_R, (text = "##right", dir = ImGuiDir.Right))) {
                WIDGETS_BASIC_ARROW_COUNTER++
            }
        }
        same_line(WIDGETS_SL_6)
        text("{WIDGETS_BASIC_ARROW_COUNTER}")

        // SetItemTooltip → item_tooltip container (binding-gap shim).
        button(WIDGETS_BASIC_TOOLTIP_BTN, (text = "Tooltip"))
        item_tooltip(WIDGETS_BASIC_TOOLTIP_TIP) {
            text("I am a tooltip")
        }

        label_text(WIDGETS_BASIC_LABEL, (key = "label", value = "Value"))

        separator_text("Inputs")

        input_text(WIDGETS_BASIC_STR0, (text = "input text", init = "Hello, world!"))
        same_line(WIDGETS_SL_7)
        text_disabled("(?)")
        item_tooltip(WIDGETS_BASIC_HM1_TIP) {
            with_text_wrap_pos(GetFontSize() * 35.0f) {
                text_unformatted((text = "USER:\n" +
                    "Hold SHIFT or use mouse to select text.\n" +
                    "CTRL+Left/Right to word jump.\n" +
                    "CTRL+A or Double-Click to select all.\n" +
                    "CTRL+X,CTRL+C,CTRL+V clipboard.\n" +
                    "CTRL+Z,CTRL+Y undo/redo.\n" +
                    "ESCAPE to revert.\n\n" +
                    "PROGRAMMER:\n" +
                    "You can use the ImGuiInputTextFlags_CallbackResize facility if you " +
                    "need to wire InputText() to a dynamic string type. See misc/cpp/" +
                    "imgui_stdlib.h for an example (this is not demonstrated in " +
                    "imgui_demo.cpp)."))
            }
        }

        input_text_with_hint(WIDGETS_BASIC_STR1,
            (text = "input text (w/ hint)", hint = "enter text here"))

        input_int(WIDGETS_BASIC_I0, (text = "input int", init = 123))
        input_float(WIDGETS_BASIC_F0,
            (text = "input float", init = 0.001f, step = 0.01f, step_fast = 1.0f,
             format = "%.3f"))
        input_double(WIDGETS_BASIC_D0,
            (text = "input double", init = 999999.00000001lf,
             step = 0.01lf, step_fast = 1.0lf, format = "%.8f"))
        input_float(WIDGETS_BASIC_F_SCI,
            (text = "input scientific", init = 1.0e10f,
             step = 0.0f, step_fast = 0.0f, format = "%e"))
        same_line(WIDGETS_SL_8)
        text_disabled("(?)")
        item_tooltip(WIDGETS_BASIC_HM2_TIP) {
            with_text_wrap_pos(GetFontSize() * 35.0f) {
                text_unformatted((text = "You can input value using the scientific notation,\n" +
                    "  e.g. \"1e+8\" becomes \"100000000\"."))
            }
        }
        input_float3(WIDGETS_BASIC_VEC4A,
            (text = "input float3", init = float3(0.10f, 0.20f, 0.30f)))

        separator_text("Drags")

        drag_int(WIDGETS_BASIC_DRAG_I1, (text = "drag int", init = 50, speed = 1.0f))
        same_line(WIDGETS_SL_9)
        text_disabled("(?)")
        item_tooltip(WIDGETS_BASIC_HM3_TIP) {
            with_text_wrap_pos(GetFontSize() * 35.0f) {
                text_unformatted((text = "Click and drag to edit value.\n" +
                    "Hold SHIFT/ALT for faster/slower edit.\n" +
                    "Double-click or CTRL+click to input value."))
            }
        }

        drag_int(WIDGETS_BASIC_DRAG_I2,
            (text = "drag int 0..100", init = 42, speed = 1.0f, bounds = (0, 100),
             format = "%d%%", flags = ImGuiSliderFlags.AlwaysClamp))
        drag_float(WIDGETS_BASIC_DRAG_F1,
            (text = "drag float", init = 1.0f, speed = 0.005f))
        drag_float(WIDGETS_BASIC_DRAG_F2,
            (text = "drag small float", init = 0.0067f, speed = 0.0001f,
             format = "%.06f ns"))

        separator_text("Sliders")

        slider_int(WIDGETS_BASIC_SLIDER_I1,
            (text = "slider int", init = 0, bounds = (-1, 3)))
        same_line(WIDGETS_SL_10)
        text_disabled("(?)")
        item_tooltip(WIDGETS_BASIC_HM4_TIP) {
            with_text_wrap_pos(GetFontSize() * 35.0f) {
                text_unformatted("CTRL+click to input value.")
            }
        }

        slider_float(WIDGETS_BASIC_SLIDER_F1,
            (text = "slider float", init = 0.123f, bounds = (0.0f, 1.0f),
             format = "ratio = %.3f"))
        slider_float(WIDGETS_BASIC_SLIDER_F2,
            (text = "slider float (log)", init = 0.0f, bounds = (-10.0f, 10.0f),
             format = "%.4f", flags = ImGuiSliderFlags.Logarithmic))

        edit_slider_angle(safe_addr(WIDGETS_BASIC_ANGLE),
            (id = "WIDGETS_BASIC_ANGLE_SLIDER", text = "slider angle"))

        // Slider (enum): omit %d in the format so the label IS the value (dynamic, so it goes in the regular arg not init=).
        let elem_names = fixed_array("Fire", "Earth", "Air", "Water")
        let elem_v = WIDGETS_BASIC_ELEM.value
        let elem_name = (elem_v >= 0 && elem_v < 4) ? elem_names[elem_v] : "Unknown"
        slider_int(WIDGETS_BASIC_ELEM,
            (text = "slider enum", init = 0, bounds = (0, 3), format = elem_name))
        same_line(WIDGETS_SL_11)
        text_disabled("(?)")
        item_tooltip(WIDGETS_BASIC_HM5_TIP) {
            with_text_wrap_pos(GetFontSize() * 35.0f) {
                text_unformatted((text = "Using the format string parameter to display a name instead of the " +
                    "underlying integer."))
            }
        }

        separator_text("Selectors/Pickers")

        color_edit3(WIDGETS_BASIC_COL1,
            (text = "color 1", init = float3(1.0f, 0.0f, 0.2f)))
        same_line(WIDGETS_SL_12)
        text_disabled("(?)")
        item_tooltip(WIDGETS_BASIC_HM6_TIP) {
            with_text_wrap_pos(GetFontSize() * 35.0f) {
                text_unformatted((text = "Click on the color square to open a color picker.\n" +
                    "Click and hold to use drag and drop.\n" +
                    "Right-click on the color square to show options.\n" +
                    "CTRL+click on individual component to input value.\n"))
            }
        }
        color_edit4(WIDGETS_BASIC_COL2,
            (text = "color 2", init = float4(0.4f, 0.7f, 0.0f, 0.5f)))

        // One-liner Combo
        combo(WIDGETS_BASIC_COMBO,
            (text = "combo", init = 0,
             items <- ["AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG",
                       "HHHH", "IIIIIII", "JJJJ", "KKKKKKK"]))
        same_line(WIDGETS_SL_13)
        text_disabled("(?)")
        item_tooltip(WIDGETS_BASIC_HM7_TIP) {
            with_text_wrap_pos(GetFontSize() * 35.0f) {
                text_unformatted((text = "Using the simplified one-liner Combo API here.\n" +
                    "Refer to the \"Combo\" section below for an explanation of how to " +
                    "use the more flexible and general BeginCombo/EndCombo API."))
            }
        }

        // One-liner ListBox via the manual BeginListBox/EndListBox container — Selectable per item, sharing the listbox's `value` for the selected row.
        list_box(WIDGETS_BASIC_LISTBOX,
            (text = "listbox", size = float2(0.0f, GetTextLineHeightWithSpacing() * 4.25f))) {
            let lb_items = fixed_array("Apple", "Banana", "Cherry", "Kiwi", "Mango",
                                       "Orange", "Pineapple", "Strawberry", "Watermelon")
            for (i in range(length(lb_items))) {
                let is_sel = WIDGETS_BASIC_LISTBOX.value == i
                if (selectable_label(WIDGETS_BASIC_LB_ROW[i],
                                     (text = lb_items[i], selected = is_sel))) {
                    WIDGETS_BASIC_LISTBOX.value = i
                }
            }
        }
        same_line(WIDGETS_SL_14)
        text_disabled("(?)")
        item_tooltip(WIDGETS_BASIC_HM8_TIP) {
            with_text_wrap_pos(GetFontSize() * 35.0f) {
                text_unformatted((text = "Using the manual BeginListBox/EndListBox idiom here.\n" +
                    "Refer to the \"List boxes\" section below for the full discussion."))
            }
        }
    }
}


// =============================================================================
// Tooltips — cpp:858-948
// =============================================================================

def private TooltipsSection() {
    tree_node(WIDGETS_TOOLTIPS_SECTION,
        (text = "Tooltips", flags = ImGuiTreeNodeFlags.None)) {

        separator_text("General")
        text_disabled("(?)")
        item_tooltip(WIDGETS_TIPS_HM1_TIP) {
            with_text_wrap_pos(GetFontSize() * 35.0f) {
                text_unformatted((text = "Tooltip are typically created by using a IsItemHovered() + " +
                    "SetTooltip() sequence.\n\nWe provide a helper SetItemTooltip() " +
                    "function to perform the two with standards flags."))
            }
        }

        // C++ passes ImVec2(-FLT_MIN, 0) for full-row-width buttons; boost `button` has no `size` yet, so these render at default content width.
        button(WIDGETS_TIP_BASIC, (text = "Basic"))
        item_tooltip(WIDGETS_TIP_BASIC_TIP) {
            text("I am a tooltip")
        }

        button(WIDGETS_TIP_FANCY, (text = "Fancy"))
        item_tooltip(WIDGETS_TIP_FANCY_TIP) {
            text("I am a fancy tooltip")
            plot_lines(WIDGETS_TIP_CURVE, "Curve",
                       [0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f])
            text("Sin(time) = {sin(float(GetTime()))}")
        }

        separator_text("Always On")

        // Three-way radio: Off / Always On (Simple) / Always On (Advanced)
        radio_button_int(WIDGETS_TIP_ALWAYS_ON, (text = "Off", v_button = 0)); same_line(WIDGETS_SL_15)
        radio_button_int(WIDGETS_TIP_ALWAYS_ON, (text = "Always On (Simple)", v_button = 1)); same_line(WIDGETS_SL_16)
        radio_button_int(WIDGETS_TIP_ALWAYS_ON, (text = "Always On (Advanced)", v_button = 2))
        if (WIDGETS_TIP_ALWAYS_ON.value == 1) {
            set_tooltip(WIDGETS_TIP_FOLLOW_TT, "I am following you around.")
        } elif (WIDGETS_TIP_ALWAYS_ON.value == 2) {
            // Advanced form draws into the tooltip body; value-gated (the boost `tooltip` container handles BeginTooltip/EndTooltip itself).
            tooltip(WIDGETS_TIP_1) {
                progress_bar((fraction = sin(float(GetTime())) * 0.5f + 0.5f,
                              size = float2(GetFontSize() * 25.0f, 0.0f),
                              overlay = ""))
            }
        }

        separator_text("Custom")
        text_disabled("(?)")
        item_tooltip(WIDGETS_TIPS_HM2_TIP) {
            with_text_wrap_pos(GetFontSize() * 35.0f) {
                text_unformatted((text = "Passing ImGuiHoveredFlags_ForTooltip to IsItemHovered() is the " +
                    "preferred way to standardize tooltip activation details across " +
                    "your application. You may however decide to use custom flags for " +
                    "a specific tooltip instance."))
            }
        }

        button(WIDGETS_TIP_MANUAL, (text = "Manual"))
        set_item_tooltip(WIDGETS_TIP_MANUAL_TT, "I am a manually emitted tooltip.")

        button(WIDGETS_TIP_DELAY_NONE, (text = "DelayNone"))
        set_item_tooltip(WIDGETS_TIP_DELAY_NONE_TT,
                         (text = "I am a tooltip with no delay.",
                          flags = ImGuiHoveredFlags.DelayNone))

        button(WIDGETS_TIP_DELAY_SHORT, (text = "DelayShort"))
        let delay_short = GetStyle().HoverDelayShort
        set_item_tooltip(WIDGETS_TIP_DELAY_SHORT_TT,
                         (text = "I am a tooltip with a short delay ({delay_short} sec).",
                          flags = ImGuiHoveredFlags.DelayShort | ImGuiHoveredFlags.NoSharedDelay))

        button(WIDGETS_TIP_DELAY_LONG, (text = "DelayLong"))
        let delay_long = GetStyle().HoverDelayNormal
        set_item_tooltip(WIDGETS_TIP_DELAY_LONG_TT,
                         (text = "I am a tooltip with a long delay ({delay_long} sec).",
                          flags = ImGuiHoveredFlags.DelayNormal | ImGuiHoveredFlags.NoSharedDelay))

        button(WIDGETS_TIP_STATIONARY, (text = "Stationary"))
        set_item_tooltip(WIDGETS_TIP_STATIONARY_TT,
                         (text = "I am a tooltip requiring mouse to be stationary before activating.",
                          flags = ImGuiHoveredFlags.Stationary))

        // Disabled-item tooltip — ForTooltip flag pulls AllowWhenDisabled from style.
        with_disabled(true) {
            button(WIDGETS_TIP_DISABLED, (text = "Disabled item"))
        }
        set_item_tooltip(WIDGETS_TIP_DISABLED_TT, "I am a a tooltip for a disabled item.")
    }
}


// =============================================================================
// Tree Nodes — cpp:957-1078
// =============================================================================

def private TreeNodesSection() {
    tree_node(WIDGETS_TREES_SECTION,
        (text = "Tree Nodes", flags = ImGuiTreeNodeFlags.None)) {

        // ----- Basic trees -----
        tree_node(WIDGETS_TREES_BASIC,
            (text = "Basic trees", flags = ImGuiTreeNodeFlags.None)) {
            for (i in range(5)) {
                if (i == 0) SetNextItemOpen(true, ImGuiCond.Once)
                // Indexed identifier — each iteration gets its own state global and telemetry path (WIDGETS_TREE_BASIC[0..4]).
                tree_node(WIDGETS_TREE_BASIC[i],
                          (text = "Child {i}", flags = ImGuiTreeNodeFlags.None)) {
                    text(WIDGETS_LOOP_TREE_BASIC_TEXT[i], (text = "blah blah"))
                    same_line(WIDGETS_LOOP_TREE_BASIC_SL[i])
                    small_button(WIDGETS_TREE_BASIC_BTN[i], (text = "button"))
                }
            }
        }

        // ----- Advanced, with Selectable nodes -----
        tree_node(WIDGETS_TREES_ADVANCED,
            (text = "Advanced, with Selectable nodes",
             flags = ImGuiTreeNodeFlags.None)) {
            text_disabled("(?)")
            item_tooltip(WIDGETS_TREE_HM1_TIP) {
                with_text_wrap_pos(GetFontSize() * 35.0f) {
                    text_unformatted((text = "This is a more typical looking tree with selectable nodes.\n" +
                        "Click to select, CTRL+Click to toggle, click on arrows or " +
                        "double-click to open."))
                }
            }

            // CheckboxFlags drives one shared int (base_flags) via edit_checkbox_flags, each call toggling one bit; plain bool checkboxes below use the regular `checkbox`.
            edit_checkbox_flags(safe_addr(WIDGETS_TREE_BASE_FLAGS),
                (id = "WIDGETS_TREE_FLAG_OPEN_ON_ARROW",
                 text = "ImGuiTreeNodeFlags_OpenOnArrow",
                 flags_value = ImGuiTreeNodeFlags.OpenOnArrow))
            edit_checkbox_flags(safe_addr(WIDGETS_TREE_BASE_FLAGS),
                (id = "WIDGETS_TREE_FLAG_OPEN_ON_DOUBLE_CLICK",
                 text = "ImGuiTreeNodeFlags_OpenOnDoubleClick",
                 flags_value = ImGuiTreeNodeFlags.OpenOnDoubleClick))
            edit_checkbox_flags(safe_addr(WIDGETS_TREE_BASE_FLAGS),
                (id = "WIDGETS_TREE_FLAG_SPAN_AVAIL_WIDTH",
                 text = "ImGuiTreeNodeFlags_SpanAvailWidth",
                 flags_value = ImGuiTreeNodeFlags.SpanAvailWidth))
            same_line(WIDGETS_SL_17)
            text_disabled("(?)")
            item_tooltip(WIDGETS_TREE_HM2_TIP) {
                with_text_wrap_pos(GetFontSize() * 35.0f) {
                    text_unformatted((text = "Extend hit area to all available width instead of allowing " +
                        "more items to be laid out after the node."))
                }
            }
            edit_checkbox_flags(safe_addr(WIDGETS_TREE_BASE_FLAGS),
                (id = "WIDGETS_TREE_FLAG_SPAN_FULL_WIDTH",
                 text = "ImGuiTreeNodeFlags_SpanFullWidth",
                 flags_value = ImGuiTreeNodeFlags.SpanFullWidth))
            edit_checkbox_flags(safe_addr(WIDGETS_TREE_BASE_FLAGS),
                (id = "WIDGETS_TREE_FLAG_SPAN_TEXT_WIDTH",
                 text = "ImGuiTreeNodeFlags_SpanLabelWidth",
                 flags_value = ImGuiTreeNodeFlags.SpanLabelWidth))
            same_line(WIDGETS_SL_18)
            text_disabled("(?)")
            item_tooltip(WIDGETS_TREE_HM3_TIP) {
                with_text_wrap_pos(GetFontSize() * 35.0f) {
                    text_unformatted("Reduce hit area to the text label and a bit of margin.")
                }
            }
            edit_checkbox_flags(safe_addr(WIDGETS_TREE_BASE_FLAGS),
                (id = "WIDGETS_TREE_FLAG_SPAN_ALL_COLUMNS",
                 text = "ImGuiTreeNodeFlags_SpanAllColumns",
                 flags_value = ImGuiTreeNodeFlags.SpanAllColumns))
            same_line(WIDGETS_SL_19)
            text_disabled("(?)")
            item_tooltip(WIDGETS_TREE_HM4_TIP) {
                with_text_wrap_pos(GetFontSize() * 35.0f) {
                    text_unformatted("For use in Tables only.")
                }
            }
            edit_checkbox_flags(safe_addr(WIDGETS_TREE_BASE_FLAGS),
                (id = "WIDGETS_TREE_FLAG_ALLOW_OVERLAP",
                 text = "ImGuiTreeNodeFlags_AllowOverlap",
                 flags_value = ImGuiTreeNodeFlags.AllowOverlap))
            edit_checkbox_flags(safe_addr(WIDGETS_TREE_BASE_FLAGS),
                (id = "WIDGETS_TREE_FLAG_FRAMED",
                 text = "ImGuiTreeNodeFlags_Framed",
                 flags_value = ImGuiTreeNodeFlags.Framed))
            same_line(WIDGETS_SL_20)
            text_disabled("(?)")
            item_tooltip(WIDGETS_TREE_HM5_TIP) {
                with_text_wrap_pos(GetFontSize() * 35.0f) {
                    text_unformatted("Draw frame with background (e.g. for CollapsingHeader)")
                }
            }

            checkbox(WIDGETS_TREE_ALIGN, (text = "Align label with current X position"))
            checkbox(WIDGETS_TREE_DRAG_TEST, (text = "Test tree node as drag source"))

            text("Hello!")
            if (WIDGETS_TREE_ALIGN.value) {
                unindent(WIDGETS_TREE_REUNINDENT, GetTreeNodeToLabelSpacing())
            }

            var node_clicked = -1
            for (i in range(6)) {
                // Disable default "open on single-click" + set Selected from mask.
                var node_flags = WIDGETS_TREE_BASE_FLAGS
                let is_selected = (WIDGETS_TREE_SELECTION_MASK & (1 << i)) != 0
                if (is_selected) {
                    node_flags |= ImGuiTreeNodeFlags.Selected
                }
                // Per-row ID scope. tree_node_ex returns the open bool without claiming a TreePop; the container can't be used here because we need post-header item-state queries (IsItemClicked, drag source, inline button) at the right call site.
                with_id("row_{i}") {
                    if (i < 3) {
                        let node_open = tree_node_ex(WIDGETS_TREE_LOOP_NODE[i],
                            (text = "Selectable Node {i}",
                             flags = node_flags))
                        if (IsItemClicked() && !IsItemToggledOpen()) {
                            node_clicked = i
                        }
                        if (WIDGETS_TREE_DRAG_TEST.value) {
                            drag_drop_source(WIDGETS_TREE_DRAG_SRC[i],
                                             (flags = ImGuiDragDropFlags.None)) {
                                SetDragDropPayload("_TREENODE", null, 0ul)
                                text(WIDGETS_LOOP_TREE_DRAG_TEXT[i], (text = "This is a drag and drop source"))
                            }
                        }
                        if (i == 2) {
                            // Item 2 has an inline button to demonstrate SpanLabelWidth.
                            same_line(WIDGETS_SL_21)
                            small_button(WIDGETS_TREE_INLINE_BTN, (text = "button"))
                        }
                        if (node_open) {
                            bullet_text(WIDGETS_LOOP_TREE_BULLETTEXT[i], (text = "Blah blah\nBlah Blah"))
                            tree_pop()
                        }
                    } else {
                        // Leaves: Leaf + NoTreePushOnOpen — no TreePop is paired.
                        node_flags |= (ImGuiTreeNodeFlags.Leaf |
                                       ImGuiTreeNodeFlags.NoTreePushOnOpen)
                        tree_node_ex(WIDGETS_TREE_LOOP_LEAF[i],
                            (text = "Selectable Leaf {i}",
                             flags = node_flags))
                        if (IsItemClicked() && !IsItemToggledOpen()) {
                            node_clicked = i
                        }
                        if (WIDGETS_TREE_DRAG_TEST.value) {
                            drag_drop_source(WIDGETS_TREE_DRAG_SRC[i],
                                             (flags = ImGuiDragDropFlags.None)) {
                                SetDragDropPayload("_TREENODE", null, 0ul)
                                text(WIDGETS_LOOP_TREE_DRAG_TEXT[i], (text = "This is a drag and drop source"))
                            }
                        }
                    }
                }
            }
            if (node_clicked != -1) {
                if (GetIO().KeyCtrl) {
                    WIDGETS_TREE_SELECTION_MASK ^= (1 << node_clicked)
                } else {
                    WIDGETS_TREE_SELECTION_MASK = (1 << node_clicked)
                }
            }
            if (WIDGETS_TREE_ALIGN.value) {
                indent(WIDGETS_TREE_REINDENT, GetTreeNodeToLabelSpacing())
            }
        }
    }
}


// =============================================================================
// Collapsing Headers — cpp:1080-1102
// =============================================================================

def private CollapsingHeadersSection() {
    tree_node(WIDGETS_CH_SECTION,
        (text = "Collapsing Headers", flags = ImGuiTreeNodeFlags.None)) {
            checkbox(WIDGETS_CH_CLOSABLE_GROUP,
                (text = "Show 2nd header", init = true))
            collapsing_header(WIDGETS_CH_HEADER,
                (text = "Header", closable = false, flags = ImGuiTreeNodeFlags.None)) {
                text("IsItemHovered: {IsItemHovered()}")
                for (i in range(5)) {
                    text(WIDGETS_LOOP_CH_TEXT_INNER[i], (text = "Some content {i}"))
                }
            }
            // The second header's X-button wires to the checkbox bool via edit_collapsing_header (external p_visible); the body runs only while true AND expanded.
            if (edit_collapsing_header(safe_addr(WIDGETS_CH_CLOSABLE_GROUP.value),
                                       (id = "WIDGETS_CH_HEADER2",
                                        text = "Header with a close button"))) {
                text("IsItemHovered: {IsItemHovered()}")
                for (i in range(5)) {
                    text(WIDGETS_LOOP_CH_TEXT_INNER2[i], (text = "More content {i}"))
                }
            }
    }
}


// =============================================================================
// Bullets — cpp:1104-1117
// =============================================================================

def private BulletsSection() {
    tree_node(WIDGETS_BULLETS_SECTION,
        (text = "Bullets", flags = ImGuiTreeNodeFlags.None)) {
            bullet_text("Bullet point 1")
            bullet_text("Bullet point 2\nOn multiple lines")
            tree_node(WIDGETS_BULLETS_INNER,
                (text = "Tree node", flags = ImGuiTreeNodeFlags.None)) {
                bullet_text("Another bullet point")
            }
        bullet(WIDGETS_BULLET_4); text("Bullet point 3 (two calls)")
        bullet(WIDGETS_BULLET_5); small_button(WIDGETS_BULLETS_BTN, (text = "Button"))

    }
}


// =============================================================================
// Text — cpp:1119-1192
// =============================================================================

def private TextSection() {
    tree_node(WIDGETS_TEXT_SECTION,
        (text = "Text", flags = ImGuiTreeNodeFlags.None)) {

            tree_node(WIDGETS_TEXT_COLORFUL,
                (text = "Colorful Text", flags = ImGuiTreeNodeFlags.None)) {
                text_colored(WIDGETS_TEXT_PINK, float4(1.0f, 0.0f, 1.0f, 1.0f), "Pink")
                text_colored(WIDGETS_TEXT_YELLOW, float4(1.0f, 1.0f, 0.0f, 1.0f), "Yellow")
                text_disabled("Disabled")
                same_line(WIDGETS_SL_22)
                text_disabled("(?)")
                item_tooltip(WIDGETS_TEXT_HM1_TIP) {
                    with_text_wrap_pos(GetFontSize() * 35.0f) {
                        text_unformatted("The TextDisabled color is stored in ImGuiStyle.")
                    }
                }
            }

            tree_node(WIDGETS_TEXT_WORDWRAP,
                (text = "Word Wrapping", flags = ImGuiTreeNodeFlags.None)) {
                text_wrapped((text = "This text should automatically wrap on the edge of the " +
                    "window. The current implementation for text wrapping " +
                    "follows simple rules suitable for English and possibly " +
                    "other languages."))
                spacing(WIDGETS_SP_1)

                slider_float(WIDGETS_TEXT_WRAP_WIDTH,
                    (text = "Wrap width", init = 200.0f, bounds = (-20.0f, 600.0f),
                     format = "%.0f"))

                for (n in range(2)) {
                    text(WIDGETS_LOOP_TEXT_PARA[n], (text = "Test paragraph {n}:"))
                    let pos = GetCursorScreenPos()
                    let wrap = WIDGETS_TEXT_WRAP_WIDTH.value
                    let marker_min = float2(pos.x + wrap, pos.y)
                    let marker_max = float2(pos.x + wrap + 10.0f,
                                            pos.y + GetTextLineHeight())
                    with_text_wrap_pos(GetCursorPos().x + wrap) {
                        if (n == 0) {
                            text((text = "The lazy dog is a good dog. This paragraph " +
                                         "should fit within {wrap} pixels. Testing a 1 " +
                                         "character word. The quick brown fox jumps over " +
                                         "the lazy dog."))
                        } else {
                            text((text = "aaaaaaaa bbbbbbbb, c cccccccc,dddddddd. d " +
                                         "eeeeeeee   ffffffff. gggggggg!hhhhhhhh"))
                        }
                        with_window_drawlist() $(var dl) {
                            dl |> add_rect(GetItemRectMin(), GetItemRectMax(),
                                           rgba(255u, 255u, 0u, 255u))
                            dl |> add_rect_filled(marker_min, marker_max,
                                                  rgba(255u, 0u, 255u, 255u))
                        }
                    }
                }
            }

            tree_node(WIDGETS_TEXT_UTF8,
                (text = "UTF-8 Text", flags = ImGuiTreeNodeFlags.None)) {
                text_wrapped((text = "CJK text will only appear if the font was loaded with the " +
                    "appropriate CJK character ranges. Call io.Fonts->" +
                    "AddFontFromFileTTF() manually to load extra character ranges. " +
                    "Read docs/FONTS.md for details."))
                // daslang source files are UTF-8 so the literals are stored as-is.
                text("Hiragana: かきくけこ (kakikukeko)")
                text("Kanjis: 日本語 (nihongo)")
                input_text(WIDGETS_TEXT_UTF8_INPUT,
                    (text = "UTF-8 input", init = "日本語"))
            }
    }
}


// =============================================================================
// Images — cpp:1194-1276
// =============================================================================

def private ImagesSection() {
    tree_node(WIDGETS_IMAGES_SECTION,
        (text = "Images", flags = ImGuiTreeNodeFlags.None)) {

        let io & = unsafe(GetIO())
        text_wrapped((text = "Below we are displaying the font texture (which is the only texture " +
            "we have access to in this demo). Use the 'ImTextureID' type as " +
            "storage to pass pointers or identifier to your own texture data. " +
            "Hover the texture for a zoomed view!"))

        // Read the atlas size from TexData (ImTextureData) and pass io.Fonts.TexRef (ImTextureRef) directly to image().
        let my_tex_w = float(io.Fonts.TexData.Width)
        let my_tex_h = float(io.Fonts.TexData.Height)

        checkbox(WIDGETS_IMAGES_USE_TINT, (text = "Use Text Color for Tint"))
        text("{int(my_tex_w)}x{int(my_tex_h)}")
        let pos = GetCursorScreenPos()
        let uv_min = float2(0.0f, 0.0f)
        let uv_max = float2(1.0f, 1.0f)
        let tint_col = WIDGETS_IMAGES_USE_TINT.value \
            ? GetStyleColorVec4(ImGuiCol.Text) : float4(1.0f, 1.0f, 1.0f, 1.0f)
        let border_col = GetStyleColorVec4(ImGuiCol.Border)
        image((user_texture_id = io.Fonts.TexRef,
               size = float2(my_tex_w, my_tex_h),
               uv0 = uv_min, uv1 = uv_max,
               tint_col = tint_col, border_col = border_col))
        // Hover-to-zoom tooltip — fancy contents, so item_tooltip container.
        item_tooltip(WIDGETS_IMAGES_ZOOM_TIP) {
            let region_sz = 32.0f
            var region_x = io.MousePos.x - pos.x - region_sz * 0.5f
            var region_y = io.MousePos.y - pos.y - region_sz * 0.5f
            let zoom = 4.0f
            if (region_x < 0.0f) {
                region_x = 0.0f
            } elif (region_x > my_tex_w - region_sz) {
                region_x = my_tex_w - region_sz
            }
            if (region_y < 0.0f) {
                region_y = 0.0f
            } elif (region_y > my_tex_h - region_sz) {
                region_y = my_tex_h - region_sz
            }
            text("Min: ({region_x}, {region_y})")
            text("Max: ({region_x + region_sz}, {region_y + region_sz})")
            let uv0 = float2(region_x / my_tex_w, region_y / my_tex_h)
            let uv1 = float2((region_x + region_sz) / my_tex_w,
                             (region_y + region_sz) / my_tex_h)
            image((user_texture_id = io.Fonts.TexRef,
                   size = float2(region_sz * zoom, region_sz * zoom),
                   uv0 = uv0, uv1 = uv1,
                   tint_col = tint_col, border_col = border_col))
        }

        text_wrapped("And now some textured buttons..")
        for (i in range(8)) {
            // Per-iteration FramePadding push for i>0. `with_style` scopes the var.
            if (i > 0) {
                with_style((ImGuiStyleVar.FramePadding,
                            float2(float(i) - 1.0f, float(i) - 1.0f))) {
                    ImageBtnAt(i, io.Fonts.TexRef, my_tex_w, my_tex_h)
                }
            } else {
                ImageBtnAt(i, io.Fonts.TexRef, my_tex_w, my_tex_h)
            }
            same_line(WIDGETS_LOOP_IMG_SL[i])
        }
        new_line(WIDGETS_NL_1)
        text("Pressed {WIDGETS_IMAGES_PRESSED_COUNT} times.")
    }
}

// ImageButton helper for the 8-button loop. Wrapped in its own function so
// `with_style` can wrap a single call cleanly without an if/else split.
def private ImageBtnAt(i : int; tex_id : ImTextureRef; tex_w, tex_h : float) {
    let size = float2(32.0f, 32.0f)
    let uv0 = float2(0.0f, 0.0f)
    let uv1 = float2(32.0f / tex_w, 32.0f / tex_h)
    let bg_col = float4(0.0f, 0.0f, 0.0f, 1.0f)
    let btn_tint = float4(1.0f, 1.0f, 1.0f, 1.0f)
    if (image_button(WIDGETS_IMAGES_BTN[i],
        (text = "", user_texture_id = tex_id, size = size,
         uv0 = uv0, uv1 = uv1, bg_col = bg_col, tint_col = btn_tint))) {
        WIDGETS_IMAGES_PRESSED_COUNT++
    }
}


// =============================================================================
// Combo — cpp:1279-1344
// =============================================================================

def private ComboSection() {
    tree_node(WIDGETS_COMBO_SECTION,
        (text = "Combo", flags = ImGuiTreeNodeFlags.None)) {

        text("Combo Boxes are also called \"Dropdown\" in other systems.")

        edit_checkbox_flags(safe_addr(WIDGETS_COMBO_FLAGS),
            (id = "WIDGETS_COMBO_F_POPUP_LEFT",
             text = "ImGuiComboFlags_PopupAlignLeft",
             flags_value = int(ImGuiComboFlags.PopupAlignLeft)))
        same_line(WIDGETS_COMBO_SL_1)
        text_disabled("(?)")
        item_tooltip(WIDGETS_COMBO_HM_LEFT) {
            with_text_wrap_pos(GetFontSize() * 35.0f) {
                text_unformatted("Only makes a difference if the popup is larger than the combo.")
            }
        }

        // NoArrowButton/NoPreview/WidthFitPreview are mutually exclusive; clearing the incompatible flag matches cpp:1287-1291.
        if (edit_checkbox_flags(safe_addr(WIDGETS_COMBO_FLAGS),
            (id = "WIDGETS_COMBO_F_NO_ARROW",
             text = "ImGuiComboFlags_NoArrowButton",
             flags_value = int(ImGuiComboFlags.NoArrowButton)))) {
            WIDGETS_COMBO_FLAGS &= ~int(ImGuiComboFlags.NoPreview)
        }
        if (edit_checkbox_flags(safe_addr(WIDGETS_COMBO_FLAGS),
            (id = "WIDGETS_COMBO_F_NO_PREVIEW",
             text = "ImGuiComboFlags_NoPreview",
             flags_value = int(ImGuiComboFlags.NoPreview)))) {
            WIDGETS_COMBO_FLAGS &= ~int(ImGuiComboFlags.NoArrowButton |
                                        ImGuiComboFlags.WidthFitPreview)
        }
        if (edit_checkbox_flags(safe_addr(WIDGETS_COMBO_FLAGS),
            (id = "WIDGETS_COMBO_F_WIDTH_FIT",
             text = "ImGuiComboFlags_WidthFitPreview",
             flags_value = int(ImGuiComboFlags.WidthFitPreview)))) {
            WIDGETS_COMBO_FLAGS &= ~int(ImGuiComboFlags.NoPreview)
        }

        // Height presets — clearing the rest of HeightMask_ matches cpp:1295-1299.
        let HEIGHT_MASK = int(ImGuiComboFlags.HeightMask_)
        if (edit_checkbox_flags(safe_addr(WIDGETS_COMBO_FLAGS),
            (id = "WIDGETS_COMBO_F_HEIGHT_S",
             text = "ImGuiComboFlags_HeightSmall",
             flags_value = int(ImGuiComboFlags.HeightSmall)))) {
            WIDGETS_COMBO_FLAGS &= ~(HEIGHT_MASK & ~int(ImGuiComboFlags.HeightSmall))
        }
        if (edit_checkbox_flags(safe_addr(WIDGETS_COMBO_FLAGS),
            (id = "WIDGETS_COMBO_F_HEIGHT_R",
             text = "ImGuiComboFlags_HeightRegular",
             flags_value = int(ImGuiComboFlags.HeightRegular)))) {
            WIDGETS_COMBO_FLAGS &= ~(HEIGHT_MASK & ~int(ImGuiComboFlags.HeightRegular))
        }
        if (edit_checkbox_flags(safe_addr(WIDGETS_COMBO_FLAGS),
            (id = "WIDGETS_COMBO_F_HEIGHT_L",
             text = "ImGuiComboFlags_HeightLargest",
             flags_value = int(ImGuiComboFlags.HeightLargest)))) {
            WIDGETS_COMBO_FLAGS &= ~(HEIGHT_MASK & ~int(ImGuiComboFlags.HeightLargest))
        }

        // combo 1 — BeginCombo + manual Selectable loop via combo_select; preview reads WIDGETS_COMBO_C1.value (index into shared items).
        let combo_flags = unsafe(reinterpret<ImGuiComboFlags>(WIDGETS_COMBO_FLAGS))
        let preview_idx = WIDGETS_COMBO_C1.value
        let preview = ((preview_idx >= 0 && preview_idx < length(WIDGETS_COMBO_ITEMS))
            ? WIDGETS_COMBO_ITEMS[preview_idx] : "")
        combo_select(WIDGETS_COMBO_C1,
            (text = "combo 1", preview_value = preview, flags = combo_flags)) {
            for (n in range(length(WIDGETS_COMBO_ITEMS))) {
                let is_selected = (WIDGETS_COMBO_C1.value == n)
                if (selectable_label(WIDGETS_COMBO_LB_ROW[n],
                                     WIDGETS_COMBO_ITEMS[n], is_selected)) {
                    WIDGETS_COMBO_C1.value = n
                }
                if (is_selected) {
                    SetItemDefaultFocus()
                }
            }
        }

        spacing(WIDGETS_COMBO_SPACING)
        separator_text(WIDGETS_COMBO_SEP, "One-liner variants")
        text_disabled("(?)")
        item_tooltip(WIDGETS_COMBO_HM_ONELINE) {
            with_text_wrap_pos(GetFontSize() * 35.0f) {
                text_unformatted("Flags above don't apply to this section.")
            }
        }

        // combo 2 one-liner: boost combo takes array<string> and packs the same null-separated buffer cpp uses; module-scope const items avoid per-frame churn.
        combo(WIDGETS_COMBO_C2,
            (text = "combo 2 (one-liner)",
             items <- WIDGETS_COMBO_ONELINER_ITEMS))

        // combo 3 array form over the full 14-item list; daslang state defaults to 0, so we skip cpp's -1 no-initial-preview trick.
        combo(WIDGETS_COMBO_C3,
            (text = "combo 3 (array)", items <- WIDGETS_COMBO_ITEMS))

        // combo 4 — getter form. The thunk indexes into the shared items list.
        combo_getter(WIDGETS_COMBO_C4,
            (text = "combo 4 (function)",
             items_count = length(WIDGETS_COMBO_ITEMS),
             getter = @(idx : int; var result : string&) : bool {
                if (idx >= 0 && idx < length(WIDGETS_COMBO_ITEMS)) {
                    result = WIDGETS_COMBO_ITEMS[idx]
                    return true
                }
                return false
             }))
    }
}


// =============================================================================
// List boxes — cpp:1347-1393
// =============================================================================

def private ListBoxesSection() {
    tree_node(WIDGETS_LB_SECTION,
        (text = "List boxes", flags = ImGuiTreeNodeFlags.None)) {

        // Two list_box instances share one selection state (cpp:1358, both read/write WIDGETS_LB_SHARED.value); the second overrides size to full width, 5 rows tall.
        list_box(WIDGETS_LB_SHARED,
            (text = "listbox 1", size = float2(0.0f, 0.0f))) {
            for (n in range(length(WIDGETS_COMBO_ITEMS))) {
                let is_selected = (WIDGETS_LB_SHARED.value == n)
                if (selectable_label(WIDGETS_LB_ROW1[n],
                                     WIDGETS_COMBO_ITEMS[n], is_selected)) {
                    WIDGETS_LB_SHARED.value = n
                }
                if (is_selected) {
                    SetItemDefaultFocus()
                }
            }
        }
        same_line(WIDGETS_LB_SL_HINT)
        text_disabled("(?)")
        item_tooltip(WIDGETS_LB_HM_SHARE) {
            with_text_wrap_pos(GetFontSize() * 35.0f) {
                text_unformatted("Here we are sharing selection state between both boxes.")
            }
        }

        text("Full-width:")
        let full_h = 5.0f * GetTextLineHeightWithSpacing()
        list_box(WIDGETS_LB_FULL,
            (text = "##listbox 2", size = float2(-1.0f, full_h))) {
            for (n in range(length(WIDGETS_COMBO_ITEMS))) {
                let is_selected = (WIDGETS_LB_SHARED.value == n)
                if (selectable_label(WIDGETS_LB_ROW2[n],
                                     WIDGETS_COMBO_ITEMS[n], is_selected)) {
                    WIDGETS_LB_SHARED.value = n
                }
                if (is_selected) {
                    SetItemDefaultFocus()
                }
            }
        }
    }
}


// =============================================================================
// Plotting — cpp:1898-1958
// =============================================================================

def private PlottingSection() {
    tree_node(WIDGETS_PLOT_SECTION,
        (text = "Plotting", flags = ImGuiTreeNodeFlags.None)) {

        checkbox(WIDGETS_PLOT_ANIMATE, (text = "Animate", init = true))

        // Frame-times array (7 values) — plot as lines AND histogram.
        plot_lines(WIDGETS_PLOT_FRAMETIMES_LINES,
            (text = "Frame Times", values <- WIDGETS_PLOT_FRAMETIMES))
        plot_histogram(WIDGETS_PLOT_FRAMETIMES_HIST,
            (text = "Histogram", values <- WIDGETS_PLOT_FRAMETIMES,
             scale_min = 0.0f, scale_max = 1.0f,
             graph_size = float2(0.0f, 80.0f)))

        // 90-sample rolling cosine buffer animated at a fixed 60 Hz to decouple from the recorder/host frame rate (cpp:1912-1924).
        if (length(WIDGETS_PLOT_VALUES) != 90) {
            WIDGETS_PLOT_VALUES |> resize(90)
        }
        let now = GetTime()
        if (!WIDGETS_PLOT_ANIMATE.value || WIDGETS_PLOT_REFRESH_TIME == 0.0lf) {
            WIDGETS_PLOT_REFRESH_TIME = now
        }
        while (WIDGETS_PLOT_REFRESH_TIME < now) {
            WIDGETS_PLOT_VALUES[WIDGETS_PLOT_VALUES_OFFSET] = cos(WIDGETS_PLOT_PHASE)
            WIDGETS_PLOT_VALUES_OFFSET = (WIDGETS_PLOT_VALUES_OFFSET + 1) % 90
            WIDGETS_PLOT_PHASE += 0.10f * float(WIDGETS_PLOT_VALUES_OFFSET)
            WIDGETS_PLOT_REFRESH_TIME += 1.0lf / 60.0lf
        }

        // Average overlay — cpp:1929-1935.
        var average = 0.0f
        for (v in WIDGETS_PLOT_VALUES) {
            average += v
        }
        average /= float(length(WIDGETS_PLOT_VALUES))
        plot_lines(WIDGETS_PLOT_LINES,
            (text = "Lines", values <- WIDGETS_PLOT_VALUES,
             scale_min = -1.0f, scale_max = 1.0f,
             graph_size = float2(0.0f, 80.0f)))
        // plot_lines takes the array as-is with no values_offset rotation (cpp skips the head via the offset; our wrapper starts at 0) — only the rolling-head alignment differs.
        text("avg {average}")

        // Sin / Saw lambda-driven plots via *_getter (cpp:1941-1954).
        separator_text(WIDGETS_PLOT_SEP_FUNC, "Functions")
        with_item_width(GetFontSize() * 8.0f) {
            combo(WIDGETS_PLOT_FUNC,
                (text = "func", items <- WIDGETS_PLOT_FUNC_ITEMS))
        }
        same_line(WIDGETS_PLOT_SL_FUNC)
        slider_int(WIDGETS_PLOT_SAMPLE_COUNT,
            (text = "Sample count", init = 70, bounds = (1, 400)))

        let is_sin = (WIDGETS_PLOT_FUNC.value == 0)
        let count = WIDGETS_PLOT_SAMPLE_COUNT.value
        plot_lines_getter(WIDGETS_PLOT_LINES_FN,
            (text = "Lines", values_count = count,
             getter = @(idx : int) : float => is_sin
                ? sin(float(idx) * 0.1f)
                : (((idx & 1) != 0) ? 1.0f : -1.0f),
             overlay_text = "",
             scale_min = -1.0f, scale_max = 1.0f,
             graph_size = float2(0.0f, 80.0f)))
        plot_histogram_getter(WIDGETS_PLOT_HIST_FN,
            (text = "Histogram", values_count = count,
             getter = @(idx : int) : float => is_sin
                ? sin(float(idx) * 0.1f)
                : (((idx & 1) != 0) ? 1.0f : -1.0f),
             overlay_text = "",
             scale_min = -1.0f, scale_max = 1.0f,
             graph_size = float2(0.0f, 80.0f)))
        separator(WIDGETS_PLOT_SEP)
    }
}


// =============================================================================
// Progress Bars — cpp:1961-1987
// =============================================================================

def private ProgressBarsSection() {
    tree_node(WIDGETS_PB_SECTION,
        (text = "Progress Bars", flags = ImGuiTreeNodeFlags.None)) {

        // Ping-pong progress in [-0.1, 1.1] driven by frame delta-time.
        let io & = unsafe(GetIO())
        WIDGETS_PROGRESS += WIDGETS_PROGRESS_DIR * 0.4f * io.DeltaTime
        if (WIDGETS_PROGRESS >= 1.1f) {
            WIDGETS_PROGRESS = 1.1f
            WIDGETS_PROGRESS_DIR *= -1.0f
        }
        if (WIDGETS_PROGRESS <= -0.1f) {
            WIDGETS_PROGRESS = -0.1f
            WIDGETS_PROGRESS_DIR *= -1.0f
        }

        progress_bar(WIDGETS_PB_DEFAULT,
            (fraction = WIDGETS_PROGRESS, size = float2(0.0f, 0.0f)))
        same_line((offset_from_start_x = 0.0f,
                   spacing = GetStyle().ItemInnerSpacing.x))
        text("Progress Bar")

        let saturated = clamp(WIDGETS_PROGRESS, 0.0f, 1.0f)
        let units = int(saturated * 1753.0f)
        progress_bar(WIDGETS_PB_OVERLAY,
            (fraction = WIDGETS_PROGRESS, size = float2(0.0f, 0.0f),
             overlay = "{units}/1753"))

        // Negative fraction renders an indeterminate animation; -GetTime() pumps a decreasing value so ImGui advances its phase.
        progress_bar(WIDGETS_PB_INDETERMINATE,
            (fraction = -1.0f * float(GetTime()),
             size = float2(0.0f, 0.0f),
             overlay = "Searching.."))
        same_line((offset_from_start_x = 0.0f,
                   spacing = GetStyle().ItemInnerSpacing.x))
        text("Indeterminate")
    }
}


// =============================================================================
// Range Widgets — cpp:2217-2225
// =============================================================================

def private RangeWidgetsSection() {
    tree_node(WIDGETS_RANGE_SECTION,
        (text = "Range Widgets", flags = ImGuiTreeNodeFlags.None)) {

        drag_float_range2(WIDGETS_RANGE_F,
            (text = "range float",
             init = float2(10.0f, 90.0f),
             bounds = (0.0f, 100.0f),
             speed = 0.25f,
             format = "%.1f %%",
             flags = ImGuiSliderFlags.AlwaysClamp))
        // cpp uses per-handle formats (Min:/Max:); our binding takes one shared format string — a label-prefix binding gap, not a behavior gap.
        drag_int_range2(WIDGETS_RANGE_I,
            (text = "range int",
             init = int2(100, 1000),
             bounds = (0, 1000),
             speed = 5.0f,
             format = "%d units"))
        drag_int_range2(WIDGETS_RANGE_I_NB,
            (text = "range int (no bounds)",
             init = int2(100, 1000),
             bounds = (0, 0),
             speed = 5.0f,
             format = "%d units"))
    }
}


// =============================================================================
// Multi-component Widgets — cpp:2352-2382
// =============================================================================

// cpp shares one vec4f[4]/vec4i[4] across all 18 widgets; our boost rail auto-emits
// independent state per widget, so values don't propagate between widgets (initial
// values seeded from the cpp tuples for visual parity).

def private MultiComponentSection() {
    tree_node(WIDGETS_MC_SECTION,
        (text = "Multi-component Widgets", flags = ImGuiTreeNodeFlags.None)) {

        separator_text(WIDGETS_MC_SEP_2, "2-wide")
        input_float2(WIDGETS_MC_IF2, (text = "input float2",
                                      init = float2(0.10f, 0.20f)))
        drag_float2(WIDGETS_MC_DF2, (text = "drag float2",
                                     init = float2(0.10f, 0.20f),
                                     bounds = (0.0f, 1.0f), speed = 0.01f))
        slider_float2(WIDGETS_MC_SF2, (text = "slider float2",
                                       init = float2(0.10f, 0.20f),
                                       bounds = (0.0f, 1.0f)))
        input_int2(WIDGETS_MC_II2, (text = "input int2",
                                    init = int2(1, 5)))
        drag_int2(WIDGETS_MC_DI2, (text = "drag int2",
                                   init = int2(1, 5),
                                   bounds = (0, 255), speed = 1.0f))
        slider_int2(WIDGETS_MC_SI2, (text = "slider int2",
                                     init = int2(1, 5),
                                     bounds = (0, 255)))

        separator_text(WIDGETS_MC_SEP_3, "3-wide")
        input_float3(WIDGETS_MC_IF3, (text = "input float3",
                                      init = float3(0.10f, 0.20f, 0.30f)))
        drag_float3(WIDGETS_MC_DF3, (text = "drag float3",
                                     init = float3(0.10f, 0.20f, 0.30f),
                                     bounds = (0.0f, 1.0f), speed = 0.01f))
        slider_float3(WIDGETS_MC_SF3, (text = "slider float3",
                                       init = float3(0.10f, 0.20f, 0.30f),
                                       bounds = (0.0f, 1.0f)))
        input_int3(WIDGETS_MC_II3, (text = "input int3",
                                    init = int3(1, 5, 100)))
        drag_int3(WIDGETS_MC_DI3, (text = "drag int3",
                                   init = int3(1, 5, 100),
                                   bounds = (0, 255), speed = 1.0f))
        slider_int3(WIDGETS_MC_SI3, (text = "slider int3",
                                     init = int3(1, 5, 100),
                                     bounds = (0, 255)))

        separator_text(WIDGETS_MC_SEP_4, "4-wide")
        input_float4(WIDGETS_MC_IF4, (text = "input float4",
                                      init = float4(0.10f, 0.20f, 0.30f, 0.44f)))
        drag_float4(WIDGETS_MC_DF4, (text = "drag float4",
                                     init = float4(0.10f, 0.20f, 0.30f, 0.44f),
                                     bounds = (0.0f, 1.0f), speed = 0.01f))
        slider_float4(WIDGETS_MC_SF4, (text = "slider float4",
                                       init = float4(0.10f, 0.20f, 0.30f, 0.44f),
                                       bounds = (0.0f, 1.0f)))
        input_int4(WIDGETS_MC_II4, (text = "input int4",
                                    init = int4(1, 5, 100, 255)))
        drag_int4(WIDGETS_MC_DI4, (text = "drag int4",
                                   init = int4(1, 5, 100, 255),
                                   bounds = (0, 255), speed = 1.0f))
        slider_int4(WIDGETS_MC_SI4, (text = "slider int4",
                                     init = int4(1, 5, 100, 255),
                                     bounds = (0, 255)))
    }
}


// =============================================================================
// Vertical Sliders — cpp:2385-2447
// =============================================================================

def private VerticalSlidersSection() {
    tree_node(WIDGETS_VS_SECTION,
        (text = "Vertical Sliders", flags = ImGuiTreeNodeFlags.None)) {

        let spacing_px = 4.0f
        with_style((ImGuiStyleVar.ItemSpacing, float2(spacing_px, spacing_px))) {
            // Single VSliderInt + a row of 7 colored VSliderFloats sharing WIDGETS_VS_VALUES (cpp:2390-2410); edit_vslider_* binds the caller-owned slot, with_id carves the per-iter ID scope.
            edit_vslider_int(safe_addr(WIDGETS_VS_INT_PTR),
                (id = "WIDGETS_VS_INT", text = "##int",
                 size = float2(18.0f, 160.0f), v_min = 0, v_max = 5))
            same_line(WIDGETS_VS_SL_AFTER_INT)

            with_id("set1") {
                for (i in range(7)) {
                    if (i > 0) {
                        same_line(WIDGETS_VS_SET1[i])
                    }
                    with_id("i_{i}") {
                        let hue = float(i) / 7.0f
                        with_style((ImGuiCol.FrameBg,        HSV(hue, 0.5f, 0.5f)),
                                   (ImGuiCol.FrameBgHovered, HSV(hue, 0.6f, 0.5f)),
                                   (ImGuiCol.FrameBgActive,  HSV(hue, 0.7f, 0.5f)),
                                   (ImGuiCol.SliderGrab,     HSV(hue, 0.9f, 0.9f))) {
                            VSlider1At(i)
                        }
                    }
                }
            }
            same_line(WIDGETS_VS_SL_SET1_END)

            // 4×3 grid of small vertical sliders (cpp:2414-2430); each column shares one float (values2[nx]) across 3 rows.
            with_id("set2") {
                let rows = 3
                let row_h = (160.0f - float(rows - 1) * spacing_px) / float(rows)
                let small_size = float2(18.0f, row_h)
                for (nx in range(4)) {
                    if (nx > 0) {
                        same_line(WIDGETS_VS_SET2[nx])
                    }
                    group(WIDGETS_VS_GROUP[nx]) {
                        with_id("col_{nx}") {
                            for (ny in range(rows)) {
                                with_id("row_{ny}") {
                                    VSlider2At(nx, small_size)
                                }
                            }
                        }
                    }
                }
            }
            same_line(WIDGETS_VS_SL_SET2_END)

            // Row of 4 fat-grab sliders reusing WIDGETS_VS_VALUES[0..3] (cpp:2434-2444); GrabMinSize style var scoped per slider.
            with_id("set3") {
                for (i in range(4)) {
                    if (i > 0) {
                        same_line(WIDGETS_VS_SET3[i])
                    }
                    with_id("i_{i}") {
                        with_style((ImGuiStyleVar.GrabMinSize, 40.0f)) {
                            VSlider3At(i)
                        }
                    }
                }
            }
        }
    }
}

// Per-iteration VSliderFloat helpers — wrapped so `with_style` can scope a
// single statement cleanly. All three bind to slices of the shared arrays
// via edit_vslider_float; tooltip-on-active mirrors cpp:2405-2406 (2425-26).
def private VSlider1At(i : int) {
    edit_vslider_float(unsafe(addr(WIDGETS_VS_VALUES[i])),
        (id = "WIDGETS_VS_SLIDER1", text = "##v",
         size = float2(18.0f, 160.0f),
         v_min = 0.0f, v_max = 1.0f, format = ""))
    if (IsItemActive() || IsItemHovered()) {
        set_tooltip(WIDGETS_VS_TIP1[i], "{WIDGETS_VS_VALUES[i]}")
    }
}

def private VSlider2At(nx : int; size : float2) {
    edit_vslider_float(unsafe(addr(WIDGETS_VS_VALUES2[nx])),
        (id = "WIDGETS_VS_SLIDER2", text = "##v",
         size = size, v_min = 0.0f, v_max = 1.0f, format = ""))
    if (IsItemActive() || IsItemHovered()) {
        set_tooltip(WIDGETS_VS_TIP2[nx], "{WIDGETS_VS_VALUES2[nx]}")
    }
}

def private VSlider3At(i : int) {
    edit_vslider_float(unsafe(addr(WIDGETS_VS_VALUES[i])),
        (id = "WIDGETS_VS_SLIDER3", text = "##v",
         size = float2(40.0f, 160.0f),
         v_min = 0.0f, v_max = 1.0f, format = "%.2f\nsec"))
}


// =============================================================================
// Disable block — cpp:2807-2813
// =============================================================================

// The checkbox toggles WIDGETS_DISABLE_ALL (the var gating with_disabled); it lives
// OUTSIDE with_disabled so it stays interactive when everything else is disabled.

def private DisableBlockSection() {
    tree_node(WIDGETS_DIS_SECTION,
        (text = "Disable block", flags = ImGuiTreeNodeFlags.None)) {

        edit_checkbox(safe_addr(WIDGETS_DISABLE_ALL),
            (id = "WIDGETS_DIS_CB", text = "Disable entire section above"))
        same_line(WIDGETS_DIS_SL)
        text_disabled("(?)")
        item_tooltip(WIDGETS_DIS_HM) {
            with_text_wrap_pos(GetFontSize() * 35.0f) {
                text_unformatted("Demonstrate using BeginDisabled()/EndDisabled() across this section.")
            }
        }
    }
}


// =============================================================================
// Text Filter — cpp:2816-2832
// =============================================================================

def private TextFilterSection() {
    tree_node(WIDGETS_TF_SECTION,
        (text = "Text Filter", flags = ImGuiTreeNodeFlags.None)) {

        text_disabled("(?)")
        item_tooltip(WIDGETS_TF_HM) {
            with_text_wrap_pos(GetFontSize() * 35.0f) {
                text_unformatted("Not a widget per se, but ImGuiTextFilter is a helper to perform simple filtering on text strings.")
            }
        }

        text_unformatted((text = "Filter usage:\n" +
            "  \"\"         display all lines\n" +
            "  \"xxx\"      display lines containing \"xxx\"\n" +
            "  \"xxx,yyy\"  display lines containing \"xxx\" or \"yyy\"\n" +
            "  \"-xxx\"     hide lines containing \"xxx\""))

        text_filter(WIDGETS_TF_FILTER,
            (text = "Filter (\"incl,-excl\") (\"error\")"))

        // Bullet each line that survives the filter; lines are module-scope const (WIDGETS_TF_LINES) to avoid a per-frame heap alloc.
        for (i in range(length(WIDGETS_TF_LINES))) {
            if (passes_filter(WIDGETS_TF_FILTER, WIDGETS_TF_LINES[i])) {
                bullet_text(WIDGETS_TF_LINE[i], (text = WIDGETS_TF_LINES[i]))
            }
        }
    }
}


// =============================================================================
// Tabs — cpp:1752-1891
// =============================================================================

// Three sub-trees: Basic (static tab_items), Advanced & Close (flag row + closable
// tabs with checkbox↔tab.open binding), Leading/Trailing (dynamic tab list, "+" add).

def private TabsSection() {
    tree_node(WIDGETS_TABS_SECTION,
        (text = "Tabs", flags = ImGuiTreeNodeFlags.None)) {
        TabsBasicSection()
        TabsAdvancedSection()
        TabsLeadingTrailingSection()
    }
}

def private TabsBasicSection() {
    tree_node(WIDGETS_TABS_BASIC,
        (text = "Basic", flags = ImGuiTreeNodeFlags.None)) {
        tab_bar(WIDGETS_TABS_B_BAR,
            (text = "MyTabBar", flags = ImGuiTabBarFlags.None)) {
            tab_item(WIDGETS_TABS_B_AVOCADO,
                (text = "Avocado", closable = false,
                 flags = ImGuiTabItemFlags.None)) {
                text(WIDGETS_TABS_B_AV_TEXT,
                    (text = "This is the Avocado tab!\nblah blah blah blah blah"))
            }
            tab_item(WIDGETS_TABS_B_BROCCOLI,
                (text = "Broccoli", closable = false,
                 flags = ImGuiTabItemFlags.None)) {
                text(WIDGETS_TABS_B_BR_TEXT,
                    (text = "This is the Broccoli tab!\nblah blah blah blah blah"))
            }
            tab_item(WIDGETS_TABS_B_CUCUMBER,
                (text = "Cucumber", closable = false,
                 flags = ImGuiTabItemFlags.None)) {
                text(WIDGETS_TABS_B_CU_TEXT,
                    (text = "This is the Cucumber tab!\nblah blah blah blah blah"))
            }
        }
        separator(WIDGETS_TABS_B_SEP)
    }
}

def private TabsAdvancedSection() {
    tree_node(WIDGETS_TABS_ADV,
        (text = "Advanced & Close Button", flags = ImGuiTreeNodeFlags.None)) {
        // 4 plain flag-bit toggles.
        edit_checkbox_flags(safe_addr(WIDGETS_TABS_ADV_FLAGS),
            (id = "WIDGETS_TABS_ADV_F_REORDER",
             text = "ImGuiTabBarFlags_Reorderable",
             flags_value = int(ImGuiTabBarFlags.Reorderable)))
        edit_checkbox_flags(safe_addr(WIDGETS_TABS_ADV_FLAGS),
            (id = "WIDGETS_TABS_ADV_F_AUTO",
             text = "ImGuiTabBarFlags_AutoSelectNewTabs",
             flags_value = int(ImGuiTabBarFlags.AutoSelectNewTabs)))
        edit_checkbox_flags(safe_addr(WIDGETS_TABS_ADV_FLAGS),
            (id = "WIDGETS_TABS_ADV_F_POPUP",
             text = "ImGuiTabBarFlags_TabListPopupButton",
             flags_value = int(ImGuiTabBarFlags.TabListPopupButton)))
        edit_checkbox_flags(safe_addr(WIDGETS_TABS_ADV_FLAGS),
            (id = "WIDGETS_TABS_ADV_F_NOMID",
             text = "ImGuiTabBarFlags_NoCloseWithMiddleMouseButton",
             flags_value = int(ImGuiTabBarFlags.NoCloseWithMiddleMouseButton)))
        // Fitting-policy bits are mutually exclusive; default to ResizeDown if none is set (cpp:1790-1791) so the mask stays valid.
        let POLICY_MASK = int(ImGuiTabBarFlags.FittingPolicyMask_)
        if ((WIDGETS_TABS_ADV_FLAGS & POLICY_MASK) == 0) {
            WIDGETS_TABS_ADV_FLAGS |= int(ImGuiTabBarFlags.FittingPolicyDefault_)
        }
        if (edit_checkbox_flags(safe_addr(WIDGETS_TABS_ADV_FLAGS),
            (id = "WIDGETS_TABS_ADV_F_FIT_RESIZE",
             text = "ImGuiTabBarFlags_FittingPolicyShrink",
             flags_value = int(ImGuiTabBarFlags.FittingPolicyShrink)))) {
            WIDGETS_TABS_ADV_FLAGS &= ~(POLICY_MASK ^
                                        int(ImGuiTabBarFlags.FittingPolicyShrink))
        }
        if (edit_checkbox_flags(safe_addr(WIDGETS_TABS_ADV_FLAGS),
            (id = "WIDGETS_TABS_ADV_F_FIT_SCROLL",
             text = "ImGuiTabBarFlags_FittingPolicyScroll",
             flags_value = int(ImGuiTabBarFlags.FittingPolicyScroll)))) {
            WIDGETS_TABS_ADV_FLAGS &= ~(POLICY_MASK ^
                                        int(ImGuiTabBarFlags.FittingPolicyScroll))
        }

        // Checkbox row and the edit_tab_item X-button both write WIDGETS_TABS_ADV_OPENED[n] — two UI surfaces, one caller-owned bool[] (cpp's `bool opened[4]`).
        for (n in range(length(WIDGETS_TABS_ADV_NAMES))) {
            if (n > 0) {
                same_line(WIDGETS_TABS_ADV_TOGGLE[n])
            }
            with_id("adv_cb_{n}") {
                edit_checkbox(unsafe(addr(WIDGETS_TABS_ADV_OPENED[n])),
                    (id = "WIDGETS_TABS_ADV_CB", text = WIDGETS_TABS_ADV_NAMES[n]))
            }
        }

        tab_bar(WIDGETS_TABS_ADV_BAR,
            (text = "MyTabBar",
             flags = unsafe(reinterpret<ImGuiTabBarFlags>(WIDGETS_TABS_ADV_FLAGS)))) {
            for (n in range(length(WIDGETS_TABS_ADV_NAMES))) {
                with_id("adv_tab_{n}") {
                    edit_tab_item(unsafe(addr(WIDGETS_TABS_ADV_OPENED[n])),
                        (id = "WIDGETS_TABS_ADV_TAB",
                         text = WIDGETS_TABS_ADV_NAMES[n],
                         flags = ImGuiTabItemFlags.None)) {
                        text(WIDGETS_TABS_ADV_BODY[n],
                            (text = "This is the {WIDGETS_TABS_ADV_NAMES[n]} tab!"))
                        // cpp adds an "odd tab" hint for n & 1 — dropped to keep the per-iteration widget count low.
                    }
                }
            }
        }
        separator(WIDGETS_TABS_ADV_SEP)
    }
}

def private TabsLeadingTrailingSection() {
    tree_node(WIDGETS_TABS_LT,
        (text = "TabItemButton & Leading/Trailing flags",
         flags = ImGuiTreeNodeFlags.None)) {

        // Seed 3 tabs on first frame (cpp:1828-1831); NEXT_ID==0 is the sentinel, so once incremented the seed never re-fires.
        if (WIDGETS_TABS_LT_NEXT_ID == 0) {
            WIDGETS_TABS_LT_ACTIVE |> reserve(3)
            for (_ in range(3)) {
                WIDGETS_TABS_LT_ACTIVE |> push(WIDGETS_TABS_LT_NEXT_ID)
                WIDGETS_TABS_LT_NEXT_ID++
            }
        }

        checkbox(WIDGETS_TABS_LT_SHOW_LEAD,
            (text = "Show Leading TabItemButton()", init = true))
        checkbox(WIDGETS_TABS_LT_SHOW_TRAIL,
            (text = "Show Trailing TabItemButton()", init = true))

        edit_checkbox_flags(safe_addr(WIDGETS_TABS_LT_FLAGS),
            (id = "WIDGETS_TABS_LT_F_POPUP",
             text = "ImGuiTabBarFlags_TabListPopupButton",
             flags_value = int(ImGuiTabBarFlags.TabListPopupButton)))
        let LT_POLICY_MASK = int(ImGuiTabBarFlags.FittingPolicyMask_)
        if ((WIDGETS_TABS_LT_FLAGS & LT_POLICY_MASK) == 0) {
            WIDGETS_TABS_LT_FLAGS |= int(ImGuiTabBarFlags.FittingPolicyDefault_)
        }
        if (edit_checkbox_flags(safe_addr(WIDGETS_TABS_LT_FLAGS),
            (id = "WIDGETS_TABS_LT_F_FIT_RESIZE",
             text = "ImGuiTabBarFlags_FittingPolicyShrink",
             flags_value = int(ImGuiTabBarFlags.FittingPolicyShrink)))) {
            WIDGETS_TABS_LT_FLAGS &= ~(LT_POLICY_MASK ^
                                       int(ImGuiTabBarFlags.FittingPolicyShrink))
        }
        if (edit_checkbox_flags(safe_addr(WIDGETS_TABS_LT_FLAGS),
            (id = "WIDGETS_TABS_LT_F_FIT_SCROLL",
             text = "ImGuiTabBarFlags_FittingPolicyScroll",
             flags_value = int(ImGuiTabBarFlags.FittingPolicyScroll)))) {
            WIDGETS_TABS_LT_FLAGS &= ~(LT_POLICY_MASK ^
                                       int(ImGuiTabBarFlags.FittingPolicyScroll))
        }

        tab_bar(WIDGETS_TABS_LT_BAR,
            (text = "MyTabBar",
             flags = unsafe(reinterpret<ImGuiTabBarFlags>(WIDGETS_TABS_LT_FLAGS)))) {
            // Leading "?" opens a help popup; popup_window MUST sit OUTSIDE the if(show_leading) gate (cpp:1855) so its state machine runs even when the trigger is hidden.
            if (WIDGETS_TABS_LT_SHOW_LEAD.value) {
                if (tab_item_button(WIDGETS_TABS_LT_HELP_BTN,
                    (text = "?",
                     flags = ImGuiTabItemFlags.Leading |
                             ImGuiTabItemFlags.NoTooltip))) {
                    open_popup("MyHelpMenu")
                }
            }
            popup_window(WIDGETS_TABS_LT_HELP_POPUP,
                (str_id = "MyHelpMenu", flags = ImGuiWindowFlags.None)) {
                selectable(WIDGETS_TABS_LT_HELP_HELLO, (text = "Hello!"))
            }

            // Trailing "+" appends a new tab id; the AutoSelectNewTabs flag focuses it on creation.
            if (WIDGETS_TABS_LT_SHOW_TRAIL.value) {
                if (tab_item_button(WIDGETS_TABS_LT_ADD_BTN,
                    (text = "+",
                     flags = ImGuiTabItemFlags.Trailing |
                             ImGuiTabItemFlags.NoTooltip))) {
                    WIDGETS_TABS_LT_ACTIVE |> push(WIDGETS_TABS_LT_NEXT_ID)
                    WIDGETS_TABS_LT_NEXT_ID++
                }
            }

            // Render current tabs, dropping entries whose X was clicked; erase(n) shrinks the array, so skip n++ to visit the shifted element next.
            var n = 0
            while (n < length(WIDGETS_TABS_LT_ACTIVE)) {
                let tab_id = WIDGETS_TABS_LT_ACTIVE[n]
                if (!key_exists(WIDGETS_TABS_LT_DYN, tab_id)) {
                    WIDGETS_TABS_LT_DYN[tab_id] = TabItemState(open = true)
                }
                tab_item(WIDGETS_TABS_LT_DYN[tab_id],
                    (text = "{tab_id:04}", closable = true,
                     flags = ImGuiTabItemFlags.None)) {
                    text(WIDGETS_TABS_LT_BODY, (text = "This is tab {tab_id}!"))
                }
                if (!WIDGETS_TABS_LT_DYN[tab_id].open) {
                    WIDGETS_TABS_LT_ACTIVE |> erase(n)
                    // Leave the table entry — NEXT_ID only increments, so IDs never reuse and there's no key-collision risk.
                } else {
                    n++
                }
            }
        }
        separator(WIDGETS_TABS_LT_SEP)
    }
}


// =============================================================================
// Selectables — cpp:1396-1553
// =============================================================================

// Seven sub-trees: Basic (self-toggling), Single/Multiple Selection, Same Line
// (AllowOverlap + small_button), In columns (data_tables), Grid (4x4 toggle +
// neighbors, all-selected animates alignment), Alignment (per-cell text align).

def private SelectablesSection() {
    tree_node(WIDGETS_SEL_SECTION,
        (text = "Selectables", flags = ImGuiTreeNodeFlags.None)) {
        SelectablesBasicSection()
        SelectablesSingleSection()
        SelectablesMultiSection()
        SelectablesSameLineSection()
        SelectablesInColumnsSection()
        SelectablesGridSection()
        SelectablesAlignmentSection()
    }
}

def private SelectablesBasicSection() {
    tree_node(WIDGETS_SEL_BASIC,
        (text = "Basic", flags = ImGuiTreeNodeFlags.None)) {
        // Rows 0-2 self-toggling selectables; cpp seeds [1]=true at load but the [widget] indexed form has no init=, so [1] starts false (minor demo-fidelity loss).
        selectable(WIDGETS_SEL_BASIC_TOGGLE[0],
            (text = "1. I am selectable"))
        selectable(WIDGETS_SEL_BASIC_TOGGLE[1],
            (text = "2. I am selectable"))
        selectable(WIDGETS_SEL_BASIC_TOGGLE[2],
            (text = "3. I am selectable"))
        // Row 3: AllowDoubleClick makes Selectable return true only on the double-click frame, matching cpp's IsMouseDoubleClicked guard with no extra check.
        selectable(WIDGETS_SEL_BASIC_TOGGLE[3],
            (text = "4. I am double clickable",
             flags = ImGuiSelectableFlags.AllowDoubleClick))
    }
}

def private SelectablesSingleSection() {
    tree_node(WIDGETS_SEL_SINGLE,
        (text = "Selection State: Single Selection",
         flags = ImGuiTreeNodeFlags.None)) {
        for (n in range(5)) {
            if (selectable_label(WIDGETS_SEL_SINGLE_ROW[n],
                                 "Object {n}", WIDGETS_SEL_SINGLE_IDX == n)) {
                WIDGETS_SEL_SINGLE_IDX = n
            }
        }
    }
}

def private SelectablesMultiSection() {
    tree_node(WIDGETS_SEL_MULTI,
        (text = "Selection State: Multiple Selection",
         flags = ImGuiTreeNodeFlags.None)) {
        // Cpp wraps this with a HelpMarker; dropped since the TextDisabled+tooltip binding gap would inflate the port for no functional gain.
        for (n in range(length(WIDGETS_SEL_MULTI_SEL))) {
            if (selectable_label(WIDGETS_SEL_MULTI_ROW[n],
                                 "Object {n}", WIDGETS_SEL_MULTI_SEL[n])) {
                if (!GetIO().KeyCtrl) {
                    for (i in range(length(WIDGETS_SEL_MULTI_SEL))) {
                        WIDGETS_SEL_MULTI_SEL[i] = false
                    }
                }
                WIDGETS_SEL_MULTI_SEL[n] = !WIDGETS_SEL_MULTI_SEL[n]
            }
        }
    }
}

def private SelectablesSameLineSection() {
    tree_node(WIDGETS_SEL_SL,
        (text = "Rendering more items on the same line",
         flags = ImGuiTreeNodeFlags.None)) {
        for (n in range(length(WIDGETS_SEL_SL_LABELS))) {
            SetNextItemAllowOverlap()
            selectable(WIDGETS_SEL_SL_ITEM[n], (text = WIDGETS_SEL_SL_LABELS[n]))
            same_line(WIDGETS_SEL_SL_SL[n])
            small_button(WIDGETS_SEL_SL_LINK[n], (text = "Link {n + 1}"))
        }
    }
}

def private SelectablesInColumnsSection() {
    tree_node(WIDGETS_SEL_COL,
        (text = "In columns", flags = ImGuiTreeNodeFlags.None)) {
        let COL_FLAGS = (ImGuiTableFlags.Resizable |
                         ImGuiTableFlags.NoSavedSettings |
                         ImGuiTableFlags.Borders)
        data_table(WIDGETS_SEL_COL1,
            (text = "split1", columns = 3, flags = COL_FLAGS,
             outer_size = float2(0.0f, 0.0f), inner_width = 0.0f)) {
            for (i in range(10)) {
                table_next_column()
                selectable(WIDGETS_SEL_COL1_ROW[i], (text = "Item {i}"))
            }
        }
        spacing(WIDGETS_SEL_COL_SP)
        data_table(WIDGETS_SEL_COL2,
            (text = "split2", columns = 3, flags = COL_FLAGS,
             outer_size = float2(0.0f, 0.0f), inner_width = 0.0f)) {
            for (i in range(10)) {
                table_next_row()
                table_next_column()
                selectable(WIDGETS_SEL_COL2_ROW[i],
                    (text = "Item {i}",
                     flags = ImGuiSelectableFlags.SpanAllColumns))
                table_next_column()
                text(WIDGETS_SEL_COL2_TXT1[i], (text = "Some other contents"))
                table_next_column()
                text(WIDGETS_SEL_COL2_TXT2[i], (text = "123456"))
            }
        }
    }
}

def private SelectablesGridSection() {
    tree_node(WIDGETS_SEL_GRID,
        (text = "Grid", flags = ImGuiTreeNodeFlags.None)) {
        // Cpp's "silly fun": when every cell is selected, a time-animated SelectableTextAlign makes labels orbit inside their squares (gated on the bool[16] all-true check).
        var winning = true
        for (i in range(length(WIDGETS_SEL_GRID_SEL))) {
            if (!WIDGETS_SEL_GRID_SEL[i]) {
                winning = false
                break
            }
        }
        let t = float(GetTime())
        let align = float2(0.5f + 0.5f * cos(t * 2.0f),
                           0.5f + 0.5f * sin(t * 3.0f))
        if (winning) {
            with_style((ImGuiStyleVar.SelectableTextAlign, align)) {
                SelectablesGridBody()
            }
        } else {
            SelectablesGridBody()
        }
    }
}

// 4x4 click-toggle-with-neighbors grid body. Extracted so `with_style` can
// scope a single call without duplicating the for/for nest.
def private SelectablesGridBody() {
    for (y in range(4)) {
        for (x in range(4)) {
            if (x > 0) {
                same_line(WIDGETS_SEL_GRID_SL[y * 4 + x])
            }
            let idx = y * 4 + x
            with_id("cell_{idx}") {
                if (selectable_label(WIDGETS_SEL_GRID_ROW[idx],
                                     "Sailor", WIDGETS_SEL_GRID_SEL[idx],
                                     ImGuiSelectableFlags.None,
                                     float2(50.0f, 50.0f))) {
                    WIDGETS_SEL_GRID_SEL[idx] = !WIDGETS_SEL_GRID_SEL[idx]
                    if (x > 0) {
                        WIDGETS_SEL_GRID_SEL[idx - 1] = !WIDGETS_SEL_GRID_SEL[idx - 1]
                    }
                    if (x < 3) {
                        WIDGETS_SEL_GRID_SEL[idx + 1] = !WIDGETS_SEL_GRID_SEL[idx + 1]
                    }
                    if (y > 0) {
                        WIDGETS_SEL_GRID_SEL[idx - 4] = !WIDGETS_SEL_GRID_SEL[idx - 4]
                    }
                    if (y < 3) {
                        WIDGETS_SEL_GRID_SEL[idx + 4] = !WIDGETS_SEL_GRID_SEL[idx + 4]
                    }
                }
            }
        }
    }
}

def private SelectablesAlignmentSection() {
    tree_node(WIDGETS_SEL_ALIGN,
        (text = "Alignment", flags = ImGuiTreeNodeFlags.None)) {
        // 3x3 grid; per-cell alignment via with_style, which scopes the push/pop tightly to each selectable (cpp pushes a fresh style var around each one).
        for (y in range(3)) {
            for (x in range(3)) {
                let cell = float2(float(x) / 2.0f, float(y) / 2.0f)
                if (x > 0) {
                    same_line(WIDGETS_SEL_ALIGN_SL[y * 3 + x])
                }
                with_style((ImGuiStyleVar.SelectableTextAlign, cell)) {
                    selectable(WIDGETS_SEL_ALIGN_ROW[y * 3 + x],
                        (text = "({cell.x:0.1},{cell.y:0.1})",
                         flags = ImGuiSelectableFlags.None,
                         size = float2(80.0f, 80.0f)))
                }
            }
        }
    }
}


// =============================================================================
// Text Input — cpp:1558-1748
// =============================================================================

// Six sub-trees: Multi-line (flag toggles), Filtered (CharFilter cb), Password
// (shared buffer), Completion/Hist/Edit (callbacks on data.EventFlag), Resize
// (CallbackResize), Misc (flag toggles).

def private TextInputSection() {
    tree_node(WIDGETS_TI_SECTION,
        (text = "Text Input", flags = ImGuiTreeNodeFlags.None)) {
        TextInputMultilineSection()
        TextInputFilteredSection()
        TextInputPasswordSection()
        TextInputCallbacksSection()
        TextInputResizeSection()
        TextInputMiscSection()
    }
}

def private TextInputMultilineSection() {
    tree_node(WIDGETS_TI_MULTILINE,
        (text = "Multi-line Text Input", flags = ImGuiTreeNodeFlags.None)) {
        // Seed the buffer once — InputTextState wakes up empty; later frames preserve the user's edits via the persistent buffer.
        if (empty(WIDGETS_TI_ML_BUF.value)) {
            WIDGETS_TI_ML_BUF.pending_value = WIDGETS_TI_MULTILINE_INIT
            WIDGETS_TI_ML_BUF.has_pending = true
        }
        edit_checkbox_flags(safe_addr(WIDGETS_TI_ML_FLAGS),
            (id = "WIDGETS_TI_ML_F_READONLY",
             text = "ImGuiInputTextFlags_ReadOnly",
             flags_value = int(ImGuiInputTextFlags.ReadOnly)))
        edit_checkbox_flags(safe_addr(WIDGETS_TI_ML_FLAGS),
            (id = "WIDGETS_TI_ML_F_TAB",
             text = "ImGuiInputTextFlags_AllowTabInput",
             flags_value = int(ImGuiInputTextFlags.AllowTabInput)))
        edit_checkbox_flags(safe_addr(WIDGETS_TI_ML_FLAGS),
            (id = "WIDGETS_TI_ML_F_CTRLENTER",
             text = "ImGuiInputTextFlags_CtrlEnterForNewLine",
             flags_value = int(ImGuiInputTextFlags.CtrlEnterForNewLine)))
        let body_h = GetTextLineHeight() * 16.0f
        input_text_multiline(WIDGETS_TI_ML_BUF,
            (text = "##source",
             size = float2(-FLT_MIN, body_h),
             flags = unsafe(reinterpret<ImGuiInputTextFlags>(WIDGETS_TI_ML_FLAGS))))
    }
}

def private TextInputFilteredSection() {
    tree_node(WIDGETS_TI_FILTERED,
        (text = "Filtered Text Input", flags = ImGuiTreeNodeFlags.None)) {
        input_text(WIDGETS_TI_F_DEFAULT, (text = "default"))
        input_text(WIDGETS_TI_F_DEC,
            (text = "decimal", flags = ImGuiInputTextFlags.CharsDecimal))
        input_text(WIDGETS_TI_F_HEX,
            (text = "hexadecimal",
             flags = ImGuiInputTextFlags.CharsHexadecimal |
                     ImGuiInputTextFlags.CharsUppercase))
        input_text(WIDGETS_TI_F_UPPER,
            (text = "uppercase", flags = ImGuiInputTextFlags.CharsUppercase))
        input_text(WIDGETS_TI_F_NOBLANK,
            (text = "no blank", flags = ImGuiInputTextFlags.CharsNoBlank))
        // CharFilter callback: lower<->upper swap; EventChar is uint16, widen to int for the ASCII range comparisons.
        input_text_callback(WIDGETS_TI_F_CASE, "casing swap",
            ImGuiInputTextFlags.CallbackCharFilter,
            @(var data : ImGuiInputTextCallbackData) : int {
                let c = int(data.EventChar)
                if (c >= int(0x61) && c <= int(0x7A)) {       // 'a'..'z' -> upper
                    data.EventChar = uint(c - int(0x20))
                } elif (c >= int(0x41) && c <= int(0x5A)) {   // 'A'..'Z' -> lower
                    data.EventChar = uint(c + int(0x20))
                }
                return 0
            })
        // CharFilter callback: pass only chars in "imgui" (i, m, g, u); return 1 to reject any other.
        input_text_callback(WIDGETS_TI_F_IMGUI, "\"imgui\"",
            ImGuiInputTextFlags.CallbackCharFilter,
            @(var data : ImGuiInputTextCallbackData) : int {
                let c = int(data.EventChar)
                if (c == int('i')) return 0
                if (c == int('m')) return 0
                if (c == int('g')) return 0
                if (c == int('u')) return 0
                return 1
            })
    }
}

def private TextInputPasswordSection() {
    tree_node(WIDGETS_TI_PASSWORD,
        (text = "Password Input", flags = ImGuiTreeNodeFlags.None)) {
        // Cpp seeds the buffer to "password123"; do the same once via pending_value so reloads preserve user edits.
        if (empty(WIDGETS_TI_PWD.value)) {
            WIDGETS_TI_PWD.pending_value = "password123"
            WIDGETS_TI_PWD.has_pending = true
        }
        input_text(WIDGETS_TI_PWD, (text = "password",
                                     flags = ImGuiInputTextFlags.Password))
        input_text_with_hint(WIDGETS_TI_PWD_HINT,
            (text = "password (w/ hint)", hint = "<password>",
             flags = ImGuiInputTextFlags.Password))
        input_text(WIDGETS_TI_PWD_CLEAR, (text = "password (clear)"))
    }
}

def private TextInputCallbacksSection() {
    tree_node(WIDGETS_TI_CALLBACKS,
        (text = "Completion, History, Edit Callbacks",
         flags = ImGuiTreeNodeFlags.None)) {
        // Completion: append ".." at cursor on Tab.
        input_text_callback(WIDGETS_TI_CB_COMPLETION, "Completion",
            ImGuiInputTextFlags.CallbackCompletion,
            @(var data : ImGuiInputTextCallbackData) : int {
                data |> InsertChars(data.CursorPos, "..")
                return 0
            })
        // History: Up replaces buffer with "Pressed Up!", Down with "Down".
        input_text_callback(WIDGETS_TI_CB_HISTORY, "History",
            ImGuiInputTextFlags.CallbackHistory,
            @(var data : ImGuiInputTextCallbackData) : int {
                if (data.EventKey == ImGuiKey.UpArrow) {
                    data |> DeleteChars(0, data.BufTextLen)
                    data |> InsertChars(0, "Pressed Up!")
                    data |> SelectAll()
                } elif (data.EventKey == ImGuiKey.DownArrow) {
                    data |> DeleteChars(0, data.BufTextLen)
                    data |> InsertChars(0, "Pressed Down!")
                    data |> SelectAll()
                }
                return 0
            })
        // Edit: tally edit count. Cpp also toggles case of buf[0] per edit, but data.Buf reads as string const and can't be written cleanly through the reinterpret rails, so we keep count-only.
        input_text_callback(WIDGETS_TI_CB_EDIT, "Edit",
            ImGuiInputTextFlags.CallbackEdit,
            @(var data : ImGuiInputTextCallbackData) : int {
                WIDGETS_TI_EDIT_COUNT++
                return 0
            })
        same_line(WIDGETS_TI_CB_EDIT_SL)
        text(WIDGETS_TI_CB_EDIT_COUNT, (text = "({WIDGETS_TI_EDIT_COUNT})"))
    }
}

def private TextInputResizeSection() {
    tree_node(WIDGETS_TI_RESIZE,
        (text = "Resize Callback", flags = ImGuiTreeNodeFlags.None)) {
        // input_text_growable wraps ImGuiInputTextFlags_CallbackResize and grows state.buffer past the initial capacity as the user types (cpp uses an ImVector<char>).
        input_text_growable(WIDGETS_TI_RESIZE_BUF, (text = "##MyStr"))
        text(WIDGETS_TI_RESIZE_STATS,
            (text = "Filled: {!empty(WIDGETS_TI_RESIZE_BUF.value)}"))
    }
}

def private TextInputMiscSection() {
    tree_node(WIDGETS_TI_MISC,
        (text = "Miscellaneous", flags = ImGuiTreeNodeFlags.None)) {
        edit_checkbox_flags(safe_addr(WIDGETS_TI_MISC_FLAGS),
            (id = "WIDGETS_TI_MISC_F_ESC",
             text = "ImGuiInputTextFlags_EscapeClearsAll",
             flags_value = int(ImGuiInputTextFlags.EscapeClearsAll)))
        edit_checkbox_flags(safe_addr(WIDGETS_TI_MISC_FLAGS),
            (id = "WIDGETS_TI_MISC_F_RO",
             text = "ImGuiInputTextFlags_ReadOnly",
             flags_value = int(ImGuiInputTextFlags.ReadOnly)))
        edit_checkbox_flags(safe_addr(WIDGETS_TI_MISC_FLAGS),
            (id = "WIDGETS_TI_MISC_F_NOUNDO",
             text = "ImGuiInputTextFlags_NoUndoRedo",
             flags_value = int(ImGuiInputTextFlags.NoUndoRedo)))
        input_text(WIDGETS_TI_MISC_BUF,
            (text = "Hello",
             flags = unsafe(reinterpret<ImGuiInputTextFlags>(WIDGETS_TI_MISC_FLAGS))))
    }
}


// =============================================================================
// Drag and Drop — cpp:2450-2591
// =============================================================================

// Four sub-trees: standard widgets (ColorEdit3/4 auto source+target), copy/swap
// (9-button grid), reorder (Selectables + IsItemActive, no DnD API), tooltip at
// target (2 drop targets + ColorButton source).

def private DragDropSection() {
    tree_node(WIDGETS_DD_SECTION,
        (text = "Drag and Drop", flags = ImGuiTreeNodeFlags.None)) {
        DragDropStandardSection()
        DragDropCopySwapSection()
        DragDropReorderSection()
        DragDropTooltipSection()
    }
}

def private DragDropStandardSection() {
    tree_node(WIDGETS_DD_STD,
        (text = "Drag and drop in standard widgets",
         flags = ImGuiTreeNodeFlags.None)) {
        // ColorEdit3/4 auto-register as DnD sources and targets for the built-in COLOR_3F/COLOR_4F payloads — no caller wiring needed.
        color_edit3(WIDGETS_DD_STD_COL1,
            (text = "color 1", init = float3(1.0f, 0.0f, 0.2f)))
        color_edit4(WIDGETS_DD_STD_COL2,
            (text = "color 2", init = float4(0.4f, 0.7f, 0.0f, 0.5f)))
    }
}

def private DragDropCopySwapSection() {
    tree_node(WIDGETS_DD_COPYSWAP,
        (text = "Drag and drop to copy/swap items",
         flags = ImGuiTreeNodeFlags.None)) {
        // Mode picker — exclusive radio group; radio_button_int reports clicks, the caller keeps WIDGETS_DD_COPY_MODE which drives the source preview text and target apply branch.
        if (radio_button_int(WIDGETS_DD_COPY_R_COPY,
                             (text = "Copy", v_button = 0))) {
            WIDGETS_DD_COPY_MODE = 0
        }
        same_line(WIDGETS_DD_COPY_R_SL1)
        if (radio_button_int(WIDGETS_DD_COPY_R_MOVE,
                             (text = "Move", v_button = 1))) {
            WIDGETS_DD_COPY_MODE = 1
        }
        same_line(WIDGETS_DD_COPY_R_SL2)
        if (radio_button_int(WIDGETS_DD_COPY_R_SWAP,
                             (text = "Swap", v_button = 2))) {
            WIDGETS_DD_COPY_MODE = 2
        }
        // 9 buttons in a 3x3 grid, each both a drag source and target — payload carries the source index, target applies the selected mode to names[] in place.
        for (n in range(length(WIDGETS_DD_COPY_NAMES))) {
            with_id("dd_copy_{n}") {
                if ((n % 3) != 0) {
                    same_line(WIDGETS_DD_COPY_SL[n])
                }
                button(WIDGETS_DD_COPY_BTN[n],
                    (text = WIDGETS_DD_COPY_NAMES[n],
                     size = float2(60.0f, 60.0f)))
                drag_drop_source(WIDGETS_DD_COPY_SRC[n],
                                  (flags = ImGuiDragDropFlags.None)) {
                    unsafe {
                        var src_n = n
                        SetDragDropPayload("DND_DEMO_CELL", addr(src_n),
                            uint64(typeinfo sizeof(src_n)), ImGuiCond.None)
                    }
                    let verb = (WIDGETS_DD_COPY_MODE == 0 ? "Copy"
                                 : (WIDGETS_DD_COPY_MODE == 1 ? "Move" : "Swap"))
                    text(WIDGETS_DD_COPY_PREVIEW[n],
                        (text = "{verb} {WIDGETS_DD_COPY_NAMES[n]}"))
                }
                drag_drop_target(WIDGETS_DD_COPY_TGT[n]) {
                    let payload = AcceptDragDropPayload("DND_DEMO_CELL",
                                                        ImGuiDragDropFlags.None)
                    if (payload != null) {
                        var payload_n = 0
                        unsafe {
                            payload_n = (reinterpret<int? const>(payload.Data))[0]
                        }
                        if (WIDGETS_DD_COPY_MODE == 0) {
                            WIDGETS_DD_COPY_NAMES[n] = WIDGETS_DD_COPY_NAMES[payload_n]
                        } elif (WIDGETS_DD_COPY_MODE == 1) {
                            WIDGETS_DD_COPY_NAMES[n] = WIDGETS_DD_COPY_NAMES[payload_n]
                            WIDGETS_DD_COPY_NAMES[payload_n] = ""
                        } else {
                            let tmp = WIDGETS_DD_COPY_NAMES[n]
                            WIDGETS_DD_COPY_NAMES[n] = WIDGETS_DD_COPY_NAMES[payload_n]
                            WIDGETS_DD_COPY_NAMES[payload_n] = tmp
                        }
                    }
                }
            }
        }
    }
}

def private DragDropReorderSection() {
    tree_node(WIDGETS_DD_REORDER,
        (text = "Drag to reorder items (simple)",
         flags = ImGuiTreeNodeFlags.None)) {
        // No DnD API — reorder via IsItemActive + GetMouseDragDelta + neighbor swap; the per-row ToggleState exists only because selectable's first slot needs one.
        for (n in range(length(WIDGETS_DD_REORDER_NAMES))) {
            selectable(WIDGETS_DD_REORDER_ROW[n],
                (text = WIDGETS_DD_REORDER_NAMES[n]))
            if (IsItemActive() && !IsItemHovered(ImGuiHoveredFlags.None)) {
                let delta_y = GetMouseDragDelta(ImGuiMouseButton.Left, -1.0f).y
                let n_next = n + (delta_y < 0.0f ? -1 : 1)
                if (n_next >= 0 && n_next < length(WIDGETS_DD_REORDER_NAMES)) {
                    let tmp = WIDGETS_DD_REORDER_NAMES[n]
                    WIDGETS_DD_REORDER_NAMES[n] = WIDGETS_DD_REORDER_NAMES[n_next]
                    WIDGETS_DD_REORDER_NAMES[n_next] = tmp
                    ResetMouseDragDelta(ImGuiMouseButton.Left)
                }
            }
        }
    }
}

def private DragDropTooltipSection() {
    tree_node(WIDGETS_DD_TIP,
        (text = "Tooltip at target location",
         flags = ImGuiTreeNodeFlags.None)) {
        // 2 drop-target buttons + 1 ColorButton source; AcceptBeforeDelivery + AcceptNoPreviewTooltip let us take over the cursor+tooltip on hover (force NotAllowed + "Cannot drop here!").
        for (n in range(2)) {
            with_id("dd_tip_{n}") {
                button(WIDGETS_DD_TIP_BTN[n],
                    (text = n == 0 ? "drop here##0" : "drop here##1"))
                drag_drop_target(WIDGETS_DD_TIP_TGT[n]) {
                    let drop_flags = (ImGuiDragDropFlags.AcceptBeforeDelivery |
                                      ImGuiDragDropFlags.AcceptNoPreviewTooltip)
                    let payload = AcceptDragDropPayload("_COL4F", drop_flags)
                    if (payload != null) {
                        SetMouseCursor(ImGuiMouseCursor.NotAllowed)
                        set_tooltip(WIDGETS_DD_TIP_NO[n], (text = "Cannot drop here!"))
                    }
                }
                if (n == 0) {
                    color_button(WIDGETS_DD_TIP_SRC,
                        (desc_id = "drag me",
                         col = float4(1.0f, 0.0f, 0.2f, 1.0f),
                         size = float2(0.0f, 0.0f),
                         flags = ImGuiColorEditFlags.None))
                }
            }
        }
    }
}


// =============================================================================
// Color/Picker Widgets — cpp:1990-2180
// =============================================================================

def private ColorPickerSection() {
    // One-shot palette init via hsv_to_rgb (cpp:2034-2046); drag-drop accept copies 3 or 4 floats from the payload, so alpha survives a COL3F drop.
    if (!WIDGETS_CP_PALETTE_INIT) {
        WIDGETS_CP_PALETTE |> reserve(32)
        for (n in range(32)) {
            let rgb = hsv_to_rgb(float(n) / 31.0f, 0.8f, 0.8f)
            WIDGETS_CP_PALETTE |> push(float4(rgb.x, rgb.y, rgb.z, 1.0f))
        }
        WIDGETS_CP_PALETTE_INIT = true
    }

    tree_node(WIDGETS_CP_SECTION,
        (text = "Color/Picker Widgets", flags = ImGuiTreeNodeFlags.None)) {
        // ===== Options =====
        separator_text(WIDGETS_CP_SEP_OPTIONS, (text = "Options"))
        checkbox(WIDGETS_CP_ALPHA_PV, (text = "With Alpha Preview", init = true))
        checkbox(WIDGETS_CP_ALPHA_HALF, (text = "With Half Alpha Preview"))
        checkbox(WIDGETS_CP_DRAG_DROP, (text = "With Drag and Drop", init = true))
        checkbox(WIDGETS_CP_OPTS_MENU, (text = "With Options Menu", init = true))
        same_line(WIDGETS_CP_SL_OPTS)
        text_disabled(WIDGETS_CP_OPTS_HELP, (text = "(?)"))
        set_item_tooltip(WIDGETS_CP_OPTS_TIP,
            (text = "Right-click on the individual color widget to show options."))
        checkbox(WIDGETS_CP_HDR, (text = "With HDR"))
        same_line(WIDGETS_CP_SL_HDR)
        text_disabled(WIDGETS_CP_HDR_HELP, (text = "(?)"))
        set_item_tooltip(WIDGETS_CP_HDR_TIP,
            (text = "Currently all this does is to lift the 0..1 limits on dragging widgets."))

        // Synthesize misc_flags from 5 toggles (cpp:2005; half-alpha preview wins when both set); ImGui flags are int-based enums, so build the bitset as int and reinterpret at the use site.
        var mf_int = 0
        if (WIDGETS_CP_HDR.value) {
            mf_int |= int(ImGuiColorEditFlags.HDR)
        }
        if (!WIDGETS_CP_DRAG_DROP.value) {
            mf_int |= int(ImGuiColorEditFlags.NoDragDrop)
        }
        if (!WIDGETS_CP_OPTS_MENU.value) {
            mf_int |= int(ImGuiColorEditFlags.NoOptions)
        }
        if (WIDGETS_CP_ALPHA_HALF.value) {
            mf_int |= int(ImGuiColorEditFlags.AlphaPreviewHalf)
        } elif (WIDGETS_CP_ALPHA_PV.value) {
            mf_int |= int(ImGuiColorEditFlags.AlphaOpaque)
        }
        let misc_flags = unsafe(reinterpret<ImGuiColorEditFlags>(mf_int))

        // ===== Inline color editor =====
        separator_text(WIDGETS_CP_SEP_INLINE, (text = "Inline color editor"))
        text(WIDGETS_CP_T1, (text = "Color widget:"))
        same_line(WIDGETS_CP_SL_T1)
        text_disabled(WIDGETS_CP_T1_HELP, (text = "(?)"))
        set_item_tooltip(WIDGETS_CP_T1_TIP,
            (text = "Click on the color square to open a color picker.\n" +
                    "CTRL+click on individual component to input value."))
        edit_color_edit3(unsafe(reinterpret<float3?>(safe_addr(WIDGETS_CP_COLOR))),
            (id = "WIDGETS_CP_E3_1", text = "MyColor##1", flags = misc_flags))

        text(WIDGETS_CP_T2, (text = "Color widget HSV with Alpha:"))
        edit_color_edit4(safe_addr(WIDGETS_CP_COLOR),
            (id = "WIDGETS_CP_E4_2", text = "MyColor##2",
             flags = misc_flags | ImGuiColorEditFlags.DisplayHSV))

        text(WIDGETS_CP_T2F, (text = "Color widget with Float Display:"))
        edit_color_edit4(safe_addr(WIDGETS_CP_COLOR),
            (id = "WIDGETS_CP_E4_2F", text = "MyColor##2f",
             flags = misc_flags | ImGuiColorEditFlags.Float))

        text(WIDGETS_CP_T3, (text = "Color button with Picker:"))
        same_line(WIDGETS_CP_SL_T3)
        text_disabled(WIDGETS_CP_T3_HELP, (text = "(?)"))
        set_item_tooltip(WIDGETS_CP_T3_TIP,
            (text = "With the ImGuiColorEditFlags_NoInputs flag you can hide all the slider/text inputs.\n" +
                    "With the ImGuiColorEditFlags_NoLabel flag you can pass a non-empty label which will only " +
                    "be used for the tooltip and picker popup."))
        edit_color_edit4(safe_addr(WIDGETS_CP_COLOR),
            (id = "WIDGETS_CP_E4_3", text = "MyColor##3",
             flags = misc_flags | ImGuiColorEditFlags.NoInputs | ImGuiColorEditFlags.NoLabel))

        // ===== Color button with custom Picker popup =====
        text(WIDGETS_CP_T3B, (text = "Color button with Custom Picker Popup:"))
        var should_open = color_button(WIDGETS_CP_OPEN_BTN,
            (desc_id = "MyColor##3b",
             col = WIDGETS_CP_COLOR,
             size = float2(0.0f, 0.0f),
             flags = misc_flags))
        same_line(WIDGETS_CP_SL_OPEN,
            (offset_from_start_x = 0.0f, spacing = GetStyle().ItemInnerSpacing.x))
        if (button(WIDGETS_CP_PALETTE_OPEN, (text = "Palette"))) {
            should_open = true
        }
        if (should_open) {
            open_popup("mypicker")
            WIDGETS_CP_BACKUP = WIDGETS_CP_COLOR
        }

        popup_window(WIDGETS_CP_PICKER_POPUP,
            (str_id = "mypicker", flags = ImGuiWindowFlags.None)) {
            text(WIDGETS_CP_PICKER_T1,
                (text = "MY CUSTOM COLOR PICKER WITH AN AMAZING PALETTE!"))
            separator(WIDGETS_CP_PICKER_SEP1)
            edit_color_picker4(safe_addr(WIDGETS_CP_COLOR),
                (id = "WIDGETS_CP_PICKER_MAIN", text = "##picker",
                 flags = misc_flags | ImGuiColorEditFlags.NoSidePreview
                                    | ImGuiColorEditFlags.NoSmallPreview))
            same_line(WIDGETS_CP_SL_PICKER)
            group(WIDGETS_CP_PICKER_GROUP) {
                text(WIDGETS_CP_PICKER_CT, (text = "Current"))
                color_button(WIDGETS_CP_PICKER_CUR,
                    (desc_id = "##current",
                     col = WIDGETS_CP_COLOR,
                     size = float2(60.0f, 40.0f),
                     flags = ImGuiColorEditFlags.NoPicker | ImGuiColorEditFlags.AlphaPreviewHalf))
                text(WIDGETS_CP_PICKER_PT, (text = "Previous"))
                if (color_button(WIDGETS_CP_PICKER_PREV,
                        (desc_id = "##previous",
                         col = WIDGETS_CP_BACKUP,
                         size = float2(60.0f, 40.0f),
                         flags = ImGuiColorEditFlags.NoPicker | ImGuiColorEditFlags.AlphaPreviewHalf))) {
                    WIDGETS_CP_COLOR = WIDGETS_CP_BACKUP
                }
                separator(WIDGETS_CP_PICKER_SEP2)
                text(WIDGETS_CP_PICKER_PALT, (text = "Palette"))
                for (n in range(length(WIDGETS_CP_PALETTE))) {
                    with_id("cp_palette_{n}") {
                        if ((n % 8) != 0) {
                            same_line(WIDGETS_CP_PALETTE_SL[n],
                                (offset_from_start_x = 0.0f,
                                 spacing = GetStyle().ItemSpacing.y))
                        }
                        let palette_flags = (ImGuiColorEditFlags.NoAlpha
                                             | ImGuiColorEditFlags.NoPicker
                                             | ImGuiColorEditFlags.NoTooltip)
                        if (color_button(WIDGETS_CP_PALETTE_BTN[n],
                                (desc_id = "##palette",
                                 col = WIDGETS_CP_PALETTE[n],
                                 size = float2(20.0f, 20.0f),
                                 flags = palette_flags))) {
                            // Preserve current alpha when picking a palette swatch (cpp:2080).
                            WIDGETS_CP_COLOR = float4(
                                WIDGETS_CP_PALETTE[n].x,
                                WIDGETS_CP_PALETTE[n].y,
                                WIDGETS_CP_PALETTE[n].z,
                                WIDGETS_CP_COLOR.w)
                        }
                        // ColorButton is a built-in drag source for COL3F/COL4F payloads; the target overwrites the swatch in place.
                        drag_drop_target(WIDGETS_CP_PALETTE_TGT[n]) {
                            let p3 = AcceptDragDropPayload("_COL3F",
                                                           ImGuiDragDropFlags.None)
                            if (p3 != null) {
                                unsafe {
                                    let ptr = reinterpret<float? const>(p3.Data)
                                    WIDGETS_CP_PALETTE[n].x = ptr[0]
                                    WIDGETS_CP_PALETTE[n].y = ptr[1]
                                    WIDGETS_CP_PALETTE[n].z = ptr[2]
                                }
                            }
                            let p4 = AcceptDragDropPayload("_COL4F",
                                                           ImGuiDragDropFlags.None)
                            if (p4 != null) {
                                unsafe {
                                    let ptr = reinterpret<float? const>(p4.Data)
                                    WIDGETS_CP_PALETTE[n] = float4(ptr[0], ptr[1],
                                                                    ptr[2], ptr[3])
                                }
                            }
                        }
                    }
                }
            }
        }

        // ===== Color button only =====
        text(WIDGETS_CP_T3C, (text = "Color button only:"))
        checkbox(WIDGETS_CP_NO_BORDER, (text = "ImGuiColorEditFlags_NoBorder"))
        var cb_int = mf_int
        if (WIDGETS_CP_NO_BORDER.value) {
            cb_int |= int(ImGuiColorEditFlags.NoBorder)
        }
        let cb_flags = unsafe(reinterpret<ImGuiColorEditFlags>(cb_int))
        color_button(WIDGETS_CP_SIMPLE_BTN,
            (desc_id = "MyColor##3c",
             col = WIDGETS_CP_COLOR,
             size = float2(80.0f, 80.0f),
             flags = cb_flags))

        // ===== Color picker explorer =====
        separator_text(WIDGETS_CP_SEP_PICKER, (text = "Color picker"))
        checkbox(WIDGETS_CP_ALPHA, (text = "With Alpha", init = true))
        checkbox(WIDGETS_CP_ALPHA_BAR, (text = "With Alpha Bar", init = true))
        checkbox(WIDGETS_CP_SIDE_PV, (text = "With Side Preview", init = true))
        if (WIDGETS_CP_SIDE_PV.value) {
            same_line(WIDGETS_CP_SL_SIDE)
            checkbox(WIDGETS_CP_REF_COL, (text = "With Ref Color"))
            if (WIDGETS_CP_REF_COL.value) {
                same_line(WIDGETS_CP_SL_REF)
                edit_color_edit4(safe_addr(WIDGETS_CP_REF_COLOR_V),
                    (id = "WIDGETS_CP_REF_COLOR_E4", text = "##RefColor",
                     flags = ImGuiColorEditFlags.NoInputs | misc_flags))
            }
        }
        combo(WIDGETS_CP_DISPLAY_MODE,
            (text = "Display Mode", init = 0,
             items <- WIDGETS_CP_DISPLAY_ITEMS))
        same_line(WIDGETS_CP_SL_DM1)
        text_disabled(WIDGETS_CP_DM_HELP, (text = "(?)"))
        set_item_tooltip(WIDGETS_CP_DM_TIP,
            (text = "ColorEdit defaults to displaying RGB inputs if you don't specify a display mode, " +
                    "but the user can change it with a right-click on those inputs.\n\n" +
                    "ColorPicker defaults to displaying RGB+HSV+Hex if you don't specify a display mode.\n\n" +
                    "You can change the defaults using SetColorEditOptions()."))
        same_line(WIDGETS_CP_SL_DM2)
        text_disabled(WIDGETS_CP_PM_HELP, (text = "(?)"))
        set_item_tooltip(WIDGETS_CP_PM_TIP,
            (text = "When not specified explicitly (Auto/Current mode), user can right-click the picker to change mode."))

        // Compose picker_flags from explorer state + misc_flags; cpp's picker_mode is vestigial (never UI-mutated), so we drop it and rely on the auto picker shape.
        var pf_int = mf_int
        if (!WIDGETS_CP_ALPHA.value) {
            pf_int |= int(ImGuiColorEditFlags.NoAlpha)
        }
        if (WIDGETS_CP_ALPHA_BAR.value) {
            pf_int |= int(ImGuiColorEditFlags.AlphaBar)
        }
        if (!WIDGETS_CP_SIDE_PV.value) {
            pf_int |= int(ImGuiColorEditFlags.NoSidePreview)
        }
        let dm = WIDGETS_CP_DISPLAY_MODE.value
        if (dm == 1) {
            pf_int |= int(ImGuiColorEditFlags.NoInputs)
        }
        if (dm == 2) {
            pf_int |= int(ImGuiColorEditFlags.DisplayRGB)
        }
        if (dm == 3) {
            pf_int |= int(ImGuiColorEditFlags.DisplayHSV)
        }
        if (dm == 4) {
            pf_int |= int(ImGuiColorEditFlags.DisplayHex)
        }
        let picker_flags = unsafe(reinterpret<ImGuiColorEditFlags>(pf_int))
        edit_color_picker4(safe_addr(WIDGETS_CP_COLOR),
            (id = "WIDGETS_CP_PICKER_4", text = "MyColor##4",
             flags = picker_flags,
             ref_col = WIDGETS_CP_REF_COLOR_V,
             has_ref_col = WIDGETS_CP_REF_COL.value))

        text(WIDGETS_CP_T_DEF, (text = "Set defaults in code:"))
        same_line(WIDGETS_CP_SL_DEF)
        text_disabled(WIDGETS_CP_DEF_HELP, (text = "(?)"))
        set_item_tooltip(WIDGETS_CP_DEF_TIP,
            (text = "SetColorEditOptions() is designed to allow you to set boot-time default.\n" +
                    "We don't have Push/Pop functions because you can force options on a per-widget basis if needed, " +
                    "and the user can change non-forced ones with the options menu.\nWe don't have a getter to avoid " +
                    "encouraging you to persistently save values that aren't forward-compatible."))
        if (button(WIDGETS_CP_DEF_BTN1, (text = "Default: Uint8 + HSV + Hue Bar"))) {
            SetColorEditOptions(ImGuiColorEditFlags.Uint8
                              | ImGuiColorEditFlags.DisplayHSV
                              | ImGuiColorEditFlags.PickerHueBar)
        }
        if (button(WIDGETS_CP_DEF_BTN2, (text = "Default: Float + HDR + Hue Wheel"))) {
            SetColorEditOptions(ImGuiColorEditFlags.Float
                              | ImGuiColorEditFlags.HDR
                              | ImGuiColorEditFlags.PickerHueWheel)
        }

        // Always display a small version of both picker types side-by-side.
        text(WIDGETS_CP_T_BOTH, (text = "Both types:"))
        let w = (GetContentRegionAvail().x - GetStyle().ItemSpacing.y) * 0.40f
        SetNextItemWidth(w)
        edit_color_picker3(unsafe(reinterpret<float3?>(safe_addr(WIDGETS_CP_COLOR))),
            (id = "WIDGETS_CP_BOTH_BAR", text = "##MyColor##5",
             flags = ImGuiColorEditFlags.PickerHueBar
                   | ImGuiColorEditFlags.NoSidePreview
                   | ImGuiColorEditFlags.NoInputs
                   | ImGuiColorEditFlags.NoAlpha))
        same_line(WIDGETS_CP_SL_BOTH)
        SetNextItemWidth(w)
        edit_color_picker3(unsafe(reinterpret<float3?>(safe_addr(WIDGETS_CP_COLOR))),
            (id = "WIDGETS_CP_BOTH_WHEEL", text = "##MyColor##6",
             flags = ImGuiColorEditFlags.PickerHueWheel
                   | ImGuiColorEditFlags.NoSidePreview
                   | ImGuiColorEditFlags.NoInputs
                   | ImGuiColorEditFlags.NoAlpha))

        // ===== HSV encoded colors =====
        spacing(WIDGETS_CP_SP_HSV)
        text(WIDGETS_CP_T_HSV1, (text = "HSV encoded colors"))
        same_line(WIDGETS_CP_SL_HSV1)
        text_disabled(WIDGETS_CP_HSV_HELP, (text = "(?)"))
        set_item_tooltip(WIDGETS_CP_HSV_TIP,
            (text = "By default, colors are given to ColorEdit and ColorPicker in RGB, " +
                    "but ImGuiColorEditFlags_InputHSV allows you to store colors as HSV " +
                    "and pass them to ColorEdit and ColorPicker as HSV. This comes with the " +
                    "added benefit that you can manipulate hue values with the picker even " +
                    "when saturation or value are zero."))
        text(WIDGETS_CP_T_HSV2, (text = "Color widget with InputHSV:"))
        edit_color_edit4(safe_addr(WIDGETS_CP_COLOR_HSV),
            (id = "WIDGETS_CP_HSV_RGB", text = "HSV shown as RGB##1",
             flags = ImGuiColorEditFlags.DisplayRGB
                   | ImGuiColorEditFlags.InputHSV
                   | ImGuiColorEditFlags.Float))
        edit_color_edit4(safe_addr(WIDGETS_CP_COLOR_HSV),
            (id = "WIDGETS_CP_HSV_HSV", text = "HSV shown as HSV##1",
             flags = ImGuiColorEditFlags.DisplayHSV
                   | ImGuiColorEditFlags.InputHSV
                   | ImGuiColorEditFlags.Float))
        edit_drag_float4(safe_addr(WIDGETS_CP_COLOR_HSV),
            (id = "WIDGETS_CP_HSV_DRAG4", text = "Raw HSV values",
             speed = 0.01f, v_min = 0.0f, v_max = 1.0f))
    }
}


// =============================================================================
// Data Types — cpp:2228-2348
// =============================================================================

// Exercises edit_drag/slider/input_scalar across all 10 scalar types. cpp passes a
// NULL upper bound for "drag double" (cpp:2294); our wrapper takes bounds by value
// (v_min==v_max==default → no clamp), so a finite sentinel replaces it — DBL_MAX triggers ImGui drag-speed pathology.

let private WIDGETS_DT_DRAG_DOUBLE_NO_UPPER : double = 1.0e15lf

def private DataTypesSection() {
    tree_node(WIDGETS_DT_SECTION,
        (text = "Data Types", flags = ImGuiTreeNodeFlags.None)) {
        // ===== Drags =====
        separator_text(WIDGETS_DT_SEP_DRAGS, (text = "Drags"))
        checkbox(WIDGETS_DT_DRAG_CLAMP, (text = "Clamp integers to 0..50"))
        same_line(WIDGETS_DT_SL_DC)
        text_disabled(WIDGETS_DT_DC_HELP, (text = "(?)"))
        set_item_tooltip(WIDGETS_DT_DC_TIP,
            (text = "As with every widget in dear imgui, we never modify values unless there is a user interaction.\n" +
                    "You can override the clamping limits by using CTRL+Click to input a value."))
        let cl = WIDGETS_DT_DRAG_CLAMP.value
        let speed = 0.2f
        edit_drag_scalar(safe_addr(WIDGETS_DT_S8),
            (id = "WIDGETS_DT_DRAG_S8", text = "drag s8",
             speed = speed, v_min = int8(0), v_max = int8(cl ? 50 : 0)))
        edit_drag_scalar(safe_addr(WIDGETS_DT_U8),
            (id = "WIDGETS_DT_DRAG_U8", text = "drag u8",
             speed = speed, v_min = uint8(0), v_max = uint8(cl ? 50 : 0), format = "%u ms"))
        edit_drag_scalar(safe_addr(WIDGETS_DT_S16),
            (id = "WIDGETS_DT_DRAG_S16", text = "drag s16",
             speed = speed, v_min = int16(0), v_max = int16(cl ? 50 : 0)))
        edit_drag_scalar(safe_addr(WIDGETS_DT_U16),
            (id = "WIDGETS_DT_DRAG_U16", text = "drag u16",
             speed = speed, v_min = uint16(0), v_max = uint16(cl ? 50 : 0), format = "%u ms"))
        edit_drag_scalar(safe_addr(WIDGETS_DT_S32),
            (id = "WIDGETS_DT_DRAG_S32", text = "drag s32",
             speed = speed, v_min = 0, v_max = cl ? 50 : 0))
        edit_drag_scalar(safe_addr(WIDGETS_DT_S32),
            (id = "WIDGETS_DT_DRAG_S32_HEX", text = "drag s32 hex",
             speed = speed, v_min = 0, v_max = cl ? 50 : 0, format = "0x%08X"))
        edit_drag_scalar(safe_addr(WIDGETS_DT_U32),
            (id = "WIDGETS_DT_DRAG_U32", text = "drag u32",
             speed = speed, v_min = 0u, v_max = cl ? 50u : 0u, format = "%u ms"))
        edit_drag_scalar(safe_addr(WIDGETS_DT_S64),
            (id = "WIDGETS_DT_DRAG_S64", text = "drag s64",
             speed = speed, v_min = 0l, v_max = cl ? 50l : 0l))
        edit_drag_scalar(safe_addr(WIDGETS_DT_U64),
            (id = "WIDGETS_DT_DRAG_U64", text = "drag u64",
             speed = speed, v_min = 0ul, v_max = cl ? 50ul : 0ul))
        edit_drag_scalar(safe_addr(WIDGETS_DT_F32),
            (id = "WIDGETS_DT_DRAG_F", text = "drag float",
             speed = 0.005f, v_min = 0.0f, v_max = 1.0f, format = "%f"))
        edit_drag_scalar(safe_addr(WIDGETS_DT_F32),
            (id = "WIDGETS_DT_DRAG_F_LOG", text = "drag float log",
             speed = 0.005f, v_min = 0.0f, v_max = 1.0f, format = "%f",
             flags = ImGuiSliderFlags.Logarithmic))
        // cpp passes NULL upper here; we substitute a large finite sentinel (WIDGETS_DT_DRAG_DOUBLE_NO_UPPER above).
        edit_drag_scalar(safe_addr(WIDGETS_DT_F64),
            (id = "WIDGETS_DT_DRAG_D", text = "drag double",
             speed = 0.0005f, v_min = 0.0lf,
             v_max = WIDGETS_DT_DRAG_DOUBLE_NO_UPPER, format = "%.10f grams"))
        edit_drag_scalar(safe_addr(WIDGETS_DT_F64),
            (id = "WIDGETS_DT_DRAG_D_LOG", text = "drag double log",
             speed = 0.0005f, v_min = 0.0lf, v_max = 1.0lf, format = "0 < %.10f < 1",
             flags = ImGuiSliderFlags.Logarithmic))

        // ===== Sliders =====
        separator_text(WIDGETS_DT_SEP_SLIDERS, (text = "Sliders"))
        edit_slider_scalar(safe_addr(WIDGETS_DT_S8),
            (id = "WIDGETS_DT_SL_S8_F", text = "slider s8 full",
             v_min = int8(-128), v_max = int8(127), format = "%d"))
        edit_slider_scalar(safe_addr(WIDGETS_DT_U8),
            (id = "WIDGETS_DT_SL_U8_F", text = "slider u8 full",
             v_min = uint8(0), v_max = uint8(255), format = "%u"))
        edit_slider_scalar(safe_addr(WIDGETS_DT_S16),
            (id = "WIDGETS_DT_SL_S16_F", text = "slider s16 full",
             v_min = int16(-32768), v_max = int16(32767), format = "%d"))
        edit_slider_scalar(safe_addr(WIDGETS_DT_U16),
            (id = "WIDGETS_DT_SL_U16_F", text = "slider u16 full",
             v_min = uint16(0), v_max = uint16(65535), format = "%u"))
        // SliderScalar's usable range is half the native type max (cpp:2246).
        let s32_min = INT_MIN / 2
        let s32_max = INT_MAX / 2
        let s32_hi_a = (INT_MAX / 2) - 100
        let s32_hi_b = INT_MAX / 2
        edit_slider_scalar(safe_addr(WIDGETS_DT_S32),
            (id = "WIDGETS_DT_SL_S32_L", text = "slider s32 low",
             v_min = 0, v_max = 50, format = "%d"))
        edit_slider_scalar(safe_addr(WIDGETS_DT_S32),
            (id = "WIDGETS_DT_SL_S32_H", text = "slider s32 high",
             v_min = s32_hi_a, v_max = s32_hi_b, format = "%d"))
        edit_slider_scalar(safe_addr(WIDGETS_DT_S32),
            (id = "WIDGETS_DT_SL_S32_F", text = "slider s32 full",
             v_min = s32_min, v_max = s32_max, format = "%d"))
        edit_slider_scalar(safe_addr(WIDGETS_DT_S32),
            (id = "WIDGETS_DT_SL_S32_X", text = "slider s32 hex",
             v_min = 0, v_max = 50, format = "0x%04X"))
        let u32_max = UINT_MAX / 2u
        let u32_hi_a = (UINT_MAX / 2u) - 100u
        let u32_hi_b = UINT_MAX / 2u
        edit_slider_scalar(safe_addr(WIDGETS_DT_U32),
            (id = "WIDGETS_DT_SL_U32_L", text = "slider u32 low",
             v_min = 0u, v_max = 50u, format = "%u"))
        edit_slider_scalar(safe_addr(WIDGETS_DT_U32),
            (id = "WIDGETS_DT_SL_U32_H", text = "slider u32 high",
             v_min = u32_hi_a, v_max = u32_hi_b, format = "%u"))
        edit_slider_scalar(safe_addr(WIDGETS_DT_U32),
            (id = "WIDGETS_DT_SL_U32_F", text = "slider u32 full",
             v_min = 0u, v_max = u32_max, format = "%u"))
        let s64_min : int64 = -9223372036854775807l / 2l - 1l
        let s64_max : int64 = 9223372036854775807l / 2l
        let s64_hi_a : int64 = s64_max - 100l
        edit_slider_scalar(safe_addr(WIDGETS_DT_S64),
            (id = "WIDGETS_DT_SL_S64_L", text = "slider s64 low",
             v_min = 0l, v_max = 50l, format = "%lld"))
        edit_slider_scalar(safe_addr(WIDGETS_DT_S64),
            (id = "WIDGETS_DT_SL_S64_H", text = "slider s64 high",
             v_min = s64_hi_a, v_max = s64_max, format = "%lld"))
        edit_slider_scalar(safe_addr(WIDGETS_DT_S64),
            (id = "WIDGETS_DT_SL_S64_F", text = "slider s64 full",
             v_min = s64_min, v_max = s64_max, format = "%lld"))
        let u64_max : uint64 = 0xfffffffffffffffful / 2ul
        let u64_hi_a : uint64 = u64_max - 100ul
        edit_slider_scalar(safe_addr(WIDGETS_DT_U64),
            (id = "WIDGETS_DT_SL_U64_L", text = "slider u64 low",
             v_min = 0ul, v_max = 50ul, format = "%llu ms"))
        edit_slider_scalar(safe_addr(WIDGETS_DT_U64),
            (id = "WIDGETS_DT_SL_U64_H", text = "slider u64 high",
             v_min = u64_hi_a, v_max = u64_max, format = "%llu ms"))
        edit_slider_scalar(safe_addr(WIDGETS_DT_U64),
            (id = "WIDGETS_DT_SL_U64_F", text = "slider u64 full",
             v_min = 0ul, v_max = u64_max, format = "%llu ms"))
        edit_slider_scalar(safe_addr(WIDGETS_DT_F32),
            (id = "WIDGETS_DT_SL_F_L", text = "slider float low",
             v_min = 0.0f, v_max = 1.0f))
        edit_slider_scalar(safe_addr(WIDGETS_DT_F32),
            (id = "WIDGETS_DT_SL_F_L_LOG", text = "slider float low log",
             v_min = 0.0f, v_max = 1.0f, format = "%.10f",
             flags = ImGuiSliderFlags.Logarithmic))
        edit_slider_scalar(safe_addr(WIDGETS_DT_F32),
            (id = "WIDGETS_DT_SL_F_H", text = "slider float high",
             v_min = -10000000000.0f, v_max = 10000000000.0f, format = "%e"))
        edit_slider_scalar(safe_addr(WIDGETS_DT_F64),
            (id = "WIDGETS_DT_SL_D_L", text = "slider double low",
             v_min = 0.0lf, v_max = 1.0lf, format = "%.10f grams"))
        edit_slider_scalar(safe_addr(WIDGETS_DT_F64),
            (id = "WIDGETS_DT_SL_D_L_LOG", text = "slider double low log",
             v_min = 0.0lf, v_max = 1.0lf, format = "%.10f",
             flags = ImGuiSliderFlags.Logarithmic))
        edit_slider_scalar(safe_addr(WIDGETS_DT_F64),
            (id = "WIDGETS_DT_SL_D_H", text = "slider double high",
             v_min = -1000000000000000.0lf, v_max = 1000000000000000.0lf, format = "%e grams"))

        // ===== Sliders (reverse) =====
        separator_text(WIDGETS_DT_SEP_SREV, (text = "Sliders (reverse)"))
        edit_slider_scalar(safe_addr(WIDGETS_DT_S8),
            (id = "WIDGETS_DT_SLR_S8", text = "slider s8 reverse",
             v_min = int8(127), v_max = int8(-128), format = "%d"))
        edit_slider_scalar(safe_addr(WIDGETS_DT_U8),
            (id = "WIDGETS_DT_SLR_U8", text = "slider u8 reverse",
             v_min = uint8(255), v_max = uint8(0), format = "%u"))
        edit_slider_scalar(safe_addr(WIDGETS_DT_S32),
            (id = "WIDGETS_DT_SLR_S32", text = "slider s32 reverse",
             v_min = 50, v_max = 0, format = "%d"))
        edit_slider_scalar(safe_addr(WIDGETS_DT_U32),
            (id = "WIDGETS_DT_SLR_U32", text = "slider u32 reverse",
             v_min = 50u, v_max = 0u, format = "%u"))
        edit_slider_scalar(safe_addr(WIDGETS_DT_S64),
            (id = "WIDGETS_DT_SLR_S64", text = "slider s64 reverse",
             v_min = 50l, v_max = 0l, format = "%lld"))
        edit_slider_scalar(safe_addr(WIDGETS_DT_U64),
            (id = "WIDGETS_DT_SLR_U64", text = "slider u64 reverse",
             v_min = 50ul, v_max = 0ul, format = "%llu ms"))

        // ===== Inputs =====
        separator_text(WIDGETS_DT_SEP_INPUTS, (text = "Inputs"))
        checkbox(WIDGETS_DT_INPUTS_STEP, (text = "Show step buttons", init = true))
        let st = WIDGETS_DT_INPUTS_STEP.value
        edit_input_scalar(safe_addr(WIDGETS_DT_S8),
            (id = "WIDGETS_DT_IN_S8", text = "input s8",
             step = int8(st ? 1 : 0), step_fast = int8(0), format = "%d"))
        edit_input_scalar(safe_addr(WIDGETS_DT_U8),
            (id = "WIDGETS_DT_IN_U8", text = "input u8",
             step = uint8(st ? 1 : 0), step_fast = uint8(0), format = "%u"))
        edit_input_scalar(safe_addr(WIDGETS_DT_S16),
            (id = "WIDGETS_DT_IN_S16", text = "input s16",
             step = int16(st ? 1 : 0), step_fast = int16(0), format = "%d"))
        edit_input_scalar(safe_addr(WIDGETS_DT_U16),
            (id = "WIDGETS_DT_IN_U16", text = "input u16",
             step = uint16(st ? 1 : 0), step_fast = uint16(0), format = "%u"))
        edit_input_scalar(safe_addr(WIDGETS_DT_S32),
            (id = "WIDGETS_DT_IN_S32", text = "input s32",
             step = st ? 1 : 0, step_fast = 0, format = "%d"))
        edit_input_scalar(safe_addr(WIDGETS_DT_S32),
            (id = "WIDGETS_DT_IN_S32_HEX", text = "input s32 hex",
             step = st ? 1 : 0, step_fast = 0, format = "%04X"))
        edit_input_scalar(safe_addr(WIDGETS_DT_U32),
            (id = "WIDGETS_DT_IN_U32", text = "input u32",
             step = st ? 1u : 0u, step_fast = 0u, format = "%u"))
        edit_input_scalar(safe_addr(WIDGETS_DT_U32),
            (id = "WIDGETS_DT_IN_U32_HEX", text = "input u32 hex",
             step = st ? 1u : 0u, step_fast = 0u, format = "%08X"))
        edit_input_scalar(safe_addr(WIDGETS_DT_S64),
            (id = "WIDGETS_DT_IN_S64", text = "input s64",
             step = st ? 1l : 0l))
        edit_input_scalar(safe_addr(WIDGETS_DT_U64),
            (id = "WIDGETS_DT_IN_U64", text = "input u64",
             step = st ? 1ul : 0ul))
        edit_input_scalar(safe_addr(WIDGETS_DT_F32),
            (id = "WIDGETS_DT_IN_F", text = "input float",
             step = st ? 1.0f : 0.0f))
        edit_input_scalar(safe_addr(WIDGETS_DT_F64),
            (id = "WIDGETS_DT_IN_D", text = "input double",
             step = st ? 1.0lf : 0.0lf))
    }
}


// =============================================================================
// Drag/Slider Flags — cpp:2183-2215
// =============================================================================

def private DragSliderFlagsSection() {
    tree_node(WIDGETS_DRAG_SLIDER_FLAGS,
        (text = "Drag/Slider Flags", flags = ImGuiTreeNodeFlags.None)) {
        // 4 flag-bit toggles against a shared int holding the ImGuiSliderFlags bitset; HelpMarker tooltips dropped (the flag names are self-documenting).
        edit_checkbox_flags(safe_addr(WIDGETS_DSF_FLAGS),
            (id = "WIDGETS_DSF_F_CLAMP",
             text = "ImGuiSliderFlags_AlwaysClamp",
             flags_value = int(ImGuiSliderFlags.AlwaysClamp)))
        edit_checkbox_flags(safe_addr(WIDGETS_DSF_FLAGS),
            (id = "WIDGETS_DSF_F_LOG",
             text = "ImGuiSliderFlags_Logarithmic",
             flags_value = int(ImGuiSliderFlags.Logarithmic)))
        edit_checkbox_flags(safe_addr(WIDGETS_DSF_FLAGS),
            (id = "WIDGETS_DSF_F_NORTF",
             text = "ImGuiSliderFlags_NoRoundToFormat",
             flags_value = int(ImGuiSliderFlags.NoRoundToFormat)))
        edit_checkbox_flags(safe_addr(WIDGETS_DSF_FLAGS),
            (id = "WIDGETS_DSF_F_NOINPUT",
             text = "ImGuiSliderFlags_NoInput",
             flags_value = int(ImGuiSliderFlags.NoInput)))

        // All 4 DragFloat widgets share WIDGETS_DSF_DRAG_F (cpp's single static float); the 5th drags the int, and the flags bitset is widened back from the int via reinterpret.
        let flags = unsafe(reinterpret<ImGuiSliderFlags>(WIDGETS_DSF_FLAGS))
        text("Underlying float value: {WIDGETS_DSF_DRAG_F}")
        edit_drag_float(safe_addr(WIDGETS_DSF_DRAG_F),
            (id = "WIDGETS_DSF_DRAG_F_01", text = "DragFloat (0 -> 1)",
             speed = 0.005f, v_min = 0.0f, v_max = 1.0f,
             format = "%.3f", flags = flags))
        edit_drag_float(safe_addr(WIDGETS_DSF_DRAG_F),
            (id = "WIDGETS_DSF_DRAG_F_0INF", text = "DragFloat (0 -> +inf)",
             speed = 0.005f, v_min = 0.0f, v_max = FLT_MAX,
             format = "%.3f", flags = flags))
        edit_drag_float(safe_addr(WIDGETS_DSF_DRAG_F),
            (id = "WIDGETS_DSF_DRAG_F_NINF1", text = "DragFloat (-inf -> 1)",
             speed = 0.005f, v_min = -FLT_MAX, v_max = 1.0f,
             format = "%.3f", flags = flags))
        edit_drag_float(safe_addr(WIDGETS_DSF_DRAG_F),
            (id = "WIDGETS_DSF_DRAG_F_BOTH", text = "DragFloat (-inf -> +inf)",
             speed = 0.005f, v_min = -FLT_MAX, v_max = FLT_MAX,
             format = "%.3f", flags = flags))
        edit_drag_int(safe_addr(WIDGETS_DSF_DRAG_I),
            (id = "WIDGETS_DSF_DRAG_I_100", text = "DragInt (0 -> 100)",
             speed = 0.5f, v_min = 0, v_max = 100,
             format = "%d", flags = flags))

        text("Underlying float value: {WIDGETS_DSF_SLIDER_F}")
        edit_slider_float(safe_addr(WIDGETS_DSF_SLIDER_F),
            (id = "WIDGETS_DSF_SLIDER_F_01", text = "SliderFloat (0 -> 1)",
             v_min = 0.0f, v_max = 1.0f,
             format = "%.3f", flags = flags))
        edit_slider_int(safe_addr(WIDGETS_DSF_SLIDER_I),
            (id = "WIDGETS_DSF_SLIDER_I_100", text = "SliderInt (0 -> 100)",
             v_min = 0, v_max = 100,
             format = "%d", flags = flags))
    }
}


// =============================================================================
// Querying Item Status — cpp:2594-2702
// =============================================================================

def private QueryingItemStatusSection() {
    tree_node(WIDGETS_QIS_SECTION,
        (text = "Querying Item Status (Edited/Active/Hovered etc.)",
         flags = ImGuiTreeNodeFlags.None)) {

        // Item-type picker (16 entries); cpp seeds item_type=4 (SliderFloat) on first frame, mirrored via init=4.
        combo(WIDGETS_QIS_TYPE_PICK,
            (text = "Item Type", init = 4, items <- WIDGETS_QIS_TYPES))
        edit_checkbox(safe_addr(WIDGETS_QIS_DISABLED),
            (id = "WIDGETS_QIS_DIS_CB", text = "Item Disabled"))

        let item_type = WIDGETS_QIS_TYPE_PICK.value
        var ret = false

        with_disabled(WIDGETS_QIS_DISABLED) {
            // 16-way dispatcher; each branch renders one widget per frame, so each per-type state global renders once and check_unique_render is satisfied.
            if (item_type == 0) {
                text("ITEM: Text")
            } elif (item_type == 1) {
                ret = button(WIDGETS_QIS_BTN, (text = "ITEM: Button"))
            } elif (item_type == 2) {
                with_button_repeat(true) {
                    ret = button(WIDGETS_QIS_BTN_REPEAT, (text = "ITEM: Button"))
                }
            } elif (item_type == 3) {
                ret = checkbox(WIDGETS_QIS_CHK, (text = "ITEM: Checkbox"))
            } elif (item_type == 4) {
                ret = edit_slider_float(safe_addr(WIDGETS_QIS_F1B),
                    (id = "WIDGETS_QIS_SLF", text = "ITEM: SliderFloat",
                     v_min = 0.0f, v_max = 1.0f))
            } elif (item_type == 5) {
                ret = input_text(WIDGETS_QIS_INPUT_TXT, (text = "ITEM: InputText"))
            } elif (item_type == 6) {
                ret = input_text_multiline(WIDGETS_QIS_INPUT_TXT_M,
                    (text = "ITEM: InputTextMultiline"))
            } elif (item_type == 7) {
                ret = edit_input_float(safe_addr(WIDGETS_QIS_F1),
                    (id = "WIDGETS_QIS_INF", text = "ITEM: InputFloat",
                     step = 1.0f))
            } elif (item_type == 8) {
                ret = edit_input_float3(safe_addr(WIDGETS_QIS_F3),
                    (id = "WIDGETS_QIS_INF3", text = "ITEM: InputFloat3"))
            } elif (item_type == 9) {
                ret = edit_color_edit4(safe_addr(WIDGETS_QIS_COL4F),
                    (id = "WIDGETS_QIS_CE4", text = "ITEM: ColorEdit4"))
            } elif (item_type == 10) {
                ret = selectable(WIDGETS_QIS_SEL, (text = "ITEM: Selectable"))
            } elif (item_type == 11) {
                ret = menu_item(WIDGETS_QIS_MENU, (text = "ITEM: MenuItem"))
            } elif (item_type == 12) {
                // tree_node_ex (leaf form) returns the open bool — same shape as cpp's TreeNode + open-gated TreePop, but without the manual TreePop.
                ret = tree_node_ex(WIDGETS_QIS_TREE_LEAF,
                    (text = "ITEM: TreeNode",
                     flags = ImGuiTreeNodeFlags.NoTreePushOnOpen))
            } elif (item_type == 13) {
                ret = tree_node_ex(WIDGETS_QIS_TREE_LEAF_DBL,
                    (text = "ITEM: TreeNode w/ ImGuiTreeNodeFlags_OpenOnDoubleClick",
                     flags = ImGuiTreeNodeFlags.OpenOnDoubleClick |
                             ImGuiTreeNodeFlags.NoTreePushOnOpen))
            } elif (item_type == 14) {
                ret = combo(WIDGETS_QIS_COMBO_FRUIT,
                    (text = "ITEM: Combo", init = 1, items <- WIDGETS_QIS_FRUITS))
            } elif (item_type == 15) {
                // ListBox container — 4 fruit rows as selectables (cpp's one-liner ListBox(items,count) was retired); selection mirrored via per-row toggle states.
                list_box(WIDGETS_QIS_LB,
                    (text = "ITEM: ListBox", size = float2(0.0f, 0.0f))) {
                    for (i in range(length(WIDGETS_QIS_FRUITS))) {
                        if (selectable(WIDGETS_QIS_LB_ROW[i],
                                (text = WIDGETS_QIS_FRUITS[i]))) {
                            ret = true
                        }
                    }
                }
            }

            // Hovered-flag flavors must be captured right after the item is submitted — the BulletText below would become the last item and clobber IsItem* readouts.
            let h_none = IsItemHovered() ? 1 : 0
            let h_stat = IsItemHovered(ImGuiHoveredFlags.Stationary) ? 1 : 0
            let h_short = IsItemHovered(ImGuiHoveredFlags.DelayShort) ? 1 : 0
            let h_norm = IsItemHovered(ImGuiHoveredFlags.DelayNormal) ? 1 : 0
            let h_tip = IsItemHovered(ImGuiHoveredFlags.ForTooltip) ? 1 : 0

            // Single-call BulletText with a multi-line payload — matches cpp's shape (one bullet, 25 readout lines).
            let r = ret ? 1 : 0
            let foc = IsItemFocused() ? 1 : 0
            let hov = IsItemHovered() ? 1 : 0
            let hov_bp = IsItemHovered(ImGuiHoveredFlags.AllowWhenBlockedByPopup) ? 1 : 0
            let hov_ba = IsItemHovered(ImGuiHoveredFlags.AllowWhenBlockedByActiveItem) ? 1 : 0
            let hov_oi = IsItemHovered(ImGuiHoveredFlags.AllowWhenOverlappedByItem) ? 1 : 0
            let hov_ow = IsItemHovered(ImGuiHoveredFlags.AllowWhenOverlappedByWindow) ? 1 : 0
            let hov_dis = IsItemHovered(ImGuiHoveredFlags.AllowWhenDisabled) ? 1 : 0
            let hov_ro = IsItemHovered(ImGuiHoveredFlags.RectOnly) ? 1 : 0
            let act = IsItemActive() ? 1 : 0
            let ed = IsItemEdited() ? 1 : 0
            let actd = IsItemActivated() ? 1 : 0
            let deact = IsItemDeactivated() ? 1 : 0
            let deact_e = IsItemDeactivatedAfterEdit() ? 1 : 0
            let vis = IsItemVisible() ? 1 : 0
            let clk = IsItemClicked() ? 1 : 0
            let togg = IsItemToggledOpen() ? 1 : 0
            let rmin = GetItemRectMin()
            let rmax = GetItemRectMax()
            let rsize = GetItemRectSize()
            bullet_text(WIDGETS_QIS_BL1,
                (text = "Return value = {r}\n" +
                        "IsItemFocused() = {foc}\n" +
                        "IsItemHovered() = {hov}\n" +
                        "IsItemHovered(_AllowWhenBlockedByPopup) = {hov_bp}\n" +
                        "IsItemHovered(_AllowWhenBlockedByActiveItem) = {hov_ba}\n" +
                        "IsItemHovered(_AllowWhenOverlappedByItem) = {hov_oi}\n" +
                        "IsItemHovered(_AllowWhenOverlappedByWindow) = {hov_ow}\n" +
                        "IsItemHovered(_AllowWhenDisabled) = {hov_dis}\n" +
                        "IsItemHovered(_RectOnly) = {hov_ro}\n" +
                        "IsItemActive() = {act}\n" +
                        "IsItemEdited() = {ed}\n" +
                        "IsItemActivated() = {actd}\n" +
                        "IsItemDeactivated() = {deact}\n" +
                        "IsItemDeactivatedAfterEdit() = {deact_e}\n" +
                        "IsItemVisible() = {vis}\n" +
                        "IsItemClicked() = {clk}\n" +
                        "IsItemToggledOpen() = {togg}\n" +
                        "GetItemRectMin() = ({rmin.x}, {rmin.y})\n" +
                        "GetItemRectMax() = ({rmax.x}, {rmax.y})\n" +
                        "GetItemRectSize() = ({rsize.x}, {rsize.y})"))
            bullet_text(WIDGETS_QIS_BL2,
                (text = "with Hovering Delay or Stationary test:\n" +
                        "IsItemHovered() = = {h_none}\n" +
                        "IsItemHovered(_Stationary) = {h_stat}\n" +
                        "IsItemHovered(_DelayShort) = {h_short}\n" +
                        "IsItemHovered(_DelayNormal) = {h_norm}\n" +
                        "IsItemHovered(_Tooltip) = {h_tip}"))
        }

        // Bottom tab-out box (ReadOnly) so the user can tab off the test widget and observe _Deactivated transitions above.
        input_text(WIDGETS_QIS_UNUSED,
            (text = "unused", flags = ImGuiInputTextFlags.ReadOnly))
    }
}


// =============================================================================
// Querying Window Status — cpp:2706-2800
// =============================================================================

def private QueryingWindowStatusSection() {
    tree_node(WIDGETS_QWS_SECTION,
        (text = "Querying Window Status (Focused/Hovered etc.)",
         flags = ImGuiTreeNodeFlags.None)) {

        edit_checkbox(safe_addr(WIDGETS_QWS_EMBED),
            (id = "WIDGETS_QWS_EMBED_CB",
             text = "Embed everything inside a child window for testing _RootWindow flag."))

        // Optional outer-child wrapping exercises the _RootWindow/_ChildWindows distinction in the readouts below (cpp uses ImGuiChildFlags_Borders).
        if (WIDGETS_QWS_EMBED) {
            child(WIDGETS_QWS_OUTER_CHILD,
                (text = "outer_child",
                 size = float2(0.0f, GetFontSize() * 20.0f),
                 child_flags = ImGuiChildFlags.Borders,
                 window_flags = ImGuiWindowFlags.None)) {
                QueryingWindowStatusBody()
            }
        } else {
            QueryingWindowStatusBody()
        }
    }
}

def private QueryingWindowStatusBody() {
    let f_none = IsWindowFocused() ? 1 : 0
    let f_cw = IsWindowFocused(ImGuiFocusedFlags.ChildWindows) ? 1 : 0
    let f_cw_np = IsWindowFocused(ImGuiFocusedFlags.ChildWindows |
                                   ImGuiFocusedFlags.NoPopupHierarchy) ? 1 : 0
    let f_cw_dk = IsWindowFocused(ImGuiFocusedFlags.ChildWindows |
                                   ImGuiFocusedFlags.DockHierarchy) ? 1 : 0
    let f_cw_rw = IsWindowFocused(ImGuiFocusedFlags.ChildWindows |
                                   ImGuiFocusedFlags.RootWindow) ? 1 : 0
    let f_cw_rw_np = IsWindowFocused(ImGuiFocusedFlags.ChildWindows |
                                      ImGuiFocusedFlags.RootWindow |
                                      ImGuiFocusedFlags.NoPopupHierarchy) ? 1 : 0
    let f_cw_rw_dk = IsWindowFocused(ImGuiFocusedFlags.ChildWindows |
                                      ImGuiFocusedFlags.RootWindow |
                                      ImGuiFocusedFlags.DockHierarchy) ? 1 : 0
    let f_rw = IsWindowFocused(ImGuiFocusedFlags.RootWindow) ? 1 : 0
    let f_rw_np = IsWindowFocused(ImGuiFocusedFlags.RootWindow |
                                   ImGuiFocusedFlags.NoPopupHierarchy) ? 1 : 0
    let f_rw_dk = IsWindowFocused(ImGuiFocusedFlags.RootWindow |
                                   ImGuiFocusedFlags.DockHierarchy) ? 1 : 0
    let f_any = IsWindowFocused(ImGuiFocusedFlags.AnyWindow) ? 1 : 0
    bullet_text(WIDGETS_QWS_BL_FOC,
        (text = "IsWindowFocused() = {f_none}\n" +
                "IsWindowFocused(_ChildWindows) = {f_cw}\n" +
                "IsWindowFocused(_ChildWindows|_NoPopupHierarchy) = {f_cw_np}\n" +
                "IsWindowFocused(_ChildWindows|_DockHierarchy) = {f_cw_dk}\n" +
                "IsWindowFocused(_ChildWindows|_RootWindow) = {f_cw_rw}\n" +
                "IsWindowFocused(_ChildWindows|_RootWindow|_NoPopupHierarchy) = {f_cw_rw_np}\n" +
                "IsWindowFocused(_ChildWindows|_RootWindow|_DockHierarchy) = {f_cw_rw_dk}\n" +
                "IsWindowFocused(_RootWindow) = {f_rw}\n" +
                "IsWindowFocused(_RootWindow|_NoPopupHierarchy) = {f_rw_np}\n" +
                "IsWindowFocused(_RootWindow|_DockHierarchy) = {f_rw_dk}\n" +
                "IsWindowFocused(_AnyWindow) = {f_any}"))

    let h_none = IsWindowHovered() ? 1 : 0
    let h_bp = IsWindowHovered(ImGuiHoveredFlags.AllowWhenBlockedByPopup) ? 1 : 0
    let h_ba = IsWindowHovered(ImGuiHoveredFlags.AllowWhenBlockedByActiveItem) ? 1 : 0
    let h_cw = IsWindowHovered(ImGuiHoveredFlags.ChildWindows) ? 1 : 0
    let h_cw_np = IsWindowHovered(ImGuiHoveredFlags.ChildWindows |
                                   ImGuiHoveredFlags.NoPopupHierarchy) ? 1 : 0
    let h_cw_dk = IsWindowHovered(ImGuiHoveredFlags.ChildWindows |
                                   ImGuiHoveredFlags.DockHierarchy) ? 1 : 0
    let h_cw_rw = IsWindowHovered(ImGuiHoveredFlags.ChildWindows |
                                   ImGuiHoveredFlags.RootWindow) ? 1 : 0
    let h_cw_rw_np = IsWindowHovered(ImGuiHoveredFlags.ChildWindows |
                                      ImGuiHoveredFlags.RootWindow |
                                      ImGuiHoveredFlags.NoPopupHierarchy) ? 1 : 0
    let h_cw_rw_dk = IsWindowHovered(ImGuiHoveredFlags.ChildWindows |
                                      ImGuiHoveredFlags.RootWindow |
                                      ImGuiHoveredFlags.DockHierarchy) ? 1 : 0
    let h_rw = IsWindowHovered(ImGuiHoveredFlags.RootWindow) ? 1 : 0
    let h_rw_np = IsWindowHovered(ImGuiHoveredFlags.RootWindow |
                                   ImGuiHoveredFlags.NoPopupHierarchy) ? 1 : 0
    let h_rw_dk = IsWindowHovered(ImGuiHoveredFlags.RootWindow |
                                   ImGuiHoveredFlags.DockHierarchy) ? 1 : 0
    let h_cw_bp = IsWindowHovered(ImGuiHoveredFlags.ChildWindows |
                                   ImGuiHoveredFlags.AllowWhenBlockedByPopup) ? 1 : 0
    let h_any = IsWindowHovered(ImGuiHoveredFlags.AnyWindow) ? 1 : 0
    let h_stat = IsWindowHovered(ImGuiHoveredFlags.Stationary) ? 1 : 0
    bullet_text(WIDGETS_QWS_BL_HOV,
        (text = "IsWindowHovered() = {h_none}\n" +
                "IsWindowHovered(_AllowWhenBlockedByPopup) = {h_bp}\n" +
                "IsWindowHovered(_AllowWhenBlockedByActiveItem) = {h_ba}\n" +
                "IsWindowHovered(_ChildWindows) = {h_cw}\n" +
                "IsWindowHovered(_ChildWindows|_NoPopupHierarchy) = {h_cw_np}\n" +
                "IsWindowHovered(_ChildWindows|_DockHierarchy) = {h_cw_dk}\n" +
                "IsWindowHovered(_ChildWindows|_RootWindow) = {h_cw_rw}\n" +
                "IsWindowHovered(_ChildWindows|_RootWindow|_NoPopupHierarchy) = {h_cw_rw_np}\n" +
                "IsWindowHovered(_ChildWindows|_RootWindow|_DockHierarchy) = {h_cw_rw_dk}\n" +
                "IsWindowHovered(_RootWindow) = {h_rw}\n" +
                "IsWindowHovered(_RootWindow|_NoPopupHierarchy) = {h_rw_np}\n" +
                "IsWindowHovered(_RootWindow|_DockHierarchy) = {h_rw_dk}\n" +
                "IsWindowHovered(_ChildWindows|_AllowWhenBlockedByPopup) = {h_cw_bp}\n" +
                "IsWindowHovered(_AnyWindow) = {h_any}\n" +
                "IsWindowHovered(_Stationary) = {h_stat}"))

    // Inner 50-px child purely to give IsWindowHovered(_ChildWindows) a real child to find.
    child(WIDGETS_QWS_INNER_CHILD,
        (text = "child", size = float2(0.0f, 50.0f),
         child_flags = ImGuiChildFlags.Borders,
         window_flags = ImGuiWindowFlags.None)) {
        text("This is another child window for testing the _ChildWindows flag.")
    }

    // Secondary closable window: title-bar IsItemHovered/IsItemActive readout (cpp:2780); its X-button writes WIDGETS_QWS_TEST via the window rail's closable bool.
    edit_checkbox(safe_addr(WIDGETS_QWS_TEST),
        (id = "WIDGETS_QWS_TEST_CB",
         text = "Hovered/Active tests after Begin() for title bar testing"))
    if (WIDGETS_QWS_TEST) {
        window(WIDGETS_QWS_TEST_WIN,
            (text = "Title bar Hovered/Active tests",
             closable = true, flags = ImGuiWindowFlags.None)) {
            popup_context_item(WIDGETS_QWS_TEST_CTX,
                (str_id = "WIDGETS_QWS_TEST_CTX",
                 flags = ImGuiPopupFlags.MouseButtonRight)) {
                if (menu_item(WIDGETS_QWS_TEST_CLOSE_MI, (text = "Close"))) {
                    WIDGETS_QWS_TEST = false
                }
            }
            let ti_hov = IsItemHovered() ? 1 : 0
            let ti_act = IsItemActive() ? 1 : 0
            text(WIDGETS_QWS_TEST_READOUT,
                (text = "IsItemHovered() after begin = {ti_hov} (== is title bar hovered)\n" +
                        "IsItemActive() after begin = {ti_act} (== is window being clicked/moved)"))
        }
        // Mirror the closable bool back so the X button propagates to our toggle (WindowState.open is false after X).
        if (!WIDGETS_QWS_TEST_WIN.open) {
            WIDGETS_QWS_TEST = false
            WIDGETS_QWS_TEST_WIN.open = true
        }
    }
}
