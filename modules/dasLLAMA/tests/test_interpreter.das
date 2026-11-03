options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require daslib/fio
require daslib/strings_boost
require strings
require math
require _model_tier
require _example_rail
require ../../../examples/dasLLAMA/interpreter/turn.das

// Interpreter (examples/dasLLAMA/interpreter): the listen's pure side (turn.das) is tested directly - the utterance's
// window, the cap and the idle rule, the drain's gain append, clip and cut decision, the turn ledger. The smoke cell spawns the example under --smoke with a Russian clip
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
def test_listen_drain(t : T?) {
    t |> run("frames append under the gain, clamped, and the drain reads their rms") @(t : T?) {
        var raw <- [0.1]
        let rms = append_gained(raw, [0.5, -0.9, 0.2, 7.0], 3, 2.0)
        t |> equal(4, length(raw), "three frames after the one already there; the fourth frame is past n")
        t |> equal(0.1, raw[0], "the earlier frame kept")
        t |> equal(1.0, raw[1], "0.5 x 2 clamps to 1")
        t |> equal(-1.0, raw[2], "-0.9 x 2 clamps to -1")
        t |> equal(0.4, raw[3], "0.2 x 2 passes")
        t |> success(abs(rms - sqrt(2.16 / 3.0)) < 1e-6, "the rms of the appended frames alone ({rms})")
        var plain : array<float>
        let rms1 = append_gained(plain, [0.5, -0.9], 2, 1.0)
        t |> equal(-0.9, plain[1], "at unit gain nothing clamps (the clamp's control)")
        t |> success(abs(rms1 - sqrt(1.06 / 2.0)) < 1e-6, "and the rms follows ({rms1})")
        delete raw
        delete plain
    }
    t |> run("the clip is the take's samples over the padded window") @(t : T?) {
        let pad = int64(UTTERANCE_PAD_S * float(LISTEN_RATE))
        var take <- [for (i in range(40000)); float(i)]
        var clip <- utterance_clip(take, 10000l, 20000l)
        t |> equal(10000l + 2l * pad, long_length(clip), "the speech plus both pads")
        t |> equal(float(10000l - pad), clip[0], "from the pad before the speech")
        t |> equal(float(20000l + pad - 1l), clip[length(clip) - 1], "to the pad after it")
        var late <- utterance_clip(take, 39000l, 40000l)
        t |> equal(40000.0 - 1.0, late[length(late) - 1], "a window past the take ends on its last sample")
        delete take
        delete clip
        delete late
    }
    t |> run("a drain's boundaries open the speech once, close it inside the take, and name the cut") @(t : T?) {
        let total = 160000l
        var s = speech_mark(SpeechSpan(), true, -5l, total)
        t |> success(s.open && s.start == 0l, "a start before the take opens at 0")
        s = speech_mark(s, true, 9000l, total)
        t |> equal(0l, s.start, "a second start leaves the open speech where it began")
        t |> equal(ListenCut.none, listen_cut(s, 16000l), "open speech under the cap waits")
        s = speech_mark(s, false, total + 50l, total)
        t |> equal(total, s.end, "an end past the take closes at its end")
        t |> success(!s.open, "and the speech is closed")
        t |> equal(ListenCut.at_end, listen_cut(s, total), "a closed speech is cut at its end")
        s = speech_mark(s, true, total - 2000l, total)
        t |> equal(total, s.next_start, "a start behind the close names the next speech, no earlier than the close")
        t |> equal(ListenCut.at_end, listen_cut(s, total), "and the closed speech is still the cut")
        let behind = span_after_cut(s, s.end)
        t |> success(behind.open && behind.start == 0l, "behind the cut the next speech is open from the kept take's start")
        let alone = span_after_cut(SpeechSpan(start = 100l, end = 9000l), 9000l)
        t |> success(!alone.open && alone.start == 0l, "with no start behind the close nothing is open")
        let capped = span_after_cut(SpeechSpan(open = true, start = 100l), total)
        t |> success(capped.open && capped.start == 0l, "speech cut at the cap runs on from the kept take's start")
        let shut = speech_mark(SpeechSpan(), false, 3000l, total)
        t |> equal(-1l, shut.end, "an end with no speech open closes nothing")
        t |> equal(ListenCut.none, listen_cut(shut, 16000l), "and a short quiet take waits")
        let cap = int64(UTTERANCE_CAP_S * float(LISTEN_RATE))
        t |> equal(ListenCut.at_cap, listen_cut(SpeechSpan(open = true, start = 100l), 100l + cap), "open speech that ran the cap is cut there")
        let idle = int64(LISTEN_IDLE_S * float(LISTEN_RATE))
        t |> equal(ListenCut.idle, listen_cut(SpeechSpan(), idle), "a quiet take at the idle bound is dropped")
        t |> equal(ListenCut.none, listen_cut(SpeechSpan(open = true, start = idle - 10l), idle), "a take with speech open never idles")
    }
}

[test]
def test_turn_ledger(t : T?) {
    t |> run("turns on their way are counted, the spoken and the empty are not, and a turn is found by its number") @(t : T?) {
        var turns : array<TurnRow> <- [TurnRow(no = 1, state = TurnState.spoken), TurnRow(no = 2, state = TurnState.empty), TurnRow(no = 3, state = TurnState.translating), TurnRow(no = 4, state = TurnState.transcribing)]
        t |> equal(2, turns_in_flight(turns), "two on their way")
        t |> equal(2, turn_index(turns, 3), "turn 3 sits third")
        t |> equal(-1, turn_index(turns, 9), "a number the ledger has none of")
        t |> success(turn_in_flight(TurnState.speaking), "speaking is on its way")
        t |> success(!turn_in_flight(TurnState.spoken), "spoken is not")
        delete turns
    }
    t |> run("a landed transcript moves the turn to translating, a landed translation to speaking, no words to empty") @(t : T?) {
        let heard = land_text(TurnRow(no = 1, state = TurnState.transcribing), true, "privet", 1200l)
        t |> success(heard.ru == "privet" && heard.asr_us == 1200l && heard.state == TurnState.translating, "the transcript and its time, on to translating")
        t |> success(empty(heard.en) && heard.nmt_us == 0l, "the translation's fields untouched")
        let said = land_text(heard, false, "hello", 800l)
        t |> success(said.en == "hello" && said.nmt_us == 800l && said.state == TurnState.speaking, "the translation and its time, on to speaking")
        t |> equal("privet", said.ru, "the transcript kept")
        t |> equal(TurnState.empty, land_text(heard, false, "", 5l).state, "a translation with no words ends the turn empty")
        t |> equal(TurnState.empty, land_text(TurnRow(no = 2), true, "", 5l).state, "a transcript with no words ends the turn empty")
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
    t |> run("a speech model that does not load ends the smoke run on the worker's error, by name") @(t : T?) {
        return if (!ready(t))
        var out = ""
        let rc = run_example("interpreter/main.das", ["--clip", asr_clip("ru_short.wav"), "--asr-model", "absent-asr.gguf"], out)
        return if (skip_without_window(t, rc, out))
        t |> success(rc != 0, "the run is a refusal, not a run that waits on the models")
        t |> success(find(out, "the models did not load") >= 0 && find(out, "absent-asr.gguf") >= 0, "the worker's error reached the frame thread, naming the file:\n{example_tail(out)}")
    }
}
