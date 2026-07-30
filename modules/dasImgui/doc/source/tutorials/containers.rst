.. _tutorial_containers:

#######################
Containers
#######################

The boost layer ships a family of containers — block-arg wrappers
around ImGui's ``Begin*``/``End*`` pairs that share three properties:

* **Block-arg shape** — the body runs once per frame inside the
  begin/end pair. No manual ``End()`` to forget.
* **Path push** — leaf widgets inside the block register under
  ``<container>/<leaf>``. Every container contributes a path segment,
  just like ``window`` and ``with_id`` did in earlier tutorials.
* **Open-state via pending flags** — ``state.pending_open = true``
  queues the container to open next frame; ``pending_close = true``
  closes it. Live commands ``imgui_open`` / ``imgui_close`` mutate the
  same flag, so app code, external drivers, and the chrome's
  close-button share one channel.

This tutorial covers four representative containers: ``menu_bar`` +
``menu`` + ``menu_item``, ``tab_bar`` + ``tab_item``, ``popup``, and
``item_tooltip``. The features-side demos
(``examples/features/containers_*.das``) cover the rest:
``child`` / ``group`` (window family), ``tree_node`` /
``collapsing_header`` (layout family), ``popup_modal`` /
``tooltip`` / ``combo_select`` / ``list_box`` (overlay family).

Source: ``examples/tutorial/containers.das``.

************
Walkthrough
************

.. video:: containers.mp4

The recording drives all four containers with real synthetic input and
self-verifies each step. It flips the General tab's ``WIRE`` checkbox,
then **clicks the Audio tab header** and asserts the switch by its effect:
``AUDIO_TAB``'s ``MUTE`` starts rendering while ``GENERAL_TAB``'s ``WIRE``
*stops* — a direct proof that only the open tab's body runs. It opens the
popup from the button's ``pending_open`` flag (asserting the popup body
appears), toggles the ``VSync`` checkbox inside it, closes it with the
popup's own ``Close`` button (asserting the body stops), and finally hovers
the button to bring up the ``item_tooltip``. Any step that failed to land
aborts the recording.

.. literalinclude:: ../../../examples/tutorial/containers.das
   :language: das
   :linenos:

Requires
========

One extra module on top of the baseline boost layer:

* ``imgui/imgui_containers_builtin`` — defines every container macro
  used here. The window/child/group/menu/tab/popup/tooltip surface
  all lives in this one module.

menu_bar / menu / menu_item
===========================

A menu bar attaches to a window that opts in via
``ImGuiWindowFlags.MenuBar``. Inside the ``window(...)`` block,
``menu_bar(...) { ... }`` provides the bar, ``menu(...) { ... }``
provides each drop-down, and ``menu_item(...)`` provides the leaves:

.. code-block:: das

   window(CONT_WIN, (text = "containers", closable = false,
                     flags = ImGuiWindowFlags.MenuBar)) {
       menu_bar(MAIN_BAR) {
           menu(FILE_MENU, (text = "File", enabled = true)) {
               menu_item(NEW_ITEM,  (text = "New",  shortcut = "Ctrl+N"))
               menu_item(OPEN_ITEM, (text = "Open", shortcut = "Ctrl+O"))
           }
       }
       // ... rest of the window
   }

Path push composes: ``NEW_ITEM`` ends up at the registry path
``CONT_WIN/MAIN_BAR/FILE_MENU/NEW_ITEM``. ``MenuBarState`` is
stateless; ``MenuState`` carries an observed ``opened`` flag (per-frame
whether the dropdown is expanded). Menu visibility is hover/click-driven
by ImGui itself — there's no ``imgui_open`` channel into the menu, so
drivers that want to fire a menu item directly use
``imgui_click`` against the ``menu_item`` target.

tab_bar / tab_item
==================

``tab_bar`` holds one or more ``tab_item`` blocks; ImGui owns the
active-tab selection:

.. code-block:: das

   tab_bar(MAIN_TABS, (text = "MainTabs", flags = ImGuiTabBarFlags.None)) {
       tab_item(GENERAL_TAB, (text = "General", closable = false,
                              flags = ImGuiTabItemFlags.None)) {
           Text("Tabs share a window; only the active tab renders.")
           checkbox(WIRE, (text = "Wireframe"))
       }
       tab_item(AUDIO_TAB, (text = "Audio", ...)) { ... }
       tab_item(INFO_TAB,  (text = "Info",  ...)) { ... }
   }

Only the **active tab's block runs each frame** — widgets inside
inactive tabs aren't in the registry that frame, so a snapshot taken
while ``GENERAL_TAB`` is active won't list any ``AUDIO_TAB`` children.
``TabItemState.pending_open`` controls the closable-tab visibility
(skip BeginTabItem entirely when ``open=false``), but it does NOT
programmatically select the active tab — that's an ImGui internal
state, set by clicking the tab header. Each ``tab_item`` registers its
header's bbox, so a driver switches tabs the way a user does: an
``imgui_click`` on the ``tab_item`` target (e.g.
``CONT_WIN/MAIN_TABS/AUDIO_TAB``) lands on the header and selects it.

popup
=====

A popup renders only when explicitly opened. The state struct's
``pending_open`` flag is the open-channel; the renderer calls
``OpenPopup`` next frame:

.. code-block:: das

   if (button(OPEN_POPUP_BTN, (text = "Open options"))) {
       OPTIONS_POPUP.pending_open = true
   }
   popup(OPTIONS_POPUP, (text = "OptionsPopup",
                         flags = ImGuiWindowFlags.None)) {
       Text("Options")
       checkbox(OPT_VSYNC, (text = "VSync"))
       if (button(POPUP_CLOSE_BTN, (text = "Close"))) {
           OPTIONS_POPUP.pending_close = true
       }
   }

External drivers reach the same flag via ``imgui_open`` /
``imgui_close`` — three control surfaces (app code, live commands, the
close-button chrome) all funnel through the popup state's pending
flags. The popup also auto-closes when the user clicks outside it
(ImGui's normal popup behavior).

item_tooltip — hover-gated overlay
==================================

ImGui's ``BeginItemTooltip`` checks ``IsItemHovered()`` internally, so
the block only runs while the immediately-preceding widget is hovered.
No manual gate:

.. code-block:: das

   button(HOVER_BTN, (text = "Hover me"))
   item_tooltip(HOVER_TIP) {
       Text("This text appears on hover.")
       Text("Driven by BeginItemTooltip (auto-gated).")
   }

For tooltips whose own gating logic differs from "previous item
hovered" — say, tooltips on an entire window or a custom hover state —
use the lower-level ``tooltip(...)`` container and gate it manually
(see ``examples/features/containers_overlay.das``).

Standalone vs live
==================

Same convention as previous tutorials.

Driving from outside
====================

Path-qualified targets for every container leaf:

.. code-block:: bash

   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command
   # Registers: CONT_WIN/MAIN_BAR/FILE_MENU/NEW_ITEM, CONT_WIN/MAIN_TABS/GENERAL_TAB/WIRE, ...

   curl -X POST -d '{"name":"imgui_open","args":{"target":"CONT_WIN/OPTIONS_POPUP"}}' \
        localhost:9090/command
   curl -X POST -d '{"name":"imgui_close","args":{"target":"CONT_WIN/OPTIONS_POPUP"}}' \
        localhost:9090/command

   curl -X POST -d '{"name":"imgui_click","args":{"target":"CONT_WIN/MAIN_BAR/FILE_MENU/NEW_ITEM"}}' \
        localhost:9090/command

Note that menu items receive ``imgui_click`` directly — they're click
targets, not open/close targets.

Context popups
==============

``popup_context_item`` is the right-click-context sibling of ``popup`` —
ImGui drives open/close internally based on the previous item receiving
a right-click; the wrapper just gates the body on Begin returning true:

.. code-block:: das

   require imgui/imgui_containers_builtin

   button(TARGET_BTN, (text = "Right-click me"))
   popup_context_item(TARGET_CTX, (str_id = "target_ctx",
                                   flags = ImGuiPopupFlags.MouseButtonRight)) {
       if (menu_item(ACTION_RENAME, (text = "Rename", shortcut = "F2"))) {
           // ...
       }
       if (menu_item(ACTION_DELETE, (text = "Delete", shortcut = "Del"))) {
           // ...
       }
   }

The popup is keyed off the **previously submitted item** — submission
order matters, and ``popup_context_item`` registers under its own path
in the snapshot. Use ``imgui_click`` to drive menu items from outside.

Feature demo: ``examples/features/popup_context_item.das``.

Next steps
==========

So far every tutorial has assumed the standard standalone/live run.
Next up is live-reload itself — the ``daslang-live`` workflow, how
``[live_command]`` / ``[before_reload]`` / ``[after_reload]`` plumb
in, and what survives a reload (state structs, ImGui context, the
HTTP server) versus what gets rebuilt.

.. seealso::

   Full source: :download:`examples/tutorial/containers.das <../../../examples/tutorial/containers.das>`

   Richer references:

   * ``examples/features/containers_window.das`` — window / child /
     group with closable second window
   * ``examples/features/containers_layout.das`` — tab_bar plus
     tree_node, collapsing_header
   * ``examples/features/containers_overlay.das`` — popup_modal,
     tooltip, combo_select, list_box

   Previous tutorial: :ref:`tutorial_state_telemetry`

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
