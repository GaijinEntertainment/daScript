options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require daslib/env_registry
require daslib/fio
require daslib/strings_boost
require strings
require math
require _model_tier
require _example_rail

// Parrot (examples/dasLLAMA/parrot): the smoke cell spawns the example under --smoke with the
// tree's own clip in place of the microphone and reads the witness lines it logs: the voice is
// cloned from the clip, the text is said in it, and the say is read out to the end. Model-gated
// on pocket-tts-en-kq.gguf (the Pocket file with its codec encoder, the one the page ships) under
// models_dir(); the voice-activity model is the tree's own. The run opens the example's window
// for about half a minute, so a box without a window server skips, and --null-audio keeps it
// silent - and off the microphone, which --clip never opens anyway.

let CLIP = "modules/dasLLAMA/models/jfk_ask_not.wav"
let SMOKE_TEXT = "The woods are lovely, dark and deep. But I have promises to keep."
let CLONED_LINE = "parrot: cloned "
let SAY_LINE = "parrot: say 1 in voice you, 65 characters"
let READ_LINE = "parrot: say 1 is read out"
let DONE_LINE = "parrot: the text was read out"

//! the window server and the model file, each absence registering its own loud skip
def private ready(t : T?) : bool {
    if (!has_window_server()) {
        t |> skip("no window server (DISPLAY and WAYLAND_DISPLAY unset) - the example opens a window")
        return false
    }
    return model_available(t, path_join(models_dir(), "pocket-tts-en-kq.gguf"))
}

//! the example's smoke rail: the clip cloned, the text said in the clone, read out to the end (the spawn's wall clock is the guard, not a frame cap)
def private run_parrot(var out : string&) : int {
    let prev_image = env_value_of("DASLLAMA_IMAGE")
    set_env_variable("DASLLAMA_IMAGE", "0")   //! never mint a sidecar beside the stocked models
    let root = get_das_root()
    let argv <- [example_daslang(), "-jit", "-dasroot", root, "{root}/examples/dasLLAMA/parrot/main.das", "--",
                 "--models", models_dir(), "--smoke", "--clip", path_join(root, CLIP), "--text", SMOKE_TEXT, "--null-audio"]
    let rc = run_and_capture(argv, out, 300.0)
    set_env_variable("DASLLAMA_IMAGE", prev_image)
    return rc
}

def private witness(out : string) : string {
    var inscope lines <- split(out, "\n")
    return join([for (line in lines); line; where find(line, "parrot:") >= 0], "\n")
}

def private tail_of(out : string) : string {
    return slice(out, max(0, length(out) - 600))
}

[test]
def test_smoke_clones_and_says(t : T?) {
    t |> run("the smoke rail clones the voice from the clip, says the text in it and reads it out to the end") @(t : T?) {
        return if (!ready(t))
        var out = ""
        let rc = run_parrot(out)
        return if (skip_without_window(t, rc, out))
        t |> equal(0, rc, "the smoke run exits clean: {tail_of(out)}")
        t |> success(find(out, CLONED_LINE) >= 0, "the voice was cloned from the clip:\n{witness(out)}")
        t |> success(find(out, SAY_LINE) >= 0, "the text was said in the cloned voice:\n{witness(out)}")
        t |> success(find(out, READ_LINE) >= 0, "the say was read out:\n{witness(out)}")
        t |> success(find(out, DONE_LINE) >= 0, "the rail ended on the read-out, not the frame cap:\n{witness(out)}")
    }
}
