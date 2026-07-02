# Vulkan + ImGui cube

A spinning cube rendered with **dasVulkan**, with a **Dear ImGui** control panel whose slider drives
the rotation speed — and the ImGui UI is rasterized by a renderer backend written **entirely in
daslang** (`vulkan/vulkan_imgui_driver`, now part of dasVulkan). No C++.

The point: a Dear ImGui renderer backend is just a draw-data replay driver. dasImgui exposes the whole
`ImDrawData` surface to daslang (vertex/index buffers, draw commands, clip rects, the 1.92 texture
queue), and dasVulkan exposes the GPU primitives, so the bridge is pure das.

## The bridge lives in dasVulkan

Two opt-in modules ship with [dasVulkan](https://github.com/borisbat/dasVulkan):

- **`vulkan/vulkan_imgui_driver`** — the 100%-daslang ImGui→Vulkan renderer backend (the bridge):
  pipeline, the 1.92 dynamic-texture queue, and the per-frame draw-data replay (upload
  vertices/indices, walk the draw commands, set the per-command scissor, bind the command's texture,
  indexed draw). Its vertex/fragment shaders are authored in daslang and lowered to SPIR-V by dasSpirv
  (the daslang equivalent of the stock `imgui_impl_vulkan` glsl), inlined into the module.
- **`vulkan/vulkan_live`** — daslang-live commands (screenshot / record / stats), the Vulkan mirror of
  `opengl_live`. It owns nothing: you hand it the device + swapchain once with `vk_live_set_target`,
  then `vk_live_draw_frame(...)` (a drop-in for `draw_frame`) renders and lets the live commands read
  the swapchain back.

## Files

- `main.das` — the app: the `vulkan/vulkan_imgui_app` harness owns the window/device/swapchain and
  wires the live stack; the cube draws in the `harness_end_frame` scene block (back-face culled, so a
  convex mesh needs no depth buffer), and the panel is imgui_boost_v2 (`CUBE_WIN/SPEED`).
- `cube_shaders.das` — the cube's lit shaders (dasSpirv), push-constant-only.

## Running

The example depends on two external modules — [dasVulkan](https://github.com/borisbat/dasVulkan) and
[dasImgui](https://github.com/borisbat/dasImgui) — declared in `.das_package`. Install them once with
`daspkg` (clones + builds the native modules into `modules/`):

```
cd examples/vulkan_imgui_cube
daslang ../../utils/daspkg/main.das -- install
```

Then run it — drag the slider to change the spin speed, press **P** to save a screenshot
(`screenshot.png`, captured by `vulkan_live`), close the window to exit:

```
daslang -project_root . main.das
```

## Live control

The UI is imgui_boost_v2, so under daslang-live every widget is drivable:

```
daslang-live -project_root . main.das
curl -X POST -d '{"name":"imgui_force_set","args":{"target":"CUBE_WIN/SPEED","value":4.0}}' localhost:9090/command
curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command
curl -X POST -d '{"name":"screenshot","args":{"file":"shot.png"}}' localhost:9090/command
```
