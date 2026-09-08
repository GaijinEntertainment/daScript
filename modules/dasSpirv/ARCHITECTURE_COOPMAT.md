# dasSpirv - cooperative matrix and emission-time unrolling

Companion to `ARCHITECTURE.md`; section numbers are `ARCHITECTURE.md`'s. Two
cooperative-matrix arms run through these sections: **KHR** is `SPV_KHR_cooperative_matrix`,
the vendor-neutral subgroup-scope tile, and **cm2** is NVIDIA's `SPV_NV_cooperative_matrix2`
with `SPV_NV_tensor_addressing`, which adds workgroup-scope tiles and tensor-addressed block
loads that decode a quantized plane through a callback function. This document carries
sections 3.2 (the cm2 tile markers), 3.3 (the four-wide decode twin), 3.5 (a kernel calling a
decode method directly), 3.6 (`for [unroll_full]`) and 3.7 (the coopmat element loop's `Unroll`).
**Read `ARCHITECTURE_COMMON.md` (repo root) first - its contract binds this document.** The
checklists that bind a diff here are `REVIEW.md` (this folder) and
`modules/REVIEW_SHADER_EMITTERS.md`.

### 3.2 A cm2 tile shape is one struct declaration {#cm2-tile-markers}

The workgroup-scope cooperative-matrix tiles are marker structs in `spirv_builtins.das` whose
NAMES carry their geometry - `coopmatWg{A|B|Acc}_{f16|f32|s8|s32}_{R}x{C}` - and
`coopmat_wg_info` parses that name instead of looking the struct up in a table. Adding a tile
shape is therefore one struct declaration plus the overload that types the das call
(`coopmatMulAdd` for a multiply tile, `coopmatConvert` for an accumulator-only tile): no branch of
the emitter changes, because every cm2 branch reads rows, columns and component width out of the
parse. The tile markers are empty structs with no storage, so every builtin over them - the tensor
loads and stores, the decode forms, `coopmatMulAdd`, `coopmatConvert`, `coopmatClamp`, the
reductions - has an inert CPU body that cannot compute what the emitted form computes. A coopmat
kernel's device test therefore judges its result against a plain CPU reference of the same
arithmetic - one of the oracle forms `REVIEW.md` (this folder) lists - because running the
kernel body itself on the CPU computes nothing. A reduction width known only at run time reaches
a SPIR-V kernel through a `tensorLayout2D` or `tensorLayout2DPad` whose dimension
`tensorLayoutSetDimension` sets - that layout is this emitter's runtime-extent descriptor.

### 3.3 The four-wide decode twin {#cm2-decode-vector}

`SPV_NV_cooperative_matrix_decode_vector` adds no opcode. It adds one capability and the
`DecodeVectorFunc` bit of the tensor-load's addressing mask. That bit's operand names a second
decode function, which returns the 4-lane vector of the tile component type. The driver calls it
for four consecutive elements along the block's last dimension, starting at a coordinate that is
a multiple of four. The scalar `DecodeFunc` stays mandatory beside it and the driver picks per
call site, so one module serves devices with and without the feature.

`coopmatLoadTensorDecode`'s tenth argument selects the twin. `true` has the emitter synthesize
it: four `OpFunctionCall`s of the scalar body at `coordInBlock.y + 0..3`, composed into the vector
(the driver's compiler inlines the calls and merges the loads they share). `false` is the
scalar-only load. A `[spirv_decode]` function returning `half4` over a `float16` decode, with the
scalar contract's parameters, is a hand-laid twin, named in that same tenth slot as `@@<function>`
or `self.<method>`. daslang spells no int8 or 32-bit four-vector the tiles would take, so the
hand-laid form is f16-only; the synthesized twin covers every scalar type a decode may return.
Twins register in their own table keyed by the function they came from, so one scalar body yields
one twin however many loads name it, and they emit after the scalar bodies they call.

The extension stands apart from the cm2 base: a module that uses the twin declares the
`CooperativeMatrixDecodeVectorNV` capability and the `SPV_NV_cooperative_matrix_decode_vector`
extension name, and the twin runs only where the device has its own
`VkPhysicalDeviceCooperativeMatrixDecodeVectorFeaturesNV` bit enabled.

The tensor-addressing operands follow the mask word in bit order - `TensorView`, then
`DecodeFunc`, then `DecodeVectorFunc` - after the load's fixed prefix: result type, result,
pointer, object, layout, memory-access mask and that mask's own extra words. The emitter appends
them in that order, and `strip_decode_vector` (below) counts forward by the same rule to the
operand it removes.

A finished module is downgraded rather than recompiled. `strip_decode_vector` walks the word
stream and removes the capability, the extension declaration, and each
`OpCooperativeMatrixLoadTensorNV`'s `DecodeVectorFunc` bit together with its operand word. The
twin's `OpFunction` stays in the module, unreferenced, and the mandatory scalar `DecodeFunc`
serves the load. One emitted blob therefore runs on a device without the feature.

### 3.5 A kernel calls a decode method directly {#direct-decode-call}

A kernel body calling a `[spirv_decode]` method directly makes an ordinary user function call:
the method has its own OpFunction, registered beside the callback form and pulling no
cooperative-matrix capability. One decode body therefore serves a tensor load on a cm2 device and
a hand-staged tile on a KHR one. The block argument takes one of two forms, fixed per method at
discovery; a method called both ways is refused.

Called on the plane element itself (`decode(wq[i], bc, cib)`), the block parameter is emitted as
the element's `uint` INDEX. The function's entry chains `OpAccessChain plane, 0, index` once,
which binds the parameter as a memory local in the plane's own storage class. The body's member
reads then chain through the plane exactly as the callback form's chain through its block
pointer, and the element is never loaded as a value. A pointer could not travel instead: under
logical addressing an SSBO pointer is not a legal OpFunctionCall argument without the
VariablePointersStorageBuffer capability, and the index needs no capability at all. Called on a
copy (`let blk = wq[i]; decode(blk, bc, cib)`), the block goes by value and lands in the spill
local of the next paragraph - the whole block loaded and stored per call, which the KHR tile
measures at a third of the element form's rate.

**A struct value parameter with an aggregate member is a memory local.** A read-only parameter of
a plain data struct that carries a fixed-array or nested-struct member (not a coopmat tile, a
tensor object, a ray query or a sampler marker) binds its SSA OpFunctionParameter and is stored at
entry into a Function-storage OpVariable. The variable is declared first in the entry block, ahead
of the body's locals and call temps; the store that fills it follows the block's last OpVariable,
because SPIR-V requires every OpVariable of a block to lead the block. The parameter is thereafter
that local: its members access-chain like a `var` struct local's. The reason is a fixed-array
member indexed at run time (`blk.qs[cib.y >> 1]`): an SSA composite offers no pointer, and
`OpCompositeExtract` takes literal indices only. A scalar-only struct parameter keeps the value
path, and its members extract. A `let` copy of a block element in a body stays a composite - its
scalar and vector members extract, and its array and struct members are refused - because reading
it whole and then storing it would double every element load, while the parameter form pays the
store once per call. The Metal emitter needs no arm for this: an MSL function takes a struct by
value and its members are addressable as written.

**A struct loaded out of a `Block` re-types at the call.** A struct loaded out of a `Block` - an
ssbo element - carries the Block's laid-out `OpTypeStruct`, a different type id from the plain
struct a parameter takes. A call passing one re-types the value with `OpCopyLogical` first, which
raises the module's version floor to 1.4 (`ARCHITECTURE.md` sec.2). The emitter remembers the
struct type each loaded id was read as and copies only when the two ids differ, so a struct
already at the plain type passes through untouched.

### 3.6 `for [unroll_full]` unrolls at emission {#unroll-full}

A `for [unroll_full] (i in range(lo, hi))` over literal bounds emits `hi - lo` copies of its body
and no loop construct. The emitter walks the body once per copy, with the induction variable bound
to that copy's `OpConstant`.

The visitor framework's own walk of the body runs after those copies and emits one more. The
emitter therefore records the lengths of the function and decoration sections after the last copy
it emits itself, and at the loop's visit truncates both sections back to those lengths - the
discarded walk's words go. The error count and the terminated flag rewind with them. The induction
binding outlives the copies, so the discarded walk still resolves the variable.

Scalar integer arithmetic on the induction constant folds at emission. Each side is a literal, the
induction constant, or an earlier fold on either, and at least one side comes from the unroll; the
folded forms are `+ - * / % << >> & | ^`, unary `-` and `~`, and a 32-bit `int`/`uint` cast. The
fold fires only inside an unrolled body, so a loop written without `[unroll_full]` emits its
arithmetic as ordinary instructions. `acc[t * 16 + c]` therefore chains an `OpConstant` index -
the shape a driver promotes to registers. A fixed array of cooperative-matrix tiles
(`coopmatAcc_f16_16x16[16]`) is one Function-storage `OpTypeArray` over the tile type, each
element reached through a constant `OpAccessChain`, so a 2-D subgroup tiling's accumulators stay
in registers where a rolled loop or the driver hint `[unroll]` leaves them indexed by the counter.

The induction variable gets no `OpVariable`: it is a constant in every copy, so `collect_locals`
skips it. The body's other locals are declared once by that same pre-scan and shared across the
copies.

A bound the compiler folds counts as a literal, so a named module constant is a legal bound (lint
and LSP compiles run with optimizations off, so `range(N)` and `int(KHR_KHALF_WORDS)` arrive
unfolded and the emitter asks for the fold). Refused: a bound the emitter cannot fold, `break`,
`continue`, `return`, `[unroll_full]` on a `while`, and any other loop hint written beside
`[unroll_full]`. An assignment to the induction variable never reaches the emitter - a loop
variable is a constant value, and the front end refuses the write. An error the body raises is
reported once, not once per copy.

dasMetal lowers the hint to `#pragma clang loop unroll(full)`, and the JIT to
`llvm.loop.unroll.full`. The interpreter ignores it: the CPU run is the same body rolled.

### 3.7 A cooperative-matrix element loop carries `Unroll`

`coopmatClamp` walks a coopmat local element by element through a hand-emitted structured loop
bounded by `OpCooperativeMatrixLengthKHR`, and its `OpLoopMerge` sets loop control `Unroll` - the
control glslang emits for `[[unroll]]`. Rolled, the dynamic per-element `OpAccessChain` index
moves the accumulator out of the registers holding the tile into addressable storage for the
whole kernel, not only for the loop.
