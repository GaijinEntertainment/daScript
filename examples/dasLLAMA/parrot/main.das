options gen2
options persistent_heap
options gc
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dasllama/dasllama   // the facade: the TTS model, its caps, the clone verb, the chunker, the synthesis, and Silero
require take                // the pure side: the take's numbers, the waveform columns, the chunk ledger
require imgui/imgui_harness // the window, the frame, the boost widgets and the draw list
require imgui/imgui_table_builtin
require daslib/jobque_boost
require daslib/strings_boost
require daslib/clargs
require daslib/fio
require daslib/archive
require audio/audio_boost
require audio/audio_record
require math
require strings

// Parrot: press record and talk; Silero VAD ends the take when you go quiet; Pocket TTS clones
// the voice from the take, and the text in the editor is read aloud in it. The lab panel shows
// what the engine is doing while it does it - the take as it records, the chunks as they are
// generated and spoken, the time each stage took, the job queue's knobs - and the same program
// runs on the desktop and, through daspkg release wasm, in the browser.

[CommandLineArgs]
struct ParrotArgs {
    @clarg_doc = "directory holding the models"
    models : string = "{get_das_root()}/../llama.cpp/models"
    @clarg_doc = "the Pocket TTS GGUF under --models (a file with its codec encoder, so it clones)"
    tts_model : string = "pocket-tts-en-kq.gguf"
    @clarg_doc = "the Silero VAD weights under --models (empty: the tree's own copy)"
    vad_model : string = ""
    @clarg_doc = "a clip to clone from instead of the microphone (wav, flac, mp3, ogg)"
    clip : string = ""
    @clarg_doc = "the text to say (empty: the poem)"
    text : string = ""
    @clarg_doc = "the smoke rail: clone --clip, say the text, exit once it is read out"
    smoke : bool = false
    @clarg_doc = "stop after this many frames (0: never)"
    max_frames : int = 0
}

enum Phase {
    idle
    recording
    cloning
}

let POEM = "Whose woods these are I think I know.\nHis house is in the village though;\nHe will not see me stopping here\nTo watch his woods fill up with snow.\n\nMy little horse must think it queer\nTo stop without a farmhouse near\nBetween the woods and frozen lake\nThe darkest evening of the year.\n\nHe gives his harness bells a shake\nTo ask if there is some mistake.\nThe only other sound's the sweep\nOf easy wind and downy flake.\n\nThe woods are lovely, dark and deep,\nBut I have promises to keep,\nAnd miles to go before I sleep,\nAnd miles to go before I sleep."

let VOICE_NAME = "you"
let READ_OUT_STATUS = "read out - change the text and say again, or record a new voice"
let PEAK_COLUMN = 240   //! samples per waveform column: 10 ms at the model's rate
let MEASURE_RUNS = 3    //! the measure button says the text this many times, silently

//! the frame thread's requests to the speech thread, as archived records
struct Ask {
    kind : int        //! 0 the model path; 1 clone the pcm; 2 say the text in `voice`; 3 stop
    text : string
    voice : string
    gen : int         //! the say this belongs to
    silent : bool     //! a say whose clips are timed, never played (the measure button)
    rate : int
    pcm : array<float>
}

//! the speech thread's answers: 0 ready (rate, cloning in gen, the roster in text); 1 cloned
//! (seconds in text, the wall time in us); 4 a chunk's text before its synthesis; 2 a chunk's
//! clip with its stage times; 3 the say's chunk count, sent before the first chunk
struct Answer {
    kind : int
    text : string
    gen : int
    chunk : int
    rate : int
    pcm : array<float>
    voices : array<string>
    us_prompt : int64
    us_backbone : int64
    us_head : int64
    us_codec : int64
    us_total : int64
}

// the per-chunk rows of the script view, the table and the measure list: indexed widgets take their state from a table
var private CHUNK_LINE : table<int; NarrativeState>
var private CHUNK_TEXT : table<int; NarrativeState>
var private CHUNK_STATE : table<int; NarrativeState>
var private CHUNK_AUDIO : table<int; NarrativeState>
var private CHUNK_PROMPT : table<int; NarrativeState>
var private CHUNK_BACKBONE : table<int; NarrativeState>
var private CHUNK_HEAD : table<int; NarrativeState>
var private CHUNK_CODEC : table<int; NarrativeState>
var private MEASURE_ROW : table<int; NarrativeState>

var g_args = ParrotArgs()
var g_phase = Phase.idle
var g_status = ""
var g_voice_ready = false
var g_voice_seconds = 0.0
var g_clone_ms = 0.0lf
var g_says = 0
var g_say_silent = false
var g_measure_left = 0
var g_measure_rtf : array<double>
var g_chunks : array<ChunkRow>       //! the say in flight
var g_chunks_known = false           //! the count answer arrived
var g_read_out_say = 0               //! the last say whose read-out was logged, so the line is written once
var g_take : array<float>            //! the microphone as recorded, 24 kHz mono, the gain applied
var g_take_scratch : array<float>
var g_take_peaks : array<float2>     //! min and max per column of the take
var g_take_window : tuple<from : int64; to : int64>   //! the kept speech once the take ended
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
var g_last_frames_s = 0.0            //! when the device last delivered a frame; a device that never does ends the take
var g_frames_seen = false
var g_level = 0.0
var g_level_peak = 0.0
var g_ready = false                  //! the speech thread has the model: its rate, whether it clones, its roster
var g_mic_rate = MIC_RATE
var g_can_clone = false
var g_roster : array<string>
var g_voice_names : array<string>    //! the picker: "you" once cloned, then the roster
var g_pending_clip : array<float>    //! a --clip decoded before the model was ready

var g_ask : Stream?
var g_answer : Stream?
var g_say_now : SeqBox?
var g_speech_done : Channel?
var g_clips : array<Answer>          //! generated, not yet played
var g_out_pcm : array<float>         //! the say's audio so far, every chunk appended
var g_out_peaks : array<float2>
var g_playing_sid = INVALID_SID
var g_playing_chunk = -1
var g_play_status : SeqBox?
var g_play_start = 0l                //! the playing chunk's first sample in g_out_pcm
var g_playhead = 0l
var g_elapsed_s = 0.0
var g_speaking_until = 0.0
var g_audio_initialized = false
var g_asch : AudioSystemChannels
var g_frames = 0
var g_jobs_max = 1
var g_jobs_applied = -1
var g_team_applied = true
var g_spin_applied = true

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
        var ready = Answer(kind = 0, text = "{length(c.voices)}", gen = c.cloning ? 1 : 0, rate = c.sample_rate, voices := c.voices)
        answer |> push_archive(ready)
        var running = true
        while (running) {
            ask |> pop_archive() $(var a : Ask&) {
                if (a.kind == 3) {
                    running = false
                } elif (a.kind == 1) {
                    let t0 = ref_time_ticks()
                    tts_register_voice(m, VOICE_NAME, a.pcm, a.rate)
                    var cloned = Answer(kind = 1, text = "{float(length(a.pcm)) / float(a.rate)}", gen = a.gen, rate = a.rate, us_total = int64(get_time_usec(t0)))
                    answer |> push_archive(cloned)
                } elif (a.kind == 2) {
                    var inscope chunks <- tts_chunks(m, a.text)
                    var count = Answer(kind = 3, text = "{length(chunks)}", gen = a.gen)
                    answer |> push_archive(count)
                    for (i, chunk in range(length(chunks)), chunks) {
                        var current = a.gen
                        now |> read() $(gen : int) {
                            current = gen
                        }
                        break if (a.gen < current)   // a say the user replaced: its clips would be dropped unheard
                        var starting = Answer(kind = 4, text = chunk, gen = a.gen, chunk = i)
                        answer |> push_archive(starting)
                        var inscope s <- synthesize(m, chunk, a.voice)
                        var clip = Answer(kind = 2, gen = a.gen, chunk = i, pcm := s.pcm, rate = s.sample_rate,
                            us_prompt = s.timings.prompt_us, us_backbone = s.timings.backbone_us,
                            us_head = s.timings.head_us, us_codec = s.timings.codec_us, us_total = s.timings.total_us)
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

def rebuild_voice_names() {
    delete g_voice_names
    if (g_voice_ready) {
        g_voice_names |> push(VOICE_NAME)
    }
    g_voice_names |> push_clone_from(g_roster)
}

def on_ready(var a : Answer) {
    g_ready = true
    g_mic_rate = a.rate
    g_can_clone = a.gen != 0
    delete g_roster
    g_roster <- a.voices
    rebuild_voice_names()
    VOICE.value = 0
    g_jobs_max = max(get_total_hw_cores() - 1, 1)   // the engine's own cap: one core stays with the caller
    JOBS.bounds = (1, g_jobs_max)
    JOBS.value = clamp(get_jobque_worker_limit(), 1, g_jobs_max)   // the limit reads as a huge number when unset
    TEAM.value = get_jobque_team_mode()
    SPIN.value = true
    g_jobs_applied = JOBS.value
    g_team_applied = TEAM.value
    g_spin_applied = SPIN.value
    g_status = g_can_clone ? "press record and talk, then say" : "this file cannot clone - parrot needs a Pocket file with its codec encoder; say reads in the model's voice"
    if (!empty(g_pending_clip)) {
        if (!g_can_clone) {
            panic("parrot: --clip needs a file that clones; {g_args.tts_model} carries no codec encoder")
        }
        clone_clip(g_pending_clip, g_mic_rate)
    }
}

def on_cloned(a : Answer) {
    g_voice_ready = true
    g_voice_seconds = to_float(a.text)
    g_clone_ms = double(a.us_total) / 1000.0lf
    rebuild_voice_names()
    VOICE.value = 0
    g_phase = Phase.idle
    g_status = "your voice is ready - press say"
    //! the smoke rail's witness lines, here and below: modules/dasLLAMA/tests/test_parrot.das reads them word for word
    to_log(LOG_INFO, "parrot: cloned {a.text} s of speech in {g_clone_ms} ms\n")
    if (g_args.smoke) {
        say_text()
    }
}

def on_chunk_starting(a : Answer) {
    while (length(g_chunks) <= a.chunk) {
        g_chunks |> push(ChunkRow())
    }
    g_chunks[a.chunk].text = clone_string(a.text)   // the record's bytes are the archive's: copied into this heap
    g_chunks[a.chunk].state = ChunkState.generating
}

def on_clip(var a : Answer) {
    while (length(g_chunks) <= a.chunk) {
        g_chunks |> push(ChunkRow())
    }
    let i = a.chunk
    g_chunks[i].state = ChunkState.generated
    g_chunks[i].samples = long_length(a.pcm)
    g_chunks[i].us_prompt = a.us_prompt
    g_chunks[i].us_backbone = a.us_backbone
    g_chunks[i].us_head = a.us_head
    g_chunks[i].us_codec = a.us_codec
    g_chunks[i].us_total = a.us_total
    if (g_say_silent) {
        g_chunks[i].state = ChunkState.spoken
        return
    }
    g_chunks[i].start = long_length(g_out_pcm)
    g_out_pcm |> push_from(a.pcm)
    peaks_append(g_out_peaks, g_out_pcm, g_chunks[i].start, PEAK_COLUMN)
    g_clips |> emplace(a)
}

//! the whole say's real-time factor over its chunks: generation seconds per second of audio
def say_rtf() : double {
    var us = 0l
    var samples = 0l
    for (row in g_chunks) {
        us += row.us_total
        samples += row.samples
    }
    return samples <= 0l ? 0.0lf : double(us) / 1e6lf / (double(samples) / double(max(g_mic_rate, 1)))
}

def on_read_out() {
    g_read_out_say = g_says
    if (g_say_silent) {
        g_measure_rtf |> push(say_rtf())
        to_log(LOG_INFO, "parrot: measure {length(g_measure_rtf)} of {MEASURE_RUNS}: rtf {say_rtf()}\n")
        if (g_measure_left > 0) {
            g_measure_left--
            say_silent()
            return
        }
        var sum = 0.0lf
        for (r in g_measure_rtf) {
            sum += r
        }
        g_status = "measured: rtf {sum / double(length(g_measure_rtf))} over {length(g_measure_rtf)} runs at {JOBS.value} jobs"
        return
    }
    if (g_phase == Phase.idle) {
        g_status = READ_OUT_STATUS
    }
    to_log(LOG_INFO, "parrot: say {g_says} is read out\n")
}

def poll_answers() {
    g_answer |> try_pop() $(bytes) {
        var a : Answer
        mem_archive_load(bytes, a)
        if (a.kind == 0) {
            on_ready(a)
        } elif (a.kind == 1) {
            on_cloned(a)
        } elif (a.gen == g_says) {
            if (a.kind == 3) {
                g_chunks_known = true
                while (length(g_chunks) < to_int(a.text)) {
                    g_chunks |> push(ChunkRow())
                }
            } elif (a.kind == 4) {
                on_chunk_starting(a)
            } else {
                on_clip(a)
            }
        }
    }
    poll_playback()
}

//! the playing clip's position, off the mixer's own clock, and the next clip once it is done
[arch(at = "../ARCHITECTURE.md#parrot-panels")]
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
        if (g_playing_chunk >= 0 && g_playing_chunk < length(g_chunks)) {
            g_chunks[g_playing_chunk].state = ChunkState.spoken
            g_playhead = g_chunks[g_playing_chunk].start + g_chunks[g_playing_chunk].samples
        }
        g_playing_chunk = -1
    }
    if (!empty(g_clips)) {
        let seconds = float(length(g_clips[0].pcm)) / float(max(g_clips[0].rate, 1))
        var pcm <- g_clips[0].pcm
        g_playing_chunk = g_clips[0].chunk
        g_play_start = g_chunks[g_playing_chunk].start
        g_chunks[g_playing_chunk].state = ChunkState.speaking
        g_playing_sid = play_sound_from_pcm(g_clips[0].rate, 1, pcm)
        g_play_status = seq_box_create()   //! one box per clip: a box shared across clips reads the last clip's stop as this one's
        set_status_update(g_playing_sid, g_play_status)
        g_clips |> erase(0)
        g_speaking_until = g_elapsed_s + seconds + 0.5   //! the mixer's stopped state is the clock; this is the guard behind it
    } elif (g_chunks_known && g_says > g_read_out_say && all_spoken(g_chunks)) {
        on_read_out()
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
    g_take_peaks |> clear()
    g_take_window = (from = 0l, to = 0l)
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

//! the frames the device has since delivered, onto the take, through the gain slider
def append_frames(n : int) {
    var ssq = 0.0
    let base = length(g_take)
    let gain = GAIN.value
    g_take |> ensure_capacity(base + n)
    g_take |> resize(base + n)
    for (i in range(n)) {
        let v = clamp(g_take_scratch[i] * gain, -1.0, 1.0)
        g_take[base + i] = v
        ssq += v * v
    }
    let rms = sqrt(ssq / float(n))
    g_level = max(rms, g_level * 0.85)
    g_level_peak = max(g_level_peak, rms)
    g_frames_seen = true
    g_last_frames_s = g_elapsed_s
    peaks_append(g_take_peaks, g_take, int64(base), PEAK_COLUMN)
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
    g_take_window = w
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
//! the playing clip's status box, unhooked from the mixer and dropped
def release_play_status() {
    return if (g_play_status == null)
    unset_status_update(g_playing_sid)
    g_play_status |> seq_box_release()
    g_play_status = null
}

def cancel_say() {
    if (g_playing_sid != INVALID_SID) {
        release_play_status()
        stop(g_playing_sid, 0.05)
        g_playing_sid = INVALID_SID
        g_playing_chunk = -1
    }
    g_says++
    g_say_now |> publish(g_says)
    g_clips |> clear()
    g_chunks |> clear()
    g_chunks_known = false
    g_out_pcm |> clear()
    g_out_peaks |> clear()
    g_playhead = 0l
    g_speaking_until = g_elapsed_s
    g_measure_left = 0
}

// ===== saying =====

def chosen_voice() : string {
    return "" if (empty(g_voice_names))
    return g_voice_names[clamp(VOICE.value, 0, length(g_voice_names) - 1)]
}

def say_text() {
    let text = strip(SCRIPT.value)
    return if (empty(text) || g_phase != Phase.idle || !g_ready)   // a say while recording would be recorded
    if (empty(g_voice_names)) {
        g_status = "no voice to read in - record one"
        return
    }
    cancel_say()
    g_say_silent = false
    delete g_measure_rtf
    let voice = chosen_voice()
    var a = Ask(kind = 2, text = text, voice = voice, gen = g_says)
    g_ask |> push_archive(a)
    g_status = voice == VOICE_NAME ? "reading in your voice..." : "reading in {voice}'s voice - record to hear yours"
    to_log(LOG_INFO, "parrot: say {g_says} in voice {voice}, {length(text)} characters\n")
}

//! the measure button: the text synthesized without playback, its chunks timed
def say_silent() {
    let text = strip(SCRIPT.value)
    return if (empty(text) || g_phase != Phase.idle || !g_ready || empty(g_voice_names))
    let left = g_measure_left   // cancel_say ends a measure in flight; this one continues it
    cancel_say()
    g_measure_left = left
    g_say_silent = true
    var a = Ask(kind = 2, text = text, voice = chosen_voice(), gen = g_says, silent = true)
    g_ask |> push_archive(a)
    g_status = "measuring - run {length(g_measure_rtf) + 1} of {MEASURE_RUNS} at {JOBS.value} jobs..."
}

def start_measure() {
    return if (g_phase != Phase.idle || !g_ready)
    cancel_say()
    delete g_measure_rtf
    g_measure_left = MEASURE_RUNS - 1
    say_silent()
}

def toggle_record() {
    if (g_phase == Phase.recording) {
        stop_take()
    } else {
        start_take()
    }
}

//! the lab panel's job queue knobs reach the queue the speech thread dispatches on
def apply_jobque_knobs() {
    return if (!g_ready)
    if (JOBS.value != g_jobs_applied) {
        g_jobs_applied = JOBS.value
        set_jobque_worker_limit(JOBS.value >= g_jobs_max ? -1 : JOBS.value)
    }
    if (TEAM.value != g_team_applied) {
        g_team_applied = TEAM.value
        set_jobque_team_mode(TEAM.value)
    }
    if (SPIN.value != g_spin_applied) {
        g_spin_applied = SPIN.value
        set_jobque_worker_spin(SPIN.value ? 30000 : 0)
    }
}

// ===== the screen =====

let INK = rgba(236, 229, 210, 255)
let DIM = rgba(141, 133, 119, 255)
let ACCENT = rgba(242, 185, 85, 255)
let RED = rgba(226, 84, 74, 255)
let GREEN = rgba(155, 196, 106, 255)
let PANEL = rgba(24, 22, 30, 255)
let WAVE = rgba(120, 160, 220, 255)
let WAVE_DIM = rgba(80, 90, 120, 255)

def phase_chip() : tuple<text : string; color : uint> {
    if (g_phase == Phase.recording) {
        return (text = "recording  {float(length(g_take)) / float(g_mic_rate)} s", color = RED)
    }
    if (g_phase == Phase.cloning) {
        return (text = "cloning your voice", color = ACCENT)
    }
    if (!g_ready) {
        return (text = "loading the model", color = DIM)
    }
    if (g_playing_sid != INVALID_SID) {
        return (text = "speaking", color = GREEN)
    }
    if (g_chunks_known && !all_spoken(g_chunks)) {
        return (text = "generating", color = ACCENT)
    }
    return (text = "ready", color = GREEN)
}

//! the state chip and the status line across the top
def draw_status_bar() {
    let chip = phase_chip()
    let origin = GetCursorScreenPos()
    let h = GetTextLineHeight() + 12.0
    with_window_drawlist() $(var dl) {
        dl |> add_rect_filled(origin, float2(origin.x + 14.0 + CalcTextSize(chip.text).x + 14.0, origin.y + h), chip.color, 6.0)
        if (g_phase == Phase.recording && (g_frames / 20) % 2 == 0) {
            dl |> add_circle_filled(float2(origin.x - 12.0, origin.y + h * 0.5), 5.0, RED, 0)
        }
    }
    dummy(CHIP_GAP, (size = float2(8.0, 6.0)))
    same_line((spacing = 6.0))
    with_style((ImGuiCol.Text, rgba(20, 18, 24, 255))) {
        text(CHIP_TEXT, (text = chip.text))
    }
    same_line((spacing = 18.0))
    with_style((ImGuiCol.Text, g_phase == Phase.idle ? ACCENT : DIM)) {
        text_wrapped(STATUS_LINE, (text = g_status))
    }
}

//! min/max columns over a box: the take or the say; the shaded window and the playhead ride on top
def draw_wave(peaks : array<float2>; size : float2; window_from, window_to, playhead : int64; ticks : array<int64>) {
    let origin = GetCursorScreenPos()
    let cols = length(peaks)
    with_window_drawlist() $(var dl) {
        dl |> add_rect_filled(origin, float2(origin.x + size.x, origin.y + size.y), PANEL, 4.0)
        let mid = origin.y + size.y * 0.5
        dl |> add_line(float2(origin.x, mid), float2(origin.x + size.x, mid), WAVE_DIM, 1.0)
        if (cols > 0) {
            let per_px = max(1.0, float(cols) / size.x)
            let total = int64(cols) * int64(PEAK_COLUMN)
            let x_of = @(sample : int64) => origin.x + size.x * float(double(sample) / double(max(total, 1l)))
            if (window_to > window_from) {
                dl |> add_rect_filled(float2(x_of(window_from), origin.y), float2(x_of(window_to), origin.y + size.y), rgba(242, 185, 85, 40), 0.0)
            }
            var px = 0.0
            while (px < size.x) {
                let c0 = int(px * per_px)
                let c1 = min(cols, int((px + 1.0) * per_px) + 1)
                var lo = 1.0
                var hi = -1.0
                for (c in range(c0, c1)) {
                    lo = min(lo, peaks[c].x)
                    hi = max(hi, peaks[c].y)
                }
                if (hi >= lo) {
                    let y0 = mid - hi * size.y * 0.48
                    let y1 = mid - lo * size.y * 0.48
                    dl |> add_line(float2(origin.x + px, y0), float2(origin.x + px, max(y1, y0 + 1.0)), WAVE, 1.0)
                }
                px++
            }
            for (t in ticks) {
                dl |> add_line(float2(x_of(t), origin.y), float2(x_of(t), origin.y + size.y), WAVE_DIM, 1.0)
            }
            if (playhead > 0l && playhead <= total) {
                dl |> add_line(float2(x_of(playhead), origin.y), float2(x_of(playhead), origin.y + size.y), ACCENT, 2.0)
            }
        }
    }
    dummy(WAVE_BOX, (size = size))
}

//! the record button: a red disc that becomes a square while recording, the silence countdown around it
def draw_record_button() {
    let size = float2(96.0, 96.0)
    let origin = GetCursorScreenPos()
    let clicked = invisible_button(REC_BTN, (text = "record", size = size))
    let hovered = IsItemHovered()
    let center = float2(origin.x + size.x * 0.5, origin.y + size.y * 0.5)
    with_window_drawlist() $(var dl) {
        dl |> add_circle(center, 44.0, hovered ? INK : DIM, 0, 2.0)
        if (g_phase == Phase.recording) {
            dl |> add_rect_filled(float2(center.x - 18.0, center.y - 18.0), float2(center.x + 18.0, center.y + 18.0), RED, 4.0)
            let left = countdown_left(g_speech_seen, g_in_speech, g_elapsed_s - g_quiet_since_s)
            if (left > 0.0) {
                dl |> path_clear()
                dl |> path_arc_to(center, 44.0, -1.5708, -1.5708 + 6.2832 * (left / SILENCE_ENDS_TAKE_S), 32)
                dl |> path_stroke(ACCENT, ImDrawFlags.None, 3.0)
            }
        } else {
            dl |> add_circle_filled(center, 30.0, g_can_clone && g_ready ? RED : DIM, 0)
        }
    }
    if (clicked) {
        toggle_record()
    }
}

def draw_voice_panel() {
    text("voice")
    separator()
    draw_record_button()
    same_line((spacing = 16.0))
    group(REC_SIDE) {
        text(REC_LABEL, (text = g_phase == Phase.recording ? "stop" : "record"))
        let meter = clamp(g_level * 8.0, 0.0, 1.0)
        progress_bar(LEVEL, (fraction = meter, size = float2(160.0, 0.0), overlay = ""))
        GAIN.bounds = (0.25, 4.0)
        with_item_width(160.0) {
            slider_float(GAIN, (text = "gain"))
        }
        if (g_phase == Phase.recording) {
            text(TAKE_STATE, (text = g_in_speech ? "speech" : (g_speech_seen ? "quiet {countdown_left(g_speech_seen, g_in_speech, g_elapsed_s - g_quiet_since_s)} s" : "listening")))
        } else {
            text(VOICE_FACT, (text = g_voice_ready ? "you: {g_voice_seconds} s, cloned in {g_clone_ms} ms" : "no take yet"))
        }
    }
    spacing()
    let no_ticks : array<int64>
    draw_wave(g_take_peaks, float2(GetContentRegionAvail().x, 72.0), g_take_window.from, g_take_window.to, 0l, no_ticks)
    spacing()
    if (!empty(g_voice_names)) {
        with_item_width(GetContentRegionAvail().x) {
            combo(VOICE, (text = "##voice", items = g_voice_names))
        }
    }
}

def chunk_color(state : ChunkState) : uint {
    if (state == ChunkState.speaking) {
        return ACCENT
    }
    if (state == ChunkState.generating) {
        return INK
    }
    if (state == ChunkState.spoken || state == ChunkState.generated) {
        return rgba(190, 184, 170, 255)
    }
    return DIM
}

//! the middle column: the editor takes two fifths of it, the say's chunks a quarter, the output the rest
def draw_text_panel() {
    text("text")
    separator()
    let avail = GetContentRegionAvail()
    let editor_h = max(120.0, avail.y * 0.38)
    let view_h = max(80.0, avail.y * 0.22)
    input_text_multiline(SCRIPT, (text = "##script", size = float2(avail.x, editor_h)))
    if (button(SAY_BTN, (text = "say", size = float2(120.0, 36.0)))) {
        say_text()
    }
    same_line((spacing = 12.0))
    with_style((ImGuiCol.Text, DIM)) {
        text("Ctrl+Enter says   Ctrl+R records and stops")
    }
    text(VIEW_LABEL, (text = empty(g_chunks) ? "the say, chunk by chunk - nothing said yet" : "the say, chunk by chunk"))
    child(SCRIPT_VIEW, (text = "script", size = float2(0.0, view_h), child_flags = ImGuiChildFlags.Borders, window_flags = ImGuiWindowFlags.None)) {
        for (i in range(length(g_chunks))) {
            with_style((ImGuiCol.Text, chunk_color(g_chunks[i].state))) {
                text_wrapped(CHUNK_LINE[i], (text = empty(g_chunks[i].text) ? "..." : g_chunks[i].text))
            }
        }
    }
}

def ms(us : int64) : string {
    return "{double(us) / 1000.0lf}"
}

def draw_output_panel() {
    text("output")
    separator()
    let ticks <- [for (row in g_chunks); row.start; where row.start > 0l]
    draw_wave(g_out_peaks, float2(GetContentRegionAvail().x, 72.0), 0l, 0l, g_playhead, ticks)
    let seconds = double(long_length(g_out_pcm)) / double(max(g_mic_rate, 1))
    text("{seconds} s of speech   rtf {say_rtf()}   playhead {double(g_playhead) / double(max(g_mic_rate, 1))} s")
    if (!empty(g_chunks)) {
        data_table(CHUNK_TABLE, (text = "chunks", columns = 7, flags = ImGuiTableFlags.RowBg | ImGuiTableFlags.SizingStretchProp | ImGuiTableFlags.ScrollY, outer_size = float2(0.0, 0.0), inner_width = 0.0)) {
            table_setup_column("chunk", ImGuiTableColumnFlags.WidthStretch)
            table_setup_column("state", ImGuiTableColumnFlags.WidthFixed, 80.0)
            table_setup_column("audio s", ImGuiTableColumnFlags.WidthFixed, 64.0)
            table_setup_column("prompt", ImGuiTableColumnFlags.WidthFixed, 64.0)
            table_setup_column("backbone", ImGuiTableColumnFlags.WidthFixed, 70.0)
            table_setup_column("head", ImGuiTableColumnFlags.WidthFixed, 56.0)
            table_setup_column("codec", ImGuiTableColumnFlags.WidthFixed, 64.0)
            table_headers_row()
            for (i, row in range(length(g_chunks)), g_chunks) {
                table_next_row()
                if (table_set_column_index(0)) {
                    text(CHUNK_TEXT[i], (text = "{slice(row.text, 0, 40)}"))
                }
                if (table_next_column()) {
                    text(CHUNK_STATE[i], (text = "{row.state}"))
                }
                if (table_next_column()) {
                    text(CHUNK_AUDIO[i], (text = row.samples > 0l ? "{double(row.samples) / double(max(g_mic_rate, 1))}" : "-"))
                }
                if (table_next_column()) {
                    text(CHUNK_PROMPT[i], (text = ms(row.us_prompt)))
                }
                if (table_next_column()) {
                    text(CHUNK_BACKBONE[i], (text = ms(row.us_backbone)))
                }
                if (table_next_column()) {
                    text(CHUNK_HEAD[i], (text = ms(row.us_head)))
                }
                if (table_next_column()) {
                    text(CHUNK_CODEC[i], (text = ms(row.us_codec)))
                }
            }
        }
    }
}

def draw_lab_panel() {
    text("lab")
    separator()
    text("model  {g_args.tts_model}")
    text("rate  {g_mic_rate} Hz   voices  {length(g_roster)}   clones  {g_can_clone ? "yes" : "no"}")
    text("platform  {get_running_platform_name()}   cores  {get_total_hw_cores()}")
    spacing()
    text("job queue  (up to {g_jobs_max} workers)")
    with_item_width(160.0) {
        slider_int(JOBS, (text = "jobs"))
    }
    checkbox(TEAM, (text = "team dispatch"))
    checkbox(SPIN, (text = "workers spin before parking"))
    apply_jobque_knobs()
    spacing()
    if (button(MEASURE_BTN, (text = "measure", size = float2(120.0, 30.0)))) {
        start_measure()
    }
    with_style((ImGuiCol.Text, DIM)) {
        text_wrapped(MEASURE_HINT, (text = "says the text {MEASURE_RUNS} times without playing it, and reports the real-time factor of each run"))
    }
    for (i in range(length(g_measure_rtf))) {
        text(MEASURE_ROW[i], (text = "run {i + 1}  rtf {g_measure_rtf[i]}"))
    }
    spacing()
    with_style((ImGuiCol.Text, DIM)) {
        text_wrapped("your recording stays in this window: nothing is uploaded, and the voice is gone when you close it")
    }
}

def draw_screen() {
    let io & = unsafe(GetIO())
    SetNextWindowPos(ImVec2(0.0, 0.0), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y), ImGuiCond.Always)
    let flags = ImGuiWindowFlags.NoTitleBar | ImGuiWindowFlags.NoResize | ImGuiWindowFlags.NoMove | ImGuiWindowFlags.NoCollapse | ImGuiWindowFlags.NoBringToFrontOnFocus | ImGuiWindowFlags.NoScrollbar
    window(MAIN_WIN, (text = "parrot", closable = false, flags = flags)) {
        draw_status_bar()
        separator()
        let avail = GetContentRegionAvail()
        let left_w = clamp(avail.x * 0.28, 260.0, 380.0)
        let right_w = clamp(avail.x * 0.26, 260.0, 360.0)
        let mid_w = avail.x - left_w - right_w - 16.0
        child(VOICE_PANEL, (text = "voice", size = float2(left_w, 0.0), child_flags = ImGuiChildFlags.None, window_flags = ImGuiWindowFlags.None)) {
            draw_voice_panel()
        }
        same_line((spacing = 8.0))
        child(TEXT_PANEL, (text = "text", size = float2(mid_w, 0.0), child_flags = ImGuiChildFlags.None, window_flags = ImGuiWindowFlags.None)) {
            draw_text_panel()
            spacing()
            draw_output_panel()
        }
        same_line((spacing = 8.0))
        child(LAB_PANEL, (text = "lab", size = float2(right_w, 0.0), child_flags = ImGuiChildFlags.None, window_flags = ImGuiWindowFlags.None)) {
            draw_lab_panel()
        }
    }
}

//! the chords: Ctrl or Command with Enter says, with R records and stops
def poll_chords() {
    let io & = unsafe(GetIO())
    let mod = io.KeyCtrl || io.KeySuper
    return if (!mod)
    if (IsKeyPressed(ImGuiKey.Enter, false) || IsKeyPressed(ImGuiKey.KeypadEnter, false)) {
        say_text()
    } elif (IsKeyPressed(ImGuiKey.R, false)) {
        toggle_record()
    }
}

// ===== the program =====

[export]
def init() {
    var inscope r <- parse_args(type<ParrotArgs>)
    if (r |> is_err) {
        panic("parrot: {r |> unwrap_err}")
    }
    g_args <- r |> move_unwrap
    SCRIPT.capacity = 8192
    SCRIPT.value = empty(g_args.text) ? POEM : g_args.text
    GAIN.value = 1.0
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
        peaks_append(g_take_peaks, g_pending_clip, 0l, PEAK_COLUMN)   // the clip stands in for the take on the voice panel
        g_take_window = (from = 0l, to = long_length(g_pending_clip))
    }
    harness_init("parrot", 1280, 760)
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
    if (!harness_begin_frame()) {
        return
    }
    g_frames++
    g_elapsed_s += get_dt()
    harness_new_frame()
    poll_chords()
    if (g_phase == Phase.recording) {
        drain_take()
    }
    poll_answers()
    draw_screen()
    harness_end_frame()
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
    harness_shutdown()
}

def read_out() : bool {
    return g_says > 0 && g_chunks_known && all_spoken(g_chunks) && empty(g_clips) && g_playing_sid == INVALID_SID
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
        harness_maybe_collect_gc()
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
