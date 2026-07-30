.. _internal_families:

.. index::
   single: Internal bindings; Families
   single: imgui_internal.h

#########################
Internal binding families
#########################

`Dear ImGui <https://github.com/ocornut/imgui>`_ keeps a second public header,
``imgui_internal.h``, holding the primitives the public widgets are built on:
the draw helpers, the item/layout machinery, the behavior state machines, and
the lower-level ``*Ex`` entry points. dasImgui binds a curated subset of these in
related **families** — each gated into the v2 surface through the allow-lists in
``bind/bind_imgui.das``, and each backed by a runnable
``examples/features/internal_*.das`` that drives the real bound API (the
"binding is usable" gate, not just "it compiles").

This is custom-widget territory: reach for these when the boost widget layer
(see :doc:`tutorials/index`) doesn't expose what you need and you want to build a
widget from the same parts ImGui itself uses.

**Surface conventions.** Each family is exposed in one of three shapes:

* **Scope guard** (``with_*`` in ``imgui_scope_builtin.das``) — for ``Begin``/``End``
  pairs, so the pairing can't leak. Runs your block between the two calls.
* **Raw call** — one-shot functions on the ``ALLOWED_IMGUI`` allow-list in
  ``imgui_lint.das``, called directly.
* **Manual C++ wrapper** (``dasIMGUI.main.cpp``) — for functions with a nullable
  ``const char*`` terminus (a daslang string can't express ``NULL``) or an
  out-parameter that comes back through a ``bool&`` / ``float&`` / ``ImS64&``.

Internal **flag** enums bind as ``int`` (cast ``int(Flag.X)`` at the call site, or
combine via the ``|`` rail in ``imgui_enums.das``); internal **plain** enums
(``ImGuiAxis``, ``ImGuiLogType``) bind as a daslang enum directly.

.. contents:: Families
   :local:
   :depth: 1


Render helpers
==============

The ``Render*`` draw helpers. Each writes straight into an ``ImDrawList`` (some take
one explicitly, some draw into the current window's), with no ImGui state — the
glyphs, frames, and decorations the widgets render with.

**Bound** (raw):

* Drawlist-arg: ``RenderArrow``, ``RenderBullet``, ``RenderCheckMark``,
  ``RenderArrowPointingAt``, ``RenderColorRectWithAlphaCheckerboard``.
* Current-window: ``RenderFrame``, ``RenderFrameBorder``, ``RenderText``.
* Rect-taking (``ImRect`` = ``float4``): ``RenderRectFilledRangeH``,
  ``RenderRectFilledWithHole``, ``RenderDragDropTargetRect``, ``RenderNavHighlight``
  (pulls the ``ImGuiNavHighlightFlags`` enum, as ``int``).
* Clipped text (C++ wrappers, ``text_end`` pinned to ``nullptr``):
  ``RenderTextClipped``, ``RenderTextClippedEx``, ``RenderTextEllipsis``.

.. code-block:: das

   with_window_drawlist() $(var dl) {
       RenderArrow(dl, pos, col, ImGuiDir.Right, 1.0)
   }
   RenderFrame(min, max, col, true, 4.0)          // into the current window
   RenderText(text_pos, "label")

.. seealso::

   :download:`internal_render.das <../../examples/features/internal_render.das>`,
   :download:`internal_render_rects.das <../../examples/features/internal_render_rects.das>`,
   :download:`internal_render_text.das <../../examples/features/internal_render_text.das>`
   — run e.g. ``daslang.exe examples/features/internal_render.das``


Vertex shading
==============

The ``ShadeVerts*`` helpers rewrite vertices that were **already** emitted into a
drawlist, over a ``[start, end)`` index range. The range is the ``VtxBuffer`` size
read before and after the draw (``length(dl.VtxBuffer)`` — an allow-listed
host-agnostic ``ImVector`` accessor).

**Bound** (raw): ``ShadeVertsLinearColorGradientKeepAlpha``, ``ShadeVertsLinearUV``,
``ShadeVertsTransformPos``.

.. code-block:: das

   with_window_drawlist() $(var dl) {
       let v0 = length(dl.VtxBuffer)
       dl |> add_rect_filled(p0, p1, rgba(255u, 255u, 255u, 255u))
       let v1 = length(dl.VtxBuffer)
       ShadeVertsLinearColorGradientKeepAlpha(dl, v0, v1, p0, p1, col_a, col_b)
   }

.. seealso::

   :download:`internal_shade_verts.das <../../examples/features/internal_shade_verts.das>`
   — ``daslang.exe examples/features/internal_shade_verts.das``


Layout & item sizing
=====================

The geometry calculators custom-widget code uses to size and position items the
same way built-in widgets do. Pure sizing/position math over already-public types.

**Bound** (raw): ``CalcItemSize``, ``CalcWrapWidthForPos``, ``GetContentRegionMaxAbs``,
``ItemSize`` (the rect overload takes ``ImRect`` = ``float4``).

.. code-block:: das

   let sz = CalcItemSize(ImVec2(0.0, 0.0), 220.0, 40.0)   // a zero picks the default
   ItemSize(box_bb)                                        // reserve layout space

.. seealso::

   :download:`internal_layout_calc.das <../../examples/features/internal_layout_calc.das>`
   — ``daslang.exe examples/features/internal_layout_calc.das``


Item hit-test & geometry
========================

The hit-test and rounding-geometry helpers below the public widget API. Each takes
an ``ImRect`` (``float4`` = min.x, min.y, max.x, max.y) and an ``ImGuiID`` and reads
the current window implicitly.

**Bound** (raw): ``ItemHoverable``, ``IsClippedEx``, ``CalcRoundingFlagsForRectInRect``.

.. code-block:: das

   let bb = float4(min.x, min.y, max.x, max.y)
   let hovered = ItemHoverable(bb, GetID("hover_rect"), 0)   // 0 = ImGuiItemFlags.None

.. seealso::

   :download:`internal_item_hittest.das <../../examples/features/internal_item_hittest.das>`
   — ``daslang.exe examples/features/internal_item_hittest.das``


Item assembly (ItemAdd + ButtonBehavior)
========================================

The two primitives the public ``button`` is built on: register an item in the
layout/ID system, then run the press/hover/hold state machine. C++ wrappers —
``ButtonBehavior``'s two state outputs come back through ``bool&`` args.

**Bound** (C++ wrappers): ``ItemAdd`` (with / without a separate nav rect),
``ButtonBehavior``. Pairs with the public ``ItemSize`` / ``GetID`` /
``GetCursorScreenPos``.

.. code-block:: das

   ItemSize(bb)
   if (ItemAdd(bb, id)) {
       var hovered = false; var held = false
       let pressed = ButtonBehavior(bb, id, hovered, held, ImGuiButtonFlags.None)
       // ... draw the box yourself from pressed/hovered/held
   }

.. seealso::

   :download:`internal_item_add_button_behavior.das <../../examples/features/internal_item_add_button_behavior.das>`
   — ``daslang.exe examples/features/internal_item_add_button_behavior.das``


Behavior-flag buttons
=====================

The ``*Ex`` buttons that take a behavior-flags argument. Their useful flag values
(``Repeat``, ``PressedOnClick``, …) live in the internal ``ImGuiButtonFlagsPrivate``
enum, combined with the public ``ImGuiButtonFlags`` via the cross-enum ``|`` rail in
``imgui_enums.das`` (both halves are one int-backed flag set).

**Bound** (raw): ``ButtonEx``, ``ArrowButtonEx``; enum ``ImGuiButtonFlagsPrivate``.

.. code-block:: das

   if (ButtonEx("Hold me", size, ImGuiButtonFlags.None | ImGuiButtonFlagsPrivate.Repeat)) {
       // fires repeatedly while held
   }

.. seealso::

   :download:`internal_button_ex.das <../../examples/features/internal_button_ex.das>`
   — ``daslang.exe examples/features/internal_button_ex.das``


Axis layout primitives (splitter & scrollbar)
==============================================

The axis-aligned interactive layout primitives. ``ScrollbarEx`` / ``SplitterBehavior``
are C++ wrappers — their in/out numeric state comes back through references
(``ImS64&`` position, ``float&`` pane widths). New enum ``ImGuiAxis`` (None / X / Y).

**Bound**: ``Scrollbar`` (raw); ``ScrollbarEx``, ``SplitterBehavior`` (C++ wrappers);
enum ``ImGuiAxis``.

.. code-block:: das

   // drag the bar — size1/size2 update live through float& out-params
   if (SplitterBehavior(bb, GetID("split"), ImGuiAxis.X, size1, size2, 40.0, 40.0)) {}

.. seealso::

   :download:`internal_splitter_scrollbar.das <../../examples/features/internal_splitter_scrollbar.das>`
   — ``daslang.exe examples/features/internal_splitter_scrollbar.das``


Tree-node behavior
==================

The tree-node state machine the public ``tree_node`` is built on. ``TreeNodeBehavior``
(C++ wrapper, ``label_end`` pinned to ``nullptr``) is the open/closed core;
``TreeNodeSetOpen`` / ``TreeNodeUpdateNextOpen`` set or resolve a node's open state by
ID. Pairs with the bound ``TreePushOverrideID`` / ``tree_pop``.

**Bound**: ``TreeNodeBehavior`` (C++ wrapper); ``TreeNodeSetOpen``,
``TreeNodeUpdateNextOpen`` (raw).

.. code-block:: das

   if (TreeNodeBehavior(GetID("node"), ImGuiTreeNodeFlags.None, "Node label")) {
       // ... children ...
       tree_pop()
   }
   TreeNodeSetOpen(GetID("node"), true)   // open it imperatively, no click

.. seealso::

   :download:`internal_tree_node_behavior.das <../../examples/features/internal_tree_node_behavior.das>`
   — ``daslang.exe examples/features/internal_tree_node_behavior.das``


Separator & low-level text
==========================

The separator and low-level text primitives below the public ``Separator`` / ``Text``.
``TextEx`` / ``SeparatorTextEx`` are C++ wrappers (terminus pinned to ``nullptr``). New
enums ``ImGuiSeparatorFlags`` (Horizontal / Vertical / SpanAllColumns) and
``ImGuiTextFlags`` (NoWidthForLargeClippedText), both as ``int`` args.

**Bound**: ``SeparatorEx`` (raw); ``TextEx``, ``SeparatorTextEx`` (C++ wrappers);
enums ``ImGuiSeparatorFlags``, ``ImGuiTextFlags``.

.. code-block:: das

   SeparatorEx(int(ImGuiSeparatorFlags.Vertical), 2.0)   // a 2px vertical separator
   TextEx("low-level text")

.. seealso::

   :download:`internal_separator_text.das <../../examples/features/internal_separator_text.das>`
   — ``daslang.exe examples/features/internal_separator_text.das``


Combo internals
===============

The low-level combo dropdown the public ``combo`` is built on, plus the custom-preview
path. Surfaced as three scope guards; the custom-preview rail folds the lone internal
``ImGuiComboFlagsPrivate`` flag (``CustomPreview``) into the public flags by value.

**Bound**: ``BeginComboPopup``, ``BeginComboPreview``, ``EndComboPreview`` →
``with_combo_popup`` / ``with_custom_preview_combo`` / ``with_combo_preview`` guards;
enum ``ImGuiComboFlagsPrivate``.

.. code-block:: das

   // a fully hand-rolled combo: open the popup over the box's rect, items in the block
   with_combo_popup(popup_id, box_bb, ImGuiComboFlags.None) {
       if (selectable_label(OPT_A, "Apple", sel == 0)) { sel = 0 }
   }

.. seealso::

   :download:`internal_combo.das <../../examples/features/internal_combo.das>`
   — ``daslang.exe examples/features/internal_combo.das``


Popups, tooltips & menus
========================

The ``*Ex`` popup / tooltip / menu entry points. The four ``Begin*Ex`` pair with the
public ``End*`` as unbalance-proof guards; the rest are raw stack-control one-shots.

**Bound**: ``BeginPopupEx``, ``BeginTooltipEx``, ``BeginTooltipHidden``, ``BeginMenuEx``
→ ``with_popup_ex`` / ``with_tooltip_ex`` / ``with_tooltip_hidden`` / ``with_menu_ex``
guards; ``OpenPopupEx``, ``ClosePopupToLevel``, ``ClosePopupsExceptModals``,
``IsPopupOpen`` (raw-ID overload), ``MenuItemEx`` (raw); enum ``ImGuiTooltipFlags``.

.. code-block:: das

   with_menu_ex("File", "F", true) {                 // icon-column submenu
       if (MenuItemEx("New", "N", "Ctrl+N", false, true)) { /* ... */ }
   }
   OpenPopupEx(popup_id, ImGuiPopupFlags.None)        // drive a popup by precomputed ID
   with_popup_ex(popup_id, ImGuiWindowFlags.None) { /* ... */ }

.. seealso::

   :download:`internal_popup_menu_ex.das <../../examples/features/internal_popup_menu_ex.das>`
   — ``daslang.exe examples/features/internal_popup_menu_ex.das``


ID stack seed & override
========================

The ID-stack seed / override helpers — hash an ``ImGuiID`` under an explicit seed,
query the hovered/focused item, or push a raw (not hash-combined) ID. ``with_override_id``
pushes the seed as-is, unlike ``with_id`` which hash-combines.

**Bound**: ``PushOverrideID`` → ``with_override_id`` guard; ``GetIDWithSeed``,
``GetFocusID``, ``GetHoveredID``, ``KeepAliveID``, ``MarkItemEdited``,
``TreePushOverrideID`` (raw).

.. code-block:: das

   let a = GetIDWithSeed(42, 0u)        // same int + different seed => different ID
   with_override_id(7777u) {
       let child_id = GetID("child")    // a stable, explicitly-seeded string ID
   }

.. seealso::

   :download:`internal_id_seed.das <../../examples/features/internal_id_seed.das>`
   — ``daslang.exe examples/features/internal_id_seed.das``


Focus scope & activation
========================

The focus-scope and item-activation helpers. Group items into a focus scope for
keyboard/gamepad nav and multi-select, or focus/activate an item by ID.

**Bound**: ``PushFocusScope`` → ``with_focus_scope`` guard; ``GetCurrentFocusScope``,
``FocusItem``, ``ActivateItemByID``, ``SetNextItemSelectionUserData`` (raw).

.. code-block:: das

   with_focus_scope(GetID("my_scope")) {
       // GetCurrentFocusScope() returns this scope's id inside the block, 0 outside
       button(ITEM_A, (text = "item A"))
   }

.. seealso::

   :download:`internal_focus_scope.das <../../examples/features/internal_focus_scope.das>`
   — ``daslang.exe examples/features/internal_focus_scope.das``


Item flags
==========

The item-flag stack (``PushItemFlag`` / ``PopItemFlag``), surfaced through
``with_item_flag`` for the internal flags lacking a dedicated public guard
(``Disabled`` / ``ButtonRepeat`` / ``NoTabStop`` already have ``with_disabled`` /
``with_button_repeat`` / ``with_tab_stop``). ``ImGuiItemFlags`` was the first internal
enum pulled into v2; its ``|`` comes from ``imgui_enums``.

**Bound**: ``PushItemFlag`` / ``PopItemFlag`` → ``with_item_flag`` guard; enum
``ImGuiItemFlags`` (``MixedValue``, ``ReadOnly``, ``NoNav``, ``AllowOverlap``, …).

.. code-block:: das

   with_item_flag(ImGuiItemFlags.ReadOnly | ImGuiItemFlags.NoNav, true) {
       slider_float(SL_RO, (text = "read-only, skipped by nav"))
   }

.. seealso::

   :download:`internal_item_flag.das <../../examples/features/internal_item_flag.das>`
   — ``daslang.exe examples/features/internal_item_flag.das``


Logging & capture
=================

The text-capture primitives beneath the public ``Log*`` API. Begin a capture to a
target — TTY, file, clipboard, or an in-memory buffer — and every widget submitted
while it's active appends its rendered text there; you can also inject lines by hand
and decorate them.

**Bound**: ``LogBegin`` → ``with_log`` guard (with the public ``LogFinish``);
``LogSetNextTextDecoration`` (raw), ``LogRenderedText`` (C++ wrapper); enum
``ImGuiLogType`` (None / TTY / File / Buffer / Clipboard).

.. code-block:: das

   with_log(ImGuiLogType.Clipboard, -1) {      // capture to the system clipboard
       LogSetNextTextDecoration("# ", "")       // prefix the next text
       text("Section: Fruits")                  // a real widget — captured
       LogRenderedText("(injected, capture-only)")
   }
   let report = GetClipboardText()              // read the capture back

``LogRenderedText`` pins ImGui's nullable ``ref_pos`` to null (embed ``\n`` for line
breaks). ``LogToBuffer`` is intentionally unbound — redundant with
``with_log(ImGuiLogType.Buffer, …)`` and unreadable without a buffer accessor.

.. seealso::

   :download:`internal_log_capture.das <../../examples/features/internal_log_capture.das>`
   — ``daslang.exe examples/features/internal_log_capture.das``
