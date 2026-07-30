.. _tutorial_docking:

#######################
Docking
#######################

ImGui's native docking lets the user grab a window's tab and drop it into a
side pane, the bottom pane, or even pop it out to a free-floating window —
all preserved across runs through ``imgui.ini``. The dasImgui boost layer
wraps the C++ surface in three macros: ``dockspace`` (the full-viewport
dockable region), the sibling ``dockspace_in_window`` (an explicit
``DockSpace`` nested inside a host ``window(HOST, …)``), and
``dock_window`` (each dockable panel inside either dockspace). A small
``DockBuilder`` helper seeds an initial layout so first-run users see
something meaningful before they start dragging tabs around.

Source: ``examples/tutorial/docking.das``.

************
Walkthrough
************

.. video:: docking.mp4

The recording is voiced and self-verifying, and it docks the way a *user* does
— no programmatic shortcuts. It drives two REAL synthetic mouse drags: it grabs
the node splitter between Explorer and Source and drags it to widen the left
pane, then grabs Output's tab and drags it onto Source to stack the two as tabs.
The synthetic mouse drives ImGui's ``SplitterBehavior`` and its
window-move + dock-preview + drop path exactly like a hand on the mouse would.
Each drag asserts the layout actually moved (the resized pane's ``size`` changed;
the re-docked panel's ``dock_id`` changed); a no-op drag aborts the recording at
teardown rather than shipping a clip where nothing happened.

.. literalinclude:: ../../../examples/tutorial/docking.das
   :language: das
   :linenos:

Requires
========

Same backend + boost layer as :ref:`tutorial_layout`, but layout helpers are
replaced by the docking module:

* ``imgui/imgui_docking_builtin`` — the ``dockspace``,
  ``dockspace_in_window``, and ``dock_window`` macros, plus the
  ``DockBuilder*`` bindings cherry-picked from ImGui's internal API.

The flag that lights it up
==========================

ImGui docking is gated by a single io flag. Without it, ``DockSpace`` calls
render nothing and ``dock_window`` panels behave like ordinary windows:

.. code-block:: das

   io.ConfigFlags |= ImGuiConfigFlags.DockingEnable

This goes in ``init()`` once per session.

Seeding the layout
==================

ImGui will happily start with every dockable window stacked in a single
tab-bar — the user is expected to drag tabs into place. For a tutorial we
ship a default arrangement via ``DockBuilder``:

.. code-block:: das

   DockBuilderRemoveNode(dock_id)                          // clear any prior state
   DockBuilderAddDockSpaceNode(dock_id, flags)             // fresh dockspace root
   DockBuilderSetNodeSize(dock_id, ImVec2(vp.Size.x, vp.Size.y))
   var left_id, right_id : uint
   DockBuilderSplitNode(dock_id, ImGuiDir.Left, 0.25f, left_id, right_id)
   var top_id, bottom_id : uint
   DockBuilderSplitNode(right_id, ImGuiDir.Up, 0.6f, top_id, bottom_id)
   DockBuilderDockWindow("Explorer", left_id)
   DockBuilderDockWindow("Source",   top_id)
   DockBuilderDockWindow("Output",   bottom_id)
   DockBuilderFinish(dock_id)

``DockBuilderDockWindow`` matches by window *title string* — the same string
you pass to ``dock_window(NAME, (text = "Explorer"))``. The boost macro
doesn't auto-derive the title from the identifier so the binding is
explicit.

The setup is gated on ``state.has_initial_layout`` so it runs once per
session — or after ``imgui_dock_reset`` flips the flag back to false.

Variant: dockspace inside a host window
=======================================

``dockspace`` wraps ``DockSpaceOverViewport`` — the dock region claims the
entire OS window. The sibling ``dockspace_in_window`` wraps the explicit
``DockSpace(id, size, flags, null)`` call so the dock node lives INSIDE
an enclosing ``window(HOST, ...)`` rather than over the viewport. Use
this when the host window needs its own menu bar, decorations, or a
floating / moveable frame around the dockable area.

.. code-block:: das

   SetNextWindowSize(ImVec2(680.0f, 440.0f), ImGuiCond.FirstUseEver)
   window(HOST, (text = "Editor",
                 closable = false,
                 flags = ImGuiWindowFlags.MenuBar |
                         ImGuiWindowFlags.NoDocking)) {
       menu_bar(HOST_MENU) {
           menu(FILE_MENU, (text = "File", enabled = true)) {
               menu_item(SAVE_ITEM, (text = "Save"))
           }
       }
       dockspace_in_window(DS, (size = float2(0.0f, 0.0f),
                                flags = ImGuiDockNodeFlags.None)) {
           dock_window(FILES, (text = "Files", closable = false,
                               flags = ImGuiWindowFlags.None)) { ... }
           dock_window(OUTPUT, (text = "Output", closable = false,
                                flags = ImGuiWindowFlags.None)) { ... }
       }
   }

The host's ``ImGuiWindowFlags.MenuBar`` / ``NoDocking`` live on the
``window`` call, not on ``dockspace_in_window`` — the dockspace container
only manages the dock node. ``size = (0,0)`` (typical) fills the host's
available content region after the menu bar.

``DS.dock_id`` is captured for ``DockBuilder*`` layout calls the same way
as ``dockspace`` — the choice between the two is purely about whether
you want the host frame around the dock area.

See :download:`examples/features/dockspace_in_window.das
<../../../examples/features/dockspace_in_window.das>` for the full
scene.

The frame loop
==============

The ``dockspace(DOCK_ROOT, ...)`` macro wraps ``DockSpaceOverViewport`` —
the dock region is the entire OS window. Inside its block, each
``dock_window(NAME, ...)`` is a Begin/End-wrapped panel that participates
in the docking system. Path-prefixing works the same as containers
(``window`` / ``child`` / ``tab_bar``): ``dock_window(EXPLORER) {
button(REFRESH_BTN, ...) }`` registers the button under
``DOCK_ROOT/EXPLORER/REFRESH_BTN``.

The ``closable = true`` option (on ``OUTPUT`` here) wires the X-button in
the tab to ``state.open`` — closing the panel without rebuilding the
layout.

Standalone vs live
==================

Same as previous tutorials — ``main()`` runs the loop standalone;
``daslang-live`` invokes ``init`` / ``update`` / ``shutdown`` directly.
ImGui's docking state is preserved across reloads because the ImGui
context survives reload (``imgui_live`` serializes the context pointer
through the reload, and the dock state lives inside that context).

Driving from outside
====================

Several live commands cover the docking surface. Targets are path-qualified
— the dockspace pushes its name onto the path, so panel targets are
``DOCK_ROOT/<name>``:

.. code-block:: bash

   # Pop Output out into a floating window
   curl -X POST -d '{"name":"imgui_undock","args":{"target":"DOCK_ROOT/OUTPUT"}}' \
        localhost:9090/command

   # Reposition the floating window (also works on docked windows — ImGui ignores
   # the SetNextWindowPos while a window is docked, so this is most useful after
   # imgui_undock). w/h are optional.
   curl -X POST -d '{"name":"imgui_set_window_pos","args":{"target":"DOCK_ROOT/OUTPUT","value":{"x":580,"y":220,"w":360,"h":220}}}' \
        localhost:9090/command

   # Reset the dockspace back to the default layout
   curl -X POST -d '{"name":"imgui_dock_reset","args":{"target":"DOCK_ROOT"}}' \
        localhost:9090/command

   # Close the OUTPUT panel (X-button equivalent — closable=true required)
   curl -X POST -d '{"name":"imgui_close","args":{"target":"DOCK_ROOT/OUTPUT"}}' \
        localhost:9090/command

   # Raise a panel to the front of its dock node — selects its tab when stacked
   curl -X POST -d '{"name":"imgui_raise","args":{"target":"DOCK_ROOT/OUTPUT"}}' \
        localhost:9090/command

``imgui_dock`` is the inverse of ``imgui_undock`` — it takes a ``value`` of
type ``uint`` (a dock-node id from a prior ``DockBuilder*`` call) and
re-docks the panel into that node. ``imgui_set_window_pos`` is the
companion you'll usually pair with ``imgui_undock``, since a freshly
undocked window picks its position from ``imgui.ini`` (or ``(0,0)`` if
the window has never floated). ``imgui_raise`` brings a panel to the front
of its dock node — when several panels share a node (so they render as a tab
strip) it selects that panel's tab; it's the automation counterpart of
clicking a tab, which a real click can't reach (the dock node's tab bar is
ImGui-internal, not a registered widget).

Next steps
==========

Style scopes are next — ``with_style`` for pushing colors and metrics
across a sub-tree of widgets, balanced pop, and how nesting stacks.

.. seealso::

   Full source: :download:`examples/tutorial/docking.das <../../../examples/tutorial/docking.das>`

   Richer reference: ``examples/features/dock_basic.das`` — same boost
   surface with a 4-panel initial layout and a wider widget set.

   Integration test: ``tests/integration/test_docking_basic.das`` —
   registration, initial-layout geometry, and live-command round-trips.

   Previous tutorial: :ref:`tutorial_layout`

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
