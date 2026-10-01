# dasLLAMA Memory and Lane Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_ENGINE.md`, `ARCHITECTURE_INVARIANTS.md`. Planned work: `followup_general.md`,
`PERF_LEDGER.md`.

An allocation is one buffer, or one sub-range of a buffer shared by several uses; a buffer that
replaces buffers the diff removes counts as added. A scaling count is a count the model file
sets, how many tokens one step computes at once, how many rows one media encode feeds (an
image's patches, a clip's frames), or how many regions one buffer is split into (the K/V
cache's device copy, one region per request served at once; an MoE dispatch's expert regions).
A shape is one setting of the scaling counts.

**A function-typed module global that a job (a forked context) invokes or a serialized exe - an
exe that restores its built program, globals included, from a saved image at startup - calls
is set by an `[init]` that re-establishes it when it reads null - never by a declaration
initializer alone.** A serialized exe and a forked context restore globals as data, so a
declaration initializer alone arrives null and dies at the first invoke while every `-jit` gate
stays green.

**A value that a team-lane kernel reads - anything reachable from a `team_parallel_for` /
`team_parallel_for_indexed` / `team_parallel_stages` body (`daslib/jobque_boost.das`, repo
root) or from a `maybe_parallel_for*` body (`dasllama/dasllama_par.das`), which can dispatch onto
those same lanes - is a `def` returning it, never a module global with a declaration initializer
(`let` or `var`).** A pooled lane's globals are not its own: a read comes back zero.

**Nothing reachable from a `team_parallel_for` / `team_parallel_for_indexed` /
`team_parallel_stages` or `maybe_parallel_for*` body writes or resizes a module global - write
into a buffer the dispatching caller sizes and passes in instead.** A pooled lane's globals are
not its own: a resize trips on a stale array.

**A buffer in `dasllama/` whose element count grows with a count the model file sets is declared
`@exact_size` (`@scratch @exact_size` when the buffer is `@scratch`), and every `resize` of it
follows a `reserve(n)` or `ensure_capacity(n)` whose `n` is the resized count - a sizing helper
of `dasllama/dasllama_math.das` (`reserve_resize`, `grow_resize`, `ensure_length`,
`overwrite_resize`), the builtin `scratch_resize` on a `@scratch` buffer, or the pair spelled
out.** A resize with no reserve before it panics once the array passes the heap's limit on
unreserved growth (`max_unreserved_size`) - on the first model big enough, which a test on a
small model never reaches.

**A diff that adds an allocation, or adds a term to an existing allocation's size, that grows
with a scaling count states that size in bytes in a `PERF_LEDGER.md` row: at the largest shape
the code path accepts, or, where the path accepts any value of the count, as a formula in the
count with its value at two shapes that differ in it.**

**A diff after which an allocation that exists before and after it starts or stops growing with
a scaling count without gaining a term ships peak footprint and wall-clock, each measured before
and after the diff at the largest shape the code path accepts - or, where the path accepts any
value of the count, at one shape the entry names - in `PERF_LEDGER.md`.** The pair records what
the change costs; it picks nothing.
