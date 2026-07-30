.. _tutorial_boost_basics:

####################
Boost basics
####################

The smallest end-to-end dasImgui v2 program. One window, one slider, one button.
The slider drives a float (``VOLUME.value``) and the button accumulates a click
counter (``BUMP_BTN.click_count``). Both pieces of state are auto-generated
daslang globals; nothing is stored inside Dear ImGui.

Source: ``examples/tutorial/boost_basics.das``.

************
Walkthrough
************

.. video:: boost_basics.mp4

The recording drives both pieces of state for real and asserts each effect:
it drags ``Volume`` and verifies ``VOLUME.value`` *changed* (``drag_through_voice``),
then clicks ``Bump`` twice and verifies each click registered
(``hold_through_voice``). A slider that stopped tracking or a button that stopped
counting would abort the recording.

.. literalinclude:: ../../../examples/tutorial/boost_basics.das
   :language: das
   :linenos:

Requires
========

The ``require`` block pulls in:

* The C++-bound Dear ImGui surface (``imgui``, ``imgui_app``).
* The GLFW backend + OpenGL renderer + live-reload host glue (``glfw/...``,
  ``opengl/...``, ``live/...``).
* The dasImgui v2 boost layer: ``imgui_live`` (lifecycle),
  ``imgui_boost_runtime`` (state structs), ``imgui_boost_v2`` (macros),
  ``imgui_widgets_builtin`` (the actual widget implementations),
  ``imgui_containers_builtin`` (the ``window(...)`` block-arg container
  and its siblings), ``imgui_visual_aids`` (the synth-IO override hook
  used by the recording driver — see :ref:`tutorial_recording`).

Init and shutdown
=================

``init()`` opens a 960x560 GLFW window via ``live_create_window`` and
hands its handle to ``live_imgui_init`` so the ImGui context, fonts, and GL
backend get wired up. ``shutdown()`` reverses the pair in shutdown order.

The frame loop
==============

``update()`` is what runs every frame. The pattern, line by line:

#. ``live_begin_frame()`` — return early if the window is closing.
#. ``begin_frame()`` — boost-side per-frame setup (registry, command dispatch).
#. ``ImGui_ImplGlfw_NewFrame`` / ``apply_synth_io_override`` /
   ``NewFrame`` — the ImGui input + frame prologue.
   ``apply_synth_io_override`` lets synthetic mouse/keyboard
   events from live-commands win against the GLFW backend's per-frame
   poll (needed for tutorial recordings).
#. The widget block (see below).
#. ``end_of_frame()`` / ``Render()`` — boost-side bookkeeping, then ImGui
   composes the draw list.
#. ``glViewport`` / ``glClear`` / ``live_imgui_render`` — paint (pure-das GL renderer).
#. ``live_end_frame()`` — swap buffers, advance live-reload housekeeping.

The window container
====================

The ``window(NAME, ...) { ... }`` macro is the boost-layer wrapper around
ImGui's ``Begin``/``End`` pair:

.. code-block:: das

   SetNextWindowPos(ImVec2(60.0, 60.0), ImGuiCond.Always)
   SetNextWindowSize(ImVec2(560.0, 240.0), ImGuiCond.Always)
   window(BASICS_WIN, (text = "Boost basics", closable = false,
                       flags = ImGuiWindowFlags.None)) {
       // ... widgets ...
   }

Three things to notice:

* The block runs the body once per frame between ``Begin`` and ``End``
  — no manual ``End()`` to forget.
* ``BASICS_WIN`` is a daslang identifier — the macro auto-emits a
  ``WindowState`` global named that, and pushes ``BASICS_WIN`` onto the
  boost registry path. Every leaf widget registers under
  ``BASICS_WIN/<ident>``.
* Per-call config goes in the named-tuple — ``text`` is the title,
  ``closable`` controls whether the X-button shows, ``flags`` are
  the raw ``ImGuiWindowFlags`` bits.

``SetNextWindowSize`` / ``SetNextWindowPos`` are the same raw ImGui
calls you'd use without the boost layer — they affect the next
``Begin`` (the one the macro emits) just like before.

Widgets
=======

Two boost macros do the actual work:

.. code-block:: das

   VOLUME.bounds = (0.0f, 1.0f)
   slider_float(VOLUME, (text = "Volume"))
   text("volume = {VOLUME.value}")

The macro form ``slider_float(VOLUME, ...)`` declares a module global
``VOLUME : SliderStateFloat`` the first time it expands. Telemetry
registers it under the path-prefixed name ``BASICS_WIN/VOLUME`` (every
widget inside ``window(BASICS_WIN, ...)`` inherits the prefix); the
bounds line is plain assignment to the struct field.

.. code-block:: das

   if (button(BUMP_BTN, (text = "Bump"))) {
       print("bump clicked\n")
   }
   text("bumps = {BUMP_BTN.click_count}")

``button(BUMP_BTN, ...)`` returns ``true`` on the frame the click registers; the
underlying ``ButtonState`` keeps ``click_count`` updated for later assertions
(e.g. from ``imgui_playwright``).

Standalone vs live
==================

``main()`` runs the loop directly when invoked as ``daslang.exe boost_basics.das``.
Under ``daslang-live`` the host calls ``init`` / ``update`` / ``shutdown``
itself; ``main`` is ignored. The slider and button states preserve across live
reloads because every boost-generated global carries ``@live`` by default.

Driving from outside
====================

When run live, the boost layer exposes the standard live-command surface at
``localhost:9090``. Mutate the slider:

.. code-block:: bash

   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"BASICS_WIN/VOLUME","value":0.75}}' \
        localhost:9090/command

…and ``VOLUME.value`` jumps to ``0.75`` on the next frame. Note the
``BASICS_WIN/`` prefix — the ``window`` container pushes its identifier
onto the registry path, so external drivers target the
path-qualified name.

Next steps
==========

The next tutorial walks the full common-widget surface — text input,
slider, checkbox, combo, color, button — on a single panel.

.. seealso::

   Full source: :download:`examples/tutorial/boost_basics.das <../../../examples/tutorial/boost_basics.das>`

   Richer reference: ``examples/tutorial/visual_aids_tour.das`` — keeper
   reference exercising highlights, mouse trail, narration, and recording.

   Next tutorial: :ref:`tutorial_widgets_tour`

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.

   :ref:`Builtin widgets <stdlib_imgui_widgets_section>` — widget reference.
