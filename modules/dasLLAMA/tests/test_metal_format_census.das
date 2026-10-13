options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama   // nolint:STYLE030,LINT019 - the public facade, reached inside the das_metal halves
require dasllama/dasllama_math   // nolint:STYLE030,LINT019 - set_wscale_f16, inside the das_metal halves
require dasllama/dasllama_kqformat   // nolint:STYLE030,LINT019 - KqFmt, inside the das_metal halves
require _metal_blob_twin   // nolint:STYLE030,LINT019 - blob_twin and the kernel census, inside the das_metal halves
require ?das_metal dasllama/dasllama_metal_common   // nolint:STYLE030,LINT019 - the kernel coverage census, inside the das_metal halves
require ?das_metal dasllama/dasllama_metal_decode   // nolint:STYLE030,LINT019 - metal_decode_stats and shutdown, inside the das_metal halves
require ?das_metal dasllama/dasllama_metal_prefill   // nolint:STYLE030,LINT019 - metal_prefill_stats and shutdown, inside the das_metal halves
require ?das_metal metal/das_metal_boost   // nolint:STYLE030,LINT019 - metal_live_object_count for the leak gate, inside the das_metal halves
require daslib/jobque_boost
require daslib/fio   // nolint:STYLE030,LINT019 - path_join and fmap, inside the das_metal halves
require math
require _model_tier
require _format_census   // census_cell, census_sites_all: the census both homes run
require dasllama/dasllama_gguf   // nolint:STYLE030,LINT019 - parse_gguf_meta, inside the das_metal halves

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // the CPU chain's prompt is the parity reference
}

// The weight-format census: each site's tag against the file's own tensor type, one soft check a site so
// the cell names every site still read through a converted copy, then a Metal prefill window and Metal
// decode steps on the blob twin against the CPU chain over the same tokens, the device counters - the
// token-table gather among them - asserted rising. Qwen3.6-35B-A3B-MTP-IQ4_XS-pure (every big plane
// IQ4_XS on disk, the table untied) is large tier (DASLLAMA_PARITY_FULL=1); Llama-3.2-3B Q4_K_M holds
// the gather on a tied Q6_K table. Each skips off the JIT, without dasMetal or without its file.

let private CENSUS_FILE = "Qwen3.6-35B-A3B-MTP-IQ4_XS-pure.gguf"
let private TIED_FILE = "Llama-3.2-3B-Instruct-Q4_K_M.gguf"
//! IQ3_S planes beside IQ3_XXS, Q4_K and a tied Q6_K table (Qwen2.5-1.5B IQ3_XS): the dense iq3s4 carrier
let private IQ3S_DENSE_FILE = "Qwen2.5-1.5B-Instruct-IQ3_XS.gguf"
//! the routed downs IQ3_S beside IQ2_S gate and up stacks (the qwen35moe hybrid, large tier): the routed iq3s4 carrier
let private IQ3S_MOE_FILE = "Qwen3.6-35B-A3B-UD-IQ3_S.gguf"
//! a qwen3 1.7B whose every plane and tied table is F16 on disk (the canary decoder): a table no KqFmt carries
let private TIED_F16_FILE = "canary-qwen-2.5b-decoder-f16.gguf"
//! the tied-F16 carrier's bar against the CPU chain: the device classifier reads the table's q8 copy where the CPU reads its fp32
//! rows - the window and six steps read 0.21-0.30 on the M5 Max, the one-step-off rows 10.4 and up
let private TIED_F16_BAR = 1.0
//! the qwen35moe support-matrix row's bar for the 35B against the CPU chain (`qwen35_row`'s cont_tol)
let private HYBRID_MOE_BAR = 6.0
//! the dense K-quant bar: twice the K-quant support-matrix row's reading against the CPU chain (1.82 of a 29-logit row)
let private DENSE_KQ_BAR = 4.0
let private DECODE_STEPS = 6l

//! the pins a cell loads under, put back as found by `census_unpin`
struct private CensusPins {
    wscale : bool
    backend : string
    mode : MetalMode
}

def private census_pin() : CensusPins {
    let pins = CensusPins(wscale = get_wscale_f16(), backend = active_kernel_backend(), mode = get_metal_mode())
    set_wscale_f16(false)   // the blob drivers read the f32 scale plane
    pin_kernel_backend("portable")   // row-major planes: the blob transform declines repacked ones
    set_metal_mode(MetalMode.when_available)
    return pins
}

def private census_unpin(pins : CensusPins) {
    set_metal_mode(pins.mode)
    clear_kernel_backend_pin()
    select_kernel_backend(pins.backend)
    set_wscale_f16(pins.wscale)
}

[test]
def test_metal_format_census_iq4xs_pure(t : T?) {
    t |> run("{CENSUS_FILE}: every site's tag is the file's format, and the Metal window and steps hold the CPU chain") <| @(t : T?) {
        if (!family_on(t, "qwen35moe") || !arm_on(t, "census-iq4xs")) return
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let path = path_join(models_dir(), CENSUS_FILE)
            return if (!jit_model_available(t, path))
            let pins = census_pin()
            var tr <- load_model_(path, QuantMode.q8)
            tr.config.seq_len = min(tr.config.seq_len, 512l)
            let want <- census_sites_all()
            census_cell(t, tr, path, "planar", want)
            var trg <- blob_twin(t, path, 512l)
            census_cell(t, trg, path, "blob twin", want)
            let c = tr.config
            var nrec = 0l
            for (l in range64(c.n_layers)) {
                if (layer_is_recurrent(c, l)) {
                    nrec++
                }
            }
            window_and_steps_cell(t, tr, trg, HYBRID_MOE_BAR, "MetalEmbedIq4xs_metal_embed_kq", "census") $ {
                let ba_n = metal_kernel_count("metal_moe_gemv_iq4xs")
                t |> success(ba_n >= 2l * nrec, "the window's beta/alpha planes rode the IQ4_XS expert-form GEMV ({ba_n} dispatches, {2l * nrec} planes)")
                t |> equal(metal_kernel_count("MetalDnBa_metal_q8_gemv"), 0l, "and the q8 narrow-projection GEMV read none of them")
                let rt_n = metal_kernel_count("metal_f32_exact_mulmm")
                t |> success(rt_n >= c.n_layers, "the window's routers rode the f32 tile over their own F32 rows ({rt_n} dispatches, {c.n_layers} routed layers)")
            }
            //! the coverage counts below are the steps' (the cell resets them after the window)
            {
                let gv_n = metal_kernel_count("MetalKqGemvIq4xs_metal_kq_gemv_iq4")
                let planes = 5l * nrec + 3l * c.n_layers   //! every recurrent layer's five deltanet planes and every layer's shared-expert triple
                t |> success(gv_n >= DECODE_STEPS * planes, "the steps read the deltanet and shared-expert planes through the IQ4_XS single-row GEMV ({gv_n} dispatches, {DECODE_STEPS * planes} planes)")
                t |> equal(metal_kernel_count("MetalQ8GemvW13Sw_metal_q8_gemv_w13sw16") + metal_kernel_count("MetalQ8GemvW13SwSg_metal_q8_gemv_w13sw16"), 0l,
                    "and the q8 fused gate+up form read no shared expert")
            }
            metal_decode_shutdown()
            metal_prefill_shutdown()
            delete trg
            delete tr
            t |> equal(metal_live_object_count(), 0l, "no leaked Metal objects after shutdown")
            census_unpin(pins)
        } else {
            t |> skip("dasMetal not built")
        }
    }
}

[test]
def test_metal_format_census_tied_kq(t : T?) {
    t |> run("{TIED_FILE}: the tied Q6_K table is the classifier's plane and the device gathers its rows") <| @(t : T?) {
        if (!family_on(t, "llama") || !arm_on(t, "census-tied")) return
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let path = path_join(models_dir(), TIED_FILE)
            return if (!jit_model_available(t, path))
            let pins = census_pin()
            var tr <- load_model_(path, QuantMode.q8)
            tr.config.seq_len = min(tr.config.seq_len, 512l)
            t |> success(tr.config.shared_weights && tr.emb_fmt == KqFmt.k6, "the file's table is tied and tagged Q6_K ({tr.emb_fmt})")
            let want <- ["attention q/k/v/o", "token table", "classifier"]
            census_cell(t, tr, path, "planar", want)
            var trg <- blob_twin(t, path, 512l)
            census_cell(t, trg, path, "blob twin", want)
            window_and_steps_cell(t, tr, trg, DENSE_KQ_BAR, "MetalEmbedK6_metal_embed_kq", "tied") $ {
                t |> equal(metal_kernel_count("metal_row_gather"), 0l, "tied: the window read no fp32 table")
            }
            metal_decode_shutdown()
            metal_prefill_shutdown()
            delete trg
            delete tr
            t |> equal(metal_live_object_count(), 0l, "no leaked Metal objects after shutdown")
            census_unpin(pins)
        } else {
            t |> skip("dasMetal not built")
        }
    }
}

//! a model's planes tagged `f` across the dense sites and the routed stacks
def private planes_tagged(m : Model; f : KqFmt) : int64 {
    return (tags_of(m.wq_fmt, f) + tags_of(m.wk_fmt, f) + tags_of(m.wv_fmt, f) + tags_of(m.wo_fmt, f) + tags_of(m.w1_fmt, f)
        + tags_of(m.w2_fmt, f) + tags_of(m.w3_fmt, f) + tags_of(m.we1_fmt, f) + tags_of(m.we2_fmt, f) + tags_of(m.we3_fmt, f))
}

def private tags_of(a : array<KqFmt>; f : KqFmt) : int64 {
    var n = 0l
    for (x in a) {
        if (x == f) {
            n++
        }
    }
    return n
}

[test]
def test_metal_format_census_iq3s4(t : T?) {
    t |> run("{IQ3S_DENSE_FILE} under the lut pin: the dense IQ3_S planes serve as iq3s4 on Metal, the window and the steps on its stamps") <| @(t : T?) {
        if (!family_on(t, "qwen2") || !arm_on(t, "census-iq3s4")) return
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let path = path_join(models_dir(), IQ3S_DENSE_FILE)
            return if (!jit_model_available(t, path))
            let pins = census_pin()
            let serve0 = get_iq3s_serve_pin()
            set_iq3s_serve_pin("lut")
            var tr <- load_model_(path, QuantMode.q8)
            tr.config.seq_len = min(tr.config.seq_len, 512l)
            let n35 = planes_tagged(tr, KqFmt.iq3s4)
            t |> success(n35 > 0l && planes_tagged(tr, KqFmt.iq3s) == 0l, "the IQ3_S planes load as iq3s4 ({n35} planes), none as the grid")
            census_cell(t, tr, path, "planar", ["attention q/k/v/o", "token table", "classifier"])
            var trg <- blob_twin(t, path, 512l)
            census_cell(t, trg, path, "blob twin", ["attention q/k/v/o", "token table", "classifier"])
            window_and_steps_cell(t, tr, trg, DENSE_KQ_BAR, "MetalEmbedK6_metal_embed_kq", "iq3s4") $ {
                //! the last layer's FFN tail narrows to the last row, so one of its sites rides the single-row GEMV
                //! the tensor twins compile only under the dev-W panel lever, so their counts read 0 when nothing compiled them
                let mm_n = (metal_kernel_count("MetalKqMulMmIq3s4_metal_kq_mulmm") + metal_kernel_count_compiled("MetalKqMulMmIq3s4T_metal_kq_mulmm_iq4xs_t")
                    + metal_kernel_count_compiled("MetalKqMulMmIq3s4TH_metal_kq_mulmm_iq4xs_t"))
                let gv_w = metal_kernel_count("MetalKqGemvIq3s4_metal_kq_gemv_iq4")
                t |> success(mm_n + gv_w >= n35 && mm_n > 0l, "the window's iq3s4 sites rode the iq3s4 stamps ({mm_n} GEMM and {gv_w} GEMV dispatches, {n35} planes)")
            }
            let gv_n = metal_kernel_count("MetalKqGemvIq3s4_metal_kq_gemv_iq4")
            t |> success(gv_n >= DECODE_STEPS * n35, "the steps read every iq3s4 plane through its single-row GEMV ({gv_n} dispatches, {DECODE_STEPS * n35} planes)")
            metal_decode_shutdown()
            metal_prefill_shutdown()
            delete trg
            delete tr
            set_iq3s_serve_pin(serve0)
            t |> equal(metal_live_object_count(), 0l, "no leaked Metal objects after shutdown")
            census_unpin(pins)
        } else {
            t |> skip("dasMetal not built")
        }
    }
    t |> run("{IQ3S_MOE_FILE} under the lut pin: the routed IQ3_S downs serve as iq3s4 on Metal, the routed block on its stamps") <| @(t : T?) {
        if (!family_on(t, "qwen35moe") || !arm_on(t, "census-iq3s4-moe")) return
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let path = path_join(models_dir(), IQ3S_MOE_FILE)
            return if (!jit_model_available(t, path))
            let pins = census_pin()
            let serve0 = get_iq3s_serve_pin()
            set_iq3s_serve_pin("lut")
            var tr <- load_model_(path, QuantMode.q8)
            tr.config.seq_len = min(tr.config.seq_len, 512l)
            let n35 = planes_tagged(tr, KqFmt.iq3s4)
            t |> success(n35 > 0l && planes_tagged(tr, KqFmt.iq3s) == 0l, "the IQ3_S expert stacks load as iq3s4 ({n35} stacks), none as the grid")
            census_cell(t, tr, path, "planar", ["routed experts"])
            var trg <- blob_twin(t, path, 512l)
            census_cell(t, trg, path, "blob twin", ["routed experts"])
            window_and_steps_cell(t, tr, trg, HYBRID_MOE_BAR, "MetalEmbedK6_metal_embed_kq", "iq3s4 moe") $ {
                let mm_n = (metal_kernel_count("metal_moe_mulmm_iq3s4") + metal_kernel_count("MetalMoeMulMmIq3s4T_metal_moe_mulmm_iq4xs_t")
                    + metal_kernel_count("MetalMoeMulMmIq3s4TH_metal_moe_mulmm_iq4xs_t") + metal_kernel_count("MetalMoeMulMmIq3s4TH128_metal_moe_mulmm_iq4xs_t")
                    + metal_kernel_count("MetalMoeMulMmIq3s4THR_metal_moe_mulmm_iq4xs_t") + metal_kernel_count("metal_moe_gemv_iq3s4"))
                t |> success(mm_n >= n35, "the window's iq3s4 expert stacks rode the iq3s4 routed stamps ({mm_n} dispatches, {n35} stacks)")
            }
            let gv_n = metal_kernel_count("metal_moe_gemv_iq3s4")
            t |> success(gv_n >= DECODE_STEPS * n35, "the steps read every iq3s4 expert stack through the routed GEMV ({gv_n} dispatches, {DECODE_STEPS * n35} stacks)")
            metal_decode_shutdown()
            metal_prefill_shutdown()
            delete trg
            delete tr
            set_iq3s_serve_pin(serve0)
            t |> equal(metal_live_object_count(), 0l, "no leaked Metal objects after shutdown")
            census_unpin(pins)
        } else {
            t |> skip("dasMetal not built")
        }
    }
}

//! a model's per-32 dense planes (q51, iq4nl32, q4032, q5032)
def private per32_planes(m : Model) : int64 => (planes_tagged(m, KqFmt.q51) + planes_tagged(m, KqFmt.iq4nl32) + planes_tagged(m, KqFmt.q4032)
    + planes_tagged(m, KqFmt.q5032))

let private PER32_STEMS = fixed_array("Q51", "Iq4nl32", "Q4032", "Q5032")

//! the window's per-32 GEMM, small-batch and GEMV dispatches (the kernel coverage census since its last reset)
def private per32_window_dispatches() : int64 {
    var n = 0l
    for (f in PER32_STEMS) {
        n += metal_kernel_count("MetalKqMulMm{f}_metal_kq_mulmm")
        for (ks in ["", "Ks"]) {
            for (w in [2, 4, 8]) {
                n += metal_kernel_count("MetalB32MvB{w}{f}{ks}_metal_b32_mvb")
            }
        }
    }
    return n + per32_gemv_dispatches()
}

//! the per-32 one-row GEMV dispatches, the two-row stamps and the K-split ones
def private per32_gemv_dispatches() : int64 {
    var n = 0l
    for (f in ["q51", "iq4nl32", "q4032", "q5032"]) {
        n += metal_kernel_count("metal_kq_gemv_{f}") + metal_kernel_count("metal_kq_gemv_{f}_ks")
    }
    return n
}

//! the sites a per-32 carrier must carry: the dense ones, and on the hybrid the deltanet planes and the NextN eh_proj
def private per32_census_sites(hybrid : bool) : array<string> {
    var s <- ["attention q/k/v/o", "token table", "classifier"]
    if (hybrid) {
        s |> push("deltanet qkv/gate/out")
        s |> push("deltanet beta/alpha")
        s |> push("NextN eh_proj")
    }
    return <- s
}

//! the per-32 carriers: the dense planes, and on the `-pure` / table mints the token table, the classifier, the NextN eh_proj and
//! the deltanet planes in the per-32 form too, each site's census asserted native
[test]
def test_metal_format_census_per32(t : T?) {
    for (spec in [(file = "Llama-3.2-1B-Instruct-Q5_1-local.gguf", fam = "llama", arm = "census-q51", emb = "MetalEmbedK6_metal_embed_kq", hy = false),
                  (file = "Qwen2.5-0.5B-Instruct-IQ4_NL-local.gguf", fam = "qwen2", arm = "census-iq4nl32", emb = "metal_embed_q8", hy = false),
                  (file = "Qwen2.5-0.5B-Instruct-IQ4_NL-pure-local.gguf", fam = "qwen2", arm = "census-iq4nl32-table", emb = "MetalEmbedB32Iq4nl32_metal_embed_b32", hy = false),
                  (file = "Qwen2.5-0.5B-Instruct-Q4_0-local.gguf", fam = "qwen2", arm = "census-q4032", emb = "MetalEmbedB32Q4032_metal_embed_b32", hy = false),
                  (file = "Qwen2.5-0.5B-Instruct-Q5_0-pure-local.gguf", fam = "qwen2", arm = "census-q5032", emb = "MetalEmbedB32Q5032_metal_embed_b32", hy = false),
                  (file = "Qwen3.5-0.8B-MTP-Q5_1-local.gguf", fam = "qwen35", arm = "census-q51-table", emb = "MetalEmbedB32Q51_metal_embed_b32", hy = true)]) {
        t |> run("{spec.file}: the per-32 planes serve their own bytes on Metal - census, window and steps") <| @(t : T?) {
            if (!family_on(t, spec.fam) || !arm_on(t, spec.arm)) return
            static_if (typeinfo builtin_module_exists(das_metal)) {
                let path = path_join(models_dir(), spec.file)
                return if (!jit_model_available(t, path))
                let pins = census_pin()
                var tr <- load_model_(path, QuantMode.q8)
                tr.config.seq_len = min(tr.config.seq_len, 512l)
                let np32 = per32_planes(tr)
                t |> success(np32 > 0l, "{spec.arm}: the dense planes load as q51 / iq4nl32 / q4032 / q5032 ({np32} planes)")
                let want <- per32_census_sites(spec.hy)
                census_cell(t, tr, path, "planar", want)
                var trg <- blob_twin(t, path, 512l)
                census_cell(t, trg, path, "blob twin", want)
                window_and_steps_cell(t, tr, trg, DENSE_KQ_BAR, spec.emb, spec.arm) $ {
                    let w = per32_window_dispatches()
                    t |> success(w >= np32, "{spec.arm}: the window's per-32 sites rode the per-32 stamps ({w} dispatches, {np32} planes)")
                }
                let gv = per32_gemv_dispatches()
                t |> success(metal_kernel_count("metal_kq_gemv_q51_ks") + metal_kernel_count("metal_kq_gemv_iq4nl32_ks") + metal_kernel_count("metal_kq_gemv_q4032_ks")
                        + metal_kernel_count("metal_kq_gemv_q5032_ks") > 0l,
                    "{spec.arm}: the narrow planes rode the K-split GEMV")
                t |> success(gv >= DECODE_STEPS * np32, "{spec.arm}: the steps read every per-32 plane through its GEMV ({gv} dispatches, {DECODE_STEPS * np32} planes)")
                metal_decode_shutdown()
                metal_prefill_shutdown()
                delete trg
                delete tr
                t |> equal(metal_live_object_count(), 0l, "no leaked Metal objects after shutdown")
                census_unpin(pins)
            } else {
                t |> skip("dasMetal not built")
            }
        }
    }
}

//! the plain routed MoE's bar against the CPU chain (the MTP parity file's plain-MoE bar): the Q5_1 mint reads 0.62-1.07, the
//! Q4_0 mint 0.97-1.74 on the M5 Max, the one-step-off rows 10.8 and up
let private PLAIN_MOE_BAR = 2.0

//! the per-32 MoE carriers (large tier): the Q5_1 mint's every site - the shared expert and the 1408-wide routed stacks among them
//! - as q51, the Q4_0 mint's 1408-wide routed down stacks as q4032, the Q5_0 `--pure` mint's every site - the routed and shared
//! expert stacks and the token table among them - as q5032, each site's census asserted native, the window and the steps on the
//! device against the CPU chain
[test]
def test_metal_format_census_per32_moe(t : T?) {
    for (spec in [(file = "Qwen1.5-MoE-A2.7B-Chat-Q5_1-local.gguf", arm = "census-moe-q51", emb = "MetalEmbedB32Q51_metal_embed_b32", f = KqFmt.q51),
                  (file = "Qwen1.5-MoE-A2.7B-Chat-Q4_0-local.gguf", arm = "census-moe-q4032", emb = "MetalEmbedQ40_metal_embed_kq", f = KqFmt.q4032),
                  (file = "Qwen1.5-MoE-A2.7B-Chat-Q5_0-pure-local.gguf", arm = "census-moe-q5032", emb = "MetalEmbedB32Q5032_metal_embed_b32", f = KqFmt.q5032)]) {
        t |> run("{spec.file}: the per-32 shared expert and routed stacks serve their own bytes on Metal - census, window and steps") <| @(t : T?) {
            if (!family_on(t, "qwen2moe") || !arm_on(t, spec.arm)) return
            static_if (typeinfo builtin_module_exists(das_metal)) {
                let path = path_join(models_dir(), spec.file)
                return if (!jit_model_available(t, path))
                let pins = census_pin()
                var tr <- load_model_(path, QuantMode.q8)
                tr.config.seq_len = min(tr.config.seq_len, 512l)
                let nexp = tags_of(tr.we1_fmt, spec.f) + tags_of(tr.we2_fmt, spec.f) + tags_of(tr.we3_fmt, spec.f)
                t |> success(nexp >= tr.config.n_layers, "{spec.arm}: the routed stacks load as {spec.f} ({nexp} stacks)")
                let want <- ["attention q/k/v/o", "routed experts", "shared expert", "token table", "classifier"]
                census_cell(t, tr, path, "planar", want)
                var trg <- blob_twin(t, path, 512l)
                census_cell(t, trg, path, "blob twin", want)
                let stem = "{spec.f}"
                let cls = spec.f == KqFmt.q51 ? "Q51" : (spec.f == KqFmt.q4032 ? "Q4032" : "Q5032")
                let tbody = spec.f == KqFmt.q4032 ? "metal_moe_mulmm_iq4nl32_t" : "metal_moe_mulmm_q51_t"   //! q5032's twins stage on q51's body
                window_and_steps_cell(t, tr, trg, PLAIN_MOE_BAR, spec.emb, spec.arm) $ {
                    var mm = metal_kernel_count("metal_moe_mulmm_{stem}") + metal_kernel_count("metal_moe_gemv_{stem}")
                    for (v in ["T", "TH", "TH128", "THR"]) {
                        mm += metal_kernel_count("MetalMoeMulMm{cls}{v}_{tbody}")
                    }
                    t |> success(mm >= nexp, "{spec.arm}: the window's {stem} expert stacks rode the {stem} routed stamps ({mm} dispatches, {nexp} stacks)")
                }
                let gv = metal_kernel_count("metal_moe_gemv_{stem}")
                t |> success(gv >= DECODE_STEPS * nexp, "{spec.arm}: the steps read every {stem} expert stack through the routed GEMV ({gv} dispatches, {DECODE_STEPS * nexp} stacks)")
                metal_decode_shutdown()
                metal_prefill_shutdown()
                delete trg
                delete tr
                t |> equal(metal_live_object_count(), 0l, "no leaked Metal objects after shutdown")
                census_unpin(pins)
            } else {
                t |> skip("dasMetal not built")
            }
        }
    }
}

[test]
def test_metal_format_census_tied_f16(t : T?) {
    t |> run("{TIED_F16_FILE}: a tied F16 table no KqFmt carries serves on Metal - the classifier through its q8 copy, the rows off the exact fp32 table") <| @(t : T?) {
        if (!family_on(t, "qwen3") || !arm_on(t, "census-f16")) return
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let path = path_join(models_dir(), TIED_F16_FILE)
            return if (!jit_model_available(t, path))
            let pins = census_pin()
            var tr <- load_model_(path, QuantMode.q8)
            tr.config.seq_len = min(tr.config.seq_len, 512l)
            t |> success(tr.config.shared_weights && tr.cls_q8_copy && cls_fmt_of(tr) == KqFmt.q8 && !cls_tied_f32(tr),
                "the file's F16 table is tied and the load kept a q8 copy for the device classifier (cls_q8_copy {tr.cls_q8_copy})")
            converted_cls_cell(t, tr, path, "planar")
            var trg <- blob_twin(t, path, 512l)
            converted_cls_cell(t, trg, path, "blob twin")
            window_and_steps_cell(t, tr, trg, TIED_F16_BAR, "metal_row_gather", "tied-f16") $ {
                t |> equal(metal_kernel_count("metal_embed_q8"), 0l, "tied-f16: the window read no q8 table - its rows are the exact fp32 ones")
            }
            metal_decode_shutdown()
            metal_prefill_shutdown()
            delete trg
            delete tr
            t |> equal(metal_live_object_count(), 0l, "no leaked Metal objects after shutdown")
            census_unpin(pins)
        } else {
            t |> skip("dasMetal not built")
        }
    }
}

//! the census of a model whose tied table no KqFmt carries: the classifier and the token table each read through a converted
//! copy (the q8 copy, the fp32 rows), listed as such in one log line - the sites the format rule names as converted, never served
def private converted_cls_cell(t : T?; tr : Model; path, tag : string) {
    let f = fopen(path, "rb")
    if (f == null) {
        t |> failure("{tag}: cannot open {path}")
        return
    }
    fmap(f) $(var bytes : array<uint8>#) {
        var inscope m <- parse_gguf_meta(bytes)
        var inscope sites <- format_census(tr, m)
        var copies = ""
        for (s in sites) {
            if (s.copies > 0) {
                copies = "{copies} [{s.site}: {s.copies}/{s.planes} - {s.first}]"
            }
            if (s.site == "classifier" || s.site == "token table") {
                t |> equal(s.copies, 1, "{tag} {s.site}: the F16 table is a converted copy on this load ({s.first})")
            }
        }
        to_log(LOG_INFO, "{tag}: sites on a converted copy:{empty(copies) ? " none" : copies}\n")
    }
    fclose(f)
}

[test]
def test_metal_tied_classifier_reads(t : T?) {
    t |> run("a tied table with no plane of its own reads fp32 alone until the load keeps a q8 copy for the device classifier") @(t : T?) {
        var inscope m <- Model()
        m.config.shared_weights = true
        t |> success(cls_tied_f32(m), "a tied fp32 table: the device has no classifier plane")
        m.cls_q8_copy = true
        t |> success(!cls_tied_f32(m), "its q8 copy: the device classifier reads it")
        t |> equal(cls_fmt_of(m), KqFmt.q8, "and reads it as q8")
        m.cls_q8_copy = false
        m.cls_q8 = true
        t |> success(!cls_tied_f32(m), "a Q8_0 table: the classifier's own q8 plane")
        m.cls_q8 = false
        m.kquant_native = true
        m.emb_fmt = KqFmt.k6
        t |> success(!cls_tied_f32(m) && emb_kq(m), "a K-quant table: the classifier is the table's own plane")
        m.config.shared_weights = false
        m.wcls_off = 64l
        m.emb_kq_off = 4096l
        t |> equal(emb_kq_plane_off(m), 4096l, "untied, the table keeps a plane of its own")
        m.config.shared_weights = true
        t |> equal(emb_kq_plane_off(m), 64l, "tied, the table is the classifier's plane")
    }
}

def private maxdiff(a, b : array<float>; n : int64) : float {
    var d = 0.0
    for (i in range64(n)) {
        d = max(d, abs(a[i] - b[i]))
    }
    return d
}

//! "1, 2, ..., n," - the confident counting prompt
def private counting_text(n : int) : string {
    return build_string() $(var w) {
        for (i in range(1, n + 1)) {
            w |> write(i)
            w |> write(", ")
        }
    }
}

//! the Metal prefill window over a counting prompt and DECODE_STEPS Metal decode steps on the blob twin, against the CPU
//! chain over the same tokens (each step fed the CPU's argmax): every compare within `bar`, the CPU's next-step row as each
//! compare's control outside it, the prefill and decode counters rising, the token-table gather `emb_kernel` dispatched by the
//! window and by every step (the host embedded no row), and the caller's own counter asserts after the window; the kernel
//! coverage the cell leaves is the steps'
[unused_argument(t, tr, trg, bar, emb_kernel, tag, after_window)]
def private window_and_steps_cell(t : T?; tr, trg : Model; bar : float; emb_kernel, tag : string; after_window : block<() : void>) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let c = tr.config
        let vocab = c.vocab_size
        var inscope prompt <- encode(tr, counting_text(24), true, false)
        with_dasllama_jobque() {
            var sc = make_run_state(c, KVDtype.f16, KVDtype.f16)
            var sg = make_run_state(trg.config, KVDtype.f16, KVDtype.f16)
            eval_(tr, sc, prompt)
            let pf0 = metal_prefill_stats().prefills
            metal_kernel_coverage_reset()
            select_prefill_override("metal")
            set_metal_prefill_min_npos(1l)
            eval_(trg, sg, prompt)
            set_metal_prefill_min_npos(64l)
            select_prefill_override("")
            t |> equal(metal_prefill_stats().prefills - pf0, 1l, "{tag}: the window was one prefill the device served")
            let emb_w = metal_kernel_count(emb_kernel)
            t |> success(emb_w >= 1l, "{tag}: the window gathered its token rows on the device ({emb_kernel}: {emb_w} dispatches)")
            invoke(after_window)
            let pf_d = maxdiff(sc.logits, sg.logits, vocab)
            var cpu_prev, gpu_window : array<float>
            cpu_prev := sc.logits
            gpu_window := sg.logits
            to_log(LOG_INFO, "{tag} window: CPU next '{eyeball_str(piece_(tr, argmax_of(sc.logits, vocab)))}' vs Metal next '{eyeball_str(piece_(tr, argmax_of(sg.logits, vocab)))}' (maxd {pf_d})\n")
            t |> success(pf_d <= bar, "{tag}: the window's last row within {bar} of the CPU chain's (maxd {pf_d})")
            select_decode_override("metal")
            let de0 = metal_decode_stats().decodes
            metal_kernel_coverage_reset()
            var worst = 0.0
            var off_min = 1.0e30
            var one <- [0l]
            for (step in range64(DECODE_STEPS)) {
                one[0] = argmax_of(sc.logits, vocab)
                eval_(tr, sc, one)
                eval_(trg, sg, one)
                let d = maxdiff(sc.logits, sg.logits, vocab)
                let off = maxdiff(cpu_prev, sg.logits, vocab)
                if (step == 0l) {
                    let ctl = maxdiff(sc.logits, gpu_window, vocab)
                    t |> success(ctl > bar, "{tag}: the window compare's control: the CPU chain's next row lands outside the bar of the window's ({ctl})")
                }
                to_log(LOG_INFO, "{tag} step {step}: fed '{eyeball_str(piece_(tr, one[0]))}', CPU next '{eyeball_str(piece_(tr, argmax_of(sc.logits, vocab)))}' vs Metal next '{eyeball_str(piece_(tr, argmax_of(sg.logits, vocab)))}' (maxd {d}, one step off {off})\n")
                worst = max(worst, d)
                off_min = min(off_min, off)
                cpu_prev := sc.logits
            }
            select_decode_override("")
            let served = metal_decode_stats().decodes - de0
            t |> equal(served, DECODE_STEPS, "{tag}: every step was a device decode")
            let emb_s = metal_kernel_count(emb_kernel)
            t |> success(emb_s >= DECODE_STEPS, "{tag}: every step gathered its token row on the device ({emb_kernel}: {emb_s} dispatches, {DECODE_STEPS} steps)")
            t |> success(worst <= bar, "{tag}: every step within {bar} of the CPU chain's (worst {worst})")
            t |> success(off_min > bar, "{tag}: the steps' control: the CPU's previous row lands outside the bar at every step (nearest {off_min})")
            delete one
            delete cpu_prev
            delete gpu_window
            delete sc
            delete sg
        }
    }
}
