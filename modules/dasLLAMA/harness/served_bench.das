options gen2
options indenting = 4

// served_bench - what a client of a live chat server waits for, read on ONE client clock for any
// OpenAI-compatible server: dasllama-server, or a third-party server beside it on the same box.
// ARCHITECTURE_MEASUREMENT_SERVED.md#served-turn-instrument names what its rows are and are not.
//
//   bin/daslang modules/dasLLAMA/harness/served_bench.das -- --url http://127.0.0.1:8080 [--label ours]
//       [--model <id>] [--sizes 3000,9000] [--reply 200] [--reps 3] [--settle 0] [--corpus <file>] [--think]
//       [--asr-url <base> --clip a.wav --clip b.wav [--asr-model <id>] [--under-load [--load-tokens 1500]]] [--no-chat]
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
//
// Speech rows (`--asr-url`, one row a `--clip`): the wall of a whole transcription request, the clip
// uploaded as a multipart form, over `--reps` reps behind one untimed request; every rep must read
// the same text. `--under-load` adds a second row a clip, timed while a chat turn of `--load-tokens`
// tokens decodes at `--url`: its reps start at that turn's first reply text, the row is refused when
// the turn ends before they do or the text differs from the idle row's.

require daslib/clargs
require daslib/strings_boost
require openai/openai_chat   // chat_stream: the streamed turn, its usage chunk and the server's own clock
require openai/openai_audio   // transcribe: the speech row's request
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

//! the rows a run reported and the ones it refused
struct Tally {
    rows : int
    refused : int
}

let LOAD_DECODING = 1   //! the load turn's state once its first reply text arrived
let LOAD_ENDED = 2
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

//! one whole transcription request under the client clock, the clip uploaded as a multipart form
def transcribe(speech : Speech; clip : string) : Heard {
    var heard = Heard()
    let t0 = ref_time_ticks()
    let res = openai_audio::transcribe(client_of(speech.url), speech.model, clip)
    heard.wall_us = int64(get_time_usec(t0))
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
    var walls : array<float>
    walls |> reserve(reps)
    var text = want
    for (rep in range(reps)) {
        let heard = transcribe(speech, clip)
        if (empty(heard.why) && !empty(text) && heard.text != text) {
            print("{speech.tag}  {what}  REFUSED at rep {rep}: the text differs from the text read before\n")
            return ""
        }
        if (!empty(heard.why)) {
            print("{speech.tag}  {what}  REFUSED at rep {rep}: {heard.why}\n")
            return ""
        }
        text = heard.text
        walls |> push(float(heard.wall_us) / 1000.0)
    }
    let s = spread_of(walls)
    print("{speech.tag}  {what}: {s.median:.1f} ms (cv {s.cv:.1f}%) over {reps} reps, {length(text)} characters read{flag(s.cv)}\n")
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
    let chat_on = !cfg.no_chat && !empty(cfg.url)
    let reps = cfg.reps > 0 ? cfg.reps : 3
    if (reps < 2) {
        print("error: --reps {reps} - one rep has no spread, a row needs two or more\n")
        return 1
    }
    if (cfg.under_load && (empty(cfg.url) || !speech_on)) {
        print("error: --under-load times --clip rows at --asr-url beside a chat turn at --url - it needs all three\n")
        return 1
    }
    let tag = !empty(cfg.tag) ? cfg.tag : (empty(cfg.url) ? cfg.asr_url : cfg.url)
    var tally = Tally()
    if (chat_on) {
        let plan = Plan(url = cfg.url, tag = tag, model = cfg.model, reply = cfg.reply > 0 ? cfg.reply : 200, reps = reps, think = cfg.think)
        let corpus = empty(cfg.corpus) ? path_join(get_das_root(), "modules/dasLLAMA/benchmarks/data/wikitext2_prompts.txt") : cfg.corpus
        tally = chat_rows(plan, corpus, empty(cfg.sizes) ? "3000,9000" : cfg.sizes, cfg.settle, cfg.chars_per_token)
    }
    if (speech_on) {
        let load = LoadJob(url = cfg.url, model = cfg.model, question = LOAD_QUESTION, budget = cfg.load_tokens > 0 ? cfg.load_tokens : 1500)
        let heard = speech_rows(Speech(url = cfg.asr_url, tag = tag, model = cfg.asr_model), cfg.clip, reps, cfg.under_load, load)
        tally.rows += heard.rows
        tally.refused += heard.refused
    }
    if (!chat_on && !speech_on) {
        print("error: nothing to time - give --url for the chat rows, --asr-url with --clip for the speech rows\n")
    }
    return tally.rows == 0 || tally.refused > 0 ? 1 : 0
}
