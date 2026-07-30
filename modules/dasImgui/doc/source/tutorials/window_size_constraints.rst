.. _tutorial_window_size_constraints:

#################################
Window size constraints
#################################

ImGui's ``SetNextWindowSizeConstraints`` clamps the next window between a
min and max corner size. The 2-arg form is enough for a static box constraint;
the 3-arg form takes an ``ImGuiSizeCallback`` — a C function pointer ImGui
invokes on every resize so the callback can re-shape the requested size
(aspect-ratio lock, fixed-step quantization, "always square", etc.).

The boost module ``imgui/imgui_window_constraints_builtin`` ships a daslang
wrapper around the 3-arg form: an ``ImGuiSizeConstraints`` struct that wraps
a daslang lambda + the existing C++ trampoline (``das_invoke_lambda<void>``
at ``src/dasIMGUI.main.cpp:361``). End-user code passes the wrapper directly:

.. code-block:: das

   let aspect = @ capture(= ratio) (var data : ImGuiSizeCallbackData) : void {
       data.DesiredSize = float2(data.DesiredSize.x,
                                  data.DesiredSize.x / ratio)
   }
   SetNextWindowSizeConstraints(float2(0,0), float2(FLT_MAX, FLT_MAX),
                                ImGuiSizeConstraints(aspect))

The 2-arg ``SetNextWindowSizeConstraints(min, max)`` form stays on the
boost-surface allow-list — only require this module when you need the
callback form.

Source: ``examples/tutorial/window_size_constraints.das``.

************
Walkthrough
************

.. video:: window_size_constraints.mp4

The recording drags each window by its bottom-right corner - the real
"drag border" gesture - to a shape that violates its constraint, and the
callback snaps it back: the first window, dragged to a wide-short rectangle,
springs to a 400x400 square; the second, dragged tall, collapses to 480x270
(16:9); the third, dragged off-grid, lands on 350x200 (nearest 50). Each
snapped size is asserted - a drag that didn't resize, or a callback that
didn't reshape the request, aborts the recording.

.. literalinclude:: ../../../examples/tutorial/window_size_constraints.das
   :language: das
   :linenos:

Requires
========

One extra module on top of the baseline boost layer:

* ``imgui/imgui_window_constraints_builtin`` — the ``ImGuiSizeConstraints``
  struct + ctor + 3-arg overload of ``SetNextWindowSizeConstraints``.

The three callback patterns
===========================

The tutorial mounts three resizable windows demonstrating the canonical
callback shapes:

**Square** — ``max(w, h)`` wins both dims:

.. code-block:: das

   let sq <- @ (var data : ImGuiSizeCallbackData) : void {
       let s = max(data.DesiredSize.x, data.DesiredSize.y)
       data.DesiredSize = float2(s, s)
   }

**Aspect 16:9** — height tracks ``width / aspect_ratio``:

.. code-block:: das

   let ratio = 16.0f / 9.0f
   let aspect <- @ capture(= ratio) (var data : ImGuiSizeCallbackData) : void {
       data.DesiredSize = float2(data.DesiredSize.x,
                                  data.DesiredSize.x / ratio)
   }

**Fixed step** — both dims snap to the nearest multiple:

.. code-block:: das

   let step = 50.0f
   let snap <- @ capture(= step) (var data : ImGuiSizeCallbackData) : void {
       let rx = float(int(data.DesiredSize.x / step + 0.5f)) * step
       let ry = float(int(data.DesiredSize.y / step + 0.5f)) * step
       data.DesiredSize = float2(rx, ry)
   }

Capture rules follow the standard daslang lambda surface — ``capture(= var)``
for by-value, ``capture(& var)`` for by-reference. The ``@`` (no-capture)
form works for callbacks that read only ``data``.

Struct layout
=============

The ``ImGuiSizeConstraints`` daslang struct mirrors
``das::DasImGuiSizeConstraints`` in ``src/dasIMGUI.main.cpp`` field-by-field:

.. code-block:: das

   struct ImGuiSizeConstraints {
       context : Context?
       callback : lambda<(var data : ImGuiSizeCallbackData) : void>
       at : LineInfo?
   }

The C++ trampoline reads field offsets directly to dispatch the lambda
through ``das_invoke_lambda<void>::invoke<ImGuiSizeCallbackData*>``. Don't
re-order the fields — the binding is not symbolic.

Standalone vs live
==================

Same convention as previous tutorials. Resize any of the three windows by
dragging a border or corner — the callback fires on every drag delta and
quantizes the result.

.. seealso::

   Full source: :download:`examples/tutorial/window_size_constraints.das <../../../examples/tutorial/window_size_constraints.das>`

   ``app_small`` ShowExampleAppConstrainedResize (mirrors ImGui's
   imgui_demo.cpp:7885-7978) exercises 9 constraint options across both
   overload forms: ``examples/imgui_demo/app_small.das``.

   Integration test: ``tests/integration/test_app_small_constrained_resize.das``.

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
