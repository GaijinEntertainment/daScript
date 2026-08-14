# dasMetal Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`MASTERPLAN.md`.

## Tests

**A new emitter capability — a new emit site or a newly ACCEPTED construct — ships, in the
same change, a text fixture under `tests/msl/` (`_msl_common.das` kernels) and a census kind
of its own.** A new emit form that reuses a sibling's census kind hides its own loss; the
default entry-point path counts as a construct (a fixture must exercise it).

**A new rejection path ships a `tests/msl/_fail_closed/` fixture and asserts its needle in
`tests/msl/test_msl_fail_closed.das`, in the same change.**

**A behavioral change ships a CPU-oracle test under `tests/metal/`** — the kernel method runs
on the CPU as ordinary das, and the GPU result must match it.

**The census gate (gate B) is two-directional, and removal is a census event too.** Every new
emit site records a census kind and the kind joins `declared_msl_census`; every removed emit
site's kind leaves it, everywhere it is named. Whether a fixture actually emits each kind is
settled by running `tests/msl` (`test_msl_census.das` asserts set equality both ways) — text
has no disassembler, so the census is the emitter's only coverage proxy.

## Implementation

**Every kernel — and every function it calls — is a construct the CPU can execute.** One that
cannot has no place in the emitter. Semantic equality is owned by the CPU-oracle rule above,
under the lifetime contract: kernel state (plain members) is PER-THREAD, so the oracle
constructs a fresh instance per simulated thread — a shared-instance oracle is a defect of
the test, not evidence against the kernel.

**Anything outside the lowered subset produces a clean compile error naming the construct —
on every rail that can see the construct** — never a silently wrong kernel.

**A capability that carries a shape constant into the kernel as a value ships a specialization
path beside it** — per-type overloads, monomorphized generics, or `static_if` on a
compile-time witness. Kernel shape is compile-time; only data is runtime (the consumer-side
statement is dasLLAMA `ARCHITECTURE.md` §2.2). Where the emitter cannot specialize, the limit
is recorded in `MASTERPLAN.md` in the same change.

**A claim about a shape constant is checked against the emitted text fixture, not the das
source.**

## The emitter mirror

**A kernel-model capability lands on both emitters, or its asymmetry is recorded.** This
emitter and dasSpirv (`modules/dasSpirv`) lower the same kernel model — classes with resource
members, ordinary methods, free-function callees, inheritance with devirtualized overrides. A
capability added here lands on the SPIR-V side too (and the reverse) unless it is genuinely
target-specific (Metal-4 tensors vs cooperative matrix, simdgroup vs subgroup semantics); a
deliberate asymmetry is recorded in `MASTERPLAN.md`. A diff that grows the kernel model on one
side with no sibling change and no ledger entry is a defect.

**A declaration moved into `daslib/shader_lingua_franca` is granted to every shader rail** —
the module is recognized wholesale (by module, not by symbol), dasGlsl included. Each rail
either lowers the new declaration or rejects it by name, with a fail-closed fixture in that
rail's test home, in the same change.
