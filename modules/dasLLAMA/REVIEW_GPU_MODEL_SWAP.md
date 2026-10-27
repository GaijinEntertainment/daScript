# dasLLAMA GPU Model-Swap Discharge Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_GPU_ASYMMETRIES.md`, `ARCHITECTURE_GPU_TOWER_VULKAN.md`. Planned work:
`followup_metal.md`, `followup_vulkan.md`.

**A diff that adds to a GPU driver file a module-level variable whose value derives from a loaded
model's weights, or names, sizes or points at device content built from that model, or measures how
a call served that model (a rate later calls size work by) - the decline reasons already logged for
it included - and which a later call reads before that call writes it, returns it to its no-model
value in the same change, in its backend's discharge: a Vulkan file
(`dasllama/dasllama_*vulkan*.das`, `dasllama/dasllama_gpu*.das`) in `moe_gpu_model_marks_restore_`
or `moe_gpu_drop_model_` (`dasllama/dasllama_gpu_resident.das`), a function either calls, or a
`register_vk_drop_hook` listener; a Metal file (`dasllama/dasllama_metal*.das`) in a function the
file registers with `register_reload_prep` (`dasllama/dasllama_metal_common.das`); a CUDA file
(`dasllama/dasllama_*cuda*.das`) in `cud_drop_model` or a function it calls.** These sites are what
each backend's unload or reload runs; a global reset elsewhere survives the swap.

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
