.. _tutorial_popup_window:

#######################
Popup window
#######################

ImGui exposes three popup-opening patterns: auto-trigger from a preceding
item (``BeginPopupContextItem``), auto-trigger from anywhere in the window
(``BeginPopupContextWindow``), and **manual-trigger** — the caller decides
when to open. The boost layer wraps each with a dedicated ``[container]``:

* ``popup_context_item(IDENT, (str_id, flags))`` — right-click on the
  previously-submitted item opens the popup.
* ``popup_context_window(IDENT, (str_id, flags))`` — right-click anywhere
  inside the enclosing window opens it.
* ``popup_window(IDENT, (str_id, flags))`` — no auto-trigger; the caller
  drives ``open_popup(str_id, flags)`` from any custom predicate.

The manual form is what the cpp demo's per-column table-context section
needs: one shared ``str_id`` across multiple ``PushID`` scopes, each
deciding for itself when to ``OpenPopup`` based on hover state, key
combinations, or any predicate that has no preceding "item" to attach to.

Source: ``examples/tutorial/popup_window.das``.

************
Walkthrough
************

.. video:: popup_window.mp4

The recording drives both trigger scopes for real: clicking ``Open via
button`` fires ``open_popup`` and the popup body renders; picking a fruit
calls ``close_current_popup`` and the ``Last pick`` line updates;
right-clicking anywhere in the hover region opens the *same* popup through
the second scope. Each open is verified against the popup body's
rendered-this-frame state and each pick against the status line, so a
trigger that failed to open or close the popup would abort the recording.

.. literalinclude:: ../../../examples/tutorial/popup_window.das
   :language: das
   :linenos:

Requires
========

Already in the baseline boost layer:

* ``imgui/imgui_containers_builtin`` — ``popup_window``, ``open_popup``,
  ``close_current_popup``.
* ``imgui/imgui_id_builtin`` — ``with_id`` for the per-scope ID-stack push.

Two scopes, one ``str_id``
==========================

ImGui keys a popup's open state by ``str_id`` *under the current ID stack*.
When two render sites want the same logical popup ("pick a fruit") but
need independent open state, push a distinct ``with_id("scope") { ... }``
around each:

.. code-block:: das

   with_id("via_button") {
       if (button(OPEN_BTN, (text = "Open via button"))) {
           open_popup("fruit_picker")
       }
       popup_window(BTN_POPUP, (str_id = "fruit_picker", flags = ImGuiWindowFlags.None)) {
           // body
       }
   }

   with_id("via_region") {
       // ...custom hover/release trigger...
       if (in_region && IsMouseReleased(ImGuiMouseButton.Right)) {
           open_popup("fruit_picker")
       }
       popup_window(REG_POPUP, (str_id = "fruit_picker", flags = ImGuiWindowFlags.None)) {
           // body
       }
   }

The two scopes see the *same* ``str_id``, but ImGui's ID-stack push from
``with_id`` mangles the open state per scope. The shared body factor lets
both scopes call the same private ``draw_picker_body()`` helper.

Custom predicates
=================

Because there's no auto-trigger, the caller is free to compose any
predicate that should open the popup. The features demo combines a
hover-region check with a right-release:

.. code-block:: das

   text(HINT, (text = "(hover this region, right-click)"))
   let region_min = GetItemRectMin()
   let region_max = GetItemRectMax()
   let in_region = (IsMouseHoveringRect(region_min, region_max, true) &&
                    !IsAnyItemHovered())
   if (in_region && IsMouseReleased(ImGuiMouseButton.Right)) {
       open_popup("fruit_picker")
   }

This shape is what the cpp ``imgui_demo``'s ``"Tables > Context menus"``
section uses to open one ``"MyPopup"`` per column. The
``examples/imgui_demo/tables.das`` port reuses ``popup_window`` for
exactly that loop.

State
=====

``PopupWindowState`` is empty — ImGui owns the open lifecycle by
``str_id`` and the boost wrapper just brackets the body. The state
global is what the registry walks to find the widget by IDENT; the
snapshot reports ``kind="popup_window"`` for every registered instance
regardless of whether the popup is currently visible.

Standalone vs live
==================

Same convention as the other tutorials.

.. seealso::

   Full source: :download:`examples/tutorial/popup_window.das <../../../examples/tutorial/popup_window.das>`

   Features-side demo: ``examples/features/popup_window.das``.

   Integration test: ``tests/integration/test_popup_window.das``.

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
