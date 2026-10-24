# dasLLAMA GPU Model-Swap Discharge Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_GPU_ASYMMETRIES.md`, `ARCHITECTURE_GPU_TOWER_VULKAN.md`. Planned work:
`followup_metal.md`, `followup_vulkan.md`.

**A diff that adds to a Vulkan driver file (`dasllama/dasllama_*vulkan*.das`,
`dasllama/dasllama_gpu*.das`) a module-level variable whose value derives from a loaded model's
weights, or names, sizes or points at device content built from a loaded model (a served model, a
draft or tower model, a TTS model or a voice loaded into one), and which a later call reads before
that call writes it - a log of the decline reasons recorded for that model included - returns it to
its no-model value in the body of `moe_gpu_model_marks_restore_` or `moe_gpu_drop_model_`
(`dasllama/dasllama_gpu_resident.das`), in a function either one calls, or in a listener the file
registers with `register_vk_drop_hook` (`dasllama/dasllama_vulkan_common.das`), in the same change.**
A global with no discharge survives a model swap and routes the next model's dispatches at the old
model's device buffers; a model drop calls every listener registered with `register_vk_drop_hook`.

**A diff that adds to a Metal driver file (`dasllama/dasllama_metal*.das`, the tower driver
included) a module-level variable whose value derives from a loaded model's weights, or names, sizes
or points at device content built from a loaded model, and which a later call reads before that call
writes it - a log of the decline reasons recorded for that model included - returns it to its
no-model value in a function the file registers with `register_reload_prep`
(`dasllama/dasllama_metal_common.das`), in the same change.** A model's unload or reload bumps the
weights epoch - a counter every model load or unload increments - and the bump runs every
registered reload prep.

**A diff that adds to a CUDA driver file (`dasllama/dasllama_*cuda*.das`) a module-level variable
whose value derives from a loaded model's weights, or names, sizes or points at device content built
from a loaded model, and which a later call reads before that call writes it - a log of the decline
reasons recorded for that model included - returns it to its no-model value in `cud_drop_model`
(`dasllama/dasllama_cuda_decode.das`) - in its body or a function it calls - in the same change.**
The tier's model drop runs `cud_drop_model` through the hook `dasllama/dasllama_math_cuda.das`
installs with `set_moe_gpu_drop_model_hook`.

**A module-level variable in a Vulkan driver file (`dasllama/dasllama_*vulkan*.das`,
`dasllama/dasllama_gpu*.das`) whose value derives from, or names, sizes or points at device content
built from, a TTS model - any model that turns text into audio - or a voice loaded into one, and
which a later call reads before that call writes it, also discharges in a listener the file
registers with `register_weights_epoch_listener` (`dasllama/dasllama_common.das`), in the same
change that adds it.** A TTS model's reload runs no Vulkan model drop, only `bump_weights_epoch`,
which runs every listener registered with `register_weights_epoch_listener`.

**A diff that changes or removes a function that returns a model-derived module-level variable of a
GPU driver file to its no-model value - or the registration that makes a model unload or reload run
it - leaves every such variable that function reset still returned to its no-model value by that
model's unload or reload, in the same change.** A reset dropped from one path is a discharge the
next model swap no longer runs.
