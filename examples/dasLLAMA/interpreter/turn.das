options gen2

require math

// The pure side of the interpreter: the numbers the microphone and the voice detector hand the
// listen, the utterance's window, and the turn ledger - none of it touches a device, a thread or
// the screen, so modules/dasLLAMA/tests/test_interpreter.das holds it without a window or a model.
// main.das drives the devices, the worker thread and the panels.

let LISTEN_RATE = 16000              //! the take's rate: GigaAM and Silero both hear 16 kHz; the microphone opens at parrot's MIC_RATE and is resampled
let UTTERANCE_PAD_S = 0.25           //! kept around the speech at both ends
let UTTERANCE_CAP_S = 25.0           //! GigaAM's span cap: speech running past it is cut here and the rest heard as the next utterance
let LISTEN_IDLE_S = 30.0             //! quiet this long with no speech drops the buffered audio and starts the detector over
let NO_FRAMES_ENDS_LISTEN_S = 6.0    //! a device that delivers nothing this long ends the listen: the microphone was refused, or is not there

//! the utterance's window in the take: the speech plus a pad at both ends, never past the take, never longer than the cap
def utterance_window(start, end, total : int64) : tuple<from : int64; to : int64> {
    let pad = int64(UTTERANCE_PAD_S * float(LISTEN_RATE))
    let cap = int64(UTTERANCE_CAP_S * float(LISTEN_RATE))
    let from = clamp(start - pad, 0l, total)
    let to = clamp(end + pad, from, min(total, from + cap))
    return (from = from, to = to)
}

def speech_at_cap(start, total : int64) : bool => total - start >= int64(UTTERANCE_CAP_S * float(LISTEN_RATE))

def is_idle_take(in_speech : bool; total : int64) : bool => !in_speech && total >= int64(LISTEN_IDLE_S * float(LISTEN_RATE))

//! the first n frames appended to raw under the gain, each clamped to [-1, 1]; answers the rms of what was appended
def append_gained(var raw : array<float>; frames : array<float>; n : int; gain : float) : float {
    var ssq = 0.0
    let base = length(raw)
    raw |> ensure_capacity(base + n)
    raw |> resize(base + n)
    for (i in range(n)) {
        let v = clamp(frames[i] * gain, -1.0, 1.0)
        raw[base + i] = v
        ssq += v * v
    }
    return sqrt(ssq / float(n))
}

//! the utterance [start, end) cut from the take inside its padded window
def utterance_clip(take : array<float>; start, end : int64) : array<float> {
    let w = utterance_window(start, end, long_length(take))
    return <- [for (i in range64(w.from, w.to)); take[i]]
}

//! the speech the voice detector's boundaries open and close over one drain
struct SpeechSpan {
    open : bool
    start : int64
    end : int64 = -1l          //! where this drain's boundaries closed the speech; -1 while none did
    next_start : int64 = -1l   //! a start after that close: the speech the take keeps open behind the cut; -1 while none came
}

//! one boundary folded in: a start opens the speech unless it is open already (after a close in the same drain it names the
//! next speech), an end closes open speech inside the take
def speech_mark(s : SpeechSpan; starts : bool; sample, total : int64) : SpeechSpan {
    var r = s
    if (starts) {
        if (r.end >= 0l) {
            r.next_start = r.next_start < 0l ? max(r.end, sample) : r.next_start
        } elif (!r.open) {
            r.start = max(0l, sample)
            r.open = true
        }
    } elif (r.open && r.end < 0l) {
        r.end = min(sample, total)
        r.open = false
    }
    return r
}

//! the span the next drain starts from once the take kept its samples from `kept_from` on: the next speech open where one
//! started behind the close, else the open speech from the kept take's start, else nothing open
def span_after_cut(s : SpeechSpan; kept_from : int64) : SpeechSpan {
    if (s.next_start >= 0l) {
        return SpeechSpan(open = true, start = max(0l, s.next_start - kept_from))
    }
    return SpeechSpan(open = s.open && s.end < 0l, start = 0l)
}

//! what a drain does with the take once its boundaries are in
enum ListenCut {
    none
    at_end    //! the speech closed: cut [start, end)
    at_cap    //! open speech ran the cap: cut it at the take's end
    idle      //! a quiet take idled out: drop it
}

def listen_cut(s : SpeechSpan; total : int64) : ListenCut {
    return ListenCut.at_end if (s.end > s.start)
    return ListenCut.at_cap if (s.open && speech_at_cap(s.start, total))
    return ListenCut.idle if (is_idle_take(s.open, total))
    return ListenCut.none
}

//! one turn, as the history shows it: transcribing (cut from the take and queued), translating, speaking (its clip generated
//! or playing), spoken; or empty, where nothing was transcribed or translated
enum TurnState {
    transcribing
    translating
    speaking
    spoken
    empty
}

struct TurnRow {
    no : int
    state : TurnState
    utterance_s : float
    ru : string
    en : string
    clip_samples : int64
    out_start : int64
    asr_us : int64
    nmt_us : int64
    tts_us : int64
}

//! the transcript (`heard`) or the translation landed after `us`: the turn moves on a stage, or to empty on no words
def land_text(t : TurnRow; heard : bool; text : string; us : int64) : TurnRow {
    var r = t
    if (heard) {
        r.ru = text
        r.asr_us = us
    } else {
        r.en = text
        r.nmt_us = us
    }
    r.state = empty(text) ? TurnState.empty : (heard ? TurnState.translating : TurnState.speaking)
    return r
}

def turn_in_flight(state : TurnState) : bool => state != TurnState.spoken && state != TurnState.empty

def turns_in_flight(turns : array<TurnRow>) : int {
    var n = 0
    for (t in turns) {
        if (turn_in_flight(t.state)) {
            n++
        }
    }
    return n
}

//! the turn numbered `no`, -1 when the ledger has none
def turn_index(turns : array<TurnRow>; no : int) : int {
    for (t, i in turns, count()) {
        return i if (t.no == no)
    }
    return -1
}
