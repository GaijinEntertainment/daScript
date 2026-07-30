.. _tutorial_popups:

##############################
Right-click context popups
##############################

Two ``[container]`` rails that attach a right-click context menu to a
target. ``popup_context_item`` attaches to the **previously submitted
item**; ``popup_context_window`` attaches to the **enclosing window**.
Both are ``stateless_finalize`` — ImGui owns open/close internally,
the body runs only while the popup is visible.

.. code-block:: das

   popup_context_item(IDENT, (str_id = "..",
                              flags = ImGuiPopupFlags.MouseButtonRight)) {
       <menu body>
   }

   popup_context_window(IDENT, (str_id = "..",
                                flags = ImGuiPopupFlags.MouseButtonRight)) {
       <menu body>
   }

Source: ``examples/tutorial/popups.das``.

************
Walkthrough
************

.. video:: popups.mp4

The recording drives both menus with real synthetic right-clicks and
clicks a menu item from each, self-verifying that the action fired
(``PO_RENAME`` then ``PO_REFRESH`` flip to ``value = true``). After each
right-click it **waits for the menu to actually render** before
travelling onto the item — never a fixed sleep guessing the popup is
open. To explore live, run the tutorial under ``daslang-live`` and
right-click the button / empty space yourself:

.. code-block:: bash

   bin/Release/daslang-live modules/dasImgui/examples/tutorial/popups.das

.. literalinclude:: ../../../examples/tutorial/popups.das
   :language: das
   :linenos:

Requires
========

* ``imgui/imgui_containers_builtin`` — both popup containers.
* ``imgui/imgui_widgets_builtin`` — ``button`` + ``menu_item`` for the
  target + menu rows.
* ``imgui/imgui_boost_runtime`` — ``PopupContextItemState`` /
  ``PopupContextWindowState`` (stateless markers).

popup_context_item — attached to an item
========================================

``BeginPopupContextItem(str_id, flags)`` attaches the menu to whatever
item was most recently submitted. Submit the target first, then the
popup:

.. code-block:: das

   button(TARGET, (text = "Right-click me"))
   popup_context_item(CTX, (str_id = "target_ctx",
                            flags = ImGuiPopupFlags.MouseButtonRight)) {
       if (menu_item(RENAME, (text = "Rename", shortcut = "F2"))) { ... }
       if (menu_item(DELETE, (text = "Delete", shortcut = "Del"))) { ... }
   }

The order matters — ``popup_context_item`` calls ``IsItemHovered`` /
``IsItemClicked`` internally against the *previous* item, so the
popup must come after the target.

``str_id`` is the popup's stack id, and **must be non-empty**. The
ImGui C-level default ``"##ContextItem"`` fires only on a NULL
``const char*``, which daslang's string binding doesn't expose. Pass
a unique non-empty string per consumer.

popup_context_window — attached to the window
=============================================

Same shape; attaches to the **enclosing window** instead of a specific
item. When a window has *both* an item menu and a window menu, OR in
``NoOpenOverItems`` so a right-click on an item opens that item's menu
and only empty space opens the window menu:

.. code-block:: das

   window(MAIN_WIN, (text = "...")) {
       button(TARGET, (text = "Right-click me"))
       popup_context_item(ITEM_CTX, (str_id = "item_ctx",
                                     flags = ImGuiPopupFlags.MouseButtonRight)) {
           if (menu_item(RENAME, (text = "Rename"))) { ... }
       }
       popup_context_window(WIN_CTX, (str_id = "win_ctx",
                                      flags = ImGuiPopupFlags.MouseButtonRight |
                                              ImGuiPopupFlags.NoOpenOverItems)) {
           if (menu_item(REFRESH, (text = "Refresh"))) { ... }
       }
   }

Without ``NoOpenOverItems`` the window popup opens on **every**
right-click inside the window — the button's right-click included — so
the item menu can never open. The flag tells the window popup to ignore
right-clicks that land on an item, ceding them to that item's own
context menu. This is the idiomatic pairing whenever a window carries
both an item menu and a window-wide menu.

Flags
=====

``ImGuiPopupFlags`` carries the trigger options:

* ``MouseButtonLeft`` / ``MouseButtonRight`` / ``MouseButtonMiddle`` —
  which button opens. ``Right`` is the conventional default.
* ``MouseButtonMask_`` — any of the three.
* ``NoOpenOverItems`` — for ``popup_context_window``: don't open when
  the right-click lands on an item, so item context menus win (see
  above).
* ``NoOpenOverExistingPopup`` — don't open if a popup is already open.

State model — stateless_finalize
================================

The states (``PopupContextItemState`` / ``PopupContextWindowState``)
carry no ``pending_open`` / ``pending_close`` fields. ImGui drives
open/close internally on actual right-click events; there's no
``imgui_open`` / ``imgui_close`` channel. The widget *body* runs only
on the frames the popup is visible, so any per-frame logic inside the
body is naturally gated.

The asymmetry vs the ``popup_modal`` family (which DOES have
``pending_open``) reflects the fact that context popups are
*reactive* to mouse events, while modals are *proactive* — you
``imgui_open`` a modal from logic, you don't ``imgui_open`` a context
menu.

.. seealso::

   Full source: :download:`examples/tutorial/popups.das <../../../examples/tutorial/popups.das>`

   Modal sibling: :ref:`tutorial_popup_modal` — proactive popup
   driven by ``pending_open``.

   Manual-trigger sibling: :ref:`tutorial_popup_window` — stateless
   ``BeginPopup`` + caller-driven ``OpenPopup``.

   Features-side demo: ``examples/features/popup_context_item.das``.

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
