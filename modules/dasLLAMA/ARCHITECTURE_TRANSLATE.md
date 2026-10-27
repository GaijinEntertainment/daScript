# dasLLAMA architecture - the Bergamot translation family

The companion `ARCHITECTURE.md` indexes: the translation charter (`ARCHITECTURE_TRANSLATE.md#nmt-charter`) and the
mechanisms the family implements - its two lanes (`ARCHITECTURE_TRANSLATE.md#nmt-lanes`), the oracle it is held to
(`ARCHITECTURE_TRANSLATE.md#nmt-oracle`), the device homes (`ARCHITECTURE_TRANSLATE.md#nmt-device-homes`) and the decode
step's pool (`ARCHITECTURE_TRANSLATE.md#nmt-step-lanes`).
`ARCHITECTURE_COMMON.md` (repo root) is the contract.

## File charters

### Bergamot {#nmt-charter}

- **`dasllama_nmt.das`** - the Bergamot students (Firefox Translations' Marian models): the reader of a Marian
  `model.<pair>.intgemm.alphas.bin` (the item table, the embedded `special:model.yml` checked for the one shape the family
  serves - a post-norm transformer encoder with ReLU FFNs, a decoder of SSRU, cross-attention and FFN blocks, tied
  embeddings, no SSRU norm), the vocabulary beside it through `dasllama_spm.das`'s model-file reader (`SpmModelFile`: the
  unigram pieces, the normalizer's charsmap, the special ids), the staging and the image rail (`bergamot-f32`,
  `bergamot-q8`), the forward (`nmt_encode`, `nmt_decode_step`) over the towers' row kernels and GEMMs, and the greedy
  loop (`nmt_translate_ids`, `nmt_translate`). `is_marian_model` is the bake tool's sniff. A stage tap sees every block's
  output as the oracle names it. One shared vocabulary is served; a split pair (`srcvocab` / `trgvocab`, the CJK models)
  is refused by name.
- **`dasllama_translate.das`** - the facade: `Translator` - a Bergamot student, or an LLM GGUF: one with no chat template
  (MiLMMT-46) completes its card's prompt, an instruct model answers an instruction through its own template with
  thinking off - `load_translator` (the q8 lane unless `exact`), `translator_langs`, `set_translator_langs`,
  `translator_languages`, `translator_language_name` and `translate`; it requires the Metal
  and Vulkan translation drivers so a facade program registers whichever the build carries. `dasllama_core.das` re-exports it.

## Mechanisms

### The source and the two lanes {#nmt-lanes}

The Marian file is a source, never a carrier: every load stages it into the family's own layout and serves the image.
Its block GEMMs are int8 stored `[out][in]` with one f32 `quantMult` a tensor (weight = int8 / quantMult), the embedding
the same untransposed, the static activation scales (`*_QuantMultA`) unread. The exact lane widens every weight to f32
- marian's float graph over the same values. The q8 lane requantizes the block GEMMs to Q8_0 and adds a Q8_0 copy of the
tied embedding for the classifier (`cls_off` in `qblob`), so a step reads 12 MB of classifier rather than the 49 MB f32
table; the embedding rows a step gathers stay the f32 table's on both lanes. The lexical shortlist beside the model is
not read: every step scores the whole vocabulary.

### The oracle {#nmt-oracle}

The reference is marian itself (`benchmarks/nmt/bergamot_oracle.py` over the patched `marian-decoder` the rig builds),
run greedy on one thread over the int8 weights dequantized to an f32 `.bin`: marian serves an `intgemm8` weight through
its int8 GEMMs whatever the precision flag, so the f32 file is the only way to its float graph. Its stage dumps
(the embedding, every encoder layer, each decoder step's three blocks, the logits) and tokens are the exact lane's
specification, held stage by stage and token for token. The shipped file under Firefox's `int8shiftAlphaAll` is a
quality reference only: it differs from the float graph on a third of the fixture lines, and the q8 lane is held by its
lines' agreement with the float graph, not with it.

### The device homes {#nmt-device-homes}

The Metal and Vulkan drivers (`dasllama_metal_nmt.das`, `dasllama_vulkan_nmt.das`) serve the q8 lane alone, each a whole
sentence on the device: one command buffer for the encode and every decoder layer's cross keys and values, then command
buffers of greedy steps whose argmax and state words never leave the device (`GkNmtPick`), the first buffer sized by the
sentences' running steps a source token. A step is a chain of single-row dispatches, so a device sentence is latency-bound:
back to back the GPU runs close to the CPU, and after an idle gap its clocks and wake cost it more than the CPU pays.

Both homes admit the same sentences: one source cap (`NMT_DEVICE_SRC_CAP`, 256 tokens; the Vulkan home's buffers are sized
to it) and one shape gate (`nmt_device_shape_ok`: heads of whole float4s, widths of whole 64-wide GEMM tiles, the narrower
home's lattice). A sentence a home declines runs the CPU chain, so which sentences a box serves on its GPU does not depend
on which GPU it has. A home's weights are keyed by the model's load generation (`NmtModel.gen`), so a model loaded in the
place of another re-uploads.

### The decode step's pool {#nmt-step-lanes}

A decode step is a chain of single-row GEMMs of a few hundred thousand multiply-adds each, where a team dispatch's
meeting cost rivals the work. The greedy loop narrows the pool to its widest block GEMM's multiply-adds over a lane's
work (`nmt_step_lanes` over `step_lanes`, the rule the transducer step shares), and puts it back when the sentence ends; the encoder keeps the whole
pool. Waking the parked workers for the classifier alone costs more than the classifier gains.
