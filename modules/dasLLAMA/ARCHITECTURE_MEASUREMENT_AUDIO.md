# dasLLAMA Architecture - the audio boards' reference instruments

Companion to `ARCHITECTURE_MEASUREMENT.md`; a section is cited by its anchor.

### The ASR board's GPU row pairs {#asr-gpu-pairs}

The das Metal ASR leg is OPT-IN per catalog row: `AsrModelSpec.metal_served`
(`performance/profile_common.das`) declares that the Metal driver serves that family end to
end - tower and decoder both. An unflagged family keeps the CPU by design, and asking for its
GPU leg reds the row: the `--ngl` arms assert that the tower engage counters moved, so a family
whose tower silently falls back fails instead of publishing a CPU wall under a GPU heading.
The MLX reference (`asr_gpu_only_tool`) runs on the Apple GPU alone: its one row is the `metal`
leg, measured on every catalog row that carries it whatever `metal_served` says.

On a Vulkan box the CPU arm's row stamps `backend = "vulkan"` when the Vulkan drivers served
every clip (the tower every encode with no decline; on whisper the ASR-decoder driver every window
too), keeps `backend = "cpu"` when they served none, and fails loudly with no number when they
served some clips but not all. The cell prints, per clip, the tower's encode and decline deltas
and, on whisper, the decoder's window and decode-batch deltas, plus one closing line counting
the fully served clips - the engage witness behind the stamp. The Vulkan readings live in
`PERF_LEDGER.md`, not in the records, against these references, each run on the pod: whisper.cpp's
own Vulkan build (the pod's checkout, commit d09f61a, version 1.9.4-dev, GCC 13.3), `whisper-cli -m
ggml-<model>-q8_0.bin -f <clip> -t 16 -bs 1 -bo 1 -nf`, timestamps on (under `-nt` it advances a whole
30 s a window: fewer encoder runs than the das loop's timestamp-seeking windows), each clip behind jfk in one
process with the jfk-only process subtracted (the first clip carries the pipeline builds; `-ng` for its CPU arm;
`GGML_VK_PERF_LOGGER=1` for the per-op table); llama-mtmd-cli b10660 (Vulkan), `llama-mtmd-cli -m <model Q8_0>
--mmproj <mmproj bf16> --audio <clip> -p "Transcribe the audio." --temp 0 --jinja -ngl 99 -t 16` (E2B with `-n 256`), its encode the sum of the clip's
`encoding done in N ms` lines; NeMo's `generate()` through `benchmarks/asr/canary_qwen_bench.py --device cuda`.

Four reference tools carry a GPU arm the board pairs against a das Metal row, each with its own
spelling: the whisper reference exe takes `-ngl`, the media-chat reference exe `-ngl 99`, the
NeMo bench script `--device mps`, and `crispasr` runs its Metal build (`crispasr_bin(true)`,
`$CRISPASR/build/`) where its CPU arm runs the Metal-OFF `build-cpu/`, which prints `gigaam:
backend = CPU` with no Metal device to pick. The GigaAM das Metal row encodes on the device whole -
the conv front and the sixteen blocks over the f16 planes - and decodes its transducer on the CPU,
as parakeet's does. The remaining two reference legs have no pair - the rig runs the
parakeet exe with `-ng`, though its Metal arm is the faster one (`PERF_LEDGER.md`), and the ONNX
export is CPU-only - so their das rows stand alone in the CPU category.

The media-chat reference exe is built as the bench exe's sibling in one reference worktree:
`benchmarks/setup_lcpp_ref.das` builds both targets, since a bench-only build leaves the image and
audio-chat cells with no binary and the board quietly mints das-only rows. The sibling needs the
timing patch beside it (`benchmarks/asr/patches/`): the record parser reads its per-rep timing
lines, an unpatched sibling mints "no rep parsed" failures, and the apply - guarded on the patched
marker already being in the tree, three-way - still applies after the reference pin moves. On Apple
boxes `performance/setup_asr_rig.das` builds a second, Metal-ON copy of the same patched checkout,
because `-ngl` on a Metal-OFF build is inert; `mtmd_bin_metal()` returns "" when it is absent and the GPU reference leg skips loudly.

### The ASR board's CPU reference rows {#asr-cpu-references}

A CPU ASR cell pairs the das row with the tool `asr_catalog()` names for the model, each spawned by
`performance/gen_bench_records.das -w asr` on the same box at the box's thread count, best of
`--asr-reps` per clip. `llama-mtmd-cli` comes from the reference checkout `setup_asr_rig.das` pins
at `LLAMA_CPP_COMMIT` with `benchmarks/asr/patches/mtmd-timing.patch` applied and built CPU-only
(`setup_asr_rig --refs`); `run_asr_mtmd` runs it greedy (`--temp 0`) with no GPU layers (`-ngl 0`)
over the model's Q8_0 file and its bf16 mmproj, and reads the reference's encode figure as the
`encode_prefill_ms` field of the first `MTMD_TIMING` line of the best-total rep - encode plus
prefill, so a reference encode cell carries the prefill the das cell's does not. `whisper-cli` and
`parakeet-cli` come from the pinned whisper.cpp checkout and run over the q8_0 model file the rig
quantizes beside the fp file; the ONNX and NeMo tools run from the rig's venvs. The reference's
encoder runs its mmproj's bf16 weights in bf16 arithmetic and the das row widens them to f32, so an
encode pair compares two arithmetics as well as two engines.

GigaAM's row runs the three FLEURS ru clips; `corpus/ru_fleurs.json` carries their FLEURS ids and reference texts.
`ru_long.wav` runs past the family's 25 s whole-clip window, so the das cell encodes it as VAD spans. Its `onnx` reference
(`gigaam-v3-e2e-rnnt-int8`, `onnx-asr`'s export of the same e2e-RNNT) encodes it in one whole-clip pass. The long-clip pair
therefore compares two long-form policies as well as two engines.

GigaAM's two `crispasr` references are CrispASR at `CRISPASR_COMMIT` (`setup_asr_rig --refs` clones it with its ggml
submodule to `$CRISPASR`, default `../CrispASR`, and builds `build/` with Metal ON and `build-cpu/` with Metal OFF, Accelerate
on in both). `run_asr_crispasr` forks one `crispasr -m <carrier> --warmup -t <threads> <clip>` process a clip, the clip
listed `--asr-reps` times; `--warmup` transcribes a silence buffer off the clock and the backend is read from the GGUF.
The carriers are the f16 GGUF the das row loads, byte for byte, and `gigaam-v3-e2e-rnnt-q8_0.gguf`, which `crispasr-quantize
<f16> <q8_0> q8_0` writes beside it with the ten block GEMMs a layer in q8_0 and every other tensor copied - the planes the das
q8 lane quantizes (`ARCHITECTURE_MEDIA_ASR.md#gigaam-planes`), so the q8 pair compares the same quantized set. The tool
prints no stage timing, so a rep's wall is its `crispasr: transcribed <a>s audio in <t>s (<r>x realtime)` line, timed from
after the load to the transcript, decode included. `crispasr_line_ms` reads it from the realtime factor over the
`crispasr: audio: <n> samples` count before it wherever that resolves finer than the 10 ms seconds field (a Metal clip runs
tens of milliseconds); the best rep is kept with its transcript. CrispASR cuts a clip into fixed 30 s slices (its default
`--chunk-seconds 30`, no VAD) and decodes each; `ru_long.wav` takes three. The CrispASR build at `CRISPASR_COMMIT` reports
itself as 0.8.45. Two more forms of the same binary serve the ledger's GigaAM entry: a clocked Metal row lists the clip
15 times and keeps the last 5 reps, and a WER run passes every wav of the set to one process
(`crispasr -m <carrier> --warmup -t <threads> -nt <every wav>`, `-nt`: no timestamps).

### The MLX ASR reference {#mlx-asr-reference}

An MLX ASR row (`engine` `mlx-audio`, `backend` `metal`) is one `gen_bench_records` spawn of
`benchmarks/asr/.venv-mlx/bin/python benchmarks/asr/mlx_asr_bench.py --variant <handle> --threads
<n> --reps <r> --wav <clip>...` over the catalog row's clips, with no environment setting beyond the
command line (`--threads` is accepted and unused: MLX runs on the GPU). The venv builds from
`benchmarks/asr/requirements-mlx.txt` (mlx and mlx-metal 0.32.4, mlx-audio 0.6.1, mlx-lm 0.32.2;
`setup_asr_rig --venvs`, Apple boxes only), and the script prints a `VERSIONS` line the row's
`versions` carries. A variant names one `mlx-community` Hugging Face repo, fetched on first use:
`parakeet-v2` / `parakeet-v3` (`parakeet-tdt-0.6b-v2` / `-v3`), `whisper-turbo-fp16` / `-8bit`
(`whisper-large-v3-turbo-asr-fp16` / `-8bit`), `qwen3-asr-0.6b-8bit` / `qwen3-asr-1.7b-8bit`
(`Qwen3-ASR-0.6B-8bit` / `-1.7B-8bit`). The model load is off the clock and one untimed
transcription runs first; a timed rep is one `generate` on the clip's path, its audio decode
inside, and the row keeps each clip's best rep.

### The TTS reference instrument {#the-tts-reference-instrument}

A TTS board cell is `lcpp_bench --tts -m <gguf>` against `harness/tts_ref_bench.py`, both over the
g2p corpus's first `--tts-sentences` sentences (20) at one voice (`tts_catalog()`), every sentence
timed best of `-r` / `--passes` (3) after one untimed warm-up, generation only: das times the
synthesis less its front end, the phoneme references take the fixture's `ps_espeak` strings, and
Pocket reads the text on both sides. Each side records one `tts:g2p<N>@<voice>` test, its `ms` the
best walls summed and `audio_s` the seconds spoken. The das cell's `--ngl` serves every chunk on the
Metal tower and reds where a seat fell back (`tts_seat_verdict`); its CPU row pins the tower off
on Metal and stamps `vulkan` on a Vulkan build whose TTS seats served every chunk. The references:
kokoro's `KModel` on the torch device (`cpu`, `mps`, `cuda` beside a `vulkan` row), KittenTTS's
ONNX files on the onnxruntime CPU provider with the package's tokenizer, style rows and speed prior,
Kyutai's `pocket-tts` on the CPU (one torch thread, the package's pin), all pinned to the
`model_specs.das` recipes' revisions from the in-repo `tts` venv; and on the metal leg mlx-audio's
ports of all three from the `mlx` venv (`--engine mlx`). Both venvs build from committed
`benchmarks/asr/requirements-*.txt` (`setup_asr_rig --venvs`), and the run prints a `VERSIONS` line
the row carries. A kitten voice name resolves through the `voice_aliases` table of the pack (the
Kitten model repo the recipe pins) on both sides - das reads it from the GGUF, the reference from
the pack's `config.json` - so a kitten pair speaks the same voice at the same speed prior (the
per-voice rate multiplier the pack's `speed_priors` assigns) and compares on audio seconds and the
real-time factor like every other TTS pair. `harness/tts_synth.das` and `harness/pocket_synth.py`
stay the scoring rig's synthesis arms.

The Pocket family's reference rows outside the board take two more scripts under the same pocket-tts venv:
`harness/pocket_ref_time.py <configs> <threads> <device>` times the package on the served bench's one sentence at
`alba` (one untimed request, then eight; the median wall, the seconds of speech, the real-time factor and the
spread; `0` threads leaves torch's default, `cpu`, `cuda` or `mps` the device) - the third-party row beside a
`dasllama-server --tts` row of `harness/served_bench.das`; `harness/pocket_ref_pace.py` reads the package's pace
on three texts (total seconds, the lead and tail silence, the pauses of five or more quiet 20 ms windows, the words
a second of speech), the twin of `harness/pocket_pace.das`'s reading of our files, whose `DAS_LOG_LEVEL=info`
timings lines are the stage split (prompt, backbone, head, codec) of each synthesis. A ledger row from either
names the venv's pocket-tts and torch versions beside its command.

### The TTS reference's CUDA provider and its g2p-venv runs {#tts-ref-g2p-venv}

`harness/tts_ref_bench.py --device cuda` opens a Kitten session on onnxruntime's CUDA execution
provider (`onnxruntime-gpu`) ahead of the CPU one; any other device opens the CPU provider alone,
and the run prints the providers the session took, so a row names the one that ran. Outside the
board the script also runs under the g2p experiment's venv (`~/Work/tts-ab/g2p/.venv-g2p`) with
`HF_HOME` at that experiment's hub cache (`~/Work/tts-ab/g2p/.hf`, holding the pinned snapshots
`harness/kitten_graph.py` names, and that script's default when `HF_HOME` is unset); a ledger row
timed there names the torch, kokoro, kittentts and onnxruntime versions the run printed beside its
command.
