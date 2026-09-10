options gen2

require math
require strings
require daslib/strings_boost

// The pure side of parrot: what the take does with the numbers the microphone and the voice
// detector hand it, the text box's editing rules, and a button's hit box - none of it touches
// a device, a thread or the screen, so modules/dasLLAMA/tests/test_parrot.das holds it
// without a window or a model. main.das drives the devices and the speech thread.

let MIC_RATE = 24000               //! the model's own rate: the take is cloned as recorded
let VAD_RATE = 16000               //! Silero listens at 16 kHz; the take is resampled for it alone
let SILENCE_ENDS_TAKE_S = 2.0      //! quiet this long after speech ends the take
let TAKE_CAP_S = 60.0              //! the model's clip cap: the take ends here, and the clip never exceeds it
let TAKE_PAD_S = 0.25              //! kept around the speech at both ends
let NO_FRAMES_ENDS_TAKE_S = 6.0    //! a device that delivers nothing this long ends the take: the microphone was refused, or is not there
let WRAP_CHARS = 74                //! droidsansmono is monospace: characters are the wrap unit
let MAX_LINES = 24

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

//! the character a key lands in the box as: letters follow shift, the list's punctuation and the
//! shifted digits that spell it, spaces; -1 for a key the box does not take
def typed_char(key : int; shift : bool) : int {
    return -1 if (key < 0 || key >= 128)
    var c = key
    if (is_alpha(c)) {
        c = shift ? c : c + 32   // GLFW hands the upper-case code
    } elif (shift) {
        c = c == '1' ? '!' : c == '/' ? '?' : c == ';' ? ':' : c == '\'' ? '"' : c == '9' ? '(' : c == '0' ? ')' : c == '-' ? '_' : c
    }
    let ok = is_alpha(c) || is_number(c) || c == ' ' || c == ',' || c == '.' || c == '\'' || c == '"' || c == '-' || c == ';' || c == ':' || c == '!' || c == '?' || c == '(' || c == ')' || c == '_'
    return ok ? c : -1
}

//! a character onto the last line, unless that line is full
def append_char(var lines : array<string>; c : int) {
    return if (empty(lines))
    let li = length(lines) - 1
    return if (length(lines[li]) >= WRAP_CHARS)
    lines[li] = "{lines[li]}{to_char(c)}"
}

//! a new line after the last, unless the box is full
def new_line(var lines : array<string>) {
    return if (length(lines) >= MAX_LINES)
    lines |> push("")
}

//! the last character off the last line; an empty last line goes away unless it is the only one
def erase_last(var lines : array<string>) {
    return if (empty(lines))
    let li = length(lines) - 1
    let n = length(lines[li])
    if (n == 0) {
        if (li > 0) {
            lines |> pop()
        }
        return
    }
    lines[li] = n == 1 ? "" : clone_string(slice(lines[li], 0, n - 1))   // never a zero-length view of the string being replaced
}

//! the box's lines from a text, one per line break, never empty
def lines_of(text : string) : array<string> {
    var lines <- split(text, "\n")
    if (empty(lines)) {
        lines |> push("")
    }
    return <- lines
}

//! inside a label's box (left, top, right, bottom relative to the pen at x, y), with a margin around it
def inside_box(box : float4; x, y, px, py, pad : float) : bool {
    return px >= x + box.x - pad && px <= x + box.z + pad && py >= y + box.y - pad && py <= y + box.w + pad
}
