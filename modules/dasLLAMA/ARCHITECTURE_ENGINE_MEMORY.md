# dasLLAMA Architecture - host memory under a device home

Companion to `ARCHITECTURE.md` beside `ARCHITECTURE_ENGINE.md`; a section is cited by its anchor.

### The host holds what the host reads {#host-holds-what-it-reads}

On a discrete home - a backend serving on a device whose memory is not the host's: a discrete card under Vulkan or
CUDA - the device holds a model's weights, and the host keeps a weight plane only where host code reads it on a served
call. Metal wraps the mapped image's pages, and Vulkan on a device that shares the host's memory is the same RAM either
way: neither is a discrete home. What a load leaves on the host is then: the config, the tokenizer and the meta fields;
`fblob`, every f32 row - the router, the norms, the biases, the sinks - which the upload reads and the host chains add;
`embq` / `embs`, the packed token rows the CPU embed reads and a lane's untied table is placed on the device from; `bf16blob`, the per-layer embedding's model projection; the
offset and format tables; and the pinned mirrors of the routed layers the plan serves on the host, which the rare rail
reads (`ARCHITECTURE_GPU_VULKAN_STREAM.md#streamed-layer-arena`). The planar quant families - `qblob`, the scale planes,
every K-quant and per-32 pair - are not among them (`trim_drop_planes`). Three loads end there: a trimmed lane mapped
(`ARCHITECTURE_IMAGE_VULKAN.md#image-vulkan-mint`), whose device twin is read off the file and never faulted in
(`ARCHITECTURE_IMAGE_VULKAN.md#lane-file-read`); a lane minted cold, which is then mapped; and a gguf with no lane
(`#gguf-live-load`). A call the driver passes to the CPU rails on such a model is a panic by name (`blob_only_panic`).
The planes stay on the host by a named ask alone - `DASLLAMA_TRIM=0` for a lane, `DASLLAMA_HOST_PLANES=1` /
`set_host_planes` for a gguf with no lane - which is what a test's device-against-CPU compare loads under.

### A gguf with no lane streams onto the device {#gguf-live-load}

A load on a discrete home that writes no lane - the load with no image (`load_model_uncached`), which is also the
cached load's arm with the image rail off; the image save off; a save that lost a write - runs the mint's walk live
(`load_gguf_live`): the plan-load records every plane's jobs and builds none
(`load_gguf_streaming`), the GPU walk gathers each region off the gguf mapping through the mint's hook
(`vk_mint_gather_hook`) with the device not dry and no sink (`vulkan_live_begin`), and each region goes to the card as
the walk places it. The emb region packs off the jobs' view, the scale halves the plan-load read go once the card holds
them, and the model is the trimmed lane's host half with no image behind it. The load takes this arm where the trim's
admission holds (`resident_trim_decline`) and the model streams whole; a model whose n-gram table rides the mapping, or
one the plan-load does not stream, loads with its planes, the reason logged. The plan-load builds one plane itself: the
per-layer embedding's bf16 projection (`bf16blob`) is read whole off the gguf and stays on the host, the CPU pre-step's
projection where the device takes none and the re-arm's upload source. The load's log line names what put it on this
arm and the ask that turns it off. A re-arm streams the gguf again over the same model (`gguf_live_rearm`,
`gguf_live_loaded`): the model keeps its source's path (`rearm_source`), and the walk needs no host plane; a re-arm
whose walk declines drops what it placed and answers `cpu`, the model then serving nothing - every call a panic by name.

### A mapped gguf's pages go as the next tensor's read begins {#gguf-source-release}

Every gguf tensor read goes through `with_tensor_view`, and on a mapping its owner armed (`gguf_source_begin`: the load's
own `fmap`; `gguf_release_arm`: each shard of `gguf_map_shards`) a read of a tensor outside those the mapping's last read
held begins by letting their whole pages leave the process's resident set (`gguf_tensor_enter`; `release_map`:
`madvise(MADV_DONTNEED)` on Linux, `msync(MS_INVALIDATE)` on macOS, where the advice alone leaves the pages, and
`VirtualUnlock` on Windows, which drops a range that is not locked from the working set) and reading its own tensor ahead.
A load, a mint and a convert then hold the tensors in flight and not the file. The unit is the read in flight and not the
call: an expert stack is read an expert at a time, and a release after each of those reads faults the whole stack back for
the next; an affine triplet is read through three views nested one in another, and a view opened inside another's joins
the held instead of releasing the tensor the outer view is about to read. The whole-file readahead would bring every page
in before the first release, so under the release each tensor is read ahead at its own turn. A gguf held in heap memory -
a synthesized one, an image its host holds - never arms: on such memory the same call discards the bytes; arming a base
replaces any row a load left there. `DASLLAMA_SOURCE_RELEASE=0` (`set_source_release` in process) keeps every page until
the mapping closes; `gguf_tensors_released` counts the tensors whose pages went.

### A streamed token table is a job a piece of rows {#table-piece-jobs}

The plan-load records a token table - the embedding, the classifier, the per-layer embedding table - past a dev-W panel's
size (`DEVW_BAKE_PANEL_MAX`) as a job every 65536 rows (`TABLE_PIECE_ROWS`), the piece the device walk places a table in
(`resident_place_table`); a table a panel holds stays one job, the panel's bake site, and a piece is never a site. A job's
staging and a placed table's view then hold a piece and never the table - the classifier's gather alone reads its rows as
one run of pieces (`moe_gpu_gather_upload`) - and every count along the way is 64-bit: the E-series' per-layer table is
2.35 billion elements. Every other tensor is one job, an expert stack loaded whole among them: a job holds whole repack
regions (`stream_job_transcode` repacks a region in its job's staging and refuses one that crosses either edge), and a
piece of a stack would cut an expert's rows; a stack requantized to q8 is read whole as f32 on the way, five bytes an
element of host staging for the largest stack.

### A save that loses a write starts the load again {#failed-save-restarts}

The streaming save consumes the plan-loaded model as it writes, so a save that loses a write part way - the disk full -
leaves a model nothing can be built from. The cached load then writes no lane and starts again off the gguf: streamed
onto the device on a discrete home (`#gguf-live-load`), through the eager rail elsewhere. A mint the walk declined
before it consumed a plane still serves its build from memory.

### The CUDA window chain's planes are taken when the driver arms {#cuda-window-planes-at-arm}

The residency plan charges the window chain's scratch planes when it sizes the K/V mirror, and the CUDA home allocates
them at the arm's last upload (`crd_pf_setup` from `cud_rdec_upload_norms`), so the room the plan counted is held before
anything attached later asks the card for memory: a block drafter attached to a full card then finds no room through its
own check and keeps its pass on the CPU. `cud_dev_zeroed` counts the allocations the card refused
(`cud_dev_failures`); a setup that allocates several planes reads the count before and after - the host-cut pair a
routed model with experts on the host uploads (`pf_ones`, `pf_ident`) checked by its handles, no count seeing an
upload - and a window whose planes are missing refuses by name (`crd_pf_ready`): no kernel launches over a null plane.
