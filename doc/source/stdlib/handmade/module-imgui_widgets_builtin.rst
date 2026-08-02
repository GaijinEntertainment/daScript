Built-in widgets shipped on top of the boost layer — the same ``[widget]``
mechanism users get. Covers the button family (``button``, ``small_button``,
``arrow_button``, ``image_button``, ``invisible_button``, ``checkbox``,
``radio_button``), the slider / drag / input numeric families across
scalar and 2/3/4-vector ``float`` / ``int`` variants, color edits and pickers,
selection widgets (``combo``, ``list_box``, ``selectable``), plot helpers
(``plot_lines``, ``plot_histogram``, ``progress_bar``), and output widgets
(``text_show``, ``label_text``, ``bullet_text``, ``help_marker``).

Each widget def shapes its body around the per-kind state struct from
:ref:`imgui_boost_runtime <stdlib_imgui_boost_runtime>` plus a brief
finalize helper. The helpers — ``click_finalize``, ``toggle_finalize``,
``radio_int_finalize``, ``pending_value_finalize`` — collapse the repetitive
``widget_finalize`` lambda construction per family so each def stays focused
on the ImGui call and per-frame state mutation.
