.. _tutorial_data_table:

#######################
data_table
#######################

ImGui's tables API — ``BeginTable`` / ``EndTable`` with body-internal
``TableSetupColumn`` / ``TableHeadersRow`` / ``TableNextRow`` /
``TableSetColumnIndex`` / ``TableNextColumn`` cursor primitives — lives
behind one boost container plus six snake_case pass-throughs in
``imgui/imgui_table_builtin``. The container is named ``data_table`` (not
``table``) because ``table`` is a daslang reserved keyword for the
``table<K;V>`` type constructor.

The container takes the ImGui id, the column count, the flags, an outer
size and an inner width — same five arguments the cpp ``BeginTable`` takes —
and brackets the matching ``EndTable``. Inside the body, the row/column
cursor calls are plain pass-throughs that resolve in the
``imgui_table_builtin`` module namespace (so the project-wide lint, which
forbids raw ``imgui::*`` calls in user code, stays satisfied without an
allow-list extension).

Source: ``examples/tutorial/data_table.das``.

************
Walkthrough
************

.. video:: data_table.mp4

The recording drives the sortable shape by clicking the real column headers:
clicking ``Name`` flips the active sort ascending to descending; clicking
``Type`` replaces the key and re-groups the rows by type; and **Shift+clicking**
``Value`` adds it as a secondary key (the ``2`` badge), so within each type the
rows break ties by value. Each click is verified against the re-rendered cell
text, so a header click that failed to re-sort would abort the recording.

.. literalinclude:: ../../../examples/tutorial/data_table.das
   :language: das
   :linenos:

Requires
========

One extra module on top of the baseline boost layer:

* ``imgui/imgui_table_builtin`` — the ``data_table`` container plus the
  six ``table_*`` snake_case primitives the body calls into.

Container shape
===============

``data_table`` follows the same named-tuple convention as the other
containers (``window`` / ``child`` / ``tab_bar``):

.. code-block:: das

   data_table(MY_TABLE, (text = "##rows", columns = 3,
                          flags = ImGuiTableFlags.Borders,
                          outer_size = float2(0.0f, 0.0f),
                          inner_width = 0.0f)) {
       // body
   }

``text`` is the ImGui id (use the ``##suffix`` convention to keep it
out of the visible label). ``columns`` is the column count;
``outer_size = float2(0,0)`` lets ImGui auto-size; ``inner_width = 0.0f``
means "no explicit inner width" (use the outer width).

Body primitives
===============

The six body cursor calls are plain ``def public`` wrappers — same
arguments as the underlying ImGui calls, snake_case names:

* ``table_setup_column(label, flags?, init_width?, user_id?)`` — declare a
  column before the header row.
* ``table_setup_scroll_freeze(cols, rows)`` — pin the first N columns / M
  rows during scrolling.
* ``table_headers_row()`` — submit the header row using the
  ``table_setup_column`` labels.
* ``table_next_row(flags?, min_row_height?)`` — start the next row.
* ``table_set_column_index(col) -> bool`` — jump to a specific column;
  returns ``true`` when the column is visible.
* ``table_next_column() -> bool`` — advance one column (or wrap to next
  row); also returns the visibility bool.

``TableState`` (the container's state struct) echoes per-call config —
columns, flags, outer_size, inner_width — so snapshot consumers can read
the table's shape without parsing the daslang call site. Multi-select
hand-off remains deferred — pinned ImGui 1.92.6 exposes
``BeginMultiSelect`` / ``ImGuiMultiSelectIO``, but the boost-wrapper
hand-off has not been designed yet. A custom row-bg callback API would
extend the state additively if added later.

Sortable tables
===============

The tutorial table uses the full sortable shape — ``Sortable | SortMulti
| Reorderable | Hideable`` flags on the table, a stable ``user_id`` on
each ``table_setup_column``, and a ``sort_specs()`` block-arg helper
inside the body that ImGui fires when the sort state goes dirty.

* ``ImGuiTableFlags.Sortable`` enables single-column sort (click any
  header). Adding ``ImGuiTableFlags.SortMulti`` enables multi-column
  sort (Shift+click a second header to append a secondary sort key).
* ``table_setup_column("Name", flags, init_width, user_id=COL_NAME)``
  tags the column with a stable identifier (a ``uint``). The sort
  comparator dispatches on ``column_user_id`` rather than
  ``column_index``, so the sort stays correct after the user reorders
  columns via drag.
* ``sort_specs() $(specs) { ... }`` is the wrapper that captures the
  ImGui ``TableGetSortSpecs()`` data, converts each
  ``ImGuiTableColumnSortSpecs`` entry into a daslang-friendly
  ``TableSortSpec`` (with ``column_index``, ``column_user_id``,
  ``sort_order``, ``sort_direction``), invokes the body block with the
  array, and auto-clears the ``SpecsDirty`` flag on return. The block
  only fires when ImGui reports dirty (header click), so the comparator
  cost is paid once per sort change rather than every frame.

The block-body comparator pattern walks the specs in priority order and
returns on the first spec that disambiguates a pair — ``sort_order = 0``
is the primary key, ``sort_order = 1`` is the first tiebreak, and so on.
A final tiebreak on a unique field (here: ``name``) keeps the order
total.

For a complete standalone example (inventory table with id / name / qty
columns and a multi-key comparator), see ``examples/features/sort_specs.das``.

Why the name
============

``table`` is a daslang reserved keyword — the type constructor for
``table<K;V>`` (the hash-map type). Defining a function or container named
``table`` is a parse error. ``data_table`` follows the standard UI-library
term (Material's ``DataTable``, Bootstrap's ``table``, etc.) and disambiguates
from the type namespace at every call site.

Standalone vs live
==================

Same convention as previous tutorials. ``daslang.exe`` runs the table
once and exits at ``exit_requested()``. ``daslang-live`` keeps the window
open and reloads on source edits.

.. seealso::

   Full source: :download:`examples/tutorial/data_table.das <../../../examples/tutorial/data_table.das>`

   Sortable inventory example: :download:`examples/features/sort_specs.das <../../../examples/features/sort_specs.das>`
   — the canonical ``sort_specs()`` reference with a multi-key comparator.

   Integration tests: ``tests/integration/test_app_small_property_editor.das``
   (uses the same ``data_table`` container surface) and
   ``tests/integration/test_sort_specs.das`` (smoke for the sortable rail).

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
