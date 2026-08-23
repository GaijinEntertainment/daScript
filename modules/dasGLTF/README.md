# dasGLTF - glTF 2.0 loader + OpenGL PBR viewer for daslang

Native [glTF 2.0](https://www.khronos.org/gltf/) loading in pure daslang: parse `.glb` / `.gltf`
(embedded, base64, or external `.bin`) into a **backend-neutral scene**, then optionally map it to
OpenGL and render it with metallic-roughness PBR. No new C++ - the module is built entirely on
existing daslib infrastructure (`json_boost`, `base64`, `fio`, `stbimage`, `math`) and the native
shader DSL (`dasGlsl`).

The core (parse + scene + skinning + animation) is **GL-free and headless-usable** - good for
tooling, mesh processing, or a future Vulkan backend. The GL adapter and PBR shader are opt-in.

## Layout

Neutral core - `require gltf/gltf_boost` (never pulls in OpenGL):

- `gltf/gltf_types.das` - the agnostic data model (`GltfScene` and its parts)
- `gltf/gltf_accessor.das` - byte-view / component readers honoring componentType/type/normalized/byteStride/sparse
- `gltf/gltf_parse.das` - GLB container + `.gltf` + base64/external buffers + image decode -> `GltfScene`
- `gltf/gltf_scene.das` - world transforms, scene bounds, animation evaluation, joint matrices
- `gltf/gltf_boost.das` - re-exports the four above

OpenGL adapter - `require gltf/gltf_gl` + `gltf/gltf_pbr` (require `dasOpenGL` / `dasGlsl`):

- `gltf/gltf_gl.das` - `GltfScene` -> per-primitive VAO/VBO/EBO + textures (`GltfGlModel`); sRGB/linear
  texture pipeline, mipmaps, sampler wrap/filter, white + flat-normal defaults, equirect HDR
  environment loader (`gltf_gl_load_hdr_env`)
- `gltf/gltf_pbr.das` - portable Cook-Torrance GGX metal-rough shader (native DSL, `[vertex_program]`/
  `[fragment_program]`): tangent-space normals, occlusion, emissive, directional light + ambient or
  image-based ambient from an equirect HDR environment (`gltf_pbr_set_environment`, roughness-mip
  reflections), sRGB output, alpha OPAQUE/MASK/BLEND, doubleSided cull, GPU skinning (64-joint palette)
- `gltf/gltf_pbr_common.das` - backend-neutral PBR shader math (GGX BRDF, normal perturbation, vector-space
  skin blend, sRGB encode): pure scalar/vector/matrix functions both shader emitters lower as user
  functions, shared with the dasVulkan renderer

## Neutral core

```das
require gltf/gltf_boost

var scene <- load_gltf("model.glb")         // .glb / .gltf / base64 / external .bin
update_world_transforms(scene)              // fill GltfNode.world from the hierarchy
let (mn, mx) = gltf_scene_bounds(scene)     // world-space AABB (auto-framing)

// already have the bytes? parse in-memory (baseDir resolves external .bin/images; "" for self-contained .glb)
var scene2 <- load_gltf_from_memory(bytes, baseDir)
```

`GltfScene` holds `meshes / materials / textures / images / samplers / nodes / roots / skins /
animations` (see `gltf_types.das`). Vertices are decoded to a fixed `GltfVertex` layout
(position, normal, tangent, uv0/uv1, color0, joints0, weights0 - joints/weights decoded to `float4`
at parse time), so integer-lane and normalized attributes need no downstream plumbing.

### Animation + skinning

```das
evaluate_animation(scene, animIndex, timeSeconds)   // STEP / LINEAR / CUBICSPLINE
                                                     // writes node TRS -> recomposes -> world transforms
let jm = gltf_joint_matrix(scene, skinIndex, j)      // jointNode.world * inverseBind[j]
```

Skinned geometry renders in skin space (mesh-node transform omitted per spec), so a skinned node
draws with `u_model = identity` and the joint palette supplies the deformation. The blend is done in
**vector space** (`world = Sum (palette[j_i] * p) * w_i`) because the shader DSL has `mat*vec` / `vec*scalar`
/ `vec+vec` but no `mat+mat` - the same body ports to Vulkan unchanged.

## OpenGL rendering

```das
require gltf/gltf_boost
require gltf/gltf_gl
require gltf/gltf_pbr

var model <- upload_gltf(scene)             // GL buffers + textures; delete to release
var renderer <- gltf_pbr_create()           // compile + link the PBR program

gltf_pbr_render(renderer, model, scene, view, proj, camPos,
    GltfPbrLight(direction = float3(0.3, 0.7, 0.5), intensity = 3.0))
```

## Examples

- `examples/opengl/10_hello_gltf.das` - static PBR viewer (orbit camera); defaults to the shipped
  BoomBox, pass a path as `argv[1]`.
- `examples/opengl/11_hello_gltf_animation.das` - skinned-animation viewer; defaults to Fox
  (3 clips), SPACE cycles clips.

Sample `.glb` assets ship under `examples/gltf/media/` (see `examples/gltf/PROVENANCE.md`).

## Scope

Full metallic-roughness PBR and skeletal skinning + TRS keyframe animation. Morph targets, saving/export,
mesh optimization, `KHR_draco_mesh_compression`, and the strip/fan/loop primitive topologies
(`LINE_LOOP`/`LINE_STRIP`/`TRIANGLE_STRIP`/`TRIANGLE_FAN`) are out of scope - geometry is expected as
indexed `TRIANGLES`, which is what glTF exporters emit almost universally. The neutral scene format is
designed to feed a separate mesh optimizer. A Vulkan backend (`gltf_vk`) is a planned parallel to `gltf_gl`.

glTF extensions are not implemented. An asset whose `extensionsRequired` names any extension **fails
closed** (empty scene + error log, per spec) instead of decoding to degenerate geometry - e.g. Draco-compressed
assets; recompress with `gltf-transform` / `gltfpack` to plain glTF. Optional `extensionsUsed`-only
extensions load with a warning and their features are ignored.
