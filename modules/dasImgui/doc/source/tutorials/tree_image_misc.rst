.. _tutorial_tree_image_misc:

####################
tree_node_ex + image
####################

Two leaf widgets the container-shaped variants don't cover.
``tree_node_ex`` is the explicit-control sibling of the ``tree_node``
container; ``image`` is the display-only sibling of ``image_button``.

.. code-block:: das

   tree_node_ex(IDENT, (text = "..", flags = ...))    // returns open bool
                                                       // caller pairs tree_pop()
   image(IDENT, (user_texture_id = tex_id, size = .., // display only
                 uv0 = .., uv1 = .., tint_col = ..,    // no ClickState
                 border_col = ..))

Source: ``examples/tutorial/tree_image_misc.das``.

************
Walkthrough
************

.. video:: tree_image_misc.mp4

The recording clicks the ``tree_node_ex`` header to expand it - the three
children appear and the node's returned open bool flips to ``true`` - then
moves to the display-only ``image``. The expand is verified against that open
flag and the image against its on-screen presence, so a click that failed to
open the node would abort the recording.

.. literalinclude:: ../../../examples/tutorial/tree_image_misc.das
   :language: das
   :linenos:

Requires
========

* ``imgui/imgui_widgets_builtin`` — ``tree_node_ex`` and ``image``.
* ``imgui/imgui_layout_builtin`` — ``tree_pop()``.
* ``imgui/imgui_boost_runtime`` — ``TreeNodeState`` / ``ImageState``.

tree_node_ex vs tree_node (container)
=====================================

The container form (``tree_node(IDENT, (text, flags)) { ... }``) is
what you reach for 90% of the time: it auto-pairs ``TreePush`` /
``TreePop``, the body block carries the children:

.. code-block:: das

   // Container form — body is the open-state branch:
   tree_node(GROUP, (text = "Group", flags = ImGuiTreeNodeFlags.None)) {
       text("  child A")
       text("  child B")
   }

``tree_node_ex`` is the leaf form: it returns the open bool, and the
caller decides what to do with it. Use it when:

* You want to render sibling content **outside** the open-state
  branch (a row of inline controls that should appear next to the
  header regardless of open state).
* You're passing ``NoTreePushOnOpen`` — that skips the TreePush, so a
  matching ``tree_pop()`` would underflow the stack. The leaf form
  makes the asymmetry explicit.
* The flag set you want (``Leaf``, ``Bullet``, ``SpanAllColumns``,
  etc.) doesn't compose well with the container body shape.

.. code-block:: das

   if (tree_node_ex(GROUP, (text = "Group",
                            flags = ImGuiTreeNodeFlags.None))) {
       text("  child A")
       text("  child B")
       tree_pop()
   }

The matching ``tree_pop()`` is YOUR responsibility — easy to forget
when refactoring. Prefer ``tree_node`` unless one of the cases above
applies.

image vs image_button
=====================

* ``image`` — display widget. Paints a texture; no ``ClickState``,
  no frame padding, no click semantics.
* ``image_button`` — click trigger. Texture-faced button with
  ``ClickState`` for accumulating clicks. See :ref:`tutorial_buttons`.

Pick by intent: if the texture is content (an avatar, a preview, an
icon strip), use ``image``. If it's an interactive surface, use
``image_button``.

.. code-block:: das

   let io & = unsafe(GetIO())
   let font_tex = io.Fonts.TexID
   if (font_tex != null) {
       image(AVATAR, (user_texture_id = font_tex,
                      size = float2(128.0f, 128.0f),
                      uv0 = float2(0.0f, 0.0f),
                      uv1 = float2(1.0f, 1.0f),
                      tint_col = float4(1.0f, 1.0f, 1.0f, 1.0f),
                      border_col = float4(0.6f, 0.6f, 0.6f, 1.0f)))
   }

``uv0`` / ``uv1`` slice into the texture (use for sprite sheets);
``tint_col`` modulates the rendered face; ``border_col`` draws a
1-pixel frame around it (transparent for no border).

The font atlas (``io.Fonts.TexID``) is always available and works as a
no-setup demo texture. Real apps load via ``stbi`` or whatever your
GL/Vulkan texture pipeline provides; pass the opaque ``ImTextureID``
(typed ``void?`` on the daslang side) as ``texture``.

Driving from outside
====================

Both widgets surface their payload via ``imgui_snapshot`` — no
``imgui_force_set`` channels (the texture is caller-owned, the open state
is local to ``tree_node_ex``'s call site):

.. code-block:: bash

   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command \
       | jq '.globals."TI_WIN/TI_TREE".payload, .globals."TI_WIN/TI_IMG".payload'

.. seealso::

   Full source: :download:`examples/tutorial/tree_image_misc.das <../../../examples/tutorial/tree_image_misc.das>`

   Container sibling: :ref:`tutorial_tree_node` (and :ref:`tutorial_collapsing_header`
   for the unbordered-open variant).

   Click sibling: :ref:`tutorial_buttons` (image_button section).

   Features-side demo: ``examples/features/display_image.das``.

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
