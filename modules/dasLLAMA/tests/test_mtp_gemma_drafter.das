options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes: the CPU cell frees the target before the run ends
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama   // the public facade — engine + arch registrations
require dasllama/dasllama_mtp_gemma   // the subject: the gemma-4 assistant drafter loader + CPU oracle
require dasllama/dasllama_math   // with_dasllama_jobque_ - the CPU prefill needs the engine's own worker pool
require dasllama/dasllama_image   // nolint:STYLE030,LINT019 — load_model_cached is called in the das_metal half
require ?das_metal dasllama/dasllama_metal_mtp_gemma   // the subject's Metal half: attach + one draft step
require ?das_metal dasllama/dasllama_metal_common   // finish_pending_step - land the pipelined decode before reading its carry; the decode drivers' counts
require ?das_metal dasllama/dasllama_metal_decode   // nolint:STYLE030,LINT019 - the Metal decode driver registers at init
require ?das_metal dasllama/dasllama_metal_prefill   // the metal prefill override + shutdown, its served count
require ?das_metal metal/das_metal_boost   // metal_live_object_count - the leak gate
require ?vulkan dasllama/dasllama_vulkan_mtp_gemma   // the subject's Vulkan half: one draft step on the resident driver
require ?cuda dasllama/dasllama_cuda_mtp_gemma   // the subject's CUDA half: the same step on the CUDA home's resident driver
require dasllama/dasllama_gpu_tier   // gpu_want_cuda: which discrete home the served cells read
require dasllama/dasllama_gpu_resident   // nolint:STYLE030,LINT019 — gpu_resident_prefills_, read in the vulkan half: the trunk's served witness
require daslib/jobque_boost
require daslib/fio
require math
require _model_tier   // models_dir() + model_available() presence/size-tier gate (see the file's header)
require _scratch_dir   // scratch_path: the refusal fixtures' files
require _scheduler_rig   // make_req / drain / toks_of - the served round rides the scheduler
require _resident_feed   // resident_ready / load_resident_at / served_every_call - the Vulkan home's trunk
require _metal_blob_twin   // metal_overrides_seat / restore_metal_overrides - the served cells' override pins
require ../performance/model_specs.das   // counting_prompt - the served cells' tie-proof prompts
require _cpu_gpu_model   // with_cpu_and_gpu_model, the forms: the trunk's block
require _parity_cells   // the parity cells the block's parity knob reaches (`_::`): the trunks' model_specs parity rows on their blocks
require _gemma_resident   // gemma_rows, ppl_within, GEMMA_RIG_CTX: the whole file's walks
require _dflash_cells   // cpu_drafter_lossless, discrete_drafter_cells: the regular DFlash drafter's legs
require dasllama/dasllama_dflash   // attach_dflash_drafter: the prompt-capture rows
require ?das_metal dasllama/dasllama_metal_dflash   // nolint:STYLE030,LINT019 - the drafter's device arm registers at init
require _compares   // mismatch_rel: the prompt-capture cell's sums
require dasllama/dasllama_env   // g_env_test: the arm filter a block with no cell left names

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // CPU-oracle test — prefills on the CPU by design
}

// The gemma-4 assistant drafter (`dasllama/dasllama_mtp_gemma`): the sidecar resolver and loader's shape and refusal
// contract, one step of the CPU reference forward against a live target
// session, and the served rounds on the GPU home - one device step against the oracle, the scheduler's rounds token for token
// the plain scheduler's at depth 1, at the depth knob's depth, over several streams in one verify, on block-codec sessions
// and on a trunk whose token table is K-quant. The drafter sidecar is a 440 MiB small-tier file; the 15.8 GiB target is
// PARITY_FULL-gated. A model cell skips where a file it reads is absent: the sidecar, the target, the Q4_0 head the type
// decline reads (`mtp-gemma-4-26B-A4B-it-Q4_0.gguf`). The attach's refusal of the per-layer-embedding E2B trunk is a cell of
// `test_gpu_resident_gemma4_e2b.das`'s load-once block. The GPU cells
// skip on a build with no GPU home and on a box that does not serve the target on it, and the served rounds skip where the
// card holds the trunk only with routed layers' experts on the host (the drafter's verify rides the N-row command, which has
// no host-experts form on either discrete home); the Metal attach's gather refusal and the verify-decline cell skip on a build
// without das_metal, the Vulkan driver check without dasVulkan. The discrete homes' served cells ride the trunks' blocks at the
// file's end - `test_gemma_drafter_26b_trunk` on the Q4_K_M file, with its whole-file perplexity walks, the CPU oracle step
// and the regular DFlash drafter's legs; `test_gemma_drafter_26b_iq3_trunk` on the UD-IQ3_XXS file, with its walks
// (its f16 device form a skipped cell of its own on a das_metal build, which serves that trunk on the Metal
// form) - each
// form loaded once; the Metal home's served cells ride the same blocks' Metal form, the file's served image loaded once
// (a skipped cell of its own on a build without das_metal, or where the image is no metal-blob on the box). Each trunk's parity
// row CPU form (the row's load mode and K-quant knob under DASLLAMA_PIN_BACKEND's pin, under the trunk's perplexity arm),
// on every box, its frozen greedy ids (`parity_spec_cell`), skipped where the box cannot take the pin.

let private TARGET_FILE = "gemma-4-26B-A4B-it-Q4_K_M.gguf"
let private IQ3_FILE = "gemma-4-26B-A4B-it-UD-IQ3_XXS.gguf"   //! its token table is Q6_K: the seed embed gathers a K-quant row
let private Q40_HEAD = "mtp-gemma-4-26B-A4B-it-Q4_0.gguf"
let private TARGET_LAYERS = 30l

// a Q8_0 tensor's byte cost in the drafter blob: whole 34-byte blocks, padded to the next
// 32-byte tensor start
def private q8_bytes(n : int64) : int64 => ((n / 32l) * 34l + 31l) / 32l * 32l

// ===== refusals and the resolver (no model file needed) =====

[test(description = "`load_gemma_drafter` refuses a missing path and a magic-less file with a reason and no weights, and the sidecar resolver finds the gemma4-assistant head beside UD, K-quant and i-quant trunks but never a qwen35 NextN head of the same name")]
def test_gemma_drafter_refusals(t : T?) {
    if (!arm_on(t, "mtp-gdraft-refuse")) {
        return
    }
    t |> run("a missing path refuses with a reason instead of panicking") @(t : T?) {
        var dr <- load_gemma_drafter(path_join(scratch_path("."), "no-such-drafter.gguf"))
        t |> success(!dr.ok, "ok is false")
        t |> success(!empty(dr.why), "why names the refusal: '{dr.why}'")
        t |> success(empty(dr.blob) && empty(dr.fblob), "a refused load carries no weights")
        delete dr
    }
    t |> run("a file with no GGUF magic refuses with a reason") @(t : T?) {
        let p = scratch_file("junk.gguf", "not a gguf file at all, but long enough to pass the length gate")
        var dr <- load_gemma_drafter(p)
        t |> success(!dr.ok, "ok is false")
        t |> success(find(dr.why, "magic") >= 0, "why names the magic: '{dr.why}'")
        delete dr
    }
    t |> run("gemma_drafter_sidecar never recurses onto a head, and refuses an absent sibling") @(t : T?) {
        t |> equal(gemma_drafter_sidecar(path_join(models_dir(), "mtp-gemma-4-26B-A4B-it-Q8_0.gguf")), "")
        t |> equal(gemma_drafter_sidecar(path_join(scratch_path("."), "no-such-model-Q4_K_M.gguf")), "")
    }
    t |> run("the resolver finds the upstream head beside an unsloth -UD- trunk, a K-quant and an i-quant one, through the facade too") @(t : T?) {
        let dir = scratch_path("ud")
        mkdir_rec(dir)
        let head = arch_gguf(path_join("ud", "mtp-gemma-4-26B-A4B-it-Q8_0.gguf"), "gemma4-assistant")
        for (trunk in ["gemma-4-26B-A4B-it-UD-Q4_K_M.gguf", "gemma-4-26B-A4B-it-UD-IQ3_XXS.gguf", "gemma-4-26B-A4B-it-Q4_K_M.gguf", "gemma-4-26B-A4B-it-IQ4_XS.gguf"]) {
            t |> equal(gemma_drafter_sidecar(path_join(dir, trunk)), head, "{trunk} finds mtp-gemma-4-26B-A4B-it-Q8_0.gguf")
            t |> equal(mtp_drafter_sidecar(path_join(dir, trunk)), head, "{trunk} finds it through the facade")
        }
        let own = arch_gguf(path_join("ud", "mtp-gemma-4-26B-A4B-it-UD-Q8_0.gguf"), "gemma4-assistant")
        t |> equal(gemma_drafter_sidecar(path_join(dir, "gemma-4-26B-A4B-it-UD-Q4_K_M.gguf")), own, "a head named with the trunk's -UD tag wins over the untagged one")
        t |> equal(gemma_drafter_sidecar(path_join(dir, "gemma-4-12B-it-UD-Q4_K_M.gguf")), "", "the control: another model's trunk finds no head")
    }
    t |> run("a split NextN head under the drafter's name is no drafter; the same name carrying an assistant is") @(t : T?) {
        let dir = scratch_path("nextn")
        mkdir_rec(dir)
        let trunk = path_join(dir, "Qwen3.8-27B-Q4_K_M.gguf")
        arch_gguf(path_join("nextn", "mtp-Qwen3.8-27B-Q8_0.gguf"), "qwen35")
        t |> equal(gemma_drafter_sidecar(trunk), "", "the qwen35 head beside the trunk is not an assistant drafter")
        let exact = arch_gguf(path_join("nextn", "mtp-Qwen3.8-27B-Q4_K_M.gguf"), "qwen35")
        t |> equal(gemma_drafter_sidecar(trunk), "", "nor is the exact-name qwen35 head")
        let side = arch_gguf(path_join("nextn", "mtp-Qwen3.8-27B-Q8_0.gguf"), "gemma4-assistant")
        t |> equal(gemma_drafter_sidecar(trunk), side, "the control: the same name carrying gemma4-assistant is found")
        remove(exact)
    }
}


[test(description = "The drafter attach's target check names a short trunk or a capture layer's mismatched head size or kv heads, the facade joins every backend's refusal, and the Vulkan driver check names the first missing fact while a complete driver passes")]
def test_gemma_drafter_attach_checks(t : T?) {
    if (!arm_on(t, "mtp-gdraft-refuse")) {
        return
    }
    t |> run("the attach's target check names the trunk's short depth and a capture layer's geometry; the matching trunk passes") @(t : T?) {
        var shell = Model()
        shell.config = g26_config()
        var dr <- g26_drafter(shell.config)
        t |> equal(gemma_drafter_target_why(shell, dr), "", "the control: a trunk of the drafter's own geometry passes")
        dr.hs[3] = 256l
        let why_hs = gemma_drafter_target_why(shell, dr)
        t |> success(find(why_hs, "drafter layer 3 reads trunk layer 29") >= 0 && find(why_hs, "the drafter wants 2x256") >= 0,
            "a capture layer of another head size refuses by layer: '{why_hs}'")
        dr.hs[3] = 512l
        dr.kvh[0] = 4l
        let why_kvh = gemma_drafter_target_why(shell, dr)
        t |> success(find(why_kvh, "drafter layer 0 reads trunk layer 28") >= 0, "a capture layer of another kv head count refuses by layer: '{why_kvh}'")
        dr.kvh[0] = 8l
        shell.config.n_layers = 2l
        let why_short = gemma_drafter_target_why(shell, dr)
        t |> success(find(why_short, "short of the two capture layers") >= 0, "a two-layer trunk refuses on its depth: '{why_short}'")
        delete dr
        delete shell
    }
    t |> run("the facade's refusal names every registered backend's own refusal, or that this build registers none") @(t : T?) {
        t |> equal(mtp_attach_refusal(default<array<string>>), ("no GPU driver on this build carries an assistant drafter - require dasllama/dasllama_metal_mtp_gemma (dasMetal), "
            + "dasllama/dasllama_vulkan_mtp_gemma (dasVulkan) or dasllama/dasllama_cuda_mtp_gemma (dasCUDA)"), "no backend registered names the modules")
        t |> equal(mtp_attach_refusal(["metal: a", "vulkan: b"]), "metal: a; vulkan: b", "each backend's refusal, joined")
        var shell = Model()
        let why = attach_mtp_drafter_(shell, path_join(scratch_path("."), "no-such-drafter.gguf"))
        t |> success(!empty(why), "a shell takes no drafter: '{why}'")
        static_if (typeinfo builtin_module_exists(das_metal)) {
            t |> success(find(why, "metal: the trunk is not the metal-blob flavor the Metal drivers serve") >= 0, "the Metal attach refuses a trunk off the metal-blob flavor before the sidecar's read: '{why}'")
        }
        static_if (typeinfo builtin_module_exists(vulkan)) {
            t |> success(find(why, "vulkan: the Vulkan resident driver does not serve the trunk whole") >= 0, "the Vulkan attach refuses with no resident driver armed: '{why}'")
        }
        t |> success(!mtp_capable(shell), "and the shell cannot speculate")
        delete shell
    }
    t |> run("the Vulkan attach's driver check names the first fact the armed driver lacks; a driver holding every one passes") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            let c = g26_config()
            var dr <- g26_drafter(c)
            dr.kv_layer <- [28l, 28l, 28l, 29l]
            var f = VkDrafterTrunk(served = true, emb_table = true, assist_rows = 16l)
            f.own_kv <- [for (_l in range64(c.n_layers)); true]
            t |> equal(vk_drafter_trunk_why(f, dr), "", "the control: a driver holding every fact takes the drafter")
            f.own_kv[29] = false
            t |> success(find(vk_drafter_trunk_why(f, dr), "the capture layer 29 keeps no K/V rows") >= 0, "a capture layer sharing its donor's rows refuses")
            f.own_kv[29] = true
            dr.kv_layer[3] = 30l
            t |> success(find(vk_drafter_trunk_why(f, dr), "the capture layer 30 keeps no K/V rows") >= 0, "a capture layer past the trunk refuses")
            dr.kv_layer[3] = 29l
            f.assist_rows = 1l
            t |> success(find(vk_drafter_trunk_why(f, dr), "the N-row token command declined the trunk") >= 0, "a driver with no N-row command refuses")
            f.emb_table = false
            t |> success(find(vk_drafter_trunk_why(f, dr), "no token table to gather the seed rows from") >= 0, "a driver holding no token table refuses first")
            f.hc = true
            t |> equal(vk_drafter_trunk_why(f, dr), "a hyper-connection trunk", "a hyper-connection trunk refuses first")
            f.served = false
            t |> success(find(vk_drafter_trunk_why(f, dr), "does not serve the trunk whole") >= 0, "a trunk the driver does not serve refuses first")
            delete f
            delete dr
        } else {
            t |> skip("no dasVulkan module on this build")
        }
    }
}

[test(description = "`assist_bind_rung` stops at the first failing rung - no armed driver, another codec, past the region's context, no free region - binds otherwise, and binds a session that owns its region whatever the others hold")]
def test_gemma_drafter_bind_ladder(t : T?) {
    if (!arm_on(t, "mtp-gdraft-refuse")) {
        return
    }
    t |> run("the Vulkan round's bind stops at the first rung of its ladder: the driver, the codec, the context, a free region") @(t : T?) {
        let none : array<int64>
        var took0 <- [0l]
        let rung = $(active, codec : bool; end : int64; owns : bool; taken : array<int64>; pin0, pin1 : bool) : AssistBindRung {
            return assist_bind_rung(active, codec, end, 64l, owns, 2l, taken) $(i) => i == 0l ? pin0 : pin1
        }
        t |> equal(rung(true, true, 64l, false, none, false, true), AssistBindRung.bind, "the control: a round inside the context with region 0 free binds")
        t |> equal(rung(false, true, 64l, false, none, false, false), AssistBindRung.inactive, "no armed driver")
        t |> equal(rung(true, false, 64l, false, none, false, false), AssistBindRung.codec, "a session on another codec")
        t |> equal(rung(true, true, 65l, false, none, false, false), AssistBindRung.cap, "a round past the region's context")
        t |> equal(rung(true, true, 64l, false, none, true, true), AssistBindRung.no_region, "every region pinned")
        t |> equal(rung(true, true, 64l, false, took0, false, true), AssistBindRung.no_region, "the one unpinned region taken by the tick's earlier stream")
        t |> equal(rung(true, true, 64l, true, took0, true, true), AssistBindRung.bind, "a session owning its region binds whatever the others hold")
        delete took0
    }
}

//! the gemma-4-26B-A4B trunk's attention geometry: thirty layers, every sixth global (512-wide heads, two kv heads), the rest sliding
//! (256-wide, eight kv heads) - the shape the drafter's capture layers read
def private g26_config() : Config {
    return Config(dim = 2816l, hidden_dim = 32l, n_layers = TARGET_LAYERS, n_heads = 16l, n_kv_heads = 2l, n_kv_heads_swa = 8l, head_size = 512l,
        head_size_swa = 256l, sliding_window = 1024l, swa_pattern = 6l, vocab_size = 8l, seq_len = 64l, kv_dim = 1024l)
}

//! a drafter whose four layers read the trunk's last two layers (unbound, as the attach reads a sidecar), each layer's sliding flag,
//! head size and kv heads off `c` at its capture layer
def private g26_drafter(c : Config) : GemmaDrafter {
    var dr = GemmaDrafter(tdim = c.dim, n_layers = 4l)
    dr.kv_layer <- [-2l, -2l, -2l, -1l]
    dr.sliding <- [for (rel in dr.kv_layer); layer_is_sliding(c, c.n_layers + rel)]
    dr.hs <- [for (rel in dr.kv_layer); layer_head_size(c, c.n_layers + rel)]
    dr.kvh <- [for (rel in dr.kv_layer); layer_n_kv_heads(c, c.n_layers + rel)]
    return <- dr
}

[test(description = "The Metal drafter attach refuses a metal-blob trunk shell of the drafter's geometry whose token table the device gathers no row of, naming the table, and the shell cannot speculate")]
def test_gemma_drafter_gather_refusal(t : T?) {
    if (!family_on(t, "gemma4moe") || !arm_on(t, "mtp-gdraft-refuse")) {
        return
    }
    t |> run("the Metal attach refuses a metal-blob trunk whose token table the device gathers no row of") @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let side = path_join(models_dir(), "mtp-gemma-4-26B-A4B-it-Q8_0.gguf")
            return if (!model_available(t, side))
            var dr <- load_gemma_drafter(side)
            t |> success(dr.ok, "the sidecar loads (why='{dr.why}')")
            var shell = Model(metal_blob = true, emb_q8 = false, tok_emb_off = -1l)
            shell.config = g26_config()
            t |> success(!emb_kq(shell), "the shell carries no K-quant token table")
            t |> equal(gemma_drafter_target_why(shell, dr), "", "the shell carries the drafter's trunk geometry: the target check passes")
            let why = attach_mtp_drafter_(shell, side)
            t |> success(find(why, "metal: the device gathers no row of the trunk's token table") >= 0, "the Metal attach names the table it cannot gather: '{why}'")
            t |> success(!mtp_capable(shell), "and the shell cannot speculate")
            delete shell
            delete dr
        } else {
            t |> skip("no das_metal module on this build")
        }
    }
}

[test(description = "Loading the Q4_0 gemma-4 drafter head fails with a reason naming the tensor's Q4_0 type and the expected Q8_0")]
def test_gemma_drafter_q40_head(t : T?) {
    if (!family_on(t, "gemma4moe") || !arm_on(t, "mtp-gdraft-refuse")) {
        return
    }
    t |> run("the Q4_0 head declines naming the tensor's type: the drafter serves a Q8_0 sidecar alone") @(t : T?) {
        let side = path_join(models_dir(), Q40_HEAD)
        return if (!model_available(t, side))
        var dr <- load_gemma_drafter(side)
        t |> success(!dr.ok, "the Q4_0 sidecar does not load")
        t |> success(find(dr.why, "Q4_0") >= 0 && find(dr.why, "expected Q8_0") >= 0, "why names the tensor's type and the type served: '{dr.why}'")
        delete dr
    }
}

// ===== loader shape (the drafter sidecar alone — no target) =====

[test(description = "The gemma-4-26B Q8_0 drafter sidecar loads with the contract's dims, per-layer head sizes, kv heads, sliding flags, capture layers 28,28,28,29 and layer scales, its blob the exact padded Q8_0 byte sum with every tensor 32-byte aligned")]
def test_gemma_drafter_load(t : T?) {
    if (!family_on(t, "gemma4moe") || !arm_on(t, "mtp-gdraft-load")) {
        return
    }
    t |> run("the gemma-4 drafter sidecar loads with the shape the encoder contract names") @(t : T?) {
        let target = path_join(models_dir(), TARGET_FILE)
        let side = gemma_drafter_sidecar(target)
        if (empty(side)) {
            t |> skip("no mtp-* drafter sidecar beside {TARGET_FILE}")
            return
        }
        t |> equal(base_name(side), "mtp-gemma-4-26B-A4B-it-Q8_0.gguf", "the sidecar prefers the Q8_0 head")
        if (!model_available(t, side)) return
        var dr <- load_gemma_drafter(side, TARGET_LAYERS)
        t |> success(dr.ok, "load ok (why='{dr.why}')")
        if (!dr.ok) {
            delete dr
            return
        }
        t |> equal(dr.dim, 1024l, "dim")
        t |> equal(dr.tdim, 2816l, "tdim (the target's hidden width)")
        t |> equal(dr.hidden, 8192l, "hidden")
        t |> equal(dr.vocab, 262144l, "vocab")
        t |> equal(dr.n_layers, 4l, "n_layers")
        t |> equal(dr.n_heads, 16l, "n_heads")
        t |> equal(dr.sliding_window, 1024l, "sliding_window")
        t |> success(abs(dr.eps - 1.0e-6) < 1.0e-10, "eps is 1e-6 (got {dr.eps})")
        t |> success(dr.rope_theta == 1.0e6 && dr.rope_theta_swa == 1.0e4,
            "rope theta {dr.rope_theta} full / {dr.rope_theta_swa} sliding")
        t |> success(length(dr.hs) == 4 && dr.hs[0] == 256l && dr.hs[1] == 256l && dr.hs[2] == 256l && dr.hs[3] == 512l,
            "hs = 256,256,256,512 (got {dr.hs})")
        t |> success(length(dr.kvh) == 4 && dr.kvh[0] == 8l && dr.kvh[1] == 8l && dr.kvh[2] == 8l && dr.kvh[3] == 2l,
            "kvh = 8,8,8,2 (got {dr.kvh})")
        t |> success(dr.sliding[0] && dr.sliding[1] && dr.sliding[2] && !dr.sliding[3],
            "sliding = T,T,T,F (got {dr.sliding})")
        t |> success(dr.kv_bound, "kv_layer is bound by the load's target_n_layers")
        t |> success(dr.kv_layer[0] == 28l && dr.kv_layer[1] == 28l && dr.kv_layer[2] == 28l && dr.kv_layer[3] == 29l,
            "kv_layer = 28,28,28,29 (got {dr.kv_layer})")
        t |> success(abs(dr.layer_scale[0] - 0.29101562) < 1.0e-6 && abs(dr.layer_scale[1] - 0.50390625) < 1.0e-6
            && abs(dr.layer_scale[2] - 0.52734375) < 1.0e-6 && abs(dr.layer_scale[3] - 0.41210938) < 1.0e-6,
            "layer_output_scale = {dr.layer_scale}")
        // the blob is every Q8_0 weight, each tensor padded to a 32-byte start
        var want_blob = q8_bytes(2l * dr.tdim * dr.dim) + q8_bytes(dr.dim * dr.tdim) + q8_bytes(dr.dim * dr.vocab)
        var want_fblob = dr.dim + dr.rope_freqs_n
        for (l in range64(dr.n_layers)) {
            let qd = dr.n_heads * dr.hs[l]
            want_blob += q8_bytes(dr.dim * qd) + q8_bytes(qd * dr.dim) + 3l * q8_bytes(dr.dim * dr.hidden)
            want_fblob += 4l * dr.dim + dr.hs[l]
        }
        t |> equal(long_length(dr.blob), want_blob, "blob bytes = the Q8_0 tensor sum")
        t |> equal(long_length(dr.fblob), want_fblob, "fblob floats = the F32 norm/table sum")
        t |> equal(dr.rope_freqs_n, 256l, "rope_freqs is 256 wide (the full class's half-dim)")
        // p-RoPE: 64 rotating dims followed by the pass-through tail
        t |> success(dr.fblob[dr.rope_freqs_off] == 1.0 && dr.fblob[dr.rope_freqs_off + 63l] == 1.0
            && dr.fblob[dr.rope_freqs_off + 64l] > 1.0e29,
            "rope_freqs = 64 x 1.0 then the 1e30 tail")
        var aligned = true
        var offs <- [dr.pre_proj_off, dr.post_proj_off, dr.tok_emb_off]
        offs |> reserve(3l + 5l * dr.n_layers)
        for (l in range64(dr.n_layers)) {
            offs |> push(dr.wq_offs[l])
            offs |> push(dr.wo_offs[l])
            offs |> push(dr.w1_offs[l])
            offs |> push(dr.w3_offs[l])
            offs |> push(dr.w2_offs[l])
        }
        for (o in offs) {
            aligned = aligned && o >= 0l && o % 32l == 0l && o < long_length(dr.blob)
        }
        t |> success(aligned, "every blob tensor starts 32-byte aligned and inside the blob ({offs})")
        delete dr
    }
}

// ===== the CPU oracle against a live target session =====

//! one CPU reference drafter step on the CPU form `tr` (its context cut to 512 for the cell), seeded with the target's argmax
//! and its live K/V and hidden
def private gdraft_cpu_step_cell(t : T?; var tr : Model) {
    with_ctx_cut(tr, 512l) {
        let target = path_join(models_dir(), TARGET_FILE)
        let side = gemma_drafter_sidecar(target)
        if (empty(side)) {
            t |> skip("no mtp-* drafter sidecar beside {TARGET_FILE}")
            return
        }
        t |> equal(tr.config.n_layers, TARGET_LAYERS, "the target carries the 30-layer gemma4 trunk")
        var dr <- load_gemma_drafter(side, tr.config.n_layers)
        t |> success(dr.ok, "drafter load ok (why='{dr.why}')")
        if (!dr.ok) {
            delete dr
            return
        }
        var prompt <- encode(tr, "The capital of France is", true, false)
        let npos = long_length(prompt)
        t |> success(npos >= 4l, "the prompt is {npos} tokens")
        var s <- make_run_state(tr.config, KVDtype.f16, KVDtype.f16)
        // prefill everything but the last token, then ONE decode step: the drafter's anchor is a
        // decode position, and forward() is what leaves the trunk residual the hidden is read from
        var head <- [for (i in range64(npos - 1l)); prompt[i]]
        eval_(tr, s, head)
        forward(tr, s, prompt[npos - 1l], npos - 1l)
        let anchor = npos   // the seed's own position = n_past: it ropes there and sees keys [0, npos)
        let tgt_next = argmax_of(s.logits, tr.config.vocab_size)
        var h : array<float>
        gemma_target_hidden(tr, s, true, h)
        t |> success(all_finite(h) && long_length(h) == tr.config.dim, "the target hidden is {long_length(h)} finite floats")
        moe_gpu_hydrate_session(tr, s)   //! the oracle reads the host cache: a resident driver's device-only rows come down first
        var out : GemmaDraftOut
        gemma_draft_step_cpu(tr, s, dr, tgt_next, h, anchor, out)
        t |> success(all_finite(out.logits) && long_length(out.logits) == dr.vocab,
            "the draft logits are {long_length(out.logits)} finite floats")
        t |> success(out.token >= 0l && out.token < dr.vocab, "the draft token {out.token} is in [0, {dr.vocab})")
        t |> success(long_length(out.h_next) == dr.tdim && all_finite(out.h_next),
            "h_next is {long_length(out.h_next)} finite floats")
        var hn_rms = 0.0
        for (v in out.h_next) {
            hn_rms += v * v
        }
        hn_rms = sqrt(hn_rms / float(dr.tdim))
        t |> success(hn_rms > 0.0, "h_next carries signal (rms {hn_rms})")
        to_log(LOG_INFO, "gdraft: anchor {anchor} seed '{eyeball_str(piece(tr, tgt_next))}' ({tgt_next}) -> draft '{eyeball_str(piece(tr, out.token))}' ({out.token}), h_next rms {hn_rms}\n")
        delete out
        delete h
        delete head
        delete prompt
        delete s
        delete dr
    }
}

// ===== the GPU home: the served target, its drivers, the served rounds =====

//! the GPU home's overrides for the served cells: the Metal prefill and decode, every prompt length on the device
def private gdraft_pin() : MetalOverridesSeat {
    let seat = metal_overrides_seat()
    static_if (typeinfo builtin_module_exists(das_metal)) {
        select_prefill_override("metal")
        select_decode_override("metal")
        set_metal_prefill_min_npos(1l)
    }
    return seat
}

//! the GPU home's drivers down after a cell, and its leak gate
def private gdraft_shutdown(t : T?) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        metal_drivers_down()
        t |> equal(metal_live_object_count(), 0l, "no leaked Metal objects after shutdown")
    }
}

//! the trunk's device counts: prompts prefilled, decode steps (single-stream and batched) and the Metal drivers' declines
struct private TrunkCounts {
    prefills : int64
    steps : int64
    declines : int64
}

def private trunk_counts() : TrunkCounts {
    var m : TrunkCounts
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let d = metal_decode_stats()
        let b = metal_batch_decode_stats()
        let p = metal_prefill_stats()
        m = TrunkCounts(prefills = p.prefills, steps = d.decodes + b.steps, declines = d.declines + b.declines + p.declines)
    } else {
        m.prefills = gpu_resident_prefills_()
    }
    return m
}

//! the trunk's calls since `m0` served on the device: a prefill and decode steps the Metal drivers counted and none declined, or
//! a resident prefill and no call the Vulkan driver passed to the CPU rails
def private trunk_served_since(t : T?; m0 : TrunkCounts; tag : string) {
    let m1 = trunk_counts()
    t |> success(m1.prefills > m0.prefills, "{tag}: the trunk's prompt prefilled on the device ({m1.prefills - m0.prefills})")
    static_if (typeinfo builtin_module_exists(das_metal)) {
        t |> success(m1.steps > m0.steps, "{tag}: the trunk's steps decoded on the device ({m1.steps - m0.steps})")
        t |> equal(m1.declines, m0.declines, "{tag}: no trunk call declined to the CPU")
    } else {
        served_every_call(t)
    }
}

//! one scheduler's run over `prompts`, `budget` tokens a stream on `kv` sessions, speculating or plain: each stream's tokens and
//! the scheduler's speculative tallies
struct private SchedRun {
    toks : array<array<int64>>
    drafted : int64
    accepted : int64
    ticks : int64
    joined_max : int64
    mtp : bool
}

[cold_path]
def private sched_run(t : T?; trg : Model; prompts : array<array<int64>>; budget : int64; kv : KVDtype; spec : bool) : SchedRun {
    var run : SchedRun
    var ev : array<SchedEvent>
    let m0 = trunk_counts()
    var sch <- create_scheduler(trg, long_length(prompts), 4l, 0l, 64l, 0l, kv, spec)
    run.mtp = sch.mtp
    for (i in range64(long_length(prompts))) {
        submit(trg, sch, make_req(i + 1l, prompts[i], budget))
    }
    drain(trg, sch, ev)
    trunk_served_since(t, m0, spec ? "the speculating scheduler" : "the plain scheduler")
    run.toks |> reserve(long_length(prompts))
    for (i in range64(long_length(prompts))) {
        run.toks |> emplace(toks_of(ev, i + 1l))
    }
    run.drafted = sch.mtp_drafted
    run.accepted = sch.mtp_accepted
    run.ticks = sch.decode_steps
    run.joined_max = sch.mtp_joined_max
    delete sch
    delete ev
    return <- run
}

//! the counting prompts, one a stream, in the parity fixture's instruction form ("Count from 1 to 60, separated by commas: 1, 2, ..., 23,"):
//! a bare count leaves the list at a near-tie on the 26B within six numbers, where two forms' rounding part the streams; each stream
//! opens at another number, so every stream walks its own count
def private counting_prompts(trg : Model; n : int64) : array<array<int64>> {
    var out : array<array<int64>>
    out |> reserve(n)
    for (i in range64(n)) {
        let start = 1 + int(i) * 100
        out |> emplace(encode(trg, "Count from {start} to {start + 59}, separated by commas: {counting_prompt(start, start + 22)}", true, false))
    }
    return <- out
}

//! the speculating streams token for token the plain streams' (both decoded in the log), every stream its whole budget
def private same_streams(t : T?; trg : Model; tag : string; plain, spec : SchedRun; budget : int64) {
    for (i in range64(long_length(plain.toks))) {
        t |> equal(long_length(spec.toks[i]), budget, "{tag}: stream {i} emits its whole budget")
        log_gen_texts(trg, "{tag} stream {i} (plain, speculating)", plain.toks[i], spec.toks[i])
        t |> success(ids_equal(spec.toks[i], plain.toks[i]), "{tag}: stream {i} walks the counting prompt as the plain scheduler does")
    }
}

//! `body` with the drafter beside `trg`'s file `file` attached to the served `trg`, detached after; skips where the card holds
//! the trunk only with routed layers' experts on the host
def private gdraft_attached(t : T?; var trg : Model; file : string; body : block<(trg : Model) : void>) {
    if (gpu_resident_host_layers() > 0l) {   // the round's verify rides the N-row command, which has no host-experts form on either discrete home
        t |> skip("the card holds the trunk only with {gpu_resident_host_layers()} routed layers' experts on the host: the drafter's verify has no host form")
        return
    }
    let side = mtp_drafter_sidecar(path_join(models_dir(), file))
    t |> success(!mtp_capable(trg), "a gemma-4 target with no drafter attached cannot speculate")
    t |> success(attach_mtp_drafter(trg, side), "the facade attaches the drafter on the GPU home")
    t |> success(mtp_capable(trg), "and the target speculates from here on")
    invoke(body, trg)
    detach_mtp_drafter(trg)
    t |> success(!mtp_capable(trg), "detached, the target cannot speculate")
}

//! the drafter beside `file` present and on the tier: false after the skip that says why
def private gdraft_sidecar_ready(t : T?; file : string) : bool {
    let side = mtp_drafter_sidecar(path_join(models_dir(), file))
    if (empty(side)) {
        t |> skip("no mtp-* drafter sidecar beside {file}")
        return false
    }
    return model_available(t, side)
}

//! `cells` on a served form of a trunk's block: on the Metal form (the file's served image) with the Metal mode off and the
//! overrides pinned as a served twin's cells run (`gdraft_pin`), then the overrides back, the drivers down and the leak gate
//! (the block puts the home back after the form); on a discrete home's form `cells` alone
def private with_served_pins(t : T?; form : ModelForm; cells : block<void>) {
    if (!form.on_metal) {
        invoke(cells)
        return
    }
    set_metal_mode(MetalMode.off)
    let pins = gdraft_pin()
    invoke(cells)
    restore_metal_overrides(pins)
    gdraft_shutdown(t)
}

//! the Q8_0-fed oracle's own flip envelope: against itself with its hidden 1e-5 to 1e-4 apart it moves 0.38-0.53 (h_next 0.50-0.62)
let private VK_FEED_FLIP_BAR = 1.0

//! the discrete home's one draft step on the resident driver: the CUDA home's under DASLLAMA_CUDA, else the Vulkan home's
def private dev_draft_step(trg : Model; var s : Session; tok, anchor : int64; h : array<float>; var out_tok : int64&; var hout : array<float>) : bool {
    static_if (typeinfo builtin_module_exists(cuda)) {
        if (gpu_want_cuda()) {
            return cud_gemma_draft_step(trg, s, tok, anchor, h, out_tok, hout)
        }
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return vk_gemma_draft_step(trg, s, tok, anchor, h, out_tok, hout)
    } else {
        return false
    }
}

//! the last device draft step's full-vocab logits on the home `dev_draft_step` ran
def private dev_drafter_last_logits(var lg : array<float>) {
    static_if (typeinfo builtin_module_exists(cuda)) {
        if (gpu_want_cuda()) {
            cud_gemma_drafter_last_logits(lg)
            return
        }
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        vk_gemma_drafter_last_logits(lg)
    }
}

//! the device step over `s`'s host rows brought up again - another session's device prefill takes the one region first - with
//! layer `l`'s K and V rows at `row` held at `v` for the step and put back after (`row` < 0 touches none): the step's logits
def private vk_reuploaded_draft(t : T?; trg : Model; var s : Session; prompt : array<int64>; seed, anchor : int64; h : array<float>;
                                l, row : int64; v : float) : array<float> {
    var lg : array<float>
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let c = trg.config
        let kvd = layer_kv_dim(c, l)
        let prow = row >= 0l ? kv_phys_row(s, row) : 0l
        var kept_k, kept_v, fill : array<float>
        if (row >= 0l) {
            kept_k |> resize(kvd)
            kept_v |> resize(kvd)
            fill |> resize(kvd)
            for (f in fill) {
                f = v
            }
            unsafe {
                kv_load_row(kv_layer_kbase(s, l, c.seq_len), s.kv_dtype_k, kvd, prow, 0l, kvd, addr(kept_k[0]))
                kv_load_row(kv_layer_vbase(s, l, c.seq_len), s.kv_dtype_v, kvd, prow, 0l, kvd, addr(kept_v[0]))
                kv_store_row(kv_layer_kbase(s, l, c.seq_len), s.kv_dtype_k, kvd, prow, 0l, kvd, addr<float const?>(fill[0]))
                kv_store_row(kv_layer_vbase(s, l, c.seq_len), s.kv_dtype_v, kvd, prow, 0l, kvd, addr<float const?>(fill[0]))
            }
        }
        var s2 <- make_run_state(c, s.kv_dtype_k, s.kv_dtype_v)
        var p40 <- [for (i in range64(min(40l, long_length(prompt)))); prompt[i]]
        let pf0 = gpu_resident_prefills_()
        eval_(trg, s2, p40)
        t |> success(gpu_resident_prefills_() > pf0, "the stealing session prefilled on the device, so the next step brings the host rows up")
        var dg = -1l
        var hg : array<float>
        hg |> resize(long_length(h))
        t |> success(dev_draft_step(trg, s, seed, anchor, h, dg, hg), "the device draft step over the brought-up rows served")
        dev_drafter_last_logits(lg)
        if (row >= 0l) {
            unsafe {
                kv_store_row(kv_layer_kbase(s, l, c.seq_len), s.kv_dtype_k, kvd, prow, 0l, kvd, addr<float const?>(kept_k[0]))
                kv_store_row(kv_layer_vbase(s, l, c.seq_len), s.kv_dtype_v, kvd, prow, 0l, kvd, addr<float const?>(kept_v[0]))
            }
        }
        delete hg
        delete p40
        delete s2
        delete fill
        delete kept_v
        delete kept_k
    }
    return <- lg
}

//! the Vulkan window witness by the rows: the same host rows brought up read as the device's own, the sliding capture layer's key one
//! past the window poisoned moves nothing, and its window's first key poisoned moves the draft (the control)
def private vk_window_witness(t : T?; trg : Model; dr : GemmaDrafter; var s : Session; prompt : array<int64>; seed, anchor : int64;
                              h, lg : array<float>; tag : string) {
    var ls = -1l
    for (l in range64(dr.n_layers)) {
        if (ls < 0l && dr.sliding[l]) {
            ls = dr.kv_layer[l]
        }
    }
    let ro = anchor - dr.sliding_window
    var base <- vk_reuploaded_draft(t, trg, s, prompt, seed, anchor, h, ls, -1l, 0.0)
    let dbase = logits_maxdiff(base, lg)
    log_logits_pair(trg, "gdraft gpu {tag}: the brought-up rows' draft against the device's own", base, lg, dbase)
    t |> equal(dbase, 0.0, "{tag}: the host rows brought up read as the device's own, bit for bit")
    var po <- vk_reuploaded_draft(t, trg, s, prompt, seed, anchor, h, ls, ro, 4.0)
    var pi <- vk_reuploaded_draft(t, trg, s, prompt, seed, anchor, h, ls, ro + 1l, 4.0)
    let dpo = logits_maxdiff(po, base)
    let dpi = logits_maxdiff(pi, base)
    log_logits_pair(trg, "gdraft gpu {tag}: key {ro} past the window poisoned, against the clean draft", po, base, dpo)
    log_logits_pair(trg, "gdraft gpu {tag}: the window's first key {ro + 1l} poisoned, against the clean draft", pi, base, dpi)
    t |> equal(dpo, 0.0, "{tag}: layer {ls}'s key {ro}, one past the sliding window, moves nothing")
    t |> success(dpi > 0.01, "{tag}: the window's first key {ro + 1l} moves the draft (maxd {dpi}) - the control")
    //! the prefill counter's control: a prompt the CPU rails read under the server's prefill pin counts no device prefill
    let pin = resident_prefill_allowed()
    set_resident_prefill_allowed(false)
    var s3 <- make_run_state(trg.config, s.kv_dtype_k, s.kv_dtype_v)
    var p40 <- [for (i in range64(min(40l, long_length(prompt)))); prompt[i]]
    let pf0 = gpu_resident_prefills_()
    eval_(trg, s3, p40)
    t |> equal(gpu_resident_prefills_(), pf0, "{tag}: a prompt read on the CPU rails under the prefill pin counts no device prefill - the re-upload's control")
    set_resident_prefill_allowed(pin)
    delete p40
    delete s3
    delete pi
    delete po
    delete base
}

// the GPU drafter step against the CPU oracle on the served twin: same seed token, same carried hidden, same anchor, so the two
// agree on the argmax and sit within the home's feed rounding - a Q8_0 feed at a rounding tie lands one level over when the sums
// differ by ulps, and the levels such ties flip move the 26B's draft by up to half a logit (`VK_FEED_FLIP_BAR`)
[cold_path]
def private gpu_step_vs_oracle(t : T?; trg : Model; var dr : GemmaDrafter; prompt : array<int64>; tag : string; window_control : bool) {
    let npos = long_length(prompt)
    var s <- make_run_state(trg.config, KVDtype.f16, KVDtype.f16)
    var head <- [for (i in range64(npos - 1l)); prompt[i]]
    let m0 = trunk_counts()
    eval_(trg, s, head)
    var one <- [prompt[npos - 1l]]
    eval_(trg, s, one)   // one GPU decode step: lands the post-norm hidden the drafter feeds on
    static_if (typeinfo builtin_module_exists(das_metal)) {
        finish_pending_step()
    }
    trunk_served_since(t, m0, "{tag}: the trunk under the draft")
    t |> equal(s.mtp_h_pos1, s.n_past, "{tag}: the decode carried the post-norm hidden for the drafter")
    let anchor = s.n_past
    let seed = argmax_of(s.logits, trg.config.vocab_size)
    var h := s.mtp_h
    var dg = -1l
    var hg : array<float>
    hg |> resize(dr.tdim)
    var lg : array<float>
    static_if (typeinfo builtin_module_exists(das_metal)) {
        t |> success(gemma_draft_step_gpu(trg, s, seed, anchor, h, dg, hg), "{tag}: the GPU draft step served")
        gemma_drafter_last_logits(lg)
    } else {
        t |> success(dev_draft_step(trg, s, seed, anchor, h, dg, hg), "{tag}: the device draft step served")
        dev_drafter_last_logits(lg)
    }
    let q8 = !(typeinfo builtin_module_exists(das_metal)) && (typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda))   // the oracle's GEMVs read the device's rows: the discrete homes' are Q8_0 activation rows, Metal's f32
    moe_gpu_hydrate_session(trg, s)   //! the oracle reads the host cache: the resident driver's device-only rows come down first
    var out : GemmaDraftOut
    gemma_draft_step_cpu(trg, s, dr, seed, h, anchor, out, q8)
    let dl = logits_maxdiff(lg, out.logits)
    let dh = logits_maxdiff(hg, out.h_next)
    t |> equal(out.token, dg, "{tag}: GPU and CPU drafts agree on the token")
    let lbar = q8 ? VK_FEED_FLIP_BAR : 0.25
    let hbar = q8 ? VK_FEED_FLIP_BAR : 0.05
    t |> success(dl <= lbar, "{tag}: draft logits within {lbar} of the oracle (maxd {dl})")
    t |> success(dh <= hbar, "{tag}: h_next within {hbar} of the oracle (maxd {dh})")
    let gpu_piece = dg >= 0l ? eyeball_str(piece(trg, dg)) : "no draft"
    to_log(LOG_INFO, "gdraft gpu {tag}: anchor {anchor} seed {seed} '{eyeball_str(piece(trg, seed))}' -> gpu {dg} '{gpu_piece}' cpu {out.token} '{eyeball_str(piece(trg, out.token))}'; h_next maxd {dh}\n")
    log_logits_pair(trg, "gdraft gpu {tag}: the device draft's logits against the oracle's", lg, out.logits, dl)
    {   //! the bars' control: the oracle drafting off the next token id lands past both
        var other : GemmaDraftOut
        gemma_draft_step_cpu(trg, s, dr, (seed + 1l) % trg.config.vocab_size, h, anchor, other, q8)
        let dlo = logits_maxdiff(lg, other.logits)
        let dho = logits_maxdiff(hg, other.h_next)
        log_logits_pair(trg, "gdraft gpu {tag}: the device draft's logits against the oracle off another seed", lg, other.logits, dlo)
        t |> success(dlo > lbar, "{tag}: the oracle off another seed token reads past the logits bar ({dlo}) - the control")
        t |> success(dho > hbar, "{tag}: the oracle off another seed token reads past the h_next bar ({dho}) - the control")
        delete other
    }
    if (window_control && q8) {
        vk_window_witness(t, trg, dr, s, prompt, seed, anchor, h, lg, tag)
    } elif (window_control) {   //! the control: the oracle's sliding layers one key wider - a device pass over the whole window reads as it does
        dr.sliding_window++
        var wide : GemmaDraftOut
        gemma_draft_step_cpu(trg, s, dr, seed, h, anchor, wide, q8)
        dr.sliding_window--
        let dlw = logits_maxdiff(lg, wide.logits)
        t |> success(dl * 8.0 < dlw, "{tag}: the device's sliding layers attend the oracle's window - its logits sit far nearer the oracle ({dl}) than one a key wider ({dlw})")
        to_log(LOG_INFO, "gdraft gpu {tag}: the one-key-wider oracle reads logits maxd {dlw}\n")
        delete wide
    }
    delete out
    delete lg
    delete hg
    delete h
    delete one
    delete head
    delete s
}

//! the GPU drafter step against the CPU oracle on the served `trg`, at a short anchor and one past the sliding window
def private gdraft_gpu_step_cell(t : T?; trg : Model) {
    var dr <- load_gemma_drafter(mtp_drafter_sidecar(path_join(models_dir(), TARGET_FILE)), trg.config.n_layers)
    t |> success(dr.ok, "the oracle's drafter copy loads (why='{dr.why}')")
    var short <- encode(trg, "The capital of France is", true, false)
    gpu_step_vs_oracle(t, trg, dr, short, "short", false)
    var long <- encode(trg, counting_prompt(1, 300), true, false)
    t |> success(long_length(long) > dr.sliding_window + 8l, "the long prompt ({long_length(long)} tokens) passes the sliding window ({dr.sliding_window})")
    gpu_step_vs_oracle(t, trg, dr, long, "past the window", true)
    delete long
    delete short
    delete dr
}

//! the scheduler at depth 1 through the drafter attached to `trg` against the plain scheduler, then the drafter detached (the control)
def private gdraft_served_cell(t : T?; trg : Model) {
    let depth_pin = mtp_depth_pin()
    set_mtp_depth(1l)
    var prompts <- counting_prompts(trg, 1l)
    var plain <- sched_run(t, trg, prompts, 24l, KVDtype.f16, false)
    let st0 = mtp_drafter_stats(trg)
    var spec <- sched_run(t, trg, prompts, 24l, KVDtype.f16, true)
    let st1 = mtp_drafter_stats(trg)
    t |> success(spec.mtp, "a scheduler told to speculate keeps it on for the drafter's model")
    same_streams(t, trg, "gdraft sched", plain, spec, 24l)
    t |> success(spec.drafted > 0l && spec.accepted > 0l, "the rounds drafted and accepted ({spec.accepted} of {spec.drafted})")
    t |> equal(st1.drafted - st0.drafted, spec.drafted, "every draft was the device drafter's: its counter rose by the scheduler's drafts")
    t |> equal(st1.drafted - st0.drafted, st1.rounds - st0.rounds, "depth 1: one draft a round")
    to_log(LOG_INFO, "gdraft sched: {spec.accepted} of {spec.drafted} drafts accepted over {spec.ticks} ticks for 24 tokens\n")
    detach_mtp_drafter(trg)   //! the control: the same speculating scheduler with the drafter detached
    var off <- sched_run(t, trg, prompts, 24l, KVDtype.f16, true)
    t |> success(!off.mtp, "a scheduler told to speculate stands down with the drafter detached")
    t |> equal(off.drafted, 0l, "and drafts nothing")
    same_streams(t, trg, "gdraft sched detached", plain, off, 24l)
    delete off
    restore_mtp_depth(depth_pin)
    delete plain
    delete spec
    delete prompts
}

//! a session on codec `kv` over `prompt`, prefilled and one decode step past it (the step lands the drafter's carry)
def private stepped_session(trg : Model; prompt : array<int64>; kv : KVDtype) : Session {
    var s <- make_run_state(trg.config, kv, kv)
    let np = long_length(prompt)
    var head <- [for (i in range64(np - 1l)); prompt[i]]
    var last <- [prompt[np - 1l]]
    eval_(trg, s, head)
    eval_(trg, s, last)
    static_if (typeinfo builtin_module_exists(das_metal)) {
        finish_pending_step()
    }
    delete last
    delete head
    return <- s
}

//! on the Metal home a joint verify over two streams on two K/V codecs carries no stream; one codec across both carries both
def private gdraft_verify_decline_cell(t : T?; trg : Model) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let depth_pin = mtp_depth_pin()
        set_mtp_depth(1l)
        var prompt <- encode(trg, "Count from 1 to 60, separated by commas: {counting_prompt(1, 23)}", true, false)
        var probe <- make_run_state(trg.config, KVDtype.f16, KVDtype.f16)
        let slice = mirror_arena_demand(trg, probe, long_length(prompt) + 2l)
        delete probe
        //! room for six f16 slices: the tick's two streams beside the dead sessions' slices the LRU keeps - an arena that holds
        //! fewer evicts a live stream's mirror, and a stream with none steps plain before any verify
        t |> success(mirror_reserve(6ul * slice, slice), "the mirror arena holds six slices")
        for (mixed in [false, true]) {
            var inscope kvs <- [KVDtype.f16, mixed ? KVDtype.q8_0 : KVDtype.f16]
            var sa <- stepped_session(trg, prompt, kvs[0])
            var sb <- stepped_session(trg, prompt, kvs[1])
            var spec <- [unsafe(addr(sa)), unsafe(addr(sb))]
            var toks <- [for (sp in spec); argmax_of(sp.logits, trg.config.vocab_size)]
            for (sp in spec) {
                t |> success(mirror_stands(*sp, sp.n_past, sp.n_past + 2l), "{kvs}: the stream's mirror holds its rows - the round finds it warm")
            }
            var acc : array<int64>
            let st0 = mtp_drafter_stats(trg)
            let carried = mtp_spec_eval_batch(trg, spec, toks, acc)
            finish_pending_step()
            let st1 = mtp_drafter_stats(trg)
            var pa <- stepped_session(trg, prompt, kvs[0])   //! after the tick: the twins' mirrors take no slot the tick's streams hold
            var pb <- stepped_session(trg, prompt, kvs[1])
            var plain <- [unsafe(addr(pa)), unsafe(addr(pb))]
            to_log(LOG_INFO, "gdraft vdecl {kvs}: carried {carried}, rounds +{st1.rounds - st0.rounds}, draft_us +{st1.draft_us - st0.draft_us}, carry rows {sa.mtp_h_pos1}/{sb.mtp_h_pos1} at {sa.n_past}/{sb.n_past}\n")
            if (!mixed) {   //! the control: one codec across the streams, the joint verify carries both
                t |> equal(carried, 2l, "one codec: the joint verify carries both streams")
                t |> equal(st1.rounds - st0.rounds, 2l, "one codec: both rounds counted")
            } else {
                t |> equal(carried, 0l, "two codecs: the verify carries no stream")
                t |> equal(st1.rounds - st0.rounds, 0l, "two codecs: no round counted")
                t |> success(st1.verify_us == st0.verify_us, "two codecs: the declined verify's rounds charge no verify time - the streams' chains ride the verify's command buffer and never ran")
                for (i in range64(2l)) {
                    t |> equal(acc[i], 0l, "stream {i}: no draft accepted")
                    var one <- [toks[i]]
                    eval_(trg, *plain[i], one)
                    finish_pending_step()
                    delete one
                    t |> equal(spec[i].n_past, plain[i].n_past, "stream {i} ({kvs[i]}): one plain step past the prompt")
                    let dsp = logits_maxdiff(spec[i].logits, plain[i].logits)
                    log_logits_pair(trg, "gdraft vdecl stream {i} ({kvs[i]}): the declined tick's step against a plain decode step", spec[i].logits, plain[i].logits, dsp)
                    t |> equal(dsp, 0.0, "stream {i} ({kvs[i]}): the step's logits bit for bit a plain decode step's")
                }
            }
            spec |> clear()   //! borrowed handles - the sessions are the locals'
            plain |> clear()
            delete toks
            delete acc
            delete sa
            delete sb
            delete pa
            delete pb
        }
        restore_mtp_depth(depth_pin)
        delete prompt
    }
}

//! the speculating scheduler at drafter depths 3 and 1 on `trg` against the plain one, and a reasoning budget's forced close
def private gdraft_depth_cell(t : T?; trg : Model) {
    var prompts <- counting_prompts(trg, 1l)
    let np = long_length(prompts[0])
    t |> success(np / 64l < (np + 32l) / 64l, "the walk's rounds cross a 64-row page ({np} prompt rows): the round grows a paged session's pages")
    var plain <- sched_run(t, trg, prompts, 32l, KVDtype.f16, false)
    let depth_pin = mtp_depth_pin()
    for (k in [3l, 1l]) {
        set_mtp_depth(k)
        let st0 = mtp_drafter_stats(trg)
        var spec <- sched_run(t, trg, prompts, 32l, KVDtype.f16, true)
        let st1 = mtp_drafter_stats(trg)
        same_streams(t, trg, "gdraft depth {k}", plain, spec, 32l)
        let rounds = st1.rounds - st0.rounds
        t |> success(rounds > 0l, "depth {k}: the tick ran device rounds ({rounds})")
        let drafted = st1.drafted - st0.drafted
        t |> success(drafted > k * (rounds - 1l) && drafted <= k * rounds, "depth {k}: every round drafted {k} tokens but a last round the 32-token budget clips ({drafted} over {rounds} rounds; the control is depth 1's one a round)")
        t |> success(spec.accepted > 0l, "depth {k}: drafts were accepted ({spec.accepted} of {spec.drafted})")
        to_log(LOG_INFO, "gdraft depth {k}: {spec.accepted} of {spec.drafted} drafts over {rounds} rounds, {spec.ticks} ticks for 32 tokens\n")
        delete spec
    }
    //! a reasoning budget that can still force a close holds the stream's rounds at depth 1: a draft past the close would be
    //! evaled and emitted ahead of it; the stand-in close is two tokens the count never writes. Four budgets in a row put the
    //! budget inside a depth-3 round whatever the rounds accept
    set_mtp_depth(3l)
    var close <- encode(trg, " THE END", false, false)
    for (budget in range64(5l, 9l)) {
        var budgeted : array<array<int64>>
        budgeted |> reserve(2)
        for (spec in [false, true]) {
            var ev : array<SchedEvent>
            var sch <- create_scheduler(trg, 1l, 4l, 0l, 64l, 0l, KVDtype.f16, spec)
            var req <- make_req(1l, prompts[0], 24l)
            req.think_budget = budget
            req.think_marks.opened = true
            req.think_marks.close := close
            t |> success(submit(trg, sch, req), "budget {budget}: the stream is queued (speculating {spec})")
            let m0 = trunk_counts()
            drain(trg, sch, ev)
            trunk_served_since(t, m0, "budget {budget} (speculating {spec})")
            budgeted |> emplace(toks_of(ev, 1l))
            delete sch
            delete ev
        }
        for (i in range64(long_length(close))) {
            t |> equal(budgeted[1][budget + i], close[i], "budget {budget}: the forced close's token {i} lands at the budget on the speculating stream")
        }
        log_gen_texts(trg, "gdraft budget {budget} (plain, speculating)", budgeted[0], budgeted[1])
        //! through the forced close: past it the streams free-run off a cut sentence, where a near-tie parts them on a card
        let through = budget + long_length(close)
        t |> success(ids_equal(subarray(budgeted[1], range64(0l, through)), subarray(budgeted[0], range64(0l, through))), "budget {budget}: the speculating stream reads as the plain one through the forced close")
        delete budgeted
    }
    delete close
    restore_mtp_depth(depth_pin)
    delete plain
    delete prompts
}

//! three speculating streams at depths 1 and 2 on `trg` ride one joint verify a tick; a lone stream the control
def private gdraft_joint_cell(t : T?; trg : Model) {
    var prompts <- counting_prompts(trg, 3l)
    var plain <- sched_run(t, trg, prompts, 24l, KVDtype.f16, false)
    let depth_pin = mtp_depth_pin()
    for (k in [1l, 2l]) {
        set_mtp_depth(k)
        let st0 = mtp_drafter_stats(trg)
        var spec <- sched_run(t, trg, prompts, 24l, KVDtype.f16, true)
        let st1 = mtp_drafter_stats(trg)
        same_streams(t, trg, "gdraft joint depth {k}", plain, spec, 24l)
        t |> equal(spec.joined_max, 3l, "depth {k}: a tick's verify carried all three streams' rows")
        t |> success(st1.joint > st0.joint, "depth {k}: the drafter counted joint verifies ({st1.joint - st0.joint})")
        t |> success(spec.accepted > 0l, "depth {k}: drafts were accepted ({spec.accepted} of {spec.drafted})")
        delete spec
    }
    //! the joint counter's control: one speculating stream at depth 1 verifies its rows alone
    set_mtp_depth(1l)
    var inscope solo <- counting_prompts(trg, 1l)
    let sj0 = mtp_drafter_stats(trg)
    var lone <- sched_run(t, trg, solo, 24l, KVDtype.f16, true)
    let sj1 = mtp_drafter_stats(trg)
    log_gen_texts(trg, "gdraft joint lone stream (plain, speculating)", plain.toks[0], lone.toks[0])
    t |> success(sj1.rounds > sj0.rounds, "one stream: the drafter ran rounds ({sj1.rounds - sj0.rounds})")
    t |> equal(sj1.joint - sj0.joint, 0l, "one stream: no joint verify counted - the counter's control")
    delete lone
    restore_mtp_depth(depth_pin)
    delete plain
    delete prompts
}

//! the drafter's rounds on `trg`'s `kv` mirror: speculating sessions walk the count as plain ones on the same codec
def private gdraft_codec_cell(t : T?; trg : Model; kv : KVDtype) {
    let depth_pin = mtp_depth_pin()
    set_mtp_depth(2l)
    var prompts <- counting_prompts(trg, 1l)
    var plain <- sched_run(t, trg, prompts, 24l, kv, false)
    let st0 = mtp_drafter_stats(trg)
    var spec <- sched_run(t, trg, prompts, 24l, kv, true)
    let st1 = mtp_drafter_stats(trg)
    same_streams(t, trg, "gdraft {kv}", plain, spec, 24l)
    t |> success(st1.rounds > st0.rounds, "{kv}: the rounds ran on the device ({st1.rounds - st0.rounds}), none cold for the codec")
    t |> success(spec.accepted > 0l, "{kv}: drafts were accepted ({spec.accepted} of {spec.drafted})")
    restore_mtp_depth(depth_pin)
    delete plain
    delete spec
    delete prompts
}

//! one ring pin's run of the past-ring cell: the one-row chain's logits at the draft's anchor, the drafter's chain
//! there (its tokens, its first step's logits), the ring rows of the first sliding capture layer, and the scheduler's speculating
//! stream with its tallies
struct private RingDraftRun {
    ok : bool
    ring : int64
    count : int   //! the counting prompt's last number: sized off the ringed load's rows, the flat load's prompt the same
    anchor : int64
    pick : int64   //! the one-row chain's argmax at the anchor, the drafter's seed
    trunk : array<float>
    drafts : array<int64>
    dlogits : array<float>
    stream : SchedRun
    text : string   //! the stream decoded
}

let private RING_DRAFT_K = 3l      //! the drafter's chain depth at the anchor, and the scheduler's depth
let private RING_ACCEPT_BAND = 0.1   //! the ringed run's accepted drafts within a tenth of the flat run's
let private RING_PF_CHUNK = 512l   //! the prompt's prefill calls: a ring sized to its widest window stays under the prompt

//! the drafter's chain of `k` steps at `anchor` off `seed` and the carried `h`, every step at the constant anchor as the round runs
//! it: the tokens, and the first step's logits
def private gdraft_chain(t : T?; trg : Model; var s : Session; seed, anchor, k : int64; h : array<float>; var drafts : array<int64>;
                         var lg : array<float>) {
    var tok = seed
    var hin := h
    var hout : array<float>
    hout |> resize(long_length(h))
    drafts |> reserve(k)
    for (i in range64(k)) {
        var dg = -1l
        static_if (typeinfo builtin_module_exists(das_metal)) {
            t |> success(gemma_draft_step_gpu(trg, s, tok, anchor, hin, dg, hout), "draft step {i} served on the device")
            if (i == 0l) {
                gemma_drafter_last_logits(lg)
            }
        } else {
            static_if (typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda)) {
                t |> success(dev_draft_step(trg, s, tok, anchor, hin, dg, hout), "draft step {i} served on the device")
                if (i == 0l) {
                    dev_drafter_last_logits(lg)
                }
            } else {
                t |> skip("draft step {i}: no GPU home on this build for the gemma drafter")
            }
        }
        drafts |> push(dg)
        tok = dg
        hin := hout
    }
    delete hout
    delete hin
}

//! the first sliding capture layer's ring rows on the served load (0 where the layer keeps a row a position)
def private gdraft_capture_ring(trg : Model; dr : GemmaDrafter) : int64 {
    for (l in range64(dr.n_layers)) {
        continue if (!dr.sliding[l])
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return mirror_ring(trg, dr.kv_layer[l])
        } else {
            let slack = gpu_resident_kv_ring_slack()
            return slack > 0l ? round_up(trg.config.sliding_window + slack, 64l) : 0l
        }
    }
    return 0l
}

//! the drafter attached to the served `trg` (the caller's ring pin `ring`) over the long counting prompt: the prompt prefilled
//! in `RING_PF_CHUNK`-row calls, one one-row step, the drafter's chain at the position past it, then a speculating scheduler
//! stream over the same prompt; `count0` is the ringed run's count, which the flat run takes so both pins draft at one anchor
//! (0 sizes the prompt off this load's ring)
def private ring_draft_rows(t : T?; trg : Model; ring : int; budget : int64; count0 : int) : RingDraftRun {
    var run : RingDraftRun
    var dr <- load_gemma_drafter(mtp_drafter_sidecar(path_join(models_dir(), TARGET_FILE)), trg.config.n_layers)
    t |> success(dr.ok, "the drafter's geometry loads (why='{dr.why}')")
    let ring_rows = gdraft_capture_ring(trg, dr)
    //! the prompt reaches past the ring's rows on any card (a number is about four tokens) and stays under the rig's context
    let count = count0 > 0 ? count0 : clamp(int(ring_rows / 4l) + 150, 500, int((trg.config.seq_len - 300l) / 4l))
    run.count = count
    var prompt <- encode(trg, "Count from 1 to {count + 100}, separated by commas: {counting_prompt(1, count)}", true, false)
    let npos = long_length(prompt)
    var s <- make_run_state(trg.config, KVDtype.f16, KVDtype.f16)
    eval_chunked(trg, s, prompt, npos - 1l, RING_PF_CHUNK)
    var one <- [prompt[npos - 1l]]
    eval_(trg, s, one)   //! the one-row chain's step at the prompt's last position
    static_if (typeinfo builtin_module_exists(das_metal)) {
        finish_pending_step()
    }
    run.ring = ring_rows
    run.anchor = s.n_past
    run.trunk := s.logits
    t |> equal(s.mtp_h_pos1, s.n_past, "ring pin {ring}: the step carried the post-norm hidden for the drafter")
    run.pick = argmax_of(s.logits, trg.config.vocab_size)
    gdraft_chain(t, trg, s, run.pick, run.anchor, RING_DRAFT_K, s.mtp_h, run.drafts, run.dlogits)
    delete s
    let depth_pin = mtp_depth_pin()
    set_mtp_depth(RING_DRAFT_K)
    var prompts : array<array<int64>>
    prompts |> push_clone(prompt)
    run.stream <- sched_run(t, trg, prompts, budget, KVDtype.f16, true)
    restore_mtp_depth(depth_pin)
    run.text = decode(trg, run.stream.toks[0])
    run.ok = true
    delete prompts
    delete prompt
    delete one
    delete dr
    return <- run
}

//! `ring_draft_rows` on the Metal form's trunk `m` under ring pin `ring`, which the Metal mirror reads live: the drafter attached
//! for the run, the context the form's own - room for a prompt past the ring with the walk after it
def private ring_draft_run(t : T?; var m : Model; ring : int; budget : int64;
                           count0 : int = 0) : RingDraftRun {
    var run : RingDraftRun
    with_gpu_kv_ring(ring) {
        gdraft_attached(t, m, TARGET_FILE) $(trg) {
            delete run
            run <- ring_draft_rows(t, trg, ring, budget, count0)
        }
    }
    return <- run
}

//! the ringed run against the flat one: the ring's rows and the anchor past them, the trunk's and the drafter's logits at the
//! anchor (bit for bit off Metal, the pick on Metal), the drafts, and the two speculating streams with their acceptance
def private past_ring_within(t : T?; ringed, flat : RingDraftRun; budget : int64) {
    t |> success(ringed.ring > 0l && flat.ring == 0l, "the ringed load rings the sliding capture layer ({ringed.ring} rows), the flat load keeps a row a position ({flat.ring})")
    t |> success(ringed.anchor > ringed.ring, "the anchor {ringed.anchor} sits past the ring: the capture layer holds more keys than its ring's rows")
    t |> equal(ringed.anchor, flat.anchor, "both pins draft at one anchor")
    static_if (typeinfo builtin_module_exists(das_metal)) {
        //! Metal's AV pass splits a ringed layer's key run at the wrap, so the sums part in order: the picks hold, the bits do not
        t |> equal(ringed.pick, flat.pick, "the one-row chain's pick at the anchor: the ringed mirror's is the flat one's")
    } else {
        check_exact(t, "the one-row chain's logits at the anchor: the ringed mirror against the flat one, bit for bit", ringed.trunk, flat.trunk)
        check_exact(t, "the drafter's first step's logits at the anchor: the ringed mirror against the flat one, bit for bit", ringed.dlogits, flat.dlogits)
    }
    t |> success(ids_equal(ringed.drafts, flat.drafts), "the drafter's chain at the anchor drafts the flat mirror's tokens ({ringed.drafts} vs {flat.drafts})")
    to_log(LOG_INFO, "gdraft ring: anchor {ringed.anchor} past the {ringed.ring}-row ring; trunk logits ringed vs flat maxd {logits_maxdiff(ringed.trunk, flat.trunk)}, draft logits maxd {logits_maxdiff(ringed.dlogits, flat.dlogits)}\n")
    t |> equal(long_length(ringed.stream.toks[0]), budget, "the ringed stream emits its whole budget")
    to_log(LOG_INFO, "gdraft ring streams (flat, ringed):\n  {flat.text}\n  {ringed.text}\n")
    t |> success(ids_equal(ringed.stream.toks[0], flat.stream.toks[0]), "the ringed speculating stream walks the count as the flat one does")
    let acc_r = ringed.stream.accepted
    let acc_f = flat.stream.accepted
    to_log(LOG_INFO, "gdraft ring: accepted {acc_r} of {ringed.stream.drafted} ringed, {acc_f} of {flat.stream.drafted} flat\n")
    t |> success(acc_f > 0l, "the flat run accepts drafts ({acc_f} of {flat.stream.drafted})")
    t |> success(float(abs(acc_r - acc_f)) <= RING_ACCEPT_BAND * float(acc_f),
        "the ringed run accepts within {RING_ACCEPT_BAND} of the flat run's drafts ({acc_r} of {ringed.stream.drafted} vs {acc_f} of {flat.stream.drafted})")
}

//! the drafter on the UD-IQ3_XXS trunk `trg`, whose Q6_K token table the seed embed gathers: drafts accepted at depth 2, the plain stream's tokens
def private gdraft_kquant_cell(t : T?; trg : Model) {
    t |> equal(trg.emb_fmt, KqFmt.k6, "the trunk's token table is Q6_K (the K-quant seed gather)")
    let depth_pin = mtp_depth_pin()
    set_mtp_depth(2l)
    var prompts <- counting_prompts(trg, 1l)
    var plain <- sched_run(t, trg, prompts, 24l, KVDtype.f16, false)
    var spec <- sched_run(t, trg, prompts, 24l, KVDtype.f16, true)
    same_streams(t, trg, "gdraft iq3", plain, spec, 24l)
    t |> success(spec.drafted > 0l && spec.accepted > 0l, "the rounds drafted and accepted ({spec.accepted} of {spec.drafted})")
    restore_mtp_depth(depth_pin)
    delete plain
    delete spec
    delete prompts
}

// ===== the trunk's block: every form of gemma-4-26B-A4B Q4_K_M loaded once =====

let private G26_PPL_ARM = "g4mk"   //! the whole Q4_K_M file's perplexity walks' arm
//! the routed model's argmax slack: a router near-tie flips whole positions between the arms, and a kernel recompile alone
//! moves the hits - 122 to 128 against the CPU chain's 134 over the 150-position cell (the readings: CLAUDE.md)
let private G26_HITS_SLACK = 16l
//! the two-window cell's band: a k4 fold's rounding pattern alone moved it 1.70 -> 2.44 against the CPU chain's 1.03 (a router
//! near-tie flips whole positions), the 150 + 150 cell 5.51 -> 5.91 against 3.36
let private G26_PPL_RATIO_TWO_WINDOWS = 2.5
//! a whole file's walks: 150 prose ids prefilled and 150 teacher-forced, then 520 (two windows) and 80
let private G26_WALKS = fixed_array((n = 150l, steps = 150l), (n = 520l, steps = 80l))
let private IQ3_PPL_ARM = "g4m"   //! the whole UD-IQ3_XXS file's perplexity walks' arm
//! the UD-IQ3_XXS file's argmax slack: a near-tie at the router flips whole positions between the arms, and the CPU chain
//! sits farther from the reference exe on this model than the resident does (its own forced-feed logits are no instrument
//! here); the hits read 126 to 139 against the CPU chain's 133 across kernel recompiles (the readings: CLAUDE.md)
let private IQ3_HITS_SLACK = 12l
let private IQ3_CUDA_ARM = "cuda-g4m"   //! the CUDA home's arm of the whole UD-IQ3_XXS file's walk
let private G26_HOST_CUDA_ARM = "cuda-g4mk"   //! the CUDA home's arm of the whole Q4_K_M file's host-experts walk
let private G26_HOST_HITS_SLACK = 12l   //! the host-experts walk's slack: a router near-tie flips whole positions between the arms

let private CAPTURE_STEPS = 4l
let private CAPTURE_REL = 0.05   //! the two routes' sums part by up to 3.2% at the deepest layers (the routed experts' float order); a row short moves every slot 7% or more

//! the prompt-capture cell's rows: the CPU route's sums over the prompt and over the prompt a token short on the CPU form,
//! the Metal window's on the Metal form, and which of the two collected
struct private CaptureRows {
    cpu : array<float>
    cpu_short : array<float>
    dev : array<float>
    cpu_ran : bool
    dev_ran : bool
}

//! what the forward of `prompt` hands the drafter on `m`: the prompt commit's last row, a sum of magnitudes a captured layer,
//! off a speculative walk of CAPTURE_STEPS tokens with the drafter attached on the asked arm
def private prompt_capture_sums(t : T?; m : Model; prompt : array<int64>; device : bool;
                                tag : string) : array<float> {
    set_dflash_device(device)
    let why = attach_dflash_drafter(m, path_join(_model_tier::models_dir(), G26_DRAFTER_FILE))
    t |> equal(why, "", "{tag}: the drafter attaches")
    return <- array<float>() if (!empty(why))
    set_dflash_record(true)
    var s = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
    var nd = 0l
    var na = 0l
    let spec <- generate_mtp_greedy(m, s, prompt, long_length(prompt) - 1l + CAPTURE_STEPS, nd, na)
    t |> success(!empty(spec), "{tag}: the walk ran")
    var sums <- dflash_recorded_prompt_sums()
    set_dflash_record(false)
    detach_dflash_drafter()
    delete s
    return <- sums
}

//! the CPU route's capture sums into `r` on the CPU form `m`, the context cut to HOST_CTX and the Metal mode off
def private capture_cpu_rows(t : T?; var r : CaptureRows; var m : Model) {
    with_cpu_drafter_scope(m) {
        var inscope prompt <- encode(m, PROMPT, true)
        r.cpu <- prompt_capture_sums(t, m, prompt, false, "26B capture, CPU route")
        var inscope short_prompt <- [for (i in range(length(prompt) - 1)); prompt[i]]
        r.cpu_short <- prompt_capture_sums(t, m, short_prompt, false,
            "26B capture, CPU route a token short")
    }
    r.cpu_ran = true
}

//! the Metal window's capture sums into `r` on the Metal form's trunk `m` under the decode override Metal's, the context cut
//! to HOST_CTX; then the block drafter and the decode and prefill drivers down, the override put back, and no Metal object left
def private capture_metal_rows(t : T?; var r : CaptureRows; var m : Model) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        on_metal_dflash_trunk() {
            with_ctx_cut(m, HOST_CTX) {
                var inscope prompt <- encode(m, PROMPT, true)
                let pf0 = metal_prefill_stats().prefills
                r.dev <- prompt_capture_sums(t, m, prompt, true, "26B capture, Metal window")
                t |> success(metal_prefill_stats().prefills > pf0,
                    "the prompt ran as a device prefill window")
            }
        }
        t |> equal(metal_log_leaks_if_any(), 0l,
            "26B capture: no leaked Metal objects after shutdown")
        r.dev_ran = true
    }
}

//! what a trunk's forms collect for the compares after the block: the walks (`GemmaWalkRows`), the two ring runs
struct private G26TrunkRows {
    walks : GemmaWalkRows
    ringed : RingDraftRun
    flat : RingDraftRun
    host : array<GemmaRows>   //! the host-experts form's first walk (`host_ran` where it collected)
    host_ran : bool
    served : bool   //! the Metal form runs the assistant drafter's served cells: an arm of theirs is on and the sidecar stands
    prompt_rows : CaptureRows
}

//! the drafter forms' cells, one form each (the `drafter` tag on the form's knobs): each cell under its arm and the drafter
//! attached; `kv` the form's asked codec
def private g26_drafter_cells(t : T?; var r : G26TrunkRows; var m : Model; form : ModelForm) {
    unsafe {   //! the cells capture the loaded trunk and the block's collection by reference
        if (form.host) {
            t |> run("the GPU drafter step matches the CPU oracle (argmax, logits, h_next), at a short anchor and one past the sliding window") @capture(& m)(t : T ?) {
                return if (!arm_on(t, "mtp-gdraft-gpu"))
                with_ctx_cut(m, 2048l) {
                    gdraft_attached(t, m, TARGET_FILE) $(trg) {
                        gdraft_gpu_step_cell(t, trg)
                    }
                }
            }
        } elif (form.regions == 3l) {
            t |> run("three speculating streams ride one verify a tick and each walks its count as the plain scheduler's stream does") @capture(& m)(t : T ?) {
                return if (!arm_on(t, "mtp-gdraft-joint"))
                gdraft_attached(t, m, TARGET_FILE) $(trg) {
                    gdraft_joint_cell(t, trg)
                }
            }
        } elif (form.kv != KVDtype.f16) {
            t |> run("the drafter's rounds read a {form.kv} mirror: speculating sessions walk the count as plain ones on the same codec") @capture(& m)(t : T ?) {
                return if (!arm_on(t, "mtp-gdraft-codec"))
                gdraft_attached(t, m, TARGET_FILE) $(trg) {
                    gdraft_codec_cell(t, trg, form.kv)
                }
            }
        } elif (!form.ring) {
            if (empty(arm_why("mtp-gdraft-ring"))) {
                gdraft_attached(t, m, TARGET_FILE) $(trg) {
                    delete r.flat
                    r.flat <- ring_draft_rows(t, trg, 0, RING_BUDGET, r.ringed.count)
                }
            }
        } else {
            t |> run("a scheduler speculates through the attached drafter at depth 1 and matches the plain scheduler") @capture(& m)(t : T ?) {
                return if (!arm_on(t, "mtp-gdraft-sched"))
                with_ctx_cut(m, 512l) {
                    gdraft_attached(t, m, TARGET_FILE) $(trg) {
                        gdraft_served_cell(t, trg)
                    }
                }
            }
            t |> run("the scheduler's tick drafts at the depth knob's depth and walks the counting prompt as the plain scheduler does") @capture(& m)(t : T ?) {
                return if (!arm_on(t, "mtp-gdraft-depth"))
                with_ctx_cut(m, 512l) {
                    gdraft_attached(t, m, TARGET_FILE) $(trg) {
                        gdraft_depth_cell(t, trg)
                    }
                }
            }
            if (empty(arm_why("mtp-gdraft-ring"))) {
                gdraft_attached(t, m, TARGET_FILE) $(trg) {
                    delete r.ringed
                    r.ringed <- ring_draft_rows(t, trg, 1, RING_BUDGET, 0)
                }
            }
        }
    }
}

let private RING_BUDGET = 24l   //! the ring runs' speculating streams' tokens
let private G26_CAPTURE_ARM = "dflash-26b-capture"   //! the prompt-capture cell's arm

//! whether a form collects the prompt-capture cell's rows: its arm on, no skip registered, and the drafter file present
def private g26_capture_on(t : T?) : bool {
    return (empty(arm_why(G26_CAPTURE_ARM))
        && model_available(t, path_join(_model_tier::models_dir(), G26_DRAFTER_FILE)))
}
let private G26_METAL_CTX = 8192l   //! the Metal form's context: the ring cell's prompt past the ring; the other cells cut theirs

//! the served cells' arms on the Metal form
let private G26_METAL_ARMS = fixed_array("mtp-gdraft-gpu", "mtp-gdraft-sched", "mtp-gdraft-vdecl",
    "mtp-gdraft-depth", "mtp-gdraft-joint", "mtp-gdraft-codec", "mtp-gdraft-ring")

//! whether the drafter's sidecar stands for the trunk's served forms: asked on every build with no das_metal, and on a das_metal
//! build where a served arm is on
def private g26_sidecar_ready(t : T?) : bool {
    static_if (!typeinfo builtin_module_exists(das_metal)) {
        return gdraft_sidecar_ready(t, TARGET_FILE)
    } else {
        return any_arm_on(G26_METAL_ARMS) && gdraft_sidecar_ready(t, TARGET_FILE)
    }
}

//! the Metal form's served cells on its trunk `m`, the file's served image: each under its arm, the drafter attached for the
//! cell and detached after it, its context cut to the cell's own; both codecs' sessions and both ring pins ride the one load
def private g26_metal_cells(t : T?; var m : Model) {
    unsafe {   //! the cells capture the loaded trunk by reference
        t |> run("the GPU drafter step matches the CPU oracle (argmax, logits, h_next), at a short anchor and one past the sliding window") @capture(& m)(t : T ?) {
            return if (!arm_on(t, "mtp-gdraft-gpu"))
            with_ctx_cut(m, 2048l) {
                gdraft_attached(t, m, TARGET_FILE) $(trg) {
                    gdraft_gpu_step_cell(t, trg)
                }
            }
        }
        t |> run("a scheduler speculates through the attached drafter at depth 1 and matches the plain scheduler") @capture(& m)(t : T ?) {
            return if (!arm_on(t, "mtp-gdraft-sched"))
            with_ctx_cut(m, 512l) {
                gdraft_attached(t, m, TARGET_FILE) $(trg) {
                    gdraft_served_cell(t, trg)
                }
            }
        }
        t |> run("a tick whose joint verify the batch rail declines (two streams on two K/V codecs) steps every stream plain, as a plain decode step does") @capture(& m)(t : T ?) {
            return if (!arm_on(t, "mtp-gdraft-vdecl"))
            with_ctx_cut(m, 512l) {
                gdraft_attached(t, m, TARGET_FILE) $(trg) {
                    gdraft_verify_decline_cell(t, trg)
                }
            }
        }
        t |> run("the scheduler's tick drafts at the depth knob's depth and walks the counting prompt as the plain scheduler does") @capture(& m)(t : T ?) {
            return if (!arm_on(t, "mtp-gdraft-depth"))
            with_ctx_cut(m, 512l) {
                gdraft_attached(t, m, TARGET_FILE) $(trg) {
                    gdraft_depth_cell(t, trg)
                }
            }
        }
        t |> run("three speculating streams ride one verify a tick and each walks its count as the plain scheduler's stream does") @capture(& m)(t : T ?) {
            return if (!arm_on(t, "mtp-gdraft-joint"))
            with_ctx_cut(m, 512l) {
                gdraft_attached(t, m, TARGET_FILE) $(trg) {
                    gdraft_joint_cell(t, trg)
                }
            }
        }
        t |> run("the drafter's rounds read a q8_0 and a tq4 mirror: speculating sessions walk the count as plain ones on the same codec") @capture(& m)(t : T ?) {
            return if (!arm_on(t, "mtp-gdraft-codec"))
            with_ctx_cut(m, 512l) {
                for (kv in [KVDtype.q8_0, KVDtype.tq4]) {
                    gdraft_attached(t, m, TARGET_FILE) $(trg) {
                        gdraft_codec_cell(t, trg, kv)
                    }
                }
            }
        }
        t |> run("the drafter past a sliding capture layer's ring reads the rows the flat mirror holds: drafts, acceptance and the trunk's logits") @capture(& m)(t : T ?) {
            return if (!arm_on(t, "mtp-gdraft-ring"))
            var inscope ringed <- ring_draft_run(t, m, 1, RING_BUDGET)
            var inscope flat <- ring_draft_run(t, m, 0, RING_BUDGET, ringed.count)
            return if (!ringed.ok || !flat.ok)
            past_ring_within(t, ringed, flat, RING_BUDGET)
        }
    }
}

//! the trunk's CPU cells on the CPU form, its first codec: the drafter's CPU oracle step, the regular DFlash drafter's
//! lossless walk over a chat turn on the CPU arm (the context cut to HOST_CTX, the Metal mode off)
def private g26_cpu_cells(t : T?; var m : Model) {
    unsafe {   //! the cells capture the loaded trunk by reference
        t |> run("one CPU drafter step over the target's live K/V produces a finite token and hidden") @capture(& m)(t : T ?) {
            return if (!arm_on(t, "mtp-gdraft-cpu"))
            gdraft_cpu_step_cell(t, m)
        }
        t |> run("On the planar gemma-4-26B Q4_K_M trunk with its Q8_0 DFlash drafter on the CPU arm, over a chat turn, the greedy speculative walk emits the plain greedy walk's ids token for token with drafts accepted") @capture(& m)(t : T ?) {
            return if (!arm_on(t, "dflash-26b-cpu"))
            with_cpu_drafter_scope(m) {
                cpu_drafter_lossless(t, m, G26_DRAFTER_FILE, "26B cpu", false, true)
            }
        }
    }
}

//! the host-experts form's collection into `r`: the plan moved the first routed layers' experts to the host and kept the rest
//! on the device, the carrier's arms, then the first walk
def private g26_host_rows(t : T?; var r : G26TrunkRows; m : Model; form : ModelForm) {
    t |> success(moe_gpu_resident_active(), "the whole-model driver armed with host experts ({gpu_resident_decline_()})")
    let host = gpu_resident_host_layers()
    t |> success(host > 0l && host < m.config.n_layers, "the first {host} of {m.config.n_layers} routed layers' experts serve on the host, the rest on the device")
    t |> success(gemma_carrier_arms(m), GEMMA_CARRIER_ARMS)
    r.host |> emplace <| gemma_rows(m, form, array<int64>(), array<int64>(), [G26_WALKS[0]])
    r.host_ran = true
}

//! one form's collection and cells: the host-experts form's walk, the default form's and the CPU form's walks, the drafter
//! forms' cells, the DFlash form's legs, the CPU form's own cells
def private g26_trunk_rows(t : T?; var r : G26TrunkRows; var m : Model; form : ModelForm; walks : bool) {
    if (form.on_metal) {
        // under the form's required mode, ahead of the served cells' own pins
        if (empty(arm_why("dflash-26b-metal"))) {
            metal_drafter_cell(t, m, "On the Metal device arm over the gemma-4-26B Q4_K_M trunk the greedy DFlash walk over a chat turn is lossless, its block rounds run as joint rounds on the batch rail, and the CPU arm accepts the same count",
                "dflash-26b-metal", G26_DRAFTER_FILE, "26B metal", true)
        }
        if (g26_capture_on(t)) {
            capture_metal_rows(t, r.prompt_rows, m)
        }
        if (r.served) {
            with_served_pins(t, form) {
                g26_metal_cells(t, m)
            }
        }
        return
    }
    if (form.host_layers > 0l) {
        g26_host_rows(t, r, m, form)
        return
    }
    if (form.strict_ctx) {
        discrete_drafter_cell(t, m, "On the gemma-4-26B's resident trunk on the serving discrete home, the Q8_0 and Q4_K_M DFlash drafters' greedy walks on the device arm are the plain walk's on the same device",
            "dflash-26b-discrete", [G26_DRAFTER_FILE, G26_DRAFTER_K4_FILE], "26B discrete", false, true, false)
        return
    }
    if (form.kv_asked) {
        g26_drafter_cells(t, r, m, form)
        return
    }
    if (walks) {
        gemma_walk_rows(r.walks, m, form, G26_WALKS)
    }
    if (!form.on_gpu && form.alone) {
        g26_cpu_cells(t, m)
        if (g26_capture_on(t)) {
            capture_cpu_rows(t, r.prompt_rows, m)
        }
    }
}

//! walk `i`'s compare on the file `tag` at `ratio`: the device served the prompt as one window chain (and on the CUDA home
//! every position), its perplexity within `ratio` of the CPU chain's, its hits within `slack` of the CPU chain's
def private g26_walk_within(t : T?; tag : string; r : G26TrunkRows; i : int; ratio : float; slack : int64) {
    gemma_walk_twin_within(t, tag, r.walks, G26_WALKS, i, ratio, slack)
}

//! the 26B trunk block's forms, each asked only where an arm of its cells is on - the drafter forms also only where the
//! sidecar stands (`g26_sidecar_ready`); sets `r.served`, which the Metal form's cells read
def private g26_trunk_forms(t : T?; var r : G26TrunkRows;
                            walks, host_walk : bool) : array<ModelForm> {
    var forms : array<ModelForm>
    if (walks) {
        forms |> push(device_form())
    }
    if (host_walk) {
        forms |> push(device_form(host = true, host_layers = 1l, strict_ctx = true))
    }
    let sidecar = g26_sidecar_ready(t)
    static_if (!typeinfo builtin_module_exists(das_metal)) {
        if (sidecar) {
            let on = fixed_array(empty(arm_why("mtp-gdraft-gpu")), empty(arm_why("mtp-gdraft-sched")) || empty(arm_why("mtp-gdraft-depth")) || empty(arm_why("mtp-gdraft-ring")),
                empty(arm_why("mtp-gdraft-joint")), empty(arm_why("mtp-gdraft-codec")), empty(arm_why("mtp-gdraft-ring")))
            if (on[0]) {
                forms |> push(device_form(regions = 1l, host = true, host_ctx = 2048l) |> asking(KVDtype.f16))
            }
            if (on[1]) {
                forms |> push(device_form(regions = 1l, host_ctx = 8192l) |> asking(KVDtype.f16))
            }
            if (on[2]) {
                forms |> push(device_form(regions = 3l, host_ctx = 512l) |> asking(KVDtype.f16))
            }
            if (on[3]) {
                forms |> push(device_form(regions = 1l, host_ctx = 512l) |> asking(KVDtype.q8_0))
                forms |> push(device_form(regions = 1l, host_ctx = 512l) |> asking(KVDtype.tq4))
            }
            if (on[4]) {
                forms |> push(device_form(ring = false, regions = 1l, host_ctx = 8192l) |> asking(KVDtype.f16))
            }
        }
    }
    r.served = sidecar && any_arm_on(G26_METAL_ARMS)
    if (r.served || empty(arm_why("dflash-26b-metal")) || empty(arm_why(G26_CAPTURE_ARM))) {
        forms |> push(metal_form(image = true, ctx = G26_METAL_CTX))
    }
    static_if (typeinfo module_exists(dasllama_vulkan_dflash) || typeinfo module_exists(dasllama_cuda_dflash)) {
        if (empty(arm_why("dflash-26b-discrete"))) {
            forms |> push(discrete_drafter_form())
        }
    } else {
        t |> skip("26B discrete: no discrete GPU home in this build")
    }
    if (empty(arm_why("mtp-gdraft-cpu")) || empty(arm_why("dflash-26b-cpu"))
        || empty(arm_why(G26_CAPTURE_ARM))) {
        forms |> push(cpu_cells_form())
    }
    return <- forms
}

//! the prompt-capture compares: both routes recorded every captured layer, the Metal window's sums the CPU route's within
//! CAPTURE_REL, the short prompt's outside it (the bar's control)
def private capture_within(t : T?; r : CaptureRows) {
    t |> success(!empty(r.cpu) && length(r.cpu) == length(r.dev),
        "both routes recorded every captured layer ({length(r.cpu)} CPU, {length(r.dev)} device)")
    to_log(LOG_INFO,
        "26B prompt capture sums: CPU {r.cpu}, device {r.dev}, CPU a token short {r.cpu_short}\n")
    t |> equal(mismatch_rel("26B capture, device against CPU", r.dev, r.cpu, CAPTURE_REL), 0,
        "the device window's captured rows are the CPU route's")
    t |> equal(length(r.cpu_short), length(r.cpu),
        "the short prompt recorded every captured layer too")
    t |> success(all_finite(r.cpu_short) && all_finite(r.cpu),
        "every captured sum of both prompts is finite before the bar's control")
    let control = mismatch_rel("26B capture CONTROL(expected-red)", r.cpu_short, r.cpu, CAPTURE_REL)
    t |> success(control > 0,
        "a prompt one token short reads other rows (the bar's control)")
}

[test(description = "On the whole-model driver gemma-4-26B Q4_K_M with q51 down experts scores teacher-forced perplexity within 2x of the CPU chain over 150 positions and 2.5x over two windows, hits within 16; on the CUDA driver at the full rig context, the first routed layers' experts moved to the host, its 150-position perplexity within 2x through the split command and cut window chain, hits within 12; on the discrete homes' served trunk the GPU drafter step agrees with the CPU oracle within the Q8_0 feed's bars at a short anchor and past the sliding window, a scheduler speculating at depth 1 through the attached drafter emits the plain scheduler's counting tokens and stands down detached, drafter depths 3 and 1 draft k tokens a round across a page boundary with a reasoning budget's forced close in place, three streams ride one joint verify a tick, q8_0 and tq4 mirrors' rounds run on the device, and past a sliding capture layer's KV ring the drafter drafts the flat mirror's tokens bit for bit with the ringed stream within 10 percent of the flat one's acceptance; the Q8_0 and Q4_K_M DFlash drafters' greedy walks on the device arm are the plain walk's; on the CPU one drafter step over the target's live K/V yields a finite token and hidden, and the DFlash drafter's greedy speculative walk over a chat turn emits the plain walk's ids; on the parity row's CPU form, on every box, the file's frozen greedy ids to the half-span prefix bar, with its flipped-id control and its per-expert-prefill arm; on the Metal home's served image, one load on a das_metal build: the GPU drafter step on the served 26B agrees with the CPU oracle's draft token and sits within the home's logits and h_next bars (0.25 and 0.05 on Metal, 1.0 on a Q8_0 feed) at a short anchor and past the sliding window, an off-by-one seed landing outside; a scheduler speculating at depth 1 through the attached gemma-4 drafter emits the plain scheduler's counting tokens exactly, every draft counted by the device drafter, and with the drafter detached it stands down, drafts nothing, and still matches; on Metal a joint drafter verify over two streams on two KV codecs carries no stream and counts no round, each stream's step equal bit for bit to a plain decode step, while one codec across both carries both streams; at drafter depths 3 and 1 the speculating scheduler drafts k tokens a round and emits the plain scheduler's counting tokens exactly across a page boundary, and a reasoning budget lands its forced close at the budget with the plain stream's tokens; three speculating streams at depths 1 and 2 ride one joint verify a tick, each emitting the plain scheduler's counting tokens exactly, while a lone stream counts no joint verify (the counter's control); on q8_0 and tq4 KV mirrors the drafter's rounds run on the device, accept drafts, and the speculating scheduler emits the same codec's plain scheduler tokens exactly; past a sliding capture layer's KV ring the drafter at the same anchor drafts the flat mirror's tokens - logits bit for bit off Metal, the same pick on Metal - and the ringed stream matches the flat one with acceptance within 10 percent; the Metal form's prefill window hands the regular DFlash drafter the prompt's layer rows, every captured layer's sum over the prompt's last row within 5% of the CPU form's route, the CPU route's row for a prompt one token short outside it; each form loads once")]
def test_gemma_drafter_26b_trunk(t : T?) {
    return if (!family_on(t, "gemma4moe"))
    let host_walk = empty(cuda_home_why()) && empty(arm_why(G26_HOST_CUDA_ARM))
    let walks = empty(arm_why(G26_PPL_ARM)) || host_walk
    var r : G26TrunkRows
    var forms <- g26_trunk_forms(t, r, walks, host_walk)
    if (empty(forms)) {
        t |> skip("the arm filter keeps none of the trunk's cells (DASLLAMA_TEST_ARMS={g_env_test.test_arms})")
        delete forms
        return
    }
    let parity = empty(arm_why(G26_PPL_ARM))
    var ran = false
    with_cuda_mmq_winners() {
        ran = with_cpu_and_gpu_model(t, "llm", "gemma-4-26B-A4B-it-Q4_K_M.gguf", GEMMA_RIG_CTX, forms, walks, parity = parity) $(var m, form) {
            g26_trunk_rows(t, r, m, form, walks)
        }
    }
    if (ran) {
        held_within(t, r.walks.carrier, "gemma-4-26B-A4B Q4_K_M: ", false)
    }
    unsafe {   //! the cells read the block's collection by reference
        if (ran && walks) {
            t |> run("gemma-4-26B-A4B Q4_K_M: 150 prose ids prefilled, 150 teacher-forced, scored as a perplexity against the CPU chain - the q51 down experts on their own rail") @capture(& r)(t : T ?) {
                return if (!arm_on(t, G26_PPL_ARM))
                g26_walk_within(t, "gemma-4-26B-A4B Q4_K_M", r, 0, GEMMA_MOE_PPL_RATIO, G26_HITS_SLACK)
            }
            t |> run("gemma-4-26B-A4B Q4_K_M: 520 prose ids prefilled (two windows), 80 teacher-forced, scored as a perplexity against the CPU chain") @capture(& r)(t : T ?) {
                return if (!arm_on(t, G26_PPL_ARM))
                assert_pf_window(t)
                g26_walk_within(t, "gemma-4-26B-A4B Q4_K_M", r, 1, G26_PPL_RATIO_TWO_WINDOWS, G26_HITS_SLACK)
            }
        }
        if (ran && host_walk) {
            t |> run("gemma-4-26B-A4B Q4_K_M with the rig's context asked whole: the first routed layers' experts on the host, 150 prose ids prefilled, 150 teacher-forced, scored as a perplexity against the CPU chain - the split command and the cut window chain under the gemma-4 form") @capture(& r)(t : T ?) {
                return if (!cuda_home_asked(t))
                if (!r.host_ran) {
                    t |> failure("gemma-4-26B-A4B Q4_K_M: the host-experts form did not arm on the CUDA home - the pinned context and the routed layer asked onto the host decide the plan whatever the card")
                    return
                }
                return if (!twin_collected(t, 0, length(r.walks.cpu), "gemma-4-26B-A4B Q4_K_M, host experts: "))
                gemma_rows_within(t, "gemma-4-26B-A4B Q4_K_M, host experts", r.host[0], r.walks.cpu[0], array<int64>(), [G26_WALKS[0]],
                    GEMMA_MOE_PPL_RATIO, G26_HOST_HITS_SLACK)
            }
        }
        if (r.prompt_rows.cpu_ran) {
            t |> run("On gemma-4-26B, whose layers close on the gemma MoE step, the Metal prefill window hands the block drafter the prompt's layer rows: every captured layer's sum over the prompt's last row is the CPU route's within 5%, the CPU route's row for a prompt one token short outside it") @capture(& r)(t : T ?) {
                return if (!arm_on(t, G26_CAPTURE_ARM))
                if (!r.prompt_rows.dev_ran) {
                    t |> skip("26B capture: the Metal form did not serve the trunk "
                        + "(no das_metal in this build, or no metal-blob image)")
                    return
                }
                capture_within(t, r.prompt_rows)
            }
        }
        if (r.ringed.ok || r.flat.ok) {
            t |> run("the drafter past a sliding capture layer's ring reads the rows the flat mirror holds: drafts, acceptance and the trunk's logits") @capture(& r)(t : T ?) {
                return if (!arm_on(t, "mtp-gdraft-ring"))
                t |> success(r.ringed.ok && r.flat.ok, "both ring pins' runs collected (ringed {r.ringed.ok}, flat {r.flat.ok})")
                return if (!r.ringed.ok || !r.flat.ok)
                past_ring_within(t, r.ringed, r.flat, RING_BUDGET)
            }
        }
    }
    delete r
    delete forms
}

[test(description = "On the whole-model driver the routed gemma-4-26B UD-IQ3_XXS, held whole on the card, scores teacher-forced perplexity within 2x of the CPU chain's over one and two windows, hits within 12 - on the CUDA driver too over 150 positions; on the discrete homes the drafter attaches to the trunk whose Q6_K token table the seed embed gathers, accepts drafts at depth 2, and emits the plain scheduler's counting tokens exactly; on the parity row's CPU form, on every box, the file's frozen greedy ids token for token; on the Metal home's served image, one load on a das_metal build, the same attach, drafts and tokens; each form loads once")]
def test_gemma_drafter_26b_iq3_trunk(t : T?) {
    return if (!family_on(t, "gemma4moe"))
    let cuda_walk = empty(cuda_home_why()) && empty(arm_why(IQ3_CUDA_ARM))
    let walks = empty(arm_why(IQ3_PPL_ARM)) || cuda_walk
    var r : G26TrunkRows
    var forms : array<ModelForm>
    if (walks) {
        forms |> push(device_form())
    }
    if (empty(arm_why("mtp-gdraft-iq3")) && gdraft_sidecar_ready(t, IQ3_FILE)) {
        static_if (!typeinfo builtin_module_exists(das_metal)) {
            forms |> push(device_form(regions = 1l, host_ctx = 512l) |> asking(KVDtype.f16))
        } else {
            skip_cell(t, "{IQ3_FILE}, the f16 device form: a das_metal build serves it on the Metal form")
        }
        forms |> push(metal_form(image = true, ctx = 512l))
    }
    if (empty(forms)) {
        t |> skip("the arm filter keeps none of the UD-IQ3_XXS trunk's cells (DASLLAMA_TEST_ARMS={g_env_test.test_arms})")
        delete forms
        return
    }
    let parity = empty(arm_why(IQ3_PPL_ARM))
    var ran = false
    with_cuda_mmq_winners() {
        ran = with_cpu_and_gpu_model(t, "llm", "gemma-4-26B-A4B-it-UD-IQ3_XXS.gguf", GEMMA_RIG_CTX, forms, walks, parity = parity) $(var m, form) {
            if (form.kv_asked || form.on_metal) {
                with_served_pins(t, form) {
                    unsafe {   //! the cell captures the loaded trunk by reference
                        t |> run("the drafter attaches to the UD-IQ3_XXS trunk, whose Q6_K token table the seed embed gathers, and speculates") @capture(& m)(t : T ?) {
                            return if (!arm_on(t, "mtp-gdraft-iq3"))
                            gdraft_attached(t, m, IQ3_FILE) $(trg) {
                                gdraft_kquant_cell(t, trg)
                            }
                        }
                    }
                }
            } elif (walks) {
                gemma_walk_rows(r.walks, m, form, G26_WALKS)
            }
        }
    }
    if (ran && walks) {
        held_within(t, r.walks.carrier, "gemma-4-26B-A4B UD-IQ3_XXS: ", false)
        unsafe {   //! the cells read the block's collection by reference
            t |> run("gemma-4-26B-A4B IQ3_XXS: 150 prose ids prefilled, 150 teacher-forced, scored as a perplexity against the CPU chain - the routed block's gemma-4 form") @capture(& r)(t : T ?) {
                return if (!arm_on(t, IQ3_PPL_ARM))
                g26_walk_within(t, "gemma-4-26B-A4B UD-IQ3_XXS", r, 0, GEMMA_MOE_PPL_RATIO, IQ3_HITS_SLACK)
            }
            t |> run("gemma-4-26B-A4B IQ3_XXS: 520 prose ids prefilled (two windows), 80 teacher-forced, scored as a perplexity against the CPU chain") @capture(& r)(t : T ?) {
                return if (!arm_on(t, IQ3_PPL_ARM))
                assert_pf_window(t)
                g26_walk_within(t, "gemma-4-26B-A4B UD-IQ3_XXS", r, 1, GEMMA_MOE_PPL_RATIO, IQ3_HITS_SLACK)
            }
            t |> run("gemma-4-26B-A4B IQ3_XXS: 150 prose ids prefilled, 150 teacher-forced, scored as a perplexity against the CPU chain - the gemma-4 routed form on the CUDA driver") @capture(& r)(t : T ?) {
                return if (!cuda_home_asked(t) || !arm_on(t, IQ3_CUDA_ARM))
                g26_walk_within(t, "gemma-4-26B-A4B UD-IQ3_XXS, cuda", r, 0, GEMMA_MOE_PPL_RATIO, IQ3_HITS_SLACK)
            }
        }
    }
    delete r
    delete forms
}
