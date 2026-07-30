Runtime support layer for the ``[widget]`` / ``[container]`` macros: the
per-kind state structs, the per-frame widget registry, the ``imgui_*``
live-command dispatcher, and the AwaitModifiers / Result framing that lets
clients drive ImGui from outside the GLFW main thread. Widget state structs
mix ``@live`` fields (preserved across hot-reload) and ``@optional`` fields
(transient, dropped from JV when zero-valued). Common per-frame ImGui state —
``hex_id``, bbox, hover / active / focus — lives in :ref:`WidgetEntry
<struct-imgui_boost_runtime-WidgetEntry>`, not the per-kind structs, so each
state struct stays small.

The live-command surface (``imgui_snapshot``, ``imgui_click``, ``imgui_force_set``,
``imgui_open``, ``imgui_close``, ``imgui_focus``, ``imgui_await``) is the
wire-protocol that ``imgui_playwright`` and external test harnesses drive.
(Multi-frame drag/type live in ``imgui_live_core``'s ``imgui_mouse_play`` /
``imgui_key_type`` timelines, which playwright composes.) Commands resolve
targets via the registry's path key (e.g. ``MAIN_WIN/SETTINGS_TAB/RPS``) or
raw ``0x<hex_id>``; results carry an :ref:`AwaitResult
<struct-imgui_boost_runtime-AwaitResult>` whose ``await_*`` modifiers tell
the client to wait for quiescence or a specific frame number before reading
the response.
