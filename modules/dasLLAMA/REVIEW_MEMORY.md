# dasLLAMA Memory and Lane Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_ENGINE.md`, `ARCHITECTURE_ENGINE_MEMORY.md`, `ARCHITECTURE_INVARIANTS.md`. Planned work:
`followup_general.md`.

An allocation is one buffer, or one sub-range of a buffer shared by several uses; a buffer that
replaces buffers the diff removes counts as added. An allocation that a path, a model class or a
box configuration - a crown (a kernel family the box's tune sidecar enables) or a knob's default
(an environment variable's or a setting's) - reaches that it did not reach before counts as added;
an allocation the diff moves with its size and reach unchanged does not.

A lasting allocation is a container (array, table, device buffer) that outlives the call making
it - held by a module global, or by a field of a struct that outlives the call; a pooled buffer
acquired and released inside one call is not one.

A model-file count is a count set by a file the model load reads: the GGUF, the `.dlim` prepared
from it, or a data file in the GGUF's directory. A scaling count is a model-file count, the context
a session serves (the model's own or one a caller pins), how many tokens one step computes at once,
how many rows one media encode feeds (an image's patches, a clip's frames), or how many regions one
buffer is split into (the K/V cache's device copy, one region per request served at once; an MoE
dispatch's expert regions). A shape is one setting of the scaling counts. Peak footprint is the
process's largest resident memory over a run.

A discrete home is a GPU backend serving on a device whose memory is not the host's: a discrete card under Vulkan or
CUDA, or any backend added for one. Metal, and Vulkan on a device that shares the host's memory (an integrated GPU), are
not discrete homes. A host copy is the bytes of a weight plane, a K/V row or a token table in process memory - heap, or
the resident pages of a file mapping - while a discrete home holds the same data in any layout. A lever is an
environment variable or a setter that changes what a run loads, keeps or computes; a run with no lever set is the
default run. A re-arm is the load placing a loaded model's planes on the device again.

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
it, whose element count grows with a model-file count, and whose declaration - the
variable, parameter or struct field that the call in that function names as the array - is not
`@exact_size` (`@scratch @exact_size` when it is `@scratch`), is a defect.** PERF032 - the lint
that flags a `resize` with no reserve before it - checks a resize only when that declaration
carries `@exact_size`.

**A diff that adds or changes a `resize` in `dasllama/` that can grow a buffer whose element count
grows with a model-file count puts before it a `reserve(n)` or `ensure_capacity(n)` whose `n` is at
least the resized count, or routes it through a `dasllama/dasllama_math.das` helper whose own body
does that before it resizes (`reserve_resize`, `grow_resize`), or makes it the builtin
`scratch_resize` on a `@scratch` buffer.**
The lint never compares the two counts, and a resize past the heap's limit on unreserved growth
(`max_unreserved_size`) panics on the first model big enough.

**Weakening PERF032 is a defect.**

**A diff that adds, on a path a run with no lever set takes, a fill - zero or any other value - of a buffer whose size
grows with a scaling count, every byte of which a later write covers before any read, is a defect - size the buffer
with no fill (`resize_no_init`, `overwrite_resize`).** A fill makes every page of the buffer resident, whatever the run
then writes.

**A diff that adds a lasting allocation whose size grows with a scaling count names, in the PR description, the code
that reads its bytes on each backend the allocation reaches (CPU, Metal, Vulkan, CUDA); an allocation a backend reaches
with no reader there is a defect - make it only on the backends that read it.**

**On a discrete home, a host copy that outlives its upload, with no host code reading it on a prompt or a step the home
serves, is a defect - drop it once the upload ends.** A read made only by a test, or by a re-arm uploading again, does
not keep a copy.

**A diff that turns on, in code outside `tests/`, `harness/` and `benchmarks/`, a lever that keeps a host copy for a
test's compare (`set_host_planes`, `set_vulkan_trim(false)`) is a defect.**

**A diff that maps a source file for a load, a mint or a convert releases the pages each tensor read touched before the
next tensor read begins (a read nested inside another is part of it) - open a gguf through `gguf_source_begin`, which
does this; a mapping that holds its pages until it closes is a defect.** Every page a read touches stays resident until something releases it.

**A diff that changes what a load or a session holds on the host records in a `PERF_LEDGER.md` row the peak footprint
and the resident memory after the load, each before and after the diff, on each backend it touches, measured with no
lever set.** A figure taken under a lever shows a path the product does not take.

**A load, mint or convert arm that holds a whole model, a whole plane family or a whole image in
RAM, for an artifact the streaming rail (`load_gguf_streaming` + `save_model_image_streaming`, the
vulkan mint off the gguf included) serves, is a defect, and so is a refusal that routes a flavor or
a carrier the rail serves around it to an eager arm.**

**When the streaming rail serves an artifact, a load that reads whole off the gguf, without a
`FillJob`, a plane whose byte count grows with a model-file count and which the host drops after the
load is a defect.**

**A diff that adds a whole-RAM arm for a flavor or a carrier the streaming rail does not cover adds
a `followup_general.md` row to extend the rail to it, in the same change.**

**A `utils/dasllama-convert/` file loads and mints through the live load's streaming rail
(`load_gguf_streaming` + `save_model_image_streaming`), never through a rail of its own.**

**A diff that adds a lasting allocation the path never made before, or adds a term to one's size,
that grows with a scaling count states that size in bytes in a `PERF_LEDGER.md` row: at the largest
shape the code path accepts, or, where the path accepts any value of the count, as a formula in the
count with its value at two shapes that differ in it.**

**A diff that changes the size formula of a lasting allocation that exists before and after it,
without adding a term - a count that starts or stops scaling it, a bound that becomes piecewise -
so that the two formulas differ at a shape both the old and the new code accept, records in a
`PERF_LEDGER.md` row the peak footprint and wall-clock, each measured before and after the diff at
one such shape, the row naming it.** The pair records what the change costs; it sets no pass/fail
threshold.

**A diff that changes the size formula of a lasting allocation that exists before and after it,
without adding a term - a count that starts or stops scaling it, a bound that becomes piecewise -
so that the two formulas differ only at shapes the old code refuses, records in a `PERF_LEDGER.md`
row the new code's peak footprint and wall-clock at the smallest such shape, and names the old
code's refusal in place of the before figure.**
