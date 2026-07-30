Stateful layout helpers — splits, columns, and a fixed-orientation sidebar —
plus thin wrappers around ImGui's column primitives. ``split_h`` / ``split_v``
divide the available region between two ``BeginChild`` panes separated by a
draggable handle, with the split ratio persisted in ``SliderStateFloat``.
``dock_left`` is a stable-position left sidebar (not real docking — that lives
in ``imgui_internal.h`` and is off-limits) with a draggable right edge. The
``columns(N, ${col0}, ${col1}, …)`` call_macro expands to
``columns_open(N)`` / ``next_col()`` / ``columns_close()`` with one block
literal per column, so the column count must match the block-arg count
statically.
