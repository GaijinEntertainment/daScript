options gen2
options persistent_heap
options gc
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dasllama/dasllama   // the facade: the ASR model, the translator, the TTS model, and Silero
require turn                // the pure side: the listen's numbers, the utterance window, the turn ledger
require ../parrot/take.das  // peaks_append, the waveform columns; MIC_RATE and the resampler parrot hears the microphone through
require ../parrot/screen.das   // the palette, the waveform box and the status head parrot draws
require imgui/imgui_harness // the window, the frame, the boost widgets and the draw list
require imgui/imgui_touch   // whether a finger, not a keyboard, is at the controls
require daslib/jobque_boost
require daslib/strings_boost
require daslib/clargs
require daslib/fio
require daslib/archive
require audio/audio_boost
require audio/audio_record
require math
require strings

// Interpreter: press listen and talk in Russian; Silero VAD cuts each utterance at the pause, GigaAM
// transcribes it, Bergamot translates it and Pocket TTS says the English. The history keeps every
// turn - what was heard, what was said, and the time each stage took - and the same program runs
// on the desktop and, through daspkg release wasm, in the browser.

[CommandLineArgs]
struct InterpreterArgs {
    @clarg_doc = "directory holding the models"
    models : string = "{get_das_root()}/../llama.cpp/models"
    @clarg_doc = "the GigaAM GGUF under --models: the Russian speech in"
    asr_model : string = "gigaam-v3-e2e-rnnt-q4_k.gguf"
    @clarg_doc = "the Bergamot ru-en student under --models (its vocab beside it): the translation"
    nmt_model : string = "bergamot/ru-en-base-memory/model.ruen.intgemm.alphas.bin"
    @clarg_doc = "the Pocket TTS GGUF under --models: the English speech out"
    tts_model : string = "pocket-tts-en-18m-stuart-kq.gguf"
    @clarg_doc = "the Silero VAD weights under --models (empty: the tree's own copy)"
    vad_model : string = ""
    @clarg_doc = "a clip heard in place of the microphone, as one utterance (wav, flac, mp3, ogg)"
    clip : string = ""
    @clarg_doc = "pin the speech model's q8 lane: the carrier dequantized to Q8_0 at load instead of serving its own planes"
    q8 : bool = false
    @clarg_doc = "the smoke rail: hear --clip, translate it, speak it, exit once it is spoken"
    smoke : bool = false
    @clarg_doc = "stop after this many frames (0: never)"
    max_frames : int = 0
}

let PEAK_COLUMN = 160   //! samples per waveform column: 10 ms at the detector's 16 kHz
let OUT_PEAK_COLUMN = 240   //! samples per output waveform column: 10 ms at Pocket's 24 kHz

enum AskKind {
    setup   //! the model paths
    hear    //! the utterance in `pcm`
    stop
}

//! the frame thread's requests to the worker thread, as archived records
struct Ask {
    kind : AskKind
    asr : string
    nmt : string
    tts : string
    pin_q8 : bool
    turn : int
    pcm : array<float>
}

enum AnswerKind {
    ready        //! the TTS rate, its voice in `text`, the speech lane and the direction
    heard        //! the transcript in `text`, the wall time in `us`
    translated   //! the English in `text`
    clip         //! the turn's speech in `pcm`
    error        //! the load or a serve panicked: why in `text`
}

//! the worker thread's answers, as archived records
struct Answer {
    kind : AnswerKind
    text : string
    lane : string
    direction : string
    turn : int
    rate : int
    us : int64
    pcm : array<float>
}

//! the per-turn rows of the history: indexed widgets take their state from a table
var private TURN_RU : table<int; NarrativeState>
var private TURN_EN : table<int; NarrativeState>
var private TURN_TIMES : table<int; NarrativeState>
var private TURN_GAP : table<int; NarrativeState>

var g_args = InterpreterArgs()
var g_listening = false
var g_status = ""
var g_turns : array<TurnRow>         //! every turn of the session, oldest first
var g_turn_no = 0
var g_raw : array<float>             //! the microphone since the last cut as recorded, MIC_RATE mono, the gain applied
var g_mic_resampler = VadResampler()
var g_resampled : array<float>       //! one drain's frames at 16 kHz, on their way onto the take
var g_take : array<float>            //! the microphone since the last cut at 16 kHz: what the detector and GigaAM hear
var g_mic_frames : array<float>      //! the frames one device read hands over
var g_take_peaks : array<float2>
var g_vad_model = VadModel()
var g_vad = VadIter()
var g_in_speech = false
var g_speech_start = 0l
var g_last_frames_s = 0.0            //! when the device last delivered a frame; a device that never does ends the listen
var g_frames_seen = false
var g_level = 0.0
var g_level_peak = 0.0
var g_models_ready = false
var g_tts_rate = 24000
var g_voice = ""
var g_lane = ""
var g_direction = ""
var g_error = ""
var g_pending_clip : array<float>    //! a --clip decoded before the models were ready

var g_ask : Stream?
var g_answer : Stream?
var g_worker_done : Channel?
var g_quitting : Atomic32?           //! set at exit: the worker skips the turns still queued ahead of the stop
var g_unplayed_clips : array<Answer>
var g_out_pcm : array<float>         //! the session's spoken audio, every clip appended
var g_out_peaks : array<float2>
var g_playing_sid = INVALID_SID
var g_playing_turn = -1
var g_play_status : SeqBox?
var g_play_start = 0l
var g_playhead = 0l
var g_elapsed_s = 0.0
var g_speaking_until = 0.0
var g_audio_initialized = false
var g_asch : AudioSystemChannels
var g_frames = 0
var g_logged_done_turns = 0

let IDLE_RELEASE_MS = 500   //! the worker thread waits this long for a request before it gives the engine's scratch back

[arch(at = "../ARCHITECTURE.md#interpreter-turns")]
def serve_turn(var asr : AsrModel; var tr : Translator; var tts : TtsModel; voice : string; var a : Ask; var answer : Stream?) {
    var t0 = ref_time_ticks()
    var inscope s <- create_session(asr, "ru")
    let ru = strip(transcribe(asr, s, a.pcm))
    var heard = Answer(kind = AnswerKind.heard, turn = a.turn, text = ru, us = int64(get_time_usec(t0)))
    answer |> push_archive(heard)
    return if (empty(ru))
    t0 = ref_time_ticks()
    let en = strip(translate(tr, ru))
    var translated = Answer(kind = AnswerKind.translated, turn = a.turn, text = en, us = int64(get_time_usec(t0)))
    answer |> push_archive(translated)
    return if (empty(en))
    t0 = ref_time_ticks()
    var pcm : array<float>
    var rate = 0
    var inscope chunks <- tts_chunks(tts, en)
    for (chunk in chunks) {
        var inscope said <- synthesize(tts, chunk, voice)
        rate = said.sample_rate
        pcm |> push_from(said.pcm)
    }
    var inscope clip = Answer(kind = AnswerKind.clip, turn = a.turn, pcm <- pcm, rate = rate, us = int64(get_time_usec(t0)))
    answer |> push_archive(clip)
}

[arch(at = "../ARCHITECTURE.md#speech-thread-stream")]
def serve_asks(var asr : AsrModel; var tr : Translator; var tts : TtsModel; var ask, answer : Stream?; quitting : Atomic32?) {
    var inscope c <- caps(tts)
    let voice = empty(c.voices) ? "" : c.voices[0]
    let langs = translator_langs(tr)
    var inscope ready = Answer(kind = AnswerKind.ready, rate = c.sample_rate, text = voice, lane = asr_exec_fmt(asr), direction = "{langs.src} -> {langs.trg}")
    answer |> push_archive(ready)
    var running = true
    var released = false
    while (running) {
        let got = ask |> pop_archive_with_timeout(IDLE_RELEASE_MS) $(var a : Ask&) {
            if (a.kind == AskKind.stop) {
                running = false
            } elif (a.kind == AskKind.hear && quitting |> get() == 0) {
                serve_turn(asr, tr, tts, voice, a, answer)
            }
        }
        if (got) {
            released = false
        } elif (!released) {
            tts_release_scratch(tts)
            released = true
        }
    }
}

[arch(at = "../ARCHITECTURE.md#speech-thread-stream")]
def start_worker_thread(var ask, answer : Stream?; var done : Channel?; quitting : Atomic32?) {
    new_thread() <| @capture(= ask, = answer, = done, = quitting) {
        setup_dasllama_jobque()   // the fork-context pool is per context: without it every parallel kernel clones the program
        var setup = Ask()
        ask |> pop_archive() $(var a : Ask&) {
            setup.asr = clone_string(a.asr)
            setup.nmt = clone_string(a.nmt)
            setup.tts = clone_string(a.tts)
            setup.pin_q8 = a.pin_q8
        }
        if (setup.pin_q8) {
            set_gigaam_q8(true)
        }
        allow_cpu_prefill()   // no LLM rides here; the roots' contract (test_program_roots) still asks every model-loading root for its prefill intent
        var loaded = false
        var why = ""
        try {
            var inscope asr <- load_asr_model(setup.asr)
            var inscope tr <- load_translator(setup.nmt)
            var inscope tts <- load_tts_model(setup.tts)
            loaded = true
            serve_asks(asr, tr, tts, ask, answer, quitting)
        } recover {
            why = this_context().last_exception |> rtrim
        }
        if (!empty(why)) {
            var failed = Answer(kind = AnswerKind.error, text = loaded ? "the worker stopped: {why}" : "the models did not load: {why}")
            answer |> push_archive(failed)
        }
        ask |> release()
        answer |> release()
        done |> notify_and_release()
    }
}

def on_ready(a : Answer) {
    g_models_ready = true
    g_tts_rate = a.rate
    g_voice = clone_string(a.text)
    g_lane = clone_string(a.lane)
    g_direction = clone_string(a.direction)
    g_status = "press listen and talk in Russian"
    to_log(LOG_INFO, "interpreter: ready - speech lane {g_lane}, {g_direction}, voice {g_voice} at {g_tts_rate} Hz\n")
    if (!empty(g_pending_clip)) {
        queue_turn(g_pending_clip)
        delete g_pending_clip
    }
}

def on_text(a : Answer; heard : bool) {
    let i = turn_index(g_turns, a.turn)
    return if (i < 0)
    g_turns[i] = land_text(g_turns[i], heard, clone_string(a.text), a.us)   // the record's bytes are the archive's: copied into this heap
    to_log(LOG_INFO, "interpreter: {heard ? "heard" : "translated"} {a.turn}: {a.text} ({ms(a.us)} ms)\n")
}

def on_clip(var a : Answer) {
    let i = turn_index(g_turns, a.turn)
    return if (i < 0)
    g_turns[i].tts_us = a.us
    g_turns[i].clip_samples = long_length(a.pcm)
    g_turns[i].out_start = long_length(g_out_pcm)
    g_out_pcm |> push_from(a.pcm)
    peaks_append(g_out_peaks, g_out_pcm, g_turns[i].out_start, OUT_PEAK_COLUMN)
    g_unplayed_clips |> emplace(a)
}

def poll_answers() {
    g_answer |> try_pop() $(bytes) {
        var a : Answer
        mem_archive_load(bytes, a)
        if (a.kind == AnswerKind.ready) {
            on_ready(a)
        } elif (a.kind == AnswerKind.heard || a.kind == AnswerKind.translated) {
            on_text(a, a.kind == AnswerKind.heard)
        } elif (a.kind == AnswerKind.clip) {
            on_clip(a)
        } else {
            on_worker_error(a)
        }
    }
    poll_playback()
}

//! the worker has exited: no answer reaches a turn in flight, so each ends as empty; the listen button closes, an open take is dropped
def on_worker_error(a : Answer) {
    g_error = clone_string(a.text)
    to_log(LOG_ERROR, "interpreter: {a.text}\n")
    g_models_ready = false
    for (row in g_turns) {
        if (turn_in_flight(row.state)) {
            row.state = TurnState.empty
        }
    }
    return if (!g_listening)
    sound_record_stop()
    g_listening = false
    flush_take()
}

def release_play_status() {
    return if (g_play_status == null)
    unset_status_update(g_playing_sid)
    g_play_status |> seq_box_release()
    g_play_status = null
}

[arch(at = "../ARCHITECTURE.md#interpreter-turns")]
def poll_playback() {
    if (g_playing_sid != INVALID_SID) {
        var done = false
        g_play_status |> read() $(s : AudioChannelStatus) {
            g_playhead = g_play_start + int64(s.playback_position)
            done = s.state == AudioChannelState.stopped
        }
        if (!done && g_elapsed_s < g_speaking_until) {
            return
        }
        release_play_status()
        g_playing_sid = INVALID_SID
        let i = turn_index(g_turns, g_playing_turn)
        if (i >= 0) {
            g_turns[i].state = TurnState.spoken
            g_playhead = g_turns[i].out_start + g_turns[i].clip_samples
            to_log(LOG_INFO, "interpreter: spoke {g_turns[i].no} - {tenths(float(g_turns[i].clip_samples) / float(max(g_tts_rate, 1)))} s of speech, synthesized in {ms(g_turns[i].tts_us)} ms\n")
        }
        g_playing_turn = -1
        if (empty(g_unplayed_clips)) {
            flush_take()
        }
    }
    if (!empty(g_unplayed_clips)) {
        let seconds = float(length(g_unplayed_clips[0].pcm)) / float(max(g_unplayed_clips[0].rate, 1))
        var pcm <- g_unplayed_clips[0].pcm
        g_playing_turn = g_unplayed_clips[0].turn
        let i = turn_index(g_turns, g_playing_turn)
        g_play_start = i >= 0 ? g_turns[i].out_start : 0l
        g_playing_sid = play_sound_from_pcm(g_unplayed_clips[0].rate, 1, pcm)
        g_play_status = seq_box_create()
        set_status_update(g_playing_sid, g_play_status)
        g_unplayed_clips |> erase(0)
        g_speaking_until = g_elapsed_s + seconds + 0.5   // the mixer's stopped state is the clock; this is the guard behind it
    }
    log_done_turns()
}

//! the done line of every turn that has ended since the last frame, in order
def log_done_turns() {
    while (g_logged_done_turns < length(g_turns) && !turn_in_flight(g_turns[g_logged_done_turns].state)) {
        let spoken = g_turns[g_logged_done_turns].state == TurnState.spoken
        to_log(LOG_INFO, "interpreter: turn {g_turns[g_logged_done_turns].no} is {spoken ? "done" : "empty"}\n")
        g_logged_done_turns++
    }
}

def reply_pending() : bool => g_playing_sid != INVALID_SID || !empty(g_unplayed_clips)

[arch(at = "../ARCHITECTURE.md#interpreter-turns")]
def start_listen() {
    return if (g_listening)
    if (!g_models_ready) {
        g_status = "the models are still loading"
        return
    }
    if (!sound_record_start(MIC_RATE, 1, MIC_RATE * 4, -1)) {
        g_status = "no microphone - is one connected, and allowed?"
        return
    }
    g_mic_frames |> resize(MIC_RATE)
    flush_take()
    g_last_frames_s = g_elapsed_s
    g_frames_seen = false
    g_level = 0.0
    g_level_peak = 0.0
    g_listening = true
    g_status = "listening - talk in Russian, pause, and hear it in English"
    to_log(LOG_INFO, "interpreter: listening\n")
}

def stop_listen() {
    return if (!g_listening)
    drain_ring_tail()   // before the device stops: stopping frees the ring
    if (g_in_speech) {
        cut_utterance(g_speech_start, long_length(g_take), long_length(g_take))
    }
    sound_record_stop()
    let dropped = sound_record_overflow_frames()
    if (dropped > 0l) {
        to_log(LOG_WARNING, "interpreter: {dropped} frames were dropped while the frame loop stalled - the take is spliced there\n")
    }
    g_listening = false
    g_status = turns_in_flight(g_turns) > 0 ? "stopped listening - the last turns are on their way" : "stopped - press listen to go on"
    to_log(LOG_INFO, "interpreter: stopped listening\n")
}

def flush_take() {
    g_raw |> clear()
    g_mic_resampler.pos = 0.0lf
    g_take |> clear()
    g_take_peaks |> clear()
    vad_iter_reset(g_vad, default_vad_opts())
    g_in_speech = false
    g_speech_start = 0l
}

def append_frames(n : int) {
    let rms = append_gained(g_raw, g_mic_frames, n, GAIN.value)
    g_level = max(rms, g_level * 0.85)
    g_level_peak = max(g_level_peak, rms)
    g_frames_seen = true
    g_last_frames_s = g_elapsed_s
    let base16 = long_length(g_take)
    resample_step(g_mic_resampler, g_raw, g_resampled)
    g_take |> push_from(g_resampled)
    peaks_append(g_take_peaks, g_take, base16, PEAK_COLUMN)
}

[arch(at = "../ARCHITECTURE.md#interpreter-turns")]
def drain_listen() {
    let n = sound_record_read(g_mic_frames)
    if (n > 0 && reply_pending()) {
        g_frames_seen = true
        g_last_frames_s = g_elapsed_s
    } elif (n > 0) {
        take_frames(n)
    }
    if (g_elapsed_s - g_last_frames_s >= NO_FRAMES_ENDS_LISTEN_S) {
        stop_listen()
        g_status = (!g_frames_seen
            ? "nothing heard - the microphone gave no audio at all: it was refused, or there is none"
            : "nothing heard for {tenths(NO_FRAMES_ENDS_LISTEN_S)} s - the microphone stopped delivering")
    }
}

//! `n` device frames onto the take, the new samples through the detector, and the cut its boundaries name
def take_frames(n : int) {
    let base = long_length(g_take)
    append_frames(n)
    var inscope added_16k <- [for (i in range64(base, long_length(g_take))); g_take[i]]
    var span = SpeechSpan(open = g_in_speech, start = g_speech_start)
    vad_iter_feed(g_vad_model, g_vad, added_16k) $(ev : VadEvent) {
        span = speech_mark(span, ev.kind == VadEventKind.speech_start, ev.sample, long_length(g_take))
    }
    let total = long_length(g_take)
    let cut = listen_cut(span, total)
    if (cut == ListenCut.at_end) {
        cut_utterance(span.start, span.end, span.end)
        span = span_after_cut(span, span.end)
    } elif (cut == ListenCut.at_cap) {
        cut_utterance(span.start, total, total)
        span = span_after_cut(span, total)
    } elif (cut == ListenCut.idle) {
        flush_take()
    }
    g_in_speech = span.open
    g_speech_start = span.start
}

def drain_ring_tail() {
    for (_i in range(8)) {
        let n = sound_record_read(g_mic_frames)
        break if (n <= 0)
        take_frames(n)
    }
}

//! the utterance [start, end) of the take, padded, to the worker thread as a turn; the take keeps its samples from `keep_from` on
def cut_utterance(start, end, keep_from : int64) {
    var clip <- utterance_clip(g_take, start, end)
    queue_turn(clip)
    keep_take_from(keep_from)
}

//! the take from `from` on, its waveform rebuilt; the detector's state runs on, the raw frames and the resampler start over
def keep_take_from(from : int64) {
    g_raw |> clear()
    g_mic_resampler.pos = 0.0lf
    var inscope rest <- [for (i in range64(from, long_length(g_take))); g_take[i]]
    g_take |> clear()
    g_take |> push_from(rest)
    g_take_peaks |> clear()
    peaks_append(g_take_peaks, g_take, 0l, PEAK_COLUMN)
}

def queue_turn(var clip : array<float>) {
    g_turn_no++
    let seconds = float(length(clip)) / float(LISTEN_RATE)
    g_turns |> push(TurnRow(no = g_turn_no, state = TurnState.transcribing, utterance_s = seconds))
    to_log(LOG_INFO, "interpreter: utterance {g_turn_no} - {tenths(seconds)} s\n")
    var a = Ask(kind = AskKind.hear, turn = g_turn_no, pcm <- clip)
    g_ask |> push_archive(a)
}

def toggle_listen() {
    if (g_listening) {
        stop_listen()
    } else {
        start_listen()
    }
}

def newest_in_flight() : int {
    var found = -1
    for (t, i in g_turns, count()) {
        if (turn_in_flight(t.state)) {
            found = i
        }
    }
    return found
}

def phase_chip() : tuple<text : string; color : uint> {
    if (!g_models_ready) {
        return (text = empty(g_error) ? "loading the models" : "stopped on an error", color = empty(g_error) ? DIM : RED)
    }
    if (g_playing_sid != INVALID_SID) {
        return (text = "speaking", color = GREEN)
    }
    let i = newest_in_flight()
    if (i >= 0) {
        let st = g_turns[i].state
        return (text = st == TurnState.transcribing ? "transcribing" : (st == TurnState.translating ? "translating" : "synthesizing"), color = ACCENT)
    }
    if (g_listening) {
        return (text = g_in_speech ? "speech" : "listening", color = g_in_speech ? RED : INK)
    }
    return (text = "ready", color = GREEN)
}

def busy() : bool => g_listening || g_playing_sid != INVALID_SID || turns_in_flight(g_turns) > 0

def draw_status_bar() {
    draw_status_head("What you say stays in this window: the Russian is heard, translated and spoken here, and nothing is uploaded.", phase_chip(), busy(), g_elapsed_s)
    with_style((ImGuiCol.Text, empty(g_error) ? (g_listening ? DIM : ACCENT) : RED)) {
        text_wrapped(STATUS_LINE, (text = empty(g_error) ? g_status : g_error))
    }
}

def draw_listen_button() {
    let size = float2(96.0, 96.0)
    let origin = GetCursorScreenPos()
    let clicked = invisible_button(LISTEN_BTN, (text = "listen", size = size))
    let hovered = IsItemHovered()
    let center = float2(origin.x + size.x * 0.5, origin.y + size.y * 0.5)
    with_window_drawlist() $(var dl) {
        dl |> add_circle(center, 44.0, hovered ? INK : DIM, 0, 2.0)
        if (g_listening) {
            dl |> add_rect_filled(float2(center.x - 18.0, center.y - 18.0), float2(center.x + 18.0, center.y + 18.0), reply_pending() ? DIM : RED, 4.0)
            let breath = clamp(g_level * 8.0, 0.0, 1.0)
            if (breath > 0.0 && !reply_pending()) {
                dl |> add_circle(center, 30.0 + 14.0 * breath, with_alpha(RED, 0.6), 0, 2.0)
            }
        } else {
            dl |> add_circle_filled(center, 30.0, g_models_ready ? RED : DIM, 0)
        }
    }
    if (clicked) {
        toggle_listen()
    }
}

def draw_listen_panel() {
    draw_listen_button()
    same_line((spacing = 16.0))
    group(LISTEN_SIDE) {
        text(LISTEN_LABEL, (text = g_listening ? "stop" : "listen"))
        text(LISTEN_HINT, (text = "Говорите по-русски"))
        let meter = clamp(g_level * 8.0, 0.0, 1.0)
        progress_bar(LEVEL, (fraction = meter, size = float2(160.0, 0.0), overlay = ""))
        GAIN.bounds = (0.25, 4.0)
        with_item_width(160.0) {
            slider_float(GAIN, (text = "gain", format = "%.2f"))
        }
        text(LISTEN_STATE, (text = !g_listening ? "not listening" : (reply_pending() ? "muted while the reply plays" : (g_in_speech ? "speech" : "listening"))))
    }
    spacing()
    let no_ticks : array<int64>
    draw_wave(g_take_peaks, float2(GetContentRegionAvail().x, 72.0), PEAK_COLUMN, 10l * int64(LISTEN_RATE), 0l, 0l, 0l, no_ticks)
    spacing()
    with_style((ImGuiCol.Text, DIM)) {
        text_wrapped(MODELS_LINE, (text = g_models_ready ? "{g_args.asr_model} ({g_lane})  ->  {g_direction}  ->  {g_args.tts_model} ({g_voice})" : "loading {g_args.asr_model}, the translator and {g_args.tts_model}..."))
        if (!imgui_touch_device) {
            text("space listens and stops")
        }
    }
}

def state_words(t : TurnRow) : string {
    if (t.state == TurnState.transcribing) {
        return "transcribing..."
    }
    if (t.state == TurnState.translating) {
        return "translating..."
    }
    if (t.state == TurnState.empty) {
        return empty(t.ru) ? "(nothing heard)" : "(nothing to say)"
    }
    return t.en
}

def draw_turn(t : TurnRow) {
    with_style((ImGuiCol.Text, INK)) {
        text_wrapped(TURN_RU[t.no], (text = "{t.no}.  {empty(t.ru) && t.state == TurnState.transcribing ? "..." : t.ru}"))
    }
    with_style((ImGuiCol.Text, t.state == TurnState.speaking ? ACCENT : (t.state == TurnState.spoken ? GREEN : DIM))) {
        text_wrapped(TURN_EN[t.no], (text = "     {state_words(t)}"))
    }
    with_style((ImGuiCol.Text, DIM)) {
        var times = "     heard {tenths(t.utterance_s)} s"
        if (t.asr_us > 0l) {
            times += "   asr {ms(t.asr_us)} ms"
        }
        if (t.nmt_us > 0l) {
            times += "   nmt {ms(t.nmt_us)} ms"
        }
        if (t.tts_us > 0l) {
            times += "   tts {ms(t.tts_us)} ms   spoke {tenths(float(t.clip_samples) / float(max(g_tts_rate, 1)))} s"
        }
        text(TURN_TIMES[t.no], (text = times))
        text(TURN_GAP[t.no], (text = ""))   // the gap between turns: a keyed line, since a bare spacing() may render once a frame
    }
}

def draw_history_panel() {
    child(HISTORY_VIEW, (text = "history", size = float2(0.0, 0.0), child_flags = ImGuiChildFlags.Borders, window_flags = ImGuiWindowFlags.None)) {
        if (empty(g_turns)) {
            with_style((ImGuiCol.Text, DIM)) {
                text_wrapped(HISTORY_EMPTY, (text = "nothing heard yet - press listen and talk"))
            }
        }
        for (t in g_turns) {
            draw_turn(t)
        }
        if (GetScrollY() >= GetScrollMaxY() - 40.0) {
            SetScrollHereY(1.0)
        }
    }
}

def draw_output_panel() {
    let ticks <- [for (t in g_turns); t.out_start; where t.out_start > 0l]
    draw_wave(g_out_peaks, float2(GetContentRegionAvail().x, 72.0), OUT_PEAK_COLUMN, 30l * int64(g_tts_rate), 0l, 0l, g_playhead, ticks)
    let seconds = double(long_length(g_out_pcm)) / double(max(g_tts_rate, 1))
    text(OUT_LINE, (text = "{tenths(float(seconds))} s spoken over {length(g_turns)} turns   playhead {tenths(float(double(g_playhead) / double(max(g_tts_rate, 1))))} s"))
}

def setup_narrow_dock_layout(dock_id, status_id, rest_id : uint) {
    var listen_id : uint = 0u
    var history_id : uint = 0u
    DockBuilderSplitNode(rest_id, ImGuiDir.Up, 0.42, listen_id, history_id)
    DockBuilderDockWindow("interpreter", status_id)
    DockBuilderDockWindow("listen", listen_id)
    DockBuilderDockWindow("output", history_id)
    DockBuilderDockWindow("history", history_id)
    DockBuilderFinish(dock_id)
}

var g_front_tab_frames = 0   //! frames left of pulling the column layout's front tab forward - a node opens on its first tab bar's own pick

def setup_dock_layout(dock_id : uint) {
    DockBuilderRemoveNode(dock_id)
    DockBuilderAddDockSpaceNode(dock_id, ImGuiDockNodeFlags.None)
    let vp = GetMainViewport()
    DockBuilderSetNodeSize(dock_id, ImVec2(vp.Size.x, vp.Size.y))
    var status_id : uint = 0u
    var rest_id : uint = 0u
    let narrow = vp.Size.x < 760.0 || vp.Size.x < vp.Size.y
    DockBuilderSplitNode(dock_id, ImGuiDir.Up, narrow ? 0.24 : 0.12, status_id, rest_id)
    if (narrow) {
        setup_narrow_dock_layout(dock_id, status_id, rest_id)
        g_front_tab_frames = 1
        return
    }
    var listen_id : uint = 0u
    var right_id : uint = 0u
    DockBuilderSplitNode(rest_id, ImGuiDir.Left, 0.3, listen_id, right_id)
    var history_id : uint = 0u
    var output_id : uint = 0u
    DockBuilderSplitNode(right_id, ImGuiDir.Up, 0.7, history_id, output_id)
    DockBuilderDockWindow("interpreter", status_id)
    DockBuilderDockWindow("listen", listen_id)
    DockBuilderDockWindow("history", history_id)
    DockBuilderDockWindow("output", output_id)
    DockBuilderFinish(dock_id)
}

def draw_screen() {
    dockspace(DOCK_ROOT, (flags = ImGuiDockNodeFlags.None)) {
        if (!DOCK_ROOT.has_initial_layout && DOCK_ROOT.dock_id != 0u) {
            setup_dock_layout(DOCK_ROOT.dock_id)
            DOCK_ROOT.has_initial_layout = true
        }
        dock_window(STATUS_WIN, (text = "interpreter", closable = false, flags = ImGuiWindowFlags.NoScrollbar)) {
            draw_status_bar()
        }
        dock_window(LISTEN_WIN, (text = "listen", closable = false, flags = ImGuiWindowFlags.None)) {
            draw_listen_panel()
        }
        dock_window(OUTPUT_WIN, (text = "output", closable = false, flags = ImGuiWindowFlags.None)) {
            draw_output_panel()
        }
        dock_window(HISTORY_WIN, (text = "history", closable = false, flags = ImGuiWindowFlags.None)) {
            draw_history_panel()
        }
        if (g_front_tab_frames > 0) {
            SetWindowFocus("history")
            g_front_tab_frames--
        }
    }
}

def poll_chords() {
    if (IsKeyPressed(ImGuiKey.Space, false)) {
        toggle_listen()
    }
}

[export]
def init() {
    var inscope r <- parse_args(type<InterpreterArgs>)
    if (r |> is_err) {
        panic("interpreter: {r |> unwrap_err}")
    }
    g_args <- r |> move_unwrap
    GAIN.value = 1.0
    g_status = "loading the models..."
    if (!empty(g_args.clip)) {
        // decoded before the window and the thread exist, so a clip that is not audio refuses here and the program ends
        delete g_pending_clip
        g_pending_clip <- load_audio_mono(g_args.clip, LISTEN_RATE)
        if (empty(g_pending_clip)) {
            panic("interpreter: {g_args.clip} did not decode")
        }
        let cap = int(UTTERANCE_CAP_S * float(LISTEN_RATE))
        if (length(g_pending_clip) > cap) {
            to_log(LOG_INFO, "interpreter: {g_args.clip} is longer than the {int(UTTERANCE_CAP_S)} s an utterance may run - the first {int(UTTERANCE_CAP_S)} s are heard\n")
            g_pending_clip |> resize(cap)
        }
        to_log(LOG_INFO, "interpreter: hearing {g_args.clip}\n")
        g_status = "hearing {g_args.clip}..."
        peaks_append(g_take_peaks, g_pending_clip, 0l, PEAK_COLUMN)   // the clip stands in for the take on the listen panel
    }
    harness_init("interpreter", 1280, 760)
    var io & = unsafe(GetIO())
    io.ConfigFlags |= ImGuiConfigFlags.DockingEnable   // the panels are dock windows: drag a split or a tab to lay them out
    if (!g_audio_initialized) {
        g_asch = audio_system_create()
        g_audio_initialized = true
    }
    create_job_que()
    setup_dasllama_jobque()
    delete g_vad_model
    g_vad_model <- load_vad_model(empty(g_args.vad_model) ? "{get_das_root()}/modules/dasLLAMA/models/silero_vad.bin" : path_join(g_args.models, g_args.vad_model))
    vad_iter_reset(g_vad, default_vad_opts())
    g_ask = unsafe(stream_create())
    g_ask |> append(1)
    g_answer = unsafe(stream_create())
    g_answer |> append(1)
    g_worker_done = unsafe(channel_create())
    g_worker_done |> append(1)
    var setup = Ask(kind = AskKind.setup, asr = path_join(g_args.models, g_args.asr_model), nmt = path_join(g_args.models, g_args.nmt_model),
        tts = path_join(g_args.models, g_args.tts_model), pin_q8 = g_args.q8)
    g_ask |> push_archive(setup)
    g_quitting = atomic32_create()
    start_worker_thread(g_ask, g_answer, g_worker_done, g_quitting)
}

[export]
def update() {
    if (!harness_begin_frame()) {
        return
    }
    g_frames++
    g_elapsed_s += get_dt()
    harness_new_frame()
    poll_chords()
    if (g_listening) {
        drain_listen()
    }
    poll_answers()
    draw_screen()
    harness_end_frame()
}

[export]
def shutdown() {
    if (g_listening) {
        sound_record_stop()
        g_listening = false
    }
    if (g_playing_sid != INVALID_SID) {
        release_play_status()
        stop(g_playing_sid, 0.05)
        g_playing_sid = INVALID_SID
    }
    g_quitting |> set(1)
    var stop_ask = Ask(kind = AskKind.stop)
    g_ask |> push_archive(stop_ask)
    g_worker_done |> join()
    unsafe {
        atomic32_remove(g_quitting)
        channel_remove(g_worker_done)
        stream_remove(g_ask)
        stream_remove(g_answer)
    }
    if (g_audio_initialized) {
        audio_system_finalize(g_asch.command, g_asch.next_sid)
        g_audio_initialized = false
    }
    delete g_unplayed_clips
    delete g_vad_model
    destroy_job_que()
    harness_shutdown()
}

def all_turns_done() : bool => !empty(g_turns) && turns_in_flight(g_turns) == 0 && empty(g_unplayed_clips) && g_playing_sid == INVALID_SID

def done_for_smoke() : bool {
    return true if (g_args.max_frames > 0 && g_frames >= g_args.max_frames)
    return g_args.smoke && (all_turns_done() || !empty(g_error))
}

[export, unsafe_heap_collect, arch(at = "../ARCHITECTURE.md#one-source-two-builds")]
def main() {
    init()
    eval_main_loop() {
        update()
        harness_maybe_collect_gc()
        return false if (done_for_smoke())
        return !exit_requested()
    }
    let finished = all_turns_done()
    shutdown()
    if (g_args.smoke && !empty(g_error)) {
        panic("interpreter: the smoke run ended on an error - {g_error}")
    }
    if (g_args.smoke) {
        to_log(LOG_INFO, finished
            ? "interpreter: the clip was interpreted after {g_frames} frames\n"
            : "interpreter: the frame cap stopped the run after {g_frames} frames\n")
    }
}
