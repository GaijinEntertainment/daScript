.. _tutorial_with_disabled:

######################################
With disabled / font / button_repeat
######################################

Four stateless scope-wrappers landed alongside the v2 boost surface. They
share the same shape: a top-level function that takes a single value plus a
trailing block, pushes the corresponding ImGui scope on entry, pops on exit.
None of them emit telemetry — no ``[widget]`` / ``[container]`` annotation,
no idents — they exist to subsume the raw ``Push*`` / ``Pop*`` pairs that
v2's lint flags as invisible state changes.

* ``with_disabled(disabled, blk)`` — ``BeginDisabled`` / ``EndDisabled``.
* ``with_font(font, blk)`` — ``PushFont`` / ``PopFont``.
* ``with_button_repeat(repeat, blk)`` — ``PushButtonRepeat`` /
  ``PopButtonRepeat``.
* ``with_clip_rect(min, max, isect, blk)`` — ``PushClipRect`` /
  ``PopClipRect``.

.. code-block:: das

   with_disabled(!ENABLED_MASTER.value) {
       button(CHILD_SAVE, (text = "Save"))
       button(CHILD_LOAD, (text = "Load"))
       slider_float(CHILD_VOL, (text = "Volume"))
   }

The block's interior renders normally; the wrapper's argument drives the
ImGui-stack push that surrounds it. Toggling ``ENABLED_MASTER`` flips
``with_disabled``'s argument and the child widgets re-render greyed (or
active) on the next frame.

Source: ``examples/tutorial/with_disabled.das``.

************
Walkthrough
************

.. video:: with_disabled.mp4

The recording drives the ``with_disabled`` contrast with real synthetic
input and self-verifies each step. While the checkbox is off it clicks the
greyed ``Save`` button — nothing happens. It then ticks the checkbox on
(asserting the value flipped), clicks ``Save`` again, and asserts
``CHILD_SAVE.click_count`` is now **exactly one** — proving the earlier
greyed click never registered. Finally it presses and holds the ``+``
button under ``with_button_repeat`` and asserts the counter climbs *twice*
under the single hold — a genuine repeat stream, not one click. Any step
that failed to land would abort the recording.

.. literalinclude:: ../../../examples/tutorial/with_disabled.das
   :language: das
   :linenos:

Requires
========

One extra module beyond the baseline boost layer:

* ``imgui/imgui_scope_builtin`` — defines the four ``with_*`` scope
  wrappers.

Why scope wrappers
==================

Raw ``BeginDisabled`` / ``EndDisabled`` pairs work, but the v2 boost lint
flags them as "invisible state changes" — if the source has
``BeginDisabled(...)`` somewhere and the matching ``EndDisabled()`` six
branches deep, a reader has to trace control flow to see whether the
disable will pop correctly on early-return paths. The scope wrapper:

* Couples the push/pop into a block — exit through any path pops.
* Reads as a single statement — the disabled argument is right at the top.
* Survives ``return``, ``break``, ``continue`` in the body — block exit
  always pops the scope.

with_disabled
=============

The argument is the *disabled* bool — ``true`` greys out the block. The
tutorial drives this with an ``ENABLED_MASTER`` checkbox and passes
``!ENABLED_MASTER.value`` so the checkbox's natural "enable child group"
reading produces the right semantics.

with_button_repeat
==================

When ``true``, holding a button inside the block fires click events at
ImGui's repeat rate (configured by ``KeyRepeatDelay`` and
``KeyRepeatRate`` in ``ImGuiIO``). Use it for stepper buttons (``+``,
``-``) where hold-to-increment is the natural interaction. Outside this
scope, buttons fire one click per press regardless of hold duration.

The tutorial's ``STEP_UP.click_count`` climbs visibly while the cursor
holds down on the ``+`` button in the recording — each repeat fires the
click handler.

with_font
=========

Push a different ``ImFont?`` for the block's render — the default font
is whatever ``GetFont()`` returns. Combine with ``load_daslang_font`` (see
``imgui/imgui_theme_daslang``) to load a custom TTF and scope-swap it for
specific UI sections (code editors, narration boxes, etc.).

The tutorial's ``with_font(GetFont())`` is a no-op for demonstration —
the rendered text is the active font. Production code would pass a
specific ``ImFont?`` returned from a prior ``AddFontFromFileTTF`` call.

with_clip_rect
==============

The fourth scope wrapper isn't exercised in this tutorial — see
``examples/features/clip_rect.das`` for a per-frame clipping demo.
``with_clip_rect((min, max, isect), blk)`` is the safest way to install a
custom clip rectangle around custom-rendered content (drawlist primitives,
images, manual layout) since the scope guarantees the prior clip rect is
restored on exit.

Combining wrappers
==================

The wrappers compose cleanly — nest them to layer scopes:

.. code-block:: das

   with_disabled(!FEATURE_ENABLED.value) {
       with_button_repeat(true) {
           button(STEP_UP, (text = "+"))
           button(STEP_DN, (text = "-"))
       }
   }

Steppers fire repeat clicks when held, but the whole pair greys out when
the feature flag is off. Each wrapper's exit pops its own scope; ImGui's
internal stacks handle the LIFO ordering.

.. seealso::

   Full source: :download:`examples/tutorial/with_disabled.das <../../../examples/tutorial/with_disabled.das>`

   Integration tests: ``tests/integration/test_disabled_block.das``,
   ``tests/integration/test_button_repeat.das``,
   ``tests/integration/test_font_stack.das``.

   Companion tutorials: :ref:`tutorial_with_id`, :ref:`tutorial_with_style`,
   :ref:`tutorial_with_tab_stop` — other ``with_*`` scope helpers.

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
