options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dasllama/dasllama   // the facade: the TTS model, its caps, the clone verb, the chunker, the synthesis, and Silero (when a take has speech and when it stops)
require take                // the pure side: the take's numbers, the text box's rules, a button's box
require daslib/jobque_boost
require daslib/strings_boost
require daslib/clargs
require daslib/fio
require daslib/archive
require glfw/glfw_boost
require live/glfw_live
require opengl/opengl_boost
require opengl/opengl_cache
require opengl/opengl_ttf
require audio/audio_boost
require audio/audio_record
require live_host
require math
require daslib/math_boost   // ortho_rh / compose for the text layer
require strings

// Parrot. You press record and talk for as long as you like; the take ends when you press stop
// or when you have been quiet for two seconds. Pocket TTS clones the voice from the take, and
// the text in the box - a poem to begin with, or whatever you type - is read aloud in it when
// you press say. Record again and the voice is replaced. Escape quits.
//
//   bin/daslang -jit examples/dasLLAMA/parrot/main.das -- --models <dir> [--text "..."] [--clip voice.wav]
//
// <dir> holds the speech model - a Pocket TTS file with its codec encoder, which is what clones -
// and silero_vad.bin. The recording stays in the program: nothing leaves the machine, and in the
// browser build nothing leaves the tab. --clip clones from a file instead of the microphone (the
// smoke rail). The microphone is drained on the frame thread; the clone and the synthesis run on
// the speech thread, fed through one stream and answering through another, and the frame thread
// plays the clips back to back.

[CommandLineArgs]
struct ParrotArgs {
    @clarg_short = "m"
    @clarg_doc = "Directory holding the speech model and the voice-activity model (default: the current directory)"
    models : string = "."

    @clarg_doc = "The TTS model file inside --models (a Pocket TTS file with its codec encoder)"
    tts_model : string = "pocket-tts-en-kq.gguf"

    @clarg_doc = "The voice-activity model file inside --models (default: the tree's own modules/dasLLAMA/models/silero_vad.bin)"
    vad_model : string

    @clarg_short = "t"
    @clarg_doc = "The text in the box at the start (default: a poem)"
    text : string

    @clarg_doc = "Clone the voice from this clip instead of the microphone (wav, flac, mp3 or ogg)"
    clip : string

    @clarg_doc = "Clone from --clip, say the text, quit when it has been read out - the smoke rail"
    smoke : bool

    @clarg_doc = "Stop after this many frames (0 = never) - the smoke rail"
    max_frames : int

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

let POEM = "Whose woods these are I think I know.\nHis house is in the village though;\nHe will not see me stopping here\nTo watch his woods fill up with snow.\n\nMy little horse must think it queer\nTo stop without a farmhouse near\nBetween the woods and frozen lake\nThe darkest evening of the year.\n\nHe gives his harness bells a shake\nTo ask if there is some mistake.\nThe only other sound's the sweep\nOf easy wind and downy flake.\n\nThe woods are lovely, dark and deep,\nBut I have promises to keep,\nAnd miles to go before I sleep,\nAnd miles to go before I sleep."

let TEXT_SIZE = 0.66
let SMALL_SIZE = 0.5
let VOICE_NAME = "you"
let READ_OUT_STATUS = "read out - change the text and say again, or record a new voice"

enum Phase {
    idle
    recording
    cloning
}

//! frame thread -> speech thread: the setup, a clip to clone, a text to say, or the stop
struct Ask {
    kind : int        //! 0 = the model path, 1 = a clone, 2 = a text, 3 = stop
    text : string
    gen : int         //! which say it belongs to: a chunk of a say the user replaced is skipped
    pcm : array<float>
    rate : int
}

//! speech thread -> frame thread: the model's facts, a voice cloned, a clip of a say, or how many clips a say will have
struct Answer {
    kind : int        //! 0 = ready (the rate, cloning in gen, the roster size in text), 1 = cloned (seconds in text), 2 = a clip, 3 = the say's chunk count (in text)
    text : string
    gen : int
    pcm : array<float>
    rate : int
}

var g_args = ParrotArgs()
var g_phase = Phase.idle
var g_lines : array<string>       //! the text box, one line each
var g_status = ""
var g_voice_ready = false
var g_voice_seconds = 0.0
var g_says = 0
var g_chunks_owed = 0             //! clips of the current say still to play; the speech thread names the count first
var g_count_known = false
var g_read_out_say = 0            //! the last say whose read-out was logged, so the line is written once
var g_take : array<float>         //! the microphone as recorded, 24 kHz mono
var g_take_scratch : array<float>
var g_vad_model = VadModel()
var g_vad = VadIter()
var g_ears = VadResampler()
var g_ears_out : array<float>
var g_speech_seen = false
var g_in_speech = false
var g_speech_start24 = 0l
var g_speech_end24 = 0l
var g_quiet_since_s = 0.0
var g_take_started_s = 0.0
var g_last_frames_s = 0.0         //! when the device last delivered a frame; a device that never does ends the take
var g_frames_seen = false
var g_level = 0.0
var g_level_peak = 0.0
var g_ready = false               //! the speech thread has the model: its rate, whether it clones, its roster
var g_mic_rate = MIC_RATE
var g_can_clone = false
var g_roster = 0
var g_pending_clip : array<float> //! a --clip decoded before the model was ready

var g_ask : Stream?
var g_answer : Stream?
var g_say_now : SeqBox?
var g_speech_done : Channel?
var g_clips : array<Answer>
var g_playing_sid = INVALID_SID
var g_elapsed_s = 0.0
var g_speaking_until = 0.0
var g_audio_initialized = false
var g_asch : AudioSystemChannels
var g_font : Font?
var g_frames = 0
var g_key_was : bool[512]   // GLFW key codes end at GLFW_KEY_MENU = 348
var g_mouse_was = false
var g_backspace_held_s = 0.0
var display_w = 0
var display_h = 0

// ===== the speech thread =====

//! the speech thread's setup rides its own stream ahead of the asks: the model path
[arch(at = "../ARCHITECTURE.md#speech-thread-stream")]
def start_speech_thread(var ask, answer : Stream?; var now : SeqBox?; var done : Channel?) {
    new_thread() <| @capture(= ask, = answer, = now, = done) {
        setup_dasllama_jobque()   // the fork-context pool is per context: without it every parallel kernel clones the program
        var tts_path = ""
        ask |> pop_archive() $(var a : Ask&) {
            tts_path = clone_string(a.text)
        }
        var inscope m <- load_tts_model(tts_path)
        var inscope c <- caps(m)
        var voice = empty(c.voices) ? "" : c.voices[0]
        var ready = Answer(kind = 0, text = "{length(c.voices)}", gen = c.cloning ? 1 : 0, rate = c.sample_rate)
        answer |> push_archive(ready)
        var running = true
        while (running) {
            ask |> pop_archive() $(var a : Ask&) {
                if (a.kind == 3) {
                    running = false
                } elif (a.kind == 1) {
                    tts_register_voice(m, VOICE_NAME, a.pcm, a.rate)
                    voice = VOICE_NAME
                    var cloned = Answer(kind = 1, text = "{float(length(a.pcm)) / float(a.rate)}", gen = a.gen, rate = a.rate)
                    answer |> push_archive(cloned)
                } elif (a.kind == 2) {
                    var inscope chunks <- tts_chunks(m, a.text)
                    var count = Answer(kind = 3, text = "{length(chunks)}", gen = a.gen)
                    answer |> push_archive(count)
                    for (chunk in chunks) {
                        var current = a.gen
                        now |> read() $(gen : int) {
                            current = gen
                        }
                        break if (a.gen < current)   // a say the user replaced: its clips would be dropped unheard
                        var inscope s <- synthesize(m, chunk, voice)
                        var clip = Answer(kind = 2, gen = a.gen, pcm := s.pcm, rate = s.sample_rate)
                        answer |> push_archive(clip)
                    }
                }
            }
        }
        ask |> release()
        answer |> release()
        now |> seq_box_release()
        done |> notify_and_release()
    }
}

def poll_answers() {
    g_answer |> try_pop() $(bytes) {
        var a : Answer
        mem_archive_load(bytes, a)
        if (a.kind == 0) {
            g_ready = true
            g_mic_rate = a.rate
            g_can_clone = a.gen != 0
            g_roster = to_int(a.text)
            g_status = g_can_clone ? "press record and talk, then say" : "this file cannot clone - parrot needs a Pocket file with its codec encoder; say reads in the model's voice"
            if (!empty(g_pending_clip)) {
                if (!g_can_clone) {
                    panic("parrot: --clip needs a file that clones; {g_args.tts_model} carries no codec encoder")
                }
                clone_clip(g_pending_clip, g_mic_rate)
            }
        } elif (a.kind == 1) {
            g_voice_ready = true
            g_voice_seconds = to_float(a.text)
            g_phase = Phase.idle
            g_status = "your voice is ready - press say"
            //! the smoke rail's witness lines, here and below: modules/dasLLAMA/tests/test_parrot.das reads them word for word
            to_log(LOG_INFO, "parrot: cloned {a.text} s of speech\n")
            if (g_args.smoke) {
                say_text()
            }
        } elif (a.gen == g_says) {
            if (a.kind == 3) {
                g_chunks_owed = to_int(a.text)
                g_count_known = true
            } else {
                g_clips |> emplace(a)
            }
        }
    }
    return if (g_elapsed_s < g_speaking_until)
    g_playing_sid = INVALID_SID
    if (!empty(g_clips)) {
        let seconds = float(length(g_clips[0].pcm)) / float(max(g_clips[0].rate, 1))
        var pcm <- g_clips[0].pcm
        g_playing_sid = play_sound_from_pcm(g_clips[0].rate, 1, pcm)
        g_clips |> erase(0)
        g_speaking_until = g_elapsed_s + seconds
        g_chunks_owed--
    } elif (g_count_known && g_chunks_owed <= 0 && g_says > g_read_out_say) {
        g_read_out_say = g_says
        if (g_phase == Phase.idle) {
            g_status = READ_OUT_STATUS
        }
        to_log(LOG_INFO, "parrot: say {g_says} is read out\n")
    }
}

// ===== the take =====

def start_take() {
    if (g_phase == Phase.cloning) {
        g_status = "cloning your voice - a moment, then record again"
        return
    }
    return if (g_phase != Phase.idle)
    if (!g_ready) {
        g_status = "the model is still loading"
        return
    }
    if (!g_can_clone) {
        g_status = "this file cannot clone - parrot needs a Pocket file with its codec encoder"
        return
    }
    cancel_say()   // a clip still playing would be recorded
    if (!sound_record_start(g_mic_rate, 1, g_mic_rate * 4, -1)) {
        g_status = "no microphone - is one connected, and allowed?"
        return
    }
    g_take |> clear()
    g_take |> reserve(int(TAKE_CAP_S) * g_mic_rate)
    g_take_scratch |> resize(g_mic_rate)
    vad_iter_reset(g_vad, default_vad_opts())
    g_ears.pos = 0.0lf
    g_speech_seen = false
    g_in_speech = false
    g_speech_start24 = 0l
    g_speech_end24 = 0l
    g_quiet_since_s = g_elapsed_s
    g_take_started_s = g_elapsed_s
    g_last_frames_s = g_elapsed_s
    g_frames_seen = false
    g_level = 0.0
    g_level_peak = 0.0
    g_phase = Phase.recording
    g_status = "listening - talk for as long as you like, then press stop or go quiet"
    to_log(LOG_INFO, "parrot: recording\n")
}

//! the frames the device has since delivered, onto the take
def append_frames(n : int) {
    var ssq = 0.0
    let base = length(g_take)
    g_take |> ensure_capacity(base + n)
    g_take |> resize(base + n)
    for (i in range(n)) {
        let v = g_take_scratch[i]
        g_take[base + i] = v
        ssq += v * v
    }
    let rms = sqrt(ssq / float(n))
    g_level = max(rms, g_level * 0.85)
    g_level_peak = max(g_level_peak, rms)
    g_frames_seen = true
    g_last_frames_s = g_elapsed_s
}

[arch(at = "../ARCHITECTURE.md#the-take")]
def drain_take() {
    let n = sound_record_read(g_take_scratch)
    if (n > 0) {
        append_frames(n)
        resample_step(g_ears, g_take, g_ears_out)
        vad_iter_feed(g_vad_model, g_vad, g_ears_out) $(ev) {
            let at24 = vad_to_mic(ev.sample)
            if (ev.kind == VadEventKind.speech_start) {
                if (!g_speech_seen) {
                    g_speech_start24 = at24
                }
                g_speech_seen = true
                g_in_speech = true
            } else {
                g_speech_end24 = at24
                g_in_speech = false
                g_quiet_since_s = g_elapsed_s
            }
        }
    }
    let seconds = float(length(g_take)) / float(g_mic_rate)
    if (take_ends(g_speech_seen, g_in_speech, g_elapsed_s - g_quiet_since_s, seconds, g_elapsed_s - g_last_frames_s)) {
        stop_take()
    }
}

def stop_take() {
    return if (g_phase != Phase.recording)
    drain_rest()   // before the device stops: stopping frees the ring
    sound_record_stop()
    let dropped = sound_record_overflow_frames()
    if (dropped > 0l) {
        to_log(LOG_WARNING, "parrot: {dropped} frames were dropped while the frame loop stalled - the take is spliced there\n")
    }
    if (!g_speech_seen) {
        g_phase = Phase.idle
        let seconds = float(length(g_take)) / float(g_mic_rate)
        g_status = (!g_frames_seen
            ? "nothing heard - the microphone gave no audio at all: it was refused, or there is none"
            : g_level_peak < 0.001
                ? "nothing heard - the microphone gave silence for {seconds} s: is it allowed for this page, and on?"
                : "nothing heard - the microphone peaked at {g_level_peak} over {seconds} s, too quiet for speech: come closer and talk")
        to_log(LOG_INFO, "parrot: nothing heard - peak {g_level_peak} over {seconds} s\n")
        return
    }
    let w = clip_window(g_speech_start24, g_speech_end24, long_length(g_take), g_in_speech)
    var clip : array<float>
    clip |> resize(w.to - w.from)
    for (i in range64(w.to - w.from)) {
        clip[i] = g_take[w.from + i]
    }
    clone_clip(clip, g_mic_rate)
}

//! the ring's tail before the device stops
def drain_rest() {
    for (_i in range(8)) {
        let n = sound_record_read(g_take_scratch)
        break if (n <= 0)
        append_frames(n)
    }
}

def clone_clip(var clip : array<float>; rate : int) {
    g_phase = Phase.cloning
    g_status = "cloning your voice from {float(length(clip)) / float(rate)} seconds..."
    var a = Ask(kind = 1, gen = g_says, pcm <- clip, rate = rate)
    g_ask |> push_archive(a)
}

//! a say in flight is dropped: its clip stops, its queued clips go unplayed, and the speech thread
//! skips its remaining chunks at the next chunk boundary
def cancel_say() {
    if (g_playing_sid != INVALID_SID) {
        stop(g_playing_sid, 0.05)
        g_playing_sid = INVALID_SID
    }
    g_says++
    g_say_now |> publish(g_says)
    g_clips |> clear()
    g_speaking_until = g_elapsed_s
    g_chunks_owed = 0
    g_count_known = false
}

// ===== saying =====

def say_text() {
    let text = strip(join(g_lines, "\n"))
    return if (empty(text) || g_phase != Phase.idle || !g_ready)   // a say while recording would be recorded
    if (!g_voice_ready && g_roster == 0) {
        g_status = "no voice to read in - record one"
        return
    }
    cancel_say()
    var a = Ask(kind = 2, text = text, gen = g_says)
    g_ask |> push_archive(a)
    g_status = g_voice_ready ? "reading in your voice..." : "reading in the model's own voice - record to hear yours"
    to_log(LOG_INFO, "parrot: say {g_says} in voice {g_voice_ready ? VOICE_NAME : "default"}, {length(text)} characters\n")
}

// ===== the text box =====

def set_text(text : string) {
    delete g_lines
    g_lines <- lines_of(text)
}

//! down this frame and not the last
def key_pressed_now(key : int) : bool {
    let down = glfwGetKey(live_window, key) == GLFW_PRESS
    let was = g_key_was[key]
    g_key_was[key] = down
    return down && !was
}

def key_down(key : int) : bool {
    return glfwGetKey(live_window, key) == GLFW_PRESS
}

//! Ctrl or Command: the say chord works the same on every desktop
def ctrl_down() : bool {
    return (key_down(GLFW_KEY_LEFT_CONTROL) || key_down(GLFW_KEY_RIGHT_CONTROL) || key_down(GLFW_KEY_LEFT_SUPER) || key_down(GLFW_KEY_RIGHT_SUPER))
}

def shift_down() : bool {
    return key_down(GLFW_KEY_LEFT_SHIFT) || key_down(GLFW_KEY_RIGHT_SHIFT)
}

//! a printable GLFW key code is its upper-case ASCII, so the key range IS the character range
[arch(at = "../ARCHITECTURE.md#polled-keys")]
def poll_keys() {
    let shift = shift_down()
    for (key in range(GLFW_KEY_SPACE, GLFW_KEY_GRAVE_ACCENT + 1)) {
        if (key_pressed_now(key)) {
            let c = typed_char(key, shift)
            if (c >= 0) {
                append_char(g_lines, c)
            }
        }
    }
    if (key_pressed_now(GLFW_KEY_BACKSPACE)) {
        erase_last(g_lines)
        g_backspace_held_s = 0.0
    } elif (glfwGetKey(live_window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) {
        g_backspace_held_s += get_dt()
        if (g_backspace_held_s > 0.4) {   // held: a repeat every 60 ms after the first 400
            erase_last(g_lines)
            g_backspace_held_s = 0.34
        }
    }
    if (key_pressed_now(GLFW_KEY_ENTER) || key_pressed_now(GLFW_KEY_KP_ENTER)) {
        if (ctrl_down()) {
            say_text()
        } else {
            new_line(g_lines)
        }
    }
    if (key_pressed_now(GLFW_KEY_TAB)) {
        toggle_record()
    }
}

def toggle_record() {
    if (g_phase == Phase.recording) {
        stop_take()
    } else {
        start_take()
    }
}

// ===== the screen =====

//! the text layer's coordinates are design pixels against a 1280x720 reference; the smaller of the two ratios
//! governs, so a window narrower than 16:9 shrinks the text instead of cutting the lines on the right
def hud_scale() : float {
    let fit = min(float(display_w) / 1280.0, float(display_h) / 720.0)
    return max(fit, 0.5)
}

def design_height() : float {
    return float(display_h) / hud_scale()
}

def text_mvp(x, y, scale : float) : float4x4 {
    let projection = ortho_rh(0.0, float(display_w), float(display_h), 0.0, -1.0, 1.0)
    let model = compose(float3(x, y, 0.0), float4(0.0, 0.0, 0.0, 1.0), float3(scale, scale, 1.0))
    return projection * model
}

def draw_text(text : string; x, y : float; size : float; tint : float3) {
    return if (g_font == null || empty(text))
    var quads <- (*g_font) |> create_quads(text)
    let s = hud_scale() * size
    (*g_font) |> draw_quads(quads, text_mvp(x * hud_scale(), y * hud_scale(), s), tint)
    delete quads
}

//! a text's box in design pixels at `size`, relative to the pen it is drawn at: left, top, right, bottom
//! (the glyphs sit above the baseline, so the top is negative)
def text_box(text : string; size : float) : float4 {
    return float4(0.0) if (g_font == null || empty(text))
    var quads <- (*g_font) |> create_quads(text)
    let d = quads_dim(quads)
    delete quads
    return float4(d.vmin.x, d.vmin.y, d.vmax.x, d.vmax.y) * size
}

//! a button: its label in a bracket, drawn at a pen position; it answers to the label's own box
struct Button {
    text : string
    x : float
    y : float
    box : float4   //! the label's box relative to the pen: left, top, right, bottom
}

def button(caption : string; x, y : float) : Button {
    return Button(text = caption, x = x, y = y, box = text_box("[ {caption} ]", TEXT_SIZE))
}

//! the pen position after the button, with a gap
def after(b : Button) : float {
    return b.x + b.box.z + 30.0
}

def draw_button(b : Button; lit : bool) {
    draw_text("[ {b.text} ]", b.x, b.y, TEXT_SIZE, lit ? float3(0.98, 0.75, 0.35) : float3(0.75, 0.72, 0.66))
}

def meter_text() : string {
    let cells = 24
    let lit = min(cells, int(g_level * 40.0 * float(cells)))
    return build_string() $(var w) {
        for (i in range(cells)) {
            w |> write(i < lit ? "#" : ".")
        }
    }
}

def draw_screen() {
    let ink = float3(0.93, 0.9, 0.82)
    let dim = float3(0.55, 0.52, 0.48)
    let accent = float3(0.98, 0.75, 0.35)
    draw_text("parrot", 60.0, 60.0, 0.6, dim)
    draw_text("your recording stays in this window: nothing is uploaded, and the voice is gone when you close it", 60.0, 96.0, SMALL_SIZE, dim)
    var y = 150.0
    let step = 30.0
    let bottom = design_height() - 150.0
    let visible = max(1, int((bottom - y) / step))
    let first = max(0, length(g_lines) - visible)
    for (i in range(first, length(g_lines))) {
        let caret = i == length(g_lines) - 1 && (g_frames / 30) % 2 == 0 ? "_" : ""
        draw_text("{g_lines[i]}{caret}", 60.0, y, TEXT_SIZE, ink)
        y += step
    }
    let row = design_height() - 120.0
    let rec = button(g_phase == Phase.recording ? "stop" : "record", 60.0, row)
    draw_button(rec, g_phase == Phase.recording)
    var x = after(rec)
    if (g_phase == Phase.recording) {
        let meter = meter_text()
        draw_text(meter, x, row, TEXT_SIZE, accent)
        draw_text("{float(length(g_take)) / float(MIC_RATE)} s", x + text_box(meter, TEXT_SIZE).z + 20.0, row, TEXT_SIZE, dim)
    } else {
        let sayb = button("say", x, row)
        draw_button(sayb, true)
        x = after(sayb)
        draw_text(g_voice_ready ? "voice: yours, from {g_voice_seconds} s" : "voice: the model's own until you record", x, row, SMALL_SIZE, dim)
    }
    draw_text(g_status, 60.0, design_height() - 70.0, SMALL_SIZE, g_phase == Phase.idle ? accent : dim)
    draw_text("Tab records and stops   Ctrl+Enter says   Enter is a new line", 60.0, design_height() - 40.0, SMALL_SIZE, dim)
}

//! the two buttons answer a click: the cursor in design pixels against their rectangles
def poll_mouse() {
    let down = glfwGetMouseButton(live_window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS
    let pressed = down && !g_mouse_was
    g_mouse_was = down
    return if (!pressed)
    var win_w, win_h : int
    glfwGetWindowSize(live_window, unsafe(addr(win_w)), unsafe(addr(win_h)))
    let c = glfwGetCursorPos(live_window) * float(display_w) / float(max(win_w, 1))   //! the cursor comes in window points; the text layer is laid out in framebuffer pixels, twice that on a retina screen
    let scale = hud_scale()
    let px = c.x / scale
    let py = c.y / scale
    let row = design_height() - 120.0
    let rec = button(g_phase == Phase.recording ? "stop" : "record", 60.0, row)
    if (hit(rec, px, py)) {
        toggle_record()
        return
    }
    if (g_phase != Phase.recording) {
        let sayb = button("say", after(rec), row)
        if (hit(sayb, px, py)) {
            say_text()
        }
    }
}

//! inside the label's box, with a margin around it
def hit(b : Button; px, py : float) : bool {
    return inside_box(b.box, b.x, b.y, px, py, 8.0)
}

// ===== the program =====

[export]
def init() {
    var inscope r <- parse_args(type<ParrotArgs>)
    if (r |> is_err) {
        panic("parrot: {r |> unwrap_err}")
    }
    g_args <- r |> move_unwrap
    set_text(empty(g_args.text) ? POEM : g_args.text)
    g_status = "press record and talk, then say"
    if (!empty(g_args.clip)) {
        // decoded before the window and the thread exist, so a clip that is not audio refuses here and the program ends
        delete g_pending_clip
        g_pending_clip <- load_audio_mono(g_args.clip, MIC_RATE)   // cloned once the thread says the file can; the model's rate is 24 kHz
        if (empty(g_pending_clip)) {
            panic("parrot: {g_args.clip} did not decode")
        }
        let cap = int(TAKE_CAP_S) * MIC_RATE
        if (length(g_pending_clip) > cap) {
            to_log(LOG_INFO, "parrot: {g_args.clip} is longer than the {int(TAKE_CAP_S)} s a voice is cloned from - the first {int(TAKE_CAP_S)} s are taken\n")
            g_pending_clip |> resize(cap)
        }
        to_log(LOG_INFO, "parrot: cloning from {g_args.clip}\n")
        g_status = "cloning from {g_args.clip}..."
    }
    live_create_window("Parrot", 1280, 720)
    cache_ttf_objects()
    g_font = cache_font("{get_das_root()}/modules/dasStbImage/fonts/droidsansmono.ttf")
    if (!g_audio_initialized) {
        g_asch = audio_system_create()
        g_audio_initialized = true
    }
    create_job_que()
    setup_dasllama_jobque()
    delete g_vad_model
    g_vad_model <- load_vad_model(empty(g_args.vad_model) ? "{get_das_root()}/modules/dasLLAMA/models/silero_vad.bin" : path_join(g_args.models, g_args.vad_model))
    g_ask = unsafe(stream_create())
    g_answer = unsafe(stream_create())
    g_say_now = seq_box_create()
    g_say_now |> publish(g_says)
    g_speech_done = unsafe(channel_create())
    g_speech_done |> append(1)
    var setup = Ask(kind = 0, text = path_join(g_args.models, g_args.tts_model))
    g_ask |> push_archive(setup)
    start_speech_thread(g_ask, g_answer, g_say_now, g_speech_done)
}

[export]
def update() {
    if (!live_begin_frame()) {
        return
    }
    g_frames++
    g_elapsed_s += get_dt()
    live_get_framebuffer_size(display_w, display_h)
    glViewport(0, 0, display_w, display_h)
    glClearColor(0.07, 0.06, 0.09, 1.0)
    glClear(GL_COLOR_BUFFER_BIT)
    glDisable(GL_DEPTH_TEST)
    glEnable(GL_BLEND)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)

    if (glfwGetKey(live_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(live_window, 1)
    }
    poll_keys()
    poll_mouse()
    if (g_phase == Phase.recording) {
        drain_take()
    }
    poll_answers()
    draw_screen()
    live_end_frame()
}

[export]
def shutdown() {
    if (g_phase == Phase.recording) {
        sound_record_stop()
    }
    cancel_say()   // the thread leaves a say in flight at its next chunk instead of finishing it
    var stop_ask = Ask(kind = 3)
    g_ask |> push_archive(stop_ask)
    g_speech_done |> join()
    g_say_now |> seq_box_release()
    unsafe {
        channel_remove(g_speech_done)
        stream_remove(g_ask)
        stream_remove(g_answer)
    }
    if (g_audio_initialized) {
        audio_system_finalize(g_asch.command, g_asch.next_sid)
        g_audio_initialized = false
    }
    delete g_vad_model
    destroy_job_que()
    live_destroy_window()
}

def read_out() : bool {
    return g_says > 0 && g_count_known && g_chunks_owed <= 0 && empty(g_clips) && g_elapsed_s >= g_speaking_until
}

def done_for_smoke() : bool {
    return true if (g_args.max_frames > 0 && g_frames >= g_args.max_frames)
    return g_args.smoke && read_out()
}

// eval_main_loop drives the block once per frame: a blocking while-loop natively, the
// browser's requestAnimationFrame on the web - one main for both
[export]
def main() {
    init()
    eval_main_loop() {
        update()
        return false if (done_for_smoke())
        return !exit_requested()
    }
    let finished = read_out()
    shutdown()
    if (g_args.smoke) {
        to_log(LOG_INFO, finished
            ? "parrot: the text was read out after {g_frames} frames\n"
            : "parrot: the frame cap stopped the run after {g_frames} frames\n")
    }
}
