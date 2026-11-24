options gen2
options indenting = 4

module _served_turn shared public

//! One streamed chat request under the client clock, with no engine module behind it: the served instruments
//! (`served_bench.das`, `model_bench.das`) time a server through the same send and the same marks.

require openai/openai_chat   // chat_stream: the streamed turn, its usage chunk and the server's own clock
require daslib/jobque_boost   // Atomic32: the first reply text signalled to another thread

let CALIBRATION_CHARS = 8000   //! the untimed probe turn's context, the ruler for characters per token
let CV_ROW_PCT = 3.0           //! a one-stream row's cv past this percent is not a clean measurement
let REPLY_HEAD_CHARS = 96      //! the reply's opening a turn keeps, for a look at what was generated
let ANSWER_HEAD_CHARS = 160    //! the opening of a reply's answer a turn keeps: a count to twenty whole

//! the first `limit` bytes of `text`, less the bytes of a character the cut would split
def text_head(text : string; limit : int) : string {
    return text if (length(text) <= limit)
    var cut = limit
    peek_data(text) $(d) {
        while (cut > 0 && (int(d[cut]) & 0xC0) == 0x80) {
            cut--
        }
    }
    return slice(text, 0, cut)
}

//! the client clock's marks over one streamed reply, in microseconds from the send
struct ReplyMarks {
    first_us : int64 = -1l      //! the first chunk carrying reply text; -1 when none came
    last_us : int64 = -1l       //! the last one
    median_gap_us : int64       //! between chunks carrying reply text, 0 under three of them
    longest_gap_us : int64
    head : string
}

//! `req` streamed at `url` (no key), cut off after `timeout_s`: the server's answer, and the clock's reading of every
//! chunk that carried reply text, reasoning included; `started`, when given, reads 1 from the first one on
def stream_marked(url : string; var req : ChatCompletionRequest; timeout_s : int; started : Atomic32? = null) : tuple<res : ChatStreamResult; marks : ReplyMarks> {
    var client = openai_client("{url}/v1")
    client.timeout_seconds = timeout_s
    var at : array<int64>
    var marks = ReplyMarks()
    let t0 = ref_time_ticks()
    var res <- chat_stream(client, req) $(piece : string) {
        if (empty(at) && started != null) {
            started |> set(1)
        }
        at |> push(get_time_nsec(t0) / 1000l)
        if (length(marks.head) < REPLY_HEAD_CHARS) {
            marks.head += piece
        }
    }
    let n = length(at)
    if (n > 0) {
        marks.first_us = at[0]
        marks.last_us = at[n - 1]
    }
    if (n > 2) {
        var gaps <- [for (i in range(1, n)); at[i] - at[i - 1]]
        sort(gaps)
        marks.median_gap_us = gaps[length(gaps) / 2]
        marks.longest_gap_us = gaps[length(gaps) - 1]
        delete gaps
    }
    delete at
    return <- (res <- res, marks = marks)
}

//! a reply's decode rate over `span_us`, its first token outside the span; 0 over no span
def reply_rate(completion_tokens : int64; span_us : float) : float
    => span_us > 0.0 ? float(completion_tokens - 1l) * 1.0e6 / span_us : 0.0
