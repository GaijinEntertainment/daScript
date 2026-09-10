options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require daslib/fio
require daslib/strings_boost
require daslib/strings_convert
require daslib/env_registry
require math
require strings
require _model_tier
require _example_rail

// The storyteller's restart rails (examples/dasLLAMA/storyteller/main.das): a space press while a
// story is being read stops the clip and starts another, and the speech thread skips the queued
// sentences of the replaced story instead of synthesizing them first. Each cell spawns the example
// under --smoke - with --restart-on-clip N (a space press as the Nth clip begins) or --restart-at N
// (a space press at frame N) - and reads the witness lines the example logs; a frame count is
// the instrument only while the story is being written, since an unthrottled window runs
// thousands of frames a second once it idles. Model-gated on stories15M-Q8_0.gguf (its ../performance/model_specs.das
// row names the conversion), kitten-nano.gguf and the front-end packs under models_dir(); a run
// opens the example's window for up to a minute, so a box without a window server skips, and
// --null-audio keeps the run silent.

let STOP_LINE = "storyteller: the clip of story 1 stops, "
let SKIP_LINE = "storyteller: a sentence of story 1 is skipped, story 2 is being told"
let DONE_LINE = "storyteller: story 2 written and read out"
let CAP_LINE = "storyteller: the frame cap stopped story 1 after 30 frames"
let FIRST_START = "storyteller: story 1 starts at frame 0"
let RESTART_MARK = "storyteller: story 2 starts at frame "
let CAP_FRAMES = 30
let NO_CAP = 0   //! a story takes a minute to read out at any frame rate - the spawn's wall clock is the guard
let PACK_FILES <- ["tts_g2p.bin", "tts_postag.bin"]

//! the window server and every fixture file, each absence registering its own loud skip
def private ready(t : T?) : bool {
    if (!has_window_server()) {
        t |> skip("no window server (DISPLAY and WAYLAND_DISPLAY unset) - the example opens a window")
        return false
    }
    for (name in ["stories15M-Q8_0.gguf", "kitten-nano.gguf"]) {
        return false if (!model_available(t, path_join(models_dir(), name)))
    }
    for (name in PACK_FILES) {
        if (!stat(path_join(models_dir(), name)).is_valid) {
            t |> skip("{name} not present")
            return false
        }
    }
    return true
}

//! the example's smoke rail: a space press at frame `restart_at` or as clip `restart_on_clip` begins (0 = none),
//! a stop at `max_frames` (0 = none); a non-empty `lanes` pins DAS_JOBQUE_THREADS for the child, which slows
//! the speech thread's model load and starves it
def private run_storyteller(restart_at, restart_on_clip, max_frames : int; lanes : string; var out : string&) : int {
    let prev_image = env_value_of("DASLLAMA_IMAGE")
    let prev_lanes = env_value_of("DAS_JOBQUE_THREADS")
    set_env_variable("DASLLAMA_IMAGE", "0")   //! never mint a sidecar beside the stocked models
    if (!empty(lanes)) {
        set_env_variable("DAS_JOBQUE_THREADS", lanes)
    }
    let root = get_das_root()
    var argv <- [example_daslang(), "-jit", "-dasroot", root, "{root}/examples/dasLLAMA/storyteller/main.das", "--",
                 "--models", models_dir(), "--smoke", "--max-frames", "{max_frames}", "--null-audio"]
    if (restart_at > 0) {
        argv |> push("--restart-at")
        argv |> push("{restart_at}")
    }
    if (restart_on_clip > 0) {
        argv |> push("--restart-on-clip")
        argv |> push("{restart_on_clip}")
    }
    let rc = run_and_capture(argv, out, 300.0)
    set_env_variable("DASLLAMA_IMAGE", prev_image)
    if (!empty(lanes)) {
        set_env_variable("DAS_JOBQUE_THREADS", prev_lanes)
    }
    return rc
}

def private count_of(hay, needle : string) : int {
    var n = 0
    var at = find(hay, needle)
    while (at >= 0) {
        n++
        at = find(hay, needle, at + length(needle))
    }
    return n
}

//! the unheard count the stop line carries; -1 when the line is absent or does not parse
def private unheard_of(out : string) : int {
    let at = find(out, STOP_LINE)
    return -1 if (at < 0)
    let start = at + length(STOP_LINE)
    let stop = find(out, " sentence", start)
    return -1 if (stop < 0)
    let r = try_to_int(slice(out, start, stop))
    return r |> is_err ? -1 : r |> unwrap
}

def private witness(out : string) : string {
    var inscope lines <- split(out, "\n")
    return join([for (line in lines); line; where find(line, "storyteller:") >= 0], "\n")
}

def private tail_of(out : string) : string {
    return slice(out, max(0, length(out) - 600))
}

[test]
def test_restart_while_reading(t : T?) {
    t |> run("a space press while the story is being read stops its clip, drops its unplayed sentences, and the next story is read out") @(t : T?) {
        return if (!ready(t))
        var out = ""
        let rc = run_storyteller(0, 2, NO_CAP, "", out)   // as story 1's second clip begins: written out, being read
        return if (skip_without_window(t, rc, out))
        t |> equal(0, rc, "the smoke run exits clean: {tail_of(out)}")
        t |> equal(count_of(out, RESTART_MARK), 1, "one restart:\n{witness(out)}")
        t |> equal(count_of(out, STOP_LINE), 1, "story 1's clip was playing and stopped once:\n{witness(out)}")
        t |> success(unheard_of(out) >= 1, "the restart found unplayed sentences of story 1 to drop:\n{witness(out)}")
        t |> success(find(out, DONE_LINE) >= 0, "story 2 was written and read out to the end:\n{witness(out)}")
    }
}

[test]
def test_restart_skips_the_replaced_story_queue(t : T?) {
    t |> run("a restart while the speech thread is still loading its model skips the replaced story's queued sentences") @(t : T?) {
        return if (!ready(t))
        var out = ""
        let restart_at = 100   // story 1 is still being written; the starved speech thread has sentences queued
        let rc = run_storyteller(restart_at, 0, NO_CAP, "2", out)
        return if (skip_without_window(t, rc, out))
        t |> equal(0, rc, "the smoke run exits clean: {tail_of(out)}")
        t |> equal(count_of(out, "{RESTART_MARK}{restart_at}"), 1, "one restart, at the frame the rail named:\n{witness(out)}")
        let skipped = count_of(out, SKIP_LINE)
        t |> success(skipped >= 1, "the speech thread skipped story 1's queued sentences instead of synthesizing them first:\n{witness(out)}")
        t |> success(find(out, DONE_LINE) >= 0, "story 2 was written and read out to the end:\n{witness(out)}")
        feint("storyteller restart while starved: {skipped} sentences skipped, unheard {unheard_of(out)}\n")
    }
}

[test]
def test_frame_cap_and_no_restart_by_default(t : T?) {
    t |> run("the frame cap ends an unread story, and the rail restarts nothing on its own") @(t : T?) {
        return if (!ready(t))
        var out = ""
        let rc = run_storyteller(0, 0, CAP_FRAMES, "", out)
        return if (skip_without_window(t, rc, out))
        t |> equal(0, rc, "the capped run exits clean: {tail_of(out)}")
        t |> success(find(out, FIRST_START) >= 0, "the first story starts at frame 0:\n{witness(out)}")
        t |> success(find(out, CAP_LINE) >= 0, "the cap line names the story and the frame it stopped at:\n{witness(out)}")
        t |> equal(count_of(out, RESTART_MARK), 0, "no --restart-at, no second story:\n{witness(out)}")
    }
}
