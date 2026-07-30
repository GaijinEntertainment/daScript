.. _tutorial_edit_tab_item:

##############
edit_tab_item
##############

Caller-owned visibility bool for a tab. The external-pointer sibling
of ``tab_item`` (which owns visibility internally). The canonical
"shared open state" pattern — a second UI surface (checkbox row,
menu toggle, ``imgui_force_set``) drives the same flag the tab's X-close
button writes.

.. code-block:: das

   var private g_tab_open : bool = true

   edit_tab_item(safe_addr(g_tab_open), (id = "TAB_A",
                                          text = "alpha",
                                          flags = ImGuiTabItemFlags.None)) {
       <tab body>
   }

When ``*p_open == false`` the tab is skipped entirely — no header
chrome, no body. The X close button on the header writes
``*p_open = false`` automatically; you flip it back to true from any
other UI surface to re-show the tab.

Source: ``examples/tutorial/edit_tab_item.das``.

************
Walkthrough
************

.. video:: edit_tab_item.mp4

The recording drives the bind end to end with real gestures. First it clicks
the external **show B** checkbox and the beta tab vanishes outright - header
and body both gone. Then it closes **alpha** with the X on its own tab header,
and the **show A** checkbox flips itself off in response: the X and the
checkbox write the very same bool. Finally it ticks both boxes back on and
every tab returns. Each step is asserted - a checkbox that didn't flip, a tab
that didn't hide, or an X that didn't back-propagate would abort the recording.

.. literalinclude:: ../../../examples/tutorial/edit_tab_item.das
   :language: das
   :linenos:

Requires
========

* ``daslib/safe_addr`` — ``safe_addr(global)`` for the pointer arg.
* ``imgui/imgui_containers_builtin`` — ``edit_tab_item`` + ``tab_bar``.
* ``imgui/imgui_widgets_builtin`` — ``edit_checkbox`` (for the external
  controls), ``text``.

Why edit_tab_item over tab_item
===============================

``tab_item`` (container) owns its open state internally on
``TabItemState.open``. That field is read-only from outside the
widget — there's no public channel to flip it. So if you need a
SECOND UI surface to drive the same visibility, you have to choose:

* Use ``tab_item`` and accept that only the X-close button can hide
  the tab.
* Use ``edit_tab_item`` with a shared external bool — both the X
  button AND your other UI write the same value.

The recording shows the second case: a row of checkboxes mirrors the
state of three tabs. Clicking a checkbox hides the tab; clicking the
tab's X writes the bool, which back-propagates to the checkbox the
next frame.

The id arg
==========

All ``edit_*`` rails require an ``id = "<literal>"`` argument — that's
the snapshot path segment. Unlike the ``IDENT`` of state-owned
widgets (which the ``[widget]`` macro promotes from a bare identifier
to a string), the ``id`` here is a regular string literal you pass
inline. Snapshot probes use it the same way:

.. code-block:: bash

   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command \
       | jq '.globals."ET_WIN/ET_BAR/ET_TAB_A"'

The pointer arg
===============

The first positional arg is ``var p_open : bool?`` — a daslang
pointer to a bool. How you get the pointer depends on where the bool
lives:

* **Module-scope global** — ``safe_addr(g_open)``. Safest; verified
  at compile time.
* **Array element** — ``unsafe(addr(arr[i]))``. Required because
  ``safe_addr`` only works on locally-scoped expressions, not
  computed addresses.
* **Struct field** — ``unsafe(addr(my_struct.field))``. Same reason.

The recording uses ``safe_addr`` on three module globals
(``g_tab_a_open`` / ``b`` / ``c``).

Driving from outside
====================

``imgui_force_set`` writes the external bool through ``edit_checkbox``'s
channel (which shares the bool with the tab):

.. code-block:: bash

   # Hide alpha tab:
   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"ET_WIN/ET_CHECK_A","value":false}}' \
        localhost:9090/command

The X-button click on the tab header writes the same bool — symmetric.

.. seealso::

   Full source: :download:`examples/tutorial/edit_tab_item.das <../../../examples/tutorial/edit_tab_item.das>`

   Internal-state sibling: :ref:`tutorial_tab_bar` (covers
   ``tab_item`` + ``tab_item_button``).

   External-binding tour: :ref:`tutorial_edit_external_tour` — every
   ``edit_*`` rail in one window.

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
