# dasLLAMA Architecture - format, load and CPU-tier file charters

Companion to `ARCHITECTURE.md` beside `ARCHITECTURE_ENGINE.md`; a section is cited by its
anchor.

### Formats and data movement {#formats-and-data-movement}

- **`dasllama_plane.das`** - the borrowed-plane vocabulary: a plane is a pointer into a prepared
  image plus its element count; the image owns the bytes, a carrier owns nothing but its backing.
  Requires nothing in dasllama - the image rail binds planes, every carrier holds them.
- **`dasllama_kqformat.das`** - format IDENTITY: the `KqFmt` enum, the per-format descriptor table
  (plane strides, block geometry, stream codes), format predicates, and the shared decode
  tables the grid and codebook formats key off - each as a builder function (`iq3s_grid()`,
  `iq4nl_lut()`) for kernels that may run on a team lane, plus a global twin for tests,
  oracles and the emitter's constant bake, the `KqTag_<fmt>` tag family (one empty struct per
  member, the LAST parameter of every per-format overload family), the `kq_fmt_stamp` call
  macro that binds a member's tag per arm, the `kq_tile_stamp` call macro that stamps a kernel
  home's per-member pick ladders over the enum (with `kq_tile_stem`, the per-32 formats' stamp
  spelling) and its format-free twin `stamp_ladder`, which stamps a ladder over any word list
  keyed by ordinal (the flash and decode attention stamps), the `[grid_words]` annotation that bakes a codebook table into a kernel home's word
  accessor, and the `KQ_CM2E_ALIASES_M` roster of the formats whose expert column the m stamp
  serves - the format-keyed facts every kernel home reads. It requires nothing else in
  dasllama, because it is the taxonomy everything keys off. ONE id space - the enum; integer ids
  exist only at the IR/kernel-param boundary. `kq_sb` is the superblock-lattice predicate: a
  `fmt != q8` test does not imply the lattice, so branch on the predicate.
- **`dasllama_convert.das`** - every tensor format CONVERSION: quantize/dequantize/transcode/encode,
  codec byte readers, numeric widen/narrow - regardless of platform or caller; plus the Q8_0/Q4_0
  format identity itself (block geometry, scale layout, the quantization-quality detector).
  Metadata-coupled drivers (GGUF lookup, threading, guards) stay with their containers and
  dispatch in. ONE carve-out: a conversion that IS a KV-cache format's store/read half lives with
  its codec family (sec. below).
- **`dasllama_repack.das`** - every disk-order -> compute-order kernel-LAYOUT transform (grp
  interleaves, disk-order extractors, panel unpacks), any format, any platform. Number sources
  (tune stamps, bake overrides) stay with their owners and pass plain parameters in.
- **`dasllama_kv_codec.das`** - the KV-cache runtime codec, one family per cache format, kept
  WHOLE: store (quantize), read (dequant), the attention score dot, and the V-accumulate axpy all
  share the format's block byte geometry (the two block byte counts a kernel home reads as module `let`s,
  `KV_Q8_BLOCK_BYTES` / `KV_TQ4_BLOCK_BYTES`, sit beside the tag in `dasllama_kv_dtype.das`, which this
  module requires), so a layout change can never span the codec and its dispatch. These are
  per-token and per-tokenxposition `[tuned]` hot kernels, not load-time converters. The tq4 FWHT
  rotation and sign vector live with their family. Codec DISPATCH (`KVDtype`) stays at common's
  `kv_store_row`/`kv_load_row`/`kv_dot`/`kv_axpy` seam; the f16 row converts are the generic pair
  in `dasllama_convert` because they are dual-use beyond the cache.
- **`dasllama_kv_dtype.das`** - the codec tag (`KVDtype`), the block codecs' byte counts the kernel
  homes stamp (`KV_Q8_BLOCK_BYTES`, `KV_TQ4_BLOCK_BYTES`), the tq4 pointer newtype (`TQ4B`) and
  the `kv_codec_side` stamp: a call macro that clones its block once per codec with the byte
  pointer read as that codec's element type behind a runtime ladder. Every attention wrapper
  that hands a worker a typed cache pointer (`attn_head_decode_d`, the three prefill `_d`
  wrappers, the slice pair, the flash-decode witness) nests one stamp per side, so the K x V
  matrix is written nowhere and a new codec is one arm in the macro. A leaf module - only
  daslib requires - so its macro context stays cheap.
- **`dasllama_rope.das`** - RoPE angle and TABLE GENERATION: the theta schedule, `rope_freqs`,
  fscale/mscale, every materialized layout - pure functions over plain parameters. Model-facing
  wrappers stay in common. APPLICATION kernels stay with their backends: the CPU `rope_scaled_*`
  leaves and the GPU fused rope-store kernels are shape-specialized deliberately, and
  single-sourcing them is generator-rail work, never a hand-merge. Float multiply order in the
  builders is contractual (parity-pinned).
- **`dasllama_gguf.das`** - the GGUF container: KV/tensor descriptors, the byte-level reader, the
  mapping. Codecs live in `dasllama_convert`; this file finds bytes, it does not decode them.
- **`dasllama_layout.das`** - disk-format -> compute-layout transforms at LOAD scope: the blob
  transform, the CPU repack walkers, the GPU tier gathers, the per-region q8->Metal-34B
  un-repack (`q8_region_to_metal_blob`, the ASR-decoder driver's upload form), the GGUF Q8_0 blocks'
  split into the quant and f16 scale planes (`q8_repack`, the Vulkan assistant drafter's upload form), and the refusal
  half (`can this model take the blob form`) split out so the image writer can commit without
  loading.
- **`dasllama_tokenizer.das`** - the tokenizer facade: backend selection off the GGUF metadata and
  the one encode/decode/piece surface models and the chat layer call. Re-exports both backends, so
  a consumer requires this file and never picks a backend by hand. `REVIEW.das`'s
  `check_unicode_tables_stamp` hashes `dasllama_unicode.das`'s RANGES and WS literals against
  `UNICODE_TABLES_HASH` and licenses no names: the tables change only through a retranscode of the
  reference checkout's unicode data, and the re-stamp is the regeneration's receipt.
- **`dasllama_unicode.das`** - the codepoint tables (RANGES, WS) and the UTF-8 codepoint helpers over
  a `string` the tokenizers and the TTS front end share: classification, `cpt_count`, the
  decode/encode of one codepoint.
- **`dasllama_spm.das`** - the SentencePiece backend: score-greedy merges over vocab pieces (Llama-2, Phi-3, Gemma),
  the unigram Viterbi arm over the same pieces (`unigram_encode`, GGUF `"t5"`, `ARCHITECTURE_POCKET.md#pocket-tts`'s tokenizer), `<0xXX>` byte fallback.
  A byte piece is found by its NAME (`byte_piece_id`, one of the derived tables), never by arithmetic on
  the byte: Llama-2 and Phi-3 keep the byte pieces at 3 + the byte, Mistral v0.3 after its 768
  control tokens, and a vocabulary with no piece for a byte it must spell panics, as the reference throws.
- **`dasllama_bpe.das`** - the byte-level BPE backend (Llama-3 / tiktoken family): vocab load, the
  GPT-2 byte alphabet, ranked merges, encode/decode. Split from SPM because the two algorithms
  share no state (their twin merge heaps are `ARCHITECTURE_ENGINE_FORMATS.md#bpe-merge-heap`'s); a third merge algorithm gets a fourth file. Two sanctioned family-name tests
  live here rather than in `dasllama_pretok`: the `pre`-name selector inside `bpe_encode`, and the
  gemma-4 newline-run split in `bpe_encode_spm_space`; `load_bpe_tokenizer_gguf`'s per-family
  metadata defaults are the third and last. A GGUF without `tokenizer.ggml.add_bos_token` takes
  upstream's per-pre default (`bpe_pre_adds_bos`): the pres on its list - the llama3 family,
  tekken, chameleon, a few smaller ones - prepend BOS, every other pre - qwen2, qwen35, gpt-2,
  gpt-4o among them - prepends none. The
  default matters: a Qwen hybrid fed a leading `<|endoftext|>` degenerates for the whole
  generation, and the unsloth Qwen3.8 conversions omit the key. `tokenizer.ggml.add_eos_token` has
  no per-family default and both backends read it: absent, no EOS is appended; present and set,
  `encode` ends every text with the EOS - the row an embedding model's last-token pooling reads (Qwen3-Embedding ships the key on,
  with `{arch}.pooling_type` = last, which `Config.pooling` carries for `embed`).
- **`dasllama_pretok.das`** - the pre-tokenizer: one hand-compiled split function per family
  (llama3/qwen2/qwen35, gpt-2, gpt-4o, tekken), selected by the BPE `pre` name. Regex-port growth
  lands here, never in the merge engine - the two change for different reasons (new model family
  vs. algorithm work). Every arm with an on-disk upstream corpus vocab is gated by its case in
  `test_tokenizer.das` (llama3, qwen2, qwen35, gpt-2); tekken has no corpus case, and gpt-4o is
  pinned by frozen ids in `test_parity.das` only.

### The two tokenizer backends carry twin merge heaps {#bpe-merge-heap}

`bigram_merge_heap` (`dasllama_bpe.das`) and `spm_merge_heap` (`dasllama_spm.das`) each merge a whole symbol run through a max-heap over candidate pairs, O(n log n) where the sequential rescan is O(n^2): BPE's priority is the negated merge rank (lowest rank first), SPM's the piece score; an entry whose endpoint merged since it was pushed fails the adjacency+id guard and is skipped, so the ids are the rescan's exactly, `-2` dead slots compact out and SPM's `-1` survives. The two bodies stay twins on a measurement: served from `dasllama_bpe.das`'s JIT partition - as one function, and as a generic stamped per backend with its helpers left in that module - SPM encode read 5% to 19% slower than its own copy; each heap lives in the partition of the encode that runs it.

### The RoPE table builders are one fill over a position source {#rope-one-fill}

Every materialized cos/sin table is the same fill; the position source is what differs. A source answers two questions - which axis half-dim j reads (`rope_axis`, once per j) and what position row pi sits at on that axis (`rope_pos`) - and the vision grid adds a third, its restarted frequency ladder (`rope_freq_of`). `freq[j]` and the axis hoist out of the row loop and `mscale` folds in at the fill, for NORM and NEOX alike.

### The per-format plane table is the Model's only fixed-size array {#model-plane-table-dim}

`Model.kq : KqPlanes[KQ_FMT_COUNT]` is the one `dim` field a `Model` carries, so every `apply` walk over it - the finalizer, the image build, the image parse, the layout describe - spells its plane-table arm as the `typeinfo is_dim(field)` branch and reaches each element's arrays from there. The table's interleaves serialize in enum order, so an appended `KqFmt` member lands last and no earlier slot moves.

### The per-32 rail is one kernel body over a format tag {#per-32-rail}

The per-32 formats (`q51`, `iq4nl32`, `q4032`, `q5032`) sit off the superblock lattice: 32-weight blocks, Q8_0-form
activations, dedicated `Model` plane pairs (`ARCHITECTURE_IMAGE.md#image-kq-table-sb-only`), and in
`dasllama_math_default.das` one body per kernel shape (`b32_batch_kernel`, `b32_groupn_kernel`,
`b32_batch_groupn_kernel`) generic over the `KqTag_<fmt>` it takes last - the tag picks the
strides, the row expand, the grp scale gather and the expanded-row dot; the two 5-bit members share
q51's leaves under a level and fold arm (`b5_level`, `b5_fold`). Every per-format seat around them
is a table indexed by `int(KqFmt)` - the backend's `B32Slots` (`KernelBackend.b32`), the layout and
stream hooks - read by one fmt-taking call (`matmul_b32q8_*`, `mm_b32_pre`, `b32_planes_of`), the
planes reached through `kq_plane_pair`. `iq4nl32` is the IQ4_NL
disk block served per-32 and `q4032` the Q4_0 one: `kq_fmt_row_ok` chooses each for a plane whose
rows are off the 256 lattice and on the 32 lattice (the superblock members `iq4nl` and `q40` serve a
`% 256` row), and `q51` and `q5032` keep their tags on any 32-multiple row - Q5_0 has no superblock
member. Every site takes that one rule - dense, expert and table planes alike (`#per-32-tables`).
The codebook levels are int8, so a q8 requant of an IQ4_NL, Q4_0 or Q5_0 block would be
value-exact: the per-32 form's gain is bytes - 18 or 22 a block against 34 - not precision.

### The table planes serve per-32 like the dense ones {#per-32-tables}

`kq_tag` is one rule for every site: the disk type's member, kept where `kq_fmt_row_ok` admits the
row, else q8 with the demotion logged. So the token table, the classifier, the NextN eh_proj, the
deltanet five and the shared expert's triple carry a per-32 tag on the same terms as a dense
plane, and each home reads it in that form: the CPU through `embed_b32_row` and the per-32 arms of
`mm_f` / `mm_b_f` (`mm_cls` included); Vulkan through the `Blk32GemvRunT` leaves - the token gather
is its `EMB` axis, the classifier and the per-op sites `resident_plane_ok`; Metal through the
`MetalBlk32GemvT` stamps and the `MetalEmbedB32T` gather. A tied per-32 table is the classifier's
plane at `wcls_off`; an untied one sits past it at `emb_kq_off`.

`q4032` is the member a Q4_0 row off the 256 lattice serves as, `q5032` the member every Q5_0 row
serves as:

| | q4032 | q5032 |
|---|---|---|
| disk type | GGML Q4_0, 18 bytes a 32-block (f16 d, 16 nibble bytes) | GGML Q5_0, 22 bytes a 32-block (f16 d, qh u32, 16 nibble bytes) |
| planes | `q4032q` 16 nibble bytes a block (`Q4032_QB`), `q4032s` the f16 d (`Q4032_SB`) - iq4nl32's split | `q5032q` the 16 nibble bytes then the qh word (`Q5032_QB`, q51's quant form), `q5032s` the f16 d (`Q5032_SB`) |
| code | `q - 8` (`q40_lut`, -8..7), the linear twin of iq4nl32's codebook | `q - 16`, q = nibble \| qh bit << 4 (-16..15), signed: no activation sum enters the fold |
| CPU | the `q4032q8_tile_gen` `[tune]` family (iq4nl32's permutations and fallback, its own layout and GEMV companions, the generator's `q40` code), the portable per-32 bodies the hand tiers' slots and the controls | the `q5032q8_tile_gen` family (q51's grid and fallback, the emitter's `emit_block_q5032`: q51's nibble+qh unpack re-centred, the signed lattice, iq4nl32's fold), the portable bodies (`expand_q5032_row`, `dot_iq4nl32e`) |
| Vulkan | the Blk32 leaves (`q40_signed_word`), the cm2 S/E stamps and the KHR tile (`Q4032Cm2T`), the `EMB` gather | the Blk32 leaves, the cm2 S/E stamps and the KHR tile (`Q5032Cm2T`), the `EMB` gather |
| Metal | iq4nl32's stamps under `Q40L`: the GEMV and its K-split, the small-batch forms, `mul_mm`, the routed GEMV and tiles, the gather | iq4nl32's stamps under `Q50`: the GEMV, its K-split, the routed GEMV and the small-batch forms over a 32-entry `q - 16` table; the gather and `mul_mm` decode `q - 16` directly; the routed tensor twins on q51's staging under `Q50` |
| ids | enum 18, stream tag `Q4032_STREAM_TAG`, no gemm schema | enum 19, stream tag `Q5032_STREAM_TAG`, no gemm schema |

### A served form is a second member over the same disk type {#served-form}

`iq3s4` is IQ3_S served as a 16-entry codebook plane: every IQ3_S weight is a signed odd
magnitude 1..15 times its block scale, so the grid entry, the `qh` ninth bit and the sign fold
at transcode into one nibble index (`iq3s4_lut`, `-15, -13, .., 15`) laid in the k4 tiling, and
the scale row is iq3s's own. The dequant is value-exact against the grid form; the planes cost
17% more bytes (128 against 104 a superblock) and decode through the iq4xs codebook kernels -
one table lookup a nibble in place of the grid gather, which is where the IQ3_S CPU decode spent
its time. The member's descriptor row names the source type (`GGML_TYPE_IQ3_S`, 110 disk bytes)
with its own schema and stream ids, so the image and the stream rail carry it like any lattice
member. The choice is `kq_fmt_of`'s at the load (`iq3s_serve_lut`, the `DASLLAMA_IQ3S_SERVE`
knob or a process pin, `set_iq3s_serve_pin`: `grid`, `lut`, `auto` = lut on the CPU rails and the
Vulkan tier, the grid under the Metal tier), and it is an image identity bit
(`DlimCpuConfig.iq3s_lut`), because it changes plane bytes. Both GPU tiers decode it on every
ladder, the codebook arithmetic (`2q - 15`, no table bound) and the stamps the iq4xs kernels with
the odd codebook switched in: on Vulkan the device row is iq4xs's quant plane under iq3s's scale
word; on Metal the blob's split scale plane carries the eight `1 + 2s` strip bytes and the f16 d
exactly as iq4xs's carries its signed sub-scales (`ARCHITECTURE_GPU_QUANT_PLANES.md#metal-kq-split-scale-plane`),
so every iq4 family stamp under `IQODD` reads it - the GEMV, the small-batch forms, the base and
tensor tiles, the routed GEMV and tiles, the token gather. So a host that sums routed experts
beside a Vulkan device reads the fast planes, and the device reads the same bytes; under Metal
`auto` keeps the grid, which streams 17% fewer bytes a superblock.

### The load and image rail {#the-load-and-image-rail}

**`dasllama_load.das`** is the GGUF load walk: metadata to `Config`, the plane layout, disk-format
detection, the eager and streamed conversion ladders, and the load entry points. It owns nothing the
forward path touches at run time - a loaded `Model` is the whole handoff - and it requires
`dasllama_common` back for `Model`/`Session`, so the transformer umbrella requires it `public` and
breaks the cycle. That re-export is what keeps every consumer on the facade. A file carrying a tensor
type the reader has no arm for (Q4_1, IQ1_S, IQ1_M, TQ1_0, TQ2_0) is declined before any plane is read,
the panic naming the tensor and its type (`gguf_decline_unread_types`). Every big-weight
conversion is accounted as it runs (`conv_account`, on both the eager and the streamed ladder),
and a load that converts ends with the report: the file's bytes, then each (disk type ->
conversion kind) as bytes on disk against bytes served with the delta, biggest overhead first,
and the count of tags the row-length rule demoted to the q8 rail (each demotion logged by
name as it is decided). The delta column is where a served form costs more than the file - a
decoded scale row, an f32 copy of a quantized token table - and it is read as a number, not
inferred from image sizes. A hybrid's deltanet planes - qkv, z, out, beta and alpha - carry the
file's format on every load, whatever backend is armed: one plane a tag, no q8 transcode beside
it, the CPU chain and the Metal drivers each reading it in that format. The one exception is a
grouped (qwen3next) file's out plane, whose input columns the load permutes across superblocks,
so the q8 transcode is its only form. A dense or routed plane whose disk type no `KqFmt` member carries - F16, BF16,
F32, and MXFP4 outside a routed stack - is the q8 transcode on every home, the CPU and both devices; the routed stacks
MXFP4 on disk, the gate and up stacks apart or as gemma-4's fused gate_up stack, load as the native-MXFP4 planes
(`experts_mx4`). No stocked gemma file carries such a dense or routed plane. Every `load_big` call names the form it
fills - the plane's tag, or the `KqFmt` member a conversion serves - so the conversion reads at the call;
`REVIEW.das`'s `check_load_big_format` reds a call with no format, and licenses no names.

- **`dasllama_image.das`** - the prepared-model `.dlim` rail, and it is ONE rail
  (`ARCHITECTURE_IMAGE.md#there-is-one-way-to-load-a`). Nothing
  outside this file may read weights into a live carrier; image backings are released only
  through the single release path in `dasllama_common` that the carrier finalizers call - never
  an ad-hoc unmap.

A split NextN/MTP head (the published split-head layout: an `mtp-<model>.gguf` beside a trunk
converted without its head) rides the load as the LAST shard of the trunk's shard walk: `gguf_shard_paths` appends the
`mtp-<trunk basename>` sibling, or the `DASLLAMA_MTP_HEAD` file, and `parse_gguf_meta_shards`
promotes the head shard's `nextn_predict_layers` onto the meta when the trunk's KVs lack it. The
head's `blk.<n_layers>.*` tensors are the draft block; its copies of the trunk's embedding,
classifier and final norm are dead by construction (tensor lookup is first-match and the head
is last). The prepared image folds the head's name and size into its path hash, so the
trunk-only and trunk+head images never collide and one image file serves both trunk and head.

### The token table and the shared expert carry their own planes {#kq-token-table}

The token table is one plane in the file's format wherever its tag is a `KqFmt` member
(`emb_kq`): tied, the classifier's plane at `wcls_off`; untied, its own plane at `emb_kq_off`,
repacked with its format's plane like every region of it. No fp32 table rides beside it, and the
CPU's `embed_row` and the devices' row gathers read that plane, tied or untied: the Vulkan
window and token commands gather an untied table's rows on the device off its plane as the Metal
drivers do. A table whose disk type no member carries (F16, BF16) keeps its
exact fp32 rows; tied, a load under a Metal mode adds a q8 transcode of it at `wcls_off`
(`cls_q8_copy`) - a converted copy, logged at the load - for the device classifier, while the
embed and the CPU classifier read the fp32 rows; under no Metal mode no device plane carries such
a tied classifier, so the Vulkan whole-model driver declines the model by name
(`resident_cls_decline`) and the per-op rails take it. A float per-layer-embedding table (F16, BF16,
F32) tags q8 and reads its q8 transcode at every home - a converted copy, the gap no stocked file
reaches. The E-series per-layer `inp_gate` and `proj` planes carry one tag a layer from their disk type
(`ple_gate_fmt` / `ple_proj_fmt`, `ple_plane`), carved from that format's planes; a kind that ships F32
keeps the file's f32 rows in fblob (`ple_gate_f32` / `ple_proj_f32`), read by the CPU's fblob GEMVs,
Metal's router GEMV and f32 tile, and Vulkan's router GEMV, f32 act and f32 tile, and a kind mixing F32
with another type declines the load. The shared expert's gate, up and down planes
(the NextN head's triple included) keep the file's format on every load (`wsh*_fmt` /
`wshk*_offs`; `shexp_plane` picks the plane by its tag), and every home reads that plane: the CPU
chain's shared-expert sites (`dasllama_moe.das`, `dasllama_blocks.das`) take `shexp_tagged` to the
format's arm. The q8 transcode holds the planes a tag left on q8 alone: the layout allots it only
where some routed layer's plane reads it (`shexp_reads_transcode`), and the load fills and the
repack walks that plane's region alone (`shexp_plane_q8`).

### CPU kernel tiers {#cpu-kernel-tiers}

- **`dasllama_math.das`** - the numeric ABSTRACTION: typedefs, active backend pointers, public
wrappers, dispatch shaping, and the array-sizing helpers every tier and the engine share
(`reserve_resize`, `grow_resize`, `ensure_length`, `overwrite_resize`), the `[tuned]` fp32 loop-hint
families (`dot`, `axpy`, the elementwise set, `f32_batch_tile`) and the fp32 batch walk
(`matmul_batch_core`, `batch_tile_block`, generic over the f32 and bf16 weight lattices -
`bf16_batch_tile` widens each halfword by the exact shift); every other kernel lives in a tier file.
- **`dasllama_math_default.das`** - the portable backend, always registered, always correct,
out-ranked by any platform tier - the per-32 rail's portable kernels and scalar leaves, which seat
every backend's unset per-32 slot (`#per-32-rail`) - and the ONE body of every Q8*Q8 and mx4 kernel
shape: each shape is a `def template` over a dot and a chunk-split placeholder, and
`[from_template]` stamps it per (dot, split) and per weight-scale plane (the f32 plane and the
wscale_f16 halfword plane, bit-identical over the widened plane). The idot4 stamps the arm64 tier
registers live here too: the `idot4` builtin is plain das that every target lowers for itself. -
**`dasllama_math_aarch64_neon.das`** - the arm64 lane-indexed SDOT tier: the `sdot4_laneq` / `tbl16`
composers only, plus the registration of the idot4 stamps as "arm64-sdot". Its `[init]` never fires
off-arch (`ARCHITECTURE_INVARIANTS.md#inherited-invariants`, three-layer safety model), so an
intrinsic here needs a correct scalar fallback body, not a guard at the call site. -
**`dasllama_math_accelerate.das`** - the Accelerate/BNNS float tier (AMX on M1-M3, SME on M4+), for
genuinely-float planes only. BLAS-for-quant is ruled out structurally
(`ARCHITECTURE_INVARIANTS.md#inherited-invariants`). - **`dasllama_math_gen.das`** /
**`dasllama_gemm_gen.das`** / **`dasllama_gemm_schema.das`** / **`dasllama_gemm_register.das`** -
the generated GEMM tier: the runtime registration, the tile generator, the layout/perm schema shared
by generator and runtime, and the `[tune]` family registration. A hand-written tile that the
generator could emit belongs in the generator. The bf16 tile's operand builders - the activation
widen (`xbf16_build`, its `bf16_bits` narrow, the f32 plane's `xbf16_build_f32`), the row-dequant
reference panel (`kq_panel_rows_bf16`) and the native bf16 plane's k-pair panel
(`bf16w_panel_build`) - sit in `dasllama_math_gen.das` beside the walk that runs them once per batch
call, in its JIT partition (`ARCHITECTURE_CPU_KERNELS.md#jit-partition-inlining`). The JIT cache
keys a generated kernel by its stub's hash and its perm's arguments, never by the generator's text,
so `REVIEW.das`'s `check_gemm_gen_stamp` pins one hash over `dasllama_gemm_gen.das` and
`dasllama_gemm_schema.das` with their comments and blank lines cut: a code edit to either is red
until the stamp moves, and the finding asks for the `LLVM_JIT_CODEGEN_VERSION` bump that re-keys the
cached kernels. The check licenses no names. - **`dasllama_tune.das`** - the per-box loop-hint tuner
(`[tuned]` / `[dasllama_grid]`) and the perm-less clone `[from_template]` (a template body into an
empty stub, placeholder calls renamed to the annotation's targets - the kernel shapes' stamp).
Tuning POLICY lives here; tuned VALUES live in the box's sidecar, never in source. The framework is
OPTIONAL: `llvm_tune` comes through the `tune_framework` group, empty in a build that did not
configure dasLLVM, and every use of it sits behind `static_if (typeinfo module_exists(llvm_tune))`,
so a build without dasLLVM opens no sidecar, reads no policy env and reports nothing - `[tuned]`
stamps the declared `fallback=` (or `DEFAULT_PERM`) and the AST rewrite is unchanged.
`box_profile_verdict_at` answers the reason NAME rather than llvm_tune's enum for the same reason: a
signature cannot be gated. - **`dasllama_tune_scope.das`** - the one `[tune_scope]` declaration
(tuner, covered modules, version pin, shipped defaults), in a module every kernel module requires: a
kernel reads its class entry in the defaults profile at its own compile, so the scope must be on the
AST first.