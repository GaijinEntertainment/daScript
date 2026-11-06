# dasLLAMA Architecture - the Metal family's device lifecycle

Companion to `ARCHITECTURE_GPU.md`; a section is cited by its anchor. This document carries the Metal family's shared
device and queue with its PSO sets, its device memory, its race code and its decline reasons. The GPU backend role
table this section builds on stays in `ARCHITECTURE_GPU.md#gpu-backends`.

### The Metal family's device lifecycle {#metal-family-lifecycle}

**PSO lifecycle - the family shares ONE device and queue** (`metal_common_init`; the tune-time
race arms' transient queue is `ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md#kernel-race-fidelity`'s). The decode
PSO set lives as `g_pso_*` in `dasllama_metal_common`, is compiled by `metal_decode_init` in
`dasllama_metal_kernels` and released by `metal_kernels_release` there - the kernels module owns
its set's lifecycle even though the vars live with the device state. Prefill's `g_pf_pso_*` set
is prefill-private end to end: `metal_prefill_init` compiles, `metal_prefill_shutdown` releases.

**Metal device memory is the drivers', never the model's.** A model's weight regions, whole-plane buffers and concat
regions sit in `dasllama_metal_common`'s address-keyed tables, and a `Model` holds no Metal handle, so `delete` of a model
frees its host planes and leaves its regions cached until the weights epoch moves (the next load drops them,
`ARCHITECTURE_RUNTIME.md#weights-epoch-on-load`) or the drivers shut down. The KV mirror arena (grown up to its ceiling,
`DASLLAMA_METAL_KV_MIRROR_MB`), the deltanet mirror pool and the buffer pools live until `metal_decode_shutdown` /
`metal_prefill_shutdown`, bounded by the largest shape the process served; a boundary that must hand the device back calls
both, and a context's end calls both for it (`metal_context_shutdown`, a `[finalize]`, where the device came up; the
GEMM driver's `metal_gemm_context_shutdown` is its twin for that driver's own device, pipelines and caches): device
state a context left alive would outlive it in the process - dastest's `--together` runs a file a context, a host may hold
several - and the next context's leak gate would count it. A released buffer leaves the process footprint because the binding reads `contents` inside an autorelease pool
(`../dasMetal/ARCHITECTURE.md`), and the footprint (`metal_footprint_settled`), not the live-object count, witnesses it -
the support matrix's release row (`metal_release_row`) holds three load-serve-delete cycles of E2B to their first close.

**Race and tune code for a kernel family lives beside the family.** The shared scaffolding
(`race_buf`, `race_envelope_ok`, `race_pair_ms`, `MetalTensorRaceResult`) is `<gpu>_common`'s;
each module races its OWN families (`metal_tensor_race_decode` in kernels, `metal_tensor_race`
in prefill) and the tuner calls those public entries.

**Decline REASONS are enum values in the shapes module** (`MetalDecodeDecline`, `MetalPrefillDecline`);
decline COUNTING lives in `<gpu>_common` beside `require_or_panic`, for both paths. A reason a plane format decided
(`kquant_native`; `graph` on a routed stack, a shared expert or a deltanet plane) keeps its enum value in the
declines-by-reason table; the shapes module's `metal_fmt_decline_say` logs the site and `KqFmt` member beside it once
per line and hands the same words to the decline, whose required-mode panic prints them in place of the bare reason.
Each driver has one decline enum: Metal's live in `dasllama/dasllama_metal_shapes.das`, and a Vulkan driver's lives in
that driver's own file.
