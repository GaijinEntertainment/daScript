options gen2
options _dasllama_internal = true

module _metal_format_census_cells shared public

//! The weight-format census per model file on one loaded form a
//! side: the blob twin's census, its Metal window and six steps on
//! a stream the Metal side picks (`CensusKept`), the CPU form's
//! census and its replay of that stream, the compare after the block

require dastest/testing_boost public
require dasllama/dasllama   // nolint:STYLE029,LINT019 - the public facade
require dasllama/dasllama_common   // nolint:STYLE030,LINT019 - with_cpu_prefill, layer_is_recurrent
require dasllama/dasllama_kqformat   // nolint:STYLE030,LINT019 - KqFmt, inside the das_metal halves
require dasllama/dasllama_gguf   // nolint:STYLE030,LINT019 - parse_gguf_meta, the F16 census
require ?das_metal dasllama/dasllama_metal_common   // nolint:STYLE030,LINT019 - the coverage reset
require ?das_metal dasllama/dasllama_metal_decode   // nolint:STYLE030,LINT019 - metal_decode_stats
require ?das_metal dasllama/dasllama_metal_prefill   // nolint:STYLE030,LINT019 - prefill stats
require ?das_metal dasllama/dasllama_metal_kernels   // nolint:STYLE030,LINT019 - the wide rule
require daslib/jobque_boost
require daslib/fio   // nolint:STYLE030,LINT019 - path_join and fmap, inside the das_metal halves
require math   // nolint:STYLE030,LINT019 - min and max, inside the das_metal halves
require _model_tier
require _model_form   // with_ctx_cut, alone_pass_of, cpu_cells_form
require _metal_blob_twin   // metal_kernel_count, counting_text, with_metal_rails
require _format_census   // census_cell, census_sites_all, format_census
require _metal4_probe   // nolint:STYLE029,STYLE030,LINT019 - metal4_tensor_available, inside the das_metal halves
require _kernel_census   // nolint:STYLE029,STYLE030,LINT019 - kernel_census_reset_metal, inside the das_metal halves

//! the context every cell here cuts its form's model to: each row's
//! model file's largest
let CENSUS_CELLS_CTX = 512l

//! the tied-F16 carrier's bar against the CPU chain: the device
//! classifier reads the table's q8 copy, the CPU its fp32 rows -
//! the window and six steps read 0.21-0.30 on the M5 Max, the
//! one-step-off rows 10.4 and up
let private TIED_F16_BAR = 1.0
//! the qwen35moe support-matrix row's bar for the 35B against the CPU
//! chain (`qwen35_row`'s cont_tol)
let private HYBRID_MOE_BAR = 6.0
//! the dense K-quant bar: twice the K-quant support-matrix row's
//! reading against the CPU chain (1.82 of a 29-logit row)
let private DENSE_KQ_BAR = 4.0
//! the nv4 routed MoE's bar: the window reads under 2 and the steps
//! 2.4 on the M5 Max, the one-step-off rows 10 and up
let private NV4_MOE_BAR = 3.0
//! the plain routed MoE's bar (the MTP parity file's plain-MoE
//! bar): the Q5_1 mint reads 0.62-1.07, the Q4_0 0.97-1.74, the
//! one-step-off rows 10.8 and up
let private PLAIN_MOE_BAR = 2.0
//! the routed affine carrier's bar: the window and six steps read
//! 1.51-2.01 on the M5 Max (the hybrid's deltanet drift under a 4-bit
//! routed stack's near-ties), the one-step-off rows 10.9 and up
let private MLX_MOE_BAR = 3.0
let private DECODE_STEPS = 6l
//! the IQ3_S serving mode the iq3s4 rows' loads pin: the sites tag
//! as iq3s4 at load
let CENSUS_IQ3S_LUT = "lut"

//! which census a row runs: its load tags, sites and kernel counts
enum CensusKind {
    iq4xs_hybrid   //!< the qwen35moe IQ4_XS-pure cut: deltanet and shared expert on IQ4_XS
    tied_kq        //!< a tied Q6_K table gathered on the device
    iq3s4_dense    //!< IQ3_S dense planes under the lut pin
    iq3s4_moe      //!< IQ3_S routed downs under the lut pin
    per32          //!< the per-32 dense planes
    nv4            //!< the nv4 dense planes
    lowbit_sb      //!< a low-bit superblock member's dense planes (`f`: tq2, tq1, iq1s, iq1m) beside K-quant ones
    nv4_moe        //!< the nv4 routed stacks
    per32_moe      //!< the per-32 routed and shared expert stacks
    lowbit_moe     //!< a low-bit member's routed stacks (`f`) on three layers of a whole carrier, every other plane Q8_0
    mlx4b_moe      //!< a snapshot's mlx4b routed stacks, every site native
    tied_f16       //!< a tied F16 table no KqFmt carries: converted copies
}

//! the parameters of one model file's census cells
struct CensusRow {
    file : string    //!< the model file under the models folder, or a snapshot's repo
    snapshot : bool  //!< `file` names a Hugging Face snapshot (`hf_snapshot_dir`)
    fam : string     //!< the family the cells run under
    arm : string     //!< the cells' arm token
    tag : string     //!< the window-and-steps compare's tag
    emb : string     //!< the token-table gather kernel the window and steps dispatch
    bar : float      //!< the compare's bar against the CPU chain
    kind : CensusKind
    hy : bool        //!< a hybrid carrier: the deltanet and NextN sites too
    f : KqFmt        //!< a per-32 MoE row's routed member, a ternary row's dense one
    kq_native : KqNative   //!< the CPU form's K-quant knob: `on` where the block already loads that form
    blocks : int64   //!< past 0: the trunk blocks a cut's name claims
    stacks : int64   //!< a low-bit routed carrier: the routed stacks its member sits on
    cut_tag : string   //!< the cut claim's message opening
    iq3s : string    //!< the IQ3_S serving pin of both loads, "" none
}

//! what the Metal side hands the CPU side and the verdict: the fed
//! stream (the Metal side's argmax), the compared rows of each side
struct CensusKept {
    metal_ran : bool
    cpu_ran : bool
    vocab : int64
    prompt : array<int64>
    fed : array<int64>
    gpu_window : array<float>
    gpu_crowned : array<float>   //!< the window's last row again under the low-bit members' tensor crowns, empty where none was forced
    gpu_steps : array<array<float>>
    cpu_window : array<float>
    cpu_steps : array<array<float>>
    pieces : table<int64; string>
}

def private row_on(t : T?; row : CensusRow) : bool => family_on(t, row.fam) && arm_on(t, row.arm)

//! the window's per-32 GEMM, small-batch and GEMV dispatches
//! since the census's last reset
def private per32_window_dispatches() : int64 {
    var n = 0l
    for (f in PER32_FMTS) {
        n += kq_window_dispatches(f)
    }
    return n
}

//! the per-32 one-row GEMV planes read: the two-row, K-split and
//! wide stamps, twice each fused gate|up dispatch (two planes)
def private per32_gemv_dispatches() : int64 {
    var n = 0l
    for (f in PER32_FMTS) {
        n += kq_gemv_dispatches(f)
    }
    return n
}

def private per32_wide_dispatches() : int64 {
    var n = 0l
    for (f in PER32_FMTS) {
        n += metal_kernel_count("MetalKqGemv{kq_stem(f)}W_metal_b32_gemv_w")
    }
    return n
}

//! the window's GEMM dispatches of an iq4-family format by its class
//! stem (`Iq3s4`, `Nv4`); an uncompiled tensor twin reads 0
def private iq4_family_mm_dispatches(stem : string) : int64 {
    var n = (metal_kernel_count("MetalKqMulMm{stem}_metal_kq_mulmm")
        + metal_kernel_count_compiled("MetalKqMulMm{stem}Sk_metal_kq_mulmm")
        + metal_kernel_count_compiled("MetalKqDequant{stem}_metal_kq_dequant"))
    for (v in ["T", "TH", "TH128"]) {
        n += metal_kernel_count_compiled("MetalKqMulMm{stem}{v}_metal_kq_mulmm_iq4xs_t")
    }
    //! the double-buffered pair and the short window's split-K stamps
    for (v in ["THDb", "THDb128", "THSk", "TSk", "THSk16", "TSk16"]) {
        n += metal_kernel_count_compiled("MetalKqMulMm{stem}{v}_metal_kq_mulmm_db")
    }
    n += (metal_kernel_count("MetalKqMvB2{stem}_metal_kq_mv_iq4")
        + metal_kernel_count("MetalKqMvB4{stem}_metal_kq_mv_iq4"))
    return n + metal_kernel_count("MetalKqMvB8{stem}_metal_kq_mvb8_iq4")
}

//! the sites a dense carrier must carry: the dense ones, and on the
//! hybrid the deltanet planes and the NextN eh_proj
def private dense_census_sites(hybrid : bool) : array<string> {
    var s <- ["attention q/k/v/o", "token table", "classifier"]
    if (hybrid) {
        s |> push("deltanet qkv/gate/out")
        s |> push("deltanet beta/alpha")
        s |> push("NextN eh_proj")
    }
    return <- s
}

def private census_want(row : CensusRow) : array<string> {
    if (row.kind == CensusKind.iq4xs_hybrid) {
        return <- census_sites_all()
    } elif (row.kind == CensusKind.iq3s4_moe) {
        return <- ["routed experts"]
    } elif (row.kind == CensusKind.per32 || row.kind == CensusKind.nv4 || row.kind == CensusKind.lowbit_sb) {
        return <- dense_census_sites(row.hy)
    } elif (row.kind == CensusKind.nv4_moe) {
        return <- ["attention q/k/v/o", "routed experts", "token table", "classifier"]
    } elif (row.kind == CensusKind.per32_moe || row.kind == CensusKind.lowbit_moe) {
        return <- ["attention q/k/v/o", "routed experts", "shared expert", "token table",
            "classifier"]
    } elif (row.kind == CensusKind.mlx4b_moe) {
        return <- ["attention q/k/v/o", "routed experts", "shared expert", "deltanet qkv/gate/out",
            "deltanet beta/alpha", "token table", "classifier"]
    }
    return <- ["attention q/k/v/o", "token table", "classifier"]
}

//! the census of a model whose tied table no KqFmt carries: the
//! classifier and the table each read through a converted copy
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
                t |> equal(s.copies, 1, ("{tag} {s.site}: the F16 table is a converted copy on " +
                    "this load ({s.first})"))
            }
        }
        to_log(LOG_INFO, "{tag}: sites on a converted copy:{empty(copies) ? " none" : copies}\n")
    }
    fclose(f)
}

//! the row's census of `m` under `tag` ("planar", "blob twin")
def private census_of(t : T?; m : Model; row : CensusRow; tag : string) {
    let path = row.snapshot ? hf_snapshot_dir(row.file) : path_join(models_dir(), row.file)
    if (row.kind == CensusKind.tied_f16) {
        converted_cls_cell(t, m, path, tag)
    } else {
        var inscope want <- census_want(row)
        census_cell(t, m, path, tag, want)
    }
}

def private keep_piece_of(var k : CensusKept; m : Model; id : int64) {
    if (!key_exists(k.pieces, id)) {
        k.pieces |> insert(id, piece_(m, id))
    }
}

def private piece_of(k : CensusKept; row : array<float>) : string =>
    eyeball_str(k.pieces?[argmax_of(row, k.vocab)] ?? "")

//! the Metal half of the window-and-steps cell on the blob twin `m`:
//! one Metal prefill window over a counting prompt, then DECODE_STEPS
//! Metal steps each fed the device's own argmax, every row kept
def private metal_window_steps(t : T?; m : Model; row : CensusRow; var kept : CensusKept;
                               after_window : block<() : void>) {   // nolint:LINT012,LINT019
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let tag = row.tag
        let emb_kernel = row.emb
        let vocab = m.config.vocab_size
        kept.vocab = vocab
        kept.prompt <- encode(m, counting_text(24), true, false)
        with_dasllama_jobque() {
            var sg = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
            let pf0 = metal_prefill_stats().prefills
            with_metal_rails(MetalRails(decode = false)) {
                kernel_census_reset_metal()
                eval_(m, sg, kept.prompt)
            }
            t |> equal(metal_prefill_stats().prefills - pf0, 1l,
                "{tag}: the window was one prefill the device served")
            let emb_w = metal_kernel_count(emb_kernel)
            t |> success(emb_w >= 1l, ("{tag}: the window gathered its token rows on the device " +
                "({emb_kernel}: {emb_w} dispatches)"))
            invoke(after_window)
            uncrowned_twins_idle(t, m, row)
            kept.gpu_window := sg.logits
            keep_piece_of(kept, m, argmax_of(sg.logits, vocab))
            var de0 = 0l
            with_metal_rails(MetalRails(prefill = false, min_npos = 0l)) {
                de0 = metal_decode_stats().decodes
                kernel_census_reset_metal()
                var one <- [0l]
                for (_step in range64(DECODE_STEPS)) {
                    one[0] = argmax_of(sg.logits, vocab)
                    kept.fed |> push(one[0])
                    eval_(m, sg, one)
                    kept.gpu_steps |> push_clone(sg.logits)
                    keep_piece_of(kept, m, argmax_of(sg.logits, vocab))
                }
                delete one
            }
            t |> equal(metal_decode_stats().decodes - de0, DECODE_STEPS,
                "{tag}: every step was a device decode")
            let emb_s = metal_kernel_count(emb_kernel)
            t |> success(emb_s >= DECODE_STEPS, ("{tag}: every step gathered its token row on " +
                "the device ({emb_kernel}: {emb_s} dispatches, {DECODE_STEPS} steps)"))
            kept.metal_ran = true
            delete sg
        }
    }
}

//! the stamp body `f`'s routed tensor twins stage on: the per-32 body, the ternary pair's K2 one, the 1-bit grid pair's own
def private routed_twin_body(f : KqFmt) : string {
    return "metal_moe_mulmm_k2_t" if (f == KqFmt.tq2 || f == KqFmt.tq1)
    return "metal_moe_mulmm_iq1_t" if (f == KqFmt.iq1s || f == KqFmt.iq1m)
    return "metal_moe_mulmm_b32_t"
}

//! the low-bit members, whose tensor families a box sidecar may leave uncrowned
let private LOWBIT_TWIN_FMTS = fixed_array(KqFmt.tq2, KqFmt.tq1, KqFmt.q20, KqFmt.q10, KqFmt.iq1s, KqFmt.iq1m)

//! the low-bit members among `m`'s planes - its routed stacks under `stacks`, else its dense planes - whose tensor family
//! (`moe_mulmm_<f>`, `kq_mulmm_<f>`) the box profile leaves uncrowned
def private lowbit_uncrowned(m : Model; stacks : bool) : array<KqFmt> {
    var fmts : array<KqFmt>
    static_if (typeinfo builtin_module_exists(das_metal)) {
        for (f in LOWBIT_TWIN_FMTS) {
            let n = sites_count(m, !stacks, stacks, $(x : KqFmt) : bool => x == f)
            if (n > 0l && !metal_tensor_crowned(stacks ? "moe_mulmm_{f}" : "kq_mulmm_{f}")) {
                fmts |> push(f)
            }
        }
    }
    return <- fmts
}

//! the crowned window's control, read off the plain window's census: an uncrowned low-bit member's dense tensor twins
//! never ran (its sites rode the dequant pass and the split tile). A routed stack has no such control: an uncrowned family
//! takes its routed twins at or under the knee too (`metal_moe_kq_twin_pick`), the window's size
def private uncrowned_twins_idle(t : T?; m : Model; row : CensusRow) {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var inscope fmts <- lowbit_uncrowned(m, false)
        for (f in fmts) {
            let tw = metal_kernel_count_prefix("MetalKqMulMm{kq_stem(f)}T")
            t |> equal(tw, 0l, "{row.tag}: the crowned window's control: with kq_mulmm_{f} uncrowned no {f} tensor twin ran")
        }
    }
}

//! the window again with the tensor family of each low-bit member among `m`'s planes - `kq_mulmm_<f>` on a dense plane,
//! `moe_mulmm_<f>` on a routed stack - crowned where the box profile leaves it out: the drivers shut down so the PSOs
//! compile under the crowns, each such dense member's tensor twins and routed member's routed twins dispatch, the last row
//! kept for the verdict; the crowns put back and the drivers shut down again. Shuts the drivers down mid-cell
def private crowned_window(t : T?; m : Model; row : CensusRow;
                           var kept : CensusKept) {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let crowns0 = get_metal_tensor_crowns()
        var crowns = crowns0
        var fmts <- lowbit_uncrowned(m, false)
        var routed <- lowbit_uncrowned(m, true)
        for (f in fmts) {
            crowns = empty(crowns) ? "kq_mulmm_{f}" : "{crowns},kq_mulmm_{f}"
        }
        for (f in routed) {
            crowns = empty(crowns) ? "moe_mulmm_{f}" : "{crowns},moe_mulmm_{f}"
        }
        if ((!empty(fmts) || !empty(routed)) && !metal4_tensor_available(g_dev)) {
            skip_cell(t, "{row.tag}: the crowned window: this box compiles no Metal-4 tensor twin")
        } elif (!empty(fmts) || !empty(routed)) {
            set_metal_tensor_crowns(crowns)
            metal_decode_shutdown()
            metal_prefill_shutdown()
            with_dasllama_jobque() {
                var sg = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
                with_metal_rails(MetalRails(decode = false)) {
                    kernel_census_reset_metal()
                    eval_(m, sg, kept.prompt)
                }
                for (f in fmts) {
                    let tw = metal_kernel_count_prefix("MetalKqMulMm{kq_stem(f)}T")
                    t |> success(tw > 0l, ("{row.tag}: under the kq_mulmm_{f} crown the window's {f} sites rode the " +
                        "tensor twins ({tw} dispatches)"))
                }
                for (f in routed) {
                    let tw = routed_twin_dispatches(f, routed_twin_body(f), false)
                    t |> success(tw > 0l, ("{row.tag}: under the moe_mulmm_{f} crown the window's {f} expert stacks rode " +
                        "the routed tensor twins ({tw} dispatches)"))
                }
                kept.gpu_crowned := sg.logits
                delete sg
            }
            set_metal_tensor_crowns(crowns0)
            metal_decode_shutdown()
            metal_prefill_shutdown()
        }
        delete fmts
        delete routed
    }
}

//! the CPU chain over the Metal side's prompt and stream, kept
def private cpu_replay(m : Model; var kept : CensusKept) {
    with_dasllama_jobque() {
        var sc = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
        eval_(m, sc, kept.prompt)
        kept.cpu_window := sc.logits
        keep_piece_of(kept, m, argmax_of(sc.logits, kept.vocab))
        var one <- [0l]
        for (id in kept.fed) {
            one[0] = id
            eval_(m, sc, one)
            kept.cpu_steps |> push_clone(sc.logits)
            keep_piece_of(kept, m, argmax_of(sc.logits, kept.vocab))
        }
        delete one
        delete sc
    }
}

def private iq4xs_hybrid_metal(t : T?; m : Model; row : CensusRow;
                               var kept : CensusKept) {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let c = m.config
        var nrec = 0l
        for (l in range64(c.n_layers)) {
            if (layer_is_recurrent(c, l)) {
                nrec++
            }
        }
        let ba_one0 = metal_prefill_dn_ba_one_sites()
        metal_window_steps(t, m, row, kept) $ {
            //! the IQ4_XS beta and alpha planes, adjacent in the
            //! image, project as one [beta | alpha] site a layer
            let ba_n = metal_prefill_dn_ba_one_sites() - ba_one0
            t |> equal(ba_n, nrec, ("the window's beta/alpha planes ran as one site a recurrent " +
                "layer ({nrec} layers)"))
            t |> equal(metal_kernel_count("metal_moe_gemv_iq4xs"), 0l,
                "and none rode the IQ4_XS expert-form GEMV")
            t |> equal(metal_kernel_count("MetalDnBa_metal_q8_gemv"), 0l,
                "and the q8 narrow-projection GEMV read none of them")
            let rt_n = metal_kernel_count("metal_f32_exact_mulmm")
            t |> success(rt_n >= c.n_layers, ("the window's routers rode the f32 tile over their " +
                "own F32 rows ({rt_n} dispatches, {c.n_layers} routed layers)"))
        }
        let gv_n = metal_kernel_count("MetalKqGemvIq4xs_metal_kq_gemv_iq4")
        let fat_n = (metal_kernel_count("MetalKqFatIq4xs_metal_kq_fat_iq4")
            + metal_kernel_count("MetalKqFatIq4xsN_metal_kq_fat_iq4")
            + metal_kernel_count("MetalKqFatIq4xsA_metal_kq_fat_iq4")
            + metal_kernel_count("MetalKqFatIq4xsNX_metal_kq_fat_iq4"))
        //! the fused chain reads a recurrent layer's deltanet planes
        //! as two fat dispatches, every layer's shared expert as one
        let fat_sites = 2l * nrec + c.n_layers
        t |> success(gv_n + fat_n >= DECODE_STEPS * fat_sites, ("the steps read the deltanet " +
            "and shared-expert planes through the IQ4_XS forms - the single-row GEMV or the fat " +
            "family ({gv_n} + {fat_n} dispatches, {DECODE_STEPS * fat_sites} sites)"))
        t |> equal(metal_kernel_count("MetalQ8GemvW13Sw_metal_q8_gemv_w13sw16")
                + metal_kernel_count("MetalQ8GemvW13SwSg_metal_q8_gemv_w13sw16"), 0l,
            "and the q8 fused gate+up form read no shared expert")
    }
}

def private tied_kq_metal(t : T?; m : Model; row : CensusRow;
                          var kept : CensusKept) {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        t |> success(m.config.shared_weights && m.emb_fmt == KqFmt.k6,
            "the file's table is tied and tagged Q6_K ({m.emb_fmt})")
        metal_window_steps(t, m, row, kept) $ {
            t |> equal(metal_kernel_count("metal_row_gather"), 0l,
                "tied: the window read no fp32 table")
        }
    }
}

//! the iq3s4 planes the lut-pinned load tagged, none on the grid
def private iq3s4_loaded(t : T?; m : Model; what, unit : string) : int64 {
    let n35 = sites_count(m, true, true, $(x : KqFmt) : bool => x == KqFmt.iq3s4)
    let ngrid = sites_count(m, true, true, $(x : KqFmt) : bool => x == KqFmt.iq3s)
    t |> success(n35 > 0l && ngrid == 0l,
        "the IQ3_S {what} load as iq3s4 ({n35} {unit}), none as the grid")
    return n35
}

def private iq3s4_dense_metal(t : T?; m : Model; row : CensusRow;
                              var kept : CensusKept) {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let n35 = iq3s4_loaded(t, m, "planes", "planes")
        metal_window_steps(t, m, row, kept) $ {
            //! the last layer's FFN tail narrows to the last row, so
            //! one of its sites rides the single-row GEMV
            let mm_n = iq4_family_mm_dispatches("Iq3s4")
            let gv_w = metal_kernel_count("MetalKqGemvIq3s4_metal_kq_gemv_iq4")
            t |> success(mm_n + gv_w >= n35 && mm_n > 0l, ("the window's iq3s4 sites rode the " +
                "iq3s4 stamps ({mm_n} GEMM and {gv_w} GEMV dispatches, {n35} planes)"))
        }
        let gv_n = metal_kernel_count("MetalKqGemvIq3s4_metal_kq_gemv_iq4")
        t |> success(gv_n >= DECODE_STEPS * n35, ("the steps read every iq3s4 plane through its " +
            "single-row GEMV ({gv_n} dispatches, {DECODE_STEPS * n35} planes)"))
    }
}

def private iq3s4_moe_metal(t : T?; m : Model; row : CensusRow;
                            var kept : CensusKept) {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let n35 = iq3s4_loaded(t, m, "expert stacks", "stacks")
        metal_window_steps(t, m, row, kept) $ {
            let mm_n = (metal_kernel_count("metal_moe_mulmm_iq3s4")
                + routed_twin_dispatches(KqFmt.iq3s4, "metal_moe_mulmm_iq4xs_t", false)
                + metal_kernel_count("metal_moe_gemv_iq3s4"))
            t |> success(mm_n >= n35, ("the window's iq3s4 expert stacks rode the iq3s4 routed " +
                "stamps ({mm_n} dispatches, {n35} stacks)"))
        }
        let gv_n = metal_kernel_count("metal_moe_gemv_iq3s4")
        t |> success(gv_n >= DECODE_STEPS * n35, ("the steps read every iq3s4 expert stack " +
            "through the routed GEMV ({gv_n} dispatches, {DECODE_STEPS * n35} stacks)"))
    }
}

def private per32_metal(t : T?; m : Model; row : CensusRow;
                        var kept : CensusKept) {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let arm = row.arm
        let np32 = per32_plane_count(m, true)
        t |> success(np32 > 0l,
            "{arm}: the dense planes load in a per-32 format ({np32} planes)")
        metal_window_steps(t, m, row, kept) $ {
            let w = per32_window_dispatches()
            t |> success(w >= np32, ("{arm}: the window's per-32 sites rode the per-32 stamps " +
                "({w} dispatches, {np32} planes)"))
        }
        let gv = per32_gemv_dispatches()
        let kv_rows = m.config.n_kv_heads * m.config.head_size
        let wide = get_metal_b32_wide()
        //! a k/v plane under the wide form's rows rides the K-split
        if (!wide || !b32_wide_fits(kv_rows)) {
            var ks = 0l
            for (f in PER32_FMTS) {
                ks += metal_kernel_count("metal_kq_gemv_{f}_ks")
            }
            t |> success(ks > 0l, ("{arm}: the narrow planes rode the K-split GEMV " +
                "(DASLLAMA_METAL_B32_WIDE={wide}, k/v rows {kv_rows})"))
        }
        t |> success(per32_wide_dispatches() > 0l || !wide, ("{arm}: the planes of 512 rows or " +
            "more rode the wide GEMV (DASLLAMA_METAL_B32_WIDE={wide})"))
        t |> success(gv >= DECODE_STEPS * np32, ("{arm}: the steps read every per-32 plane " +
            "through its GEMV ({gv} dispatches, {DECODE_STEPS * np32} planes)"))
    }
}

def private nv4_metal(t : T?; m : Model; row : CensusRow;
                      var kept : CensusKept) {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let arm = row.arm
        let nplanes = sites_count(m, true, true, $(x : KqFmt) : bool => x == KqFmt.nv4)
        t |> success(nplanes > 0l, "{arm}: the dense planes load as nv4 ({nplanes} planes)")
        metal_window_steps(t, m, row, kept) $ {
            //! the last layer's FFN tail rides the single-row GEMV,
            //! the hybrid's beta/alpha window rows the routed GEMV
            let w = (iq4_family_mm_dispatches("Nv4")
                + metal_kernel_count("MetalKqGemvNv4_metal_kq_gemv_iq4")
                + metal_kernel_count("metal_moe_gemv_nv4"))
            t |> success(w >= nplanes, ("{arm}: the window's nv4 sites rode the nv4 stamps " +
                "({w} dispatches, {nplanes} planes)"))
        }
        let gv = metal_kernel_count("MetalKqGemvNv4_metal_kq_gemv_iq4")
        t |> success(gv >= DECODE_STEPS * nplanes, ("{arm}: the steps read every nv4 plane " +
            "through its GEMV ({gv} dispatches, {DECODE_STEPS * nplanes} planes)"))
    }
}

//! a low-bit superblock member's dense planes: they load as `row.f`, the window rides the member's tiles, small-batch forms
//! and GEMV, every step reads each plane through the member's GEMV
def private lowbit_sb_metal(t : T?; m : Model; row : CensusRow;
                            var kept : CensusKept) { // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let arm = row.arm
        let f = row.f
        let stem = kq_stem(f)
        let nplanes = sites_count(m, true, true, $(x : KqFmt) : bool => x == f)
        t |> success(nplanes > 0l, "{arm}: the dense planes load as {f} ({nplanes} planes)")
        metal_window_steps(t, m, row, kept) $ {
            let w = (metal_kernel_count_prefix("MetalKqMulMm{stem}") + metal_kernel_count_prefix("MetalKqDequant{stem}")
                + metal_kernel_count_prefix("MetalKqMvB2{stem}") + metal_kernel_count_prefix("MetalKqMvB4{stem}")
                + metal_kernel_count_prefix("MetalKqMvB8{stem}") + metal_kernel_count("metal_kq_gemv_{f}"))
            t |> success(w >= nplanes, ("{arm}: the window's {f} sites rode the {f} stamps " +
                "({w} dispatches, {nplanes} planes)"))
        }
        let gv = metal_kernel_count("metal_kq_gemv_{f}")
        t |> success(gv >= DECODE_STEPS * nplanes, ("{arm}: the steps read every {f} plane " +
            "through its GEMV ({gv} dispatches, {DECODE_STEPS * nplanes} planes)"))
    }
}

def private nv4_moe_metal(t : T?; m : Model; row : CensusRow;
                          var kept : CensusKept) {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let nexp = sites_count(m, false, true, $(x : KqFmt) : bool => x == KqFmt.nv4)
        t |> success(nexp >= 3l * m.config.n_layers,
            "census-moe-nv4: every routed stack loads as nv4 ({nexp} stacks)")
        metal_window_steps(t, m, row, kept) $ {
            let mm = (metal_kernel_count("metal_moe_mulmm_nv4")
                + metal_kernel_count("metal_moe_gemv_nv4")
                + routed_twin_dispatches(KqFmt.nv4, "metal_moe_mulmm_iq4xs_t", true))
            t |> success(mm >= nexp, ("census-moe-nv4: the window's nv4 expert stacks rode the " +
                "nv4 routed stamps ({mm} dispatches, {nexp} stacks)"))
        }
        let gv = metal_kernel_count("metal_moe_gemv_nv4")
        t |> success(gv >= DECODE_STEPS * nexp, ("census-moe-nv4: the steps read every nv4 " +
            "expert stack through the routed GEMV ({gv} dispatches, {DECODE_STEPS * nexp} stacks)"))
    }
}

//! the per-32 and mlx4b routed rows' Metal cells: the stacks' tags,
//! the window's and the steps' routed stamps; on the mlx4b snapshot
//! every gate, up and down stack native and the steps' routed form
def private per32_moe_metal(t : T?; m : Model; row : CensusRow;
                            var kept : CensusKept) {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let arm = row.arm
        let mlx = row.kind == CensusKind.mlx4b_moe
        let nexp = sites_count(m, false, true, $(x : KqFmt) : bool => x == row.f)
        let carrier = row.kind == CensusKind.lowbit_moe
        if (mlx) {
            t |> success(nexp >= 3l * m.config.n_layers,
                "{arm}: every routed stack loads as {row.f} ({nexp} stacks)")
        } elif (carrier) {
            t |> equal(nexp, row.stacks, "{arm}: the carrier's routed stacks that load as {row.f}")
        } else {
            t |> success(nexp >= m.config.n_layers,
                "{arm}: the routed stacks load as {row.f} ({nexp} stacks)")
        }
        let stem = "{row.f}"
        let cls = kq_stem(row.f)
        let tbody = routed_twin_body(row.f)
        //! a carrier's member may be one the box profile leaves uncrowned: its twin then never compiled
        let uncrowned = carrier && !metal_tensor_crowned("moe_mulmm_{stem}")
        metal_window_steps(t, m, row, kept) $ {
            let mm = (metal_kernel_count("metal_moe_mulmm_{stem}")
                + routed_gemv_dispatches(row.f, cls) + routed_twin_dispatches(row.f, tbody, uncrowned))
            t |> success(mm >= nexp, ("{arm}: the window's {stem} expert stacks rode the {stem} " +
                "routed stamps ({mm} dispatches, {nexp} stacks)"))
        }
        if (mlx) {
            routed_steps_form_cell(t, row.f, cls, arm, DECODE_STEPS, m.config.n_layers)
        }
        let gv = routed_gemv_dispatches(row.f, cls)
        t |> success(gv >= DECODE_STEPS * nexp, ("{arm}: the steps read every {stem} expert " +
            "stack through the routed GEMV ({gv} dispatches, {DECODE_STEPS * nexp} stacks)"))
    }
}

def private tied_f16_metal(t : T?; m : Model; row : CensusRow;
                           var kept : CensusKept) {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let kept_q8 = m.config.shared_weights && m.cls_q8_copy && cls_fmt_of(m) == KqFmt.q8
        t |> success(kept_q8 && !cls_tied_f32(m), ("the file's F16 table is tied and the load " +
            "kept a q8 copy for the device classifier (cls_q8_copy {m.cls_q8_copy})"))
        metal_window_steps(t, m, row, kept) $ {
            t |> equal(metal_kernel_count("metal_embed_q8"), 0l,
                "tied-f16: the window read no q8 table - its rows are the exact fp32 ones")
        }
    }
}

def private kind_metal(t : T?; m : Model; row : CensusRow; var kept : CensusKept) {
    if (row.kind == CensusKind.iq4xs_hybrid) {
        iq4xs_hybrid_metal(t, m, row, kept)
    } elif (row.kind == CensusKind.tied_kq) {
        tied_kq_metal(t, m, row, kept)
    } elif (row.kind == CensusKind.iq3s4_dense) {
        iq3s4_dense_metal(t, m, row, kept)
    } elif (row.kind == CensusKind.iq3s4_moe) {
        iq3s4_moe_metal(t, m, row, kept)
    } elif (row.kind == CensusKind.per32) {
        per32_metal(t, m, row, kept)
    } elif (row.kind == CensusKind.nv4) {
        nv4_metal(t, m, row, kept)
    } elif (row.kind == CensusKind.lowbit_sb) {
        lowbit_sb_metal(t, m, row, kept)
    } elif (row.kind == CensusKind.nv4_moe) {
        nv4_moe_metal(t, m, row, kept)
    } elif (row.kind == CensusKind.per32_moe || row.kind == CensusKind.mlx4b_moe || row.kind == CensusKind.lowbit_moe) {
        per32_moe_metal(t, m, row, kept)
    } else {
        tied_f16_metal(t, m, row, kept)
    }
}

//! the row's Metal cells on the blob twin (`metal_form(ctx = 512)`),
//! cut to `CENSUS_CELLS_CTX`: load tags, census, window and steps
[cold_path]
def format_census_metal(t : T?; var m : Model; form : ModelForm; row : CensusRow;
                        var kept : CensusKept) {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return if (!form.on_metal || !row_on(t, row))
        if (form.metal_image) {
            t |> failure("{row.file}: the census cells run on the blob twin form (`metal_form()`)")
            return
        }
        with_ctx_cut(m, CENSUS_CELLS_CTX) {
            if (row.blocks > 0l) {
                cut_within(t, row.cut_tag, m, row.blocks)
            }
            census_of(t, m, row, "blob twin")
            kind_metal(t, m, row, kept)
            if (kept.metal_ran) {
                crowned_window(t, m, row, kept)
            }
        }
    } else {
        t |> skip("dasMetal not built")
    }
}

//! the CPU form the row's controls wait for: the portable backend,
//! IQ3_S served as the row's pin
def census_cpu_form(row : CensusRow) : ModelForm =>
    cpu_cells_form(backend = "portable", kq_native = row.kq_native, iq3s_serve = row.iq3s)

//! the row's CPU cells on `census_cpu_form(row)` alone, cut to
//! `CENSUS_CELLS_CTX`: the census and the Metal stream's replay
[cold_path]
def format_census_cpu(t : T?; var m : Model; form : ModelForm; row : CensusRow;
                      var kept : CensusKept) {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return if (!alone_pass_of(form, census_cpu_form(row)) || !kept.metal_ran)
        with_ctx_cut(m, CENSUS_CELLS_CTX) {
            census_of(t, m, row, "planar")
            with_cpu_prefill(true) {   //! the CPU chain's prompt is the parity reference
                cpu_replay(m, kept)
            }
            kept.cpu_ran = true
        }
    }
}

//! `row`'s Metal census forms onto `forms`: the blob twin and, on a das_metal build, the row's CPU form
def format_census_forms(var forms : array<ModelForm>; row : CensusRow) {
    forms |> push(metal_form(ctx = CENSUS_CELLS_CTX))
    if (empty(metal_build_why())) {
        forms |> push(census_cpu_form(row))
    }
}

//! whether `form` is one of the Metal census's of `row`, its share run: the blob twin's census, window and steps, the row's
//! CPU form's replay
def format_census_pass(t : T?; var m : Model; form : ModelForm; row : CensusRow; var kept : CensusKept) : bool {
    if (form.on_metal) {
        format_census_metal(t, m, form, row, kept)
        return true
    }
    return false if (!alone_pass_of(form, census_cpu_form(row)))
    format_census_cpu(t, m, form, row, kept)
    return true
}

//! the window and every step against the CPU chain within the bar,
//! the CPU's next row (window) and previous row (steps) the controls
def private steps_verdict(t : T?; row : CensusRow; kept : CensusKept) {
    let tag = row.tag
    let bar = row.bar
    let pf_d = logits_maxdiff(kept.cpu_window, kept.gpu_window)
    to_log(LOG_INFO, ("{tag} window: CPU next '{piece_of(kept, kept.cpu_window)}' vs Metal next " +
        "'{piece_of(kept, kept.gpu_window)}' (maxd {pf_d})\n"))
    t |> success(pf_d <= bar,
        "{tag}: the window's last row within {bar} of the CPU chain's (maxd {pf_d})")
    if (!empty(kept.gpu_crowned)) {
        let cr_d = logits_maxdiff(kept.cpu_window, kept.gpu_crowned)
        to_log(LOG_INFO, "{tag} window under the low-bit tensor crowns: maxd {cr_d} of the CPU chain's\n")
        t |> success(cr_d <= bar,
            "{tag}: the crowned window's last row within {bar} of the CPU chain's (maxd {cr_d})")
    }
    var worst = 0.0
    var off_min = 1.0e30
    for (cs, gs, step in kept.cpu_steps, kept.gpu_steps, count()) {
        let d = logits_maxdiff(cs, gs)
        let off = (step == 0 ? logits_maxdiff(kept.cpu_window, gs)
            : logits_maxdiff(kept.cpu_steps[step - 1], gs))
        if (step == 0) {
            let ctl = logits_maxdiff(cs, kept.gpu_window)
            t |> success(ctl > bar, ("{tag}: the window compare's control: the CPU chain's next " +
                "row lands outside the bar of the window's ({ctl})"))
        }
        if (step == 0 && !empty(kept.gpu_crowned)) {
            let ctl = logits_maxdiff(cs, kept.gpu_crowned)
            t |> success(ctl > bar, ("{tag}: the crowned window compare's control: the CPU chain's next " +
                "row lands outside the bar of the crowned window's ({ctl})"))
        }
        let fed = eyeball_str(kept.pieces?[kept.fed[step]] ?? "")
        to_log(LOG_INFO, ("{tag} step {step}: fed '{fed}', CPU next '{piece_of(kept, cs)}' vs " +
            "Metal next '{piece_of(kept, gs)}' (maxd {d}, one step off {off})\n"))
        worst = max(worst, d)
        off_min = min(off_min, off)
    }
    t |> success(worst <= bar, "{tag}: every step within {bar} of the CPU chain's (worst {worst})")
    t |> success(off_min > bar, ("{tag}: the steps' control: the CPU's previous row lands " +
        "outside the bar at every step (nearest {off_min})"))
}

//! the row's compares after the block call, `kept` deleted
[cold_path]
def format_census_verdict(t : T?; row : CensusRow; var kept : CensusKept) {
    if (kept.metal_ran && !kept.cpu_ran) {
        t |> failure(("{row.file}: the Metal window and steps ran, the CPU chain's replay did " +
            "not (`census_cpu_form(row)`: backend portable, IQ3_S pin '{row.iq3s}')"))
    } elif (kept.metal_ran) {
        steps_verdict(t, row, kept)
    }
    delete kept
}

def private dense_row(file, fam, arm, emb : string; kind : CensusKind; hy : bool) : CensusRow {
    return CensusRow(file = file, fam = fam, arm = arm, tag = arm, emb = emb, bar = DENSE_KQ_BAR,
        kind = kind, hy = hy)
}

//! a llama carrier of low-bit superblock member `f`, its table gathered by `emb`; the CPU side the block's portable
//! K-quant-native form
def private lowbit_sb_row(file : string; f : KqFmt; emb : string) : CensusRow => CensusRow(
    file = file, fam = "llama", arm = "census-{f}", tag = "{f}", emb = emb,
    bar = DENSE_KQ_BAR, kind = CensusKind.lowbit_sb, f = f, kq_native = KqNative.on)

def private per32_moe_row(file, arm, emb : string; f : KqFmt) : CensusRow {
    return CensusRow(file = file, fam = "qwen2moe", arm = arm, tag = arm, emb = emb,
        bar = PLAIN_MOE_BAR, kind = CensusKind.per32_moe, f = f, blocks = 4l, cut_tag = "{file}: ")
}

//! the 35B-A3B 4L IQ4_XS-pure cut: every big plane IQ4_XS, untied
def format_census_iq4xs_pure() : CensusRow => CensusRow(
    file = "Qwen3.6-35B-A3B-4L-MTP-IQ4_XS-pure-local.gguf", fam = "qwen35moe", arm = "census-iq4xs",
    tag = "census", emb = "MetalEmbedIq4xs_metal_embed_kq", bar = HYBRID_MOE_BAR,
    kind = CensusKind.iq4xs_hybrid, blocks = 4l)

//! Llama-3.2-3B Q4_K_M: the gather on a tied Q6_K table
def format_census_tied_kq() : CensusRow => CensusRow(
    file = "Llama-3.2-3B-Instruct-Q4_K_M.gguf", fam = "llama", arm = "census-tied", tag = "tied",
    emb = "MetalEmbedK6_metal_embed_kq", bar = DENSE_KQ_BAR, kind = CensusKind.tied_kq)

//! Qwen2.5-1.5B IQ3_XS: IQ3_S beside IQ3_XXS, Q4_K, a tied Q6_K
def format_census_iq3s4_dense() : CensusRow => CensusRow(
    file = "Qwen2.5-1.5B-Instruct-IQ3_XS.gguf", fam = "qwen2", arm = "census-iq3s4", tag = "iq3s4",
    emb = "MetalEmbedK6_metal_embed_kq", bar = DENSE_KQ_BAR, kind = CensusKind.iq3s4_dense,
    iq3s = CENSUS_IQ3S_LUT)

//! the qwen35moe cut: IQ3_S routed downs beside IQ2_S gate and up
def format_census_iq3s4_moe() : CensusRow => CensusRow(
    file = "Qwen3.6-35B-A3B-4L-UD-IQ3_S-local.gguf", fam = "qwen35moe", arm = "census-iq3s4-moe",
    tag = "iq3s4 moe", emb = "MetalEmbedK6_metal_embed_kq", bar = HYBRID_MOE_BAR,
    kind = CensusKind.iq3s4_moe, blocks = 4l, iq3s = CENSUS_IQ3S_LUT)

def format_census_q51() : CensusRow => dense_row("Llama-3.2-1B-Instruct-Q5_1-local.gguf", "llama",
    "census-q51", "MetalEmbedK6_metal_embed_kq", CensusKind.per32, false)

def format_census_iq4nl32() : CensusRow => dense_row("Qwen2.5-0.5B-Instruct-IQ4_NL-local.gguf",
    "qwen2", "census-iq4nl32", "metal_embed_q8", CensusKind.per32, false)

def format_census_iq4nl32_table() : CensusRow => dense_row(
    "Qwen2.5-0.5B-Instruct-IQ4_NL-pure-local.gguf", "qwen2", "census-iq4nl32-table",
    "MetalEmbedB32Iq4nl32_metal_embed_b32", CensusKind.per32, false)

def format_census_q4032() : CensusRow => dense_row("Qwen2.5-0.5B-Instruct-Q4_0-local.gguf", "qwen2",
    "census-q4032", "MetalEmbedB32Q4032_metal_embed_b32", CensusKind.per32, false)

def format_census_q5032() : CensusRow => dense_row("Qwen2.5-0.5B-Instruct-Q5_0-pure-local.gguf",
    "qwen2", "census-q5032", "MetalEmbedB32Q5032_metal_embed_b32", CensusKind.per32, false)

//! Ternary-Bonsai 1.7B Q2_0: every matrix and the tied table q20
def format_census_q20() : CensusRow => dense_row("Ternary-Bonsai-1.7B-Q2_0_g64.gguf", "qwen3",
    "census-q20", "MetalEmbedB32Q20_metal_embed_b32", CensusKind.per32, false)

//! Bonsai 1.7B Q1_0: every matrix and the tied table q10
def format_census_q10() : CensusRow => dense_row("Bonsai-1.7B-Q1_0.gguf", "qwen3",
    "census-q10", "MetalEmbedB32Q10_metal_embed_b32", CensusKind.per32, false)

def format_census_q51_table() : CensusRow => dense_row("Qwen3.5-0.8B-MTP-Q5_1-local.gguf", "qwen35",
    "census-q51-table", "MetalEmbedB32Q51_metal_embed_b32", CensusKind.per32, true)

def format_census_nv4() : CensusRow => dense_row("Llama-3.2-1B-Instruct-NVFP4-local.gguf", "llama",
    "census-nv4", "MetalEmbedK6_metal_embed_kq", CensusKind.nv4, false)

def format_census_nv4_table() : CensusRow => dense_row("Llama-3.2-1B-Instruct-emb-NVFP4-local.gguf",
    "llama", "census-nv4-table", "MetalEmbedNv4_metal_embed_kq", CensusKind.nv4, false)

def format_census_nv4_hybrid() : CensusRow => dense_row("Qwen3.5-0.8B-MTP-NVFP4-pure-local.gguf",
    "qwen35", "census-nv4-hybrid", "MetalEmbedNv4_metal_embed_kq", CensusKind.nv4, true)

//! TriLM 1.1B as `f` (tq2, tq1): every dense plane the member's, an untied Q4_K table
def format_census_ternary(file : string; f : KqFmt) : CensusRow => lowbit_sb_row(file, f, "MetalEmbedK4_metal_embed_kq")

//! a Llama-3.2-1B 1-bit grid mint as `f` (iq1s, iq1m): 78 dense planes the member's, the rest K-quant and IQ2_XXS, the tied
//! Q5_K table gathered on the device
def format_census_iq1(file : string; f : KqFmt) : CensusRow => lowbit_sb_row(file, f, "MetalEmbedK5_metal_embed_kq")

//! the Qwen3-30B-A3B `--pure` NVFP4 mint's seven-block cut: the
//! 768-wide routed stacks, the table and the classifier as nv4
def format_census_nv4_moe() : CensusRow => CensusRow(
    file = "Qwen3-30B-A3B-7L-NVFP4-pure-local.gguf", fam = "qwen3moe", arm = "census-moe-nv4",
    tag = "census-moe-nv4", emb = "MetalEmbedNv4_metal_embed_kq", bar = NV4_MOE_BAR,
    kind = CensusKind.nv4_moe, blocks = 7l)

//! a low-bit member's routed carrier: the whole Qwen1.5-MoE, `f` on `stacks` routed stacks of three layers, a Q8_0 table;
//! the CPU chain reads those stacks off their own bytes
def format_census_moe_lowbit(file : string; f : KqFmt; stacks : int64) : CensusRow => CensusRow(file = file, fam = "qwen2moe",
    arm = "census-moe-{f}", tag = "census-moe-{f}", emb = "metal_embed_q8", bar = PLAIN_MOE_BAR,
    kind = CensusKind.lowbit_moe, f = f, kq_native = KqNative.on_q40, stacks = stacks)

def format_census_moe_q51() : CensusRow => per32_moe_row("Qwen1.5-MoE-A2.7B-4L-Q5_1-local.gguf",
    "census-moe-q51", "MetalEmbedB32Q51_metal_embed_b32", KqFmt.q51)

def format_census_moe_q4032() : CensusRow => per32_moe_row("Qwen1.5-MoE-A2.7B-4L-Q4_0-local.gguf",
    "census-moe-q4032", "MetalEmbedQ40_metal_embed_kq", KqFmt.q4032)

def format_census_moe_q5032() : CensusRow => per32_moe_row(
    "Qwen1.5-MoE-A2.7B-4L-Q5_0-pure-local.gguf", "census-moe-q5032",
    "MetalEmbedB32Q5032_metal_embed_b32", KqFmt.q5032)

//! the canary decoder: a qwen3 1.7B, every plane and tied table F16
def format_census_tied_f16() : CensusRow => CensusRow(
    file = "canary-qwen-2.5b-decoder-f16.gguf", fam = "qwen3", arm = "census-f16", tag = "tied-f16",
    emb = "metal_row_gather", bar = TIED_F16_BAR, kind = CensusKind.tied_f16)

//! the mlx-community Qwen3.6-35B-A3B 4-bit snapshot (large tier):
//! every site but the router in its own mlx4b member, the routed
//! stacks among them
def format_census_mlx4b_moe() : CensusRow => CensusRow(
    file = "mlx-community/Qwen3.6-35B-A3B-4bit", snapshot = true, fam = "qwen35moe",
    arm = "census-moe-mlx4b", tag = "census-moe-mlx4b", emb = "MetalEmbedB32Mlx4b_metal_embed_b32",
    bar = MLX_MOE_BAR, kind = CensusKind.mlx4b_moe, f = KqFmt.mlx4b)
