.. _tutorial_main_menu_bar:

#######################
Main menu bar
#######################

Two related v2 additions live together in this tutorial. The first is
``main_menu_bar(IDENT) { ... }`` — the screen-top bar (``BeginMainMenuBar``
/ ``EndMainMenuBar``), distinct from ``menu_bar(IDENT) { ... }`` which
attaches to the current window (``BeginMenuBar``, requires
``ImGuiWindowFlags.MenuBar``). The second is the
``menu_item`` / ``menu_label`` split for the leaves under a menu:

* ``menu_item(IDENT, (text=, shortcut=, enabled=))`` — toggleable.
  ``ToggleState.value`` flips on click.
* ``menu_label(IDENT, (text=, shortcut=, selected=, enabled=))`` —
  static. ``ClickState.click_count`` accumulates; ``selected`` is a
  fixed check-mark (not driven by the click).

``menu_label`` is the right rail for section headers
(``selected=false, enabled=false``), permanent-check labels
(``selected=true``), and any "do something, don't toggle" menu entry.

.. code-block:: das

   main_menu_bar(MAIN_BAR) {
       menu(FILE_MENU, (text = "File", enabled = true)) {
           menu_item(MI_DARKMODE, (text = "Dark mode", shortcut = "Ctrl+D"))
           menu_item(MI_AUTOSAVE, (text = "Auto-save", shortcut = ""))
           separator(FILE_SEP)
           menu_label(FILE_HEADER, (text = "(recent files)",
                                    shortcut = "", selected = false,
                                    enabled = false))
           menu_label(FILE_RECENT1, (text = "demo.das"))
           menu_label(FILE_STAR, (text = "Star this project",
                                  shortcut = "", selected = true))
       }
   }

Source: ``examples/tutorial/main_menu_bar.das``.

************
Walkthrough
************

.. video:: main_menu_bar.mp4

.. literalinclude:: ../../../examples/tutorial/main_menu_bar.das
   :language: das
   :linenos:

Requires
========

Baseline boost layer (``imgui/imgui_boost_v2`` re-exports
``imgui/imgui_containers_builtin``). No extra modules.

main_menu_bar vs menu_bar
=========================

The two bar variants pick opposite anchor strategies:

* ``main_menu_bar(IDENT) { ... }`` attaches to the viewport — sits at the
  top of the screen, persists across windows, ImGui draws the chrome.
  Use it when there's exactly one bar in the app.
* ``menu_bar(IDENT) { ... }`` attaches to the current window — appears
  inside that window's titlebar area. The host window must have
  ``ImGuiWindowFlags.MenuBar`` set, or the menu_bar render is a no-op.
  Use it for per-document menus inside a tabbed editor, the imgui_demo's
  own menu bar, or any window-local command surface.

Both expose the same ``menu()`` / ``menu_item`` / ``menu_label`` children;
only the anchor differs.

menu_item vs menu_label
=======================

``menu_item`` and ``menu_label`` look identical visually (text + optional
shortcut + optional check-mark) but back different state structs:

* ``menu_item`` carries ``ToggleState`` — the ``value`` bool flips on
  every click. Use it for boolean app-state toggles ("Dark mode",
  "Auto-save", "Show hidden files").
* ``menu_label`` carries ``ClickState`` — the ``click_count`` int
  accumulates without touching ``selected``. Use it for actions
  ("Undo", "Quit", "About") and for static labels with a fixed check
  (``selected=true``) or no check at all.

Combine ``selected=`` and ``enabled=`` on ``menu_label`` to get section
headers (``selected=false, enabled=false`` greys the entry and shows no
check) or permanent-check items (``selected=true, enabled=true`` shows
the check and stays clickable).

Driving from outside
====================

The full menu hierarchy is routable once a menu is open:

.. code-block:: bash

   curl -X POST -d '{"name":"imgui_click","args":{"target":"MAIN_BAR/FILE_MENU/MI_DARKMODE"}}' \
        localhost:9090/command

The bar's own header (``MAIN_BAR``) has a degenerate bbox (the chrome has
no clickable "header"). The first-level ``menu()`` children
(``MAIN_BAR/FILE_MENU``, ``MAIN_BAR/EDIT_MENU``) DO register routable
headers. Submenu items only register a bbox *while their parent menu is
open*, so external drivers wanting to click a menu_item have to open the
parent first (synth click on the header, wait one frame, then click the
child). The walkthrough above does exactly this with real clicks, dropping
straight DOWN the open menu's column onto each item: a diagonal travel to an
item that sits off-axis from its header would clip a sibling header, and
ImGui hover-switches the open menu mid-travel (the click then lands in the
wrong menu). Menu items span the full menu width, so a vertical drop at the
header's center-x always lands on the item.

Snapshot shape
==============

``MI_DARKMODE.value`` is the live toggle bool; ``EDIT_UNDO.click_count``
is the accumulating action counter. Both surface in the snapshot under
their qualified path:

.. code-block:: bash

   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command \
       | jq '.globals."MAIN_BAR/FILE_MENU/MI_DARKMODE".payload'

The tutorial's body window prints both per-frame so the state changes are
visible alongside the menu interactions.

.. seealso::

   Full source: :download:`examples/tutorial/main_menu_bar.das <../../../examples/tutorial/main_menu_bar.das>`

   Integration tests: ``tests/integration/test_menu_main.das`` and ``tests/integration/test_menu_label_static.das``.

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
