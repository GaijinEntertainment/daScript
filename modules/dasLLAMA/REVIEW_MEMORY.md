# dasLLAMA Memory and Lane Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_ENGINE.md`, `ARCHITECTURE_INVARIANTS.md`. Planned work: `followup_general.md`.

An allocation is one buffer, or one sub-range of a buffer shared by several uses; a buffer that
replaces buffers the diff removes counts as added. An allocation that a path or a model class
reaches that it did not reach before counts as added; an allocation the diff moves with its size
and reach unchanged does not. A scaling count is a count the model file sets, the context a
session serves (the model's own or one a caller pins), how many tokens one step computes at
once, how many rows one media encode feeds (an image's patches, a clip's frames), or how many
regions one buffer is split into (the K/V cache's device copy, one region per request served at
once; an MoE dispatch's expert regions). A shape is one setting of the scaling counts.

**A function-typed module global that a job (a forked context) invokes or a serialized exe - an
exe that restores its built program, globals included, from a saved image at startup - calls
is set by an `[init]` that re-establishes it when it reads null - never by a declaration
initializer alone.** A serialized exe and a forked context restore globals as data, so a
declaration initializer alone arrives null and dies at the first invoke while every `-jit` gate
stays green.

**A value that a team-lane kernel reads - anything reachable from a `team_parallel_for` /
`team_parallel_for_indexed` / `team_parallel_stages` body (`daslib/jobque_boost.das`, repo
root) or from a `maybe_parallel_for*` body (`dasllama/dasllama_par.das`), which can dispatch onto
those same lanes - is a `def` returning it, or a module `let` of a bool, number, enum or bitfield
type whose initializer is a constant - never a module `var` or any other module `let`.** A pooled
lane's globals are not its own: a read comes back zero; the optimizer, when on, folds such a
`let` into its literal at each read.

**Nothing reachable from a `team_parallel_for` / `team_parallel_for_indexed` /
`team_parallel_stages` or `maybe_parallel_for*` body writes or resizes a module global - write
into a buffer the dispatching caller sizes and passes in instead.** A pooled lane's globals are
not its own: a resize trips on a stale array.

**A buffer in `dasllama/` whose element count grows with a count the model file sets is declared
`@exact_size` (`@scratch @exact_size` when the buffer is `@scratch`).** PERF032 - the lint that
flags a `resize` with no reserve before it - checks only `@exact_size` arrays.

**Every `resize` of a buffer in `dasllama/` whose element count grows with a count the model
file sets follows a `reserve(n)` or `ensure_capacity(n)` whose `n` is the resized count, or goes
through a helper that does both - `reserve_resize`, `grow_resize`, `ensure_length` or
`overwrite_resize` of `dasllama/dasllama_math.das`, or the builtin `scratch_resize` on a
`@scratch` buffer; weakening PERF032, the lint that flags the `resize` with no reserve before
it, is a defect.** The lint never compares the two counts, and a resize past the heap's limit on
unreserved growth (`max_unreserved_size`) panics on the first model big enough.

**A diff that adds an allocation that outlives the call that makes it - a device buffer, or an
array a module or a struct holds - or adds a term to such an allocation's size, that grows with
a scaling count states that size in bytes in a `PERF_LEDGER.md` row: at the largest shape the
code path accepts, or, where the path accepts any value of the count, as a formula in the count
with its value at two shapes that differ in it.**

**A diff after which an allocation that outlives the call that makes it - a device buffer, or an
array a module or a struct holds - and that exists before and after the diff starts or stops
growing with a scaling count without gaining a term ships peak footprint and wall-clock, each
measured before and after the diff at the largest shape the code path accepts - or, where the
path accepts any value of the count, at one shape the entry names - in `PERF_LEDGER.md`.** The
pair records what the change costs; it picks nothing.
