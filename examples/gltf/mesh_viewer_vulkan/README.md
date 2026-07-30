# dasGLTF Mesh Viewer (Vulkan + Dear ImGui)

The Vulkan sibling of `mesh_viewer_opengl`: an interactive glTF 2.0 viewer built on the in-tree
**dasGLTF** module (`modules/dasGLTF`), the **dasVulkan** renderer (`vulkan/vulkan_gltf` — the same
Cook-Torrance PBR, sharing `gltf/gltf_pbr_common` math with the GL adapter), and **dasImgui** for
the UI. It loads a `.glb`/`.gltf`, frames it automatically, orbits it under the metallic-roughness
PBR shader, and drives the model transform, animation playback and lighting from two ImGui windows.

The scene renders into an offscreen color+depth target (the Vulkan ImGui harness swapchain pass has
no depth attachment), which a fullscreen triangle composites under the UI each frame.

## Run

dasVulkan is an external dependency, pulled via daspkg from this example's `.das_package`
(dasImgui ships in-tree with daslang — nothing to install for it):

```sh
# once — installs dasVulkan into ./modules (gitignored), built against your daslang SDK
daslang utils/daspkg/main.das -- install --root examples/gltf/mesh_viewer_vulkan

# then run the viewer, rooted here so the deps resolve
daslang -project_root examples/gltf/mesh_viewer_vulkan examples/gltf/mesh_viewer_vulkan/main.das
```

Pass a model path to load your own; without one it opens the shipped **BoomBox** (full PBR):

```sh
daslang -project_root examples/gltf/mesh_viewer_vulkan \
    examples/gltf/mesh_viewer_vulkan/main.das examples/gltf/media/Fox.glb
```

`Fox.glb` exercises the animation UI (3 clips: Survey / Walk / Run) through the skinned pipeline
(joint-palette UBO).

## Controls

- **drag** — orbit  ·  **scroll** — zoom  ·  **P** — screenshot (`screenshot.png`)
- **Model** window — open a file, pick an animation clip, play/scrub time, rotate/scale, auto-rotate
- **Lighting** window — light direction, intensity, color, ambient, environment amount, reset to defaults

Metals reflect an equirect HDR environment (`tutorials/_assets/hdri/cannon_2k.hdr`, CC0 — the same
map OpenGL tutorial 12 uses); the **Environment** slider scales it, 0 falls back to flat ambient.
