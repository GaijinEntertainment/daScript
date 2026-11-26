options gen2
options indenting = 4

// model_bench - a model's own prefill and decode rates, read through any OpenAI-compatible chat server on ONE client
// clock: dasllama-server, or a third-party server beside it on the same box.
//
//   bin/daslang modules/dasLLAMA/harness/model_bench.das -- --url http://127.0.0.1:8080 [--model <config entry>]
//       [--config <json>] [--quick] [--max-length N] [--cell-seconds S] [--reps N] [--model-id <id>] [--corpus <file>]
//       [--streams N] [--wide] [--arm <name>] [--out <tests.json>] [--raw <requests.jsonl>] [--label <label>]
//       [--ultralong <ctx>] [--behind <ctx>] [--slot-tag <tag>] [--only <kinds>] [--lengths <n,n,...>]
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
// Every cell of the ladder and the width rows runs behind one untimed turn it drops: a context cell's sizing turn, and an 8-token turn of the task alone
// for the shortest cell and for every width cell; a warm-up turn that fails refuses its cell.
// Flags, printed on the row and written with it: `noisy` (cv past 3%), `cliff` (a rate under `cliff_share` of the rung
// before it: a prefill rate at twice the length or more, a decode rate at any later rung), `stall` (one gap between
// reply chunks past `stall_times` the median gap).
// The width rows follow the ladder, at the config's `width_lengths`: a wave of 2, 4 (and under `--wide` 8, 16, 32)
// streams a rep, each its own prompt and its own task, sent at once. A width row is the streams summed - their prompt tokens over the
// time to the wave's last first text, their decode rates added - named `pp4096@4`, `tg128+4096@4`, `tg128@4` - with the wave's
// end beside it: the page reads a reply row as the wave's reply tokens over that time, since a stream's own rate runs from its
// first text and so charges a server for giving it sooner. A wave
// wider than `--streams` is not sent: it would time the server's queue. `--wide` is a pass of its own against a server
// started with that many streams (the runner's `wide_server`): the wide widths alone, over base cells it does not record. Their flags: `noisy` (cv past 6%), `scaling`
// (a summed rate under nine tenths of the width before it), `unfair` (the last first text past `unfair_times` x the width x the first).
// `--arm <name>` is a run against a server started to speculate (the runner's job): the decode rows alone, one
// stream, at the config's `width_lengths`, in replies of `arm_reply_tokens` (a drafter is read past a reply's opening),
// named `tg512:<name>`, `tg512+4096:<name>`, each with the drafted and the accepted tokens its turns report
// (`timings.draft_n`, `draft_n_accepted`). `--arm off` takes the same rows at a plain server, unsuffixed: what an
// arm's rows are read against.
// `--behind <ctx>` is a run of its own against a server started with one stream and a slot directory: at each base length
// the base turn is restored off its slot file or prefilled cold and saved, then timed turns replay it and add 4096 tokens
// (`pp4096+131072`, `tg128+4096+131072`; `restore131072`, `save131072` and the cold `cold131072` beside them). The pass
// drops no turn: a timed turn runs behind the base the server already holds. `--behind <ctx> --arm <name>` is that pass
// at a server started to speculate, on the plain pass's slot files (the same `--slot-tag`): the decode row alone, in the
// arm's long reply (`tg512+4096+65536:<name>`), a restore in place of the prefill; `--arm off` gives the plain row beside it.
// Exit 1 when a cell was refused, or when a run that times rows reported none (`--only eyeball` times none).

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
require _prompt_files   // id_list: the lengths of --lengths
require dashv/dashv_boost   // POST: the behind pass's slot routes
require daslib/json_boost   // read_json: a slot route's answer

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

    @clarg_doc = "The ultralong pass: the context in tokens of a server started with one stream. Its rows are the prompts of 65536, 131072 and that context less 512 tokens, each that fits it, the corpus repeated to their length; a cell may take an hour"
    ultralong : int

    @clarg_doc = "The behind pass: the context in tokens of a server started with one stream and a slot directory (--slot-save-path). At each base of 32768, 65536 and 131072 tokens that fits it, and the deepest one that does: the base turn is restored off its slot file where the server holds one, else prefilled cold (row pp<base>) and saved; then timed turns replay it and add 4096 tokens - rows pp4096+<base> and tg<reply>+4096+<base>. --lengths names other bases"
    behind : int
    @clarg_name = "slot-tag"
    @clarg_doc = "The behind pass's slot file names open with this (default: slot): one tag a model and server, so a file is restored only into what saved it"
    slot_tag : string
    @clarg_doc = "The speculative arm the server was started for: its long-reply decode rows alone, at the config's width lengths; `off` takes them at a plain server"
    arm : string

    @clarg_doc = "What a run holds past its eyeball, comma-separated: pp (the prefill rows; alone, a turn's reply is cut to 8 tokens), tg (the one-stream decode rows), width (the multi-stream rows; alone, over one-stream turns at the width lengths), eyeball (alone: the eyeball and no timed cell). Default: every kind. An arm's run takes eyeball alone, the wide pass no kinds"
    only : string

    @clarg_doc = "The prompt lengths to run, comma-separated tokens (0 = the task alone), in place of the config's ladder"
    lengths : string

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
    answer : string             //! the opening of the reply's answer, its reasoning left out: what the eyeball reads
}

let CV_NOISY_WIDE_PCT = 6.0        //! a width row's: several streams at once spread more
let WARM_TOKENS = 8             //! a warm-up turn's reply
let SCALING_SHARE = 0.9        //! a summed rate under this share of the width before it lost ground: a server that prefills one prompt at a time holds level
let CACHED_MAX = 64l           //! prompt tokens a turn may read off a cache: a chat template's fixed head, never the context
let STALL_FLOOR_US = 200000l   //! a gap shorter than this is no stall, whatever the median
let FIRST_TIMEOUT_S = 600      //! a turn ahead of any measured rate
let TASK = "Write a complete, well-commented Python module that implements a token-bucket rate limiter with per-key buckets, a sliding-window variant, a thread-safe wrapper, and a pytest test suite. Write at least 600 lines. Do not stop early. Reply with the code only: start with the first line of the module, no introduction."

let WAVE_SUBJECTS = fixed_array(
    "a token-bucket rate limiter with per-key buckets, a sliding-window variant and a thread-safe wrapper",
    "an LRU cache with time-to-live entries, a size-bounded variant and hit and miss counters",
    "a trie-backed autocomplete index with insertion, deletion, prefix search and ranked results",
    "an interval tree with insertion, deletion, overlap queries and a merge of touching intervals",
    "a retrying job queue with priorities, exponential backoff and a dead-letter list",
    "a consistent-hash ring with virtual nodes, node weights and a rebalancing report",
    "a text diff tool with a longest-common-subsequence core, unified output and a patch applier",
    "a matrix library with multiplication, LU decomposition, a linear solver and a determinant")
//! the language of every eighth stream on: with the eight subjects, a wide wave's 32 streams are 32 tasks
let WAVE_LANGUAGES = fixed_array("Python", "Go", "Rust", "TypeScript")

//! stream `i`'s task of a wave: stream 0's subject and language are `TASK`'s
[arch(at = "../ARCHITECTURE_MEASUREMENT_SERVED.md#model-bench")]
def wave_task(i : int) : string {
    let subject = WAVE_SUBJECTS[i % length(WAVE_SUBJECTS)]
    let language = WAVE_LANGUAGES[(i / length(WAVE_SUBJECTS)) % length(WAVE_LANGUAGES)]
    return "Write a complete, well-commented {language} module that implements {subject}, and a test suite for it. Write at least 600 lines. Do not stop early. Reply with the code only: start with the first line of the module, no introduction."
}

//! a prompt no server has seen: `chars` characters of `base` under the nonce, then `task`; the task alone at 0
def prompt_of(base, nonce : string; chars : int; task : string = TASK) : string {
    // a bare "Request <id>." reads to a small model as the request, and it answers with the id
    return "Reference id {nonce} (not part of the task).\n\nThe task: {task}" if (chars <= 0)
    return build_string() $(var w) {
        w |> write("Reference id {nonce} (not part of the task). The files of a code base follow; the task comes after them.\n\n")
        w |> write(slice(base, 0, min(chars, length(base))))
        w |> write("\n\nThe task: ")
        w |> write(task)
    }
}

//! one streamed chat turn under the client clock, cut off after `timeout_s`
def bench_turn(url, model_id, prompt : string; budget, timeout_s : int) : BenchTurn {
    var one <- [ChatMessage(role = "user", content = prompt)]
    return chat_turn(url, model_id, one, budget, timeout_s, false)
}

//! why `turn` was not served at all: its error, or a stream with no reply text; "" for a turn that was
def unserved_why(turn : BenchTurn) : string
    => !empty(turn.error) ? turn.error : (turn.ttft_us < 0l ? "the stream carried no reply text" : "")

//! the reply's kept opening `head` is one character alone, `ONE_CHAR_RUN` of it or more
def one_char_run(head : string) : bool => length(head) >= ONE_CHAR_RUN && head == repeat(slice(head, 0, 1), length(head))

//! why a timed turn cannot enter its cell, "" when it can. `behind`: the behind pass's timed turn, whose base is to come off
//! the cache - its cache refusal is a base that did not, where any other turn's is a prompt that read off one
def refusal_of(turn : BenchTurn; budget : int; behind : bool = false) : string {
    let unserved = unserved_why(turn)
    return unserved if (!empty(unserved))
    return "the stream carried no usage chunk (stream_options.include_usage)" if (turn.prompt_tokens == 0l)
    let fresh = turn.prompt_tokens - max(turn.cached_tokens, 0l)
    return "the base did not come off the cache: {fresh} of {turn.prompt_tokens} prompt tokens prefilled" if (behind && fresh > int64(BEHIND_FRESH_MAX * BEHIND_EXTRA))
    return "{turn.cached_tokens} of {turn.prompt_tokens} prompt tokens came off a cache" if (!behind && turn.cached_tokens > CACHED_MAX)
    return "the reply ended at {turn.completion_tokens} of {budget} tokens (finish_reason {turn.finish})" if (turn.completion_tokens < int64(budget))
    return "a reply of {turn.completion_tokens} token(s) has no decode span" if (turn.completion_tokens < 2l || turn.gen_us <= 0l)
    return "the reply is one character repeated ('{slice(turn.reply_head, 0, 1)}'): the model is not answering" if (one_char_run(turn.reply_head))
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
    stream : int    //! the prompt's place in the wave: its turn's `rep`
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
    done_ms : float             //! the wave's end: from its send to its last stream's last text
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

//! `prompts` sent at once, a turn each, its `rep` the prompt's place: every thread is up before the first request goes
def wave_turns(args : BenchArgs; prompts : array<string>; budget, timeout_s : int) : array<BenchTurn> {
    let width = length(prompts)
    var turns : array<BenchTurn>
    with_stream() $(var jobs : Stream?) {
        for (i in range(width)) {
            var job = WaveJob(url = args.url, model_id = args.model_id, prompt = prompts[i], stream = i,
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
                                var turn = bench_turn(job.url, job.model_id, job.prompt, job.budget, job.timeout_s)
                                turn.rep = job.stream
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
                        landed.answer = clone_string(turn.answer)
                        turns |> emplace(landed)
                    }
                }
            }
        }
    }
    return <- turns
}

def run_wave(args : BenchArgs; name, base, run : string; rep, chars, width, budget, timeout_s : int) : array<BenchTurn> {
    var prompts <- [for (i in range(width)); prompt_of(base, "{run}-{name}-{rep}-{i}", chars, wave_task(i))]
    var turns <- wave_turns(args, prompts, budget, timeout_s)
    delete prompts
    return <- turns
}

//! `reps` waves of `width` streams at one prompt length, behind one untimed short turn
def run_wave_cell(args : BenchArgs; name, base, run : string; chars, width, reps, budget, timeout_s : int; unfair_times : float) : WaveCell {
    var cell = WaveCell()
    var prefill, decode, per_stream, last_ttft, done : array<float>
    per_stream |> reserve(width * reps)
    cell.why = warm_up(args, base, "{run}-{name}-warm", timeout_s)
    for (rep in range(reps)) {
        break if (!empty(cell.why))
        var turns <- run_wave(args, name, base, run, rep, chars, width, budget, timeout_s)
        var tokens = 0l
        var first_us = 0l
        var last_us = 0l
        var done_us = 0l
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
            done_us = max(done_us, turn.ttft_us + turn.gen_us)
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
        done |> push(float(done_us) / 1000.0)
        cell.unfair ||= float(last_us) > unfair_times * float(width) * float(first_us)
    }
    if (empty(cell.why)) {
        cell.prefill = spread_of(prefill)
        cell.decode = spread_of(decode)
        cell.stream_decode = spread_of(per_stream).median
        cell.last_ttft_ms = spread_of(last_ttft).median
        cell.done_ms = spread_of(done).median
    }
    delete prefill
    delete decode
    delete per_stream
    delete last_ttft
    delete done
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

//! what a decode row's name carries after its lengths: the arm, nothing for a plain run and for `off`
def private arm_suffix(arm : string) : string => empty(arm) || arm == "off" ? "" : ":{arm}"

//! a decode row's printed draft counts; an arm whose turns reported none says so
def private drafts_note(arm : string; draft_n, accepted : int64) : string {
    return ", {accepted} of {draft_n} drafted tokens accepted" if (draft_n > 0l)
    return empty(arm_suffix(arm)) ? "" : ", NO draft reported"
}

//! One rung's cell as its rows, printed and - but in the wide pass, whose one-stream cells are its base alone - recorded:
//! the prefill row of a context rung of the plain pass, and the decode row with the turn's draft counts.
def private rung_rows(args : BenchArgs; s : Settings; kinds : RowKinds; row_label, pp_name, tg_name : string; len : int; cell : Cell; last_pp, last_tg : float; doubled, first : bool; var into : RunOut) {
    if (len > 0 && empty(args.arm) && !args.wide && kinds.pp) {   // the task alone has no prefill rate worth a row, and an arm's prefill is the plain one's
        let pp_flags = flags_of(cell.prefill, last_pp, s.cliff_share, doubled, false)
        into.tests |> insert(pp_name, row_of(cell.prefill, cell.prompt_tokens, s.reps, cell.server_prefill, pp_flags))
        print("{row_label}  {pp_name}: {cell.prefill.median:.1f} tok/s (cv {cell.prefill.cv:.1f}%), {cell.prompt_tokens} prompt tokens, first text after {cell.ttft_ms / 1000.0:.2f} s{server_note(cell.server_prefill)}{flag_note(pp_flags)}\n")
    }
    return if (!kinds.tg)
    let tg_flags = flags_of(cell.decode, last_tg, s.cliff_share, !first, cell.stalled)
    var tg_row = row_of(cell.decode, cell.prompt_tokens, s.reps, cell.server_decode, tg_flags)
    tg_row.ttft_ms = double(cell.ttft_ms)
    tg_row.draft_n = cell.draft_n
    tg_row.draft_n_accepted = cell.draft_n_accepted
    if (!args.wide) {
        into.tests |> insert(tg_name, tg_row)
    }
    print("{row_label}  {tg_name}: {cell.decode.median:.1f} tok/s (cv {cell.decode.cv:.1f}%) after {cell.prompt_tokens} prompt tokens{drafts_note(args.arm, cell.draft_n, cell.draft_n_accepted)}{server_note(cell.server_decode)}{flag_note(tg_flags)}\n")
}

[arch(at = "../ARCHITECTURE_MEASUREMENT_SERVED.md#model-bench")]
def private length_ladder(args : BenchArgs; s : Settings; kinds : RowKinds; row_label, base, run : string; chars_per_token : float; base_tokens : int64; probe_rate : float; var into : RunOut; var bases : table<int; LengthBase>) {
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
        let arm = arm_suffix(args.arm)
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
        rung_rows(args, s, kinds, row_label, pp_name, tg_name, len, cell, last_pp, last_tg, last_len > 0 && len >= 2 * last_len, first, into)
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
        tg_row.done_ms = double(cell.done_ms)
        into.tests |> insert(tg_name, tg_row)
        print("{row_label}  {tg_name}: {cell.decode.median:.1f} tok/s combined over {width} streams (cv {cell.decode.cv:.1f}%), the wave done after {cell.done_ms / 1000.0:.2f} s{flag_note(tg_flags)}\n")
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

let EYEBALL_REPLY = 48      //! an eyeball turn's reply, tokens: room for the count to run on to twenty; an entry's `eyeball_reply_tokens` over it
let EYEBALL_COUNT_TO = 10   //! the count's reply runs on at least this far

//! one eyeball ask: a prompt any working model answers alike on every backend - no step of its reply a near-tie, which a
//! device's rounding would turn - and what its reply holds
struct EyeballAsk {
    name : string
    prompt : string
    words : string    //! the reply holds one of these, comma-separated, in any case; "" on the count, which has its own check
}

let EYEBALL_ASKS <- [
    EyeballAsk(name = "sky", prompt = "Why is the sky blue? Answer in one short sentence.", words = "scatter,wavelength,rayleigh"),
    EyeballAsk(name = "count", prompt = "Count from 1 to 20. Reply with the numbers only, separated by commas.", words = ""),
    EyeballAsk(name = "capital", prompt = "What is the capital of France? Answer with the city's name only.", words = "paris"),
    EyeballAsk(name = "sum", prompt = "What is 2 + 2? Answer with the number only.", words = "4,four")]

//! the numbers of `reply` run on in order - from six, or from one where the reply opens on the sequence it was given - to
//! `EYEBALL_COUNT_TO` at least; its last number is not read where no character follows it, since the reply's cut may have split it
def count_runs_on(reply : string) : bool {
    var want = 0
    var n = -1
    var reached = 0
    for (ch in reply) {
        if (is_number(ch)) {
            n = max(n, 0) * 10 + (ch - '0')
        } elif (n >= 0) {
            want = want != 0 ? want : ((n == 1 || n == 6) ? n : -1)
            return false if (n != want)
            reached = n
            want++
            n = -1
        }
    }
    return reached >= EYEBALL_COUNT_TO
}

//! why `turn`'s reply to `ask` reads wrong, "" when it reads right
def eyeball_why(ask : EyeballAsk; turn : BenchTurn) : string {
    let unserved = unserved_why(turn)
    return unserved if (!empty(unserved))
    return "the reply's {turn.completion_tokens} tokens were reasoning, with no answer after it" if (empty(turn.answer))
    if (empty(ask.words)) {
        return count_runs_on(turn.answer) ? "" : "the numbers do not run on from 6 to {EYEBALL_COUNT_TO}"
    }
    let low = to_lower(turn.answer)
    var words <- split(ask.words, ",")
    var held = false
    for (w in words) {
        held ||= find(low, w) >= 0
    }
    delete words
    return held ? "" : "the reply holds none of: {ask.words}"
}

//! one eyeball reply recorded and printed, a line a reply, `note` after its verdict; 1 when it reads wrong, which is a refusal
//! of the run too
def private eyeball_reply(var into : RunOut; mode : string; ask : EyeballAsk; turn : BenchTurn; note : string = "") : int {
    let why = eyeball_why(ask, turn)
    let text = replace(empty(turn.answer) ? turn.reply_head : turn.answer, "\n", " ")
    into.eyeball |> push(EyeballReply(mode = mode, ask = ask.name, why = why, note = note, reply = text))
    if (empty(why)) {
        let noted = empty(note) ? "" : " ({note})"
        print("EYEBALL {mode} {ask.name}: ok{noted} | {text}\n")
        return 0
    }
    print("EYEBALL {mode} {ask.name}: FAILED - {why} | {text}\n")
    into.refused |> push(Refusal(cell = "eyeball {mode} {ask.name}", why = why))
    return 1
}

[arch(at = "../ARCHITECTURE_MEASUREMENT_SERVED.md#model-bench")]
def run_eyeball(args : BenchArgs; reply : int; var into : RunOut) : int {
    let arm = !empty(args.arm)
    let one = arm ? ":{args.arm}" : "plain"
    var bad = 0
    var alone : string[2]
    for (i in range(2)) {
        let turn = bench_turn(args.url, args.model_id, EYEBALL_ASKS[i].prompt, reply, FIRST_TIMEOUT_S)
        alone[i] = turn.reply_head
        bad += eyeball_reply(into, one, EYEBALL_ASKS[i], turn)
    }
    if (!arm) {
        for (width in [2, 4]) {
            break if (args.streams > 0 && width > args.streams)
            var prompts <- [for (i in range(width)); EYEBALL_ASKS[i].prompt]
            var turns <- wave_turns(args, prompts, reply, FIRST_TIMEOUT_S)
            for (turn in turns) {
                let apart = turn.rep < 2 && turn.reply_head != alone[turn.rep]
                bad += eyeball_reply(into, "@{width}", EYEBALL_ASKS[turn.rep], turn, apart ? "not the one-stream reply" : "")
            }
            delete turns
            delete prompts
        }
    }
    let total = length(into.eyeball)
    if (bad == 0) {
        print("EYEBALL: every one of {total} replies reads right\n")
    } else {
        print("EYEBALL FAILED: {bad} of {total} replies read wrong - the run stops here\n")
    }
    return bad
}

let BEHIND_EXTRA = 4096            //! the tokens a behind row's timed turn adds after its base
let BEHIND_ROOM = 384              //! the tokens of a timed turn that are no base, no added message and no reply: the template and the replayed reply
let ONE_CHAR_RUN = 16      //! a reply's kept opening this long or longer, all one character, is a broken model's, not an answer
let BEHIND_BASE_REPLY = 16         //! the base turn's reply: past one token, so a discrete home's rows come down whole for the save
let BEHIND_REPLY = "Understood. Send the next files and the task."   //! the base turn's reply as every later turn replays it: a fixed text, the same on any server life
let BEHIND_FRESH_MAX = 2           //! a timed turn may prefill this many times `BEHIND_EXTRA`: past it the base did not come off the cache

//! the tokens a behind row's timed turn adds after a base of `len`: `BEHIND_EXTRA`, less where a long reply behind the
//! deepest base leaves less; none (0 or under) where the base leaves the reply no room at all
def behind_extra(behind, len, reply_tokens : int) : int => min(BEHIND_EXTRA, behind - len - reply_tokens - BEHIND_ROOM)

//! the lengths of `rungs` under `top`, then `top` itself, ascending: a pass's ladder up to the deepest length its context holds
def private lengths_under(top : int; rungs : array<int>) : array<int> {
    var out <- [for (len in rungs); len; where len > 0 && len < top]
    if (top > 0) {
        out |> push(top)
    }
    return <- out
}

//! the deepest base leaves room after it for the timed turn and its reply
def behind_lengths(ctx : int) : array<int>
    => <- lengths_under(ctx - BEHIND_EXTRA - ULTRALONG_TAIL, [32768, 65536, 131072])

[arch(at = "../ARCHITECTURE_MEASUREMENT_SERVED.md#model-bench")]
def behind_base(base : string; chars : int) : string
    => "The files of a code base follow; more files and the task come in a later message.\n\n{slice(base, 0, min(chars, length(base)))}"

//! `chars` characters of `base` from `at` under the nonce, then the task
def behind_timed_message(base, nonce : string; at, chars : int) : string {
    let room = max(length(base) - chars, 0)
    let lo = min(at, room)
    return "Reference id {nonce} (not part of the task). More files follow; the task comes after them.\n\n{slice(base, lo, min(lo + chars, length(base)))}\n\nThe task: {TASK}"
}

//! one streamed chat turn of `messages` under the client clock, cut off after `timeout_s`; `cache_prompt` lets it read and feed the prompt cache
def chat_turn(url, model_id : string; var messages : array<ChatMessage>; budget, timeout_s : int; cache_prompt : bool) : BenchTurn {
    var turn = BenchTurn(cached_tokens = -1l, ttft_us = -1l)
    var req <- ChatCompletionRequest(model = model_id, max_tokens = budget, messages <- messages,
                                     stream_options = StreamOptions(include_usage = true),
                                     chat_template_kwargs = "\{\"enable_thinking\":false}", cache_prompt = cache_prompt ? "true" : "false")
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
    turn.answer = text_head(got.res.content, ANSWER_HEAD_CHARS)
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

//! what a slot route answered: the tokens it saved or restored and its own milliseconds, or why it did neither
struct SlotDone {
    why : string
    tokens : int64
    ms : float
}

//! `POST /slots/0?action=<action>` with `filename` at `url` - llama.cpp's slot routes, ours alike
def slot_action(url, action, filename : string) : SlotDone {
    var done = SlotDone(why = "no answer")
    POST("{url}/slots/0?action={action}", "\{\"filename\":\"{filename}\"}") $(resp) {
        let text = string(resp.body)
        var err : string
        var js = read_json(text, err)
        if (int(resp.status_code) == 200 && js != null) {
            done.why = ""
            done.tokens = max(js?["n_saved"] ?? 0l, js?["n_restored"] ?? 0l)
            done.ms = float(max(js?["timings"]?["save_ms"] ?? 0.0lf, js?["timings"]?["restore_ms"] ?? 0.0lf))
        } else {
            done.why = "HTTP {int(resp.status_code)}: {text_head(text, 200)}"
        }
        unsafe {
            delete js
        }
    }
    return done
}

[arch(at = "../ARCHITECTURE_MEASUREMENT_SERVED.md#model-bench")]
def private slot_row(done : SlotDone) : TestRow
    => TestRow(tok_s = done.ms > 0.0 ? double(done.tokens) * 1000.0lf / double(done.ms) : 0.0lf, prompt_tokens = done.tokens, reps = 1)

//! A rung's base on the server: restored off `file`, else prefilled cold and saved. `plain` records the base's rows. False
//! where it neither restored nor ran
[arch(at = "../ARCHITECTURE_MEASUREMENT_SERVED.md#model-bench")]
def private behind_base_turn(args : BenchArgs; row_label, base_text, file : string; len : int; cold_rate : float; plain : bool; var into : RunOut) : bool {
    let restored = slot_action(args.url, "restore", file)
    if (empty(restored.why)) {
        if (plain) {
            into.tests |> insert("restore{len}", slot_row(restored))
        }
        print("{row_label}  restore{len}: {restored.tokens} tokens off {file} in {restored.ms / 1000.0:.2f} s\n")
    } else {
        print("{row_label}  restore{len}: not restored ({restored.why}) - the base prefills cold\n")
        let timeout_s = max(turn_timeout_s(float(len) / cold_rate), ULTRALONG_CELL_SECONDS)   // a deep prefill runs far under the short prompt's rate
        var first <- [ChatMessage(role = "user", content = base_text)]
        var turn = chat_turn(args.url, args.model_id, first, BEHIND_BASE_REPLY, timeout_s, true)
        turn.cell = "cold{len}"
        append_line(args.raw, sprint_json(turn, false))
        let why = unserved_why(turn)
        if (!empty(why) || turn.prompt_tokens <= 0l) {
            into.refused |> push(Refusal(cell = "cold{len}", why = empty(why) ? "the stream carried no usage chunk" : why))
            print("{row_label}  cold{len}: REFUSED - {why}\n")
            return false
        }
        if (plain) {
            into.tests |> insert("cold{len}", TestRow(tok_s = double(prefill_rate(turn)), prompt_tokens = turn.prompt_tokens, reps = 1,
                cached_tokens = max(turn.cached_tokens, 0l)))
        }
        print("{row_label}  cold{len}: {prefill_rate(turn):.1f} tok/s over {turn.prompt_tokens} prompt tokens, cold, one turn\n")
        let saved = slot_action(args.url, "save", file)
        if (empty(saved.why)) {
            if (plain) {
                into.tests |> insert("save{len}", slot_row(saved))
            }
            print("{row_label}  save{len}: {saved.tokens} tokens into {file} in {saved.ms / 1000.0:.2f} s\n")
        } else {
            print("{row_label}  save{len}: not saved ({saved.why}) - the next run prefills this base cold again\n")
        }
    }
    return true
}

//! false where the base neither restored nor ran
[arch(at = "../ARCHITECTURE_MEASUREMENT_SERVED.md#model-bench")]
def private behind_rung(args : BenchArgs; s : Settings; row_label, base, run : string; len : int; chars_per_token, cold_rate : float; var into : RunOut) : bool {
    let chars = int(float(len) * chars_per_token)
    let file = "{empty(args.slot_tag) ? "slot" : args.slot_tag}-{len}-{chars}.ctx"
    let base_text = behind_base(base, chars)
    let plain = empty(args.arm)   // an arm's run, and the long plain run beside it, record their decode row alone: the base's rows are the plain run's
    return false if (!behind_base_turn(args, row_label, base_text, file, len, cold_rate, plain, into))
    var prefill, decode, ttft : array<float>
    var why = ""
    var prompt_tokens = 0l
    var cached = 0l
    var draft_n = 0l
    var draft_n_accepted = 0l
    let extra = behind_extra(args.behind, len, s.reply_tokens)
    let extra_chars = int(float(extra) * chars_per_token)
    let pp_name = "pp{extra}+{len}"
    let tg_name = "tg{s.reply_tokens}+{extra}+{len}{arm_suffix(args.arm)}"
    for (rep in range(s.reps)) {
        var turns <- [ChatMessage(role = "user", content = base_text), ChatMessage(role = "assistant", content = BEHIND_REPLY),
            ChatMessage(role = "user", content = behind_timed_message(base, "{run}-{len}-{rep}", chars + rep * extra_chars, extra_chars))]
        var turn = chat_turn(args.url, args.model_id, turns, s.reply_tokens,
            turn_timeout_s(float(BEHIND_FRESH_MAX * BEHIND_EXTRA) / cold_rate + float(s.reply_tokens) / 4.0), true)
        turn.cell = pp_name
        turn.rep = rep
        append_line(args.raw, sprint_json(turn, false))
        let refusal = refusal_of(turn, s.reply_tokens, true)
        if (!empty(refusal)) {
            why = "at rep {rep}: {refusal}"
            break
        }
        prefill |> push(prefill_rate(turn))
        decode |> push(reply_rate(turn.completion_tokens, float(turn.gen_us)))
        ttft |> push(float(turn.ttft_us) / 1000.0)
        prompt_tokens = turn.prompt_tokens
        cached = max(turn.cached_tokens, 0l)
        draft_n += turn.draft_n
        draft_n_accepted += turn.draft_n_accepted
    }
    if (!empty(why)) {
        into.refused |> push(Refusal(cell = pp_name, why = why))
        print("{row_label}  {pp_name}: REFUSED - {why}\n")
    } else {
        let pp = spread_of(prefill)
        let tg = spread_of(decode)
        var pp_row = row_of(pp, prompt_tokens, s.reps, 0.0, flags_of(pp, 0.0, s.cliff_share, false, false))
        pp_row.cached_tokens = cached
        var tg_row = row_of(tg, prompt_tokens, s.reps, 0.0, flags_of(tg, 0.0, s.cliff_share, false, false))
        tg_row.ttft_ms = double(spread_of(ttft).median)
        tg_row.draft_n = draft_n
        tg_row.draft_n_accepted = draft_n_accepted
        if (plain) {
            into.tests |> insert(pp_name, pp_row)
        }
        into.tests |> insert(tg_name, tg_row)
        print("{row_label}  {pp_name}: {pp.median:.1f} tok/s (cv {pp.cv:.1f}%) over the {prompt_tokens - cached} tokens past {cached} cached, first text after {tg_row.ttft_ms / 1000.0lf:.2f} s\n")
        print("{row_label}  {tg_name}: {tg.median:.1f} tok/s (cv {tg.cv:.1f}%) after {prompt_tokens} prompt tokens{drafts_note(args.arm, draft_n, draft_n_accepted)}\n")
    }
    delete prefill
    delete decode
    delete ttft
    return true
}

let ULTRALONG_CELL_SECONDS = 3600   //! what a cell of the ultralong pass may take
let ULTRALONG_TAIL = 512            //! the room the pass's longest prompt leaves in the context: the reply and the template

[arch(at = "../ARCHITECTURE_MEASUREMENT_SERVED.md#model-bench")]
def ultralong_lengths(ctx : int) : array<int>
    => <- lengths_under(ctx - ULTRALONG_TAIL, [65536, 131072])

[arch(at = "../ARCHITECTURE_MEASUREMENT_SERVED.md#model-bench")]
def repeated_corpus(base : string; chars : int) : string {
    return base if (empty(base) || length(base) >= chars)
    return build_string() $(var w) {
        w |> write(base)
        var held = length(base)
        var copy = 2
        while (held < chars) {
            w |> write("\n\n# ---- the same files again, copy {copy} ----\n\n")
            w |> write(base)
            held += length(base)
            copy++
        }
    }
}

let PP_ONLY_REPLY = 8   //! a turn's longest reply where the prefill rows alone are asked for

def pp_only_reply(reply_tokens : int) : int => min(reply_tokens, PP_ONLY_REPLY)

//! the row kinds a plain run records
struct RowKinds {
    pp : bool
    tg : bool
    width : bool
}

//! the timed kinds `only` names - every kind where it names none, none where it names the eyeball alone - beside its first
//! word that is no kind
def row_kinds_of(only : string) : tuple<k : RowKinds; unknown : string> {
    return (k = RowKinds(pp = true, tg = true, width = true), unknown = "") if (empty(only))
    var k : RowKinds
    var unknown = ""
    var words <- split(only, ",")
    for (w in words) {
        if (w == "pp") {
            k.pp = true
        } elif (w == "tg") {
            k.tg = true
        } elif (w == "width") {
            k.width = true
        } elif (!is_bench_kind(w) && empty(unknown)) {
            unknown = w
        }
    }
    delete words
    return (k = k, unknown = unknown)
}

//! The settings a run uses - its config entry's, the command line's reps and cell time over them, and the width lengths
//! alone as its ladder under an arm, the wide pass or the width rows asked for alone - beside why they cannot run, "" when
//! they can. --lengths names the ladder over all of that; the prefill rows asked for alone cut the reply and the task-alone rung
def run_settings(args : BenchArgs; kinds : RowKinds) : tuple<s : Settings; unfit : string> {
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
    if (!empty(args.arm) || args.wide || (kinds.width && !kinds.pp && !kinds.tg)) {
        s.lengths := s.width_lengths
    }
    if (!empty(args.arm) && s.arm_reply_tokens > 0) {
        s.reply_tokens = s.arm_reply_tokens
    }
    if (args.ultralong > 0) {
        var long_lengths <- ultralong_lengths(args.ultralong)
        s.lengths := long_lengths
        delete long_lengths
        s.cell_seconds = args.cell_seconds > 0 ? args.cell_seconds : max(s.cell_seconds, ULTRALONG_CELL_SECONDS)
    }
    if (!empty(args.lengths)) {
        var named <- named_lengths(args.lengths, 0l)
        s.lengths := named
        delete named
    }
    if (kinds.pp && !kinds.tg && !kinds.width) {
        s.reply_tokens = pp_only_reply(s.reply_tokens)
        var past_task <- [for (n in s.lengths); n; where n > 0]
        s.lengths := past_task
        delete past_task
    }
    let fits = !empty(s.lengths) && s.reps >= 1 && s.reply_tokens >= 2 && s.cell_seconds >= 1
    let unfit = fits ? "" : "the settings of '{args.entry}' in {config_path} name no lengths, no reps, no cell time or a reply under 2 tokens"
    return <- (s <- s, unfit = unfit)
}

//! a run's end: its file written and its counts printed. 1 when a cell was refused, the file did not write, or no row was
//! reported where the run times rows (`rows_needed`)
def private finish_run(args : BenchArgs; res : RunOut; rows_needed : bool) : int {
    let rows = length(res.tests)
    let refused = length(res.refused)
    if (!empty(args.out) && !fwrite(args.out, sprint_json(res, true))) {
        print("error: could not write {args.out}\n")
        return 1
    }
    print("model_bench: {rows} row(s), {refused} cell(s) refused\n")
    return refused > 0 || (rows == 0 && rows_needed) ? 1 : 0
}

//! `--lengths`' tokens, none under `lowest`
def private named_lengths(text : string; lowest : int64) : array<int> {
    var named <- id_list(text, lowest, 2147483647l)
    var out <- [for (n in named); int(n)]
    delete named
    return <- out
}

//! One bench run against `args.url` under its config entry: the ladder, then the width rows, every row and refusal into
//! `res` and the run file written. 0 when every cell reported; 1 when one was refused, a run that times rows reported
//! none, or the run could not start (the config, the corpus, the calibration turns)
[arch(at = "../ARCHITECTURE_MEASUREMENT_SERVED.md#model-bench")]
def run_bench(args : BenchArgs; var res : RunOut) : int {
    if ((args.ultralong > 0 || args.behind > 0) && (args.wide || !empty(args.only) || (args.ultralong > 0 && (args.behind > 0 || !empty(args.arm))))) {
        print("error: --ultralong and --behind are each a run of its own: neither takes --wide, --only or the other, and --behind alone takes --arm\n")
        return 1
    }
    let picked = row_kinds_of(args.only)
    if (!empty(picked.unknown)) {
        print("error: --only {args.only}: '{picked.unknown}' is no kind (pp, tg, width, eyeball)\n")
        return 1
    }
    let eyeball_alone = args.only == "eyeball"
    if (!empty(args.only) && (args.wide || (!empty(args.arm) && !eyeball_alone))) {
        print("error: --only {args.only}: an arm's run takes eyeball alone, the wide pass no kinds\n")
        return 1
    }
    let kinds = picked.k
    var settled <- run_settings(args, kinds)
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
    if (args.behind > 0) {
        return run_behind(args, s, row_label, base, run, chars_per_token, prefill_rate(probe), res)
    }
    let wrong = args.wide || args.arm == "off" ? 0 : run_eyeball(args, s.eyeball_reply_tokens > 0 ? s.eyeball_reply_tokens : EYEBALL_REPLY, res)
    if (wrong > 0) {
        res.bench.stopped = "at the eyeball: {wrong} replies read wrong"
    } elif (!eyeball_alone) {
        var bases : table<int; LengthBase>
        let context = args.ultralong > 0 ? repeated_corpus(base, int(float(args.ultralong) * chars_per_token * 1.05)) : base
        length_ladder(args, s, kinds, row_label, context, run, chars_per_token, short.prompt_tokens, prefill_rate(probe), res, bases)
        if (!empty(res.bench.stopped)) {
            print("{row_label}  the ladder stopped {res.bench.stopped}\n")
        }
        if (empty(args.arm) && kinds.width && args.ultralong == 0) {
            width_ladder(args, s, row_label, base, run, bases, res)
        }
        delete bases
    }
    return finish_run(args, res, !eyeball_alone)
}

def private run_behind(args : BenchArgs; s : Settings; row_label, base, run : string; chars_per_token, cold_rate : float; var res : RunOut) : int {
    var lengths <- behind_lengths(args.behind)
    if (!empty(args.lengths)) {
        delete lengths
        lengths <- named_lengths(args.lengths, 1l)
    }
    let top = empty(lengths) ? 0 : lengths[length(lengths) - 1]
    if (top > 0 && behind_extra(args.behind, top, s.reply_tokens) <= 0) {
        print("REFUSED: {row_label}: a base of {top} tokens leaves a timed turn no room under --behind {args.behind} (a reply of {s.reply_tokens}, {BEHIND_ROOM} kept)\n")
        delete lengths
        return 1
    }
    let context = repeated_corpus(base, int(float(top + (s.reps + 1) * BEHIND_EXTRA) * chars_per_token * 1.05))
    print("model_bench: {row_label} at {args.url}, the behind pass at {lengths} tokens, {s.reps} rep(s) a cell, {BEHIND_EXTRA} tokens added a turn, reply {s.reply_tokens} tokens\n")
    for (len in lengths) {
        break if (!behind_rung(args, s, row_label, context, run, len, chars_per_token, cold_rate, res))
    }
    delete lengths
    return finish_run(args, res, true)
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
