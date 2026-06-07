dasImguiNodeEditor
==================

dasImguiNodeEditor is the daslang binding for `imgui-node-editor
<https://github.com/thedmd/imgui-node-editor>`_, the node/graph editor built on
Dear ImGui. It sits beside :doc:`dasimgui` as a second ImGui-extension binding —
it depends on dasImgui and loads alongside it, without modifying it.

Where to go
-----------

* **Documentation**: https://borisbat.github.io/dasImguiNodeEditor/
* **Repository**: https://github.com/borisbat/dasImguiNodeEditor

What's there
------------

* Bindings covering the imgui-node-editor surface — nodes, pins, links, the
  app-owned editor context, selection, context menus, node drag and link flow,
  groups, style vars/colors, pin pivots, view operations, the node-background
  drawlist, canvas theming, and clipboard/shortcuts.
* A ``boost`` macro layer (``imgui_node_editor_boost_v2``) that wraps the
  ``Begin*`` / ``End*`` pairs into scoped blocks, mirroring dasImgui's v2 style.
* A snapshot-based test/playwright layer (``imgui_node_editor_playwright``) that
  drives the graph headlessly under ``dastest``, so node editors are testable in
  CI without a GL context.
* Examples and tutorials, each runnable as a standalone binary and live-reloaded
  under ``daslang-live``.

Versioning
----------

The documentation tracks the **v2** surface, matching dasImgui's pinned Dear ImGui
release.
