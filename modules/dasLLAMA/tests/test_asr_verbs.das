options gen2
options _dasllama_internal = true
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends
require dastest/testing_boost public
require strings
require daslib/strings_boost   // count: the knob's lines a child printed
require dasllama/dasllama_whisper
require dasllama/dasllama_parakeet
require dasllama/dasllama_gigaam
require dasllama/dasllama_canary
require daslib/defer
require dasllama/dasllama_gemma4a
require dasllama/dasllama_qwen3a
require dasllama/dasllama_asr   // set_asr_kv / get_asr_kv - the decoder K/V knob
require dasllama/dasllama_common   // KVDtype
require dasllama/dasllama_image   // image_family_tags - the lanes the facade's image route must accept
require dasllama/dasllama_tower   // TowerDev - the home a device lane names
require dasllama/dasllama_tower_image   // the flavor pick over a source's formats and the halfword-lane helpers
require dasllama/dasllama_gguf   // GGML_TYPE_*, ggml_type_bytes: the synthetic sources' disk types
require daslib/fio   // fopen / fwrite: the synthetic sources
require daslib/archive   // Archive: the lane-tag images' empty meta
require _gguf_writer   // gguf_header, gguf_tensor_info, gguf_pad_data: the synthetic sources' header
require _child_run   // DASLLAMA_AUDIO_LANE loads once at context init: a value a process

//! every lane tag a family the facade loads by image registers routes to that family: a lane `asr_image_route` misses panics a `.dlim` load by path
[test(description = "asr_image_route maps every lane tag the six ASR families register to its family, with at least two lanes a family as the sweep control, and routes chat-tower and unregistered lanes nowhere")]
def test_asr_image_routes(tst : T?) {
    tst |> run("load_asr_model's image route accepts every lane its families register") @(t : T?) {
        let families <- ["whisper", "parakeet", "gigaam", "qwen3a", "gemma4a", "canary"]
        var seen : table<string; int>
        for (flavored in image_family_tags()) {
            let tag = image_tag_lane(flavored)   // a GPU flavor's tag routes by its lane, as `load_asr_model` reads a `.dlim`
            let dash = find(tag, "-")
            continue if (dash < 0)
            let fam = slice(tag, 0, dash)
            continue if (!has_value(families, fam))
            t |> equal(asr_image_route(tag), fam, "the registered tag '{flavored}' routes to its family")
            seen[fam] = (seen?[fam] ?? 0) + 1
        }
        for (fam in families) {
            t |> success((seen?[fam] ?? 0) >= 2, "the {fam} family registered its lanes ({seen?[fam] ?? 0}) - the sweep's control")
        }
        t |> equal(asr_image_route("tower-f16"), "", "a chat tower's lane routes nowhere")
        t |> equal(asr_image_route("whisper-f16"), "", "an unregistered lane routes nowhere")
    }
}

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

[test(description = "gigaam_detokenize over toy vocabs joins charwise pieces verbatim, and in SPM mode turns the mark into a space, drops the leading one and renders control pieces as nothing")]
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

[test(description = "asr_default_lang picks the single listed language, auto for an empty list and English for several, and the GigaAM caps sheet resolves to ru")]
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

[test(description = "gigaam_check_lang accepts ru, the empty string and auto without a panic, and panics on en with a message naming Russian (ru)")]
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

[test(description = "Each ASR family exec_fmt verb, fed hand-set model structs, stamps the exact precision string for its q8, f32, f16 and K-quant lane flags")]
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
        g.gemm_f16 = true
        t |> equal(gemma4a_exec_fmt(g, "q8"), "f16/q8", "gemma4a exact encoder on a GPU home's f16 lane")
        c.gemm_f16 = true
        t |> equal(canary_exec_fmt(c, "q8"), "f16/q8", "canary exact encoder on a GPU home's f16 lane")
        var q = Qwen3aTower()
        q.core.q8 = true
        t |> equal(qwen3a_exec_fmt(q, "q8"), "q8/q8", "qwen3a q8 core")
        q.core.q8 = false
        t |> equal(qwen3a_exec_fmt(q, "q8"), "f32/q8", "qwen3a fp32 parity core")
    }
}

// the audio families' lane policies themselves - model-free: the un-pinned default is q8 on every
// home (gigaam's follows its GPU seat's exact-lane predicate), the pin flips it either way, reset returns.
def private seat_yes() : bool => true

def private seat_no() : bool => false

[test(description = "gpu_exact_serves follows the serves hook when no exact ask is set, and an exact ask answering false or a non-serving seat stands the exact lane down to the q8 image")]
def test_gpu_lane_seat_exact_ask(t : T?) {
    let plain = GpuLaneSeat(set = true, serves = @@seat_yes)
    let crowned = GpuLaneSeat(set = true, serves = @@seat_yes, exact_asks = true, exact = @@seat_yes)
    let bare = GpuLaneSeat(set = true, serves = @@seat_yes, exact_asks = true, exact = @@seat_no)
    let off = GpuLaneSeat(set = true, serves = @@seat_no, exact_asks = true, exact = @@seat_yes)
    t |> success(gpu_exact_serves(plain), "a seat with no exact ask follows its serves hook")
    t |> success(gpu_exact_serves(crowned), "an exact ask answering true keeps the exact lane")
    t |> success(!gpu_exact_serves(bare), "an exact ask answering false stands the exact lane down")
    t |> success(!gpu_exact_serves(off), "a seat that does not serve has no exact lane whatever the ask")
    t |> equal(lane_why(bare, false), "the GPU tower serves the q8 image (the default lane)", "the stood-down seat still serves its q8 image")
}

[test(description = "Each audio family q8 lane defaults to its policy, a pin flips it either way and reset returns to the policy, and the ASR decoder K/V dtype defaults to f16 and reads back its setter")]
def test_audio_lane_knobs(t : T?) {
    t |> run("qwen3a: the q8 lane pin, and reset back to the policy") @(tt : T?) {
        tt |> success(qwen3a_serves_q8(), "the default is q8 on every home")
        set_qwen3a_q8(true)
        tt |> success(qwen3a_serves_q8(), "pinned q8")
        set_qwen3a_q8(false)
        tt |> success(!qwen3a_serves_q8(), "pinned exact")
        reset_qwen3a_q8()   // from the EXACT pin, so a no-op reset cannot pass as a q8 policy default
        tt |> success(qwen3a_serves_q8(), "reset returns to the policy")
    }
    t |> run("gemma4a: the q8 lane pin, and reset back to the policy") @(tt : T?) {
        tt |> success(gemma4a_serves_q8(), "the default is q8 on every home")
        set_gemma4a_q8(true)
        tt |> success(gemma4a_serves_q8(), "pinned q8")
        set_gemma4a_q8(false)
        tt |> success(!gemma4a_serves_q8(), "pinned exact")
        reset_gemma4a_q8()
        tt |> success(gemma4a_serves_q8(), "reset returns to the policy")
    }
    t |> run("canary: the q8 lane pin, and reset back to the policy") @(tt : T?) {
        tt |> success(canary_enc_serves_q8(), "the default is q8 on every home")
        set_canary_enc_q8(true)
        tt |> success(canary_enc_serves_q8(), "pinned q8")
        set_canary_enc_q8(false)
        tt |> success(!canary_enc_serves_q8(), "pinned exact")
        reset_canary_enc_q8()
        tt |> success(canary_enc_serves_q8(), "reset returns to the policy")
    }
    t |> run("parakeet: the q8 lane pin, and reset back to the policy") @(tt : T?) {
        tt |> success(parakeet_serves_q8(), "the default is q8 on every home")
        {
            defer() {
                reset_parakeet_q8()
            }
            set_parakeet_q8(true)
            tt |> success(parakeet_serves_q8(), "pinned q8")
            set_parakeet_q8(false)
            tt |> success(!parakeet_serves_q8(), "pinned exact")
        }
        tt |> success(parakeet_serves_q8(), "the pin's scope ends on the policy")
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

let private LANE_FAMILIES = ["qwen3a", "gemma4a", "canary", "parakeet", "gigaam", "tower"]
let private LANE_SAY = "dasLLAMA: DASLLAMA_AUDIO_LANE="
let private LANE_SAY_TAIL = " over its family's own lane; unset it for that lane"

//! `_audio_lane_root.das` under DASLLAMA_AUDIO_LANE=`v` ("" unset), at the info floor its records print at
def private audio_lane_child(t : T?; v : string) : string {
    var out : string
    let rc = run_child(child_cmd("_audio_lane_root.das", [(name = "DASLLAMA_AUDIO_LANE", value = v), (name = "DAS_LOG_LEVEL", value = "info")]), out)
    t |> equal(rc, 0, "the lane root ran (DASLLAMA_AUDIO_LANE='{v}'):\n{out}")
    t |> success(find(out, "ROOT_DONE") >= 0, "the lane root reached its end:\n{out}")
    return out
}

//! the word after `key` on its line of `out` ("" where no line carries it)
def private word_after(out, key : string) : string {
    let at = find(out, key)
    return "" if (at < 0)
    let from = at + length(key)
    let eol = find(out, "\n", from)
    return strip(slice(out, from, eol < 0 ? length(out) : eol))
}

def private lane_at(out, family, state : string) : string => word_after(out, "LANE {family} {state} ")

//! every `LANE` record of a child, in order - two children agree on every lane exactly when these agree
def private lane_records(out : string) : string {
    return build_string() $(w) {
        for (line in split(out, "\n")) {
            let at = find(line, "LANE ")
            w |> write("{slice(line, at)}\n") if (at >= 0)
        }
    }
}

//! each family's trio and the whisper facade under the knob `v` ("8" or "16"): un-pinned and after reset the knob's lane,
//! each pin its own lane, the facade's fp32 rail the exact one
def private knob_lanes(t : T?; out, v : string) {
    let want = v == "8" ? "q8" : "exact"
    for (fam in LANE_FAMILIES) {
        t |> equal(lane_at(out, fam, "unpinned"), want, "{fam} un-pinned takes the knob's lane")
        t |> equal(lane_at(out, fam, "pin_q8"), "q8", "{fam}'s q8 pin beats the knob")
        t |> equal(lane_at(out, fam, "pin_exact"), "exact", "{fam}'s exact pin beats the knob")
        t |> equal(lane_at(out, fam, "reset"), want, "{fam}'s reset returns to the knob")
    }
    t |> equal(lane_at(out, "whisper", "unpinned"), want, "whisper's facade load takes the knob's lane")
    t |> equal(lane_at(out, "whisper", "rail_exact"), "exact", "the facade's tower fp32 rail beats the knob")
    t |> equal(lane_at(out, "parakeet", "facade_q8"), "q8", "set_asr_tower_q8(true) beats the knob")
    t |> equal(lane_at(out, "parakeet", "facade_exact"), "exact", "set_asr_tower_q8(false) beats the knob")
    var moved = lane_at(out, "whisper", "unpinned") != word_after(out, "POLICY whisper ")
    for (fam in LANE_FAMILIES) {
        moved = moved || word_after(out, "POLICY {fam} ") != want
    }
    t |> equal(count(out, LANE_SAY_TAIL), moved ? 1 : 0, "the knob says itself once where it moves a family off its policy, never where it moves none:\n{out}")
    let ann = slice(out, find(out, "CASE announce"))
    let asked = "asked by DASLLAMA_AUDIO_LANE={v}"
    t |> equal(count(ann, asked), 1, "the lane line names the knob for the lane it asked, and only that one:\n{ann}")
    t |> success(find(ann, v == "8" ? "GEMM lane q8 - {asked}" : "GEMM lane f16 planes - {asked}") >= 0, "... on the lane the knob asked")
    t |> success(find(ann, "f16 planes - pinned via set_probe_q8") >= 0, "a pin's line names the pin under the knob")
    let other = v == "8" ? "GEMM lane f16 planes" : "GEMM lane q8"
    let unheard = "the caller's lane - DASLLAMA_AUDIO_LANE={v} does not reach a named image or a lane the caller hands in"
    t |> success(find(ann, "{other} - {unheard}") >= 0, "a lane the knob did not ask says the knob did not reach it:\n{ann}")
    t |> success(find(ann, "UNHEARD ' - DASLLAMA_AUDIO_LANE={v} does not reach the probe's planes'") >= 0, "the unreached words name the knob and what it does not reach:\n{ann}")
}

//! DASLLAMA_AUDIO_LANE over every audio family's lane policy, one child a value: unset the policy, 8 and 16 the knob's lane
//! under every API pin, any other value a warning and the policy
[test(description = "In child processes DASLLAMA_AUDIO_LANE=8 or 16 moves every un-pinned audio family to that lane while API pins still win and the lane lines name the knob; unset or a bad value leaves the policy lanes")]
def test_audio_lane_env(t : T?) {
    let unset = audio_lane_child(t, "")
    let q8 = audio_lane_child(t, "8")
    let exact = audio_lane_child(t, "16")
    let bad = audio_lane_child(t, "4")
    t |> run("unset: every family's un-pinned lane is its policy, reset returns there, and nothing names the knob") @(t : T?) {
        for (fam in LANE_FAMILIES) {
            let policy = word_after(unset, "POLICY {fam} ")
            t |> success(policy == "q8" || policy == "exact", "{fam}'s policy read:\n{unset}")
            t |> equal(lane_at(unset, fam, "unpinned"), policy, "{fam} un-pinned follows its policy")
            t |> equal(lane_at(unset, fam, "reset"), policy, "{fam}'s reset returns to its policy")
            t |> equal(lane_at(unset, fam, "pin_q8"), "q8", "{fam}'s q8 pin")
            t |> equal(lane_at(unset, fam, "pin_exact"), "exact", "{fam}'s exact pin")
        }
        t |> equal(lane_at(unset, "whisper", "unpinned"), "q8", "whisper's facade load is q8")
        t |> equal(lane_at(unset, "whisper", "rail_exact"), "exact", "the tower fp32 rail asks the exact encoder")
        t |> equal(count(unset, "DASLLAMA_AUDIO_LANE"), 0, "nothing names the knob:\n{unset}")
        t |> success(find(unset, "UNHEARD ''") >= 0, "no unreached words:\n{unset}")
    }
    t |> run("8: every un-pinned family takes the q8 lane, a pin beats it both ways, reset returns to it") @(t : T?) {
        knob_lanes(t, q8, "8")
    }
    t |> run("16: every un-pinned family takes the exact lane, a pin beats it both ways, reset returns to it") @(t : T?) {
        knob_lanes(t, exact, "16")
        t |> equal(count(exact, LANE_SAY_TAIL), 1, "the q8 families moved off their policy: one line")
    }
    t |> run("4: a warning naming the value, and every lane the unset run's") @(t : T?) {
        t |> success(find(bad, "DASLLAMA_AUDIO_LANE=4 is not one of 8 | 16") >= 0, "the malformed value warns:\n{bad}")
        t |> equal(lane_records(bad), lane_records(unset), "every lane the policy's")
        t |> equal(count(bad, LANE_SAY), 1, "no line but the warning names the knob")
        t |> success(lane_records(q8) != lane_records(exact), "the two knob values move the lanes apart (the compare's control)")
    }
}

[test(description = "Each ASR family encode-total profiler bucket is its pinned name, and the facade asr_encode_bucket answers the same bucket for every AsrKind")]
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

//! the facade's device lane for an empty parakeet model whose image is flavored `dev`; one model a frame
def private parakeet_device_lane_of(dev : TowerDev) : string {
    var inscope m <- AsrModel(kind = AsrKind.parakeet)
    m.pk.dev_backend = dev
    return asr_device_lane(m)
}

//! ... for an empty GigaAM model
def private gigaam_device_lane_of(dev : TowerDev) : string {
    var inscope m <- AsrModel(kind = AsrKind.gigaam)
    m.ga.dev_backend = dev
    return asr_device_lane(m)
}

[test(description = "asr_device_lane for a parakeet or gigaam model names the GPU home its image is flavored for, cpu for an unflavored image and for gigaam where its GPU driver does not serve")]
def test_asr_device_lane_names_home(t : T?) {
    t |> run("a one-file family's device lane names the home its image is flavored for") @(t : T?) {
        for (dev in [TowerDev.metal, TowerDev.vulkan, TowerDev.cuda]) {
            t |> equal(parakeet_device_lane_of(dev), "{dev}", "parakeet flavored {dev}")
            t |> equal(gigaam_device_lane_of(dev), gigaam_gpu_serves() ? "{dev}" : "cpu", "gigaam flavored {dev}")
        }
        t |> equal(parakeet_device_lane_of(TowerDev.none), "cpu", "an unflavored parakeet image serves on the CPU")
        t |> equal(gigaam_device_lane_of(TowerDev.none), "cpu", "an unflavored gigaam image serves on the CPU")
    }
}

//! a GGUF at `path` of `tensors` (name, disk type), each 256 x 4 and zero: the header and tensor table a flavor pick reads
def private write_src_gguf(path : string; tensors : array<tuple<string; int>>) {
    var b : array<uint8>
    gguf_header(b, uint64(long_length(tensors)), 0ul)
    var off = 0l
    for (ti in tensors) {
        gguf_tensor_info(b, ti._0, [256ul, 4ul], ti._1, uint64(off))
        off += (ggml_type_bytes(ti._1, 1024l) + 31l) & ~31l
    }
    gguf_pad_data(b)
    b |> resize(long_length(b) + off)
    fopen(path, "wb") $(f) {
        fwrite(f, b)
    }
    delete b
}

[unused_argument(lane)]
def private serves_every_lane(lane : bool) : bool => true

//! the line qwen3a's pick says over a source whose audio tensor is stored as `what`, on the lane form `fmt`
def private kq_conversion_line(what, fmt : string) : string
    => "dasLLAMA qwen3a: the source stores an audio tensor as {what} - the load decodes it to f32 and re-encodes it into the {fmt} lane"

//! the GPU flavor a source load takes over the formats of its audio tensors - a quantized tensor but Q8_0 keeps it, the load naming the conversion
[test(description = "Over synthetic GGUFs a quantized non-Q8_0 audio tensor keeps the CUDA flavor and the load says one conversion line naming the type, member and lane form, while Q8_0, float and non-audio sources say none")]
def test_tower_kq_source_conversion(t : T?) {
    let dir = test_temp_dir()
    let q4k = path_join(dir, "a_q4k.gguf")
    let q6k = path_join(dir, "mm_q6k.gguf")
    let q41 = path_join(dir, "a_q41.gguf")
    let q80 = path_join(dir, "a_q80.gguf")
    let f16 = path_join(dir, "a_f16.gguf")
    let vq4k = path_join(dir, "v_q4k.gguf")
    let junk = path_join(dir, "junk.gguf")
    write_src_gguf(q4k, [("a.blk.0.attn_q.weight", GGML_TYPE_Q4_K), ("a.blk.0.attn_k.weight", GGML_TYPE_F16)])
    write_src_gguf(q6k, [("a.blk.0.attn_q.weight", GGML_TYPE_F16), ("mm.a.mlp.0.weight", GGML_TYPE_Q6_K)])
    write_src_gguf(q41, [("a.blk.0.attn_q.weight", GGML_TYPE_Q4_1)])
    write_src_gguf(q80, [("a.blk.0.attn_q.weight", GGML_TYPE_Q8_0), ("a.blk.0.attn_k.weight", GGML_TYPE_F32)])
    write_src_gguf(f16, [("a.blk.0.attn_q.weight", GGML_TYPE_F16), ("a.blk.0.ffn_up.weight", GGML_TYPE_BF16)])
    write_src_gguf(vq4k, [("v.blk.0.attn_q.weight", GGML_TYPE_Q4_K), ("a.blk.0.attn_q.weight", GGML_TYPE_F16)])
    fopen(junk, "wb") $(f) {
        fwrite(f, "not a gguf file at all, but long enough to pass the length gate")
    }
    t |> run("the pick in this process: every source keeps the CUDA flavor on both lanes, the quantized type named by its member or as no member's") @(t : T?) {
        t |> equal(tower_src_quant_type(q4k), GGML_TYPE_Q4_K, "an a.* Q4_K tensor")
        t |> equal(tower_src_quant_type(q6k), GGML_TYPE_Q6_K, "an mm.a.* Q6_K tensor past a float one")
        t |> equal(tower_src_quant_type(q41), GGML_TYPE_Q4_1, "an a.* Q4_1 tensor")
        for (p in [q80, f16, vq4k, junk, path_join(dir, "absent.gguf")]) {
            t |> equal(tower_src_quant_type(p), -1, "no quantized audio tensor but Q8_0 in {base_name(p)}")
        }
        t |> equal(tower_src_quant_name(GGML_TYPE_Q4_K), "Q4_K (the k4 member)", "Q4_K names its member")
        t |> equal(tower_src_quant_name(GGML_TYPE_Q6_K), "Q6_K (the k6 member)", "Q6_K names its member")
        t |> equal(tower_src_quant_name(GGML_TYPE_Q4_1), "Q4_1 (a type no KqFmt member serves)", "Q4_1, which kq_fmt_of answers with its q8 fallback, names no member")
        t |> equal(tower_src_quant_name(9999), "type 9999 (a type no KqFmt member serves)", "a type no reader knows names no member")
        let cuda = Qwen3aFlavor(backend = TowerDev.cuda, serves = @@serves_every_lane)
        for (q8 in fixed_array(true, false)) {
            for (p in [q4k, q6k, q41, q80, f16, vq4k, junk, path_join(dir, "a_q4k.dlim")]) {
                t |> equal(tower_dev_flavor_announced(cuda, q8, p, "qwen3a"), TowerDev.cuda, "{base_name(p)} keeps the CUDA flavor (q8={q8})")
            }
        }
        let cpu = Qwen3aFlavor(backend = TowerDev.none, serves = @@serves_every_lane)
        t |> equal(tower_dev_flavor_announced(cpu, true, q4k, "qwen3a"), TowerDev.none, "no driver registered: the CPU image")
        var pinned = TowerDev.cuda
        with_cpu_flavor() ${
            pinned = tower_dev_flavor_announced(cuda, true, q4k, "qwen3a")
        }
        t |> equal(pinned, TowerDev.none, "the CPU flavor pin: the CPU image")
    }
    t |> run("the line the pick says: the type, its member and the lane form, once a load, and none over a Q8_0 or float source") @(t : T?) {
        var out : string
        let paths = join([for (p in [q4k, q6k, q41, q80, f16, vq4k, junk]); "\"{p}\""], " ")
        let rc = run_child(child_cmd("_flavor_decline_root.das", [(name = "DAS_LOG_LEVEL", value = "info")], "src {paths}"), out)
        t |> equal(rc, 0, "the root ran:\n{out}")
        t |> success(find(out, "ROOT_DONE") >= 0, "the root reached its end:\n{out}")
        for (what in fixed_array("Q4_K (the k4 member)", "Q6_K (the k6 member)", "Q4_1 (a type no KqFmt member serves)")) {
            t |> equal(count(out, kq_conversion_line(what, "q8")), 2, "{what}: the q8 lane's line under the CUDA flavor and the CPU image:\n{out}")
            t |> equal(count(out, kq_conversion_line(what, "f16")), 1, "{what}: the exact pick's f16 lane under the CUDA flavor")
            t |> equal(count(out, kq_conversion_line(what, "f32")), 1, "{what}: the exact pick's f32 lane on the CPU image")
        }
        t |> equal(count(out, "the source stores an audio tensor as"), 12, "no Q8_0 or float source, no tensor outside the tower, no unread file says a line")
        t |> success(find(out, "FLAVOR a_q4k.gguf cuda q8=false cuda") >= 0 && find(out, "FLAVOR a_q4k.gguf none q8=true none") >= 0,
            "the child's pick is this process's")
    }
}

//! a bare image of one plane: what `tower_load_f16` reads off a `.dlim` is its baked tag alone
struct private LaneTagImage {
    rows : array<float>
    image_map : void?
    image_bytes : uint64
}

[unused_argument(arch, t)]
def private serialize_image_meta(var arch : Archive; var t : LaneTagImage) {}

//! a `LaneTagImage` baked under `tag` in `dir`, its path
def private lane_tag_image(t : T?; dir, tag : string) : string {
    let path = path_join(dir, "{tag}.dlim")
    var img = LaneTagImage()
    img.rows |> resize(64)
    t |> success(save_image(img, path, tag), "the {tag} image bakes")
    delete img
    return path
}

//! the halfword-lane helpers over every flavor, the expectations written out: the exact pick stages halfwords on every GPU home
[test(description = "The halfword-lane helpers stage the exact pick as f16 on every GPU home and never the q8 pick or the CPU flavor, and tower_load_f16 reads a .dlim by its baked lane tag over the caller flavor")]
def test_tower_lane_helpers(t : T?) {
    t |> run("tower_dev_halfword: every GPU home stages the exact pick as halfwords, the CPU flavor none") @(t : T?) {
        t |> equal(tower_dev_halfword(TowerDev.none), false, "the CPU flavor")
        t |> equal(tower_dev_halfword(TowerDev.metal), true, "metal")
        t |> equal(tower_dev_halfword(TowerDev.vulkan), true, "vulkan")
        t |> equal(tower_dev_halfword(TowerDev.cuda), true, "cuda")
    }
    t |> run("tower_lane_f16: the exact pick on a GPU flavor, never the q8 pick") @(t : T?) {
        for (dev in [TowerDev.none, TowerDev.metal, TowerDev.vulkan, TowerDev.cuda]) {
            t |> equal(tower_lane_f16(true, dev), false, "the q8 pick under {dev}")
        }
        t |> equal(tower_lane_f16(false, TowerDev.none), false, "the exact pick on the CPU flavor: the file's f32")
        t |> equal(tower_lane_f16(false, TowerDev.metal), true, "the exact pick on metal")
        t |> equal(tower_lane_f16(false, TowerDev.vulkan), true, "the exact pick on vulkan")
        t |> equal(tower_lane_f16(false, TowerDev.cuda), true, "the exact pick on cuda")
    }
    t |> run("tower_staging_f16: the pick under the running staging's flavor, the CPU's outside one") @(t : T?) {
        t |> equal(tower_staging_dev(), TowerDev.none, "no staging runs")
        t |> equal(tower_staging_f16(false), false, "the exact pick outside a staging: the file's f32")
        var cuda_exact = false
        var cuda_q8 = true
        var cpu_exact = true
        with_tower_dev_staging(TowerDev.cuda) ${
            cuda_exact = tower_staging_f16(false)
            cuda_q8 = tower_staging_f16(true)
        }
        with_tower_dev_staging(TowerDev.none) ${
            cpu_exact = tower_staging_f16(false)
        }
        t |> equal(cuda_exact, true, "the exact pick under a CUDA staging: halfwords")
        t |> equal(cuda_q8, false, "the q8 pick under a CUDA staging")
        t |> equal(cpu_exact, false, "the exact pick under the CPU flavor's staging")
        t |> equal(tower_staging_dev(), TowerDev.none, "the staging's flavor put back")
    }
    t |> run("tower_load_f16: a .dlim by its baked lane tag whatever flavor the caller hands, a source by its flavor") @(t : T?) {
        let dir = test_temp_dir()
        let src = path_join(dir, "src.gguf")
        t |> equal(tower_load_f16(src, false, TowerDev.cuda), true, "a source's exact pick under a CUDA flavor")
        t |> equal(tower_load_f16(src, false, TowerDev.none), false, "... under the CPU flavor")
        t |> equal(tower_load_f16(src, true, TowerDev.cuda), false, "a source's q8 pick")
        let f16_cuda = lane_tag_image(t, dir, "qwen3a-f16-cuda")
        let f32 = lane_tag_image(t, dir, "qwen3a-f32")
        let q8_vk = lane_tag_image(t, dir, "qwen3a-q8-vulkan")
        t |> equal(tower_load_f16(f16_cuda, false, TowerDev.none), true, "an image baked on the f16 lane's CUDA flavor, the caller handing none")
        t |> equal(tower_load_f16(f16_cuda, true, TowerDev.cuda), false, "... the q8 pick over it")
        t |> equal(tower_load_f16(f32, false, TowerDev.cuda), false, "an f32 image under a CUDA flavor: the baked tag, not the flavor")
        t |> equal(tower_load_f16(q8_vk, false, TowerDev.vulkan), false, "a q8 image's tag under a Vulkan flavor")
    }
}

[test(description = "parakeet_detokenize strips the spaces of the first piece SPM marks but keeps them on later pieces, passes unmarked pieces through, drops control pieces and maps no tokens to the empty string")]
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
