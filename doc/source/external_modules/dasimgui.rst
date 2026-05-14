dasImgui
========

dasImgui is the daslang binding for `Dear ImGui <https://github.com/ocornut/imgui>`_.
It ships with the boost macro layer that compresses immediate-mode boilerplate
into block-style one-liners, full live-reload integration, and a snapshot-based
test harness that drives the widget tree headlessly under ``dastest``.

Where to go
-----------

* **Documentation**: https://borisbat.github.io/dasImgui/
* **Repository**: https://github.com/borisbat/dasImgui

What's there
------------

* Bindings covering the Dear ImGui surface, generated from the upstream
  ``imgui.h`` via the in-repo binder.
* The ``imgui_boost`` macro layer — ``window`` / ``tab_bar`` / ``tree_node``
  / ``table`` and friends as block macros, so the typical ``Begin*`` / ``End*``
  pair becomes a single scoped block.
* ``imgui_test``: a Playwright-style widget walker that asserts against the
  rendered tree without needing a GL context — usable from dastest, runnable
  in CI.
* Examples and tutorials under ``examples/features/``, every one runnable both
  as a standalone binary and live-reloaded under ``daslang-live``.

Versioning
----------

The documentation tracks the **v2.0** surface. The legacy ``daslib/imgui_boost``
(v1) is not documented at the new site; v1 users pin to the older daspkg
release.
