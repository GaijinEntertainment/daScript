options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama   // nolint:STYLE030,LINT019 - the public facade, reached inside the das_metal halves
require dasllama/dasllama_math   // nolint:STYLE030,LINT019 - set_wscale_f16, inside the das_metal halves
require dasllama/dasllama_gguf   // nolint:STYLE030,LINT019 - the file's own tensor types, inside the das_metal halves
require _metal_blob_twin   // nolint:STYLE030,LINT019 - blob_twin and the kernel census, inside the das_metal halves
require ?das_metal dasllama/dasllama_metal_common   // nolint:STYLE030,LINT019 - the kernel coverage census, inside the das_metal halves
require ?das_metal dasllama/dasllama_metal_decode   // nolint:STYLE030,LINT019 - metal_decode_stats and shutdown, inside the das_metal halves
require ?das_metal dasllama/dasllama_metal_prefill   // nolint:STYLE030,LINT019 - metal_prefill_stats and shutdown, inside the das_metal halves
require ?das_metal metal/das_metal_boost   // nolint:STYLE030,LINT019 - metal_live_object_count for the leak gate, inside the das_metal halves
require daslib/jobque_boost
require daslib/fio   // nolint:STYLE030,LINT019 - path_join and fmap, inside the das_metal halves
require math
require _model_tier

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // the CPU chain's prompt is the parity reference
}

// The weight-format census on Qwen3.6-35B-A3B-MTP-IQ4_XS-pure, every big plane IQ4_XS on disk: each
// site's tag against the file's own tensor type, one soft check a site so the cell names every
// site still read through a converted copy, then a Metal prefill window and Metal decode steps on
// the blob twin against the CPU chain over the same tokens, the device counters asserted rising.
// Large tier (DASLLAMA_PARITY_FULL=1); skips off the JIT, without dasMetal or without the file.

let private CENSUS_FILE = "Qwen3.6-35B-A3B-MTP-IQ4_XS-pure.gguf"
//! the qwen35moe support-matrix row's bar for the 35B against the CPU chain (`qwen35_row`'s cont_tol)
let private HYBRID_MOE_BAR = 6.0
let private DECODE_STEPS = 6l

//! one site's tally: how many of its planes the file carries and how many the load serves in another form
struct private SiteCensus {
    site : string
    planes : int
    copies : int
    first : string
}

def private census_plane(var c : SiteCensus; m : GGUFMeta; name : string; tag : KqFmt) {
    return if (gguf_find_tensor(m, name) < 0)
    c.planes++
    let disk = gguf_tensor_type(m, name)
    if (kq_ggml_type(tag) != disk) {
        if (c.copies == 0) {
            c.first = "{name}: {ggml_type_name(disk)} on disk, served as {tag}"
        }
        c.copies++
    }
}

//! every site's tally over the loaded model `tr` against the file's tensor types
def private format_census(tr : Model; m : GGUFMeta) : array<SiteCensus> {
    var attn = SiteCensus(site = "attention q/k/v/o")
    var exps = SiteCensus(site = "routed experts")
    var shexp = SiteCensus(site = "shared expert")
    var dn = SiteCensus(site = "deltanet qkv/gate/out")
    var ba = SiteCensus(site = "deltanet beta/alpha")
    var emb = SiteCensus(site = "token table")
    var cls = SiteCensus(site = "classifier")
    var eh = SiteCensus(site = "NextN eh_proj")
    let c = tr.config
    for (l in range64(c.n_layers + c.n_layer_nextn)) {
        census_plane(attn, m, "blk.{l}.attn_q.weight", fmt_at(tr.wq_fmt, l))
        census_plane(attn, m, "blk.{l}.attn_k.weight", fmt_at(tr.wk_fmt, l))
        census_plane(attn, m, "blk.{l}.attn_v.weight", fmt_at(tr.wv_fmt, l))
        census_plane(attn, m, "blk.{l}.attn_output.weight", fmt_at(tr.wo_fmt, l))
        census_plane(exps, m, "blk.{l}.ffn_gate_exps.weight", fmt_at(tr.we1_fmt, l))
        census_plane(exps, m, "blk.{l}.ffn_up_exps.weight", fmt_at(tr.we3_fmt, l))
        census_plane(exps, m, "blk.{l}.ffn_down_exps.weight", fmt_at(tr.we2_fmt, l))
        census_plane(shexp, m, "blk.{l}.ffn_gate_shexp.weight", fmt_at(tr.wsh1_fmt, l))
        census_plane(shexp, m, "blk.{l}.ffn_up_shexp.weight", fmt_at(tr.wsh3_fmt, l))
        census_plane(shexp, m, "blk.{l}.ffn_down_shexp.weight", fmt_at(tr.wsh2_fmt, l))
        census_plane(dn, m, "blk.{l}.attn_qkv.weight", fmt_at(tr.dnqkv_fmt, l))
        census_plane(dn, m, "blk.{l}.attn_gate.weight", fmt_at(tr.dngate_fmt, l))
        census_plane(dn, m, "blk.{l}.ssm_out.weight", fmt_at(tr.dnout_fmt, l))
        census_plane(ba, m, "blk.{l}.ssm_beta.weight", fmt_at(tr.dnbeta_fmt, l))
        census_plane(ba, m, "blk.{l}.ssm_alpha.weight", fmt_at(tr.dnalpha_fmt, l))
    }
    census_plane(emb, m, "token_embd.weight", tr.emb_fmt)
    census_plane(cls, m, "output.weight", tr.wcls_fmt)
    census_plane(eh, m, "blk.{c.n_layers}.nextn.eh_proj.weight", tr.mtp_ehproj_fmt)
    return <- [attn, exps, shexp, dn, ba, emb, cls, eh]
}

//! the census of `tr` asserted site by site under `tag`; the sites still on a converted copy logged as one list
def private census_cell(t : T?; tr : Model; path, tag : string) {
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
            t |> success(s.planes > 0, "{tag} {s.site}: the file carries the site")
            t |> success(s.copies == 0, "{tag} {s.site}: {s.copies} of {s.planes} planes read through a converted copy ({s.first})")
            if (s.copies > 0) {
                copies = "{copies} [{s.site}: {s.copies}/{s.planes}]"
            }
        }
        to_log(LOG_INFO, "{tag}: sites still on a converted copy:{empty(copies) ? " none" : copies}\n")
    }
    fclose(f)
}

[test]
def test_metal_format_census_iq4xs_pure(t : T?) {
    t |> run("{CENSUS_FILE}: every site's tag is the file's format, and the Metal window and steps hold the CPU chain") <| @(t : T?) {
        if (!family_on(t, "qwen35moe") || !arm_on(t, "census-iq4xs")) return
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let path = path_join(models_dir(), CENSUS_FILE)
            return if (!jit_model_available(t, path))
            let prior_wscale = get_wscale_f16()
            let prior_backend = active_kernel_backend()
            let prior_mode = get_metal_mode()
            set_wscale_f16(false)   // the blob drivers read the f32 scale plane
            pin_kernel_backend("portable")   // row-major planes: the blob transform declines repacked ones
            set_metal_mode(MetalMode.when_available)
            var tr <- load_model_(path, QuantMode.q8)
            tr.config.seq_len = min(tr.config.seq_len, 512l)
            census_cell(t, tr, path, "planar")
            var trg <- blob_twin(t, path, 512l)
            census_cell(t, trg, path, "blob twin")
            window_and_steps_cell(t, tr, trg)
            metal_decode_shutdown()
            metal_prefill_shutdown()
            delete trg
            delete tr
            t |> equal(metal_live_object_count(), 0l, "no leaked Metal objects after shutdown")
            set_metal_mode(prior_mode)
            clear_kernel_backend_pin()
            select_kernel_backend(prior_backend)
            set_wscale_f16(prior_wscale)
        } else {
            t |> skip("dasMetal not built")
        }
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
//! chain over the same tokens (each step fed the CPU's argmax): every compare within HYBRID_MOE_BAR, the CPU's next-step
//! row as each compare's control outside it, the prefill and decode counters and the IQ4_XS single-row GEMV count rising
[unused_argument(t, tr, trg)]
def private window_and_steps_cell(t : T?; tr, trg : Model) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let c = tr.config
        let vocab = c.vocab_size
        var nrec = 0l
        for (l in range64(c.n_layers)) {
            if (layer_is_recurrent(c, l)) {
                nrec++
            }
        }
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
            t |> equal(metal_prefill_stats().prefills - pf0, 1l, "the window was one prefill the device served")
            let ba_n = metal_kernel_count("metal_moe_gemv_iq4xs")
            t |> success(ba_n >= 2l * nrec, "the window's beta/alpha planes rode the IQ4_XS expert-form GEMV ({ba_n} dispatches, {2l * nrec} planes)")
            t |> equal(metal_kernel_count("MetalDnBa_metal_q8_gemv"), 0l, "and the q8 narrow-projection GEMV read none of them")
            let pf_d = maxdiff(sc.logits, sg.logits, vocab)
            var cpu_prev, gpu_window : array<float>
            cpu_prev := sc.logits
            gpu_window := sg.logits
            to_log(LOG_INFO, "census window: CPU next '{eyeball_str(piece_(tr, argmax_of(sc.logits, vocab)))}' vs Metal next '{eyeball_str(piece_(tr, argmax_of(sg.logits, vocab)))}' (maxd {pf_d})\n")
            t |> success(pf_d <= HYBRID_MOE_BAR, "the window's last row within {HYBRID_MOE_BAR} of the CPU chain's (maxd {pf_d})")
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
                    t |> success(ctl > HYBRID_MOE_BAR, "the window compare's control: the CPU chain's next row lands outside the bar of the window's ({ctl})")
                }
                to_log(LOG_INFO, "census step {step}: fed '{eyeball_str(piece_(tr, one[0]))}', CPU next '{eyeball_str(piece_(tr, argmax_of(sc.logits, vocab)))}' vs Metal next '{eyeball_str(piece_(tr, argmax_of(sg.logits, vocab)))}' (maxd {d}, one step off {off})\n")
                worst = max(worst, d)
                off_min = min(off_min, off)
                cpu_prev := sc.logits
            }
            select_decode_override("")
            let served = metal_decode_stats().decodes - de0
            let gv_n = metal_kernel_count("MetalKqGemvIq4xs_metal_kq_gemv_iq4")
            t |> equal(served, DECODE_STEPS, "every step was a device decode")
            t |> success(gv_n >= DECODE_STEPS * 5l * nrec, "the steps read every recurrent layer's five planes through the IQ4_XS single-row GEMV ({gv_n} dispatches, {DECODE_STEPS * 5l * nrec} planes)")
            t |> success(worst <= HYBRID_MOE_BAR, "every step within {HYBRID_MOE_BAR} of the CPU chain's (worst {worst})")
            t |> success(off_min > HYBRID_MOE_BAR, "the steps' control: the CPU's previous row lands outside the bar at every step (nearest {off_min})")
            delete one
            delete cpu_prev
            delete gpu_window
            delete sc
            delete sg
        }
    }
}
