.. _tutorial_slider:

#######################
Slider widgets
#######################

The slider family is **bounded numeric editing along a fixed track**:
click the track to jump, drag the handle to scrub. Same call shape spans
scalar / vector / vertical and float / int — ten widgets, one mental
model.

.. code-block:: das

   slider_float(IDENT, (text = "..", format = "%.3f",
                        flags = ImGuiSliderFlags....))
   slider_int(IDENT, (text = "..", format = "%d"))
   slider_float2 / slider_float3 / slider_float4   // vector forms
   slider_int2   / slider_int3   / slider_int4
   vslider_float(IDENT, (text, size = float2(w, h), format = "%.3f"))
   vslider_int(IDENT, (text, size, format))

Bounds are **required** — slider has a fixed-width track between
``(state.bounds.min, state.bounds.max)``. Unlike drag, zero-init bounds
``(0, 0)`` collapses the track to a single point.

Source: ``examples/tutorial/slider.das``.

************
Walkthrough
************

.. video:: slider.mp4

.. literalinclude:: ../../../examples/tutorial/slider.das
   :language: das
   :linenos:

Requires
========

Already in the baseline boost layer:

* ``imgui/imgui_widgets_builtin`` — every ``slider_*`` / ``vslider_*`` rail.
* ``imgui/imgui_boost_runtime`` — ``SliderStateFloat`` / ``SliderStateInt`` /
  vector state structs.

Bounds
======

Bounds are a property of the **state**, not the call. Set them every
frame (idempotent assignment, no branching):

.. code-block:: das

   S_FLOAT.bounds = (0.0f, 1.0f)
   slider_float(S_FLOAT, (text = "volume", format = "%.2f"))

The wrapper passes ``(min, max)`` to ImGui's ``SliderFloat`` which
constrains the handle. Unlike :ref:`tutorial_drag`, zero bounds are
**not** unclamped — the track has nowhere to render.

Format
======

``format`` is the printf-style label format. Default ``"%.3f"`` /
``"%d"``; customize for units:

.. code-block:: das

   slider_int(S_INT, (text = "percent", format = "%d%%"))    // 42%
   slider_float(S_TEMP, (text = "temp", format = "%.1f °C")) // 23.5 °C

Vector forms
============

The ``2`` / ``3`` / ``4`` suffix puts that many handles on one row.
``state.value`` becomes ``float2`` / ``float3`` / ``float4`` (or
``int2`` / ``int3`` / ``int4``):

.. code-block:: das

   S_VEC3.bounds = (-1.0f, 1.0f)        // applies to every component
   slider_float3(S_VEC3, (text = "rotation", format = "%.2f"))
   // S_VEC3.value.x, S_VEC3.value.y, S_VEC3.value.z

Vertical orientation
====================

``vslider_float`` / ``vslider_int`` share the same state struct as the
horizontal form, but render the track **top-to-bottom**. The
distinguishing arg is ``size : float2`` (width, height):

.. code-block:: das

   V_MID.bounds = (0.0f, 1.0f)
   vslider_float(V_MID, (text = "##mid",
                         size = float2(36.0f, 140.0f), format = "%.2f"))

Common pattern: three vslider_floats with ``same_line`` between them
form a mixer-channel strip. The ``"##mid"`` text prefix hides the label
(everything after ``##`` is ID-only); use ``"label"`` instead to
display it above the slider.

Driving from outside
====================

Every slider exposes the same telemetry channel — ``imgui_force_set`` writes
``state.pending_value`` which the next frame consumes:

.. code-block:: bash

   # Scalar:
   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"SLIDER_WIN/S_FLOAT","value":0.8}}' \
        localhost:9090/command
   # Vector — one number per component:
   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"SLIDER_WIN/S_VEC3","value":[1.0,2.0,3.0]}}' \
        localhost:9090/command

The dispatcher (``[widget_dispatch]`` on ``SliderStateFloat`` and
friends) accepts the right JSON shape per state type — scalar number or
array of numbers.

Slider vs drag vs input
=======================

The three numeric-edit families differ in interaction shape:

* **slider** — click and drag along a fixed-width track between
  ``v_min`` / ``v_max``. Best for bounded percentages, settings sliders.
* **drag** — click and scrub, no fixed track. Best for "tweak this
  value" where the range is large or open-ended.
* **input** — type the number, optionally with ``+`` / ``-`` step
  buttons. Best for precise values where the user knows the number.

All three families share the same vector / scalar / format conventions.
See :ref:`tutorial_drag`.

Caller-owned variant
====================

For sites where the value lives on an external scalar (not a widget
state struct), use the ``edit_slider_*`` rail instead — it takes a
``T?`` pointer via ``safe_addr`` and skips the state-struct allocation:

.. code-block:: das

   var g_volume : float = 0.5f
   edit_slider_float(safe_addr(g_volume), (id = "VOLUME",
                                           text = "volume",
                                           v_min = 0.0f, v_max = 1.0f))

See :ref:`tutorial_edit_external_tour`.

.. seealso::

   Full source: :download:`examples/tutorial/slider.das <../../../examples/tutorial/slider.das>`

   Features-side demo: ``examples/features/inputs_slider.das`` /
   ``examples/features/edit_vsliders.das`` — every slider in one window,
   useful for ``imgui_force_set`` smoke testing.

   Sibling tutorial: :ref:`tutorial_drag`.

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
