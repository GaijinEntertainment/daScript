.. _tutorial_tree_node:

#######################
Tree node
#######################

``tree_node`` is the block-arg ``[container]`` form of ImGui's
``TreeNodeEx``: branch headers that fold-and-expand with an auto-paired
``TreePop``. The wrapper's signature:

.. code-block:: das

   tree_node(IDENT, (text = "...",
                     flags = ImGuiTreeNodeFlags....)) {
       // body — runs only while the chevron is expanded (TreeNodeEx == true)
   }

Body invocation gated on ImGui returning ``true`` for ``TreeNodeEx``;
``TreePop`` only fires when the body was invoked. ``TreeNodeState``
mirrors the per-frame open status (``opened``) and exposes the
``pending_open`` / ``pending_close`` flags so ``imgui_open`` /
``imgui_close`` can drive the chevron from outside.

Source: ``examples/tutorial/tree_node.das``.

************
Walkthrough
************

.. video:: tree_node.mp4

.. literalinclude:: ../../../examples/tutorial/tree_node.das
   :language: das
   :linenos:

Requires
========

Already in the baseline boost layer:

* ``imgui/imgui_containers_builtin`` — ``tree_node`` container.
* ``imgui/imgui_widgets_builtin`` — ``slider_float``, ``checkbox``,
  ``text``, plus the leaf ``tree_node_ex``.

Nesting and path composition
============================

Tree nodes nest naturally. Each level's IDENT pushes onto the path
hash, so a slider three levels deep registers at
``TN_WIN/RENDER_TREE/CAMERA_SUBTREE/RENDER_FOV``:

.. code-block:: das

   tree_node(RENDER_TREE, (text = "Render", flags = ImGuiTreeNodeFlags.None)) {
       tree_node(CAMERA_SUBTREE, (text = "Camera",
                                  flags = ImGuiTreeNodeFlags.DefaultOpen)) {
           slider_float(RENDER_FOV, (text = "fov"))
       }
       tree_node(MESH_SUBTREE, (text = "Mesh",
                                flags = ImGuiTreeNodeFlags.None)) {
           checkbox(MESH_WIRE, (text = "Wireframe"))
       }
   }

When ``RENDER_TREE`` is collapsed, neither subtree's body runs — the
sliders/checkboxes aren't in the snapshot until the user expands
``RENDER_TREE``. The state structs themselves (``CAMERA_SUBTREE``,
``MESH_SUBTREE``) still exist; their ``opened`` field is false.

Open / close from outside
=========================

Two channels feed open/close into the wrapper:

* ``state.pending_open = true`` — app code anywhere. Next frame the
  wrapper applies ``SetNextItemOpen(true, Always)`` before TreeNodeEx
  runs. Cleared on consume.
* ``imgui_open`` / ``imgui_close`` — live commands routed by path.

.. code-block:: bash

   curl -X POST -d '{"name":"imgui_open","args":{"target":"TN_WIN/PHYSICS_TREE"}}' \
        localhost:9090/command
   curl -X POST -d '{"name":"imgui_close","args":{"target":"TN_WIN/PHYSICS_TREE"}}' \
        localhost:9090/command

Both flow through ``state.pending_open`` / ``state.pending_close`` —
the dispatcher just walks the registry and flips the flag. ``Always``
in the ``SetNextItemOpen`` call means the override wins over ImGui's
stored chevron state for that one frame.

Reading state.opened
====================

``state.opened`` is a per-frame mirror of TreeNodeEx's bool return.
A snapshot reports it under ``opened``:

.. code-block:: das

   var PHYSICS_TREE : TreeNodeState
   // After the frame:
   //   PHYSICS_TREE.opened : bool   // chevron expanded this frame?
   //   PHYSICS_TREE.flags  : ImGuiTreeNodeFlags   // sticky

Drivers verify "user expanded this tree" by reading ``opened==true``.
Note that ``opened`` is set to ``im_open`` BEFORE the body runs — the
wrapper writes it as soon as TreeNodeEx returns, so even a body that
panics still leaves a correct ``opened`` for the snapshot.

ImGuiTreeNodeFlags
==================

The third arg's bitfield. Most useful:

* ``DefaultOpen`` — first-render expansion; subsequent frames respect
  user input. The Render-Camera example uses this so the sub-tree is
  open by default after a clean launch.
* ``OpenOnArrow`` — only the chevron click toggles; clicking the label
  itself doesn't expand. Pair with a Selectable-row pattern.
* ``OpenOnDoubleClick`` — single click selects, double click toggles.
* ``Leaf`` — render the row without a chevron. The body is required to
  not exist (or be empty); use ``tree_node_ex`` (the [widget] leaf form)
  for nodes that should be selectable rows without children. See
  ``examples/features/tree_node_ex.das``.
* ``Bullet`` — bullet point instead of chevron. Pair with ``Leaf``.
* ``Framed`` — solid background under the header strip.
* ``NoTreePushOnOpen`` — open the node WITHOUT indenting the body. Used
  by ``tree_node_ex`` rails where the body lives outside the conditional.
* ``SpanAllColumns`` / ``SpanAvailWidth`` / ``SpanFullWidth`` — hit-test
  region tweaks for trees inside tables.

tree_node vs tree_node_ex
=========================

Same backing struct, different ergonomics:

* ``tree_node(IDENT, (...)) { body }`` (this tutorial) — ``[container]``
  block-arg. Body runs gated on the open chevron; wrapper handles
  ``TreePop``.
* ``tree_node_ex(IDENT, (...))`` — ``[widget]`` leaf. Returns a bool;
  caller pairs ``tree_pop()`` (the rail) manually, *outside* an
  ``if (open)`` if needed. The escape hatch for "header has a sibling
  ``same_line()`` widget" or "Leaf node with selectable behavior" —
  see ``examples/features/tree_node_ex.das`` for the rail-side demo and
  ``examples/features/tree_node_open_manual.das`` for the sibling
  same_line pattern.

When in doubt, use ``tree_node`` — block-arg auto-pairing is harder to
get wrong.

Standalone vs live
==================

Same convention as the other tutorials.

.. seealso::

   Full source: :download:`examples/tutorial/tree_node.das <../../../examples/tutorial/tree_node.das>`

   Features-side demos:
   ``examples/features/containers_layout.das`` — tree_node alongside
   tab_bar + collapsing_header;
   ``examples/features/tree_node_open_manual.das`` — leaf-form manual
   ``tree_pop`` pairing with a sibling button on the header row;
   ``examples/features/tree_node_ex.das`` — Leaf / Bullet / OpenOnArrow
   variations against the [widget] leaf form.

   Sibling: :ref:`tutorial_collapsing_header` — same flags-and-state shape
   without the chevron / TreePop dance.

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
