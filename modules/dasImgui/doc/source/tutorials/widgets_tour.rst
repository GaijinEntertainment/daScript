.. _tutorial_widgets_tour:

##################
Widgets tour
##################

One of every common boost widget on a single panel. The example frames as
an audio-settings dialog so each widget reads contextually: a text input
for the user name, a slider for volume, a checkbox for mute, a combo for
codec quality, a color editor for the accent color, and a button to save.

Source: ``examples/tutorial/widgets_tour.das``.

************
Walkthrough
************

.. video:: widgets_tour.mp4

The recording is voiced and self-verifying: each stage speaks a line while a
**real** gesture fires under it and is asserted — typing into the name field
(committed value checked), dragging ``VOLUME`` (value must change), clicking
``MUTED`` (toggle must register), opening the ``QUALITY`` combo and picking
*High* (selection verified), setting ``TINT`` from the API (``color_edit3``'s
picker is mouse-only, so the tour drives it via ``force_set`` and verifies the
swatch took), and clicking *Save* (click must register). A silently broken
stage aborts the recording at teardown instead of shipping.

.. literalinclude:: ../../../examples/tutorial/widgets_tour.das
   :language: das
   :linenos:

Requires
========

The ``require`` block matches :ref:`tutorial_boost_basics` exactly — backend
(``imgui_app`` / ``glfw`` / ``opengl``), live host (``live/*``), and the v2
boost layer (``imgui_live``, ``imgui_boost_runtime``, ``imgui_boost_v2``,
``imgui_widgets_builtin``, ``imgui_containers_builtin`` for the
``window(...)`` block container). One extra line pulls in
``imgui/imgui_visual_aids`` so the post-it narrate overlay the driver script
paints into the recording is available at render time.

Init and shutdown
=================

``init()`` opens a 1024x720 GLFW window via ``live_create_window`` and hands
the handle to ``live_imgui_init``. It also bumps ``io.FontGlobalScale`` to
``1.5`` so the recorded APNG reads at typical Sphinx HTML widths without
zooming. ``shutdown()`` mirrors the pair in reverse order.

The frame loop
==============

``update()`` follows the standard dasImgui v2 shape — see
:ref:`tutorial_boost_basics` for the line-by-line breakdown. The only addition
is a single call to ``apply_synth_io_override()`` between
``ImGui_ImplGlfw_NewFrame`` and ``NewFrame``. The GLFW backend polls real OS
mouse data every focused frame and would otherwise win the IO race against
any synthesized event the driver script posted just before. The override
re-asserts the synth IO so live-driven clicks land at the right widget.

Widgets
=======

All six widgets live inside a single ``window(AUDIO_WIN, ...)`` boost
container — same pattern :ref:`tutorial_boost_basics` introduced, so leaves
register at ``AUDIO_WIN/<ident>``:

.. code-block:: das

   window(AUDIO_WIN, (text = "Audio settings", closable = false,
                      flags = ImGuiWindowFlags.None)) {
       input_text(USER_NAME, (text = "Your name"))
       slider_float(VOLUME, (text = "Master volume"))
       checkbox(MUTED, (text = "Mute"))
       combo(QUALITY, (text = "Quality", items <- ["Low", "Medium", "High", "Ultra"]))
       color_edit3(TINT, (text = "Accent color"))
       if (button(SAVE_BTN, (text = "Save settings"))) { ... }
   }

Each boost macro declares the named global the first time it expands and
registers it under the path-prefixed name. The slider's range comes from a
plain field assignment one line above the macro call:
``VOLUME.bounds = (0.0f, 1.0f)``. The combo's item list moves into the
named argument with ``items <-`` because string arrays are non-copyable.
``SAVE_BTN.click_count`` accumulates across frames — handy for assertions in
a test harness.

Standalone vs live
==================

``main()`` runs the loop directly when invoked as
``daslang.exe widgets_tour.das``. Under ``daslang-live`` the host calls
``init`` / ``update`` / ``shutdown`` itself; ``main`` is ignored. Every state
struct (``USER_NAME``, ``VOLUME``, ``MUTED``, ``QUALITY``, ``TINT``,
``SAVE_BTN``) carries ``@live`` by default, so live-reloading the source
preserves the widget contents.

Driving from outside
====================

Under ``daslang-live`` the boost layer exposes ``imgui_force_set`` and
``imgui_click`` over ``localhost:9090``. The top of the source file lists
one ``curl`` invocation per widget. Each ``target`` is path-qualified:

.. code-block:: bash

   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"AUDIO_WIN/USER_NAME","value":"Boris"}}' \
        localhost:9090/command

For ``color_edit3``, ``value`` is a JSON object with ``x`` / ``y`` / ``z``
fields — ``from_JV`` reads ``float3`` from object form, not array form. For
``combo``, ``value`` is the zero-based index into the ``items`` array.

Next steps
==========

Layout helpers — splitters, columns, child windows — are next on top of the
same widget set. To write your own widget kind on the same rails — a rotary
knob, an XY-pad, a meter — see :ref:`tutorial_custom_widgets`.

.. seealso::

   Full source: :download:`examples/tutorial/widgets_tour.das <../../../examples/tutorial/widgets_tour.das>`

   Richer reference: ``examples/features/inputs_*.das`` — every widget with
   every option exercised, against the same boost layer.

   Previous tutorial: :ref:`tutorial_boost_basics`

   Next tutorial: :ref:`tutorial_custom_widgets`

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.

   :ref:`Builtin widgets <stdlib_imgui_widgets_section>` — widget reference.
