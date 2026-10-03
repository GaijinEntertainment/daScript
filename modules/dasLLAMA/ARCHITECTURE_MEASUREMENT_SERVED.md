# dasLLAMA Architecture - the served-turn instrument

Companion to `ARCHITECTURE_MEASUREMENT.md`; a section is cited by its anchor. This document
carries one section, `ARCHITECTURE_MEASUREMENT_SERVED.md#served-turn-instrument`: the client
clock around a live chat server's turns, and what its figures are. The benchmark rig, the tune
gate and the instrumentation rails stay in `ARCHITECTURE_MEASUREMENT.md`.

### The served-turn instrument {#served-turn-instrument}

`harness/served_bench.das` reads what a client of a live chat server waits for. It streams chat
turns at any OpenAI-compatible endpoint and times them on its own clock: the request sent, the
first chunk that carries reply text, the last one that does. The wire is `dasOPENAI`'s client
(`chat_stream`, whose delta callback takes the clock marks; `transcribe` for the speech rows) and
the corpus reader is the profiling rig's (`harness/_corpus.das`); the instrument loads no engine
module and dispatches no kernel - the server under the clock is another process, ours or a third
party's, and the clock is the same for both. A third-party server keeps no clock a caller can read, so the
one ruler that fits both sides of a served comparison sits in the client. Beside a dasllama-server
row the instrument prints the server's own clock for the same turns (`timings.ttft_ms` and
`timings.gen_ms` on the closing usage chunk a stream sends under `stream_options.include_usage`),
so the HTTP path's share of a figure shows on the row.

Per prompt size it runs pairs of turns under one system prompt: cold, the prompt opening on a
nonce no server has seen, then warm, the same system prompt under another question. Its rows are
the cold TTFT with the prefill rate it implies, the warm TTFT, and the decode rate; the token
counts are the server's own, off the usage chunk. A row is computed over all its reps, is printed
VOID past a 3% cv, and is refused whole when a turn fails or carries no usage chunk.

Its speech rows time a whole transcription request - the clip uploaded as a multipart form to
`/v1/audio/transcriptions` - one row a clip, over all reps behind one untimed request, every rep
reading the same text. A second row a clip is timed while a chat turn decodes at the chat
endpoint: the reps start at that turn's first reply text, and the row is refused when the turn
ends before they do or the text differs from the idle row's. Where the speech encoder runs -
beside the chat model on the GPU or on the CPU team - is the server's launch, not the
instrument's: each placement is one run.

A served comparison's recipe, as the `served` entries ran it on the M5 Max: ours is
`bin/daslang -jit utils/dasllama-server/main.das -- -m <gguf> -p 8123 -s 1 --ctx 16384 --kv-dtype f16 [--mtp]`
under `DAS_TUNE_MANIFEST=modules/dasLLAMA/performance/m5.tune.json`; the reference is the pinned
llama.cpp `b6fdd0ac` (`llama-server --version`: 0.3.0-dev, build 832, commit 6fdd0ac89, AppleClang 21
arm64, a stock Metal build) as `llama-server -m <gguf> --port 8902 -ngl 99 -c 16384 -np 1 -ctk f16 -ctv f16`,
and `mlx_lm.server` from mlx-lm 0.32 where an entry names it; the client is
`harness/served_bench.das -- --url <server> --sizes 3000,9000 --reply 200 --reps 5` (two reps on a
model past 20 GB), after a rest of 120 s before each life, the box otherwise idle. Its rows are
`out-of-process` readings by construction, and `direction-grade` where an entry sets one life
against another.

Its figures are ledger figures - `PERF_LEDGER.md`, tagged `served` - and nothing else. No record
store, board or ladder takes them, and none of them compares against an `lcpp_bench` row: the
rig's `pp` and `tg` time the kernels from an empty cache with no scheduler, no HTTP path and no
prefix cache inside the clock, and those three are what this instrument times.
