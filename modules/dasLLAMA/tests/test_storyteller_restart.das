options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require daslib/fio
require daslib/strings_boost
require daslib/strings_convert
require strings
require math
require _model_tier
require _example_rail

// The storyteller's restart rails (examples/dasLLAMA/storyteller/main.das): a space press while a
// story is being written or read stops the clip and starts another, and the speech thread skips the
// queued sentences of the replaced story instead of synthesizing them first. Each cell spawns the
// example under --smoke with the rails that make its run a sequence of events, never a race:
// --seed pins the story, --hold-speech keeps the speech thread off its sentences until the story is
// written or replaced (so the sentences said before then are all queued when it opens), and the
// restart fires on an event - --restart-on-sentence N once the story has said N sentences,
// --restart-on-clip N as the Nth clip begins; --await-speech-story N pauses story N's writing after
// its first sentence until its first clip begins, so speech beside the writing is an event too. The cells read the witness lines the example and its
// speech thread log - which story each sentence the thread took belongs to, synthesized or skipped,
// in the thread's order - and never a time; the spawn's 300 s wall clock is a hang guard only.
// Model-gated on stories15M-Q8_0.gguf (its ../performance/model_specs.das row names the conversion),
// kitten-nano.gguf and the front-end packs under models_dir(); a run opens the example's window, so a
// box without a window server skips, and --null-audio keeps the run silent.

let STOP_LINE = "storyteller: the clip of story 1 stops, "
let REPLACE_LINE = "storyteller: story 1 is replaced with "
let WRITTEN_LINE = "storyteller: story 1 is written, "
let SKIP_LINE = "storyteller: a sentence of story 1 is skipped, story 2 is being told"
let SYNTH1_LINE = "storyteller: a sentence of story 1 is synthesized"
let SYNTH2_LINE = "storyteller: a sentence of story 2 is synthesized"
let DONE_LINE = "storyteller: story 2 written and read out"
let DONE1_LINE = "storyteller: story 1 written and read out"
let WRITTEN2_LINE = "storyteller: story 2 is written, "
let CLOSING_SKIP_LINE = "storyteller: a sentence of story 1 is skipped, the storyteller is closing"
let ANY_REPLACE = " is replaced with "
let CAP_LINE = "storyteller: the frame cap stopped story 1 after 30 frames"
let FIRST_START = "storyteller: story 1 starts at frame 0"
let RESTART_MARK = "storyteller: story 2 starts at frame "
let STORY_SEED = 1234   //! story 1 under this seed runs past CAP_FRAMES' tokens and says at least two sentences - the frame-cap and read-out cells assert both
let OTHER_SEED = 4321
let NEVER_SAID = 1000   //! a sentence count no story of MAX_TOKENS reaches: the sentence rail fires on the told story instead
let CAP_FRAMES = 30
let NO_CAP = 0
let PACK_FILES <- ["tts_g2p.bin", "tts_postag.bin"]

//! the window server and every fixture file, each absence registering its own loud skip
def private ready(t : T?) : bool {
    return false if (!example_ready(t, ["stories15M-Q8_0.gguf", "kitten-nano.gguf"]))
    for (name in PACK_FILES) {
        if (!stat(path_join(models_dir(), name)).is_valid) {
            t |> skip("{name} not present")
            return false
        }
    }
    return true
}

//! the example's smoke rail with `rail` after its silent-audio flag
def private run_storyteller(rail : array<string>; var out : string&) : int {
    var args <- ["--null-audio"]
    args |> push_from(rail)
    return run_example("storyteller/main.das", args, out)
}

//! the rail on the pinned story with the speech thread held: a space press once story 1 has said `restart_on_sentence`
//! sentences or as clip `restart_on_clip` begins (0 = none), a stop at `max_frames` (0 = none)
def private held_rail(restart_on_sentence, restart_on_clip, max_frames : int) : array<string> {
    var args <- ["--max-frames", "{max_frames}", "--hold-speech", "--seed", "{STORY_SEED}"]
    if (restart_on_sentence > 0) {
        args |> push("--restart-on-sentence")
        args |> push("{restart_on_sentence}")
    }
    if (restart_on_clip > 0) {
        args |> push("--restart-on-clip")
        args |> push("{restart_on_clip}")
    }
    return <- args
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

//! the sentence count a witness line `line` carries right after its text; -1 when the line is absent or does not parse
def private count_after(out, line : string) : int {
    let at = find(out, line)
    return -1 if (at < 0)
    let start = at + length(line)
    let stop = find(out, " sentence", start)
    return -1 if (stop < 0)
    let r = try_to_int(slice(out, start, stop))
    return r |> is_err ? -1 : r |> unwrap
}

def private witness(out : string) : string => example_witness(out, "storyteller")

//! the story text a capped smoke run prints after its cap line; "" when the line is absent
def private capped_story(out : string) : string {
    let at = find(out, CAP_LINE)
    return "" if (at < 0)
    let nl = find(out, "\n", at)
    return nl < 0 ? "" : strip(slice(out, nl + 1))
}

//! `first` is logged, and before `then` - one thread's log lines land in its order
def private logged_before(t : T?; out, first, then, what : string) {
    let a = find(out, first)
    let b = find(out, then)
    t |> success(a >= 0 && b >= 0 && a < b, "{what}:\n{witness(out)}")
}

//! the speech thread took every sentence of story 1 before the first of story 2: one thread pops the stream in order
def private story_1_first(t : T?; out : string) {
    let last_1 = max(rfind(out, SKIP_LINE), rfind(out, SYNTH1_LINE))
    let first_2 = find(out, SYNTH2_LINE)
    t |> success(first_2 >= 0, "the speech thread synthesized story 2:\n{witness(out)}")
    t |> success(last_1 < first_2, "the speech thread took story 1's sentences before story 2's:\n{witness(out)}")
}

[test]
def test_restart_while_reading(t : T?) {
    t |> run("a space press as story 1's first clip begins stops it, drops its other sentences, and story 2 is read out") @(t : T?) {
        return if (!ready(t))
        var out = ""
        let rc = run_storyteller(held_rail(0, 1, NO_CAP), out)   // the thread opens on the written story, so every sentence is queued ahead of clip 1
        return if (skip_without_window(t, rc, out))
        t |> equal(0, rc, "the smoke run exits clean: {example_tail(out)}")
        t |> equal(count_of(out, RESTART_MARK), 1, "one restart:\n{witness(out)}")
        let said = count_after(out, WRITTEN_LINE)
        t |> success(said >= 2, "the pinned story 1 says at least two sentences:\n{witness(out)}")
        t |> equal(count_of(out, STOP_LINE), 1, "story 1's clip was playing and stopped once:\n{witness(out)}")
        t |> equal(said - 1, count_after(out, STOP_LINE), "every sentence of story 1 but the playing one goes unheard:\n{witness(out)}")
        t |> equal(said - 1, count_after(out, REPLACE_LINE), "the restart replaced story 1 with all but one sentence unplayed:\n{witness(out)}")
        t |> equal(count_of(out, ANY_REPLACE), 1, "the first story's start replaces nothing:\n{witness(out)}")
        let synthesized = count_of(out, SYNTH1_LINE)
        t |> success(synthesized >= 1, "story 1's first sentence was synthesized:\n{witness(out)}")
        t |> equal(said, synthesized + count_of(out, SKIP_LINE), "each sentence of story 1 was synthesized or skipped, none lost:\n{witness(out)}")
        story_1_first(t, out)
        t |> success(find(out, DONE_LINE) >= 0, "story 2 was written and read out to the end:\n{witness(out)}")
    }
}

[test]
def test_restart_skips_the_replaced_story_queue(t : T?) {
    t |> run("a restart while the speech thread is held off its sentences skips every queued sentence of the replaced story") @(t : T?) {
        return if (!ready(t))
        var out = ""
        let rc = run_storyteller(held_rail(2, 0, NO_CAP), out)   // story 1 is still being written; the held thread has read none of it
        return if (skip_without_window(t, rc, out))
        t |> equal(0, rc, "the smoke run exits clean: {example_tail(out)}")
        t |> equal(count_of(out, RESTART_MARK), 1, "one restart:\n{witness(out)}")
        t |> equal(count_of(out, WRITTEN_LINE), 0, "story 1 was replaced while it was being written:\n{witness(out)}")
        let queued = count_after(out, REPLACE_LINE)
        t |> success(queued >= 2, "story 1 had said the sentences the rail waited for:\n{witness(out)}")
        t |> equal(count_of(out, STOP_LINE), 0, "no clip of story 1 began while the speech thread was held:\n{witness(out)}")
        t |> equal(count_of(out, SYNTH1_LINE), 0, "the speech thread synthesized no sentence of story 1:\n{witness(out)}")
        t |> equal(queued, count_of(out, SKIP_LINE), "the speech thread skipped every queued sentence of story 1:\n{witness(out)}")
        story_1_first(t, out)
        t |> success(find(out, DONE_LINE) >= 0, "story 2 was written and read out to the end:\n{witness(out)}")
    }
}

[test]
def test_frame_cap_and_no_restart_by_default(t : T?) {
    t |> run("the frame cap ends an unread story, and the rail restarts nothing on its own") @(t : T?) {
        return if (!ready(t))
        var out = ""
        let rc = run_storyteller(held_rail(0, 0, CAP_FRAMES), out)
        return if (skip_without_window(t, rc, out))
        t |> equal(0, rc, "the capped run exits clean: {example_tail(out)}")
        t |> success(find(out, FIRST_START) >= 0, "the first story starts at frame 0:\n{witness(out)}")
        t |> equal(count_of(out, WRITTEN_LINE), 0, "the pinned story 1 is still being written at the cap:\n{witness(out)}")
        t |> success(find(out, CAP_LINE) >= 0, "the cap line names the story and the frame it stopped at:\n{witness(out)}")
        t |> equal(count_of(out, SYNTH1_LINE), 0, "the held speech thread synthesized nothing before the cap:\n{witness(out)}")
        t |> success(count_of(out, CLOSING_SKIP_LINE) >= 1, "shutdown skipped the queued sentences as the storyteller closed:\n{witness(out)}")
        t |> equal(count_of(out, SKIP_LINE), 0, "no sentence was skipped for a story 2:\n{witness(out)}")
        t |> equal(count_of(out, RESTART_MARK), 0, "no restart rail, no second story:\n{witness(out)}")
    }
}

[test]
def test_seed_pins_the_story(t : T?) {
    t |> run("one --seed writes one story, another seed another") @(t : T?) {
        return if (!ready(t))
        var stories : array<string>
        stories |> reserve(3)
        for (seed in [STORY_SEED, STORY_SEED, OTHER_SEED]) {
            var out = ""
            let rc = run_storyteller(["--max-frames", "{CAP_FRAMES}", "--hold-speech", "--seed", "{seed}"], out)
            return if (skip_without_window(t, rc, out))
            t |> equal(0, rc, "the capped run exits clean: {example_tail(out)}")
            let story = capped_story(out)
            t |> success(length(story) > length("Once upon a time"), "seed {seed} wrote past the prompt by the cap:\n{example_tail(out)}")
            stories |> push(story)
        }
        t |> equal(stories[0], stories[1], "seed {STORY_SEED} twice, the same story")
        t |> success(stories[0] != stories[2], "seed {OTHER_SEED} writes another story:\n{stories[0]}\n---\n{stories[2]}")
    }
}

[test]
def test_sentence_rail_fires_on_a_told_story(t : T?) {
    t |> run("a sentence rail the story never reaches fires once the story is told, and its held sentences are skipped") @(t : T?) {
        return if (!ready(t))
        var out = ""
        let rc = run_storyteller(held_rail(NEVER_SAID, 0, NO_CAP), out)
        return if (skip_without_window(t, rc, out))
        t |> equal(0, rc, "the smoke run exits clean: {example_tail(out)}")
        let said = count_after(out, WRITTEN_LINE)
        t |> success(said >= 1 && said < NEVER_SAID, "story 1 was told short of the rail's count:\n{witness(out)}")
        t |> equal(count_of(out, RESTART_MARK), 1, "the told story restarted once:\n{witness(out)}")
        t |> equal(count_of(out, SYNTH1_LINE), 0, "the told story's gate stayed shut for the restart:\n{witness(out)}")
        t |> equal(said, count_of(out, SKIP_LINE), "every sentence of the told story was skipped for story 2:\n{witness(out)}")
        t |> success(find(out, DONE_LINE) >= 0, "story 2 was written and read out to the end:\n{witness(out)}")
    }
}

[test]
def test_speech_runs_beside_the_writing(t : T?) {
    t |> run("with no hold, the speech thread reads story 1 while it is being written") @(t : T?) {
        return if (!ready(t))
        var out = ""
        let rc = run_storyteller(["--seed", "{STORY_SEED}", "--await-speech-story", "1"], out)   // the writing waits for story 1's first clip
        return if (skip_without_window(t, rc, out))
        t |> equal(0, rc, "the smoke run exits clean: {example_tail(out)}")
        logged_before(t, out, SYNTH1_LINE, WRITTEN_LINE, "story 1's first sentence was synthesized while story 1 was being written")
        t |> equal(count_of(out, RESTART_MARK), 0, "no restart rail, no second story:\n{witness(out)}")
        t |> success(find(out, DONE1_LINE) >= 0, "story 1 was written and read out to the end:\n{witness(out)}")
    }
    t |> run("a restart opens the held speech thread while story 2 is being written") @(t : T?) {
        return if (!ready(t))
        var out = ""
        var rail <- held_rail(2, 0, NO_CAP)
        rail |> push_from(["--await-speech-story", "2"])   // story 2's writing waits for its first clip: only the restart's gate lets it on
        let rc = run_storyteller(rail, out)
        return if (skip_without_window(t, rc, out))
        t |> equal(0, rc, "the smoke run exits clean: {example_tail(out)}")
        t |> equal(count_of(out, RESTART_MARK), 1, "one restart:\n{witness(out)}")
        t |> equal(count_of(out, SYNTH1_LINE), 0, "the speech thread synthesized no sentence of story 1:\n{witness(out)}")
        t |> equal(count_after(out, REPLACE_LINE), count_of(out, SKIP_LINE), "the speech thread skipped every queued sentence of story 1:\n{witness(out)}")
        logged_before(t, out, SYNTH2_LINE, WRITTEN2_LINE, "story 2's first sentence was synthesized while story 2 was being written")
        t |> success(find(out, DONE_LINE) >= 0, "story 2 was written and read out to the end:\n{witness(out)}")
    }
}
