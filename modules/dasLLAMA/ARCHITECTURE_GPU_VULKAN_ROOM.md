# dasLLAMA Architecture - the Vulkan tier's room on the card

Companion to `ARCHITECTURE_GPU_VULKAN_RESIDENCY.md`; a section is cited by its anchor. This document carries
what the residency plan of `ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#resident-plan` may fill on a card that
shares itself with a desktop: the room the OS reports and the headroom where none does, the two guards
that stand whatever the plan decided, and the two allocation shapes the Windows memory manager pages out
from under a running model - the priority tag and the mapped memory type - both opt-ins, and the
per-call buffers the mapped type never serves. Every rate in
this document is a `benchmarks/lcpp_bench.das` reading (`-jit --for-debug-purposes`, `DASLLAMA_IMAGE=0`,
`DASLLAMA_GPU=1`, 16 threads) on the zen2 box's RTX 5060 Ti 16 GB, driver 616.56.

### The plan's room: the OS's answer, the headroom, the pageable sets {#vk-plan-room}

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
contract, and the plan fills it. The rates here are tg128 readings (`-r 5 -p 512 -n 128`).

**Where no OS answers, the auto arm's headroom is the larger of 2 GiB and 27% of the tier's
cap on Windows, and the 2 GiB floor alone elsewhere (`plan_headroom`).** On Windows that share
keeps a 16 GB card's plan near 10.7 GB and leaves an 8 GB card's plan where the fixed term
already put it; it declines every 27B file on a 16 GB card, and the ladder behind it (the 9B
hybrid at 6.8, 3.4 and 49.8 tok/s across 12.5, 12 and 11 GB plans) was taken with every
allocation tagged priority 1.0, the arm that made the planes pageable. The OS query is the
Windows memory manager's, so on Linux no OS ever answers; its Mesa drivers fill `heapBudget`
from the kernel's system-wide accounting and the NVIDIA driver fails an allocation past the
card out loud, so there is no demotion knee for the share to guard, and the share alone would
decline the 35B-A3B hybrid on a 16 GB card whose floor arms it at a reduced context. A decline
taken with no OS answer names the headroom it stood on and `DASLLAMA_GPU_VRAM_MB` as the pin
that reads no room.

**The room is one heap on a discrete card and every heap on an integrated device.** A discrete
card's host-visible heaps are the host's memory across the bus, so `heap_room` takes the one
device-local heap with the largest live budget (the largest size where `VK_EXT_memory_budget` is
absent). An integrated device's heaps - a device-local carve-out and a host heap - are one DRAM
the device reads at one bandwidth, so the room is their sum, and a device buffer the device-local heap
cannot take falls to a host-visible, host-coherent type on a heap that is not device-local
(`uma_spill_pick` from `make_device_buf`, the spill said once) - a device-local type that is also
host-visible sits on the heap that just refused. The device-init line prints the room taken: on the zen4
box's Radeon 760M (RADV, subgroup 32 pinned), `benchmarks/lcpp_bench.das --for-debug-purposes -p 512 -n 128`
on `Qwen3.6-35B-A3B-MTP-UD-Q4_K_M.gguf` under `DASLLAMA_GPU=1 DASLLAMA_IMAGE=0` (`-jit`, the box's mint,
`DAS_TUNE_POLICY` and `DASLLAMA_COOPMAT` unset) read `device heap budget 22459 MB, weight cap 20376 MB` off
the one device-local heap, its plan `weights 21615 MB ... does not fit` and the window profile `31 host
layers left out of the role table` - 31 routed layers' experts summed on the CPU; over every heap the same
run reads `device heap budget 33689 MB (used 0) over every heap of the integrated device, weight cap 31638
MB` and the model loads whole.

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

**Long-lived planes sit in unmapped device-local memory; the ReBAR type is an opt-in
(`DASLLAMA_VK_REBAR=1`).** A plane allocated from the host-visible device-local type is the
other pageable set on the NVIDIA WDDM driver: a CPU write into it lands in a host-side shadow
the driver migrates on the device's first touch, and under pressure the manager parks the
shadow in system memory and leaves it there, with no error. The pressure arrives with the
host-experts form, whose pinned expert mirrors the adapter maps beside the planes: gemma-4
26B-A4B Q8_0 at a 12.2 GB plan (22 host layers, 17 GB of host buffers mapped) held 7.9 GB
dedicated with 4.3 GB of ReBAR-typed planes demoted and decoded at 8.1 tok/s (`debug-jit`, the ReBAR entry in `PERF_LEDGER.md`), a 95 ms wait a
token on an idle device; the same plan on plain device-local memory held every plane and
decoded at 32.7 (pp64 141 against 243), and the 35B-A3B UD-IQ3_XXS at a 12.8 GB plan went
the same way (4.1 GB demoted, 38.4 against 71.1; pp64 150 against 473). The UD-IQ4_XS and
UD-Q4_K_M files of the gemma went either side of the knee at the same plan room, so the plan
cannot size around it. Where nothing demotes the two rails decode alike (Qwen3.6-35B-A3B-MTP
UD-Q4_K_M, 39 host layers and a 34-slot pool: 53.4 against 54.1). The ReBAR write's only return
was the load's "direct" upload figure (9.0 GB in 0.9 s against 1.3 s staged), which is the
shadow's fill rate, not the link's; the staged rail's one exposure was a staging round trip on each
of the pool's host placements at a window's end, which the placement batch closes
(`ARCHITECTURE_GPU_VULKAN_POOL.md#pool-window-prefix`), and the DMA fill that lands a step's
misses touches neither. The
demotion warning (`vk_demotion_check`, the shared usage past the desktop's baseline and our
own host buffers) is the tell either way.

### Per-call buffers never take the mapped type {#vk-per-call-buffers}

**A per-call buffer is allocated from unmapped device-local memory whatever `DASLLAMA_VK_REBAR`
says; the mapped type serves long-lived planes alone.** A per-call buffer is one the host refills
on a later call after a device command has read it: the audio samples and mel rows of a tower's
front, the encoder rows a decoder reads, rope and position tables, decoder state seeds, the VAD's
input and carried state, the prefill window's rows, the seams' rows, the KV mirror slabs and the
remedy mirrors, the n-gram ring, the hot pool's and the stream slots' expert planes, and every TTS
row buffer. On the mapped type each such refill is a host write after the device's first touch,
and on the NVIDIA driver a write-migrate-write cycle of a shadowed allocation hangs the machine; the
type's one gain, the load's direct upload, a per-call buffer never collects.

The property sits on the allocation: `make_device_buf`'s `per_call` flag, spelled
`make_call_buf` / `try_call_buf` / `soft_call_buf` (`dasllama_vulkan_common.das`), the drivers'
`vt_call_buf` (tower), `wd_call_buf` and `rv_call_buf` (ASR decoders), and `make_stack_shell`'s
`per_call` for the pool and stream slots. Every `ts_rows_alloc` row buffer carries it, the TTS
slabs included: `ts_upload` refills rows of any kind, so the record cannot tell a slab from a slot.

`REVIEW.das`'s `check_per_call_buffers` holds it over `dasllama_vulkan_*.das`: a host upload
(`upload_region_at`, `upload_region`) is a per-call fill, and every allocation its target buffer
field can come from - followed through field aliases, `TsRows(dev = s.devs[slot])` and
`k_slabs |> push(mir.a)` alike, matched by field name across the files - carries a `*_call_buf`
allocator. The check licenses three shapes: an upload in a `[cold_path]` function (a load-time
fill), an upload in the function that allocates the buffer (the fill lands before any device
touch), and an upload into a parameter of the uploading function (a wrapper; its callers'
uploads are the ones checked). A per-call refill on a cold leg - the n-gram ring's `up`, the
remedy hydrate, the hot pool's placement, the TTS voice slot - is outside its reach and carries
the property by the allocation sites above.
