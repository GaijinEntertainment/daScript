# dasVulkan

Vulkan bindings for [daslang](https://dascript.org/), generated from the Khronos `vk.xml` registry.

- **`vulkan`** — the raw binding: the full Vulkan API surface (core + extensions), generated as a daslang C++ module dispatching through [volk](https://github.com/zeux/volk).
- **`vulkan_boost`** — the ergonomic layer: ownership-tracking handles with `finalize`, idiomatic structs (arrays instead of count+pointer pairs, auto-filled `sType`), named arguments with defaults, and block-bracketing helpers.

The binding generator is itself written in daslang (`generator/`), parsing `vk.xml` with `dasPUGIXML`.

## Status

Working. The raw `vulkan` module covers the full API surface, and the generated `vulkan_boost` layer makes it ergonomic — RAII-owned handle wrappers, sType-filling constructors, `array<T>`-based struct views, and high-level builders. Three offscreen examples (triangle, compute, device enumeration) run on a real GPU and on Mesa lavapipe (Linux CI); a macOS CI lane build-gates the MoltenVK path (GitHub's macOS runners have no GPU to render on, so render testing stays on real hardware).

```
daslang -load_module <path-to-dasVulkan> examples/offscreen_triangle_boost.das   # boost triangle
daslang -load_module <path-to-dasVulkan> examples/compute_boost.das              # boost compute
daslang -load_module <path-to-dasVulkan> examples/enumerate.das                  # device info
```

### Layers

- **`vulkan`** — the raw binding, generated from `vk.xml` over volk.
- **`vulkan/vulkan_boost`** — the ergonomic layer: `create_instance` / `create_device` / `create_image` … return RAII wrappers (`var inscope` destroys in reverse), VkFlags are daslang bitfields (`usage.color_attachment = true`), and builders (`build_offscreen_target`, `run_cmd_sync`, `record_render_pass`) collapse the boilerplate. The boost triangle is ~1/3 the lines of the raw one and renders byte-identically.

## Vendored dependencies

| What | Version | License |
|---|---|---|
| [volk](https://github.com/zeux/volk) | vulkan-sdk-1.4.350.0 | MIT |
| [Vulkan-Headers](https://github.com/KhronosGroup/Vulkan-Headers) (headers + `registry/vk.xml`) | vulkan-sdk-1.4.350.0 | Apache-2.0 / MIT |

Building the module requires no Vulkan SDK — the vendored headers and volk's runtime loading cover it. The LunarG SDK (validation layers, glslang) is recommended for development.

**Running** needs a Vulkan loader + driver at runtime. Linux: `mesa-vulkan-drivers` (lavapipe) or a vendor ICD. **macOS**: `brew install molten-vk vulkan-loader vulkan-tools` — dasVulkan finds the Homebrew loader and enables MoltenVK portability automatically, so the offscreen examples and the windowed `examples/window_triangle.das` just work on Apple GPUs with no env vars.

## Acknowledgements

Two earlier daScript Vulkan binding projects informed the design (clean-room — patterns, not code):

- [e-dog/dasVulkan](https://github.com/e-dog/dasVulkan) — vk.xml-driven generation over volk.
- [olegus8/dasVulkan](https://github.com/olegus8/dasVulkan) — the boost-layer ergonomics (ownership-tracked handles, view structs, the simple-app frame loop).

## License

MIT
