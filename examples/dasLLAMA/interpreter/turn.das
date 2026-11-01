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

//! whether a take with no speech in it is dropped now, so an hour of quiet holds no audio
def is_idle_take(in_speech : bool; total : int64) : bool => !in_speech && total >= int64(LISTEN_IDLE_S * float(LISTEN_RATE))

//! one turn, as the history shows it: cut from the take and queued, transcribing, translating, speaking (its clip generated or
//! playing), spoken; or empty, where nothing was transcribed or translated
enum TurnState {
    heard
    transcribing
    translating
    speaking
    spoken
    empty
}

struct TurnRow {
    no : int
    state : TurnState
    heard_s : float    //! the utterance heard
    ru : string
    en : string
    clip_samples : int64   //! the spoken clip's samples
    out_start : int64  //! its first sample in the output waveform
    asr_us : int64
    nmt_us : int64
    tts_us : int64
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
