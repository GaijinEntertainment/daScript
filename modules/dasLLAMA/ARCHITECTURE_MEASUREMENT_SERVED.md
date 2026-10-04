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

Its follow-up row times a short request under a system line no server has seen, then the same
request with one word changed: both TTFTs and the prompt tokens the server reports as cached for
the second - the reuse of a prefix shorter than any cache page, which the size rows never reach.
The changed request's cv is the one that voids the row, and the row is refused when the prompt or
cached token count differs between reps.

Its speech rows time a whole transcription request - the clip uploaded as a multipart form to
`/v1/audio/transcriptions` - one row a clip, over all reps behind one untimed request, every rep
reading the same text. Every request uploads a copy of the clip with one sample moved, the sample
picked off the clock: a server that keeps the prompts it has evaluated answers a clip it has heard
from that cache - audio rows included - and a transcription service is not sent the same audio
twice. A clip that is no PCM WAV goes up as it is. A second row a clip is timed while a chat turn decodes at the chat
endpoint: the reps start at that turn's first reply text, and the row is refused when the turn
ends before they do or the text differs from the idle row's. Where the speech encoder runs -
beside the chat model on the GPU or on the CPU team - is the server's launch, not the
instrument's: each placement is one run.

Its media rows (`--image`, and `--chat-clip` for an audio clip sent as an `input_audio` part) time a
question about the part answered in one token, the part ahead of the text, so the wall is the
part's encode, the prompt's prefill and that token: per rep a question under a system line no
server has seen, then a second question under the same line - asked again, which a server that
pays for a part once answers from its cache, neither encoding nor prefilling the part a second
time. The untimed request ahead of a row asks for a whole answer and the bench prints it (`says:`),
so a row is read beside what the model said of the part. The row carries the prompt tokens the part and the text make, the same on every rep or the
row is refused; the first question's cv is the one that voids it.

Its synthesis rows time a whole `/v1/audio/speech` request, one row a model: the answer is the
finished WAV, so the wall is the time to first audio, printed beside the seconds of speech the
answer carries and the real-time factor of the two.

Its scene rows set every engine of a voice exchange against itself: a chat turn decodes while
each named ASR model transcribes one clip and a TTS model speaks, every lane a client thread of
its own from the turn's first reply text to its last, a request every length of the speech it
carries - a clip every clip length, a synthesis every length of the audio it returned - or back
to back. A row is one lane, or the turn's decode rate: its median alone on the idle server, its
median over the rounds, and their ratio. A scene median's spread is contention, so the alone
samples' cv is what voids a row. A lane's row is refused, with the reason, when a request of
its failed or a round finished none of its requests inside the chat turn.

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

The image and transcription rows take the same two servers with the model's projector beside
it: ours as `main.das -- -m <gguf> --image-mmproj <mmproj> -p 8123 -s 1 --ctx 16384 --kv-dtype f16`
for an image row and `main.das -- -m <llm gguf> --asr <asr gguf> --mmproj <mmproj> -p 8123 -s 1`
for a transcription row, the reference as `llama-server -m <gguf> --mmproj <mmproj> --port 8902
-ngl 99 -np 1` (with `-c 16384 -ctk f16 -ctv f16` for an image row); the client is
`served_bench.das -- --url <server> --no-chat --image <file> --reps 5` and
`served_bench.das -- --asr-url <server> --clip <wav> --reps 5`.

Its figures are ledger figures - `PERF_LEDGER.md`, tagged `served` - and nothing else. No record
store, board or ladder takes them, and none of them compares against an `lcpp_bench` row: the
rig's `pp` and `tg` time the kernels from an empty cache with no scheduler, no HTTP path and no
prefix cache inside the clock, and those three are what this instrument times.
