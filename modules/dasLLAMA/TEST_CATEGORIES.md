# dasLLAMA test categories

A category is a set of model files no test run opens unless the run names it. A model sits in a
category when a smaller model of its family takes the same paths: the per-PR suites run the small
one, and the category holds the cells only the large one reaches. Where a family has no small
model, the small one is a cut of the large file to its first blocks
(`harness/gguf_cut_layers.py`), which keeps every path but the depth. The table is
`tests/_model_categories.das`; `tests/REVIEW.das` holds this file to it.

Run a category when the work touches one of its topics - on this box first, on a pod when the
change reaches a device this box has none of:

```
./bin/daslang -jit modules/dasLLAMA/tests/run.das -- --category <name>
```

A category run opens its categories' files and no other model: every cell on another model skips,
and so does every category cell in a run that does not name it.

## Categories

| Category | Model files: a name that starts with | Run it when the work touches |
|---|---|---|
| `gemma4-e4b` | `gemma-4-E4B`, `mmproj-gemma-4-E4B` | the gemma-4 audio embedder widening 1536 to a 2560 decoder, the gemma-4 vision projector at 2560 with no std bias |
| `gemma4-dense` | `gemma-4-12B-it`, `gemma-4-12b-it`, `gemma-3-4b` | the dense gemma-4 decoder at its forty-eight blocks: the frozen greedy fixtures against the reference, the resident drivers' teacher-forced perplexity against the CPU chain, the Metal counting generates and batched decode token for token against the CPU truth on the K-quant and Q8_0 planes, the image chat on the 12B; the gemma-3 dense base at the 4B's wider geometry, and the image chat on it |
| `gemma4-31b` | `gemma-4-31B`, `mmproj-gemma-4-31B` | the Metal residual step past its 4096-float slab, on a decode of the 31B's 5376-wide rows |
| `gptoss-20b` | `gpt-oss-20b` | the gpt-oss decoder at its twenty-four blocks: the frozen greedy fixtures against the reference, short and with the sliding window engaged; a cold planar mint streaming the native-MXFP4 expert planes under their bytes |
| `gemma4-moe` | `gemma-4-26B-A4B-it`, `gemma-4-26B_q4_0`, `mtp-gemma-4-26B`, `mmproj-gemma-4-26B` | the routed gemma-4 decoder at its thirty blocks: the frozen greedy fixtures against the reference, the resident drivers' teacher-forced perplexity against the CPU chain on the IQ, K-quant and MXFP4 expert stacks and with routed layers' experts on the host under a pinned plan room, the IQ4_XS stack only the last block carries, a cold planar mint streaming the native-MXFP4 expert planes, the QAT Q4_0 file's repacked expert planes, the NextN draft head, the DFlash block drafter's rounds on the routed trunk |
