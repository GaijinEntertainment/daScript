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
