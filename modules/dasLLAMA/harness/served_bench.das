options gen2
options indenting = 4

// served_bench - what a client of a live chat server waits for, read on ONE client clock for any
// OpenAI-compatible server: dasllama-server, or a third-party server beside it on the same box.
// ARCHITECTURE_MEASUREMENT_SERVED.md#served-turn-instrument names what its rows are and are not.
//
//   bin/daslang modules/dasLLAMA/harness/served_bench.das -- --url http://127.0.0.1:8080 [--label ours]
//       [--model <id>] [--sizes 3000,9000] [--reply 200] [--reps 3] [--settle 0] [--corpus <file>] [--think] [--follow-up]
//       [--asr-url <base> --clip a.wav --clip b.wav [--asr-model <id>] [--under-load [--load-tokens 1500]]] [--no-chat]
//       [--tts-url <base> [--tts-model <id> ...] [--tts-voice <name>] [--tts-text <text>]]
//       [--scene [--scene-asr <id> ...] [--scene-flood]] [--image a.jpg ...]
//
// Per prompt size, `--reps` pairs of streamed chat turns under one system prompt of about that many
// tokens, greedy, thinking off unless --think:
//   cold - the system prompt opens on a nonce no server has seen, so no prefix cache holds it
//   warm - the same system prompt again under another question, straight after its cold turn
// A turn's clock starts before the request is sent. TTFT is the arrival of the first chunk that
// carries reply text; the decode span runs from that chunk to the last one that does. The token
// counts are the server's own, off the closing usage chunk (`stream_options.include_usage`).
// Rows, each over all `--reps` reps: cold TTFT and the prefill rate it implies (prompt tokens not
// served from a cache / TTFT), warm TTFT, and the decode rate over both turns of every pair.
// Warmup: ONE untimed turn ahead of every row - it also reads the server's characters per token,
// which sizes the prompts; two servers compared on one model take the first one's figure through
// --chars-per-token, so both read the same text. A row whose cv passes 3% is printed VOID; a row with a turn that failed
// or carried no usage chunk is refused whole. Exit 1 when a row was refused or none was reported.
// `--follow-up` adds one row: per rep a short request under a system line no server has seen, then the same
// request with one word changed - both TTFTs, and the prompt tokens the server says the second one reused.
//
// Speech rows (`--asr-url`, one row a `--clip`): the wall of a whole transcription request, the clip
// uploaded as a multipart form, over `--reps` reps behind one untimed request; every rep must read
// the same text. `--under-load` adds a second row a clip, timed while a chat turn of `--load-tokens`
// tokens decodes at `--url`: its reps start at that turn's first reply text, the row is refused when
// the turn ends before they do or the text differs from the idle row's.
//
// Synthesis rows (`--tts-url`, one row a `--tts-model`): the wall of a whole speech request - the answer is
// the finished WAV, so the wall is the time to first audio - over `--reps` reps behind one untimed request,
// beside the seconds of speech it carried and the real-time factor the two imply.
//
// Image rows (`--image`, one row each, at `--url`): a question about the image answered in ONE token, so the wall is
// the image's encode, the prompt's prefill and that token - per rep under a system line no server has seen, then a
// second question under the same line (a server that keeps the image's rows answers that one from its cache).
//
// The scene (`--scene`): a chat turn of `--load-tokens` tokens decodes at `--url` while every `--scene-asr`
// model transcribes the first `--clip` and the first `--tts-model` speaks, each lane on a thread of its own from
// the turn's first reply text to its last, a request every length of the speech it carries (`--scene-flood`:
// back to back). One row a lane and one for the turn's decode rate, each the median alone on the idle server
// over `--reps` samples beside the median over `--reps` rounds of the scene, and their ratio. A scene median's
// cv is contention, not noise: the alone cv is the one that voids a row.

require daslib/clargs
require daslib/strings_boost
require openai/openai_chat   // chat_stream: the streamed turn, its usage chunk and the server's own clock
require openai/openai_audio   // transcribe: the speech row's request
require openai/openai_vision   // chat_vision: the image row's request
require audio/audio_wav   // decode_wav / read_wav: the seconds of speech an answer carries
require daslib/strings_convert   // try_to_int: the --sizes list
require daslib/fio
require strings
require math
require daslib/jobque_boost   // the load turn's thread: its job over a Stream, its state in an atomic
require _row_stats   // spread_of: the row statistic over reps
require _corpus   // load_corpus: the prompt corpus the system prompt is cut from

[CommandLineArgs]
struct Args {
    @clarg_short = "u"
    @clarg_doc = "Base URL of the chat server under the clock, e.g. http://127.0.0.1:8080"
    url : string

    @clarg_doc = "Base URL of the speech server; with --clip, adds the transcription rows"
    asr_url : string

    @clarg_doc = "The transcription request's \"model\" field (default: none)"
    asr_model : string

    @clarg_doc = "A WAV to transcribe (repeatable), one row each"
    clip : array<string>

    @clarg_doc = "Also time each clip while a chat turn decodes at --url"
    under_load : bool

    @clarg_doc = "The load turn's reply budget in tokens (default 1500)"
    load_tokens : int

    @clarg_doc = "Skip the chat rows"
    no_chat : bool

    @clarg_doc = "Base URL of the server that speaks; adds a synthesis row a --tts-model"
    tts_url : string

    @clarg_doc = "A speech request's \"model\" field (repeatable; default: one row on the server's default model)"
    tts_model : array<string>

    @clarg_doc = "The speech requests' \"voice\" field (default: none - the model's own default voice)"
    tts_voice : string

    @clarg_doc = "The text a speech request says (default: one sentence of about four seconds)"
    tts_text : string

    @clarg_doc = "Time the scene: a chat turn decoding at --url while every --scene-asr model transcribes the first --clip and the first --tts-model speaks, all at once, each against itself alone"
    scene : bool

    @clarg_doc = "An ASR model of the scene, by its \"model\" field (repeatable; default: the --asr-model one)"
    scene_asr : array<string>

    @clarg_doc = "Send each scene lane's requests back to back (default: at the pace of speech - a clip every clip length, a synthesis every length of the audio it returns)"
    scene_flood : bool

    @clarg_name = "label"
    @clarg_doc = "Row label (default: the URL)"
    tag : string

    @clarg_doc = "The request's \"model\" field (default: none - the server's own default model)"
    model : string

    @clarg_doc = "Comma-separated system prompt sizes in tokens (default 3000,9000)"
    sizes : string

    @clarg_doc = "Reply token budget per turn (default 200)"
    reply : int

    @clarg_doc = "Cold/warm pairs per size (default 3; one rep is refused)"
    reps : int

    @clarg_doc = "Seconds to idle before each size (default 0)"
    settle : int

    @clarg_doc = "Text the system prompts are cut from (default: benchmarks/data/wikitext2_prompts.txt)"
    corpus : string

    @clarg_doc = "Characters a prompt token, sizing the prompts (default: read off the calibration turn). Pass one server's value to the next so both get the same text"
    chars_per_token : float

    @clarg_doc = "Leave the model's thinking on (default: off)"
    think : bool

    @clarg_doc = "Add the follow-up row: a short request, then the same request with one word changed"
    follow_up : bool

    @clarg_doc = "An image to ask the chat server about (repeatable, jpg or png), one row each"
    image : array<string>

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

let CALIBRATION_CHARS = 8000   //! the untimed first turn's system prompt, the ruler for characters per token
let CV_VOID = 3.0              //! percent; a row past it is not a measurement
let TURN_TIMEOUT_S = 900
let QUESTIONS = fixed_array(
    "Summarize the reference notes above in detail, section by section.",
    "Write a long, careful critique of how the reference notes above are organized.",
    "List every person named in the reference notes above and say what each is known for.",
    "Retell the reference notes above as one continuous story for a general reader.",
    "Explain which parts of the reference notes above a student should memorize, and why.",
    "Compare the first and the last section of the reference notes above at length.",
    "Draft an encyclopedia-style overview that covers the reference notes above.",
    "Describe the reference notes above to someone who has never read them, thoroughly.")

//! one streamed turn as the client saw it; `why` is non-empty when the turn cannot enter a row
struct Turn {
    why : string
    ttft_us : int64             //! send -> the first chunk carrying reply text
    gen_us : int64              //! that chunk -> the last one carrying reply text
    prompt_tokens : int64
    cached_tokens : int64       //! -1: the server reported none
    completion_tokens : int64
    self_ttft_ms : float        //! dasllama-server's own admit -> first token, 0 from a server that reports none
    self_gen_ms : float         //! its own first token -> finish
}

//! one transcription request as the client saw it; `why` is non-empty when it cannot enter a row
struct Heard {
    why : string
    wall_us : int64
    text : string
}

struct Speech {
    url : string
    tag : string
    model : string
}

//! the chat turn a loaded speech row is timed beside; it crosses to its thread as an archive
struct LoadJob {
    url : string
    model : string
    question : string
    budget : int
}

//! one image question as the client saw it; `why` is non-empty when it cannot enter a row
struct Seen {
    why : string
    wall_us : int64
    prompt_tokens : int64
}

//! one synthesis request as the client saw it; `why` is non-empty when it cannot enter a row
struct Said {
    why : string
    wall_us : int64
    audio_s : float   //! the seconds of speech the answer carries
}

//! one engine of the scene: it crosses to its thread as an archive. `lane` is its place in the scene's rows.
struct SceneJob {
    lane : int
    tts : bool
    url : string
    model : string
    voice : string
    payload : string   //! the clip an ASR lane transcribes, the text a TTS lane says
    period_ms : int    //! a request every this long, the pace of the speech it carries; 0 sends them back to back
}

//! one request a scene lane finished, or the chat turn's decode rate (lane SCENE_CHAT)
struct SceneSample {
    lane : int
    round : int        //! the scene round it landed in; 0 for a sample taken alone
    value : float      //! a request's wall in ms; the chat turn's tok/s; below zero when it failed
    audio_s : float
    why : string       //! what a failed one said
}

//! the rows a run reported and the ones it refused
struct Tally {
    rows : int
    refused : int
}

let FOLLOW_UP_FIRST = "Build me a 10 line blob of Python that prints the Fibonacci numbers, with a comment on every line."
let FOLLOW_UP_CHANGED = "Build me a 20 line blob of Python that prints the Fibonacci numbers, with a comment on every line."
let LOAD_DECODING = 1   //! the load turn's state once its first reply text arrived
let LOAD_ENDED = 2
let SCENE_CHAT = -1     //! the scene sample that carries the chat turn's decode rate
let SCENE_MIN_REQUESTS = 2   //! a lane with fewer finished requests beside the chat turn has no spread
let TTS_TEXT = "The lighthouse keeper climbed the stairs once more, counting each step as the storm rolled in."
let LOAD_QUESTION = "Tell one very long story about a lighthouse keeper, with every scene in full detail, and do not stop early."

struct Plan {
    url : string
    tag : string
    model : string
    reply : int
    reps : int
    think : bool
}

//! the client of a server under the clock: no key, the turn's own timeout
def private client_of(url : string) : OpenAIClient {
    var cl = openai_client("{url}/v1")
    cl.timeout_seconds = TURN_TIMEOUT_S
    return cl
}

//! one streamed chat turn under the client clock; `started`, when given, reads 1 from the first reply text on
def run_turn(plan : Plan; system, question : string; budget : int; started : Atomic32? = null) : Turn {
    var turn = Turn(cached_tokens = -1l)
    var req <- ChatCompletionRequest(model = plan.model, max_tokens = budget, temperature = 0.0,
                                     messages <- [ChatMessage(role = "system", content = system), ChatMessage(role = "user", content = question)],
                                     stream_options = StreamOptions(include_usage = true),
                                     chat_template_kwargs = "\{\"enable_thinking\":{plan.think}}")
    var first_us = -1l
    var last_us = -1l
    let t0 = ref_time_ticks()
    let res <- chat_stream(client_of(plan.url), req) $(_piece : string) {   //! reply text, reasoning included, moves the two clock marks
        let now = int64(get_time_usec(t0))
        if (first_us < 0l) {
            first_us = now
            if (started != null) {
                started |> set(1)
            }
        }
        last_us = now
    }
    delete req
    turn.prompt_tokens = int64(res.usage.prompt_tokens)
    turn.completion_tokens = int64(res.usage.completion_tokens)
    turn.cached_tokens = int64(res.usage.prompt_tokens_details.cached_tokens)
    turn.self_ttft_ms = res.timings.ttft_ms
    turn.self_gen_ms = res.timings.gen_ms
    if (!res.ok) {
        turn.why = "HTTP {res.error.status}"
    } elif (first_us < 0l) {
        turn.why = "the stream carried no reply text"
    } elif (turn.prompt_tokens == 0l) {
        turn.why = "the stream carried no usage chunk (stream_options.include_usage)"
    } elif (turn.completion_tokens < 2l || last_us <= first_us) {
        turn.why = "a reply of {turn.completion_tokens} token(s) has no decode span"
    }
    turn.ttft_us = first_us
    turn.gen_us = last_us - first_us
    return turn
}

//! the corpus as one text: its `%%` segments, the comment ones dropped, a blank line between them; "" for a file that is not there
def corpus_text(path : string) : string {
    return "" if (!stat(path).is_valid)
    var inscope texts <- load_corpus(path)
    return join(texts, "\n\n")
}

//! a system prompt of `chars` characters cut from `base`, cycled, opening on `nonce`
def system_prompt(base, nonce : string; chars : int) : string {
    return build_string() $(var w) {
        w |> write("Session {nonce}. You are a careful assistant. Reference notes follow.\n\n")
        let whole = chars / length(base)
        for (part in range(whole)) {
            w |> write("[part {part + 1}]\n")
            w |> write(base)
            w |> write("\n\n")
        }
        w |> write("[part {whole + 1}]\n")
        w |> write(slice(base, 0, chars - whole * length(base)))
    }
}

def private flag(cv : float) : string => cv > CV_VOID ? "  VOID: cv over {CV_VOID}%" : ""

//! what the server's own clock read for the same turns, as a row's bracket: the median of `vals`, or nothing when a turn reported none
def own(var vals : array<float>; what, unit : string) : string {
    for (v in vals) {
        return "" if (v <= 0.0)
    }
    return "  [server's own {what} {spread_of(vals).median:.1f} {unit}]"
}

//! a turn's decode rate over `span_us`, the first token outside the span
def rate(turn : Turn; span_us : float) : float => span_us > 0.0 ? float(turn.completion_tokens - 1l) * 1.0e6 / span_us : 0.0

//! the three rows of one prompt size; false when a turn refused them
def private size_rows(plan : Plan; base, run : string; size : int; chars_per_token : float) : bool {
    var ttft_cold : array<float>
    var ttft_warm : array<float>
    var prefill : array<float>
    var decode : array<float>
    var own_cold : array<float>
    var own_warm : array<float>
    var own_decode : array<float>
    var prompt_tokens = 0l
    var warm_cached = -1l
    ttft_cold |> reserve(plan.reps)
    ttft_warm |> reserve(plan.reps)
    prefill |> reserve(plan.reps)
    own_cold |> reserve(plan.reps)
    own_warm |> reserve(plan.reps)
    decode |> reserve(2 * plan.reps)
    own_decode |> reserve(2 * plan.reps)
    for (rep in range(plan.reps)) {
        let system = system_prompt(base, "{run}-{size}-{rep}", int(float(size) * chars_per_token))
        let cold = run_turn(plan, system, QUESTIONS[(2 * rep) % length(QUESTIONS)], plan.reply)
        let warm = empty(cold.why) ? run_turn(plan, system, QUESTIONS[(2 * rep + 1) % length(QUESTIONS)], plan.reply) : cold
        if (!empty(warm.why)) {
            print("{plan.tag}  {size}  REFUSED at rep {rep}: {warm.why}\n")
            return false
        }
        let fresh = cold.prompt_tokens - max(cold.cached_tokens, 0l)
        ttft_cold |> push(float(cold.ttft_us) / 1.0e6)
        ttft_warm |> push(float(warm.ttft_us) / 1.0e6)
        prefill |> push(float(fresh) * 1.0e6 / float(cold.ttft_us))
        for (turn in [cold, warm]) {
            decode |> push(rate(turn, float(turn.gen_us)))
            own_decode |> push(rate(turn, turn.self_gen_ms * 1000.0))
        }
        own_cold |> push(cold.self_ttft_ms)
        own_warm |> push(warm.self_ttft_ms)
        prompt_tokens = cold.prompt_tokens
        warm_cached = warm.cached_tokens
    }
    let cold_s = spread_of(ttft_cold)
    let warm_s = spread_of(ttft_warm)
    let pre_s = spread_of(prefill)
    let dec_s = spread_of(decode)
    let cached = warm_cached >= 0l ? ", {warm_cached} cached" : ""
    print("{plan.tag}  cold {size}: {prompt_tokens} prompt tokens, TTFT {cold_s.median * 1000.0:.1f} ms (cv {cold_s.cv:.1f}%), prefill {pre_s.median:.0f} tok/s{own(own_cold, "TTFT", "ms")}{flag(cold_s.cv)}\n")
    print("{plan.tag}  warm {size}: TTFT {warm_s.median * 1000.0:.1f} ms (cv {warm_s.cv:.1f}%){cached}{own(own_warm, "TTFT", "ms")}{flag(warm_s.cv)}\n")
    print("{plan.tag}  decode {size}: {dec_s.median:.1f} tok/s (cv {dec_s.cv:.1f}%) over {length(decode)} turns{own(own_decode, "rate", "tok/s")}{flag(dec_s.cv)}\n")
    return true
}

//! one rep of a row: the wall of its request in ms, the wall of its second request when a rep is a pair, and why it
//! cannot enter the row ("" when it can)
struct RepWall {
    why : string
    first_ms : float
    again_ms : float
}

//! a row's reps: both wall lists, and why the row is refused - the rep that said so named - or "" when it stands
struct RepWalls {
    why : string
    first : array<float>
    again : array<float>
}

//! `reps` reps of one row: `rep_of` runs rep `rep` and answers its walls, and the first rep that says why ends the row
def collect_reps(reps : int; rep_of : block<(rep : int) : RepWall>) : RepWalls {
    var walls : RepWalls
    walls.first |> reserve(reps)
    walls.again |> reserve(reps)
    for (rep in range(reps)) {
        let wall = invoke(rep_of, rep)
        if (!empty(wall.why)) {
            walls.why = "at rep {rep}: {wall.why}"
            break
        }
        walls.first |> push(wall.first_ms)
        walls.again |> push(wall.again_ms)
    }
    return <- walls
}

//! The follow-up row: per rep a short request under a system line no server has seen, then the same request with one
//! word changed - what the second one waits for, and how much of its prompt the server says it reused. False when a turn refused it.
def followup_row(plan : Plan; run : string) : bool {
    var prompt_tokens = 0l
    var cached = -1l
    var walls <- collect_reps(plan.reps) $(rep) {
        let system = "Session {run}-follow-{rep}. You write short, careful code."
        let a = run_turn(plan, system, FOLLOW_UP_FIRST, plan.reply)
        let b = empty(a.why) ? run_turn(plan, system, FOLLOW_UP_CHANGED, plan.reply) : a
        return RepWall(why = b.why) if (!empty(b.why))
        if (rep > 0 && (b.prompt_tokens != prompt_tokens || b.cached_tokens != cached)) {
            return RepWall(why = "{b.cached_tokens} of {b.prompt_tokens} prompt tokens cached, {cached} of {prompt_tokens} the rep before")
        }
        prompt_tokens = b.prompt_tokens
        cached = b.cached_tokens
        return RepWall(first_ms = float(a.ttft_us) / 1000.0, again_ms = float(b.ttft_us) / 1000.0)
    }
    let stands = empty(walls.why)
    if (stands) {
        let first_s = spread_of(walls.first)
        let changed_s = spread_of(walls.again)
        let reused = cached >= 0l ? ", {cached} of its {prompt_tokens} prompt tokens cached" : ", {prompt_tokens} prompt tokens, no cache count reported"
        print("{plan.tag}  follow-up: TTFT {first_s.median:.1f} ms (cv {first_s.cv:.1f}%), with one word changed {changed_s.median:.1f} ms (cv {changed_s.cv:.1f}%){reused}{flag(changed_s.cv)}\n")
    } else {
        print("{plan.tag}  follow-up  REFUSED {walls.why}\n")
    }
    delete walls
    return stands
}

//! the chat rows of every size in `sizes`: a calibration turn, then three rows a size
def private chat_rows(plan : Plan; corpus, sizes : string; settle : int; fixed_chars_per_token : float) : Tally {
    var tally = Tally()
    let base = corpus_text(corpus)
    if (empty(base)) {
        print("error: no text in the corpus {corpus}\n")
        return Tally(refused = 1)
    }
    let run = "{ref_time_ticks()}"
    let probe = run_turn(plan, system_prompt(base, "{run}-calibration", CALIBRATION_CHARS), QUESTIONS[0], 8)
    if (!empty(probe.why)) {
        print("error: the calibration turn at {plan.url} failed - {probe.why}\n")
        return Tally(refused = 1)
    }
    let chars_per_token = fixed_chars_per_token > 0.0 ? fixed_chars_per_token : float(CALIBRATION_CHARS) / float(probe.prompt_tokens)
    let model = empty(plan.model) ? "the server's default model" : plan.model
    print("served_bench: {plan.tag} at {plan.url}, {model}, reply {plan.reply} tokens, {plan.reps} reps, thinking {plan.think ? "on" : "off"}, {chars_per_token:.4f} chars per token{fixed_chars_per_token > 0.0 ? " (given)" : ""}\n")
    var each <- split(sizes, ",")
    for (s in each) {
        let size = try_to_int(strip(s)) ?? 0
        if (size <= 0) {
            print("error: --sizes carries '{s}', not a token count\n")
            tally.refused++
            continue
        }
        if (settle > 0) {
            sleep(uint(settle) * 1000u)
        }
        if (size_rows(plan, base, run, size, chars_per_token)) {
            tally.rows += 3
        } else {
            tally.refused++
        }
    }
    delete each
    return tally
}

//! A copy of `clip` no server has heard - one sample, picked off the clock, moved - as a 16-bit temp file;
//! "" when `clip` is no PCM WAV or the copy cannot be written.
def unseen_clip(clip : string) : string {
    var samples : array<float>
    var rate = 0
    var channels = 0
    var path = ""
    if (read_wav(clip, samples, rate, channels) && !empty(samples)) {
        let ticks = ref_time_ticks()
        let at = ticks % long_length(samples)
        samples[at] += samples[at] < 0.5 ? 2.0 / 32767.0 : -2.0 / 32767.0   // two steps: write_wav truncates, and one would round back
        var err = ""
        path = path_join(temp_directory(err), "served_bench_{ticks}.wav")
        if (!write_wav(path, samples, uint(rate), channels)) {
            path = ""
        }
    }
    delete samples
    return path
}

//! one whole transcription request under the client clock, an unseen copy of the clip uploaded as a multipart form
def transcribe(speech : Speech; clip : string) : Heard {
    var heard = Heard()
    let unseen = unseen_clip(clip)
    let t0 = ref_time_ticks()
    let res = openai_audio::transcribe(client_of(speech.url), speech.model, empty(unseen) ? clip : unseen)
    heard.wall_us = int64(get_time_usec(t0))
    if (!empty(unseen)) {
        remove(unseen)
    }
    if (!res.ok) {
        heard.why = "HTTP {res.error.status}"
        return heard
    }
    heard.text = res.text
    if (empty(strip(heard.text))) {
        heard.why = "the answer carried no text"
    }
    return heard
}

//! `reps` timed transcriptions of `clip` into one row named `what`; `want`, when not empty, is the text every rep must read.
//! Answers the text the reps read, "" when the row was refused.
def clip_row(speech : Speech; clip, what, want : string; reps : int) : string {
    var text = want
    var walls <- collect_reps(reps) $(_rep) {
        let heard = transcribe(speech, clip)
        return RepWall(why = heard.why) if (!empty(heard.why))
        return RepWall(why = "the text differs from the text read before") if (!empty(text) && heard.text != text)
        text = heard.text
        return RepWall(first_ms = float(heard.wall_us) / 1000.0)
    }
    if (empty(walls.why)) {
        let s = spread_of(walls.first)
        print("{speech.tag}  {what}: {s.median:.1f} ms (cv {s.cv:.1f}%) over {reps} reps, {length(text)} characters read{flag(s.cv)}\n")
    } else {
        print("{speech.tag}  {what}  REFUSED {walls.why}\n")
        text = ""
    }
    delete walls
    return text
}

//! `clip`'s row timed while a chat turn of `load.budget` tokens decodes: the reps start at the turn's first reply text.
//! False when the turn never spoke, ended before the reps did, or a rep refused.
def loaded_clip_row(speech : Speech; load : LoadJob; clip, what, want : string; reps : int) : bool {
    var read = ""
    var spoke = false
    var outlasted = false
    with_stream() $(var job : Stream?) {
        var mine = load
        job |> push_archive(mine)
        with_job_status(1) $(finished) {
            with_atomic32() $(state) {
                new_thread() @() {
                    job |> pop_archive() $(var turn : LoadJob) {
                        run_turn(Plan(url = turn.url, model = turn.model), "You are a storyteller.", turn.question, turn.budget, state)
                    }
                    job |> release
                    state |> set(LOAD_ENDED)
                    finished |> notify_and_release
                }
                while (state |> get == 0) {
                    sleep(1u)
                }
                spoke = state |> get == LOAD_DECODING
                if (spoke) {
                    read = clip_row(speech, clip, what, want, reps)
                    outlasted = state |> get == LOAD_DECODING
                }
                finished |> join
            }
        }
    }
    if (!spoke) {
        print("{speech.tag}  {what}  REFUSED: the chat turn at {load.url} was over before the first rep\n")
    } elif (!empty(read) && !outlasted) {
        print("{speech.tag}  {what}  REFUSED: the chat turn at {load.url} ended before the reps did - raise --load-tokens\n")
    }
    return !empty(read) && outlasted
}

//! the speech rows: one a clip, and under `load` a second one timed beside a decoding chat turn
def private speech_rows(speech : Speech; clips : array<string>; reps : int; under_load : bool; load : LoadJob) : Tally {
    var tally = Tally()
    print("served_bench: {speech.tag} speech at {speech.url}, {empty(speech.model) ? "the server's default model" : speech.model}, {reps} reps\n")
    for (clip in clips) {
        let name = base_name(clip)
        if (!empty(transcribe(speech, clip).why)) {   //! the untimed request ahead of the row
            print("{speech.tag}  asr {name}  REFUSED: the warmup request failed\n")
            tally.refused++
            continue
        }
        let text = clip_row(speech, clip, "asr {name}", "", reps)
        if (empty(text)) {
            tally.refused++
            continue
        }
        tally.rows++
        if (under_load) {
            if (loaded_clip_row(speech, load, clip, "asr {name} while a chat turn decodes", text, reps)) {
                tally.rows++
            } else {
                tally.refused++
            }
        }
    }
    return tally
}

//! one question about an image, answered in one token: the wall is the image's encode, the prompt's prefill and that token
def see(plan : Plan; system, question, uri : string) : Seen {
    var seen = Seen()
    let t0 = ref_time_ticks()
    var res <- chat_vision(client_of(plan.url), plan.model, question, uri, 1, system)
    seen.wall_us = int64(get_time_usec(t0))
    seen.prompt_tokens = int64(res.response.usage.prompt_tokens)
    if (!res.ok) {
        seen.why = "HTTP {res.error.status}"
    } elif (seen.prompt_tokens == 0l) {
        seen.why = "the answer carried no usage"
    }
    delete res
    return seen
}

//! The image rows, one an image: per rep a question under a system line no server has seen, then another question
//! under the same line - what the first token of each waits for, and the prompt tokens the image and the text make.
def image_rows(plan : Plan; images : array<string>; run : string) : Tally {
    var tally = Tally()
    for (path in images) {
        let name = base_name(path)
        let uri = image_file_data_uri(path)
        let warmup = empty(uri) ? "the file is not there" : see(plan, "", "What is in this picture?", uri).why   //! the untimed request ahead of the row
        if (!empty(warmup)) {
            print("{plan.tag}  image {name}  REFUSED: {warmup}\n")
            tally.refused++
            continue
        }
        var prompt_tokens = 0l
        var walls <- collect_reps(plan.reps) $(rep) {
            let system = "Session {run}-{name}-{rep}. Answer briefly."
            let a = see(plan, system, "What is in this picture?", uri)
            let b = empty(a.why) ? see(plan, system, "What colors stand out in this picture?", uri) : a
            return RepWall(why = b.why) if (!empty(b.why))
            if (rep > 0 && a.prompt_tokens != prompt_tokens) {
                return RepWall(why = "{a.prompt_tokens} prompt tokens, {prompt_tokens} the rep before")
            }
            prompt_tokens = a.prompt_tokens
            return RepWall(first_ms = float(a.wall_us) / 1000.0, again_ms = float(b.wall_us) / 1000.0)
        }
        if (empty(walls.why)) {
            let first_s = spread_of(walls.first)
            let again_s = spread_of(walls.again)
            print("{plan.tag}  image {name}: {prompt_tokens} prompt tokens, first token {first_s.median:.1f} ms (cv {first_s.cv:.1f}%), asked again {again_s.median:.1f} ms (cv {again_s.cv:.1f}%){flag(first_s.cv)}\n")
            tally.rows++
        } else {
            print("{plan.tag}  image {name}  REFUSED {walls.why}\n")
            tally.refused++
        }
        delete walls
    }
    return tally
}

//! the seconds of audio `samples` hold at `rate` over `channels`, 0 when `decoded` says they are no WAV's
def private pcm_seconds(decoded : bool; var samples : array<float>; rate, channels : int) : float {
    let seconds = decoded && rate > 0 && channels > 0 ? float(length(samples)) / float(rate * channels) : 0.0
    delete samples
    return seconds
}

//! the seconds of audio in a WAV answer, 0 for bytes that are not one
def wav_seconds(bytes : array<uint8>) : float {
    var samples : array<float>
    var rate = 0
    var channels = 0
    let decoded = decode_wav(bytes, samples, rate, channels)
    return pcm_seconds(decoded, samples, rate, channels)
}

//! the seconds of audio in the WAV at `path`, 0 for a file that is not one
def wav_file_seconds(path : string) : float {
    var samples : array<float>
    var rate = 0
    var channels = 0
    let decoded = read_wav(path, samples, rate, channels)
    return pcm_seconds(decoded, samples, rate, channels)
}

//! one whole synthesis request under the client clock: the answer is the finished WAV, so its wall is the time to first audio
def say(speech : Speech; voice, text : string) : Said {
    var said = Said()
    let t0 = ref_time_ticks()
    var res <- openai_audio::speech(client_of(speech.url), SpeechRequest(model = speech.model, input = text, voice = voice, response_format = "wav"))
    said.wall_us = int64(get_time_usec(t0))
    if (!res.ok) {
        said.why = "HTTP {res.error.status}"
    } else {
        said.audio_s = wav_seconds(res.audio)
        if (said.audio_s <= 0.0) {
            said.why = "the answer is not a PCM WAV"
        }
    }
    delete res
    return said
}

//! the synthesis rows: one a model, `reps` requests behind one untimed request - the wall of a request and the real-time factor it implies
def tts_rows(url, tag : string; models : array<string>; voice, text : string; reps : int) : Tally {
    var tally = Tally()
    for (model in models) {
        let speech = Speech(url = url, tag = tag, model = model)
        let what = "tts {empty(model) ? "default model" : model}"
        let warmup = say(speech, voice, text).why   //! the untimed request ahead of the row
        if (!empty(warmup)) {
            print("{tag}  {what}  REFUSED: {warmup}\n")
            tally.refused++
            continue
        }
        var spoken : array<float>
        var rtfs : array<float>
        spoken |> reserve(reps)
        rtfs |> reserve(reps)
        var walls <- collect_reps(reps) $(_rep) {
            let said = say(speech, voice, text)
            return RepWall(why = said.why) if (!empty(said.why))
            spoken |> push(said.audio_s)
            rtfs |> push(float(said.wall_us) / (said.audio_s * 1.0e6))
            return RepWall(first_ms = float(said.wall_us) / 1000.0)
        }
        if (empty(walls.why)) {
            let s = spread_of(walls.first)
            print("{tag}  {what}: {s.median:.1f} ms for {spread_of(spoken).median:.2f} s of speech, rtf {spread_of(rtfs).median:.3f} (cv {s.cv:.1f}%) over {reps} reps{flag(s.cv)}\n")
            tally.rows++
        } else {
            print("{tag}  {what}  REFUSED {walls.why}\n")
            tally.refused++
        }
        delete walls
        delete spoken
        delete rtfs
    }
    return tally
}

//! one request of a scene lane: its wall in ms and the speech it carried, a negative wall when it failed
def private scene_request(job : SceneJob) : SceneSample {
    let speech = Speech(url = job.url, model = job.model)
    if (job.tts) {
        let said = say(speech, job.voice, job.payload)
        return SceneSample(lane = job.lane, value = empty(said.why) ? float(said.wall_us) / 1000.0 : -1.0, audio_s = said.audio_s, why = said.why)
    }
    let heard = transcribe(speech, job.payload)
    return SceneSample(lane = job.lane, value = empty(heard.why) ? float(heard.wall_us) / 1000.0 : -1.0, why = heard.why)
}

//! One round of the scene: a chat turn of `load.budget` tokens decodes while every lane sends request after request,
//! from the turn's first reply text to its last. Every finished request, and the turn's decode rate, lands in `samples`
//! under `round`.
def scene_round(load : LoadJob; lanes : array<SceneJob>; var samples : array<SceneSample>; round : int = 0) {
    with_stream() $(var jobs : Stream?) {
        var turn_job = load
        jobs |> push_archive(turn_job)
        for (lane in lanes) {
            var mine = lane
            jobs |> push_archive(mine)
        }
        with_channel(length(lanes) + 1) $(out) {
            with_atomic32() $(state) {
                new_thread() @() {
                    jobs |> pop_archive() $(var job : LoadJob) {   //! the first archive is the chat turn's
                        let turn = run_turn(Plan(url = job.url, model = job.model), "You are a storyteller.", job.question, job.budget, state)
                        state |> set(LOAD_ENDED)
                        out |> push_clone(SceneSample(lane = SCENE_CHAT, value = empty(turn.why) ? rate(turn, float(turn.gen_us)) : -1.0, why = turn.why))
                    }
                    jobs |> release
                    out |> notify_and_release
                }
                while (state |> get == 0) {
                    sleep(1u)
                }
                for (_lane in lanes) {
                    new_thread() @() {
                        jobs |> pop_archive() $(var job : SceneJob) {
                            while (state |> get == LOAD_DECODING) {
                                let sent = ref_time_ticks()
                                let sample = scene_request(job)
                                break if (state |> get != LOAD_DECODING)   //! a request the turn did not outlast is not the scene's
                                out |> push_clone(sample)
                                while (state |> get == LOAD_DECODING && get_time_usec(sent) / 1000 < job.period_ms) {
                                    sleep(1u)
                                }
                            }
                        }
                        jobs |> release
                        out |> notify_and_release
                    }
                }
                out |> for_each_clone() $(sample : SceneSample#) {
                    var landed : SceneSample
                    landed := sample
                    landed.round = round
                    samples |> emplace(landed)
                }
            }
        }
    }
}

//! the row of one scene lane: its median alone, its median beside the others, and the ratio. False when it has no spread.
def scene_row(tag, what, unit : string; var alone, together : array<float>; rtf : string = "") : bool {
    if (length(alone) < SCENE_MIN_REQUESTS || length(together) < SCENE_MIN_REQUESTS) {
        print("{tag}  scene {what}  REFUSED: {length(alone)} sample(s) alone and {length(together)} in the scene - a row needs {SCENE_MIN_REQUESTS} of each\n")
        return false
    }
    let a = spread_of(alone)
    let t = spread_of(together)
    print("{tag}  scene {what}: {a.median:.1f} {unit} alone (cv {a.cv:.1f}%), {t.median:.1f} in the scene (cv {t.cv:.1f}%, {length(together)} samples), {t.median / a.median:.2f}x{rtf}{flag(a.cv)}\n")
    return true
}

//! every lane and the chat turn alone on the idle server, `reps` samples each behind one untimed request a lane;
//! unless `flood`, each lane's period becomes the length of the speech it carries
def private scene_alone(load : LoadJob; var lanes : array<SceneJob>; reps : int; flood : bool) : array<SceneSample> {
    var alone : array<SceneSample>
    alone |> reserve((length(lanes) + 1) * reps)
    for (lane in lanes) {
        scene_request(lane)   //! the untimed request ahead of the lane's alone samples
        for (_rep in range(reps)) {
            alone |> push(scene_request(lane))
        }
        if (!flood) {   // the pace of speech: the clip's own length, the length of the audio a synthesis returned
            let spoken_s = lane.tts ? alone[length(alone) - 1].audio_s : wav_file_seconds(lane.payload)
            lane.period_ms = int(spoken_s * 1000.0)
        }
    }
    let plan = Plan(url = load.url, model = load.model)
    for (_rep in range(reps)) {
        let turn = run_turn(plan, "You are a storyteller.", load.question, load.budget)
        alone |> push(SceneSample(lane = SCENE_CHAT, value = empty(turn.why) ? rate(turn, float(turn.gen_us)) : -1.0))
    }
    return <- alone
}

//! the real-time factor of every sample of `lane` that carried speech: its wall over the speech it returned
def private lane_rtfs(samples : array<SceneSample>; lane : int) : array<float> {
    return <- [for (smp in samples); smp.value / (smp.audio_s * 1000.0); where smp.lane == lane && smp.audio_s > 0.0]
}

//! why lane `lane` has no row: a request of its that failed, or a round of `rounds` it finished no request in; "" when it stands
def private scene_lane_refusal(lane, rounds : int; alone, together : array<SceneSample>) : string {
    for (smp in alone) {
        return "a request alone failed - {smp.why}" if (smp.lane == lane && smp.value < 0.0)
    }
    var per_round : array<int>
    per_round |> resize(rounds)
    for (smp in together) {
        continue if (smp.lane != lane)
        return "a request in round {smp.round + 1} failed - {smp.why}" if (smp.value < 0.0)
        per_round[smp.round]++
    }
    var why = ""
    for (r, n in count(), per_round) {
        if (n == 0 && empty(why)) {
            why = "round {r + 1} finished no request inside the chat turn - raise --load-tokens"
        }
    }
    delete per_round
    return why
}

//! the row of lane `lane` (SCENE_CHAT: the turn's decode rate) off both sample sets, `rounds` rounds of them; false when it is refused
def private scene_lane_row(tag : string; lanes : array<SceneJob>; lane, rounds : int; alone, together : array<SceneSample>) : bool {
    var what = "chat decode"
    if (lane != SCENE_CHAT) {
        what = "{lanes[lane].tts ? "tts" : "asr"} {empty(lanes[lane].model) ? "default model" : lanes[lane].model}"
    }
    let why = scene_lane_refusal(lane, rounds, alone, together)
    if (!empty(why)) {
        print("{tag}  scene {what}  REFUSED: {why}\n")
        return false
    }
    var was <- [for (smp in alone); smp.value; where smp.lane == lane]
    var now <- [for (smp in together); smp.value; where smp.lane == lane]
    var rtf_was <- lane_rtfs(alone, lane)
    var rtf_now <- lane_rtfs(together, lane)
    let rtf = empty(rtf_was) || empty(rtf_now) ? "" : ", rtf {spread_of(rtf_was).median:.3f} alone and {spread_of(rtf_now).median:.3f} in the scene"
    let stands = scene_row(tag, what, lane == SCENE_CHAT ? "tok/s" : "ms", was, now, rtf)
    delete was
    delete now
    delete rtf_was
    delete rtf_now
    return stands
}

//! The scene rows: every lane and the chat turn alone on an idle server, `reps` samples each, then `reps` rounds of
//! all of them at once. A lane's row sets its two medians side by side; the alone cv is the one that can void it.
def scene_rows(tag : string; load : LoadJob; var lanes : array<SceneJob>; reps : int; flood : bool) : Tally {
    var tally = Tally()
    var alone <- scene_alone(load, lanes, reps, flood)
    let pace = flood ? "back to back" : join([for (lane in lanes); "{lane.period_ms} ms"], ", ")
    print("{tag}  scene: {length(lanes)} speech lanes beside a chat turn of {load.budget} tokens, {reps} rounds, a request every {pace}\n")
    var together : array<SceneSample>
    for (round in range(reps)) {
        scene_round(load, lanes, together, round)
    }
    for (lane in range(SCENE_CHAT, length(lanes))) {
        if (scene_lane_row(tag, lanes, lane, reps, alone, together)) {
            tally.rows++
        } else {
            tally.refused++
        }
    }
    delete alone
    delete together
    return tally
}

//! the chat rows the flags ask for: the size rows, then the follow-up row under `--follow-up`
def private chat_section(cfg : Args; tag : string; reps : int) : Tally {
    let plan = Plan(url = cfg.url, tag = tag, model = cfg.model, reply = cfg.reply > 0 ? cfg.reply : 200, reps = reps, think = cfg.think)
    let corpus = empty(cfg.corpus) ? path_join(get_das_root(), "modules/dasLLAMA/benchmarks/data/wikitext2_prompts.txt") : cfg.corpus
    var tally = chat_rows(plan, corpus, empty(cfg.sizes) ? "3000,9000" : cfg.sizes, cfg.settle, cfg.chars_per_token)
    if (cfg.follow_up) {
        if (followup_row(plan, "{ref_time_ticks()}")) {
            tally.rows++
        } else {
            tally.refused++
        }
    }
    return tally
}

//! the scene's lanes off the command line: an ASR lane a `--scene-asr` model on the first clip, then the first TTS model's
def scene_lanes(cfg : Args; tts_model, tts_text : string) : array<SceneJob> {
    var lanes : array<SceneJob>
    lanes |> reserve(length(cfg.scene_asr) + 2)
    if (empty(cfg.scene_asr)) {
        lanes |> push(SceneJob(url = cfg.asr_url, model = cfg.asr_model, payload = cfg.clip[0]))
    }
    for (model in cfg.scene_asr) {
        lanes |> push(SceneJob(lane = length(lanes), url = cfg.asr_url, model = model, payload = cfg.clip[0]))
    }
    lanes |> push(SceneJob(lane = length(lanes), tts = true, url = cfg.tts_url, model = tts_model, voice = cfg.tts_voice, payload = tts_text))
    return <- lanes
}

//! what the flags ask for that the other flags do not provide, "" when they stand together
def args_error(cfg : Args; reps : int) : string {
    let speech_on = !empty(cfg.asr_url) && !empty(cfg.clip)
    return "--reps {reps} - one rep has no spread, a row needs two or more" if (reps < 2)
    if (cfg.under_load && (empty(cfg.url) || !speech_on)) {
        return "--under-load times --clip rows at --asr-url beside a chat turn at --url - it needs all three"
    }
    if (!empty(cfg.image) && empty(cfg.url)) {
        return "--image asks the chat server at --url about each image - it needs --url"
    }
    if (cfg.follow_up && (empty(cfg.url) || cfg.no_chat)) {
        return "--follow-up is one of the chat rows at --url - it needs --url, and not --no-chat"
    }
    if (cfg.scene && (empty(cfg.url) || !speech_on || empty(cfg.tts_url))) {
        return "--scene times a chat turn at --url, the first --clip at --asr-url and a synthesis at --tts-url at once - it needs all of them"
    }
    return ""
}

[export]
def main : int {
    var r <- parse_args(type<Args>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<Args>), "served_bench")
        return 1
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<Args>), "served_bench")
        return 0
    }
    let speech_on = !empty(cfg.asr_url) && !empty(cfg.clip)
    let tts_on = !empty(cfg.tts_url)
    let chat_on = !cfg.no_chat && !empty(cfg.url)
    let reps = cfg.reps > 0 ? cfg.reps : 3
    let bad = args_error(cfg, reps)
    if (!empty(bad)) {
        print("error: {bad}\n")
        return 1
    }
    let tag = !empty(cfg.tag) ? cfg.tag : (empty(cfg.url) ? cfg.asr_url : cfg.url)
    var tally = Tally()
    if (chat_on) {
        tally = chat_section(cfg, tag, reps)
    }
    if (speech_on) {
        let load = LoadJob(url = cfg.url, model = cfg.model, question = LOAD_QUESTION, budget = cfg.load_tokens > 0 ? cfg.load_tokens : 1500)
        let heard = speech_rows(Speech(url = cfg.asr_url, tag = tag, model = cfg.asr_model), cfg.clip, reps, cfg.under_load, load)
        tally.rows += heard.rows
        tally.refused += heard.refused
    }
    let tts_text = empty(cfg.tts_text) ? TTS_TEXT : cfg.tts_text
    var tts_models <- empty(cfg.tts_model) ? [""] : clone_to_move(cfg.tts_model)
    if (tts_on) {
        let said = tts_rows(cfg.tts_url, tag, tts_models, cfg.tts_voice, tts_text, reps)
        tally.rows += said.rows
        tally.refused += said.refused
    }
    if (cfg.scene) {
        var lanes <- scene_lanes(cfg, tts_models[0], tts_text)
        let load = LoadJob(url = cfg.url, model = cfg.model, question = LOAD_QUESTION, budget = cfg.load_tokens > 0 ? cfg.load_tokens : 1500)
        let acted = scene_rows(tag, load, lanes, reps, cfg.scene_flood)
        tally.rows += acted.rows
        tally.refused += acted.refused
        delete lanes
    }
    delete tts_models
    if (!empty(cfg.url) && !empty(cfg.image)) {
        let looked = image_rows(Plan(url = cfg.url, tag = tag, model = cfg.model, reps = reps), cfg.image, "{ref_time_ticks()}")
        tally.rows += looked.rows
        tally.refused += looked.refused
    }
    if (!chat_on && !speech_on && !tts_on && !cfg.scene && empty(cfg.image)) {
        print("error: nothing to time - give --url for the chat rows (and --image for the image rows), --asr-url with --clip for the transcription rows, --tts-url for the synthesis rows, --scene for all of them at once\n")
    }
    return tally.rows == 0 || tally.refused > 0 ? 1 : 0
}
