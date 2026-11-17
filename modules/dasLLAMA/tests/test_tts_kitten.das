options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_kitten
require dasllama/dasllama_tts
require dasllama/dasllama_tts_types
require dasllama/dasllama_styletts2
require dasllama/dasllama_math   // with_dasllama_jobque_
require daslib/jobque_boost
require daslib/defer
require daslib/fio
require _tts_corpus
require _tts_parity
require _model_tier
require _gguf_writer   // gguf_retyped_copy / gguf_retyped_refusal: the stored file's refusal carriers
require dasllama/dasllama_gguf    // the GGML types a carrier is retyped to; f16_to_f32: a stored scale is a half
require dasllama/dasllama_image   // dlim_wipe, image_path_for, image_family_tag: the images a stored file's load mints beside it
require dasllama/dasllama_env     // g_env_engine.image: the image rail's switch

// The KittenTTS family. Model-free: the symbol map (the front end's inventory into the
// espeak-style IPA the models consume) over the 200-sentence corpus, and the reference driver's
// token rule. Model-gated (`kitten-<size>.gguf` + `tts_oracle/kitten_<size>/` under models_dir(),
// both minted by performance/build_tts_data.das through harness/convert_kitten.py and
// harness/kitten_oracle.py): the parity rail of `_tts_parity.das` per size, and a facade smoke
// cell that speaks one sentence. The facade and GPU cells also need the front-end packs
// (`tts_g2p.bin`, `tts_postag.bin`) beside the models; the GPU cells run on Metal or Vulkan, skip
// where no device serves or the driver registers no seat for the stage, and red where a present
// seat declines a stage. The facade cell also speaks the published files that store a lane
// (`kitten-nano-q8.gguf`, `kitten-nano-kq.gguf`), each leg skipping without its file; the stored-file
// cell writes its own files from kitten-nano.gguf, its later legs skip where the first wrote none, and
// its image-rail leg skips with the rail off (DASLLAMA_IMAGE=0).

[test(description = "`kitten_phonemes_from_misaki` rewrites all 200 corpus sentences to the fixture's espeak IPA exactly plus the calibrated rules, and `kitten_token_ids` emits pad-wrapped ids with punctuation as its own token and a closing ellipsis")]
def test_symbol_map(t : T?) {
    t |> run("the corpus rewrites symbol for symbol") @(t : T?) {
        var inscope rows <- load_tts_corpus()
        var mismatches = 0
        for (r in rows) {
            let got = kitten_phonemes_from_misaki(r.ps_misaki)
            if (got != r.ps_espeak) {
                mismatches++
                t |> failure("{r.id}: got  {got}\n       want {r.ps_espeak}")
            }
        }
        t |> equal(mismatches, 0, "sentences rewritten differently")
    }
    t |> run("the calibrated rules") @(t : T?) {
        t |> equal(kitten_phonemes_from_misaki("kəmpjˈuTəɹ"), "kəmpjˈuːɾɚ")   // nolint:STYLE039
        t |> equal(kitten_phonemes_from_misaki("bˈɜɹd"), "bˈɜːd")   // nolint:STYLE039
        t |> equal(kitten_phonemes_from_misaki("ʧˈOk"), "tʃˈoʊk")   // nolint:STYLE039
        t |> equal(kitten_phonemes_from_misaki("ɹˈɪTᵊn"), "ɹˈɪɾən")   // nolint:STYLE039
    }
    t |> run("the token rule") @(t : T?) {
        // pad, "h", "i", space, ",", space, "w", space, ".", ellipsis, pad - punctuation is its own token
        var inscope ids <- kitten_token_ids("hi, w.")
        t |> equal(length(ids), 11)
        t |> equal(ids[0], 0)
        t |> equal(ids[3], 16, "the space symbol")
        t |> equal(ids[4], 3, "the comma")
        t |> equal(ids[length(ids) - 2], 10, "the ellipsis closes")
        t |> equal(ids[length(ids) - 1], 0)
        t |> equal(kitten_style_row("abc", 400), 3)
        t |> equal(kitten_style_row("", 400), 0)
    }
}

//! the asserts `spectrum_diff` fails on a probe over a two-bin, two-frame spectrum `ours` against `ref`
def private spectrum_fails(ours, ref : array<float>) : int {
    return probe_fails() $(p) {
        spectrum_diff(p, "probe", ours, ref, 2l, 2l)
    }
}

[test(description = "`spectrum_diff` passes a clean spectrum within 1e-6 and fails when a NaN sits in any magnitude or phase element on our side or any magnitude element of the reference")]
def test_spectrum_diff_reds_a_nan(t : T?) {
    let ref <- [1.0, 2.0, 3.0, 4.0, 0.1, 0.2, 0.3, 0.4]
    let ours <- [for (v in ref); v + 1e-6]
    let nan = NAN_SENTINEL
    t |> equal(spectrum_fails(ours, ref), 0, "control: the clean spectrum holds both bars")
    for (at in [1, 3, 5, 7]) {
        var inscope bad := ours
        bad[at] = nan
        t |> success(spectrum_fails(bad, ref) > 0, "a NaN in our {at < 4 ? "magnitude" : "phase"} element {at} reds the compare")
    }
    for (at in [1, 3]) {
        var inscope bad := ref
        bad[at] = nan
        t |> success(spectrum_fails(ours, bad) > 0, "a NaN in the reference magnitude element {at} reds the compare")
    }
}

def private oracle_dir(size : string) : string =>
    path_join(models_dir(), "tts_oracle/kitten_{size}")

def private gguf_path(size : string) : string => path_join(models_dir(), "kitten-{size}.gguf")

// the front end's espeak-inventory phonemes, as the fixture carries them, into the driver's ids
def private kitten_ids_of(ps : string) : array<int> => <- kitten_token_ids(ps)

// text in, speech out through the facade: the front end's packs sit beside the GGUF in the store
[test(description = "Kitten nano - its file of floats, and the published files that store its Q8_0 lane and its recipe lane - through the facade lists 8 voices at 24 kHz and speaks a 51-character sentence as 1 to 10 seconds of finite audio peaking above 0.05, with every stage clock running and the total covering its parts")]
def test_kitten_facade(t : T?) {
    for (stem in fixed_array("nano", "nano-q8", "nano-kq")) {   // the file of floats, then the two published files that store a lane
        t |> run("{stem}: text in, speech out") @(t : T?) {
            facade_smoke(t, gguf_path(stem), true, 8) $(var m : TtsModel; c : TtsCaps) {
                let served0 = styletts2_gpu_stats("decode").served
                var inscope audio <- synthesize(m, "The quick brown fox jumps over the lazy dog, twice.", "Bella")
                t |> equal(audio.sample_rate, 24000)
                // 51 characters of text: well over a second of speech, under ten
                t |> success(length(audio.pcm) > 24000 && length(audio.pcm) < 240000, "{length(audio.pcm)} samples")
                let lv = speech_level(audio.pcm)
                t |> success(lv.finite, "every sample finite")
                t |> success(lv.peak > 0.05, "peak {lv.peak}: the model spoke")
                let tm & = audio.timings
                let gen_on_tower = tm.generator_us == 0l && tm.source_us == 0l && styletts2_gpu_stats("decode").served > served0
                t |> success(tm.total_us > 0l && (tm.generator_us > 0l || gen_on_tower) && tm.decoder_us > 0l && tm.front_end_us > 0l, "the stage clocks ran")
                t |> success(tm.total_us >= tm.front_end_us + tm.bert_us + tm.decoder_us + tm.generator_us, "the total covers its parts")
                to_log(LOG_INFO, "{stem} facade: {length(audio.pcm)} samples, peak {lv.peak}; {timings_line(tm, audio.sample_rate)}\n")
            }
        }
    }
}

// the generator hook under test: declines, or serves a silent waveform of the CPU chain's length
var private g_hook_serves = false

[unused_argument(d, x_rows, c, s, har_spec, har_frames, sc)]
def private generator_stub(d : St2Decoder; x_rows : array<float>; c, t : int64; s : array<float>;
                           har_spec : array<float>; har_frames : int64; var wave : array<float>; var sc : St2Scratch) : bool {
    if (!g_hook_serves) {
        return false
    }
    wave |> resize(t * 600l)
    for (i in range64(t * 600l)) {
        wave[i] = 0.0
    }
    return true
}

struct HookProbe {
    calls_after_decline : int
    served_after_decline : int
    spoke_after_decline : bool
    served_after_serve : int
    silent_after_serve : bool
}

def private hook_probe(path : string) : HookProbe {
    var r = HookProbe()
    var inscope m <- load_tts_model(path)
    register_styletts2_gpu(St2GpuDriver(gen = @@generator_stub))   // the generator seat alone: the decode seat empty, so the CPU chain reaches it
    defer() {
        unregister_styletts2_gpu()
    }
    g_hook_serves = false
    let s0 = styletts2_generator_gpu_stats()
    var inscope a <- synthesize(m, "The hook declines and the model speaks.", "Bella")
    let s1 = styletts2_generator_gpu_stats()
    r.calls_after_decline = s1.calls - s0.calls
    r.served_after_decline = s1.served - s0.served
    r.spoke_after_decline = logits_maxabs(a.pcm) > 0.05
    g_hook_serves = true
    var inscope b <- synthesize(m, "The hook serves silence.", "Bella")
    let s2 = styletts2_generator_gpu_stats()
    r.served_after_serve = s2.served - s1.served
    r.silent_after_serve = logits_maxabs(b.pcm) == 0.0 && !empty(b.pcm)
    g_hook_serves = false
    return r
}

[test(description = "A registered StyleTTS2 generator hook is called once per chunk: when it declines nothing is counted served and the CPU chain still speaks, and when it serves its silent waveform reaches the facade and is counted")]
def test_kitten_generator_hook(t : T?) {
    t |> run("nano: the generator hook declines into the CPU chain and serves when it answers") @(t : T?) {
        if (tts_data_missing(t, gguf_path("nano"), false)) {
            return
        }
        with_dasllama_jobque_() {
            let r = hook_probe(gguf_path("nano"))
            t |> equal(r.calls_after_decline, 1, "one chunk, one hook call")
            t |> equal(r.served_after_decline, 0, "a decline serves nothing")
            t |> success(r.spoke_after_decline, "the CPU chain spoke after the decline")
            t |> equal(r.served_after_serve, 1, "the serving hook is counted")
            t |> success(r.silent_after_serve, "the served waveform reached the facade")
        }
    }
}

[test(description = "Kitten nano on the GPU tower serves every seat once a chunk on the served lane, speaks above the 0.05 peak floor with a sample count within a twentieth of the CPU chain, then holds the f32 reference lane against the CPU")]
def test_kitten_synthesis_metal(t : T?) {
    t |> run("nano: the synthesis on the Metal tower vs the CPU chain, the served lane") @(t : T?) {
        tts_gpu_synthesis(t, gguf_path("nano"), "The tower serves the generator, and the words still land.", "Bella")
    }
}

// ARCHITECTURE_TTS.md#tts-image-rail: q8 is the served default, f32 the reference lane the parity
// rail holds against; a pin resets from the exact pin it took.
[test(description = "The TTS lane defaults to q8 when unpinned, an f32 pin reaches the StyleTTS2 family, a q8 pin holds, and `reset_tts_q8` returns to the q8 default from either pin")]
def test_tts_lane_pin(t : T?) {
    t |> run("the served lane: q8 un-pinned, either lane on demand, the default back on reset") @(t : T?) {
        {
            defer() {
                reset_tts_q8()
            }
            t |> success(tts_serves_q8(), "the un-pinned default serves q8")
            set_tts_q8(false)
            t |> success(!tts_serves_q8(), "the f32 pin is the reference lane")
            t |> success(!styletts2_serves_q8(), "the facade's pin reaches the family")
            reset_tts_q8()
            t |> success(tts_serves_q8(), "reset from the f32 pin returns to the policy default")
            set_tts_q8(true)
            t |> success(tts_serves_q8(), "the q8 pin")
            reset_tts_q8()
            t |> success(tts_serves_q8(), "reset from the q8 pin returns to the policy default")
        }
    }
}

// The reference pipeline truncates a chunk past the encoder's positions and logs it; nothing on
// the served path may panic on a long one.
[test(description = "`styletts2_synthesize` on kitten nano truncates an id sequence 64 past the encoder's max positions instead of panicking, producing samples that the timings count exactly")]
def test_kitten_over_long_chunk(t : T?) {
    t |> run("nano: a chunk past the encoder's positions is truncated, not refused") @(t : T?) {
        if (!model_available(t, gguf_path("nano"))) {
            return
        }
        with_dasllama_jobque_() {
            var inscope m <- load_styletts2(gguf_path("nano"))
            let cap = m.albert.max_pos
            let said = styletts2_spoken(m, int(cap) + 64)
            t |> equal(int64(said.ids), cap + 64l, "the chunk is 64 ids past the encoder's {cap} positions")
            t |> success(said.samples > 0l, "{said.ids} ids spoke {said.samples} samples")
            t |> equal(said.counted, said.samples, "the timings count the samples the truncated chunk produced")
        }
    }
}

// a staged read's quant bytes and floats against another's: the bytes that differ, and the floats that are the other's exactly,
// the other's rounded to a half, or neither
def private staged_against(a, b : St2Staging) : tuple<qdiff : int64; same : int64; half : int64; other : int64> {
    var r = (qdiff = 0l, same = 0l, half = 0l, other = 0l)
    for (x, y in a.qblob, b.qblob) {
        r.qdiff += x != y ? 1l : 0l
    }
    for (x, y in a.blob, b.blob) {
        if (x == y) {
            r.same++
        } elif (x == f16_to_f32(f32_to_f16(y))) {
            r.half++
        } else {
            r.other++
        }
    }
    return r
}

// a stored file this test writes, under the run's temp folder; the name is built, so the runner's scan of model-file
// literals does not list it as a model the box lacks
def private stored_tmp(tag : string) : string => path_join(test_temp_dir(), "kitten-nano-stored-{tag}.gguf")

// a leg of `test_kitten_stored_file` that reads a file its first leg writes skips, loudly, where that leg wrote none
def private stored_file_absent(t : T?; stored : string) : bool {
    return false if (stat(stored).is_valid)
    t |> skip("{base_name(stored)} is absent: the leg that writes it wrote none")
    return true
}

// the stored file's refusals, over carriers retyped or poked from the q8 file `test_kitten_stored_file` wrote
def private stored_file_refusals(t : T?; src : string) {
        let stored = stored_tmp("q8")
        return if (stored_file_absent(t, stored))
        let newer = stored_tmp("layout2")
        var inscope none : table<string; int>
        gguf_retyped_copy(stored, newer, none, [(key = "styletts2.stored_layout", at = 0, v = 2)])
        var why = panic_text() {
            var inscope st <- stage_styletts2(newer)
        }
        t |> success(why |> find("layout 2") >= 0 && why |> find("reads layout 1") >= 0, "a layout this build does not read is refused by its number: {why}")
        remove(newer)
        why = gguf_retyped_refusal(src, "kitten-nano-undeclared", {"bert_encoder.weight" => GGML_TYPE_Q8_0}) $(path : string) {
            var inscope st <- stage_styletts2(path, true)
        }
        t |> success(why |> find("bert_encoder.weight") >= 0 && why |> find("declares no styletts2.stored_layout") >= 0,
            "a quantized tensor in a file that declares no layout is refused by name: {why}")
        why = gguf_retyped_refusal(stored, "kitten-nano-linear-k4", {"bert_encoder.weight" => GGML_TYPE_Q4_K}) $(path : string) {
            var inscope st <- stage_styletts2(path)
        }
        t |> success(why |> find("bert_encoder.weight") >= 0 && why |> find("a dense layer's stored form is Q8_0") >= 0, "a K-quant dense layer is refused by name: {why}")
        why = gguf_retyped_refusal(stored, "kitten-nano-q40", {"bert_encoder.weight" => GGML_TYPE_Q4_0}) $(path : string) {
            var inscope st <- stage_styletts2(path)
        }
        t |> success(why |> find("bert_encoder.weight' is stored as q40") >= 0, "a format no site takes is refused by its name: {why}")
        let again = stored_tmp("again")
        why = panic_text() {
            write_styletts2_gguf(stored, again, false)
        }
        t |> success(why |> find("the writer takes a file of floats") >= 0 && !stat(again).is_valid, "the writer refuses a file that already stores its weights: {why}")
        let lane_x = stored_tmp("lane-x")
        gguf_retyped_copy(stored, lane_x, none, [(key = "styletts2.stored_lane", at = 8, v = 'x')])   // past the string's length word
        why = panic_text() {
            var inscope st <- stage_styletts2(lane_x)
        }
        t |> success(why |> find("names the stored lane 'x8'") >= 0, "a lane the file cannot store is refused by its name: {why}")
        remove(lane_x)
        why = gguf_retyped_refusal(stored, "kitten-nano-conv-unnamed", {"decoder.generator.noise_convs.0.weight" => GGML_TYPE_Q8_0}) $(path : string) {
            var inscope st <- stage_styletts2(path)
        }
        t |> success(why |> find("styletts2.stored_conv.decoder.generator.noise_convs.0.weight gives") >= 0,
            "a quantized conv whose channel counts the file does not name is refused: {why}")
        why = gguf_retyped_refusal(stored, "kitten-nano-lstm-half", {"predictor.lstm.weight_hh" => GGML_TYPE_F32}) $(path : string) {
            var inscope st <- stage_styletts2(path)
        }
        t |> success(why |> find("'predictor.lstm' stores q8 and floats") >= 0, "an LSTM with one matrix of floats is refused: {why}")
        why = gguf_retyped_refusal(stored, "kitten-nano-voice-k4", {"voice.expr-voice-2-f" => GGML_TYPE_Q4_K}) $(path : string) {
            var inscope st <- stage_styletts2(path)
        }
        t |> success(why |> find("'voice.expr-voice-2-f' is stored k4; a voice pack's stored form is Q8_0") >= 0, "a K-quant voice pack is refused: {why}")
        why = gguf_retyped_refusal(src, "kitten-nano-bias-f16", {"bert_encoder.bias" => GGML_TYPE_F16}) $(path : string) {
            var inscope st <- stage_styletts2(path, true)
        }
        t |> equal(why, "", "control: a tensor of halves in a file of floats is floats, and stages")
        let nowhere = path_join(test_temp_dir(), "no-such-dir/kitten-nano-q8.gguf")
        t |> success(!write_styletts2_gguf(src, nowhere, false) && !stat("{nowhere}.tmp").is_valid, "a destination that cannot be written answers false and leaves no temp file")
}

// the stored file under a recipe pin: what the writer stores, what the bake tags, where the load goes
def private stored_file_pins(t : T?; src : string) {
        let stored = stored_tmp("kq")
        return if (stored_file_absent(t, stored))
        let k5 = stored_tmp("k5")
        remove(k5)
        set_styletts2_recipe("ups=k5")
        t |> success(write_styletts2_gguf(src, k5, true), "the recipe lane with the upsamplers pinned Q5_K is written")
        var inscope from_lane <- stage_styletts2(src, true, true)
        reset_styletts2_recipe()
        var inscope from_file <- stage_styletts2(k5)
        var inscope unpinned <- stage_styletts2(src, true, true)
        let diff = staged_against(from_file, from_lane)
        t |> success(long_length(from_file.qblob) == long_length(from_lane.qblob) && diff.qdiff == 0l && diff.other == 0l,
            "read back with no pin set, the file's planes are the pinned lane's ({diff.qdiff} quant bytes off, {diff.other} floats off)")
        t |> success(long_length(unpinned.qblob) != long_length(from_file.qblob), "control: the recipe as it is mints the upsamplers in another format")
        remove(k5)
        // a stored file is no in-memory mint under a pinned recipe: it loads through the image rail, on its own lane
        let image = path_join(test_temp_dir(), "kitten-nano-stored-kq-baked.dlim")
        remove(image)
        t |> success(bake_styletts2_image(stored, image, true, false), "the stored kq file bakes when the q8 lane is asked")
        t |> equal(image_family_tag(image), ST2_IMAGE_TAG_KQ, "the image carries the file's lane, not the asked one")
        remove(image)
        if (!g_env_engine.image) {
            t |> skip("the image rail is off (DASLLAMA_IMAGE=0): a stored file's load writes no image to read back")
        } else {
            dlim_wipe(stored)
            set_styletts2_recipe("ups=k5")
            {
                var inscope m <- load_styletts2(stored)
                t |> success(m.q8 && m.kq, "under a pinned recipe the stored file serves its own lane")
            }
            reset_styletts2_recipe()
            t |> success(stat(image_path_for(stored, ST2_IMAGE_TAG_KQ)).is_valid, "and its load wrote the lane's image beside it, where a pinned mint of a file of floats writes none")
            dlim_wipe(stored)
        }
}

[test(description = "kitten-nano written as a file that stores the Q8_0 lane's weights, and as one that stores the recipe lane's, stages the lane's own quant bytes with each Q8_0 scale as a half, is smaller than its source, serves the lane it names over an f32 pin and a recipe ask, and speaks; a pinned group is stored in its pin's format and a stored file bakes and loads on its own lane under a pin; a layout this build does not read, an unknown lane, a quantized tensor in a file that declares no layout, a format a site does not take, a conv whose channels the file does not name, a half-quantized LSTM, a rewrite of a stored file and an unwritable destination are refused or declined by name")]
def test_kitten_stored_file(t : T?) {
    let src = gguf_path("nano")
    return if (!model_available(t, src))
    defer() {
        reset_styletts2_recipe()
        reset_styletts2_q8()
    }
    reset_styletts2_recipe()   // the writer's stage and the lanes it is held to read the pins: start from none
    reset_styletts2_q8()
    with_dasllama_jobque_() {
        for (kq in fixed_array(false, true)) {
            let lane = kq ? "kq" : "q8"
            let stored = stored_tmp(lane)
            remove(stored)
            t |> run("the {lane} lane written to a file: the lane's quant bytes, its Q8_0 scales as halves") @(t : T?) {
                t |> success(write_styletts2_gguf(src, stored, kq), "the file is written")
                t |> success(stat(stored).size * 2ul < stat(src).size, "at {int64(stat(stored).size)} bytes it is under half its source's {int64(stat(src).size)}")
                t |> equal(styletts2_stored_lane(stored), lane, "the file names its lane")
                t |> equal(styletts2_stored_lane(src), "", "control: the source of floats names none")
                var inscope from_file <- stage_styletts2(stored)   // asked for the f32 lane: the file decides
                var inscope from_lane <- stage_styletts2(src, true, kq)
                t |> success(from_file.e.q8 && from_file.e.kq == kq, "the staged read takes the file's lane over the lane it was asked for")
                t |> equal(long_length(from_file.qblob), long_length(from_lane.qblob), "as many quant bytes as the lane mints")
                t |> equal(long_length(from_file.blob), long_length(from_lane.blob), "and as many floats")
                let diff = staged_against(from_file, from_lane)
                to_log(LOG_INFO, "kitten-nano {lane} file: {diff.qdiff} quant bytes off, {diff.same} floats equal, {diff.half} as halves, {diff.other} other\n")
                t |> equal(diff.qdiff, 0l, "every quant and K-quant byte is the lane's")
                t |> equal(diff.other, 0l, "every float is the lane's, or the lane's rounded to a half")
                t |> success(diff.half > 0l, "the Q8_0 scales are the halves the blocks hold ({diff.half} of them)")
                var inscope other_lane <- stage_styletts2(src, true, !kq)
                t |> success(long_length(other_lane.qblob) != long_length(from_file.qblob), "control: the other lane mints another byte count")
            }
            t |> run("the {lane} file serves as stored: over an f32 pin, over a recipe ask, and it speaks") @(t : T?) {
                return if (stored_file_absent(t, stored))
                set_styletts2_q8(false)
                if (!kq) {
                    set_styletts2_recipe("")
                }
                var inscope m <- load_styletts2(stored)
                reset_styletts2_recipe()
                reset_styletts2_q8()
                t |> success(m.q8 && m.kq == kq, "the model is on the file's lane, not the pin's or the ask's")
                let said = styletts2_spoken(m, 40)
                t |> success(said.samples > 0l && said.level.finite && said.level.peak > 0.05, "it speaks {said.samples} samples at peak {said.level.peak}")
                dlim_wipe(stored)
            }
        }
        t |> run("refusals: an unread layout, an unknown lane, a quantized tensor with no layout declared, formats a site does not take, a stored file as the writer's source, an unwritable destination") @(t : T?) {
            stored_file_refusals(t, src)
        }
        t |> run("a pinned group is stored in its pin's format, and a stored file loads through the image rail under a pin") @(t : T?) {
            stored_file_pins(t, src)
        }
        for (lane in fixed_array("q8", "kq")) {
            remove(stored_tmp(lane))
        }
    }
}

[test(description = "Kitten nano and mini against onnxruntime dumps with captured noise: every case's token ids and sample count match the oracle, and each traced stage holds 1e-4 of the oracle's peak, with poisoned controls")]
def test_kitten_oracle(t : T?) {
    for (size in ["nano", "mini"]) {
        t |> run("{size} against onnxruntime, captured noise") @(t : T?) {
            tts_parity(t, TtsParityRig(family = "kitten-tts", gguf = gguf_path(size), oracle_dir = oracle_dir(size), ids = @@kitten_ids_of))
        }
    }
}
