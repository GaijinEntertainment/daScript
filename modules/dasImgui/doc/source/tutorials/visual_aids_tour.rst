.. _tutorial_visual_aids_tour:

#######################
Visual aids tour
#######################

Visual aids are the layer above ImGui that makes a running app
**self-narrating** for tutorials, demos, and recordings. Five overlays
draw on top of any ImGui frame:

* **Highlight** — flash a colored rectangle around a widget's bbox for
  N frames. Pinpoints "this thing right here."
* **Mouse trail** — fading line behind ``io.MousePos``, so the synth
  cursor's path through the UI is visible in a recording.
* **Cursor sprite** — in-frame pointer drawn at ``io.MousePos``.
  Without it, recordings show no cursor (the OS-level cursor doesn't
  reach the framebuffer).
* **Narrate** — sticky-note callout with an optional connector line
  to a target widget. Auto-fits to avoid sibling-widget overlap.
* **Auto-highlight on command** — global flag that fires
  ``highlight`` on every accepted live command's target. One-shot
  debug aid for figuring out which widget a curl invocation actually
  hit.

Plus two more for keyboard work — exercised by this tutorial's
recording:

* **Key HUD** — bottom-center keycap pops for every synth key event,
  plus a Ctrl/Shift/Alt/Super modifier strip lit while a modifier is
  held.
* **Focus rect** — colored rectangle around the widget that owns
  ``io.active_widget``. Shows where typing lands.

This is the **richer reference** for the recording surface — the
``recording`` tutorial walked through the driver script anatomy;
this one walks through what each visual aid does in isolation.

Source: ``examples/tutorial/visual_aids_tour.das``.

************
Walkthrough
************

.. video:: visual_aids_tour.mp4

The recording is voiced and self-verifying: each beat speaks a line while a
**real** gesture fires under it and is asserted — clicking the highlight and
narrate buttons (the click must register), scrubbing ``VOLUME`` (the value must
change), and typing into ``NAME_INPUT`` then clearing it with Ctrl+A /
Backspace (the buffer must end empty). A silently broken beat aborts the
recording at teardown instead of shipping.

.. literalinclude:: ../../../examples/tutorial/visual_aids_tour.das
   :language: das
   :linenos:

Layout
======

The subject has two ``window(...)`` containers:

* ``SUBJECT_WIN`` (titled "Demoed widgets") — the things the visual
  aids point at: ``STATUS`` (text_show), ``VOLUME`` (slider_float),
  ``SAVE_BTN`` (button), ``NAME_INPUT`` (input_text). Leaves register
  at ``SUBJECT_WIN/<ident>``.
* ``CONTROLS_WIN`` (titled "Visual aids controls") — buttons that fire
  each aid in-process so you can iterate without a separate driver
  shell.

In the recording at the top, the driver drives the mouse aids **through**
the controls window — it clicks ``BTN_HIGHLIGHT_*`` and ``BTN_NARRATE_*``
for real (each click verified), so the buttons' own handlers fire
``highlight`` / ``narrate``. The keyboard overlays (``imgui_key_hud`` /
``imgui_focus_rect``) and the typing/chords are posted as direct live
commands. The buttons exist so a user dropped into ``daslang-live`` can
drive every aid by hand for exploration too.

Highlight
=========

A colored rectangle drawn around a widget's bbox for N frames:

.. code-block:: das

   highlight("SUBJECT_WIN/VOLUME")              // default: yellow, 60 frames
   highlight("SUBJECT_WIN/SAVE_BTN", 120, 0xFFFF8030u)   // orange, 120 frames

Highlights are short by design — long enough for "look here" to
register, short enough that two consecutive highlights compose
visually rather than queue. The recording clicks the three highlight
buttons in turn (status, volume, save), each flashing a rect on the
matching widget. Tunable defaults live in ``imgui_visual_aids.das``
(``highlight_default_frames``, ``highlight_color``).

Mouse trail
===========

A fading line behind ``io.MousePos``, drawn every frame the trail is
enabled:

.. code-block:: das

   mouse_trail(true)                // on
   mouse_trail(true, 0.45f)         // 450ms fade
   mouse_trail(false)               // off

The trail's value is mostly to make synth-cursor recordings parseable
— without it, the cursor teleports between waypoints and the viewer
can't tell what happened. Real mouse motion shows a trail too, which
is sometimes a nice touch in live demos.

Cursor sprite
=============

A visible mouse-pointer sprite drawn at ``io.MousePos``:

.. code-block:: das

   cursor_sprite(true)
   cursor_sprite(false)

OS-level cursors don't reach the framebuffer — the screen recorder
sees ImGui's draw output, not the cursor that the WM compositor draws
on top. Without ``cursor_sprite``, a recording shows widgets reacting
to clicks with no visible "thing" doing the clicking. Always enable
this before ``record_start`` for any tutorial recording.

Narrate
=======

A sticky-note callout with optional connector line to a target widget:

.. code-block:: das

   narrate("Click here to save.", "SUBJECT_WIN/SAVE_BTN")   // 180 frames default
   narrate("Drag this to set volume.", "SUBJECT_WIN/VOLUME", 240)
   narrate("Floating overlay, no target.")                   // no connector line

The auto-fit logic tries four candidate anchors (right / left / below
/ above the target widget) and picks the first that doesn't overlap
the widget OR the viewport edge OR (when enabled) the key_hud zone.
Falls back to right-then-clamp only when every candidate overflows.
The result: a sticky-note that points at the right thing without
covering it.

Auto-highlight on command
=========================

A module-scope flag that fires ``highlight`` on every accepted live
command's target:

.. code-block:: das

   imgui_auto_highlight_on_command = true
   // any imgui_click / imgui_force_set / imgui_open ... now flashes its target

Useful when debugging "why didn't my curl do anything" — turn it on,
fire the command, and either see a highlight flash (command reached
the right widget) or no flash (typo in the target path, or the widget
isn't in the registry that frame).

Key HUD + focus rect (recording's keyboard tour)
================================================

Beyond the mouse-focused aids:

* ``imgui_key_hud`` pops a keycap label at bottom-center for every
  synthesized key event. ``imgui_focus_rect`` draws a colored
  rectangle around whichever widget has keyboard focus right now.
* The recording's second half exercises both: a real click on
  ``NAME_INPUT`` lights the focus rect, then ``imgui_key_type`` types
  "Hello, World!" into the input — every keycap pops at the bottom
  with the matching mod-strip flash on H, W, and ! (auto-shift keys).
  The committed value is verified. Then ``imgui_key_chord`` fires
  Ctrl+A — Ctrl pill lights up while the "A" keycap pops — and a
  Backspace clears the (selected) buffer, verified empty.
* All three keyboard live commands route through the L1 synth IO
  layer described in :ref:`tutorial_driving_outside`.

Standalone vs live
==================

The visual-aid functions (``highlight``, ``mouse_trail``, ``narrate``,
``cursor_sprite``) work in both modes — they're just drawing code.
The **live-command wrappers** (``imgui_highlight`` / ``imgui_mouse_trail``
/ etc.) need ``daslang-live`` for the HTTP surface.

Driving from outside
====================

Every aid is reachable via curl:

.. code-block:: bash

   curl -X POST -d '{"name":"imgui_highlight","args":{"target":"SUBJECT_WIN/VOLUME"}}' \
        localhost:9090/command
   curl -X POST -d '{"name":"imgui_mouse_trail","args":{"enabled":true}}' \
        localhost:9090/command
   curl -X POST -d '{"name":"imgui_cursor_sprite","args":{"enabled":true}}' \
        localhost:9090/command
   curl -X POST -d '{"name":"imgui_narrate","args":{"text":"click me","target":"SUBJECT_WIN/SAVE_BTN","frames":180}}' \
        localhost:9090/command
   curl -X POST -d '{"name":"imgui_auto_highlight","args":{"enabled":true}}' \
        localhost:9090/command
   curl -X POST -d '{"name":"imgui_key_hud","args":{"enabled":true,"show_modifiers":true}}' \
        localhost:9090/command
   curl -X POST -d '{"name":"imgui_focus_rect","args":{"enabled":true}}' \
        localhost:9090/command

The recording at the top of this page fires this exact sequence (plus
the keyboard tour) through the playwright transport instead of curl.

.. seealso::

   Full source: :download:`examples/tutorial/visual_aids_tour.das <../../../examples/tutorial/visual_aids_tour.das>`

   Driver: :download:`tests/integration/record_visual_aids_tour.das <../../../tests/integration/record_visual_aids_tour.das>` — the voiced, self-verifying tour that walks every aid in turn.

   Implementation: ``modules/dasImgui/widgets/imgui_visual_aids.das`` —
   the full surface plus narrate auto-fit, key HUD, focus rect.

   Previous tutorial: :ref:`tutorial_driving_outside`

   Next tutorial: :ref:`tutorial_recording`

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
