---
slug: dasimgui-new-widget-module-needs-das-module-entry
title: When adding a new .das sibling module under modules/dasImgui/widgets/, why does `require imgui/<name>` fail with "missing prerequisite; file not found" (error 20605)?
created: 2026-05-15
last_verified: 2026-05-15
links: []
---

**Cause:** dasImgui uses `.das_module` (at `modules/dasImgui/.das_module`) to map `imgui/<name>` requires to widgets/*.das via `register_native_path("imgui", "name", "{project_path}/widgets/file.das")`. A new .das file under `widgets/` is **not** auto-discovered; the module path registration is what makes `require imgui/<name>` resolve.

**Fix:** add a `register_native_path` line in `modules/dasImgui/.das_module`'s `initialize()` function alongside the existing entries for `imgui_lint`, `imgui_live`, etc.

```das
register_native_path("imgui", "imgui_harness", "{project_path}/widgets/imgui_harness.das")
```

No daslang rebuild needed — `.das_module` runs at module-load time on every invocation.

**Counterpart for daslib/:** the `daslib/` path uses a different mechanism (auto-discovered from `modules/dasImgui/daslib/*.das`) and doesn't need `.das_module` entries.

**Symptom this caused:** PR creating `widgets/imgui_harness.das` saw `require imgui/imgui_harness` fail with error 20605 until the `.das_module` line was added.

## Questions
- When adding a new .das sibling module under modules/dasImgui/widgets/, why does `require imgui/<name>` fail with "missing prerequisite; file not found" (error 20605)?
