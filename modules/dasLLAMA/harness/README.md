# dasLLAMA harness

Verification / eval test beds and GGUF inspection tools (not shipped with the module). The
table lists the parity/inspection core; the ~40 probe and bench beds beside them are one-off
instruments - each carries its purpose in its header comment.

| File | What it does |
|---|---|
| `parity.das` | dasLLAMA side of the token-for-token oracle check: prompt IDs in -> greedy generated IDs out (`GEN_IDS: ...`). Arch-agnostic - routes through `load_gguf` + `generate`. |
| `parity.sh` | Runs the reference oracle and `parity.das` on the same prompt and diffs the two ID streams. |
| `pod_fleet.das` | The test runner's pod fleet: `up --pods n` makes n RunPod pods on the models volume, each with its own copy of the volume's tree; `sync` copies it again after a push to the volume; `run` starts one `--shard i/n` of a `tests/run.das` command on each, the logs under `/workspace/logs/fleet/<run>/` on the volume; `status` reads every shard's `REPORT` through one pod; `down` deletes the pods the tool made and no other. The fleet file is `~/.dasllama/fleet.json`. |
| `kernel_ladder.sh` | The per-box kernel table: every format's stamped kernel (`benchmarks/matmul/kq_kernel_bench.das`, one thread, no model) against the reference exe's `test-backend-ops perf` at the same shape, decode and prefill rows, joined with the ratio. The reference binary carries the `GGML_BENCH_THREADS` define (a three-line edit of its `N_THREADS`; `backend_ops_shapes.patch` is the older, wider form). |
| `oracle/simple_ids.cpp` | The reference oracle - links against a reference-engine build and prints greedy token IDs (CPU, no EOG break) rather than decoded text. |
| `quant_eval.das` / `quant_eval_q4.das` | Q8 / Q4 reconstruction-error stats vs the fp32 weights. |
| `bpe_test.das` | BPE tokenizer corpus gate against the in-repo `ggml-vocab-llama-bpe` fixtures (no model needed). |
| `gguf_dump.das` | Dump a GGUF's metadata + tensor table. |
| `asr_corpus_eval.das` / `score_asr_corpus.py` | The ASR corpus check: one model on one weight lane (`--lane q8\|exact` pins it; un-pinned, a quantized GigaAM carrier takes its kq lane) over a FLEURS-layout corpus (`<split>.tsv` beside `<split>/<wav>`), one TSV row a clip, every hypothesis and reference translated where `--nmt` names a Bergamot student; the scorer reads every run under `<root>/ru_ru/hyp/` and prints WER/CER against the reference, the WER change against a base run with a paired 95% bootstrap interval, WER against the base run's transcripts, chain chrF against the en_us reference, and x realtime. A quantized carrier on the exact lane serves f32, so its speed column is not the format's. |
| `pocket_stage_probe.das` | The Pocket TTS stage probe: one chunk's frame loop alone and the codec alone over that chunk's latents, `--reps` passes each on the CPU chain and on the Metal tower in one process - the loop's cost a frame and the codec's rel-l2 against the CPU chain (`-m`, `-t`, `--voice`, `--q8` / `--f32`). |
| `batch_decline_census.das` | The batched-step decline census: one host-cached N-stream row per GGUF through the Metal batch driver (`--npl`, `--ctx`, `--kv f16|f32|q8_0|tq4`), then the step census (device / cpu_stack / per_row) and both drivers' declines-by-reason tables - names which serving shapes still step a row at a time, and reads a dying per-row fallback as text. |
| `parity_dump.das` | The parity dump: one arm's forced-feed run to a file (`--model`, `--out`; the prompt and feed ids, the routing tape the arm recorded, every row's logits), the other arm's run on that file's prompt, feed and tape (`--ref`, the tape replayed so a near-tie flips nowhere), and two files compared (`--compare-a`, `--compare-b`, `--bar`): every row within the bar of the reference row's live max and past its one-step-off control, exit 1 on a miss. The parity of a model that cannot hold both arms in one process, and of a CPU reference run once and kept. The tape carries the host's selects alone: a routed layer the device serves whole picks in its kernel and lands nothing, so the recording run says how many routed layers the tape covers and exits 3 where it is short - dump with `DASLLAMA_GPU_HOST_LAYERS=<routed layers>` for a replayable tape (a hyper-connection model on a card that holds no routed layer whole is complete as it is). `_parity_dump.das` is the file form and the compare the tool and `tests/test_parity_dump.das` share. |
| `forced_feed_probe.das` | The forced-feed probe: a CPU prefill on a CPU session and on the GPU-decode blob twin, then N steps fed the CPU chain's greedy tokens on both (`--kv`, `--steps`), printing each step's logits maxd and both argmaxes - separates a decode step's numerics from the drift a diverging chain hides. |
| `served_bench.das` | The served-turn instrument: one client clock around streamed chat turns of ANY OpenAI-compatible server (`--url`, `--sizes 3000,9000`, `--reply`, `--reps`) - per prompt size the cold TTFT and the prefill rate it implies, the warm TTFT under the same system prompt, and the decode rate, each over all reps with its cv, a row past 3% printed VOID and a row with a failed turn refused; beside a dasllama-server row, the server's own clock for the same turns. With `--follow-up`, one more row: a short request, then the same request with one word changed, and the prompt tokens the server reused for it. With `--asr-url` and `--clip`, one row a clip for a whole transcription request, and under `--under-load` a second one timed while a chat turn decodes at `--url`. With `--image`, one row an image: the first token of a question about it, and of a second question asked again. With `--tts-url`, one row a `--tts-model` for a whole speech request beside its real-time factor. With `--scene`, a chat turn, every `--scene-asr` model and the first `--tts-model` at once, each lane's median alone beside its median in the scene. Loads no engine module. Its rows are ledger figures, never records (`../ARCHITECTURE_MEASUREMENT_SERVED.md#served-turn-instrument`). |
| `nmt_probe.das` | The translation homes' race: one Bergamot model over a corpus of sentences, the arms (cpu-q8 the baseline, cpu-f32 the reference, device on a build whose Metal or Vulkan driver serves it - `DASLLAMA_GPU=1` for Vulkan) a round each in turn after `--warm`, each arm's best round in ms a line with mean, worst and cv (VOID past 3%), the baseline's lines held against the reference and the device's against the baseline by token ids, a device round the driver did not serve whole refusing the row; behind the tune gate. |
| `asr_stage_probe.das` | The ASR stage instrument, any `load_asr_model` family: one clip, the model loaded off the clock, `--warm` warm-ups (`--settle` seconds idle first), then `--reps` profiled passes - each pass's encode-only bucket and dispatches, the encode row, the asr_prof stage table and the decoder engine's buckets (`--trace` saves the passes' jobque lanes). `--e2e` adds the end-to-end `create_session` + `transcribe` walls with the rail off ahead of them, `--vad` the silero VAD over the whole clip behind them (`--vad-only` alone); each timed row its mean, min-max and cv, VOID past 3%, behind the tune gate. `--lane cpu-q8\|cpu-f16\|metal` pins a lane, `--replay` arms the Metal rotary chain's restride and slab-trio replay (`set_metal_fc_prof_replay`), and `--fp32` / `--fp32-tower` / `--kv` / `--wdec-step` pin the precision rails. The ledger's GigaAM walls come from it. |
| `mem.das` | Resident memory footprint of a loaded model, by region. |
| `metal_release_probe.das` | Load-serve-delete cycles of one model's blob twin on Metal (`-- <model.gguf> <cycles>`), the process footprint's IOAccelerator and MALLOC_LARGE categories and the live Metal objects by creation site after each cycle's delete and after its close at the drivers' shutdown - a device buffer a delete leaves alive reads as a category that climbs a model a cycle. |
| `hot_pool_sim.das` | The hot expert pool's offline instrument: a routed pick trace (`DASLLAMA_MOE_TRACE=<file>` on a serving run) replayed through a pool policy (`--policy heat\|lru\|lfu`, `--slots`, `--swaps`, `--async --place-host-us --place-link-us`) - the decode hits, the placements, a token wall modeled from the measured costs, per layer under `--per-layer`. |
| `_window_race_fixture.das` | The window races' shared fixture: `int_list` (a comma list's numbers) and `counting_tokens` (a counting prompt of `n` tokens from a seed) both window race beds read. |
| `vk_short_window_race.das` | The Vulkan short window's race: a continuation of N rows after a prompt through the token command's peel against the same rows through the window chain, the lever `set_vulkan_prefill_peel` flipped between the forms in one process, interleaved (`--model`, `--rows 1,2,4,8`, `--reps`, `--prompt`), the fastest of the reps a form, a row a row count with both times, the peel's share, the peels counted and the forms' widest last-row logits' difference. |
| `seg_window_race.das` | The segmented prefill window's race: S sessions' prompts of N tokens each as ONE Metal window against the same S as S windows in sequence, on a blob twin (`--model`, `--lens 3000,9000`, `--streams 1,2,4`, `--reps`), the fastest of the reps a form, a row a length and stream count with the one-window time as a share of the S-window time and the widest last-row logits' difference between the forms (a crossed segment shows there); `DASLLAMA_METAL_TENSOR=0` is the M4 arm. |
| `vk_spv_diff.das` | The SPIR-V identity gate for a kernel fold: two `DASLLAMA_VK_SPV_DUMP` directories in, the stamps whose emitted words moved, appeared or vanished out; exit 0 only when every stamp is byte-identical on both sides. |

## Token-for-token parity (the correctness detector)

Correctness is proven by reproducing a reference **token-for-token** under greedy decoding, with the
tokenizer kept out of the loop - only the loader + forward pass are under test. The reference IDs come
from `simple_ids` (the same method the arc used); the prompt is tokenized once by the reference
engine and the exact IDs are fed to both sides.

### One-time: build the oracle

`simple_ids` links against an existing reference-engine build. From the reference-engine checkout:

```sh
cp <daScript>/modules/dasLLAMA/harness/oracle/simple_ids.cpp .
clang++ -std=c++17 -O2 -I include -I ggml/include simple_ids.cpp \
  -L build/bin -lllama -lggml -Wl,-rpath,"$PWD/build/bin" -o build/bin/simple_ids
```

### Run a parity check

```sh
# fp32 storage for an F16/F32 file; q8 for everything quantized (Q8_0, and the K-quant /
# mxfp4 / Q4_0 formats serve their native planes under q8)
modules/dasLLAMA/harness/parity.sh $MODELS/tinyllama-1.1b-v0.3-f16.gguf 40 fp32
modules/dasLLAMA/harness/parity.sh $MODELS/Qwen2.5-0.5B-Instruct-Q8_0.gguf 40 q8
```

Prints both ID streams and `PASS: N/N token-for-token` or the first mismatch index.

### Just the dasLLAMA side

```sh
bin/daslang -jit modules/dasLLAMA/harness/parity.das -- \
  -m <model.gguf> -n 40 --quant q8 --ids 1,9038,2501,263,931
```

Frozen fixtures (prompt IDs + reference GEN_IDS) live with the suite in `modules/dasLLAMA/tests/test_parity.das`
so the parity gate runs without the reference engine or the oracle at test time. The GGUF models themselves are
gitignored, so each case skips cleanly when its model is absent (a real local regression, a no-op in
CI). **Run model tests with the JIT** - interpreted is far too slow for any real model:

```sh
bin/daslang -jit dastest/dastest.das -- --test modules/dasLLAMA/tests/test_parity.das
```

## Sliding-window attention (Gemma2) - long-context check

Gemma2 uses sliding-window (local) attention on its even layers (window 4096, `swa_period=2`) and
global attention on the odd layers. It's a **no-op for contexts <= 4096**, so the short parity prompts
above never exercise the window. To test it, feed a prompt that tokenizes past 4096 and diff the
continuation against the oracle (which applies SWA correctly):

```sh
# build a >4096-token prompt (distinctive early fact + filler + a recall query), then:
modules/dasLLAMA/harness/parity.sh $MODELS/gemma-2-2b-it-Q8_0.gguf 12 q8 "$(cat long_prompt.txt)"
```

Verified: a 4168-token context is **11/11 token-for-token** vs the oracle - the window path is correct.

Two caveats worth knowing:
- **It stays a manual harness check, not a CI fixture** - it needs a hand-built >4k prompt and
  the oracle binary beside a reference-engine build.
- **Token-level parity is a weak SWA discriminator on Gemma.** Its *global* (odd) layers carry the
  long-range signal, so toggling the sliding layers off often yields the *same* tokens. The window is
  still load-bearing: at the last position of a 4168-token context (where the sliding layers exclude
  the early tokens) the last-position logits shift by up to **~2.06** (on the ~30 soft-capped scale)
  between SWA-on and SWA-off - i.e. a near-tie prompt *would* flip, so the oracle gate can catch a
  broken window given a sensitive enough prompt.
