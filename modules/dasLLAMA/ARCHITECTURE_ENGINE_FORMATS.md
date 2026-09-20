# dasLLAMA Architecture - format, load and CPU-tier file charters

Companion to `ARCHITECTURE.md` beside `ARCHITECTURE_ENGINE.md`; section numbers are
`ARCHITECTURE.md`'s.

### 1.2 Formats and data movement

- **`dasllama_plane.das`** - the borrowed-plane vocabulary: a plane is a pointer into a prepared
  image plus its element count; the image owns the bytes, a carrier owns nothing but its backing.
  Requires nothing in dasllama - the image rail binds planes, every carrier holds them.
- **`dasllama_kqformat.das`** - format IDENTITY: the `KqFmt` enum, the per-format descriptor table
  (plane strides, block geometry, stream codes), format predicates, and the shared decode
  tables the grid and codebook formats key off - each as a builder function (`iq3s_grid()`,
  `iq4nl_lut()`) for kernels that may run on a team lane, plus a global twin for tests,
  oracles and the emitter's constant bake. It requires nothing else in
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
  share the format's block byte geometry, so a layout change can never span modules. These are
  per-token and per-tokenxposition `[tuned]` hot kernels, not load-time converters. The tq4 FWHT
  rotation and sign vector live with their family. Codec DISPATCH (`KVDtype`) stays at common's
  `kv_store_row`/`kv_load_row`/`kv_dot`/`kv_axpy` seam; the f16 row converts are the generic pair
  in `dasllama_convert` because they are dual-use beyond the cache.
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
  un-repack (`q8_region_to_metal_blob`, the ASR-decoder driver's upload form), and the refusal
  half (`can this model take the blob form`) split out so the image writer can commit without
  loading.
- **`dasllama_tokenizer.das`** - the tokenizer facade: backend selection off the GGUF metadata and
  the one encode/decode/piece surface models and the chat layer call. Re-exports both backends, so
  a consumer requires this file and never picks a backend by hand.
- **`dasllama_spm.das`** - the SentencePiece backend: score-greedy merges over vocab pieces (Llama-2, Phi-3, Gemma),
  the unigram Viterbi arm over the same pieces (`unigram_encode`, GGUF `"t5"`, sec.1.7d's tokenizer), `<0xXX>` byte fallback.
- **`dasllama_bpe.das`** - the byte-level BPE backend (Llama-3 / tiktoken family): vocab load, the
  GPT-2 byte alphabet, ranked merges, encode/decode. Split from SPM because the two algorithms
  share no state; a third merge algorithm gets a fourth file. Two sanctioned family-name tests
  live here rather than in `dasllama_pretok`: the `pre`-name selector inside `bpe_encode`, and the
  gemma-4 newline-run split in `bpe_encode_spm_space`; `load_bpe_tokenizer_gguf`'s per-family
  metadata defaults are the third and last. A GGUF without `tokenizer.ggml.add_bos_token` takes
  upstream's per-pre default (`bpe_pre_adds_bos`): the pres on its list - the llama3 family,
  tekken, chameleon, a few smaller ones - prepend BOS, every other pre - qwen2, qwen35, gpt-2,
  gpt-4o among them - prepends none. The
  default matters: a Qwen hybrid fed a leading `<|endoftext|>` degenerates for the whole
  generation, and the unsloth Qwen3.8 conversions omit the key.
- **`dasllama_pretok.das`** - the pre-tokenizer: one hand-compiled split function per family
  (llama3/qwen2/qwen35, gpt-2, gpt-4o, tekken), selected by the BPE `pre` name. Regex-port growth
  lands here, never in the merge engine - the two change for different reasons (new model family
  vs. algorithm work). Every arm with an on-disk upstream corpus vocab is gated by its case in
  `test_tokenizer.das` (llama3, qwen2, qwen35, gpt-2); tekken has no corpus case, and gpt-4o is
  pinned by frozen ids in `test_parity.das` only.

### 1.3 The load and image rail

**`dasllama_load.das`** is the GGUF load walk: metadata to `Config`, the plane layout, disk-format
detection, the eager and streamed conversion ladders, and the load entry points. It owns nothing the
forward path touches at run time - a loaded `Model` is the whole handoff - and it requires
`dasllama_common` back for `Model`/`Session`, so the transformer umbrella requires it `public` and
breaks the cycle. That re-export is what keeps every consumer on the facade. Every big-weight
conversion is accounted as it runs (`conv_account`, on both the eager and the streamed ladder),
and a load that converts ends with the report: the file's bytes, then each (disk type ->
conversion kind) as bytes on disk against bytes served with the delta, biggest overhead first,
and the count of tags the row-length rule demoted to the q8 rail (each demotion logged by
name as it is decided). The delta column is where a served form costs more than the file - a
decoded scale row, an f32 copy of a quantized token table - and it is read as a number, not
inferred from image sizes.

- **`dasllama_image.das`** - the prepared-model `.dlim` rail, and it is ONE rail
  (`ARCHITECTURE_IMAGE.md` sec.2.1). Nothing
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

### 1.4 CPU kernel tiers

- **`dasllama_math.das`** - the numeric ABSTRACTION: typedefs, active backend pointers, public
  wrappers, dispatch shaping. Kernels themselves live in a tier file; a kernel body here is a
  placement defect.
- **`dasllama_math_default.das`** - the portable backend, always registered, always correct,
  out-ranked by any platform tier.
- **`dasllama_math_aarch64_neon.das`** - the arm64 SDOT/tbl tier. Its `[init]` never fires
  off-arch (`ARCHITECTURE_INVARIANTS.md` sec.3, three-layer safety model), so an intrinsic here
  needs a correct scalar fallback body, not a guard at the call site.
- **`dasllama_math_accelerate.das`** - the Accelerate/BNNS float tier (AMX on M1-M3, SME on M4+),
  for genuinely-float planes only. BLAS-for-quant is ruled out structurally
  (`ARCHITECTURE_INVARIANTS.md` sec.3).
- **`dasllama_math_gen.das`** / **`dasllama_gemm_gen.das`** / **`dasllama_gemm_schema.das`** /
  **`dasllama_gemm_register.das`** - the generated GEMM tier: the runtime registration, the tile
  generator, the layout/perm schema shared by generator and runtime, and the `[tune]` family
  registration. A hand-written tile that the generator could emit belongs in the generator.
- **`dasllama_tune.das`** - the per-box loop-hint tuner (`[tuned]` / `[dasllama_grid]`). Tuning
  POLICY lives here; tuned VALUES live in the box's sidecar, never in source. The framework is
  OPTIONAL: `llvm_tune` comes through the `tune_framework` group, empty in a build that did not
  configure dasLLVM, and every use of it sits behind `static_if (typeinfo module_exists(llvm_tune))`, so a build without dasLLVM opens no
  sidecar, reads no policy env and reports nothing - `[tuned]` stamps the declared `fallback=`
  (or `DEFAULT_PERM`) and the AST rewrite is unchanged. `box_profile_verdict_at` answers the
  reason NAME rather than llvm_tune's enum for the same reason: a signature cannot be gated.
- **`dasllama_tune_scope.das`** - the one `[tune_scope]` declaration (tuner, covered modules,
  version pin, shipped defaults), in a module every kernel module requires: a kernel reads its
  class entry in the defaults profile at its own compile, so the scope must be on the AST first.
