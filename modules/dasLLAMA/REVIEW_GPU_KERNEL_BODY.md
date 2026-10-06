# dasLLAMA GPU Kernel Body Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_GPU.md`, `ARCHITECTURE_GPU_VULKAN.md`, `ARCHITECTURE_GPU_VULKAN_NROW.md`,
`ARCHITECTURE_GPU_RACE_SHAPES.md`, `ARCHITECTURE_GPU_PREFILL_WINDOW.md`. Planned work:
`followup_metal.md`, `followup_vulkan.md`.

**A kernel body that emits a function pointer or a vtable into the shader is a defect - splice
the choice at compile time instead.** A `class template` / `def abstract` / `def override`
splice is compile-time and conforms - check the emission, not the das spelling.

**A host-fixed branch inside a kernel's main loop whose deciding value is not a per-call extent
takes that value from a `@template_constant`, a literal or a module `let` - never a module `var`,
push constant, uniform or kargs (kernel-argument struct) field.** A host-fixed branch
is a loop bound or branch whose deciding value the host fixes before it records the dispatch: a
bounds guard, a tail guard, a nested loop's own bound, or an `[unroll]` count whose live
iterations run different bodies. A per-call extent is a count that can differ between two
dispatches of one kernel instance (one class, one set of template constants) within the inference
of at least one model that instance serves. The main loop is a loop whose trip count grows with the work one
thread does, per element or per row.

**A host-fixed main-loop branch whose deciding value is a per-call extent is never stamped (baked
into the kernel as a `@template_constant` or literal): peel it (the full chunks run under the
stamped chunk bound, then one tail pass carries the guard), or,
outside an `[unroll]` loop, replace it with an index clamp that runs the guarded work on an index
inside the extent and never stores that iteration's result.**

**Inside an `[unroll]` loop, never replace a host-fixed main-loop branch with an index clamp -
stamp its deciding value, or peel the loop when that value is a per-call extent.** Inside an
`[unroll]` loop a clamp folds every dead iteration - one past the live count - against a live one
and costs what the branch saves.

**A diff that adds or changes, inside a `for [unroll_full]` loop of a Metal kernel, a read of a
kernel buffer argument whose index a local variable or a class method builds as a run-time base
plus a constant offset of the loop variable takes the address once outside the loop
(`let p = unsafe(addr(buf[<base>]))`) and reads `unsafe(p[<offset>])`.** `REVIEW.das` flags the same
sum written directly in the index, and weakening that check is a defect. The Metal compiler does
not fold a `uint` index sum into one address, so adjacent loads do not merge
(`ARCHITECTURE_GPU_PREFILL_WINDOW.md#kernel-load-addressing`).

**A diff that adds or changes a `[metal_dispatch]` kernel whose addressing assumes an alignment
of a value the builder receives - a `params=` name or a kargs field - declares each such
alignment as one `<lhs> % N` item in `requires =`, comma-separated.** An assumed alignment
includes a main loop stepping fixed-size chunks with no partial-last-chunk check, a vector-typed
view of a row (a float4 index of a stride) and a fixed-size run a lane loads; the generated
builder then trips on the first misaligned dispatch instead of silently reading into the next
row's bytes.

**A diff that adds or changes a driver that keeps misaligned shapes off a kernel whose addressing
assumes an alignment of a value its builder receives gates each dispatch site of that kernel on
that site's own K - the extent the kernel's fixed-size chunk steps along, in a loop or across
invocations; one gate shared by several sites conforms only when it tests every one of those
sites' K.**

**A site's alignment gate on a value that site uses only as its K divides by the chunk its
kernel steps along K, or by the multiple of that chunk the site's split of K across dispatches
forces.** A gate that checks less than the kernel's chunk silently drops a tail; a gate that
checks more than the site's own split forces never sees a shape the kernel could serve. A divisor
the source weight format forces is a check that the weights are well-formed, not an alignment
gate; the site keeps it as a separate check next to the kernel-chunk gate.

**A value that dispatch sites read in more than one role - K at one, the output extent at another -
is gated, at every one of those sites, on the least common multiple of the divisors each role
requires.** A gate at one role's divisor
admits a shape that misaligns the other role's kernel, which then drops that site's tail.

**A diff that annotates a kernel class with `[metal_kernel(float_a_ok=true)]` names in the PR
description the `ARCHITECTURE_GPU_RACE_SHAPES.md#tensor-gemm-shapes-that-measured-out-m5` line
whose stated property covers the class, or adds that line in the same change.** A float
`matmul2d` A operand keeps the op off its native fast path; the section is the ledger of the
shapes that accept the slower path deliberately.

**Never threadgroup-stage a `matmul2d` operand whose staged form matches its stored form -
stream it from device instead.** A dequant, a transpose, or a layout or element-type change
makes the forms differ. A staged pass-through costs the op more than the reads it saves.

**Never fill a `@workgroup` tile with a loop whose tile address - where the lane writes in the
tile - takes a div or mod whose dividend is any run-time value but the lane's own slot index (the
index that steps by one from lane to lane) plus compile-time constants; the counter of an
`[unroll_full]` loop is a compile-time constant once unrolled, and the divisor may be a run-time
value. Give each lane a consecutive run of elements, or a lane-coalesced stride (`i += 32`),
instead.**

**Never decide a row's validity or owner in the bucket-ordered buffer (the routed rows sorted so
each expert's rows form one contiguous run, a bucket) by scanning the per-bucket base and count
arrays, nor by testing the row against the pad sentinel `0xFFFFFFFF` - read the row's per-row
bucket entry, the one the bucket-building kernel writes, and for validity compare it with the live
entry count (positions x experts per token, `npos * nk`).** The scan repeats on every thread of
every row's threadgroup and grows with the bucket count; rows past the last expert's written tail
hold stale pool bytes, not the sentinel, and an equality test sends their token index out of
bounds.

**A method of a Metal kernel class that folds one plain float sum or max (no compensation term,
no index carried alongside) across the threadgroup by hand - a `simd_shuffle_xor` loop that
halves the lane distance each step, a lane-0 loop over a `@workgroup` float array with one slot
per simdgroup, one `@workgroup` value that one lane writes and every lane reads - is a defect: a
class deriving `MetalTgReduceBase` calls its fold methods over its own `partial[]`; any other
class calls `tg_sum_all` / `tg_max_all` over its own `@workgroup` array.**

**A diff that adds or changes, in the body of a dispatched kernel class (one a `[vk_dispatch]`
declares), a hand-written fold of one plain float sum or max (no compensation term, no index
carried alongside) over every lane of a Vulkan kernel's workgroup - a subgroup shuffle loop, a
lane-0 loop over a `@workgroup` array, one `@workgroup` value that one lane writes and every lane
reads - is a defect: derive `WgReduceBase` and call its `wg_sum`, `wg_max` or `wg_rms_inv`
instead - a kernel both homes stamp derives `GkWgReduce` (the base `WgReduceBase` rides) and calls
its `wg_sum_into` / `wg_max_into`.** A fold into more than one result - separate sums over parts of the
workgroup - is not one value; `ARCHITECTURE_GPU.md#gpu-backends` names the bodies that fold that
way.

**A diff that adds or changes a Metal kernel body that can run one fold call on a `@workgroup`
array after another on the same array - two calls to `tg_sum_all` / `tg_max_all` or a
`MetalTgReduceBase` fold method in sequence, or one such call inside a loop - puts a `barrier()`
between them.**

**Never put an op every lane of its exchange set must reach together - a `barrier()`, a simdgroup
matrix op, a subgroup shuffle, vote, ballot or reduction, or a call to a function that runs one,
directly or through its callees (`tg_sum_all`, `tg_max_all` and every `MetalTgReduceBase` fold
method do) - behind an early `return`, a loop or a branch that a per-lane value decides, unless
that value is equal across the op's exchange set; gate or bound it with such a value, or hoist the
op out.** The exchange set is the lanes the op reads: the workgroup for a barrier, the simdgroup
for a simdgroup matrix op, vote, ballot or reduction, the partner lanes a shuffle reads, and for a
call the widest set among the ops it reaches. A lane that exits early, or reaches the op a
different number of times, leaves the set unable to complete it.

**An encoder that dispatches a kernel form (a kernel class or a template instance) indexing a
device buffer by a host-chosen count or base offset, in a walk with no bounds or tail guard or a
walk bounded by a stamped constant, sizes that buffer to the walk's last address, so no address
passes the allocation.** A dimension the class's `requires =` contract names needs no
padded buffer, because the builder rejects the misaligned shape; an unchecked claim that an
extent divides evenly does not. A walk that rounds the extent up to whole chunks reads past the
live extent, and one read of stale bytes in a shared tile corrupts real rows.

**An encoder that dispatches a kernel form whose `@workgroup` stage is sized by a literal, and
indexed by a host-chosen count or base offset, declines a shape larger than that literal in the
dispatching code before the dispatch is recorded.** Workgroup memory has no bounds check, so an
index past the literal reads or writes another stage's bytes.

**Never let a pad row that feeds the reduction of a live output row - a pad along the reduction
axis - reach a `matmul2d` or a staged cooperative tile as an operand; stage it as zero, or bound
the walk at the live row count. A pad row is a row past a buffer's live count that the dispatch
producing the buffer did not write.** A pad along the reduction axis holds recycled pool bytes,
so it multiplies stale values (NaN included) into every live output row.
