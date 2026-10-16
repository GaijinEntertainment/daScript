# dasLLAMA GPU Resident-Session Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_GPU_MTP.md`, `ARCHITECTURE_GPU_VULKAN_NROW.md`. Planned work: `followup_metal.md`,
`followup_vulkan.md`. The parent `REVIEW_GPU.md` routes here.

**A change that can alter what a GPU decode or prefill call on a session computes or selects ships
GPU-vs-CPU parity on one q8 model, one K-quant model, and one model of a format outside both, for
each of the three whose path the change reaches - or names both compares in the PR body: its emitted
kernels byte-identical before and after (the `*_msl` globals or the AIR, Metal's compiled shader IR,
they build into; the SPIR-V words `DASLLAMA_VK_SPV_DUMP` writes) and the host's stamp, dispatch
selection and every bind argument (buffer, offset, kargs value) unchanged on every input.** Such a
change is anything the call executes or that selects what it executes - a driver, a kernel class it
dispatches, that class's builder, a servability gate, a race that picks which kernel serves, a
forwarder default, a weight-region or residency path, the tier forwarders and the Vulkan
tier-dispatch seams (`dasllama/dasllama_vulkan_seams.das`) the call routes through; only both
compares together show the change cannot alter what the path computes or selects.

**A diff that adds a call site handing a whole GPU decode or prefill call the device served before
the diff to the CPU path is a defect - it ships the device path in the same change.** A call that
runs on the CPU gives the user who selected the GPU a fraction of its speed.

**A diff that lands a model family, a backend arm or a session shape whose batched decode step
reads a weight plane once per row rather than once for all the step's rows, for any model it
serves, is a defect: it ships that plane's batched read - one read serving every row of the
step - in the same change, or the arm declines that model, its decline naming the layer kind or
model feature it does not serve, and that backend's architecture doc -
`ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command` for Vulkan,
`ARCHITECTURE_GPU_MTP.md#mtp-joint-verify` for Metal - gains the kinds of layer the batched step
does not serve.** The weight stream is what the batch amortizes.

**A diff in `dasllama/` that calls `create_device_session`, or turns a scheduler's device mode
on (`set_device_kv`), shows at that call site that the live device-home sessions - a device-home
session keeps its K/V only in a device K/V region and has no host cache - stay within
`gpu_device_sessions()`, a scheduler counting as its own `max_streams` of them.** The driver
panics on a device-home session that finds no region.

**A diff that calls a `set_gpu_*` or `reset_gpu_*` function of `dasllama/dasllama_core.das` that
forwards into `dasllama/dasllama_gpu_resident.das` calls it before the load it governs.** The load
reads these values once, to decide what the whole-model GPU driver holds on the device; a value set
after the load never reaches that decision.

**A diff that adds an `RdecPass` value gives it its own reader-facing sentence in
`rdec_pass_words`, in plain words that name no part of the engine - no pass, region, mirror,
resident driver, tier or rails - in the same change.** A status page prints those words; a
value with none prints its machine name instead.

**A diff that drops a model (`moe_gpu_drop_model`) a device-mode scheduler serves turns that
scheduler's device mode off (`set_device_kv(sch, false)`) before its next step, in the same
change.** The drop releases the regions, and a scheduler left in device mode admits the next
stream as device-home onto none.

**A change to the bake-trim path in `dasllama/dasllama_gpu_resident.das` (`trim_model_planes`)
ships a `dasllama-convert --trim` bake plus a serve of the trimmed image, on one q8 model, one
K-quant model, and one model of a format outside both, for each of the three the trim path
bakes.** Parity runs never reach it.

**An f16 store into any device buffer holding K or V rows that does not clamp to the f16 finite
range (+/-65504) is a defect.**

**Never gather, store or release K/V rows a layer borrows from another layer - a shared-KV layer
reading its donor layer's rows, held in a per-layer device K/V panel the prefill GEMMs write or in
the donor's slab of the Vulkan resident mirror - through the borrowing layer; do it only through
the donor.** A borrower that gathers, stores or releases them again frees the donor's storage
twice or overwrites its rows.

**A resident override that touches the mirror before gating the session on the armed mirror
codec is a defect** - a resident override is a
decode/prefill hook `dasllama/dasllama_gpu_resident.das` registers in
`dasllama/dasllama_common.das`'s override registries.

**An override that byte-copies mirror bytes across codecs is a defect - bytes move only between
same-codec session rows and mirror rows.** A cross-codec copy corrupts the host's authoritative
cache.

**A diff that adds or changes a GPU path that runs a recurrent (deltanet) forward itself makes it
zero the deltanet state on a call at position zero, and decline every call at a nonzero position
other than the next position the state it advances expects - `Session.dn_pos` for the host
state, `DnMirror.pos` for a device mirror.** It runs the whole forward itself, so the engine's
own forward-only guard never runs.
