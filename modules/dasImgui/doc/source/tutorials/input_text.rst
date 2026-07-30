.. _tutorial_input_text:

#######################
Input text widgets
#######################

The input_text family is **text editing** — single-line, multiline,
grow-on-overflow, callback-driven, and the inline filter editor. Six
widgets, one ``InputTextState`` (text_filter uses its own
``TextFilterState``).

.. code-block:: das

   input_text(IDENT, (text = "..", flags = ImGuiInputTextFlags....))
   input_text_with_hint(IDENT, (text, hint, flags))
   input_text_multiline(IDENT, (text, size = float2(w, h), flags))
   input_text_growable(IDENT, (text, flags))          // CallbackResize plumbed
   input_text_callback(IDENT, text, flags, cb)        // user lambda on flagged events
   text_filter(IDENT, (text, width)) + passes_filter(IDENT, line)

Buffer-as-pointer path: state owns ``array<uint8> buffer`` + a string
mirror. ``input_text_growable`` plumbs ImGui's ``CallbackResize`` through
a daslang lambda + stack thunk so the buffer expands past
``state.capacity`` automatically.

Source: ``examples/tutorial/input_text.das``.

************
Walkthrough
************

.. video:: input_text.mp4

.. literalinclude:: ../../../examples/tutorial/input_text.das
   :language: das
   :linenos:

Requires
========

Already in the baseline boost layer:

* ``imgui/imgui_widgets_builtin`` — every ``input_text*`` rail +
  ``text_filter`` + ``passes_filter``.
* ``imgui/imgui_boost_runtime`` — ``InputTextState`` (buffer + mirror) and
  ``TextFilterState`` (bound ``ImGuiTextFilter`` inline).
* ``strings`` — for ``starts_with`` used in the callback completion stage.

Basic and with_hint
===================

``input_text`` is the single-line editor backed by a fixed-size buffer
(``state.capacity``, default 256 bytes). ``input_text_with_hint`` adds a
placeholder rendered when the buffer is empty:

.. code-block:: das

   input_text(NAME, (text = "name"))
   input_text_with_hint(EMAIL, (text = "email",
                                hint = "you@example.com"))

Both share ``InputTextState``. ``state.value`` is a clone of the buffer
content updated each frame.

Multiline
=========

``input_text_multiline`` accepts CR / LF and exposes a sized text box.
``size = float2(0, 0)`` lets ImGui pick a default rect — wide and short.
Pass an explicit ``(w, h)`` for editor-style panels:

.. code-block:: das

   input_text_multiline(BIO, (text = "bio",
                              size = float2(0.0f, 90.0f)))  // 90px tall

Growable
========

``input_text_growable`` is the same as ``input_text`` but the buffer
**resizes itself** when the user types past ``state.capacity``. ImGui's
``CallbackResize`` event is plumbed through a daslang lambda; the thunk
holding ``(Context*, LineInfo*, lambda)`` lives on the C call stack for
the duration of the ``ImGui::InputText`` call:

.. code-block:: das

   input_text_growable(FREE, (text = "free-form"))
   // state.capacity grows as needed; state.buffer follows

Use this when the input is genuinely free-form (notes, code, long
URLs); use the fixed-size form when 256 bytes is enough (names, emails,
identifiers).

Callback
========

``input_text_callback`` takes an extra ``cb`` arg — a lambda fired by
ImGui on flagged events. Wire the events you want via ``flags``:

* ``CallbackCompletion`` — TAB pressed
* ``CallbackHistory`` — Up/Down arrows
* ``CallbackAlways`` — every frame the field is active
* ``CallbackCharFilter`` — per-character; return non-zero to reject
* ``CallbackEdit`` — buffer modified
* ``CallbackResize`` — buffer overflow (use ``input_text_growable`` for
  the standard impl)

.. code-block:: das

   input_text_callback(CMD, "command",
                       ImGuiInputTextFlags.CallbackCompletion,
                       @(var data : ImGuiInputTextCallbackData) =>
                           completion_cb(data))

The ``data : ImGuiInputTextCallbackData`` pointer is valid only inside
the call. ``data.Buf`` is the current buffer; ``DeleteChars`` /
``InsertChars`` are the in-place edit helpers. Cloning ``data.Buf`` to a
daslang string + doing the matching in pure das is the common pattern.

text_filter
===========

``text_filter`` renders an inline ``InputText`` editor whose buffer is
parsed as comma-separated tokens:

* ``foo,bar`` — pass lines containing ``foo`` OR ``bar``
* ``-debug`` — exclude lines containing ``debug``
* ``info,-debug`` — combine

Pair with ``passes_filter(STATE, line) : bool`` to gate output:

.. code-block:: das

   text_filter(FILTER)
   for (i in range(length(LOG_LINES))) {
       if (passes_filter(FILTER, LOG_LINES[i])) {
           text(LOG_LINE[i], (text = LOG_LINES[i]))
       }
   }

While the filter expression is empty, ``passes_filter`` returns ``true``
for every line — the filter is silently disabled until the user types.
Use ``is_active(STATE)`` to branch on filter-empty vs filter-non-empty
when you want a different code path (e.g. clipper-cull vs sequential
scan).

Indexed-form note
=================

The ``text(IDENT[i], (text = ...))`` form requires you to declare the
table at module scope:

.. code-block:: das

   var private LOG_LINE : table<int; NarrativeState>

The single-state form (``text(IDENT, (text = ...))``) auto-emits the
state global from the macro. Indexed form does not — the table key type
needs the user's hand.

Driving from outside
====================

The walkthrough above types into every field with **real** synthetic key
events (a real click to focus, then ``imgui_key_type``), so it exercises
exactly what a user would: the completion callback fires on Tab, the filter
narrows the log as you type, and one ``\n`` in the multiline buffer inserts
exactly one line break. For *scripted* setup that skips the keystrokes,
``imgui_force_set`` writes ``state.pending_value`` and the next frame
overwrites the buffer:

.. code-block:: bash

   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"IT_WIN/IT_NAME","value":"hello"}}' \
        localhost:9090/command
   # Multiline content — embed \n in the JSON string:
   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"IT_WIN/IT_BIO","value":"line 1\nline 2"}}' \
        localhost:9090/command

For ``input_text_growable``, sending a payload longer than current
``state.capacity`` triggers ``CallbackResize`` on the same frame the
buffer is consumed.

text_filter is currently **read-only** via telemetry — there's no
``imgui_force_set`` path that writes the filter expression. Click the filter
field and type directly to change it.

.. seealso::

   Full source: :download:`examples/tutorial/input_text.das <../../../examples/tutorial/input_text.das>`

   Features-side demos: ``examples/features/inputs_text.das`` (all five
   non-filter forms) and ``examples/features/input_text_callback.das``
   (canonical TAB-completion).

   Sibling tutorial: :ref:`tutorial_input_numeric`.

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
