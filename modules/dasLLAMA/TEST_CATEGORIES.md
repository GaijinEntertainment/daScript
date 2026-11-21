# dasLLAMA test categories

A category is a set of model files no test run opens unless the run names it. A model sits in a
category when a smaller model of its family takes the same paths: the per-PR suites run the small
one, and the category holds the cells only the large one reaches. Where a family has no small
model, the small one is a cut of the large file to its first blocks - and its NextN draft block,
renumbered after them, where the file has one (`harness/gguf_cut_layers.py`) - which keeps
every path but the depth. The table is `tests/_model_categories.das`; `tests/REVIEW.das` holds this file to it.

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
| `qwen35-4b` | `Qwen3.5-4B` | the deltanet hybrid at the 4B's 2560-wide rows over thirty-two layers: the CUDA driver's forced feed against the CPU chain, the Metal MTP rail's verify rows against the plain GPU step |
| `qwen35-9b` | `Qwen3.5-9B` | the deltanet hybrid on K-quant recurrent projections: the N-row recurrent form's Q8_K feed at every recurrent head, batched and past the unsplit position, its K-quant draft head through the pool's moves, the Vulkan census's gated 256-key decode attention, the Metal MTP rail's verify rows |
| `qwen35-27b` | `Qwen3.6-27B`, `Qwen3.8-27B-Q`, `Qwen3.8-27B-UD`, `Qwen3.8-27B.i1`, `Qwen3.8-27B-4bit`, `Qwen3.8-27B-DFlash2`, `mtp-Qwen3.6-27B`, `mtp-Qwen3.8-27B` | the deltanet hybrid at its sixty-four trunk layers: the NextN draft block at layer index 64 under self-speculative decode, the DFlash block drafter's rounds on the 27B trunk, the NextN head files' architectures, the tokenizer.json twin of the 27B's GGUF, the Metal MTP rail on the 27B |
| `qwen15-moe` | `Qwen1.5-MoE-A2.7B-Chat` | the shared-expert routed block at its twenty-four blocks: the host twin's model-level matrix - the routed experts requantized to one format a file, routed layers' experts pinned to the host under the streamed prompt, the hot pool and the expert cache on twenty host layers |
| `qwen3moe-30b` | `Qwen3-30B-A3B-Instruct-2507` | the routed block with no shared expert at its forty-eight blocks: the host-layer split a strict 32768 context forces on a 16 GB card - the split token command and the cut window chain against the CPU chain, a 30-layer ask past the forced count, a two-token prompt on the window chain, layer 0's mirror rows the cut chain writes |
| `qwen35moe-35b` | `Qwen3.6-35B-A3B-UD`, `Qwen3.6-35B-A3B-MTP`, `Qwen3.6-35B-A3B-NVFP4` | the deltanet hybrid MoE at its forty layers: the host split a strict 131072 context forces on a 16 GB card against the CPU chain, the published NVFP4 file's attention and expert sites on the nv4 kernels beside four host layers, the Metal support matrix row's generate token for token against the CPU truth, the Metal MTP rail's sampled selection landed on the device against the rows-landed stream |
| `qwen3-4b` | `Qwen3-4B-Instruct-2507-Q8_0` | the dense qwen3 decoder at the 4B's 2560-wide rows over thirty-six layers: the CUDA driver's forced feed with the fused q/k-norm rope pass against the CPU chain |
