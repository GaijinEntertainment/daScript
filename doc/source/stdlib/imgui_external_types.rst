.. _stdlib_imgui_external_types_section:

***************
External types
***************

Types defined by C++-bound sister modules (``imgui`` core bindings, ``dasGLFW``)
that dasImgui references but does not own. Until those modules ship their own
Sphinx sites and an intersphinx bridge, this page anchors the labels so the
generated module pages can link without dangling references.

.. _enum-imgui-ImGuiCol:

``imgui::ImGuiCol``
===================

Style-color enum exposed by the ``imgui`` builtin module (the C++ Dear ImGui
binding layer). Indexes a slot in the active style's color table — passed to
:ref:`push_style_one <function-imgui_style_builtin_push_style_one_ImGuiCol_uint>`,
``push_style_color``, ``pop_style_color``, and friends. Values map directly to
upstream ``ImGuiCol_*`` enumerants (``Text``, ``WindowBg``, ``Button``, …); see
the `Dear ImGui ImGuiCol_ reference <https://github.com/ocornut/imgui/blob/master/imgui.h>`_
for the full enumerator list.

.. _enum-imgui-ImGuiWindowFlags:
.. _enum-imgui-ImGuiChildFlags:
.. _enum-imgui-ImGuiTabBarFlags:
.. _enum-imgui-ImGuiTabItemFlags:
.. _enum-imgui-ImGuiTreeNodeFlags:
.. _enum-imgui-ImGuiPopupFlags:
.. _enum-imgui-ImGuiTableFlags:
.. _enum-imgui-ImGuiTableColumnFlags:
.. _enum-imgui-ImGuiTableRowFlags:

``imgui::ImGui*Flags``
======================

Bitfield enums exposed by the ``imgui`` builtin module: ``ImGuiWindowFlags``,
``ImGuiChildFlags``, ``ImGuiTabBarFlags``, ``ImGuiTabItemFlags``,
``ImGuiTreeNodeFlags``, ``ImGuiPopupFlags``, ``ImGuiTableFlags``,
``ImGuiTableColumnFlags``, ``ImGuiTableRowFlags``. Passed to the corresponding
``imgui::Begin*`` / ``BeginTabBar`` / ``BeginTabItem`` / ``TreeNodeEx`` /
``OpenPopup`` / ``BeginTable`` / ``TableSetupColumn`` / ``TableNextRow`` calls.
Values map directly to upstream ``ImGui*Flags_*`` enumerants; see the
`Dear ImGui flag enums <https://github.com/ocornut/imgui/blob/master/imgui.h>`_
for the full enumerator lists.

.. _enum-imgui-ImGuiCond:

``imgui::ImGuiCond``
====================

Window-state condition enum exposed by the ``imgui`` builtin module. Used as
the trailing ``cond`` argument to ``SetNextWindowPos`` / ``SetNextWindowSize`` /
``set_window_size`` etc. to gate when the value applies (``Always`` / ``Once`` /
``FirstUseEver`` / ``Appearing``). Values map directly to upstream ``ImGuiCond_*``
enumerants; see the
`Dear ImGui ImGuiCond_ reference <https://github.com/ocornut/imgui/blob/master/imgui.h>`_
for the full list.

.. _alias-imvec2:
.. _alias-imvec4:
.. _alias-imcolor:

``imgui::ImVec2`` / ``imgui::ImVec4`` / ``imgui::ImColor``
==========================================================

Core vector aliases exposed by the ``imgui`` builtin module and consumed across
the style/layout APIs. ``ImVec2`` is used for 2D positions/sizes (window pos,
cursor pos, spacing, paddings). ``ImVec4`` is used for RGBA colors and 4D style
values. ``ImColor`` is a ``float4``-aliased convenience wrapper for RGBA
colors. These aliases map directly to Dear ImGui's ``ImVec2``/``ImVec4``/
``ImColor`` structs.

.. _handle-imgui-ImGuiViewport:

``imgui::ImGuiViewport``
========================

Per-viewport handle exposed by the ``imgui`` builtin module. Returned (as
``ImGuiViewport?``) from ``GetMainViewport`` and consumed by the
``viewport_center`` wrapper plus the multi-viewport docking pipeline. Fields
``Pos`` / ``Size`` / ``WorkPos`` / ``WorkSize`` / ``ID`` are directly accessible;
the ``GetCenter()`` method is wrapped as ``viewport_center(self)`` in
``imgui/imgui_containers_builtin``. See
`ImGuiViewport in imgui.h <https://github.com/ocornut/imgui/blob/master/imgui.h>`_
for the upstream definition.

.. _handle-imgui-ImGuiSizeCallbackData:

``imgui::ImGuiSizeCallbackData``
================================

Per-resize callback payload passed by ImGui to the
``SetNextWindowSizeConstraints`` callback (``ImGuiSizeCallback``). Daslang
exposes this through the ``ImGuiSizeConstraints`` lambda wrapper in
``imgui/imgui_window_constraints_builtin``: the lambda body reads
``data.CurrentSize`` and writes ``data.DesiredSize`` to re-shape the requested
window size (aspect-ratio lock, fixed-step quantization, "always square", …).
See
`ImGuiSizeCallbackData in imgui.h <https://github.com/ocornut/imgui/blob/master/imgui.h>`_
for the upstream definition.

.. _handle-glfw-GLFWwindow:

``glfw::GLFWwindow``
====================

Opaque GLFW window handle exposed by the ``dasGLFW`` sister module. Passed
through the live-reload host's window lifecycle (creation, swap, event drain)
and forwarded into ImGui's GLFW backend. See the
`GLFW documentation <https://www.glfw.org/docs/latest/group__window.html>`_
for the handle's C-side semantics.

.. _handle-imgui-ImTextureRef:

``imgui::ImTextureRef``
=======================

Texture reference taken by the image widgets (``Image``, ``ImageButton``, draw
list image primitives) since Dear ImGui 1.92: wraps either a backend
``ImTextureID`` or a pointer to backend-managed ``ImTextureData``.
``image_texture_id(texture)`` in ``imgui/imgui_widgets_builtin`` unwraps the
stable 64-bit id for hashing/caching across frames. See
`ImTextureRef in imgui.h <https://github.com/ocornut/imgui/blob/master/imgui.h>`_
for the upstream definition.

.. _handle-imgui-imguitextfilter:

``imgui::ImGuiTextFilter``
==========================

C++ helper for parsed comma-separated include/exclude filter expressions
(``-prefix`` excludes), with an inline editor renderable via ``Draw(label,
width)`` and a ``PassFilter(text)`` predicate. Held inline as a value field
of ``TextFilterState`` backing the ``text_filter`` boost widget. See
`ImGuiTextFilter in imgui.h <https://github.com/ocornut/imgui/blob/master/imgui.h>`_
for the upstream definition.
