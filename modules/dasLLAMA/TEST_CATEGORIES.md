# dasLLAMA test categories

A category is a set of model files no test run opens unless the run names it. A model sits in a
category when a smaller model of its family takes the same paths: the per-PR suites run the small
one, and the category holds the cells only the large one reaches. Where a family has no small
model, the small one is a cut of the large file to its first blocks - and its NextN draft block,
renumbered after them, where the file has one (`harness/gguf_cut_layers.py`) - which keeps
every path but the depth. The table is `tests/_model_categories.das`; `tests/REVIEW.das` holds this file to it.

Every model file past the large tier (`MODEL_TIER_CEILING_BYTES`, 6 GiB) is a category's, so no run
opens it unless it names the category - the full suite (`DASLLAMA_PARITY_FULL=1`) included, which
lifts the size tier for files no category holds. `test_run_suites.das` fails a stocked file the
tests or the spec table name past the line that no category holds, `test_model_specs.das` a spec
or provenance row whose `bytes` are.

Run a category when the work touches one of its topics - on this box first, on a pod when the
change reaches a device this box has none of:

```
./bin/daslang -jit modules/dasLLAMA/tests/run.das -- --category <name>
```

A category run opens its categories' files, the files its rows' last column names - files of no
category its decoders pair with or its cells nest in, which every run opens too - and no other
model: every cell on another model skips, and so does every category cell in a run that does not
name it.

## Categories

| Category | Model files: a name that starts with | Run it when the work touches | Opens beside its own: a tower its decoders pair with or the model whose test its cells nest in, which every run opens |
|---|---|---|---|
| `gemma4-e4b` | `gemma-4-E4B`, `mmproj-gemma-4-E4B` | the gemma-4 audio embedder widening 1536 to a 2560 decoder, the gemma-4 vision projector at 2560 with no std bias |  |
| `gemma4-dense` | `gemma-4-12B-it`, `gemma-4-12b-it`, `gemma-3-4b` | the dense gemma-4 decoder at its forty-eight blocks: the frozen greedy fixtures against the reference, the resident drivers' teacher-forced perplexity against the CPU chain, the Metal counting generates and batched decode token for token against the CPU truth on the K-quant and Q8_0 planes, the image chat on the 12B; the gemma-3 dense base at the 4B's wider geometry, and the image chat on it | `mmproj-gemma-4-12B`, `mmproj-gemma-3-4b` |
| `gemma4-31b` | `gemma-4-31B`, `mmproj-gemma-4-31B` | the Metal residual step past its 4096-float slab, on a decode of the 31B's 5376-wide rows |  |
| `gptoss-20b` | `gpt-oss-20b` | the gpt-oss decoder at its twenty-four blocks: the frozen greedy fixtures against the reference, short and with the sliding window engaged; a cold planar mint streaming the native-MXFP4 expert planes under their bytes |  |
| `gemma4-moe` | `gemma-4-26B-A4B-it`, `gemma-4-26B_q4_0`, `mtp-gemma-4-26B`, `mmproj-gemma-4-26B` | the routed gemma-4 decoder at its thirty blocks: the frozen greedy fixtures against the reference, the resident drivers' teacher-forced perplexity against the CPU chain on the IQ, K-quant and MXFP4 expert stacks and with routed layers' experts on the host under a pinned plan room, the IQ4_XS stack only the last block carries, a cold planar mint streaming the native-MXFP4 expert planes, the QAT Q4_0 file's repacked expert planes, the NextN draft head, the DFlash block drafter's rounds on the routed trunk |  |
| `qwen35-4b` | `Qwen3.5-4B` | the deltanet hybrid at the 4B's 2560-wide rows over thirty-two layers: the CUDA driver's forced feed against the CPU chain, the Metal MTP rail's verify rows against the plain GPU step |  |
| `qwen35-9b` | `Qwen3.5-9B` | the deltanet hybrid on K-quant recurrent projections: the N-row recurrent form's Q8_K feed at every recurrent head, batched and past the unsplit position, its K-quant draft head through the pool's moves, the Vulkan census's gated 256-key decode attention, the Metal MTP rail's verify rows |  |
| `qwen35-27b` | `Qwen3.6-27B`, `Qwen3.8-27B-Q`, `Qwen3.8-27B-UD`, `Qwen3.8-27B.i1`, `Qwen3.8-27B-4bit`, `Qwen3.8-27B-DFlash2`, `mtp-Qwen3.6-27B`, `mtp-Qwen3.8-27B` | the deltanet hybrid at its sixty-four trunk layers: the NextN draft block at layer index 64 under self-speculative decode, the DFlash block drafter's rounds on the 27B trunk, the NextN head files' architectures, the tokenizer.json twin of the 27B's GGUF, the Metal MTP rail on the 27B |  |
| `qwen15-moe` | `Qwen1.5-MoE-A2.7B-Chat` | the shared-expert routed block at its twenty-four blocks: the host twin's model-level matrix - the routed experts requantized to one format a file, routed layers' experts pinned to the host under the streamed prompt, the hot pool and the expert cache on twenty host layers |  |
| `qwen3moe-30b` | `Qwen3-30B-A3B-Instruct-2507`, `Qwen3-30B-A3B-Q8_0`, `Qwen3-30B-A3B-Thinking-2507`, `Qwen3-30B-A3B-NVFP4-pure` | the routed block with no shared expert at its forty-eight blocks: the host-layer split a strict 32768 context forces on a 16 GB card - the split token command and the cut window chain against the CPU chain, a 30-layer ask past the forced count, a two-token prompt on the window chain, layer 0's mirror rows the cut chain writes; the frozen greedy fixtures of the base Q8_0 and the Thinking files |  |
| `qwen35moe-35b` | `Qwen3.6-35B-A3B-UD`, `Qwen3.6-35B-A3B-MTP`, `Qwen3.6-35B-A3B-NVFP4`, `Qwen3.6-35B-A3B-Q8_0`, `Qwen3.5-35B-A3B-UD` | the deltanet hybrid MoE at its forty layers: the host split a strict 131072 context forces on a 16 GB card against the CPU chain, the published NVFP4 file's attention and expert sites on the nv4 kernels beside four host layers, the Metal support matrix row's generate token for token against the CPU truth, the Metal MTP rail's sampled selection landed on the device against the rows-landed stream; the frozen greedy fixtures of the Q8_0 file and the Qwen3.5 i-quant files |  |
| `flash-next` | `Qwen3.8-Flash-Next`, `mtp-Qwen3.8-Flash-Next` | the hyper-connection hybrid MoE with the n-gram side input at its three shards and split NextN head: the whole-model resident driver's window chain, split token command, NextN draft and speculative round against the CPU chain, the continuations from an adopted checkpoint, the parity file's spec row, the Metal support matrix's served row and the MTP parity rows, the scheduler's cells on the carrier |  |
| `qwen3-4b` | `Qwen3-4B-Instruct-2507-Q8_0` | the dense qwen3 decoder at the 4B's 2560-wide rows over thirty-six layers: the CUDA driver's forced feed with the fused q/k-norm rope pass against the CPU chain |  |
| `qwen3-8b` | `Qwen3-8B-BF16` | the dense qwen3 decoder on bf16 planes: its frozen greedy fixture |  |
| `qwen3coder-30b` | `Qwen3-Coder-30B-A3B` | the routed qwen3 Coder files at Q8_0, Q4_K_M and UD-Q3_K_XL: their frozen greedy fixtures |  |
| `qwen3coder-next` | `Qwen3-Coder-Next` | the qwen3next file layout at 512 experts: the whole-model driver's device select past 256 experts with the first routed layers' experts on the host, one window against the CPU chain; the loader's grouped-to-tiled permutes under its frozen greedy fixture |  |
| `qwen3omni-30b` | `Qwen3-Omni-30B` | the Omni MoE thinker under its towers: the qwen3a AuT tower's greedy audio transcription token for token with llama-mtmd-cli, the image chat turn and a text turn recalling it through the qwen3v tower | `mmproj-Qwen3-Omni-30B` |
| `qwen3vl-8b` | `Qwen3VL-8B` | no cell loads the file: its spec row and provenance pin, the board's vision row |  |
| `qwen2audio-7b` | `qwen2audio-7b` | the qwen2audio chat template's audio markers against the decoder's own vocab |  |
| `gemma3-12b` | `gemma-3-12b-it` | the gemma3v image turn at the 3840-wide projection on the 12B pair, the Metal family matrix's K-quant gemma3 row on k4 and k6 planes and the Q6_K token table, the 12B's frozen greedy fixture | `mmproj-gemma-3-12b` |
| `llama31-8b` | `Meta-Llama-3.1-8B-Instruct-Q8_0` | the Metal dim ladder at the 8B's 4096-wide rows, nested in the 1B's tests: batched decode past B=4 on the q8 GEMM tier in the decode and prefill parity files, the support matrix's 8B matrix cell and batched dim ladder | `Llama-3.2-1B-Instruct-Q8_0` |
| `llama33-70b` | `Llama-3.3-70B` | the Metal dim ladder at the 70B's 8192-wide rows, nested in the 1B's tests: the decode parity's 70B decode served on the GPU, the support matrix's 70B K-quant matrix cell and batched dim ladder on the kq mul_mm twin | `Llama-3.2-1B-Instruct-Q8_0` |
| `mistral-7b` | `Mistral-7B-Instruct-v0.3-Q8_0` | the Mistral v0.3 decoder's frozen greedy fixture, the v0.3 vocabulary's byte pieces past its 768 control tokens |  |
| `mistral3-24b` | `Mistral-Small-3.1-24B` | the mistral3 decoder on the tekken vocabulary: its frozen greedy fixture |  |
| `glm45-air` | `GLM-4.5-Air` | the only non-recurrent NextN file, with qkv bias at the draft block: self-speculative decode token for token against plain decode, the recr_mask-zero reject branch forced on every third draft |  |
