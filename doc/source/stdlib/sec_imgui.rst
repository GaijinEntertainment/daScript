.. _stdlib_imgui_section:

***********************
GUI (Dear ImGui)
***********************

dasImgui — the daslang binding for `Dear ImGui <https://github.com/ocornut/imgui>`_,
organised by area: the boost macro spine, builtin widgets, hot-reload + transport,
and the testing toolkit. Built-in Dear ImGui functions (``imgui::*``) are not
duplicated here; consult the upstream Dear ImGui documentation for the C++ surface.
This documents the **v2** surface — the legacy ``imgui/imgui_boost`` (v1) module
is registered but undocumented.

Start with :doc:`/reference/tutorials/imgui/application_lifecycle`: one
application structure for standalone, live-reload, and headless execution.

.. toctree::

   sec_imgui_boost.rst
   sec_imgui_widgets.rst
   sec_imgui_live.rst
   sec_imgui_testing.rst
   imgui_external_types.rst
   imgui_internal_families.rst
