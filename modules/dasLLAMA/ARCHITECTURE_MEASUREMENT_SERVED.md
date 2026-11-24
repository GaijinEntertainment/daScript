# dasLLAMA Architecture - the served-turn instrument

Companion to `ARCHITECTURE_MEASUREMENT.md`; a section is cited by its anchor. This document
carries the client clock around a live chat server's turns and what its figures are
(`#served-turn-instrument`), the third-party servers it reads under the same clock
(`#served-third-party`), and the model bench, which reads a model's own rates through a server
(`#model-bench`, `#model-bench-runner`). The benchmark rig, the tune gate and the instrumentation rails stay in
`ARCHITECTURE_MEASUREMENT.md`.

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
time. The untimed request ahead of a row asks for a whole answer with thinking off and the bench prints it (`says:`),
so a row is read beside what the model said of the part. A second untimed request under a system line of the reps' shape follows it:
a rep's line shares its opening with the line of the rep before, and without it the first rep alone prefills that opening. The row carries the prompt tokens the part and the text make, the same on every rep or the
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

A transcription row with no chat model behind it launches ours with the ASR model alone:
`main.das -- --asr <whisper bin>` for a whisper model, `main.das -- --asr <gguf> --mmproj <mmproj>`
for Qwen3-ASR. The whisper reference is whisper.cpp `6fc7c33`, built on the box, as
`whisper-server -m <bin> --port <port> --inference-path /v1/audio/transcriptions`, so the
client's transcription request reaches it at the path it sends. The client is
`served_bench.das -- --no-chat --asr-url <server> --clip <wav> --reps 5`.

An audio-chat row (a clip asked about as an `input_audio` part) launches ours with the tower's
projector beside the model, `main.das -- -m <gguf> --audio-mmproj <mmproj> -p 8123 -s 1`, and the
client as `served_bench.das -- --url <server> --no-chat --chat-clip <wav> --reps 5`.

Its figures are ledger figures - `PERF_LEDGER.md`, tagged `served` - and nothing else. No record
store, board or ladder takes them, and none of them compares against an `lcpp_bench` row: the
rig's `pp` and `tg` time the kernels from an empty cache with no scheduler, no HTTP path and no
prefix cache inside the clock, and those three are what this instrument times.

### Third-party servers under the same clock {#served-third-party}

A third-party row is the same client against another process's endpoint, and its ledger entry
carries the recipe the way ours carries the server line. Strata (the native-pack MoE server) is
launched on the same shards as ours, `strata serve <pack> --port <port> --spec 2 --suffix-draft 0`:
`--spec 2` is the pack's floor and keeps the MTP head out of the round, `--suffix-draft 0` turns
the suffix drafter off, so the row is the plain decode against our round-off row; its expert
cache sizes itself (the entry quotes its log's slot count, tagged `external`, since no clock of
ours reads it). The client is `served_bench.das -- --url <server> --sizes 512,3000,9000 --reply 200
--reps 2`, the same corpus and the same reply length on both sides, and the entry names both
servers' flags beside the figures.

### The model bench {#model-bench}

`harness/model_bench.das` reads a model's own rates through a chat server: prefill and decode, at
one stream and at several, with a drafter and without, on the same client clock and wire as the
served-turn instrument. Where that instrument times what a client of a server waits for - a cold
turn, a warm one, the cache between them - the model bench keeps every cache out of its clock. Each
prompt opens on an id no server has seen, a turn is refused when the server reports prompt tokens
read off a cache past a chat template's fixed head, and the request carries `cache_prompt: false`,
which keeps a turn out of dasllama-server's prefix cache whole - no opening attached, no checkpoint
stop, nothing left behind - and off llama.cpp-server's.

One streamed turn gives both rates: the prompt's tokens over the time to the first reply text, and
the reply's tokens after the first over the time after it, which is the decode rate at that context
length. The prompt is a frozen corpus of this repository's Python sources
(`benchmarks/data/python_context.txt`) cut to the length, then a task that asks for code alone; one
untimed turn a cell reads the tokens its cut makes, so a cell's prompts land within a hundredth of
the length it names. The ladder climbs the config's lengths and stops at the first cell its own
measured rates predict past the cell's time, and a turn that outlasts three times its prediction is
cut off: a slow box gets a shorter ladder, and a collapse is a refused cell, not a hang. A cell that
a turn failed, cut short or read off a cache is refused with its reason and never averaged in.

The width rows send a wave of streams at once, each its own prompt and its own task (`wave_task`: a subject a stream, a
language every eighth stream on), at two lengths. Copies of one task decode one reply in step, and a routed model's
streams then pick the same experts at every step and read each stack once, so a wave of copies reads a rate
no two users' requests reach, and reads another rate on the run where one stream starts a token late. A width row
carries the wave's end too (`done_ms`, the page's "wave done, s"): the time from the wave's send to its last stream's
last text. The summed decode rate counts a stream's wait for another stream's prompt as its decode time, so a server
that answers the first prompt while it reads the second sums lower than one that reads both prompts and then answers
both, though its first text comes sooner and its wave ends sooner. A row is the
streams combined - their prompt tokens over the time to the wave's last first text, their decode
rates summed. A wave wider than the server's streams is not sent, since it would time the server's
queue. A speculative arm's rows are long replies at the same two lengths at one stream, read against
plain rows of the same reply length, each with the drafted and accepted tokens the server reports
(`timings.draft_n`, `draft_n_accepted`); a round's verify pays attention over the whole context, so
an arm's gain shrinks with the prompt, and the row at the longer length shows where it ends.

A run opens on its eyeball, ahead of any timing: obvious asks whose replies a reader checks at a glance and the bench
checks by rule, so a server that answers fast and wrong stops the run at its first lines and no test has to find it. At one
stream it asks why the sky is blue and asks for a count from 1 to 20 - on an arm's server those two turns are the
speculative path - and on a plain server it sends a wave of two and a wave of four, a stream an ask (the capital of
France, 2 + 2), none wider than the server's streams. A wave's reply that differs from its ask's one-stream reply is
read by the same rule and its line says so (`not the one-stream reply`); it is no refusal, since rows batched together may
round a near-tie the other way. The eyeball reads a reply's answer, its reasoning left out: the
opening of the answer is a printed `EYEBALL <mode> <ask>` line and a record of the run file; the count must run on in
order to ten, each other reply must hold one of its ask's words, a reply that spent its tokens on reasoning has no answer
to read, and a reply that reads wrong is a refusal that ends the run before its first timed cell (`EYEBALL FAILED`, exit
1). An entry's `eyeball_reply_tokens` gives the turn room where a model reasons ahead of every answer (gpt-oss). The arms'
plain long-reply run and the wide pass ride a server the plain run has read and ask nothing.

The ultralong pass is off unless asked for (`--ultralong`, or `--only ultralong` for it alone): one more server start under
the entry's `ultralong_server` - the model's whole context, one stream - and the rows of prompts of 65536 and 131072 tokens
and of that context less 512, each that fits it (`pp65536`, `tg128+65536`, ...). The corpus is shorter than those prompts,
so the pass repeats it behind a marked seam; a cell's time budget is an hour (`ULTRALONG_CELL_SECONDS`), and a model that cannot hold the context refuses its
row as any cell does. The lengths are the ones a short ladder never reaches: the quadratic terms, the rings and the
mirror's sizing are read there.

The behind pass reads the same depth without paying its prefill a run (`--behind`, or `--only behind` alone): a server
start under `ultralong_server` with a slot directory (`--slot-save-path`, the flag ours and llama-server share), and at each
base of 32768, 65536 and 131072 tokens that fits the context, and the deepest that does, a base turn the server holds from
then on - restored off its slot file (`POST /slots/0?action=restore`, row `restore<base>`), else prefilled cold (row
`cold<base>`, one turn) and saved (`save<base>`). A `restore<base>` or `save<base>` row is one rep: the tokens the slot
route moved over the milliseconds the server reports for the move (`timings.restore_ms`, `timings.save_ms`) - the server's
clock, not the client's. The timed turns replay that turn, a fixed reply after it, and add a
message of 4096 new tokens under a fresh id: `pp4096+<base>` is the tokens prefilled past the cache over the time to the
first text - the cached share's attach is inside that time, as a user's turn pays it - and `tg<reply>+4096+<base>` the
decode after them; a turn whose base did not come off the cache refuses its cell. The base carries no id and its characters
come from the run's own calibration, so one server life's file opens the next life's prompt; the files stay in the
runner's `--slot-dir` from run to run, a tag a model and side. A restored base carries the rounding of the build that
recorded it (`ARCHITECTURE_ENGINE_SERVING.md#context-file`): the pass times what follows a long context and is no
correctness cell. Our server arms a NextN head by itself for a slot of one stream, so the runner starts this pass and the
ultralong one with `--mtp=false`: their decode rows are plain rows, as the reference's are.

A run takes a part of that on request, for a change that moves one kind of row. `--only` names the row kinds of a plain
run - `pp`, `tg`, `width`, or `eyeball` for the eyeball and no timed cell - and `--lengths` names the ladder in the config's place. The prefill rows asked for alone cut each
turn's reply to eight tokens and skip the task-alone turn; the width rows asked for alone run their one-stream turns at the
width lengths and record no row for them. The runner's `--only` takes the same kinds for the plain start, `long` for its
long-reply rows, an arm by its name and `eyeball` for every start's eyeball, and makes no server start that nothing names. A run file and a card of such a run
hold the rows that ran and nothing in place of the others; the command line they carry says what was asked for.

A row is named as the ladder names its rows, with the context a decode ran after, the width and the
arm: `pp4096`, `tg128+4096`, `pp4096@4`, `tg128@8`, `tg512+4096:dflash2`. A row carries its flags -
`noisy`, `cliff` (a rate under a share of the rung before it, at twice the length), `stall` (one gap
between reply chunks far past the median), `scaling` (a combined rate under the width before it),
`unfair` (a wave's last first text far past its first) - and they are what the bench is for: a
pathological case shows as a flagged or refused row without a reader comparing columns.

### The model bench's runner and its cards {#model-bench-runner}

`harness/model_bench_run.das` owns everything the bench does not: it starts a server for a model,
waits until it answers, runs the bench against it, stops it, and writes the model's card. The
server and the bench are children of the runner (`daslib/fio`'s process handle): the runner drains
the server's output into a log while the bench runs, and closing the handle stops whatever of the
server still runs, so a server that never answers or does not stop takes no port into the next
start. A server counts as up once the model list that answers on the port names the model file the
runner started it on; any other answer is another process on the port, and the arm fails. The server
runs from the tree it is told, with that tree's binary, so the bench of one tree reads the server of
another; under `--llama-server` the server is llama.cpp's on the same file - every layer on the GPU,
the config's slots with the config's context a slot, greedy - and its card is an engine `llama.cpp`
run beside ours; `--server-arg` gives any other server's command line an argument at a time, with the
model file, the port and the entry's server plan filled in. A model's entry in
`harness/model_bench.json` is its whole setup: the lengths and widths, the server's streams and
context, its arms with the drafter each takes, and the wide pass's server, which holds enough
streams, each with room for the longest width prompt, for a wave of sixteen. An arm is a server
start of its own, with the arguments the config's `arm_flags` table gives that arm on that server,
because a slot that drafts is set up to draft at boot; the plain start runs the whole ladder and the
long plain rows the arms are read against.

A card is one run of the ladder's record (`BenchModel` / `BenchRun`, `performance/profile_common`):
the engine, backend, box and hardware, the server's commit, both command lines, the model file's
identity, and the bench's rows as its `tests`. The bench's own run file beside it keeps what the
record's schema does not carry - a row's token count, flags and draft counts, what was refused and
why - and every timed request is a line of a third file. The files land under the tree's ignored
`logs/model_bench` unless the run names a folder.

The page (`<entry>.md`, `harness/_model_bench_report.das`) is what a reader opens. The reference's
files carry `.ref` in their names, so both servers' runs of a model share a folder, and each run
ends by writing the page again from every run file and card the folder holds for the model: who ran
each side, then a table a section - prefill and decode at one stream, the streams at once, the
speculative arms, the rows behind a long context with the prompt tokens each read off the cache -
where a rate cell reads `ours / the reference's` with ours over the reference's
beside it, an arm's row carries its rate over the plain row of its reply and its accepted share of
the drafted tokens, and a row one side did not run shows a dash. A streams row's reply rate on the
page is the wave's reply tokens over the wave's wall, from the send to the last stream's last text
(`done_ms`): the bench's summed rate runs each stream's clock from its own first text, so it charges
a server for answering one stream sooner. A streams prompt row's rate is the bench's own. The flags
and the refused cells
close the page. `--report` writes the pages of a folder and starts nothing.
