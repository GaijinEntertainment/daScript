options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_kv_dtype   // KVDtype: the block-codec cells' mirror ask
require _gemma_resident
require dasllama/dasllama_kqformat   // KqFmt - the PLE table's tag
require dasllama/dasllama_gpu_resident   // resident_plan - the table's seat
require _format_census   // census_cell - the per-layer planes in their own formats
require daslib/fio

// gemma-4-E2B Q8_0 (the E-series carrier: per-layer embeddings, the upper layers sharing the K/V of the ones below, two
// dense FFN widths, a sliding window the two-window cell engages): arm `g4e`. Skips off the JIT, without DASLLAMA_GPU=1
// on a device the whole-model driver serves, without the file under models_dir(), or when the residency plan does not
// fit the card (DASLLAMA_GPU_MIN_CTX shortens the mirror).
let private GEMMA4E_FILE = GemmaFile(file = "gemma-4-E2B-it-Q8_0.gguf", arm = "g4e")

[test]
def test_gpu_resident_gemma4_e2b(t : T?) {
    t |> run("gemma-4-E2B Q8_0: resident prefill + decode - the per-layer-embedding branch, the shared-KV layers, two FFN widths - vs the CPU chain, forty tokens") @(t : T?) {
        gemma_forced_feed(t, 40l, GEMMA4E_FILE)
    }
    t |> run("gemma-4-E2B Q8_0: 150 prose ids prefilled, 150 teacher-forced, scored as a perplexity against the CPU chain") @(t : T?) {
        ppl_compare(t, 150l, 150l, GEMMA4E_FILE)
    }
    t |> run("gemma-4-E2B Q8_0: 520 ids prefilled - two windows, the sliding window engaged on the last eight - then 80 teacher-forced") @(t : T?) {
        assert_pf_window(t)
        ppl_compare(t, PF_WINDOW_ROWS + 8l, 80l, GEMMA4E_FILE)
    }
    t |> run("gemma-4-E2B Q8_0: eight tokens through the window chain against the token command, both arms") @(t : T?) {
        prefill_vs_decode(t, 8l, GEMMA4E_FILE)
    }
}

[test]
def test_gpu_resident_gemma4_e2b_block_codecs(t : T?) {
    t |> run("gemma-4-E2B Q8_0: a q8_0 K/V mirror - the shared-KV layers store no row of their own - vs the CPU chain, forty tokens") @(t : T?) {
        with_gpu_kv_dtype(t, KVDtype.q8_0) {
            gemma_forced_feed(t, 40l, GEMMA4E_FILE)
        }
    }
    t |> run("gemma-4-E2B Q8_0: a tq4 K/V mirror - a shared-KV layer rotates its query and stores nothing - scored as a perplexity against the CPU chain") @(t : T?) {
        with_gpu_kv_dtype(t, KVDtype.tq4) {
            ppl_compare(t, 150l, 150l, GEMMA4E_FILE)
        }
    }
}

//! the per-layer-embedding table in its own tag on the device (arm `g4e-ple`): E2B Q8_0 with the token and PLE tables
//! requantized to Q5_0, Q5_1 and Q4_0 (`../performance/model_specs.das`) and the Q8_0 file as the shape control - the plan
//! seats the table under its tag, and the forced feed's prefill gathers the side rows off it against the CPU chain
[test]
def test_gpu_resident_gemma4_e2b_ple_tables(t : T?) {
    for (spec in [(file = "gemma-4-E2B-it-Q8_0.gguf", fmt = KqFmt.q8),
                  (file = "gemma-4-E2B-it-emb-Q5_0-local.gguf", fmt = KqFmt.q5032),
                  (file = "gemma-4-E2B-it-emb-Q5_1-local.gguf", fmt = KqFmt.q51),
                  (file = "gemma-4-E2B-it-emb-Q4_0-local.gguf", fmt = KqFmt.q40)]) {
        let gf = GemmaFile(file = spec.file, arm = "g4e-ple")
        t |> run("{spec.file}: the {spec.fmt} per-layer-embedding table seats on the device and the prefill gathers off it") @(t : T?) {
            with_gemma_rig(t, gf) $(tr) {
                t |> equal(tr.ple_emb_fmt, spec.fmt, "the PLE table keeps its disk format's tag")
                let plan = resident_plan(tr, 4096l, KVDtype.f16, KVDtype.f16)
                t |> success(plan.ple_tbl_bytes > 0l, "the plan seats the {spec.fmt} table on the device ({plan.ple_tbl_bytes} bytes)")
            }
            gemma_forced_feed(t, 40l, gf)
        }
    }
}

//! the per-layer gate and projection planes in their own formats on the device (arm `g4e-kq`): the E2B Q4_K_M file ships them
//! F32 - the router GEMV, the f32 act and the f32 tile read the file's rows - and the Q4_0 file Q4_0, on the q40 GEMV and tiles
//! with the f32 classes undispatched (the control)
[test]
def test_gpu_resident_gemma4_e2b_ple_planes(t : T?) {
    for (spec in [(file = "gemma-4-E2B-it-Q4_K_M.gguf", f32 = true), (file = "gemma-4-E2B-it-Q4_0.gguf", f32 = false)]) {
        let gf = GemmaFile(file = spec.file, arm = "g4e-kq")
        t |> run("{spec.file}: the per-layer gate and projection planes in their file format, the forced feed against the CPU chain") @(t : T?) {
            var armed = false   //! the rig armed the driver on the file (else every cell below skipped, saying why)
            with_gemma_rig(t, gf) $(tr) {
                armed = true
                census_cell(t, tr, path_join(models_dir(), spec.file), "vulkan load", ["per-layer embeddings"])
                t |> equal(tr.ple_gate_f32 && tr.ple_proj_f32, spec.f32, "the planes ship F32 exactly where the file carries F32")
            }
            let act0 = census("ple_act_f32_cls_spv")
            let gemm0 = census("dn_ba_cls_spv")
            gemma_forced_feed(t, 40l, gf)
            return if (!armed || act0 == NO_CENSUS)
            let act_n = census("ple_act_f32_cls_spv") - act0
            let gemm_n = census("dn_ba_cls_spv") - gemm0
            if (spec.f32) {
                t |> success(act_n > 0l && gemm_n > 0l, "the token command's f32 act ({act_n}) and the window's f32 tile ({gemm_n}) served the f32 rows")
            } else {
                t |> equal(act_n + gemm_n, 0l, "no f32 per-layer plane, no f32 act or tile dispatched")
            }
        }
    }
}

//! a two-token prompt on the E-series takes the window chain: the token command's peel declines the per-layer-embedding side rows
[test]
def test_gpu_resident_gemma4_e2b_short_window(t : T?) {
    t |> run("gemma-4-E2B Q8_0: a two-token prompt takes the window chain, not the token command's peel") @(t : T?) {
        with_gemma_rig(t, GEMMA4E_FILE) $(tr) {
            assert_short_window_takes_chain(t, tr, "a per-layer-embedding model")
        }
    }
}
