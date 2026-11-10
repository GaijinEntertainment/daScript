# dasLLAMA tests - Lane Pins Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `../ARCHITECTURE.md`. Planned work: `../followup_general.md`, `../followup_vulkan.md`,
`../followup_metal.md`.

- A cell is one `t |> run` subtest, or a `[test]` function that runs no subtest, and owns the
  asserts of every helper it calls.
- A family is one model architecture's loader and kernels (`../dasllama/dasllama_<family>.das`).
- A family's serving lane is the weight form it runs a model's weights in: q8 planes, a quantized
  carrier's own planes (kq), or the exact lane, whose form - f16 or f32 - the `.dlim` flavor baked
  for the serving GPU home (Vulkan, CUDA or Metal) picks.
- The blob form is a model after `convert_model_to_metal_blob`, the only form the Metal drivers
  serve.
- A lane setter is a call that pins a family's serving lane - `set_<name>_q8`, the ASR facade's
  `set_asr_fp32` / `set_asr_tower_fp32` / `set_asr_tower_q8` - or a facade call that makes that
  call; a call that sets a knob a family's `*_serves_q8` accessor reads when no lane is pinned
  (`set_gpu_gigaam_exact_lane`) is a driver setter.
- A driver setter is any other `../dasllama/` call the cell makes whose purpose is to set a value
  - a `set_*`, `enable_*`, `disable_*`, `pin_*`, `reset_*`, `restore_*`, `register_*`,
  `select_*_override` or `unset_*` call - stored in a module global or process-global state that a
  later load, route choice or kernel dispatch reads; a call a loader makes on its own is not the
  cell's.
- A route-deciding setter is a driver setter whose last-written value decides which route or
  kernel (which branch of a dispatch) an asserted call takes, whether an assert the cell makes
  passes, or what an assert's expected value is computed from. The GPU home is not one: the home the
  run arms - none, Vulkan under `DASLLAMA_GPU=1`, CUDA where the run also asks for it
  (`DASLLAMA_CUDA=1`, read through `_gpu_knobs.das`'s `cuda_tower_asked`), or Metal on a build that
  carries das_metal - is the run's choice, and a cell reads it rather than setting it. Every other knob's
  environment-read default is a value a route-deciding setter writes.
- A driver setter's getter is the `../dasllama/` call, or the set of them, that returns the
  values the driver setter last wrote - the driver setter itself among them when it returns the
  value its call replaced.
- A lane pin is a lane setter, a `set_metal_tensor_crowns` / `pin_metal_tensor_crowns` call, or
  the CPU kernel backend's pin (`pin_kernel_backend`, `DASLLAMA_PIN_BACKEND`) - a call or knob that
  picks the weight form, the Metal tensor crowns or the CPU kernel backend a load serves on.

**A diff that adds or edits a cell or `[test]` function, or code one reaches through a helper,
makes every cell that reaches the edited code call every route-deciding setter BEFORE the first
call or assert its value decides, even when the value it writes is the DEFAULT - in the cell or
the `[test]` function that runs it, directly or through a helper one of them calls.**

**A cell whose claim depends on a family's serving lane pins that lane in the cell itself - through
a lane setter or a loader parameter that takes the lane - never in the file's `[init]`.** A cell
that counts on the box declining the other lane, instead of pinning, measures whichever lane the
box's policy picked.

**A cell that reads a family's serving lane - a load, or a `*_serves_q8` call - and expects the
unpinned default calls, before the first such read, the unset call of every lane setter that read
consults - `reset_<name>_q8` for `set_<name>_q8` and, for a facade setter that pins several
families (`set_asr_tower_q8`), each pinned family's `reset_<name>_q8`, and `set_asr_fp32(false)` /
`set_asr_tower_fp32(false)` for a load through the ASR facade (`load_asr_model`).**
A pin an earlier cell leaked moves the accessor's answer and the load alike.

**A diff that adds or edits a cell or `[test]` function, or code one reaches through a helper, that
calls a lane setter - directly or through a helper it calls - makes each cell that reaches the call
return with each lane it pinned unset through the lane setter's paired unset call and, for a facade
setter that pins several families (`set_asr_tower_q8`), each pinned family's `reset_<name>_q8`.**

**A diff that adds or edits a cell or `[test]` function, or code one reaches through a helper, that
calls a driver setter other than a `register_*` call - directly or through a helper it calls -
makes each cell that reaches the call return with that setter back at the value its getter
returned before the first call, never at a literal.**

**A diff that adds or edits a cell or `[test]` function, or code one reaches through a helper, that
calls a `register_*` - directly or through a helper of this folder - makes each cell that reaches
the call return with that name's entry as the cell found it: an entry the call created removed by
the `../dasllama/` call that removes that name's entry (added in the same change where none
exists), an entry it overwrote put back.** A `register_*` inside a `../dasllama/` setter's own
body is put back by restoring that setter to its getter's earlier value.

**A diff that adds or edits a cell, or code one reaches through a helper, that writes through a
driver setter that is not a `register_*` call and has no getter, or whose getter cannot return the
value written, adds in the same change a getter in `../dasllama/` that returns it.**

**A cell asserting the unpinned default lane of a family that has a `*_serves_q8` accessor
compares the accessor's q8-or-not choice against the predicates that accessor reads for its
unpinned default (whatever its body calls), or against the accessor's own answer read before the
load, never against a hardcoded lane.** The default lane differs per box.

**A cell asserting a lane the loader picks from the carrier's content (the file's weight formats)
or from the GPU home's `.dlim` flavor (the exact lane's f16 or f32 form) asserts it by the loaded
model's own lane field - the tag or device-block field the load set - never against a hardcoded
lane.**

**A cell of `test_model_image*.das` that loads a model to test a lane pin's effect on the image
identity loads through `load_model_image` (`../dasllama/dasllama_image.das`), the loader that bakes
the `.dlim`, never through a staging path around it.** The pin is part of what the image identity
records.

**A CPU-vs-GPU cell on Metal that compares the outputs of a decode or prefill override - the Metal
driver hook that runs the model's decode or prefill stages on the GPU - against the CPU's runs its
CPU stages on a PLANAR model (the non-blob form, the only one CPU inference reads) and the
override's stages on that model's blob twin (`blob_twin(t, path, seq_cap)`,
`_metal_blob_twin.das`), both over one `Session`.** The planar model and its blob twin share one
shape, so one `Session` serves both.
