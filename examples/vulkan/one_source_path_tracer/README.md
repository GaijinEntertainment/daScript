# One source, four engines — a daslang path tracer

The same daslang functions — scene, camera, RNG, materials, the NEE integrator — run on four
execution tiers, live-switchable from an ImGui panel:

| Engine | What runs | 512², measured on an RTX 3060 Ti box |
|---|---|---|
| CPU — 1 thread | `pt_core` in the interpreter (or JIT with `-jit`) | ~0.03 / ~0.4 Mpaths/s |
| CPU — band threads | the same rows split across `new_thread` bands | ~0.1 / ~1.6 Mpaths/s |
| GPU — dasSpirv compute | the same functions lowered to a SPIR-V compute kernel | ~110 Mpaths/s |
| GPU — hardware ray tracing | raygen/miss/closest-hit, also lowered by dasSpirv; hardware traversal replaces the software intersector | ~330 Mpaths/s |

No GLSL anywhere. The scene lives in `@ssbo` globals the CPU fills and reads directly while the GPU
binds them as storage buffers, and the hardware-RT BLAS is built from the same arrays. The CPU and
compute engines produce pixel-identical images (same PCG RNG streams); hardware RT differs only at
watertight-traversal edges.

## Files

- `pt_core.das` — the shared core: Cornell scene, Möller–Trumbore intersector, PCG RNG, NEE + mirror
  integrator, tonemap. Written to the dasSpirv lowering rules (vector params, no recursion) so every
  function runs on all four tiers verbatim.
- `pt_shaders.das` — the GPU entry points: the compute megakernel, the RT stages (payload carries
  t + primitive id; all shading stays in raygen, reusing the shared functions), and the display quad.
- `main.das` — the app: vulkan_imgui_app harness, the band-thread state machine, the engine stacks,
  and the imgui_boost_v2 panel.
- `pt_reference.das` — headless CPU renderer, writes `pt_reference.png` (the pixel-parity oracle).

## Running

The example depends on [dasVulkan](https://github.com/borisbat/dasVulkan) and
[dasImgui](https://github.com/borisbat/dasImgui), declared in `.das_package`. Install once:

```
cd examples/vulkan/one_source_path_tracer
daslang ../../../utils/daspkg/main.das -- install
```

Then:

```
daslang -project_root . main.das          # interpreter CPU tiers
daslang -jit -project_root . main.das     # JIT CPU tiers
```

Click the engine radios; watch the samples counter and Mpaths/s. **P** saves a screenshot. Flags:
`--engine=<Single|Threads|Compute|RayTrace>`, `--max-frames=N` (renders N frames, writes
`one_source_path_tracer_proof.png`, exits — for smoke runs and screenshots). Hardware RT needs
`VK_KHR_ray_tracing_pipeline`; without it the radio reports unavailable and the other three run.

## Live control

The UI is imgui_boost_v2, so under daslang-live every widget is drivable:

```
daslang-live -project_root . main.das
curl -X POST -d '{"name":"imgui_force_set","args":{"target":"PT_WIN/ENGINE","value":3}}' localhost:9090/command
curl -X POST -d '{"name":"imgui_click","args":{"target":"PT_WIN/ENGINE#1"}}' localhost:9090/command
curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command
curl -X POST -d '{"name":"screenshot","args":{"file":"shot.png"}}' localhost:9090/command
```

`ENGINE` values: 0 = 1 thread, 1 = band threads, 2 = compute, 3 = hardware RT; the threads slider is
`PT_WIN/THREADS`. Editing `pt_core.das` and reloading re-lowers the shaders — CPU and GPU pick up the
change together.
