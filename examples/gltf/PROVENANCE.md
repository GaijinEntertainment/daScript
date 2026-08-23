# dasGLTF sample assets - provenance

Every `.glb` / `.gltf` here comes from the Khronos **glTF-Sample-Assets** repository
(<https://github.com/KhronosGroup/glTF-Sample-Assets>), taken from each model's
`Models/<Name>/glTF-Binary/<Name>.glb`. Licenses are reproduced in
[`GLTF_SAMPLE_ASSETS.LICENSE`](GLTF_SAMPLE_ASSETS.LICENSE) and summarized below; that file
ships with the SDK because the Fox rig/animation is CC-BY 4.0 (attribution required).

## Shipped with the SDK - `examples/gltf/media/`

| File | Size | License | Exercises | Viewer |
|---|---|---|---|---|
| `BoomBox.glb` | 10.6 MB | CC0 1.0 | full metallic-roughness PBR (base-color / metal-rough / normal / occlusion / emissive, 2K maps) | `examples/opengl/10_hello_gltf.das` |
| `Fox.glb` | 162 KB | CC0 model + **CC-BY 4.0** rig/animation | 24-joint skeletal skin + 3 keyframe clips (Survey / Walk / Run), untextured | `examples/opengl/11_hello_gltf_animation.das` |

BoomBox is the static PBR showcase; Fox is the skinned-animation showcase. Both viewers
default to the shipped model and accept an override path as `argv[1]`.

## Parse/skin unit-test fixtures - `tests/dasGLTF/models/` (not installed)

Small deterministic fixtures, all **CC0 1.0**, kept in-repo for `tests/dasGLTF`:

| File | Exercises |
|---|---|
| `Box.glb` | minimal untextured mesh (baseline parse) |
| `BoxInterleaved.glb` | interleaved accessor `byteStride` |
| `SimpleSkin.gltf` | plain `.gltf` + base64 buffer, 2-joint skin + animation (skinning asserts) |

## Not shipped (deliberately)

- **BrainStem** - Poser EULA, redistribution restricted.
- **CesiumMan / DamagedHelmet** - CC-BY but redundant (Fox covers skinning, BoomBox covers PBR).

## Regenerating

Re-download the per-model `.glb` from the Khronos repo above (paths as noted). The dasGLTF
loader consumes `.glb` and `.gltf` directly - no conversion step.
