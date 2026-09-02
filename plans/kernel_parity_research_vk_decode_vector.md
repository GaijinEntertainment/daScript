# Technique memo: the four-wide cm2 decode callback (SPV_NV_cooperative_matrix_decode_vector)

Read-only research, 2026-09-02. Trees: `D:\Work\daScript-iquant` at `e3ab267bc` (branch
`bbatkin/vulkan-sdk-357`), `D:\Work\llama.cpp` at `6c84c7d5d`, Vulkan SDK `1.4.357.0`.
Nothing in any tree was modified.

This memo closes delta 7 of `plans/kernel_parity_research_vk.md:356-368`, which predicted the
gap and prescribed the measurement. The measurement has been run and is recorded at
`plans/kernel_parity_pass.md:1047-1072`: on the RTX 5060 Ti with driver 616.56, llama.cpp's
vector-decode build beats its own scalar build by **5% to 13% pp512 on every quantized format**
(Q3_K_L 1.106, Q2_K 1.117, IQ4_XS 1.107, IQ3_M 1.131, Q4_K_M 1.049), tg128 flat, Q8_0 pp512
0.966. The reference our Vulkan pp512 rows are held to moved up by that much.

The delta-7 note said the blocker was llama.cpp's glslc. **That blocker does not exist for us.**
`modules/dasSpirv` is a self-hosted emitter with no glslc, glslang or shaderc dependency
(`modules/dasSpirv/spirv/` is eight hand-written `.das` files; the only mentions of glslang are
prose comments, e.g. `modules/dasSpirv/spirv/spirv_builder.das:193`). We emit the words
ourselves, so the extension is available to us the moment the grammar and three emitter arms
carry it.

---

## 1. The extension in facts

| Fact | Value | Source |
|---|---|---|
| SPIR-V extension string | `SPV_NV_cooperative_matrix_decode_vector` | `C:\VulkanSDK\1.4.357.0\Include\spirv\unified1\spirv.core.grammar.json:18311` |
| New capability | `CooperativeMatrixDecodeVectorNV` = **5447** | same file `:18307-18308` |
| Capability it requires | `CooperativeMatrixBlockLoadsNV` (5434) | same file `:18309` |
| New opcode | **none** | registry page, "No new opcode is added" |
| New operand | `DecodeVectorFunc` = **0x0004** in the `TensorAddressingOperands` BitEnum, one trailing `IdRef` | `spirv.core.grammar.json:19156-19163` |
| Instruction it extends | `OpCooperativeMatrixLoadTensorNV` (opcode 5367) | `spirv.core.grammar.json:7287-7300` |
| GLSL extension string | `GL_NV_cooperative_matrix_decode_vector` | `C:\VulkanSDK\1.4.357.0\Include\glslang\MachineIndependent\Versions.h:297` |
| Vulkan extension | `VK_NV_cooperative_matrix_decode_vector`, spec version 1, depends on `VK_NV_cooperative_matrix2` | `C:\VulkanSDK\1.4.357.0\Include\vulkan\vulkan_core.h:26494-26497`; `modules/dasVulkan/vendor/Vulkan-Headers/registry/vk.xml:32306` |
| Vulkan feature struct | `VkPhysicalDeviceCooperativeMatrixDecodeVectorFeaturesNV { sType; pNext; VkBool32 cooperativeMatrixDecodeVector; }`, sType `..._PHYSICAL_DEVICE_COOPERATIVE_MATRIX_DECODE_VECTOR_FEATURES_NV` = 1000689000 | `vulkan_core.h:26498-26502`, `:1490` |
| SPIR-V capability enablement | that feature bit, requiring that extension | `vk.xml:34929-34930` |

### The decode-vector function's signature

Spec wording (SPIR-V registry page and `GLSL_NV_cooperative_matrix_decode_vector.txt`):

- Return type: **a vector of the matrix Result Type's component type with V components, V in
  {2, 4, 8}.** ("The _Result Type_ of **DecodeVectorFunc**'s function must be a vector of the
  Component Type of Result Type with 2, 4, or 8 components.")
- Parameters, identical in shape to the scalar `DecodeFunc`: "first parameter a pointer in the
  **PhysicalStorageBuffer** storage class, second and third parameters arrays of 32-bit integers
  whose length equals the tensor layout dimension."
- GLSL spelling of the same thing: `vector<float16_t, V> f(const in <buffer_reference> b,
  const in uint32_t blockCoord[LDim], const in uint32_t coordInBlock[LDim])`.

### Constraints, all of them

1. **`blockSize[LDim-1]` must be a multiple of V.** The GLSL spec says the behavior of
   `coopMatLoadTensorNV` is *undefined* if this does not hold. `LDim-1` is the last (fastest
   varying) tensor-layout dimension.
2. **`coordInBlock[LDim-1]` values are V consecutive integers starting at a multiple of V.**
   The V elements share the same `blockCoord` and the same `coordInBlock` in every dimension
   except `LDim-1`.
3. **`DecodeFunc` must also be present in the same Tensor Addressing Operands.** The vector
   function is additive, never a replacement. "The implementation may invoke either DecodeFunc
   (per element) or DecodeVectorFunc (per group of V block-adjacent elements) at any call site,
   choosing whichever fits better." This is what makes both the driver fallback and the
   SPIR-V-level downgrade trivial.
4. Multiple invocations with the same parameters must return the same value (purity).
5. Operand word order on the instruction is the BitEnum's bit order: mask, then `TensorView`
   (0x1) id, then `DecodeFunc` (0x2) id, then `DecodeVectorFunc` (0x4) id. llama.cpp's stripper
   asserts exactly this at `D:\Work\llama.cpp\ggml\src\ggml-vulkan\ggml-vulkan.cpp:2784`.

`spirv-val` in this SDK is SPIRV-Tools v2026.3 with grammar rev 7, so it validates the new bit;
our vendored grammar is rev 4 and does not (sec. 4).

---

## 2. How llama.cpp uses it

### 2.1 The shape: one shader set, two decode functions, a load-time downgrade

llama.cpp does **not** stamp a second shader variant. Whenever the build host's glslc knows the
extension, `GGML_VULKAN_COOPMAT2_DECODE_VECTOR` is put into `base_dict` for **every** coopmat2
matmul variant unconditionally
(`D:\Work\llama.cpp\ggml\src\ggml-vulkan\vulkan-shaders\vulkan-shaders-gen.cpp:480-484`), and
the variant names are unchanged (`vulkan-shaders-gen.cpp:424-425` has no `_dv` suffix). The
blob always carries the extension; devices that lack it get it surgically removed at pipeline
creation.

The A-tile load, one source line serving both cases
(`ggml/src/ggml-vulkan/vulkan-shaders/mul_mm_cm2.comp:381`, and eleven sibling sites):

```glsl
coopMatLoadTensorNV(mat_a, data_a, pos_a, sliceTensorLayoutNV(tensorLayoutA, ir * BM, BM, block_k, BK) DECODEFUNCA);
```

with (`mul_mm_cm2.comp:80-89`):

```glsl
#if defined(dequantFuncA_v) && defined(GGML_VULKAN_COOPMAT2_DECODE_VECTOR)
#define DECODEFUNCA , dequantFuncA, dequantFuncA_v
#else
#define DECODEFUNCA , dequantFuncA
#endif
```

Both functions, scalar first. The `#extension` line is itself ifdef'd
(`mul_mm_cm2.comp:14-16`).

### 2.2 The `_v` twin: signature and the four-element mapping

Every twin has the identical signature, character for character apart from the buffer-reference
type (`ggml/src/ggml-vulkan/vulkan-shaders/dequant_funcs_cm2.glsl:576` for Q4_K):

```glsl
f16vec4 dequantFunc<FMT>_v(const in decodeBuf<FMT> bl, const in uint blockCoords[2], const in uint coordInBlock[2])
```

against the scalar `float16_t dequantFunc<FMT>(...)` (`dequant_funcs_cm2.glsl:528`). V = 4
everywhere; no format uses 2 or 8.

**The four elements are `coordInBlock[1] + 0,1,2,3` - contiguous along dimension 1, which for
the A matrix is k, the reduction dimension.** `.x` is `idx+0`, `.w` is `idx+3`. It is not four
rows and it is not strided. The A tensor layout sets `blockSize = (1, QUANT_K)`
(`mul_mm_cm2.comp:315-318`), unchanged by the flag, so `coordInBlock[1]` is the element index
inside the quant block: 0..31 for legacy, 0..255 for K-quants. Both are multiples of 4, so
constraint 1 holds for free. In-source confirmation that `idx % 4 == 0`:
`dequant_funcs_cm2.glsl:615-616`, `:327`, `:397`.

### 2.3 The three shapes a `_v` twin takes

| Family | What is shared across the four lanes | What the four-wide extract is |
|---|---|---|
| **SWAR-packed** (Q4_0, Q4_1, Q4_K, Q5_K, Q2_K, Q8_0, Q3_K, Q6_K, TQ2_0, Q5_0, Q5_1, Q1_0, Q2_0) | the whole scale/min/sub-block decode, bit-identical to the scalar | one 32-bit word (via a `..._packed32` buffer_reference alias where the block size is a multiple of 4, else two adjacent `uint16_t` OR'd into a register), one shift, one per-byte mask (`0x0F0F0F0F` / `0x03030303`), one `unpack8` |
| **LUT / grid** (all IQ*, MXFP4, NVFP4) | the scale, the sign byte, the codebook word - one lookup serves all four | the LUT entry *is* four bytes; `unpack8` it. The per-element lookup is replicated only where the grid index differs |
| **F32** | - | **the only format with no `_v` twin** (`dequant_funcs_cm2.glsl` has 24 twins; F32 at `:16` has none, and `QUANT_K == 1` means the whole `DECODEFUNCA` block is empty for it anyway) |

Q4_K, worked (`dequant_funcs_cm2.glsl:615-622`), the model for our own rewrite:

```glsl
    // idx in [0,256); vector decode uses idx a multiple of 4. packed32 word index:
    // (qs_i >> 1) == (idx >> 6) * 8 + ((idx & 0x1E) >> 2). sh is 0 or 4 only, so a
    // single (w >> sh) & 0x0F0F0F0F isolates all four nibbles without inter-byte leakage.
    const uint sh = (idx & 0x20u) >> 3u;
    const uint w = uint32_t(bl32.block.qs[(idx >> 6) * 8u + ((idx & 0x1Eu) >> 2)]);
    const u8vec4 q = unpack8((w >> sh) & 0x0F0F0F0Fu);

    return f16vec4(vec4(d) * vec4(q) - vec4(m));
```

against the scalar's per-element `qs = (qs >> (b * 4 + 8 * (idx & 1))) & 0xF`
(`dequant_funcs_cm2.glsl:568-569`). Four elements for about the cost of one.

Q6_K (`:790-804`) is the "block size is not a multiple of 4" case - `block_q6_K` is 210 bytes,
so there is no `packed32` view and the twin reads two adjacent `uint16_t`s and glues them:
`qlw = uint32_t(ql[i]) | (uint32_t(ql[i+1]) << 16)`, twice (ql and qh), then two per-byte masks.

IQ2_XS (`:1002-1025`) is the cleanest LUT case: one `qs` u16 load, one `dscale`, one sign
popcount and one `iq2xs_grid[..]` word serve all four lanes; the four signs are four adjacent
bits of the same sign byte.

### 2.4 The driver-side strip

`ggml_vk_strip_decode_vector` (`ggml-vulkan.cpp:2682-2800`) rewrites a compiled module for a
coopmat2-without-decode-vector driver. Three edits, nothing else:

1. drop `OpExtension "SPV_NV_cooperative_matrix_decode_vector"` (`:2730-2738`);
2. drop `OpCapability 5447` (`:2740-2745`);
3. on every opcode-5367 instruction whose TA mask has bit `0x4`: clear the bit, delete the
   trailing id, decrement the word count by one (`:2747-2793`).

It runs at pipeline creation, `ggml-vulkan.cpp:2968-2978`, guarded by
`device->coopmat2 && !device->coopmat2_decode_vector`. The now-unreferenced `_v` function body
stays in the module as dead code; nothing removes it.

### 2.5 What else the flag changes

| Site | Effect |
|---|---|
| `ggml-vulkan.cpp:873` | the device field: `bool coopmat2_decode_vector;` |
| `:6307-6309` | extension-string check, with env kill switch `GGML_VK_DISABLE_COOPMAT2_DECODE_VECTOR` |
| `:6619-6625` | feature struct chained into the `VkDeviceCreateInfo` pNext, extension pushed onto `device_extensions` |
| `:6868` | latch: `coopmat2_decode_vector_support && features.cooperativeMatrixDecodeVector`, inside the block that also sets `device->coopmat2` |
| `:2969` | false -> strip (sec. 2.4) |
| `:4280` | `mmqid_bk = coopmat2_decode_vector ? 64u : 32u` - the K tile of **all three** matmul_id warptiles doubles. Affordable because each callback now yields four elements. Plain matmul tiles are untouched (`:4256-4277`) |
| `:10196-10217` | MoE B-side alignment. `decodeFuncB_v` (`mul_mm_cm2.comp:135-142`) reads a `vec4` alias of binding 1 at `base >> 2`, so B's element offset must be 4-aligned; when it is not, B is staged into `prealloc_y` with a padded row stride (`ggml_vk_align_size(ne10, 4)`), and a coherency bit `prealloc_y_last_decode_vector_staging` invalidates the scratch-reuse cache across the layout switch |
| `mul_mm_cm2.comp:319-321` | MoE only: `setTensorLayoutBlockSizeNV(tensorLayoutB, 1, BK)` - B needs a block size at all, because with the default 1 the callback sees no within-block coordinate to round to 4 |
| `:7380` | the device banner reads `NV_coopmat2v` instead of `NV_coopmat2` |

The flash-attention shader gets it too, self-detecting on the glslc-predefined macro rather than
the ggml define (`vulkan-shaders/flash_attn_cm2.comp:18-20`, `:68-100`).

The feature probe glslc is handed is four lines
(`vulkan-shaders/feature-tests/coopmat2_decode_vector.comp`):

```glsl
#version 460
#extension GL_NV_cooperative_matrix_decode_vector : require
void main() { }
```

---

## 3. Ours today

### 3.1 The builtin and its lowering

Surface (`modules/dasSpirv/spirv/spirv_builtins.das:540-541`):

```das
[unused_argument(dst, src, base, tl, r0, nr, c0, nc, decode), sideeffects]
def public coopmatLoadTensorDecode(var dst; src; base : int | uint; var tl; r0, nr, c0, nc : uint; decode) : void {}
```

`decode` is the ninth argument and is either `@@fn` (an `ExprAddr`) or `self.<method>` (an
`ExprField`, devirtualized by the discovery pass `SpirvMethodScan.preVisitExprCall`,
`modules/dasSpirv/spirv/spirv_emit.das:7178-7207`). Resolution and rejection:
`spirv_emit.das:4963-4991`. The lowering, `spirv_emit.das:5017-5033`:

```
OpCooperativeMatrixLoadTensorNV <dst.pty> <res> <sptr> <cur_obj> <sliced> <MemoryOperand=None> <ta_mask> <ta_extra>
```

with `ta_mask = SpvTensorAddressingOperands.DecodeFunc` (0x0002) and `ta_extra = fid`
(`spirv_emit.das:4990-4991`). There is exactly **one** `ta_extra` slot, and the decode arm and
the tensor-view arm are `if` / `elif` (`:4963` / `:4992`) - so today a decode load and a
transpose view are mutually exclusive, and there is no room for a second callback id.

`Object` is an `OpLoad` of the tile local (`:5024`), `Pointer` an `OpAccessChain` into the SSBO
in `StorageBuffer` class (`coop_buf_ptr`, `spirv_emit.das:3455-3479`), `TensorLayout` the result
of `OpTensorLayoutSliceNV` (`cm2_sliced_layout`, `:3480-3504`).

### 3.2 The `[spirv_decode]` contract as emitted

The annotation itself validates nothing - it only pins `neverInline`
(`modules/dasSpirv/spirv/spirv_shader.das:276-289`). Everything is enforced at emission by
`register_decode_func` (`spirv_emit.das:6908-6973`):

| Rule | Line | Text |
|---|---|---|
| return type | `:6915-6925` | `float16`, `float` or `int8` only; else `"decode function '<n>': result must be float16, float, or int8 (the tile's component type)"` |
| arity | `:6928-6939` | 3 params, or 4 iff `arguments[0].name == "self"` inside a kernel class; a 4-param non-method errors `"a 4-parameter decode must be a class method (leading self)"` (fail-closed fixture `tests/spirv/_fail_closed/_fc_decode4.das`, asserted `tests/spirv/test_fail_closed.das:81`) |
| param 1 | `:6942-6953` | must be a structure; laid out by `build_block_struct(..., as_block = true, BlockLayoutRules.std430)`, then `type_pointer(m, PhysicalStorageBuffer, blk.type_id)` |
| params 2-3 | `:6955-6961` | both `uint2`, else `"parameter {n} must be uint2 (the tensor coordinates)"` |
| `OpTypeFunction` | `:6963-6969` | `(PSB-ptr-to-block, uint[2], uint[2]) -> ret_type` - the coordinates are SPIR-V **arrays**, not vectors |

The emitted body (`emit_decode_function`, `spirv_emit.das:6980-7028`): param 0 is the PSB
pointer decorated `Aliased` (`:6992`) and registered as an opaque `LocalVar` so member reads
become PSB access chains with `Aligned` loads (`:6993-6994`); params 1-2 are `uint[2]` arrays
immediately rebuilt into `uvec2` SSA values by `OpCompositeExtract` x2 + `OpCompositeConstruct`
(`:7009-7017`), because the das AST believes they are `uint2`. Bodies emit after the entry point
in first-reference order (`spirv_emit.das:7657-7664`); ids pre-allocate at registration so the
load forward-references.

The canonical shapes, one plain and one method:

```das
[spirv_decode, unused_argument(bc)]                                   // tests/spirv/_spirv_common.das:2250-2251
def cm2_decode_q8(blk : Cm2BlockQ8; bc, cib : uint2) : float16

[spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN.md#cm2-decode-16bit-lanes")]
def decode(blk : VkK4Blk; bc, cib : uint2) : float16                  // dasllama_vulkan_classes.das:5027-5028
```

Fourteen `*Cm2T` format templates carry the method form
(`modules/dasLLAMA/dasllama/dasllama_vulkan_classes.das:5010, 5023, 5043, 5067, 5091, 5110,
5130, 5152, 5172, 5198, 5231, 5251, 5277, 5303`), all one signature.

### 3.3 The capability set and the tile call site

`ensure_cooperative_matrix2` (`modules/dasSpirv/spirv/spirv_builder.das:215-229`) emits
`PhysicalStorageBufferAddresses`, `CooperativeMatrixTensorAddressingNV`,
`CooperativeMatrixBlockLoadsNV`, `TensorAddressingNV` plus `OpExtension` for
`SPV_NV_cooperative_matrix2` and `SPV_NV_tensor_addressing`, over the KHR coopmat base
(`:196-207`). Three opt-in siblings ride on top with the same pool-key idiom:
`ensure_coopmat_reductions` (`:235-241`), `ensure_coopmat_per_element` (`:242-248`),
`ensure_coopmat_conversions` (`:249-255`). Addressing model flips to
`PhysicalStorageBuffer64` off the `cap_cm2` pool key (`spirv_emit.das:7701`).

The dasLLAMA call site (`dasllama_vulkan_classes.das:4911, 4917`):

```das
coopmatLoadTensorDecode(a, wq, wblk0, fla, m0, 128u, k, 64u, self.decode)
```

with `tensorLayoutSetBlockSize(fla, 1u, BLKW)` (`:4896`). **`BLKW` is 256 by default
(`:4810`) and only Q8 overrides it to 32 (`:5013`)** - both multiples of 4, so spec constraint 1
already holds on every one of our formats, with no layout change.

### 3.4 The device-feature enable pattern

`modules/dasVulkan/daslib/vulkan_boost.das`:

- `cooperative_matrix2_supported(phys)` (`:701-718`): `device_extension_available` for both
  extension strings, then one `vkGetPhysicalDeviceFeatures2` with the structs chained, then the
  four `fcm2.*` bits.
- `cooperative_matrix2_fa_supported(phys)` (`:724-737`): the opportunistic trio, queried the
  same way and gated separately.
- `create_device_storage_8_16_int_dot_coopmat2(phys, ...)` (`:788-897`): pre-queries into `q*`
  structs, sets the required bits hard (`fcm2.cooperativeMatrixWorkgroupScope = 1u` etc.,
  `:825-828`), copies the opportunistic bits from the query
  (`fcm2.cooperativeMatrixReductions = qcm2.cooperativeMatrixReductions`, `:832-834`), chains
  by hand with `addr<void?>` and pushes extension strings onto `ext` (`:876-885`).

dasLLAMA latches the result into `g_gpu` (`modules/dasLLAMA/dasllama/dasllama_vulkan_common.das:471-474`):
`has_coopmat2` set at `:1017`, `has_coopmat2_fa` at `:1018`, and `coopmat_mode` (0..4, where 4
is "cm2 decode-in-load", the default where the device has it) resolved by
`resolve_coopmat_mode` (`:835`). Kernels gate on `g_gpu.coopmat_mode == 4 && g_gpu.has_coopmat2`.

**The dasVulkan bindings already carry the new struct** - the sdk-357 roll (commit `5c86e6604`)
regenerated them: `PhysicalDeviceCooperativeMatrixDecodeVectorFeaturesNV` at
`modules/dasVulkan/daslib/vulkan_structs.das:21232-21245`, the sType-prefilled ctor at
`modules/dasVulkan/daslib/vulkan_ctors.das:6654-6659`. No binding regen is owed.

---

## 4. The delta

### 4.1 Emitter

| # | Change | Where |
|---|---|---|
| E1 | Re-vendor `spirv.core.grammar.json` from rev 4 to rev 7 (the SDK copy at `C:\VulkanSDK\1.4.357.0\Include\spirv\unified1\spirv.core.grammar.json`, 630254 bytes) and re-run the generator. This adds `CooperativeMatrixDecodeVectorNV = 5447u` next to `TensorAddressingNV = 5439u` and `DecodeVectorFunc = 0x0004u` to `SpvTensorAddressingOperands`. Both are absent today. | `modules/dasSpirv/spirv_headers/spirv.core.grammar.json`; targets `modules/dasSpirv/spirv/spirv_grammar.das:2776` and `:2924-2928` |
| E1a | **Regeneration hazard, fix first.** The generator writes only `SPV_VERSION_1_0/1_3/1_4/1_6` (`modules/dasSpirv/generator/gen_spirv_grammar.das:114-118`), but the checked-in table carries `SPV_VERSION_1_5` at `spirv_grammar.das:12`, which `spirv_emit.das:2565, 2566, 5508, 5509` reads. A naive regen deletes it and breaks the build. | `gen_spirv_grammar.das:114-118` |
| E2 | `ensure_coopmat_decode_vector(var m)`: `ensure_cooperative_matrix2(m)`, then `OpCapability CooperativeMatrixDecodeVectorNV` + `OpExtension "SPV_NV_cooperative_matrix_decode_vector"`, pool key `cap_cm2_decvec`. Byte-for-byte the `ensure_coopmat_reductions` shape. | `modules/dasSpirv/spirv/spirv_builder.das`, after `:255` |
| E3 | Widen the return-type whitelist in `register_decode_func` to admit `Type.tHalf4` (and, for symmetry, `tHalf2`/`tHalf8` if V=2/8 is ever wanted). `emit_type` already yields `OpTypeVector(OpTypeFloat 16, 4)` for it and `ensure_width_cap` already emits `OpCapability Float16`. Nothing else in the decode path is scalar-shaped: `df.ret_type` is read only at `:6967` and `:6989`. | `spirv_emit.das:6915-6925` |
| E4 | A second `ta_extra` slot. Today `ops` carries one (`spirv_emit.das:5026-5029`); it needs to push ids in bit order, and `ta_mask` needs `\| DecodeVectorFunc`. Also worth fixing while there: the decode / tensor-view arms are `if`/`elif` (`:4963`/`:4992`), so `TensorView + DecodeFunc` is inexpressible - the same widening removes that limitation. | `spirv_emit.das:4963-4991, 5017-5033` |
| E5 | The surface. **Recommended: a tenth argument, not a new builtin** - `coopmatLoadTensorDecode(dst, src, base, tl, r0, nr, c0, nc, decode, decodeV)`, mirroring llama.cpp's `, dequantFuncA, dequantFuncA_v`. The spec *requires* both functions (sec. 1, constraint 3), so an annotation like `[spirv_decode(vec = 4)]` on a single function would be wrong: it would have to synthesize the scalar twin, and the two bodies are genuinely different code. `[spirv_decode]` stays the marker on both; the vector one is recognized by its `half4` return, and `register_decode_func` should reject a `half4` function passed in the scalar slot and a scalar one passed in the vector slot. | `modules/dasSpirv/spirv/spirv_builtins.das:540-541`; the arity checks at `spirv_emit.das:4992`, `:7178-7207` |
| E6 | Duties, from the checklists. `modules/dasSpirv/REVIEW.md:7-12`: the capability needs a fixture in `tests/spirv/_spirv_common.das` plus a `_gen_golden.das` row and `_golden/*.txt`, or a word-level assertion. `:14-17`: any new rejection path (E5's two type errors) needs a `_fail_closed/` fixture and its error text asserted. `:19-21`: a downstream device test in `modules/dasLLAMA/tests/test_vulkan_kernels.das`. `modules/REVIEW_SHADER_EMITTERS.md:12-13`: the new builtin ships a CPU body returning what the emitted form returns, argument for argument - here that is a `half4` CPU body, which is also the four-wide decode's correctness oracle. `modules/dasMetal/ARCHITECTURE.md:167-171` already carries "cm2 decode-in-load is Vulkan-only by hardware, deliberate"; a widening rides that entry, no new ledger row. | |
| E7 | **`tests/spirv/test_census.das` will not catch this.** The census asserts opcode sets both directions (`modules/dasSpirv/ARCHITECTURE.md:159-165`), and decode-vector adds **no opcode** - only an operand bit. The fixture must assert the TA mask word and the extra operand explicitly. | |
| E8 | Doc. `modules/dasSpirv/ARCHITECTURE.md:88-90` carries the `[spirv_decode]` method-form paragraph inside `{#files-and-emission}` with no anchor of its own. A four-wide form wants its own anchored subsection for the `[arch]` citations to point at. | |

**`half4` is the right daslang spelling and it already lowers.** There is no `float16x4` or
`f16vec4` token: `src/parser/ds2_parser.ypp:273-277` defines `float16`, `half2`, `half3`,
`half4`, `half8`. `spirv_emit.das:2345` maps `half4` to (lanes 4, float class, width 16) and
`modules/dasSpirv/spirv/spirv_types.das:36-40` turns that into `OpTypeVector(OpTypeFloat 16, 4)`.
A user function already returns `half4` by value in `tests/spirv/test_float16.das:44-56`, and
`tests/spirv/test_conf_half4.das:17-23` is the standing gate.

### 4.2 Device layer

| # | Change | Model |
|---|---|---|
| D1 | `cooperative_matrix2_decode_vector_supported(phys) : bool` - `device_extension_available(phys, "VK_NV_cooperative_matrix_decode_vector")`, then one `vkGetPhysicalDeviceFeatures2` with `PhysicalDeviceCooperativeMatrixDecodeVectorFeaturesNV` chained, then `!= 0u`. Should require `cooperative_matrix2_supported(phys)` first, the way the fa gate does. | `vulkan_boost.das:724-737` |
| D2 | In `create_device_storage_8_16_int_dot_coopmat2`: pre-query the bit into a `qdv` struct, enable it **opportunistically** (`fdv.cooperativeMatrixDecodeVector = qdv.cooperativeMatrixDecodeVector`), chain `fdv` after `fcm2`, and push the extension string onto `ext` only when reported. The extension is not universally present on coopmat2 devices, so a hard enable would break device creation on Turing/Ampere. | `vulkan_boost.das:832-834` (the trio), `:876-885` (the ext list) |
| D3 | `g_gpu.has_coopmat2_decvec : bool` beside `has_coopmat2_fa`, set at device init from D1. | `dasllama_vulkan_common.das:471-474`, `:1017-1018` |
| D4 | The kernel gate. Because the spec requires both callbacks and lets the driver pick, a device without the feature is served by the **scalar** function in the same module - but only if the module does not declare the capability, which it must to be loadable. Two options: (a) stamp two class variants and pick on `has_coopmat2_decvec` (our normal shape - `vkd_class_pipe` is the single A/B seat, `modules/dasLLAMA/ARCHITECTURE_GPU_VULKAN.md:132-140`); (b) port llama.cpp's strip - one blob, three word-level edits at pipeline creation. (a) is cheaper to land and costs pipeline count; (b) is cheaper at runtime and is ~120 lines of SPIR-V surgery. Our stamping machinery already carries per-format x per-tile variants, so (a) is the smaller diff. | |
| D5 | **The recorded identity.** `resolve_coopmat_mode` feeds both the live device pick and the `.dlim` flavor tag (`dasllama_math_vulkan.das:299-308` -> `DlimVulkanConfig.coopmat_mode`, `dasllama_config.das:51`, stamped by `dlim_vulkan_tag`, `:161-166`), and `ARCHITECTURE_GPU_VULKAN.md:111-115` states the guarantee as "the recorded mode and the running mode cannot drift". A capability that changes which kernel serves must therefore reach the tag too - either as a new mode rung or as a field beside `coopmat_mode`. A flag that changes the served kernel but not the tag breaks that invariant silently: two boxes would share one `.dlim` identity and run different kernels. | |
| D6 | The full plumbing path a new capability walks, from agent survey: `vulkan_boost.das` `<cap>_supported(phys)` -> `GpuState.has_<cap>` set in `vk_moe_init` (`dasllama_vulkan_common.das:1016-1019`) -> the device-creator branch (`:1027-1039`) -> a resolver or predicate (`resolve_coopmat_mode` `:834-858` / `pf_f16_feed` `dasllama_vulkan_prefill.das:41-44`) -> the `.dlim` tag (D5) -> the `cm2_cls_ensure/set/enc` arms (`dasllama_vulkan_prefill.das:58-102, 105-189, 192-`) -> the test guards (18 sites spelling `g_gpu.coopmat_mode == 4 && g_gpu.has_coopmat2` in `test_vulkan_kernels.das`). The dry/offline-bake mirror at `dasllama_vulkan_common.das:988-997` needs the same field. | |
| D7 | `modules/dasLLAMA/HOW_TO_ADD_A_FORMAT.md:193-204` ("6b. The cm2 prefill tile - a decode method on the template") describes adding a format as authoring one `[spirv_decode] def decode`. A four-wide form makes that two methods; the recipe updates in the same change. | |

### 4.3 A format decode method: Q4_K worked

Today (`dasllama_vulkan_classes.das:5027-5040`):

```das
[spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN.md#cm2-decode-16bit-lanes")]
def decode(blk : VkK4Blk; bc, cib : uint2) : float16 {
    let g = cib.y >> 5u
    let e = cib.y & 31u
    let bidx = g * 16u + (e & 15u)
    let by = uint(int(unpack8(blk.qs[int(bidx >> 1u)])[int(bidx & 1u)])) & 0xFFu
    let q = (by >> ((e >> 4u) * 4u)) & 0xFu
    let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
    let dm = unpackHalf2x16(ws[srow])
    let sc = (ws[srow + 1u + (g >> 2u)] >> ((g & 3u) * 8u)) & 0xFFu
    let mn = (ws[srow + 3u + (g >> 2u)] >> ((g & 3u) * 8u)) & 0xFFu
    return float16(dm.x * float(sc) * float(q) - dm.y * float(mn))
}
```

`VkK4Blk.qs : int16[64]` = 128 nibble bytes as 16-bit lanes (`:4748-4750`). Which of these are
shared across `cib.y = i, i+1, i+2, i+3` with `i % 4 == 0`:

| Term | Shared? | Why |
|---|---|---|
| `g = cib.y >> 5` | yes | `i % 4 == 0` and `32 % 4 == 0`, so all four stay in one 32-run |
| `e >> 4` (the nibble half) | yes | `i % 4 == 0` and `16 % 4 == 0` |
| `srow`, `dm`, `sc`, `mn` | yes | they depend only on `g`, `bc.x`, `bc.y` |
| `bidx = g*16 + (e & 15)` | no - **four consecutive, 4-aligned** | `e & 15` in {0,4,8,12}, so `bidx..bidx+3` never crosses the 16-byte group |

So `bidx >> 1` and `bidx >> 1 + 1` are the two `int16` lanes holding all four bytes, and the
whole scale path is computed once. The four-wide body:

```das
[spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN.md#cm2-decode-16bit-lanes")]
def decode_v(blk : VkK4Blk; bc, cib : uint2) : half4 {
    let g = cib.y >> 5u
    let e = cib.y & 31u
    let bidx = g * 16u + (e & 15u)
    let w0 = int(bidx >> 1u)
    // the four bytes bidx..bidx+3 are 4-aligned, so they are exactly two 16-bit lanes
    let qw = (uint(blk.qs[w0]) & 0xFFFFu) | ((uint(blk.qs[w0 + 1]) & 0xFFFFu) << 16u)
    // (e >> 4) is shared, so one shift of 0 or 4 isolates all four nibbles with no byte leakage
    let q4 = int4(unpack8(int((qw >> ((e >> 4u) * 4u)) & 0x0F0F0F0Fu)))
    let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
    let dm = unpackHalf2x16(ws[srow])
    let sc = (ws[srow + 1u + (g >> 2u)] >> ((g & 3u) * 8u)) & 0xFFu
    let mn = (ws[srow + 3u + (g >> 2u)] >> ((g & 3u) * 8u)) & 0xFFu
    let d = dm.x * float(sc)
    let m = dm.y * float(mn)
    return half4(float4(q4) * d - m)
}
```

Lane j reads byte j of `qw`, nibble half `(e >> 4) * 4`. `int4(unpack8(...))` plus `.x/.y/.z/.w`
is the shape `tests/spirv/_spirv_common.das:2048-2051` already proves emits. Ten scalar
invocations of the scale path collapse to one per four elements, and the four nibble extracts
collapse to one shift plus one mask.

**The other thirteen formats.** All default to `BLKW = 256u`, so all satisfy the alignment
constraint. Two families:

- **Nibble/bit SWAR** (K4, K5, K6, K3, K2, Q40, Iq4nl, Iq4xs): the same rewrite - two `int16`
  lanes glued, one shift, one `0x0F0F0F0F` or `0x03030303` mask. K6 additionally needs the qh
  plane glued the same way, exactly as `dequant_funcs_cm2.glsl:790-804` does.
- **Grid/LUT** (Iq3s, Iq3xxs, Iq2s, Iq2xs, Iq2xxs): the largest win, because the grid word
  *already is* four bytes. In `Iq3sCm2T.decode`
  (`dasllama_vulkan_classes.das:5178-5194`), `wj = r >> 2` and therefore `gw =
  iq3s_gridc[qb | ...]` are **shared across the four lanes**, and `gb = (gw >> ((r & 3) * 8)) &
  0xFF` picks byte `r & 3` - so the four lanes are precisely `unpack8(gw)`. The sign bits
  `(sgb >> (((r >> 2) & 1) * 4 + (r & 3))) & 1` are four adjacent bits of one byte. One grid
  lookup, one sign byte, one scale, four outputs.

---

## 5. Risks and the measurement that proves each

| # | Risk | The measurement |
|---|---|---|
| R1 | **The four-wide body drops off the driver's block-load path.** `modules/dasLLAMA/ARCHITECTURE_GPU_VULKAN.md:72-83` records that the vendor compiler pattern-matches only the 16-bit-load + `unpack8(w)[i & 1u]` spelling into the block-load path, that a 32-bit word with a variable shift is slower, and that a **runtime-indexed** byte4 select costs about two thirds of the rate. The four-wide body deliberately uses a 32-bit word and a variable shift. The distinguishing claim - that a *static* `.x/.y/.z/.w` swizzle is fine where a dynamic index is not - is exactly what llama.cpp's twins rely on, but it is untested on our emission. **This is the top risk; it is also cheap to settle - see O1.** | `modules/dasLLAMA/harness/vk_gemm_probe.das`. The decode-spelling bisect is the right axis, not the whole-tile one: `k6x` holds the tile fixed in `K6PxBase` (`:473-538`) and varies only the decode across five leaves (`:540, 548, 568, 587, 611`); `cm2x` is the Q8 twin (`:72-84`). `k4` / `k6` (`run_k4_shape:358-464`, `run_k6_shape:638`) are the whole-kernel A/B, printing `ms/dispatch` + `TFLOP/s` per arm (`:456`, `:758`) from `usec_per_dispatch` (`:769-777`: 2 warm + 100 timed submits x 16 dispatches). Every row is `timing-only` and enters no record store (`ARCHITECTURE_MEASUREMENT.md:49-55`), so a decision it seeds is confirmed afterwards by the e2e board rows. |
| R2 | **Numerical drift.** The four-wide body recomputes the same arithmetic in a different association (`float4(q4) * d - m` vs per-element `d * float(q) - m`), and the driver may call either callback for the same element. | `modules/dasLLAMA/tests/test_vulkan_kernels.das:1584` `test_vkd_k4_cm2_batch` is the gate. It is **not** bit-exact: it compares against the CPU reference `k4f16_gemm_oracle` (`modules/dasLLAMA/tests/_vkd_oracles.das:97-125`, which walks the device plane layout and reproduces the decode's rounding exactly at `:116`, but accumulates in f32 where the GPU accumulates in f16) through `mismatch_bars(y_cls, y_ref, 2e-2, 4e-3 * max_abs(y_ref))` (`:1692`) and requires **zero** elements off the bars (`:1694`). `mismatch_bars` (`:237-250`) passes an element on 2% relative **or** 0.4% of the result plane's peak magnitude, whichever is looser; a poisoned element proves the bars can red (`:1695-1698`). The fixture crosses superblock bounds (`n = 768`, `d = 160`, two regions of 300 and 260 rows, so both fast and edge paths run) and sweeps all three tiles. **All fourteen formats have a sibling** (`:1479, 1584, 1713, 1842, 1970, 2097, 2225, 2353, 2480, 2608, 2736, 2864, 2992, 3120`, plus m/s/split at `:3247, 3351, 3459`). Because the driver may pick either callback per call site, a *passing* run does not prove the vector path executed - see O2/O3. |
| R3 | **Fallback on a device without the feature.** The module declares capability 5447; a driver lacking it fails `vkCreateShaderModule` / pipeline creation. Two-variant stamping (D4a) never presents such a module; the strip (D4b) removes three things. | `test_vkd_k4_cm2_batch:1591-1594` already skips unless `coopmat_mode == 4 && has_coopmat2`; a new `has_coopmat2_decvec` skip arm keeps the suite green on a coopmat2-without-decvec device. The negative case needs a device that has cm2 but not decode-vector (a Turing/Ampere box, or lavapipe, which has neither). |
| R4 | **`spirv-val` rejects our words.** The module-level gate validates every blob (`modules/dasSpirv/ARCHITECTURE.md:143-146`, hard-required in CI). The SDK's `spirv-val` is SPIRV-Tools v2026.3 (grammar rev 7) and knows the bit; an older `VULKAN_SDK` on a CI lane would not. | The standing suite command, `modules/dasSpirv/ARCHITECTURE.md:174-178`. A validator too old soft-skips locally and would fail closed in CI - worth confirming which SDK the Linux lane resolves. |
| R5 | **The MoE B-side.** llama.cpp needed `setTensorLayoutBlockSizeNV(tensorLayoutB, 1, BK)` plus a 4-aligned or staged B for its `decodeFuncB_v`. We do not decode B at all (`coopmatLoadTensor(b, xf16, 0u, flb, ...)`, `dasllama_vulkan_classes.das:4912`), so none of that applies - but our activation plane's row stride `pa.n & ~7u` (`:4901`) should be re-checked if a B-side decode is ever added. | Not needed for the A-only change. |
| R6 | **Emission order / dead code.** With E5, two decode functions are registered per load; `decode_order` (`spirv_emit.das:142`) keeps first-reference order and both bodies emit. A variant that ends up not using the vector one leaves it as dead code, which is valid but grows the module. | The golden snapshots (`tests/spirv/_gen_golden.das` + `_golden/*.txt`) make any unexpected extra function visible in review. |
| R7 | **A standing REVIEW ban sits next to this change.** `modules/dasLLAMA/REVIEW_GPU.md:243-247`: "**Never read a `[spirv_decode]` callback's quant bytes by indexing `unpack8` of a 32-bit word with a runtime value - read them as 16-bit lanes instead.**" The four-wide body satisfies the ban's letter - its `unpack8` components are selected by **static** swizzle, never a runtime index - but it does read a 32-bit word with a variable shift, which the companion architecture note calls slower (`ARCHITECTURE_GPU_VULKAN.md:76-77`). The ban and the note were written when every decode produced one element; a four-wide decode is the case they did not contemplate. | O1 settles it. Whichever way it lands, `REVIEW_GPU.md:243-247` and `ARCHITECTURE_GPU_VULKAN.md:72-83` are edited in the same change - either narrowed to "runtime index" explicitly with the four-wide static-swizzle form carved out, or upheld with the four-wide form ruled out on measured grounds. Landing the kernel change without touching them leaves a rule that reads as forbidding the new code. |

---

## 6. Open questions, each with the experiment that settles it

**O1. Does a static `.x/.y/.z/.w` swizzle on `unpack8`'s byte4 keep the kernel on the driver's
block-load path, where a runtime index does not?** The architecture note
(`ARCHITECTURE_GPU_VULKAN.md:76-79`) and the REVIEW ban (R7) rule out the runtime-indexed form
but say nothing about the static one, and every existing decode uses the runtime form because it
decodes one element. **This is the question that decides the whole arc, and it needs no emitter
work at all.**
*Experiment:* the seat already exists. `harness/vk_gemm_probe.das` `k6x` runs the decode-spelling
bisect: `K6PxBase` (`:473-538`) holds the tile fixed and five leaf classes override only the
decode - `K6PxNil` (`:540-546`, the constant ceiling), `K6PxFlat` (`:548-566`), `K6PxQl`
(`:568-585`), `K6PxPair` (`:587-609`, the 32-bit compose with `unpack8(int(...))`) and
`K6PxPair16` (`:611-633`, the same compose through `unpack8(int16(...))`). Add a sixth leaf whose
decode composes the 32-bit word and selects the byte by **static** swizzle, and run
`vk_gemm_probe k6x`. `ARCHITECTURE_MEASUREMENT.md:35-41` names this exact axis - "one decode
callback against spellings of itself with the rest of the tile held fixed (the `cm2x` and `k6x`
bisect arms) ... A new arm joins one of the three" - so this is the contractual home for the
question, not a new rig. Run it before E1-E5.

**O2. Does the NVIDIA driver actually call the vector function, and how often?** The spec lets
the implementation pick per call site. A rate win is the only externally visible proof, and it
could be swamped by R1.
*Experiment:* make the vector body return a deliberately wrong value (say, all zeros) while the
scalar body stays correct, and run `test_vkd_k4_cm2_batch`. If the test still passes, the vector
function was never invoked. If it fails, the run tells you which elements went through it. Revert
before landing; this is a probe, not a fixture.

**O3. Once the vector path is live, how is "the vector path ran" asserted in CI?** A passing
tolerance test does not distinguish the two callbacks (R2).
*Experiment:* the `DASLLAMA_VK_SPV_DUMP=<dir>` seat (`ARCHITECTURE_GPU_VULKAN.md:136-141`) writes
the emitted words per kernel. A word-level assertion in `tests/spirv/` on the TA mask (`0x6`,
both bits) plus two trailing ids is the emission-side proof; there is no runtime-side proof short
of O2's poisoning trick.

**O4. Should V be 4, or is 8 better on Blackwell?** The spec allows 2, 4 and 8; llama.cpp uses
only 4 in all 24 twins, with no comment explaining the choice. A `half8` decode over 8 consecutive
nibbles is one 64-bit read, which our int16-lane block structs would express as four lanes glued.
*Experiment:* after V=4 lands and R1 is settled, stamp a V=8 K4 twin (daslang has the `half8`
token, `ds2_parser.ypp:277`) and A/B it in `vk_gemm_probe k4`. This is cheap once E3 admits vector
returns, since the whitelist widening is per-lane-count.

**O5. Two variants (D4a) or the SPIR-V strip (D4b)?** llama.cpp chose the strip because it would
otherwise double thousands of permutations at build time. Our stamping is per (format, tile) and
already exists; the added axis multiplies it by two.
*Experiment:* count the resulting pipeline set. `kq_batch_cls_ensure` stamps per format per tile
(14 formats x 3 tiles); doubling to 84 is a compile-time and pipeline-cache cost that
`vkd_class_pipe` timing (the A/B seat, `ARCHITECTURE_GPU_VULKAN.md:132-135`) measures directly.
If startup cost is material, port the strip - it is three word-level edits and llama.cpp's version
(`ggml-vulkan.cpp:2682-2800`) is a complete reference including the MemoryAccess operand-skipping
arithmetic.

**O0 (not a question, a defect found in passing).** `ARCHITECTURE_GPU_VULKAN.md:79` says the cm2
decode callbacks cover "q8 and the six kq superblock formats". There are **thirteen** kq formats
today (fourteen decode methods, `dasllama_vulkan_classes.das:5010-5333`). The count is stale and
should be fixed whether or not this arc proceeds; this memo is read-only and did not touch it.
Two smaller items from the same survey: `Iq2xsCm2T` (`:5277`) and `Iq2xxsCm2T` (`:5303`) omit the
`[ |> template_struct_instance]` line their twelve siblings carry (they work through the parent's
propagating form, but a fifteenth format copy-pasted from either would be a coin flip), and
`spirv_emit.das:5155`, `:5205` and `callback_param_type` (`:7031-7039`) make the same
scalar-only assumption `register_decode_func` does - so a uniform "callbacks may return vectors"
generalization touches three registration functions, not one.

**O6. Does the Q8_0 pp512 regression (0.966, `plans/kernel_parity_pass.md:1062`) reproduce on our
Q8 tile?** Q8_0 *does* have a `_v` twin (`dequant_funcs_cm2.glsl:239`), so the loss is not "no
twin"; the ledger calls it a codegen side effect of the rebuilt shader set. Our Q8 tile has
`BLKW = 32u` rather than 256, which is the one structural difference from every other format.
*Experiment:* stamp the four-wide Q8 decode and run `vk_gemm_probe cm2x` (the Q8 decode-cost
bisect, `vk_gemm_probe.das:72-84`), whose nil/flat/lit rungs separate decode cost from tile cost.
If Q8 regresses while K4 gains, keep Q8 on the scalar path - the per-format pick is already a
free parameter, since each `*Cm2T` template is stamped separately.
