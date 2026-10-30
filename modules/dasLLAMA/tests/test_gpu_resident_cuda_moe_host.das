options gen2
options stack = 524288
options persistent_heap
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // the umbrella's [init] registrations arm the MoE archs and the CUDA home
require dasllama/dasllama_common   // forward_prefill, forward, mtp_spec_eval, get_mtp_spec / set_mtp_spec
require dasllama/dasllama   // create_session
require daslib/jobque_boost
require dasllama/dasllama_math   // with_dasllama_jobque_
require dasllama/dasllama_gpu_resident   // gpu_resident_host_layers, gpu_resident_hot_stats: the form's witnesses
require dasllama/dasllama_gpu_tier   // rdec_token_n_rows: the N-row command's decline of the host-experts plan; g_gpu_want / set_gpu_tier_want_: the pool's slot ask
require dasllama/dasllama_core   // gpu_device_session_ctx: the context the plan armed at
require dasllama/dasllama_env   // g_env_gpu.gpu_heat: the environment's slot ask, which shadows the want
require dasllama/dasllama_cuda_decode   // cud_rdec_hot_stats: the hit chains the pool launched; set_cuda_mmq_pin: the file's MMQ route
require dasllama/dasllama_cuda_stream   // cud_stream_stats, cud_stream_dma_fills: the streamed prompt's and the DMA fill's witnesses
require dasllama/dasllama_blocks   // g_moe_meet_*_experts, set_moe_host_first: the meet's witnesses and its host-share pin
require _model_tier
require _resident_feed
require _cuda_feed
require _moe_resident   // MOE_LOGIT_BAR_REL: the routed chain's CPU-vs-device noise class

// The host-experts form on the CUDA home's whole-model driver (`DASLLAMA_GPU=1 DASLLAMA_CUDA=1`): each fixture asks the plan for
// its first four routed layers' experts on the host (`set_gpu_host_layers`, the plan held to exactly that count), so the split
// token command - a captured graph a segment, the host's routed sum between them - and the cut window chain run on every card
// the file loads on, not only on one whose room forces the form. The two carriers reach the Q8_0 and the K-quant expert gathers
// of the hot expert pool, whose hit chains serve the pooled picks beside the host's sums; the 35B NextN hybrid (large tier,
// the plan moving more than the one layer asked) rides the same form with its head: the speculative round's draft and verify
// over the segments. The forced-feed form the CUDA files share (`_cuda_feed.das`) at the MoE bar, the MMQ route pinned
// `CUDA_MMQ_WINNERS` for every load (the file's `[init]`). Every cell skips off the JIT, without DASLLAMA_CUDA=1, without
// DASLLAMA_GPU=1 on a device the driver serves, without the file under models_dir(), without DASLLAMA_PARITY_FULL=1 (every
// carrier sits past the large tier's bytes), or where the prompt window is not PF_WINDOW_ROWS rows; the gpt-oss cells also skip
// on a card below the FP4 rung (`FP4_RUNG_SM`, compute capability 120); the pool cells skip where the plan finds no room for
// the pool beside the mirror, and where DASLLAMA_GPU_HEAT in the environment shadows the cell's slot ask; the 35B cell's pool
// assert skips where the plan armed no pool. The meet's asserts run under DASLLAMA_GPU_MOE_SPLIT=1 (the default) alone, and
// their texts name the knob's value.

let private FILE_Q8 = "Qwen1.5-MoE-A2.7B-Chat.Q8_0.gguf"   //! 60 experts, 4 routed, a gated shared expert, Q8_0 stacks
let private FILE_K = "Qwen1.5-MoE-A2.7B-Chat-Q4_K_M-local.gguf"   //! the same block over K-quant stacks
let private HOST_LAYERS = 4l
let private HOST_FIRST = 8l   //! the rarest experts the meet hands the host before the device claims: the host's rows on every carrier
let private POOL_SLOTS = 8l   //! the pool cell's slot ask a layer: fewer than the carrier's experts, so the pool fills and misses land by DMA
let private POOL_CTX = 8192l   //! the pool cell's context (`set_gpu_ctx_max`, put back): a mirror short enough to leave the K-quant carrier's pool its room on a 16 GB card

[init]
def private moe_host_intent() {
    set_cuda_mmq_pin(CUDA_MMQ_WINNERS)   // the MMQ route every load of the file measures: the K-quant and Q8_0 window sites on the int8 leaves
}

//! the host cells' pins (`stream_pins` as the cell found them, `restore_stream_pins` puts them back): the prompt stream on, the
//! meet's host share at `HOST_FIRST`, the pool's misses filled by DMA (the knob's default), the decode meet off (the one-row host
//! step the split command takes) and no slot ask (the plan's own pool)
def private pin_stream : StreamPins {
    let was = stream_pins()
    set_gpu_want_pf_stream(1)
    set_moe_host_first(HOST_FIRST)
    set_gpu_hot_dma(1)
    set_gpu_dec_meet(0)
    want_heat(0l)
    return was
}

//! the knob's value a red names: the meet runs under DASLLAMA_GPU_MOE_SPLIT=1, the whole-layer rail under 0
def private split_knob : string => "DASLLAMA_GPU_MOE_SPLIT={gpu_want_moe_split() ? 1 : 0}"

//! the streamed prompt's witnesses after a feed: the chain ran the window(s) it was asked (`cud_stream_stats`), the meet split the
//! experts between the device and the host; under `off` the control - no window streamed, no expert met on either side
def private stream_witness(t : T?; file : string; w0 : int64; g0, c0 : int64; off : bool) {
    let st = cud_stream_stats()
    to_log(LOG_INFO, "{file}: the chain ran {st.windows - w0} streamed windows, {st.bundles} bundles of {st.experts} experts since the device came up; the meet took {g_moe_meet_gpu_experts - g0} experts on the device and handed the host {g_moe_meet_cpu_experts - c0} ({split_knob()})\n")
    if (off) {
        t |> equal(st.windows - w0, 0l, "control: with the prompt stream off the chain ran no window - the host summed every routed layer on the host")
        t |> equal(g_moe_meet_gpu_experts - g0, 0l, "control: with the prompt stream off the meet took no expert on the device ({split_knob()})")
        t |> equal(g_moe_meet_cpu_experts - c0, 0l, "control: ... and handed the host none")
        return
    }
    t |> success(st.windows - w0 > 0l, "the {gpu_want_moe_split() ? "per-expert chain" : "whole-layer rail"} ran {st.windows - w0} streamed windows over the prompt's host layers ({split_knob()})")
    return if (!gpu_want_moe_split())   // DASLLAMA_GPU_MOE_SPLIT=0: the whole-layer rail takes every region, no meet
    t |> success(g_moe_meet_gpu_experts - g0 > 0l, "the meet took {g_moe_meet_gpu_experts - g0} experts on the device ({split_knob()})")
    t |> success(g_moe_meet_cpu_experts - c0 > 0l, "the meet handed the host {g_moe_meet_cpu_experts - c0} experts (set_moe_host_first({HOST_FIRST}), {split_knob()})")
}

[cold_path]
def private host_cells(t : T?; file : string) {
    t |> run("{file} with {HOST_LAYERS} routed layers' experts pinned to the host: the split token command and the cut window chain on the CUDA driver vs the CPU chain, one window (40 tokens) and eight steps; the window's picks streamed through the per-expert chain, the meet splitting them with the host") @(t : T?) {
        let pins = pin_stream()
        let w0 = cud_stream_stats().windows
        let g0 = g_moe_meet_gpu_experts
        let c0 = g_moe_meet_cpu_experts
        cuda_forced_feed(t, file, "qwen2moe", 40l, 1l, MOE_LOGIT_BAR_REL, HOST_LAYERS, false) $ {
            let hot = cud_rdec_hot_stats()
            to_log(LOG_INFO, "{file}: {gpu_resident_hot_slots()} pool slots a layer; the pool launched {hot.submits} hit chains serving {hot.hits} picks\n")
            stream_witness(t, file, w0, g0, c0, false)
            t |> equal(rdec_token_n_rows(), 0l, "the N-row command declines a plan with routed experts on the host: the split command steps a row at a time")
        }
        restore_stream_pins(pins)
    }
    t |> run("{file} with {HOST_LAYERS} routed layers' experts pinned to the host: the same across two windows") @(t : T?) {
        let pins = pin_stream()
        let w0 = cud_stream_stats().windows
        let g0 = g_moe_meet_gpu_experts
        let c0 = g_moe_meet_cpu_experts
        cuda_forced_feed(t, file, "qwen2moe", PF_WINDOW_ROWS + 88l, 2l, MOE_LOGIT_BAR_REL, HOST_LAYERS, false) $ {
            stream_witness(t, file, w0, g0, c0, false)
        }
        restore_stream_pins(pins)
    }
    t |> run("{file} with {HOST_LAYERS} routed layers' experts pinned to the host: the streamed windows' rows against the host's sums' at the stream twin's bar, the device claiming first; a plain model's chain takes no bias row") @(t : T?) {
        let path = cuda_model_path(t, file)
        return if (empty(path))
        let pins = pin_stream()
        let b0 = cud_stream_stats().biased
        stream_twin_within(t, path, HOST_LAYERS, 40l, 8l, STREAM_TWIN_BAR_REL, "{file} cuda ")
        t |> equal(cud_stream_stats().biased, b0, "control: the chain ran no biased act on a plain model")
        restore_stream_pins(pins)
    }
    t |> run("{file} with {HOST_LAYERS} routed layers' experts pinned to the host, the prompt stream off (set_gpu_want_pf_stream(0)): the window keeps the host's sums, the chain runs no window and the meet claims nothing") @(t : T?) {
        let pins = pin_stream()
        set_gpu_want_pf_stream(0)
        let w0 = cud_stream_stats().windows
        let g0 = g_moe_meet_gpu_experts
        let c0 = g_moe_meet_cpu_experts
        cuda_forced_feed(t, file, "qwen2moe", 40l, 1l, MOE_LOGIT_BAR_REL, HOST_LAYERS, false) $ {
            stream_witness(t, file, w0, g0, c0, true)
        }
        restore_stream_pins(pins)
    }
}

//! the pool cells' room: the plan armed a pool, else the skip naming why; true where the asserts on the pool may run
def private pool_armed(t : T?) : bool {
    if (gpu_resident_hot_slots() == 0l) {
        t |> skip("the plan found no room for the pool beside the mirror on this card")
        return false
    }
    return true
}

//! the pool cells' gate: their slot ask of `POOL_SLOTS` goes through the want, which DASLLAMA_GPU_HEAT in the environment
//! shadows - false after the skip naming the environment's value where it does
def private pool_asked(t : T?) : bool {
    let env = g_env_gpu.gpu_heat ?? POOL_SLOTS
    if (env != POOL_SLOTS) {
        t |> skip("DASLLAMA_GPU_HEAT={env} in the environment shadows the cell's slot ask of {POOL_SLOTS}")
        return false
    }
    return true
}

//! the pool cells' plan witnesses: the plan armed at the asked context, and its slots at the asked count where it armed a pool
def private pool_plan_held(t : T?) {
    t |> equal(gpu_device_session_ctx(), POOL_CTX, "the plan armed at the {POOL_CTX} context asked (set_gpu_ctx_max)")
    if (gpu_resident_hot_slots() > 0l) {
        t |> equal(gpu_resident_hot_slots(), POOL_SLOTS, "the pool seats the {POOL_SLOTS} slots a layer asked")
    }
}

//! the pool's two knobs on the K-quant carrier at a slot ask the plan can seat: the one-row rail's misses land in the pool by DMA off
//! the pinned stream stacks, the decode meet's picks claim through the window's meet over the pool, and both off as the control; the
//! fed steps against the CPU chain at the MoE bar, each knob's counter the witness that it ran; then the same carrier with every
//! expert on the device - no host layer, so no pool, no hit chain, no DMA fill and no met step (the pool counters' control)
[test]
def test_gpu_resident_cuda_moe_host_pool(t : T?) {   // nolint:STYLE038 - the pool's four legs (DMA + meet, both off, no host layer) are one flat list over one carrier
    t |> run("{FILE_K} with {HOST_LAYERS} routed layers' experts pinned to the host and {POOL_SLOTS} pool slots a layer asked (set_gpu_tier_want_, the want DASLLAMA_GPU_HEAT shadows) at a {POOL_CTX} context: the steps' misses filled by DMA off the stream stacks, the hit chains serving the pooled picks") @(t : T?) {
        return if (!pool_asked(t))
        let pins = pin_stream()
        let levers = resident_levers()
        set_gpu_ctx_max(POOL_CTX)
        want_heat(POOL_SLOTS)
        set_gpu_hot_dma(1)
        set_gpu_dec_meet(0)
        let d0 = cud_stream_dma_fills()
        let s0 = gpu_resident_hot_stats()
        cuda_forced_feed(t, FILE_K, "qwen2moe", 40l, 1l, MOE_LOGIT_BAR_REL, HOST_LAYERS, false) $ {
            let s1 = gpu_resident_hot_stats()
            let hot = cud_rdec_hot_stats()
            to_log(LOG_INFO, "{FILE_K}: {gpu_resident_hot_slots()} pool slots a layer; the DMA fill copied {cud_stream_dma_fills() - d0} experts ({s1.dma - s0.dma} placed), the pool launched {hot.submits} hit chains serving {hot.hits} picks\n")
            pool_plan_held(t)
            return if (!pool_armed(t))
            t |> success(s1.dma - s0.dma > 0l, "the DMA fill landed {s1.dma - s0.dma} experts in the pool off the stream stacks over the cell")
            t |> success(cud_stream_dma_fills() - d0 >= s1.dma - s0.dma, "every landed expert was the copy stream's ({cud_stream_dma_fills() - d0} copies)")
            t |> success(hot.submits > 0l && hot.hits > 0l, "the pool's hit chains served picks beside the host's sums ({hot.submits} chains, {hot.hits} picks)")
        }
        restore_resident_levers(levers)
        restore_stream_pins(pins)
    }
    t |> run("{FILE_K} with {HOST_LAYERS} routed layers' experts pinned to the host and {POOL_SLOTS} pool slots a layer at a {POOL_CTX} context, the decode meet on: the fed steps' picks claimed through the window's meet over the pool, each step's layers a streamed window") @(t : T?) {
        return if (!pool_asked(t))
        let pins = pin_stream()
        let levers = resident_levers()
        set_gpu_ctx_max(POOL_CTX)
        want_heat(POOL_SLOTS)
        set_gpu_dec_meet(1)
        let w0 = cud_stream_stats().windows
        let s0 = gpu_resident_hot_stats()
        cuda_forced_feed(t, FILE_K, "qwen2moe", 40l, 1l, MOE_LOGIT_BAR_REL, HOST_LAYERS, false) $ {
            let s1 = gpu_resident_hot_stats()
            to_log(LOG_INFO, "{FILE_K}: {gpu_resident_hot_slots()} pool slots a layer; the decode meet served {s1.meet_steps - s0.meet_steps} steps over {cud_stream_stats().windows - w0} streamed windows, the pool promoted {s1.uploads - s0.uploads} experts\n")
            pool_plan_held(t)
            return if (!pool_armed(t))
            t |> success(s1.meet_steps - s0.meet_steps > 0l, "the decode meet served {s1.meet_steps - s0.meet_steps} of the fed steps' routed layers over the pool")
            t |> success(cud_stream_stats().windows - w0 > s1.meet_steps - s0.meet_steps, "every met step's layer ran as a streamed window beside the prompt's ({cud_stream_stats().windows - w0} windows)")
        }
        restore_resident_levers(levers)
        restore_stream_pins(pins)
    }
    t |> run("{FILE_K} with {HOST_LAYERS} routed layers' experts pinned to the host and {POOL_SLOTS} pool slots a layer at a {POOL_CTX} context, the DMA fill off and the meet off (control): no expert lands by DMA, no step is met, the steps' misses take the host's upload") @(t : T?) {
        return if (!pool_asked(t))
        let pins = pin_stream()
        let levers = resident_levers()
        set_gpu_ctx_max(POOL_CTX)
        want_heat(POOL_SLOTS)
        set_gpu_hot_dma(0)
        set_gpu_dec_meet(0)
        let d0 = cud_stream_dma_fills()
        let s0 = gpu_resident_hot_stats()
        cuda_forced_feed(t, FILE_K, "qwen2moe", 40l, 1l, MOE_LOGIT_BAR_REL, HOST_LAYERS, false) $ {
            pool_plan_held(t)
            return if (!pool_armed(t))
            let s1 = gpu_resident_hot_stats()
            t |> equal(cud_stream_dma_fills() - d0, 0l, "control: with the fill off the copy stream carried no pool expert")
            t |> equal(s1.meet_steps - s0.meet_steps, 0l, "control: with the meet off no fed step's layer went through the window's meet")
            t |> success(s1.uploads > 0l, "the window's placements and the steps' misses took the host's upload ({s1.uploads})")
        }
        restore_resident_levers(levers)
        restore_stream_pins(pins)
    }
    t |> run("{FILE_K} with every routed layer's experts on the device (no host layer asked) at a {POOL_CTX} context (control): the plan arms no pool, so the pool launches no hit chain, the copy stream fills nothing and no step is met") @(t : T?) {
        return if (!pool_asked(t))
        let pins = pin_stream()
        let levers = resident_levers()
        set_gpu_ctx_max(POOL_CTX)
        want_heat(POOL_SLOTS)
        set_gpu_hot_dma(1)
        set_gpu_dec_meet(1)
        let d0 = cud_stream_dma_fills()
        let h0 = cud_rdec_hot_stats()
        cuda_forced_feed(t, FILE_K, "qwen2moe", 40l, 1l, MOE_LOGIT_BAR_REL, 0l, false) $ {
            let s1 = gpu_resident_hot_stats()
            let hot = cud_rdec_hot_stats()
            t |> equal(gpu_resident_host_layers(), 0l, "control: no routed layer's experts serve on the host")
            t |> equal(gpu_resident_hot_slots(), 0l, "control: with no host layer the plan arms no pool")
            t |> equal(hot.submits - h0.submits, 0l, "control: the pool launched no hit chain ({hot.submits - h0.submits})")
            t |> equal(hot.hits - h0.hits, 0l, "control: ... and served no pick")
            t |> equal(cud_stream_dma_fills() - d0, 0l, "control: the copy stream filled no slot")
            t |> equal(s1.meet_steps, 0l, "control: no fed step went through the meet")
        }
        restore_resident_levers(levers)
        restore_stream_pins(pins)
    }
}

//! the pool with the prompt stream off: no stream stack stands, so the chain's pool window and the DMA fill both decline by name and
//! the host's fill lands every pooled expert; the decode's hit chains still serve off the slots, the fed steps the CPU chain's
[test]
def test_gpu_resident_cuda_moe_host_pool_unstreamed(t : T?) {
    t |> run("{FILE_K} with {HOST_LAYERS} routed layers' experts pinned to the host, {POOL_SLOTS} pool slots a layer at a {POOL_CTX} context and the prompt stream OFF: no stream stack stands, so the DMA fill declines by name and the host's fill lands every pooled expert; the fed steps still the CPU chain's") @(t : T?) {
        return if (!pool_asked(t))
        let pins = pin_stream()
        let levers = resident_levers()
        set_gpu_ctx_max(POOL_CTX)
        want_heat(POOL_SLOTS)
        set_gpu_want_pf_stream(0)
        set_gpu_hot_dma(1)
        set_gpu_dec_meet(0)
        let d0 = cud_stream_dma_fills()
        cuda_forced_feed(t, FILE_K, "qwen2moe", 40l, 1l, MOE_LOGIT_BAR_REL, HOST_LAYERS, false) $ {
            let s1 = gpu_resident_hot_stats()
            let hot = cud_rdec_hot_stats()
            to_log(LOG_INFO, "{FILE_K}: {gpu_resident_hot_slots()} pool slots a layer with the prompt stream off; the DMA fill copied {cud_stream_dma_fills() - d0} experts ({s1.dma} placed), the pool launched {hot.submits} hit chains serving {hot.hits} picks\n")
            pool_plan_held(t)
            return if (!pool_armed(t))
            t |> equal(s1.dma, 0l, "with no streamed stack the DMA fill placed nothing over this load (the load resets the count) - the host's fill took every pooled expert")
            t |> equal(cud_stream_dma_fills() - d0, 0l, "and the copy stream copied none")
            t |> success(hot.submits > 0l && hot.hits > 0l, "the pool's hit chains served picks off the host-filled slots ({hot.submits} chains, {hot.hits} picks)")
        }
        restore_resident_levers(levers)
        restore_stream_pins(pins)
    }
}

let private FILE_GPTOSS = "gpt-oss-20b-mxfp4.gguf"   //! 32 experts, 4 routed, the biased softmax router, native-MXFP4 stacks: the biased expert act
let private HOST_LAYERS_GPTOSS = 2l

//! gpt-oss's biased stacks on the host-layers form: the window's picks through the per-expert chain over the mx4 slot - the s column
//! alone, the only tile the format has - the fed steps against the CPU chain at the routed block's bar (the CUDA MoE file's reading)
[test]
def test_gpu_resident_cuda_moe_host_gptoss(t : T?) {
    t |> run("{FILE_GPTOSS} with {HOST_LAYERS_GPTOSS} routed layers' experts pinned to the host: the split token command and the cut window chain on the CUDA driver vs the CPU chain, one window (40 tokens) and eight steps; the biased mx4 stacks streamed through the per-expert chain") @(t : T?) {
        return if (!fp4_rung_on(t))
        let pins = pin_stream()
        set_moe_host_first(0l)   // the device claims first: a 32-expert layer at a host-first of eight hands the host every region
        let w0 = cud_stream_stats().windows
        let g0 = g_moe_meet_gpu_experts
        let c0 = g_moe_meet_cpu_experts
        with_cuda_mmq_pin(CUDA_MMQ_WINNERS) {
            cuda_forced_feed(t, FILE_GPTOSS, "gpt-oss", 40l, 1l, MOE_LOGIT_BAR_REL, HOST_LAYERS_GPTOSS, false) $ {
                stream_witness(t, FILE_GPTOSS, w0, g0, c0, false)
            }
        }
        restore_stream_pins(pins)
    }
    t |> run("{FILE_GPTOSS} with {HOST_LAYERS_GPTOSS} routed layers' experts pinned to the host: the streamed windows' rows against the host's sums' at the stream twin's bar - the chain's clamped swiglu and the picks' gate and up bias rows") @(t : T?) {
        return if (!fp4_rung_on(t))
        let path = cuda_model_path(t, FILE_GPTOSS)
        return if (empty(path))
        let pins = pin_stream()
        let b0 = cud_stream_stats().biased
        with_cuda_mmq_pin(CUDA_MMQ_WINNERS) {
            stream_twin_within(t, path, HOST_LAYERS_GPTOSS, 40l, 8l, STREAM_TWIN_BAR_REL, "{FILE_GPTOSS} cuda ")
        }
        t |> success(cud_stream_stats().biased > b0, "the chain's acts took the routed form's bias rows ({cud_stream_stats().biased - b0} biased acts)")
        restore_stream_pins(pins)
    }
}


[test]
def test_gpu_resident_cuda_moe_host_q8(t : T?) {
    host_cells(t, FILE_Q8)
}

[test]
def test_gpu_resident_cuda_moe_host_kq(t : T?) {
    host_cells(t, FILE_K)
}

let private FILE_35B = "Qwen3.6-35B-A3B-MTP-UD-Q4_K_M.gguf"   //! the deltanet MoE hybrid with its NextN head: large tier, the board's host-layers carrier on a 16 GB card
let private ROUNDS_35B = 6l

//! the speculative round on the 35B: every round's draft and verify on the device over the segments, the rollback where a draft
//! misses, the stream decoded; then the same tokens stepped plain with speculation off - no draft, no verify (the counters' control)
[cold_path]
def private mtp_rounds_served(t : T?; tr : Model) {
    var prompt <- resident_prompt(40l)
    var toks <- resident_feed(ROUNDS_35B)
    var s <- create_session(tr, mirror_kv_dtype())
    let spec0 = get_mtp_spec()
    set_mtp_spec(true)
    forward_prefill(tr, s, prompt, long_length(prompt), 0l)
    s.n_past = long_length(prompt)
    let drafts0 = moe_gpu_resident_drafts()
    let verifies0 = moe_gpu_resident_verifies()
    var ids : array<int64>
    ids |> reserve(ROUNDS_35B * 2l)
    for (tok in toks) {
        var acc = 0l
        let hit = mtp_spec_eval(tr, s, tok, acc)
        ids |> push(tok)
        if (hit) {
            ids |> push(s.mtp_vbatch[1])
        }
    }
    set_mtp_spec(spec0)
    to_log(LOG_INFO, "{FILE_35B}: {ROUNDS_35B} rounds committed {length(ids)} tokens: '{decode_(tr, ids)}'\n")
    t |> equal(moe_gpu_resident_drafts() - drafts0, ROUNDS_35B, "every round drafted on the device")
    t |> equal(moe_gpu_resident_verifies() - verifies0, ROUNDS_35B, "every round verified on the device over the segments")
    served_every_call(t)
    delete s
    var p <- create_session(tr, mirror_kv_dtype())
    set_mtp_spec(false)
    forward_prefill(tr, p, prompt, long_length(prompt), 0l)
    let drafts1 = moe_gpu_resident_drafts()
    let verifies1 = moe_gpu_resident_verifies()
    for (tok, i in toks, count()) {
        forward(tr, p, tok, long_length(prompt) + int64(i))
    }
    set_mtp_spec(spec0)
    t |> equal(moe_gpu_resident_drafts(), drafts1, "control: with speculation off the plain steps drafted nothing")
    t |> equal(moe_gpu_resident_verifies(), verifies1, "control: ... and verified nothing")
    served_every_call(t)
    delete p
    delete ids
    delete toks
    delete prompt
}

[test]
def test_gpu_resident_cuda_moe_host_35b(t : T?) {
    t |> run("{FILE_35B} with its first routed layer's experts asked onto the host (the plan moves what the card cannot hold): the split token command and the cut window chain on the CUDA driver vs the CPU chain, one window (40 tokens) and eight steps, the window's picks streamed through the per-expert chain over the pool's prefix, the hot expert pool's hit chains serving picks") @(t : T?) {
        let pins = pin_stream()
        let w0 = cud_stream_stats().windows
        let g0 = g_moe_meet_gpu_experts
        let c0 = g_moe_meet_cpu_experts
        let d0 = cud_stream_dma_fills()
        cuda_forced_feed(t, FILE_35B, "qwen35moe", 40l, 1l, MOE_LOGIT_BAR_REL, 1l, true) $ {
            let hot = cud_rdec_hot_stats()
            to_log(LOG_INFO, "{FILE_35B}: {gpu_resident_hot_slots()} pool slots a layer; the pool launched {hot.submits} hit chains serving {hot.hits} picks; the DMA fill copied {cud_stream_dma_fills() - d0} experts\n")
            if (pool_armed(t)) {
                t |> success(hot.submits > 0l && hot.hits > 0l, "the pool's hit chains served picks beside the host's sums ({hot.submits} chains, {hot.hits} picks)")
            }
            stream_witness(t, FILE_35B, w0, g0, c0, false)
        }
        restore_stream_pins(pins)
    }
    t |> run("{FILE_35B}: the NextN head beside the host layers - {ROUNDS_35B} speculative rounds, every draft and verify on the device; the same tokens plain with speculation off drafting and verifying nothing") @(t : T?) {
        let path = cuda_model_path(t, FILE_35B)
        if (empty(path)) return
        let pins = pin_stream()
        var tr <- load_resident(path, 1l, 0l, 1l)
        if (resident_admitted(t, "the whole-model driver armed on the 35B with host layers (the load log names any decline)", "")) {
            with_dasllama_jobque_() {
                mtp_rounds_served(t, tr)
            }
        }
        delete tr
        restore_stream_pins(pins)
    }
}
