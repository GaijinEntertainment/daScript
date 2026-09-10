options gen2

require math

// The pure side of parrot: what the take does with the numbers the microphone and the voice
// detector hand it, the waveform's columns, and the chunk ledger of a say - none of it touches
// a device, a thread or the screen, so modules/dasLLAMA/tests/test_parrot.das holds it
// without a window or a model. main.das drives the devices, the speech thread and the panels.

let MIC_RATE = 24000               //! the model's own rate: the take is cloned as recorded
let VAD_RATE = 16000               //! Silero listens at 16 kHz; the take is resampled for it alone
let SILENCE_ENDS_TAKE_S = 2.0      //! quiet this long after speech ends the take
let TAKE_CAP_S = 60.0              //! the model's clip cap: the take ends here, and the clip never exceeds it
let TAKE_PAD_S = 0.25              //! kept around the speech at both ends
let NO_FRAMES_ENDS_TAKE_S = 6.0    //! a device that delivers nothing this long ends the take: the microphone was refused, or is not there

//! 24 kHz to 16 kHz by linear interpolation over the whole take: the next position to sample
//! carries across drains, so the stream Silero hears has no seam where the drains met
struct VadResampler {
    pos : double   //! the next 24 kHz position to sample
}

//! the 16 kHz samples the take has grown enough to yield since the last call
def resample_step(var r : VadResampler; take : array<float>; var out : array<float>) {
    out |> clear()
    let n = long_length(take)
    let step = double(MIC_RATE) / double(VAD_RATE)
    while (int64(r.pos) + 1l < n) {
        let j = int64(r.pos)
        let f = float(r.pos - double(j))
        out |> push(take[j] * (1.0 - f) + take[j + 1l] * f)
        r.pos += step
    }
}

//! a 16 kHz sample index of the detector as a 24 kHz index into the take
def vad_to_mic(sample16 : int64) : int64 {
    return sample16 * int64(MIC_RATE) / int64(VAD_RATE)
}

//! whether the take ends now: speech was heard and the quiet after it has lasted, the cap, or a
//! device that gave nothing for long enough to be gone
def take_ends(speech_seen, in_speech : bool; quiet_for_s, seconds, silent_device_for_s : float) : bool {
    return (speech_seen && !in_speech && quiet_for_s >= SILENCE_ENDS_TAKE_S) || seconds >= TAKE_CAP_S || silent_device_for_s >= NO_FRAMES_ENDS_TAKE_S
}

//! the clip's window in the take: the speech plus a pad at both ends, an open speech running to
//! the take's end, and never longer than the model's clip cap
def clip_window(start24, end24, total : int64; in_speech : bool) : tuple<from : int64; to : int64> {
    let pad = int64(TAKE_PAD_S * float(MIC_RATE))
    let cap = int64(TAKE_CAP_S) * int64(MIC_RATE)
    let from = clamp(start24 - pad, 0l, total)
    var to = min(total, (in_speech ? total : end24) + pad)
    to = clamp(to, from, from + cap)
    return (from = from, to = to)
}

//! the seconds of quiet left before the take ends on its own; 0 while speech runs or before any
def countdown_left(speech_seen, in_speech : bool; quiet_for_s : float) : float {
    return 0.0 if (!speech_seen || in_speech)
    return max(SILENCE_ENDS_TAKE_S - quiet_for_s, 0.0)
}

//! the waveform's columns: the lowest and highest sample of each `per_column` samples, grown from
//! `from` to the end of `samples` - a column that was open is redone, so a take drained mid-column
//! and the say's chunks appended one by one both draw the same picture
def peaks_append(var peaks : array<float2>; samples : array<float>; from : int64; per_column : int) {
    let first = from / int64(per_column)
    peaks |> resize(first)
    let n = long_length(samples)
    var c = first
    while (c * int64(per_column) < n) {
        let lo_i = c * int64(per_column)
        let hi_i = min(lo_i + int64(per_column), n)
        var lo = 1.0
        var hi = -1.0
        for (i in range64(lo_i, hi_i)) {
            lo = min(lo, samples[i])
            hi = max(hi, samples[i])
        }
        peaks |> push(float2(lo, hi))
        c++
    }
}

//! one chunk of a say, as the panels show it: waiting for its turn, its frames being generated,
//! generated and queued to play, playing, played
enum ChunkState {
    pending
    generating
    generated
    speaking
    spoken
}

struct ChunkRow {
    text : string
    state : ChunkState
    samples : int64
    start : int64      //! its first sample in the say's output waveform
    us_prompt : int64
    us_backbone : int64
    us_head : int64
    us_codec : int64
    us_total : int64
}

//! every chunk of the say played out (a say with no chunks yet is not read out)
def all_spoken(chunks : array<ChunkRow>) : bool {
    return false if (empty(chunks))
    for (row in chunks) {
        return false if (row.state != ChunkState.spoken)
    }
    return true
}
