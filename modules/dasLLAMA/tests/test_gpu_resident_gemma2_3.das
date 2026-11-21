options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_kv_dtype   // KVDtype: the block-codec cells' mirror ask
require _gemma_resident
require dasllama/dasllama_math   // with_dasllama_jobque_: the ring compares after the block read argmax off the queue
require _resident_regions   // the regions cells' collections, REGIONS_RIG_CTX
require _resident_feed   // Held, held_within: the regions cells' readings
require dasllama/dasllama_common   // Model

// gemma-2-2b Q8_0 (the attention logit softcap, a window on the even layers, no qk-norm): arm `g2`. Skips off the
// JIT, without DASLLAMA_GPU=1 on a device the whole-model driver serves, without the file under models_dir(), or
// when the residency plan does not fit the card (DASLLAMA_GPU_MIN_CTX shortens the mirror). The two-region form, at
// REGIONS_RIG_CTX, carries the rig's batched cells: the N-row token command serves two rows bit for bit against the sessions
// alone - at one window, past the unsplit position so the split attention form records, and a batched step before any one-row
// step - and, on the Vulkan home with a kernel census, the stamp cell.
let private GEMMA2_FILE = GemmaFile(file = "gemma-2-2b-it-Q8_0.gguf", arm = "g2")

[test(description = "On the whole-model driver gemma-2-2b Q8_0 with the attention softcap keeps forty-token forced-feed logits within 16 percent of max logit of the CPU chain, perplexity within 1.05x, and window chain against token command within 6x of the CPU chain's own two paths; over two mirror regions, with the attention softcap and the final logit softcap, batched steps over two sessions - at one window, past the unsplit position, and before any one-row step - read what each session reads alone, and its two q heads per kv head make a step dispatch the two-head slab decode attention stamp of the armed mirror codec; each form loads once")]
def test_gpu_resident_gemma2(t : T?) {
    return if (!family_on(t, "gemma2") || !arm_on(t, GEMMA2_FILE.arm))
    let gaps <- [8l]
    let walks <- [(n = 150l, steps = 150l)]
    var feeds <- [40l]
    if (gpu_want_cuda()) {   //! the CUDA home's own cell: a two-window prompt and eight steps
        feeds |> push(PF_WINDOW_ROWS + 88l)
    }
    var rows : GemmaRows[2]
    var regions : Held
    let forms <- [device_form(), device_form(regions = 2l, ctx = REGIONS_RIG_CTX)]
    with_cuda_mmq_winners() {
        let ran = with_cpu_and_gpu_model(t, "llm", "gemma-2-2b-it-Q8_0.gguf", GEMMA_RIG_CTX, forms) $(m, form) {
            if (form.on_gpu && form.regions == 2l) {
                if (gpu_want_cuda()) {
                    regions |> held_skip("the CUDA home is asked: the slab pick and the census names are the Vulkan driver's")
                } else {   //! first on the load: the census counts a token command's dispatches as it records, and a later step replays
                    regions_stamp_rows(regions, m, 2)
                }
                regions_batched_once_rows(regions, m)
            } else {
                rows[form.on_gpu ? 1 : 0] <- gemma_rows(m, form, feeds, gaps, walks)
            }
        }
        if (ran) {
            gemma_rows_within(t, "gemma-2-2b", rows[1], rows[0], gaps, walks)
            held_within(t, regions, "gemma-2-2b Q8_0, two regions: ", false)
        }
    }
    delete rows
    delete regions
    delete feeds
}

// gemma-3-1b Q8_0 (the dense base: sandwich norms, sliding layers beside the global ones with their own rope class,
// the GeGLU FFN, the sqrt(dim) embed scale): arm `g3`. Skips off the JIT, without DASLLAMA_GPU=1 on a device the
// whole-model driver serves, without the file under models_dir(), or when the residency plan does not fit the card; the
// block-codec forms also skip where the environment arms another mirror codec over the one they ask (DASLLAMA_VK_KV32=1). The
// two-region form, at REGIONS_RIG_CTX, carries the rig's batched cells and, on its ringed load, the session's ring floor through
// every rail that moves its rows (a continuation, a prompt from 0, a park, an export, an import).
let private GEMMA3_FILE = GemmaFile(file = "gemma-3-1b-it-Q8_0.gguf", arm = "g3")

let private PAST_SLIDING_WINDOW = 2l * PF_WINDOW_ROWS + 76l   //! a third prefill window: it starts a window's rows past the 512-key sliding window

[test(description = "On the whole-model driver gemma-3-1b Q8_0 with sandwich norms and sliding layers on the device keeps forced-feed logits within 16 percent of max logit of the CPU chain at prompts of 40, 8, 520 and 600 tokens, and its window chain and token command agree over 8 and 40 tokens within 6x of the CPU chain's own two paths; over a q8_0 K/V mirror it keeps the forced-feed logits within 16 percent past the sliding window, over a tq4 mirror the teacher-forced perplexity within 1.05x; its ringed K/V mirrors match a flat every-row mirror bit for bit over 1300 prose ids and eight steps on the default and the q8_0 mirror; over two mirror regions, with sandwich norms, sliding and global layers and GeGLU, batched steps over two sessions - at one window, past the unsplit position, and before any one-row step - read what each session reads alone, and on the ringed load the ring floor sits at the prompt's rows less the slack: a call under it panics, a prompt from 0 clears it, and a park, export or import under it refuses while every row moves with it; each form loads once")]
def test_gpu_resident_gemma3(t : T?) {
    return if (!family_on(t, "gemma3") || !arm_on(t, GEMMA3_FILE.arm))
    var r : G3Rows
    //! forty and eight tokens in one window; 520 and 600 across two, the sliding window engaged, where the prompt window is the cells' own
    r.feeds <- assert_pf_window(t) ? [40l, 8l, PF_WINDOW_ROWS + 8l, PF_WINDOW_ROWS + 88l] : [40l, 8l]
    let forms <- [device_form(), device_form(kv = KVDtype.q8_0), device_form(kv = KVDtype.tq4), device_form(ring = false),
        device_form(kv = KVDtype.q8_0, ring = false), device_form(regions = 2l, ctx = REGIONS_RIG_CTX)]
    with_cuda_mmq_winners() {
        let ran = with_cpu_and_gpu_model(t, "llm", "gemma-3-1b-it-Q8_0.gguf", GEMMA_RIG_CTX, forms) $(m, form) {
            g3_rows(r, m, form)
        }
        if (ran) {
            g3_rows_within(t, r)
        }
    }
    delete r
}

//! what the gemma-3-1b file's forms collect: the default device form's feeds and path gaps with their CPU side on its codec
//! (`main`), the q8_0 mirror's forced feed past the sliding window and the tq4 mirror's teacher-forced walk (each a CPU side,
//! a device side), the rings (the default codec ringed and flat, the q8_0 mirror ringed and flat), and the rig's cells on the
//! two-region form; each `_ran` says a form collected
struct G3Rows {
    feeds : array<int64>
    main : MainPair
    rows : GemmaRows[2]
    codec : CodecRows
    regions : Held
}

let private G3_GAPS = fixed_array(8l, 40l)
//! the q8_0 feed runs past the sliding window; a 4-bit mirror reads one step 5.0 of a 33.7 max logit from the CPU's, so no
//! logit bar holds its walk with room
let private G3_CODEC = CodecCells(q8_feed = PAST_SLIDING_WINDOW, tq4_walk = (n = PAST_SLIDING_WINDOW, steps = 80l),
    ring_kv = KVDtype.q8_0, q8_note = "the third window's f16 shadow starts past the sliding layers' window",
    tq4_note = "its teacher-forced walk")

//! one form's collection of the gemma-3-1b file's cells into `r`
def private g3_rows(var r : G3Rows; m : Model; form : ModelForm) {
    var inscope gaps <- [for (g in G3_GAPS); g]
    let side = main_side(r.main, form)
    if (form.on_gpu && form.regions == 2l) {
        regions_batched_once_rows(r.regions, m)
        regions_ring_floor_rows(r.regions, m)
        return
    }
    if (side == MainSide.device) {
        r.rows[1] <- gemma_rows(m, form, r.feeds, gaps, array<GemmaWalk>())
    } elif (side == MainSide.cpu) {
        r.rows[0] <- gemma_rows(m, form, r.feeds, gaps, array<GemmaWalk>())
    }
    codec_rows(r.codec, m, form, G3_CODEC)
}

//! the gemma-3-1b file's compares on what its forms collected, each cell where its forms ran
def private g3_rows_within(t : T?; r : G3Rows) {
    var inscope gaps <- [for (g in G3_GAPS); g]
    if (r.main.ran) {
        gemma_rows_within(t, "gemma-3-1b", r.rows[1], r.rows[0], gaps, array<GemmaWalk>())
    }
    codec_rows_within(t, "gemma-3-1b Q8_0", r.codec, G3_CODEC)
    held_within(t, r.regions, "gemma-3-1b Q8_0, two regions: ", false)
    with_dasllama_jobque_() {
        rings_within(t, "gemma-3-1b Q8_0", GEMMA_RING_IDS, r.codec.rings, r.main.kv,
            G3_CODEC.ring_kv)
    }
}

//! the driver's load-time lines, read off two loads in the test child: no in-process cell reads what to_log prints
[test(description = "The whole-model driver's load log on gemma-3-1b names its rings on a ringed load, its every-row mirror on a flat one, and the host embed only where the token gather is pinned off", timeout = 1200)]
def test_gpu_resident_gemma3_load_log(t : T?) {
    return if (!family_on(t, "gemma3"))
    t |> run("gemma-3-1b Q8_0: the resident driver's load log names the rings, the flat mirror and the host embed under their pins") @(t : T?) {
        ring_load_announces(t, GEMMA3_FILE)
    }
}

// gemma-3-4b Q8_0 (the dense base at its wider geometry): arm `g3b`. Skips off the JIT, without DASLLAMA_GPU=1 on
// a device the whole-model driver serves, without the file under models_dir(), or when the residency plan does not
// fit the card (DASLLAMA_GPU_MIN_CTX shortens the mirror).
let private GEMMA3_4B_FILE = GemmaFile(file = "gemma-3-4b-it-Q8_0.gguf", arm = "g3b")

[test(description = "On the whole-model driver gemma-3-4b Q8_0 keeps forty-token forced-feed logits within 16 percent of max logit of the CPU chain and its 150-position teacher-forced perplexity within 1.05x of the CPU chain's; each form loads once")]
def test_gpu_resident_gemma3_4b(t : T?) {
    return if (!family_on(t, "gemma3") || !arm_on(t, GEMMA3_4B_FILE.arm))
    let walks <- [(n = 150l, steps = 150l)]
    var rows : GemmaRows[2]
    let ran = with_cpu_and_gpu_model(t, "llm", "gemma-3-4b-it-Q8_0.gguf", GEMMA_RIG_CTX) $(m, form) {
        rows[form.on_gpu ? 1 : 0] <- gemma_rows(m, form, [40l], array<int64>(), walks)
    }
    if (ran) {
        gemma_rows_within(t, "gemma-3-4b", rows[1], rows[0], array<int64>(), walks)
    }
    delete rows
}
