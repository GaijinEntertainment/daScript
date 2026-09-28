# dasLLAMA tests - Lane Pins Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `../ARCHITECTURE_RUNTIME.md`, `../ARCHITECTURE_MEDIA.md`. Planned work:
`../followup_general.md`, `../followup_vulkan.md`, `../followup_metal.md`.

A cell is one `t |> run` subtest, or a `[test]` function that runs no subtest; a helper's
asserts belong to every cell that calls it. A lane setter is a call whose value a family's
loader reads to pick its lane, or a facade call that makes that call. A driver setter is any
other `../dasllama/` call that writes process-global state a later load, route choice or kernel
dispatch reads. A driver setter's getter is a `../dasllama/` call that returns exactly the value
the driver setter last wrote.

**A cell, or the `[init]` of the file where the cell is defined, sets every driver setter whose
value the cell's claim depends on, even when that value is its DEFAULT.**

**A cell whose claim depends on a family serving lane pins that lane in the cell itself - through
a lane setter (`set_<name>_q8`, or whisper's `set_asr_fp32` / `set_asr_tower_fp32`) or a loader
parameter that takes the lane - never in the file's `[init]`.** A cell that counts on the box
declining the other lane, instead of pinning, measures whichever lane the box's policy picked.

**A cell whose claim needs a family serving lane unset first calls that lane's unset call -
`reset_<name>_q8` for a lane `set_<name>_q8` pins, and whisper's `set_asr_fp32(false)` and
`set_asr_tower_fp32(false)`.**

**A cell that sets a lane setter or a driver setter - directly, through a helper it calls, or
through a loader parameter that takes the lane - returns with each lane it pinned unset through
the lane setter's paired unset call, and each driver setter it set back at the value that
setter's getter returned before the cell set it.**

**A diff that adds or edits a cell setting a driver setter that has no getter adds that getter
in `../dasllama/`, in the same change.**

**A cell asserting the unpinned default lane compares against the predicates the family's
`*_serves_q8` accessor reads for its unpinned default (whatever its body calls), never against a
hardcoded lane.** The default lane differs per box.

**An image-suite cell whose subject IS the lane knob loads through the `.dlim`-baking loader,
never around it.** The pin is part of what the image identity records.

**A CPU-vs-GPU cell on Metal serving an LLM `Model` through a decode or prefill override - the
Metal driver hook that runs the model's decode or prefill stages on the GPU - one comparing the
two routes' outputs, not one whose subject is the GPU route's decline, runs its CPU stages on a
PLANAR model (the non-blob form, the only one CPU inference reads) and the stages the override
selects on that model's blob twin (`blob_twin(t, path, seq_cap)`, `_metal_blob_twin.das`), in one
session.** The planar model and its blob twin share one shape, so one session serves both.
