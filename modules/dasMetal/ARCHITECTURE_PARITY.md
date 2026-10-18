# Cross-backend kernel-model asymmetry ledger

Companion to [ARCHITECTURE.md](ARCHITECTURE.md#kernel-model-asymmetry-ledger).

This ledger holds every kernel-model capability one emitter has and the other lacks, for both
backends; `modules/REVIEW_SHADER_EMITTERS.md` routes a diff here. The primary correctness oracle
is the CPU-reference run of the same das body - cheaper and stricter than a second GPU.
Cross-GPU parity of one source is secondary. Current entries:

- **`textureGrad` sampling is SPIR-V/GLSL-only; native MSL support is pending.** The shared
  sampler2D/3D/Cube/2DArray overloads lower to explicit gradient operands in SPIR-V.
  Native `metal_kernel` rejects the call by name; it must not execute the marker-type CPU stub.

- **`family=` surface sharing is Vulkan-only.** Both emitters accept N kernel methods per class
  with `kernel=` picking per instance (the sibling fixtures are
  `modules/dasLLAMA/tests/_mtl_toy.das` and `modules/dasLLAMA/tests/_vkd_toy.das`); `family=`
  has no Metal counterpart because Metal's `enc_*` builder is the whole generated surface, so a
  family has nothing to share.
- **Method flattening, scope splicing, and plain-member thread-local state are Metal-only.**
  `msl_emit` splices method bodies inline (statically devirtualized, value and statement
  position) and lowers plain class members to kernel-entry thread-locals (scalar-with-init,
  fixed array, carried pointer walk - the state contract is per-thread); `spirv_emit` rejects an unannotated member and
  lowers a call as a call, but a `[spirv_inline]` helper's body in place (`msl_emit` ignores the mark). Deliberate.
- **The SSBO access decorations - `@readonly`, `@maybe_readonly`, `@coherent` - are Vulkan-only, and
  the asymmetry is deliberate.** `spirv_emit` decorates the `OpVariable` (`NonWritable`, `Coherent`);
  MSL has no per-buffer qualifier for either - a `device` buffer's stores reach another threadgroup
  through the device-scope fence the kernel already issues, so the last-arriving-workgroup hand-off
  needs no decoration there, and `msl_emit` reads the three annotations as no-ops.
- **The tmm2d tensor-GEMM builtin family is Metal-only; the asymmetry is pending, not
  deliberate.** The family - whole-GEMM helpers plus the staged threadgroup protocol - lowers
  MPP `matmul2d` cooperative tensors, and the CPU-replay bodies are its only cross-checked
  twin.
- **Grid-literal validation is asymmetric; pending, not deliberate.** Vulkan rejects a
  non-int64 ceil-div grid parameter at macro apply; the Metal emitter accepts it and infers a
  type silently.
- **cm2 decode-in-load is Vulkan-only by hardware.** `[spirv_decode]`'s callback form lowers
  SPV_NV_cooperative_matrix2 tensor loads. Metal-4 tensors have no decode-callback analogue, so
  the Metal quant GEMMs stage dequant through threadgroup memory instead. Deliberate,
  target-specific - not a pending port.
- **The workgroup-scope cooperative-matrix tile family is Vulkan-only by hardware.** The
  `coopmatWg{A|B|Acc}_*` marker structs, their `coopmatMulAdd` / `coopmatConvert` overloads and
  the tensor-layout loads and stores lower SPV_NV_cooperative_matrix2 workgroup tiles; Metal-4
  tensors carry no workgroup-scope tile of that form, so the Metal GEMMs stage through
  threadgroup memory. Deliberate, target-specific - a new tile shape (a marker struct and the
  overload that types its call) is a member of this family, not a new asymmetry.
- **A kernel body calling a `[spirv_decode]` method directly is Vulkan-only; pending, not
  deliberate.** dasSpirv emits the call as an ordinary function - on a plane element the
  callee takes the element's index and chains through the plane, on a copy the struct
  parameter spills to a local (`dasSpirv/ARCHITECTURE_COOPMAT.md` sec.3.5). An MSL function takes a
  struct by value with its members addressable as written, so Metal needs no spill; the
  direct-call form itself has no MSL fixture yet, and a Metal kernel wanting one decode body
  for a staged tile is what would land it.
- **Literal fixed-array hoisting is Metal-only; pending, not deliberate.** `msl_emit` lowers a
  `let` fixed-array local whose elements are all literals to a program-scope `constant` table
  (renamed on a same-name/different-content collision); `spirv_emit` keeps such a local in
  Function storage, where a dynamically indexed read spills to private memory - the codebook
  and grid quant kernels on Vulkan therefore pack their tables into `uint4` words by hand.
  Done = the SPIR-V twin (a constant-storage array for the same shape) or a lint on a
  dynamically indexed fixed-array local inside a kernel class.
- **No-contraction is a compile option on Metal and a decoration on Vulkan; deliberate.** Metal
  carries it by `[metal_kernel(fastmath=false)]`, the library's `fastmath=false` compile option,
  and writes no MSL text for it; the SPIR-V emitter's `precise` mark decorates each float result
  `NoContraction` (`modules/dasSpirv/ARCHITECTURE.md`, the `precise` section), so the mark has no
  Metal twin - a body ported between the emitters carries the property on its annotation.

