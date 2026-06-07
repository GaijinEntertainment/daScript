dasImguiImplot
==============

dasImguiImplot is the daslang binding for `ImPlot
<https://github.com/epezent/implot>`_, the immediate-mode plotting library for
Dear ImGui. Like :doc:`dasimguinodeeditor`, it sits beside :doc:`dasimgui` as an
ImGui-extension binding — it depends on dasImgui and loads alongside it, without
modifying it.

Where to go
-----------

* **Documentation**: https://borisbat.github.io/dasImguiImplot/
* **Repository**: https://github.com/borisbat/dasImguiImplot

What's there
------------

* Bindings covering the ImPlot item surface — line / scatter / bars / stairs /
  shaded / stems / inf-lines / error-bars / digital / heatmap / histogram, plus
  the categorical bar-groups and pie items, over ``float`` / ``double`` / ``int``
  data.
* Two wrapper tiers: a thin v1 ``imgui_implot_boost`` and the v2 DSL
  ``imgui_implot_boost_v2`` with RAII ``plot`` / ``subplots`` scopes, axis setup,
  ``SetNext*`` styling, colormaps, and interactive drag tools.
* A snapshot-based test/playwright layer (``imgui_implot_playwright``) that
  serializes plot state — geometry, axis limits, hovered, mouse-plot-position,
  legend entries — each frame, so plots are queryable and testable headlessly
  under ``dastest``.
* Eleven progressive tutorials, each with a voiced, self-verifying screen
  recording, runnable as a standalone binary and live-reloaded under
  ``daslang-live``.

Versioning
----------

The documentation tracks **ImPlot v0.16**, the release that matches dasImgui's
pinned Dear ImGui version.
