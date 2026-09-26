# dasLLAMA GPU Kernel Body Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_GPU.md`, `ARCHITECTURE_GPU_VULKAN.md`, `ARCHITECTURE_GPU_VULKAN_NROW.md`,
`ARCHITECTURE_GPU_RACE_SHAPES.md`. Planned work: `followup_metal.md`, `followup_vulkan.md`.

**Routed from `REVIEW_GPU.md`: a diff that checklist routes here applies this list together
with `REVIEW_GPU.md`'s and `REVIEW.md`'s.**

**A kernel body that emits a function pointer or a vtable into the shader is a defect - splice
the choice at compile time instead.** A `class template` / `def abstract` / `def override`
splice is compile-time and conforms - check the emission, not the das spelling.

**Inside the body of a kernel's main loop - a loop whose trip count grows with the work one
thread of the dispatch does, per element or per row - a bounds guard, a tail guard, a nested
loop's own bound, or an `[unroll]` count whose live iterations run different bodies, when its
answer is the same for every thread and the host fixes its deciding value before it records the
dispatch, is a defect: stamp the deciding value as a `@template_constant` or a literal in the
body (a kernel body reads no module constant), never a push constant, uniform or kargs field;
where the value is a per-call extent (a row, key or frame count), peel the loop instead - the
full chunks under the stamped chunk bound, then one tail pass that carries the guard.** An
`[unroll]` count whose every live iteration runs the same body is no such branch - the GPU
compiler hoists it.

**Outside an `[unroll]` loop, a main-loop guard of that kind may instead clamp the index so the
guarded work runs on a live value - an index inside the extent - whose result is never stored.**
Inside an `[unroll]` loop a clamp folds every dead iteration - one past the live count - against a
live one and costs what the branch saves.

**Inside an `[unroll]` loop, never replace a host-fixed main-loop branch with an index clamp -
write the value into the body instead; outside one, a clamp that runs the guarded work on a live
value and never stores its result conforms.** Inside an `[unroll]` loop a clamp folds every dead
iteration against a live one and costs what the branch saves.

**A diff that adds or changes a `[metal_dispatch]` kernel whose addressing assumes an alignment
of a value the builder receives - a `params=` name or a kargs field - declares each such
alignment as one `<lhs> % N` item in `requires =`, comma-separated.** An assumed alignment
includes a main loop stepping fixed-size chunks with no partial-last-chunk check, a vector-typed
view of a row (a float4 index of a stride) and a fixed-size run a lane loads; the generated
builder then trips on the first misaligned dispatch instead of silently reading into the next
row's bytes.

**A driver that keeps misaligned shapes off a kernel whose addressing assumes an alignment of a
value its builder receives gates each dispatch site of that kernel on that site's own K, the
extent that site's loop steps along, never on one gate covering every site.**

**A site's alignment gate on a value that site uses only as its K extent divides by the chunk its
kernel steps, or by the multiple of that chunk the site's split of K across dispatches forces;
any other divisor is a defect.** A gate that checks less than the kernel's chunk silently drops a
tail; a gate that checks more than the site's own split forces never sees a shape the kernel could
serve. A divisor the source weight format forces is a check that the weights are well-formed, not
an alignment gate; the site keeps it as a separate check next to the kernel-chunk gate.

**A value that dispatch sites read in more than one role - K at one, the output extent at another -
is gated, at every one of those sites, on the least common multiple of the divisors each role
requires; a gate on it at one role's divisor alone is a defect.** A gate at one role's divisor
admits a shape that misaligns the other role's kernel, which then drops that site's tail.

**A diff that annotates a kernel class with `[metal_kernel(float_a_ok=true)]` names in the PR
description the `ARCHITECTURE_GPU_RACE_SHAPES.md` sec.2.2b line whose stated property covers the
class, or adds that line in the same change.** A float `matmul2d` A operand keeps the op off its
native fast path; the section is the ledger of the shapes that accept the slower path
deliberately.

**Never threadgroup-stage a `matmul2d` operand whose staged form matches its stored form -
stream it from device instead.** A dequant, a transpose, or a layout or element-type change
makes the forms differ. A staged pass-through costs the op more than the reads it saves.

**Never fill a `@workgroup` tile with a loop whose tile address - where the lane writes in the
tile - needs a div or mod of a run-time value other than the lane's own slot index (the index that
steps by one from lane to lane); the counter of an `[unroll_full]` loop is a compile-time constant
once unrolled, not a run-time value. Give each lane a consecutive run of elements, or a
lane-coalesced stride (`i += 32`), instead.**

**Never decide a kernel row's validity or owner by scanning the per-bucket base and count
arrays - the bucket-ordered buffer is the routed rows sorted so each expert's rows form one
contiguous run, a bucket - read the one per-row entry instead.** The bucket-building kernel
writes that per-row entry. The scan repeats on every thread of every row's threadgroup, and it
grows with the bucket count.

**Never test the validity of a row in the bucket-ordered buffer (the routed rows sorted so each
expert's rows form one contiguous run) against the pad sentinel `0xFFFFFFFF`; compare the row's
per-row bucket entry, the one the bucket-building kernel writes, with the live entry count
(positions x experts per token, `npos * nk`) instead.** Rows past the last expert's written tail
hold stale pool bytes, not the sentinel, and an equality test sends their token index out of
bounds.

**A method of a Metal kernel class that folds one plain float sum or max (no compensation term, no
index carried alongside) across the threadgroup by hand - a `simd_shuffle_xor` loop that halves the lane
distance each step, a lane-0 loop over a `@workgroup` float array with one slot per simdgroup, one
`@workgroup` value that one lane writes and every lane reads - is a defect: a class deriving
`MetalTgReduceBase` calls its fold methods over its own `partial[]`; any other class calls
`tg_sum_all` / `tg_max_all` over its own `@workgroup` array.**

**A Metal kernel body that can run one fold call on a `@workgroup` array after another on the same
array - two calls to `tg_sum_all` / `tg_max_all` or a `MetalTgReduceBase` fold method in sequence,
or one such call inside a loop - runs a `barrier()` between them.**

**Never put an op every lane of the group must reach together - a `barrier()`, a simdgroup matrix
op, a subgroup shuffle, vote, ballot or reduction, or a call to a function that runs one, directly
or through its callees (`tg_sum_all`, `tg_max_all` and every `MetalTgReduceBase` fold method do) -
behind an early `return`, a loop or a branch that a per-lane value decides, unless that value is
equal across every lane the op exchanges with (the workgroup for a barrier; the simdgroup for a
simdgroup matrix op or a subgroup shuffle, vote, ballot or reduction; for a call, the widest scope
among the ops it reaches); gate or bound it with such a value, or hoist the op out.** A lane that exits early, or reaches the op a
different number of times, leaves the group unable to complete it.

**An encoder that dispatches a kernel form (a kernel class or a template instance) indexing any
fixed-capacity array or buffer by a host-chosen count - a loop with no bounds or tail guard, a
walk bounded by a stamped constant, a `@workgroup` stage sized by a literal - never lets an
address pass the allocation: it sizes a device buffer to the walk's last address, and a
threadgroup stage's literal capacity is held by a check in the dispatching code that declines a
larger shape before the dispatch is recorded (on the tower, the seat's shape check - the seat
being the driver's per-stage dispatch record); an encoder without that guarantee is a defect.** A
`requires =` contract on the class is that guarantee for the dimension it names; an unchecked
claim that an extent divides evenly is not. A padded chunk's
walk can run past the live extent, and one read of stale bytes in a shared tile corrupts real
rows.

**Never let a pad row that feeds the reduction of a live output row - a pad along the reduction
axis - reach a `matmul2d` or a staged cooperative tile as an operand; stage it as zero, or bound
the walk at the live row count. A pad row is a row past a buffer's live count that the dispatch
producing the buffer did not write.** A pad along the reduction axis holds recycled pool bytes,
so it multiplies stale values (NaN included) into every live output row.
