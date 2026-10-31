# dasLLAMA Architecture - the prepared-image rail, the vulkan lane's mint

Companion to `ARCHITECTURE_IMAGE.md`; this document carries one section, `ARCHITECTURE_IMAGE_VULKAN.md#image-vulkan-mint`,
cited by its anchor.

### The vulkan lane is minted off the gguf by its own walk {#image-vulkan-mint}

A vulkan-armed cold load mints the vulkan lane and no other: the streaming save (`save_model_image_streaming`, its
in-memory twin under `DASLLAMA_IMAGE_SAVE=0`) runs the GPU walk at the first plane's turn, dry, and every entry the walk
places is gathered off the gguf mapping and streamed to the writer as the plan records it (`vulkan_mint_begin`, the
sink's `VK_ENTRY_ALIGN` import windows), so on the file sink the device twin never sits whole in RAM and no planar file
precedes it; the in-memory sink holds the twin inside the chunk it serves from, as it holds every plane. The walk under
the mint collects alone: `vulkan_mint_begin` sets the live tier's `dry` flag for the walk's span (`moe_gpu_mint_dry`),
so every device-side site - the pinned stream stacks, the device buffers, the uploads - skips as the offline bake's walk
does, since the lane the walk mints is mapped and armed right after and a device armed off the build would only be
dropped; the pinned stacks a live walk would build are the model's streamed experts whole, which is what a 64 GB box has
no room for beside the lane. A model's PLE n-gram table (`ngram_tab`, the largest raw plane a model carries) rides the
same rail: the streamed load leaves it on the mapping (`ngram_stream_source`) and its plane's turn writes it straight off
the gguf (`gguf_stream_tensor_raw`), sized into the save's reserve, so the table never has a copy in RAM either.
`utils/dasllama-convert --stream -f vulkan` is this rail under the dry bake arm, not a rail of its own
(`REVIEW_MEMORY.md`). An entry's source is the run of streamed jobs that together hold its elements - one job, or the adjacent jobs a MoE
stack takes when the loader records it one expert at a time (`stream_job_span`) - transcoded into a view of the model's
planes whose offset 0 is the run's first `woff` (`stream_job_view`: the scale halves as the gathers read them, every job's
repack regions applied, the frozen K-quant interleave copied), and the arena placement's own gather runs over that view
(`layout_gather_dispatch`). The gather keeps the last run's view while the next entry sits in the same run
(`vk_mint_gather_hook`), so the pieces of one entry - a token table's rows - share one transcode; an nv4 entry's site
scales ride into the view under its rebased keys, one a slice, so a stack's experts keep their own. A model whose streaming
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
the CPU lane on its own first load: each lane is its mode's (`#image-lane-name`). The mapped lane arms the device
through the slice walk; the mint's dry walk armed nothing off the streamed struct the map replaces.

**The CUDA home mints the `cuda` lane by the same walk.** The mint, the slice, the plan's marker entries and the stream
sources live in the shared driver (`dasllama_gpu_resident`), so the CUDA home's registration fills the same seats - the bake
tag off the discrete config section with `home` set to `cuda` (`cud_dlim_source`), the dry-bake arm (`cud_dry_bake_arm`: it
refuses a config whose `home` is not `cuda` or whose tag disagrees with this process' wants, arms the device for the walk's
kernels, and hands the walk the config's `vram_mb` as the dry budget - `cud_mint_dry_budget`, read by `cud_dry_budget` under
the dry flag - so the card's live free memory never sizes a bake and a bake beside a loaded model plans the lane an empty
card would), the would-accept
arithmetic (the format and the row lattice) and the mint's dry flag (`cud_mint_dry`: every seat the load walk reaches runs
its accept / refuse arithmetic and allocates nothing, the arenas counting blocks with no device memory behind them,
`g_cud_dry_arenas`; the pool arms nothing and the pre-step's projection stays with the host under the dry walk, as on the
Vulkan home) - and `image_lane_name` folds a `cuda*` tag into the `cuda` lane. The two lanes hold the same device-layout
planes and differ in the plan; a box with both homes keeps one lane each, since each home's budget and carve move the
plan's split. The CUDA home's stream mirrors are its own pinned copies, never imports of the mapping, so the unmap notice
has nothing to release there.

**The walk declines rather than fail the load.** A trimmed mint the walk cannot honour - the whole-model driver did not
arm, a plan role outside the arena, an emb region no run of jobs holds or that would not pack - abandons the save with its
reason logged (`vk_mint_walk`), and the same load builds the planar image again in memory without the mint and serves it,
since the walk runs before any plane is consumed; nothing is written, and the next process mints again. The mint's
state - the walk's pointers into the save and the driver-side mint arm - is reset by every bake entry point, so a panic
mid-walk leaves nothing armed for the next load.

**The trim lever** (`DASLLAMA_TRIM`, on by default; `set_vulkan_trim`, `restore_vulkan_trim` for a fixture) mints the TRIMMED lane, where
`planes_trimmed` is answered by `resident_would_serve` before the walk (the meta serializes first): a dense q8 model with
no per-layer embedding that passes the gate chain `resident_upload` declines by (`resident_decline_reason`: the seams, a
prefill pinned or overridden away from the driver, the features, every layer's shape and plane forms), whose NextN head,
where it has one, the driver homes, and whose plan fits the driver's arena, the plan asked quietly so the pinned-context
line prints once per walk. The converter's `--trim` (`trim_model_planes`) asks the same answer over its dry walk's plan, so
a NextN model trims on both rails or on neither. The planar families the arena holds whole are refused at their turns (`trim_plane_dropped`), the emb region is
packed off its run's view - or off the whole planes where no job streams (`trim_pack_emb`) - and a walk that armed
anything but the whole-model driver after the plan accepted declines the mint. A trimmed image serves the device and
the CPU embed alone: a call the driver passes to the CPU rails panics by name, the NextN draft's CPU fallback included, and a
mapped lane the driver does not arm for is refused at its load (`ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#vk-shared-device`).
A model the plan would not take whole keeps its planar families under the trim, and the load says so; the lane's
identity folds the lever on every tier, the dry tier's included, so a flip re-bakes it. The identity does not fold the
context pin, the mirror codec, the resident route, the sliding layers' ring pin or the host-layers ask
(`DASLLAMA_GPU_CTX_MAX`, `DASLLAMA_GPU_KV` / `set_gpu_kv_dtype`, `DASLLAMA_VK_KV32`, `DASLLAMA_GPU_RESIDENT`,
`DASLLAMA_GPU_KV_RING` / `set_gpu_kv_ring`, `DASLLAMA_GPU_HOST_LAYERS` / `set_gpu_host_layers`): each moves what the
plan charges the card, since the K/V mirror is the device's and never the lane's - and on a plain MoE the plan's
host-layer split moves the lane's bytes too: a routed layer the plan keeps on the device bakes its expert stacks whole
into the arena, a layer it moves to the host bakes them an expert at a time for the stream, so a lane minted under one
split serves no other. The bake records the split as a marker entry of the plan (`VkBakeRole.host_split`, its `woff` the
count, no bytes; the slice walks past it), and `vk_baked_host_layers` reads it back (-1 off a plan with no marker, which
a lane of the current `IMAGE_VERSION` never carries: the marker's arrival bumped it, so an older lane is version-stale
and re-minted). So a trimmed lane the driver would decline under this run's knobs, or whose baked split is not
this run's plan's, is caught before its upload: the map asks the plan again, and a decline that is the run's
(`resident_trim_decline_is_run_mode`, the split's included) keeps the lane on disk and serves the streamed build
planar from memory - the split's decline also holds the mint, so a run under another split never writes its lane over
the one on disk - while a decline of the model's shape deletes the lane and mints it again - or, where the caller named
the `.dlim` itself and no gguf stands behind it,
panics by name with the decline and the knobs the identity leaves out (`load_model_image`, `trim_lane_run_knobs`). A MoE lane trims
like a dense one: every expert stack the device holds (`VkBakeRole.expert`) or streams (`VkBakeRole.expert_stream`) has its
twin in `vkblob`, and the host reads a streamed stack off its pinned mirror - the decode picks, the window sums and the
hot-pool fills (`ARCHITECTURE_GPU_VULKAN_STREAM.md#streamed-layer-arena`) - so the trim's admission (`trim_plan_decline`)
takes a plan of arena and expert roles with every host layer's stream mirror made, and declines by name a model whose
host still reads the planar planes: one with per-layer embeddings, biased expert stacks or the gemma-4 MoE form
(`resident_trim_decline`), and, as this run's knobs, the stream arm or the rare rail's arena or one-pass form off
(`TRIM_STREAM_DECLINE`, `TRIM_RAIL_DECLINE`: run-mode). A run-mode decline with no trimmed lane on disk writes the
untrimmed lane at the lane's path under the untrimmed identity (`untrimmed_lane_tag`), and a trimmed load falls back to it
while the decline holds; a run that can serve the trim mints the trimmed lane over it. A run-mode decline while a trimmed
lane sits on disk writes nothing: the lane stays for the next run and this run serves planar from memory. On the mapped trimmed lane a planar read of
an expert stack - the rail or the mirror declined after all - is a panic by name (`trim_planar_guard`), as a live gather
is. A lane baked under one host-layers count and mapped under another leaves the baked plan at
the first routed layer the counts disagree on, and the walk regathers its expert entries live off those planes from
there, the load warning (`vk_bake_slice_off`) - on an untrimmed lane; a trimmed one panics there. The eager rail (a gguf that would not open for streaming, a
chunk that would not allocate) and a planar image already mapped bake the lane from RAM or the mapping with the families
kept, the eager rail writing the CPU lane beside it and serving that.
