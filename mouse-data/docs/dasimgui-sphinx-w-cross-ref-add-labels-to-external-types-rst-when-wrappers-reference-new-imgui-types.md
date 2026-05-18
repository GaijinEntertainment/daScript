---
slug: dasimgui-sphinx-w-cross-ref-add-labels-to-external-types-rst-when-wrappers-reference-new-imgui-types
title: When I add a new dasImgui boost-surface wrapper that references an ImGui type (e.g. `ImGuiTableFlags`, `ImGuiCond`, `ImGuiViewport`), why does CI sphinx-W fail with "undefined label" warnings even though the wrapper compiles fine?
created: 2026-05-17
last_verified: 2026-05-17
links: []
---

**Because `imgui2rst.das` (the dasImgui doc generator) emits `:ref:\`enum-imgui-<TypeName>\`` / `:ref:\`handle-imgui-<TypeName>\`` cross-refs in every generated `stdlib/generated/<module>.rst` whenever a wrapper's signature mentions an imgui-owned type — but those labels are NOT in any generated file (they live in `doc/source/stdlib/external_types.rst`, which is hand-maintained).** New wrapper signature → new cross-ref → if the matching `.. _enum-imgui-<TypeName>:` / `.. _handle-imgui-<TypeName>:` anchor doesn't exist in `external_types.rst`, sphinx-W explodes the CI build with `undefined label: enum-imgui-imguitableflags` etc.

**Surface:** local `daspkg build` won't catch it — only sphinx-W (`sphinx-build -W -b html doc/source doc/build/html`) does, which runs in the docs CI job. AND local sphinx-W might pass anyway if you don't first invoke `imgui2rst.das` to regenerate `stdlib/generated/`; CI does this in-pipeline.

**The discipline:** any time you add a wrapper module / signature in `widgets/` that mentions a previously-unreferenced ImGui type, add the cross-ref anchor to `doc/source/stdlib/external_types.rst` in the SAME PR. The label format is lowercased-by-sphinx but you write the proper case:

```rst
.. _enum-imgui-ImGuiTableFlags:
.. _enum-imgui-ImGuiTableColumnFlags:
.. _enum-imgui-ImGuiTableRowFlags:

``imgui::ImGui*Flags``
======================
Bitfield enums exposed by the ``imgui`` builtin module: ... list ...

.. _enum-imgui-ImGuiCond:

``imgui::ImGuiCond``
====================
[description]

.. _handle-imgui-ImGuiViewport:

``imgui::ImGuiViewport``
========================
[description]
```

Multiple anchor lines stacked above ONE heading is the working pattern (sphinx points all of them at the same section). Used for `ImGui*Flags` (one heading, ~9 stacked labels) and `ImGuiViewport` / `ImGuiSizeCallbackData` (one heading each).

**Concrete instance (2026-05-17, PR #45):** `widgets/imgui_table_builtin.das` introduced `data_table(..., flags : ImGuiTableFlags, ...)` + 6 pass-throughs mentioning `ImGuiTableColumnFlags` / `ImGuiTableRowFlags`; `widgets/imgui_window_constraints_builtin.das` introduced `ImGuiSizeCallbackData` in the lambda signature; `widgets/imgui_containers_builtin.das` added `set_window_size(cond : ImGuiCond)` + `viewport_center(self : ImGuiViewport)`. Build went green locally, sphinx-W in CI rejected the PR until 6 new labels (3 table flags + ImGuiCond + ImGuiViewport + ImGuiSizeCallbackData) landed in `external_types.rst`.

**Companion:** `feedback_dasimgui_sphinx_w_apng_must_exist.md` is the cousin for static assets (`.. image:: _static/tutorials/<name>.apng` must exist at CI time). Both fail in the same job; check both when sphinx-W errors land on a wrapper-addition PR.

## Questions
- When I add a new dasImgui boost-surface wrapper that references an ImGui type (e.g. `ImGuiTableFlags`, `ImGuiCond`, `ImGuiViewport`), why does CI sphinx-W fail with "undefined label" warnings even though the wrapper compiles fine?
- Where do I add the `enum-imgui-<Name>` / `handle-imgui-<Name>` Sphinx anchors that `imgui2rst.das` emits cross-refs to?
- Why does my local sphinx-W pass on a dasImgui doc build but CI fails on the same commit?
