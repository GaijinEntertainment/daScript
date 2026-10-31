# dasLLAMA Memory and Lane Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_ENGINE.md`, `ARCHITECTURE_INVARIANTS.md`. Planned work: `followup_general.md`.

An allocation is one buffer, or one sub-range of a buffer shared by several uses; a buffer that
replaces buffers the diff removes counts as added. An allocation that a path, a model class or a
box configuration - a crown (a kernel family the box's tune sidecar enables) or a knob's default
(an environment variable's or a setting's) - reaches that it did not reach before counts as added;
an allocation the diff moves with its size and reach unchanged does not.

A lasting allocation is a container (array, table, device buffer) that outlives the call making
it - held by a module global, or by a field of a struct that outlives the call; a pooled buffer
acquired and released inside one call is not one.

A scaling count is a count the model file sets, the context a session serves (the model's own or
one a caller pins), how many tokens one step computes at once, how many rows one media encode
feeds (an image's patches, a clip's frames), or how many regions one buffer is split into (the K/V
cache's device copy, one region per request served at once; an MoE dispatch's expert regions). A
shape is one setting of the scaling counts. Peak footprint is the process's largest resident
memory over a run.

A team-lane body is anything reachable from a `team_parallel_for` / `team_parallel_for_indexed` /
`team_parallel_stages` body (`daslib/jobque_boost.das`, repo root) or from a `maybe_parallel_for*`
body (`dasllama/dasllama_par.das`), which can dispatch onto those same lanes.

**A module global whose declaration initializer holds a function value - `var g_x = @@fn`, or a
table or struct literal with a `@@fn` field - is a defect unless an `[init]` in its file sets it
again when it reads null; a module `let` holding a function value is a defect.** A serialized exe -
one that restores its built program, globals included, from a saved image at startup - and a forked
context restore globals as data, so a declaration initializer alone arrives null and dies at the
first invoke while every `-jit` gate stays green.

**A module-level value that a team-lane body reads is a `def` returning it, or a module `let` of a
bool, number, number-vector (`float2`-`float4`, `int2`-`int4`, `uint2`-`uint4`), enum or bitfield
type whose initializer is a constant or a vector constructor over constants - never a module `var`
or any other module `let`.** A pooled lane runs in another context, so a module global it reads
comes back zero; the optimizer, when on, folds such a `let` into its literal at each read.

**Nothing in a team-lane body names a module global it writes or resizes - write into a buffer the
dispatching caller sizes and passes in instead.** A pooled lane runs in another context, so a
resize acts on that context's out-of-date copy of the global.

**An array a `dasllama/` function resizes through the builtin `resize` or any helper that reaches
it, whose element count grows with a count the model file sets and is not declared `@exact_size`
(`@scratch @exact_size` when the buffer is `@scratch`), is a defect.** PERF032 - the lint that flags
a `resize` with no reserve before it - checks only `@exact_size` arrays.

**Every `resize` of a buffer in `dasllama/` whose element count grows with a count the model
file sets follows a `reserve(n)` or `ensure_capacity(n)` whose `n` is the resized count, or goes
through a `dasllama/dasllama_math.das` helper whose own body does that before it resizes
(`reserve_resize`, `grow_resize`), or is the builtin `scratch_resize` on a `@scratch` buffer;
weakening PERF032 is a defect.**
The lint never compares the two counts, and a resize past the heap's limit on unreserved growth
(`max_unreserved_size`) panics on the first model big enough.

**A load, mint or convert arm that holds a whole model, a whole plane family or a whole image in
RAM where the streaming rail (`load_gguf_streaming` + `save_model_image_streaming`, the vulkan mint
off the gguf included) serves the same artifact is a defect, and so is a refusal that routes a
flavor or a carrier around that rail to an eager arm.** A flavor or a carrier the rail does not
cover gets the rail extended; `utils/dasllama-convert` takes the live load's rail, never one of
its own. A plane whose byte count scales with the model and that the load reads whole off the
gguf without a `FillJob` is the same defect.

**A diff that adds a lasting allocation the path never made before, or adds a term to one's size,
that grows with a scaling count states that size in bytes in a `PERF_LEDGER.md` row: at the largest
shape the code path accepts, or, where the path accepts any value of the count, as a formula in the
count with its value at two shapes that differ in it.**

**A diff that changes the size formula of a lasting allocation that exists before and after it
without adding a term - a count that starts or stops scaling it, a bound that becomes piecewise -
records in a `PERF_LEDGER.md` row the peak footprint and wall-clock, each measured before and
after the diff at a shape both the old and the new code accept where the two formulas differ, the
row naming that shape; where the formulas differ only at shapes the old code refuses, the row
records the new code's peak footprint and wall-clock at the smallest such shape and names the old
code's refusal in place of the before figure.** The pair records what the change costs; it sets no
pass/fail threshold.
