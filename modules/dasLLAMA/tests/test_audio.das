options gen2
options persistent_heap   // model-scale allocations (2.5 GB mmproj tower) — freed per test
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_audio
require dasllama/dasllama_tower
require dasllama/dasllama_math   // with_dasllama_jobque_
require daslib/jobque_boost
require daslib/fio
require daslib/defer
require math
require _model_tier   // models_dir() + model_available() presence/size-tier gate (see the file's header)
require ?das_metal dasllama/dasllama_metal_tower   // pin the CPU-oracle encodes off-GPU + the blocks parity cell
require ?das_metal dasllama/dasllama_metal_common   // set_metal_take_max_probe_: the scratch declines' seam
require ?das_metal dasllama/dasllama_metal_prefill   // pf_hmm_ready: the half tensor GEMM, the exact lane's device condition
require _rnnt_oracle   // rnnt_seat_*: the build's device transducer seat, its pin and counters
require ?vulkan dasllama/dasllama_vulkan_tower   // the Vulkan home's seams: the storage-range legs' counters, gigaam's standalone mel
require ?vulkan dasllama/dasllama_vulkan_common   // g_gpu: the storage range the blocks-refused legs stand in
require _ctc_oracle   // nolint:STYLE030,LINT019 — ctc_leg / ctc_cpu_leg / ctc_hard_flips, inside the vulkan arm alone
require daslib/linq   // sequence_equal: the twin legs' ids
require daslib/strings_boost   // split: a kq carrier's members by name
require strings   // find: a refusal's words
require dasllama/dasllama_gguf   // GGML_TYPE_*: the retyped carriers' tensor types
require _gguf_writer   // gguf_retyped_copy: the retyped carriers the kq planner refuses
require dasllama/dasllama_kqformat   // kq_fmt_of_slot / kq_qsb: the poisoned kq leg's member planes
require dasllama/dasllama_gemma4a
require dasllama/dasllama_canary
require dasllama/dasllama_qwen3a
require dasllama/dasllama_parakeet
require dasllama/dasllama_asr   // load_asr_model, asr_needs_mmproj: the facade's Parakeet GGUF route
require dasllama/dasllama_gigaam
require dasllama/dasllama_audio_io
require _tower_twin
require _audio_twin   // the FastConformer twins' poison, rows and range-refusal legs
require _model_once   // with_model_once: a Parakeet carrier's one block in the area; own_load
require _audio_asr_cells   // the ASR families' oracle and facade cells, run under their files' blocks
require _audio_embedder_cells   // the audio carrier's gemma4a cells, run under the E2B mmproj's block
require _cpu_gpu_model   // with_cpu_and_gpu_model: a chat decoder's one block in the area
require _f32_tile_pin   // with_put_back: a mint's lane pin put back however its staging leaves
require dasllama/dasllama_env   // parity_full_mode: the full suite's flag
require _image_memory   // read_page_aligned: a host's own copy of an image
require dasllama/dasllama_image   // with_image_destination: the cold mint's scratch path
require ?cuda dasllama/dasllama_cuda_tower_fc   // cuda_gigaam_mel: gigaam's standalone device mel on the CUDA home
require ?cuda dasllama/dasllama_cuda_common   // cud_kernel_census_of: the kq lane's tile census on the CUDA home

// Audio tower tests. The mel/gelu/filterbank tests are self-contained; the encoder tests
// need the locally-converted Qwen2-Audio mmproj (2.5 GB, gitignored) and skip without it.
// The GigaAM device twins are cells of test_gigaam.das's blocks (`_audio_gigaam_cells`). A Parakeet carrier loads once a
// form, in its one block here (`parakeet_form`: the facade's `image` form, the forms minted in memory): the v2 and v3 f32
// bins' blocks carry the oracle cells (`_audio_asr_cells`), the device twins' and the device-decode twins' legs, the
// GGUF carriers' blocks the carrier and kq-lane cells; a compare of two forms, or of two files, follows the block on what
// the forms kept. The parakeet device-decode twin's forms skip where no GPU device stands up for the serving tower
// (`serving_tower_device_up`), its clips without jfk.wav or gb1.wav in benchmarks/asr/corpus/, on the lane the build's
// chain does not flavor (Metal's q8, a discrete home's exact), and where the build or the box has no device transducer
// seat (`rnnt_seat_skip`).
// The oracle constants come from llama-mtmd-debug (CPU backend). The discrete-home twins run on the home each family's
// seats serve on (`_gpu_knobs`' `CUDA_AUDIO_FAMILIES`) and skip without their carriers (the qwen2audio / voxtral / omni-3b
// f32 mmprojs, the E2B bf16 and E4B BF16 mmprojs, the canary f32 encoder, the parakeet f32 encoder, the ultravox f16
// mmproj, the Qwen3-ASR bf16 mmproj), without jfk.wav, without that home's device under DASLLAMA_GPU=1, on the Vulkan
// home under the CUDA ask (DASLLAMA_CUDA=1 or set_gpu_home("cuda"), where the Vulkan tower's seats stand down -
// `vulkan_twin_seat`), and on a build with das_metal; the qwen3a front also skips when interpreted. The Metal twins
// (`test_fc_metal_twin`: canary; the v3 bin's block: parakeet v3; `test_gemma4a_metal_twin`: E2B and E4B) skip on a build without
// das_metal, with no Metal device standing up for the tower, under a family filter leaving the family out, without
// their carriers (the canary and parakeet v3 f32 encoders, the E2B / E4B mmprojs) and without jfk.wav; the gemma4a
// one also when interpreted. The Parakeet GGUF cells skip without parakeet-tdt-0.6b-v3-Q4_K_M.gguf (the carrier cell's
// first run and the lane-tag controls without parakeet-tdt-0.6b-v3-F32.gguf or the v3 f32 bin), the encode cells without
// jfk.wav, `test_parakeet_kq_discrete_twin`'s forms as the discrete-home twins do, `test_parakeet_kq_metal_twin`'s as the Metal twins do.
// The ASR oracle cells (`_audio_asr_cells`) skip when interpreted. The canary oracle cell is large tier
// (DASLLAMA_PARITY_FULL=1), and its device walk skips where no GPU tower serves canary's default lane. The gemma4a oracle
// skips without gemma4a_test2.wav. The E4B facade cell skips when interpreted, without gemma-4-E4B-it-Q8_0.gguf (large
// tier) and mmproj-gemma-4-E4B-it-BF16.gguf, and without jfk.wav; its device legs run only where the serving tower's device
// stands up.

[test(description = "gelu_erf reproduces the exact-erf form x times Phi(x) at 1, -1, 0, 3 and 0.5 to within 1e-6 (1e-5 at 3) of the closed-form values")]
def test_gelu_erf(t : T?) {
    t |> run("gelu_erf matches the exact-erf form") @(t : T?) {
        var g = [1.0, -1.0, 0.0, 3.0, 0.5]
        gelu_erf(g, 5l)
        // 0.5·x·(1+erf(x/√2)) = x·Φ(x): Φ(1)=0.8413447, Φ(3)=0.9986501, Φ(0.5)=0.6914625
        t |> success(abs(g[0] - 0.8413447) < 1e-6, "gelu_erf(1)")
        t |> success(abs(g[1] + 0.1586553) < 1e-6, "gelu_erf(-1)")
        t |> success(g[2] == 0.0, "gelu_erf(0)")
        t |> success(abs(g[3] - 2.9959502) < 1e-5, "gelu_erf(3)")
        t |> success(abs(g[4] - 0.3457312) < 1e-6, "gelu_erf(0.5)")
    }
}

[test(description = "The 400-point Hann window is periodic: zero at 0, peak 1 at 200, 0.5 at 100, and h[399] equals h[1]")]
def test_hann_window(t : T?) {
    t |> run("periodic hann-400 endpoints and center") @(t : T?) {
        let h <- build_hann_window(400l)
        t |> success(h[0] == 0.0, "h[0] = 0")
        t |> success(abs(h[200] - 1.0) < 1e-7, "h[200] = 1 (periodic peak)")
        t |> success(abs(h[100] - 0.5) < 1e-6, "h[100] = 0.5")
        t |> success(abs(h[399] - h[1]) < 1e-6, "periodic symmetry h[399] = h[1]")
    }
}

[test(description = "The 128-mel slaney filterbank for a 400-point FFT at 16 kHz is 128 x 201 with no negative weight and nonzero support on every mel row")]
def test_mel_filterbank(t : T?) {
    t |> run("slaney filterbank shape invariants") @(t : T?) {
        let fb <- build_mel_filterbank(128l, 400l, 16000l)
        t |> success(length(fb) == 128 * 201, "128 x 201")
        var neg = 0
        var empty_rows = 0
        for (m in range(128)) {
            var row_sum = 0.0
            for (k in range(201)) {
                let v = fb[m * 201 + k]
                if (v < 0.0) {
                    neg++
                }
                row_sum += v
            }
            if (row_sum <= 0.0) {
                empty_rows++
            }
        }
        t |> success(neg == 0, "non-negative weights")
        t |> success(empty_rows == 0, "every mel row has support")
    }
}

[test(description = "Whisper log-mel puts a 440 Hz sine peak on mel row 18, clamps the spread to 2, fills padded silence uniformly at the floor, and chunks 5 s into one 3000-frame chunk and 35 s into two")]
def test_log_mel_chunks(t : T?) {
    t |> run("whisper mel: 440 Hz sine lands on its mel row, silence clamps") @(t : T?) {
        // 5 s of sine and the 30 s of zeros past it: one full 3000-frame chunk, the partial one dropped
        var s : array<float>
        s |> resize(80000)
        for (i in range(80000)) {
            s[i] = sin(2.0 * 3.14159265 * 440.0 * float(i) / 16000.0)
        }
        let chunks <- log_mel_chunks(s, 128l)
        t |> equal(length(chunks), 1, "a clip under 30 s is one chunk")
        t |> equal(chunks[0].n_mel, 128l, "n_mel")
        t |> equal(chunks[0].n_len, 3000l, "n_len")
        // 440 Hz sits in the slaney linear region: center(m=18) = 19·(m_hi/129)/lin_slope ≈ 444 Hz
        var best_m = 0
        var best_v = -1e9
        for (m in range(128)) {
            let v = chunks[0].data[m * 3000 + 100]
            if (v > best_v) {
                best_v = v
                best_m = m
            }
        }
        t |> equal(best_m, 18, "peak mel row at 440 Hz")
        // the whisper dynamic clamp bounds the spread to exactly 8 log10 units / 4
        var mn = chunks[0].data[0]
        var mx = chunks[0].data[0]
        for (v in chunks[0].data) {
            mn = min(mn, v)
            mx = max(mx, v)
        }
        t |> success(mx - mn <= 2.0 + 1e-6, "max-min <= 8/4 (global clamp)")
        // past the audio the chunk is padded silence: every value at the clamp floor
        var uniform = true
        for (m in range(128)) {
            for (i in range(600, 3000)) {
                if (chunks[0].data[m * 3000 + i] != chunks[0].data[600]) {
                    uniform = false
                }
            }
        }
        t |> success(uniform, "the frames past the audio are uniform at the floor")
        t |> success(chunks[0].data[18 * 3000 + 100] != chunks[0].data[600], "the control: a frame of the sine is off the floor")
        // 35 s of audio: the 30 s of zeros past it complete a second chunk
        s |> resize(560000)
        let two <- log_mel_chunks(s, 128l)
        t |> equal(length(two), 2, "a clip between 30 and 60 s is two chunks")
    }
}

[test(description = "swiglu_swapped_rows applies silu to the second half of each row and multiplies the first half by it, matching hand-computed values within 1e-6")]
def test_swiglu_swapped(t : T?) {
    t |> run("swiglu_swapped: silu on the SECOND half, passthrough first") @(t : T?) {
        // rows of [a b | c d] -> [silu(c)*a, silu(d)*b]; silu(1)=0.7310586, silu(-1)=-0.2689414
        let x = [2.0, 4.0, 1.0, -1.0]
        var o = [0.0, 0.0]
        swiglu_swapped_rows(o, x, 2l, 1l)
        t |> success(abs(o[0] - 2.0 * 0.7310586) < 1e-6, "silu(second)*first [0]")
        t |> success(abs(o[1] + 4.0 * 0.2689414) < 1e-6, "silu(second)*first [1]")
    }
}

// The chat towers' mmprojs - the whisper-class tower with its projector: qwen2audio, voxtral, omni, ultravox - load once a
// form, each in its file's one block (`aud_form`). proj_dim comes from the mm tensors, NOT clip.audio.projection_dim -
// the ultravox-1b GGUF metadata says 4096 while mm2 actually projects into the 2048-wide decoder. Token counts follow
// the projector chain's floor math.

//! a chat tower minted from `stg`, a staged read on the lane `q8`: `zero_block` >= 0 zeroes `poison_blocks` blocks from it
//! on, on the lane's planes; `dev` the image flavor (`none` the CPU image, `#tower-dev-block`)
def private mint_aud(var stg : AudioTowerStaging; q8 : bool; zero_block : int64; dev : TowerDev) : AudioTower {
    if (zero_block >= 0l) {
        for (b in range64(zero_block, min(stg.e.n_layer, zero_block + poison_blocks(stg.e.n_layer)))) {
            if (q8) {
                zero_plane(stg.qblob, stg.e.qlayers_off + b * stg.e.qlayer_stride, stg.e.qlayer_stride)
            } elif (stg.e.gemm_f16) {
                zero_plane(stg.wblob, stg.e.qlayers_off + b * stg.e.qlayer_stride, stg.e.qlayer_stride)
            } else {
                zero_plane(stg.fblob, stg.e.layers_off + b * stg.e.layer_stride, stg.e.layer_stride)
            }
        }
    }
    var tw = AudioTower()
    mint_audio_tower(stg, tw, q8, dev)
    return <- tw
}

def private aud_twin_encode(tw : AudioTower; var enc : EncoderState; mel : MelChunk; var out : array<float>) : int64 {
    audio_encode(tw, enc, mel)
    out := enc.out
    return long_length(out)
}

//! a chat tower mmproj's form read off its name (`tower_form_of`): the lanes `f32` (the exact lane) and `q8`; `emb` names
//! no lane - `image_emb` is the audio carrier's load of the mmproj
def private aud_form_of(form : string) : TowerForm => tower_form_of(form, ["f32", "q8", "emb"], "chat tower")

//! the staged read a chat tower mmproj's form is minted from (`tower_stage_key_of`)
def private aud_stage_key(form : string) : string => tower_stage_key_of(aud_form_of(form))

//! a chat tower mmproj's staged read the key `key` names: its lane, in its image flavor
def private aud_stage(path, key : string) : AudioTowerStaging {
    let f = aud_form_of(key)
    return <- tower_dev_staged(tower_form_dev_of(f, TowerFamily.whisper)) $ { return <- stage_audio_tower(path, f.lane == "q8") }
}

//! A chat tower mmproj's form as its block holds it: the tower `tw`, or - on the `image_emb` form - the audio carrier `e`.
struct private AudForm {
    tw : AudioTower = AudioTower()
    e : AudioEmbedder = AudioEmbedder()
}

def private finalize(var h : AudForm) {
    delete h.tw
    delete h.e
}

//! One form of a chat tower mmproj, its block's form loader. `image_f32` is the tower's own load (`load_audio_tower`) on its
//! exact lane's CPU image - every GPU tower knob off for the load, put back however it leaves - and `image_emb` the audio
//! carrier's load (`load_audio_embedder`). Any other form is minted in memory (`mint_aud`) from `stg`, its own copy of the
//! mmproj's staging on the lane it names: `_z<block>` zeroes the poison's blocks from that block on, `@dev` is the image
//! flavor of the whisper-class tower's serving home.
def private aud_form(path, form : string; var stg : AudioTowerStaging) : AudForm {
    let f = aud_form_of(form)
    var inscope h = AudForm()
    if (!f.image) {
        if (f.lane != "f32" && f.lane != "q8") {
            panic("the chat tower tests: '{form}' names no lane to mint")
        }
        h.tw <- mint_aud(stg, f.lane == "q8", f.zero_block, tower_form_dev_of(f, TowerFamily.whisper))
        return <- h
    }
    if (f.lane == "emb") {
        h.e <- load_audio_embedder(path)
    } elif (f.lane == "f32") {
        let knobs = gpu_knobs()
        set_every_gpu_tower(false)
        with_put_back($ {
            set_gpu_knobs(knobs)
        }) {
            h.tw <- load_audio_tower(path, false)
        }
    } else {
        panic("the chat tower tests: '{form}' names no load of the image rail")
    }
    return <- h
}

//! The form is the load, lane, poison and image flavor it names: on `h`, a chat tower mmproj loaded as the form `form` of
//! `file` - the carrier's whisper-class arm on `image_emb`, else the tower with its lane flag, and on a minted form the
//! poison's block the tower's middle one, and the image flavor.
def private aud_form_is(t : T?; file, form : string; h : AudForm) {
    let f = aud_form_of(form)
    if (f.lane == "emb") {
        t |> success(h.e.kind == AudioKind.audio_tower, "{file}: the {form} form is the carrier's whisper-class arm")
        return
    }
    t |> success(h.tw.n_layer > 0l, "{file}: the {form} form holds the tower")
    minted_form_is(t, file, form, f, TowerFamily.whisper, h.tw.q8, h.tw.n_layer, h.tw.dev_backend)
}

//! the minted forms a chat tower's twin reads: the exact form and the q8 form, the q8 lane's image in the home's flavor and
//! its poison, the exact lane's image in the home's flavor and its poison
let private AUD_TWIN_FORMS = fixed_array("f32", "q8", "q8@dev", "q8_z16@dev", "f32@dev", "f32_z16@dev")

//! What a chat tower mmproj's block keeps for the twin compare that follows it, on a synthetic mel. `family` and `tag` name
//! the twin, `kind` and `layers` the projector and the depth its cells name (0: none named), `metal_cell` and
//! `discrete_cell` the compare on each home, `form` the form whose cells run; the three clean forms' lane flags, the device
//! image's flavor, the q8 form's projector, depth and positions; the q8 lane's rows three ways (`LaneTwinKept`) and the
//! projector tails its legs served; the exact lane's rows (`ExactFlavorKept`) and the tails its device legs served.
struct private AudTwinKept {
    family : string
    tag : string
    kind : AudioProjKind
    layers : int64
    metal_cell : string
    discrete_cell : string
    form : string
    flags : LaneFlags
    c_kind : AudioProjKind
    n_layer : int64
    n_ctx : int64
    tail : int64
    x_tail : int64
    twin : LaneTwinKept = LaneTwinKept()
    exact : ExactFlavorKept
}

//! The homes a chat tower's twin runs on this box - `metal` and `discrete` the homes its cells name - with the twin's tag
//! set by the home that runs it, its forms added to `forms` through the asked homes' reasons (`add_served_forms`), and
//! each asked home the box lacks a skipped cell named for its compare.
[cold_path]
def private aud_twin_asked(t : T?; var forms : array<string>; var k : AudTwinKept; file : string; metal, discrete : bool) : tuple<metal_on : bool; discrete_on : bool> {
    let metal_why = metal_twin_jit_why(TowerFamily.whisper)
    let discrete_why = audio_discrete_form_why(TowerFamily.whisper)
    let metal_on = metal && empty(metal_why)
    let discrete_on = discrete && !metal_on && empty(discrete_why)
    k.tag = metal_on ? "{file} metal synthetic mel" : "{k.family} synthetic mel"
    if (metal && !metal_on) {
        skip_cell(t, "{k.metal_cell} - {metal_why}")
    }
    if (discrete && !discrete_on) {
        skip_cell(t, "{k.discrete_cell} - {metal_on ? "das_metal on this build owns the tower hooks" : discrete_why}")
    }
    var inscope asked <- [for (f in AUD_TWIN_FORMS); f]
    add_served_forms(t, forms, file, asked, metal && discrete ? either_why(metal_why, discrete_why) : (metal ? metal_why : discrete_why))
    return (metal_on = metal_on, discrete_on = discrete_on)
}

// A form's legs of a chat tower's twin on the synthetic mel, each on the form's one home: the exact form the reference
// chain and the exact lane's CPU leg, the q8 form the CPU q8 chain, its `@dev` form the device chain, the zeroed `@dev`
// forms the input poisons, the exact lane's `@dev` form its device leg; the projector tails each clean leg served.
[cold_path]
def private aud_twin_leg(t : T?; tw : AudioTower; var k : AudTwinKept) {
    let f = aud_form_of(k.form)
    let mel <- synth_mel(tw.n_mel)
    var enc <- make_encoder_state(tw)
    k.twin.rows.spec = TwinSpec(family = k.family, tag = k.tag, n_layer = tw.n_layer, metric = TwinMetric.rel_l2, seat_family = TowerFamily.whisper)
    exact_flavor_named(k.exact, k.tag, TowerFamily.whisper, TWIN_EXACT_REL_L2_VK, tw.n_ctx)
    let tail0 = gpu_tower_tail_encodes(TowerFamily.whisper)
    let leg = lane_exact_leg(t, k.twin, k.exact, k.flags, f, true, tw, enc) $(e; var s; var out) => aud_twin_encode(e, s, mel, out)
    if (leg == LaneFormLeg.cpu) {
        k.c_kind = tw.proj_kind
        k.n_layer = tw.n_layer
        k.n_ctx = tw.n_ctx
    }
    let tails = gpu_tower_tail_encodes(TowerFamily.whisper) - tail0
    if (f.lane == "f32" && (f.dev || f.zero_block >= 0l)) {
        k.x_tail += tails
    } elif (f.zero_block < 0l) {
        k.tail += tails
    }
    delete enc
}

//! the exact lane held, after the block (`exact_flavor_held`): the conv stem and the projector tail served on both device encodes
def private aud_exact_held(t : T?; k : AudTwinKept) {
    let tag = k.tag
    let convs = exact_flavor_held(t, k.exact)
    t |> equal(convs, 2l, "{tag} exact lane: the conv stem served on the device, the clean encode and the poisoned one")
    t |> equal(k.x_tail, 2l, "{tag} exact lane: the projector tail rode the device behind both encodes' blocks")
}

//! a chat tower's device leg held inside `twin_rows_held`: exactly the device leg dispatched, every block, every row
def private aud_block_counters(t : T?; k : AudTwinKept; legs : TwinLegs) {
    let tag = k.tag
    t |> equal(legs.dev.encodes - legs.pre.encodes, 1l, "{tag}: exactly the device leg dispatched")
    t |> equal(legs.dev.blocks - legs.pre.blocks, k.n_layer, "{tag}: blocks delta = n_layer")
    t |> equal(legs.dev.rows - legs.pre.rows, k.n_ctx, "{tag}: rows delta = n_ctx")
}

// A chat tower on the discrete home, after its mmproj's block: every block and the projector tail on the device within the
// three-way bar on the synthetic mel, a poison past it, and the exact lane within its bar.
[cold_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private test_encoder_blocks_vulkan(t : T?; k : AudTwinKept) {
    let tag = k.tag
    let layers_claimed = k.layers
    t |> success(!k.flags.x_q8 && k.flags.c_q8, "{tag}: the exact and the q8 towers minted")
    if (layers_claimed > 0l) {
        t |> equal(k.n_layer, layers_claimed, "{tag}: the tower's depth is the {layers_claimed} blocks the cell names")
    }
    twin_rows_held(t, k.twin.rows) $(legs) {
        aud_block_counters(t, k, legs)
    }
    t |> equal(k.tail, 1l, "{tag}: the projector tail rode the device behind the device leg's blocks")
    aud_exact_held(t, k)
}

// A chat tower on the Metal home, after its mmproj's block: the q8 lane three ways - the exact CPU image the reference, the
// CPU q8 image, the q8 lane's Metal image with the projector tail behind its blocks - with its poison, then the exact lane
// on its f16 Metal image.
[cold_path]
def private test_encoder_blocks_gpu(t : T?; k : AudTwinKept) {
    let tag = k.tag
    let layers = k.layers
    t |> success(k.c_kind == k.kind, "{tag}: the projector kind the cell names")
    if (layers > 0l) {
        t |> equal(k.n_layer, layers, "{tag}: the tower's depth is the {layers} blocks the cell names")
    }
    t |> success(!k.flags.x_q8 && k.flags.c_q8 && k.flags.d_q8 && k.flags.d_backend == TowerDev.metal, "{tag}: the exact and the q8 towers minted, the q8 lane's Metal image beside")
    twin_rows_held(t, k.twin.rows) $(legs) {
        aud_block_counters(t, k, legs)
    }
    t |> equal(k.tail, 1l, "{tag}: the q8 projector tail rode the device behind the device leg's blocks")
    t |> equal(k.twin.f16_encodes, 0l, "{tag}: the q8 lane read the q8 slab, not the halfword plane")
    aud_exact_held(t, k)
}

//! the twin's cells of a chat tower mmproj's minted form `form`: its legs on the home the box serves
[cold_path]
def private aud_twin_cells(t : T?; file, form : string; var tw : AudioTower; var kept : AudTwinKept; homes : tuple<metal_on : bool; discrete_on : bool>) {
    return if (!homes.metal_on && !homes.discrete_on)
    tower_cell(t, "{file}: the {homes.metal_on ? "Metal" : "discrete"} twin, the {form} form's legs", tw, kept, @@aud_twin_leg)
}

//! the twin's compare of a chat tower after its mmproj's block, on the home that ran it, and the home's residents released
[cold_path]
def private aud_twin_held(t : T?; var kept : AudTwinKept; homes : tuple<metal_on : bool; discrete_on : bool>) {
    if (homes.metal_on) {
        kept_cell(t, kept.metal_cell, kept, @@test_encoder_blocks_gpu)
    }
    if (homes.discrete_on) {
        kept_cell(t, kept.discrete_cell, kept, @@test_encoder_blocks_vulkan)
    }
    if (homes.metal_on || homes.discrete_on) {
        serving_tower_shutdown(TowerFamily.whisper)
    }
}

// The ultravox-1b f32 mmproj's tower on its exact lane's CPU image: the stack-8 projector's shapes, and the CPU encode of a
// constant mel against llama-mtmd-debug.
[cold_path]
def private ultravox_1b_tower(t : T?; tw : AudioTower) {
    t |> success(tw.proj_kind == AudioProjKind.ultravox, "kind")
    t |> equal(tw.stack_factor, 8l, "stack factor")
    t |> equal(tw.mm1_out, 4096l, "mm1 out (pre-swiglu)")
    t |> equal(tw.proj_dim, 2048l, "proj_dim from mm2 (metadata lies: 4096)")
    t |> success(!audio_has_avgpool(tw), "no avg-pool")
    t |> equal(audio_tokens_per_chunk(tw), 187l, "tokens per 30 s chunk (floor 1500/8)")
    if (jit_enabled()) {
        var mel = MelChunk(n_mel = tw.n_mel, n_len = AUDIO_CHUNK_FRAMES)
        mel.data |> resize(tw.n_mel * AUDIO_CHUNK_FRAMES)
        for (i in range(length(mel.data))) {
            mel.data[i] = 1.0
        }
        var enc <- make_encoder_state(tw)
        with_gpu_knobs() {
            set_every_gpu_tower(false)
            with_dasllama_jobque_() {
                audio_encode(tw, enc, mel)
            }
        }
        // llama-mtmd-debug -p encode --audio one -n 3000 (CPU, f32 mmproj): token 0 opens
        // -0.831656 1.679679 0.112337 -0.558047; stats mean=0.002889 max=4.306354.
        t |> success(abs(enc.out[0] + 0.831656) < 5e-3, "tok0[0]")
        t |> success(abs(enc.out[1] - 1.679679) < 5e-3, "tok0[1]")
        t |> success(abs(enc.out[2] - 0.112337) < 5e-3, "tok0[2]")
        t |> success(abs(enc.out[3] + 0.558047) < 5e-3, "tok0[3]")
        var sum = 0.0lf
        var mx = enc.out[0]
        for (v in enc.out) {
            sum += double(v)
            mx = max(mx, v)
        }
        t |> success(abs(float(sum / double(187l * tw.proj_dim)) - 0.002889) < 5e-4, "mean")
        t |> success(abs(mx - 4.306354) < 5e-2, "max")
        delete enc
    }
}

[test(description = "mmproj-ultravox-1b-f32.gguf loads once: its exact lane's CPU image is a stack-8 projector with proj_dim 2048 read from mm2 and 187 tokens a chunk, and its CPU encode of a constant mel matches llama-mtmd-debug token 0, mean and max")]
def test_ultravox_tower(t : T?) {
    with_model_once(t, "audio", "mmproj-ultravox-1b-f32.gguf", ["image_f32"], @@aud_stage_key, @@aud_stage, @@aud_form) $(var h; form) {
        aud_form_is(t, "mmproj-ultravox-1b-f32.gguf", form, h)
        tower_cell(t, "ultravox mmproj: stack-8 projector shapes + 187 tokens/chunk", h.tw, @@ultravox_1b_tower)
    }
}

// The voxtral f32 mmproj's tower on its exact lane's CPU image: the avg-pooled stack-4 projector's shapes, and the CPU
// encode of a constant mel against llama-mtmd-debug.
[cold_path]
def private test_voxtral_tower(t : T?; tw : AudioTower) {
    t |> success(tw.proj_kind == AudioProjKind.voxtral, "kind")
    t |> equal(tw.stack_factor, 4l, "stack factor")
    t |> equal(tw.mm1_out, 3072l, "mm1 out")
    t |> equal(tw.proj_dim, 3072l, "proj_dim")
    t |> success(audio_has_avgpool(tw), "avg-pool")
    t |> equal(audio_tokens_per_chunk(tw), 187l, "tokens per 30 s chunk (floor 750/4)")
    if (jit_enabled()) {
        var mel = MelChunk(n_mel = tw.n_mel, n_len = AUDIO_CHUNK_FRAMES)
        mel.data |> resize(tw.n_mel * AUDIO_CHUNK_FRAMES)
        for (i in range(length(mel.data))) {
            mel.data[i] = 1.0
        }
        var enc <- make_encoder_state(tw)
        with_gpu_knobs() {
            set_every_gpu_tower(false)
            with_dasllama_jobque_() {
                audio_encode(tw, enc, mel)
            }
        }
        // llama-mtmd-debug -p encode --audio one -n 3000 (CPU, f32 mmproj): token 0 opens
        // 0.967699 -0.765652 0.050477 -0.571310; stats mean=-0.001517 max=1.459851.
        t |> success(abs(enc.out[0] - 0.967699) < 5e-3, "tok0[0]")
        t |> success(abs(enc.out[1] + 0.765652) < 5e-3, "tok0[1]")
        t |> success(abs(enc.out[2] - 0.050477) < 5e-3, "tok0[2]")
        t |> success(abs(enc.out[3] + 0.571310) < 5e-3, "tok0[3]")
        var sum = 0.0lf
        var mx = enc.out[0]
        for (v in enc.out) {
            sum += double(v)
            mx = max(mx, v)
        }
        t |> success(abs(float(sum / double(187l * tw.proj_dim)) + 0.001517) < 5e-4, "mean")
        t |> success(abs(mx - 1.459851) < 5e-2, "max")
        delete enc
    }
}

// The voxtral mini mmproj's tower, one block.
[test(description = "voxtral-mini-mmproj-f32.gguf loads its tower once a form. Its exact lane's CPU image is an avg-pooled stack-4 projector with proj_dim 3072 and 187 tokens a chunk, and its CPU encode of a constant mel matches llama-mtmd-debug token 0, mean and max. On Metal the tower runs every block and the projector tail on the q8 slab within the three-way bar on a synthetic mel, a poison exceeds it, and the f16 exact lane holds; on the discrete home every block and the projector tail run on the device within the three-way bar, a poison exceeds it, and the exact lane holds")]
def test_voxtral_carrier(t : T?) {
    var inscope kept = AudTwinKept(family = "voxtral", kind = AudioProjKind.voxtral,
        metal_cell = "voxtral tower: the Metal chain vs the CPU encoder - pair pool, post-norm, stacked GELU MLP",
        discrete_cell = "voxtral tower: the discrete home's blocksvs the CPU q8 chain on the synthetic mel")
    var inscope forms <- ["image_f32"]
    let homes = aud_twin_asked(t, forms, kept, "voxtral-mini-mmproj-f32.gguf", true, true)
    let ran = with_model_once(t, "audio", "voxtral-mini-mmproj-f32.gguf", forms, @@aud_stage_key, @@aud_stage, @@aud_form) $(var h; form) {
        aud_form_is(t, "voxtral-mini-mmproj-f32.gguf", form, h)
        kept.form = form
        if (form == "image_f32") {
            tower_cell(t, "voxtral mmproj: avg-pool + stack-4 projector shapes + 187 tokens/chunk", h.tw, @@test_voxtral_tower)
            return
        }
        aud_twin_cells(t, "voxtral-mini-mmproj-f32.gguf", form, h.tw, kept, homes)
    }
    if (ran) {
        aud_twin_held(t, kept, homes)
    }
}

//! `n` GEMM weights at `off` zeroed on the staging's lane: the q8 plane, the f16 lane's halfwords, else the f32 blob
def private zero_g4a_gemm(var stg : Gemma4aStaging; off, n : int64) {
    if (stg.e.q8) {
        zero_plane(stg.qblob, off, n)
    } elif (stg.e.gemm_f16) {
        zero_plane(stg.wblob, off, n)
    } else {
        zero_plane(stg.blob, off, n)
    }
}

//! a gemma4a mmproj's staged read the key `key` names (`tower_stage_key_of`): under its lane pin, put back however the
//! staging leaves, in its image flavor
def private g4a_stage(mm, key : string) : Gemma4aStaging {
    let f = g4a_form_of(key)
    return <- staged_twin_tower(mm, f.lane == "q8", tower_form_dev_of(f, TowerFamily.gemma4a), @@set_gemma4a_q8, @@reset_gemma4a_q8,
        @@ < (path : string) : Gemma4aStaging > stage_gemma4a_encoder)
}

//! a gemma4a encoder minted from `stg`: `zero_block` >= 0 zeroes the eleven GEMM planes of `poison_blocks` blocks from it
//! on, on the lane's planes; `dev` the image flavor
def private mint_g4a(var stg : Gemma4aStaging; zero_block : int64; dev : TowerDev) : Gemma4aEncoder {
    if (zero_block >= 0l) {
        let d = stg.e.d_model
        let ff = stg.e.n_ff
        for (b in range64(zero_block, min(stg.e.n_layer, zero_block + poison_blocks(stg.e.n_layer)))) {
            let lo = stg.layers[b]
            for (w in fixed_array(lo.q, lo.k, lo.v, lo.o, lo.attn_k_rel, lo.conv_pw2)) {
                zero_g4a_gemm(stg, w, d * d)
            }
            for (w in fixed_array(lo.ff_up, lo.ff_down, lo.ff_up_1, lo.ff_down_1)) {
                zero_g4a_gemm(stg, w, d * ff)
            }
            zero_g4a_gemm(stg, lo.conv_pw1, d * 2l * d)
        }
    }
    var enc = Gemma4aEncoder()
    mint_gemma4a_encoder(stg, enc, "", dev)
    return <- enc
}


//! the synthetic speech-scale mel every twin reads: the all-ones fixture is degenerate (identical rows make every attention weight
//! a near-tie, so single elements flip O(1)); `other` is the second clip, the control's rows outside the bar
def private synth_mel_fill(var mel : MelChunk; other : bool) {
    mel.data |> resize(mel.n_mel * AUDIO_CHUNK_FRAMES)
    for (i in range(length(mel.data))) {
        mel.data[i] = other ? sin(float(i) * 0.0091) * 0.8 + sin(float(i) * 0.00417) * 0.5 : sin(float(i) * 0.0137) * 0.8 + sin(float(i) * 0.00291) * 0.5
    }
}

def private synth_mel(n_mel : int64) : MelChunk {
    var mel = MelChunk(n_mel = n_mel, n_len = AUDIO_CHUNK_FRAMES)
    synth_mel_fill(mel, false)
    return <- mel
}

//! a Canary encoder minted from `stg`, a staged read on the lane `q8`: `zero_block` >= 0 zeroes the eleven GEMM planes of
//! `poison_blocks` blocks from it on, in the lane's GEMM plane; `dev` the image flavor
def private mint_cn(var stg : CanaryStaging; q8 : bool; zero_block : int64; dev : TowerDev) : CanaryEncoder {
    if (q8) {
        poison_fc_blocks(stg.qblob, stg.e.n_layer, zero_block, stg.e.d_model, stg.e.n_ff) $(b) => stg.layers[b]
    } elif (stg.e.gemm_f16) {
        poison_fc_blocks(stg.wblob, stg.e.n_layer, zero_block, stg.e.d_model, stg.e.n_ff) $(b) => stg.layers[b]
    } else {
        poison_fc_blocks(stg.blob, stg.e.n_layer, zero_block, stg.e.d_model, stg.e.n_ff) $(b) => stg.layers[b]
    }
    var enc = CanaryEncoder()
    mint_canary_encoder(stg, enc, dev)
    return <- enc
}

[cold_path]
def private cn_twin_encode(enc : CanaryEncoder; var st : CanaryState; clip : array<float>; var out : array<float>) : int64 {
    canary_log_mel(enc, clip, st)
    let n = canary_encode(enc, st)
    delete out
    out <- copy_span(st.out, 0l, n * enc.proj_dim)   // the encode's rows: the state's plane keeps a longer clip's past them
    return n
}


//! parakeet minted in memory from `stg`, a staged read on the lane asked - `kq` a quantized GGUF's own planes, else `q8`
//! or the exact lane; `zero_block` >= 0 zeroes the GEMM planes of the blocks from it on
def private mint_pk(var stg : ParakeetStaging; q8 : bool; zero_block : int64; dev : TowerDev; kq : bool) : ParakeetModel {
    if (kq) {
        if (zero_block >= 0l) {
            poison_kq_seats(stg.gemm_seats, zero_block * PK_LAYER_SEATS, stg.e.n_layer * PK_LAYER_SEATS, stg.kq, stg.qblob)
        }
    } elif (q8) {
        poison_fc_blocks(stg.qblob, stg.e.n_layer, zero_block, stg.e.n_state, 4l * stg.e.n_state) $(b) => pk_fc_layer(pk_layer_offsets(stg.e, b))
    } elif (stg.e.gemm_f16) {
        poison_fc_blocks(stg.wblob, stg.e.n_layer, zero_block, stg.e.n_state, 4l * stg.e.n_state) $(b) => pk_fc_layer(pk_layer_offsets(stg.e, b))
    } else {
        poison_fc_blocks(stg.blob, stg.e.n_layer, zero_block, stg.e.n_state, 4l * stg.e.n_state) $(b) => pk_fc_layer(pk_layer_offsets(stg.e, b))
    }
    var m = ParakeetModel()
    mint_parakeet_model(stg, m, q8, dev)
    return <- m
}

[cold_path]
def private pk_twin_encode(m : ParakeetModel; var st : ParakeetState; clip : array<float>; var out : array<float>) : int64 {
    return x_rows_encode(st, m.n_state, out) $(var s) {
        parakeet_log_mel(clip, m, s)
        return parakeet_encode(m, s)
    }
}

let private PK_V2 = "ggml-parakeet-tdt-0.6b-v2-f32.bin"
let private PK_V3 = "ggml-parakeet-tdt-0.6b-v3-f32.bin"
let private PK_V3_Q8_0 = "ggml-parakeet-tdt-0.6b-v3-q8_0.bin"
let private PK_Q4 = "parakeet-tdt-0.6b-v3-Q4_K_M.gguf"
let private PK_F32 = "parakeet-tdt-0.6b-v3-F32.gguf"
let private PK_Q8_0 = "parakeet-tdt-0.6b-v3-Q8_0.gguf"

//! a Parakeet carrier's form read off its name (`tower_form_of`): the lanes `f32` (the exact lane), `q8`, and `kq` (a
//! quantized GGUF's own planes)
def private parakeet_form_of(form : string) : TowerForm => tower_form_of(form, ["f32", "q8", "kq"], "parakeet")

//! the staged read a Parakeet carrier's form is minted from (`tower_stage_key_of`)
def private parakeet_stage_key(form : string) : string => tower_stage_key_of(parakeet_form_of(form))

//! a Parakeet carrier's staged read the key `key` names: its lane - a kq lane the GGUF's own planes - in its image flavor
def private parakeet_stage(path, key : string) : ParakeetStaging {
    let f = parakeet_form_of(key)
    return <- tower_dev_staged(tower_form_dev_of(f, TowerFamily.parakeet)) $ {
        return <- stage_parakeet_kq_model(path) if (f.lane == "kq")
        return <- stage_parakeet_model(path, f.lane == "q8")
    }
}

//! One form of a Parakeet carrier as the ASR facade holds it, its block's form loader (`parakeet_form_of`): `image` the
//! facade's own load with the family's lane pin unset; any other minted in memory (`mint_pk`) from `stg`, its own copy of
//! the carrier's staging on the lane it names - `_z<block>` zeroes the GEMM planes of the poison's blocks from that block
//! on (a kq lane's seats), `@dev` is the image flavor of the family's serving home.
def private parakeet_form(path, form : string; var stg : ParakeetStaging) : AsrModel {
    let f = parakeet_form_of(form)
    if (f.image) {
        reset_parakeet_q8()
        set_asr_fp32(false)
        set_asr_tower_fp32(false)
        return <- load_asr_model(path)
    }
    var inscope m = AsrModel(kind = AsrKind.parakeet)
    m.pk <- mint_pk(stg, f.lane == "q8", f.zero_block, tower_form_dev_of(f, TowerFamily.parakeet), f.lane == "kq")
    return <- m
}

//! The form is the lane, poison and image flavor it names: on `m`, a carrier loaded as the form `form` of `file`, the lane
//! flags, the poison's block the tower's middle one, and the image flavor. The `image` form's lane and flavor are the
//! run's policy: not read.
def private parakeet_form_is(t : T?; file, form : string; m : AsrModel) {
    let f = parakeet_form_of(form)
    t |> success(m.kind == AsrKind.parakeet, "{file}: the {form} form is a parakeet model")
    return if (f.image)
    t |> success(m.pk.kq_lane == (f.lane == "kq") && m.pk.q8 == (f.lane == "q8"), "{file}: the {form} form serves the lane it names")
    form_zero_and_flavor_is(t, file, form, f, TowerFamily.parakeet, m.pk.n_layer, m.pk.dev_backend)
}

let private SHORT_CLIP_SAMPLES = fixed_array(320l, 480l, 640l, 800l, 1600l)   //! 20 to 100 ms: a few mel frames each, from under four frames to past it
let private SHORT_CLIP_BAR = 5e-2lf   //! the device rows' rel_l2 from the CPU q8 chain's over a clip of a few frames - two q8 chains, the device's GEMMs on the f16 feed

//! the next sample of a seeded white noise in [-0.3, 0.3)
def private short_clip_noise(var seed : uint&) : float {
    seed = seed * 1664525u + 1013904223u
    return (float(seed >> 8u) / 16777216.0 - 0.5) * 0.6
}


//! The short clips of one family as two forms keep them, for the compare after the block (`test_vulkan_short_clips`): each
//! clip's rows, row count and the driver's counters across its encode on the CPU image and on the device image, and the
//! device image's rows of a noise clip (the control).
struct private ShortClipsKept {
    tag : string
    cpu : array<array<float>>
    dev : array<array<float>>
    n_cpu : array<int64>
    n_dev : array<int64>
    cpu_encodes : array<int64>
    dev_encodes : array<int64>
    dev_declines : array<int64>
    other : array<float>
    legs : int
}

//! A form's leg of the short clips `k` on family `fam`'s home: every clip through the form's tower `tw` with the tower
//! knob on - the CPU image (`dev` false), which never dispatches, or the GPU-flavored image, which then encodes the noise
//! clip too; `encode` the family's encode of a clip.
[cold_path]
def private short_clip_leg(var k : ShortClipsKept; fam : TowerFamily; dev : bool; tw : auto(TW); var st : auto(ST);
                           encode : block<(e : TW; var s : ST; clip : array<float>; var out : array<float>) : int64>) {
    for (n in SHORT_CLIP_SAMPLES) {
        var inscope clip <- [for (i in range64(n)); 0.3 * sin(float(i) * 0.05) + 0.1 * sin(float(i) * 0.31)]
        var rows : array<float>
        var n_rows = 0l
        var s0 = GpuTowerCounters()
        var s1 = GpuTowerCounters()
        with_gpu_knobs() {
            with_dasllama_jobque_() {
                set_serving_gpu_tower(true, fam)
                s0 = gpu_tower_counters(fam)
                n_rows = invoke(encode, tw, st, clip, rows)
                s1 = gpu_tower_counters(fam)
            }
        }
        if (dev) {
            k.dev |> emplace(rows)
            k.n_dev |> push(n_rows)
            k.dev_encodes |> push(s1.encodes - s0.encodes)
            k.dev_declines |> push(s1.declines - s0.declines)
        } else {
            k.cpu |> emplace(rows)
            k.n_cpu |> push(n_rows)
            k.cpu_encodes |> push(s1.encodes - s0.encodes)
        }
    }
    if (dev) {
        var seed = 12345u
        var inscope noise <- [for (_i in range64(SHORT_CLIP_SAMPLES[length(SHORT_CLIP_SAMPLES) - 1])); short_clip_noise(seed)]
        with_gpu_knobs() {
            with_dasllama_jobque_() {
                set_serving_gpu_tower(true, fam)
                invoke(encode, tw, st, noise, k.other)
            }
        }
    }
    k.legs++
}

// Clips a few mel frames long through a family's GPU-flavored tower against its CPU image at `SHORT_CLIP_BAR`, after the
// file's block, on what the two forms kept; the device's rows of a noise clip the control.
[cold_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private test_vulkan_short_clips(t : T?; k : ShortClipsKept) {
    let tag = k.tag
    t |> equal(k.legs, 2, "{tag}: the short clips kept their CPU image's leg and their device image's")
    return if (k.legs != 2)
    for (n, i in SHORT_CLIP_SAMPLES, count()) {
        let ctag = "{tag} {n}-sample clip"
        t |> equal(k.cpu_encodes[i], 0l, "{ctag}: the CPU image's leg never dispatched")
        t |> equal(k.dev_encodes[i], 1l, "{ctag}: the device served the encode")
        t |> equal(k.dev_declines[i], 0l, "{ctag}: the device leg declined nothing")
        t |> equal(k.n_dev[i], k.n_cpu[i], "{ctag}: the device's row count is the CPU's")
        var got = TowerParityStats()
        if (k.n_cpu[i] > 0l && tower_parity_stats(t, ctag, k.cpu[i], k.dev[i], got)) {
            to_log(LOG_INFO, "test_audio: {ctag}: device vs CPU q8 rel_l2 {got.rel_l2} over {k.n_cpu[i]} rows\n")
            t |> success(got.rel_l2 <= SHORT_CLIP_BAR, "{ctag}: the device rows within {SHORT_CLIP_BAR} of the CPU's ({got.rel_l2})")
        }
    }
    var ctl = TowerParityStats()
    if (tower_parity_stats(t, "{tag} control", k.cpu[length(k.cpu) - 1], k.other, ctl)) {
        t |> success(is_finite(ctl.rel_l2) && ctl.rel_l2 > SHORT_CLIP_BAR, "{tag}: the device's rows of a noise clip land outside the bar of the longest clip's CPU rows ({ctl.rel_l2}) - the control")
    }
}


//! the f32 ggml bin's side of the F32 GGUF's compares: its shape, its encoder rows of jfk and of jfk at half volume, and
//! its transcript
struct private PkCarrierKept {
    n_layer : int64
    n_vocab : int64
    blob_n : int64
    rows : array<float>
    loud : array<float>
    text : string
    kept : bool
}

//! the Q4_K_M carrier's CPU lanes as its forms keep them: the lane flags and jfk's encoder rows on the dequantized exact
//! lane, the q8 lane, the kq lane, and the kq lane with its seats zeroed from the middle block on
struct private PkKqLanes {
    x_kq : bool
    q_q8 : bool
    k_kq : bool
    k_q8 : bool
    n_layer : int64
    ex : array<float>
    q8 : array<float>
    kq : array<float>
    bad : array<float>
    legs : int
}

//! What a Parakeet `[test]`'s blocks keep for the compares that follow them. `tag` names the carrier in the cells' tags
//! and `form` the form whose cells run. The device twin: `twin_tag` its asserts' tag, `exact_lane` whether the carrier
//! takes the exact lane on the device, `range_refusal` whether the device leg runs the storage-range refusal, the three
//! forms' lane flags, the device image's flavor, lane name and host planes, the kq tiles and mul_mm stamps its leg moved,
//! and the rows (`LaneTwinKept`, `ExactFlavorKept`).
struct private PkKept {
    tag : string
    form : string
    twin_tag : string
    exact_lane : bool
    range_refusal : bool
    flags : LaneFlags
    c_kq : bool
    d_kq : bool
    d_fmt : string
    d_host_planes : int64
    kq_tiles : int64
    kq_mm : int64
    twin : LaneTwinKept = LaneTwinKept()
    exact : ExactFlavorKept
    clips : ShortClipsKept
    decode : array<RnntDecodeTwin>
    carrier : PkCarrierKept
    lanes : PkKqLanes
    oracle : PkOracleKept
    text : PkBinText
}

// A form's legs of a Parakeet carrier's device twin on jfk, each on the form's one home: the lane's rows three ways with
// the input poison (`LaneTwinKept`) - the exact form the reference, the q8 or kq form the CPU chain, its `@dev` form the
// device chain - and, on an f32 bin, the exact lane on the device (`ExactFlavorKept`).
[cold_path]
def private pk_twin_leg(t : T?; m : ParakeetModel; var k : PkKept) {
    let samples <- asr_samples(t, "jfk.wav")
    return if (empty(samples))
    let f = parakeet_form_of(k.form)
    let fam = TowerFamily.parakeet
    lane_twin_named(k.twin, k.twin_tag, fam, m.n_layer)
    exact_flavor_named(k.exact, k.twin_tag, fam, TWIN_EXACT_REL_L2_VK, -1l)
    var st = ParakeetState()
    let fmt = parakeet_exec_fmt(m)
    let host_planes = m.kq.n + m.ks.n + m.qblob.n
    let k4_0 = kq_tile_count("k4", fam)
    let mm0 = kq_mm_count()
    let leg = lane_exact_leg(t, k.twin, k.exact, k.flags, f, k.exact_lane, m, st) $(e; var s; var out) => pk_twin_encode(e, s, samples, out)
    if (leg == LaneFormLeg.dev) {
        k.d_kq = m.kq_lane
        k.d_fmt = fmt
        k.d_host_planes = host_planes
        k.kq_tiles = kq_tile_count("k4", fam) - k4_0
        k.kq_mm = kq_mm_count() - mm0
        if (k.range_refusal) {
            var st_rf = ParakeetState()
            var out_rf : array<float>
            blocks_refused_leg(t, k.twin_tag, fam, 256l << 20l) $ {
                pk_twin_encode(m, st_rf, samples, out_rf)
            }
            delete out_rf
            delete st_rf
        }
    } elif (leg == LaneFormLeg.cpu) {
        k.c_kq = m.kq_lane
    }
    delete st
}

// A form's leg of the v2 bin's short clips: its q8 lane's CPU image, or that lane's image in the discrete home's flavor.
[cold_path]
def private pk_short_clip_leg(t : T?; m : ParakeetModel; var k : PkKept) {
    t |> success(m.q8, "{k.clips.tag}: the short clips ride the q8 lane")
    var st = ParakeetState()
    short_clip_leg(k.clips, TowerFamily.parakeet, m.dev_backend != TowerDev.none, m, st) $(e; var s; clip; var out) => pk_twin_encode(e, s, clip, out)
    delete st
}

// Parakeet-TDT v2 on the discrete home, after the bin's block: the front and every block on the device within the three-way
// bar on jfk, a poison past it, and the exact lane within its bar.
[cold_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private test_parakeet_discrete_twin(t : T?; k : PkKept) {
    return if (asr_clip_absent(t, "jfk.wav"))
    let home = serving_tower_dev(TowerFamily.parakeet)
    t |> success(!k.flags.x_q8 && k.flags.c_q8 && k.flags.d_backend == home, "the exact and the q8 models minted, the q8 lane's {home} image beside")
    let n_layer = k.twin.rows.spec.n_layer
    twin_rows_held(t, k.twin.rows) $(legs) {
        fc_discrete_counters(t, "parakeet jfk", n_layer, legs)
    }
    fc_exact_lane_held(t, k.exact)
}

// Parakeet-TDT v3 on the Metal FastConformer chain, after the bin's block: the q8 lane three ways with its poison, then
// the exact lane.
[cold_path]
def private pk_fc_metal_twin(t : T?; k : PkKept) {
    return if (asr_clip_absent(t, "jfk.wav"))
    fc_metal_twin_held(t, "models", k.flags, k.twin, k.exact)
}

// The Q4_K_M carrier's kq lane on the discrete home, after its block: the front and every block from the kq lane's device
// planes within the three-way bar on jfk, a k4 tile counted, zeroed seats past the bar.
[cold_path, arch(at = "../ARCHITECTURE_MEDIA_ASR.md#parakeet-kq-seats")]
def private test_parakeet_kq_discrete_twin(t : T?; k : PkKept) {
    return if (asr_clip_absent(t, "jfk.wav"))
    let home = serving_tower_dev(TowerFamily.parakeet)
    t |> success(k.c_kq && k.d_kq && k.flags.d_backend == home, "the kq lane's CPU image, its {home} image beside")
    t |> equal(k.d_fmt, "k4", "the {home} image's lane is named by its one K-quant member's disk type")
    t |> equal(k.d_host_planes, 0l, "the {home} image carries the device planes alone")
    let n_layer = k.twin.rows.spec.n_layer
    twin_rows_held(t, k.twin.rows) $(legs) {
        t |> equal(legs.n_dev, legs.n_exact, "parakeet kq jfk: the device chain's frame count")
        t |> equal(legs.dev.encodes - legs.pre.encodes, 1l, "parakeet kq jfk: the device served the encode")
        t |> equal(legs.dev.blocks - legs.pre.blocks, n_layer, "parakeet kq jfk: every block ran on the driver")
        front_served(t, "parakeet kq jfk", legs.dev.convs - legs.pre.convs, 1l, legs.dev.declines - legs.cpu.declines)
    }
    t |> success(k.kq_tiles > 0l, "parakeet kq jfk: a k4 tile served the K-quant seats (the census counter rose)")
}

// The Q4_K_M carrier's kq lane on the Metal FastConformer chain, after its block: the front and every block from the kq
// lane's slabs within the three-way bar on jfk with no `quant_mode` decline, a K-quant mul_mm stamp counted, zeroed seats
// past the bar.
[cold_path, arch(at = "../ARCHITECTURE_MEDIA_ASR.md#parakeet-kq-seats")]
def private test_parakeet_kq_metal_twin(t : T?; k : PkKept) {
    return if (asr_clip_absent(t, "jfk.wav"))
    t |> success(k.c_kq && k.d_kq && k.flags.d_backend == TowerDev.metal, "the kq lane's CPU image, its Metal image beside")
    t |> equal(k.d_fmt, "k4", "the Metal image's lane is named by its one K-quant member's disk type")
    metal_q8_twin_held(t, k.twin)
    t |> success(k.kq_mm > 0l, "parakeet kq metal jfk: a K-quant mul_mm stamp served the K-quant seats (the census counter rose)")
}

//! the encoder rows of `samples` through `m` on the CPU chain
def private pk_cpu_rows(m : ParakeetModel; samples : array<float>) : array<float> {
    var out : array<float>
    var st = ParakeetState()
    with_gpu_knobs() {
        with_dasllama_jobque_() {
            set_serving_gpu_tower(false, TowerFamily.parakeet)
            pk_twin_encode(m, st, samples, out)
        }
    }
    delete st
    return <- out
}

//! the encoder rows of `samples` through the image file `dlim`, mapped for the call alone; inside a job queue
[cold_path]
def private pk_image_file_rows(dlim : string; samples : array<float>) : array<float> {
    var inscope m <- load_parakeet_model(dlim, true)
    return <- pk_cpu_rows(m, samples)
}

[test(description = "A Parakeet image its host holds in memory loads from the bytes on the lane it was baked on and encodes jfk to the rows the image's file gives, with the file gone, and `load_asr_model` routes the bytes to parakeet; with its planes zeroed the load encodes other rows")]
def test_parakeet_from_memory(tst : T?) {
    tst |> run("parakeet-TDT 0.6B v3 Q4_K_M: the CPU image's bytes, borrowed in place") @(t : T?) {
        own_load("reloads the model: the cold mint lands in scratch, loads back as a file, then three times from the bytes in memory")
        let gguf = path_join(models_dir(), "parakeet-tdt-0.6b-v3-Q4_K_M.gguf")
        return if (!model_available(t, gguf))
        let samples <- asr_samples(t, "jfk.wav")
        return if (empty(samples))
        with_dasllama_jobque_() {
            with_gpu_knobs() {
                set_serving_gpu_tower(false, TowerFamily.parakeet)   // the CPU image: the one an image in memory serves
                reset_parakeet_q8()   // the un-pinned load: the kq lane for this quantized carrier
                let dlim = path_join(test_temp_dir(), "pk_memory.dlim")
                var minted_kq = false
                with_image_destination(dlim) {   // the cold mint lands in scratch, not beside the carrier
                    var inscope minted <- load_parakeet_model(gguf, true)
                    minted_kq = minted.kq_lane
                }
                var inscope from_file <- pk_image_file_rows(dlim, samples)
                t |> success(!empty(from_file), "the image's file encodes the clip")
                var raw : array<uint8>
                var size = 0ul
                let base = read_page_aligned(dlim, raw, size)
                remove(dlim)
                t |> success(base != null, "the image reads into memory")
                if (base != null) {
                    {
                        var inscope m <- load_parakeet_model(base, size)
                        t |> equal(m.kq_lane, minted_kq, "the bytes load on the lane the image was baked on")
                        var inscope from_memory <- pk_cpu_rows(m, samples)
                        t |> equal(length(from_memory), length(from_file), "the same row count")
                        t |> success(sequence_equal(from_memory, from_file), "and encode the clip to the same rows")
                    }
                    {
                        var inscope routed <- load_asr_model(base, size)
                        t |> success(routed.kind == AsrKind.parakeet, "the ASR loader routes the bytes to parakeet")
                    }
                    zero_planes(base, size)
                    var inscope poisoned <- load_parakeet_model(base, size)
                    var inscope other <- pk_cpu_rows(poisoned, samples)
                    t |> success(!sequence_equal(other, from_file), "its planes zeroed, the load encodes other rows (the control)")
                }
                delete raw
            }
        }
    }
}

// The f32 ggml bin's side of the F32 GGUF's compares, on the v3 bin's `f32` form: its shape, jfk's encoder rows whole and
// at half volume, and its transcript at jfk's word floor.
[cold_path]
def private pk_bin_carrier_leg(t : T?; mb : ParakeetModel; var k : PkKept) {
    let samples <- asr_samples(t, "jfk.wav")
    return if (empty(samples))
    k.carrier.n_layer = mb.n_layer
    k.carrier.n_vocab = mb.n_vocab
    k.carrier.blob_n = mb.blob.n
    k.carrier.rows <- pk_cpu_rows(mb, samples)
    var inscope loud <- [for (v in samples); v * 0.5]
    k.carrier.loud <- pk_cpu_rows(mb, loud)
    let bin_text = pk_cpu_text(mb, samples)
    jfk_words_floor(t, "parakeet f32 bin", bin_text)
    k.carrier.text = bin_text
    k.carrier.kept = true
}

//! true, with the cell skipped, where jfk.wav is absent or the v3 f32 bin's block kept nothing for the GGUF's compares
def private pk_no_bin_reference(t : T?; k : PkKept) : bool {
    return true if (asr_clip_absent(t, "jfk.wav"))
    return false if (k.carrier.kept)
    t |> skip("{PK_V3}'s f32 form kept no rows - its block did not run")
    return true
}

// The F32 GGUF against the f32 ggml bin, on the GGUF's `f32` form: the bin's planes staged, jfk encoded to the same rows.
[cold_path, arch(at = "../ARCHITECTURE_MEDIA_ASR.md#parakeet-gguf")]
def private test_parakeet_gguf_carrier(t : T?; mg : ParakeetModel; k : PkKept) {
    return if (pk_no_bin_reference(t, k))
    let samples <- load_audio_16k_mono(asr_clip("jfk.wav"))
    t |> equal(mg.n_layer, k.carrier.n_layer, "the block count")
    t |> equal(mg.n_vocab, k.carrier.n_vocab, "the vocabulary without the blank")
    t |> equal(mg.blob.n, k.carrier.blob_n, "the f32 blob's length")
    var inscope rg <- pk_cpu_rows(mg, samples)
    var got = TowerParityStats()
    if (tower_parity_stats(t, "parakeet gguf vs bin", k.carrier.rows, rg, got)) {
        to_log(LOG_INFO, "test_audio: parakeet F32 gguf vs f32 bin on jfk: rel_l2 {got.rel_l2}\n")
        t |> success(got.rel_l2 <= PK_GGUF_BIN_BAR, "the GGUF's encoder rows are the bin's ({got.rel_l2})")
    }
    var ctl = TowerParityStats()
    if (long_length(k.carrier.loud) == long_length(k.carrier.rows) && tower_parity_stats(t, "parakeet gguf vs bin control", k.carrier.loud, rg, ctl)) {
        t |> success(is_finite(ctl.rel_l2) && ctl.rel_l2 > PK_GGUF_BIN_BAR, "the bin's rows of the clip at half volume land outside the bar ({ctl.rel_l2}) - the control")
    }
}

// The decoder half of the F32 GGUF, on its `f32` form: jfk's transcript is the f32 bin's.
[cold_path]
def private pk_gguf_bin_text(t : T?; mg : ParakeetModel; k : PkKept) {
    return if (pk_no_bin_reference(t, k))
    let samples <- load_audio_16k_mono(asr_clip("jfk.wav"))
    let bin_text = k.carrier.text
    t |> equal(pk_cpu_text(mg, samples), bin_text, "the F32 GGUF's transcript is the f32 bin's: the predictor, the joint and the durations read alike")
}

// A form's rows of the Q4_K_M carrier's CPU lanes on jfk: the dequantized exact lane (`f32`), the q8 lane, the kq lane -
// named k4, K-quant seats beside Q8_0 ones - and the kq lane with its seats zeroed from the middle block on (`kq_z12`).
[cold_path]
def private pk_kq_lane_leg(t : T?; m : ParakeetModel; var k : PkKept) {
    let samples <- asr_samples(t, "jfk.wav")
    return if (empty(samples))
    let f = parakeet_form_of(k.form)
    if (f.lane == "f32") {
        k.lanes.x_kq = m.kq_lane
        k.lanes.ex <- pk_cpu_rows(m, samples)
    } elif (f.lane == "q8") {
        k.lanes.q_q8 = m.q8
        k.lanes.q8 <- pk_cpu_rows(m, samples)
    } elif (f.zero_block >= 0l) {
        k.lanes.bad <- pk_cpu_rows(m, samples)
    } else {
        k.lanes.k_kq = m.kq_lane
        k.lanes.k_q8 = m.q8
        k.lanes.n_layer = m.n_layer
        t |> equal(parakeet_exec_fmt(m), "k4", "the lane's name: its one K-quant member's disk type")
        t |> success(m.kq.n > 0l && m.qblob.n > 0l, "K-quant seats beside Q8_0 ones: the attention outputs and the pointwise convs")
        k.lanes.kq <- pk_cpu_rows(m, samples)
    }
    k.lanes.legs++
}

// The Q4_K_M carrier's kq lane on the CPU against its q8 lane, after the block: the kq lane's rows within
// `PK_KQ_CPU_SLACK` x the q8 lane's distance from the dequantized chain, the zeroed seats past that bar.
[cold_path]
def private pk_kq_lanes_held(t : T?; k : PkKept) {
    return if (asr_clip_absent(t, "jfk.wav"))
    t |> equal(k.lanes.legs, 4, "the Q4_K_M carrier kept its exact, q8, kq and poisoned kq rows")
    return if (k.lanes.legs != 4)
    t |> success(k.lanes.k_kq && !k.lanes.k_q8 && !k.lanes.x_kq && k.lanes.q_q8, "the kq lane's model beside the exact and the q8 lanes'")
    var q8_ex = TowerParityStats()
    var kq_ex = TowerParityStats()
    if (tower_parity_stats(t, "parakeet q8 vs exact", k.lanes.ex, k.lanes.q8, q8_ex) && tower_parity_stats(t, "parakeet kq vs exact", k.lanes.ex, k.lanes.kq, kq_ex)) {
        let bar = PK_KQ_CPU_SLACK * q8_ex.rel_l2 + TWIN_REL_L2_FLOOR
        to_log(LOG_INFO, "test_audio: parakeet Q4_K_M on jfk: rel_l2 from the dequantized chain - kq {kq_ex.rel_l2}, q8 {q8_ex.rel_l2}, the bar {bar}\n")
        t |> success(kq_ex.rel_l2 <= bar, "the kq lane's rows within {PK_KQ_CPU_SLACK}x the q8 lane's rel_l2 from the dequantized chain ({kq_ex.rel_l2} vs {q8_ex.rel_l2})")
        var bad_ex = TowerParityStats()
        if (tower_parity_stats(t, "parakeet poisoned kq vs exact", k.lanes.ex, k.lanes.bad, bad_ex)) {
            t |> success(is_finite(bad_ex.rel_l2) && bad_ex.rel_l2 > bar, "zeroed seats from the middle block on EXCEED the bar ({bad_ex.rel_l2} vs {bar})")
        }
    }
}

// The decoder half of the Q4_K_M carrier, on its `kq` form: jfk's transcript at the word floor.
[cold_path]
def private pk_kq_text_cell(t : T?; mk : ParakeetModel) {
    let samples <- asr_samples(t, "jfk.wav")
    return if (empty(samples))
    let kq_text = pk_cpu_text(mk, samples)
    to_log(LOG_INFO, "test_audio: parakeet Q4_K_M kq jfk: {kq_text}\n")
    jfk_words_floor(t, "parakeet Q4_K_M kq lane", kq_text)
}

// The Q8_0 GGUF on the kq lane, on its `kq` form: every seat Q8_0, so the lane is named q8; an F16 GGUF is no kq carrier.
[cold_path]
def private pk_q8_0_gguf_lane(t : T?; m : ParakeetModel) {
    t |> equal(parakeet_exec_fmt(m), "q8", "the Q8_0 file: every seat Q8_0")
    let f16 = path_join(models_dir(), "parakeet-tdt-0.6b-v3-F16.gguf")
    t |> equal(parakeet_load_tag(f16, true) == PARAKEET_KQ_TAG, false, "an F16 GGUF is no kq carrier") if (model_available(t, f16))
}

let private PK_GGUF_BIN_BAR = 1e-4lf   //! the two carriers' rows apart: one model's f32 tensors, the GGUF's filterbank and window rebuilt from its header
let private PK_KQ_CPU_SLACK = 2.5lf    //! the CPU kq lane's rel_l2 over the q8 lane's, which it sits at about twice: a K-quant seat's Q8_K feed holds one scale a 256 elements where Q8_0's holds one a 32

//! the kq planner's refusal of the Q4_K_M carrier with `retype` applied, staged on the kq lane - "" where it staged
[cold_path]
def private pk_retyped_refusal(name : string; retype : table<string; int>) : string {
    own_load("asserts on the load itself - its refusal: the kq planner over a retyped copy of the carrier written to scratch")
    return gguf_retyped_refusal(path_join(models_dir(), "parakeet-tdt-0.6b-v3-Q4_K_M.gguf"), name, retype) $(p) {
        var inscope st <- stage_parakeet_kq_model(p)
    }
}

[test(description = "Over retyped copies of the Q4_K_M Parakeet carrier the kq planner refuses a Q5_1 seat, q in Q8_0 beside K-quant k and v, and one block's position linear in Q8_0, each by name, stages an unchanged copy, refuses a ggml bin, and the load tag is the kq lane's for a quantized GGUF asked for q8 un-pinned alone"), arch(at = "../ARCHITECTURE_MEDIA_ASR.md#parakeet-kq-seats")]
def test_parakeet_kq_refusals(tst : T?) {
    let path = path_join(models_dir(), "parakeet-tdt-0.6b-v3-Q4_K_M.gguf")
    return if (!model_available(tst, path))
    tst |> run("the kq planner refuses what its lane cannot seat") @(t : T?) {
        let b0 = "enc.blocks.0."
        let seat = pk_retyped_refusal("pk_seat", { "{b0}ff1.linear1.weight" => GGML_TYPE_Q5_1 })
        t |> success(find(seat, "has no seat for") >= 0 && find(seat, "{b0}ff1.linear1.weight") >= 0, "a Q5_1 GEMM refused at its seat naming the tensor: '{seat}'")
        let qkv = pk_retyped_refusal("pk_qkv", { "{b0}attn.linear_q.weight" => GGML_TYPE_Q8_0 })
        t |> success(find(qkv, "attention q, k and v mix Q8_0 with a K-quant format") >= 0, "q in Q8_0 beside K-quant k and v refused: '{qkv}'")
        let pos = pk_retyped_refusal("pk_pos", { "enc.blocks.1.attn.linear_pos.weight" => GGML_TYPE_Q8_0 })
        t |> success(find(pos, "position linears mix Q8_0 with a K-quant format") >= 0 && find(pos, "block 1") >= 0, "one block's position linear in Q8_0 refused naming the block: '{pos}'")
        let nothing : table<string; int>
        t |> equal(pk_retyped_refusal("pk_clean", nothing), "", "the copy with nothing retyped stages (the control)")
    }
    tst |> run("the kq lane is a quantized GGUF's alone") @(t : T?) {
        own_load("asserts on the load itself - its refusal: the kq lane refuses a ggml bin")
        reset_parakeet_q8()
        t |> equal(parakeet_load_tag(path, true), PARAKEET_KQ_TAG, "the Q4_K_M GGUF asked for q8, un-pinned: the kq lane")
        t |> equal(parakeet_load_tag(path, false), "parakeet-f32", "asked for the exact lane: the f32 lane")
        set_parakeet_q8(true)
        t |> equal(parakeet_load_tag(path, true), "parakeet-q8", "pinned q8: the q8 lane, the carrier dequantized")
        reset_parakeet_q8()
        let f32 = path_join(models_dir(), "parakeet-tdt-0.6b-v3-F32.gguf")
        if (model_available(t, f32)) {
            t |> equal(parakeet_load_tag(f32, true), "parakeet-q8", "the F32 GGUF asked for q8: the q8 lane (the control)")
        }
        let bin = path_join(whisper_dir(), "ggml-parakeet-tdt-0.6b-v3-f32.bin")
        if (model_available(t, bin)) {
            let why = panic_text() $ {
                var inscope st <- stage_parakeet_kq_model(bin)
            }
            t |> success(find(why, "is a ggml bin") >= 0, "the kq lane refuses a ggml bin: '{why}'")
        }
    }
}

//! jfk-style clip `samples` transcribed through `m` on the CPU chain and the CPU decode
def private pk_cpu_text(m : ParakeetModel; samples : array<float>) : string {
    var text = ""
    var st = ParakeetState()
    with_gpu_knobs() {
        with_dasllama_jobque_() {
            set_serving_gpu_tower(false, TowerFamily.parakeet)
            parakeet_log_mel(samples, m, st)
            let tt = parakeet_encode(m, st)
            parakeet_tdt_decode(m, st, tt)
            text = parakeet_detokenize(m, st.toks)
        }
    }
    delete st
    return text
}

//! the loader's refusal of the Q4_K_M carrier with byte `v` written at offset `at` of header key `key`'s value - "" where it staged
[cold_path]
def private pk_header_refusal(name, key : string; at, v : int) : string {
    own_load("asserts on the load itself - its refusal: the loader over a copy of the carrier with one header byte rewritten, written to scratch")
    let dst = path_join(test_temp_dir(), "{name}.gguf")
    let nothing : table<string; int>
    gguf_retyped_copy(path_join(models_dir(), "parakeet-tdt-0.6b-v3-Q4_K_M.gguf"), dst, nothing, [(key = key, at = at, v = v)])
    let why = panic_text() $ {
        var inscope st <- stage_parakeet_kq_model(dst)
    }
    remove(dst)
    return why
}

//! the kq lane's name over the Q4_K_M carrier with `retype` applied, staged
[cold_path]
def private pk_retyped_lane_name(name : string; retype : table<string; int>) : string {
    own_load("asserts on the load itself - the lane a retyped copy of the carrier, written to scratch, stages on")
    let dst = path_join(test_temp_dir(), "{name}.gguf")
    gguf_retyped_copy(path_join(models_dir(), "parakeet-tdt-0.6b-v3-Q4_K_M.gguf"), dst, retype)
    var inscope st <- stage_parakeet_kq_model(dst)
    remove(dst)
    return parakeet_exec_fmt(st.e)
}

//! the Parakeet reader's refusal of a GigaAM carrier, by name
def private pk_gigaam_refusal(t : T?) {
    own_load("asserts on the load itself - its refusal: the Parakeet reader refuses a GigaAM carrier by name")
    let ga = path_join(models_dir(), "gigaam-v3-e2e-rnnt-f16.gguf")
    if (model_available(t, ga)) {
        let why = panic_text() $ {
            var inscope st <- stage_parakeet_model(ga, false)
        }
        t |> success(find(why, "parakeet") >= 0 && !empty(why), "a GigaAM GGUF refused by the Parakeet reader: '{why}'")
    }
}

[test(description = "The Parakeet GGUF loader refuses a foreign architecture, linear biases, input scaling, an FF off four times the width, a token count off the predictor's and a front end off 16 kHz by name; over retyped copies a Q6_K seat names the lane kq and a BF16 GEMM stages on the k4 lane"), arch(at = "../ARCHITECTURE_MEDIA_ASR.md#parakeet-gguf")]
def test_parakeet_gguf_loads(tst : T?) {
    let path = path_join(models_dir(), "parakeet-tdt-0.6b-v3-Q4_K_M.gguf")
    return if (!model_available(tst, path))
    tst |> run("the header's refusals") @(t : T?) {
        let enc = "stt.parakeet.encoder"
        t |> success(find(pk_header_refusal("pk_bias", "{enc}.use_bias", 0, 1), "{enc}.use_bias") >= 0, "linear biases refused naming the key")
        t |> success(find(pk_header_refusal("pk_xs", "{enc}.xscaling", 0, 1), "{enc}.xscaling") >= 0, "input scaling refused naming the key")
        t |> success(find(pk_header_refusal("pk_dff", "{enc}.d_model", 0, 1), "an FF four times the model width") >= 0, "an FF off four times the width refused")
        t |> success(find(pk_header_refusal("pk_tok", "stt.parakeet.predictor.vocab", 0, 7), "the blank is the last of them") >= 0, "a token count off the predictor's refused")
        t |> success(find(pk_header_refusal("pk_rate", "stt.frontend.sample_rate", 0, 129), "Hz front end") >= 0, "a front end off 16 kHz refused")
        t |> success(find(pk_header_refusal("pk_hop", "stt.frontend.hop_length", 0, 161), "Hz front end") >= 0, "a hop off 160 refused")
        t |> equal(pk_header_refusal("pk_same", "stt.frontend.hop_length", 0, 160), "", "the header rewritten with its own byte stages (the control)")
        pk_gigaam_refusal(t)
    }
    tst |> run("the lane's name follows its seats") @(t : T?) {
        let b0 = "enc.blocks.0."
        t |> equal(pk_retyped_lane_name("pk_mix", { "{b0}ff1.linear1.weight" => GGML_TYPE_Q6_K }), "kq", "a Q6_K seat beside the Q4_K ones: the members mix")
        t |> equal(pk_retyped_lane_name("pk_bf16", { "{b0}ff1.linear1.weight" => GGML_TYPE_BF16 }), "k4", "a BF16 GEMM takes a Q8_0 seat: the lane keeps its one member's name")
        let nothing : table<string; int>
        t |> equal(pk_retyped_lane_name("pk_plain", nothing), "k4", "the copy with nothing retyped (the control)")
    }
}

[test(description = "The ASR facade loads the Q4_K_M Parakeet GGUF alone on the kq lane's CPU image, names the lane k4, holds jfk's word floor, and loads the lane's image back by path to the same transcript"), arch(at = "../ARCHITECTURE_MEDIA_ASR.md#parakeet-kq-seats")]
def test_parakeet_gguf_facade(tst : T?) {
    let path = path_join(models_dir(), "parakeet-tdt-0.6b-v3-Q4_K_M.gguf")
    return if (!model_available(tst, path))
    tst |> run("the facade loads the GGUF alone, and its kq image back by path") @(t : T?) {
        own_load("writes a `.dlim` image beside the file: the facade's load of the carrier bakes the kq lane's image, which the cell loads back by path")
        let samples <- asr_samples(t, "jfk.wav")
        return if (empty(samples))
        t |> success(!asr_needs_mmproj(path), "a Parakeet GGUF is a one-file carrier")
        with_gpu_knobs() {
            set_serving_gpu_tower(false, TowerFamily.parakeet)
            reset_parakeet_q8()
            var inscope am <- load_asr_model(path)
            t |> success(am.kind == AsrKind.parakeet && am.pk.kq_lane && am.pk.dev_backend == TowerDev.none, "the GGUF routes to parakeet on the kq lane's CPU image")
            t |> equal(asr_exec_fmt(am), "k4", "the facade names the lane by its member")
            let text = pk_cpu_text(am.pk, samples)
            jfk_words_floor(t, "parakeet Q4_K_M through the facade", text)
            let img = "{path}.{PARAKEET_KQ_TAG}.dlim"
            var fs : FStat
            if (stat(img, fs) && fs.is_reg) {
                var inscope im <- load_asr_model(img)
                t |> success(im.kind == AsrKind.parakeet && im.pk.kq_lane, "the image routes by its tag to the kq lane")
                t |> equal(pk_cpu_text(im.pk, samples), text, "the image by path transcribes as the load that minted it")
            } else {
                t |> skip("no prepared image beside the carrier (the image rail is off)")
            }
        }
    }
}

//! `mel_spectrum_rows` on a synthetic DFT table: the ln of each filter's sum over the spectrum `magnitude` asks for, in double
def private spectrum_ref(xw, tw, filt : array<float>; n_len, n_fft, n_bins, n_mel : int64; magnitude : bool) : array<float> {
    var out : array<float>
    out |> reserve(n_len * n_mel)
    for (i in range64(n_len)) {
        for (jm in range64(n_mel)) {
            var sum = 0.0lf
            for (k in range64(n_bins)) {
                var re = 0.0lf
                var im = 0.0lf
                for (n in range64(n_fft)) {
                    re += double(xw[i * n_fft + n]) * double(tw[n * n_bins * 2l + 2l * k])
                    im += double(xw[i * n_fft + n]) * double(tw[n * n_bins * 2l + 2l * k + 1l])
                }
                let p = re * re + im * im
                sum += double(filt[jm * n_bins + k]) * (magnitude ? sqrt(p) : p)
            }
            out |> push(float(log(max(sum, 1e-10lf))))
        }
    }
    return <- out
}

[test(description = "mel_spectrum_rows sums the magnitude or the power of the spectrum into the filters to within 1e-5 of a double-precision DFT reference, and the magnitude form must mismatch the power reference")]
def test_mel_spectrum_magnitude(t : T?) {
    t |> run("the magnitude form sums |X| into the filters, the power form |X|^2, each against a double DFT") @(t : T?) {
        let n_len = 5l
        let n_fft = 16l
        let n_bins = 9l
        let n_mel = 4l
        var inscope xw <- [for (i in range64(n_len * n_fft)); sin(0.37 * float(i)) * 3.0]
        var inscope tw <- [for (i in range64(n_fft * n_bins * 2l)); cos(0.11 * float(i * i % 97))]
        var inscope filt <- [for (i in range64(n_mel * n_bins)); 0.1 + 0.05 * float(i % 7)]
        var inscope reim, mel_mag, mel_pow : array<float>
        let fp = unsafe(addr<float const?>(filt[0]))
        let tp = unsafe(addr<float const?>(tw[0]))
        mel_spectrum_rows(xw, tp, fp, n_len, n_fft, n_bins, n_mel, MelForm(magnitude = true, log = MelLog.ln_max, floor = 1e-10lf), reim, mel_mag)
        mel_spectrum_rows(xw, tp, fp, n_len, n_fft, n_bins, n_mel, MelForm(log = MelLog.ln_max, floor = 1e-10lf), reim, mel_pow)
        var inscope want_mag <- spectrum_ref(xw, tw, filt, n_len, n_fft, n_bins, n_mel, true)
        var inscope want_pow <- spectrum_ref(xw, tw, filt, n_len, n_fft, n_bins, n_mel, false)
        let bar = Bar(rel = 1e-5, abs_floor = 1e-5)
        check_rows(t, "the magnitude form", mel_mag, want_mag, bar, int(n_mel))
        check_rows(t, "the power form", mel_pow, want_pow, bar, int(n_mel))
        check_exceeds(t, "the magnitude form against the power reference (MUST mismatch)", mel_mag, want_pow, bar)
    }
}

let private CN_ENC = "canary-qwen-2.5b-encoder-f32.bin"
let private G4A_E2B = "mmproj-gemma-4-E2B-it-bf16.gguf"
let private G4A_E4B = "mmproj-gemma-4-E4B-it-BF16.gguf"

//! the Canary encoder bin's form read off its name (`tower_form_of`): the lanes `f32` (the exact lane) and `q8`
def private canary_form_of(form : string) : TowerForm => tower_form_of(form, ["f32", "q8"], "canary")

//! the staged read a Canary encoder bin's form is minted from (`tower_stage_key_of`)
def private canary_stage_key(form : string) : string => tower_stage_key_of(canary_form_of(form))

//! the Canary encoder bin's staged read the key `key` names: its lane, in its image flavor
def private canary_stage(path, key : string) : CanaryStaging {
    let f = canary_form_of(key)
    return <- tower_dev_staged(tower_form_dev_of(f, TowerFamily.canary)) $ { return <- stage_canary_encoder(path, f.lane == "q8") }
}

//! One form of the Canary encoder bin, its block's form loader (`canary_form_of`): minted in memory (`mint_cn`) from `stg`,
//! its own copy of the bin's staging on the lane it names - never `load_canary_encoder`, which bakes a `.dlim` -
//! `_z<block>` zeroing the GEMM planes of the poison's blocks from that block on, `@dev` the image flavor of canary's
//! serving home.
[unused_argument(path)]
def private canary_form(path, form : string; var stg : CanaryStaging) : CanaryEncoder {
    let f = canary_form_of(form)
    if (f.image) {
        panic("the canary tests: '{form}' names the image rail - the encoder's forms are minted in memory")
    }
    return <- mint_cn(stg, f.lane == "q8", f.zero_block, tower_form_dev_of(f, TowerFamily.canary))
}

//! The form is the lane, poison and image flavor it names: on `enc`, the encoder minted as the form `form` of `file`, the
//! lane flag, the poison's block the tower's middle one, and the image flavor.
def private canary_form_is(t : T?; file, form : string; enc : CanaryEncoder) {
    let f = canary_form_of(form)
    minted_form_is(t, file, form, f, TowerFamily.canary, enc.q8, enc.n_layer, enc.dev_backend)
}

//! a gemma4a mmproj's form read off its name (`tower_form_of`): the lanes `f32` (the exact lane) and `q8`; `asr` and
//! `asrdev` name no lane - `image_asr` and `image_asrdev` are the ASR facade's two-file load of the mmproj
def private g4a_form_of(form : string) : TowerForm => tower_form_of(form, ["f32", "q8", "asr", "asrdev"], "gemma4a")

//! the staged read a gemma4a mmproj's form is minted from (`tower_stage_key_of`)
def private g4a_stage_key(form : string) : string => tower_stage_key_of(g4a_form_of(form))

//! A gemma4a mmproj's form as its block holds it: the audio carrier `e` - its own load, or an encoder minted in memory -
//! or, on the `image_asr` and `image_asrdev` forms, the ASR facade's pair `asr` of the mmproj and its decoder.
struct private G4aForm {
    e : AudioEmbedder = AudioEmbedder()
    asr : AsrModel = AsrModel()
}

def private finalize(var h : G4aForm) {
    delete h.e
    delete h.asr
}

//! whether the form `f` is the ASR facade's two-file load
def private g4a_form_asr(f : TowerForm) : bool => f.image && (f.lane == "asr" || f.lane == "asrdev")

//! One form of a gemma4a mmproj, its block's form loader behind the file's own (`gemma4a_e2b_form`,
//! `gemma4a_e4b_form`, which name `decoder`, the mmproj's Q8 decoder under the models directory). `image` is the audio
//! carrier's own load (`load_audio_embedder`) and `image_asr` the ASR facade's two-file load with `decoder`
//! (`load_asr_model`, which sniffs the pair's route and bakes each file's image), each with the family's lane pin unset,
//! as the run's knobs route it, the facade's load keeping the host planes its f32 K/V cells read; `image_asrdev` is the facade's load with gemma4a's serving tower knob on for the load,
//! which the load's flavor pick reads - the knobs put back however a load leaves. Any other form is minted in memory
//! (`mint_g4a`) from `stg`, its own copy of the mmproj's staging on the lane it names: `_z<block>` zeroes the GEMM planes
//! of the poison's blocks from that block on, `@dev` is the image flavor of gemma4a's serving home.
def private gemma4a_form(path, form, decoder : string; var stg : Gemma4aStaging) : G4aForm {
    let f = g4a_form_of(form)
    var inscope h = G4aForm()
    if (!f.image) {
        if (f.lane != "f32" && f.lane != "q8") {
            panic("the gemma4a tests: '{form}' names no lane to mint")
        }
        h.e.kind = AudioKind.gemma4a
        h.e.gemma4a <- mint_g4a(stg, f.zero_block, tower_form_dev_of(f, TowerFamily.gemma4a))
        return <- h
    }
    let knobs = gpu_knobs()
    let planes = host_planes_pin()
    reset_gemma4a_q8()
    if (f.lane == "asrdev") {
        set_serving_gpu_tower(true, TowerFamily.gemma4a)
    }
    if (f.lane == "asr") {
        set_host_planes(true)   //! its cells ride the f32 K/V CPU rail: a load onto a discrete home keeps no plane on the host
    }
    with_put_back($ {
        set_gpu_knobs(knobs)
        restore_host_planes(planes)
    }) {
        if (g4a_form_asr(f)) {
            h.asr <- load_asr_model(path_join(models_dir(), decoder), path)
        } else {
            h.e <- load_audio_embedder(path)
        }
    }
    return <- h
}

//! the E2B mmproj's form loader: `gemma4a_form` with the E2B Q8 decoder
def private gemma4a_e2b_form(path, form : string; var stg : Gemma4aStaging) : G4aForm => <- gemma4a_form(path, form, "gemma-4-E2B-it-Q8_0.gguf", stg)

//! the E4B mmproj's form loader: `gemma4a_form` with the E4B Q8 decoder
def private gemma4a_e4b_form(path, form : string; var stg : Gemma4aStaging) : G4aForm => <- gemma4a_form(path, form, "gemma-4-E4B-it-Q8_0.gguf", stg)

//! The form is the family, lane, poison and image flavor it names: on `h`, a mmproj loaded as the form `form` of `file`,
//! the facade's gemma4a route on an ASR form, else the carrier's gemma4a arm and, on a minted form, the lane flag, the
//! poison's block the tower's middle one, and the image flavor. An image form's lane and flavor are the run's policy:
//! not read.
def private gemma4a_form_is(t : T?; file, form : string; h : G4aForm) {
    let f = g4a_form_of(form)
    if (g4a_form_asr(f)) {
        t |> success(h.asr.kind == AsrKind.gemma4a, "{file}: the {form} form is the facade's gemma4a pair")
        return
    }
    t |> success(h.e.kind == AudioKind.gemma4a, "{file}: the {form} form is the carrier's gemma4a arm")
    return if (f.image)
    minted_form_is(t, file, form, f, TowerFamily.gemma4a, h.e.gemma4a.q8, h.e.gemma4a.n_layer, h.e.gemma4a.dev_backend)
}

//! What a Canary or gemma4a tower file's block keeps for the compares that follow it. `tag` names the twin in its asserts
//! and `form` the form whose cells run; the three clean forms' lane flags and the device image's flavor; a gemma4a
//! encoder's own width and its projection, `want_proj` the decoder's width; the q8 lane's rows three ways
//! (`LaneTwinKept`), the regrown residencies' (`TwinRegrowKept`), the exact lane's (`ExactFlavorKept`) and the short
//! clips' (`ShortClipsKept`).
struct private EncTwinKept {
    tag : string
    form : string
    flags : LaneFlags
    mid_dim : int64
    proj_dim : int64
    want_proj : int64
    twin : LaneTwinKept = LaneTwinKept()
    regrow : TwinRegrowKept
    exact : ExactFlavorKept
    clips : ShortClipsKept
}

// The legs the form `f` of a tower `tw` adds to its file's twin on jfk, each on the form's one home: the exact form the
// reference chain and the exact lane's CPU leg, the q8 form the CPU q8 chain, its `@dev` form the device chain - and, on
// a discrete home, the regrown residencies - the zeroed `@dev` forms the input poisons, the exact lane's `@dev` form its
// device leg. `encode` is the family's encode of a clip.
[cold_path]
def private enc_twin_leg(t : T?; var k : EncTwinKept; f : TowerForm; fam : TowerFamily; tw : auto(TW); var st : auto(ST); samples : array<float>;
                         encode : block<(e : TW; var s : ST; clip : array<float>; var out : array<float>) : int64>) {
    let discrete = serving_tower_dev(fam) != TowerDev.metal
    lane_twin_named(k.twin, k.tag, fam, twin_facts(tw).n_layer)
    exact_flavor_named(k.exact, k.tag, fam, TWIN_EXACT_REL_L2_VK, -1l)
    if (f.lane != "f32" && f.zero_block >= 0l && discrete) {
        twin_regrow_poison_leg(k.regrow, fam, tw, st, samples, encode)
        return
    }
    let leg = lane_exact_leg(t, k.twin, k.exact, k.flags, f, true, tw, st) $(e; var s; var out) => invoke(encode, e, s, samples, out)
    if (leg == LaneFormLeg.dev && discrete) {
        var regrown = ST()
        twin_regrow_dev_leg(k.regrow, fam, tw, regrown, samples, encode)
        delete regrown
    }
}

// A form's legs of the Canary encoder's twin on jfk.
[cold_path]
def private cn_twin_leg(t : T?; enc : CanaryEncoder; var k : EncTwinKept) {
    let samples <- asr_samples(t, "jfk.wav")
    return if (empty(samples))
    var st = CanaryState()
    enc_twin_leg(t, k, canary_form_of(k.form), TowerFamily.canary, enc, st, samples) $(e; var s; clip; var out) => cn_twin_encode(e, s, clip, out)
    delete st
}

// A form's leg of the Canary encoder's short clips: its q8 lane's CPU image, or that lane's image in the discrete home's flavor.
[cold_path]
def private cn_short_clip_leg(t : T?; enc : CanaryEncoder; var k : EncTwinKept) {
    t |> success(enc.q8, "{k.clips.tag}: the short clips ride the q8 lane")
    var st = CanaryState()
    short_clip_leg(k.clips, TowerFamily.canary, enc.dev_backend != TowerDev.none, enc, st) $(e; var s; clip; var out) => cn_twin_encode(e, s, clip, out)
    delete st
}

// A form's legs of a gemma4a encoder's twin on jfk; the q8 form's own width and projection kept.
[cold_path]
def private g4a_twin_leg(t : T?; enc : Gemma4aEncoder; var k : EncTwinKept) {
    let samples <- asr_samples(t, "jfk.wav")
    return if (empty(samples))
    let f = g4a_form_of(k.form)
    if (f.lane == "q8" && f.zero_block < 0l && !f.dev) {
        k.mid_dim = gemma4a_mid_dim(enc)
        k.proj_dim = enc.proj_dim
    }
    var st = Gemma4aState()
    enc_twin_leg(t, k, f, TowerFamily.gemma4a, enc, st, samples) $(e; var s; clip; var out) => gemma4a_encode_all(e, s, clip, out)
    delete st
}

// A form's leg of the E2B encoder's short clips: its q8 lane's CPU image, or that lane's image in the discrete home's flavor.
[cold_path]
def private g4a_short_clip_leg(t : T?; enc : Gemma4aEncoder; var k : EncTwinKept) {
    t |> success(enc.q8, "{k.clips.tag}: the short clips ride the q8 lane")
    var st = Gemma4aState()
    short_clip_leg(k.clips, TowerFamily.gemma4a, enc.dev_backend != TowerDev.none, enc, st) $(e; var s; clip; var out) => gemma4a_encode_all(e, s, clip, out)
    delete st
}

// One gemma4a encoder on the discrete home, after its mmproj's block: jfk three ways on the q8 lane with its regrown
// residencies and their poison, then its exact lane on the home's exact image.
[cold_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private test_gemma4a_discrete_twin(t : T?; var k : EncTwinKept) {
    return if (asr_clip_absent(t, "jfk.wav"))
    let jt = k.tag
    t |> success(!k.flags.x_q8 && k.flags.c_q8, "the exact and the q8 encoders minted")
    t |> equal(k.mid_dim, 1536l, "{jt}: the encoder's own width")
    t |> equal(k.proj_dim, k.want_proj, "{jt}: the embedder projects to the decoder's width")
    let n_layer = k.twin.rows.spec.n_layer
    let held = twin_rows_held(t, k.twin.rows) $(legs) {
        let n_chunks = legs.dev.encodes - legs.pre.encodes
        t |> equal(legs.n_cpu, legs.n_exact, "{jt}: the q8 chain's token count")
        t |> equal(legs.n_dev, legs.n_exact, "{jt}: the device chain's token count")
        t |> success(n_chunks >= 1l, "{jt}: the device served the chunk encodes ({n_chunks})")
        t |> equal(legs.dev.blocks - legs.pre.blocks, n_chunks * n_layer, "{jt}: every block of every served encode ran on the driver")
        front_served(t, jt, legs.dev.convs - legs.pre.convs, n_chunks, legs.dev.declines - legs.cpu.declines)
    }
    twin_regrow_held(t, jt, TowerFamily.gemma4a, false, k.regrow, k.twin.rows.exact, held.bar)
    k.exact.lane.npos = k.twin.rows.n_exact
    let xconvs = exact_flavor_held(t, k.exact)
    t |> success(xconvs >= 2l, "{jt} exact lane: the chunk front served on both device encodes ({xconvs} dispatches)")
}

// One gemma4a encoder on the Metal Conformer chain, after its mmproj's block: the q8 lane three ways with its poison,
// then the exact lane.
[cold_path, arch(at = "../ARCHITECTURE_GPU_TOWER.md#tower-encode-chains")]
def private test_gemma4a_metal_twin(t : T?; k : EncTwinKept) {
    return if (asr_clip_absent(t, "jfk.wav"))
    let jt = k.tag
    t |> success(!k.flags.x_q8 && k.flags.c_q8 && k.flags.d_q8 && k.flags.d_backend == TowerDev.metal, "{jt}: the exact and the q8 encoders minted, the q8 lane's Metal image beside")
    metal_q8_twin_held(t, k.twin)
    exact_flavor_held(t, k.exact)
}

// The Canary encoder on the discrete home, after its bin's block: the front and every block on the device within the
// three-way bar on jfk, held across the regrown residencies, a poison past it, and the exact lane within its bar.
[cold_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private test_canary_discrete_twin(t : T?; k : EncTwinKept) {
    return if (asr_clip_absent(t, "jfk.wav"))
    t |> success(!k.flags.x_q8 && k.flags.c_q8, "the exact and the q8 encoders minted")
    let n_layer = k.twin.rows.spec.n_layer
    let held = twin_rows_held(t, k.twin.rows) $(legs) {
        fc_discrete_counters(t, "canary jfk", n_layer, legs)
    }
    twin_regrow_held(t, "canary jfk", TowerFamily.canary, true, k.regrow, k.twin.rows.exact, held.bar)
    fc_exact_lane_held(t, k.exact)
}

// The Metal home's FastConformer rel-form twin of the Canary encoder on jfk, after its bin's block: the q8 lane three ways
// with its poison, then the exact lane.
[cold_path, arch(at = "../ARCHITECTURE_GPU_TOWER.md#tower-encode-chains")]
def private test_fc_metal_twin(t : T?; k : EncTwinKept) {
    return if (asr_clip_absent(t, "jfk.wav"))
    fc_metal_twin_held(t, "encoders", k.flags, k.twin, k.exact)
}

// A canary encode served on the Metal tower leaves no encoder rows held on the device - no transducer decodes them - on
// the exact lane's `f32@dev` form of a das_metal build.
[cold_path, unused_argument(t, enc)]
def private test_canary_metal_holds_no_rows(t : T?; enc : CanaryEncoder) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var inscope samples <- asr_samples(t, "jfk.wav")
        return if (empty(samples))
        with_dasllama_jobque_() {
            with_gpu_knobs() {
                set_metal_tower(true)
                var st = CanaryState()
                let e0 = metal_tower_stats()
                canary_log_mel(enc, samples, st)
                canary_encode(enc, st)
                t |> equal(metal_tower_stats().encodes - e0.encodes, 1l, "the encode served on the device")
                t |> equal(metal_tower_held_rows_elems(), 0l, "no rows held past it")
                delete st
                metal_tower_shutdown()
            }
        }
    }
}

let private Q3A = "mmproj-Qwen3-ASR-0.6B-bf16.gguf"

//! the Qwen3-ASR mmproj's staged read the key `key` names (`tower_stage_key_of`): under its lane pin, put back however the
//! staging leaves, in its image flavor
def private qwen3a_stage(path, key : string) : Qwen3aStaging {
    let f = qwen3a_form_of(key)
    return <- staged_twin_tower(path, f.lane == "q8", tower_form_dev_of(f, TowerFamily.qwen3a), @@set_qwen3a_q8, @@reset_qwen3a_q8,
        @@ < (path : string) : Qwen3aStaging > stage_qwen3a_tower)
}

//! qwen3a minted in memory from `stg`, a staged read on the lane `q8`; `zero_block` >= 0 zeroes `poison_blocks` blocks
//! from it on; `dev` the flavor
def private mint_q3a(var stg : Qwen3aStaging; q8 : bool; zero_block : int64; dev : TowerDev) : Qwen3aTower {
    if (zero_block >= 0l) {
        let nl = stg.e.core.n_layer
        for (b in range64(zero_block, min(nl, zero_block + poison_blocks(nl)))) {
            if (q8) {
                zero_plane(stg.core.qblob, stg.e.core.qlayers_off + b * stg.e.core.qlayer_stride, stg.e.core.qlayer_stride)
            } elif (stg.e.core.gemm_f16) {
                zero_plane(stg.core.wblob, stg.e.core.qlayers_off + b * stg.e.core.qlayer_stride, stg.e.core.qlayer_stride)
            } else {
                zero_plane(stg.core.fblob, stg.e.core.layers_off + b * stg.e.core.layer_stride, stg.e.core.layer_stride)
            }
        }
    }
    var tw = Qwen3aTower()
    mint_qwen3a_tower(stg, tw, dev)
    return <- tw
}

//! the Qwen3-ASR mmproj's form read off its name (`tower_form_of`): the lanes `f32` (the exact lane) and `q8`
def private qwen3a_form_of(form : string) : TowerForm => tower_form_of(form, ["f32", "q8"], "qwen3a")

//! the staged read a Qwen3-ASR mmproj's form is minted from (`tower_stage_key_of`)
def private qwen3a_stage_key(form : string) : string => tower_stage_key_of(qwen3a_form_of(form))

//! One form of the Qwen3-ASR mmproj, its block's form loader. `image_f32` is the tower's own load (`load_qwen3a_tower`) on
//! the exact lane's CPU image: the lane pinned and every GPU tower knob off for the load, put back however it leaves. Any
//! other form is minted in memory (`mint_q3a`) from `stg`, its own copy of the mmproj's staging on the lane it names:
//! `_z<block>` zeroes the poison's blocks from that block on, `@dev` is the image flavor of qwen3a's serving home.
def private qwen3a_form(path, form : string; var stg : Qwen3aStaging) : Qwen3aTower {
    let f = qwen3a_form_of(form)
    if (!f.image) {
        return <- mint_q3a(stg, f.lane == "q8", f.zero_block, tower_form_dev_of(f, TowerFamily.qwen3a))
    }
    if (f.lane != "f32") {
        panic("the qwen3a tests: '{form}' names no load of the image rail")
    }
    let knobs = gpu_knobs()
    set_every_gpu_tower(false)
    set_qwen3a_q8(false)
    var tw = Qwen3aTower()
    with_put_back($ {
        reset_qwen3a_q8()
        set_gpu_knobs(knobs)
    }) {
        tw <- load_qwen3a_tower(path)
    }
    return <- tw
}

//! The form is the lane, poison and image flavor it names: on `tw`, the Qwen3-ASR mmproj loaded as the form `form` of
//! `file`, the core's lane flag, the poison's block the tower's middle one, and the image flavor.
def private qwen3a_form_is(t : T?; file, form : string; tw : Qwen3aTower) {
    let f = qwen3a_form_of(form)
    minted_form_is(t, file, form, f, TowerFamily.qwen3a, tw.core.q8, tw.core.n_layer, twin_facts(tw).dev)
}

//! a qwen3a twin leg's two states over one tower
struct Q3aLegState {
    enc : EncoderState
    conv : Qwen3aConvState
}

def private q3a_leg_state(tw : Qwen3aTower) : Q3aLegState {
    var s = Q3aLegState()
    s.enc <- make_encoder_state(tw.core)
    return <- s
}

//! the device leg (the knob on) encodes the device mel's `dev_win`, the CPU legs the CPU mel's `win`
[cold_path]
def private q3a_twin_encode(tw : Qwen3aTower; var s : Q3aLegState; win, dev_win : MelChunk; var out : array<float>) : int64 {
    if (serving_gpu_tower(TowerFamily.qwen3a)) {
        qwen3a_encode(tw, s.enc, s.conv, dev_win)
    } else {
        qwen3a_encode(tw, s.enc, s.conv, win)
    }
    out := s.enc.out
    return long_length(out)
}

//! the Vulkan storage-range seam over the qwen3a mel: a range its scratch runs past declines `memory` and keeps no capacity, so the next mel rebuilds
[unused_argument(t, samples, n_mel, mel, dev_mel)]   // without dasVulkan the gated body is empty
def private q3a_mel_range_legs(t : T?; samples : array<float>; n_mel : int64; mel, dev_mel : WhisperMel) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return if (tower_family_cuda(TowerFamily.qwen3a))
        vulkan_tower_shutdown()
        let msr = g_gpu.msr
        let k0 = vulkan_tower_declines()?["memory"] ?? 0l
        g_gpu.msr = 4096l
        let cpu_mel <- log_mel_qwen3a(samples, n_mel)
        g_gpu.msr = msr
        t |> equal((vulkan_tower_declines()?["memory"] ?? 0l) - k0, 1l, "qwen3a jfk mel: the scratch past the range declined memory")
        check_exact(t, "qwen3a jfk mel: the declined mel is the CPU's", cpu_mel.data, mel.data)
        let m3 = vulkan_tower_stats()
        let re_mel <- log_mel_qwen3a(samples, n_mel)
        t |> equal(vulkan_tower_stats().mels - m3.mels, 1l, "qwen3a jfk mel: the range put back, the device mel served on a rebuilt scratch")
        check_exact(t, "qwen3a jfk mel: the rebuilt scratch's rows are the first device mel's", re_mel.data, dev_mel.data)
    }
}

//! One jfk window of the qwen3a twin as its forms keep it: the window's offset, its rows three ways (`TwinRows`), its conv
//! chunks on the device mel and the projector tails its clean legs served.
struct private Q3aWindowKept {
    off : int64
    rows : TwinRows = TwinRows()
    n_chunks : int64
    tail : int64
}

//! What the Qwen3-ASR mmproj's block keeps for the discrete twin compare that follows it: `form` the form whose cells run;
//! jfk's CPU and device log-mels, which the device form's mel cell keeps and every leg reads; the exact and the q8 forms'
//! lane flags; the twins of window 0 and of the tail window (`Q3aWindowKept`) and window 0's exact lane (`ExactFlavorKept`).
struct private Q3aKept {
    form : string
    mels : bool
    mel : WhisperMel
    dev_mel : WhisperMel
    x_q8 : bool
    c_q8 : bool
    c_cq8 : bool
    win0 : Q3aWindowKept = Q3aWindowKept()
    win1 : Q3aWindowKept = Q3aWindowKept(off = QWEN3A_WINDOW)
    exact : ExactFlavorKept
}

// jfk's log-mel on qwen3a's discrete home against the CPU's, both kept for every leg of the twin: the knob-off mel ran on
// the CPU, the device mel served with no decline within 1e-3 of it, and the storage-range seam (`q3a_mel_range_legs`).
[cold_path]
def private q3a_mel_leg(t : T?; tw : Qwen3aTower; var k : Q3aKept) {
    let samples <- load_audio_16k_mono(asr_clip("jfk.wav"))
    with_gpu_knobs() {
        set_serving_gpu_tower(false, TowerFamily.qwen3a)
        let m0 = gpu_tower_counters(TowerFamily.qwen3a)
        var mel <- log_mel_qwen3a(samples, tw.core.n_mel)
        let m1 = gpu_tower_counters(TowerFamily.qwen3a)
        t |> equal(m1.mels - m0.mels, 0l, "qwen3a jfk mel: the knob-off mel ran on the CPU (mels delta)")
        set_serving_gpu_tower(true, TowerFamily.qwen3a)
        var dev_mel <- log_mel_qwen3a(samples, tw.core.n_mel)
        let m2 = gpu_tower_counters(TowerFamily.qwen3a)
        t |> equal(m2.mels - m1.mels, 1l, "qwen3a jfk mel: the device mel served (mels delta)")
        t |> equal(m2.declines, m1.declines, "qwen3a jfk mel: the device mel served with no decline")
        t |> equal(dev_mel.n_len, mel.n_len, "qwen3a jfk mel: the device mel has the CPU mel's frames")
        let mel_maxd = logits_maxdiff(dev_mel.data, mel.data)
        t |> success(mel_maxd < 1e-3, "qwen3a jfk mel: the device log-mel matches the CPU's within 1e-3 (max |diff| {mel_maxd})")
        q3a_mel_range_legs(t, samples, tw.core.n_mel, mel, dev_mel)
        k.mel <- mel
        k.dev_mel <- dev_mel
    }
    k.mels = true
}

//! the window `w`'s twin named on a tower of `n_layer` blocks
def private q3a_window_named(var w : Q3aWindowKept; n_layer : int64) {
    w.rows.spec = TwinSpec(family = "qwen3a", tag = "qwen3a jfk mel+front+blocks at {w.off}", n_layer = n_layer, metric = TwinMetric.rel_l2,
        seat_family = TowerFamily.qwen3a)
}

//! the leg `leg` of the window `w` over the form's tower `tw` and its state `s`: the window cut from jfk's kept mels, the rows
//! and counters kept (`twin_rows_leg`), the projector tails the leg served
def private q3a_window_leg(var w : Q3aWindowKept; leg : TwinLeg; tw : Qwen3aTower; var s : Q3aLegState; mel, dev_mel : WhisperMel) {
    let win <- qwen3a_mel_window(mel, w.off)
    let dev_win <- qwen3a_mel_window(dev_mel, w.off)
    w.n_chunks = dev_win.n_len / QWEN3A_CHUNK
    q3a_window_named(w, tw.core.n_layer)
    let tail0 = gpu_tower_tail_encodes(TowerFamily.qwen3a)
    twin_rows_leg(w.rows, leg, tw, s) $(e; var st; var out) => q3a_twin_encode(e, st, win, dev_win, out)
    w.tail += gpu_tower_tail_encodes(TowerFamily.qwen3a) - tail0
}

// A form's legs of the qwen3a twin on jfk, each on the form's one home: the exact form the reference chain on both windows
// and window 0's exact lane CPU leg, the q8 form the CPU q8 chain, its `@dev` form the device chain, the zeroed q8 `@dev`
// form window 0's input poison, the exact lane's `@dev` form and its poison window 0's exact lane device legs.
[cold_path]
def private q3a_twin_leg(t : T?; tw : Qwen3aTower; var k : Q3aKept) {
    if (!k.mels) {
        t |> skip("the device form's mel cell kept no jfk mel")
        return
    }
    let f = qwen3a_form_of(k.form)
    exact_flavor_named(k.exact, "qwen3a jfk mel+front+blocks at 0", TowerFamily.qwen3a, TWIN_EXACT_REL_L2_VK, -1l)
    var s <- q3a_leg_state(tw)
    let win <- qwen3a_mel_window(k.mel, 0l)
    let dev_win <- qwen3a_mel_window(k.dev_mel, 0l)
    if (f.lane == "f32" && f.zero_block >= 0l) {
        exact_flavor_poison_leg(t, k.exact, tw, s) $(e; var st; var out) => q3a_twin_encode(e, st, win, dev_win, out)
    } elif (f.lane == "f32" && f.dev) {
        exact_flavor_dev_leg(k.exact, tw, s) $(e; var st; var out) => q3a_twin_encode(e, st, win, dev_win, out)
    } elif (f.zero_block >= 0l) {
        q3a_window_named(k.win0, tw.core.n_layer)
        twin_rows_poison_leg(t, k.win0.rows, f.zero_block, tw, s) $(e; var st; var out) => q3a_twin_encode(e, st, win, dev_win, out)
    } else {
        let leg = f.lane == "f32" ? TwinLeg.exact : (f.dev ? TwinLeg.dev : TwinLeg.cpu)
        q3a_window_leg(k.win0, leg, tw, s, k.mel, k.dev_mel)
        q3a_window_leg(k.win1, leg, tw, s, k.mel, k.dev_mel)
        if (leg == TwinLeg.exact) {
            k.x_q8 = tw.core.q8
            exact_flavor_cpu_leg(k.exact, tw, s) $(e; var st; var out) => q3a_twin_encode(e, st, win, dev_win, out)
        } elif (leg == TwinLeg.cpu) {
            k.c_q8 = tw.core.q8
            k.c_cq8 = tw.cq8
        }
    }
    delete s
}

//! one window's twin held, after the block: the front and the block loop served the window, the device chain within the
//! three-way bar (`twin_rows_held`, its poison where the window took one), the projector behind the device leg alone
def private q3a_window_held(t : T?; w : Q3aWindowKept) {
    let tag = w.rows.spec.tag
    let n_chunks = w.n_chunks
    twin_rows_held(t, w.rows) $(legs) {
        if (legs.dev.convs == legs.pre.convs) {
            to_log(LOG_INFO, "{tag}: the driver declined - {gpu_tower_declines(TowerFamily.qwen3a)}\n")
        }
        front_served(t, tag, legs.dev.convs - legs.pre.convs, n_chunks, legs.dev.declines - legs.cpu.declines)
        t |> equal(legs.dev.encodes - legs.pre.encodes, 1l, "{tag}: the block loop served the window")
    }
    t |> equal(w.tail, 1l, "{tag}: the projector ran on the device behind the device leg's blocks and on neither knob-off leg (tail encodes delta)")
}

// Qwen3-ASR on the discrete home, after its mmproj's block: front, blocks and projector on windows 0 and the tail within
// the three-way bar on jfk, window 0's poison past it, and window 0's exact lane within its bar.
[cold_path]
def private test_qwen3a_vulkan_front(t : T?; var k : Q3aKept) {
    t |> success(!k.x_q8 && k.c_q8 && k.c_cq8, "qwen3a jfk: the exact and the q8 towers minted, the cblob trio on the q8 lane")
    q3a_window_held(t, k.win0)
    q3a_window_held(t, k.win1)
    let tag = "qwen3a jfk mel+front+blocks at 0"
    let n_chunks = k.win0.n_chunks
    k.exact.lane.npos = n_chunks * QWEN3A_TOK_PER_CHUNK
    let convs = exact_flavor_held(t, k.exact)
    t |> equal(convs, 2l * n_chunks, "{tag} exact lane: the front served every chunk of both device encodes")
}

// The Qwen3-ASR mmproj's tower, one block: the CPU oracle on its exact lane's image, the discrete twin's forms minted - the
// device forms first, the first of them keeping jfk's two mels.
[test(description = "mmproj-Qwen3-ASR-0.6B-bf16.gguf loads its qwen3a tower once a form. On the CPU f32 rail it loads its shape and encodes jfk window 0 to a first token within 5e-3 of llama-mtmd's embeddings on its first four values. On the discrete home it computes the jfk log-mel within 1e-3 of the CPU mel, then serves front, blocks and projector on windows 0 and the tail within the three-way bar, a poison and the exact lane holding theirs")]
def test_qwen3a_carrier(t : T?) {
    var inscope kept = Q3aKept()
    let twin_why = jfk_twin_why(TowerFamily.qwen3a)
    let discrete_on = empty(twin_why)
    if (!discrete_on) {
        skip_cell(t, "qwen3a: the discrete home's mel + conv front + blocks vs the CPU q8 chain on jfk windows 0 and the tail - {twin_why}")
    }
    var inscope forms <- ["image_f32"]
    add_served_forms(t, forms, Q3A, ["q8@dev", "q8_z9@dev", "f32@dev", "f32_z9@dev", "f32", "q8"], twin_why)
    let ran = with_model_once(t, "audio", "mmproj-Qwen3-ASR-0.6B-bf16.gguf", forms, @@qwen3a_stage_key, @@qwen3a_stage, @@qwen3a_form) $(var tw; form) {
        qwen3a_form_is(t, Q3A, form, tw)
        kept.form = form
        if (form == "image_f32") {
            tower_cell(t, "qwen3a tower: conv2d frontend + windowed encode matches mtmd on jfk", tw, @@test_qwen3a_tower)
            return
        }
        if (form == "q8@dev") {
            tower_cell(t, "{Q3A}: jfk's log-mel on the discrete home's mel seat against the CPU's, the {form} form's", tw, kept, @@q3a_mel_leg)
        }
        tower_cell(t, "{Q3A}: the discrete twin, the {form} form's legs", tw, kept, @@q3a_twin_leg)
    }
    if (ran && discrete_on) {
        kept_cell(t, "qwen3a: the discrete home's mel + conv front + blocks vs the CPU q8 chain on jfk windows 0 and the tail", kept, @@test_qwen3a_vulkan_front)
        gpu_tower_shutdown(TowerFamily.qwen3a)
    }
}

//! The chat towers' chunked mel on the device seat against the CPU spectrum: a 128-mel clip's chunk within 1e-3 of the CPU
//! mel, the mel counter up by the device leg alone, an 80-mel call (off the seat's lattice) served by the CPU with the
//! counter unmoved, and a second clip's mel outside the bar (the control).
[test(description = "The chunked 128-mel log-mel on the Metal mel seat lands within 1e-3 of the CPU mel with one device mel counted, an 80-mel call falls to the CPU, and another clip mel lands outside the bar")]
def test_log_mel_chunks_gpu(t : T?) {
    t |> run("the chunked whisper mel rides the device mel seat") @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!jit_only(t))
            var inscope samples <- [for (i in range(16000 * 5)); 0.3 * sin(float(i) * 0.05) + 0.1 * sin(float(i) * 0.31)]
            var inscope other <- [for (i in range(16000 * 5)); 0.3 * sin(float(i) * 0.11)]
            let was = metal_tower_knob()
            with_dasllama_jobque_() {
                set_metal_tower(false)
                let m0 = metal_tower_mel_encodes()
                var inscope cpu <- log_mel_chunks(samples, 128l)
                t |> equal(metal_tower_mel_encodes() - m0, 0l, "tower off: the CPU spectrum")
                set_metal_tower(true)
                var inscope dev <- log_mel_chunks(samples, 128l)
                t |> equal(metal_tower_mel_encodes() - m0, 1l, "tower on: one device mel a clip")
                var inscope dev2 <- log_mel_chunks(other, 128l)
                var inscope m80 <- log_mel_chunks(samples, 80l)
                t |> equal(metal_tower_mel_encodes() - m0, 2l, "an 80-mel call is off the seat's lattice: the CPU serves it")
                t |> success(length(cpu) == 1 && length(dev) == 1 && length(m80) == 1, "one chunk each")
                if (length(cpu) == 1 && length(dev) == 1 && length(dev2) == 1) {
                    let d = logits_maxdiff(cpu[0].data, dev[0].data)
                    let ctl = logits_maxdiff(cpu[0].data, dev2[0].data)
                    t |> success(d <= 1e-3, "device mel within 1e-3 of the CPU mel (max diff {d})")
                    t |> success(ctl > 1e-3, "another clip's mel lands outside the bar (max diff {ctl}) - the control")
                }
            }
            set_metal_tower(was)
        }
    }
}

// The Qwen2.5-Omni 3B f32 mmproj's audio side on its exact lane's CPU image: a qwen2a tower, the vision tensors skipped.
[cold_path]
def private test_omni_tower(t : T?; tw : AudioTower) {
    // loading at all proves the v.* tensors are skipped
    t |> success(tw.proj_kind == AudioProjKind.qwen2a, "kind aliases qwen2a")
    t |> equal(tw.proj_dim, 2048l, "proj_dim (3B decoder)")
    t |> equal(tw.n_layer, 32l, "layers")
    t |> equal(audio_tokens_per_chunk(tw), 750l, "tokens per 30 s chunk")
}

// The qwen2audio f32 tower on its exact lane's CPU image: the pinned shape, and the CPU encode of an all-ones mel against
// llama-mtmd-debug.
[cold_path]
def private test_encoder_oracle(t : T?; tw : AudioTower) {
    return if (!jit_only(t))
    t |> equal(tw.n_mel, 128l, "n_mel")
    t |> equal(tw.d_model, 1280l, "d_model")
    t |> equal(tw.n_head, 20l, "heads")
    t |> equal(tw.n_layer, 32l, "layers")
    t |> equal(tw.n_ff, 5120l, "ffn")
    t |> equal(tw.n_ctx, 1500l, "positions")
    t |> equal(tw.proj_dim, 4096l, "projection")
    var mel = MelChunk(n_mel = tw.n_mel, n_len = AUDIO_CHUNK_FRAMES)
    mel.data |> resize(tw.n_mel * AUDIO_CHUNK_FRAMES)
    for (i in range(length(mel.data))) {
        mel.data[i] = 1.0
    }
    var enc <- make_encoder_state(tw)
    with_gpu_knobs() {
        set_every_gpu_tower(false)
        with_dasllama_jobque_() {
            audio_encode(tw, enc, mel)
        }
    }
    // llama-mtmd-debug -p encode --audio one -n 3000 (CPU): token 0 opens
    // 0.251974 0.311756 0.556828 0.005232 -0.838854; stats mean=0.009261 max=7.527778.
    // fp32 CPU re-orders put us ~1e-3 off their flash-attn encoder; gate at 5e-3.
    t |> success(abs(enc.out[0] - 0.251974) < 5e-3, "tok0[0]")
    t |> success(abs(enc.out[1] - 0.311756) < 5e-3, "tok0[1]")
    t |> success(abs(enc.out[2] - 0.556828) < 5e-3, "tok0[2]")
    t |> success(abs(enc.out[4] + 0.838854) < 5e-3, "tok0[4]")
    var sum = 0.0lf
    var mx = enc.out[0]
    for (v in enc.out) {
        sum += double(v)
        mx = max(mx, v)
    }
    let mean = sum / double(750l * tw.proj_dim)
    t |> success(abs(float(mean) - 0.009261) < 5e-4, "mean")
    t |> success(abs(mx - 7.527778) < 5e-2, "max")
    delete enc
}

// The Qwen2-Audio mmproj's tower, one block.
[test(description = "qwen2audio-mmproj-f32.gguf loads its tower once a form. Its exact lane's CPU image has the pinned shape, and its CPU encode of an all-ones mel matches the llama-mtmd-debug token 0 values within 5e-3 and the mean and max of the output. On Metal the tower runs all 32 blocks and the projector tail on the q8 slab within the three-way bar on a synthetic mel, a poison exceeds it, and the f16 exact lane holds; on the discrete home all 32 blocks and the projector tail run on the device within the three-way bar, a poison exceeds it, and the exact lane holds")]
def test_qwen2audio_carrier(t : T?) {
    var inscope kept = AudTwinKept(family = "qwen2audio", kind = AudioProjKind.qwen2a, layers = 32l,
        metal_cell = "qwen2audio tower: the Metal chain vs the CPU encoder, 32 layers",
        discrete_cell = "qwen2audio tower: the discrete home's blocksvs the CPU q8 chain on the synthetic mel, 32 layers")
    var inscope forms <- ["image_f32"]
    let homes = aud_twin_asked(t, forms, kept, "qwen2audio-mmproj-f32.gguf", true, true)
    let ran = with_model_once(t, "audio", "qwen2audio-mmproj-f32.gguf", forms, @@aud_stage_key, @@aud_stage, @@aud_form) $(var h; form) {
        aud_form_is(t, "qwen2audio-mmproj-f32.gguf", form, h)
        kept.form = form
        if (form == "image_f32") {
            tower_cell(t, "all-ones mel through the tower matches mtmd (needs qwen2audio mmproj)", h.tw, @@test_encoder_oracle)
            return
        }
        aud_twin_cells(t, "qwen2audio-mmproj-f32.gguf", form, h.tw, kept, homes)
    }
    if (ran) {
        aud_twin_held(t, kept, homes)
    }
}

// The omni-3b f32 mmproj's audio tower, one block; its Metal twin reads the Qwen2.5-Omni 3B f16 mmproj's.
[test(description = "mmproj-omni-3b-f32.gguf loads its audio tower once a form. Its exact lane's CPU image is a qwen2a tower (proj_dim 2048, 32 layers, 750 tokens a chunk) with the vision tensors skipped. On the discrete home every block and the projector tail run on the device within the three-way bar on a synthetic mel, a poison exceeds it, and the exact lane holds")]
def test_omni3b_carrier(t : T?) {
    var inscope kept = AudTwinKept(family = "omni-3b", kind = AudioProjKind.qwen2a,
        discrete_cell = "omni-3b audio tower: the discrete home's blocksvs the CPU q8 chain on the synthetic mel")
    var inscope forms <- ["image_f32"]
    let homes = aud_twin_asked(t, forms, kept, "mmproj-omni-3b-f32.gguf", false, true)
    let ran = with_model_once(t, "audio", "mmproj-omni-3b-f32.gguf", forms, @@aud_stage_key, @@aud_stage, @@aud_form) $(var h; form) {
        aud_form_is(t, "mmproj-omni-3b-f32.gguf", form, h)
        kept.form = form
        if (form == "image_f32") {
            tower_cell(t, "qwen2.5o mmproj: audio side is qwen2a verbatim, vision tensors skipped", h.tw, @@test_omni_tower)
            return
        }
        aud_twin_cells(t, "mmproj-omni-3b-f32.gguf", form, h.tw, kept, homes)
    }
    if (ran) {
        aud_twin_held(t, kept, homes)
    }
}

// The Qwen2.5-Omni 3B f16 mmproj's audio tower, one block; its discrete twin reads the omni-3b f32 mmproj's.
[test(description = "mmproj-Qwen2.5-Omni-3B-f16.gguf loads its audio tower once a form. The audio carrier's load takes its audio side as the whisper-class tower beside its vision tensors at the probed width 2048, encodes finite rows a chunk (750) deterministically on a reused state without heap growth, another clip's rows apart. On Metal the tower runs every block and the projector tail on the q8 slab within the three-way bar on a synthetic mel, a poison exceeds it, and the f16 exact lane holds")]
def test_qwen25_omni_carrier(t : T?) {
    var inscope kept = AudTwinKept(family = "mmproj-Qwen2.5-Omni-3B-f16.gguf", kind = AudioProjKind.qwen2a,
        metal_cell = "omni-3b tower: the Metal chain vs the CPU encoder - pair pool, post-norm, one biased linear")
    var inscope forms <- ["image_emb"]
    let homes = aud_twin_asked(t, forms, kept, "mmproj-Qwen2.5-Omni-3B-f16.gguf", true, false)
    let ran = with_model_once(t, "audio", "mmproj-Qwen2.5-Omni-3B-f16.gguf", forms, @@aud_stage_key, @@aud_stage, @@aud_form) $(var h; form) {
        aud_form_is(t, "mmproj-Qwen2.5-Omni-3B-f16.gguf", form, h)
        kept.form = form
        if (form == "image_emb") {
            tower_cell(t, "the Qwen2.5-Omni mmproj's audio side loads as the whisper-class tower beside its vision tensors", h.e, @@embedder_omni_arm)
            return
        }
        aud_twin_cells(t, "mmproj-Qwen2.5-Omni-3B-f16.gguf", form, h.tw, kept, homes)
    }
    if (ran) {
        aud_twin_held(t, kept, homes)
    }
}

// The ultravox v0_5 f16 mmproj's audio tower, one block.
[test(description = "mmproj-ultravox-v0_5-llama-3_2-1b-f16.gguf loads its audio tower once a form. The audio carrier's load holds the whisper-class tower at the probed width 2048, encodes finite rows a chunk (187) deterministically on a reused state without heap growth, another clip's rows apart. On Metal the tower runs every block - post-norm, stacked rms - and the gated-MLP tail on the q8 slab within the three-way bar on a synthetic mel, a poison exceeds it, and the f16 exact lane holds; on the discrete home every block and the gated-MLP tail run on the device within the three-way bar, a poison exceeds it, and the exact lane holds")]
def test_ultravox_v0_5_carrier(t : T?) {
    var inscope kept = AudTwinKept(family = "ultravox", kind = AudioProjKind.ultravox,
        metal_cell = "ultravox tower: the Metal chain vs the CPU encoder - post-norm, stacked rms, gated MLP",
        discrete_cell = "ultravox audio tower: the discrete home's blocksand the gated-MLP tail vs the CPU q8 chain on the synthetic mel")
    var inscope forms <- ["image_emb"]
    let homes = aud_twin_asked(t, forms, kept, "mmproj-ultravox-v0_5-llama-3_2-1b-f16.gguf", true, true)
    let ran = with_model_once(t, "audio", "mmproj-ultravox-v0_5-llama-3_2-1b-f16.gguf", forms, @@aud_stage_key, @@aud_stage, @@aud_form) $(var h; form) {
        aud_form_is(t, "mmproj-ultravox-v0_5-llama-3_2-1b-f16.gguf", form, h)
        kept.form = form
        if (form == "image_emb") {
            tower_cell(t, "an ultravox mmproj loads as the whisper-class tower", h.e, @@embedder_ultravox_arm)
            return
        }
        aud_twin_cells(t, "mmproj-ultravox-v0_5-llama-3_2-1b-f16.gguf", form, h.tw, kept, homes)
    }
    if (ran) {
        aud_twin_held(t, kept, homes)
    }
}

// Model-free: encoder-state sizing on a whole-clip tower — tt = the clip's frames, so at
// minutes scale the tt- and 4·tt·d-sized buffers cross the 64 MB max_unreserved_size guard
// and make_encoder_state must reserve each exact size (a bare resize panics here).
[test(description = "make_encoder_state for a 16384-frame whole-clip tower reserves x, hp and h at their exact sizes past the max_unreserved_size guard without panicking")]
def test_encoder_state_sizing(t : T?) {
    t |> run("whole-clip encoder state clears max_unreserved_size") @(t : T?) {
        return if (!jit_only(t))
        let tw = AudioTower(n_mel = 128l, d_model = 1088l, n_head = 8l, n_layer = 1l,
            n_ff = 1024l, n_ctx = 16384l, proj_dim = 0l,
            conv1_w_off = 0l, conv1_b_off = 0l, conv2_w_off = 0l, conv2_b_off = 0l,
            pos_off = 0l, layers_off = 0l, layer_stride = 0l, lnp_w_off = 0l, lnp_b_off = 0l,
            proj_w_off = 0l, proj_b_off = 0l)
        var s <- make_encoder_state(tw)
        t |> equal(long_length(s.x), tw.n_ctx * tw.d_model)
        t |> equal(long_length(s.hp), 4l * tw.n_ctx * tw.d_model)
        t |> equal(long_length(s.h), tw.n_ctx * tw.n_ff)
        delete s
    }
}

let private PK_DEVICE_PLOG_BAR = 1e-4   //!< the device decode's plog off the CPU's: float log-sums over a recurrence of several hundred tokens, 2.3e-5 measured on gb1

// The device legs of a Parakeet bin's device-decode twin, on a lane's `@dev` form - `m`, the lane's GPU-flavored model
// (`#tower-dev-block`): each clip's encode with the serving tower pinned on, then the TDT decode on the build's device
// seat, counted once there, projected on the device off the encode's device rows where the lane's home projects it. The
// encode's rows are kept for the CPU form's legs.
[cold_path]
def private pk_decode_dev_legs(t : T?; m : ParakeetModel; var k : PkKept) {
    let q8 = m.q8
    let home = serving_tower_dev(TowerFamily.parakeet)
    var dev = TowerDev.none
    with_gpu_knobs() {
        set_serving_gpu_tower(true, TowerFamily.parakeet)
        dev = parakeet_dev_flavor(q8)
    }
    t |> equal(dev, home, "parakeet {k.tag}: the serving tower flavors the lane its chain serves")
    return if (dev == TowerDev.none)
    let projected = m.q8 && !m.jenc_exact ? 0l : 1l
    for (clip in ["jfk", "gb1"]) {
        let wav = asr_clip("{clip}.wav")
        if (!stat(wav).is_valid) {
            t |> skip("{clip}.wav not present")
            continue
        }
        let tag = "parakeet {k.tag} {q8 ? "q8" : "exact"} {clip}"
        continue if (rnnt_seat_skip(t, tag))
        var inscope samples <- load_audio_16k_mono(wav)
        var d = RnntDecodeTwin(tag = tag, q8 = q8, cpu_q8 = q8 && !m.jenc_exact)
        with_gpu_knobs() {
            set_serving_gpu_tower(true, TowerFamily.parakeet)
            var st = ParakeetState()
            parakeet_log_mel(samples, m, st)
            d.tt = parakeet_encode(m, st)
            d.rows := st.x
            rnnt_decode_dev_leg(t, "test_audio: {tag}", projected, projected) {
                parakeet_tdt_decode(m, st, d.tt)
            }
            d.dev := st.toks
            d.dev_text = parakeet_detokenize(m, d.dev)
            delete st
        }
        d.legs = 1
        k.decode |> emplace(d)
    }
}

// The CPU legs of the device-decode twin, on a bin's CPU form `mc` - the image whose projection the device runs: the
// device form's kept rows decoded on the CPU loop with the seat pinned off (the decode counter must not move, so a knob
// that pins nothing reds), then at 1.05x - the control.
[cold_path]
def private pk_decode_cpu_legs(t : T?; mc : ParakeetModel; var k : PkKept) {
    for (d in k.decode) {
        continue if (d.cpu_q8 != mc.q8 || d.legs != 1)
        with_gpu_knobs() {
            set_serving_gpu_tower(true, TowerFamily.parakeet)
            var stc = ParakeetState()
            rnnt_decode_cpu_legs(t, "test_audio: {d.tag}", d) $(rows; tt; var toks) {
                stc.x := rows
                parakeet_tdt_decode(mc, stc, tt)
                toks := stc.toks
            }
            d.cpu_text = parakeet_detokenize(mc, d.cpu)
            delete stc
        }
    }
}

// A form's legs of a Parakeet bin's device-decode twin: the device legs on a `@dev` form, the CPU legs on a CPU form.
[cold_path]
def private pk_decode_legs(t : T?; m : ParakeetModel; var k : PkKept) {
    with_dasllama_jobque_() {
        if (parakeet_form_of(k.form).dev) {
            pk_decode_dev_legs(t, m, k)
        } else {
            pk_decode_cpu_legs(t, m, k)
        }
    }
}

// The TDT decode on the build's device seat against the CPU decoder's, after the bin's block, the twins of the lane
// `k.form` names (`exact`, `q8`): the same tokens, the plog within `PK_DEVICE_PLOG_BAR`, the 1.05x control's plogs
// outside it (`rnnt_twin_held`); both decodes' transcripts logged.
[cold_path]
def private test_parakeet_device_decode_twin(t : T?; k : PkKept) {
    for (d in k.decode) {
        continue if (d.q8 != (k.form == "q8"))
        rnnt_decode_twin_held(t, "test_audio: {d.tag}", d, PK_DEVICE_PLOG_BAR)
    }
}

//! the device-decode twin's compares of a bin, a cell a lane, after its block
def private pk_decode_held(t : T?; var kept : PkKept) {
    for (lane in ["exact", "q8"]) {
        kept.form = lane
        kept_cell(t, "parakeet {kept.tag} {lane}: the TDT decode on the device seat is the CPU decoder's", kept, @@test_parakeet_device_decode_twin)
    }
}

// The Parakeet-TDT v2 f32 bin, one block.
[test(description = "ggml-parakeet-tdt-0.6b-v2-f32.bin loads once a form. Greedy on jfk.wav through the facade emits parakeet-cli's 33 tokens and text, a reused session and a flat heap hold, the other weight lane and the one-lane decode step give the same token ids. The TDT decode on the device seat, both lanes, matches the CPU decoder's tokens with log-probs within PK_DEVICE_PLOG_BAR on jfk and gb1, and encoder rows scaled 1.05x move them past it. On the discrete home clips of 320 to 1600 samples serve as one device encode with no decline within SHORT_CLIP_BAR of the CPU image's rows, a noise clip outside it; the front and every block serve on the device within the three-way bar on jfk, the CPU chain is refused past the storage range, and a poison and the exact lane hold their bars")]
def test_parakeet_carrier_v2(t : T?) {
    return if (!family_on(t, "parakeet"))
    var inscope kept = PkKept(tag = "v2", twin_tag = "parakeet jfk", exact_lane = true, range_refusal = true)
    kept.clips.tag = "parakeet"
    let discrete_why = audio_discrete_form_why(TowerFamily.parakeet)
    let decode_why = device_up_why(TowerFamily.parakeet)
    let discrete_on = empty(discrete_why)
    let decode_on = empty(decode_why)
    let either = either_why(decode_why, discrete_why)
    var inscope forms <- ["image"]
    add_served_forms(t, forms, PK_V2, ["q8@dev"], either)
    add_served_forms(t, forms, PK_V2, ["q8_z12@dev"], discrete_why)
    add_served_forms(t, forms, PK_V2, ["f32@dev"], either)
    add_served_forms(t, forms, PK_V2, ["f32_z12@dev"], discrete_why)
    add_served_forms(t, forms, PK_V2, ["q8"], either)
    forms |> push("f32")
    let ran = with_model_once_at(t, "audio", "ggml-parakeet-tdt-0.6b-v2-f32.bin", forms, whisper_dir(), @@parakeet_stage_key, @@parakeet_stage, @@parakeet_form) $(var m; form) {
        parakeet_form_is(t, PK_V2, form, m)
        kept.form = form
        let f = parakeet_form_of(form)
        if (form == "image") {
            tower_cell(t, "parakeet-TDT greedy on jfk.wav is token-for-token with parakeet-cli", m, kept.oracle, @@test_parakeet_oracle)
            return
        }
        if (form == "f32") {
            tower_cell(t, "parakeet-TDT v2: jfk's tokens on the other weight lane", m, kept.oracle, @@parakeet_other_lane_leg)
        }
        if (discrete_on) {
            tower_cell(t, "{PK_V2}: the discrete twin, the {form} form's legs", m.pk, kept, @@pk_twin_leg)
            if (f.lane == "q8" && f.zero_block < 0l) {
                tower_cell(t, "{PK_V2}: the short clips, the {form} form's leg", m.pk, kept, @@pk_short_clip_leg)
            }
        }
        if (decode_on && f.zero_block < 0l) {
            tower_cell(t, "parakeet v2: the TDT decode twin, the {form} form's legs", m.pk, kept, @@pk_decode_legs)
        }
    }
    return if (!ran)
    kept_cell(t, "parakeet-TDT v2: the other weight lane reads jfk to the facade load's token ids", kept.oracle, @@parakeet_lanes_held)
    if (decode_on) {
        pk_decode_held(t, kept)
    }
    if (discrete_on) {
        kept_cell(t, "parakeet-TDT 0.6B v2: clips of 320 to 1600 samples serve on the discrete home's image as the CPU image reads them", kept.clips, @@test_vulkan_short_clips)
        kept_cell(t, "parakeet-TDT 0.6B v2: the discrete home's FastConformerfront and blocks vs the CPU q8 chain on jfk, the exact chain the reference", kept, @@test_parakeet_discrete_twin)
    }
    gpu_tower_shutdown(TowerFamily.parakeet)
}

// The Parakeet-TDT v3 f32 bin, one block, then the q8_0 bin and the F32 GGUF of the same model, which read what it kept.
[test(description = "ggml-parakeet-tdt-0.6b-v3-f32.bin loads once a form. Through the facade it loads as a 25-language 8192-vocab model on the q8 lane and greedy on jfk.wav emits parakeet-cli's 38 tokens and exact text, reporting no detected language. The TDT decode on the device seat, both lanes, matches the CPU decoder's tokens with log-probs within PK_DEVICE_PLOG_BAR on jfk and gb1, and encoder rows scaled 1.05x move them past it. On the Metal FastConformer chain every block serves on the q8 slab within the three-way bar on jfk with no quant_mode decline, a poison exceeds it, and the exact f16 lane holds its bar. The v3 q8_0 bin loads through the reader the whisper bins share and transcribes jfk to exactly the text the f32 bin produces. The F32 GGUF stages the f32 bin's planes, encodes jfk to the same rows with the bin's rows at half volume outside the bar, and transcribes jfk as the f32 bin does, at jfk's word floor")]
def test_parakeet_carrier_v3(t : T?) {
    return if (!family_on(t, "parakeet"))
    var inscope kept = PkKept(tag = "v3", twin_tag = "parakeet metal jfk", exact_lane = true)
    let metal_why = metal_twin_seat_why(TowerFamily.parakeet)
    let decode_why = device_up_why(TowerFamily.parakeet)
    let metal_on = empty(metal_why)
    let decode_on = empty(decode_why)
    var inscope forms <- ["image"]
    add_served_forms(t, forms, PK_V3, ["q8@dev"], decode_why)
    add_served_forms(t, forms, PK_V3, ["q8_z12@dev"], metal_why)
    add_served_forms(t, forms, PK_V3, ["f32@dev"], decode_why)
    add_served_forms(t, forms, PK_V3, ["f32_z12@dev"], metal_why)
    add_served_forms(t, forms, PK_V3, ["q8"], decode_why)
    forms |> push("f32")
    let ran = with_model_once_at(t, "audio", "ggml-parakeet-tdt-0.6b-v3-f32.bin", forms, whisper_dir(), @@parakeet_stage_key, @@parakeet_stage, @@parakeet_form) $(var m; form) {
        parakeet_form_is(t, PK_V3, form, m)
        kept.form = form
        let f = parakeet_form_of(form)
        if (form == "image") {
            tower_cell(t, "parakeet-TDT v3 (25-lang) greedy on jfk.wav is token-for-token with parakeet-cli", m, @@test_parakeet_v3_oracle)
            return
        }
        if (form == "f32") {
            tower_cell(t, "parakeet-TDT 0.6B v3: the f32 ggml bin's rows and transcript on jfk, the F32 GGUF's reference", m.pk, kept, @@pk_bin_carrier_leg)
            tower_cell(t, "{PK_V3}: jfk on the exact lane, the q8_0 bin's reference text", m, kept.text, @@parakeet_f32_text_leg)
        }
        if (metal_on) {
            tower_cell(t, "{PK_V3}: the Metal twin, the {form} form's legs", m.pk, kept, @@pk_twin_leg)
        }
        if (decode_on && f.zero_block < 0l) {
            tower_cell(t, "parakeet v3: the TDT decode twin, the {form} form's legs", m.pk, kept, @@pk_decode_legs)
        }
    }
    if (ran && decode_on) {
        pk_decode_held(t, kept)
    }
    if (ran && metal_on) {
        kept_cell(t, "parakeet-TDT 0.6B v3: the Metal FastConformer chain's q8 and f16 lanes vs the CPU chains on jfk, the exact chain the reference", kept, @@pk_fc_metal_twin)
    }
    if (ran) {
        serving_tower_shutdown(TowerFamily.parakeet)
    }
    with_model_once_at(t, "audio", "ggml-parakeet-tdt-0.6b-v3-q8_0.bin", ["f32"], whisper_dir(), @@parakeet_stage_key, @@parakeet_stage, @@parakeet_form) $(var m8; form8) {
        parakeet_form_is(t, PK_V3_Q8_0, form8, m8)
        tower_cell(t, "ggml-parakeet-tdt-0.6b-v3-q8_0.bin: a q8_0 bin loads and reads jfk as the f32 bin does", m8, kept.text, @@test_parakeet_q8_0_file)
    }
    with_model_once(t, "audio", "parakeet-tdt-0.6b-v3-F32.gguf", ["f32"], @@parakeet_stage_key, @@parakeet_stage, @@parakeet_form) $(var mg; formg) {
        parakeet_form_is(t, PK_F32, formg, mg)
        tower_cell(t, "parakeet-TDT 0.6B v3: the F32 GGUF against the f32 ggml bin", mg.pk, kept, @@test_parakeet_gguf_carrier)
        tower_cell(t, "the decoder half: transcripts on jfk", mg.pk, kept, @@pk_gguf_bin_text)
    }
}

// The Parakeet-TDT v3 Q4_K_M GGUF, one block, then the Q8_0 GGUF.
[test(description = "parakeet-tdt-0.6b-v3-Q4_K_M.gguf loads once a form. Its kq lane, named k4, holds its CPU rows on jfk within 2.5x the q8 lane's distance from the dequantized chain with a zeroed half outside it, and transcribes jfk at the word floor. On the discrete home it serves its front and every block from the kq lane's device planes within the three-way bar on jfk, a k4 tile counted, and zeroed seats exceed the bar; on the Metal FastConformer chain from the kq lane's slabs with no quant_mode decline, a K-quant mul_mm stamp counted, and zeroed seats exceed the bar. The Q8_0 GGUF on the kq lane names it q8")]
def test_parakeet_carrier_q4_k_m(t : T?) {
    return if (!family_on(t, "parakeet"))
    var inscope kept = PkKept(tag = "v3 Q4_K_M")
    let metal_why = metal_twin_seat_why(TowerFamily.parakeet)
    let twin_why = either_why(metal_why, audio_discrete_form_why(TowerFamily.parakeet))
    let metal_on = empty(metal_why)
    let twin_on = empty(twin_why)
    kept.twin_tag = metal_on ? "parakeet kq metal jfk" : "parakeet kq jfk"
    var inscope forms <- ["f32", "q8", "kq", "kq_z12"]
    add_served_forms(t, forms, PK_Q4, ["kq@dev", "kq_z12@dev"], twin_why)
    let ran = with_model_once(t, "audio", "parakeet-tdt-0.6b-v3-Q4_K_M.gguf", forms, @@parakeet_stage_key, @@parakeet_stage, @@parakeet_form) $(var m; form) {
        parakeet_form_is(t, PK_Q4, form, m)
        kept.form = form
        if (!parakeet_form_of(form).dev) {
            tower_cell(t, "{PK_Q4}: the CPU lanes on jfk, the {form} form's rows", m.pk, kept, @@pk_kq_lane_leg)
        }
        if (form == "kq") {
            tower_cell(t, "the decoder half: the Q4_K_M kq lane's transcript on jfk", m.pk, @@pk_kq_text_cell)
        }
        if (twin_on && form != "q8" && form != "kq_z12") {
            tower_cell(t, "{PK_Q4}: the kq lane's device twin, the {form} form's legs", m.pk, kept, @@pk_twin_leg)
        }
    }
    if (ran) {
        kept_cell(t, "parakeet-TDT 0.6B v3 Q4_K_M: the kq lane on the CPU against the q8 lane, the dequantized f32 chain the reference", kept, @@pk_kq_lanes_held)
    }
    if (ran && metal_on) {
        kept_cell(t, "parakeet-TDT 0.6B v3 Q4_K_M: the Metal chain on the kq lane vs the CPU kq chain on jfk, the dequantized chain the reference", kept, @@test_parakeet_kq_metal_twin)
    } elif (ran && twin_on) {
        kept_cell(t, "parakeet-TDT 0.6B v3 Q4_K_M: the discrete home's chain on the kq lane vs the CPU kq chain on jfk, the dequantized chain the reference", kept, @@test_parakeet_kq_discrete_twin)
    }
    if (ran && twin_on) {
        serving_tower_shutdown(TowerFamily.parakeet)
    }
    with_model_once(t, "audio", "parakeet-tdt-0.6b-v3-Q8_0.gguf", ["kq"], @@parakeet_stage_key, @@parakeet_stage, @@parakeet_form) $(var m8; form8) {
        parakeet_form_is(t, PK_Q8_0, form8, m8)
        tower_cell(t, "the lane's name follows its seats: the Q8_0 file", m8.pk, @@pk_q8_0_gguf_lane)
    }
}

//! The Canary encoder bin's block, with the compares that follow it. `dec` is the Canary-Qwen decoder its own block holds
//! loaded around this call, null where that block did not run: the oracle's cells pair it with the `f32` form and with the
//! form minted on the lane the home's tower serves canary by default.
[cold_path]
def private canary_encoder_block(t : T?; var dec : Model?) {
    var inscope kept = EncTwinKept()
    let metal_why = metal_twin_seat_why(TowerFamily.canary)
    let discrete_why = audio_discrete_form_why(TowerFamily.canary)
    let twin_why = either_why(metal_why, discrete_why)
    let metal_on = empty(metal_why)
    let discrete_on = !metal_on && empty(discrete_why)
    kept.tag = metal_on ? "canary metal jfk" : "canary jfk"
    kept.clips.tag = "canary"
    var walk_form = ""
    if (dec != null) {
        with_gpu_knobs() {
            set_serving_gpu_tower(true, TowerFamily.canary)
            reset_canary_enc_q8()
            let q8 = canary_enc_serves_q8()
            if (canary_dev_flavor(q8) == TowerDev.none) {
                skip_cell(t, "no GPU tower on this box serves canary's default {q8 ? "q8" : "exact"} lane - the device walk needs one")
            } else {
                walk_form = q8 ? "q8@dev" : "f32@dev"
            }
        }
    }
    if (!metal_on) {
        skip_cell(t, "canary-qwen 2.5B: the Metal FastConformer chain's twin - {metal_why}")
    }
    if (!discrete_on) {
        skip_cell(t, "canary-qwen 2.5B: the discrete home's twin and short clips - {metal_on ? "das_metal on this build owns the tower hooks" : discrete_why}")
    }
    var inscope forms <- ["f32"]
    add_served_forms(t, forms, CN_ENC, ["q8", "q8@dev", "q8_z16@dev"], twin_why)
    add_served_forms(t, forms, CN_ENC, ["f32@dev"], either_why(metal_build_why(), twin_why))
    add_served_forms(t, forms, CN_ENC, ["f32_z16@dev"], twin_why)
    if (!empty(walk_form) && !has_value(forms, walk_form)) {
        forms |> push(walk_form)
    }
    let ran = with_model_once(t, "audio", "canary-qwen-2.5b-encoder-f32.bin", forms, @@canary_stage_key, @@canary_stage, @@canary_form) $(var enc; form) {
        canary_form_is(t, CN_ENC, form, enc)
        kept.form = form
        let f = canary_form_of(form)
        if (form == "f32") {
            tower_cell(t, "canary log-mel is bit-identical across calls on one reused state", enc, @@canary_mel_state_reuse)
            if (dec != null) {
                canary_pair_cell(t, "Canary-Qwen 2.5B greedy is token-for-token with the NeMo SALM oracle", *dec, enc, @@test_canary_qwen_oracle)
            }
        }
        static_if (typeinfo builtin_module_exists(das_metal)) {
            if (form == "f32@dev") {
                tower_cell(t, "canary's served Metal encode leaves no encoder rows on the device: no transducer decodes them", enc, @@test_canary_metal_holds_no_rows)
            }
        }
        if (dec != null && form == walk_form) {
            canary_pair_cell(t, "Canary-Qwen 2.5B on the GPU tower's default lane, the {form} form: the oracle's device walk", *dec, enc, @@canary_oracle_device_walk)
        }
        if (metal_on || discrete_on) {
            tower_cell(t, "{CN_ENC}: the {metal_on ? "Metal" : "discrete"} twin, the {form} form's legs", enc, kept, @@cn_twin_leg)
        }
        if (discrete_on && f.lane == "q8" && f.zero_block < 0l) {
            tower_cell(t, "{CN_ENC}: the short clips, the {form} form's leg", enc, kept, @@cn_short_clip_leg)
        }
    }
    static_if (!typeinfo builtin_module_exists(das_metal)) {
        skip_cell(t, "canary's served Metal encode leaves no encoder rows on the device - no Metal build")
    }
    return if (!ran)
    if (metal_on) {
        kept_cell(t, "canary-qwen 2.5B: the Metal FastConformer chain's q8 and f16 lanes vs the CPU chains on jfk, the exact chain the reference", kept, @@test_fc_metal_twin)
    }
    if (discrete_on) {
        kept_cell(t, "canary: clips of 320 to 1600 samples serve on the discrete home's image as the CPU image reads them", kept.clips, @@test_vulkan_short_clips)
        kept_cell(t, "canary-qwen 2.5B: the discrete home's FastConformerblocks vs the CPU q8 chain on jfk, the exact chain the reference", kept, @@test_canary_discrete_twin)
    }
    serving_tower_shutdown(TowerFamily.canary)
}

// The Canary-Qwen 2.5B pair: the encoder bin's block, under the decoder's block where the full suite loads it.
[test(description = "canary-qwen-2.5b-encoder-f32.bin loads once a form. Its log-mel of jfk computed twice on one reused state is bit-identical, a NaN counting as a difference. On the Metal FastConformer chain it serves every block on the q8 slab within the three-way bar on jfk with no quant_mode decline, a poison exceeds it, the exact f16 lane holds its bar, and a served encode leaves zero encoder rows held on the device, since no transducer decodes them. On the discrete home it serves its front and every block on the device within the three-way bar on jfk, holds it across regrown and fresh residencies and on the exact f16 lane, a poison exceeds it, and clips of 320 to 1600 samples serve as one device encode with no decline within SHORT_CLIP_BAR of the CPU image's rows, a noise clip outside it. Under the full suite canary-qwen-2.5b-decoder-f16.gguf loads once around it: on three clips, on the CPU chain and the GPU tower's default lane, every NeMo SALM oracle token stays within 0.25 logit of the argmax (one-step-early feed misses) and greedy ids match the oracle through the forced walk's agreement")]
def test_canary_carrier(t : T?) {
    return if (!family_on(t, "canary"))
    if (!parity_full_mode()) {   // ~6.7 GB two-file (decoder GGUF + f32 encoder bin) - large-tier
        skip_cell(t, "Canary-Qwen 2.5B greedy is token-for-token with the NeMo SALM oracle - canary-qwen is large-tier, set DASLLAMA_PARITY_FULL=1 to run")
        canary_encoder_block(t, null)
        return
    }
    var under_decoder = false
    unsafe {
        t |> run("the Canary-Qwen decoder, the encoder's block under it") @capture(& under_decoder)(t : T ?) {
            with_cpu_and_gpu_model(t, "audio", "canary-qwen-2.5b-decoder-f16.gguf", ASR_DEC_CTX, [cpu_cells_form(QuantMode.q8)]) $(var dec; form) {
                return if (!form.alone)
                under_decoder = true
                canary_encoder_block(t, unsafe(addr(dec)))
            }
        }
    }
    if (!under_decoder) {
        canary_encoder_block(t, null)
    }
}

//! the minted forms a gemma4a mmproj's twins read: the exact form and the q8 form, the q8 lane's image in the home's
//! flavor and its poison, the exact lane's image in the home's flavor and its poison
let private G4A_TWIN_FORMS = fixed_array("f32", "q8", "q8@dev", "q8_z6@dev", "f32@dev", "f32_z6@dev")

//! the twins' cells of a gemma4a mmproj's minted form `form`: its legs on the home the box serves, and - `short_clips` -
//! the short clips' leg on the discrete home's q8 forms
[cold_path]
def private g4a_twin_cells(t : T?; file, form : string; var enc : Gemma4aEncoder; var kept : EncTwinKept; metal_on, discrete_on, short_clips : bool) {
    let f = g4a_form_of(form)
    if (metal_on || discrete_on) {
        tower_cell(t, "{file}: the {metal_on ? "Metal" : "discrete"} twin, the {form} form's legs", enc, kept, @@g4a_twin_leg)
    }
    if (short_clips && discrete_on && f.lane == "q8" && f.zero_block < 0l) {
        tower_cell(t, "{file}: the short clips, the {form} form's leg", enc, kept, @@g4a_short_clip_leg)
    }
}

//! the twins' named skips of the gemma4a encoder `tag` on a box that lacks a home
[cold_path]
def private g4a_twin_skips(t : T?; tag : string; metal_why, discrete_why : string) {
    let metal_on = empty(metal_why)
    if (!metal_on) {
        skip_cell(t, "gemma4a {tag}: the Metal Conformer chain's twin - {metal_why}")
    }
    if (metal_on || !empty(discrete_why)) {
        skip_cell(t, "gemma4a {tag}: the discrete home's twin - {metal_on ? "das_metal on this build owns the tower hooks" : discrete_why}")
    }
}

//! the twins' compares of the gemma4a encoder `tag` (`mm_file` its mmproj), after its block
[cold_path]
def private g4a_twin_held(t : T?; tag, mm_file : string; var kept : EncTwinKept; metal_on, discrete_on, short_clips : bool) {
    if (metal_on) {
        kept_cell(t, "gemma4a {tag}: the Metal Conformer chain's q8 and f16 lanes vs the CPU chains on jfk ({mm_file}), the exact chain the reference", kept, @@test_gemma4a_metal_twin)
    }
    if (discrete_on && short_clips) {
        kept_cell(t, "gemma4a {tag}: clips of 320 to 1600 samples serve on the discrete home's image as the CPU image reads them", kept.clips, @@test_vulkan_short_clips)
    }
    if (discrete_on) {
        kept_cell(t, "gemma4a {tag}: the discrete home's Conformer chain vs the CPU q8 chain on jfk, then its exact lane, the exact chain the reference", kept, @@test_gemma4a_discrete_twin)
    }
    if (metal_on || discrete_on) {
        serving_tower_shutdown(TowerFamily.gemma4a)
    }
}

// The E2B Q4_K_M decoder's block, nested in the mmproj's form minted on the lane the policy names: the rows seam on a
// plain chat reads both files.
[cold_path]
def private g4a_e2b_q4_decoder(t : T?; var e : AudioEmbedder) {
    unsafe {
        t |> run("the gemma-4 E2B Q4_K_M decoder under the gemma4a encoder") @capture(& e)(t : T ?) {
            with_cpu_and_gpu_model(t, "audio", "gemma-4-E2B-it-Q4_K_M.gguf", 2048l, [cpu_cells_form(QuantMode.q8)]) $(var m; form) {
                audio_chat_cell(t, "rows land, the turn answers, a second clip appends, and the refusals panic", m, form, e, @@test_audio_rows_on_a_plain_chat)
            }
        }
    }
}

// The gemma-4 E2B mmproj's audio tower, one block; E2B's audio embedder is square on the encoder's 1536.
[test(description = "mmproj-gemma-4-E2B-it-bf16.gguf loads its audio tower once a form. The carrier's load probes and loads as gemma4a at width 1536 and pads a sub-frame clip to one soft token, and a CURRENT gemma4a .dlim routes on its baked tag - the one the load baked beside the mmproj, and one minted into the scratch directory. The ASR facade's two-file load pairs it with gemma-4-E2B-it-Q8_0.gguf: on the f32 KV rail the pair transcribes gemma4a_test2.wav with the first 40 token ids of llama-mtmd-cli's greedy output, the confident prefix before its own near-tie, with a flat heap across reuse. On a form minted in memory the log-mel of jfk computed twice on one reused state is bit-identical, and under it gemma-4-E2B-it-Q4_K_M.gguf loads once: gemma4a rows on a plain chat append across clips, prefill as head plus rows plus tail and are consumed by the turn, while rows of the wrong length or beside a queued image panic. On the Metal Conformer chain the encoder serves every block on the q8 slab within the three-way bar on jfk, a poison exceeds it, and the exact f16 lane holds its bar. On the discrete home it stays within the three-way bar the CPU q8 chain sets against the exact chain on jfk, across regrown and fresh residencies, a zeroed-blocks poison exceeds it, the exact lane holds its bar, and clips of 320 to 1600 samples serve as one device encode with no decline within SHORT_CLIP_BAR of the CPU image's rows, a noise clip outside it")]
def test_gemma4a_carrier_e2b(t : T?) {
    return if (!family_on(t, "gemma4a"))
    var inscope kept = EncTwinKept(want_proj = 1536l)
    let metal_why = metal_twin_jit_why(TowerFamily.gemma4a)
    let discrete_why = audio_discrete_form_why(TowerFamily.gemma4a)
    let metal_on = empty(metal_why)
    let discrete_on = !metal_on && empty(discrete_why)
    kept.tag = metal_on ? "gemma4a E2B metal jfk" : "gemma4a E2B jfk"
    kept.clips.tag = "gemma4a"
    g4a_twin_skips(t, "E2B", metal_why, discrete_why)
    reset_gemma4a_q8()
    let policy = gemma4a_serves_q8() ? "q8" : "f32"   //! the lane a mint with no pin takes: the mel cell's and the rows seam's
    var inscope forms <- ["image"]
    if (asr_decoder_available(t, "gemma-4-E2B-it-Q8_0.gguf")) {
        forms |> push("image_asr")
    }
    forms |> push(policy)
    add_served_forms(t, forms, G4A_E2B, [for (f in G4A_TWIN_FORMS); f; where f != policy], either_why(metal_why, discrete_why))
    let ran = with_model_once(t, "audio", "mmproj-gemma-4-E2B-it-bf16.gguf", forms, @@g4a_stage_key, @@g4a_stage, @@gemma4a_e2b_form) $(var h; form) {
        gemma4a_form_is(t, G4A_E2B, form, h)
        kept.form = form
        if (form == "image") {
            tower_cell(t, "the E2B mmproj probes and loads as gemma4a with its width", h.e, @@test_audio_embedder_gemma4a_arm)
            return
        }
        if (form == "image_asr") {
            tower_cell(t, "Gemma-4 E2B audio greedy is token-for-token with llama-mtmd-cli (confident prefix)", h.asr, @@test_gemma4a_audio_oracle)
            return
        }
        if (form == policy) {
            tower_cell(t, "gemma4a log-mel is bit-identical across calls on one reused state", h.e.gemma4a, @@test_mel_state_reuse_deterministic)
            g4a_e2b_q4_decoder(t, h.e)
        }
        g4a_twin_cells(t, G4A_E2B, form, h.e.gemma4a, kept, metal_on, discrete_on, true)
    }
    t |> run("an E2B gemma4a image minted for this box into the scratch directory reads CURRENT and maps by the direct route") @(t : T?) {
        embedder_scratch_image(t)
    }
    if (ran) {
        g4a_twin_held(t, "E2B", G4A_E2B, kept, metal_on, discrete_on, true)
    }
}

// The gemma-4 E4B mmproj's audio tower, one block; E4B's audio embedder widens the encoder's 1536 to the decoder's 2560 -
// the projector tail at the other width.
[test(description = "mmproj-gemma-4-E4B-it-BF16.gguf loads its audio tower once a form, and probes its decoder's width 2560. The ASR facade's two-file load, the serving tower on, pairs it with gemma-4-E4B-it-Q8_0.gguf: the pair widens the encoder's 1536 to the decoder's 2560 and transcribes jfk past the words floor, with soft tokens off the serving GPU tower where its device stands up. On the Metal Conformer chain the encoder serves every block on the q8 slab within the three-way bar on jfk, a poison exceeds it, and the exact f16 lane holds its bar. On the discrete home it stays within the three-way bar the CPU q8 chain sets against the exact chain on jfk, across regrown and fresh residencies, a zeroed-blocks poison exceeds it, and the exact lane holds its bar")]
def test_gemma4a_carrier_e4b(t : T?) {
    return if (!family_on(t, "gemma4a"))
    var inscope kept = EncTwinKept(want_proj = 2560l)
    let metal_why = metal_twin_jit_why(TowerFamily.gemma4a)
    let discrete_why = audio_discrete_form_why(TowerFamily.gemma4a)
    let metal_on = empty(metal_why)
    let discrete_on = !metal_on && empty(discrete_why)
    kept.tag = metal_on ? "gemma4a E4B metal jfk" : "gemma4a E4B jfk"
    g4a_twin_skips(t, "E4B", metal_why, discrete_why)
    var inscope forms : array<string>
    if (stat(path_join(models_dir(), G4A_E4B)).is_valid && asr_decoder_available(t, "gemma-4-E4B-it-Q8_0.gguf")) {
        forms |> push("image_asrdev")
    }
    add_served_forms(t, forms, G4A_E4B, [for (f in G4A_TWIN_FORMS); f], either_why(metal_why, discrete_why))
    let ran = with_model_once(t, "audio", "mmproj-gemma-4-E4B-it-BF16.gguf", forms, @@g4a_stage_key, @@g4a_stage, @@gemma4a_e4b_form) $(var h; form) {
        gemma4a_form_is(t, G4A_E4B, form, h)
        kept.form = form
        if (form == "image_asrdev") {
            tower_cell(t, "Gemma-4 E4B audio through the facade: the widening embedder's soft tokens transcribe jfk", h.asr, @@test_gemma4a_e4b_audio_facade)
            return
        }
        g4a_twin_cells(t, G4A_E4B, form, h.e.gemma4a, kept, metal_on, discrete_on, false)
    }
    t |> run("the E4B mmproj probes its own decoder's width, read off the embedder's rows") @(t : T?) {
        embedder_e4b_probe(t)
    }
    if (ran) {
        g4a_twin_held(t, "E4B", G4A_E4B, kept, metal_on, discrete_on, false)
    }
}
