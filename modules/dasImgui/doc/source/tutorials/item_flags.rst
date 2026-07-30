.. _tutorial_item_flags:

#######################
Item flags
#######################

Dear ImGui 1.92 split the *private* item flags into their own
``ImGuiItemFlagsPrivate`` enum and unified the per-item flag push onto
``PushItemFlag(flag, enabled)`` (the old ``PushButtonRepeat`` /
``PushTabStop`` are gone). dasImgui wraps that as the ``with_item_flag``
scope guard:

.. code-block:: das

   with_item_flag(ImGuiItemFlags.None | ImGuiItemFlagsPrivate.MixedValue, true) {
       checkbox(CB_MIXED, (text = "drawn as a tri-state dash"))
   }

Because a private flag and a public flag are different enum types,
``imgui/imgui_enums`` supplies cross-enum ``|`` operators so they combine into a
single ``ImGuiItemFlags`` in one push:

.. code-block:: das

   with_item_flag(ImGuiItemFlagsPrivate.ReadOnly | ImGuiItemFlags.NoNav, true) {
       slider_float(SL_RO, (text = "read-only, skipped by nav"))
   }

The three public flags that already have dedicated wrappers —
``with_disabled``, ``with_button_repeat``, ``with_tab_stop`` — should use those;
``with_item_flag`` is for the flags without a named guard.

Source: ``examples/tutorial/item_flags.das``.

************
Walkthrough
************

.. video:: item_flags.mp4

.. literalinclude:: ../../../examples/tutorial/item_flags.das
   :language: das
   :linenos:

Requires
========

``with_item_flag`` is in ``imgui/imgui_scope_builtin``; the cross-enum ``|``
operators and ``ImGuiItemFlagsPrivate`` come from ``imgui/imgui_enums`` (both
re-exported by ``imgui/imgui_boost_v2``).

Behaviour
=========

* **MixedValue** — draws a checkbox as the indeterminate dash regardless of its
  real checked state (the classic tri-state look).
* **ReadOnly** (private) ``|`` **NoNav** (public) — the slider ignores drags and
  is skipped by keyboard / gamepad navigation; the two flags are pushed together
  via the cross-enum operator.
* **AllowOverlap** — the wide button yields hover to a later widget drawn on top
  of it instead of swallowing the overlap.

The flag scope follows the block exactly: ``PushItemFlag`` on entry,
``PopItemFlag`` on exit, so siblings outside the block are unaffected.

.. seealso::

   Full source: :download:`examples/tutorial/item_flags.das <../../../examples/tutorial/item_flags.das>`

   Feature smoke: ``examples/features/internal_item_flag.das``.

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
