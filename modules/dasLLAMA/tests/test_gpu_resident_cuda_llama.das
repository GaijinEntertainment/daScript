options gen2
options stack = 524288
options persistent_heap
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // nolint:STYLE029,LINT019 — the umbrella's [init] registrations arm the llama arch and the CUDA home
require dasllama/dasllama_common
require daslib/jobque_boost
require dasllama/dasllama_math
require dasllama/dasllama_gpu_tier
require dasllama/dasllama_gpu_resident
require dasllama/dasllama_cuda_decode   // set_cuda_mmq_pin - the FP4 arm's knob-off control
require _model_tier
require _resident_feed
require _cuda_feed
require daslib/fio   // path_join
require dasllama/dasllama   // load_model_: the declined-route cell's own load
require dasllama/dasllama_kv_dtype
require _kv_codec_cells

// The CUDA home's whole-model driver on Llama-3.2-1B Q8_0 (`DASLLAMA_GPU=1 DASLLAMA_CUDA=1`): the dense slice - one
// arena of Q8_0 planes, the f16 mirror, the token step as a captured graph, the prefill as the window chain over the
// cm2 tiles. The forced-feed logits-tolerance form of test_gpu_resident_llama: the same fixed tokens through the CUDA
// chain and the all-CPU chain, the prefill logits and every step's within the bar, the one-step-off control past it,
// at 40 tokens (one window) and at a prompt of two windows; the witnesses that the CUDA driver served every position,
// ran the windows the prompt takes and replayed the captured graph past the first step, a CPU run with the model off the
// device moving none of them (the control).
// Every cell skips off the JIT, without DASLLAMA_CUDA=1, without DASLLAMA_GPU=1 on a device the driver serves, without
// the file under models_dir(), or, on the forced-feed cells, where the prompt window is not PF_WINDOW_ROWS rows
// (`assert_pf_window`: DASLLAMA_GPU_PF_ROWS or a file's pin moved it).

let private FILE_1B = "Llama-3.2-1B-Instruct-Q8_0.gguf"
let private FILE_1B_K = "Llama-3.2-1B-Instruct-Q4_K_M.gguf"   //! the K-quant planes (q4_K, q6_K): the kq GEMV leaves, the Q8_K feeds, the kq cm2 tiles
let private FILE_1B_Q51 = "Llama-3.2-1B-Instruct-Q5_1-local.gguf"   //! every dense plane q51 (the mint's recipe row in `../performance/model_specs.das`): the per-32 GEMV leaves, the s tile, the q51 table gather
let private FILE_1B_Q50 = "Llama-3.2-1B-Instruct-Q5_0-local.gguf"   //! the q5032 planes
let private FILE_1B_IQ4XS = "Llama-3.2-1B-Instruct-IQ4_XS.gguf"      //! the iq4xs planes: the codebook decode through the home's permute
let private FILE_1B_IQ4NL = "Llama-3.2-1B-Instruct-IQ4_NL-local.gguf"   //! the iq4nl superblock planes (on the 256 lattice), the same decode
let private FILE_1B_Q40 = "Llama-3.2-1B-Instruct-Q4_0-local.gguf"       //! the q40 superblock planes: the signed nibble codes under the d pairs
let private FILE_1B_NV4 = "Llama-3.2-1B-Instruct-NVFP4-local.gguf"      //! the nv4 planes: the window on the FP4 tensor rung (W4A4 - the activations quantized to e2m1 per token), the steps on the nv4 GEMV leaves
let private FILE_3B = "Llama-3.2-3B-Instruct-Q8_0.gguf"   //! head size 128: the window's attention on the h128 flash tile

[test(description = "On the CUDA whole-model driver Llama-3.2-1B Q8_0 keeps prefill and eight fed-step logits within 6 percent of max logit of the all-CPU chain over one and two windows, serving every position and replaying the captured graph, one-step-off control past the bar")]
def test_gpu_resident_cuda_llama_1b(t : T?) {
    t |> run("{FILE_1B}: one window (40 tokens) and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_1B, "llama", 40l, 1l, LOGIT_BAR_REL)
    }
    t |> run("{FILE_1B}: a {PF_WINDOW_ROWS + 88l}-token prompt (two windows) and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_1B, "llama", PF_WINDOW_ROWS + 88l, 2l, LOGIT_BAR_REL)
    }
}

//! the greedy chain on the CUDA home (`greedy_chain_cell`): the chained walks' tokens and logits bit for bit the plain walks', the
//! hits and misses counted where the Vulkan llama file counts them
[test(description = "On the CUDA driver with Llama-3.2-1B Q8_0, the greedy chain's chained walks reproduce the plain walks' tokens and logits bit for bit, with hits and misses counted as on the Vulkan home")]
def test_gpu_resident_cuda_llama_greedy_chain(t : T?) {
    t |> run("{FILE_1B}: the greedy chain's walks on the CUDA driver") @(t : T?) {
        return if (!cuda_home_asked(t))
        greedy_chain_cell(t, path_join(models_dir(), FILE_1B))
    }
}

[test(description = "On the CUDA driver with Llama-3.2-1B Q8_0, `set_cuda_kernel_skip` drops the captured graphs: under a skip naming the decode attention the next step launches none of it and the q8 GEMV anew, and cleared the attention launches again, the unskipped replay counting both alike")]
def test_gpu_resident_cuda_llama_kernel_skip(t : T?) {
    t |> run("{FILE_1B}: the kernel skip re-captures the step without the skipped kernel, and clears") @(t : T?) {
        let path = cuda_model_path(t, FILE_1B)
        if (empty(path)) return
        kernel_skip_cell(t, path, "da_attn_f16_cls", "q8_gemv_cls")
    }
}

//! a model the whole-model driver does not take under the CUDA ask runs on the CPU rails: this home has no per-op tier, so the
//! route lever off leaves every plane on the host and the decline line names the CPU rails - the load, the window and eight steps
//! run with no per-op stub reached
[test(description = "Under the CUDA ask with the whole-model route off, Llama-3.2-1B places no layer on the device, since this home has no per-op tier, and the CPU rails still run the window and eight steps")]
def test_gpu_resident_cuda_llama_declined_runs_on_cpu(t : T?) {
    t |> run("{FILE_1B} with the whole-model route off under the CUDA ask: no per-op tier serves, the CPU rails run the window and eight steps") @(t : T?) {
        return if (!cuda_home_asked(t))
        let path = path_join(models_dir(), FILE_1B)
        return if (!resident_ready(t, path))
        let route = gpu_resident_route_lever()
        set_gpu_resident_route(false)
        var tr <- load_model_(path, QuantMode.q8)
        set_gpu_resident_route(route)
        t |> success(!moe_gpu_resident_active(), "the whole-model route is off: the driver did not take the model")
        var placed = 0l
        for (l in range64(tr.config.n_layers)) {
            placed += moe_gpu_layer_on_gpu(l) ? 1l : 0l
        }
        t |> equal(placed, 0l, "no layer sits on the device: this home has no per-op tier to place one")
        with_dasllama_jobque_() {
            var prompt <- resident_prompt(40l)
            var toks <- resident_feed(8l)
            var got <- feed_alone(tr, mirror_kv_dtype(), prompt, toks)
            t |> success(long_length(got) >= 8l, "the window and eight steps landed on the CPU rails ({long_length(got)} rows)")
            delete got
            delete prompt
            delete toks
        }
        delete tr
    }
}

[test(description = "On the CUDA driver Llama-3.2-1B Q4_K_M, through the kq GEMV leaves, Q8_K feeds and kq cm2 tiles, keeps forced-feed logits within 6 percent of max logit of the CPU chain over one and two windows")]
def test_gpu_resident_cuda_llama_1b_k4(t : T?) {
    t |> run("{FILE_1B_K}: one window (40 tokens) and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_1B_K, "llama", 40l, 1l, LOGIT_BAR_REL)
    }
    t |> run("{FILE_1B_K}: a {PF_WINDOW_ROWS + 88l}-token prompt (two windows) and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_1B_K, "llama", PF_WINDOW_ROWS + 88l, 2l, LOGIT_BAR_REL)
    }
}

[test(description = "On the CUDA driver a Q5_1 Llama-3.2-1B, through the per-32 GEMV leaves, s tile and q51 table gather, keeps forced-feed logits within 6 percent of max logit of the CPU chain over one and two windows")]
def test_gpu_resident_cuda_llama_1b_q51(t : T?) {
    t |> run("{FILE_1B_Q51}: one window (40 tokens) and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_1B_Q51, "llama", 40l, 1l, LOGIT_BAR_REL)
    }
    t |> run("{FILE_1B_Q51}: a {PF_WINDOW_ROWS + 88l}-token prompt (two windows) and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_1B_Q51, "llama", PF_WINDOW_ROWS + 88l, 2l, LOGIT_BAR_REL)
    }
}

[test(description = "On the CUDA driver a Q5_0 Llama-3.2-1B keeps one-window prefill and eight fed-step logits within 6 percent of max logit of the CPU chain")]
def test_gpu_resident_cuda_llama_1b_q50(t : T?) {
    t |> run("{FILE_1B_Q50}: one window (40 tokens) and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_1B_Q50, "llama", 40l, 1l, LOGIT_BAR_REL)
    }
}

[test(description = "On the CUDA driver Llama-3.2-1B IQ4_XS, decoding its codebook through the home's permute, keeps one-window prefill and eight fed-step logits within 6 percent of max logit of the CPU chain")]
def test_gpu_resident_cuda_llama_1b_iq4xs(t : T?) {
    t |> run("{FILE_1B_IQ4XS}: one window (40 tokens) and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_1B_IQ4XS, "llama", 40l, 1l, LOGIT_BAR_REL)
    }
}

[test(description = "On the CUDA driver an IQ4_NL Llama-3.2-1B on its superblock planes keeps one-window prefill and eight fed-step logits within 6 percent of max logit of the CPU chain")]
def test_gpu_resident_cuda_llama_1b_iq4nl(t : T?) {
    t |> run("{FILE_1B_IQ4NL}: one window (40 tokens) and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_1B_IQ4NL, "llama", 40l, 1l, LOGIT_BAR_REL)
    }
}

[test(description = "On the CUDA driver a Q4_0 Llama-3.2-1B on its superblock planes keeps one-window prefill and eight fed-step logits within 6 percent of max logit of the CPU chain")]
def test_gpu_resident_cuda_llama_1b_q40(t : T?) {
    t |> run("{FILE_1B_Q40}: one window (40 tokens) and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_1B_Q40, "llama", 40l, 1l, LOGIT_BAR_REL)
    }
}

//! the FP4 leaf's witness ratio: W4A4's e2m1 activations carry one mantissa bit against the CPU chain's seven, so single logits
//! scatter past any max |logit| bar a one-step-off control still clears (a 48-token window read 0.24, its control 0.16); over a
//! window and eight steps the device reads 2.6 to 3.5 times the CPU chain's distance from the f32 chain, the control 18 to 21
let private FP4_F32_DIST_REL = 4.0

[test(description = "On the CUDA driver an NVFP4 Llama-3.2-1B with windows on the W4A4 FP4 tensor leaf sits within 4x the CPU chain's distance from the f32 chain over one and two windows, and pinned off MMQ the cm2 tiles hold the dense 6 percent bar")]
def test_gpu_resident_cuda_llama_1b_nv4(t : T?) {
    t |> run("{FILE_1B_NV4}: one window (40 tokens) and eight steps on the CUDA driver, the window on the FP4 tensor leaf") @(t : T?) {
        cuda_forced_feed_f32_witness(t, FILE_1B_NV4, "llama", 40l, 1l, FP4_F32_DIST_REL)
    }
    t |> run("{FILE_1B_NV4}: a {PF_WINDOW_ROWS + 88l}-token prompt (two windows) and eight steps on the CUDA driver, the windows on the FP4 tensor leaf") @(t : T?) {
        cuda_forced_feed_f32_witness(t, FILE_1B_NV4, "llama", PF_WINDOW_ROWS + 88l, 2l, FP4_F32_DIST_REL)
    }
    t |> run("{FILE_1B_NV4}: one window (40 tokens) and eight steps pinned CUDA_MMQ_OFF - the cm2 tiles over f16 activations hold the dense bar (the FP4 arm's control)") @(t : T?) {
        with_cuda_mmq_pin(CUDA_MMQ_OFF) {
            cuda_forced_feed(t, FILE_1B_NV4, "llama", 40l, 1l, LOGIT_BAR_REL)
        }
    }
}

//! the short window's peel through the N-row command on the CUDA driver (`peel_cell`): tails of 1, 2, 5 and 8 rows after a 40-row
//! opening, one row past the per-row planes and the eight-row tail with the lever off taking the window chain
[cold_path]
def private cuda_peel(t : T?; file : string) {
    let path = cuda_model_path(t, file)
    if (empty(path)) return
    let levers = resident_levers()
    cuda_feed_window()
    var tr <- load_resident(path, 1l)
    t |> success(moe_gpu_resident_active(), "the whole-model driver armed on the model (the load log names any decline)")
    with_dasllama_jobque_() {
        peel_cell(t, tr, SPLIT_BAR_REL, "{file} cuda, ", true)
    }
    delete tr
    restore_resident_levers(levers)
}

//! the last row's logits of an eight-row tail (the N-row command, the rows leaf's feed) after a 40-row opening on a fresh load of
//! `path`, the driver dropped with the model after it
def private tail_after_load(path : string; tail : array<int64>) : array<float> {
    var tr <- load_resident(path, 1l)
    var s <- create_session(tr)
    var prompt <- resident_prompt(40l)
    eval_(tr, s, prompt)
    eval_(tr, s, tail)
    var row <- logits_of(s, tr.config.vocab_size)
    delete prompt
    delete s
    delete tr
    return <- row
}

[test(description = "On the CUDA driver a Llama-3.2-1B Q8_0 load after a dropped one reads an eight-row tail's logits bit for bit as the first load did, the rows leaf's feed record cleared by the drop; a tail one token off is the control")]
def test_gpu_resident_cuda_llama_drop_reload(t : T?) {
    t |> run("{FILE_1B}: load, tail, drop; load again, the same tail") @(t : T?) {
        let path = cuda_model_path(t, FILE_1B)
        if (empty(path)) return
        let levers = resident_levers()
        cuda_feed_window()
        with_dasllama_jobque_() {
            var tail <- resident_feed(8l)
            let c0 = cuda_positions_served()
            var first <- tail_after_load(path, tail)
            cuda_served_since(t, c0, 1l, "the first load's tail")
            var again <- tail_after_load(path, tail)
            t |> equal(logits_maxdiff(again, first), 0.0, "the reload's tail logits are the first load's bit for bit")
            tail[7]++
            var off <- tail_after_load(path, tail)
            t |> success(logits_maxdiff(off, first) > 0.0, "control: a tail one token off reads other logits ({logits_maxdiff(off, first)})")
            delete off
            delete again
            delete first
            delete tail
        }
        restore_resident_levers(levers)
    }
}

[test(description = "On the CUDA driver, short tails of 1, 2, 5 and 8 rows after a 40-row opening peel through the N-row command within the 6 percent split bar of the one-row steps on Q8_0, Q4_K_M and Q5_1 Llama-3.2-1B")]
def test_gpu_resident_cuda_llama_peel(t : T?) {
    t |> run("{FILE_1B}: a short tail peels through the N-row command on the CUDA driver and reads what the one-row steps read") @(t : T?) {
        cuda_peel(t, FILE_1B)
    }
    t |> run("{FILE_1B_K}: the same on the K-quant planes' N-column leaves and the rows' split Q8_K step") @(t : T?) {
        cuda_peel(t, FILE_1B_K)
    }
    t |> run("{FILE_1B_Q51}: the same on the per-32 planes' N-column leaves") @(t : T?) {
        cuda_peel(t, FILE_1B_Q51)
    }
}

[test(description = "On the CUDA driver Llama-3.2-3B Q8_0, head size 128 on the h128 flash tile, keeps forced-feed logits within 6 percent of max logit of the CPU chain over one and two windows")]
def test_gpu_resident_cuda_llama_3b(t : T?) {
    t |> run("{FILE_3B}: one window (40 tokens) and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_3B, "llama", 40l, 1l, LOGIT_BAR_REL)
    }
    t |> run("{FILE_3B}: a {PF_WINDOW_ROWS + 88l}-token prompt (two windows) and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_3B, "llama", PF_WINDOW_ROWS + 88l, 2l, LOGIT_BAR_REL)
    }
}

// The CUDA home's whole-model driver under a q8_0 K/V mirror (`DASLLAMA_GPU=1 DASLLAMA_CUDA=1`): the rope stamps stage the
// token's row, the codec's store quantizes it into the mirror, the decode attention's q8_0 stamps score the blocks, the window
// chain stores its rows and attends the f16 shadow - the codec rig's forced-feed form (`_kv_codec_cells`) on Llama-3.2-1B
// Q8_0 at one window and two, and on the Q4_K_M file, each at the 10% bar the Vulkan carriers hold (both chains quantize each
// query, each its own, so two roundings part). Every cell skips off the JIT, without DASLLAMA_CUDA=1, without the armed tier,
// without the file under models_dir(), or under DASLLAMA_VK_KV32=1 (the f32 mirror armed over the codec the cell asks).

let private Q8KV_BAR_REL = 0.10   //! of the step's max |logit|: the Vulkan codec carriers' bar

def private cuda_codec_cell(t : T?; file : string; n : int64) {
    return if (!cuda_home_asked(t))
    cell_codec_forced_feed(t, file, n, KVDtype.q8_0, Q8KV_BAR_REL)
}

[test(description = "On the CUDA whole-model driver with a q8_0 K/V mirror, Llama-3.2-1B Q8_0 over one and two windows and Q4_K_M over one keep forced-feed logits within 10 percent of max logit of the CPU chain on q8_0 sessions")]
def test_gpu_resident_cuda_q8kv(t : T?) {
    t |> run("{FILE_1B}: a q8_0 mirror, one window (40 tokens) and eight steps on the CUDA driver") @(t : T?) {
        cuda_codec_cell(t, FILE_1B, 40l)
    }
    t |> run("{FILE_1B}: a q8_0 mirror, a {PF_WINDOW_ROWS + 88l}-token prompt (two windows) and eight steps on the CUDA driver") @(t : T?) {
        cuda_codec_cell(t, FILE_1B, PF_WINDOW_ROWS + 88l)
    }
    t |> run("{FILE_1B_K}: a q8_0 mirror, one window (40 tokens) and eight steps on the CUDA driver") @(t : T?) {
        cuda_codec_cell(t, FILE_1B_K, 40l)
    }
}

//! tq4 in the distance form (no logit bar holds a 4-bit mirror): the device at tq4 against the f16 chain within `TQ4_DIST_REL` of
//! the CPU tq4 chain's own distance, the two tq4 chains within `TQ4_PAIR_REL` of it - the Vulkan llama codec file's bars
let private TQ4_DIST_REL = 1.15
let private TQ4_PAIR_REL = 1.4

[test(description = "On the CUDA whole-model driver with a tq4 K/V mirror, Llama-3.2-1B Q4_K_M sits within 1.15x the CPU tq4 chain's distance from the f16 chain and within 1.4x of it from the CPU tq4 chain")]
def test_gpu_resident_cuda_tq4kv(t : T?) {
    t |> run("{FILE_1B_K}: a tq4 mirror, the distance form over 40 tokens and eight steps on the CUDA driver") @(t : T?) {
        return if (!cuda_home_asked(t))
        cell_codec_distance(t, FILE_1B_K, 40l, KVDtype.tq4, false, TQ4_DIST_REL, TQ4_PAIR_REL)
    }
}
