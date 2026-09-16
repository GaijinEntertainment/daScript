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

## Optional directional shadows

`gltf_pbr_set_point_lights(renderer, lights)` configures up to eight unshadowed
local lights (`GltfPbrPointLight`: position, linear color/radiance, radius).
They use the same PBR material response with a smooth finite-radius falloff.
An empty array clears them; existing callers default to no local lights.

`gltf_pbr_render_depth(renderer, model, scene, root, light_vp)` renders the same
skinned/rigid transforms into the caller's bound depth framebuffer. Alpha-masked
materials retain their cutouts; blended primitives are skipped. The depth program
is created lazily. The caller owns framebuffer, viewport, depth state, and clear.

`gltf_pbr_set_shadow(renderer, depth_tex, light_vp, resolution, bias, strength)`
enables bilinear PCF shadowing of direct light. Sixteen nearest depth comparisons
with subtexel weights reproduce a 3x3 kernel of bilinear PCF taps, keeping coverage
continuous across texel boundaries. Raw depth is never interpolated. Use a depth texture with compare
mode `GL_NONE`, nearest filtering, and clamp-to-edge wrapping. Texture unit 6 is
reserved for this sampler. Passing texture 0 disables shadows. The renderer owns
neither the texture nor framebuffer. Existing callers have shadows disabled.

Render the depth pass before the lit pass, using the same transforms in both.

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

The PBR adapter also offers `gltf_pbr_render_normals` for an opaque/masked depth-normal
prepass. It shares the regular skinning, material-normal and alpha-cutoff paths. The
caller owns the framebuffer and depth texture; encoded world normals are written as
`normal * 0.5 + 0.5`. Blended materials are excluded.

A caller-owned viewport-sized SSAO texture can be supplied through `GltfPbrRenderer.aoTex`
and `aoInverseSize`. Texture zero disables it. Visibility multiplies ambient/environment
lighting only, leaving direct lights and emission intact. The adapter does not own the AO
texture; the application owns its AO generation and depth-aware filtering.

Directional PCF compares each sampled texel against the corresponding depth on the
geometric receiver plane, reconstructed from shadow-coordinate derivatives. This
prevents sloped surfaces from shadowing themselves across the filter footprint as
the camera or fitted shadow coverage changes. The remaining small depth bias handles
rasterization precision; it is not used to hide the entire filter footprint.

`gltf_pbr_render_silhouette` draws opaque/masked models with a flat framebuffer color,
using the normal skinning and alpha-cutoff paths. The caller owns the depth comparison
and depth-write mask. For an occluded-character indicator, draw against environment depth
with `GL_GREATER` and depth writes disabled, then draw the normal visible character.
This ordering avoids tinting visible portions through the character's own overlapping parts.

`upload_gltf_geometry(scene, texture_owner)` uploads independent geometry buffers
while borrowing the owner's texture handles. The texture owner must outlive all
borrowers; deleting a borrower releases only its geometry and handle arrays.
It does not require decoded images on the supplied scene. This is useful for
spatial batches without replicating each asset's decoded images and GPU textures.

The GL renderer also accepts `atmosphere` and `backgroundTint` settings. Atmosphere
uses the shared `gltf/gltf_atmosphere` ray integral; zero densities disable it.
Background tint's RGB multiplies luminance, with its W controlling the blend.
Local shadow atlases are caller-owned: `localShadowTex`, `localShadowSize` (face
resolution, row count), and `pointShadow` entries (row+1, strength, near, far).
Each row contains +X, -X, +Y, -Y, +Z, -Z depth faces; zero strength disables an entry.

## Processed glTF assets

`gltf/gltf_processed` supplies a shared daslang processing pipeline backed by
`dasMeshOptimizer` and `stbimage/texture_blocks`. Native callers may process regular
glTF/GLB on load; packaged applications load prepared assets directly.

- `.das_gltf`: JSON manifest referencing a `.das_mesh` payload and shared `.das_tex` files.
- `.das_glb`: self-contained binary asset, including its compressed images.

These are versioned daslang runtime formats, not standard glTF files. Node names,
hierarchies, materials, skins, animation data and bounds are retained. Geometry
contains explicit attribute flags/strides and meshoptimizer-encoded buffers.
Positions stay float32. Normals/tangents use signed 10-bit components; UVs use
binary16; joints and weights use 16-bit components. Optional UV1/vertex colour
retention is part of `ProcessingProfile`. The default PBR profile uses 20 bytes for
textured rigid vertices and 36 for textured skinned vertices, adding tangents only
for normal-mapped materials. No simplification or LOD generation is performed.

Processing deduplicates vertices, improves cache/fetch order, and encodes buffers.
Translucent primitives preserve triangle order. `gltf/gltf_processed_gl` decodes
directly to the packed GPU layout and chooses uint16 indices when possible.
`unpack_processed_geometry` supports CPU-side mesh assembly; ordinary rendering
keeps vertices in the packed GPU layout.

```sh
bin/daslang -no-module-cache modules/dasGLTF/tools/convert.das -- \
  --root path/to/source-assets \
  --out path/to/processed-assets
```

Explicit input files can follow the options. `--self-contained` produces
`.das_glb`; `--force` rewrites assets; `--cache DIR` changes the texture cache.
The default cache is `.jitted_scripts/assets/textures`, relative to the working
directory. Repeated conversion checks asset keys and leaves unchanged outputs
alone. `asset-build.json` records the converted catalog and can be included in
application build fingerprints. Conversion never rewrites the source glTF files.

Texture cache identity includes decoded content, dimensions, colour space, format
and codec-layer version. BC1 handles opaque images; BC3 preserves image alpha.
Full mip chains are generated before compression. A `GltfGlTexturePool`, scoped
to one GL context, shares uploaded images across models with equivalent sampler
settings. Delete its models before deleting the pool. Unsupported block formats
or incompatible dimensions use an RGBA8 decode fallback; RGB565 is not used.

The binary serializer fixes accompanying this pipeline correct array element
allocation stride and release the writer's temporary callback buffer. Regression
coverage includes nested arrays with explicit deletion, both asset containers,
node/skin/animation preservation, bounds, transparent triangle order, and codecs.

### Optional screen-space material inputs

`gltf_pbr_render_normals` accepts `pack_roughness=true` to put perceptual roughness
in normal-buffer alpha, and `material_buffer=true` to draw linear albedo plus
metallic into RGBA instead. These paths share skinning and alpha-mask handling with
ordinary rendering. Existing calls retain opaque normal-buffer alpha.

`GltfPbrRenderer.materialScale` controls roughness/metallic multipliers (default 1).
`materialWetness` optionally interprets the metal/rough texture's unused red channel
as a wetness mask, blending roughness toward .16; its default zero preserves glTF.
Callers must enable this only for textures authored with that mask convention.
`reflectionAmount` adds reflection-only environment lighting, independent of the
existing image-based diffuse/ambient amount. It defaults to zero and uses the
caller-owned environment texture set through `gltf_pbr_set_environment`.
