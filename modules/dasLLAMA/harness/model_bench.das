options gen2
options indenting = 4

// model_bench - a model's own prefill and decode rates, read through any OpenAI-compatible chat server on ONE client
// clock: dasllama-server, or a third-party server beside it on the same box.
//
//   bin/daslang modules/dasLLAMA/harness/model_bench.das -- --url http://127.0.0.1:8080 [--model <config entry>]
//       [--config <json>] [--quick] [--max-length N] [--cell-seconds S] [--reps N] [--model-id <id>] [--corpus <file>]
//       [--streams N] [--wide] [--arm <name>] [--out <tests.json>] [--raw <requests.jsonl>] [--label <label>]
//
// A cell is one prompt length: `reps` streamed chat turns, greedy, thinking off, each a prompt no server has seen (a
// nonce opens it, and `cache_prompt: false` asks a llama.cpp-family server to keep it out of its cache), answered in
// `reply_tokens` tokens. ONE request gives both rates: prefill = the prompt tokens over the time to the first reply
// text, decode = the reply's tokens after the first over the time after it - the decode rate AT that context length.
// The prompt is Python sources (`benchmarks/data/python_context.txt`) cut to the length, then a code-writing task; the
// shortest cell is the task alone. Token counts are the server's own, off the closing usage chunk: one untimed turn a
// cell reads the tokens its cut makes and the cut is scaled to the length (the cell's warm-up too).
// The ladder climbs the config's lengths and stops at the first cell its own measured rates predict past
// `cell_seconds`; a turn that outlasts three times its prediction is cut off and its cell refused. A cell is refused,
// with the reason, when a turn fails, carries no usage chunk, reads prompt tokens off a cache, or ends early.
// Every cell runs behind one untimed turn it drops: a context cell's sizing turn, and an 8-token turn of the task alone
// for the shortest cell and for every width cell; a warm-up turn that fails refuses its cell.
// Flags, printed on the row and written with it: `noisy` (cv past 3%), `cliff` (a rate under `cliff_share` of the rung
// before it: a prefill rate at twice the length or more, a decode rate at any later rung), `stall` (one gap between
// reply chunks past `stall_times` the median gap).
// The width rows follow the ladder, at the config's `width_lengths`: a wave of 2, 4 (and under `--wide` 8, 16, 32)
// streams a rep, each its own prompt, sent at once. A width row is the streams summed - their prompt tokens over the
// time to the wave's last first text, their decode rates added - named `pp4096@4`, `tg128+4096@4`, `tg128@4`. A wave
// wider than `--streams` is not sent: it would time the server's queue. `--wide` is a pass of its own against a server
// started with that many streams (the runner's `wide_server`): the wide widths alone, over base cells it does not record. Their flags: `noisy` (cv past 6%), `scaling`
// (a summed rate under nine tenths of the width before it), `unfair` (the last first text past `unfair_times` x the width x the first).
// `--arm <name>` is a run against a server started to speculate (the runner's job): the decode rows alone, one
// stream, at the config's `width_lengths`, in replies of `arm_reply_tokens` (a drafter is read past a reply's opening),
// named `tg512:<name>`, `tg512+4096:<name>`, each with the drafted and the accepted tokens its turns report
// (`timings.draft_n`, `draft_n_accepted`). `--arm off` takes the same rows at a plain server, unsuffixed: what an
// arm's rows are read against.
// Exit 1 when a cell was refused or none was reported.

require daslib/clargs
require daslib/strings_boost
require daslib/fio
require daslib/jobque_boost   // a wave's threads: each request over a Stream, its turn back over a channel
require strings
require math
require openai/openai_chat  // chat_stream: the streamed turn, its usage chunk and the server's own clock
require _row_stats   // spread_of: the cell statistic over reps
require _served_turn   // stream_marked: the streamed turn under the client clock, the served instruments' one
require _corpus   // corpus_text: the context the prompts are cut from
require _model_bench   // the config's shape and the run file's

[CommandLineArgs]
struct BenchArgs {
    @clarg_short = "u"
    @clarg_doc = "Base URL of the chat server under the clock, e.g. http://127.0.0.1:8080"
    url : string

    @clarg_short = "m"
    @clarg_doc = "The config entry to run, a name under the config's \"models\" (default: the config's defaults alone)"
    @clarg_name = "model"
    entry : string

    @clarg_doc = "The config file (default: modules/dasLLAMA/harness/model_bench.json)"
    config : string

    @clarg_doc = "Quick mode: one rep a cell"
    quick : bool

    @clarg_name = "max-length"
    @clarg_doc = "The longest prompt to try, in tokens (default: the config's whole ladder)"
    max_length : int

    @clarg_name = "cell-seconds"
    @clarg_doc = "Seconds a cell may take: the ladder stops at the first cell predicted past it (default: the config's)"
    cell_seconds : int

    @clarg_doc = "Reps a cell (default: the config's)"
    reps : int

    @clarg_doc = "The streams the server runs at once: a wave wider than this is not sent (default: every width of the config is)"
    streams : int

    @clarg_doc = "The wide pass: the config's wide widths (8, 16, 32) alone, at its width lengths, for a server started with that many streams"
    wide : bool

    @clarg_doc = "The speculative arm the server was started for: its long-reply decode rows alone, at the config's width lengths; `off` takes them at a plain server"
    arm : string

    @clarg_name = "model-id"
    @clarg_doc = "The request's \"model\" field (default: none - the server's own default model)"
    model_id : string

    @clarg_doc = "Text the context is cut from (default: benchmarks/data/python_context.txt)"
    corpus : string

    @clarg_doc = "Write the run - its rows as a ladder record's \"tests\" object, what it refused, how it ran - to this file"
    out : string

    @clarg_doc = "Append every timed request to this file, one JSON object a line"
    raw : string

    @clarg_doc = "Row label (default: the config entry, else the URL)"
    @clarg_name = "label"
    row_label : string

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

//! one streamed turn as the client saw it
struct BenchTurn {
    cell : string               //! the row the turn belongs to
    rep : int
    error : string              //! the transport or HTTP failure, "" for a turn that streamed
    ttft_us : int64             //! send -> the first chunk carrying reply text; -1 when none came
    gen_us : int64              //! that chunk -> the last one carrying reply text
    median_gap_us : int64       //! between two reply chunks
    longest_gap_us : int64
    prompt_tokens : int64
    cached_tokens : int64       //! -1: the server reported none
    completion_tokens : int64
    finish : string
    server_ttft_ms : float        //! the server's own admit -> first token, 0 from a server that reports none
    server_gen_ms : float
    draft_n : int64
    draft_n_accepted : int64
    reply_head : string         //! the reply's opening, for a reader checking the model answered the task
}

let CV_NOISY_WIDE_PCT = 6.0        //! a width row's: several streams at once spread more
let WARM_TOKENS = 8             //! a warm-up turn's reply
let SCALING_SHARE = 0.9        //! a summed rate under this share of the width before it lost ground: a server that prefills one prompt at a time holds level
let CACHED_MAX = 64l           //! prompt tokens a turn may read off a cache: a chat template's fixed head, never the context
let STALL_FLOOR_US = 200000l   //! a gap shorter than this is no stall, whatever the median
let FIRST_TIMEOUT_S = 600      //! a turn ahead of any measured rate
let TASK = "Write a complete, well-commented Python module that implements a token-bucket rate limiter with per-key buckets, a sliding-window variant, a thread-safe wrapper, and a pytest test suite. Write at least 600 lines. Do not stop early. Reply with the code only: start with the first line of the module, no introduction."

//! a prompt no server has seen: `chars` characters of `base` under the nonce, then the task; the task alone at 0
def prompt_of(base, nonce : string; chars : int) : string {
    // a bare "Request <id>." reads to a small model as the request, and it answers with the id
    return "Reference id {nonce} (not part of the task).\n\nThe task: {TASK}" if (chars <= 0)
    return build_string() $(var w) {
        w |> write("Reference id {nonce} (not part of the task). The files of a code base follow; the task comes after them.\n\n")
        w |> write(slice(base, 0, min(chars, length(base))))
        w |> write("\n\nThe task: ")
        w |> write(TASK)
    }
}

//! one streamed chat turn under the client clock, cut off after `timeout_s`
def bench_turn(url, model_id, prompt : string; budget, timeout_s : int) : BenchTurn {
    var turn = BenchTurn(cached_tokens = -1l, ttft_us = -1l)
    var req <- ChatCompletionRequest(model = model_id, max_tokens = budget,
                                     messages <- [ChatMessage(role = "user", content = prompt)],
                                     stream_options = StreamOptions(include_usage = true),
                                     chat_template_kwargs = "\{\"enable_thinking\":false}", cache_prompt = "false")
    var got <- stream_marked(url, req, timeout_s)
    delete req
    turn.prompt_tokens = int64(got.res.usage.prompt_tokens)
    turn.completion_tokens = int64(got.res.usage.completion_tokens)
    turn.cached_tokens = int64(got.res.usage.prompt_tokens_details.cached_tokens)
    turn.finish = got.res.finish_reason
    turn.server_ttft_ms = got.res.timings.ttft_ms > 0.0 ? got.res.timings.ttft_ms : got.res.timings.prompt_ms   // ours, else llama.cpp-server's
    turn.server_gen_ms = got.res.timings.gen_ms > 0.0 ? got.res.timings.gen_ms : got.res.timings.predicted_ms
    turn.draft_n = int64(got.res.timings.draft_n)
    turn.draft_n_accepted = int64(got.res.timings.draft_n_accepted)
    turn.reply_head = got.marks.head
    if (!got.res.ok) {
        turn.error = "{got.res.error.kind} {got.res.error.status} {got.res.error.message}"
    }
    if (got.marks.first_us >= 0l) {
        turn.ttft_us = got.marks.first_us
        turn.gen_us = got.marks.last_us - got.marks.first_us
    }
    turn.median_gap_us = got.marks.median_gap_us
    turn.longest_gap_us = got.marks.longest_gap_us
    delete got
    return turn
}

//! why a timed turn cannot enter its cell, "" when it can
//! why `turn` was not served at all: its error, or a stream with no reply text; "" for a turn that was
def unserved_why(turn : BenchTurn) : string
    => !empty(turn.error) ? turn.error : (turn.ttft_us < 0l ? "the stream carried no reply text" : "")

def refusal_of(turn : BenchTurn; budget : int) : string {
    let unserved = unserved_why(turn)
    return unserved if (!empty(unserved))
    return "the stream carried no usage chunk (stream_options.include_usage)" if (turn.prompt_tokens == 0l)
    return "{turn.cached_tokens} of {turn.prompt_tokens} prompt tokens came off a cache" if (turn.cached_tokens > CACHED_MAX)
    return "the reply ended at {turn.completion_tokens} of {budget} tokens (finish_reason {turn.finish})" if (turn.completion_tokens < int64(budget))
    return "a reply of {turn.completion_tokens} token(s) has no decode span" if (turn.completion_tokens < 2l || turn.gen_us <= 0l)
    return ""
}

def prefill_rate(turn : BenchTurn) : float => float(turn.prompt_tokens - max(turn.cached_tokens, 0l)) * 1.0e6 / float(turn.ttft_us)


def private append_line(path, line : string) {
    return if (empty(path))
    fopen(path, "ab") $(f) {
        if (f != null) {
            f |> fwrite("{line}\n")
        }
    }
}

//! a cell's reps as its two rows; `why` is non-empty when a turn refused the cell
struct Cell {
    why : string
    prompt_tokens : int64
    prefill : RowSpread
    decode : RowSpread
    ttft_ms : float
    server_prefill : float         //! the server's own clock for the same turns, 0 where a turn reported none
    server_decode : float
    stalled : bool
    cut_chars : int
    draft_n : int64             //! the tokens its turns' speculative rounds drafted, and the ones accepted
    draft_n_accepted : int64
}

//! what the length ladder read at one length, for the width rows there: the cut, and the one-stream rates
struct LengthBase {
    cut_chars : int
    prefill : float             //! 0 at the task alone
    decode : float
}

//! one request of a wave; it crosses to its thread as an archive
struct WaveJob {
    url : string
    model_id : string
    prompt : string
    budget : int
    timeout_s : int
}

//! a width cell's reps - a wave of streams a rep - as its two rows; `why` is non-empty when a turn refused the cell
struct WaveCell {
    why : string
    prompt_tokens : int64
    prefill : RowSpread         //! every stream's prompt tokens over the wave's time to its last first text
    decode : RowSpread          //! the streams' decode rates summed
    stream_decode : float       //! one stream's median decode rate
    last_ttft_ms : float
    unfair : bool
}

//! The characters of context a prompt of `want` tokens takes: one untimed turn of `chars` characters, answered in one
//! token, reads the tokens they make (the corpus is not one density throughout) and the count is scaled once. That turn
//! is the cell's warm-up too. 0 when the turn failed.
def private sized_chars(args : BenchArgs; name, base, run : string; chars, timeout_s : int; want, base_tokens : int64) : int {
    let sizing = bench_turn(args.url, args.model_id, prompt_of(base, "{run}-{name}-size", chars), 1, timeout_s)
    return 0 if (sizing.prompt_tokens <= base_tokens)
    return min(length(base), int(float(chars) * float(want - base_tokens) / float(sizing.prompt_tokens - base_tokens)))
}

//! the untimed short turn a cell with no sizing turn runs behind; "" when it served, else why the cell cannot run
def private warm_up(args : BenchArgs; base, nonce : string; timeout_s : int) : string {
    let why = unserved_why(bench_turn(args.url, args.model_id, prompt_of(base, nonce, 0), WARM_TOKENS, timeout_s))
    return empty(why) ? "" : "the warm-up turn: {why}"
}

//! a turn's timeout: three times the seconds its cell's measured rates predict, and a floor
def private turn_timeout_s(predicted_s : float) : int => max(60, int(3.0 * predicted_s) + 30)

//! `reps` turns of one prompt length behind one untimed turn: the sizing turn of a context cell, a short turn of the task alone
def run_cell(args : BenchArgs; name, base, run : string; chars, reps, budget, timeout_s : int; stall_times : float; want, base_tokens : int64) : Cell {
    var cell = Cell()
    var prefill, decode, ttft, server_prefill, server_decode : array<float>
    var cut = 0
    if (chars > 0) {
        cut = sized_chars(args, name, base, run, chars, timeout_s, want, base_tokens)
        if (cut <= 0) {
            cell.why = "the sizing turn failed"
            return cell
        }
    } else {
        cell.why = warm_up(args, base, "{run}-{name}-warm", timeout_s)
    }
    for (rep in range(reps)) {
        break if (!empty(cell.why))
        var turn = bench_turn(args.url, args.model_id, prompt_of(base, "{run}-{name}-{rep}", cut), budget, timeout_s)
        turn.cell = name
        turn.rep = rep
        append_line(args.raw, sprint_json(turn, false))
        cell.why = refusal_of(turn, budget)
        if (!empty(cell.why)) {
            cell.why = "at rep {rep}: {cell.why}"
            break
        }
        prefill |> push(prefill_rate(turn))
        decode |> push(reply_rate(turn.completion_tokens, float(turn.gen_us)))
        ttft |> push(float(turn.ttft_us) / 1000.0)
        server_prefill |> push(turn.server_ttft_ms > 0.0 ? float(turn.prompt_tokens - max(turn.cached_tokens, 0l)) * 1000.0 / turn.server_ttft_ms : 0.0)
        server_decode |> push(reply_rate(turn.completion_tokens, turn.server_gen_ms * 1000.0))
        cell.prompt_tokens = turn.prompt_tokens
        cell.draft_n += turn.draft_n
        cell.draft_n_accepted += turn.draft_n_accepted
        cell.stalled ||= turn.longest_gap_us > STALL_FLOOR_US && float(turn.longest_gap_us) > stall_times * float(turn.median_gap_us)
    }
    if (empty(cell.why)) {
        cell.prefill = spread_of(prefill)
        cell.decode = spread_of(decode)
        cell.ttft_ms = spread_of(ttft).median
        cell.server_prefill = readings_median(server_prefill)
        cell.server_decode = readings_median(server_decode)
        cell.cut_chars = cut
    }
    delete prefill
    delete decode
    delete ttft
    delete server_prefill
    delete server_decode
    return cell
}

//! `width` turns sent at once, each a prompt of its own: every thread is up before the first request goes
def run_wave(args : BenchArgs; name, base, run : string; rep, chars, width, budget, timeout_s : int) : array<BenchTurn> {
    var turns : array<BenchTurn>
    with_stream() $(var jobs : Stream?) {
        for (i in range(width)) {
            var job = WaveJob(url = args.url, model_id = args.model_id, prompt = prompt_of(base, "{run}-{name}-{rep}-{i}", chars),
                budget = budget, timeout_s = timeout_s)
            jobs |> push_archive(job)
        }
        with_channel(width) $(out) {
            with_atomic32() $(armed) {
                with_atomic32() $(go) {
                    for (i in range(width)) {
                        new_thread() @() {
                            jobs |> pop_archive() $(var job : WaveJob) {
                                armed |> inc
                                while (go |> get == 0) {
                                    sleep(1u)
                                }
                                let turn = bench_turn(job.url, job.model_id, job.prompt, job.budget, job.timeout_s)
                                out |> push_clone(turn)
                            }
                            jobs |> release
                            out |> notify_and_release
                        }
                        while (armed |> get <= i) {   // one thread at the stream at a time: each takes its job before the next starts
                            sleep(1u)
                        }
                    }
                    go |> set(1)
                    out |> for_each_clone() $(turn : BenchTurn#) {
                        var landed : BenchTurn
                        landed := turn
                        // the turn's strings live in its thread's heap, which goes with the thread: each is copied into this one
                        landed.error = clone_string(turn.error)
                        landed.finish = clone_string(turn.finish)
                        landed.reply_head = clone_string(turn.reply_head)
                        turns |> emplace(landed)
                    }
                }
            }
        }
    }
    return <- turns
}

//! `reps` waves of `width` streams at one prompt length, behind one untimed short turn
def run_wave_cell(args : BenchArgs; name, base, run : string; chars, width, reps, budget, timeout_s : int; unfair_times : float) : WaveCell {
    var cell = WaveCell()
    var prefill, decode, per_stream, last_ttft : array<float>
    per_stream |> reserve(width * reps)
    cell.why = warm_up(args, base, "{run}-{name}-warm", timeout_s)
    for (rep in range(reps)) {
        break if (!empty(cell.why))
        var turns <- run_wave(args, name, base, run, rep, chars, width, budget, timeout_s)
        var tokens = 0l
        var first_us = 0l
        var last_us = 0l
        var summed_decode = 0.0
        for (turn in turns) {
            turn.cell = name
            turn.rep = rep
            append_line(args.raw, sprint_json(turn, false))
            let why = refusal_of(turn, budget)
            if (!empty(why) && empty(cell.why)) {
                cell.why = "at rep {rep}: {why}"
            }
            tokens += turn.prompt_tokens - max(turn.cached_tokens, 0l)
            first_us = first_us == 0l ? turn.ttft_us : min(first_us, turn.ttft_us)
            last_us = max(last_us, turn.ttft_us)
            let stream_rate = reply_rate(turn.completion_tokens, float(turn.gen_us))
            summed_decode += stream_rate
            per_stream |> push(stream_rate)
            cell.prompt_tokens = turn.prompt_tokens
        }
        delete turns
        break if (!empty(cell.why))
        prefill |> push(float(tokens) * 1.0e6 / float(last_us))
        decode |> push(summed_decode)
        last_ttft |> push(float(last_us) / 1000.0)
        cell.unfair ||= float(last_us) > unfair_times * float(width) * float(first_us)
    }
    if (empty(cell.why)) {
        cell.prefill = spread_of(prefill)
        cell.decode = spread_of(decode)
        cell.stream_decode = spread_of(per_stream).median
        cell.last_ttft_ms = spread_of(last_ttft).median
    }
    delete prefill
    delete decode
    delete per_stream
    delete last_ttft
    return cell
}

def private row_of(s : RowSpread; tokens : int64; reps : int; own : float; flags : string) : TestRow
    => TestRow(tok_s = double(s.median), stddev = double(s.cv * s.mean / 100.0), prompt_tokens = tokens, reps = reps,
        server_tok_s = double(own), flags = flags)

def flags_of(s : RowSpread; before, cliff_share : float; comparable, stalled : bool) : string {
    var flags : array<string>
    if (s.cv > CV_ROW_PCT) {
        flags |> push("noisy")
    }
    if (comparable && before > 0.0 && s.median < cliff_share * before) {
        flags |> push("cliff")
    }
    if (stalled) {
        flags |> push("stall")
    }
    let text = join(flags, ",")
    delete flags
    return text
}

def private server_note(own : float) : string => own > 0.0 ? "  [server's own {own:.1f}]" : ""

def private flag_note(flags : string) : string => empty(flags) ? "" : "  FLAGS: {flags}"

//! One rung's cell as its rows, printed and - but in the wide pass, whose one-stream cells are its base alone - recorded:
//! the prefill row of a context rung of the plain pass, and the decode row with the turn's draft counts.
def private rung_rows(args : BenchArgs; s : Settings; row_label, pp_name, tg_name : string; len : int; cell : Cell; last_pp, last_tg : float; doubled, first : bool; var into : RunOut) {
    if (len > 0 && empty(args.arm) && !args.wide) {   // the task alone has no prefill rate worth a row, and an arm's prefill is the plain one's
        let pp_flags = flags_of(cell.prefill, last_pp, s.cliff_share, doubled, false)
        into.tests |> insert(pp_name, row_of(cell.prefill, cell.prompt_tokens, s.reps, cell.server_prefill, pp_flags))
        print("{row_label}  {pp_name}: {cell.prefill.median:.1f} tok/s (cv {cell.prefill.cv:.1f}%), {cell.prompt_tokens} prompt tokens, first text after {cell.ttft_ms / 1000.0:.2f} s{server_note(cell.server_prefill)}{flag_note(pp_flags)}\n")
    }
    let tg_flags = flags_of(cell.decode, last_tg, s.cliff_share, !first, cell.stalled)
    var tg_row = row_of(cell.decode, cell.prompt_tokens, s.reps, cell.server_decode, tg_flags)
    tg_row.ttft_ms = double(cell.ttft_ms)
    tg_row.draft_n = cell.draft_n
    tg_row.draft_n_accepted = cell.draft_n_accepted
    if (!args.wide) {
        into.tests |> insert(tg_name, tg_row)
    }
    let drafts = cell.draft_n > 0l ? ", {cell.draft_n_accepted} of {cell.draft_n} drafted tokens accepted" : (empty(args.arm) || args.arm == "off" ? "" : ", NO draft reported")
    print("{row_label}  {tg_name}: {cell.decode.median:.1f} tok/s (cv {cell.decode.cv:.1f}%) after {cell.prompt_tokens} prompt tokens{drafts}{server_note(cell.server_decode)}{flag_note(tg_flags)}\n")
}

[arch(at = "../ARCHITECTURE_MEASUREMENT_SERVED.md#model-bench")]
def private length_ladder(args : BenchArgs; s : Settings; row_label, base, run : string; chars_per_token : float; base_tokens : int64; probe_rate : float; var into : RunOut; var bases : table<int; LengthBase>) {
    var last_len = 0
    var last_pp = probe_rate
    var last_tg = 0.0
    var first = true
    for (len in s.lengths) {
        if (args.max_length > 0 && len > args.max_length) {
            into.bench.stopped = "at {last_len} tokens: --max-length {args.max_length}"
            break
        }
        let turn_s = first ? 0.0 : float(len) / last_pp + float(s.reply_tokens) / last_tg
        let cell_s = turn_s * float(s.reps) + (first ? 0.0 : float(len) / last_pp)   // the reps and the sizing turn's prefill
        if (!first && cell_s > float(s.cell_seconds)) {
            into.bench.stopped = "at {last_len} tokens: {s.reps} turn(s) of {len} tokens would take about {cell_s:.0f} s, past the {s.cell_seconds} s a cell may"
            break
        }
        let pp_name = "pp{len}"
        let arm = empty(args.arm) || args.arm == "off" ? "" : ":{args.arm}"
        let tg_name = len == 0 ? "tg{s.reply_tokens}{arm}" : "tg{s.reply_tokens}+{len}{arm}"
        let chars = len == 0 ? 0 : int(float(int64(len) - base_tokens) * chars_per_token)
        if (chars > length(base)) {
            into.bench.stopped = "at {last_len} tokens: the corpus holds {length(base)} characters, {len} tokens need {chars}"
            break
        }
        let cell = run_cell(args, len == 0 ? "short" : pp_name, base, run, chars, s.reps, s.reply_tokens, first ? FIRST_TIMEOUT_S : turn_timeout_s(turn_s), s.stall_times, int64(len), base_tokens)
        if (!empty(cell.why)) {
            print("{row_label}  {len == 0 ? tg_name : pp_name}  REFUSED {cell.why}\n")
            into.refused |> push(Refusal(cell = len == 0 ? tg_name : pp_name, why = cell.why))
            into.bench.stopped = "at {last_len} tokens: the cell of {len} was refused"
            break
        }
        rung_rows(args, s, row_label, pp_name, tg_name, len, cell, last_pp, last_tg, last_len > 0 && len >= 2 * last_len, first, into)
        if (len > 0) {
            last_pp = cell.prefill.median
        }
        last_tg = cell.decode.median
        last_len = len
        first = false
        bases |> insert(len, LengthBase(cut_chars = cell.cut_chars, prefill = len > 0 ? cell.prefill.median : 0.0, decode = cell.decode.median))
    }
}

def wave_flags_of(s : RowSpread; before : float; unfair : bool) : string {
    var flags : array<string>
    if (s.cv > CV_NOISY_WIDE_PCT) {
        flags |> push("noisy")
    }
    if (before > 0.0 && s.median < SCALING_SHARE * before) {
        flags |> push("scaling")
    }
    if (unfair) {
        flags |> push("unfair")
    }
    let text = join(flags, ",")
    delete flags
    return text
}

//! The width rows of one prompt length: a wave of 2, 4, ... streams a rep, each row the streams summed, against the width
//! before it. Stops at the first width past the server's streams, past the cell's time, or refused.
def private width_rows(args : BenchArgs; s : Settings; row_label, base, run : string; len : int; one_stream : LengthBase; widths : array<int>; var into : RunOut) {
    var before_pp = one_stream.prefill
    var before_tg = one_stream.decode
    for (width in widths) {
        continue if (width <= 1)
        let at = len == 0 ? "" : "+{len}"
        let pp_name = "pp{len}@{width}"
        let tg_name = "tg{s.reply_tokens}{at}@{width}"
        let shown = len == 0 ? tg_name : pp_name
        if (args.streams > 0 && width > args.streams) {
            print("{row_label}  {shown}  SKIPPED: the server runs {args.streams} stream(s), a wider wave would time its queue\n")
            break
        }
        // a wave's prompts prefill one after another and its replies decode batched, at the summed rate of the width before
        let turn_s = (len > 0 ? float(len) * float(width) / one_stream.prefill : 0.0) + float(s.reply_tokens) * float(width) / before_tg
        let cell_s = turn_s * float(s.reps)
        if (cell_s > float(s.cell_seconds)) {
            print("{row_label}  {shown}  SKIPPED: {s.reps} wave(s) of {width} could take about {cell_s:.0f} s, past the {s.cell_seconds} s a cell may\n")
            break
        }
        let cell = run_wave_cell(args, shown, base, run, one_stream.cut_chars, width, s.reps, s.reply_tokens, turn_timeout_s(turn_s), s.unfair_times)
        if (!empty(cell.why)) {
            print("{row_label}  {shown}  REFUSED {cell.why}\n")
            into.refused |> push(Refusal(cell = shown, why = cell.why))
            break
        }
        if (len > 0) {
            let pp_flags = wave_flags_of(cell.prefill, before_pp, cell.unfair)
            var pp_row = row_of(cell.prefill, cell.prompt_tokens, s.reps, 0.0, pp_flags)
            pp_row.ttft_ms = double(cell.last_ttft_ms)
            into.tests |> insert(pp_name, pp_row)
            print("{row_label}  {pp_name}: {cell.prefill.median:.1f} tok/s combined (cv {cell.prefill.cv:.1f}%), {width} prompts of {cell.prompt_tokens} tokens, the last first text after {cell.last_ttft_ms / 1000.0:.2f} s{flag_note(pp_flags)}\n")
            before_pp = cell.prefill.median
        }
        let tg_flags = wave_flags_of(cell.decode, before_tg, len == 0 && cell.unfair)
        var tg_row = row_of(cell.decode, cell.prompt_tokens, s.reps, 0.0, tg_flags)
        tg_row.stream_tok_s = double(cell.stream_decode)
        into.tests |> insert(tg_name, tg_row)
        print("{row_label}  {tg_name}: {cell.decode.median:.1f} tok/s combined over {width} streams (cv {cell.decode.cv:.1f}%){flag_note(tg_flags)}\n")
        before_tg = cell.decode.median
    }
}

//! the width rows at every length the config names and the length ladder reached
def private width_ladder(args : BenchArgs; s : Settings; row_label, base, run : string; var bases : table<int; LengthBase>; var into : RunOut) {
    var widths : array<int>
    if (args.wide) {
        widths := s.wide_widths
    } else {
        widths := s.widths
    }
    for (len in s.width_lengths) {
        if (!key_exists(bases, len)) {
            print("{row_label}  the width rows at {len} tokens SKIPPED: the length ladder did not reach it\n")
            continue
        }
        width_rows(args, s, row_label, base, run, len, unsafe(bases[len]), widths, into)
    }
    delete widths
}

//! The settings a run uses - its config entry's, the command line's reps and cell time over them, and the width lengths
//! alone as its ladder under an arm or the wide pass - beside why they cannot run, "" when they can.
def private run_settings(args : BenchArgs) : tuple<s : Settings; unfit : string> {
    let config_path = empty(args.config) ? default_config_path() : args.config
    var config : BenchConfig
    if (!read_config(config_path, config)) {
        var none : Settings
        return <- (s <- none, unfit = "the config {config_path} does not read")
    }
    var picked <- settings_of(config, args.entry)
    delete config
    if (!empty(args.entry) && !picked.known) {
        print("model_bench: '{args.entry}' is not in {config_path} - running the defaults\n")
    }
    var s <- picked.s
    s.reps = args.reps > 0 ? args.reps : (args.quick ? 1 : s.reps)
    s.cell_seconds = args.cell_seconds > 0 ? args.cell_seconds : s.cell_seconds
    if (!empty(args.arm) || args.wide) {
        s.lengths := s.width_lengths
    }
    if (!empty(args.arm) && s.arm_reply_tokens > 0) {
        s.reply_tokens = s.arm_reply_tokens
    }
    let fits = !empty(s.lengths) && s.reps >= 1 && s.reply_tokens >= 2 && s.cell_seconds >= 1
    let unfit = fits ? "" : "the settings of '{args.entry}' in {config_path} name no lengths, no reps, no cell time or a reply under 2 tokens"
    return <- (s <- s, unfit = unfit)
}

//! One bench run against `args.url` under its config entry: the ladder, then the width rows, every row and refusal into
//! `res` and the run file written. 0 when every cell reported; 1 when one was refused, none reported, or the run could
//! not start (the config, the corpus, the calibration turns)
def run_bench(args : BenchArgs; var res : RunOut) : int {
    var settled <- run_settings(args)
    if (!empty(settled.unfit)) {
        print("error: {settled.unfit}\n")
        return 1
    }
    var s <- settled.s
    let corpus = empty(args.corpus) ? path_join(get_das_root(), "modules/dasLLAMA/benchmarks/data/python_context.txt") : args.corpus
    let base = corpus_text(corpus)
    if (empty(base)) {
        print("error: no text in the corpus {corpus}\n")
        return 1
    }
    let row_label = !empty(args.row_label) ? args.row_label : (!empty(args.entry) ? args.entry : args.url)
    let run = "{ref_time_ticks()}"
    let short = bench_turn(args.url, args.model_id, prompt_of(base, "{run}-calibration-short", 0), 8, FIRST_TIMEOUT_S)
    let probe = bench_turn(args.url, args.model_id, prompt_of(base, "{run}-calibration", CALIBRATION_CHARS), 8, FIRST_TIMEOUT_S)
    if (short.prompt_tokens <= 0l || probe.prompt_tokens <= short.prompt_tokens || probe.ttft_us <= 0l) {
        print("error: the calibration turns at {args.url} failed - {short.error} {probe.error} ({short.prompt_tokens} and {probe.prompt_tokens} prompt tokens)\n")
        return 1
    }
    let chars_per_token = float(CALIBRATION_CHARS) / float(probe.prompt_tokens - short.prompt_tokens)
    res.bench = Provenance(cmd = join(get_command_line_arguments(), " "), url = args.url, entry = args.entry,
        reply_tokens = s.reply_tokens, reps = s.reps, cell_seconds = s.cell_seconds, corpus = base_name(corpus),
        corpus_bytes = length(base), chars_per_token = chars_per_token, base_tokens = short.prompt_tokens)
    print("model_bench: {row_label} at {args.url}, reply {s.reply_tokens} tokens, {s.reps} rep(s) a cell, {s.cell_seconds} s a cell, {chars_per_token:.3f} chars per token, the task alone {short.prompt_tokens} tokens\n")
    var bases : table<int; LengthBase>
    length_ladder(args, s, row_label, base, run, chars_per_token, short.prompt_tokens, prefill_rate(probe), res, bases)
    if (!empty(res.bench.stopped)) {
        print("{row_label}  the ladder stopped {res.bench.stopped}\n")
    }
    if (empty(args.arm)) {
        width_ladder(args, s, row_label, base, run, bases, res)
    }
    delete bases
    let rows = length(res.tests)
    let refused = length(res.refused)
    if (!empty(args.out) && !fwrite(args.out, sprint_json(res, true))) {
        print("error: could not write {args.out}\n")
        return 1
    }
    print("model_bench: {rows} row(s), {refused} cell(s) refused\n")
    return refused > 0 || rows == 0 ? 1 : 0
}

[export]
def main : int {
    var r <- parse_args(type<BenchArgs>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<BenchArgs>), "model_bench")
        return 1
    }
    let args <- r |> move_unwrap
    if (args.help || empty(args.url)) {
        print_help(get_command_info(type<BenchArgs>), "model_bench")
        return args.help ? 0 : 1
    }
    var res : RunOut
    let rc = run_bench(args, res)
    delete res
    return rc
}
