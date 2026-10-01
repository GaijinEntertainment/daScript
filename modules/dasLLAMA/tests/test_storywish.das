options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require daslib/strings_boost
require strings
require _model_tier
require _example_rail
require ../../../examples/dasLLAMA/storywish/wish.das

// Storywish (examples/dasLLAMA/storywish): the request side is pure and tested directly - the
// typed line becomes the words the corpus spells, the words become the prompt in the corpus's
// layout, and a line the model writes is recognized as a field the corpus puts after a story.
// The smoke cell spawns the example under --smoke with fixed words and reads the witness lines it
// logs: the story starts with those words and is written and read out to the end. Model-gated on
// tinystories-instruct-27M-Q8_0.gguf (its ../performance/model_specs.das row names the source)
// and pocket-tts-en-stuart-kq.gguf (the one-voice Pocket file the page ships) under models_dir();
// the run opens the example's window for about a minute, so a box without a window server
// skips, and --null-audio keeps it silent.

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
def private ready(t : T?) : bool => example_ready(t, ["tinystories-instruct-27M-Q8_0.gguf", "pocket-tts-en-stuart-kq.gguf"])

//! the example's smoke rail: the words the first story must use, read out to the end (the spawn's wall clock is the guard, not a frame cap)
def private run_storywish(words : string; var out : string&) : int => run_example("storywish/main.das", ["--words", words, "--seed", "7", "--null-audio"], out)

def private witness(out : string) : string => example_witness(out, "storywish")

[test]
def test_smoke_tells_the_wished_story(t : T?) {
    t |> run("the smoke rail tells a story from the words it was given and reads it out to the end") @(t : T?) {
        return if (!ready(t))
        var out = ""
        let rc = run_storywish(SMOKE_WORDS, out)
        return if (skip_without_window(t, rc, out))
        t |> equal(0, rc, "the smoke run exits clean: {example_tail(out)}")
        t |> success(find(out, START_LINE) >= 0, "the story starts at frame 0 with the words the rail named:\n{witness(out)}")
        t |> success(find(out, DONE_LINE) >= 0, "the story was written and read out to the end:\n{witness(out)}")
        let story = example_tail(out)
        t |> success(!empty(strip(story)), "the run prints the story it told")
    }
}
