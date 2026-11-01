options gen2
options _dasllama_internal = true
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends
require dastest/testing_boost public
require strings
require dasllama/dasllama_whisper
require dasllama/dasllama_parakeet
require dasllama/dasllama_gigaam
require dasllama/dasllama_canary
require daslib/defer
require dasllama/dasllama_gemma4a
require dasllama/dasllama_qwen3a
require dasllama/dasllama_asr   // set_asr_kv / get_asr_kv - the decoder K/V knob
require dasllama/dasllama_common   // KVDtype
require dasllama/dasllama_audio   // audio_tower_gpu_serves — the qwen3a lane policy's own predicate

// The family-owned halves of the ASR facade verbs (asr_exec_fmt / asr_encode_bucket), fed
// constructed structs — the moved-bit test the extraction owes. Plus parakeet's SPM detokenizer
// and GigaAM's charwise and SPM ones over hand-built vocabs.

// A parakeet model carrying nothing but a toy SPM vocab — parakeet_detokenize reads m.vocab only.
// ▁ is U+2581, the SentencePiece space marker.
def private toy_parakeet() : ParakeetModel {
    var m = ParakeetModel()
    m.vocab <- ["▁hello", "▁▁▁world", "▁▁", "there", "▁tail", "<pad>"]   // nolint:STYLE039 (vocabulary data, not prose)
    return <- m
}

def private detok(m : ParakeetModel; ids : array<int>) : string {
    var toks <- [for (i in ids); ParakeetToken(id = i)]
    let s = parakeet_detokenize(m, toks)
    delete toks
    return s
}

//! A GigaAM model carrying nothing but a toy vocab - gigaam_detokenize reads m.vocab and m.spm only.
def private toy_gigaam(spm : bool) : GigaamModel {
    var m = GigaamModel(spm = spm)
    if (spm) {
        m.vocab <- ["▁При", "вет", "▁мир", ",", "<unk>"]   // nolint:STYLE039 (vocabulary data, not prose)
    } else {
        m.vocab <- [" ", "а", "б", "в"]   // nolint:STYLE039 (Russian vocabulary data, not prose)
    }
    return <- m
}

def private ga_detok(m : GigaamModel; ids : array<int>) : string {
    var toks <- [for (i in ids); GigaamToken(id = i)]
    let s = gigaam_detokenize(m, toks)
    delete toks
    return s
}

[test]
def test_gigaam_detokenize(t : T?) {
    t |> run("charwise pieces join as they are, spaces included") @(t : T?) {
        var inscope m <- toy_gigaam(false)
        t |> equal(ga_detok(m, [1, 2, 0, 3]), "аб в", "the space piece is a space")   // nolint:STYLE039 (Russian vocabulary data, not prose)
        t |> equal(ga_detok(m, [0, 1]), " а", "a leading space piece stays - charwise strips nothing")   // nolint:STYLE039 (Russian vocabulary data, not prose)
    }
    t |> run("SentencePiece pieces: the mark becomes a space, the leading one dropped, control pieces vanish") @(t : T?) {
        var inscope m <- toy_gigaam(true)
        t |> equal(ga_detok(m, [0, 1, 3, 2]), "Привет, мир", "a word split over two pieces, then punctuation")   // nolint:STYLE039 (Russian vocabulary data, not prose)
        t |> equal(ga_detok(m, [0, 1, 4, 3]), "Привет,", "a <...> control piece renders as nothing")   // nolint:STYLE039 (Russian vocabulary data, not prose)
        t |> equal(ga_detok(m, [4, 2]), " мир", "a leading control piece is the first piece: the word after it keeps its space")   // nolint:STYLE039 (Russian vocabulary data, not prose)
        var none : array<int>
        t |> equal(ga_detok(m, none), "", "no tokens, no transcript")
        delete none
    }
}

def private caps_of(langs : array<string>) : AsrCaps {
    var c = AsrCaps()
    c.languages := langs
    return <- c
}

[test]
def test_asr_default_lang(t : T?) {
    t |> run("no language asked: the one code a sheet lists, auto where it lists none, English where it lists several") @(t : T?) {
        let none : array<string>
        var inscope one <- caps_of(["ru"])
        var inscope zero <- caps_of(none)
        var inscope many <- caps_of(["en", "de", "fr"])
        t |> equal(asr_default_lang(one), "ru")
        t |> equal(asr_default_lang(zero), "auto")
        t |> equal(asr_default_lang(many), "en")
    }
    t |> run("the GigaAM sheet resolves to Russian") @(t : T?) {
        var inscope m = GigaamModel()
        var inscope c <- gigaam_asr_caps(m)
        t |> equal(asr_default_lang(c), "ru")
    }
}

[test]
def test_gigaam_check_lang(t : T?) {
    t |> run("ru, \"\" and auto are taken; any other code is refused naming Russian") @(t : T?) {
        var inscope m = GigaamModel()
        for (lang in ["ru", "", "auto"]) {
            let taken = panic_text() ${
                gigaam_check_lang(m, lang)
            }
            t |> equal(taken, "", "'{lang}' taken")
        }
        let why = panic_text() ${
            gigaam_check_lang(m, "en")
        }
        t |> success(find(why, "transcribes Russian (ru)") >= 0, "'en' refused: {why}")
    }
}

[test]
def test_family_exec_fmt_verbs(t : T?) {
    t |> run("each family verb stamps its own precision") @(t : T?) {
        var w = WhisperModel()
        w.enc.q8 = true
        w.dq8 = false
        t |> equal(whisper_exec_fmt(w), "q8/f32", "whisper mixed rail")
        w.dq8 = true
        t |> equal(whisper_exec_fmt(w), "q8/q8", "whisper q8 rail")
        var p = ParakeetModel()
        p.q8 = true
        t |> equal(parakeet_exec_fmt(p), "q8", "parakeet single carrier")
        var ga = GigaamModel()
        ga.q8 = true
        t |> equal(gigaam_exec_fmt(ga), "q8", "gigaam q8 lane")
        ga.q8 = false
        ga.gemm_f16 = true
        t |> equal(gigaam_exec_fmt(ga), "f16", "gigaam exact lane names the plane its block GEMMs serve: the carrier's f16")
        ga.gemm_f16 = false
        t |> equal(gigaam_exec_fmt(ga), "f32", "gigaam exact lane over a carrier with no f16 block GEMMs: f32")
        ga.kq_lane = true
        ga.kq_fmt = int(KqFmt.k4)
        t |> equal(gigaam_exec_fmt(ga), "k4", "gigaam kq lane names its one K-quant member")
        ga.kq_fmt = int(KqFmt.q8)
        t |> equal(gigaam_exec_fmt(ga), "q8", "gigaam kq lane over a carrier whose every block GEMM is Q8_0: the exact blocks, named q8")
        ga.kq_fmt = -1
        t |> equal(gigaam_exec_fmt(ga), "kq", "gigaam kq lane over mixed K-quant members: kq")
        delete ga
        var c = CanaryEncoder()
        c.q8 = true
        t |> equal(canary_exec_fmt(c, "q8"), "q8/q8", "canary q8 rail")
        c.q8 = false
        t |> equal(canary_exec_fmt(c, "q8"), "f32/q8", "canary fp32 parity encoder")
        var g = Gemma4aEncoder()
        g.q8 = true
        t |> equal(gemma4a_exec_fmt(g, "q8"), "q8/q8", "gemma4a q8 tower")
        g.q8 = false
        t |> equal(gemma4a_exec_fmt(g, "q8"), "f32/q8", "gemma4a fp32 parity encoder")
        var q = Qwen3aTower()
        q.core.q8 = true
        t |> equal(qwen3a_exec_fmt(q, "q8"), "q8/q8", "qwen3a q8 core")
        q.core.q8 = false
        t |> equal(qwen3a_exec_fmt(q, "q8"), "f32/q8", "qwen3a fp32 parity core")
    }
}

// the audio families' lane policies themselves - model-free: the un-pinned default follows
// whether that family's Metal driver would serve, the pin flips it either way, reset returns.
// Each assert names the predicate the policy consults, never a hardcoded lane.
[test]
def test_audio_lane_knobs(t : T?) {
    t |> run("qwen3a: the q8 lane pin, and reset back to the policy") @(tt : T?) {
        tt |> equal(qwen3a_serves_q8(), !audio_tower_gpu_serves(), "the default follows the Metal tower")
        set_qwen3a_q8(true)
        tt |> success(qwen3a_serves_q8(), "pinned q8")
        set_qwen3a_q8(false)
        tt |> success(!qwen3a_serves_q8(), "pinned exact")
        reset_qwen3a_q8()   // from the EXACT pin, so a no-op reset cannot pass as a q8 policy default
        tt |> equal(qwen3a_serves_q8(), !audio_tower_gpu_serves(), "reset returns to the policy")
    }
    t |> run("gemma4a: the q8 lane pin, and reset back to the policy") @(tt : T?) {
        tt |> equal(gemma4a_serves_q8(), !gemma4a_gpu_serves(), "the default follows the Metal Conformer driver")
        set_gemma4a_q8(true)
        tt |> success(gemma4a_serves_q8(), "pinned q8")
        set_gemma4a_q8(false)
        tt |> success(!gemma4a_serves_q8(), "pinned exact")
        reset_gemma4a_q8()
        tt |> equal(gemma4a_serves_q8(), !gemma4a_gpu_serves(), "reset returns to the policy")
    }
    t |> run("canary: the q8 lane pin, and reset back to the policy") @(tt : T?) {
        tt |> equal(canary_enc_serves_q8(), !canary_gpu_serves(), "the default follows the Metal FastConformer driver")
        set_canary_enc_q8(true)
        tt |> success(canary_enc_serves_q8(), "pinned q8")
        set_canary_enc_q8(false)
        tt |> success(!canary_enc_serves_q8(), "pinned exact")
        reset_canary_enc_q8()
        tt |> equal(canary_enc_serves_q8(), !canary_gpu_serves(), "reset returns to the policy")
    }
    t |> run("parakeet: the q8 lane pin, and reset back to the policy") @(tt : T?) {
        tt |> equal(parakeet_serves_q8(), !parakeet_gpu_serves(), "the default follows the Metal FastConformer driver")
        {
            defer() {
                reset_parakeet_q8()
            }
            set_parakeet_q8(true)
            tt |> success(parakeet_serves_q8(), "pinned q8")
            set_parakeet_q8(false)
            tt |> success(!parakeet_serves_q8(), "pinned exact")
        }
        tt |> equal(parakeet_serves_q8(), !parakeet_gpu_serves(), "the pin's scope ends on the policy")
    }
    t |> run("gigaam: the q8 lane pin, and reset back to the policy") @(tt : T?) {
        tt |> equal(gigaam_serves_q8(), !gigaam_gpu_exact_serves(), "the default follows the GPU Conformer driver's faster lane")
        {
            defer() {
                reset_gigaam_q8()
            }
            set_gigaam_q8(true)
            tt |> success(gigaam_serves_q8(), "pinned q8")
            set_gigaam_q8(false)
            tt |> success(!gigaam_serves_q8(), "pinned exact")
        }
        tt |> equal(gigaam_serves_q8(), !gigaam_gpu_exact_serves(), "the pin's scope ends on the policy")
    }
    t |> run("the ASR decoder's K/V dtype: f16 by default, the setter's value read back, put back as found") @(tt : T?) {
        let found = get_asr_kv()
        tt |> equal(KVDtype.f16, found, "the serving default")
        set_asr_kv(KVDtype.f32)
        tt |> equal(KVDtype.f32, get_asr_kv(), "the parity rail the oracle cells pin")
        set_asr_kv(KVDtype.q8_0)
        tt |> equal(KVDtype.q8_0, get_asr_kv(), "another codec")
        set_asr_kv(found)
        tt |> equal(found, get_asr_kv(), "put back as found")
    }
}

[test]
def test_family_encode_buckets(t : T?) {
    t |> run("encode-total buckets are family facts") @(t : T?) {
        t |> equal(whisper_encode_bucket(), "wh.encode_total")
        t |> equal(canary_encode_bucket(), "cn.encode")
        t |> equal(gemma4a_encode_bucket(), "g4a.encode")
        t |> equal(PARAKEET_ENCODE_BUCKET, "pk.encode_total")
        t |> equal(GIGAAM_ENCODE_BUCKET, "ga.encode_total")
        t |> equal(qwen3a_encode_bucket(false), "q3a.encode")
        t |> equal(qwen3a_encode_bucket(true), "q3o.encode")
    }
    t |> run("the facade's asr_encode_bucket answers each family's own bucket") @(t : T?) {
        for (kind, want in [AsrKind.whisper, AsrKind.canary, AsrKind.gemma4a, AsrKind.parakeet, AsrKind.gigaam, AsrKind.qwen3a, AsrKind.qwen3omni],
                ["wh.encode_total", "cn.encode", "g4a.encode", "pk.encode_total", "ga.encode_total", "q3a.encode", "q3o.encode"]) {
            var inscope m <- AsrModel(kind = kind)
            t |> equal(asr_encode_bucket(m), want, "{kind}")
        }
    }
}

[test]
def test_parakeet_detokenize(t : T?) {
    t |> run("the FIRST piece drops the spaces its ▁ prefix produced") @(t : T?) {   // nolint:STYLE039
        var inscope m <- toy_parakeet()
        t |> equal(detok(m, [0]), "hello", "one leading mark")
        t |> equal(detok(m, [1]), "world", "three leading marks")
        t |> equal(detok(m, [2]), "", "an all-marks first piece detokenizes to nothing")
    }
    t |> run("a LATER piece keeps the space its ▁ prefix produced") @(t : T?) {   // nolint:STYLE039
        var inscope m <- toy_parakeet()
        t |> equal(detok(m, [0, 4]), "hello tail", "second piece keeps its single space")
        t |> equal(detok(m, [0, 1]), "hello   world", "and keeps all three of them")
        // the all-marks piece emptied out at position 0; at position 1 it is two real spaces
        t |> equal(detok(m, [2, 4]), " tail", "an emptied first piece does not re-arm the strip")
        t |> equal(detok(m, [3, 2]), "there  ", "a later all-marks piece stays two spaces")
    }
    t |> run("pieces with no ▁ prefix pass through; control pieces render as nothing") @(t : T?) {   // nolint:STYLE039
        var inscope m <- toy_parakeet()
        t |> equal(detok(m, [3]), "there", "a bare first piece is untouched")
        t |> equal(detok(m, [0, 5]), "hello", "a <...> control piece drops out")
    }
    t |> run("an empty token list detokenizes to the empty transcript") @(t : T?) {
        var inscope m <- toy_parakeet()
        var none : array<int>
        t |> equal(detok(m, none), "", "no tokens, no transcript")
        delete none
    }
}
