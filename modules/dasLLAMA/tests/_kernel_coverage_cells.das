options gen2
options _dasllama_internal = true

module _kernel_coverage_cells shared public

//! The kernel coverage census as cells: each zoo row's
//! dispatch-making work runs on a model a block loaded. The verdict
//! that reads every test process's table merged and asserts the floor
//! and the declared blind spots is `_kernel_census.das`'s.
//! No entry loads a model

require dastest/testing_boost public
require dasllama/dasllama   // nolint:STYLE029,LINT019 - the public facade
require dasllama/dasllama_gpu_tier   // kernel_coverage_report
require dasllama/dasllama_whisper   // nolint:STYLE030,LINT019 - WhisperModel
require dasllama/dasllama_tower   // nolint:STYLE030,LINT019 - the tower types
require dasllama/dasllama_audio   // nolint:STYLE030,LINT019 - AudioTower
require dasllama/dasllama_gemma4uv   // nolint:STYLE030,LINT019 - Gemma4uvEmbedder
require dasllama/dasllama_gemma4v   // nolint:STYLE030,LINT019 - Gemma4vTower
require dasllama/dasllama_gemma3v   // nolint:STYLE030,LINT019 - Gemma3vTower
require dasllama/dasllama_qwen3a   // nolint:STYLE030,LINT019 - Qwen3aTower
require dasllama/dasllama_gemma4a   // nolint:STYLE030,LINT019 - Gemma4aEncoder
require dasllama/dasllama_canary   // nolint:STYLE030,LINT019 - CanaryEncoder
require dasllama/dasllama_parakeet   // nolint:STYLE030,LINT019 - ParakeetModel
require dasllama/dasllama_gigaam   // nolint:STYLE030,LINT019 - GigaamModel
require dasllama/dasllama_nmt   // nolint:STYLE030,LINT019 - NmtModel
require dasllama/dasllama_vad   // nolint:STYLE030,LINT019 - VadModel
require dasllama/dasllama_qwen25v   // nolint:STYLE030,LINT019 - Qwen25vTower
require dasllama/dasllama_qwen3v   // nolint:STYLE030,LINT019 - Qwen3vTower
require dasllama/dasllama_styletts2   // nolint:STYLE030,LINT019 - StyleTts2Model
require dasllama/dasllama_pocket   // nolint:STYLE030,LINT019 - PocketModel
require dasllama/dasllama_tts_types   // nolint:STYLE030,LINT019 - TtsTimings
require dasllama/dasllama_env   // nolint:STYLE030,LINT019 - g_env_test
require dasllama/dasllama_math   // nolint:STYLE030,LINT019 - reserve_resize
require ?das_metal dasllama/dasllama_metal_common   // nolint:STYLE030,LINT019 - the census
require ?das_metal dasllama/dasllama_metal_tower   // nolint:STYLE030,LINT019 - the tower knobs
require ?das_metal dasllama/dasllama_metal_asr_dec   // nolint:STYLE030,LINT019 - set_metal_wdec
require ?das_metal dasllama/dasllama_metal_nmt   // nolint:STYLE030,LINT019 - set_metal_nmt
require ?das_metal dasllama/dasllama_audio_io   // nolint:STYLE030,LINT019 - the jfk clip
require ?vulkan dasllama/dasllama_vulkan_common   // nolint:STYLE030,LINT019 - the census
require ?vulkan dasllama/dasllama_vulkan_tower   // nolint:STYLE030,LINT019 - set_vulkan_tower
require ?vulkan dasllama/dasllama_vulkan_classes   // nolint:STYLE030,LINT019 - the tile routers
require daslib/jobque_boost
require math   // nolint:STYLE030,LINT019 - sin, inside the GPU halves
require strings
require daslib/strings_boost   // nolint:STYLE030,LINT019 - split, the crown list
require _model_tier
require _model_form   // with_ctx_cut: a cell's context on the form's one load
require _metal_blob_twin   // with_metal_rails: the zoo row's Metal overrides
require _kernel_census public   // the notes, the census tables a verdict reads
require _compares   // nolint:STYLE030,LINT019 - jit_only, inside the das_metal half
require _gpu_knobs   // nolint:STYLE030,LINT019 - with_gpu_knobs, inside the GPU halves
require _gpu_tier_armed   // nolint:STYLE030,LINT019 - gpu_tier_armed, the vulkan gate
require _tts_parity   // nolint:STYLE030,LINT019 - styletts2_spoken, inside the GPU halves
require ../performance/model_specs.das   // nolint:STYLE030,LINT019 - counting_prompt

//! the context every Metal zoo cell cuts its form's model to: the
//! digit-splitting tokenizers encode the 140-number counting prompt
//! to about 590 tokens, and eval's capacity tripwire panics past it
let KERNEL_COVERAGE_CTX = 1024l

//! the context every Vulkan census cell cuts its form's model to
let VK_KERNEL_COVERAGE_CTX = 512l

//! the home a tower cell's census runs on: its arm, its gate, the
//! tower knob it pins; `none` a build whose GPU home serves no census
enum CovHome {
    metal
    vulkan
    none
}

//! the home the tower family `fam`'s census runs on in this build:
//! Metal on a das_metal build, else the Vulkan home where `fam`
//! serves there, else none
def cov_home_of(fam : TowerFamily) : CovHome {   // nolint:LINT012,LINT019 - GPU halves
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return CovHome.metal
    } else {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return serving_tower_dev(fam) == TowerDev.vulkan ? CovHome.vulkan : CovHome.none
        } else {
            return CovHome.none
        }
    }
}

def private note_metal_crowns {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        for (f in split(get_metal_tensor_crowns(), ",")) {
            if (!empty(f)) {
                kernel_census_note_flag("{NOTE_METAL_CROWN}{f}")
            }
        }
    }
}

def private note_vk_device {   // nolint:LINT012,LINT019 - vulkan half
    static_if (typeinfo builtin_module_exists(vulkan)) {
        if (vk_mmq_any()) {
            kernel_census_note_flag(NOTE_VK_INT8_COLUMNS)
        }
    }
}

//! whether a census cell runs here: the row's family, the home's arm
//! and the home's own gate (JIT on Metal, an armed tier on Vulkan)
def private cov_on(t : T?; home : CovHome; family : string) : bool {
    return false if (home == CovHome.none || (!empty(family) && !family_on(t, family)))
    if (home == CovHome.metal) {
        return arm_on(t, "coverage") && jit_only(t)
    }
    return arm_on(t, "coverage-vk") && gpu_tier_armed(t)
}

//! `body` with the home's tower knob pinned on - on Metal the ASR
//! decoder's too - every knob put back after it
[unused_argument(home)]
def private with_census_towers(home : CovHome; body : block<() : void>) {
    with_gpu_knobs() {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            if (home == CovHome.metal) {
                set_metal_tower(true)
                set_metal_wdec(true)
            }
        }
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (home == CovHome.vulkan) {
                set_vulkan_tower(true)
            }
        }
        invoke(body)
    }
}

//! a prefill of the counting prompt to `last` and two greedy decode
//! steps through `s`
def private cov_prefill_two_steps(m : Model; var s : Session; last : int) {
    let vocab = m.config.vocab_size
    var inscope p <- encode(m, counting_prompt(1, last), true, false)
    eval_(m, s, p)
    var cur = argmax_of(s.logits, vocab)
    for (_i in range(2)) {
        var inscope one <- [cur]
        eval_(m, s, one)
        cur = argmax_of(s.logits, vocab)
    }
}

//! the continuation prefill (start_pos > 0): the window-landing
//! utilities serve only when a second window lands on a warm cache
def private cov_continuation(m : Model; var s : Session) {
    var inscope p2 <- encode(m, counting_prompt(50, 90), false, false)
    eval_(m, s, p2)
}

//! one batched step over `nb` sessions of codec `kdt`, each prefilled
//! on its own counting prompt
def private cov_batch_step(m : Model; kdt : KVDtype; nb : int) {
    let vocab = m.config.vocab_size
    var inscope ss : array<Session>
    ss |> reserve(nb)
    var sess : array<Session?>   //! borrowed handles into ss
    sess |> reserve(nb)
    var inscope curs : array<int64>
    curs |> reserve(nb)
    for (i in range(nb)) {
        ss |> emplace <| make_run_state(m.config, kdt, kdt)
        var inscope pb <- encode(m, counting_prompt(3 + i, 44 + i), true, false)
        eval_(m, ss[i], pb)
        sess |> push(unsafe(addr(ss[i])))
        curs |> push(argmax_of(ss[i].logits, vocab))
    }
    var ws <- create_batch_workspace_(m)
    eval_batch_(m, ws, sess, curs)
    sess |> clear()   // borrowed handles: a delete of array<T?> cascades to the pointees
    unsafe {
        delete ws
        delete sess
    }
}

//! One Metal zoo row: its file, family token, KV codecs, batch shapes
//! and the trunk blocks its cut claims (0: no claim)
struct CovZooRow {
    file : string
    family : string
    kdts : array<KVDtype>
    nbs : array<int>
    blocks : int64
}

//! one zoo row's sweep on the form's model, the blob twin, as a
//! subtest of `t` its gates skip alone: per KV codec a prefill past
//! the half-X, tall-tile and MoE-gather row floors, two greedy steps
//! and a continuation, then a batched step a shape. No parity assert:
//! the census wants dispatch diversity
[unused_argument(form)]
def coverage_zoo_metal(t : T?; var m : Model; form : ModelForm; row : CovZooRow) {
    let what = "{row.file}: the kernel census's zoo row"
    coverage_cell(t, what, cov_metal_home(), m, row, @@zoo_census)
}

[unused_argument(t, m, row)]   // off-Apple the gated body is empty
def private zoo_census(t : T?; var m : Model; row : CovZooRow) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return if (!cov_on(t, CovHome.metal, row.family))
        if (row.blocks > 0l) {
            cut_within(t, "{row.file}: ", m, row.blocks)
        }
        note_metal_crowns()
        with_metal_rails(MetalRails(min_npos = 0l)) {
            with_ctx_cut(m, KERNEL_COVERAGE_CTX) {
                with_dasllama_jobque() {
                    for (kdt in row.kdts) {
                        var s = make_run_state(m.config, kdt, kdt)
                        cov_prefill_two_steps(m, s, 140)
                        cov_continuation(m, s)
                        delete s
                        for (nb in row.nbs) {
                            cov_batch_step(m, kdt, nb)
                        }
                    }
                }
            }
        }
    }
}

def private zoo_row(file, family : string; kdts : array<KVDtype>; nbs : array<int>;
                    blocks : int64 = 0l) : CovZooRow {
    var r = CovZooRow(file = file, family = family, blocks = blocks)
    r.kdts := kdts
    r.nbs := nbs
    return <- r
}

let private ALL_CODECS = fixed_array(KVDtype.f16, KVDtype.q8_0, KVDtype.tq4)

//! C1: llama, the dim-ladder base - all three codecs, B 2/4/8
def cov_zoo_c1_llama_q8 : CovZooRow
    => <- zoo_row("Llama-3.2-1B-Instruct-Q8_0.gguf", "llama", [for (k in ALL_CODECS); k], [2, 4, 8])

//! C2: the same carrier on the f16 weight-scale plane, f16, B 2
def cov_zoo_c2_llama_q8_s16 : CovZooRow
    => <- zoo_row("Llama-3.2-1B-Instruct-Q8_0.gguf", "llama", [KVDtype.f16], [2])

//! C3: the q5_K tail - the k5 GEMV, mul_mm and batch tiles
def cov_zoo_c3_llama_q5km : CovZooRow
    => <- zoo_row("Llama-3.2-1B-Instruct-Q5_K_M.gguf", "llama", [KVDtype.f16], [2, 4, 8])

//! C4: the NVFP4 mint - the nv4 GEMV, its small-batch forms and its
//! dense mul_mm under the box's crown
def cov_zoo_c4_llama_nvfp4 : CovZooRow
    => <- zoo_row("Llama-3.2-1B-Instruct-NVFP4-local.gguf", "llama", [KVDtype.f16], [2, 4, 8])

//! C5: gemma2 - K-quants under softcap, sliding, geglu and wide
//! heads; the three codecs reach the head-of-256 attention's three
//! stamps
def cov_zoo_c5_gemma2 : CovZooRow
    => <- zoo_row("gemma-2-2b-it-Q4_K_M.gguf", "gemma2", [for (k in ALL_CODECS); k], [2, 4, 8])

//! C6: phi3 - a head of 96 and an untied K-quant embedding
def cov_zoo_c6_phi3 : CovZooRow
    => <- zoo_row("Phi-3.5-mini-instruct-Q4_K_M.gguf", "phi3", [KVDtype.f16, KVDtype.q8_0], [2])

//! C7: qwen3 - qk_norm and qd != dim
def cov_zoo_c7_qwen3 : CovZooRow
    => <- zoo_row("Qwen3-0.6B-Q8_0.gguf", "qwen3", [KVDtype.f16, KVDtype.tq4], [2])

//! C8: qwen2 - qkv_bias
def cov_zoo_c8_qwen2 : CovZooRow
    => <- zoo_row("Qwen2.5-0.5B-Instruct-Q8_0.gguf", "qwen2", [KVDtype.f16, KVDtype.q8_0], [2])

//! C9: qwen3.5 - the DeltaNet hybrid (its batch is the per-row
//! fallback by design)
def cov_zoo_c9_qwen35 : CovZooRow
    => <- zoo_row("Qwen3.5-0.8B-Q8_0.gguf", "qwen35", [KVDtype.f16], [2])

//! C10: gemma4e - PLE and V-from-K, no batch shape (a blob-twin CPU
//! batch fallback trips the blob-only panic); the three codecs reach
//! the head-of-512 attention's three stamps
def cov_zoo_c10_gemma4e : CovZooRow {
    let none : array<int>
    return <- zoo_row("gemma-4-E2B-it-Q8_0.gguf", "gemma4e", [for (k in ALL_CODECS); k], none)
}

//! C11: gemma4 12B's first twelve blocks - the checkpoint suppress
//! list and K-quants at 12B shapes
def cov_zoo_c11_gemma4_12b : CovZooRow
    => <- zoo_row("gemma-4-12B-12L-Q4_K_M-local.gguf", "gemma4", [KVDtype.f16], [2], 12l)

//! C12: qwen3moe - the whole-graph MoE family; no batch shape (MoE
//! batch declines are graph-level)
def cov_zoo_c12_qwen3moe : CovZooRow {
    let none : array<int>
    return <- zoo_row("Qwen3-30B-A3B-7L-Q4_K_M-local.gguf", "qwen3moe", [KVDtype.f16], none, 7l)
}

//! C13: gptoss - the MXFP4 expert stacks, swiglu_oai and attention
//! sinks; no batch shape
def cov_zoo_c13_gptoss : CovZooRow {
    let none : array<int>
    return <- zoo_row("gpt-oss-4L-mxfp4-local.gguf", "gptoss", [KVDtype.f16], none, 4l)
}

//! C14: qwen2moe - q8 expert stacks and the sigmoid-gated shared
//! expert; no batch shape
def cov_zoo_c14_qwen2moe : CovZooRow {
    let none : array<int>
    return <- zoo_row("Qwen1.5-MoE-A2.7B-4L-Q8_0-local.gguf", "qwen2moe", [KVDtype.f16], none, 4l)
}

//! C15: gemma4moe - the one carrier of a Q4_0 routed expert plane;
//! no batch shape
def cov_zoo_c15_gemma4moe : CovZooRow {
    let none : array<int>
    return <- zoo_row("gemma-4-26B-A4B-6L-Q4_0-local.gguf", "gemma4moe", [KVDtype.f16], none, 6l)
}

//! a canvas of `side` x `side` RGB planes: amp * sin(i * freq) + bias
def private cov_canvas(side : int64; freq, amp, bias : float) : array<float> {
    var planes : array<float>
    planes |> reserve_resize(side * side * 3l)
    for (i in range64(side * side * 3l)) {
        planes[i] = sin(float(i) * freq) * amp + bias
    }
    return <- planes
}

//! the jfk clip at 16 kHz mono, empty where the corpus lacks it
def private cov_samples : array<float> {
    var samples : array<float>
    static_if (typeinfo builtin_module_exists(das_metal)) {
        samples <- load_audio_16k_mono(asr_clip("jfk.wav"))
    }
    return <- samples
}

//! the census home of a Metal-only cell: Metal on a das_metal build,
//! none on any other
def private cov_metal_home : CovHome {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return CovHome.metal
    } else {
        return CovHome.none
    }
}

//! the census home of a Vulkan row: Vulkan on a build with the
//! module, none on any other
def private cov_vk_home : CovHome {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return CovHome.vulkan
    } else {
        return CovHome.none
    }
}

//! a census cell of a block on `home`: `census` on the form's model
//! `tw` and the cell's row `row` (a tower cell's row is its home), as
//! the subtest `what` of `t` its gates skip alone; no subtest where
//! `home` is none
def private coverage_cell(t : T?; what : string; home : CovHome; var tw : auto(TW); row : auto(ROW);
                          census : auto(CELL)) {
    return if (home == CovHome.none)
    unsafe {   // nolint:STYLE025 - a capture by reference, read untyped
        t |> run(what) @capture(& tw, & row)(t : T ?) {
            invoke(census, t, tw, row)
        }
    }
}

//! a tower cell's census on `home`: the row's gates (`cov_on`), then
//! `encode` with the home's tower knobs pinned, on a job queue
def private tower_census(t : T?; home : CovHome; family : string; encode : block<() : void>) {
    return if (!cov_on(t, home, family))
    with_census_towers(home) {
        with_dasllama_jobque() {
            invoke(encode)
        }
    }
}

//! C16/C17: whisper tiny's mixed chunk on the form's Metal bake - the
//! tower LUT flavor's encoder, the wdec driver's decoder
def coverage_tower_whisper_metal(t : T?; what : string; var w : WhisperModel) {
    coverage_cell(t, what, cov_metal_home(), w, cov_metal_home(), @@whisper_census)
}

def private whisper_census(t : T?; var w : WhisperModel; home : CovHome) {
    tower_census(t, home, "") {
        var inscope samples <- cov_samples()
        return if (empty(samples))
        var ws <- create_session(w)
        transcribe(w, ws, samples) $(_seg) {
            pass
        }
        delete ws
    }
}

//! C18: the erf-GELU carrier - one qwen2audio chunk on varied mel
def coverage_tower_qwen2audio_metal(t : T?; what : string; var atw : AudioTower) {
    coverage_cell(t, what, cov_metal_home(), atw, cov_metal_home(), @@qwen2audio_census)
}

def private qwen2audio_census(t : T?; var atw : AudioTower; home : CovHome) {
    tower_census(t, home, "") {
        var mel = MelChunk(n_mel = atw.n_mel, n_len = AUDIO_CHUNK_FRAMES)
        mel.data |> resize(atw.n_mel * AUDIO_CHUNK_FRAMES)
        for (i in range(length(mel.data))) {
            mel.data[i] = sin(float(i) * 0.0137) * 0.8
        }
        var aenc <- make_encoder_state(atw)
        audio_encode(atw, aenc, mel)
        delete aenc
        delete mel
    }
}

//! C19: the qwen3a conv2d and mel carrier - the mel built from raw
//! samples, so the GPU mel hook dispatches ahead of the im2col2d
//! window and its padded tile GEMMs
def coverage_tower_qwen3a_metal(t : T?; what : string; var qtw : Qwen3aTower) {
    coverage_cell(t, what, cov_metal_home(), qtw, cov_metal_home(), @@qwen3a_census)
}

def private qwen3a_census(t : T?; var qtw : Qwen3aTower; home : CovHome) {
    tower_census(t, home, "") {
        var inscope samples <- cov_samples()
        return if (empty(samples))
        var qenc <- make_encoder_state(qtw.core)
        var qcs = Qwen3aConvState()
        var qrows : array<float>
        var qmel <- log_mel_qwen3a(samples, qtw.core.n_mel)
        qwen3a_encode_all(qtw, qenc, qcs, qmel, qrows)
        delete qmel
        delete qrows
        delete qcs
        delete qenc
    }
}

//! C20: one gemma4a whole chunk from raw samples - GPU mel, both
//! subsample convs and the 12-block Conformer chain
def coverage_tower_gemma4a_metal(t : T?; what : string; var genc : Gemma4aEncoder) {
    coverage_cell(t, what, cov_metal_home(), genc, cov_metal_home(), @@gemma4a_census)
}

def private gemma4a_census(t : T?; var genc : Gemma4aEncoder; home : CovHome) {
    tower_census(t, home, "gemma4a") {
        var inscope samples <- cov_samples()
        return if (empty(samples))
        var gst = Gemma4aState()
        var grows : array<float>
        gemma4a_encode_all(genc, gst, samples, grows)
        delete grows
        delete gst
    }
}

//! C21: CPU mel, then the 32-block FastConformer chain - the rel-pos
//! XL attention and the centered-k9 depthwise kernels
def coverage_tower_canary_metal(t : T?; what : string; var cenc : CanaryEncoder) {
    coverage_cell(t, what, cov_metal_home(), cenc, cov_metal_home(), @@canary_census)
}

def private canary_census(t : T?; var cenc : CanaryEncoder; home : CovHome) {
    tower_census(t, home, "canary") {
        var inscope samples <- cov_samples()
        return if (empty(samples))
        var cst = CanaryState()
        canary_log_mel(cenc, samples, cst)
        canary_encode(cenc, cst)
        delete cst
    }
}

//! C22: the tap-major depthwise stamp (enc_pk_dw) and the transducer
//! decode's four stamps off the encode's device rows
def coverage_tower_parakeet_metal(t : T?; what : string; var pm : ParakeetModel) {
    coverage_cell(t, what, cov_metal_home(), pm, cov_metal_home(), @@parakeet_census)
}

def private parakeet_census(t : T?; var pm : ParakeetModel; home : CovHome) {
    tower_census(t, home, "parakeet") {
        var inscope samples <- cov_samples()
        return if (empty(samples))
        var pst = ParakeetState()
        parakeet_log_mel(samples, pm, pst)
        let tt = parakeet_encode(pm, pst)
        parakeet_tdt_decode(pm, pst, tt)
        delete pst
    }
}

//! C23: the GigaAM rotary Conformer's whole encode off raw samples on
//! the f16 planes - device mel, conv front, blocks - and the
//! transducer decode's four stamps
def coverage_tower_gigaam_metal(t : T?; what : string; var gm : GigaamModel) {
    coverage_cell(t, what, cov_metal_home(), gm, cov_metal_home(), @@gigaam_census)
}

def private gigaam_census(t : T?; var gm : GigaamModel; home : CovHome) {
    tower_census(t, home, "gigaam") {
        var inscope samples <- cov_samples()
        return if (empty(samples))
        var gst = GigaamState()
        let tt = gigaam_encode_span(samples, 0l, long_length(samples), gm, gst)
        gigaam_decode(gm, gst, tt)
        delete gst
    }
}

let private BERGAMOT_LINES = fixed_array(
    "Где ближайшая аптека?",   // nolint:STYLE039,LINT019 - the source language's own text
    "Сегодня мы идём в горы.")   // nolint:STYLE039,LINT019 - the source language's own text

//! C24: two Russian sentences through ru-en tiny on the q8 lane, the
//! whole sentence on the device - embed, SSRU and pick stamps beside
//! the borrowed GEMM, attention and seam kernels
def coverage_tower_bergamot_metal(t : T?; what : string; var m : NmtModel) {
    coverage_cell(t, what, cov_metal_home(), m, cov_metal_home(), @@bergamot_census)
}

def private bergamot_census(t : T?; var m : NmtModel; home : CovHome) {
    tower_census(t, home, "bergamot") {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let was = metal_nmt_knob()
            set_metal_nmt(true)
            var inscope st = NmtState()
            for (ln in BERGAMOT_LINES) {
                nmt_translate(m, st, ln)
            }
            set_metal_nmt(was)
        }
    }
}

//! C25: a small canvas through the 12B dense embedder chain
def coverage_tower_gemma4uv_metal(t : T?; what : string; var emb : Gemma4uvEmbedder) {
    coverage_cell(t, what, cov_metal_home(), emb, cov_metal_home(), @@gemma4uv_census)
}

def private gemma4uv_census(t : T?; var emb : Gemma4uvEmbedder; home : CovHome) {
    tower_census(t, home, "") {
        var st = Gemma4uvState()
        var inscope planes <- cov_canvas(96l, 0.013, 0.7, 0.0)
        var out : array<float>
        gemma4uv_encode(emb, st, planes, 96l, 96l, out)
        delete out
        delete st
    }
}

//! C26: a 96x96 canvas through the E2B ViT tower on the file's planes
def coverage_tower_gemma4v_metal(t : T?; what : string; var tw : Gemma4vTower) {
    coverage_cell(t, what, cov_metal_home(), tw, cov_metal_home(), @@gemma4v_census)
}

def private gemma4v_census(t : T?; var tw : Gemma4vTower; home : CovHome) {
    tower_census(t, home, "") {
        var st = Gemma4vState()
        var inscope planes <- cov_canvas(96l, 0.017, 0.5, 0.5)
        var out : array<float>
        gemma4v_encode(tw, st, planes, 96l, 96l, out)
        delete out
        delete st
    }
}

//! C27: the fixed 896 canvas through the SigLIP tower on the f32
//! planes, both attention routes - the 4096-token canvas serves the
//! tower flash, then a flash-off encode drives the slab route
def coverage_tower_gemma3v_metal(t : T?; what : string; var tw : Gemma3vTower) {
    coverage_cell(t, what, cov_metal_home(), tw, cov_metal_home(), @@gemma3v_census)
}

def private gemma3v_census(t : T?; var tw : Gemma3vTower; home : CovHome) {
    tower_census(t, home, "gemma3v") {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            var st = Gemma3vState()
            var inscope planes <- cov_canvas(896l, 0.017, 0.5, 0.5)
            var out : array<float>
            let prev_flash = get_metal_tower_flash()
            gemma3v_encode(tw, st, planes, 896l, 896l, out)
            set_metal_tower_flash(false)
            var out2 : array<float>
            gemma3v_encode(tw, st, planes, 896l, 896l, out2)
            set_metal_tower_flash(prev_flash)
            delete out2
            delete out
            delete st
        }
    }
}

//! C28: one 896 encode on the crowned twin-W form, whose bake holds
//! the halfword twin - the f16-route kernels (cvt_half, the half
//! GEMMs) dispatch
def coverage_tower_gemma3v_twin_w_metal(t : T?; what : string; var tw : Gemma3vTower) {
    coverage_cell(t, what, cov_metal_home(), tw, cov_metal_home(), @@gemma3v_twin_w_census)
}

def private gemma3v_twin_w_census(t : T?; var tw : Gemma3vTower; home : CovHome) {
    tower_census(t, home, "gemma3v") {
        var st = Gemma3vState()
        var inscope planes <- cov_canvas(896l, 0.017, 0.5, 0.5)
        var out : array<float>
        gemma3v_encode(tw, st, planes, 896l, 896l, out)
        delete out
        delete st
    }
}

//! C29: one 112x112 window (16 tokens) through the window ViT - the
//! per-window attention kernel and the row-gather stem sort
def coverage_tower_qwen25v_metal(t : T?; what : string; var tw : Qwen25vTower) {
    coverage_cell(t, what, cov_metal_home(), tw, cov_metal_home(), @@qwen25v_census)
}

def private qwen25v_census(t : T?; var tw : Qwen25vTower; home : CovHome) {
    tower_census(t, home, "qwen25v") {
        var st = Qwen25vState()
        var inscope planes <- cov_canvas(112l, 0.019, 0.5, 0.5)
        var out : array<float>
        qwen25v_encode(tw, st, planes, 112l, 112l, out)
        delete out
        delete st
    }
}

//! C30: a 96x96 canvas through the merger tower - the fused GPU stem
//! (patch GEMM, merge-walk assemble) ahead of the block slab
def coverage_tower_qwen3v_metal(t : T?; what : string; var tw : Qwen3vTower) {
    coverage_cell(t, what, cov_metal_home(), tw, cov_metal_home(), @@qwen3v_census)
}

def private qwen3v_census(t : T?; var tw : Qwen3vTower; home : CovHome) {
    tower_census(t, home, "qwen3v") {
        var st = Qwen3vState()
        var inscope planes <- cov_canvas(96l, 0.023, 0.5, 0.5)
        var out : array<float>
        qwen3v_encode(tw, st, planes, 96l, 96l, out)
        delete out
        delete st
    }
}

//! C31/C32 (Metal), C66/C67 (Vulkan): one synthesis of the StyleTTS2
//! file `file` on `home` (`cov_home_of`) - kitten-nano.gguf the ONNX
//! law's stamps, kokoro-82m.gguf the torch law's
def coverage_tower_styletts2(t : T?; what : string; home : CovHome; var m : StyleTts2Model;
                             file : string) {
    coverage_cell(t, what, home, m, (home = home, file = file), @@styletts2_census)
}

def private styletts2_census(t : T?; var m : StyleTts2Model;
                             row : tuple<home : CovHome; file : string>) {
    tower_census(t, row.home, row.file == "kokoro-82m.gguf" ? "kokoro" : "kitten") {
        styletts2_spoken(m, 48)
    }
}

//! C33 (Metal), C68 (Vulkan): pocket-tts-en.gguf's codec over 24
//! latent frames and one spoken line, on `home` (`cov_home_of`)
def coverage_tower_pocket(t : T?; what : string; home : CovHome; var m : PocketModel) {
    coverage_cell(t, what, home, m, home, @@pocket_census)
}

def private pocket_census(t : T?; var m : PocketModel; home : CovHome) {
    tower_census(t, home, "pocket") {
        var inscope sc = PocketScratch()
        let frames = 24l
        var inscope lat : array<float>
        lat |> resize(frames * m.latent_dim)
        for (i in range(length(lat))) {
            lat[i] = 0.1 * float(i % 17) - 0.8
        }
        var inscope wave : array<float>
        var tm = TtsTimings()
        pocket_decode_latents(m, lat, frames, sc, wave)
        pocket_speak(m, "The census row speaks one short line.", m.default_voice,
            m.default_temperature, 1ul, tm, sc)
    }
}

//! C34 (Metal), C69 (Vulkan): pocket-tts-en-kq.gguf, the small form -
//! its head's q8 linears take the q8 stamps - on `home`
//! (`cov_home_of`)
def coverage_tower_pocket_kq(t : T?; what : string; home : CovHome; var m : PocketModel) {
    coverage_cell(t, what, home, m, home, @@pocket_kq_census)
}

def private pocket_kq_census(t : T?; var m : PocketModel; home : CovHome) {
    tower_census(t, home, "pocket") {
        var inscope sck = PocketScratch()
        var tm = TtsTimings()
        pocket_speak(m, "The small form speaks the same line.", m.default_voice,
            m.default_temperature, 1ul, tm, sck)
    }
}

//! C35 (Metal), C70 (Vulkan): the silero VAD's two kernels - the
//! batch probabilities over five seconds of a synthetic signal and
//! one streaming feed on the device lever - on `home` (`cov_home_of`)
def coverage_tower_vad(t : T?; what : string; home : CovHome; var v : VadModel) {
    coverage_cell(t, what, home, v, home, @@vad_census)
}

def private vad_census(t : T?; var v : VadModel; home : CovHome) {
    tower_census(t, home, "vad") {
        var sig : array<float>
        sig |> resize(5 * 16000)
        for (i in range(length(sig))) {
            sig[i] = 0.3 * sin(float(i) * 0.05) * sin(float(i) * 0.0007)
        }
        var inscope probs <- vad_probs(v, sig)
        let was = set_vad_stream_gpu(true)
        var it : VadIter
        vad_iter_reset(it, default_vad_opts())
        vad_iter_feed(v, it, sig) $(_ev) {
            pass
        }
        set_vad_stream_gpu(was)
        delete sig
    }
}

let NO_TWIN = -1   //! a row no forced coopmat mode routes to a twin stamp

//! One Vulkan census row: its file, family token, batch shapes, the
//! trunk blocks its cut claims (0: no claim), and on a twin leg of
//! the mode sweep the plane format whose coopmat twin the form's
//! forced mode reaches
struct CovVkRow {
    file : string
    family : string
    nbs : array<int>
    blocks : int64
    twin_fmt : int = NO_TWIN
}

//! One codec carrier row: its file, family token and the block codec
//! its form asks the K/V mirror in (`device_form() |> asking(kv)`)
struct CovVkCodecRow {
    file : string
    family : string
    kv : KVDtype
}

//! the forced coopmat mode of a twin leg by its `DASLLAMA_COOPMAT`
//! spelling
def private vk_cov_twin_mode : string {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return g_gpu.coopmat_mode == COOPMAT_F16 ? "f16" : "int8"
    } else {
        return ""
    }
}

//! whether this device builds the batch tile the forced coopmat mode
//! routes a plane of format `fmt` to: the answer of the router the
//! window itself asks, read under that mode
[unused_argument(fmt)]   // without vulkan the gated body is empty
def private vk_cov_twin_builds(fmt : int) : bool {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        if (fmt == int(KqFmt.q8)) {
            return q8_batch_cls_ensure(q8_batch_cls_variant(q8_batch_tile(), false))
        }
        return kq_batch_cls_ensure(fmt, kq_batch_q40cm(fmt))
    } else {
        return false
    }
}

//! the device form a census model or routed row runs on: the planes
//! kept on the host beside the device's copy, which the row's f32
//! sessions read; `coopmat` the mode a sweep leg pins
def vk_census_form(coopmat : int = -1) : ModelForm
    => device_form(host = true, coopmat = coopmat)

//! whether the run reaches the census's Vulkan rows: a build with the
//! Vulkan home, off the CUDA home, the `coverage-vk` arm on
def vk_census_asked : bool {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return !gpu_want_cuda() && empty(arm_why("coverage-vk"))
    } else {
        return false
    }
}

//! whether `f` is a `vk_census_form`, under any coopmat mode
def vk_census_form_is(f : ModelForm) : bool {
    var plain = f
    plain.host = false
    plain.coopmat = -1
    return f.host && !f.drops_device && device_default(plain)
}

//! one row's sweep on a `vk_census_form`'s model: an f32 session (the
//! CPU rails over the host's planes) and an f16 one (the whole-model
//! driver), each a prefill, two greedy steps and a continuation, then
//! one batched step of f32 sessions a shape - the passes those leave
//! cleared where the form had none; false where the gate, or on a
//! twin leg the device's decline of that twin, leaves the row out
[unused_argument(t, m, row)]   // without vulkan the gated body is empty
def private vk_cov_sweep(t : T?; var m : Model; row : CovVkRow) : bool {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return false if (!cov_on(t, CovHome.vulkan, row.family))
        if (row.twin_fmt != NO_TWIN && !vk_cov_twin_builds(row.twin_fmt)) {
            t |> skip(("{row.file}: the {vk_cov_twin_mode()} coopmat mode skipped - this device " +
                "declines the batch tile that mode routes the file's planes to (the driver's " +
                "decline line names the stamp and the reason)"))
            return false
        }
        if (row.blocks > 0l) {
            cut_within(t, "{row.file}: ", m, row.blocks)
        }
        note_vk_device()
        var passed <- gpu_cpu_passes_()
        let served = empty(passed)
        delete passed
        with_ctx_cut(m, VK_KERNEL_COVERAGE_CTX) {
            with_dasllama_jobque() {
                for (kdt in fixed_array(KVDtype.f32, KVDtype.f16)) {
                    var s = make_run_state(m.config, kdt, kdt)
                    cov_prefill_two_steps(m, s, 40)
                    cov_continuation(m, s)
                    delete s
                }
                for (nb in row.nbs) {
                    cov_batch_step(m, KVDtype.f32, nb)
                }
            }
        }
        if (served) {
            reset_gpu_cpu_passes()   //! the f32 sessions pass to the CPU rails by design
        }
        return true
    } else {
        return false
    }
}

//! a Vulkan census row (C36-C38, C44-C61) on the device form's model,
//! as a subtest of `t` its gates skip alone; no parity assert - the
//! census wants dispatch diversity
[unused_argument(form)]
def coverage_vk_model(t : T?; var m : Model; form : ModelForm; row : CovVkRow) {
    let what = "{row.file}: the kernel census's Vulkan row"
    coverage_cell(t, what, cov_vk_home(), m, row, @@vk_model_census)
}

def private vk_model_census(t : T?; var m : Model; row : CovVkRow) {
    vk_cov_sweep(t, m, row)
}

//! a routed row (C62-C65): `coverage_vk_model`, and where it ran the
//! carrier note the conditional entries read - one routed row
[unused_argument(form)]
def coverage_vk_routed(t : T?; var m : Model; form : ModelForm; row : CovVkRow) {
    let what = "{row.file}: the kernel census's routed row"
    coverage_cell(t, what, cov_vk_home(), m, row, @@vk_routed_census)
}

def private vk_routed_census(t : T?; var m : Model; row : CovVkRow) {
    return if (!vk_cov_sweep(t, m, row))
    static_if (typeinfo builtin_module_exists(vulkan)) {
        kernel_census_note_add(NOTE_VK_ROUTED, 1l)
    }
}

//! a codec carrier row (C39-C43) on its form's model, the mirror in
//! the row's block codec, on a session of that codec: a prefill
//! (the window's staged store, the shadow, the flash tiles over it)
//! and two decode steps (the token store, the codec's attention
//! stamp, tq4's un-rotation), as a subtest of `t` its gates skip
//! alone. A model the engine serves no such cache on skips by name:
//! the mirror arms another codec there
[unused_argument(form)]
def coverage_vk_codec(t : T?; var m : Model; form : ModelForm; row : CovVkCodecRow) {
    let what = "{row.file}: the kernel census's {row.kv} codec row"
    coverage_cell(t, what, cov_vk_home(), m, row, @@vk_codec_census)
}

[unused_argument(t, m, row)]   // without vulkan the gated body is empty
def private vk_codec_census(t : T?; var m : Model; row : CovVkCodecRow) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return if (!cov_on(t, CovHome.vulkan, row.family))
        note_vk_device()
        let unserved = kv_codec_serving_why(m.config, row.kv)
        if (!empty(unserved)) {
            t |> skip(("{row.file}: the {row.kv} pass skipped, the engine serves no {row.kv} K/V " +
                "cache on this model - {unserved}"))
            return
        }
        with_ctx_cut(m, VK_KERNEL_COVERAGE_CTX) {
            with_dasllama_jobque() {
                var s = make_run_state(m.config, row.kv, row.kv)
                cov_prefill_two_steps(m, s, 40)
                delete s
            }
        }
    }
}

def private vk_row(file, family : string; nbs : array<int>; blocks : int64 = 0l) : CovVkRow {
    var r = CovVkRow(file = file, family = family, blocks = blocks)
    r.nbs := nbs
    return <- r
}

//! C36: llama q8 - the resident dense ladder and the per-op rails'
//! batched steps at 2, 4 and 8 f32 sessions
def cov_vk_c36_tinyllama : CovVkRow
    => <- vk_row("tinyllama-1.1b-chat-v1.0.Q8_0.gguf", "llama", [2, 4, 8])

//! C37: gemma3 q8 - the per-op arena rails and qk_norm
def cov_vk_c37_gemma3 : CovVkRow {
    let none : array<int>
    return <- vk_row("gemma-3-1b-it-Q8_0.gguf", "gemma3", none)
}

//! C38: phi3 K-quant - a head of 96: the window's head-96 flash tiles
//! (`fa_cm2_h96*`), the token step's narrow attention
def cov_vk_c38_phi3 : CovVkRow {
    let none : array<int>
    return <- vk_row("Phi-3.5-mini-instruct-Q4_K_M.gguf", "phi3", none)
}

//! C60: qwen3.5 q8 - the DeltaNet chains and the decode step
def cov_vk_c60_qwen35 : CovVkRow {
    let none : array<int>
    return <- vk_row("Qwen3.5-0.8B-Q8_0.gguf", "qwen35", none)
}

//! C61: qwen3.5 9B (large tier) - head-256 gated attention on the cm2
//! flash stamps, the K-quant deltanet planes on the resident driver
def cov_vk_c61_qwen35_9b : CovVkRow {
    let none : array<int>
    return <- vk_row("Qwen3.5-9B-MTP-UD-Q5_K_XL.gguf", "qwen35", none)
}

//! C62: the Qwen1.5-MoE Q8_0 cut's routed block
def cov_vk_c62_qwen2moe_q8 : CovVkRow {
    let none : array<int>
    return <- vk_row("Qwen1.5-MoE-A2.7B-4L-Q8_0-local.gguf", "qwen2moe", none, 4l)
}

//! C63: the Qwen3-30B Q4_K_M cut's routed block
def cov_vk_c63_qwen3moe_q4km : CovVkRow {
    let none : array<int>
    return <- vk_row("Qwen3-30B-A3B-7L-Q4_K_M-local.gguf", "qwen3moe", none, 7l)
}

//! C64: the Qwen1.5-MoE Q4_K_M twin, whole on a 16 GB card - the
//! whole-model driver's MoE block
def cov_vk_c64_qwen2moe_q4km : CovVkRow {
    let none : array<int>
    return <- vk_row("Qwen1.5-MoE-A2.7B-4L-Q4_K_M-local.gguf", "qwen2moe", none, 4l)
}

//! C65: the UD-IQ2_XXS 30B - the resident MoE block's s and e stamps
//! over iq2xxs expert planes
def cov_vk_c65_qwen3moe_iq2xxs : CovVkRow {
    let none : array<int>
    return <- vk_row("Qwen3-30B-A3B-7L-UD-IQ2_XXS-local.gguf", "qwen3moe", none, 7l)
}

//! C39/C40: qwen2 under q8_0 / tq4 - the four-head slab (the engine
//! serves no tq4 cache on qwen2's biased attention: that pass skips)
def cov_vk_codec_qwen2(kv : KVDtype) : CovVkCodecRow
    => CovVkCodecRow(file = "Qwen2.5-0.5B-Instruct-Q8_0.gguf", family = "qwen2", kv = kv)

//! C41/C42: gemma-2 under q8_0 / tq4 - the two-head slab
def cov_vk_codec_gemma2(kv : KVDtype) : CovVkCodecRow
    => CovVkCodecRow(file = "gemma-2-2b-it-Q8_0.gguf", family = "gemma2", kv = kv)

//! C43: tinyllama under tq4 - tq4's four-head slab
def cov_vk_c43_codec_tinyllama : CovVkCodecRow
    => CovVkCodecRow(file = "tinyllama-1.1b-chat-v1.0.Q8_0.gguf", family = "llama",
        kv = KVDtype.tq4)

struct private SweepVehicle {
    file : string
    family : string
    twin : int = NO_TWIN   //! the plane format the f16 and int8 twin legs reach
}

//! the mode sweep's vehicles, C44 to C59 in order: every prefill tile
//! family under the coopmat modes its planes have twins in
let private SWEEP_VEHICLES = fixed_array(
    SweepVehicle(file = "Qwen3-0.6B-Q8_0.gguf", family = "qwen3", twin = int(KqFmt.q8)),
    SweepVehicle(file = "Llama-3.2-1B-Instruct-Q4_0-local.gguf", family = "llama",
        twin = int(KqFmt.q40)),
    SweepVehicle(file = "Qwen3-4B-Instruct-2507-Q4_K_M.gguf", family = "qwen3"),   // k4 + k6
    SweepVehicle(file = "Llama-3.2-1B-Instruct-IQ4_XS.gguf", family = "llama"),
    SweepVehicle(file = "Llama-3.2-1B-Instruct-Q3_K_L.gguf", family = "llama"),   // k3 + k5
    SweepVehicle(file = "Llama-3.2-1B-Instruct-IQ3_M.gguf", family = "llama"),   // iq3s
    SweepVehicle(file = "Llama-3.2-1B-Instruct-IQ3_XXS-local.gguf", family = "llama"),
    SweepVehicle(file = "Llama-3.2-1B-Instruct-IQ4_NL-local.gguf", family = "llama"),
    SweepVehicle(file = "Llama-3.2-1B-Instruct-Q2_K-local.gguf", family = "llama"),
    SweepVehicle(file = "Llama-3.2-1B-Instruct-TQ2_0-local.gguf", family = "llama"),
    SweepVehicle(file = "Llama-3.2-1B-Instruct-TQ1_0-local.gguf", family = "llama"),
    SweepVehicle(file = "Llama-3.2-1B-Instruct-IQ1_S-local.gguf", family = "llama"),
    SweepVehicle(file = "Llama-3.2-1B-Instruct-IQ1_M-local.gguf", family = "llama"),
    SweepVehicle(file = "Llama-3.2-1B-Instruct-IQ2_XS-local.gguf", family = "llama"),
    SweepVehicle(file = "Llama-3.2-1B-Instruct-IQ2_XXS-local.gguf", family = "llama"),
    // iq2s on the attention planes
    SweepVehicle(file = "Llama-3.2-1B-Instruct.i1-IQ3_XXS.gguf", family = "llama"))

//! the mode sweep's vehicle count: rows C44 to C59
let COV_VK_SWEEP_ROWS = 16

//! whether sweep vehicle `i` (row C44 + i) takes the two twin legs -
//! the f16 and int8 coopmat modes - past the box's mode, mm and sdot4
def cov_vk_sweep_twin(i : int) : bool => SWEEP_VEHICLES[i].twin != NO_TWIN

//! sweep vehicle `i` (row C44 + i) as one form's row: `twin_leg` on
//! a form forcing the f16 or int8 coopmat mode, where the row asks
//! the device for the twin tile first
def cov_vk_sweep_row(i : int; twin_leg : bool) : CovVkRow {
    let none : array<int>
    var r <- vk_row(SWEEP_VEHICLES[i].file, SWEEP_VEHICLES[i].family, none)
    r.twin_fmt = twin_leg ? SWEEP_VEHICLES[i].twin : NO_TWIN
    return <- r
}
