.. _tutorial_drag_drop:

#######################
Drag and drop
#######################

ImGui's drag-drop API is two paired ``Begin*``/``End*`` calls: one
attached to the previously submitted item that's being dragged, one
attached to the previously submitted item that's receiving. Both
``Begin*`` calls return ``false`` unless ImGui's internal drag state
machine says "drag-active" or "hover-over-target with payload"; calling
``End*`` when ``Begin*`` returned false is undefined behavior.

The boost layer ships two ``[container]``s — ``drag_drop_source`` and
``drag_drop_target`` — that gate their bodies on the corresponding
``Begin*`` return. Payload set/accept stay raw calls inside the body
because the data is caller-owned and crosses widget boundaries.

Source: ``examples/features/drag_drop.das``.

************
Walkthrough
************

.. video:: drag_drop.mp4

The recording introduces the source button and the drop target, then performs a
real drag of the source onto the target. On release the target accepts the
``MY_INT`` payload — ``Target received value`` flips to ``42`` and
``Drops accepted`` ticks to ``1``. The drag is synthesized through the same
synth-mouse pipeline a test would use, and the recording asserts the drop
actually landed (see *Driving from outside* below).

.. literalinclude:: ../../../examples/features/drag_drop.das
   :language: das
   :linenos:

Requires
========

Same backend + boost layer as the other container tutorials, plus the
``drag_drop_source`` / ``drag_drop_target`` macros from
``imgui/imgui_containers_builtin`` (which is already required by the
container family).

Source side
===========

Attach a ``drag_drop_source`` immediately after the source widget. ImGui
implicitly binds it to the previously submitted item:

.. code-block:: das

   button(SOURCE_BTN, (text = "SOURCE"))
   drag_drop_source(SOURCE_DD, (flags = ImGuiDragDropFlags.None)) {
       unsafe {
           SetDragDropPayload("MY_INT", addr(PAYLOAD_VALUE),
                              uint64(typeinfo sizeof(PAYLOAD_VALUE)),
                              ImGuiCond.Once)
       }
       Text("Dragging: {PAYLOAD_VALUE}")
   }

The body runs **only while the drag is active** — after the user has
pressed the mouse on ``SOURCE_BTN`` and dragged past ImGui's
``io.MouseDragThreshold``. Inside the body:

* ``SetDragDropPayload(type, data, size, cond)`` publishes the payload.
  ``type`` is a short string (max 32 chars) that the target compares
  against; ``data`` is a raw pointer with caller-controlled lifetime
  (ImGui copies it into its internal buffer on each call, so a stack
  pointer is fine inside the body).
* Any rendering call (``Text``, ``Image``, etc.) draws the drag preview
  tooltip that follows the cursor.

Target side
===========

Attach a ``drag_drop_target`` immediately after the target widget:

.. code-block:: das

   button(TARGET_BTN, (text = "TARGET"))
   drag_drop_target(TARGET_DD) {
       let payload = AcceptDragDropPayload("MY_INT", ImGuiDragDropFlags.None)
       if (payload != null) {
           unsafe {
               RECEIVED = (reinterpret<int? const>(payload.Data))[0]
           }
       }
   }

The body runs **only while a drag is hovering this widget**. Inside:

* ``AcceptDragDropPayload(type, flags)`` returns a non-null
  ``ImGuiPayload?`` on the frame the user releases over the target with
  a payload whose type matches ``type``; otherwise it returns null
  (including on every frame the drag is hovering but not yet dropped).
* The returned payload's ``Data`` field is a ``void? const`` —
  ``reinterpret<T? const>(payload.Data)[0]`` reads the typed value back.

Driving from outside
====================

Playwright exposes a high-level ``drag_to`` that bridges source-bbox →
target-bbox automatically:

.. code-block:: das

   require imgui/imgui_playwright
   drag_to(app, "MAIN_WIN/SOURCE_BTN", "MAIN_WIN/TARGET_BTN", steps = 8)

It reads both bboxes from a fresh snapshot, computes ``(dx, dy) =
target_center - source_center``, and composes an ``imgui_mouse_play``
timeline — warp to the source, press, ``steps``-scaled interpolated
moves to the target, release. ImGui's drag state machine activates around
the second or third move (depending on ``io.MouseDragThreshold``) and
resolves on release.

The lower-level ``drag(app, target, dx, dy, steps)`` is available when
you want absolute offsets — useful for testing drag-without-drop or
drag-cancel paths.

**Verifying the drop.** ``drag_drop_target`` surfaces a running ``accepted``
count in its telemetry payload — it counts the drops ImGui actually delivers
(``GetDragDropPayload().Delivery``), so a driver or test can assert the drop
*landed* without reaching into the example's caller-owned globals:

.. code-block:: das

   drag_to(app, "MAIN_WIN/SOURCE_BTN", "MAIN_WIN/TARGET_BTN")
   // accepted ticks 0 -> 1 on a real delivery over the target
   wait_for_payload_value(app, "MAIN_WIN/TARGET_DD", "accepted", 1, 300)

Next steps
==========

That's the last container tutorial. The next walkthrough switches
focus to live-reload internals.

.. seealso::

   Full source: :download:`examples/features/drag_drop.das <../../../examples/features/drag_drop.das>`

   Integration test: ``tests/integration/test_drag_drop.das``.

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.

   :ref:`Containers <tutorial_containers>` — the container family in general.
