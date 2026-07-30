.. _tutorial_log_capture:

#######################
Log capture
#######################

ImGui's logging API captures the *rendered text* of every widget submitted
inside a log scope to a target (TTY, file, clipboard, or an in-memory buffer).
Dear ImGui 1.92 renamed the target enum from ``ImGuiLogType`` to
``ImGuiLogFlags`` and gave the values an ``Output`` prefix
(``OutputTTY`` / ``OutputFile`` / ``OutputClipboard`` / ``OutputBuffer``). The
boost ``with_log`` wrapper takes the renamed enum:

.. code-block:: das

   with_log(ImGuiLogFlags.OutputClipboard, -1) {
       text("=== Daslang Widget Report ===")
       text("Section: Fruits")
       LogRenderedText("(this line is captured but never drawn)")
   }
   let captured = GetClipboardText()

The second argument is the tree depth to auto-expand (``-1`` = default).
``LogBegin`` asserts that logging is not already active, so ``with_log`` must
**not** nest.

Source: ``examples/tutorial/log_capture.das``.

************
Walkthrough
************

.. video:: log_capture.mp4

.. literalinclude:: ../../../examples/tutorial/log_capture.das
   :language: das
   :linenos:

Requires
========

``with_log`` is in ``imgui/imgui_scope_builtin`` (re-exported by
``imgui/imgui_boost_v2``). ``GetClipboardText`` and ``LogRenderedText`` are in
the carve-out of raw calls the lint allows.

Behaviour
=========

* Each widget submitted inside the ``with_log`` block has its rendered text
  appended to the chosen target.
* ``LogRenderedText`` injects a line into the capture that is **never** drawn on
  screen — useful for machine-readable markers.
* On the capture frame the demo logs to the clipboard, then reads it straight
  back with ``GetClipboardText`` and displays it in a child panel, so you can
  see exactly what was captured.

Migration note
==============

Pre-1.92 code calling ``LogBegin(ImGuiLogType.Clipboard, ...)`` — or
``with_log(ImGuiLogType.Clipboard, ...)`` — should move to
``ImGuiLogFlags.OutputClipboard`` (and the matching ``Output*`` value for other
targets).

.. seealso::

   Full source: :download:`examples/tutorial/log_capture.das <../../../examples/tutorial/log_capture.das>`

   Feature smoke: ``examples/features/internal_log_capture.das``.

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
