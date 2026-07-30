.. _tutorial_color:

#######
Color
#######

Five shapes for picking a color: two inline editors, two pop-out
pickers, and one caller-owned swatch:

.. code-block:: das

   color_edit3(IDENT, (text = ".."))           // inline RGB row
   color_edit4(IDENT, (text = ".."))           // inline RGBA row (+ alpha)
   color_picker3(IDENT, (text = ".."))         // full picker wheel, RGB
   color_picker4(IDENT, (text = "..",          // same, optional ref swatch
                         flags = ...,
                         ref_col = float4(...),
                         has_ref_col = true))
   color_button(IDENT, "##desc", col)          // click-trigger swatch;
                                               //   caller owns `col`

State types:

* ``color_edit3`` / ``color_picker3`` → ``ColorState3`` (``value : float3``)
* ``color_edit4`` / ``color_picker4`` → ``ColorState4`` (``value : float4`` incl. alpha)
* ``color_button``                    → ``ClickState``  (color is caller-owned)

Source: ``examples/tutorial/color.das``.

************
Walkthrough
************

.. video:: color.mp4

.. literalinclude:: ../../../examples/tutorial/color.das
   :language: das
   :linenos:

Requires
========

Already in the baseline boost layer:

* ``imgui/imgui_widgets_builtin`` — ``color_edit3/4`` / ``color_picker3/4`` /
  ``color_button`` rails.
* ``imgui/imgui_boost_runtime`` — ``ColorState3`` / ``ColorState4`` /
  ``ClickState`` structs.

edit vs picker
==============

The two families differ in shape, not in semantics:

* ``color_edit*`` — inline row: small swatch + numeric fields. Clicking
  the swatch pops up ImGui's stock picker. Best for "tweak a value
  beside other inputs."
* ``color_picker*`` — always-visible wheel + RGB sliders. Heavier on
  screen; best for "this dialog is *for* picking a color."

Both write to ``state.value`` (``float3`` / ``float4``). ``state.changed``
mirrors ImGui's per-frame "did it change" return — same in both
families.

The ``3`` vs ``4`` suffix is alpha:

* ``3`` → ``float3``, no alpha row.
* ``4`` → ``float4`` (``.a`` is the alpha row), and the picker variant
  takes optional ``ref_col`` + ``has_ref_col = true`` to surface a
  comparison swatch alongside the new color.

color_button — click trigger, caller-owned color
================================================

The asymmetry: ``color_button`` does NOT have a ``ColorState`` — it
takes the color **inline** as ``col : float4`` and stores only click
bookkeeping. Use it for cells in a palette grid or as the trigger for
a custom picker popup:

.. code-block:: das

   let RED : float4 = float4(0.85f, 0.20f, 0.20f, 1.0f)
   if (color_button(SWATCH, (desc_id = "##red", col = RED,
                              size = float2(96.0f, 48.0f)))) {
       // open a custom popup, set the active palette index, etc.
   }

``desc_id`` is the ImGui label (typically ``"##name"`` to suppress
visible text). ``size = (0, 0)`` lets ImGui pick a default square.

For a swatch that reports **hover** without click bookkeeping, see
:ref:`tutorial_color_button_hover`.

Flags
=====

``flags : ImGuiColorEditFlags`` carries the standard edit-mode toggles
— ``NoAlpha``, ``NoPicker``, ``NoOptions``, ``NoSmallPreview``,
``NoInputs``, ``NoTooltip``, ``NoLabel``, ``NoSidePreview``,
``NoDragDrop``, ``NoBorder``, ``AlphaBar``, ``AlphaPreview``,
``AlphaPreviewHalf``, ``HDR``, plus display-mode bits
(``DisplayRGB`` / ``DisplayHSV`` / ``DisplayHex``) and input-mode bits
(``InputRGB`` / ``InputHSV``). Composable via ``|``:

.. code-block:: das

   color_edit4(TINT, (text = "tint",
                      flags = ImGuiColorEditFlags.NoInputs |
                              ImGuiColorEditFlags.AlphaBar))

Driving from outside
====================

.. code-block:: bash

   # RGB editor / picker — 3-element value
   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"CO_WIN/CO_EDIT3","value":[1.0,0.5,0.0]}}' \
        localhost:9090/command
   # RGBA editor / picker — 4-element value
   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"CO_WIN/CO_EDIT4","value":[0.2,0.4,0.9,0.6]}}' \
        localhost:9090/command
   # color_button — click trigger
   curl -X POST -d '{"name":"imgui_click","args":{"target":"CO_WIN/CO_SWATCH"}}' \
        localhost:9090/command

Caller-owned variants
=====================

For sites where the value lives on an external ``float3`` / ``float4``,
use the ``edit_*`` rails — they take a ``T?`` pointer via
``safe_addr`` and skip the state-struct allocation:

.. code-block:: das

   var g_tint : float4 = float4(1.0f, 1.0f, 1.0f, 1.0f)
   edit_color_edit4(safe_addr(g_tint), (id = "TINT", text = "tint"))

See :ref:`tutorial_edit_external_tour`.

.. seealso::

   Full source: :download:`examples/tutorial/color.das <../../../examples/tutorial/color.das>`

   Hover variant: :ref:`tutorial_color_button_hover` — swatch as a
   per-frame hover *surface*, no click bookkeeping.

   Sibling tutorial: :ref:`tutorial_buttons` (the broader click-trigger
   family).

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
