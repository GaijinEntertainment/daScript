# dasLLAMA Architecture - the Vulkan tier's model residency

Companion to `ARCHITECTURE_GPU_VULKAN.md`; section numbers are `ARCHITECTURE.md`'s. This
document carries sections 2.2n-2.2o: the residency plan that sizes a whole model before a byte
uploads, and the marks swap that lets one GPU slot serve many models. The prefill chain and byte
stores that run once a model is resident are `ARCHITECTURE_GPU_VULKAN.md` sections 2.2j, 2.2p,
2.2ab, 2.2ac and 2.2ad, and the cooperative-matrix GEMM tiles under them are
`ARCHITECTURE_GPU_VULKAN_GEMM.md` sections 2.2k-2.2m, 2.2q and 2.2ae; the per-op tier's decode era is
`ARCHITECTURE_GPU_VULKAN_DECODE.md` sections 2.2r-2.2v. The GPU backend role table these
sections build on stays in `ARCHITECTURE_GPU.md` sec.1.5.

### 2.2n The residency plan sizes a whole model before a byte uploads {#resident-plan}

The resident driver is all-or-nothing, so the plan IS the decision, and it is computed from
`Model` metadata alone. It sizes four numbers against the tier's weight budget: the dense weight
planes, the KV mirror at `seq_cap`, the driver's own device scratch, and the headroom the auto
arm leaves unfilled (zero when the user pins VRAM). KV is reserved BEFORE weights and never
grows: on a discrete card the two compete directly, and evicting weights to grow KV would mean
re-uploading gigabytes. A decline carries a reason, and where the numbers allow one it carries
the remedy that works - a shorter context, because the weights are fixed and the KV is not.

**The mirror's context is capped by the device's single-binding range before any byte is
counted.** Each K/V side binds as one SSBO range, so `seq_cap` is at most `maxStorageBufferRange`
over the side's bytes per position summed across the layers (`resident_binding_ctx`). The sum is
what makes a hybrid right: a recurrent layer's KV width is zero, so the stride counts the
attention layers alone; read from layer 0 instead - recurrent on Qwen3.5 - the stride is zero,
the cap is skipped, the plan sizes a mirror the device prepare refuses, and the whole driver
declines to the per-op rails with the dense FFN on the CPU. The prepare keeps its own guard for
direct callers.

**The auto arm sizes against the room the OS reports on the adapter, where the OS reports
it.** Vulkan cannot see the desktop: `VK_EXT_memory_budget` reads a flat 16024 MB on the 16 GB
reference card with 3 GiB of another process's memory resident, and so does the process's own
WDDM budget (`D3DKMTQueryVideoMemoryInfo`) - on the NVIDIA driver that number is the card minus
the OS reserve, whatever anyone else holds. What does carry the desktop is the adapter's
system-wide dedicated usage, the "GPU Adapter Memory / Dedicated Usage" performance counter
summed over the adapter's segments. The boost helper `os_video_memory` returns all three -
the ceiling, this process's usage, the adapter's usage - for the physical device's LUID, and
the tier installs it as the resident plan's `moe_gpu_os_memory` source. The plan's usable
bytes are then the tier's cap, or the ceiling minus what every other process holds minus a
1 GiB reserve, whichever is smaller; the headroom share is zero. The reserve is not slack: the
process's dedicated footprint runs about 0.7 GB past the plan's own tally (arena slab tails,
descriptor pools, pipelines, the transfer queue's staging - a 14.0 GB plan held 14.66 GB), and
a decline names what the other processes hold. Measured untagged on that card with a 0.63 GB desktop: plans of 13.35, 14.0 and 14.6 GB
(the card at 14.7, 15.3 and 15.9 GB dedicated) all decoded the 27B at full rate; with 2 GiB
more held by another process the same 13.35 GB plan lost 1.7 GB to system memory and decoded
at 4.35 tok/s, and with 4 GiB more the device returned all-zero logits with no error. The
manager never demotes the idle other process; ours is the victim, so the room left is the only
honest input. A pinned `VRAM_MB` takes no headroom and reads no room: the number is the user's
contract, and the plan fills it. Every rate in this section is a `benchmarks/lcpp_bench.das`
tg128 reading (`-jit --for-debug-purposes -r 5 -p 512 -n 128`, `DASLLAMA_IMAGE=0`,
`DASLLAMA_GPU=1`, 16 threads) on the zen2 box's RTX 5060 Ti 16 GB, driver 616.56.

**Where no OS answers, the auto arm's headroom is the larger of 2 GiB and 27% of the tier's
cap.** That share keeps a 16 GB card's plan near 10.7 GB and leaves an 8 GB card's plan where
the fixed term already put it; it declines every 27B file on a 16 GB card, and the ladder
behind it (the 9B hybrid at 6.8, 3.4 and 49.8 tok/s across 12.5, 12 and 11 GB plans) was taken
with every allocation tagged priority 1.0, the arm that made the planes pageable. Linux's Mesa
drivers fill `heapBudget` from the kernel's system-wide accounting and the NVIDIA driver fails
an allocation past the card out loud, so the share is a coarse stand-in there, not a measured
knee.

**Two guards stand whatever the plan decided.** The resident decode panics on an all-zero
logits row (the first four tokens and every 256th are scanned) naming the over-commit, and at
its eighth token it reads the demotion tell and measures the token's wall against the weights
streamed at 40 GB/s, the PCIe class; either past its line logs the remedy once. Every resident
prefill reads the tell the same way. The tell is the adapter's shared usage - system memory the
GPU maps - grown past the desktop's baseline at tier init plus every live host-visible buffer
of ours (`make_host_buf`'s tally) by more than 512 MB: a demoted plane lands exactly there
(the 27B beside a 2 GiB hog read 3.07 GB shared against 0.67 GB of its own host buffers), while
the dedicated-usage counters can sit a few MB over the soft ceiling with nothing demoted.

**Device allocations carry no memory-priority tag.** The tier enables
`VK_EXT_memory_priority` and `VK_EXT_pageable_device_local_memory` on the device (the boost
creator does, whenever the card reports them) but chains no priority into an allocation
unless `DASLLAMA_VK_MEMPRIO=1` asks. On the NVIDIA WDDM driver the priority-1.0 tag is what
makes an allocation pageable: the 27B UD-IQ4_XS at a 13.35 GB plan on the 16 GB card held
12.26 GB dedicated with 1.7 GB demoted and decoded at 4.2 tok/s tagged, and held 14.66 GB
dedicated at 23.5 tok/s untagged, same run shape, same desktop (llama.cpp, which tags only
behind its own opt-in, sat at 13.7 GB dedicated and 24.1). The tag is an A/B arm.

An OPTIONAL plane rides only the room left under the budget at THIS context - what remains of
`budget_bytes - headroom_bytes` after weights, KV and scratch; the reserved headroom itself
stays unfilled. It never shrinks any of the three, and it reports zero bytes when it does not
fit - so the same model plans the plane in at a short context and out at a long one. The raw f32 embed
table is the one optional plane today.

### 2.2o One GPU slot, many models: the marks swap {#gpu-slot-marks}

A multi-model host runs one device tier under several loaded models, and the tier's per-model
state is offset-keyed - two models' marks installed together route one model's dispatches at
the other's planes. `GpuModelMarks` is that state WHOLE: the loader-contract marks plus every
resident-driver per-model global (the activation, the mirror count, the mirror cap, the mirror
codec, and the device-embed arm). The save moves the installed state out and leaves the globals
reading as no-model; the restore is its exact inverse. The whole-model drop clears the same set
and deselects the `"vulkan"` overrides, so a dropped model's prefill and decode take the plain
CPU path and a later re-arm passes `resident_upload`'s no-active-override gate. The three carry
the same set, which is why a model's device state never survives into the next. The upload
rail enforces it from its own side: a load that finds marks still installed - a model deleted
without the drop, the shape every test process and single-model tool takes - drops that
model's device state before uploading its own, carrying the load's MoE layer request across
the drop (the one mark the drop's reset would otherwise zero before the rail reads it).
Without that drop the second model's stacks
land beside the first's, and the offset-keyed stack lookup serves whichever model's plane
registered that offset first: the decode attention block asserts on the geometry change, and a
model whose geometry matches decodes the earlier model's weights.
