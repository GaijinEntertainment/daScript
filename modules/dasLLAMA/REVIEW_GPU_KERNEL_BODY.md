# dasLLAMA GPU Kernel Body Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_GPU.md`, `ARCHITECTURE_GPU_VULKAN.md`, `ARCHITECTURE_GPU_VULKAN_NROW.md`,
`ARCHITECTURE_GPU_RACE_SHAPES.md`. Planned work: `followup_metal.md` for Metal,
`followup_vulkan.md` for Vulkan.

**Routed from `REVIEW_GPU.md`: a diff that checklist routes here applies this list together
with `REVIEW_GPU.md`'s and `REVIEW.md`'s.**

**A kernel body that emits a function pointer or a vtable into the shader is a defect - splice
the choice at compile time instead.** A `class template` / `def abstract` / `def override`
splice is compile-time and conforms - check the emission, not the das spelling.

**In a kernel's main loop, a branch whose answer is the same for every thread of the dispatch, and
whose deciding value the host fixes before it records the dispatch, is a defect. This covers a
bounds guard, a tail guard, a nested loop's own bound, and a push-constant count tested inside an
`[unroll]` loop whose live iterations run different bodies. Stamp the deciding value - a
`@template_constant`, or a module constant the class reads where no template instantiates it;
never a push constant, uniform or kargs field; for a guard outside an `[unroll]` loop, you may
instead clamp the index so the guarded work runs on a live value and its result is never
stored.** Inside an `[unroll]` loop a clamp folds every dead iteration against a live one and
costs what the branch saves, while a count whose every live iteration runs the same body costs
nothing - the GPU compiler hoists it.

**A chunk-stepping `[metal_dispatch]` kernel (its main loop steps fixed-size chunks with no
partial-last-chunk check) declares each alignment it assumes on a value the builder receives - a
`params=` name or a kargs field - as one `<lhs> % N` item in `requires =`, comma-separated.** The
generated builder then trips on the first misaligned dispatch instead of reading the next row.

**A driver that keeps misaligned shapes off a chunk-stepping kernel (its main loop steps
fixed-size chunks with no partial-last-chunk check) gates each dispatch site of that kernel on
that site's own K, the extent that site's loop steps along, never on one gate covering every
site.**

**A dispatch site's alignment gate whose divisor is neither the chunk the kernel that site
dispatches steps nor a multiple of that chunk the site forces by splitting its K extent across
dispatches is a defect.** A gate that checks less than the kernel's chunk silently drops a tail;
a gate that checks more than the site's own split forces never sees a shape the kernel could serve.

**Weakening this folder's `tests/test_metal_float_a_gate.das` - the gate that checks the MSL
emitter refuses a float `matmul2d` A operand without the `[metal_kernel(float_a_ok=true)]`
license - is a defect.** A float operand keeps the op off its native fast path.

**A diff that stamps a kernel class `[metal_kernel(float_a_ok=true)]` outside the set
`ARCHITECTURE_GPU_RACE_SHAPES.md` sec.2.2b sanctions extends that section in the same change.**
A class the section already covers as a property needs no new line.

**Never threadgroup-stage a `matmul2d` operand whose staged form matches its stored form -
stream it from device instead.** A dequant, a transpose, or a layout or element-type change
makes the forms differ. A staged pass-through costs the op more than the reads it saves.

**Never fill a `@workgroup` tile with a loop whose per-element address needs a div or mod of
anything but the lane's own slot index (the index that steps by one from lane to lane); give
each lane a consecutive run of elements, or a lane-coalesced stride (`i += 32`), instead.** A
device-to-device copy loop is already coalesced and conforms.

**Never decide a kernel row's validity or owner by scanning the per-bucket base and count
arrays - a bucket is the run of rows one expert owns in the bucket-ordered buffer - read the
one per-row entry instead.** The bucket-building kernel writes that per-row entry. The scan
repeats on every thread of every row's threadgroup, and it grows with the bucket count.

**Never test the validity of a row in the bucket-ordered buffer - where each expert owns one
run of rows - against the pad sentinel `0xFFFFFFFF`; compare the row's per-row bucket entry,
the one the bucket-building kernel writes, with the live entry count (positions x experts per
token, `npos * nk`) instead.** Rows past the last expert's written tail hold stale pool
bytes, not the sentinel, and an equality test sends their token index out of bounds.

**Never put an op every lane of the group must reach together - a `barrier()`, a simdgroup matrix
op, or a subgroup shuffle, vote, ballot or reduction - behind an early `return`, a loop or a
branch that a per-lane value decides, unless that value is equal across every lane the op
exchanges with (the workgroup for a barrier or matrix op, the subgroup for the rest); gate or
bound it with such a value, or hoist the op out.** A lane that exits early, or reaches the op a
different number of times, leaves the group unable to complete it.

**An encoder that picks a kernel form whose loop carries no bounds or tail guard, or a stamped
constant - a `@template_constant`, or a module constant the class reads where no template
instantiates it; never a push constant, uniform or kargs field - bound in place of one, written
without one or generated from a template instance that has none, shows that every address the
form touches stays inside its buffers' allocations.** A `requires =` contract on the class is
that showing for the dimension it names; an unchecked claim that an extent divides evenly is not.
A padded chunk's walk can run past the live extent, and one poisoned read in a shared tile
corrupts real rows.

**Never let a pad row reach a `matmul2d` or a staged cooperative tile as an operand; stage it as
zero, or bound the walk at the live row count. A pad row is a row past a buffer's live count that
the dispatch producing the buffer did not write.** A pad row holds recycled pool bytes, so one
used as an operand multiplies stale values (NaN included) into every real row of the tile.
