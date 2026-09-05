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
// voice while the next one is still being written. Space starts a new story, Escape quits.
//
//   bin/daslang -jit examples/dasLLAMA/storyteller/main.das -- --models <dir>
//
// <dir> holds the two models and the front-end packs tts_g2p.bin and tts_postag.bin. A model
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

    @clarg_doc = "Start a story at once and quit when it has been read out - the smoke rail"
    autoplay : bool

    @clarg_doc = "Stop after this many frames (0 = never) - the smoke rail"
    max_frames : int

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
let VISIBLE_LINES = 15            // what fits between the title and the status line at the reference size
let TEXT_SIZE = 0.72
let SMALL_SIZE = 0.52

enum Phase {
    idle
    writing
    told
}

struct Line {
    text : string   //! a sentence for the speech thread; "" asks it to stop
    story : int     //! which story it belongs to: a clip from an older story is dropped unheard
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
var g_lines : array<string>       // the story as wrapped screen lines
var g_sentence : array<string>    // the pieces of the sentence being written, handed to speech at its end
var g_ntokens = 0
var g_stories = 0

var g_say : Stream?               // frame thread -> speech thread: sentences
var g_speech : Stream?            // speech thread -> frame thread: clips
var g_speech_done : Channel?      // the speech thread signals it before it ends
var g_clips : array<Speech>       // clips waiting for the speaker
var g_now = 0.0                   // seconds since start, from frame dt
var g_speaking_until = 0.0
var g_pending_speech = 0          // sentences sent and not yet played
var g_audio_initialized = false
var g_asch : AudioSystemChannels
var g_font : Font?
var g_frames = 0
var g_space_was = false
var display_w = 0
var display_h = 0

// --- speech thread ---

def start_speech_thread(tts_path, voice_pick : string; var say, speech : Stream?; var done : Channel?) {
    new_thread() <| @capture(= tts_path, = voice_pick, = say, = speech, = done) {
        setup_dasllama_jobque()   // the fork-context pool is per context: without it every parallel kernel clones the program
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
                var inscope a <- synthesize(m, l.text, voice, 1.0)
                var clip = Speech(pcm := a.pcm, rate = a.sample_rate, story = l.story)
                speech |> push_archive(clip)
            }
        }
        say |> release()
        speech |> release()
        done |> notify_and_release()
    }
}

def say(text : string) {
    var l = Line(text = text, story = g_stories)
    g_say |> push_archive(l)
    g_pending_speech++
}

def poll_speech() {
    g_speech |> try_pop() $(bytes) {
        var clip : Speech
        mem_archive_load(bytes, clip)
        if (clip.story == g_stories) {
            g_clips |> emplace(clip)
        }
    }
    if (g_now >= g_speaking_until && !empty(g_clips)) {
        let seconds = float(length(g_clips[0].pcm)) / float(max(g_clips[0].rate, 1))
        var pcm <- g_clips[0].pcm
        play_sound_from_pcm(g_clips[0].rate, 1, pcm)
        g_clips |> erase(0)
        g_speaking_until = g_now + seconds
        g_pending_speech--
    }
}

// --- the story ---

def start_story() {
    delete g_session
    g_session <- create_session(g_model)
    set_seed(g_session, int(ref_time_ticks() % 2147483647l))   // a fresh session samples the same story every time otherwise
    g_lines |> clear()
    g_lines |> push("")
    g_sentence |> clear()
    g_sentence |> reserve(64)
    g_ntokens = 0
    g_stories++
    g_clips |> clear()          // the previous story's unplayed clips; its sentences still in flight come back tagged and drop
    g_pending_speech = 0
    g_speaking_until = g_now
    var prompt <- encode(g_model, PROMPT, true)
    eval(g_model, g_session, prompt)
    append_text(PROMPT)
    g_sentence |> push(PROMPT)
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
            g_lines |> push("")
        }
        let li = length(g_lines) - 1
        g_lines[li] = "{g_lines[li]}{part}"
        wrap_last_line()
    }
}

// break the last line at its last space before the wrap column, as often as it takes
def wrap_last_line() {
    while (length(g_lines[length(g_lines) - 1]) > WRAP_CHARS) {
        let li = length(g_lines) - 1
        var head = ""
        var tail = ""
        peek_data(g_lines[li]) $(d) {
            var cut = rfind(slice(d, 0, WRAP_CHARS), " ")
            if (cut <= 0) {
                cut = WRAP_CHARS
            }
            head = clone_string(slice(d, 0, cut))
            tail = clone_string(slice(d, cut))
        }
        g_lines[li] = head
        g_lines |> push(strip(tail))
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
        g_sentence |> push(text)
        if (sentence_ends(text)) {
            say(strip(join(g_sentence, "")))
            g_sentence |> clear()
        }
        one[0] = tok
        eval(g_model, g_session, one)
    }
}

def finish_story() {
    let rest = strip(join(g_sentence, ""))
    if (!empty(rest)) {
        say(rest)
    }
    g_sentence |> clear()
    g_phase = Phase.told
}

// --- text layer, in design pixels against a 1280x720 reference ---

def hud_scale() : float {
    return max(float(display_h) / 720.0, 0.5)
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
    let first = max(0, length(g_lines) - VISIBLE_LINES)   // a long story scrolls: the newest lines stay on screen
    for (i in range(first, length(g_lines))) {
        draw_text(g_lines[i], 60.0, y, TEXT_SIZE, ink)
        y += step
    }
    let status = (g_phase == Phase.idle ? "press SPACE for a story"
        : g_phase == Phase.writing ? "writing..."
        : g_pending_speech > 0 || g_now < g_speaking_until ? "reading aloud..."
        : "the end - press SPACE for another")
    draw_text(status, 60.0, 660.0, SMALL_SIZE, g_phase == Phase.idle ? accent : dim)
}

// --- lifecycle ---

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
    g_speech_done = unsafe(channel_create())
    g_speech_done |> append(1)
    start_speech_thread(path_join(g_args.models, g_args.tts_model), g_args.voice, g_say, g_speech, g_speech_done)
    if (g_args.autoplay) {
        start_story()
    }
}

[export]
def update() {
    if (!live_begin_frame()) {
        return
    }
    g_frames++
    g_now += get_dt()
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
    if (space_just && g_phase != Phase.writing) {
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
    say("")   // the speech thread's stop word
    g_speech_done |> join()
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

def done_for_smoke() : bool {
    return true if (g_args.max_frames > 0 && g_frames >= g_args.max_frames)
    return g_args.autoplay && g_phase == Phase.told && g_pending_speech == 0 && g_now >= g_speaking_until
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
    shutdown()
    if (g_args.autoplay) {
        print("{join(g_lines, "\n")}\n")
    }
}
