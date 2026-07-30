Live-reload lifecycle plus the synthetic-IO bypass driver. The ``imgui_live``
half wires up GLFW window creation, ImGui context construction, and the
``daslang-live`` integration so a script can hot-reload its widget code
without losing window state. The ``imgui_live_core`` half carries the
synth-IO transport — mouse + keyboard timelines, ``imgui_mouse_play`` /
``imgui_key_play`` event drains, side-mod alias emission, position-prefix
button ordering — so headless test recordings and playwright drivers
generate input that ImGui sees as if it came from the OS.

The synth path replaces the GLFW backend's ``UpdateMouseData`` polling
once per frame via :ref:`apply_synth_io_override
<function-imgui_visual_aids_apply_synth_io_override>`, reasserting the synth
cursor pose between ``Impl_NewFrame`` and ``NewFrame``. Visible widgets that
"don't fire" trace to this override race; the bypass driver always wins.
