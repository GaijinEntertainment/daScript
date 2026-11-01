options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require daslib/fio
require daslib/strings_boost
require strings
require _model_tier
require _example_rail
require ../../../examples/dasLLAMA/interpreter/turn.das

// Interpreter (examples/dasLLAMA/interpreter): the listen's pure side (turn.das) is tested directly - the utterance's
// window, the cap and the idle rule, the turn ledger. The smoke cell spawns the example under --smoke with a Russian clip
// in place of the microphone and reads the witness lines it logs: the clip is heard, translated, spoken and the turn is
// done. Model-gated on the GigaAM q4_k carrier, the Bergamot ru-en student and the Pocket one-voice file under
// models_dir(), and on the ASR corpus's ru_short.wav; the voice-activity model is the tree's own. The run opens the
// example's window for about a minute and plays the English through the mixer, so a box without a window server skips.

[test]
def test_utterance_window(t : T?) {
    t |> run("the window is the speech plus a quarter second at each end, inside the take") @(t : T?) {
        let pad = int64(UTTERANCE_PAD_S * float(LISTEN_RATE))
        let w = utterance_window(16000l, 48000l, 160000l)
        t |> equal(16000l - pad, w.from, "the pad before the speech")
        t |> equal(48000l + pad, w.to, "the pad after it")
        let early = utterance_window(1000l, 48000l, 160000l)
        t |> equal(0l, early.from, "a pad past the take's start stops at 0")
        let late = utterance_window(100000l, 159000l, 160000l)
        t |> equal(160000l, late.to, "a pad past the take's end stops at the end")
    }
    t |> run("a window never runs past the cap") @(t : T?) {
        let cap = int64(UTTERANCE_CAP_S * float(LISTEN_RATE))
        let w = utterance_window(0l, cap * 3l, cap * 4l)
        t |> equal(cap, w.to - w.from, "the cap's samples")
        t |> success(speech_at_cap(0l, cap), "speech that has run the cap is cut")
        t |> success(!speech_at_cap(0l, cap - 1l), "one sample short of it is not")
    }
    t |> run("a take with no speech idles out, one with speech open never does") @(t : T?) {
        let idle = int64(LISTEN_IDLE_S * float(LISTEN_RATE))
        t |> success(is_idle_take(false, idle), "the quiet take at the idle bound")
        t |> success(!is_idle_take(false, idle - 1l), "under it")
        t |> success(!is_idle_take(true, idle * 2l), "speech open, however long")
    }
}

[test]
def test_turn_ledger(t : T?) {
    t |> run("turns on their way are counted, the spoken and the empty are not, and a turn is found by its number") @(t : T?) {
        var turns : array<TurnRow> <- [TurnRow(no = 1, state = TurnState.spoken), TurnRow(no = 2, state = TurnState.empty), TurnRow(no = 3, state = TurnState.translating), TurnRow(no = 4, state = TurnState.heard)]
        t |> equal(2, turns_in_flight(turns), "two on their way")
        t |> equal(2, turn_index(turns, 3), "turn 3 sits third")
        t |> equal(-1, turn_index(turns, 9), "a number the ledger has none of")
        t |> success(turn_in_flight(TurnState.speaking), "speaking is on its way")
        t |> success(!turn_in_flight(TurnState.spoken), "spoken is not")
        delete turns
    }
}

let private HEARD_LINE = "interpreter: heard 1:"
let private TRANSLATED_LINE = "interpreter: translated 1:"
let private SPOKE_LINE = "interpreter: spoke 1 -"
let private TURN_LINE = "interpreter: turn 1 is done"
let private DONE_LINE = "interpreter: the clip was interpreted"
let private NMT = "bergamot/ru-en-base-memory/model.ruen.intgemm.alphas.bin"

def private ready(t : T?) : bool {
    return false if (!example_ready(t, ["gigaam-v3-e2e-rnnt-q4_k.gguf", NMT, "pocket-tts-en-18m-stuart-kq.gguf"]))
    if (!stat(asr_clip("ru_short.wav")).is_valid) {
        t |> skip("ru_short.wav not present under the ASR corpus")
        return false
    }
    return true
}

def private witness(out : string) : string => example_witness(out, "interpreter")

//! the English the run spoke: the translated line's text
def private translated(out : string) : string {
    var inscope lines <- split(out, "\n")
    for (line in lines) {
        let at = find(line, TRANSLATED_LINE)
        return strip(slice(line, at + length(TRANSLATED_LINE))) if (at >= 0)
    }
    return ""
}

[test]
def test_smoke_hears_translates_and_speaks(t : T?) {
    t |> run("the smoke rail hears the clip, translates it, speaks the English and ends on the spoken turn") @(t : T?) {
        return if (!ready(t))
        var out = ""
        let rc = run_example("interpreter/main.das", ["--clip", asr_clip("ru_short.wav")], out)
        return if (skip_without_window(t, rc, out))
        t |> equal(0, rc, "the smoke run exits clean: {example_tail(out)}")
        t |> success(find(out, HEARD_LINE) >= 0, "the clip was heard:\n{witness(out)}")
        t |> success(find(out, TRANSLATED_LINE) >= 0, "and translated:\n{witness(out)}")
        let en = translated(out)
        t |> success(find(en, "Wednesday") >= 0 || find(en, "competition") >= 0, "the English carries the clip's words ({en})")
        t |> success(find(out, SPOKE_LINE) >= 0, "and spoken:\n{witness(out)}")
        t |> success(find(out, TURN_LINE) >= 0, "the turn is done:\n{witness(out)}")
        t |> success(find(out, DONE_LINE) >= 0, "the rail ended on the spoken turn, not the frame cap:\n{witness(out)}")
    }
    t |> run("a --clip that is not audio is refused by name, before any window opens") @(t : T?) {
        return if (!ready(t))
        var out = ""
        let root = get_das_root()
        let argv <- [example_daslang(), "-jit", "-dasroot", root, "{root}/examples/dasLLAMA/interpreter/main.das", "--",
                     "--models", models_dir(), "--smoke", "--clip", "{root}/examples/dasLLAMA/interpreter/models.json"]
        let rc = run_and_capture(argv, out, 240.0)   // the cap covers a cold JIT of the whole facade (ASR, translation, TTS), past a minute on a slow box
        t |> success(rc != 0, "a clip that does not decode is a refusal, not a run")
        t |> success(find(out, "did not decode") >= 0, "and it says so by name:\n{example_tail(out)}")
    }
}
