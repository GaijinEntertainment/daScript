options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dasllama/dasllama   // the facade: model, session, eval / sample / piece, the TTS entry points
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
require live_host
require math
require daslib/math_boost   // ortho_rh / compose for the text layer
require strings

// Storyteller. A tiny story model (Karpathy's stories15M, Q8) writes a children's story on
// screen a few tokens per frame, and KittenTTS reads each finished sentence aloud in a kid's
// voice while the next one is still being written. Space starts a new story at any moment - the
// clip being read stops, and the sentences of the old story still queued for speech are skipped,
// so the new one is heard at once. Escape quits.
//
//   bin/daslang -jit examples/dasLLAMA/storyteller/main.das -- --models <dir>
//
// <dir> holds the two models and the front-end packs - tts_g2p.bin (or its American-only twin
// tts_g2p_en_us.bin, which the web set ships) and tts_postag.bin. A model
// is its gguf (the default names) or a prepared .dlim image baked for the running build's
// identity (dasllama-convert --config) - the web build ships images only, the shell names them.
// The language model runs on the frame thread in per-frame token budgets, so the loop never
// blocks; speech synthesis runs on its own thread, fed sentences through one stream and
// answering with PCM through another, and the frame thread plays the clips back to back.

[CommandLineArgs]
struct StoryArgs {
    @clarg_short = "m"
    @clarg_doc = "Directory holding the story model, the TTS model and its phoneme packs"
    models : string = "/Users/borisbatkin/Work/llama.cpp/models"

    @clarg_doc = "The story model file inside --models"
    story_model : string = "stories15M-Q8_0.gguf"

    @clarg_doc = "The TTS model file inside --models"
    tts_model : string = "kitten-nano.gguf"

    @clarg_doc = "Voice name or alias (default: the model's last voice)"
    voice : string

    @clarg_doc = "Start a story at once (the browser shell's start button) - the loop then runs on, space for the next"
    autoplay : bool

    @clarg_doc = "Start a story at once and quit when it has been read out - the smoke rail"
    smoke : bool

    @clarg_doc = "Stop after this many frames (0 = never) - the smoke rail"
    max_frames : int

    @clarg_doc = "Start another story at this frame, as a space press would (0 = never) - the smoke rail's restart-while-writing check"
    restart_at : int

    @clarg_doc = "Start another story when this many clips have begun playing, as a space press would (0 = never) - the smoke rail's restart-while-reading check"
    restart_on_clip : int

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

let PROMPT = "Once upon a time"
let MAX_TOKENS = 220
let STORY_CONTEXT = 256l          // prompt + MAX_TOKENS must fit; stories15M was trained at 256
let TOKENS_PER_FRAME = 2          // ~120 tokens a second at 60 Hz: readable typing, never a stall
let BOS_TOKEN = 1l                // llama2 vocabulary; the TinyStories corpus separates stories with BOS, so it is the stop word too
let EOS_TOKEN = 2l
let WRAP_CHARS = 74               // droidsansmono is monospace: characters are the wrap unit
let TEXT_SIZE = 0.72
let SMALL_SIZE = 0.52

enum Phase {
    idle
    writing
    told
}

struct Line {
    text : string   //! a sentence for the speech thread; "" asks it to stop
    story : int     //! which story it belongs to: the speech thread skips a sentence of a replaced story, and a clip of one that slipped through is dropped unheard
}

struct Speech {
    pcm : array<float>
    rate : int
    story : int
}

var g_args = StoryArgs()
var g_model = Model()
var g_session = Session()
var g_params = SamplingParams(temp = 0.8, top_k = 40l, penalty = 1.1)
var g_phase = Phase.idle
var g_wrapped_lines : array<string>
var g_sentence_pieces : array<string>
var g_ntokens = 0
var g_stories = 0

var g_say : Stream?               // frame thread -> speech thread: sentences
var g_speech : Stream?            // speech thread -> frame thread: clips
var g_story_now : SeqBox?         // frame thread -> speech thread: the story being told, so a queued sentence of an older one is skipped
var g_speech_done : Channel?
var g_clips : array<Speech>
var g_playing_sid = INVALID_SID   // the clip being read; a new story stops it
var g_elapsed_s = 0.0
var g_speaking_until = 0.0
var g_unplayed_sentences = 0
var g_audio_initialized = false
var g_asch : AudioSystemChannels
var g_font : Font?
var g_frames = 0
var g_clips_played = 0
var g_rail_restarted = false      // the smoke rail's restart fires once
var g_space_was = false
var display_w = 0
var display_h = 0

//! the speech thread's setup rides its own stream ahead of the sentences: the model path, then the voice. A string
//! captured by the lambda is a pointer into the frame thread's heap, which that thread reuses before a slow worker
//! reads it; an archived line is copied out of the stream into the thread's own heap
def start_speech_thread(var say, speech : Stream?; var now : SeqBox?; var done : Channel?) {
    new_thread() <| @capture(= say, = speech, = now, = done) {
        setup_dasllama_jobque()   // the fork-context pool is per context: without it every parallel kernel clones the program
        var tts_path = ""
        var voice_pick = ""
        say |> pop_archive() $(var l : Line&) {
            tts_path = clone_string(l.text)
        }
        say |> pop_archive() $(var l : Line&) {
            voice_pick = clone_string(l.text)
        }
        var inscope m <- load_tts_model(tts_path)
        var inscope c <- caps(m)
        let voice = empty(voice_pick) ? c.voices[length(c.voices) - 1] : voice_pick
        var running = true
        while (running) {
            say |> pop_archive() $(var l : Line&) {
                if (empty(l.text)) {
                    running = false
                    return
                }
                var current = l.story
                now |> read() $(story : int) {
                    current = story
                }
                if (l.story < current) {   // a sentence of a story space already replaced: its clip would be dropped unheard, and the synthesis would keep the new story waiting
                    to_log(LOG_INFO, "storyteller: a sentence of story {l.story} is skipped, story {current} is being told\n")
                    return
                }
                var inscope a <- synthesize(m, l.text, voice, 1.0)
                var clip = Speech(pcm := a.pcm, rate = a.sample_rate, story = l.story)
                speech |> push_archive(clip)
            }
        }
        say |> release()
        speech |> release()
        now |> seq_box_release()
        done |> notify_and_release()
    }
}

def say(text : string) {
    var l = Line(text = text, story = g_stories)
    g_say |> push_archive(l)
    g_unplayed_sentences++
}

def poll_speech() {
    g_speech |> try_pop() $(bytes) {
        var clip : Speech
        mem_archive_load(bytes, clip)
        if (clip.story == g_stories) {
            g_clips |> emplace(clip)
        }
    }
    return if (g_elapsed_s < g_speaking_until)
    g_playing_sid = INVALID_SID   // the clip has played out: a restart now has nothing to stop
    if (!empty(g_clips)) {
        let seconds = float(length(g_clips[0].pcm)) / float(max(g_clips[0].rate, 1))
        var pcm <- g_clips[0].pcm
        g_playing_sid = play_sound_from_pcm(g_clips[0].rate, 1, pcm)
        g_clips |> erase(0)
        g_speaking_until = g_elapsed_s + seconds
        g_unplayed_sentences--
        g_clips_played++
    }
}

def start_story() {
    if (g_playing_sid != INVALID_SID) {
        //! the smoke rail's witness lines, here and below: modules/dasLLAMA/tests/test_storyteller_restart.das reads them word for word
        to_log(LOG_INFO, "storyteller: the clip of story {g_stories} stops, {g_unplayed_sentences} sentences of it go unheard\n")
        stop(g_playing_sid, 0.05)
        g_playing_sid = INVALID_SID
    }
    delete g_session
    g_session <- create_session(g_model)
    set_seed(g_session, int(ref_time_ticks() % 2147483647l))   // a fresh session samples the same story every time otherwise
    g_wrapped_lines |> clear()
    g_wrapped_lines |> push("")
    g_sentence_pieces |> clear()
    g_sentence_pieces |> reserve(64)
    g_ntokens = 0
    g_stories++
    g_story_now |> publish(g_stories)
    to_log(LOG_INFO, "storyteller: story {g_stories} starts at frame {g_frames}\n")
    g_clips |> clear()
    g_unplayed_sentences = 0
    g_speaking_until = g_elapsed_s
    var prompt <- encode(g_model, PROMPT, true)
    eval(g_model, g_session, prompt)
    append_text(PROMPT)
    g_sentence_pieces |> push(PROMPT)
    g_phase = Phase.writing
}

// the corpus writes curly quotes; the screen font carries ASCII, so they would vanish
def plain_quotes(piece : string) : string {
    return piece |> replace("“", "\"") |> replace("”", "\"") |> replace("‘", "'") |> replace("’", "'")
}

def append_text(piece : string) {
    var inscope parts <- split(piece, "\n")
    for (i, part in iter_range(parts), parts) {
        if (i > 0) {
            g_wrapped_lines |> push("")
        }
        let li = length(g_wrapped_lines) - 1
        g_wrapped_lines[li] = "{g_wrapped_lines[li]}{part}"
        wrap_last_line()
    }
}

def wrap_last_line() {
    while (length(g_wrapped_lines[length(g_wrapped_lines) - 1]) > WRAP_CHARS) {
        let li = length(g_wrapped_lines) - 1
        var head = ""
        var tail = ""
        peek_data(g_wrapped_lines[li]) $(d) {
            var cut = rfind(slice(d, 0, WRAP_CHARS), " ")
            if (cut <= 0) {
                cut = WRAP_CHARS
            }
            head = clone_string(slice(d, 0, cut))
            tail = clone_string(slice(d, cut))
        }
        g_wrapped_lines[li] = head
        g_wrapped_lines |> push(strip(tail))
    }
}

def sentence_ends(piece : string) : bool {
    let t = strip(piece)
    return ends_with(t, ".") || ends_with(t, "!") || ends_with(t, "?") || ends_with(t, "\"") && (find(t, ".") >= 0 || find(t, "!") >= 0 || find(t, "?") >= 0)
}

def write_step() {
    var one : array<int64>
    one |> resize(1)
    for (_i in range(TOKENS_PER_FRAME)) {
        let tok = sample(g_session, g_params)
        if (tok == EOS_TOKEN || tok == BOS_TOKEN || g_ntokens >= MAX_TOKENS) {
            finish_story()
            return
        }
        g_ntokens++
        let text = plain_quotes(piece(g_model, tok))
        append_text(text)
        g_sentence_pieces |> push(text)
        if (sentence_ends(text)) {
            say(strip(join(g_sentence_pieces, "")))
            g_sentence_pieces |> clear()
        }
        one[0] = tok
        eval(g_model, g_session, one)
    }
}

def finish_story() {
    let rest = strip(join(g_sentence_pieces, ""))
    if (!empty(rest)) {
        say(rest)
    }
    g_sentence_pieces |> clear()
    g_phase = Phase.told
}

//! the text layer's coordinates are design pixels against a 1280x720 reference; the smaller of the two ratios
//! governs, so a window narrower than 16:9 shrinks the text instead of cutting the lines on the right
def hud_scale() : float {
    let fit = min(float(display_w) / 1280.0, float(display_h) / 720.0)
    return max(fit, 0.5)
}

//! the design height the window offers at that scale - 720 at 16:9, more in a taller window
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

def draw_story() {
    let ink = float3(0.93, 0.9, 0.82)
    let dim = float3(0.55, 0.52, 0.48)
    let accent = float3(0.98, 0.75, 0.35)
    draw_text("storyteller", 60.0, 60.0, 0.6, dim)
    var y = 130.0
    let step = 34.0 * TEXT_SIZE / 0.72
    let status_y = design_height() - 60.0
    let visible = max(1, int((status_y - 24.0 - y) / step))   // what fits between the title and the status line
    let first = max(0, length(g_wrapped_lines) - visible)   // a long story scrolls: the newest lines stay on screen
    for (i in range(first, length(g_wrapped_lines))) {
        draw_text(g_wrapped_lines[i], 60.0, y, TEXT_SIZE, ink)
        y += step
    }
    let status = (g_phase == Phase.idle ? "press SPACE for a story"
        : g_phase == Phase.writing ? "writing..."
        : g_unplayed_sentences > 0 || g_elapsed_s < g_speaking_until ? "reading aloud..."
        : "the end - press SPACE for another")
    draw_text(status, 60.0, status_y, SMALL_SIZE, g_phase == Phase.idle ? accent : dim)
}

[export]
def init() {
    var inscope r <- parse_args(type<StoryArgs>)
    if (r |> is_err) {
        panic("storyteller: {r |> unwrap_err}")
    }
    g_args <- r |> move_unwrap
    live_create_window("Storyteller", 1280, 720)
    cache_ttf_objects()
    g_font = cache_font("{get_das_root()}/modules/dasStbImage/fonts/droidsansmono.ttf")
    if (!g_audio_initialized) {
        g_asch = audio_system_create()
        g_audio_initialized = true
    }
    create_job_que()
    setup_dasllama_jobque()
    delete g_model
    g_model <- load_model(path_join(g_args.models, g_args.story_model), QuantMode.q8)
    g_model.config.seq_len = max(g_model.config.seq_len, STORY_CONTEXT)   // the session's KV cache is sized to it
    allow_cpu_prefill()
    g_say = unsafe(stream_create())
    g_speech = unsafe(stream_create())
    g_story_now = seq_box_create()
    g_story_now |> publish(g_stories)
    g_speech_done = unsafe(channel_create())
    g_speech_done |> append(1)
    var setup = Line(text = path_join(g_args.models, g_args.tts_model), story = -1)
    g_say |> push_archive(setup)
    setup.text = g_args.voice
    g_say |> push_archive(setup)
    start_speech_thread(g_say, g_speech, g_story_now, g_speech_done)
    if (g_args.autoplay || g_args.smoke) {
        start_story()
    }
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

    let space = glfwGetKey(live_window, GLFW_KEY_SPACE) == GLFW_PRESS
    let space_just = space && !g_space_was
    g_space_was = space
    if (glfwGetKey(live_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(live_window, 1)
    }
    let rail_due = (!g_rail_restarted
        && ((g_args.restart_at > 0 && g_frames == g_args.restart_at)
            || (g_args.restart_on_clip > 0 && g_clips_played == g_args.restart_on_clip)))
    if (space_just || rail_due) {
        g_rail_restarted ||= rail_due
        start_story()
    }
    if (g_phase == Phase.writing) {
        write_step()
    }
    poll_speech()
    draw_story()
    live_end_frame()
}

[export]
def shutdown() {
    say("")
    g_speech_done |> join()
    g_story_now |> seq_box_release()
    unsafe {
        channel_remove(g_speech_done)
        stream_remove(g_say)
        stream_remove(g_speech)
    }
    if (g_audio_initialized) {
        audio_system_finalize(g_asch.command, g_asch.next_sid)
        g_audio_initialized = false
    }
    delete g_session
    delete g_model
    destroy_job_que()
    live_destroy_window()
}

def story_read_out() : bool {
    return g_phase == Phase.told && g_unplayed_sentences == 0 && g_elapsed_s >= g_speaking_until
}

def done_for_smoke() : bool {
    return true if (g_args.max_frames > 0 && g_frames >= g_args.max_frames)
    return g_args.smoke && story_read_out()
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
    let read_out = story_read_out()   // before shutdown: its stop sentinel goes through say(), which counts it as unplayed
    shutdown()
    if (g_args.smoke) {
        to_log(LOG_INFO, read_out
            ? "storyteller: story {g_stories} written and read out after {g_frames} frames\n"
            : "storyteller: the frame cap stopped story {g_stories} after {g_frames} frames\n")
        print("{join(g_wrapped_lines, "\n")}\n")
    }
}
