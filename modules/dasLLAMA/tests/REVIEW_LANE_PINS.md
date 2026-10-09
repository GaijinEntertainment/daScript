# dasLLAMA tests - Lane Pins Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `CLAUDE.md`. Planned work: `../followup_general.md`, `../followup_vulkan.md`,
`../followup_metal.md`.

A cell is one `t |> run` subtest, or a `[test]` function that runs no subtest; a helper's
asserts belong to every cell that calls it. A family is one model architecture's loader and
kernels (`../dasllama/dasllama_<family>.das`); its serving lane is the weight form - q8 or float -
the family runs a model's weights in; the blob form is a model after
`convert_model_to_metal_blob`, the only form the Metal drivers serve. A lane setter is a call that
writes the value a family's loader or its `*_serves_q8` accessor reads to choose the family's
serving lane (`set_<name>_q8`, whisper's `set_asr_fp32` / `set_asr_tower_fp32`), or a facade call
that makes that call. A driver setter is any other `../dasllama/` call that writes process-global
state - state every context shares, not a module global, which each context holds its own copy
of - that a later load, route choice or kernel dispatch reads. A driver setter's getter is the
`../dasllama/` call, or the set of them, that returns the values the driver setter last wrote.

**A diff that adds or edits a cell makes the cell, the `[test]` function that runs it, or the
`[init]` of the file holding that function, call - directly or through a helper it calls - every
driver setter whose last-written value decides whether an assert the cell makes passes, or is what
an assert's expected value is computed from, BEFORE the first such assert, even when the value it
writes is the DEFAULT.**

**A cell whose claim depends on a family's serving lane pins that lane in the cell itself - through
a lane setter or a loader parameter that takes the lane - never in the file's `[init]`.** A cell
that counts on the box declining the other lane, instead of pinning, measures whichever lane the
box's policy picked.

**A cell whose claim needs a family's serving lane unset first calls that lane's unset call -
`reset_<name>_q8` for a lane `set_<name>_q8` pins, and whisper's `set_asr_fp32(false)` and
`set_asr_tower_fp32(false)`.**

**A diff that adds or edits a cell or `[test]` function that calls a lane setter - directly or
through a helper it calls - makes it return with each lane it pinned unset through the lane
setter's paired unset call.**

**A diff that adds or edits a cell or `[test]` function that calls a driver setter - directly or
through a helper it calls - makes it return with that setter back at the value its getter returned
before the first call, never at a literal.**

**A diff that adds or edits a cell that writes, through a driver setter, a value that setter's
getter cannot return adds a getter that returns it, in `../dasllama/`, in the same change.**

**A cell asserting the unpinned default lane of a family that has a `*_serves_q8` accessor
compares against the predicates that accessor reads for its unpinned default (whatever its body
calls), or against the accessor's own answer read before the load, never against a hardcoded
lane.** The default lane differs per box.

**A cell of `test_model_image*.das` whose subject is a lane pin's effect on the image identity
loads through `load_model_image` (`../dasllama/dasllama_image.das`), the loader that bakes the
`.dlim`, never through a staging path around it.** The pin is part of what the image identity
records.

**A CPU-vs-GPU cell on Metal that compares the outputs of a decode or prefill override - the Metal
driver hook that runs the model's decode or prefill stages on the GPU - against the CPU's runs its
CPU stages on a PLANAR model (the non-blob form, the only one CPU inference reads) and the
override's stages on that model's blob twin (`blob_twin(t, path, seq_cap)`,
`_metal_blob_twin.das`), both over one `Session`.** The planar model and its blob twin share one
shape, so one `Session` serves both.
