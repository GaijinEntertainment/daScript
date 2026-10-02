# dasLLAMA Architecture - the prepared-image rail, the vulkan lane's mint

Companion to `ARCHITECTURE_IMAGE.md`; this document carries one section, `ARCHITECTURE_IMAGE_VULKAN.md#image-vulkan-mint`,
cited by its anchor.

### The vulkan lane is minted off the gguf by its own walk {#image-vulkan-mint}

A vulkan-armed cold load mints the vulkan lane and no other: the streaming save (`save_model_image_streaming`, its
in-memory twin under `DASLLAMA_IMAGE_SAVE=0`) runs the GPU walk at the first plane's turn, live, and every entry the walk
places is gathered off the gguf mapping, uploaded to the device and streamed to the writer as the plan records it
(`vulkan_mint_begin`, the sink's `VK_ENTRY_ALIGN` import windows), so on the file sink the device twin never sits whole in
RAM and no planar file precedes it; the in-memory sink holds the twin inside the chunk it serves from, as it holds every
plane. An entry's source is the run of streamed jobs that together hold its elements - one job, or the adjacent jobs a MoE
stack takes when the loader records it one expert at a time (`stream_job_span`) - transcoded into a view of the model's
planes whose offset 0 is the run's first `woff` (`stream_job_view`: the scale halves as the gathers read them, every job's
repack regions applied, the frozen K-quant interleave copied), and the arena placement's own gather runs over that view
(`layout_gather_dispatch`). The gather keeps the last run's view while the next entry sits in the same run
(`vk_mint_gather_hook`), so the pieces of one entry - a token table's rows - share one transcode. A model whose streaming
plan disarmed has no jobs and its planes whole in RAM, and the walk gathers them as the eager rail would; an entry no run
of recorded jobs holds is a shape the mint does not know: the hook feeds the walk a zero stand-in of the entry's size so
the walk runs to its end without a gather over a plane the stream never filled, records the entry, and the mint declines
after the walk - a panic there would take a server down at boot, where its loads run under no catch. The twin is the one
plane the sizing pass cannot know, since the walk sizes its entries as it places
them: `vk_mint_extra_bytes` reserves the quant planes' bytes, both halves - a device scale row is its disk row's f16 half
or narrower, so the twin never outgrows its planar source - an eighth over for the entry pads, the packed emb region
where the lane is trimmed, and a window for the plan; the file sink truncates the surplus at close, and a twin past the
bound is the chunk rail's panic, never a truncated image - the sink stops the walk at the first entry past it. The plan
lands in `t.vkplan` for its own turn, which follows `vkblob` in field order. An armed tier that places nothing writes no
twin: the lane carries the planar families alone, as a CPU-mode load's lane does. A CPU-mode load of the same file mints
the CPU lane on its own first load: each lane is its mode's (`#image-lane-name`). The mapped lane re-arms the device
through the slice walk, since the mint's walk armed it off the streamed struct the map replaces.

**The walk declines rather than fail the load.** A trimmed mint the walk cannot honour - the whole-model driver did not
arm, a plan role outside the arena, an emb region no run of jobs holds or that would not pack - abandons the save with its
reason logged (`vk_mint_walk`), and the same load builds the planar image again in memory without the mint and serves it,
since the walk runs before any plane is consumed; nothing is written, and the next process mints again. The mint's
state - the walk's pointers into the save and the driver-side mint arm - is reset by every bake entry point, so a panic
mid-walk leaves nothing armed for the next load.

**The trim lever** (`DASLLAMA_TRIM`, `set_vulkan_trim`, `restore_vulkan_trim` for a fixture) mints the TRIMMED lane, where
`planes_trimmed` is answered by `resident_would_serve` before the walk (the meta serializes first): a dense q8 model with
no per-layer embedding whose plan fits the driver's arena, the plan asked quietly so the pinned-context line prints once
per walk. The planar families the arena holds whole are refused at their turns (`trim_plane_dropped`), the emb region is
packed off its run's view - or off the whole planes where no job streams (`trim_pack_emb`) - and a walk that armed
anything but the whole-model driver after the plan accepted declines the mint. A trimmed image serves the device and
the CPU embed alone: a call the driver passes to the CPU rails panics by name, the NextN draft's CPU fallback included.
A model the plan would not take whole keeps its planar families under the trim, and the load says so; the lane's
identity folds the lever on every tier, the dry tier's included, so a flip re-bakes it. The identity does not fold the
context pin, the mirror codec or the resident route (`DASLLAMA_GPU_CTX_MAX`, `DASLLAMA_GPU_KV` / `set_gpu_kv_dtype`, `DASLLAMA_VK_KV32`, `DASLLAMA_GPU_RESIDENT`),
so a trimmed lane the driver would decline under this run's knobs is caught before its upload: the map asks the plan
again, deletes the lane and mints under the run's knobs. A streamed MoE's expert planes stay planar
(`VkBakeRole.expert_stream`: decode runs them on the CPU); the eager rail (a gguf that would not open for streaming, a
chunk that would not allocate) and a planar image already mapped bake the lane from RAM or the mapping with the families
kept, the eager rail writing the CPU lane beside it and serving that.
