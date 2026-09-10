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
require ../../../examples/dasLLAMA/parrot/take.das

// Parrot (examples/dasLLAMA/parrot): the take's pure side (take.das) is tested directly - the
// resampler Silero hears through, the rule that ends a take, the clip's window, the silence
// countdown, the waveform's columns and the chunk ledger. The smoke cell spawns the example under --smoke with the
// tree's own clip in place of the microphone and reads the witness lines it logs: the voice is
// cloned from the clip, the text is said in it, and the say is read out to the end. Model-gated
// on pocket-tts-en-kq.gguf (the Pocket file with its codec encoder, the one the page ships) under
// models_dir(); the voice-activity model is the tree's own. The run opens the example's window
// for about half a minute, so a box without a window server skips, and --null-audio keeps it
// silent - and off the microphone, which --clip never opens anyway.

[test]
def test_vad_resampler(t : T?) {
    t |> run("24 kHz to 16 kHz over the whole take: fed in drains of any size, the stream is the one fed at once") @(t : T?) {
        var take : array<float>
        take |> resize(24000)
        for (i in range(24000)) {
            take[i] = sin(float(i) * 0.013) + 0.3 * cos(float(i) * 0.071)
        }
        var whole = VadResampler()
        var at_once : array<float>
        resample_step(whole, take, at_once)
        t |> equal(16000, length(at_once), "one second of 24 kHz is 16000 samples of 16 kHz (the last at position 23998.5, still inside the take)")
        t |> equal(at_once[0], take[0], "the first sample is the take's first")
        t |> success(abs(at_once[2] - take[3]) < 1.0e-6, "position 3.0 lands on the take's third sample")
        t |> success(abs(at_once[1] - 0.5 * (take[1] + take[2])) < 1.0e-6, "position 1.5 is the mean of its neighbours")
        var drained = VadResampler()
        var stream : array<float>
        var partial : array<float>
        var fed = 0
        var chunk = 37
        while (fed < 24000) {
            let n = min(chunk, 24000 - fed)
            partial |> resize(fed + n)
            for (i in range(fed, fed + n)) {
                partial[i] = take[i]
            }
            fed += n
            chunk = (chunk * 7 + 11) % 900 + 1   // drains of every size the device might hand over
            var piece : array<float>
            resample_step(drained, partial, piece)
            stream |> push_from(piece)
        }
        t |> equal(length(stream), length(at_once), "the same count")
        var diffs = 0
        for (a, b in stream, at_once) {
            diffs += a != b ? 1 : 0
        }
        t |> equal(diffs, 0, "sample for sample the same stream - no seam where the drains met")
    }
    t |> run("a detector index maps back to the take") @(t : T?) {
        t |> equal(vad_to_mic(16000l), 24000l)
        t |> equal(vad_to_mic(0l), 0l)
        t |> equal(vad_to_mic(512l), 768l)
    }
}

[test]
def test_take_rules(t : T?) {
    t |> run("the take ends two seconds after speech, at the cap, or when the device gave nothing for long") @(t : T?) {
        t |> success(!take_ends(false, false, 100.0, 10.0, 0.0), "no speech yet, nothing ends it")
        t |> success(!take_ends(true, true, 100.0, 10.0, 0.0), "speech still going")
        t |> success(!take_ends(true, false, 1.9, 10.0, 0.0), "quiet, not yet two seconds")
        t |> success(take_ends(true, false, 2.0, 10.0, 0.0), "quiet for two seconds")
        t |> success(take_ends(true, true, 0.0, 60.0, 0.0), "the cap, mid-speech")
        t |> success(take_ends(false, false, 0.0, 60.0, 0.0), "the cap, no speech")
        t |> success(!take_ends(false, false, 0.0, 0.0, 5.9), "a device silent for less than six seconds")
        t |> success(take_ends(false, false, 0.0, 0.0, 6.0), "a device that gave nothing for six seconds")
    }
    t |> run("the clip's window: the speech plus a pad, an open speech to the end, never past the cap") @(t : T?) {
        let pad = int64(TAKE_PAD_S * float(MIC_RATE))
        var w = clip_window(48000l, 96000l, 240000l, false)
        t |> equal(w.from, 48000l - pad, "a quarter second before the speech")
        t |> equal(w.to, 96000l + pad, "a quarter second after it")
        w = clip_window(2000l, 96000l, 240000l, false)
        t |> equal(w.from, 0l, "the pad never reaches before the take")
        w = clip_window(48000l, 96000l, 100000l, false)
        t |> equal(w.to, 100000l, "the pad never reaches past the take")
        w = clip_window(48000l, 0l, 240000l, true)
        t |> equal(w.to, 240000l, "speech still open at the stop runs to the take's end")
        let cap = int64(TAKE_CAP_S) * int64(MIC_RATE)
        w = clip_window(0l, 0l, cap + 24000l * 3l, true)
        t |> equal(w.to - w.from, cap, "a take past the cap yields a clip of exactly the cap")
        w = clip_window(240000l, 0l, 240000l, true)
        t |> success(w.to >= w.from, "an empty window is never negative")
    }
}

[test]
def test_panels(t : T?) {
    t |> run("the silence countdown: nothing before speech or during it, the seconds left after") @(t : T?) {
        t |> equal(countdown_left(false, false, 0.5), 0.0, "no speech yet")
        t |> equal(countdown_left(true, true, 0.5), 0.0, "speech still going")
        t |> equal(countdown_left(true, false, 0.5), SILENCE_ENDS_TAKE_S - 0.5, "quiet for half a second")
        t |> equal(countdown_left(true, false, 9.0), 0.0, "never below zero")
    }
    t |> run("the waveform's columns: min and max per column, grown in drains, the open column redone") @(t : T?) {
        var samples : array<float>
        samples |> resize(1000)
        for (i in range(1000)) {
            samples[i] = float(i % 100) / 100.0 - 0.5
        }
        var at_once : array<float2>
        peaks_append(at_once, samples, 0l, 100)
        t |> equal(length(at_once), 10, "ten full columns of a hundred")
        t |> equal(at_once[0].x, -0.5, "the column's lowest sample")
        t |> equal(at_once[0].y, 0.49, "and its highest")
        var grown : array<float2>
        var partial : array<float>
        var fed = 0
        var step = 37
        while (fed < 1000) {
            let n = min(step, 1000 - fed)
            partial |> resize(fed + n)
            for (i in range(fed, fed + n)) {
                partial[i] = samples[i]
            }
            let from = int64(fed)
            fed += n
            step = (step * 7 + 11) % 260 + 1
            peaks_append(grown, partial, from, 100)
        }
        t |> equal(length(grown), length(at_once), "the same count")
        var diffs = 0
        for (a, b in grown, at_once) {
            diffs += (a.x != b.x || a.y != b.y) ? 1 : 0
        }
        t |> equal(diffs, 0, "column for column the same picture - a column open at a drain's end is redone by the next")
        var tail : array<float2>
        peaks_append(tail, samples, 950l, 100)
        t |> equal(length(tail), 10, "a start inside the last column redoes that column alone")
    }
    t |> run("a say is read out when every chunk played, and never before it has any") @(t : T?) {
        let none : array<ChunkRow>
        t |> success(!all_spoken(none), "no chunks yet")
        var rows <- [ChunkRow(state = ChunkState.spoken), ChunkRow(state = ChunkState.speaking)]
        t |> success(!all_spoken(rows), "one still speaking")
        rows[1].state = ChunkState.spoken
        t |> success(all_spoken(rows), "all played")
    }
}

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
                 "--models", models_dir(), "--smoke", "--clip", path_join(root, CLIP), "--text", SMOKE_TEXT]
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

//! the read-out line's two figures: "... is read out - S s of speech over P s of playback"
def private read_out_figures(out : string) : tuple<speech : float; playback : float> {
    var inscope lines <- split(out, "\n")
    for (line in lines) {
        continue if (find(line, READ_LINE) < 0)
        var inscope words <- split(line, " ")
        var inscope got <- [for (i in range(length(words) - 1)); to_float(words[i]); where words[i + 1] == "s" && (i + 2 >= length(words) || words[i + 2] == "of")]
        if (length(got) >= 2) {
            return (speech = got[0], playback = got[1])
        }
    }
    return (speech = 0.0, playback = 0.0)
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
        let figures = read_out_figures(out)
        t |> success(figures.speech > 2.0, "the say carries speech ({figures.speech} s):\n{witness(out)}")
        t |> success(figures.playback >= figures.speech * 0.8, "the say played to its end ({figures.playback} s of playback for {figures.speech} s of speech): a clip read as stopped before its end cuts the say short\n{witness(out)}")
        t |> success(find(out, DONE_LINE) >= 0, "the rail ended on the read-out, not the frame cap:\n{witness(out)}")
    }
    t |> run("a --clip that is not audio is refused by name, before any window opens") @(t : T?) {
        return if (!ready(t))
        var out = ""
        let root = get_das_root()
        let argv <- [example_daslang(), "-jit", "-dasroot", root, "{root}/examples/dasLLAMA/parrot/main.das", "--",
                     "--models", models_dir(), "--smoke", "--clip", "{root}/examples/dasLLAMA/parrot/models.json"]
        let rc = run_and_capture(argv, out, 60.0)
        t |> success(rc != 0, "a clip that does not decode is a refusal, not a run")
        t |> success(find(out, "did not decode") >= 0, "and it says so by name:\n{tail_of(out)}")
    }
}
