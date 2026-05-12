---
slug: how-do-i-pack-an-im-col32-color-from-dasimgui-v2-code-without-depending-on-the-v1-daslib-imgui-boost-path
title: How do I pack an IM_COL32 color from dasImgui v2 code without depending on the v1 daslib/imgui_boost path?
created: 2026-05-11
last_verified: 2026-05-11
links: []
---

`IM_COL32` only exists in v1 `daslib/imgui_boost.das:154` — v2 paths (`imgui/imgui_boost_v2`, `imgui/imgui_widgets_builtin`, etc.) don't have it. To avoid pulling the v1 module into a v2 file, write your own packer using dasIMGUI's exported shift constants:

```das
def rgba(r, g, b, a : int) : uint {
    return ((uint(a) << uint(IM_COL32_A_SHIFT))
          | (uint(r) << uint(IM_COL32_R_SHIFT))
          | (uint(g) << uint(IM_COL32_G_SHIFT))
          | (uint(b) << uint(IM_COL32_B_SHIFT)))
}
```

The `IM_COL32_*_SHIFT` constants are bound globals — defined in `modules/dasImgui/src/dasIMGUI.const.inc:6-11` and reach daslang via the imgui module.

**Byte-order note:** dasIMGUI is compiled with `IMGUI_USE_BGRA_PACKED_COLOR` defined (so `R_SHIFT=16`, `B_SHIFT=0`, byte order `0xAARRGGBB`). The renderer side decodes the same way, so colors round-trip correctly. Verified: `rgba(255, 204, 51, 255)` produces a visibly orange/amber stroke (narrate border in the visual-aids tour example).

Live reference: `modules/dasImgui/widgets/imgui_visual_aids.das` defines this as `def public rgba(...)` so other modules can spell colors as `imgui_visual_aids::rgba(255, 0, 0, 255)` without re-implementing.

## Questions
- How do I pack an IM_COL32 color from dasImgui v2 code without depending on the v1 daslib/imgui_boost path?
