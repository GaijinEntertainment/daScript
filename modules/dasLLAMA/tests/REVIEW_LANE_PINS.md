# dasLLAMA tests - Lane Pins Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `CLAUDE.md`. Planned work: `../followup_general.md`, `../followup_vulkan.md`,
`../followup_metal.md`.

A cell is one `t |> run` subtest, or a `[test]` function that runs no subtest; a helper's
asserts belong to every cell that calls it.

**A cell, or the `[init]` of the file where the cell is defined, sets every driver setter - a
`set_*` / `pin_*` call in `../dasllama/` that changes the driver's route or the engage mode for
the rest of the process - whose value the cell's claim depends on, even when that value is its
DEFAULT.**

**A cell whose claim depends on a family serving lane pins that lane itself, never through the
file's `[init]`, through a lane setter - a call whose value the family's loader reads to pick its
lane, or a facade call that makes that call (`set_<family>_q8`; `set_styletts2_q8` for kitten and
kokoro; `set_tts_q8`, which pins every TTS family; `set_canary_enc_q8`; whisper's `set_asr_fp32` /
`set_asr_tower_fp32`) - or through a loader parameter that takes the lane; a claim that needs the
lane unset establishes it with the unset call paired with the setter it pins through -
`reset_<name>_q8` for a `set_<name>_q8` (`reset_tts_q8` for the TTS facade), whisper's
`set_asr_fp32(false)` and `set_asr_tower_fp32(false)`.** A cell that counts on the box declining
the other lane, instead of pinning, measures whichever lane the box's policy picked.

**A cell that sets a lane setter or a driver setter - directly, through a helper it calls, or
through a loader parameter that takes the lane - returns with every pin it set unset through its
paired unset call, and every setter it set back where it found it; where the driver exposes no
getter, back at the value the file's `[init]` sets, else at its default.** A pin left set makes
the next cell measure this cell's lane, not its own.

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
