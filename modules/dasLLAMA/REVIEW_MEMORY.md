# dasLLAMA Memory and Lane Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE_RUNTIME.md`. Planned work: `followup_general.md`.

**A function-typed module global that a job (a forked context) invokes or a serialized exe calls
is set by an `[init]` that re-establishes it when it reads null - never by a declaration
initializer alone.** A serialized exe and a forked context restore globals as data, so a
declaration initializer alone arrives null and dies at the first invoke while every `-jit` gate
stays green.

**A value that a team-lane kernel reads - anything reachable from a `team_parallel_for` /
`team_parallel_for_indexed` / `team_parallel_stages` body (`daslib/jobque_boost.das`, repo
root) or from a `maybe_parallel_for*` body (`dasllama/dasllama_par.das`), which can dispatch onto
those same lanes - is a `def` returning it, never a module global with a declaration initializer
(`let` or `var`), and nothing reachable from such a body writes or resizes a module global.** A
pooled lane's globals are not its own: a read comes back zero, a resize trips on a stale array.

**A buffer in `dasllama/` whose element count grows with a count the model file sets is declared
`@exact_size` (`@scratch @exact_size` on a `@scratch` carrier), and every `resize` of it follows
a `reserve(n)` or `ensure_capacity(n)` whose `n` is the resized count - a sizing helper of
`dasllama/dasllama_math.das` (`reserve_resize`, `grow_resize`, `ensure_length`,
`overwrite_resize`), the builtin `scratch_resize` on a `@scratch` carrier, or the pair spelled
out - however small the count looks.** A bare grow past the heap's unreserved-size cap panics
the load on the first big model, not at the call site.

**A diff that adds an allocation, or adds a term to an existing allocation's size, that grows
with a scaling count states that size in bytes in a `PERF_LEDGER.md` row: at the largest shape
the code path accepts, or, where the path accepts any value of the count, as a formula in the
count with its value at two shapes that differ in it - a shape being one setting of the scaling
counts.** An allocation is one buffer, or one sub-range of a buffer shared by several uses. A
scaling count is a count the model file sets, how many tokens one step computes at once, how many
rows one media encode feeds (an image's patches, a clip's frames), or how many regions one buffer
is split into (the K/V cache's device copy, one region per request served at once; an MoE
dispatch's expert regions).

**A diff after which an existing allocation's size starts or stops growing with a scaling count
(a model-file count, tokens per step, rows per media encode, or regions per buffer) without
gaining a term ships the measured pair - peak footprint and wall-clock - in `PERF_LEDGER.md`,
with the decision it settles.**
