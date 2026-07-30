.. _tutorial_tab_bar:

#######################
Tab bar
#######################

``tab_bar`` hosts one or more ``tab_item`` blocks under a strip of
clickable headers. ImGui owns active-tab selection internally — the
boost wrapper just observes — and routes click events to the right
body. Three things make tabs subtle:

* **Only the active tab's body runs each frame.** An inactive tab's
  leaves aren't submitted to ImGui, so a snapshot shows them
  ``rendered:false`` and their container-path key (e.g.
  ``AUDIO_TAB/A_VOLUME``) only resolves once that tab has been active —
  switch the tab before reading or writing leaves under it. The
  container's state (TabBarState / TabItemState) lives outside the body
  and persists.
* **There is no ``pending_select`` channel.** ImGui owns selection;
  ``imgui_click`` against a tab_item path is how external drivers switch
  tabs.
* **Closable tabs** add an X-button. The wrapper feeds ``&state.open``
  to BeginTabItem's ``p_open`` parameter; ImGui flips it false on
  X-click and the wrapper skips the tab next frame.

Source: ``examples/tutorial/tab_bar.das``.

************
Walkthrough
************

.. video:: tab_bar.mp4

.. literalinclude:: ../../../examples/tutorial/tab_bar.das
   :language: das
   :linenos:

Requires
========

Already in the baseline boost layer:

* ``imgui/imgui_containers_builtin`` — ``tab_bar``, ``tab_item``,
  ``edit_tab_item``.
* ``imgui/imgui_widgets_builtin`` — leaves used inside each tab body.

Active tab + body gating
========================

ImGui drives active-tab selection internally. Each ``tab_item``'s body
only runs while that tab is active:

.. code-block:: das

   tab_bar(MAIN_TABS, (text = "MainTabs",
                       flags = ImGuiTabBarFlags.Reorderable)) {
       tab_item(GENERAL_TAB, (text = "General", closable = false,
                              flags = ImGuiTabItemFlags.None)) {
           checkbox(G_WIRE, (text = "Wireframe"))   // only registered while active
       }
       tab_item(AUDIO_TAB, ...) { ... }
       tab_item(INFO_TAB,  ...) { ... }
   }

A snapshot taken while ``AUDIO_TAB`` is active shows ``G_WIRE`` under
``GENERAL_TAB`` as ``rendered:false`` — the leaf wasn't submitted to
ImGui that frame. Drivers must switch the active tab first before
reading or writing leaves underneath.

Switching tabs from outside
===========================

``imgui_click`` against a tab_item path hits the tab strip header (the
boost wrapper captures the header bbox separately so click resolves to
the strip, not the body):

.. code-block:: bash

   curl -X POST -d '{"name":"imgui_click","args":{"target":"TB_WIN/MAIN_TABS/AUDIO_TAB"}}' \
        localhost:9090/command

This is the only programmatic way to switch tabs — there's no
``imgui_set_active`` because ImGui's internal selection isn't exposed
as a struct field. ``imgui_click`` is enough: it produces a real ImGui
mouse event against the header, which is exactly what manual user
interaction does.

Closable tabs
=============

``closable=true`` makes the X-button appear:

.. code-block:: das

   tab_item(DRAFT_TAB, (text = "Draft", closable = true,
                        flags = ImGuiTabItemFlags.None)) {
       text("DRAFT_TAB.open = {DRAFT_TAB.open}")
   }

The wrapper threads ``&state.open`` into BeginTabItem's ``p_open``.
When the user clicks X, ImGui flips ``state.open=false``. Next frame
the wrapper sees ``!state.open`` and **skips BeginTabItem entirely** —
the tab disappears from the strip. Bring it back with
``DRAFT_TAB.pending_open = true`` from app code or ``imgui_open``
from a driver.

``state.open`` defaults to true so closable tabs appear from frame 1.
``@live open : bool = true`` in TabItemState carries the value across
live reload — the user's "I closed this tab" state survives a recompile.

Sharing open state across two surfaces
======================================

For the canonical "the same flag lives in a checkbox and a tab's X"
case, use ``edit_tab_item`` against a caller-owned bool pointer:

.. code-block:: das

   var private DRAFT_TAB_OPEN : bool = true

   // Checkbox row mirrors the X-button flag.
   edit_checkbox(safe_addr(DRAFT_TAB_OPEN), (id = "TAB_VISIBLE",
                                              text = "Show Draft tab"))
   // ...
   edit_tab_item(safe_addr(DRAFT_TAB_OPEN), "Draft",
                 ImGuiTabItemFlags.None) {
       text("Draft body — same flag the checkbox flips")
   }

The two rails share one ``bool`` — flip via either control surface and
the other auto-mirrors. Internal-only ``tab_item.open`` doesn't expose
this; ``edit_tab_item`` is the bidir-bind variant.

ImGuiTabBarFlags
================

Bar-level chrome:

* ``Reorderable`` — drag tab headers to reorder them.
* ``AutoSelectNewTabs`` — newly-submitted tabs become the active selection.
* ``TabListPopupButton`` — adds a ▼ menu on the right with all tabs as
  a list (useful when there are many tabs).
* ``NoCloseWithMiddleMouseButton`` — disable the middle-mouse
  shortcut for closable tabs.
* ``NoTooltip`` — suppress hover tooltips on tab headers.
* ``FittingPolicyResizeDown`` / ``FittingPolicyScroll`` — what happens
  when the strip doesn't fit horizontally.

Per-tab flags via ``ImGuiTabItemFlags``: ``UnsavedDocument`` (marks the
header with a dot), ``SetSelected`` (a one-shot select-this-tab nudge),
``NoCloseButton`` (closable=true but no X), ``Leading``/``Trailing``
(pin to bar edges), ``NoReorder``.

Standalone vs live
==================

Same convention as the other tutorials.

.. seealso::

   Full source: :download:`examples/tutorial/tab_bar.das <../../../examples/tutorial/tab_bar.das>`

   Features-side demos: ``examples/features/containers_layout.das`` —
   tab_bar + tree_node + collapsing_header composition;
   ``examples/features/tab_item_button.das`` — the ``tab_item_button``
   widget for an action-button styled as a tab.

   Sibling: :ref:`tutorial_containers` — umbrella container tour.

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
