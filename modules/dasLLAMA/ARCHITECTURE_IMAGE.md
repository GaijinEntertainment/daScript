# dasLLAMA Architecture - the prepared-image rail

Companion to `ARCHITECTURE.md`; section numbers are that document's.

### 2.1 There is ONE way to load a model

A weight carrier becomes a live struct through exactly two functions here, and nothing else may
read weights into one:

- **`build_image`** walks a carrier's planes into a sink - a `.dlim` file, or a page-aligned memory
  chunk when there is nowhere to write.
- **`parse_image`** turns `(base, bytes)` back into borrowed-plane fields, and does not know which
  sink produced them.

Cold and warm therefore yield the SAME struct. A cold load reaches it by building the image and
handing off *through the file* - write, drop the model, map - so the model and its image are never
both resident. That handoff costs a close and a re-map of a multi-GB file and is the *slower* cold
start on purpose, under the tiebreak in `ARCHITECTURE_INVARIANTS.md` sec.3. `cache_via_image` is
that handoff for every weight carrier; the streaming forms transcode planes from the gguf
mapping straight into the image so they never materialize at all.

**An image is judged only where its identity can be recomputed - the LANE rule.** An identity's
(quant, tag) pair is its lane, and lanes coexist by design: a bake proves dead only its OWN lane's
siblings, plus BROKEN and version-stale images anywhere. Everything else is `FOREIGN` - kept, and
counted in the log. Two contexts produce lanes this process cannot reproduce: a GPU flavor (a blob
bake selects its own backend and winners, so the string differs even where the tag matches:
`portable|q8 mr4` against a planar `arm64-gen|q8 mr8`), and a family tag whose module is not loaded
(the tag registry is filled by each family's `[init]`, so a process that never required whisper has
no way to know a whisper image is current). A process that cannot recompute an identity has no
standing to call it dead - the orchestrator of a sweep least of all, since its cells measure through
an exe carrying its own baked winners. The one owner carve-out is the batch lifecycle: the board
rigs own the model dirs for a whole run, so `dlim_wipe` (verdict-blind, `dasllama_image.das`)
clears them behind the exe gate at batch start and after each model's last cell, with every image
re-baked from its gguf on demand. Judging stays forbidden; owning the directory for the batch is
what licenses deletion without judgment.

### 2.1a Page alignment is the no-copy contract {#image-page-alignment}

Every plane section starts on a 16 KiB boundary (`IMAGE_PAGE`, the Apple-Silicon page) and the
image's total length is a page multiple. The alignment is what lets a mapped plane be wrapped for
the GPU with no copy - Metal's `bytesNoCopy` requires it - and what lets a load borrow a plane in
place instead of reading it.

### 2.1b The meta blob leads with two strings {#dlim-meta-head}

The meta blob sits at the image tail and leads with two strings - the identity the image was baked
for and its config JSON - then the section table and the walk's scalar stream. The strings lead so
a peek, or a load that is about to decline, can print what the image was baked for without parsing
anything else.

### 2.1c Array payloads reach the archive in bulk {#image-bulk-serialize}

Array payloads reach the archive in bulk - one stream call per array (`serialize_pod_array`),
string arrays as a length vector plus one byte blob (`serialize_strings`). `daslib/archive`'s
per-element generic dispatch costs on the order of 340 us per element, which puts a 128k-entry
vocabulary near 238 seconds; the bulk forms make the same work milliseconds.

### 2.1d An interpreted gguf load pays; an image load does not {#image-interp-load}

A gguf load's O(model) transform loops run about ten times slower interpreted - a tinyllama load
takes 53 s against 5.5 s jitted, and a 69 GB hybrid extrapolates to an hour (the repack itself is
native tune kernels and costs the same either way). A prepared image costs nothing interpreted,
because mapping and borrowing planes runs no such loop. That asymmetry is why the guard fires on
the gguf path and never on the image path.

### 2.1e Publishing an image {#image-publish}

An image is published by writing a temp file beside its destination and renaming over it. POSIX
makes that replace atomic. Windows has no rename-over, so the publish removes the destination
first and a concurrent reader can see a brief absence - which costs that reader a regenerate,
never a corrupt map.

### 2.1f The image's size is known before the first byte {#image-sizing-exactness}

The image's final size is known before a byte goes out: the meta blob is serialized first, so its
length is in hand, and every plane contributes its own bytes (`image_total_bytes`). The sink
preallocates from that number, so the file lands as one contiguous run and the memory chunk is
exact. The walk advances the writer by exactly the bytes it accepted, so the section table can
never drift from the file. An append past the chunk is therefore a disagreement between the sizing
pass and the walk - a bug, not a disk condition - and the chunk rail panics rather than limping on.
The one decline it survives is failing to get the chunk at all: that happens before the walk
starts, so the carrier is still whole and its caller keeps serving it.

### 2.1g Identity names the backend, so the backend is selected first {#image-identity-backend-order}

An image's identity names the active matmul backend, so the backend is selected before any
identity is computed or compared. `image_identity` is a pure formatter over `DlimConfiguration`;
the backend select happens inside the config's CPU source, so a caller needs no ordering ritual of
its own. A load pins the box profile first because that pin can change the backend, and the parse
runs the same load select the gguf loader runs, before any kernel touches planes packed for that
backend.


### 2.1h The baked dev-W f16 panel plane {#image-devw-plane}

A metal-flavor image carries `devwf16`: every dev-W-eligible q8 and k4/k5/k6 weight (a plane
job whose f16 panel fits `DEVW_BAKE_PANEL_MAX`, the prefill knee's mirror) pre-dequantized at
mint into one concatenated f16 plane, with parallel site tables (`devwf16_fmt/key/off/n`)
serialized as ordinary planes. The mint dequant is the format's CPU mirror of the runtime
dev-W dequant kernel, and every mirror is bit-exact by construction: each f32 product fits
the 24-bit significand exactly (the widest, k6's `dsc*q`, needs 24), so only the final f16
round exists and contraction cannot move it - a baked panel is bit-equal to a runtime one.
The first served prefill seeds the resident-panel cache with zero-copy `(buffer, offset)`
views over the mapped plane, so a baked site never dispatches a dequant and holds no
dedicated memory; sites the bake does not cover (q40/q51 - the dev-W route has no dequant
kernel for them - and owned gguf loads) stay on the runtime path, resident under
`DASLLAMA_METAL_DEVW_RESIDENT` or scratch.
