options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes below: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration (llama)
require dasllama/dasllama   // create_session, moe_gpu_drop_model
require daslib/jobque_boost
require daslib/fio
require dasllama/dasllama_math   // with_dasllama_jobque_: the compares after the block decode off the queue
require dasllama/dasllama_gpu_tier   // gpu_cpu_passes_: the calls the driver handed to the CPU rails
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active, moe_gpu_layer_on_gpu, the N-row command's counters
require dasllama/dasllama_kv_dtype   // KVDtype: the codec forms' mirror asks
require dasllama/dasllama_cuda_common   // cud_kernel_coverage_of: the N-row command's stamps on the CUDA home
require dasllama/dasllama_cuda_decode   // cuda_window_planes_held: the window-fault form's planes
require daslib/rtti   // this_context().last_exception: the refused window's text
require strings   // find: the refusal's words
require dasllama/dasllama_bench   // bench_tg_batched_rep: the batched row's refusal and its served rep
require _model_tier   // models_dir() + model_available() presence/size-tier gate
require _resident_feed   // the forced-feed rig, the peel, the greedy chain, the kernel skip, Held
require _cuda_feed   // LOGIT_BAR_REL: the dense carriers' bar on both homes
require _cpu_gpu_model   // with_cpu_and_gpu_model, the forms, DenseFeeds, FeedRows
require _resident_regions   // the regions cells' collections, REGIONS_RIG_CTX
require _kv_codec_cells   // the codec cells' halves, CODEC_FED_STEPS
require _segment_cells   // dense_segments_cells, SEG_WINDOW: the segment forms' cells
require _chat_cells   // the SmolLM2, tinyllama and Llama-3.2-1B chat cells on their CPU forms
require _scheduler_cells   // the SmolLM2 scheduler cells
require _batch_decode_cells   // the SmolLM2 batched-decode codec cells and the per-row refusal
require _kv_codec_cpu_cells   // kv_q8_prefill_vs_decode_cell, kv_q8_decode_cell
require _kquant_cells   // the fused qkv slices and LongRoPE factors cells: Phi-3.5-mini Q4_K_M's K-quant-native portable form
require _kv_paged_cells   // kv_paged_q8_cell
require _fused_decode_cells   // fused_attn_chain_cell, the deep-context witness cells
require _parity_cells   // SmolLM2's tied rows (tied_cls_rows, tied_cls_q8_rows_cell); the 1B files' parity rows on their blocks
require _sampling_cells   // constrained_sampler_cell
require _embedding_cells   // the tinyllama embedding cells
require _ffn_slice_cells   // last_layer_slice_cell: the 1B's default form
require _mlx_cells   // the 1B MLX 8-bit snapshot's block beside its GGUF twin's
require _metal_batched_cells   // metal_llama_1b_cells: the 1B Q8_0 file's Metal form's cells
require _metal_matrix_cells   // the support matrix's llama, tensor-twin, dim and phi3 rows
require _metal_prefill_cells   // the Metal prefill parity rows
require _metal_decode_cells   // the Metal decode parity rows
require _metal_mtp_cells   // mtp_verify_metal: the 1B's same-slab verify control
require _metal_format_census_cells   // the format census's q51 and NVFP4 rows
require _kernel_coverage_cells   // the coverage census's zoo, Vulkan and sweep rows
require _perop_ffn_cells   // the per-op dense FFN rail's rows
require _metal_blob_twin   // blob_twin: the own-load test's twins
require ?vulkan dasllama/dasllama_vulkan_common   // nolint:STYLE030,LINT019 - COOPMAT_*

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // each block's CPU form, and the by-design passes, prefill on the CPU
}

// The llama family on the whole-model driver under DASLLAMA_GPU=1, on the Vulkan home or under DASLLAMA_CUDA=1 on the CUDA
// home (the CUDA pass keeps the file by its `resident_llama` token): the NORM rope (adjacent pairs), no q/k/v bias, no q/k
// norm, GQA, the tied classifier of the 3.2 files. Each model file is ONE block (`with_cpu_and_gpu_model`): its forms load
// once each and the CPU form once a mirror codec, the cells collect what the compares read and the compares follow the
// block; a cell of one form alone runs on its load under the label its cell carried before the fold.
// - The forced feed of every carrier's default form against the CPU form (`dense_feeds`): the same fixed tokens through the
//   device and the CPU chain, the prefill logits and every step's within the dense bar, the one-step-off control past it; on
//   the Vulkan home with the arm witnesses that the file is a llama with neither bias nor NEOX rope and the pool's spin
//   window - the GPU-served window in force while the driver is armed, the CPU window back after the drop - on the CUDA
//   home with the driver's counters (every position served, the window chains, the captured graph, the unsplit form) and
//   their control. Llama-3.2-1B Q8_0, its Q5_1 mint and Llama-3.2-3B Q8_0 at one window and two on both homes; the Q4_0
//   mint at one window and two on the Vulkan home, one on the CUDA home; Llama-3.1-8B Q4_K_M (the mixed Q8_0 and K-quant
//   planes) at one window and two on the Vulkan home alone; on the CUDA home alone Llama-3.2-1B Q4_K_M at one window and
//   two, the Q5_0 mint, IQ4_XS and the IQ4_NL mint at one window, and the NVFP4 mint on the FP4 tensor leaf at one window
//   and two held by the same-or-better witness against the f32 CPU form (`FP4_F32_DIST_REL`), its MMQ-off form on the cm2
//   tiles at the dense bar the FP4 arm's control.
// - On the one-region forms the short window's peel (`peel_cell`) on the Q8_0, Q5_1 and Q4_K_M files and the greedy chain
//   (`greedy_chain_rows`) on the Q8_0 file, on both homes; the Q5_1 mint's q51 tag on every dense plane on the Vulkan home.
// - Llama-3.2-1B Q8_0: on the Vulkan home the batched bench row at two regions; on the CUDA home the N-row command's
//   batched rows at two regions and at twelve (on the rows tensor leaf and with its knee off), the q8_0 K/V mirror's forced
//   feeds at one window and two, the route-off load running the window and eight steps on the CPU rails, and the window
//   chain's planes taken at the arm (a load under the window-fault seat refusing a prompt window by name, then the
//   one-region form, the seat lifted, holding the planes before any prompt). The kernel
//   skip (`kernel_skip_rows`) runs on both homes on the default form, after its feeds. On the CUDA home two lifecycle cells
//   load the file on their own, after every block: a reload after a drop reads an eight-row tail bit for bit as the first
//   load did, and, the file's last cell, the tier's shutdown leaves no CUDA object or allocation live.
// - Llama-3.2-1B Q4_K_M on both homes: the q8_0 mirror's forced feed and the tq4 mirror's distance form at 40 tokens, and
//   the regions rig's batched steps at two regions (one window and past the unsplit position), twelve (the column tiles,
//   the rows tensor leaf on the CUDA home) and twenty (two commands of ten); on the CUDA home the N-row command's two rows.
// - Llama-3.2-1B Q8_0 (the ids form: the driver gathers the embedding) and Q4_K_M (the x form: the host embeds) on both homes:
//   the segmented window's cells (`dense_segments_cells`, `_segment_cells.das`) on a segment form - two regions, the planes
//   on the host, the `SEG_WINDOW`-row prompt window - their by-design passes reset after them.
// - Llama-3.2-1B IQ4_XS on the Vulkan home: the q8_0 mirror's forced feed and the tq4 mirror's distance form at 40 tokens.
// - Llama-3.2-1B Q8_0's default form also runs the last layer's FFN slice (`_ffn_slice_cells.das`), and its q8 uncached CPU form,
//   on every box, the chat cells (`_chat_cells.das`) and the GGUF
//   twin's side of the 1B MLX 8-bit snapshot's compare, the
//   snapshot's own block following the file's in the same test
//   (`_mlx_cells.das`).
// - On every box, the parity row of each 1B file model_specs pins one for - Q8_0, the Q4_0 and IQ4_NL mints, Q4_K_M, IQ4_XS
//   and the NVFP4 mint - on its CPU form (the row's load mode and K-quant knob under DASLLAMA_PIN_BACKEND's pin): the row's
//   frozen greedy ids (`parity_spec_cell`); the form skips where the box cannot take the pin.
// - SmolLM2-135M and tinyllama-1.1b, CPU forms alone on every box: the cells their topic files' fixture modules hold
//   (`_scheduler_cells.das`, `_batch_decode_cells.das`, `_kv_codec_cpu_cells.das`, `_kv_paged_cells.das`,
//   `_fused_decode_cells.das`, `_parity_cells.das`, `_chat_cells.das`, `_sampling_cells.das`, `_embedding_cells.das`);
//   SmolLM2's portable-pinned CPU form carries the cells whose load lays the q8 rows out for the portable backend (the
//   q8_0 K/V decode cell, the fused attention chain's portable pass and the deep-context witness cells).
// - Llama-3.2-1B Q8_0's Metal form (`metal_form`, the blob twin) carries the Metal batched driver's cells
//   (`_metal_batched_cells.das`): the batched row, the batched gather, the pre-encode rail, the landing across a move and a
//   release, and the per-row tick. The form is one skipped cell on a build without das_metal.
// - The Metal suites' cells ride the files' Metal forms - the blob
//   twin, an f32-scale twin where a file's rows read both weight
//   scales, the served image for the same-slab verify - their CPU
//   controls on the portable-pinned CPU forms, their compares after
//   each block: the support matrix (`_metal_matrix_cells.das`), the
//   prefill and decode parity (`_metal_prefill_cells.das`,
//   `_metal_decode_cells.das`), the MTP verify
//   (`_metal_mtp_cells.das`), the format census
//   (`_metal_format_census_cells.das`), the kernel coverage census
//   (`_kernel_coverage_cells.das`: its Vulkan rows and coopmat mode
//   sweep on device forms); the per-op dense FFN rail's cells
//   (`_perop_ffn_cells.das`) on per-op forms.
// Every test skips off the JIT, without DASLLAMA_GPU=1 on a device the driver serves, without its file under models_dir(),
// or where a form's plan does not fit the card; a codec form is a held skip where DASLLAMA_VK_KV32=1 arms an f32 mirror,
// a two-window feed where the prompt window is not PF_WINDOW_ROWS rows. Under DASLLAMA_CUDA=1 the Vulkan home's cells -
// the arm and spin witnesses, the q51 tags, the bench row, the 8B, the IQ4_XS codec cells - give way, each named in a
// skipped cell, and without it the CUDA home's cells do the same. The segmented window's capture cell skips under
// DASLLAMA_GPU_DN_SNAPS=1: a region then keeps one checkpoint row, and the cell plants two.

let private FILE_1B = "Llama-3.2-1B-Instruct-Q8_0.gguf"
let private FILE_1B_K = "Llama-3.2-1B-Instruct-Q4_K_M.gguf"   //! the K-quant planes (q4_K, q6_K): the kq GEMV leaves, the Q8_K feeds, the kq cm2 tiles
let private FILE_1B_Q51 = "Llama-3.2-1B-Instruct-Q5_1-local.gguf"   //! every dense plane q51 (the mint's recipe row in `../performance/model_specs.das`): the per-32 GEMV leaves, the s tile, the q51 table gather
let private FILE_1B_Q40 = "Llama-3.2-1B-Instruct-Q4_0-local.gguf"   //! the q40 planes: the window chain's kq batch arm off the f16 feed and the cm2 tiles on it
let private FILE_1B_IQ4NL = "Llama-3.2-1B-Instruct-IQ4_NL-local.gguf"   //! iq4nl planes
let private FILE_1B_NV4 = "Llama-3.2-1B-Instruct-NVFP4-local.gguf"   //! the nv4 planes: the window on the FP4 tensor rung (W4A4), the steps on the nv4 GEMV leaves

//! of the step's max |logit|: the codec carriers' q8_0 mirror bar - both chains quantize each query, each its own, so two
//! roundings part; the Q4_K_M rows read 0.25-0.37 of an 8.4-9.3 max logit, the IQ4_XS rows 0.29-0.42 of 8.7-9.4
let private CODEC_Q8KV_BAR_REL = 0.10
//! the tq4 distance form: the device's distance from the f16 chain, of the CPU codec chain's own, reads 0.99 (Q4_K_M) and
//! 1.00 (IQ4_XS), the f16 rows one step off 4.7 and 5.0
let private CODEC_DIST_REL = 1.15
//! the two codec chains' distance from each other, of the CPU codec chain's from f16: reads 1.02 and 1.13, the CPU codec rows
//! one step off 4.9 and 5.1. The codec moves these carriers' logits a point a row, three times the kernels' rounding, so the
//! chains part as two independent quantizations do - under the square root of two.
let private CODEC_PAIR_REL = 1.4

//! the FP4 leaf's witness ratio: W4A4's e2m1 activations carry one mantissa bit against the CPU chain's seven, so single logits
//! scatter past any max |logit| bar a one-step-off control still clears (a 48-token window read 0.24, its control 0.16); over a
//! window and eight steps the device reads 2.6 to 3.5 times the CPU chain's distance from the f32 chain, the control 18 to 21
let private FP4_F32_DIST_REL = 4.0

let private BATCH_STEPS = 6l
let private BATCH_PROMPT_A = 40l
let private BATCH_PROMPT_B = 57l
let private BATCH_RIG_CTX = 2048l   //! the Q8_0 batched forms' mirror context a region: twelve regions of the 1B's f16 rows fit the card beside the planes

//! one form's share of a llama file's forced feeds (`dense_feeds_rows`), the llama arm held off the default form: the family,
//! and on the Vulkan home neither q/k/v bias nor the NEOX rope, the pool's GPU-served spin window while the driver is armed,
//! every call of the feeds served, and the CPU window back once the CPU form's load dropped the driver
def private llama_feed_rows(var c : DenseFeeds; m : Model; form : ModelForm) {
    if (device_default(form) && !c.cuda) {
        c.carrier |> held_fact(!m.config.attn_qkv_bias && !m.config.rope_neox, "the llama arm: no q/k/v bias, the NORM rope")
        c.carrier |> held_equal(get_jobque_spin_in_force(), get_jobque_spin_gpu_us(), "the armed driver put the GPU-served spin window in force")
    }
    let side = dense_feeds_rows(c, m, form, "llama")
    return if (c.cuda)
    if (side == MainSide.device) {
        c.carrier |> held_served("the device served every call of the resident run")
    } elif (side == MainSide.cpu) {
        c.carrier |> held_equal(get_jobque_spin_in_force(), get_jobque_spin_us(), "the dropped driver handed the pool the CPU window back")
    }
}

//! the codec cells of a carrier: the q8_0 mirror's forced feeds (a prompt each of `prompts`) and the tq4 mirror's distance form
//! at 40 tokens - the device form's rows, the CPU form's on the codec, and for the distance form at f16; each `_ran` says its
//! device form collected, `h` the codecs the environment keeps off
struct LlamaCodec {
    prompts : array<int64>
    q8 : array<FeedRows>
    q8_ran : bool
    q8_cpu : array<FeedRows>
    tq4 : array<array<float>>
    tq4_ran : bool
    tq4_cpu : FeedRows
    tq4_f16 : FeedRows
    h : Held
}

//! one form's share of the codec cells: the device rows on the forms asking a codec, the CPU form's rows on the codec it answers
def private llama_codec_rows(t : T?; var c : LlamaCodec; m : Model; form : ModelForm) {
    if (form.on_gpu && form.kv_asked && form.kv == KVDtype.q8_0) {
        c.q8 |> reserve(length(c.prompts))
        for (n in c.prompts) {
            c.q8 |> emplace <| resident_feed_rows(m, form, n, CODEC_FED_STEPS)
        }
        c.q8_ran = true
    } elif (form.on_gpu && form.kv_asked && form.kv == KVDtype.tq4) {
        c.tq4 <- codec_device_rows(t, m, 40l, KVDtype.tq4, false)
        c.tq4_ran = true
    } elif (!form.on_gpu && form.kv == KVDtype.q8_0 && c.q8_ran) {
        c.q8_cpu |> reserve(length(c.prompts))
        for (n in c.prompts) {
            c.q8_cpu |> emplace <| resident_feed_rows(m, form, n, CODEC_FED_STEPS)
        }
    } elif (!form.on_gpu && form.kv == KVDtype.tq4 && c.tq4_ran) {
        c.tq4_cpu <- resident_feed_rows(m, form, 40l, CODEC_FED_STEPS)
        var f16 = form
        f16.kv = KVDtype.f16
        c.tq4_f16 <- resident_feed_rows(m, f16, 40l, CODEC_FED_STEPS)
    }
}

//! the codec cells' compares after the block: each q8_0 feed against the CPU chain on q8_0 sessions, then the tq4 distance form
//! as its own cell `tq4_title`
def private llama_codec_within(t : T?; file, tq4_title : string; c : LlamaCodec) {
    held_within(t, c.h, "{file}: ", false)
    with_dasllama_jobque_() {
        feeds_within(t, "{file} q8_0 mirror, ", c.q8, c.q8_cpu, CODEC_Q8KV_BAR_REL)
        if (c.tq4_ran) {
            unsafe {
                t |> run(tq4_title) @capture(& c)(t : T ?) {
                    return if (!twin_collected(t, 0, empty(c.tq4_cpu.rows) ? 0 : 1, "tq4, 40 tokens: "))
                    codec_distances_within(t, "tq4, 40 tokens", c.tq4, c.tq4_cpu.rows, c.tq4_f16.rows, 0, CODEC_DIST_REL,
                            CODEC_PAIR_REL) $(id) {
                        return feed_piece(c.tq4_cpu, c.tq4_f16, id)
                    }
                }
            }
        }
    }
}

//! the short window's peel (`peel_cell`) on the one-region form as its own cell `title`, the tag naming the home
def private peel_on(t : T?; var m : Model; file, title : string) {
    let tag = gpu_want_cuda() ? "{file} cuda, " : "{file}, "
    unsafe {
        t |> run(title) @capture(& m)(t : T ?) {
            peel_cell(t, m, SPLIT_BAR_REL, tag, true)
        }
    }
}

//! the greedy chain's walks (`greedy_chain_rows`) on the Q8_0 file's one-region form as their own cell
def private greedy_chain_on(t : T?; var m : Model) {
    let title = (gpu_want_cuda() ? "{FILE_1B}: the greedy chain's walks on the CUDA driver"
        : "Llama-3.2-1B Q8_0: the greedy chain's steps are the plain steps bit for bit, hits on the pick and misses off it")
    unsafe {
        t |> run(title) @capture(& m)(t : T ?) {
            var inscope h : Held
            greedy_chain_rows(h, m, path_join(models_dir(), FILE_1B))
            held_within(t, h)
        }
    }
}

//! the batched bench row on the 1B, the driver homing two streams: a refusal names the streams the device homes against
//! the row's ask and reads 0, and a rep the device can home serves - a rate, no refusal, every stream parked after it
//! (a one-stream row is no batch: the step census counts it a row at a time and the row refuses by name)
def private batched_row_cell(t : T?; var m : Model) {
    unsafe {
        t |> run("bench_tg_batched_rep: the refusal past the homed streams, then one served rep") @capture(& m)(t : T ?) {
            let homes = gpu_device_sessions()
            t |> success(homes >= 2l, "the armed driver homes two streams ({homes})")
            let over_home = homes + 1l
            let refused = bench_tg_batched_rep(m, over_home, 32l, 4l, 1l)
            t |> equal(refused.tps, 0.0lf, "a row asking more streams than the device homes reads 0")
            t |> equal(refused.why, "the device homes {homes} stream(s), the row asks {over_home}", "... and names the refusal")
            let served = bench_tg_batched_rep(m, homes, 32l, 4l, 1l)
            t |> equal(served.why, "", "a row the device homes is served: {served.why}")
            t |> success(served.tps > 0.0lf, "... at a rate ({served.tps} tok/s summed over {homes} stream(s))")
            t |> equal(gpu_device_sessions(), homes, "the rep parked every stream: the device homes as many as before")
            var passes <- gpu_cpu_passes_()
            t |> equal(length(passes), 0, "the device served every call of the rep: {passes}")
            delete passes
        }
    }
}

//! a model the whole-model driver does not take under the CUDA ask runs on the CPU rails: this home has no per-op tier, so the
//! route lever off (the per-op form) leaves every plane on the host - the window and eight steps run with no per-op stub reached
def private declined_runs_on_cpu_cell(t : T?; var m : Model) {
    unsafe {
        t |> run("{FILE_1B} with the whole-model route off under the CUDA ask: no per-op tier serves, the CPU rails run the window and eight steps") @capture(& m)(t : T ?) {
            t |> success(!moe_gpu_resident_active(), "the whole-model route is off: the driver did not take the model")
            var placed = 0l
            for (l in range64(m.config.n_layers)) {
                placed += moe_gpu_layer_on_gpu(l) ? 1l : 0l
            }
            t |> equal(placed, 0l, "no layer sits on the device: this home has no per-op tier to place one")
            var inscope prompt <- resident_prompt(40l)
            var inscope toks <- resident_feed(8l)
            var inscope got <- feed_alone(m, mirror_kv_dtype(), prompt, toks)
            t |> success(long_length(got) >= 8l, "the window and eight steps landed on the CPU rails ({long_length(got)} rows)")
        }
    }
}

//! the window chain's planes are the arm's, on the CUDA home: under the window-fault seat (the fault form, `cuda_window_fault`)
//! the driver arms without them and a prompt window refuses by name
def private window_planes_fault_cell(t : T?; var m : Model) {
    unsafe {
        t |> run("{FILE_1B}: the window planes are taken at the arm, and a window whose planes found no room refuses by name") @capture(& m)(t : T ?) {
            t |> success(moe_gpu_resident_active(), "under the seat the whole-model driver still arms (the load log names any decline)")
            t |> success(!cuda_window_planes_held(), "... without its window chain's planes")
            var inscope prompt <- resident_prompt(40l)
            var inscope toks <- resident_feed(2l)
            var s <- create_session(m, mirror_kv_dtype())
            var why = ""
            try {
                var rows <- feed_logits(m, s, prompt, toks)
                delete rows
            } recover {
                why = this_context().last_exception
            }
            t |> success(find(why, "found no room on the device when the driver armed") >= 0, "a prompt window refuses by name: '{why}'")
            delete s
        }
    }
}

//! the next load after the fault form, the seat lifted (the one-region form, first on its load): the planes held before any
//! prompt, and the window and its steps served on the CUDA driver
def private window_planes_lifted_cell(t : T?; var m : Model) {
    unsafe {
        t |> run("{FILE_1B}: the seat lifted, the next load holds the window planes before any prompt and serves the window on them") @capture(& m)(t : T ?) {
            t |> success(moe_gpu_resident_active(), "the seat lifted, the driver arms again")
            t |> success(cuda_window_planes_held(), "... holding its window chain's planes before any prompt")
            var inscope prompt <- resident_prompt(40l)
            var inscope toks <- resident_feed(2l)
            let c0 = cuda_positions_served()
            var inscope got <- feed_alone(m, mirror_kv_dtype(), prompt, toks)
            t |> success(long_length(got) >= 2l, "the window and its steps are served ({long_length(got)} rows)")
            cuda_served_since(t, c0, 40l, "the window over the planes the arm took")
            served_every_call(t)
        }
    }
}

//! the segmented window's cells (`dense_segments_cells`) on a file's segment form as their own cell `title`; the passes its
//! declines leave by design reset after it, before the form's served witness
def private segments_on(t : T?; var m : Model; title : string) {
    unsafe {
        t |> run(title) @capture(& m)(t : T ?) {
            t |> equal(m.arch, "llama", "the file is a llama-family model")
            dense_segments_cells(t, m)
        }
    }
    reset_gpu_cpu_passes()
}

//! the segment form: two regions, the planes on the host (the declines' CPU prefill reads them), the segment window
def private segment_form() : ModelForm => device_form(regions = 2l, host = true, pf_window_rows = SEG_WINDOW)

//! `cpus` added to `forms` where the build serves the Metal home: the
//! CPU forms a block's Metal cells read their controls on
def private metal_cpu_forms(var forms : array<ModelForm>; cpus : array<ModelForm>) {
    if (empty(metal_build_why())) {
        forms |> push_from(cpus)
    }
}

//! the per-op FFN rail's forms (`_perop_ffn_cells.das`) of `file`,
//! the dense FFN rail pinned off then on, where the arm filter
//! reaches `vk-perop-ffn`: on the Vulkan home, skipped cells on the
//! CUDA home
def private perop_ffn_forms(t : T?; file : string; var forms : array<ModelForm>) {
    return if (!empty(arm_why("vk-perop-ffn")))
    forms_or_skips(t, forms, file,
        [device_form(perop = true, dense_ffn = 0), device_form(perop = true, dense_ffn = 1)],
        vulkan_home_why("the per-op FFN rail is the Vulkan home's cell"))
}

//! a per-op form pinning the dense FFN rail
def private perop_ffn_form(f : ModelForm) : bool => f.perop && f.dense_ffn >= 0

//! the CPU form answering the device forms: the q8 cached load
def private main_cpu_pass(f : ModelForm) : bool
    => !f.on_gpu && same_cpu_load(f, cpu_quant_form(QuantMode.q8))

//! the sweep's vehicle carrying `file` (`cov_vk_sweep_row`), -1 none
def private sweep_of(file : string) : int {
    for (i in range(COV_VK_SWEEP_ROWS)) {
        var inscope row <- cov_vk_sweep_row(i, false)
        return i if (row.file == file)
    }
    return -1
}

//! the sweep's forms of the vehicle `file` where its arm runs, each a
//! census form (`vk_census_form`): the box's mode, the KHR and sdot4
//! modes, and on a vehicle with twins the f16 and int8 modes; skipped
//! cells on the CUDA home and on a build with no Vulkan
def private sweep_forms(t : T?; var forms : array<ModelForm>; file : string) {
    return if (!empty(arm_why("coverage-vk")))
    static_if (typeinfo builtin_module_exists(vulkan)) {
        var inscope legs <- [vk_census_form(), vk_census_form(COOPMAT_MM),
            vk_census_form(COOPMAT_SDOT4)]
        if (cov_vk_sweep_twin(sweep_of(file))) {
            legs |> push(vk_census_form(COOPMAT_F16))
            legs |> push(vk_census_form(COOPMAT_INT8))
        }
        forms_or_skips(t, forms, file, legs,
            vulkan_home_why("the coopmat mode sweep is the Vulkan home's cell"))
    } else {
        skip_cell(t, ("{file}, the coopmat mode sweep's device forms: no Vulkan home in this " +
            "build"))
    }
}

//! whether `form` is a leg of the sweep (`sweep_forms`)
def private sweep_leg(f : ModelForm) : bool => vk_census_asked() && vk_census_form_is(f)

//! a sweep form forcing the f16 or int8 mode: the twin leg
[unused_argument(f)]
def private sweep_twin_leg(f : ModelForm) : bool {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return f.coopmat == COOPMAT_F16 || f.coopmat == COOPMAT_INT8
    } else {
        return false
    }
}

//! the sweep's row of the vehicle `file` on its leg `form`, where the
//! form is one
def private sweep_cell(t : T?; var m : Model; form : ModelForm; file : string) {
    return if (!sweep_leg(form))
    coverage_vk_model(t, m, form, cov_vk_sweep_row(sweep_of(file), sweep_twin_leg(form)))
}

//! a tensor-twin row's carrier absent on a das_metal build under the
//! `tensor` arm: red, never a skip - the carriers are provisioned
//! (`fetch_models.das`), and a box that skips ships every tensor twin
//! unexercised
def private tensor_carrier_held(t : T?; file : string) {
    return if (!empty(metal_build_why()) || !empty(arm_why("tensor")))
    let path = path_join(models_dir(), file)
    if (!stat(path).is_valid) {
        t |> failure(("tensor twin carrier missing: {path} - bin/daslang " +
            "modules/dasLLAMA/performance/fetch_models.das -- --fetch"))
    }
}

//! the launch counts of three stamp lists, one after another
def private stamp_counts(col_stamps, any_stamps, no_stamps : array<string>) : array<int> {
    var n : array<int>
    n |> reserve(length(col_stamps) + length(any_stamps) + length(no_stamps))
    for (list in [unsafe(addr(col_stamps)), unsafe(addr(any_stamps)), unsafe(addr(no_stamps))]) {
        for (name in *list) {
            n |> push(cud_kernel_coverage_of(name))
        }
    }
    return <- n
}

//! since `before` (`stamp_counts`): every stamp of `col_stamps` launched, one of `any_stamps` at the least (empty = none
//! asked), none of `no_stamps`
def private stamps_launched(t : T?; col_stamps, any_stamps, no_stamps : array<string>; before : array<int>) {
    for (name, i in col_stamps, count()) {
        let n = cud_kernel_coverage_of(name) - before[i]
        t |> success(n > 0, "the batched steps launched the stamp {name} ({n} launches)")
    }
    var any_launched = 0
    for (name, i in any_stamps, count(length(col_stamps))) {
        any_launched += cud_kernel_coverage_of(name) - before[i]
    }
    t |> success(empty(any_stamps) || any_launched > 0, "the batched steps launched one of the stamps {any_stamps} ({any_launched} launches)")
    for (name, i in no_stamps, count(length(col_stamps) + length(any_stamps))) {
        t |> equal(cud_kernel_coverage_of(name) - before[i], 0, "the batched steps launched no {name}: another form serves its sites")
    }
}

//! the CUDA home's N-row token command (`ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command` on this home): `rows` sessions over
//! prompts of their own lengths (every prompt distinct), batched `BATCH_STEPS` steps through the N-row command against each
//! alone on `tr` armed at `rows` regions - every row's logits a step within the split bar of the session alone, the row one
//! step off the control, the N-row counter unmoved by the sessions alone and its row counter up by every row of every step;
//! every stamp of `col_stamps` launched by the batched steps, one of `any_stamps` at the least (empty = none asked), and none
//! of `no_stamps`
def private batched_rows_cell(t : T?; tr : Model; file : string; rows : int64; col_stamps, any_stamps, no_stamps : array<string>) {
    t |> equal(rdec_token_n_rows(), RDEC_NROW_MAX, "the N-row command steps {RDEC_NROW_MAX} rows at once on the armed driver")
    var prompts, feeds : array<array<int64>>
    prompts |> reserve(rows)
    feeds |> reserve(rows)
    for (i in range64(rows)) {
        prompts |> emplace <| resident_prompt(i % 2l == 0l ? BATCH_PROMPT_A + i : BATCH_PROMPT_B + i, i * 7l + 1l)
        feeds |> emplace <| resident_feed(BATCH_STEPS)
    }
    let steps_alone0 = rdec_nrow_steps()
    var alone : array<array<array<float>>>
    alone |> reserve(rows)
    for (i in range64(rows)) {
        alone |> emplace <| feed_alone(tr, mirror_kv_dtype(), prompts[i], feeds[i])
    }
    t |> equal(rdec_nrow_steps() - steps_alone0, 0l, "the sessions alone stepped through the one-row command: the N-row counter's control")
    t |> success(logits_maxabs(alone[0][0]) < INFINITE_DIFF && logits_maxabs(alone[1][0]) < INFINITE_DIFF, "the first two prompts' logits are finite")
    t |> success(logits_maxdiff(alone[0][0], alone[1][0]) > 0.0, "the prompts read different logits, so a crossed region cannot pass")
    let rows0 = rdec_nrow_rows()
    let steps0 = rdec_nrow_steps()
    var before <- stamp_counts(col_stamps, any_stamps, no_stamps)
    var got <- batched_logits_rows(t, tr, prompts, feeds, mirror_kv_dtype(), BATCH_STEPS) <| $ => rdec_nrow_steps()
    let commands = rdec_nrow_steps() - steps0
    t |> equal(rdec_nrow_rows() - rows0, rows * commands, "every N-row command carried all {rows} rows ({rdec_nrow_rows() - rows0} rows over {commands} commands)")
    stamps_launched(t, col_stamps, any_stamps, no_stamps, before)
    batched_rows_within(t, tr, file, rows, got, alone)
    delete before
    served_every_call(t)
    delete got
    delete alone
    delete prompts
    delete feeds
}

//! `batched_rows_cell`'s compares: every batched row a step within the split bar of its session alone, the previous step
//! outside it, every row the compare and its control read finite, both sides' argmax rows decoded
def private batched_rows_within(t : T?; tr : Model; file : string; rows : int64; got : array<array<float>>; alone : array<array<array<float>>>) {
    var worst = 0.0
    var tight = 1.0e30
    for (st in range64(BATCH_STEPS)) {
        for (i in range64(rows)) {
            let finite = (logits_maxabs(alone[i][st + 1l]) < INFINITE_DIFF && logits_maxabs(alone[i][st]) < INFINITE_DIFF
                && logits_maxabs(got[st * rows + i]) < INFINITE_DIFF)
            t |> success(finite, "batched step {st}, row {i}: the batched row, the session's row and its previous step are finite")
            let bar = SPLIT_BAR_REL * logits_maxabs(alone[i][st + 1l])
            let diff = logits_maxdiff(got[st * rows + i], alone[i][st + 1l])
            let off = logits_maxdiff(got[st * rows + i], alone[i][st])
            to_log(LOG_INFO, "{file} batched step {st}, row {i}: maxdiff {diff}, bar {bar}, the previous step {off} off\n")
            t |> success(diff <= bar, "batched step {st}, row {i}: within the split bar of the session alone (maxdiff {diff}, bar {bar})")
            t |> success(off > bar, "batched step {st}, row {i}: the session's previous step lands outside the bar (the control, {off})")
            worst = max(worst, diff / bar)
            tight = min(tight, off / bar)
        }
    }
    for (i in range64(rows)) {
        var inscope got_i, want_i : array<array<float>>
        got_i |> reserve(BATCH_STEPS)
        want_i |> reserve(BATCH_STEPS)
        for (st in range64(BATCH_STEPS)) {
            got_i |> push_clone(got[st * rows + i])
            want_i |> push_clone(alone[i][st + 1l])
        }
        log_argmax_rows(tr, "{file} batched row {i}: the session alone, then the batched rows", got_i, want_i)
    }
    to_log(LOG_INFO, "{file} at {rows} rows: the widest miss {worst} of the bar, the tightest control {tight} bars\n")
}

//! the Q8_0 file's N-row cells on the CUDA home: two sessions at two regions with the rows leaf's knees at 0 (the q8 two-column
//! stamp launched), or twelve at twelve regions on the rows tensor leaf and again with its knee pinned off
def private cuda_batch_q8_cells(t : T?; var m : Model; rows : int64) {
    unsafe {
        if (rows == 2l) {
            t |> run("{FILE_1B}: two sessions at two regions, six batched steps through the N-row command against each alone") @capture(& m)(t : T ?) {
                var inscope none : array<string>
                with_rows_leaf(false) {
                    batched_rows_cell(t, m, FILE_1B, 2l, ["q8_gemv_n2_cls_cuda"], none, none)
                }
            }
            return
        }
        t |> run("{FILE_1B}: twelve sessions at twelve regions, one command of twelve rows a step on the rows tensor leaf") @capture(& m)(t : T ?) {
            var inscope none : array<string>
            with_rows_leaf(true) {
                batched_rows_cell(t, m, FILE_1B, 12l, none, ["q8_rows_mma_n_cuda", "q8_rows_mma_w_cuda"], ["q8_gemv_n8_cls_cuda", "q8_gemv_n4_cls_cuda"])
            }
        }
        t |> run("{FILE_1B}: the same with the leaf's knee pinned off (a tile of eight columns and one of four)") @capture(& m)(t : T ?) {
            var inscope none : array<string>
            with_rows_leaf(false) {
                batched_rows_cell(t, m, FILE_1B, 12l, ["q8_gemv_n8_cls_cuda", "q8_gemv_n4_cls_cuda"], none, ["q8_rows_mma_n_cuda", "q8_rows_mma_w_cuda"])
            }
        }
    }
}

//! what the Q8_0 file's Metal forms, its per-op FFN forms and their
//! CPU forms keep for the compares after the block: the support
//! matrix's window and tensor-twin rows, the prefill cells', the
//! decode legs' on each weight scale, the per-op FFN rail's, and the
//! GGUF twin's side of the 8-bit MLX snapshot's compare
struct Llama1bKept {
    matrix : MatrixLlama1bKept
    tensor : MatrixTensorKept
    prefill : PrefillKept
    decode : DecodeKept
    decode_s32 : DecodeKept
    perop : PeropFfnKept
    twin : MlxPairSide
}

//! what the Q8_0 file's forms collect: the default form's feeds and the q8_0 mirror's feeds
struct Llama1bRows {
    feeds : DenseFeeds
    codec : LlamaCodec
    far_row : Held   //! the CUDA token step's far row on the default form
    kept : Llama1bKept
}

//! the Q8_0 file's blob twins' context: the largest its cells cut
//! from, the decode legs' depth
let private LLAMA_1B_METAL_CTX = DECODE_CTX_LLAMA_1B_Q8

//! the Q8_0 file's Metal forms past its default twin: the f32-scale
//! twin, the served image where the arm filter reaches a verify arm,
//! and the CPU forms their controls run on
def private llama_1b_metal_forms(t : T?; var forms : array<ModelForm>) {
    forms |> push(metal_form(ctx = LLAMA_1B_METAL_CTX, wscale_f32 = true))
    var inscope verify <- mtp_verify_arms("l1b")
    if (any_arm_on(verify)) {
        forms |> push(metal_form(image = true, ctx = METAL_MTP_CELLS_CTX))
    } else {
        skip_cell(t, "{FILE_1B}: the served image's verify cells: the arm filter reaches none")
    }
    metal_cpu_forms(forms, [prefill_cpu_form(), decode_cpu_form(true)])
}

//! the Q8_0 file's Metal forms' cells: on the served image the
//! same-slab verify control; on the f32-scale twin the support
//! matrix's f32 rows, the C1 zoo row and the decode legs; on the
//! default twin the support matrix's f16 rows, the decode legs (their
//! refusal arm reads a form no step has refused on yet, so they run
//! ahead of the batched driver's cells), the batched driver's cells,
//! the C2 zoo row, then the tensor twins and the prefill cells
def private llama_1b_metal_rows(t : T?; var k : Llama1bKept; var m : Model; form : ModelForm) {
    if (form.metal_image) {
        mtp_verify_metal(t, m, form, mtp_verify_llama_1b())
        return
    }
    matrix_llama_1b_metal(t, m, form, k.matrix)
    if (form.wscale_f32) {
        coverage_zoo_metal(t, m, form, cov_zoo_c1_llama_q8())
        decode_parity_llama_1b_q8_metal(t, m, form, k.decode_s32)
        return
    }
    decode_parity_llama_1b_q8_metal(t, m, form, k.decode)
    metal_llama_1b_cells(t, m)
    coverage_zoo_metal(t, m, form, cov_zoo_c2_llama_q8_s16())
    matrix_tensor_metal(t, m, form, matrix_tensor_q8(), k.tensor)
    prefill_parity_metal(t, m, form, prefill_llama_1b_q8(), k.prefill)
}

//! the Q8_0 file's Metal cells' CPU sides, each acting on its CPU
//! form's pass: the default scale's portable form, the f32 twin's
//! decode controls on the f32 scale's
def private llama_1b_metal_cpu(t : T?; var k : Llama1bKept; var m : Model; form : ModelForm) {
    matrix_llama_1b_cpu(t, m, form, k.matrix)
    matrix_tensor_cpu(t, m, form, matrix_tensor_q8(), k.tensor)
    decode_parity_cpu(t, m, form, k.decode)
    decode_parity_cpu(t, m, form, k.decode_s32)
    prefill_parity_cpu(t, m, form, k.prefill)
}

//! the compares after the Q8_0 file's block of what its Metal, per-op
//! FFN and CPU forms kept
def private llama_1b_kept_within(t : T?; var k : Llama1bKept) {
    matrix_llama_1b_verdict(t, k.matrix)
    matrix_tensor_verdict(t, matrix_tensor_q8(), k.tensor)
    prefill_parity_verdict(t, k.prefill)
    decode_parity_verdict(t, k.decode)
    decode_parity_verdict(t, k.decode_s32)
    perop_ffn_verdict(t, perop_ffn_llama_1b_q8(), k.perop)
}

//! The Llama-3.2-1B MLX 8-bit snapshot's block, after its GGUF twin's
//! (`twin`, its uncached form's side): on its CPU form the snapshot's
//! side of the pair, compared after the block; on its Metal form the
//! served, per-32 and prefill cells, their CPU rails on `mlx_cpu_form`
def private llama_1b_mlx8_block(t : T?; twin : MlxPairSide) {
    let repo = "mlx-community/Llama-3.2-1B-Instruct-8bit"
    var forms <- [cpu_cells_form()]
    var inscope arms <- ["metal-mlx8", "metal-prefill-mlx8", "batch-per32"]
    var inscope metal <- mlx_metal_forms(t, repo, arms)
    forms |> push_from(metal)
    var k : MlxLlamaMetalKept
    var inscope snap : MlxPairSide
    with_cpu_and_gpu_snapshot(t, "llm", "mlx-community/Llama-3.2-1B-Instruct-8bit", MLX_SNAPSHOT_CTX, forms) $(var m, form) {
        if (form.on_metal) {
            mlx_llama_metal_cells(t, m, form, KqFmt.mlx8, "metal-mlx8", "metal-prefill-mlx8",
                decode_per32_mlx_1b_8bit(), k)
        } elif (alone_pass_of(form, cpu_cells_form())) {
            delete snap
            snap <- mlx_llama_1b_8bit_side(t, m)
        } else {
            mlx_llama_metal_cpu(m, form, k)
        }
    }
    mlx_llama_1b_8bit_within(t, twin, snap)
    mlx_llama_metal_verdict(t, k)
    delete forms
}

//! the Q8_0 file's forms: the default (the feeds), one region (the greedy chain, the peel), two regions (the bench row on the
//! Vulkan home, the N-row command's two rows on the CUDA home), the segment form; on the CUDA home the window-fault form at one region ahead of
//! the one-region form, twelve regions (the N-row command's twelve rows), the per-op form (the route off) and the q8_0 mirror
//! (a codec the environment keeps off held as a skip); on every box the q8 uncached CPU form whose chat cells stand alone
def private llama_1b_forms(t : T?; var r : Llama1bRows) : array<ModelForm> {
    if (!r.feeds.cuda) {
        var vk <- [device_form(), device_form(regions = 1l), device_form(regions = 2l, ctx = BATCH_RIG_CTX), segment_form(),
            cpu_cells_form(uncached = true), metal_form(ctx = LLAMA_1B_METAL_CTX)]
        perop_ffn_forms(t, FILE_1B, vk)
        llama_1b_metal_forms(t, vk)
        return <- vk
    }
    var forms <- [device_form(), device_form(regions = 1l, cuda_window_fault = true), device_form(regions = 1l),
        device_form(regions = 2l, ctx = BATCH_RIG_CTX), segment_form()]
    forms |> push(device_form(regions = 12l, ctx = BATCH_RIG_CTX))
    forms |> push(device_form(perop = true))
    perop_ffn_forms(t, FILE_1B, forms)
    r.codec.prompts <- [40l, TWO_WINDOWS]
    codec_forms(forms, r.codec.h, [KVDtype.q8_0], device_form()) $(kv) => "{FILE_1B}: the {kv} mirror's feeds on the CUDA driver"
    forms |> push(cpu_cells_form(uncached = true))
    forms |> push(metal_form(ctx = LLAMA_1B_METAL_CTX))
    llama_1b_metal_forms(t, forms)
    return <- forms
}

//! the home's kernel-skip setter on the Q8_0 file's default form (`kernel_skip_rows`) as its own cell, after the form's feeds
def private kernel_skip_on(t : T?; var m : Model) {
    let title = (gpu_want_cuda() ? "{FILE_1B}: the kernel skip re-captures the step without the skipped kernel, and clears"
        : "Llama-3.2-1B Q8_0: the kernel skip re-records the step without the skipped kernel, and clears")
    unsafe {
        t |> run(title) @capture(& m)(t : T ?) {
            kernel_skip_rows(t, m, path_join(models_dir(), FILE_1B), "da_attn_f16_cls", "q8_gemv_cls")
        }
    }
}

//! one form's collection of the Q8_0 file's cells into `r`; the fault, one-, two- and twelve-region forms' and the per-op
//! form's own cells run on their load, and the default form's kernel skip after its feeds
def private llama_1b_rows(t : T?; var r : Llama1bRows; var m : Model; form : ModelForm) {
    if (form.on_metal) {
        llama_1b_metal_rows(t, r.kept, m, form)
    } elif (!form.on_gpu && form.uncached) {
        chat_llama32_cells(t, m)
        r.kept.twin <- mlx_llama_1b_twin_side(m)
    } elif (!form.on_gpu && !main_cpu_pass(form)) {
        llama_1b_metal_cpu(t, r.kept, m, form)
    } elif (perop_ffn_form(form)) {
        perop_ffn_device_cell(t, m, form, perop_ffn_llama_1b_q8(), r.kept.perop)
    } elif (form.perop) {
        declined_runs_on_cpu_cell(t, m)
    } elif (form.pf_window_rows == SEG_WINDOW) {
        segments_on(t, m, "Llama-3.2-1B Q8_0: two sessions' tails as one segmented window, within and across windows, hold the split bar of their solo windows; ownership binds owners first and the decline arms fall back unsegmented")
    } elif (form.cuda_window_fault) {
        window_planes_fault_cell(t, m)
    } elif (form.on_gpu && form.regions == 1l) {
        if (r.feeds.cuda) {
            window_planes_lifted_cell(t, m)
        }
        greedy_chain_on(t, m)
        peel_on(t, m, FILE_1B, (r.feeds.cuda ? "{FILE_1B}: a short tail peels through the N-row command on the CUDA driver and reads what the one-row steps read"
            : "Llama-3.2-1B Q8_0: a short tail peels through the token commands and reads what the one-row steps read"))
    } elif (form.on_gpu && form.regions == 2l && r.feeds.cuda) {
        cuda_batch_q8_cells(t, m, 2l)
    } elif (form.on_gpu && form.regions == 2l) {
        batched_row_cell(t, m)
    } elif (form.on_gpu && form.regions == 12l) {
        cuda_batch_q8_cells(t, m, 12l)
    } elif (form.kv_asked) {
        llama_codec_rows(t, r.codec, m, form)
    } else {
        llama_feed_rows(r.feeds, m, form)
        if (!form.on_gpu) {
            llama_codec_rows(t, r.codec, m, form)
            perop_ffn_cpu(t, m, form, perop_ffn_llama_1b_q8(), r.kept.perop)
        } elif (device_default(form)) {
            if (r.feeds.cuda) {
                cuda_far_row_rows(r.far_row, m)
            }
            last_layer_slice_cell(t, m)
            kernel_skip_on(t, m)
        }
    }
}

[test(description = "On Llama-3.2-1B Q8_0 the resident chain holds the CPU chain's logits within 6 percent of the max over one and two windows plus eight steps, on the Vulkan home with the GPU spin window in force while armed, on the CUDA driver with every position served and the captured graph replayed, and a step with the far count pinned to the first key attending on the decode tile form at head 64 within the split bar of the row form; at one region a short tail of 1, 2, 5 and 8 rows peels within the split bar of the row-at-a-time tail, one row more or the lever off taking the window chain, and the greedy chain's steps reproduce the plain steps bit for bit; on the Vulkan home bench_tg_batched_rep refuses by name past the homed streams and serves a row it homes; on the CUDA home two and twelve sessions batched through the N-row command land every row within the split bar of each alone, on the rows tensor leaf and with its knee off, a q8_0 K/V mirror holds the CPU chain on q8_0 sessions within 10 percent over one and two windows, and with the whole-model route off no layer sits on the device and the CPU rails run the window and eight steps, and under the window-fault seat the driver arms without its window chain's planes and a prompt window refuses by name while the next load, the seat lifted, holds the planes before any prompt and serves the window and its steps; on the segment form two sessions' tails as one segmented window, within and across windows, hold the split bar of their solo windows, ownership binds owners first and the decline arms fall back unsegmented; on the default form the home's kernel-skip setter (`set_vulkan_kernel_skip`, under the CUDA ask `set_cuda_kernel_skip`) drops the recorded step: under a skip naming the decode attention the next step dispatches none of it and the q8 GEMV anew, and cleared the attention dispatches again, the unskipped step counting both alike, and a 512-token prefill with the last layer's FFN sliced yields the whole window's logits bit for bit, split-k pinned off for both arms; on the q8 uncached CPU form, on every box, the Llama-3 header prefill and the template's dated system header with and without a tool render, and the Llama-3.2-1B MLX 8-bit snapshot loads NEOX-roped with the file's config and mlx8 planes everywhere, its teacher-forced logits within 8 percent of the file's range and within 0.4 of the snapshot runtime's top-1 logits; on the parity row's CPU form, on every box, the file's frozen greedy ids token for token; on the Metal home's blob twin, one load on a das_metal build: on Llama-3.2-1B Q8 with no whole-model driver homing a stream, the batched bench row serves four host-cached streams at a rate, every timed step a Metal batched-driver device step, none on the CPU stack; on Llama-3.2-1B Q8, the Metal batched step gathers every row of three sessions off its token id on the device, its logits bit for bit those with the gather off, where the host embeds every row; on Llama-3.2-1B Q8, four greedy streams emit the same tokens with batched steps pre-encoded under the step before (16 or more taken) as encoded at call, and retiring a parked step leaks no Metal object; on Llama-3.2-1B Q8, two streams stepped through the Metal batched driver emit the same tokens with the pre-encode rail on and with the pipelined landing on as with each step built at its call, across a move of both sessions (the step encoded ahead commits through the sessions' new addresses) and the release of one (which leaves no landing naming a session, and lands the step in flight); on Llama-3.2-1B Q8, two sampled streams through two-row ticks the batched driver declines step one row at a time, landing every selection, and emit what each samples right after its own forward; on the Metal home, a das_metal build, on blob twins of the f16 and f32 weight scales the support matrix's weights x KV-dtype cells engage the Metal prefill, decode and batch paths, the npos window is planar-only and the MetalMode knob behaves, the coverage census's zoo rows dispatch their stamps, the batched and resident decode legs emit the portable CPU controls' tokens and their one-step gates hold, and on the f16 twin the forced tensor crowns serve a tmm2d twin whose generate matches the CPU truth token for token and the prefill parity cells hold the CPU controls; on the served image the same-slab verify rows hold the dff and vff control carrier within 0.5; with the whole-model route off on the Vulkan home the per-op dense FFN rail runs every layer's FFN row on the device within 6 percent of the CPU chain, the rail off its control; then the Llama-3.2-1B MLX 8-bit snapshot's own block: on the Metal home its window and 16 greedy steps ride the affine GEMV and embed stamps and emit the CPU rails' ids, two to twelve per-32 streams batch token for token the single-row step's, and its prefill window, uncrowned and with the kq_mulmm twins crowned, lands within 4.0 of the CPU rails' while the one-token-short control lands outside; each form loads once")]
def test_gpu_resident_llama_1b(t : T?) {
    return if (!family_on(t, "llama"))
    var r : Llama1bRows
    dense_feeds(r.feeds, [40l, TWO_WINDOWS])
    var forms <- llama_1b_forms(t, r)
    if (r.feeds.cuda) {
        skip_cell(t, "{FILE_1B}: bench_tg_batched_rep's refusal and served rep: {vulkan_home_why("the batched bench row is the Vulkan home's cell")}")
    } else {
        skip_cell(t, "{FILE_1B}: the N-row command's batched rows, the q8_0 mirror's feeds and the route-off load: {cuda_home_why()}")
    }
    tensor_carrier_held(t, FILE_1B)
    let ran = with_cpu_and_gpu_model(t, "llm", "Llama-3.2-1B-Instruct-Q8_0.gguf", REGIONS_RIG_CTX, forms, parity = true) $(var m, form) {
        llama_1b_rows(t, r, m, form)
    }
    if (ran) {
        dense_feeds_within(t, FILE_1B, r.feeds, LOGIT_BAR_REL)
        held_within(t, r.far_row, "{FILE_1B}, the far row: ", false)
        llama_codec_within(t, FILE_1B, "", r.codec)
    }
    llama_1b_kept_within(t, r.kept)
    llama_1b_mlx8_block(t, r.kept.twin)
    delete r
    delete forms
}

[test(description = "On the Metal home's blob twin, a das_metal build: Llama-3.2-1B Q8_0 loaded a second time beside the first continues 40 tokens token for token the CPU control's - the weights-epoch guard over the reload")]
def test_metal_llama_1b_reload(t : T?) {
    own_load("reloads the model: a second load beside the first, the weights-epoch guard")
    return if (!family_on(t, "llama"))
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return if (!arm_on(t, "arm9-reload"))
        let path = path_join(models_dir(), "Llama-3.2-1B-Instruct-Q8_0.gguf")
        return if (!jit_model_available(t, path))
        let prior_wscale = get_wscale_f16()
        let prior_backend = kernel_backend_seat()
        set_wscale_f16(false)
        pin_kernel_backend("portable")
        var tr <- load_model_(path, QuantMode.q8)
        tr.config.seq_len = min(tr.config.seq_len, DECODE_RELOAD_CTX)
        var trg <- blob_twin(t, path, DECODE_RELOAD_CTX)
        if (decode_reload_warmed(tr, trg)) {
            var trr <- load_model_(path, QuantMode.q8)
            trr.config.seq_len = min(trr.config.seq_len, DECODE_RELOAD_CTX)
            var trrg <- blob_twin(t, path, DECODE_RELOAD_CTX)
            decode_reload_within(t, trr, trrg)
            delete trr
            delete trrg
        }
        delete tr
        delete trg
        restore_kernel_backend(prior_backend)
        set_wscale_f16(prior_wscale)
    } else {
        feint("das_metal is not built on this platform; nothing to compare\n")
    }
}

[test(description = "The Llama-3.2-1B Q4_0 requant's q40 planes on the resident chain hold the CPU chain's logits within 6 percent over one and two windows plus eight steps, on the CUDA driver over one window with every position served; on the parity row's CPU form, on every box, the file's frozen greedy ids token for token; on the Vulkan home the kernel coverage census's coopmat mode sweep runs the q40 planes under the box's, KHR, sdot4, f16 and int8 modes; each form loads once")]
def test_gpu_resident_llama_1b_q40(t : T?) {
    return if (!family_on(t, "llama"))
    var c : DenseFeeds
    dense_feeds(c, gpu_want_cuda() ? [40l] : [40l, TWO_WINDOWS])
    var forms <- [device_form()]
    sweep_forms(t, forms, FILE_1B_Q40)
    let ran = with_cpu_and_gpu_model(t, "llm", "Llama-3.2-1B-Instruct-Q4_0-local.gguf", REGIONS_RIG_CTX, forms, parity = true) $(var m, form) {
        if (sweep_leg(form)) {
            sweep_cell(t, m, form, FILE_1B_Q40)
        } else {
            llama_feed_rows(c, m, form)
        }
    }
    if (ran) {
        dense_feeds_within(t, FILE_1B_Q40, c, LOGIT_BAR_REL)
    }
    delete c
    delete forms
}

//! what the Q5_1 file's Metal, per-op FFN and CPU forms keep for the
//! compares after its block
struct LlamaQ51Kept {
    census : CensusKept
    prefill : PrefillKept
    perop : PeropFfnKept
}

//! one form's cells of the Q5_1 file: on the Metal twin the format
//! census's q51 row, the per-32 carrier's batched streams and the
//! prefill cells (last: they shut the drivers down), their controls on
//! the portable CPU form; the per-op FFN rail; on the one-region form
//! the peel; else the forced feeds, on the Vulkan home with the q51
//! tags, and the per-op rail's CPU side on the CPU form
def private llama_q51_rows(t : T?; var c : DenseFeeds; var k : LlamaQ51Kept; var m : Model;
                           form : ModelForm) {
    if (form.on_metal) {
        format_census_metal(t, m, form, format_census_q51(), k.census)
        decode_per32_metal(t, m, form, decode_per32_q5_1())
        prefill_parity_metal(t, m, form, prefill_llama_1b_q51(), k.prefill)
        return
    }
    if (!form.on_gpu && !main_cpu_pass(form)) {
        format_census_cpu(t, m, form, format_census_q51(), k.census)
        prefill_parity_cpu(t, m, form, k.prefill)
        return
    }
    if (perop_ffn_form(form)) {
        perop_ffn_device_cell(t, m, form, perop_ffn_llama_1b_q51(), k.perop)
        return
    }
    if (form.on_gpu && form.regions == 1l) {
        let title = (c.cuda ? "{FILE_1B_Q51}: the same on the per-32 planes' N-column leaves"
            : "Llama-3.2-1B Q5_1: the same on the per-32 planes' N-column leaves")
        peel_on(t, m, FILE_1B_Q51, title)
        return
    }
    if (device_default(form) && !c.cuda) {
        unsafe {
            t |> run("{FILE_1B_Q51}: every dense plane keeps the q51 tag") @capture(& m)(t : T ?) {
                dense_planes_tagged(t, m, path_join(models_dir(), FILE_1B_Q51), KqFmt.q51)
            }
        }
    }
    llama_feed_rows(c, m, form)
    if (!form.on_gpu) {
        perop_ffn_cpu(t, m, form, perop_ffn_llama_1b_q51(), k.perop)
    }
}

[test(description = "The Llama-3.2-1B Q5_1 requant keeps the q51 tag on every dense plane on the Vulkan home, the resident chain holds the CPU chain's logits within 6 percent over one and two windows plus eight steps on both homes, and short tails peel through the per-32 planes' N-column leaves within the split bar of the row-at-a-time tail, the longer tail and lever-off arm taking the chain; on the Vulkan home with the whole-model route off the per-op dense FFN rail runs every layer's FFN row on the per-32 planes within 6 percent of the CPU chain, the rail off its control; on the Metal home, a das_metal build, every site loads native and the Metal window and six steps ride the per-32 stamps and GEMVs within 4.0 of the portable CPU chain, two to twelve per-32 streams batch token for token the single-row step's, and the prefill parity cells - the short window's kq tail and the scratch panels - hold the CPU controls; each form loads once")]
def test_gpu_resident_llama_1b_q51(t : T?) {
    return if (!family_on(t, "llama"))
    var c : DenseFeeds
    dense_feeds(c, [40l, TWO_WINDOWS])
    if (c.cuda) {
        skip_cell(t, "{FILE_1B_Q51}: every dense plane keeps the q51 tag: {vulkan_home_why("the plane tags are the Vulkan home's cell")}")
    }
    var forms <- [device_form(), device_form(regions = 1l)]
    perop_ffn_forms(t, FILE_1B_Q51, forms)
    forms |> push(metal_form(ctx = PREFILL_LLAMA_1B_Q51_CTX))
    metal_cpu_forms(forms, [prefill_cpu_form()])
    var k : LlamaQ51Kept
    let ran = with_cpu_and_gpu_model(t, "llm", "Llama-3.2-1B-Instruct-Q5_1-local.gguf", REGIONS_RIG_CTX, forms) $(var m, form) {
        llama_q51_rows(t, c, k, m, form)
    }
    if (ran) {
        dense_feeds_within(t, FILE_1B_Q51, c, LOGIT_BAR_REL)
    }
    format_census_verdict(t, format_census_q51(), k.census)
    prefill_parity_verdict(t, k.prefill)
    perop_ffn_verdict(t, perop_ffn_llama_1b_q51(), k.perop)
    delete c
    delete forms
}

//! what the Q4_K_M file's Metal, per-op FFN and CPU forms keep for
//! the compares after its block
struct LlamaKKept {
    matrix : MatrixLlamaKqKept
    prefill : PrefillKept
    decode : DecodeKept
    perop : PeropFfnKept
}

//! what the Q4_K_M file's forms collect: the CUDA home's feeds and the codec cells
struct LlamaKRows {
    feeds : DenseFeeds
    codec : LlamaCodec
    kept : LlamaKKept
}

//! the Q4_K_M file's Metal twin's cells - the support matrix's K-quant
//! row, then the decode legs and the prefill cells, which shut the
//! drivers down - and their controls on the portable CPU form
def private llama_k_metal_rows(t : T?; var k : LlamaKKept; var m : Model; form : ModelForm) {
    if (form.on_metal) {
        matrix_llama_1b_kq_metal(t, m, form, k.matrix)
        decode_parity_llama_1b_q4km_metal(t, m, form, k.decode)
        prefill_parity_metal(t, m, form, prefill_llama_1b_q4km(), k.prefill)
    } elif (alone_pass_of(form, prefill_cpu_form())) {
        matrix_llama_1b_kq_cpu(t, m, form, k.matrix)
        decode_parity_cpu(t, m, form, k.decode)
        prefill_parity_cpu(t, m, form, k.prefill)
    }
}

//! the compares after the Q4_K_M file's block of what its Metal,
//! per-op FFN and CPU forms kept
def private llama_k_kept_within(t : T?; var k : LlamaKKept) {
    matrix_llama_1b_kq_verdict(t, k.matrix)
    decode_parity_verdict(t, k.decode)
    prefill_parity_verdict(t, k.prefill)
    perop_ffn_verdict(t, perop_ffn_llama_1b_q4km(), k.perop)
}

//! the regions rig's groups cell on a load of `rows` regions - its GEMVs in column tiles with the CUDA home's rows leaf off,
//! every row within the split bar of its session alone - and at twelve the leaf cell on the CUDA home's rows tensor leaf, a
//! held skip on a home with none
def private regions_groups_cells(t : T?; var m : Model; rows : int64) {
    unsafe {
        t |> run("{rows} sessions, a region each: a batched step over all of them runs in groups of the command's rows, its GEMVs in column tiles, and every row reads what its session reads alone") @capture(& m)(t : T ?) {
            held_on(t) $(var h) {
                regions_groups_rows(h, m, rows, LogitsBar.split_bar)
            }
        }
        return if (rows != 12l)
        t |> run("{rows} sessions, a region each, on the rows tensor leaf: a leaf stamp launches and every row reads nearer its session alone than its previous step") @capture(& m)(t : T ?) {
            held_on(t) $(var h) {
                regions_leaf_rows(h, m, rows)
            }
        }
    }
}

//! the Q4_K_M file's forms: one region (the peel), two, twelve and twenty regions (the rig's batched steps), the segment form, on the CUDA home
//! the default (the feeds), and the q8_0 and tq4 mirrors (a codec the environment keeps off held as a skip)
def private llama_k_forms(t : T?; var r : LlamaKRows) : array<ModelForm> {
    var forms <- [device_form(regions = 1l), device_form(regions = 2l), device_form(regions = 12l), device_form(regions = 20l),
        segment_form()]
    if (r.feeds.cuda) {
        forms |> push(device_form())
    }
    r.codec.prompts <- [40l]
    codec_forms(forms, r.codec.h, [KVDtype.q8_0, KVDtype.tq4], device_form()) $(kv) => "{FILE_1B_K}: the {kv} mirror's cells"
    perop_ffn_forms(t, FILE_1B_K, forms)
    forms |> push(metal_form(ctx = PREFILL_LLAMA_1B_Q4KM_CTX))
    metal_cpu_forms(forms, [prefill_cpu_form()])
    return <- forms
}

//! one form's collection of the Q4_K_M file's cells into `r`; the regions forms' own cells run on their load
def private llama_k_rows(t : T?; var r : LlamaKRows; var m : Model; form : ModelForm) {
    let cuda = r.feeds.cuda
    if (form.on_metal || (!form.on_gpu && !main_cpu_pass(form))) {
        llama_k_metal_rows(t, r.kept, m, form)
    } elif (perop_ffn_form(form)) {
        perop_ffn_device_cell(t, m, form, perop_ffn_llama_1b_q4km(), r.kept.perop)
    } elif (form.pf_window_rows == SEG_WINDOW) {
        segments_on(t, m, "Llama-3.2-1B Q4_K_M: two sessions' tails as one segmented window, within and across windows, hold the split bar of their solo windows; ownership binds owners first and the decline arms fall back unsegmented")
    } elif (form.on_gpu && form.regions == 1l) {
        peel_on(t, m, FILE_1B_K, (cuda ? "{FILE_1B_K}: the same on the K-quant planes' N-column leaves and the rows' split Q8_K step"
            : "Llama-3.2-1B Q4_K_M: the same on the K-quant planes' N-column leaves"))
    } elif (form.on_gpu && form.regions == 2l) {
        regions_steps_cells(t, m, LogitsBar.split_bar)   //! the N-row command's Q8_K sites round apart from the one-row command's (`../followup_vulkan.md` item 75)
        if (cuda) {
            unsafe {
                t |> run("{FILE_1B_K}: the same on the K-quant planes (the kq N-column leaves, the Q8_K feeds)") @capture(& m)(t : T ?) {
                    var inscope none : array<string>
                    with_rows_leaf(false) {
                        batched_rows_cell(t, m, FILE_1B_K, 2l, none, none, none)
                    }
                }
            }
        }
    } elif (form.on_gpu && form.regions > 2l) {
        regions_groups_cells(t, m, form.regions)
    } elif (form.kv_asked) {
        llama_codec_rows(t, r.codec, m, form)
    } else {
        llama_feed_rows(r.feeds, m, form)
        if (!form.on_gpu) {
            llama_codec_rows(t, r.codec, m, form)
            perop_ffn_cpu(t, m, form, perop_ffn_llama_1b_q4km(), r.kept.perop)
        }
    }
}

[test(description = "On Llama-3.2-1B Q4_K_M short tails peel through the K-quant planes' N-column leaves within the split bar of the row-at-a-time tail, the longer tail and lever-off arm taking the chain; a q8_0 K/V mirror holds the CPU chain on q8_0 sessions within 10 percent and over a tq4 mirror the device sits within 1.15x the CPU tq4 chain's distance from the f16 chain and within 1.4x of it from the CPU tq4 chain; the N-row command's K-quant GEMV leaves and split Q8_K sites batch two sessions within the split bar of each alone at one window and past the unsplit position, twelve in eight- and four-column tiles and on the CUDA home's rows tensor leaf, and twenty as two commands of ten; on the segment form two sessions' tails as one segmented window, within and across windows, hold the split bar of their solo windows, ownership binds owners first and the decline arms fall back unsegmented; on the CUDA driver the forced feed holds 6 percent over one and two windows and two sessions batch through the N-row command within the split bar; on the parity row's CPU form, on every box, the file's frozen greedy ids token for token; on the Vulkan home with the whole-model route off the per-op dense FFN rail on the Q8_K feed runs every layer's FFN row on the device within 6 percent of the CPU chain, the rail off its control; on the Metal home, a das_metal build, the support matrix's K-quant row emits the CPU tokens exactly, the kq batched decode streams, the half-x leg and the 40-token K-quant continuation match the portable CPU controls, and the prefill parity cells hold them; each form loads once")]
def test_gpu_resident_llama_1b_k(t : T?) {
    return if (!family_on(t, "llama"))
    var r : LlamaKRows
    if (gpu_want_cuda()) {
        dense_feeds(r.feeds, [40l, TWO_WINDOWS])
    }
    var forms <- llama_k_forms(t, r)
    if (!r.feeds.cuda) {
        skip_cell(t, "{FILE_1B_K}: the forced feed and the N-row command's two rows on the CUDA driver: {cuda_home_why()}")
    }
    let ran = with_cpu_and_gpu_model(t, "llm", "Llama-3.2-1B-Instruct-Q4_K_M.gguf", REGIONS_RIG_CTX, forms, parity = true) $(var m, form) {
        llama_k_rows(t, r, m, form)
    }
    if (ran) {
        dense_feeds_within(t, FILE_1B_K, r.feeds, LOGIT_BAR_REL)
        let tq4_title = (r.feeds.cuda ? "{FILE_1B_K}: a tq4 mirror, the distance form over 40 tokens and eight steps on the CUDA driver"
            : "Llama-3.2-1B Q4_K_M: the tq4 K/V mirror - resident prefill + decode against the CPU chain's tq4 and f16 rows")
        llama_codec_within(t, FILE_1B_K, tq4_title, r.codec)
    }
    llama_k_kept_within(t, r.kept)
    delete r
    delete forms
}

[test(description = "On the CUDA driver a Q5_0 Llama-3.2-1B keeps one-window prefill and eight fed-step logits within 6 percent of max logit of the CPU chain, every position served; each form loads once")]
def test_gpu_resident_llama_1b_q50(t : T?) {
    return if (!family_on(t, "llama") || !cuda_home_asked(t))
    var c : DenseFeeds
    dense_feeds(c, [40l])
    dense_feeds_block(t, "Llama-3.2-1B-Instruct-Q5_0-local.gguf", REGIONS_RIG_CTX, c, LOGIT_BAR_REL) $(m, form) {
        llama_feed_rows(c, m, form)
    }
    delete c
}

[test(description = "On Llama-3.2-1B IQ4_XS the whole-model driver's prefill and decode over a q8_0 K/V mirror keep forced-feed logits within 10 percent of max logit of the CPU chain on q8_0 sessions and over a tq4 mirror the device sits within 1.15x the CPU tq4 chain's distance from the f16 chain and within 1.4x of it from the CPU tq4 chain, on the Vulkan home; on the CUDA driver, decoding its codebook through the home's permute, one-window prefill and eight fed-step logits within 6 percent of the CPU chain; on the parity row's CPU form, on every box, the file's frozen greedy ids token for token; on the Vulkan home the kernel coverage census's coopmat mode sweep runs the file under the box's, KHR and sdot4 modes; on the Metal home, a das_metal build, the prefill parity cells - the main arms, the short window's kq tail and the scratch panels - hold the portable CPU controls; each form loads once")]
def test_gpu_resident_llama_1b_iq4xs(t : T?) {
    return if (!family_on(t, "llama"))
    let file = "Llama-3.2-1B-Instruct-IQ4_XS.gguf"
    var c : DenseFeeds
    var codec : LlamaCodec
    var forms : array<ModelForm>
    if (gpu_want_cuda()) {
        dense_feeds(c, [40l])
        forms |> push(device_form())
        skip_cell(t, "{file}: the q8_0 and tq4 mirrors' cells: {vulkan_home_why("the IQ4_XS codec cells are the Vulkan home's")}")
    } else {
        codec.prompts <- [40l]
        codec_forms(forms, codec.h, [KVDtype.q8_0, KVDtype.tq4], device_form()) $(kv) => "{file}: the {kv} mirror's cells"
    }
    sweep_forms(t, forms, file)
    forms |> push(metal_form(ctx = PREFILL_LLAMA_1B_IQ4XS_CTX))
    metal_cpu_forms(forms, [prefill_cpu_form()])
    var prefill : PrefillKept
    let ran = with_cpu_and_gpu_model(t, "llm", "Llama-3.2-1B-Instruct-IQ4_XS.gguf", REGIONS_RIG_CTX, forms, parity = true) $(var m, form) {
        if (form.on_metal) {
            prefill_parity_metal(t, m, form, prefill_llama_1b_iq4xs(), prefill)
            return
        }
        if (!form.on_gpu && !main_cpu_pass(form)) {
            prefill_parity_cpu(t, m, form, prefill)
            return
        }
        if (sweep_leg(form)) {
            sweep_cell(t, m, form, file)
            return
        }
        if (form.kv_asked || !form.on_gpu) {
            llama_codec_rows(t, codec, m, form)
        }
        if (!form.kv_asked) {
            llama_feed_rows(c, m, form)
        }
    }
    prefill_parity_verdict(t, prefill)
    if (ran) {
        dense_feeds_within(t, file, c, LOGIT_BAR_REL)
        llama_codec_within(t, file, "Llama-3.2-1B IQ4_XS: the tq4 K/V mirror - resident prefill + decode against the CPU chain's tq4 and f16 rows", codec)
    } else {
        held_within(t, codec.h, "{file}: ", false)
    }
    delete c
    delete codec
    delete forms
}

[test(description = "On the CUDA driver an IQ4_NL Llama-3.2-1B on its superblock planes keeps one-window prefill and eight fed-step logits within 6 percent of max logit of the CPU chain, every position served; on the parity row's CPU form, on every box, the file's frozen greedy ids token for token; on the Vulkan home the kernel coverage census's coopmat mode sweep runs the file under the box's, KHR and sdot4 modes; each form loads once")]
def test_gpu_resident_llama_1b_iq4nl(t : T?) {
    return if (!family_on(t, "llama"))
    var c : DenseFeeds
    var forms : array<ModelForm>
    let why = cuda_home_why()
    if (empty(why)) {
        dense_feeds(c, [40l])
        forms |> push(device_form())
    } else {
        skip_cell(t, "Llama-3.2-1B-Instruct-IQ4_NL-local.gguf: the CUDA forced feed: {why}")
    }
    sweep_forms(t, forms, FILE_1B_IQ4NL)
    let ran = with_cpu_and_gpu_model(t, "llm", "Llama-3.2-1B-Instruct-IQ4_NL-local.gguf", REGIONS_RIG_CTX, forms, empty(why), parity = true) $(var m, form) {
        if (sweep_leg(form)) {
            sweep_cell(t, m, form, FILE_1B_IQ4NL)
            return
        }
        llama_feed_rows(c, m, form)
    }
    if (ran) {
        dense_feeds_within(t, FILE_1B_IQ4NL, c, LOGIT_BAR_REL)
    }
    delete c
    delete forms
}

//! the NVFP4 file's compares after the block: each FP4-leaf feed by the same-or-better witness against the f32 chain (the
//! window chain served the prompt, the CUDA driver every position), and the MMQ-off feed at the dense bar, the FP4 arm's control
def private nv4_within(t : T?; r : DenseFeeds) {
    return if (!r.main.ran)
    held_within(t, r.carrier, "{FILE_1B_NV4}: ", false)
    with_dasllama_jobque_() {
        for (f, i in r.gpu, count()) {
            let what = "{FILE_1B_NV4} cuda prefill ({f.nprompt} tokens) and {DENSE_FED_STEPS} steps [{tile_arm_tag()}]"
            continue if (!twin_collected(t, i, length(r.cpu), "{what}: ") || !twin_collected(t, i, length(r.f32), "{what}, the f32 chain: "))
            t |> equal(f.prefills, 1l, "{what}: the resident window chain served the {f.nprompt}-token prompt")
            cuda_served_within(t, f.cuda, f.nprompt + long_length(f.toks), "{what}: the resident feed")
            f32_chain_witness(t, what, f.rows, r.cpu[i].rows, r.f32[i].rows, FP4_F32_DIST_REL) $(id) {
                return feed_piece(f, r.cpu[i], id)
            }
        }
        mmq_off_within(t, r.mmq, r.cpu, "{FILE_1B_NV4} pinned CUDA_MMQ_OFF: ",
            "{FILE_1B_NV4} cuda, pinned CUDA_MMQ_OFF - the cm2 tiles over f16 activations (the FP4 arm's control): ", LOGIT_BAR_REL)
    }
}

[test(description = "On the CUDA driver an NVFP4 Llama-3.2-1B with windows on the W4A4 FP4 tensor leaf sits within 4x the CPU chain's distance from the f32 chain over one and two windows, and pinned off MMQ the cm2 tiles hold the dense 6 percent bar; on the parity row's CPU form, on every box, the file's frozen greedy ids token for token; on the Metal home, a das_metal build, every site loads native and the Metal window and six steps ride the nv4 stamps and GEMVs within 4.0 of the portable CPU chain, and the kernel coverage census's NVFP4 zoo row dispatches the nv4 GEMV, its small-batch forms and its dense mul_mm; each form loads once")]
def test_gpu_resident_llama_1b_nv4(t : T?) {
    return if (!family_on(t, "llama"))
    var r : DenseFeeds
    let why = cuda_home_why()
    var forms <- empty(why) ? [device_form(), device_form(mmq_off = true), f32_form()] : array<ModelForm>()
    if (empty(why)) {
        dense_feeds(r, [40l, TWO_WINDOWS])
    } else {
        skip_cell(t, "{FILE_1B_NV4}: the FP4 leaf's feeds and the MMQ-off control: {why}")
    }
    forms |> push(metal_form(ctx = KERNEL_COVERAGE_CTX))
    metal_cpu_forms(forms, [prefill_cpu_form()])
    var census : CensusKept
    let ran = with_cpu_and_gpu_model(t, "llm", "Llama-3.2-1B-Instruct-NVFP4-local.gguf", REGIONS_RIG_CTX, forms, parity = true) $(var m, form) {
        if (form.on_metal) {
            format_census_metal(t, m, form, format_census_nv4(), census)
            coverage_zoo_metal(t, m, form, cov_zoo_c4_llama_nvfp4())
        } elif (!form.on_gpu && !main_cpu_pass(form) && form.quant == QuantMode.q8) {
            format_census_cpu(t, m, form, format_census_nv4(), census)
        } else {
            llama_feed_rows(r, m, form)
        }
    }
    if (ran) {
        nv4_within(t, r)
    }
    format_census_verdict(t, format_census_nv4(), census)
    delete r
    delete forms
}

[test(description = "On the Metal home's blob twin, a das_metal build, on the Llama-3.2-1B NVFP4 mint with its nv4 token table every site loads native and the Metal window and six steps ride the nv4 stamps and GEMVs, the table gathered on the device, within 4.0 of the portable CPU chain fed the same ids; each form loads once")]
def test_gpu_resident_llama_1b_emb_nv4(t : T?) {
    return if (!family_on(t, "llama"))
    var forms <- [metal_form(ctx = CENSUS_CELLS_CTX)]
    metal_cpu_forms(forms, [prefill_cpu_form()])
    var census : CensusKept
    with_cpu_and_gpu_model(t, "llm", "Llama-3.2-1B-Instruct-emb-NVFP4-local.gguf", CENSUS_CELLS_CTX, forms) $(var m, form) {
        if (form.on_metal) {
            format_census_metal(t, m, form, format_census_nv4_table(), census)
        } else {
            format_census_cpu(t, m, form, format_census_nv4_table(), census)
        }
    }
    format_census_verdict(t, format_census_nv4_table(), census)
    delete forms
}

[test(description = "On Llama-3.2-3B Q8_0, head size 128 (on the CUDA home the h128 flash tile), the resident chain holds the CPU chain's logits within 6 percent over one and two windows plus eight steps, every call served on the device, and on the CUDA home a step with the far count pinned to the first key attends on the decode tile form at head 128 within the split bar of the row form; on the Metal home, a das_metal build, the hs=128 partD batched streams and the 40 tokens across depth 2048 equal the portable CPU controls, and the prefill parity cells - at contexts to 131072 - hold the CPU controls; each form loads once")]
def test_gpu_resident_llama_3b(t : T?) {
    return if (!family_on(t, "llama"))
    var c : DenseFeeds
    var inscope far : Held
    dense_feeds(c, [40l, TWO_WINDOWS])
    var forms <- [device_form(), metal_form(ctx = PREFILL_LLAMA_3B_CTX)]
    metal_cpu_forms(forms, [prefill_cpu_form()])
    var prefill : PrefillKept
    var decode : DecodeKept
    let ran = with_cpu_and_gpu_model(t, "llm", "Llama-3.2-3B-Instruct-Q8_0.gguf", REGIONS_RIG_CTX, forms) $(var m, form) {
        if (form.on_metal) {
            decode_parity_llama_3b_q8_metal(t, m, form, decode)
            prefill_parity_metal(t, m, form, prefill_llama_3b(), prefill)
        } elif (!form.on_gpu && !main_cpu_pass(form)) {
            decode_parity_cpu(t, m, form, decode)
            prefill_parity_cpu(t, m, form, prefill)
        } else {
            llama_feed_rows(c, m, form)
            if (c.cuda && form.on_gpu) {
                cuda_far_row_rows(far, m)
            }
        }
    }
    if (ran) {
        dense_feeds_within(t, "Llama-3.2-3B-Instruct-Q8_0.gguf", c, LOGIT_BAR_REL)
    }
    held_within(t, far, "Llama-3.2-3B Q8_0, the far row: ", false)
    decode_parity_verdict(t, decode)
    prefill_parity_verdict(t, prefill)
    delete c
    delete forms
}

//! the 8B's twin context: its cells' largest, the dim ladder's
let private LLAMA31_8B_CTX = PREFILL_LLAMA31_8B_CTX

[test(description = "On the Metal home's blob twin, a das_metal build, Meta-Llama-3.1-8B Q8_0 (the llama31-8b category, dim 4096): the support matrix's 8B matrix cell engages the Metal prefill, decode and batch paths and the q8 GEMM tier batches past B=4; a B=8 batched decode step's logits and K/V rows sit within tolerance of the CPU step's; the prefill parity cells hold the portable CPU chain, the 40-token dim-4096 continuation token for token; each form loads once")]
def test_metal_llama31_8b(t : T?) {
    return if (!family_on(t, "llama"))
    var forms <- [metal_form(ctx = LLAMA31_8B_CTX)]
    metal_cpu_forms(forms, [prefill_cpu_form()])
    var prefill : PrefillKept
    var decode : DecodeKept
    with_cpu_and_gpu_model(t, "llm", "Meta-Llama-3.1-8B-Instruct-Q8_0.gguf", LLAMA31_8B_CTX, forms) $(var m, form) {
        if (form.on_metal) {
            matrix_dim_metal(t, m, form, matrix_dim_8b())
            decode_parity_llama31_8b_q8_metal(t, m, form, decode)
            prefill_parity_metal(t, m, form, prefill_llama31_8b(), prefill)
        } else {
            decode_parity_cpu(t, m, form, decode)
            prefill_parity_cpu(t, m, form, prefill)
        }
    }
    decode_parity_verdict(t, decode)
    prefill_parity_verdict(t, prefill)
    delete forms
}

[test(description = "On the Metal home's blob twin, a das_metal build, Llama-3.3-70B Q4_K_M (the llama33-70b category, dim 8192): the support matrix's 70B K-quant matrix cell engages the Metal prefill, decode and batch paths and its batched dim ladder rides the kq mul_mm twin; each form loads once")]
def test_metal_llama33_70b(t : T?) {
    return if (!family_on(t, "llama"))
    var forms : array<ModelForm>
    if (empty(arm_why("dim70b"))) {
        forms |> push(metal_form(ctx = MATRIX_LLAMA_70B_CTX))
    } else {
        skip_cell(t, ("Llama-3.3-70B-Instruct-Q4_K_M.gguf: the dim ladder's 70B row: the arm " +
            "filter leaves dim70b out"))
    }
    with_cpu_and_gpu_model(t, "llm", "Llama-3.3-70B-Instruct-Q4_K_M.gguf", MATRIX_LLAMA_70B_CTX, forms, false) $(var m, form) {
        matrix_dim_metal(t, m, form, matrix_dim_70b())
    }
    delete forms
}

//! what the Q5_K_M file's Metal and CPU forms keep for the compares
//! after its block
struct LlamaK5Kept {
    tensor : MatrixTensorKept
    decode : DecodeKept
}

[test(description = "On the Metal home's blob twin, a das_metal build, Llama-3.2-1B Q5_K_M: the k5 batched decode streams equal the portable CPU control at B=3 and B=2, the kernel coverage census's q5_K zoo row dispatches the k5 GEMV, mul_mm and batch tiles, and with the tensor crowns forced a crowned tmm2d twin serves and the generate matches the CPU truth token for token; on the parity row's CPU form, on every box, the file's frozen greedy ids token for token; each form loads once")]
def test_gpu_resident_llama_1b_k5(t : T?) {
    return if (!family_on(t, "llama"))
    let file = "Llama-3.2-1B-Instruct-Q5_K_M.gguf"
    var forms <- [metal_form(ctx = KERNEL_COVERAGE_CTX)]
    metal_cpu_forms(forms, [prefill_cpu_form()])
    var k : LlamaK5Kept
    tensor_carrier_held(t, file)
    with_cpu_and_gpu_model(t, "llm", "Llama-3.2-1B-Instruct-Q5_K_M.gguf", KERNEL_COVERAGE_CTX, forms, parity = true) $(var m, form) {
        if (form.on_metal) {
            coverage_zoo_metal(t, m, form, cov_zoo_c3_llama_q5km())
            decode_parity_llama_1b_q5km_metal(t, m, form, k.decode)
            matrix_tensor_metal(t, m, form, matrix_tensor_k5(), k.tensor)
        } elif (alone_pass_of(form, prefill_cpu_form())) {
            decode_parity_cpu(t, m, form, k.decode)
            matrix_tensor_cpu(t, m, form, matrix_tensor_k5(), k.tensor)
        }
    }
    decode_parity_verdict(t, k.decode)
    matrix_tensor_verdict(t, matrix_tensor_k5(), k.tensor)
    delete forms
}

[test(description = "On the Metal home's blob twin, a das_metal build, the Llama-3.2-1B IQ2_XXS local requant (the iquant crown witness, a skip where it is not minted): with the tensor crowns forced a crowned tmm2d twin serves and the generate matches the portable CPU truth token for token; on the Vulkan home the coverage census's coopmat mode sweep runs the vehicle's prefill, steps and continuation under the box's, KHR and sdot4 modes; on the parity row's CPU form, on every box, the file's frozen greedy ids token for token; each form loads once")]
def test_gpu_resident_llama_1b_iq2xxs(t : T?) {
    return if (!family_on(t, "llama"))
    let file = "Llama-3.2-1B-Instruct-IQ2_XXS-local.gguf"
    var forms <- [metal_form(ctx = MATRIX_CTX)]
    metal_cpu_forms(forms, [prefill_cpu_form()])
    sweep_forms(t, forms, file)
    var tensor : MatrixTensorKept
    with_cpu_and_gpu_model(t, "llm", "Llama-3.2-1B-Instruct-IQ2_XXS-local.gguf", MATRIX_CTX, forms, false, parity = true) $(var m, form) {
        if (form.on_metal) {
            matrix_tensor_metal(t, m, form, matrix_tensor_iq2xxs(), tensor)
        } elif (alone_pass_of(form, prefill_cpu_form())) {
            matrix_tensor_cpu(t, m, form, matrix_tensor_iq2xxs(), tensor)
        } else {
            sweep_cell(t, m, form, file)
        }
    }
    matrix_tensor_verdict(t, matrix_tensor_iq2xxs(), tensor)
    delete forms
}

//! one sweep vehicle's block body (`sweep_cell`), its device forms the
//! sweep's legs alone
def private sweep_vehicle_forms(t : T?; file : string) : array<ModelForm> {
    var forms : array<ModelForm>
    sweep_forms(t, forms, file)
    return <- forms
}

[test(description = "On the Vulkan home the kernel coverage census's coopmat mode sweep runs Llama-3.2-1B Q3_K_L's k3 and k5 planes through a prefill, two greedy steps and a continuation on f32 and f16 sessions under the box's, KHR and sdot4 modes; on the parity row's CPU form, on every box, the file's frozen greedy ids token for token; each form loads once")]
def test_gpu_resident_llama_1b_q3kl(t : T?) {
    return if (!family_on(t, "llama"))
    let file = "Llama-3.2-1B-Instruct-Q3_K_L.gguf"
    var forms <- sweep_vehicle_forms(t, file)
    with_cpu_and_gpu_model(t, "llm", "Llama-3.2-1B-Instruct-Q3_K_L.gguf", VK_KERNEL_COVERAGE_CTX, forms, false, parity = true) $(var m, form) {
        sweep_cell(t, m, form, file)
    }
    delete forms
}

[test(description = "On the Vulkan home the kernel coverage census's coopmat mode sweep runs Llama-3.2-1B IQ3_M's iq3s planes through a prefill, two greedy steps and a continuation on f32 and f16 sessions under the box's, KHR and sdot4 modes; on the parity row's CPU form, on every box, the file's frozen greedy ids token for token; each form loads once")]
def test_gpu_resident_llama_1b_iq3m(t : T?) {
    return if (!family_on(t, "llama"))
    let file = "Llama-3.2-1B-Instruct-IQ3_M.gguf"
    var forms <- sweep_vehicle_forms(t, file)
    with_cpu_and_gpu_model(t, "llm", "Llama-3.2-1B-Instruct-IQ3_M.gguf", VK_KERNEL_COVERAGE_CTX, forms, false, parity = true) $(var m, form) {
        sweep_cell(t, m, form, file)
    }
    delete forms
}

[test(description = "On the Vulkan home the kernel coverage census's coopmat mode sweep runs the Llama-3.2-1B IQ3_XXS local requant's iq3xxs planes through a prefill, two greedy steps and a continuation on f32 and f16 sessions under the box's, KHR and sdot4 modes; on the parity row's CPU form, on every box, the file's frozen greedy ids token for token; each form loads once")]
def test_gpu_resident_llama_1b_iq3xxs(t : T?) {
    return if (!family_on(t, "llama"))
    let file = "Llama-3.2-1B-Instruct-IQ3_XXS-local.gguf"
    var forms <- sweep_vehicle_forms(t, file)
    with_cpu_and_gpu_model(t, "llm", "Llama-3.2-1B-Instruct-IQ3_XXS-local.gguf", VK_KERNEL_COVERAGE_CTX, forms, false, parity = true) $(var m, form) {
        sweep_cell(t, m, form, file)
    }
    delete forms
}

[test(description = "On the Vulkan home the kernel coverage census's coopmat mode sweep runs the Llama-3.2-1B Q2_K local requant's q2_K planes through a prefill, two greedy steps and a continuation on f32 and f16 sessions under the box's, KHR and sdot4 modes; on the parity row's CPU form, on every box, the file's frozen greedy ids token for token; each form loads once")]
def test_gpu_resident_llama_1b_q2k(t : T?) {
    return if (!family_on(t, "llama"))
    let file = "Llama-3.2-1B-Instruct-Q2_K-local.gguf"
    var forms <- sweep_vehicle_forms(t, file)
    with_cpu_and_gpu_model(t, "llm", "Llama-3.2-1B-Instruct-Q2_K-local.gguf", VK_KERNEL_COVERAGE_CTX, forms, false, parity = true) $(var m, form) {
        sweep_cell(t, m, form, file)
    }
    delete forms
}

[test(description = "On the Vulkan home the kernel coverage census's coopmat mode sweep runs the Llama-3.2-1B IQ2_XS local requant's iq2xs planes through a prefill, two greedy steps and a continuation on f32 and f16 sessions under the box's, KHR and sdot4 modes; on the parity row's CPU form, on every box, the file's frozen greedy ids token for token; each form loads once")]
def test_gpu_resident_llama_1b_iq2xs(t : T?) {
    return if (!family_on(t, "llama"))
    let file = "Llama-3.2-1B-Instruct-IQ2_XS-local.gguf"
    var forms <- sweep_vehicle_forms(t, file)
    with_cpu_and_gpu_model(t, "llm", "Llama-3.2-1B-Instruct-IQ2_XS-local.gguf", VK_KERNEL_COVERAGE_CTX, forms, false, parity = true) $(var m, form) {
        sweep_cell(t, m, form, file)
    }
    delete forms
}

[test(description = "On the Vulkan home the kernel coverage census's coopmat mode sweep runs the Llama-3.2-1B i1-IQ3_XXS file's iq3xxs planes and iq2s attention planes through a prefill, two greedy steps and a continuation on f32 and f16 sessions under the box's, KHR and sdot4 modes; on the parity row's CPU form, on every box, the file's frozen greedy ids token for token; each form loads once")]
def test_gpu_resident_llama_1b_i1_iq3xxs(t : T?) {
    return if (!family_on(t, "llama"))
    let file = "Llama-3.2-1B-Instruct.i1-IQ3_XXS.gguf"
    var forms <- sweep_vehicle_forms(t, file)
    with_cpu_and_gpu_model(t, "llm", "Llama-3.2-1B-Instruct.i1-IQ3_XXS.gguf", VK_KERNEL_COVERAGE_CTX, forms, false, parity = true) $(var m, form) {
        sweep_cell(t, m, form, file)
    }
    delete forms
}

[test(description = "On Llama-3.1-8B Q4_K_M the resident chain's mixed Q8_0 and K-quant planes hold the CPU chain's logits within 6 percent over one and two windows plus eight steps, on the Vulkan home; each form loads once")]
def test_gpu_resident_llama_8b_k(t : T?) {
    return if (!family_on(t, "llama") || !vulkan_home_asked(t, "the 8B's forced feed is the Vulkan home's cell"))
    var c : DenseFeeds
    dense_feeds(c, [40l, TWO_WINDOWS])
    dense_feeds_block(t, "Meta-Llama-3.1-8B-Instruct-Q4_K_M.gguf", REGIONS_RIG_CTX, c, LOGIT_BAR_REL) $(m, form) {
        llama_feed_rows(c, m, form)
    }
    delete c
}

let private SMOLLM2_CTX = 512l   //! the SmolLM2 forms' sessions' context: the chat cells' own; every other cell cuts its own
let private SCHEDULER_CTX = 128l   //! the scheduler cells' context

//! what SmolLM2's forms collect: each load's tied embedding rows (`tied_cls_rows`), the fp32 form's and the q8 form's
struct SmolRows {
    f32 : TiedClsRows
    f32_ran : bool
    q8 : TiedClsRows
    q8_ran : bool
}

//! the scheduler's cells on SmolLM2's q8 form, its context cut to SCHEDULER_CTX for them
def private smollm2_scheduler_cells(t : T?; var m : Model) {
    with_ctx_cut(m, SCHEDULER_CTX) {
        test_scheduler_matches_generate(t, m)
        test_scheduler_stop_strings(t, m)
        test_scheduler_think_budget(t, m)
        test_scheduler_chunk_sizes(t, m)
        test_scheduler_idle_quantum(t, m)
        test_scheduler_finishing_tick(t, m)
        test_scheduler_decode_events_precede_chunk(t, m)
        test_scheduler_admission_beside_prefill(t, m)
        test_eval_segments_one_token_chunk(t, m)
        test_scheduler_batching(t, m)
        test_scheduler_prefix_cache(t, m)
        test_scheduler_paged_sampled(t, m)
        test_scheduler_media_splice(t, m)
        test_scheduler_device_mode_switch(t, m)
        test_scheduler_media_validation(t, m)
        test_scheduler_mrope_validation(t, m)
        test_scheduler_span_cut(t, m)
        test_scheduler_span_validation(t, m)
    }
    scheduler_evict_and_context_length_cell(t, m)
}

//! one form's cells of SmolLM2, each form's on its pass that stands alone: the fp32 form's tied rows; the uncached form's chat
//! cells; the portable-pinned form's fused attention chain, deep-context witnesses and q8_0 K/V decode; on the q8 form the tied
//! rows, the scheduler's, batched decode's, the K/V cache's and fused decode's cells, the render seam, and the mrope cell last
//! (it stamps and restores the model's rope config)
def private smollm2_rows(t : T?; var r : SmolRows; var m : Model; form : ModelForm) {
    return if (!form.alone)
    if (form.backend == "portable") {
        fused_attn_chain_cell(t, m, "fused attention chain bit-exact on SmolLM2-135M, the portable pin (needs the gguf)")
        fused_flash_deep_ctx_cell(t, m)
        fused_flash_deep_ctx_q8_cell(t, m)
        kv_q8_decode_cell(t, m)
        return
    }
    if (form.quant == QuantMode.fp32) {
        r.f32 <- tied_cls_rows(m)
        r.f32_ran = true
        return
    }
    if (form.uncached) {
        chat_smollm2_cells(t, m)
        return
    }
    r.q8 <- tied_cls_rows(m)
    r.q8_ran = true
    smollm2_scheduler_cells(t, m)
    batch_decode_codec_cells(t, m)
    batch_decode_row_refuses_cell(t, m)
    kv_q8_prefill_vs_decode_cell(t, m)
    kv_paged_q8_cell(t, m)
    if (kernel_backend_has_rows()) {
        fused_attn_chain_cell(t, m, "fused attention chain bit-exact on SmolLM2-135M, the auto backend's rows core (needs the gguf)")
    }
    chat_smollm2_render_seam(t, m)
    scheduler_mrope_cell(t, m)
}

[test(description = "SmolLM2-135M on its CPU forms, on every box: on the q8 form the scheduler's cells capped at 128 positions (generate parity, stop ids and strings, the reasoning budget, chunking and the tick order, admission, eval_segments, batching, the prefix cache, paged sampling, media splices and spans, the session-kind switch and submit's refusals), eviction mid-run and the context-length finish, the q8_0 and tq4 batched-decode codec cells and the bench row's per-row refusal, a q8_0 cache's classic prefill bit-identical to per-token decode, paged q8_0 sessions bit-identical to flat ones, fused decode bit-exact on the auto backend's rows core, the render seam, and the media_grid mrope walk; on the portable-pinned form fused decode bit-exact on the portable pin, past 256 positions the flash-sliced attention passing its in-chain witness within 1e-4 at every sliced layer under f32 and q8_0 K/V with teacher-forced logits within 4 of the per-op path, and teacher-forced logits over q8_0 and q8_0-K/f16-V caches within 4.0 of the f16 cache's with fused and standard decode under q8_0 bit for bit; on the q8 uncached form the consumed pending text cleared and the spans a marker-less template refuses; the tied q8 load's embedding rows bit-identical to the fp32 form's table; each form loads once")]
def test_gpu_resident_smollm2(t : T?) {
    return if (!family_on(t, "llama"))
    var r : SmolRows
    with_cpu_and_gpu_model(t, "llm", "SmolLM2-135M-Instruct-Q8_0.gguf", SMOLLM2_CTX,
            [cpu_cells_form(), cpu_cells_form(uncached = true), cpu_cells_form(QuantMode.fp32),
            cpu_cells_form(backend = "portable")]) $(m, form) {
        smollm2_rows(t, r, m, form)
    }
    if (r.f32_ran && r.q8_ran) {
        tied_cls_q8_rows_cell(t, r.f32, r.q8)
    } elif (r.f32_ran || r.q8_ran) {
        t |> failure("SmolLM2-135M: the tied rows' compare lost a half (fp32 collected {r.f32_ran}, q8 collected {r.q8_ran})")
    }
    delete r
}

let private TINYLLAMA_CTX = 2048l   //! tinyllama's own context: its cells read at it, the past-the-context cell cutting it

[test(description = "tinyllama Q8_0 on its q8 uncached CPU form, on every box: a text running past the context embeds to a finite unit vector at cosine above 0.9999 to its leading seq_len tokens alone, the chat model file naming no pooling defaults to mean pooling, the chat engine's first-turn prefill equals the hand-built Zephyr sequence, and under a TokenConstraint the sampler emits only admitted tokens, walks the whole row, flags a dead end and a scheduler stream finishes constraint; on the Vulkan home the kernel coverage census runs the resident dense ladder and the per-op rails' batched steps at 2, 4 and 8 sessions, and under a tq4 mirror a prefill and two steps on tq4's four-head slab; each form loads once")]
def test_gpu_resident_tinyllama(t : T?) {
    return if (!family_on(t, "llama"))
    var forms <- [cpu_cells_form(uncached = true)]
    if (empty(arm_why("coverage-vk"))) {
        forms_or_skips(t, forms, "tinyllama-1.1b-chat-v1.0.Q8_0.gguf",
            [vk_census_form(), device_form() |> asking(KVDtype.tq4)],
            vulkan_home_why("the kernel coverage census rows are the Vulkan home's cell"))
    }
    with_cpu_and_gpu_model(t, "llm", "tinyllama-1.1b-chat-v1.0.Q8_0.gguf", TINYLLAMA_CTX, forms, false) $(var m, form) {
        if (on_device_home(form) && form.kv_asked) {
            coverage_vk_codec(t, m, form, cov_vk_c43_codec_tinyllama())
            return
        }
        if (on_device_home(form)) {
            coverage_vk_model(t, m, form, cov_vk_c36_tinyllama())
            return
        }
        return if (!form.alone)
        embeddings_past_the_context_cell(t, m)
        embeddings_chat_model_default_mean_cell(t, m)
        chat_tinyllama_cells(t, m)
        constrained_sampler_cell(t, m)
    }
    delete forms
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
    own_load("reloads the model: a second load reads the first's tail bit for bit")
    return if (!family_on(t, "llama"))
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

//! the last cell of the file - keep it last: it takes the device down, and the next file's context brings its own up
[test(description = "With Llama-3.2-1B Q8_0 served on the CUDA driver and its Model deleted with no drop, the tier's shutdown frees the driver's device state too: no CUDA object or allocation is left live")]
def test_gpu_resident_cuda_shutdown_drops_the_model(t : T?) {
    own_load("shuts down the model: the tier's shutdown drops the installed model's device state")
    return if (!family_on(t, "llama"))
    t |> run("{FILE_1B}: load, tail, delete the model; the shutdown leaves nothing live") @(t : T?) {
        let path = cuda_model_path(t, FILE_1B)
        if (empty(path)) return
        let levers = resident_levers()
        cuda_feed_window()
        with_dasllama_jobque_() {
            var tail <- resident_feed(8l)
            let c0 = cuda_positions_served()
            var row <- tail_after_load(path, tail)
            cuda_served_since(t, c0, 1l, "the load's tail")
            t |> success(cuda_live_object_count() > 0l, "the served model's device state is live after its Model is deleted ({cuda_live_object_count()} objects)")
            cud_shutdown()
            let held = cuda_device_bytes_allocated()
            t |> equal(cuda_log_leaks_if_any(), 0l, "the shutdown dropped the installed model's device state with the device ({held} bytes of device memory still allocated)")
            delete row
            delete tail
        }
        restore_resident_levers(levers)
    }
}

// The phi3 family on the whole-model driver: Phi-3.5-mini (head 96 over 32 heads - on the CUDA home the window's attention on
// the h96 flash tile - the fused qkv plane, the fused gate-up plane, the long-rope factors). Each model file is ONE block
// (`with_cpu_and_gpu_model`) under the MMQ route pinned `CUDA_MMQ_WINNERS` on the CUDA home (`with_cuda_mmq_winners`): its
// forms load once each, the cells collect what the compares read and the compares follow the block.
// - Phi-3.5-mini Q4_K_M (`test_gpu_resident_phi35_mini`): on both homes, at two regions, the regions rig's batched-step cells
//   on a K-quant carrier - the N-row token command's K-quant GEMV leaves and its split Q8_K sites serve two rows against the
//   sessions alone at one window and past the unsplit position, at the split bar with the one-token-off control, not bit for
//   bit (`../followup_vulkan.md` item 75); on the CUDA home the CUDA llama carriers' forced feed (`dense_feeds`, the driver's
//   counters and their control) at the two-window prompt alone; on its uncached CPU form, on every box, the Phi-3 prefill
//   (`_chat_cells.das`); on its K-quant-native CPU form under the portable backend's pin, on every box, the fused qkv's
//   K-quant slices at their plane offsets and the LongRoPE long factors (`_kquant_cells.das`).
// - Phi-3.5-mini Q8_0 (`test_gpu_resident_cuda_phi35_mini_q8`, the q8 decode chain): the CUDA home's forced feed at one
//   window and two.
// Every test skips off the JIT, without DASLLAMA_GPU=1 on a device the driver serves, without its file under models_dir(), or
// where a form's plan does not fit the card; the feeds without DASLLAMA_CUDA=1, a two-window feed where the prompt window is
// not PF_WINDOW_ROWS rows.

let private FILE_35 = "Phi-3.5-mini-instruct-Q4_K_M.gguf"   //! head 96: the h96 flash tile, the K-quant planes

//! the K-quant file runs the two-window prompt alone: after a 40-token prompt its steps at 41 and 44 read 7.7 and 6.6% of the max logit off the CPU's (the Q8_0 file's within 0.73 of a 1.9 bar) while the one-step-off rows sit 6.5% apart, so no one bar holds both arms
[test(description = "On Phi-3.5-mini Q4_K_M with fused qkv and gate-up planes and long-rope factors, a batched step over two sessions holds the split bar of each alone, at one window and past the unsplit position, the one-token-off control past it; on the CUDA whole-model driver, head 96 on the h96 flash tile, two-window forced-feed logits within 6 percent of max logit of the CPU chain; on the uncached CPU form, on every box, the first-turn prefill equals the hand-built Phi-3 sequence of system, user, end and assistant special ids around the encoded text token for token; on the K-quant-native portable-pinned CPU form the fused attn_qkv splits into k5 q and k slices within 1e-3 relative L2 of the file dequant, the k slice at its dim x qd source offset, and the model, its context past the original 4096, carries the file's 48 long rope factors bit for bit, apart from the short set; on the Vulkan home the kernel coverage census runs the head-96 window's flash tiles; on the Metal home, a das_metal build, the support matrix's phi3 K-quant row derives the NEOX need, engages the three Metal paths on f16 and q8_0 K/V and its metal-overrides generate emits the portable CPU's ids, the single row past the ceiling on the narrow fused stamp within its bar, and the coverage census's phi3 zoo row dispatches its stamps; each form loads once")]
def test_gpu_resident_phi35_mini(t : T?) {
    return if (!family_on(t, "phi3"))
    var c : DenseFeeds
    var forms <- [device_form(regions = 2l), cpu_cells_form(uncached = true),
        cpu_cells_form(backend = "portable", kq_native = KqNative.on)]
    if (gpu_want_cuda()) {
        dense_feeds(c, [TWO_WINDOWS])
        forms |> push(device_form())
    } else {
        skip_cell(t, "{FILE_35}: the forced feed on the CUDA driver: {cuda_home_why()}")
        if (vk_census_asked()) {
            forms |> push(vk_census_form())
        }
    }
    forms |> push(metal_form(ctx = KERNEL_COVERAGE_CTX))
    metal_cpu_forms(forms, [prefill_cpu_form()])
    var family : MatrixFamilyKept
    var ran = false
    with_cuda_mmq_winners() {
        ran = with_cpu_and_gpu_model(t, "llm", "Phi-3.5-mini-instruct-Q4_K_M.gguf", REGIONS_RIG_CTX, forms, c.cuda) $(var m, form) {
            if (form.on_metal) {
                matrix_family_metal(t, m, form, matrix_row_phi35_k4(), family)
                coverage_zoo_metal(t, m, form, cov_zoo_c6_phi3())
            } elif (alone_pass_of(form, prefill_cpu_form())) {
                matrix_family_cpu(t, m, form, matrix_row_phi35_k4(), family)
            } elif (form.uncached) {
                chat_phi_cells(t, m)
            } elif (form.kq_native == KqNative.on) {
                kq_fused_qkv_slices_cell(t, m)
                kq_phi3_longrope_factors_cell(t, m)
            } elif (form.on_gpu && form.regions == 2l) {
                regions_steps_cells(t, m, LogitsBar.split_bar)
            } elif (vk_census_form_is(form)) {
                coverage_vk_model(t, m, form, cov_vk_c38_phi3())
            } else {
                dense_feeds_rows(c, m, form, "phi3")
            }
        }
    }
    if (ran) {
        dense_feeds_within(t, FILE_35, c, LOGIT_BAR_REL)
    }
    matrix_family_verdict(t, matrix_row_phi35_k4(), family)
    delete c
    delete forms
}

[test(description = "On the CUDA whole-model driver Phi-3.5-mini Q8_0 keeps prefill and eight fed-step logits within 6 percent of max logit of the CPU chain over one and two windows; on the Metal home, a das_metal build, the support matrix's phi3 Q8_0 row on the f16 weight scale engages the three Metal paths through the fused qkv_rs NEOX mapping and its metal-overrides generate emits the portable CPU's ids, the single row past the ceiling on the narrow fused stamp within its bar; each form loads once")]
def test_gpu_resident_cuda_phi35_mini_q8(t : T?) {
    return if (!family_on(t, "phi3"))
    let file = "Phi-3.5-mini-instruct-Q8_0.gguf"
    var c : DenseFeeds
    var forms : array<ModelForm>
    let why = cuda_home_why()
    if (empty(why)) {
        dense_feeds(c, [40l, TWO_WINDOWS])
        forms |> push(device_form())
    } else {
        skip_cell(t, "{file}: the forced feed on the CUDA driver: {why}")
    }
    forms |> push(metal_form(ctx = MATRIX_CTX))
    metal_cpu_forms(forms, [prefill_cpu_form()])
    var family : MatrixFamilyKept
    var ran = false
    with_cuda_mmq_winners() {
        ran = with_cpu_and_gpu_model(t, "llm", "Phi-3.5-mini-instruct-Q8_0.gguf", REGIONS_RIG_CTX, forms, c.cuda) $(var m, form) {
            if (form.on_metal) {
                matrix_family_metal(t, m, form, matrix_row_phi35_q8(), family)
            } elif (alone_pass_of(form, prefill_cpu_form())) {
                matrix_family_cpu(t, m, form, matrix_row_phi35_q8(), family)
            } else {
                dense_feeds_rows(c, m, form, "phi3")
            }
        }
    }
    if (ran) {
        dense_feeds_within(t, file, c, LOGIT_BAR_REL)
    }
    matrix_family_verdict(t, matrix_row_phi35_q8(), family)
    delete c
    delete forms
}
