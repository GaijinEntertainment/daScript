# dasLLAMA GPU Resident-Session Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_GPU.md`, `ARCHITECTURE_GPU_MTP.md`, `ARCHITECTURE_GPU_VULKAN_NROW.md`. Planned work:
`followup_metal.md`, `followup_vulkan.md`, `followup_general.md` (the CUDA rows).

**A diff never hands a whole GPU decode or prefill call that the device served before the diff to
the CPU path - the shape it cannot serve on the device declines by name instead.** A call that
runs on the CPU gives the user who selected the GPU a fraction of its speed.

**A diff that lands or changes a model family, a backend arm or a session shape so that, for any
model it serves, its batched decode step reads a weight plane once per row instead of once for all
the step's rows, ships that plane's batched read - one read serving every row of the step - in the
same change, or the arm declines that model with a decline naming the layer kind or model feature
it does not serve.** The weight stream is what the batch amortizes.

**A diff that adds such a per-row decline names the declined layer kind in the backend's
architecture doc in the same change - `ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command` for
Vulkan, `ARCHITECTURE_GPU_MTP.md#batch-same-slab` for Metal, the `dasllama_cuda_decode` entry of the
`dasllama_<gpu>_decode` row of `ARCHITECTURE_GPU.md#gpu-backends` for CUDA.**

**A diff in `dasllama/` that calls `create_device_session` or `create_device_session_` for a
session holding its own region, or turns a scheduler's device mode on (`set_device_kv`),
guards the call in code with a check that the live device-home sessions - a device-home session
keeps its K/V only in a device K/V region and has no host cache - stay within
`gpu_device_sessions()`, a scheduler in device mode counting as `max_streams` device-home
sessions, and declines the call when they would not.** The driver panics on a device-home session
that finds no region.

**A diff in `dasllama/` that creates device-home sessions the driver seats back to back in one
scratch region guards their creation in code with a check that one region no live device-home
session holds is free (`gpu_free_regions()` or `gpu_free_regions_()` above zero), and creates
host-cached sessions when none is.** The driver panics on a device-home session that finds no region.

**A diff that calls a listed setter calls it before the load it governs - the `set_gpu_*` and
`reset_gpu_*` functions of `dasllama/dasllama_core.das`, `set_pf_window_rows_pin`,
`set_expert_cache_mb`, `set_gpu_want_pf_stream`, `set_cuda_mmq_pin`, `set_resident_upload_fault`,
`set_moe_gpu_streams`, `vk_rdec_chain_mode_set`, `cud_rdec_chain_mode_set`,
`install_moe_gpu_resident_site_feeds`, `install_moe_gpu_resident_kv_ring` and
`install_moe_gpu_resident_kv_share`; this list is the trigger.** The load reads these values once,
to decide what the whole-model GPU driver holds on the device; a value set after the load never
reaches that decision.

**A diff that defines a setter - a function whose stored value the next load reads to plan what
the device holds, a `set_*` knob or an `install_*` declaration alike - adds its name to the list
that opens "the `set_gpu_*` and `reset_gpu_*` functions of `dasllama/dasllama_core.das`", in the
same change.** The list is the trigger a reviewer reads; an unlisted setter reaches no call-order
check.

**A diff that adds or rewords an `RdecPass` value's sentence in `rdec_pass_words` writes it in
plain words a reader who never saw the engine understands, naming no part of the engine.** A
status page prints those words to a reader who never saw the engine.

**Weakening `REVIEW.das`'s `check_rdec_pass_words` is a defect.** That check requires every
`RdecPass` value to have its own sentence in `rdec_pass_words`.

**A diff that drops a model (`moe_gpu_drop_model`) a device-mode scheduler serves turns that
scheduler's device mode off (`set_device_kv(sch, false)`) before its next step, in the same
change.** The drop releases the regions, and a scheduler left in device mode admits the next
stream as device-home onto none.

**A change to the bake-trim path in `dasllama/dasllama_gpu_resident.das` (`trim_model_planes`)
names in the PR body a `dasllama-convert --trim` bake plus a serve of the trimmed image, on one q8
model, one K-quant model, and one model of a format outside both, for each of the three the trim
path bakes.** Parity runs never reach it.

**An f16 store into any device buffer holding K or V rows that does not clamp to the f16 finite
range (+/-65504) is a defect.**

**Never gather, store or release K/V rows a layer borrows from another layer - a shared-KV layer
reading its donor layer's rows, held in a per-layer device K/V panel the prefill GEMMs write or in
the donor's slab of the Vulkan resident mirror - through the borrowing layer; do it only through
the donor.** A borrower that gathers, stores or releases them again frees the donor's storage
twice or overwrites its rows.

**A diff that adds or changes a resident override - a decode/prefill hook
`dasllama/dasllama_gpu_resident.das` registers in `dasllama/dasllama_common.das`'s override
registries - makes it check that the session's K/V rows use the codec the mirror is armed with (the
row encoding - f16, q8_0, tq4 or f32 - the mirror was built in) before it touches the mirror (the
device copy of the K/V cache a GPU decode reads and writes).**

**A resident override that byte-copies mirror rows across codecs - the mirror being the device
copy of the K/V cache a GPU decode reads, the codec its rows' encoding - is a defect: bytes move
only between same-codec session rows and mirror rows.** A cross-codec copy corrupts the host's
authoritative cache.

**A diff that adds or changes a GPU path that runs a recurrent (deltanet) forward itself makes it
zero the deltanet state on a call at position zero, and decline every call at a nonzero position
other than the next position the state it advances expects - `Session.dn_pos` for the host
state, `DnMirror.pos` for a device mirror.** It runs the whole forward itself, so the engine's
own forward-only guard never runs.
