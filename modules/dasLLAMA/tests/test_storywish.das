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
require ../../../examples/dasLLAMA/storywish/wish.das

// Storywish (examples/dasLLAMA/storywish): the request side is pure and tested directly - the
// typed line becomes the words the corpus spells, the words become the prompt in the corpus's
// layout, and a line the model writes is recognized as a field the corpus puts after a story.
// The smoke cell spawns the example under --smoke with fixed words and reads the witness lines it
// logs: the story starts with those words and is written and read out to the end. Model-gated on
// tinystories-instruct-27M-Q8_0.gguf (its ../performance/model_specs.das row names the source),
// kitten-nano.gguf and the front-end packs under models_dir(); the run opens the example's window
// for about a minute, so a box without a window server skips, and --null-audio keeps it silent.

let PACK_FILES <- ["tts_g2p.bin", "tts_postag.bin"]
let SMOKE_WORDS = "dragon, cake, moon"
let START_LINE = "storywish: story 1 starts at frame 0 - words: dragon, cake, moon"
let DONE_LINE = "storywish: story 1 written and read out"

[test]
def test_wish_words(t : T?) {
    t |> run("the typed line becomes the words the corpus spells: lower case, one space after each comma, no empties") @(t : T?) {
        t |> equal("dragon, cake, moon", wish_words("Dragon,cake ,  MOON"))
        t |> equal("dragon", wish_words(" dragon "))
        t |> equal("cake, moon", wish_words(",cake,,moon,"))
        t |> equal("", wish_words(" , , "))
        t |> equal("", wish_words(""))
    }
}

[test]
def test_build_prompt(t : T?) {
    t |> run("the prompt is the corpus's layout - Features first when asked, Words, then Story: and its line break") @(t : T?) {
        t |> equal("Words: dragon, cake, moon\nStory: \n", build_prompt("dragon, cake, moon", false))
        t |> equal("Features: Dialogue\nWords: dragon, cake, moon\nStory: \n", build_prompt("dragon, cake, moon", true))
    }
}

[test]
def test_line_is_a_field(t : T?) {
    t |> run("a line opening with one of the corpus's field names ends the story; story text never does") @(t : T?) {
        t |> success(line_is_a_field("Summary: A dragon finds a cake."), "Summary:")
        t |> success(line_is_a_field("  Words: dragon, cake"), "Words: after leading spaces")
        t |> success(line_is_a_field("Features: Dialogue"), "Features:")
        t |> success(line_is_a_field("Random sentence: The moon was bright."), "Random sentence:")
        t |> success(line_is_a_field("Story: "), "Story:")
        t |> success(!line_is_a_field("Summ"), "the field name arrives token by token - a prefix of one is not a field yet")
        t |> success(!line_is_a_field("The summary of the day was a cake."), "prose that contains a field word is not a field")
        t |> success(!line_is_a_field("\"Words are fun,\" said the dragon."), "a quoted field word is not a field")
        t |> success(!line_is_a_field(""), "an empty line is not a field")
    }
}

//! the window server and every fixture file, each absence registering its own loud skip
def private ready(t : T?) : bool {
    if (!has_window_server()) {
        t |> skip("no window server (DISPLAY and WAYLAND_DISPLAY unset) - the example opens a window")
        return false
    }
    for (name in ["tinystories-instruct-27M-Q8_0.gguf", "kitten-nano.gguf"]) {
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

//! the example's smoke rail: the words the first story must use, read out to the end (the spawn's wall clock is the guard, not a frame cap)
def private run_storywish(words : string; var out : string&) : int {
    let prev_image = env_value_of("DASLLAMA_IMAGE")
    set_env_variable("DASLLAMA_IMAGE", "0")   //! never mint a sidecar beside the stocked models
    let root = get_das_root()
    let argv <- [example_daslang(), "-jit", "-dasroot", root, "{root}/examples/dasLLAMA/storywish/main.das", "--",
                 "--models", models_dir(), "--smoke", "--words", words, "--seed", "7", "--null-audio"]
    let rc = run_and_capture(argv, out, 300.0)
    set_env_variable("DASLLAMA_IMAGE", prev_image)
    return rc
}

def private witness(out : string) : string {
    var inscope lines <- split(out, "\n")
    return join([for (line in lines); line; where find(line, "storywish:") >= 0], "\n")
}

def private tail_of(out : string) : string {
    return slice(out, max(0, length(out) - 600))
}

[test]
def test_smoke_tells_the_wished_story(t : T?) {
    t |> run("the smoke rail tells a story from the words it was given and reads it out to the end") @(t : T?) {
        return if (!ready(t))
        var out = ""
        let rc = run_storywish(SMOKE_WORDS, out)
        return if (skip_without_window(t, rc, out))
        t |> equal(0, rc, "the smoke run exits clean: {tail_of(out)}")
        t |> success(find(out, START_LINE) >= 0, "the story starts at frame 0 with the words the rail named:\n{witness(out)}")
        t |> success(find(out, DONE_LINE) >= 0, "the story was written and read out to the end:\n{witness(out)}")
        let story = tail_of(out)
        t |> success(!empty(strip(story)), "the run prints the story it told")
    }
}
