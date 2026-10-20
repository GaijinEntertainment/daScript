options gen2
options stack = 524288
options _dasllama_internal = true

// The parity dump: one arm's forced-feed run written to a file - the prompt and feed ids, the routing tape the arm took or
// replayed, and every row's logits - so two arms that cannot share a process (a model past the box with both arms up, a
// CPU reference that runs once and serves every device run after it) compare as files. `parity_dump.das` is the tool,
// `tests/test_parity_dump.das` the round trip.

require dasllama/dasllama_common
require dasllama/dasllama_moe
require dasllama/dasllama_gpu_resident
require dasllama/dasllama
require dasllama/dasllama_math
require daslib/fio
require daslib/linq
require math
require ../tests/_compares.das   // logits_maxdiff: the row compare, a NaN on either side infinite; logits_of: the session's row copied
require ../tests/_model_tier.das   // live_maxabs: the reference row's scale

let PARITY_DUMP_MAGIC = 1146110020l   //! "DLPD" as a little-endian word
let PARITY_DUMP_VERSION = 2l   //! the record is the binary serializer's bytes behind the magic and version words

//! one arm's forced-feed run: the prompt, the fed tokens, the routing tape (`picks_per_select` picks a select, in call order)
//! and the rows' logits - the prefill's row first, then a row a fed token
struct ParityDump {
    model : string
    arm : string             //! "device" where the whole-model driver served the run, else "cpu"
    vocab : int64
    picks_per_select : int64 //! the model's n_expert_used (0 = the model routes nothing)
    prompt : array<int64>
    feed : array<int64>
    tape : array<int64>
    tape_seen : int64        //! a replaying arm: the selects it replayed
    tape_miss : int64        //! ... and the own picks the tape did not carry
    rows : array<array<float>>
}

//! the dump to `path`, whole: the magic word, the version word, the record's byte count and the record's bytes
def parity_dump_write(path : string; d : ParityDump) {
    for (r in d.rows) {
        verify(long_length(r) == d.vocab, "parity dump: a row is not vocab wide")
    }
    var wrote = false
    fopen(path, "wb") $(f) {
        return if (f == null)   // the block runs with a null handle where the open failed
        fwrite(f, PARITY_DUMP_MAGIC)
        fwrite(f, PARITY_DUMP_VERSION)
        binary_save(d) $(data) {
            fwrite(f, long_length(data))
            fwrite(f, data)
        }
        wrote = true
    }
    if (!wrote) {
        panic("parity dump: cannot write '{path}'")
    }
}

//! the next header word of `f`
def private read_word(f : file) : int64 {
    var w : array<int64>   // a one-word array: fread writes through a const view, which a scalar local would fold away
    w |> resize(1)
    verify(fread(f, w) == 8, "parity dump: the file ends inside a header word")
    return w[0]
}

//! why the two header words of `f` refuse the file: "" where they are this build's magic and version
def private header_refusal(f : file; path : string) : string {
    return "parity dump: '{path}' is not a parity dump" if (read_word(f) != PARITY_DUMP_MAGIC)
    return "parity dump: '{path}' is another version's" if (read_word(f) != PARITY_DUMP_VERSION)
    return ""
}

//! why the file at `path` is no parity dump this build reads: "" where its magic and version words are this build's
def parity_dump_refusal(path : string) : string {
    var why = "parity dump: cannot read '{path}'"
    fopen(path, "rb") $(f) {
        return if (f == null)
        why = header_refusal(f, path)
    }
    return why
}

//! the dump at `path`, read whole
def parity_dump_read(path : string) : ParityDump {
    var d : ParityDump
    var read = false
    fopen(path, "rb") $(f) {
        return if (f == null)
        let why = header_refusal(f, path)
        if (!empty(why)) {
            panic(why)
        }
        let n = read_word(f)
        var bytes : array<uint8>
        bytes |> resize(n)
        verify(long_fread(f, bytes) == n, "parity dump: the file ends inside the record")
        binary_load(d, bytes)
        delete bytes
        read = true
    }
    if (!read) {
        panic("parity dump: cannot read '{path}'")
    }
    return <- d
}

//! the forced-feed run on `tr`, a row a step: the routing recorded where `ref_tape` is empty and replayed otherwise; the run's own greedy continuation where `feed` is empty and `steps` asks for one
def parity_dump_run(tr : Model; model : string; kv : KVDtype; prompt : array<int64>; feed : array<int64>; steps : int64; ref_tape : array<int64>) : ParityDump {
    var d : ParityDump
    let c = tr.config
    d.model = model
    d.arm = moe_gpu_resident_active() ? "device" : "cpu"
    d.vocab = c.vocab_size
    d.picks_per_select = c.n_expert_used
    d.prompt |> push_from(prompt)
    var s <- create_session(tr, kv)
    let np = long_length(prompt)
    let replay = !empty(ref_tape)
    if (replay) {
        moe_pick_tape_replay_from(ref_tape)
    } else {
        moe_pick_tape(MoePickTape.record)
    }
    forward_prefill(tr, s, prompt, np, 0l)
    let nsteps = empty(feed) ? steps : long_length(feed)
    d.rows |> reserve(nsteps + 1l)
    d.feed |> reserve(nsteps)
    d.rows |> emplace <| logits_of(s, d.vocab)
    for (step in range64(nsteps)) {
        let tok = empty(feed) ? parallel_argmax(s.logits, d.vocab) : feed[step]
        d.feed |> push(tok)
        forward(tr, s, tok, np + step)
        d.rows |> emplace <| logits_of(s, d.vocab)
    }
    if (replay) {
        let tape_stats = moe_pick_tape_stats()
        d.tape_seen = tape_stats.seen
        d.tape_miss = tape_stats.miss
        d.tape |> push_from(ref_tape)
    } else {
        d.tape <- moe_pick_tape_picks()
    }
    moe_pick_tape(MoePickTape.off)
    delete s
    return <- d
}

//! one row's compare: the two rows' largest difference against `rel` of the reference row's live max, and the same against
//! the reference row one step off (the control: a compare that passes it says nothing)
struct ParityRowReport {
    row : int64
    diff : float
    bar : float
    ctrl : float
    ok : bool
    argmax_a : int64
    argmax_b : int64
}

typedef ParityCompare = tuple<ok : bool; rows : array<ParityRowReport>; why : string>

def private refused(why : string) : ParityCompare {
    var none : array<ParityRowReport>
    return (ok = false, rows <- none, why = why)
}

//! `a` against the reference `b`: the same prompt, feed and tape, then every row within the bar and past its one-step-off
//! control (a NaN difference reads infinite and passes no control); `ok` where every row holds
def parity_dump_compare(a, b : ParityDump; rel : float) : ParityCompare {
    return refused("the vocabularies differ ({a.vocab} against {b.vocab})") if (a.vocab != b.vocab)
    return refused("the prompts differ") if (!sequence_equal(a.prompt, b.prompt))
    return refused("the fed tokens differ") if (!sequence_equal(a.feed, b.feed))
    return refused("the routing tapes differ ({length(a.tape)} against {length(b.tape)} picks)") if (!sequence_equal(a.tape, b.tape))
    return refused("the row counts differ ({length(a.rows)} against {length(b.rows)})") if (length(a.rows) != length(b.rows))
    var out : array<ParityRowReport>
    var ok = true
    out |> reserve(long_length(b.rows))
    for (i in range64(long_length(b.rows))) {
        let bar = rel * live_maxabs(b.rows[i])
        let ctrl_i = i > 0l ? i - 1l : min(1l, long_length(b.rows) - 1l)
        var r = ParityRowReport(row = i, diff = logits_maxdiff(a.rows[i], b.rows[i]), bar = bar, ctrl = logits_maxdiff(a.rows[i], b.rows[ctrl_i]),
            argmax_a = parallel_argmax(a.rows[i], a.vocab), argmax_b = parallel_argmax(b.rows[i], b.vocab))
        r.ok = r.diff <= bar && (ctrl_i == i || (is_finite(r.ctrl) && r.ctrl > bar))
        ok = ok && r.ok
        out |> push(r)
    }
    return (ok = ok, rows <- out, why = "")
}
