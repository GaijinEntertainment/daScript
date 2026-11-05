# dasLLAMA Architecture - the safetensors snapshot source and the affine members

Companion to `ARCHITECTURE_ENGINE_FORMATS.md`; a section is cited by its anchor. This document carries the
loader's second source - a HuggingFace snapshot directory read in place of a GGUF - and the five affine
members (four per-32, one 6-bit) that serve its affine-quantized planes from their own bytes.

### A snapshot directory loads through a synthesized GGUF meta {#mlx-snapshot-source}

A path is a snapshot when it is a directory holding `config.json` (`hf_snapshot`). Its weight files are
`model.safetensors.index.json`'s distinct files, sorted, else `model.safetensors`; each maps as a shard,
and each file's 8-byte header length plus its JSON header gives the data section's start, which need not
be 4-byte aligned - every plane is copied out, never viewed in place.

`hf_snapshot_meta` builds the `GGUFMeta` the GGUF loader already walks, so `load_gguf_parsed` runs on
it: tensors renamed to their GGUF names (`hf_gguf_stem`; a vision tower or draft head is not read), dims
reversed to GGUF order, and the config's geometry written as the `{arch}.*` KVs a converter writes. The
KVs, the chat template, the tokenizer KVs and the synthetic tensors (llama3's `rope_freqs.weight`) live in
one extra buffer, the meta's last shard: its bytes are the `bytes` every KV getter reads. The family a
`model_type` loads as is `hf_family` (`llama`; `qwen3_5` as qwen35, `qwen3_5_moe` as qwen35moe and `qwen4_exp` as qwen4exp,
their geometry under `text_config`). A family that names a draft-head prefix (`mtp_prefix`) reads that head too.

An affine triplet's group is its own `quantization` entry's, else the block's default (`hf_quant_of`), and its width follows
from the shapes under that group. A checkpoint that stores its RMSNorm gains zero-centred - w where the forward multiplies by
1 + w - is told by its first trunk q_norm's mean (near 0 against near 1 baked; `hf_zero_centred`), never by model type; there
every unit-offset gain (`HF_UNIT_NORMS`; the deltanet's gated `ssm_norm` is stored plain both ways) is synthesized as the f32
1 + w a converter writes.

A llama checkpoint keeps its q/k rows in the half-split rotary order (dims i and i + head/2 pair), where
the converter permutes them to adjacent pairs; the meta marks it (`hf_rope_halves`) and the load ropes
those rows as NEOX pairs, so no row moves. The streamed load does not arm on a snapshot (the transcode
reads three tensors per plane); the image mint converts eagerly.

The tokenizer is tokenizer.json written as the GGUF tokenizer KVs: the vocab padded to the model's
`vocab_size` with UNUSED `[PAD<i>]` rows, the merges as `left right` strings, the converter's token types
(an added token is control where marked special or spelled `<|...|>`, user-defined otherwise), and the
pre-tokenizer its split regex names (`hf_bpe_pre`: `\p{N}{1,3}` is llama-bpe, `\p{M}` qwen35, `\p{N}`
qwen2), tokenizer_config.json's `pretokenize_regex` taking precedence over tokenizer.json's own.

### A routed snapshot's experts are expert-major affine stacks {#mlx-moe-snapshot}

`qwen3_5_moe` loads as qwen35moe. `put_moe_kvs` writes the converter's expert KVs off `text_config` - `num_experts`,
`num_experts_per_tok`, `moe_intermediate_size`, `shared_expert_intermediate_size`, and `norm_topk_prob` where the config
spells it (the family renorms by default, as mlx-lm does); a config that routes a layer subset (`decoder_sparse_step`,
`mlp_only_layers`) is refused, since the qwen35moe graph routes every layer. A routed config carries no `intermediate_size`,
so the meta writes no dense width.

The expert stacks `mlp.switch_mlp.{gate,up,down}_proj` are 3-D triplets, codes `[E, out, in * bits / 32]` and pairs
`[E, out, in / 64]`, presented as one logical `[in, out, E]` tensor (`ffn_{gate,up,down}_exps`). Their rows run expert after
expert, so the stack's one contiguous load lands every expert at its slot and `gguf_transcode_mlx`'s whole-row walk serves
them with no arm: an expert row keeps the dense planes' treatment, its codes verbatim and its pairs interleaved. The shared
expert's projections are `ffn_{gate,up,down}_shexp`, dense planes in their own member.

The router (`mlp.gate`, `ffn_gate_inp`) and the shared expert's gate vector (`mlp.shared_expert_gate`, `ffn_gate_inp_shexp`)
are the one converted plane. Every backend's router site reads f32 rows (the CPU's top-k GEMV off the fblob, the Metal f32
tile, the device routers), as every converter writes them, and no site of theirs takes a `KqFmt` member; the checkpoint
quantizes both as 8-bit members, so `gguf_read_tensor_f32` reads them through the member's own transcode and block dequant -
`scale * q + bias`, the value mlx-lm's dequant gives. On the 35B-A3B snapshot that is 84 MB of f32 against 18.6 GB served
(0.45%).

A text-model or draft-head tensor that maps to no GGUF name and no synthesis read panics naming it (`hf_refuse_unmapped`); a
vision tower is not read. mlx-community snapshots ship no MTP head, so a routed snapshot loads with no NextN layer.

### A qwen4_exp snapshot maps its mixers, side input and draft head onto the converter's planes {#mlx-qwen4exp-snapshot}

`qwen4_exp` (Qwen3.8-Flash-Next) loads as qwen4exp: the qwen35moe KVs, then `put_qwen4exp_kvs` - the hyper-connection
geometry, the indexer's heads, key width and budget, its compress ratio on the `layer_types` full-attention layers (0
elsewhere, one entry a trunk layer), the n-gram side input's layer (`ple_layer_ids` counts from 1), order, heads, taps and row
width (`ple_embed_dim / ((ngram_size - 1) x heads_per_ngram)`), and its hash constants off the checkpoint's I64
`ple_embedding.*` tensors, which only a KV reads. A deltanet out-gate other than sigma(z) is refused.

| checkpoint (under `language_model.model.`, the head under `mtp.`) | GGUF | how |
|---|---|---|
| `layers.N.{attn,mlp}_hyper_connection.input_mix_weight_{down,up}` | `blk.N.hc_{attn,ffn}_{down,up}` | the triplet, rows as stored |
| `layers.N.*_hyper_connection.hc_norm`, `q_norm`, `k_norm`, `indexer.{q,k}_layernorm`, `ple.norm_*` | `hc_*_norm`, `attn_{q,k}_norm`, `indexer.{q,k}_norm`, `ple_norm_*` | 1 + w, f32 |
| `layers.N.*_hyper_connection.block_inject_weight` | `blk.N.hc_*_inject` | BF16 bytes as stored |
| `layers.N.self_attn.indexer.index_qk_proj` | `indexer.q_proj` + `indexer.k_proj` | `hf_indexer_split`: rows [0, heads x dim) and the next dim rows, one row offset into all three planes |
| `layers.N.ple.{key,value}_proj`, `ple.conv1d` | `ple_key`, `ple_value`, `ple_conv1d` | the triplets; the taps' BF16 bytes, the unit dim dropped |
| `hyper_connection_mixer.*` | `output_hc_{norm,down,up}` | as the layer mixers |
| `mtp.layers.0.*` | `blk.<trunk>.*` | the layer map under the head's block |
| `mtp.pre_fc_norm_{embedding,hidden}`, `mtp.hyper_connection_mixer.*` | `nextn.{enorm,hnorm}`, `nextn.hc_head_*` | 1 + w; the mixer as above |
| `mtp.fc_embedding` + `mtp.fc_hidden` | `nextn.eh_proj` | `hf_mtp_eh_proj`: each row the embedding triplet's row then the hidden one's, codes and 64-group pairs verbatim in the meta's buffer |

The draft head sets the meta's `nextn_sidecar`, so the KVs keep the trunk's block count as a split head's do.

The n-gram table's row shards (`ple_embedding.ngram_embedding.shards.<i>`, 4-bit codes in 32-groups) are one
`per_layer_token_embd` of the loader type `GGML_TYPE_NGRAM_A4G32` over their bytes in place (`hf_ngram_table`): every shard
the same rows, its codes, scales and biases back to back, the shards back to back in one file, 20 bytes a 32-group. The load
maps that file for the model's life and views the table in it (`map_ngram_table`; the finalizer closes the mapping), reading
it ahead at map as the image does its table plane, since the gather's rows are random pages; a minted image carries the same
bytes as its table plane and the rows a shard as `ngram_shard_rows`. Row r gathers off shard r / rows at its local row
(`ngram_row_a4g32`): element k is the low nibble of code byte k / 2 for even k, the high one for odd, as scale x q + bias.

The mixers' down/up planes and the n-gram key/value projections carry plane tags on the native q8 rail like every dense plane
(`hc_down_fmt` / `hc_up_fmt` by site - a layer's two, then the head mixer's, then the draft head's - and `ple_kv_fmt`): a q8
GGUF tags q8, the pack its 6- and 8-bit members, and the CPU mixers and side input read each plane in its tag. Metal serves
every tag that has a dense form (`#mlx-metal-forms`); a plane none reads declines the device's decode and prefill as `graph`,
naming the site (`hc_planes_metal_unserved`), and the CPU serves the call. Off the native rail no tags exist, and
`hc_planes_unserved` declines an affine mixer at config time, before a weight is read.

### The affine planes are the triplet's codes and its group pairs {#mlx-affine-planes}

A quantized linear is three tensors: U32 codes `[out, in * bits / 32]`, and F16 or BF16 scales and biases
`[out, in / 64]`; a weight is `scale * q + bias` with q unsigned. 4-bit element k sits at bits 4k..4k+3 of
the little-endian code words - byte k / 2, the low nibble first, never Q4_0's k / k + 16 pairing - and an
8-bit element is byte k. A 6-bit element k sits at bits 6k..6k+5 of the row's little-endian bit stream: four codes
fill three bytes, sixteen fill three words, so a 32-block is six whole words and a code past bit 26 of a word (bit 2
of a byte) carries into the next one. The meta presents the triplet as one logical `[in, out]` tensor of a loader
type (`GGML_TYPE_MLX4` .. `MLX8B`, `MLX6B`, ids outside ggml's range) beside `<name>.scales` / `<name>.biases`.

`gguf_transcode_mlx` copies each row's code bytes verbatim into the member's quant plane (`MLX4_QB`,
`MLX6_QB` or `MLX8_QB` a 32-block) and interleaves the row's halves into the scale plane, scale then bias, one 4-byte
pair a 64-group (`MLX_PAIR_BYTES`); a block's 2-byte scale unit (`MLX_SB`) is half its group's pair, and a
block reads the pair at its even neighbour's offset. A meta permute (`GGUFRowPermute`) reorders whole rows
or whole 64-group column segments on the way in, leaving every group's bytes untouched. The members are
the 4- and 8-bit code widths under the two half types - mlx4 / mlx8 (F16), mlx4b / mlx8b (BF16) - and the 6-bit
width under BF16, mlx6b, the width the Qwen3.8-Flash-Next pack gives its attention planes and its hyper-connection
mixes; a 6-bit triplet under F16 scales has no member, and the snapshot loader refuses it by name.

The disk-order dot (`dot_mlxq8_scalar`) takes a 32-block's exact code sum `isum = Sum q*a` and activation sum
`asum = Sum a`, and folds `scale * isum + bias * asum` by the block's Q8_0 scale. The expanded row the MAC runs
over holds the 4- or 6-bit code, or the 8-bit code - 128 (an int8), and its fold adds the level bias back as
`128 * asum` inside the integer term, so the expanded MAC is bit-identical to the scalar dot. The planes sit
in disk order on every backend (`b32_layout_mr` is 1): the dense, table and classifier sites serve on the
portable per-32 kernels, and no generated tile or panel stamp exists for the members. Their registries carry the
row-major family alone (`<member>q8_rm_gemv_gen`, its tile and witness companions): the same disk-order dot,
generated by `rm_aff_gen` with the 8-bit codes riding as signed levels and `128 * asum` added back - the contract
every format but q8 keeps, raced like every row-major family; `rm_dev_serves` names no member, so the one-copy
rail never dispatches it until a discrete home stamps the member. The discrete GPU homes (Vulkan, CUDA) stamp none of them: every tile ladder of theirs
skips them (`kq_discrete_unstamped`), and the GPU tier's upload and the whole-model driver decline the model
with the site and member named (`discrete_unstamped_plane`); the CPU serves it there. Metal serves them.

### The Metal home serves the members on the per-32 family's forms {#mlx-metal-forms}

On Metal a member is a per-32 plane like q5_1: `kq_fmt_gpu_supported` admits it, both planes bind verbatim
through `b32_planes_of` (`off % 256` keeps the 16- or 32-byte quant and the 2-byte scale binds 16B-aligned,
`metal_blob_off_ok`), the metal flavor rebakes no scale plane, so the `.dlim` metal lane holds the same bytes as
the CPU lane, and every per-32 ladder names the member: the one-row GEMV, its K-split stamp and its wide stamp
(`ARCHITECTURE_GPU_QUANT_PLANES.md#metal-b32-wide-gemv`), the expert-indexed
GEMV, the two-, four- and eight-column small-batch stamps and their K-split twins (`B32_STAMP_FMTS`,
`B32_GEMV_STEMS`, `B32_MVB_STEMS`, `B32_MOE_GEMV_STEMS`), the token-table gather, the simdgroup base mul_mm and its
split-K stamp (`B32_MM_FORMS`), the dense tensor twins behind the member's `kq_mulmm_<member>` crown and its race
(`race_kq_mulmm_b32`), the dev-W dequant pass, and the rows8 matrix-unit form at five to eight rows
(`ARCHITECTURE_GPU_MTP_DECODE_ROWS.md#rows8-affine`). Each form is its template's affine arm, selected by three
template constants: `AFF` (codes verbatim, `scale * q + bias` off the block's 64-group pair - the halves at the
group's even block, `blk & ~1`), `A8` (8-bit codes, eight words a block) or `A6` (6-bit codes, six words a block)
and `BF` (`BFS` on the tensor scaffold: the scale plane bound as BF16 halfwords, `uint_bits_to_float(h << 16)`,
where the F16 members bind it as halves).
A GEMV lane holds elements `4it .. 4it + 3` in every per-32 format - word `it / 2` at shift `16 (it % 2)` on a
4-bit block, word `it` on an 8-bit one (`lane_word`, `lane_shift`), and on a 6-bit block the 24-bit field at bit
`24 it`, two words where it straddles one (`lane_code`) - so the x view, the walk and the small-batch columns are
q5_1's, and a batched row is the one-row GEMV's bit for bit (`b32_mvb_gate`). A 6-bit form that decodes a 16-element
half (the gather, the tiles, the dev-W pass) reads the half's three words and takes code t at bit `6t` of them
(`mlx6_el_code`); its wide form masks the field's codes in place at bits 0, 6, 12 and 18 and divides those powers
of two out of x, as the 4- and 8-bit wide forms do theirs.

mlx6b serves the dense, table and classifier sites on every form above, and no routed form: it stamps no
expert-indexed GEMV and no routed tile, so `moe_fmt_metal_served` and `moe_site_ok` refuse a 6-bit expert stack
(and with it a 6-bit deltanet beta/alpha plane, whose window rides the expert-indexed GEMV) and the CPU serves it.

A hyper-connection mixer reads its two planes in the site's tags (`hc_site_fmts`). At one row a q8, mlx4b, mlx6b or mlx8b
pair (`hc_mixer_fused_fmt`) takes the fused pair - the down under the grouped norm with the inject rows, the up over silu(lo /
hc) with the gated mean - as stamps of the q8 one-row templates whose fill is the member (`MetalGemvHcNT` on
`MetalQ8GemvRsT`, `MetalGemvSgLoT` on `MetalQ8GemvT`, the affine fill under `AFF`): the per-32 family cannot carry q8, whose
34-byte blocks are no whole words. Any other pair runs the staged chain - the grouped norm, the down site, silu(lo / hc), the up
site, the gated stream mean - each site on its tag's own forms: the one-row GEMV (the down's K split, the up's wide stamp),
the small-batch stamps at two to four rows, and the window's dense tile (`pf_enc_site_mm`). At five to eight rows a q8,
mlx6b or mlx8b pair (`metal_hc_mix8_fmt`) takes the eight-row mixer's four dispatches, the down and up arms the rows8
affine arms under `KSPL` and `HCMIX` (`ARCHITECTURE_GPU_MTP_DECODE_HC.md#metal-hc-mix8`). The n-gram key and value
projections ride the same one-row, small-batch and window forms in their tags. `hc_planes_metal_unserved` declines a plane
whose tag has no one-row GEMV over its K or whose rows are off the window tile's 64.

The GEMV forms fold as the CPU's dot does - `scale * sum(q * x) + bias * sum(x)` over a lane's four codes
(`blk_dot`), the raw codes the lane's decode (`blk_w`) - against f32 activations where the CPU reads Q8_0 rows; the
gather and the tile forms, whose output is a weight, decode `scale * q + bias` (`mlx_el_level`, the CPU dequant's
order). The kernel cells hold each form on an exact fixture
(pow2 scales, quarter-valued biases, the engine's own block dequant reading the planes back bit for bit) against
the CPU float product, and the served cells hold the device rows to the CPU rails' within the dense bar.

A routed stack in a member serves on the same arms (`moe_fmt_metal_served`, `moe_site_ok` at `off % 256` and whole 64-groups
an expert): the decode reads it through the expert-indexed GEMV - its wide stamp on an expert of 512 rows or more
(`ARCHITECTURE_GPU_QUANT_PLANES.md#metal-b32-wide-gemv`) - which also serves the deltanet beta/alpha window, and the window
through the routed simdgroup base (`MetalMoeMulMm<Member> : MetalMoeMulMmKqT`, the dense base's `AFF` decode) and the four
split twins (`MetalMoeMulMm<Member>{T,TH,TH128,THR}`, the dense twins' `AFF` staging on the expert walk), one `pf_moe_kq_row`
row a member and a `moe_mulmm_<member>` race. An expert's blocks start on a group's even block, since an expert holds whole
rows of whole 64-groups, so the pair read at `blk & ~1` never crosses an expert.

The routed block's fused pair takes an mlx4b or mlx8b stack (`moe_gu_aff_fmt`) on the per-32 family's stamps: the gate and up
stacks with the act in one dispatch (`MetalB32GuT`), and the down with its shared down in the same member and the combine
(`MetalB32Down2T`'s `down2_wide_body` under `AFF` and `SHAFF`). Both take the wide gathered GEMV's lane map - one row a
simdgroup, a whole block a lane, x prescaled once a block for both stacks - and not the two-row map the iq4nl32 stamps share:
the per-stack route's gathered GEMVs are wide on rows of 512 or more, and the two-row map loses the block to them by a third at
one row (gate and up 49.6 us against 37.5 on the pack's shape, the down 33.2 against 25.0), where the wide map wins it
(57.6 us against 63.4 at one row, 314 against 356 at eight; `moe_pair_race` under `DASLLAMA_MOE_PAIR_FMT=mlx4b`).
Each stack's rows are the wide GEMV's bit for bit and the slots sum in the combine's order. A stack whose shared down is in
another member - the draft head's mlx4b experts beside its mlx8b shared expert - keeps the per-stack route.
