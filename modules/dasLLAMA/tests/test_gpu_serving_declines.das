options gen2
options stack = 524288   // three Model shells live in one cell's frame - the default test stack overflows on them
options _dasllama_internal = true
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require dasllama/dasllama_common
require _override_probes   // decode_declines: the fake override's step
require dasllama/dasllama_sampling
require dasllama/dasllama_gpu_resident
require dasllama/dasllama_gpu_tier   // install_moe_gpu_resident_bias - the bias seat a served qwen2 needs
require dasllama/dasllama_env   // set_pf_window_rows_pin - the ring cells' 512-row window
require ?vulkan dasllama/dasllama_vulkan_decode   // rd_routed_slots_fit - the N-row command's slot cap, where the module exists
require ?vulkan dasllama/dasllama_vulkan_prefill  // ple_proj_gemv_why - the window projection's tile gate, where the module exists
require dasllama/dasllama_gpu_kernels_common  // gk_bf16_pairs - the bf16 projection's stamp pick on both homes
require ?vulkan dasllama/dasllama_vulkan_classes   // ple_proj_bf16_hw - the resident projection's halfword pick, where the module exists
require ?vulkan dasllama/dasllama_vulkan_common   // GpuState / g_gpu - the device shell ple_proj_gemv_why reads, where the module exists
require ?vulkan vulkan/vulkan_boost   // subgroup_properties / compute_subgroup_size_pinnable - the pin cell's own reading of the device
require ?vulkan dasllama/dasllama_vulkan_tower   // vulkan_tower_front_route - the fronts' route asked before any device is up, where the module exists
require dasllama/dasllama_metal_shapes   // moe_metal_ok - the Metal routed block's admission, portable
require ?das_metal dasllama/dasllama_metal_common   // decline - the required-mode panic, where the module exists
require ?das_metal dasllama/dasllama_metal_decode   // the metal decode override `set_metal_mode` selects
require ?das_metal dasllama/dasllama_metal_prefill   // the metal prefill override beside it
require dasllama/dasllama_math   // kernel_backend_seat / restore_kernel_backend - the backend pin the metal mode sets, put back
require dasllama/dasllama_image   // save_model_image / load_model_image / image_identity - the trimmed lane's map under the ring pin
require dasllama/dasllama_arch_llama   // the arch the trimmed lane's shell names, registered for its map
require daslib/fio
require strings
require daslib/strings_boost
require daslib/enum_trait   // the format-set drift guard walks the whole KqFmt enum

[unused_argument(rows)]
def private test_upload_bias(rows : array<float>) {}

[unused_argument(rows)]
def private test_upload_sinks(rows : array<float>) : bool => true

// The whole-model driver's decline reasons, decided from a Config or a synthetic Model shell
// (no weights on disk): every feature and layer shape the driver has no arm for is named in
// the text a user reads, and a shape it serves yields no text at all. The speculative round's seat
// ownership rides the same shells: a decode override's seats take a round only on a model its owner claims.

def private shell(n_layers : int64; recr_mask : uint64) : Model {
    var m = Model()
    m.config.n_layers = n_layers
    m.config.recr_mask = recr_mask
    m.config.hidden_dim = 64l
    m.config.head_size = 32l
    m.config.n_heads = 2l
    m.config.n_kv_heads = 1l
    m.kv_src <- [for (l in range64(n_layers)); l]
    m.wq_offs |> resize(n_layers)
    m.wk_offs |> resize(n_layers)
    m.wv_offs |> resize(n_layers)
    return <- m
}

[test]
def test_resident_unserved_features(t : T?) {
    t |> run("a plain dense config has no unserved feature") @(t : T?) {
        let c = Config()
        t |> equal(resident_unserved_features(c), "")
    }
    t |> run("the qwen35 shape is served whole (nextn block, gated q, partial rope over qk-norm); without qk-norm the two attention arms are named") @(t : T?) {
        var c = Config()
        c.q_gated = true
        c.qk_norm = true
        c.n_layer_nextn = 1l
        c.head_size = 256l
        c.rope_dim = 64l
        t |> equal(resident_unserved_features(c), "")
        c.qk_norm = false
        let s = resident_unserved_features(c)
        t |> success(contains(s, "gated attention without qk-norm"), "gated attention named: {s}")
        t |> success(contains(s, "partial rotary (64 of 256) without qk-norm"), "partial rotary named with its widths: {s}")
        t |> equal(length(split(s, ", ")), 2)
    }
    t |> run("one feature alone is one name, no separator") @(t : T?) {
        var c = Config()
        c.q_gated = true
        t |> equal(resident_unserved_features(c), "gated attention without qk-norm")
    }
    t |> run("the attention sinks are named only while no sink seat is installed; a seat serves them at head size 64 alone, and the output bias is served") @(t : T?) {
        var c = Config()
        c.attn_sinks = true
        c.head_size = 64l
        let saved_seat = g_rdec_upload_sinks
        let saved_installed = g_rdec_sinks_installed
        g_rdec_sinks_installed = false
        t |> equal(resident_unserved_features(c), "attention sinks")
        install_moe_gpu_resident_sinks(@@test_upload_sinks)
        t |> equal(resident_unserved_features(c), "", "gpt-oss's sinks are served with the seat installed")
        c.attn_out_bias = true
        t |> equal(resident_unserved_features(c), "", "the attention output bias rides the norms plane into the residual step")
        c.head_size = 128l
        t |> equal(resident_unserved_features(c), "attention sinks at head size 128 (the sink stamps serve 64)")
        g_rdec_upload_sinks = saved_seat
        g_rdec_sinks_installed = saved_installed
    }
    t |> run("the q/k/v projection bias is named only while no bias seat is installed; a seat serves the qwen2 shape") @(t : T?) {
        var c = Config()
        c.attn_qkv_bias = true
        c.rope_neox = true
        let saved_seat = g_rdec_upload_bias
        let saved_installed = g_rdec_bias_installed
        g_rdec_bias_installed = false
        t |> equal(resident_unserved_features(c), "attention qkv bias")
        install_moe_gpu_resident_bias(@@test_upload_bias)
        t |> equal(resident_unserved_features(c), "")
        t |> equal(attn_chain_unserved_features(c), "", "the per-op chain carries the bias through its hooks - no seat to install")
        g_rdec_upload_bias = saved_seat
        g_rdec_bias_installed = saved_installed
    }
}

[test]
def test_attn_chain_unserved_features(t : T?) {
    t |> run("interleaved rope is the one thing a plain config lacks; NEOX or partial rope serves") @(t : T?) {
        var c = Config()
        t |> equal(attn_chain_unserved_features(c), "interleaved (non-NEOX) rope")
        c.rope_neox = true
        t |> equal(attn_chain_unserved_features(c), "")
        c.attn_qkv_bias = true
        t |> equal(attn_chain_unserved_features(c), "", "the q/k/v projection bias is served (qwen2moe)")
        c.attn_out_bias = true
        t |> equal(attn_chain_unserved_features(c), "attention output bias")
        var p = Config()
        p.head_size = 256l
        p.rope_dim = 64l
        t |> equal(attn_chain_unserved_features(p), "")
    }
    t |> run("the chain names every gemma generation's attention as unserved: the sandwich norms, gemma-2's softcap, gemma-4's v-norm") @(t : T?) {
        var g3 = Config()
        g3.rope_neox = true
        g3.qk_norm = true
        g3.pre_post_norm = true
        t |> equal(attn_chain_unserved_features(g3), "pre/post norms", "gemma-3")
        var g2 := g3
        g2.qk_norm = false
        g2.attn_logit_softcap = 50.0
        t |> equal(attn_chain_unserved_features(g2), "attention logit softcap, pre/post norms", "gemma-2")
        var g4 := g3
        g4.v_norm = true
        t |> equal(attn_chain_unserved_features(g4), "v-norm, pre/post norms", "gemma-4")
    }
}

//! a three-layer shell whose layer 1 is recurrent with every deltanet plane at offset 0, its qkv/z/out planes Q4_K
def private dn_shell() : Model {
    var m <- shell(3l, 2ul)
    m.config.ssm_d_inner = 256l
    m.dnqkv_offs <- [-1l, 0l, -1l]
    m.dngate_offs <- [-1l, 0l, -1l]
    m.dnout_offs <- [-1l, 0l, -1l]
    m.dnbeta_offs <- [-1l, 0l, -1l]
    m.dnalpha_offs <- [-1l, 0l, -1l]
    m.dnqkv_fmt <- [KqFmt.q8, KqFmt.k4, KqFmt.q8]
    m.dngate_fmt <- [KqFmt.q8, KqFmt.k4, KqFmt.q8]
    m.dnout_fmt <- [KqFmt.q8, KqFmt.k4, KqFmt.q8]
    m.dnbeta_fmt <- [KqFmt.q8, KqFmt.q8, KqFmt.q8]
    m.dnalpha_fmt <- [KqFmt.q8, KqFmt.q8, KqFmt.q8]
    return <- m
}

[test]
def test_dn_plane_format_declines(t : T?) {
    t |> run("the whole-model driver reads q8, superblock and per-32 beta/alpha planes by tag and names an mx4 one") @(t : T?) {
        var m <- dn_shell()
        t |> success(!contains(resident_layer_decline(m, 1l), "beta/alpha planes carry"), "q8 beta/alpha planes draw no format decline")
        m.dnbeta_fmt[1] = KqFmt.iq4xs
        m.dnalpha_fmt[1] = KqFmt.k4
        let sk = resident_layer_decline(m, 1l)
        t |> success(!contains(sk, "beta/alpha planes carry"), "K-quant beta/alpha planes draw no format decline - the resident GEMVs and tiles read them by tag: '{sk}'")
        for (f in [KqFmt.q51, KqFmt.iq4nl32, KqFmt.q4032, KqFmt.q5032]) {
            m.dnbeta_fmt[1] = f
            m.dnalpha_fmt[1] = f
            let sp = resident_layer_decline(m, 1l)
            t |> success(!contains(sp, "beta/alpha planes carry"), "{f} beta/alpha planes draw no format decline - the per-32 GEMVs and tiles read them: '{sp}'")
        }
        m.dnbeta_fmt[1] = KqFmt.mx4
        t |> success(contains(resident_layer_decline(m, 1l), "layer 1's beta/alpha planes carry a format"), "an mx4 beta plane is named: no resident GEMV or tile reads it (the control)")
        delete m
    }
    t |> run("the per-op deltanet chain takes superblock and per-32 planes by tag and qkv/z planes of two classes; an mx4 plane and a missing triple are named") @(t : T?) {
        var m <- dn_shell()
        t |> equal(perop_dn_decline(m, 1l), "", "the per-op deltanet chain takes K-quant qkv/z/out planes by tag")
        m.dngate_fmt[1] = KqFmt.q8
        t |> equal(perop_dn_decline(m, 1l), "", "a K-quant qkv beside a q8 z rides the chain: the z plane reads its own image")
        m.dngate_fmt[1] = KqFmt.k4
        for (f in [KqFmt.q51, KqFmt.iq4nl32, KqFmt.q4032, KqFmt.q5032]) {
            m.dnout_fmt[1] = f
            t |> success(!contains(perop_dn_decline(m, 1l), "outside the chain"), "a {f} out plane rides the chain's per-32 GEMVs")
        }
        m.dnout_fmt[1] = KqFmt.mx4
        t |> success(contains(perop_dn_decline(m, 1l), "outside the chain"), "an mx4 out plane is named (the control)")
        t |> success(contains(perop_dn_decline(m, 0l), "no deltanet planes"), "a layer without the triple is named")
        delete m
    }
}

[test]
def test_perop_dense_ffn_mixed_forms(t : T?) {
    t |> run("the per-op FFN rail takes a gate and up of two activation forms - each GEMM over its own image; an unservable plane is named") @(t : T?) {
        var m = Model()
        m.config.dim = 256l
        m.config.hidden_dim = 512l
        m.kq_repacked = true
        m.w1_offs <- [0l]
        m.w3_offs <- [0l]
        m.w2_offs <- [0l]
        m.w1_fmt <- [KqFmt.q51]
        m.w3_fmt <- [KqFmt.k4]
        m.w2_fmt <- [KqFmt.k6]
        t |> equal(perop_dense_ffn_decline(m, 0l), "", "a per-32 gate beside a K-quant up rides the rail")
        m.w1_fmt[0] = KqFmt.k4
        t |> equal(perop_dense_ffn_decline(m, 0l), "", "one activation form rides the rail")
        m.w3_fmt[0] = KqFmt.mx4
        t |> equal(perop_dense_ffn_decline(m, 0l), "FFN gate/up/down formats k4/mx4/k6 not tier-servable", "an mx4 up plane is named by site and member (the control)")
        delete m
    }
}

//! the tied classifier on a table no device plane carries: an F16 table (no q8 tag, no K-quant plane, no Metal-mode copy) is named,
//! where the resident plan would seat a q8 plane at an offset that holds no bytes; a Q8_0, a K-quant and an untied table pass
[test]
def test_resident_cls_decline(t : T?) {
    t |> run("the tied classifier of an F32/F16/BF16 token table declines the whole-model driver by name") @(t : T?) {
        var m = Model()
        m.config.shared_weights = true
        t |> success(contains(resident_cls_decline(m), "tied classifier"), "a tied table with no classifier plane is named")
        m.cls_q8 = true
        t |> equal(resident_cls_decline(m), "", "a tied Q8_0 table's plane serves (the control)")
        m.cls_q8 = false
        m.cls_q8_copy = true
        t |> equal(resident_cls_decline(m), "", "a tied F16 table whose load kept the classifier's q8 copy serves off the copy")
        m.cls_q8_copy = false
        m.kquant_native = true
        m.emb_fmt = KqFmt.k6
        t |> equal(resident_cls_decline(m), "", "a tied K-quant table's plane serves")
        m.emb_fmt = KqFmt.q8
        m.config.shared_weights = false
        t |> equal(resident_cls_decline(m), "", "an untied classifier carries its own plane")
        delete m
    }
}

[test]
def test_resident_layer_decline(t : T?) {
    t |> run("a recurrent layer without deltanet planes is named by number; the attention layers around it pass") @(t : T?) {
        var m <- shell(3l, 2ul)
        t |> equal(resident_layer_decline(m, 0l), "")
        t |> success(contains(resident_layer_decline(m, 1l), "layer 1 lacks a deltanet plane"), "layer 1 named")
        t |> equal(resident_layer_decline(m, 2l), "")
        m.dnqkv_offs <- [-1l, 0l, -1l]
        m.dngate_offs <- [-1l, 0l, -1l]
        m.dnout_offs <- [-1l, 0l, -1l]
        m.dnbeta_offs <- [-1l, 0l, -1l]
        m.dnalpha_offs <- [-1l, 0l, -1l]
        // a K-quant qkv/z pair (same activation class) is served by the resident GEMVs, so the
        // format is no decline - the reason, if any, is downstream (the seat install, the FFN width)
        m.dnqkv_fmt <- [KqFmt.q8, KqFmt.k4, KqFmt.q8]
        m.dngate_fmt <- [KqFmt.q8, KqFmt.k4, KqFmt.q8]
        m.dnout_fmt <- [KqFmt.q8, KqFmt.q8, KqFmt.q8]
        let sq = resident_layer_decline(m, 1l)
        t |> success(!contains(sq, "kq-tagged") && !contains(sq, "do not serve"), "a K-quant qkv/z pair is no format decline: '{sq}'")
        m.dnout_fmt <- [KqFmt.q8, KqFmt.k4, KqFmt.q8]
        let so = resident_layer_decline(m, 1l)
        t |> success(!contains(so, "out plane"), "a K-quant out plane is no format decline: '{so}'")
        //! an inner width off the format's block leaves the out plane untileable - the decline names it
        m.config.ssm_d_inner = 100l
        t |> success(contains(resident_layer_decline(m, 1l), "layer 1's deltanet out plane carries a format"), "an untileable out plane is named")
        delete m
    }
    t |> run("shared K/V, a missing plane and a head size off the kernels' shape each name the layer; a differing FFN width or a sliding window is served") @(t : T?) {
        var m <- shell(3l, 0ul)
        m.kv_src[2] = 1l
        t |> equal(resident_layer_decline(m, 2l), "", "a layer sharing the K/V of a layer below it is served (Q only, the donor's rows)")
        m.kv_src[1] = 2l
        t |> success(contains(resident_layer_decline(m, 1l), "layer 1 shares layer 2's K/V, a layer above it"), "a donor above the sharing layer is named")
        m.kv_src[1] = 1l
        m.kv_src[2] = 2l
        m.wq_offs[0] = -1l
        t |> success(contains(resident_layer_decline(m, 0l), "layer 0 lacks a q/k plane"), "missing plane named")
        m.wq_offs[0] = 0l
        m.wv_offs[0] = -1l
        t |> success(contains(resident_layer_decline(m, 0l), "layer 0 lacks a v plane"), "a missing v plane is named where the layer is not V-from-K")
        m.config.v_from_k_mask = 1ul
        t |> equal(resident_layer_decline(m, 0l), "", "a V-from-K layer needs no v plane: V is its k row at its own rms")
        m.ffn_w <- [64l, 96l, 64l]
        t |> equal(resident_layer_decline(m, 1l), "", "a layer's own dense FFN width is its registration's, not a decline")
        m.config.sliding_window = 128l
        m.config.swa_pattern = 2l
        t |> equal(resident_layer_decline(m, 1l), "", "a sliding layer is served (its window rides the attention push)")
        m.config.head_size = 512l
        t |> equal(resident_layer_decline(m, 1l), "", "a 512 head is served (two elements a thread)")
        m.config.head_size = 544l
        t |> success(contains(resident_layer_decline(m, 1l), "layer 1 head size 544"), "a head past the kernels' 512 is named by layer")
        m.config.head_size = 40l
        t |> success(contains(resident_layer_decline(m, 1l), "layer 1 head size 40"), "a head off the 32-multiple is named by layer")
        delete m
    }
    t |> run("a MoE layer's dense width is its shared expert's; a layer past the dense lead without expert planes is named") @(t : T?) {
        var m <- shell(3l, 0ul)
        m.config.n_expert = 8l
        m.config.n_expert_used = 2l
        m.config.n_ff_exp = 256l
        m.config.n_ff_shexp = 512l
        m.config.n_layer_dense_lead = 1l
        t |> success(!layer_is_moe(m, 1l), "no expert offsets at all: no layer routes")
        m.we1_offs <- [-1l, 0l, 0l]
        m.we3_offs <- [-1l, 0l, 0l]
        m.we2_offs <- [-1l, 0l, -1l]
        t |> success(!layer_is_moe(m, 0l) && layer_is_moe(m, 1l) && !layer_is_moe(m, 2l), "a layer routes only past the dense lead with all three expert planes")
        t |> equal(resident_dense_width(m, 0l), 64l, "the dense-lead layer's width is its FFN's")
        t |> equal(resident_dense_width(m, 1l), 512l, "an MoE layer's width is the shared expert's")
        t |> success(contains(resident_layer_decline(m, 2l), "layer 2 lacks an expert plane"), "a layer past the dense lead without its expert planes is named")
        t |> equal(resident_layer_decline(m, 0l), "", "the dense-lead layer's own width is its registration's, whatever the shared expert's")
        delete m
    }
}

[test]
def test_resident_unserved_moe_features(t : T?) {
    t |> run("the routed block's reach: a softmax router with plain expert stacks is served, every other router shape is named") @(t : T?) {
        var c = Config()
        c.n_expert = 60l
        c.n_expert_used = 4l
        t |> equal(resident_unserved_features(c), "", "a plain softmax MoE names nothing")
        c.moe_gate = MoeGate.sigmoid
        t |> equal(resident_unserved_features(c), "a sigmoid router", "the router shape the kernels do not serve is named by its form")
        c.moe_gate = MoeGate.softmax_weight
        t |> equal(resident_unserved_features(c), "", "gpt-oss's softmax over the picks is served (the renormalized select is the same weights)")
        c.moe_gate = MoeGate.softmax
        c.moe_router_bias = true
        c.moe_exp_probs = true
        let s = resident_unserved_features(c)
        t |> success(!contains(s, "router bias") && contains(s, "an expert selection bias"), "the router bias is served (the select adds it), the expert selection bias named: {s}")
        c.moe_router_bias = false
        c.moe_exp_probs = false
        c.n_expert = 512l
        c.n_expert_used = 10l
        t |> equal(resident_unserved_features(c), "", "512 experts at ten picks (Qwen3-Next, Qwen3.5-397B) are inside the select's reach: the driver admits them")
        c.n_expert = 513l
        c.n_expert_used = 70l
        let g = resident_unserved_features(c)
        t |> success(contains(g, "513 experts") && contains(g, "70 routed experts"), "the kernels' reach named with the counts, one expert past it the control: {g}")
        c.n_expert = 60l
        c.n_expert_used = 4l
        c.moe_exps_bias = true
        c.ffn_act = FfnAct.swiglu_oai
        c.layer_out_scale = true
        t |> equal(resident_unserved_features(c), "", "gpt-oss's biased expert stacks, its clamped swiglu and the layer output scale are all served")
        c.moe_exps_bias = false
        c.ffn_act = FfnAct.gelu
        c.moe_dense_shexp = true
        t |> equal(resident_unserved_features(c), rdec_moe_installed() ? "" : "dense shared expert",
            "gemma-4's parallel dense shared expert is served where a backend installed the MoE seats, named where none did (installed here: {rdec_moe_installed()})")
        var d = Config()
        d.moe_gate = MoeGate.sigmoid
        t |> equal(resident_unserved_features(d), "", "a dense model's router field is no feature")
        var p = Config()
        p.n_embd_per_layer = 256l
        t |> equal(resident_unserved_features(p), rdec_ple_installed() ? "" : "per-layer embeddings",
            "the E-series per-layer embeddings are served where a backend installed the ple seats, named where none did (installed here: {rdec_ple_installed()})")
    }
}

// a routed shell the Metal admission accepts: dim and expert width on the 256 lattice, offsets
// on every plane's alignment, softmax router, silu experts, no shared expert
def private moe_shell(fmt : KqFmt) : Model {
    var m <- shell(2l, 0ul)
    m.config.dim = 256l
    m.config.moe = true
    m.config.n_expert = 8l
    m.config.n_expert_used = 2l
    m.config.n_ff_exp = 256l
    m.config.norm_topk_prob = true
    m.router_off = 0l
    m.we1_offs <- [0l, 0l]
    m.we2_offs <- [0l, 0l]
    m.we3_offs <- [0l, 0l]
    m.we1_fmt <- [fmt, fmt]
    m.we2_fmt <- [fmt, fmt]
    m.we3_fmt <- [fmt, fmt]
    return <- m
}

//! the Metal prefill's one-row GEMV predicate (`kq_fmt_one_row_gemv`): q8 at any width, a served superblock format at a
//! whole-superblock width and not at a partial one, a per-32 plane at whole 32-blocks, and an mx4 plane at no width
[test]
def test_kq_fmt_one_row_gemv(t : T?) {
    t |> run("the last-layer narrowing admits q8 at any width, a served superblock format at whole superblocks and a per-32 plane at whole blocks") @(t : T?) {
        t |> success(kq_fmt_one_row_gemv(KqFmt.q8, 100l), "q8 has a one-row GEMV at any width")
        t |> success(kq_fmt_one_row_gemv(KqFmt.k4, 8192l), "a served K-quant at whole superblocks")
        t |> success(!kq_fmt_one_row_gemv(KqFmt.k4, 8191l), "... and not at a partial superblock")
        t |> success(kq_fmt_one_row_gemv(KqFmt.iq3s4, 8192l), "the lut-form IQ3_S rides the iq4 family's form")
        t |> success(!kq_fmt_one_row_gemv(KqFmt.mx4, 8192l), "an mx4 plane has none")
        t |> success(kq_fmt_one_row_gemv(KqFmt.q51, 896l), "a per-32 Q5_1 plane has its GEMV at whole 32-blocks")
        t |> success(kq_fmt_one_row_gemv(KqFmt.iq4nl32, 896l), "... and the per-32 IQ4_NL plane")
        t |> success(kq_fmt_one_row_gemv(KqFmt.q4032, 896l), "... and the per-32 Q4_0 plane off the 256 lattice")
        t |> success(kq_fmt_one_row_gemv(KqFmt.q5032, 896l), "... and the per-32 Q5_0 plane")
        t |> success(!kq_fmt_one_row_gemv(KqFmt.q51, 900l), "... and not at a partial block")
        t |> success(kq_fmt_one_row_gemv(KqFmt.q40, 8192l), "Q4_0 repacks to whole superblocks, so it has the form")
    }
}

//! the Metal deltanet gates over every recurrent plane's tag: `dn_metal_ok` admits a plane with a one-row GEMV at its
//! width, `dn_ba_metal_window_ok` a beta/alpha plane with the expert-form GEMV the window's rows ride
[test]
def test_dn_metal_plane_formats(t : T?) {
    t |> run("the deltanet gates read each recurrent plane by its tag") @(t : T?) {
        var m <- shell(3l, 2ul)
        m.config.dim = 256l
        m.config.ssm_d_inner = 256l
        m.config.ssm_d_state = 128l
        m.config.ssm_d_conv = 4l
        t |> success(dn_metal_ok(m) && dn_ba_metal_window_ok(m), "untagged (q8) planes serve")
        m.dnqkv_fmt <- [KqFmt.q8, KqFmt.iq4xs, KqFmt.q8]
        m.dngate_fmt <- [KqFmt.q8, KqFmt.iq4xs, KqFmt.q8]
        m.dnout_fmt <- [KqFmt.q8, KqFmt.iq4xs, KqFmt.q8]
        m.dnbeta_fmt <- [KqFmt.q8, KqFmt.iq4xs, KqFmt.q8]
        m.dnalpha_fmt <- [KqFmt.q8, KqFmt.iq4xs, KqFmt.q8]
        t |> success(dn_metal_ok(m) && dn_ba_metal_window_ok(m), "IQ4_XS planes serve on both")
        m.dnbeta_fmt[1] = KqFmt.k2
        t |> success(dn_metal_ok(m), "a Q2_K beta plane has the one-row GEMV the step reads")
        t |> success(dn_ba_metal_window_ok(m), "... and the expert-form GEMV the window's rows read")
        m.dnbeta_fmt[1] = KqFmt.iq3s4
        t |> success(dn_metal_ok(m) && dn_ba_metal_window_ok(m), "a lut-form IQ3_S beta plane has both forms")
        for (f in [KqFmt.q51, KqFmt.iq4nl32, KqFmt.q4032, KqFmt.q5032]) {
            m.dnbeta_fmt[1] = f
            m.dnalpha_fmt[1] = f
            m.dnout_fmt[1] = f
            t |> success(dn_metal_ok(m) && dn_ba_metal_window_ok(m), "{f} beta/alpha/out planes have the per-32 GEMV and expert form")
        }
        m.dnalpha_fmt[1] = KqFmt.iq4xs
        m.dnout_fmt[1] = KqFmt.iq4xs
        m.dnbeta_fmt[1] = KqFmt.mx4
        t |> success(!dn_ba_metal_window_ok(m), "an mx4-tagged beta plane has no expert-form GEMV for the window's rows")
        m.dnbeta_fmt[1] = KqFmt.iq4xs
        m.dnout_fmt[1] = KqFmt.mx4
        t |> success(!dn_metal_ok(m), "an out plane in a format the Metal kernels lack declines")
        m.dnout_fmt[1] = KqFmt.iq4xs
        m.config.ssm_d_inner = 200l
        t |> success(!dn_metal_ok(m), "an out plane whose width is no whole superblock declines")
        delete m
    }
}

[test]
def test_moe_metal_expert_formats(t : T?) {
    t |> run("the Metal routed block admits exactly the expert plane formats it has kernels for; every other format declines") @(t : T?) {
        for (f in [KqFmt.q8, KqFmt.q51, KqFmt.iq4nl32, KqFmt.q4032, KqFmt.q5032, KqFmt.k4, KqFmt.k5, KqFmt.k6, KqFmt.k3, KqFmt.k2, KqFmt.q40, KqFmt.nv4, KqFmt.iq4xs, KqFmt.iq4nl, KqFmt.iq3s, KqFmt.iq3xxs, KqFmt.iq2s, KqFmt.iq2xs, KqFmt.iq2xxs, KqFmt.iq3s4]) {
            var m <- moe_shell(f)
            t |> success(moe_metal_ok(m), "{f} expert planes are served")
            delete m
        }
        var mx <- moe_shell(KqFmt.mx4)
        t |> success(!moe_metal_ok(mx), "an mx4 expert plane tag declines the routed block (the mx4 stacks key off `experts_mx4`)")
        delete mx
        var mixed <- moe_shell(KqFmt.k4)
        mixed.we2_fmt[1] = KqFmt.mx4
        t |> success(!moe_metal_ok(mixed), "one mx4-tagged down plane declines a q4_K model")
        mixed.we2_fmt[1] = KqFmt.iq3s4
        t |> success(moe_metal_ok(mixed), "a lut-form IQ3_S down plane beside q4_K stacks is served")
        delete mixed
    }
    t |> run("the native-MXFP4 stacks serve under the plain routed gate as under gpt-oss's; only the repacked planes decline") @(t : T?) {
        var m <- moe_shell(KqFmt.q8)
        m.experts_mx4 = true
        t |> success(moe_metal_ok(m), "mx4 stacks under a softmax / silu gate are served")
        m.mx4_repacked = true
        t |> success(!moe_metal_ok(m), "the CPU laneq repack has no GPU carry")
        delete m
        var g <- moe_shell(KqFmt.q8)
        g.config.moe_gate = MoeGate.softmax_weight
        t |> success(!moe_metal_ok(g), "gpt-oss's gate over q8 expert stacks has no biased Metal arm")
        delete g
    }
    t |> run("the NextN head's routed stacks are walked like a trunk layer's") @(t : T?) {
        var m <- moe_shell(KqFmt.k4)
        m.config.n_layer_nextn = 1l
        m.we1_offs |> push(0l)
        m.we2_offs |> push(0l)
        m.we3_offs |> push(0l)
        m.we1_fmt |> push(KqFmt.k4)
        m.we2_fmt |> push(KqFmt.k4)
        m.we3_fmt |> push(KqFmt.k4)
        t |> success(moe_metal_ok(m), "a head stack in a served format is admitted")
        m.we2_fmt[2] = KqFmt.mx4
        t |> success(!moe_metal_ok(m), "a head down stack the routed kernels lack declines the routed block")
        delete m
    }
    t |> run("the resident driver names a biased expert stack feeding a K-quant down plane by layer; a per-32 down plane draws no such decline") @(t : T?) {
        var kb <- moe_shell(KqFmt.k6)
        kb.config.moe_exps_bias = true
        t |> success(contains(resident_layer_decline(kb, 0l), "layer 0's biased expert stacks feed a K-quant down plane"), "the biased K-quant down is named")
        delete kb
        var qb <- moe_shell(KqFmt.q8)
        qb.config.moe_exps_bias = true
        t |> success(!contains(resident_layer_decline(qb, 0l), "biased expert stacks"), "a biased stack over a Q8_0 down plane draws no bias decline")
        delete qb
    }
    t |> run("the kernel roster and the site-alignment gate name ONE format set - every KqFmt reads the same on both") @(t : T?) {
        // No format the site gate admits is refused by the roster: the two lists are identical,
        // so a roster that answered true for everything is caught only by walking the WHOLE enum -
        // mx4 is the member that must read false on both. A format added to one
        // list and not the other reds here.
        for (f in type<KqFmt>) {
            t |> equal(moe_fmt_metal_served(f), moe_site_ok(f, 0l, 512l), "{f}: the roster and the site gate agree")
        }
    }
}

[test]
def test_moe_metal_expert_count(t : T?) {
    t |> run("the select stamps' ladder reaches 512 experts and no further") @(t : T?) {
        var m <- moe_shell(KqFmt.k4)
        m.config.n_expert = 256l
        t |> success(moe_metal_ok(m), "256 experts ride the 8-deep select stamp")
        m.config.n_expert = 512l
        t |> success(moe_metal_ok(m), "512 experts ride enc_moe_select_512's 16-deep ladder")
        m.config.n_expert = 513l
        t |> success(!moe_metal_ok(m), "513 experts: no select stamp holds that many logits per lane, so the routed block declines")
        m.config.n_expert = 512l
        t |> success(moe_metal_ok(m), "and the count alone decided it - nothing else about the shell moved")
        delete m
    }
}

[test]
def test_moe_metal_expert_weights_scale(t : T?) {
    t |> run("a global expert_weights_scale serves on Metal: the select carries it") @(t : T?) {
        var m <- moe_shell(KqFmt.k4)
        m.config.expert_weights_scale = 2.5
        t |> success(moe_metal_ok(m), "a scale of 2.5 serves (the select's args carry it)")
        m.config.n_expert = 513l
        t |> success(!moe_metal_ok(m), "the control: the same shell past the select stamps' reach still declines")
        delete m
    }
}

[test]
def test_decode_shape_dense_width(t : T?) {
    t |> run("the decode shape gate reads the dense FFN width only where a dense FFN dispatches") @(t : T?) {
        let saved_rope = get_rope_table_mode()
        set_rope_table_mode(true)   // the gate declines rope_mode off the table; this claim is about the width
        var routed <- moe_shell(KqFmt.k4)
        routed.quant = QuantMode.q8
        routed.blocks.ffn_is_dense = false
        routed.config.hidden_dim = 5488l   // off the 32 lattice the dense arm checks, dispatched by no layer
        t |> success(!has_dense_ffn(routed), "a routed model with no dense lead and no dense shared branch has no dense FFN")
        // mint_time skips the backend-repack clause, whose value this box decides
        t |> equal(decode_shape_decline(routed, DECODE_NEEDS_OK, true, false, false, mint_time = true), MetalDecodeDecline.none,
            "the routed model's odd dense width gates nothing")
        t |> equal(decode_shape_decline(routed, BATCH_NEEDS_OK, true, false, false, mint_time = true), MetalDecodeDecline.none,
            "and the batch gate reads the same width the same way")
        delete routed
        var dense <- shell(2l, 0ul)
        dense.quant = QuantMode.q8
        dense.config.dim = 256l
        dense.config.hidden_dim = 5488l
        t |> success(has_dense_ffn(dense), "a dense model dispatches its width")
        t |> equal(decode_shape_decline(dense, DECODE_NEEDS_OK, true, false, false, mint_time = true), MetalDecodeDecline.shape,
            "the same odd width on a dense model declines shape")
        t |> equal(decode_shape_decline(dense, BATCH_NEEDS_OK, true, false, false, mint_time = true), MetalDecodeDecline.shape,
            "the batch gate declines it too")
        dense.config.hidden_dim = 5472l
        t |> equal(decode_shape_decline(dense, DECODE_NEEDS_OK, true, false, false, mint_time = true), MetalDecodeDecline.none,
            "and its lattice width passes - the width is what declined, not the rest of the shell")
        delete dense
        set_rope_table_mode(saved_rope)
    }
}

//! a plane format the Metal kernels do not read declines with a line naming the site and the `KqFmt` member: a dense plane under
//! `kquant_native`, a routed stack under `graph`; the enum value the declines-by-reason table counts stays the gate's
[test]
def test_metal_fmt_decline_names_site(t : T?) {
    t |> run("a dense plane's unserved format names its layer, site and member; the decode gate still answers kquant_native") @(t : T?) {
        var m <- shell(2l, 0ul)
        m.quant = QuantMode.q8
        m.kquant_native = true
        m.wq_fmt <- [KqFmt.k4, KqFmt.k4]
        m.wk_fmt <- [KqFmt.k4, KqFmt.k4]
        m.wv_fmt <- [KqFmt.k6, KqFmt.k6]
        m.wo_fmt <- [KqFmt.k4, KqFmt.k4]
        m.w1_fmt <- [KqFmt.k4, KqFmt.k4]
        m.w3_fmt <- [KqFmt.k4, KqFmt.k4]
        m.w2_fmt <- [KqFmt.k6, KqFmt.k6]
        t |> equal(kq_load_gpu_unserved(m), "", "every plane in a served format: nothing to name")
        t |> equal(metal_fmt_decline_line("decode", "kquant_native", m), "", "and no line")
        m.w3_fmt[1] = KqFmt.mx4
        t |> equal(kq_load_gpu_unserved(m), "layer 1's FFN up plane is mx4")
        t |> equal(decode_shape_decline(m, DECODE_NEEDS_OK, true, false, false, mint_time = true), MetalDecodeDecline.kquant_native,
            "the gate's reason is the enum value the declines table counts")
        t |> equal(metal_fmt_decline_line("decode", "{MetalDecodeDecline.kquant_native}", m),
            "dasLLAMA metal decode: declined kquant_native - layer 1's FFN up plane is mx4, a format the Metal kernels do not read there; the CPU serves the call")
        t |> equal(metal_fmt_decline_line("decode", "{MetalDecodeDecline.shape}", m), "", "a decline no plane format decided says no format line")
        m.w3_fmt[1] = KqFmt.k4
        m.emb_fmt = KqFmt.mx4
        t |> equal(kq_load_gpu_unserved(m), "the token table plane is mx4")
        m.emb_fmt = KqFmt.q8
        m.wcls_fmt = KqFmt.mx4
        t |> equal(kq_load_gpu_unserved(m), "the classifier plane is mx4")
        delete m
    }
    t |> run("an nv4 plane serves; the same plane under a per-tensor .scale names its layer, site and member") @(t : T?) {
        var m <- shell(2l, 0ul)
        m.quant = QuantMode.q8
        m.kquant_native = true
        m.wq_fmt <- [KqFmt.nv4, KqFmt.nv4]
        m.wk_fmt <- [KqFmt.nv4, KqFmt.nv4]
        m.wv_fmt <- [KqFmt.nv4, KqFmt.nv4]
        m.wo_fmt <- [KqFmt.nv4, KqFmt.nv4]
        m.w1_fmt <- [KqFmt.nv4, KqFmt.nv4]
        m.w3_fmt <- [KqFmt.nv4, KqFmt.nv4]
        m.w2_fmt <- [KqFmt.nv4, KqFmt.nv4]
        m.wq_offs <- [0l, 1024l]   //! every site its own plane offset: the key is format and offset
        m.wk_offs <- [0l, 2048l]
        m.wv_offs <- [0l, 3072l]
        m.wo_offs <- [0l, 4096l]
        m.w1_offs <- [0l, 5120l]
        m.w3_offs <- [0l, 6144l]
        m.w2_offs <- [0l, 7168l]
        t |> equal(kq_load_gpu_unserved(m), "", "every dense plane nv4 with no .scale: served")
        for (off, role in [1024l, 2048l, 3072l, 4096l, 5120l, 6144l, 7168l], ["attn_q", "attn_k", "attn_v", "attn_output", "ffn_gate", "ffn_up", "ffn_down"]) {
            m.site_scale[site_scale_key(KqFmt.nv4, off)] = 0.01
            t |> equal(kq_load_gpu_unserved(m), "layer 1's {kq_site_words(role)} plane is nv4 under a per-tensor .scale")
            t |> equal(decode_shape_decline(m, DECODE_NEEDS_OK, true, false, false, mint_time = true), MetalDecodeDecline.kquant_native,
                "{role}: the gate's reason is the enum value the declines table counts")
            m.site_scale |> clear()
        }
        t |> equal(kq_load_gpu_unserved(m), "", "the .scale gone, the planes serve again")
        delete m
    }
    t |> run("a routed stack's unserved format names its layer, site and member under graph") @(t : T?) {
        var m <- moe_shell(KqFmt.k4)
        t |> equal(moe_fmts_unserved(m), "", "a k4 stack has the routed block's kernels")
        m.we2_fmt[1] = KqFmt.mx4
        t |> success(!moe_metal_ok(m), "the routed block declines the stack")
        t |> equal(moe_fmts_unserved(m), "layer 1's expert stack down plane is mx4")
        t |> equal(metal_fmt_decline_line("prefill", "{MetalPrefillDecline.graph}", m),
            "dasLLAMA metal prefill: declined graph - layer 1's expert stack down plane is mx4, a format the Metal kernels do not read there; the CPU serves the call")
        m.we2_fmt[1] = KqFmt.nv4
        t |> equal(moe_fmts_unserved(m), "", "an nv4 stack with no .scale has the routed block's kernels")
        m.site_scale[site_scale_key(KqFmt.nv4, m.we2_offs[1])] = 0.01
        t |> success(!moe_metal_ok(m), "the routed block declines an nv4 stack under a per-expert .scale: no Metal kernel reads the factor")
        t |> equal(moe_fmts_unserved(m), "layer 1's expert stack down plane is nv4 under a per-tensor .scale")
        delete m
    }
}

//! the routed block's other `graph` declines name their plane the same way: a sigmoid-gated shared expert's, a deltanet plane's
[test]
def test_metal_graph_decline_names_site(t : T?) {
    t |> run("a sigmoid-gated shared expert's unserved format names its layer, site and member under graph") @(t : T?) {
        var m <- moe_shell(KqFmt.k4)
        m.config.n_ff_shexp = 256l
        m.config.moe_shexp_gated = true
        m.wshk1_offs <- [0l, 0l]
        m.wshk2_offs <- [0l, 0l]
        m.wshk3_offs <- [0l, 0l]
        m.wsh1_fmt <- [KqFmt.k4, KqFmt.k4]
        m.wsh2_fmt <- [KqFmt.k4, KqFmt.k4]
        m.wsh3_fmt <- [KqFmt.k4, KqFmt.k4]
        t |> equal(shexp_fmts_unserved(m), "", "k4 shared-expert planes have their one-row GEMVs")
        m.wsh3_fmt[1] = KqFmt.mx4
        t |> success(!moe_metal_ok(m), "the routed block declines the shared expert's plane")
        t |> equal(shexp_fmts_unserved(m), "layer 1's shared expert up plane is mx4")
        t |> equal(metal_fmt_decline_line("decode", "{MetalDecodeDecline.graph}", m),
            "dasLLAMA metal decode: declined graph - layer 1's shared expert up plane is mx4, a format the Metal kernels do not read there; the CPU serves the call")
        m.wsh3_fmt[1] = KqFmt.nv4
        t |> equal(shexp_fmts_unserved(m), "", "an nv4 shared-expert plane with no .scale has its one-row GEMV")
        m.site_scale[site_scale_key(KqFmt.nv4, m.wshk3_offs[1])] = 0.01
        t |> success(!moe_metal_ok(m), "the routed block declines an nv4 shared-expert plane under a .scale")
        t |> equal(shexp_fmts_unserved(m), "layer 1's shared expert up plane is nv4 under a per-tensor .scale")
        delete m
    }
    t |> run("a deltanet plane's unserved format names its layer, site and member under graph") @(t : T?) {
        var m <- shell(3l, 2ul)
        m.config.dim = 256l
        m.config.ssm_d_inner = 256l
        m.config.ssm_d_state = 128l
        m.config.ssm_d_conv = 4l
        m.dnqkv_fmt <- [KqFmt.q8, KqFmt.k4, KqFmt.q8]
        m.dngate_fmt <- [KqFmt.q8, KqFmt.k4, KqFmt.q8]
        m.dnout_fmt <- [KqFmt.q8, KqFmt.k4, KqFmt.q8]
        m.dnbeta_fmt <- [KqFmt.q8, KqFmt.k4, KqFmt.q8]
        m.dnalpha_fmt <- [KqFmt.q8, KqFmt.k4, KqFmt.q8]
        t |> equal(dn_fmts_unserved(m), "", "k4 deltanet planes have their one-row GEMVs")
        t |> equal(dn_ba_window_fmts_unserved(m), "", "and the beta/alpha planes their expert-form GEMV")
        m.dngate_fmt[1] = KqFmt.mx4
        t |> success(!dn_metal_ok(m), "the deltanet gate declines the hybrid")
        t |> equal(dn_fmts_unserved(m), "layer 1's deltanet gate plane is mx4")
        t |> equal(metal_fmt_decline_line("prefill", "{MetalPrefillDecline.graph}", m),
            "dasLLAMA metal prefill: declined graph - layer 1's deltanet gate plane is mx4, a format the Metal kernels do not read there; the CPU serves the call")
        m.dngate_fmt[1] = KqFmt.k4
        m.dnalpha_fmt[1] = KqFmt.mx4
        t |> success(!dn_ba_metal_window_ok(m), "the window's beta/alpha gate declines an mx4 alpha plane")
        t |> equal(dn_ba_window_fmts_unserved(m), "layer 1's deltanet alpha plane is mx4")
        m.dnalpha_fmt[1] = KqFmt.k4
        m.dnqkv_fmt <- [KqFmt.nv4, KqFmt.nv4, KqFmt.nv4]
        m.dngate_fmt <- [KqFmt.nv4, KqFmt.nv4, KqFmt.nv4]
        m.dnout_fmt <- [KqFmt.nv4, KqFmt.nv4, KqFmt.nv4]
        m.dnbeta_fmt <- [KqFmt.nv4, KqFmt.nv4, KqFmt.nv4]
        m.dnalpha_fmt <- [KqFmt.nv4, KqFmt.nv4, KqFmt.nv4]
        m.dnqkv_offs <- [0l, 8192l, 16384l]   //! every site its own plane offset: the key is format and offset
        m.dngate_offs <- [0l, 9216l, 17408l]
        m.dnout_offs <- [0l, 10240l, 18432l]
        m.dnbeta_offs <- [0l, 11264l, 19456l]
        m.dnalpha_offs <- [0l, 12288l, 20480l]
        t |> equal(dn_fmts_unserved(m), "", "nv4 deltanet planes with no .scale: served")
        for (off, words in [8192l, 9216l, 10240l, 11264l, 12288l],
                            ["deltanet qkv", "deltanet gate", kq_site_words("ssm_out"), kq_site_words("ssm_beta"), kq_site_words("ssm_alpha")]) {
            m.site_scale[site_scale_key(KqFmt.nv4, off)] = 0.01
            t |> success(!dn_metal_ok(m), "{words}: a deltanet plane under a per-tensor .scale declines the hybrid")
            t |> equal(dn_fmts_unserved(m), "layer 1's {words} plane is nv4 under a per-tensor .scale")
            m.site_scale |> clear()
        }
        delete m
    }
}

//! under `MetalMode.required` a plane-format decline panics with the words its warning line logs, not the bare reason
[test]
def test_metal_decline_panic_names_site(t : T?) {
    t |> run("the required-mode panic carries the site and member the warning line names") @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            var m <- shell(2l, 0ul)
            m.quant = QuantMode.q8
            m.kquant_native = true
            m.w3_fmt <- [KqFmt.k4, KqFmt.mx4]
            let words = metal_fmt_decline_words("decode", "{MetalDecodeDecline.kquant_native}", m)
            t |> equal(words, "kquant_native - layer 1's FFN up plane is mx4, a format the Metal kernels do not read there")
            let mode0 = get_metal_mode()
            let seat = kernel_backend_seat()
            let pf0 = active_prefill_override()
            let dc0 = active_decode_override()
            set_metal_mode(MetalMode.required)
            let why = panic_text() $ { decline(MetalDecodeDecline.kquant_native, words) }
            let bare = panic_text() $ { decline(MetalDecodeDecline.shape) }
            set_metal_mode(mode0)
            select_prefill_override(pf0)
            select_decode_override(dc0)
            restore_kernel_backend(seat)
            t |> success(find(why, "decode declined: {words} (the GPU cannot serve this call)") >= 0, "the panic names the site and member: {why}")
            t |> success(find(bare, "decode declined: shape (") >= 0, "a decline no plane format decided panics with its reason alone (the control): {bare}")
            delete m
        } else {
            t |> skip("the required-mode panic is the Metal drivers' - no das_metal in this build")
        }
    }
}

[test]
def test_mm_tile_widths_64_dense_width(t : T?) {
    t |> run("the mul_mm tile gate reads the dense FFN width only where a dense FFN dispatches: a routed model's odd dense width gates nothing") @(t : T?) {
        var routed <- moe_shell(KqFmt.k4)
        routed.blocks.ffn_is_dense = false
        routed.config.n_kv_heads = 2l   // the shell's one kv head x 32 sits off the 64 lattice the gate also checks
        routed.config.hidden_dim = 5472l   // Qwen3-Coder-30B-A3B's feed_forward_length: off the 64 lattice, dispatched by no layer
        t |> success(!has_dense_ffn(routed), "a routed model with no dense lead has no dense FFN")
        t |> success(mm_tile_widths_64_ok(routed), "the routed model passes the tile gate on its odd dense width")
        routed.config.n_layer_dense_lead = 1l
        t |> success(has_dense_ffn(routed) && !mm_tile_widths_64_ok(routed), "a dense lead layer dispatches the width: the odd width declines")
        delete routed
        var dense <- shell(2l, 0ul)
        dense.config.dim = 256l
        dense.config.n_kv_heads = 2l
        dense.config.hidden_dim = 5472l
        t |> success(has_dense_ffn(dense) && !mm_tile_widths_64_ok(dense), "a dense model's odd FFN width declines")
        dense.config.hidden_dim = 6144l
        t |> success(mm_tile_widths_64_ok(dense), "and its lattice width passes")
        delete dense
    }
}

[test]
def test_resident_region_ctx(t : T?) {
    t |> run("a region's context is its share of the mirror side's one binding") @(t : T?) {
        t |> equal(resident_region_ctx(131072l, 65535l, 1l), 65535l, "one region takes the whole binding")
        t |> equal(resident_region_ctx(131072l, 65535l, 4l), 16383l, "four regions take a quarter each")
        t |> equal(resident_region_ctx(8192l, 65535l, 4l), 8192l, "a session context under its share stands")
        t |> equal(resident_region_ctx(131072l, 0l, 4l), 131072l, "no binding cap known: the context stands")
        t |> equal(resident_region_ctx(131072l, 65535l, 0l), 65535l, "a region count under one plans one region")
    }
}

[test]
def test_resident_fit_ctx(t : T?) {
    t |> run("a plan past its room shortens to the context the room holds: a block codec's shadow shortens with the mirror") @(t : T?) {
        let mb = 1_048_576l
        let asked = 100000l
        let kv_pos = 3456l          // 24 layers' K and V rows of 128 elements at tq4's 18 bytes a 32-block
        let shadow_pos = 512l       // both sides' rows of one layer as halves
        let staged = 10l * mb
        var plan = ResidentPlan(usable_bytes = 800l * mb, weight_bytes = 500l * mb, kv_bytes = asked * kv_pos, scratch_bytes = staged + asked * shadow_pos)
        let room = 800l * mb - 500l * mb - staged
        let want = (room / 8l * 7l) / (kv_pos + shadow_pos)
        t |> equal(resident_fit_ctx(plan, asked, shadow_pos), want, "the room past the weights and the staged rows, over the mirror's and the shadow's bytes a position")
        t |> success(want * (kv_pos + shadow_pos) <= room / 8l * 7l && (want + 1l) * (kv_pos + shadow_pos) > room / 8l * 7l, "the mirror and its shadow at that context fill the room less its slack, and one position more does not fit")
        let shadow_whole = ((room - asked * shadow_pos) / 8l * 7l) / kv_pos
        t |> success(want > shadow_whole, "the shadow charged whole at the asked context holds fewer positions ({shadow_whole} against {want})")
        plan.scratch_bytes = staged
        t |> equal(resident_fit_ctx(plan, asked, 0l), (room / 8l * 7l) / kv_pos, "a scalar codec has no shadow: the room over the mirror's bytes a position")
        plan.hot_bytes = 100l * mb
        t |> equal(resident_fit_ctx(plan, asked, 0l), ((room - 100l * mb) / 8l * 7l) / kv_pos, "the hot pool's bytes come off the room")
        plan.weight_bytes = 900l * mb
        t |> equal(resident_fit_ctx(plan, asked, 0l), 0l, "weights past the room: no context fits")
    }
    t |> run("a ringed mirror's rings are bytes the context does not scale: they come off the room, and the rest of the mirror scales") @(t : T?) {
        let mb = 1_048_576l
        let asked = 100000l
        let flat_pos = 1000l        //! the global layers' rows a position
        let ring_bytes = 40l * mb   //! the sliding layers' rings, fixed at any context past their reach
        let plan = ResidentPlan(usable_bytes = 800l * mb, weight_bytes = 500l * mb, kv_bytes = asked * flat_pos + ring_bytes, kv_ring_bytes = ring_bytes)
        let room = 800l * mb - 500l * mb - ring_bytes
        let want = (room / 8l * 7l) / flat_pos
        t |> equal(resident_fit_ctx(plan, asked, 0l), want, "the room past the weights and the rings, over the global layers' bytes a position")
        let scaled = ((800l * mb - 500l * mb) / 8l * 7l) / ((asked * flat_pos + ring_bytes) / asked)
        t |> success(scaled < want, "the rings charged a position at the asked context hold fewer positions ({scaled} against {want})")
    }
}

[test]
def test_resident_binding_ctx(t : T?) {
    t |> run("the KV mirror's binding cap is one slab's range over the widest attention layer's row, whatever the layer count") @(t : T?) {
        let bcap = 4_294_967_295l   //! the 4 GiB maxStorageBufferRange of the NV/AMD drivers
        //! 32 layers, attention every fourth (3, 7, ..., 31), kv width 8 x 128 in f16: one layer's row is 2048 B a position
        var hy <- shell(32l, 0x7777_7777ul)
        hy.config.n_kv_heads = 8l
        hy.config.head_size = 128l
        t |> success(layer_is_recurrent(hy.config, 0l) && layer_kv_dim(hy.config, 0l) == 0l, "layer 0 is recurrent: its kv width reads 0")
        t |> equal(resident_binding_ctx(hy, KVDtype.f16, bcap), 2097151l, "the hybrid's ctx is the range over one attention layer's row, not skipped for layer 0's zero width")
        t |> equal(resident_binding_ctx(hy, KVDtype.f32, bcap), 1048575l, "f32 rows halve it")
        t |> equal(resident_binding_ctx(hy, KVDtype.f16, 0l), 0l, "no cap known reads 0")
        //! a block codec's kernels address bytes in 32 bits: past that many bytes the device's range no longer raises the context
        let wide = 4l * BLOCK_CODEC_SIDE_BYTES
        t |> equal(resident_binding_ctx(hy, KVDtype.q8_0, wide), resident_binding_ctx(hy, KVDtype.q8_0, BLOCK_CODEC_SIDE_BYTES), "a q8_0 mirror stops at the byte cap on a device that binds four times it")
        t |> equal(resident_binding_ctx(hy, KVDtype.tq4, wide), resident_binding_ctx(hy, KVDtype.tq4, BLOCK_CODEC_SIDE_BYTES), "... and a tq4 mirror")
        t |> equal(resident_binding_ctx(hy, KVDtype.f16, wide), wide / 2048l, "control: an f16 mirror, addressed by element, takes the wider range over its 2048-byte row")
        t |> success(resident_binding_ctx(hy, KVDtype.tq4, bcap) > resident_binding_ctx(hy, KVDtype.q8_0, bcap)
            && resident_binding_ctx(hy, KVDtype.q8_0, bcap) > resident_binding_ctx(hy, KVDtype.f16, bcap), "under the cap one range holds more tq4 positions than q8_0, and more q8_0 than f16")
        delete hy
        var dense <- shell(32l, 0ul)
        dense.config.n_kv_heads = 8l
        dense.config.head_size = 128l
        t |> equal(resident_binding_ctx(dense, KVDtype.f16, bcap), 2097151l, "the dense twin's thirty-two rows read the same: a slab holds whole layers, so the widest row bounds the context and the layers fill slabs")
        for (k in range64(8l)) {   //! every fourth layer shares the K/V of the layer below it: eight sharers, twenty-four owners
            dense.kv_src[4l * k + 1l] = 4l * k
        }
        t |> equal(resident_binding_ctx(dense, KVDtype.f16, bcap), 2097151l, "a shared-KV layer adds no row of its own and moves nothing")
        delete dense
        var recr <- shell(4l, 0xFul)
        t |> equal(resident_binding_ctx(recr, KVDtype.f16, bcap), 0l, "no attention layer reads 0")
        delete recr
    }
}

[test]
def test_resident_mirror_slabs(t : T?) {
    t |> run("the mirror's slabs: contiguous layer runs under the cap, cut for the least charge, one stride for every slab, the donor's seat for a sharer, the head last") @(t : T?) {
        //! 8 layers, kv width 32 in f16: a layer's rows are 100 x 32 = 3200 units; a slab of 13000 holds four
        var dense <- shell(8l, 0ul)
        var s <- resident_mirror_slabs(dense, 100l, KVDtype.f16, 13000l, false)
        t |> equal(s.n_slabs, 2l, "eight attention layers over a cap of four: two slabs of four")
        t |> equal(s.units, 12800l, "every slab takes the widest slab's stride - four layers' rows, no padding")
        t |> equal("{s.slab}", "{[0l, 0l, 0l, 0l, 1l, 1l, 1l, 1l, -1l]}", "the layers fill slabs in order, and no head")
        t |> equal("{s.base}", "{[0l, 3200l, 6400l, 9600l, 0l, 3200l, 6400l, 9600l, -1l]}", "a layer's base restarts at every slab")
        delete s
        delete dense
        //! layer 2 recurrent: seven owners, which no cut into two or three slabs fills evenly
        var m <- shell(8l, 0x4ul)
        s <- resident_mirror_slabs(m, 100l, KVDtype.f16, 13000l, false)
        t |> equal(s.n_slabs, 7l, "seven layers over a cap of four: the cut that charges least is a slab a layer (22400 units) - four and three would pad to 25600")
        t |> equal(s.units, 3200l, "at one layer's stride")
        t |> equal("{s.slab}", "{[0l, 1l, -1l, 2l, 3l, 4l, 5l, 6l, -1l]}", "the recurrent layer sits in no slab")
        delete s
        m.kv_src[4l] = 3l
        s <- resident_mirror_slabs(m, 100l, KVDtype.f16, 13000l, false)
        t |> equal(s.n_slabs, 2l, "a shared-KV layer holds no rows: six owners cut three and three")
        t |> equal(s.units, 9600l, "at three layers' stride, no padding")
        t |> equal("{s.slab}", "{[0l, 0l, -1l, 0l, 0l, 1l, 1l, 1l, -1l]}", "the sharer carries its donor's slab")
        t |> equal("{s.base}", "{[0l, 3200l, -1l, 6400l, 6400l, 0l, 3200l, 6400l, -1l]}", "and its donor's base")
        delete s
        s <- resident_mirror_slabs(m, 100l, KVDtype.f16, 0l, false)
        t |> equal(s.n_slabs, 1l, "no cap: one slab")
        t |> equal(s.units, 19200l, "of every owner's rows")
        t |> equal(s.base[7l], 16000l, "the last owner sits after the five before it")
        delete s
        m.config.n_layer_nextn = 1l
        s <- resident_mirror_slabs(m, 100l, KVDtype.f16, 13000l, true)
        t |> equal(s.n_slabs, 7l, "the head's rows seat after the last layer's: seven owners again, a slab each")
        t |> equal(s.slab[8l], 6l, "... the head in the last slab")
        t |> equal(s.base[8l], 0l, "... at its start")
        delete s
        s <- resident_mirror_slabs(m, 100l, KVDtype.f16, 26000l, true)
        t |> equal(s.n_slabs, 1l, "a cap past every row: one slab")
        t |> equal(s.base[8l], 19200l, "the head's rows after the six layers'")
        delete s
        s <- resident_mirror_slabs(m, 100l, KVDtype.f16, 3000l, true)
        t |> equal(s.n_slabs, 7l, "a cap under one layer's rows gives every owner its own slab (the binding cap bounds the context before the partition)")
        t |> equal(s.units, 3200l, "at one layer's stride")
        delete s
        s <- resident_mirror_slabs(m, 100l, KVDtype.q8_0, 7000l, true)
        t |> equal(s.units, 700l, "a block codec counts blocks: six owners and the head at 100 x 1 blocks each")
        t |> equal(s.n_slabs, 1l, "... in one slab, since the cap counts the same units")
        delete s
        m.config.n_layer_nextn = 0l
        var cut <- resident_mirror_slabs(m, 100l, KVDtype.f16, resident_slab_cap_units(KVDtype.f16, 2l), false)
        t |> equal(resident_mirror_bytes(m, 100l, KVDtype.f16, 2l), 2l * 2l * cut.n_slabs * (cut.units + moe_gpu_mirror_slack()) * 2l, "the plan charges both sides of every slab at the widest slab's stride plus the tier's slack, times the regions ({cut.n_slabs} slabs of {cut.units} units under this box's cap)")
        delete cut
        delete m
        var recr <- shell(4l, 0xFul)
        var s4 <- resident_mirror_slabs(recr, 100l, KVDtype.f16, 7000l, false)
        t |> equal(s4.n_slabs, 0l, "no attention layer: no slab")
        t |> equal(s4.units, 0l, "and no stride")
        delete s4
        delete recr
    }
}

[test]
def test_slab_limit(t : T?) {
    t |> run("the cut's stride on varied widths: the fewest slabs the total admits can leave one past the cap, which no cut may") @(t : T?) {
        t |> equal(slab_limit([9l, 2l, 9l], 10l), 9l, "two slabs would hold 11 and 9 against a cap of 10, so three of at most 9 cut")
        t |> equal(slab_limit([9l, 2l, 9l], 20l), 20l, "a cap past the total: one slab")
        t |> equal(slab_limit([5l, 5l, 5l, 5l], 10l), 10l, "uniform widths that divide: two slabs of two charge the total, as four of one would - the fewer win")
        t |> equal(slab_limit([5l, 5l, 5l, 5l], 3l), 5l, "a cap under one layer: one layer's stride")
    }
}

[test]
def test_resident_shadow_ctx(t : T?) {
    t |> run("a block codec's f16 shadow caps a region's context at one binding of the widest K/V row's halves") @(t : T?) {
        let bcap = 4_294_967_295l
        var hy <- shell(32l, 0x7777_7777ul)
        hy.config.n_kv_heads = 8l
        hy.config.head_size = 128l   // a 1024-wide K/V row: 2048 B of halves a position
        t |> equal(resident_shadow_ctx(hy, KVDtype.q8_0, bcap), 2097151l, "the range over one layer's row as halves, whatever the layer count")
        t |> equal(resident_shadow_ctx(hy, KVDtype.tq4, bcap), 2097151l, "the shadow is the same halves under tq4")
        t |> equal(resident_shadow_ctx(hy, KVDtype.f16, bcap), 0l, "control: a scalar codec has no shadow, so no cap")
        t |> equal(resident_shadow_ctx(hy, KVDtype.q8_0, 0l), 0l, "no cap known reads 0")
        hy.config.head_size = 256l
        t |> equal(resident_shadow_ctx(hy, KVDtype.q8_0, bcap), 1048575l, "a row twice as wide halves it")
        delete hy
        var recr <- shell(4l, 0xFul)
        t |> equal(resident_shadow_ctx(recr, KVDtype.q8_0, bcap), 0l, "no attention layer reads 0")
        delete recr
    }
}

let private TRIM_KIND_PROBE = "trim-kind-probe"

//! a prefill override that passes every call to the CPU layer loop: the foreign override the trim-kind cell selects
[unused_argument(tr, s, npos, start_pos)]
def private pass_prefill_probe(tr : Model; var s : Session; npos : int64; start_pos : int64) : bool => false

//! the trim's admission tells this run's declines (the route, a prefill pin or override, the tier's seams, the card's room) from the
//! model's (experts, the residual, a feature): a lane minted under the first would be wrong for the next run, so the mint writes none
[test]
def test_trim_decline_kind(t : T?) {
    t |> run("a trimmed lane's decline is this run's or the model's") @(t : T?) {
        t |> success(resident_trim_decline_is_run_mode("the whole-model driver is not asked for (DASLLAMA_GPU_RESIDENT, set_gpu_resident_route)"), "the route is the run's")
        t |> success(resident_trim_decline_is_run_mode("the residency plan does not fit the card"), "the card's room is the run's")
        t |> success(resident_trim_decline_is_run_mode("the tier installed no resident-driver seams"), "the tier's seams are the run's")
        t |> success(resident_trim_decline_is_run_mode("routed experts (64) - the tier installed no MoE seats; the per-op tier's expert rails serve"), "the MoE seats are the run's")
        t |> success(resident_trim_decline_is_run_mode("{TRIM_SPLIT_DECLINE} (18 routed layers' experts on the host in the lane, 20 in this run's plan)"), "a lane minted under another host-layer split is this run's decline: the mint holds")
        t |> success(!resident_trim_decline_is_run_mode("the trim serves dense models without a hyper-connection residual or per-layer embeddings"), "the model's shape is the model's")
        t |> success(!resident_trim_decline_is_run_mode("no arm for: sinks"), "a feature is the model's")
        t |> success(!resident_trim_decline_is_run_mode(""), "an empty decline is no decline")
    }
    t |> run("a foreign prefill override selected in-process is a run-mode decline the chain names") @(t : T?) {
        let found = active_prefill_override()
        register_prefill_override(TRIM_KIND_PROBE, @@pass_prefill_probe)
        select_prefill_override(TRIM_KIND_PROBE)
        let why = resident_run_mode_decline()
        t |> equal(why, "the '{TRIM_KIND_PROBE}' prefill override is active")
        t |> success(resident_trim_decline_is_run_mode(why), "the override's decline is this run's")
        select_prefill_override(found)
        t |> success(empty(resident_run_mode_decline()) || active_prefill_override() == found, "control: the override put back, the decline reads as the process found it")
    }
}

[test]
def test_resident_mirror_codec_ask(t : T?) {
    t |> run("a block codec's geometry: whole 32-blocks a row and a head, a power-of-two head under tq4") @(t : T?) {
        var m <- shell(2l, 0ul)
        t |> success(empty(kv_codec_geometry_why(m.config, KVDtype.q8_0)) && empty(kv_codec_geometry_why(m.config, KVDtype.tq4)), "a 32-wide head holds both block codecs")
        m.config.head_size = 96l
        t |> success(empty(kv_codec_geometry_why(m.config, KVDtype.q8_0)), "a 96-wide head is whole blocks: q8_0 holds it")
        t |> success(find(kv_codec_geometry_why(m.config, KVDtype.tq4), "power-of-two head_size") >= 0, "tq4 names the head it cannot rotate")
        m.config.head_size = 48l
        t |> success(find(kv_codec_geometry_why(m.config, KVDtype.q8_0), "multiples of 32") >= 0, "a 48-wide head is no whole block: q8_0 names it")
        t |> success(empty(kv_codec_geometry_why(m.config, KVDtype.f16)), "a scalar codec holds any head")
        delete m
    }
    t |> run("the mirror codec the plan arms: the ask where the model holds it, the served codec where it does not, f16 for an f32 ask") @(t : T?) {
        let pin0 = gpu_kv_dtype_pin_()
        set_gpu_kv_dtype_(KVDtype.tq4)
        if (resident_mirror_dtype() != KVDtype.tq4) {
            t |> skip("the environment arms a {resident_mirror_dtype()} mirror over the ask (DASLLAMA_VK_KV32=1) - the cell needs that unset")
        } else {
            var m <- shell(2l, 0ul)
            t |> equal(resident_mirror_dtype_of(m), KVDtype.tq4, "tq4 asked of a 32-wide head arms tq4")
            m.config.head_size = 96l
            t |> equal(resident_mirror_dtype_of(m), KVDtype.q8_0, "tq4 asked of a 96-wide head arms q8_0, the codec the sessions step down to")
            m.config.head_size = 32l
            m.config.attn_qkv_bias = true
            t |> equal(resident_mirror_dtype_of(m), KVDtype.q8_0, "tq4 asked of a Q/K/V-biased attention arms q8_0")
            m.config.attn_qkv_bias = false
            m.config.head_size = 48l
            t |> equal(resident_mirror_dtype_of(m), KVDtype.f16, "tq4 asked of a 48-wide head arms f16: no block codec holds it")
            m.config.head_size = 96l
            set_gpu_kv_dtype_(KVDtype.q8_0)
            t |> equal(resident_mirror_dtype_of(m), KVDtype.q8_0, "control: q8_0 holds the same head")
            set_gpu_kv_dtype_(KVDtype.f32)
            t |> equal(resident_mirror_dtype(), KVDtype.f16, "an f32 ask arms f16: the f32 mirror is DASLLAMA_VK_KV32=1's alone")
            reset_gpu_kv_dtype_()
            t |> success(!gpu_kv_dtype_pin_().pinned, "the reset lifts the ask")
            delete m
        }
        restore_gpu_kv_dtype(pin0)
    }
    t |> run("the fixtures' put-back of the codec ask: the pin again where one stood, the reset where none did") @(t : T?) {
        let found = gpu_kv_dtype_pin_()
        set_gpu_kv_dtype_(KVDtype.q8_0)
        let pinned = gpu_kv_dtype_pin_()
        set_gpu_kv_dtype_(KVDtype.tq4)
        restore_gpu_kv_dtype(pinned)
        t |> success(gpu_kv_dtype_pin_().pinned && gpu_kv_dtype_pin_().dt == KVDtype.q8_0, "a pinned ask comes back as the pin it was, not the one set after it")
        reset_gpu_kv_dtype_()
        let lifted = gpu_kv_dtype_pin_()
        set_gpu_kv_dtype_(KVDtype.tq4)
        restore_gpu_kv_dtype(lifted)
        t |> success(!gpu_kv_dtype_pin_().pinned, "an unpinned ask comes back lifted")
        restore_gpu_kv_dtype(found)
    }
    t |> run("a codec's spelling, in any case, and its ordinal across the tier's seam") @(t : T?) {
        let upper = kv_dtype_named("Q8_0")
        t |> success(upper.known && upper.dt == KVDtype.q8_0, "an upper-case spelling names the codec")
        t |> success(kv_dtype_named("tq4").dt == KVDtype.tq4 && kv_dtype_named("f32").dt == KVDtype.f32 && kv_dtype_named("f16").known, "every codec has its spelling")
        let bogus = kv_dtype_named("bogus")
        t |> success(!bogus.known && bogus.dt == KVDtype.f16, "a spelling that names no codec reads f16, marked unknown")
        t |> equal(kv_dtype_of(int(KVDtype.tq4)), KVDtype.tq4, "an ordinal names its codec")
        let why = panic_text() $ {
            kv_dtype_of(99)
        }
        t |> success(find(why, "no KVDtype has ordinal 99") >= 0, "an ordinal no codec carries panics by name: {why}")
    }
}

[test]
def test_plan_room(t : T?) {
    t |> run("the plan's room: the tier cap less headroom, capped by the OS's room when it answers") @(t : T?) {
        let gib = 1_073_741_824l
        let silent = (budget = 0l, usage = 0l, adapter_dedicated = 0l, adapter_shared = 0l)
        t |> equal(16l * gib * 27l / 100l, plan_headroom(16l * gib, true), "no OS answer on WDDM: 27% of a 16 GiB cap, past the 2 GiB floor, is the headroom")
        t |> equal(2l * gib, plan_headroom(4l * gib, true), "no OS answer on WDDM: the 2 GiB floor wins on a small cap")
        t |> equal(2l * gib, plan_headroom(16l * gib, false), "no OS answer off WDDM: the 2 GiB floor alone - the driver refuses an over-commit out loud, nothing demotes")
        let no_os = plan_room(16l * gib, false, silent)
        t |> equal(plan_headroom(16l * gib, get_platform_name() == "windows"), no_os.headroom, "no OS answer: the plan takes this platform's headroom")
        t |> equal(16l * gib - no_os.headroom, no_os.usable, "no OS answer: the cap less the headroom")
        t |> equal(0l, no_os.others, "no OS answer: nobody else is counted")
        let os = (budget = 14l * gib, usage = 1l * gib, adapter_dedicated = 5l * gib, adapter_shared = 0l)
        let answered = plan_room(16l * gib, false, os)
        t |> equal(0l, answered.headroom, "an OS answer takes no headroom")
        t |> equal(4l * gib, answered.others, "the other processes hold the adapter's dedicated usage less ours")
        t |> equal(14l * gib - 4l * gib - 1l * gib, answered.usable, "the OS budget less the others and the 1 GiB reserve")
        let pinned = plan_room(8l * gib, true, os)
        t |> equal(8l * gib, pinned.usable, "a pinned cap fills itself and reads no room")
        t |> equal(0l, pinned.others, "a pinned cap counts nobody else")
    }
}

[test]
def test_resident_hot_slots(t : T?) {
    t |> run("the hot expert pool's slots: every slot that fits beside the whole mirror, up to 32 out of the mirror's context, a count asked from the floor's room, none under four") @(t : T?) {
        let mib = 1_048_576l
        let column = 125l * mib
        t |> equal(72l, resident_hot_slots(-1l, 9000l * mib, 12000l * mib, column, 512l), "unasked, 72 slots fit beside the whole mirror: the pool takes every one and the mirror keeps its context")
        t |> equal(40l, resident_hot_slots(-1l, 5000l * mib, 12000l * mib, column, 512l), "unasked, 40 fit beside the mirror: past 32, the mirror yields nothing")
        t |> equal(32l, resident_hot_slots(-1l, 681l * mib, 5600l * mib, column, 512l), "unasked, five fit beside the mirror: the mirror's context yields the room of 32")
        t |> equal(20l, resident_hot_slots(-1l, 681l * mib, 2500l * mib, column, 512l), "... and no more than the floor's room holds")
        t |> equal(31l, resident_hot_slots(-1l, 31l * column, -500l * mib, column, 512l), "unasked, 31 fit beside the mirror and the floor's room is past the plan: the yield never takes the slots beside the mirror away")
        t |> equal(44l, resident_hot_slots(48l, 681l * mib, 5600l * mib, column, 512l), "a count asked is served from the floor's room")
        t |> equal(16l, resident_hot_slots(16l, 9000l * mib, 12000l * mib, column, 512l), "a count asked under the room stands")
        t |> equal(0l, resident_hot_slots(0l, 9000l * mib, 12000l * mib, column, 512l), "0 asks no pool")
        t |> equal(0l, resident_hot_slots(-1l, 0l, 3l * column, column, 512l), "three slots do not pay for their submit: no pool")
        t |> equal(0l, resident_hot_slots(-1l, -500l * mib, -100l * mib, column, 512l), "a plan already past its room takes none")
        t |> equal(8l, resident_hot_slots(-1l, 9000l * mib, 12000l * mib, column, 8l), "a layer's experts cap its slots")
    }
    t |> run("the pool's share of the plan: the chain's scratch is charged with a pool and never without one, and the slots are sized past it") @(t : T?) {
        let mib = 1_048_576l
        let column = 125l * mib
        let chain = RDEC_HOT_CHAIN_BYTES
        let none = resident_hot_fit(0l, HOT_SLOTS_AUTO, 9000l * mib, 12000l * mib, column, 512l)
        t |> equal(0l, none.slots, "no pool asked: no slots")
        t |> equal(0l, none.chain_bytes, "no pool asked: the chain's scratch is not charged")
        t |> equal(0l, none.hot_bytes, "no pool asked: no pool bytes")
        let tight = resident_hot_fit(HOT_SLOTS_AUTO, HOT_SLOTS_AUTO, 0l, 3l * column + chain, column, 512l)
        t |> equal(0l, tight.slots, "three slots past the chain's scratch: no pool")
        t |> equal(0l, tight.chain_bytes, "... and nothing charged for the chain the pool would have ridden")
        let some = resident_hot_fit(HOT_SLOTS_AUTO, HOT_SLOTS_AUTO, 64l * column + chain, 12000l * mib, column, 512l)
        t |> equal(64l, some.slots, "64 slots fit beside the mirror once the chain's scratch is taken off the room")
        t |> equal(chain, some.chain_bytes, "a pool charges the chain's scratch")
        t |> equal(64l * column, some.hot_bytes, "the pool's bytes are its slots'")
        let short = resident_hot_fit(HOT_SLOTS_AUTO, HOT_SLOTS_AUTO, 64l * column, 12000l * mib, column, 512l)
        t |> equal(61l, short.slots, "the chain's scratch comes out of the same room: 320 MB is two and a half slots of 125 MB, so 61 fit where 64 did")
        let capped = resident_hot_fit(HOT_SLOTS_AUTO, 16l, 9000l * mib, 12000l * mib, column, 512l)
        t |> equal(16l, capped.slots, "a cap a first plan settled holds the count")
        t |> equal(chain, capped.chain_bytes, "... with the chain still charged")
    }
}

[test]
def test_resident_arm_floor(t : T?) {
    t |> run("the smallest context a capped mirror arms at: the built-in floor, the env floor, and a caller's pin under either") @(t : T?) {
        t |> equal(4096l, resident_arm_floor(0l, 0l), "no env floor, no pin: the built-in 4096")
        t |> equal(1024l, resident_arm_floor(1024l, 0l), "the env floor stands in for the built-in one")
        t |> equal(256l, resident_arm_floor(100l, 0l), "an env floor under 256 reads 256")
        t |> equal(648l, resident_arm_floor(0l, 648l), "a pin under the floor is the floor: the pin is the shape the caller serves")
        t |> equal(4096l, resident_arm_floor(0l, 8192l), "a pin over the floor leaves the floor alone")
        t |> equal(512l, resident_arm_floor(1024l, 512l), "a pin under the env floor wins over it too")
        t |> equal(256l, resident_arm_floor(0l, 64l), "a pin under 256 reads 256")
    }
    t |> run("a strict pin is the floor whatever its size: the mirror holds the context asked or the driver declines") @(t : T?) {
        t |> equal(131072l, resident_arm_floor(0l, 131072l, true), "a strict pin over the built-in floor raises the floor to the pin")
        t |> equal(131072l, resident_arm_floor(1024l, 131072l, true), "... over the env floor too")
        t |> equal(648l, resident_arm_floor(0l, 648l, true), "a strict pin under the floor is the floor, as a plain pin is")
        t |> equal(256l, resident_arm_floor(0l, 64l, true), "a strict pin under 256 reads 256")
        t |> equal(4096l, resident_arm_floor(0l, 0l, true), "strict with no pin asks nothing: the built-in floor")
        t |> equal(4096l, resident_arm_floor(0l, 131072l, false), "control: the same pin, not strict, leaves the floor alone")
    }
    t |> run("a plain MoE the card does not hold whole moves its first routed layers' experts to the host: the fewest that make the rest fit") @(t : T?) {
        var need <- [for (_i in range(6)); 100l]
        t |> equal(0l, resident_host_layers_fit(need, 600l), "every layer's experts fit: none to the host")
        t |> equal(1l, resident_host_layers_fit(need, 599l), "one byte short: the first layer goes")
        t |> equal(3l, resident_host_layers_fit(need, 300l), "half the room: half the layers")
        t |> equal(6l, resident_host_layers_fit(need, 0l), "no room: every layer")
        t |> equal(-1l, resident_host_layers_fit(need, -1l), "the dense planes alone pass the card: no count makes it fit")
        var uneven <- [for (i in range(3)); 100l * int64(i + 1)]
        t |> equal(1l, resident_host_layers_fit(uneven, 500l), "the first layers go first whatever their size: 200 + 300 fit after 100 leaves")
        delete need
        delete uneven
    }
    t |> run("the hot expert pool yields to a context asked whole: the mirror's floor is the plan's context, not 32768") @(t : T?) {
        t |> equal(131072l, resident_hot_floor_ctx(131072l, true), "a strict 131072 keeps every position before a slot is counted")
        t |> equal(32768l, resident_hot_floor_ctx(131072l, false), "unasked, the pool keeps the 32768 floor and takes the rest")
        t |> equal(8192l, resident_hot_floor_ctx(8192l, false), "a plan under the floor keeps its own context either way")
        t |> equal(8192l, resident_hot_floor_ctx(8192l, true), "a strict context under the built-in floor is the pool's floor too")
    }
    t |> run("the binding range over the regions bounds a strict context too, and a strict context past it is a decline, not a shorter mirror") @(t : T?) {
        t |> equal(resident_region_ctx(131072l, 196608l, 4l), 49152l, "four regions share one binding's 196608 positions: 49152 each, under the 131072 asked")
        t |> equal(resident_region_ctx(131072l, 196608l, 1l), 131072l, "one region holds the ask")
        t |> success(resident_region_ctx_declines(131072l, 49152l, true), "asked whole, the 49152 a region holds is a decline")
        t |> success(!resident_region_ctx_declines(131072l, 49152l, false), "unasked, the plan takes the 49152 and says so")
        t |> success(!resident_region_ctx_declines(131072l, 131072l, true), "a region that holds the ask declines nothing")
    }
}

[test]
def test_routed_slots_fit(t : T?) {
    t |> run("the routed planes' slot cap: every row's picks fit the 128 slots or the N-row command declines the model") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            t |> success(rd_routed_slots_fit(16l, 8l), "sixteen rows of eight picks fill the slots")
            t |> success(!rd_routed_slots_fit(16l, 9l), "sixteen rows of nine picks pass them")
            t |> success(rd_routed_slots_fit(12l, 10l), "twelve rows of ten picks fit (the 512-expert trunk's twelve-row step)")
            t |> success(rd_routed_slots_fit(4l, 32l), "four rows of thirty-two picks fit")
            t |> success(!rd_routed_slots_fit(5l, 32l), "five rows of thirty-two pass them")
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

let private PROBE_OWNED = "mtp_seat_probe"   //! a fake decode override whose seats carry an owner the probe answers
let private PROBE_UNOWNED = "mtp_seat_probe_unowned"   //! ... and one whose seats carry none
var private g_probe_owns = false
var private g_probe_seat_calls = 0l

[unused_argument(t)]
def private probe_owner(t : Model) : bool => g_probe_owns

[unused_argument(t)]
def private probe_rows(t : Model) : int64 {
    g_probe_seat_calls++
    return 5l
}

[unused_argument(t, s, pos, k)]
def private probe_cold(t : Model; s : Session; pos, k : int64) : string {
    g_probe_seat_calls++
    return "the probe's own cold reason"
}

//! how many seat reads reached the probe's seats, how many round entries opened the CPU round, the batch tick's answer, and the
//! two seat answers the engine read back (the verify-rows cap and the cold reason)
struct private SeatDispatch {
    seat : int64
    cpu : int64
    batch : int64
    rows : int64
    cold : string
}

//! the round entries and the seat reads once each under the fake override `name` (its owner answering `owns`) on a Model shell;
//! the session carries a parked token, so the CPU round's opening refuses it before any forward and counts as a CPU call
[cold_path]
def private seat_dispatch(name : string; owns : bool) : SeatDispatch {
    for (nm in [PROBE_OWNED, PROBE_UNOWNED]) {
        register_decode_override(nm, @@decode_declines)
        register_mtp_verify_rows_override(nm, @@probe_rows)
        register_mtp_cold_override(nm, @@probe_cold)
    }
    register_mtp_seat_owner(PROBE_OWNED, @@probe_owner)
    g_probe_owns = owns
    g_probe_seat_calls = 0l
    let prev = active_decode_override()
    select_decode_override(name)
    var m = Model()
    var s : Session
    s.mtp_pre_drawn = true
    var out : SeatDispatch
    try {
        var acc = 0l
        mtp_spec_eval(m, s, 1l, acc)
    } recover {
        out.cpu++
    }
    try {
        mtp_spec_round(m, s, 1l)
    } recover {
        out.cpu++
    }
    var sessions : array<Session?>
    let toks : array<int64>
    var accepted : array<int64>
    out.batch = mtp_spec_eval_batch(m, sessions, toks, accepted)
    out.rows = mtp_verify_rows_cap(m)
    out.cold = mtp_round_cold_seat(m, s, 1l, 1l)
    select_decode_override(prev)
    for (nm in [PROBE_OWNED, PROBE_UNOWNED]) {
        unregister_decode_override(nm)
    }
    out.seat = g_probe_seat_calls
    delete s
    delete m
    delete accepted
    return out
}

[test]
def test_mtp_seat_owner(t : T?) {
    t |> run("the speculative seats serve only a model the active decode override's owner claims; the round itself is the common one on every home") @(t : T?) {
        let refused = seat_dispatch(PROBE_OWNED, false)
        t |> equal(refused.seat, 0l, "an owner refusing the model reaches no seat")
        t |> equal(refused.cpu, 2l, "mtp_spec_eval and mtp_spec_round open the common round")
        t |> equal(refused.batch, 0l, "the batch tick steps each stream alone")
        t |> equal(refused.rows, 0l, "the verify-rows cap reads no cap")
        t |> equal(refused.cold, "", "the cold seat answers no reason")
        let unowned = seat_dispatch(PROBE_UNOWNED, true)
        t |> equal(unowned.seat, 0l, "seats registered with no owner are never read")
        t |> equal(unowned.cpu, 2l, "... and the common round runs")
        t |> equal(unowned.rows, 0l, "... reading no cap")
        let owned = seat_dispatch(PROBE_OWNED, true)
        t |> equal(owned.seat, 2l, "an owner claiming the model hands every seat read to its seat")
        t |> equal(owned.cpu, 2l, "the round entries still open the common round - no home registers a round of its own")
        t |> equal(owned.batch, 0l, "the batch tick steps each stream through the common round")
        t |> equal(owned.rows, 5l, "the verify-rows cap is the seat's answer")
        t |> equal(owned.cold, "the probe's own cold reason", "the cold reason is the seat's answer")
    }
}

//! the CPU round's opening on a session carrying an unconsumed landed pick, under the unowned probe override
[cold_path]
def private round_refuses_pick() : bool {
    register_decode_override(PROBE_UNOWNED, @@decode_declines)
    let prev = active_decode_override()
    select_decode_override(PROBE_UNOWNED)
    var m = Model()
    var s : Session
    land_pick(s, 3l)
    var refused = false
    try {
        var acc = 0l
        mtp_spec_eval(m, s, 1l, acc)
    } recover {
        refused = true
    }
    select_decode_override(prev)
    delete s
    delete m
    return refused
}

[test]
def test_land_pick(t : T?) {
    t |> run("a landed pick is the next sample_'s answer in place of a logits row, consumed once; a second landing before that, and a round opened on it, refuse") @(t : T?) {
        var s : Session
        s.logits |> resize(4)
        s.logits[2] = 9.0
        t |> equal(sample_(s, SamplingParams()), 2l, "with no pick landed the bare argmax reads the logits row")
        land_pick(s, 3l)
        t |> success(s.pick_ready, "the landing arms the pick")
        t |> equal(sample_(s, SamplingParams()), 3l, "the next sample_ returns the pick, whatever the logits row holds")
        t |> success(!s.pick_ready, "... and consumes it")
        t |> equal(sample_(s, SamplingParams()), 2l, "the sample_ after it reads the logits row again")
        land_pick(s, 5l)
        var refused = false
        try {
            land_pick(s, 6l)
        } recover {
            refused = true
        }
        t |> success(refused, "a second landing before a sample_ is refused: the first pick would be lost")
        t |> equal(s.pick_tok, 5l, "the refused landing left the first pick standing")
        delete s
        t |> success(round_refuses_pick(), "the CPU round's opening refuses a session whose landed pick nobody sampled")
    }
}

let private ROUND_SEATS = "mtp_round_probe"   //! a fake decode override claiming every model, with a draft, a verify and a rollback seat
let private ROUND_SEATS_NO_ROLLBACK = "mtp_round_probe_no_rollback"   //! ... and one with the draft and verify seats alone
let private ROUND_DIM = 4l
let private ROUND_CONV = 6l   //! the shell's conv history floats: what a snapshot must size its buffer to
let private ROUND_STATE = 8l
let private ROUND_TOKEN = 3l
var private g_round_pick = 5l   //! the fake draft's token
var private g_round_truth = 5l   //! the fake verify's row-0 pick: the draft accepts, any other token rejects
var private g_round_rollback_ok = true
var private g_round_draft_calls = 0l
var private g_round_verify_calls = 0l
var private g_round_rollback_calls = 0l

[unused_argument(t)]
def private round_owner(t : Model) : bool => true

[unused_argument(t, s, token, pos, pick_only)]
def private round_draft(t : Model; var s : Session; token, pos : int64; pick_only : bool) : int64 {
    g_round_draft_calls++
    return g_round_pick
}

//! the fake verify: serves, every row's pick landed (row 0's `g_round_truth`), every row's hidden a constant row in `mtp_hrows`
//! (row 0's copied to `mtp_h0` as the real seat does), the carry's watermark past its rows as the real seat leaves it
[unused_argument(toks, pick_only)]
def private round_verify(t : Model; var s : Session; toks : array<int64>; n, pos : int64; pick_only : bool) : bool {
    g_round_verify_calls++
    s.mtp_h_pos1 = pos + n
    s.mtp_picks |> resize(n)
    s.mtp_picks[0] = g_round_truth
    for (i in range64(1l, n)) {
        s.mtp_picks[i] = 9l + i
    }
    let dim = t.config.dim
    s.mtp_hrows |> reserve(n * dim)
    s.mtp_hrows |> resize(n * dim)
    for (i in range64(n * dim)) {
        s.mtp_hrows[i] = 0.25
    }
    s.mtp_h0 |> resize(dim)
    copy_floats(s.mtp_hrows, 0l, s.mtp_h0, 0l, dim)
    return true
}

[unused_argument(t, s, pos, a)]
def private round_rollback(t : Model; var s : Session; pos, a : int64) : bool {
    g_round_rollback_calls++
    return g_round_rollback_ok
}

//! a NextN shell a round can open on: one head, a small width, room for the round's two rows; `recurrent` sets layer 0 recurrent
def private round_shell(recurrent : bool) : Model {
    var m = Model()
    m.config.n_layer_nextn = 1l
    m.config.n_layers = 1l
    m.config.dim = ROUND_DIM
    m.config.seq_len = 64l
    m.config.recr_mask = recurrent ? 1ul : 0ul
    return <- m
}

//! a session at position 1 with the carry's watermark there, the carry and the verify batch sized, and a recurrent state
//! sized so a snapshot has something to copy
def private round_session() : Session {
    var s <- Session(n_past = 1l, mtp_h_pos1 = 1l)
    s.mtp_h |> reserve(ROUND_DIM)
    s.mtp_h |> resize(ROUND_DIM)
    s.mtp_vbatch |> resize(2l)
    s.dn_conv_state |> reserve(ROUND_CONV)
    s.dn_conv_state |> resize(ROUND_CONV)
    s.dn_state |> reserve(ROUND_STATE)
    s.dn_state |> resize(ROUND_STATE)
    return <- s
}

//! the fake seats registered under both names, the counters zeroed, the round's outcome dialed: the draft `pick`, the verify's
//! row-0 `truth`, the rollback seat's answer
def private round_seats_arm(pick, truth : int64; rollback_ok : bool) {
    for (nm in [ROUND_SEATS, ROUND_SEATS_NO_ROLLBACK]) {
        register_decode_override(nm, @@decode_declines)
        register_mtp_seat_owner(nm, @@round_owner)
        register_mtp_draft_override(nm, @@round_draft)
        register_mtp_verify_override(nm, @@round_verify)
    }
    register_mtp_rollback_override(ROUND_SEATS, @@round_rollback)
    g_round_pick = pick
    g_round_truth = truth
    g_round_rollback_ok = rollback_ok
    g_round_draft_calls = 0l
    g_round_verify_calls = 0l
    g_round_rollback_calls = 0l
}

//! one round of `ROUND_TOKEN` on `s` under the fake override `name`: the panic's text, "" where none fired
[cold_path]
def private round_under(name : string; var s : Session; m : Model; var hit : bool&) : string {
    let prev = active_decode_override()
    select_decode_override(name)
    let why = panic_text() $ {
        var acc = 0l
        hit = mtp_spec_eval(m, s, ROUND_TOKEN, acc)
    }
    select_decode_override(prev)
    return why
}

[test]
def test_mtp_round_panics(t : T?) {
    t |> run("the round's fail-closed exits on a Model shell: a pick ask beside a sampled walk panics before any draft; a rollback seat declining after its verify seat served panics naming the override") @(t : T?) {
        round_seats_arm(5l, 7l, false)
        var m <- round_shell(true)
        var s <- round_session()
        var sp = SamplingParams(temp = 0.7)
        s.spec_params = unsafe(addr(sp))
        s.pick_asked = true
        var hit = false
        let asked = round_under(ROUND_SEATS, s, m, hit)
        to_log(LOG_INFO, "pick ask beside a sampled walk: {empty(asked) ? "no panic" : asked}\n")
        t |> success(contains(asked, "asked for a pick") && contains(asked, "sample"), "a pick ask beside a sampled walk panics, naming both: '{asked}'")
        t |> equal(g_round_draft_calls, 0l, "the panic fired before the draft")
        s.spec_params = null
        s.pick_asked = false
        let declined = round_under(ROUND_SEATS, s, m, hit)
        to_log(LOG_INFO, "rollback seat declining after a served verify: {empty(declined) ? "no panic" : declined}\n")
        t |> success(contains(declined, "rollback seat declined a reject after its verify seat served") && contains(declined, ROUND_SEATS), "the reject panics naming the override whose rollback seat declined: '{declined}'")
        t |> equal(g_round_verify_calls, 1l, "the verify seat served the round")
        t |> equal(g_round_rollback_calls, 1l, "the rollback seat was asked once and declined")
        delete s
        delete m
    }
}

[test]
def test_mtp_verify_snapshot(t : T?) {
    t |> run("mtp_verify_two's snapshot on a Model shell: a recurrent model with a verify seat that serves and no rollback seat takes the CPU snapshot before the seat, a non-recurrent one skips it, and a rollback seat that serves takes none - the reject rolling back through it and re-seeding the carry from row 0") @(t : T?) {
        round_seats_arm(5l, 5l, true)
        var m <- round_shell(true)
        var s <- round_session()
        var hit = false
        let why = round_under(ROUND_SEATS_NO_ROLLBACK, s, m, hit)
        t |> success(empty(why), "the round completed ({why})")
        t |> success(hit, "the fake verify's row-0 pick is the draft: the round accepted")
        t |> equal(s.n_past, 3l, "an accept commits the token and the draft")
        t |> equal(long_length(s.mtp_snap.conv), ROUND_CONV, "recurrent, no rollback seat: the CPU snapshot ran before the seat - its conv buffer sized to the history")
        t |> equal(long_length(s.mtp_snap.state), ROUND_STATE, "... and its state buffer to the state")
        t |> equal(g_round_verify_calls, 1l, "the verify seat served")
        delete s
        delete m
        var plain <- round_shell(false)
        var ps <- round_session()
        let why_plain = round_under(ROUND_SEATS_NO_ROLLBACK, ps, plain, hit)
        t |> success(empty(why_plain) && hit, "the non-recurrent round completed and accepted ({why_plain})")
        t |> equal(long_length(ps.mtp_snap.conv), 0l, "non-recurrent: no snapshot - nothing to restore")
        round_seats_arm(5l, 7l, true)
        let why_reject = round_under(ROUND_SEATS_NO_ROLLBACK, ps, plain, hit)
        t |> success(empty(why_reject) && !hit, "the non-recurrent reject completed ({why_reject})")
        t |> equal(g_round_rollback_calls, 0l, "a non-recurrent reject never reaches a rollback seat")
        t |> equal(ps.n_past, 4l, "the reject commits the token alone: one row past the accepted round's position")
        delete ps
        delete plain
        round_seats_arm(5l, 7l, true)
        var rm <- round_shell(true)
        var rs <- round_session()
        let why_rolled = round_under(ROUND_SEATS, rs, rm, hit)
        t |> success(empty(why_rolled) && !hit, "the recurrent reject through a serving rollback seat completed ({why_rolled})")
        t |> equal(g_round_rollback_calls, 1l, "the rollback seat served the reject")
        t |> equal(long_length(rs.mtp_snap.conv), 0l, "a rollback seat behind the verify seat: no snapshot was taken")
        t |> equal(rs.n_past, 2l, "the reject commits the token alone")
        t |> equal(rs.mtp_h_pos1, 2l, "the carry's watermark sits at the token's row")
        t |> equal(rs.mtp_h[0], 0.25, "the carry re-seeds from the verify's row-0 hidden")
        delete rs
        delete rm
    }
}

//! the shape arms that keep a short window off the token command's peel, one at a time on a dense shell that fits
[test]
def test_peel_shape_arms(t : T?) {
    t |> run("the peel's shape arms name the hyper-connection chain, the per-layer embedding and the recurrent state apart; a NextN head fits") @(t : T?) {
        var fits <- shell(2l, 0ul)
        t |> equal(rdec_peel_shape_why(fits.config), "", "a dense shell fits the peel")
        var nextn <- shell(2l, 0ul)
        nextn.config.n_layer_nextn = 1l
        t |> equal(rdec_peel_shape_why(nextn.config), "", "a NextN head fits the peel: the token commands draft off the head")
        var hc <- shell(2l, 0ul)
        hc.config.hyper_conn = true
        t |> equal(rdec_peel_shape_why(hc.config), "a hyper-connection chain", "a hyper-connection chain keeps the chain")
        var ple <- shell(2l, 0ul)
        ple.config.n_embd_per_layer = 64l
        t |> equal(rdec_peel_shape_why(ple.config), "", "a per-layer embedding fits the peel: the token commands carry the side rows")
        var recr <- shell(2l, 1ul)
        let recr_why = rdec_token_seq_installed() ? "" : "a recurrent model (no in-order rows command installed)"
        t |> equal(rdec_peel_shape_why(recr.config), recr_why, "a recurrent state keeps the chain unless the in-order rows command is installed")
        t |> success(!gpu_device_prefill_segments_(fits), "with no driver armed on it, no model's device-home tick takes the segmented window")
        delete fits
        delete nextn
        delete hc
        delete ple
        delete recr
    }
}

//! gemma-4-26B-A4B's attention geometry on a shell: thirty layers, every sixth global (two kv heads of 512), the other twenty-five
//! sliding over 1024 positions (eight kv heads of 256)
def private gemma4_26b_shell() : Model {
    var m <- shell(30l, 0ul)
    m.config.sliding_window = 1024l
    m.config.swa_pattern = 6l
    m.config.head_size = 512l
    m.config.n_kv_heads = 2l
    m.config.head_size_swa = 256l
    m.config.n_kv_heads_swa = 8l
    m.config.n_heads = 16l
    return <- m
}

let private HOUSE_26B_FLAT_PLAN_MB = 39949l   //! the 26B UD-IQ3_XXS's strict 32768 x 4 plan with every mirror row flat, read on the pod's 32 GB card
let private HOUSE_32GB_ROOM_MB = 28527l       //! that card's plan room, read in the same load log

let private RING_CELL_WINDOW = 512l   //! the ring cells' prompt window: the rows a sliding ring holds past its window

//! the sliding layers' rings in the plan (`../ARCHITECTURE_GPU_KV_RING.md#kv-sliding-ring`): on the 26B at 32768 positions over four
//! regions - the house shape - a sliding layer holds its window and the window chain's rows (pinned at 512 here), rounded to 64, and a
//! global layer every position; the flat pin charges every layer the whole context, and the ring bytes are what the context does not
//! scale; the pod's flat plan less the flat mirror plus the ringed one fits that card's room
[test]
def test_resident_ring_mirror(t : T?) {
    t |> run("the 26B's mirror at 32768 x 4 under the rings: 1536 rows a sliding layer, every position a global one") @(t : T?) {
        var m <- gemma4_26b_shell()
        let window_was = pf_window_rows_pin()
        set_pf_window_rows_pin(RING_CELL_WINDOW)
        with_gpu_kv_ring(1) {
            t |> equal(kv_ring_rows(m.config, 0l, 32768l, 512l), 1536l, "a sliding layer: its 1024-position window and the 512 rows a window writes")
            t |> equal(kv_ring_rows(m.config, 5l, 32768l, 512l), 0l, "a global layer keeps every position")
            t |> equal(kv_ring_rows(m.config, 0l, 1536l, 512l), 0l, "a ring that would reach the context keeps every position")
            t |> equal(kv_ring_rows(m.config, 0l, 32768l, 100l), 1152l, "the slack rounds the ring up to the tiles' 64-row step")
            t |> equal(resident_layer_ring(m, 0l, 32768l), 1536l, "the plan's ring: the window chain's 512 rows of slack")
            let sliding_units = 25l * 1536l * 2048l
            let global_units = 5l * 32768l * 1024l
            t |> equal(resident_ring_bytes(m, 32768l, KVDtype.f16, 4l), 2l * 4l * sliding_units * 2l, "the rings' bytes: both sides of every region, f16")
            let ringed = resident_mirror_bytes(m, 32768l, KVDtype.f16, 4l)
            t |> success(ringed >= 2l * 4l * (sliding_units + global_units) * 2l, "the ringed mirror charges every row it seats ({ringed >> 20l} MB)")
            set_gpu_kv_ring(0)
            let flat = resident_mirror_bytes(m, 32768l, KVDtype.f16, 4l)
            t |> equal(resident_ring_bytes(m, 32768l, KVDtype.f16, 4l), 0l, "control: the flat pin rings nothing")
            t |> success(flat >= 2l * 4l * 32768l * (25l * 2048l + 5l * 1024l) * 2l, "the flat mirror charges the whole context on every layer ({flat >> 20l} MB)")
            to_log(LOG_INFO, "26B at 32768 x 4, f16: the ringed mirror {ringed >> 20l} MB against {flat >> 20l} MB flat\n")
            t |> success(ringed * 7l < flat, "the rings cut the mirror past seven times ({ringed >> 20l} MB against {flat >> 20l} MB)")
            let rest = HOUSE_26B_FLAT_PLAN_MB - (flat >> 20l)
            t |> success((ringed >> 20l) + rest < HOUSE_32GB_ROOM_MB,
                "the house plan fits the 32 GB card's room under the rings: {(ringed >> 20l) + rest} MB of {HOUSE_32GB_ROOM_MB} (flat {HOUSE_26B_FLAT_PLAN_MB})")
        }
        set_pf_window_rows_pin(window_was)
        delete m
    }
}

//! the ring pin sits outside a trimmed vulkan lane's identity (`../ARCHITECTURE_IMAGE_VULKAN.md#image-vulkan-mint`): it moves the
//! mirror the plan charges, the plan a lane's map asks again, and no byte of the image - so a lane mapped by name declines under
//! the run's knobs and names the pin among the knobs its identity leaves out
[test]
def test_trim_lane_ring_pin(t : T?) {
    t |> run("the ring pin moves the plan a trimmed lane's map asks again, never the lane's identity, and the map's decline names it") @(t : T?) {
        var m <- gemma4_26b_shell()
        var ids : array<string>
        var mirrors : array<int64>
        let window_was = pf_window_rows_pin()
        set_pf_window_rows_pin(RING_CELL_WINDOW)
        for (pin in [1, 0]) {
            with_gpu_kv_ring(pin) {
                ids |> push(image_identity(moe_gpu_bake_tag()))
                mirrors |> push(resident_mirror_bytes(m, 32768l, KVDtype.f16, 4l))
            }
        }
        set_pf_window_rows_pin(window_was)
        t |> equal(ids[0], ids[1], "the lane's identity reads the same under either pin: the image holds no K/V row")
        t |> success(mirrors[0] * 7l < mirrors[1], "the plan charges the ringed mirror under one pin and the flat one under the other ({mirrors[0] >> 20l} MB against {mirrors[1] >> 20l} MB)")
        var terr = ""
        let path = path_join(temp_directory(terr), "dasllama_trim_lane_ring_{ref_time_ticks()}.vulkan.dlim")
        let route = gpu_resident_route_lever()
        set_gpu_resident_route(false)   //! the route off: the decline is this run's on every box, a device or none
        for (trimmed in [true, false]) {
            var lane = Model()
            lane.arch = "llama"
            lane.quant = QuantMode.q8
            lane.config.n_layers = 2l
            lane.planes_trimmed = trimmed
            t |> success(save_model_image(lane, path), "a {trimmed ? "trimmed" : "planar"} shell's image saves")
            var mapped = false
            let why = panic_text() $ {
                var back = Model()
                mapped = load_model_image(path, back)
                delete back
            }
            if (trimmed) {
                t |> success(find(why, "is a trimmed vulkan lane") >= 0 && find(why, "DASLLAMA_GPU_KV_RING / set_gpu_kv_ring") >= 0
                    && find(why, "sit outside the lane's identity") >= 0, "the trimmed lane's map declines under this run's knobs, naming the ring pin: {why}")
            } else {
                t |> success(empty(why) && mapped, "control: the planar shell maps with no decline ({why})")
            }
            delete lane
            remove(path)
        }
        set_gpu_resident_route(route)
        delete ids
        delete mirrors
        delete m
    }
}

[test]
def test_kv_ring_floor_check(t : T?) {
    t |> run("a call at or past the ring floor continues, one under it panics, and a start at 0 rebuilds the history and clears the floor") @(t : T?) {
        var s : Session
        s.kv_ring_floor = 6l
        t |> success(empty(panic_text() $ { kv_ring_floor_check(s, 8l) }), "a call past the floor continues")
        t |> success(empty(panic_text() $ { kv_ring_floor_check(s, 6l) }), "a call at the floor continues")
        t |> equal(s.kv_ring_floor, 6l, "and leaves the floor standing")
        t |> success(!empty(panic_text() $ { kv_ring_floor_check(s, 3l) }), "a call under the floor panics: the ring holds no window under it")
        t |> success(empty(panic_text() $ { kv_ring_floor_check(s, 0l) }), "a start at 0 continues nothing")
        t |> equal(s.kv_ring_floor, 0l, "and clears the floor")
        t |> success(empty(panic_text() $ { kv_ring_floor_check(s, 3l) }), "so the same call at 3 continues the rebuilt history (the control)")
        delete s
    }
}

[test]
def test_resident_host_layers_pin(t : T?) {
    t |> run("the host-layers pin: set reads back, a negative lifts it, zero pins the plan's own count, put back as found") @(t : T?) {
        let pin0 = gpu_host_layers_pin_()
        set_gpu_host_layers_(3l)
        t |> equal(gpu_host_layers_pin_(), 3l, "a pin of 3 reads back")
        set_gpu_host_layers_(-7l)
        t |> equal(gpu_host_layers_pin_(), HOST_LAYERS_UNPINNED, "a negative ask lifts the pin")
        set_gpu_host_layers_(0l)
        t |> equal(gpu_host_layers_pin_(), 0l, "zero is a pin - the plan's own count over the environment's ask")
        set_gpu_host_layers_(pin0)
        t |> equal(gpu_host_layers_pin_(), pin0, "the pin put back reads as found")
    }
    t |> run("the host-layer count an ask puts to the plan: none for no ask, capped at the routed layers where the plain split form takes the model, none where it does not") @(t : T?) {
        var c = Config()
        c.n_expert = 64l
        c.n_expert_used = 4l
        let plain = rdec_hc_installed()   //! the split seats sit on the Vulkan tier; without them no ask applies
        t |> equal(resident_host_layers_want(c, 24l, 0l), -1l, "no ask leaves the plan its own count")
        t |> equal(resident_host_layers_want(c, 24l, 4l), plain ? 4l : -1l, "an ask under the routed count passes whole where the split form takes the model")
        t |> equal(resident_host_layers_want(c, 24l, 40l), plain ? 24l : -1l, "an ask past the routed count is capped at it")
        var hc = Config()
        hc.n_expert = 512l
        hc.n_expert_used = 10l
        hc.hyper_conn = true
        t |> equal(resident_host_layers_want(hc, 24l, 4l), -1l, "a hyper-connection model takes no ask: every routed layer serves on the host regardless")
        let dense = Config()
        t |> equal(resident_host_layers_want(dense, 0l, 4l), -1l, "a dense model takes no ask")
    }
}

//! the E-series pre-step's shape gates: Metal's `metal_ple_dev_ok` serves any model dim on either projection form, any ple width,
//! and a table bound on its own format's blocks, naming a table off them; Vulkan's bf16 projection takes the word-pair stamp at an
//! even dim and base and the halfword stamp off them (`gk_bf16_pairs`)
[test]
def test_ple_proj_dim_gates(t : T?) {
    t |> run("the pre-step's shapes on each home") @(t : T?) {
        var m = Model()
        m.config.n_layers = 2l
        m.config.n_embd_per_layer = 256l
        m.metal_blob = true
        m.ple_model_bf16 = true
        m.config.dim = 1536l
        t |> success(metal_ple_dev_ok(m), "the bf16 rows serve an even dim (the E2B's 1536)")
        m.config.dim = 1537l
        t |> success(metal_ple_dev_ok(m), "... and an odd one")
        m.ple_model_bf16 = false
        t |> success(metal_ple_dev_ok(m), "the f32 rows serve a dim off the float4 lattice")
        m.config.n_embd_per_layer = 2048l
        t |> success(metal_ple_dev_ok(m), "a ple width past 1024 serves")
        m.config.n_embd_per_layer = 48l
        t |> success(metal_ple_dev_ok(m), "... and one off the 32-lane step")
        m.config.n_embd_per_layer = 0l
        t |> equal(metal_ple_pre_unserved(m), "", "a model with no PLE has nothing to decline")
        t |> success(!metal_ple_dev_ok(m), "... and no device pre-step")
        m.config.n_embd_per_layer = 48l
        m.ple_emb_fmt = KqFmt.q51
        m.ple_emb_off = 96l
        t |> success(metal_ple_dev_ok(m), "a per-32 table off a superblock serves on its own blocks (96 elements a row at 96)")
        m.ple_emb_off = 112l
        let off = metal_ple_pre_unserved(m)
        t |> success(find(off, "PLE table") >= 0 && find(off, "q51") >= 0 && find(off, "112") >= 0 && find(off, "32-element") >= 0,
            "... a base off them is named by site, format, offset and block: '{off}'")
        t |> success(!metal_ple_dev_said("decode", m), "the driver's verdict is the predicate's, its reason logged")
        m.ple_emb_off = 0l
        m.config.n_embd_per_layer = 40l
        t |> success(find(metal_ple_pre_unserved(m), "80 elements a row") >= 0, "... and a row off them")
        m.ple_emb_fmt = KqFmt.k4
        m.config.n_embd_per_layer = 128l
        t |> success(metal_ple_dev_ok(m), "a superblock table on its superblocks serves (256 elements a row)")
        m.ple_emb_off = 32l
        t |> success(find(metal_ple_pre_unserved(m), "256-element") >= 0, "... off them is named at the superblock")
        t |> success(gk_bf16_pairs(1536l, 0l), "the bf16 rows step word pairs at an even dim (the E2B's 1536)")
        t |> success(!gk_bf16_pairs(1537l, 0l), "... and halfwords at an odd one")
        t |> success(!gk_bf16_pairs(1536l, 3l), "... or off an odd base")
    }
    t |> run("the window's projection names an f16 panel past the device's storage range") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            var shell = GpuState(coopmat_mode = COOPMAT_CM2, msr = 1l << 20l)   //! a cm2 device shell whose storage range is 1 MiB
            var live = g_gpu
            g_gpu = unsafe(addr(shell))
            let dim = int64(F16G_BK) * F16G_SPLIT
            t |> equal(ple_proj_gemv_why(dim, 256l, 1l << 20l), "", "the control: a panel at the range takes the tile")
            let past = ple_proj_gemv_why(dim, 256l, (1l << 20l) + 2l)
            t |> success(find(past, "f16 panel ({(1l << 20l) + 2l} bytes) passes the device's storage range {1l << 20l}") >= 0,
                "a panel two bytes past the range names both: '{past}'")
            t |> success(find(ple_proj_gemv_why(dim + 1l, 256l, (1l << 20l) + 2l), "no f16 tile serves it") >= 0,
                "an off-lattice width names the tile first")
            g_gpu = live
        } else {
            t |> skip("no dasVulkan module on this build")
        }
    }
}

//! the window's f32 per-layer rows: the staged tile on its k step under the knob, the scalar tile off either; the gate's k chunks
//! fit the split-k scratch; the resident projection's bf16 rows take the halfword stamp at an odd dim
[test]
def test_ple_f32_tile_gates(t : T?) {
    t |> run("the window's f32 per-layer rows take the staged tile only on its k step") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            var shell = GpuState(coopmat_mode = COOPMAT_CM2, has_coopmat = true, subgroup_size = 32l)   //! a KHR-capable shell at subgroup 32
            var live = g_gpu
            g_gpu = unsafe(addr(shell))
            t |> success(ple_f32_tile_ok(1536l, 256l, true), "the control: the E2B's dim and ple take the staged tile")
            t |> success(!ple_f32_tile_ok(1536l, 256l, false), "... and the knob off (DASLLAMA_VK_PLE_F32_TILE=0) keeps the scalar f32 tile")
            let roomy = 1l << 30l
            t |> equal(pf_f32_split(1536l, 512l, 256l, roomy), 6l, "the E2B gate's K 1536 splits six ways, a chunk of 256 each")
            t |> equal(pf_f32_split(2560l, 512l, 256l, roomy), 5l, "the E4B gate's K 2560 five ways, the most chunks of 512 cm2_chunk admits")
            t |> equal(pf_f32_split(256l, 512l, 256l, roomy), 1l, "the projection's K 256 runs whole")
            t |> equal(pf_f32_split(1536l, 512l, 256l, 3l * 512l * 256l * 4l), 3l, "a scratch of three 512-row partial planes cuts the E2B gate to three chunks")
            t |> equal(pf_f32_split(1536l, 512l, 256l, 512l * 256l * 4l), 1l, "... one plane's room runs it whole - never a split past the scratch")
            t |> success(!ple_f32_tile_ok(1544l, 256l, true), "a dim off the 32-deep k step keeps the scalar f32 tile (its last row read would pass the plane)")
            t |> success(!ple_f32_tile_ok(1536l, 48l, true), "... and so does a ple width off it")
            shell.subgroup_size = 64l
            t |> success(!ple_f32_tile_ok(1536l, 256l, true), "... and a wave64 device, which has no KHR kq tile")
            t |> success(!ple_proj_bf16_hw(1536l), "the resident projection's bf16 rows take the word-pair stamp at the E2B's even dim")
            t |> success(ple_proj_bf16_hw(1537l), "... and the halfword stamp at an odd one")
            g_gpu = live
        }
    }
}

//! the h128 coopmat attention twin serves a KHR device at subgroup 32 alone: its fragment owners are 32-lane subgroups
[test]
def test_h128_twin_gate(t : T?) {
    t |> run("the h128 twin declines a wave64 device") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            var shell = GpuState(coopmat_mode = COOPMAT_MM, has_coopmat = true, subgroup_size = 32l)   //! a KHR-only shell at subgroup 32
            var live = g_gpu
            g_gpu = unsafe(addr(shell))
            t |> success(h128_twin_on(), "the control: a coopmat device at subgroup 32 takes the twin")
            shell.subgroup_size = 64l
            t |> success(!h128_twin_on(), "a coopmat device at subgroup 64 (RDNA's wave64) takes the generic chunked tile")
            shell.subgroup_size = 32l
            shell.has_coopmat = false
            t |> success(!h128_twin_on(), "... and so does a device without coopmat")
            g_gpu = live
        }
    }
}

//! the subgroup pin: the class pipelines run at DASLLAMA_VK_SUBGROUP where the device's own width differs and it can pin, and
//! the width-keyed routes read the pinned width
[test]
def test_vk_subgroup_pin(t : T?) {
    t |> run("a device whose own width differs from the knob's pins where it can, and the routes read the pin") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init()) {
                t |> skip("no Vulkan device")
                return
            }
            let own = subgroup_properties(g_gpu.phys).subgroupSize
            let want = g_env_vulkan.vk_subgroup
            if (want <= 0l || uint(want) == own) {
                t |> equal(g_gpu.sg_pin, 0u, "the knob off, or a device already at its width ({int(own)}): no pin")
                t |> equal(g_gpu.subgroup_size, int64(own), "... and the tier runs at the device's own width")
            } elif (compute_subgroup_size_pinnable(g_gpu.phys, uint(want))) {
                t |> equal(g_gpu.sg_pin, uint(want), "a device at {int(own)} that can pin {want} pins it")
                t |> equal(g_gpu.subgroup_size, want, "... the tier runs at the pinned width")
                t |> equal(khr_kq_tile_on(), g_gpu.has_coopmat && want == 32l, "... and the KHR kq tile reads it")
                t |> equal(g_gpu.coopmat_mode != COOPMAT_SDOT4, g_gpu.has_coopmat && want == 32l,
                    "... and the coopmat mode resolved at the pinned width, not the device's own (a tile mode on a coopmat device)")
            } else {
                t |> equal(g_gpu.sg_pin, 0u, "a device at {int(own)} that cannot pin {want} runs at its own width")
                t |> equal(g_gpu.subgroup_size, int64(own), "... which the tier reads")
            }
        }
    }
}

//! the prefill window's chunk cap: one layer a submit on an integrated GPU (the amdgpu watchdog), the ramp's hold on a discrete one
[test]
def test_pf_chunk_cap(t : T?) {
    t |> run("an integrated GPU submits the window one layer at a time") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (g_env_vulkan.vk_chunk > 0l) {
                t |> skip("DASLLAMA_VK_CHUNK is set - the device's own cap is not what this process runs")
                return
            }
            var shell = GpuState(integrated = false)
            var live = g_gpu
            g_gpu = unsafe(addr(shell))
            t |> equal(pf_chunk_cap(), PF_CHUNK_MAX, "the control: a discrete GPU holds the ramp at {PF_CHUNK_MAX} layers")
            shell.integrated = true
            t |> equal(pf_chunk_cap(), 1l, "an integrated GPU caps a submit at one layer")
            g_gpu = live
        }
    }
}

//! the expert cache's profile beside the lane: a file the writer made reads back whole for its model's shape, and another
//! model's shape, a bad magic or a short file reads empty (the cache then keeps the first ids)
[test]
def test_expert_heat_profile(t : T?) {
    let path = path_join(test_temp_dir(), "expert_heat_probe.heat")
    var heat <- [for (i in range64(3l * 4l)); i * 7l + 1l]
    t |> success(expert_heat_write_to(path, 3l, 4l, heat), "the profile wrote")
    let back <- expert_heat_read(path, 3l, 4l)
    t |> equal(long_length(back), 12l, "the model's shape reads every pick")
    var same = long_length(back) == 12l
    for (b, h in back, heat) {
        same = same && b == h
    }
    t |> success(same, "... as written")
    t |> equal(long_length(expert_heat_read(path, 4l, 3l)), 0l, "another model's shape reads empty")
    t |> equal(long_length(expert_heat_read(path_join(test_temp_dir(), "expert_heat_none.heat"), 3l, 4l)), 0l, "a missing profile reads empty")
    fopen(path, "wb") $(f) {
        fwrite(f, [int64(0x12345678), 3l, 4l])
        fwrite(f, heat)
    }
    t |> equal(long_length(expert_heat_read(path, 3l, 4l)), 0l, "a bad magic reads empty")
    var short_heat <- [for (i in range64(5l)); i]
    t |> success(expert_heat_write_to(path, 3l, 4l, short_heat), "a short profile wrote")
    t |> equal(long_length(expert_heat_read(path, 3l, 4l)), 0l, "a short profile reads empty")
    remove(path)
}

[test]
def test_kv_codec_serving_rule(t : T?) {
    t |> run("the serving rule: tq4 steps down to q8_0 on a Q/K/V-biased attention and on a head it cannot rotate, to f16 where q8_0 holds no block either") @(t : T?) {
        var m <- shell(2l, 0ul)
        var why = ""
        t |> equal(kv_codec_served(m.config, KVDtype.tq4, why), KVDtype.tq4, "a 32-wide head without a bias serves tq4 as asked")
        t |> success(empty(why), "and names no step down")
        m.config.attn_qkv_bias = true
        t |> equal(kv_codec_served(m.config, KVDtype.tq4, why), KVDtype.q8_0, "a Q/K/V bias steps tq4 down to q8_0")
        t |> success(find(why, "Q/K/V bias") >= 0, "and names the bias ({why})")
        t |> equal(kv_codec_served(m.config, KVDtype.q8_0, why), KVDtype.q8_0, "q8_0 serves a biased attention")
        t |> success(empty(why) && empty(kv_codec_serving_why(m.config, KVDtype.f16)), "as does f16, neither naming a step")
        m.config.attn_qkv_bias = false
        m.config.head_size = 96l
        t |> equal(kv_codec_served(m.config, KVDtype.tq4, why), KVDtype.q8_0, "a 96-wide head steps tq4 down to q8_0")
        t |> success(find(why, "power-of-two") >= 0, "naming the rotation ({why})")
        m.config.head_size = 48l
        t |> equal(kv_codec_served(m.config, KVDtype.tq4, why), KVDtype.f16, "a 48-wide head holds no block codec: tq4 lands at f16")
        t |> equal(kv_codec_served(m.config, KVDtype.q8_0, why), KVDtype.f16, "and so does q8_0")
        delete m
    }
}

[test]
def test_baked_host_layers(t : T?) {
    t |> run("a lane's host-layer split read off its plan's marker entry: the host_split entry's woff, -1 where the lane carries none") @(t : T?) {
        let arena = int(VkBakeRole.arena)
        let iq4xs = int(KqFmt.iq4xs)
        var plan <- [VkPlanEntry(fmt = iq4xs, role = arena, woff = 3000l, n = 2048l, rows = 131072l),
                     VkPlanEntry(fmt = -1, role = int(VkBakeRole.host_split), woff = 18l, n = 0l, rows = 0l),
                     VkPlanEntry(fmt = iq4xs, role = arena, woff = 4000l, n = 2048l, rows = 131072l)]
        t |> equal(vk_baked_host_layers(plan), 18l, "the marker's woff is the split")
        var old <- [VkPlanEntry(fmt = iq4xs, role = arena, woff = 3000l, n = 2048l, rows = 131072l)]
        t |> equal(vk_baked_host_layers(old), -1l, "a lane minted before the marker answers -1")
        let none : array<VkPlanEntry>
        t |> equal(vk_baked_host_layers(none), -1l, "an empty plan answers -1")
        delete plan
        delete old
    }
}

[test]
def test_vulkan_tower_route_before_a_device(t : T?) {
    t |> run("the audio fronts' route answers off before a device is up - the scratch sizing asks it ahead of the arm") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (g_gpu != null) {
                t |> skip("a device is up in this process - the no-device answer is not reachable here")
                return
            }
            t |> success(!vulkan_tower_front_route(), "no device - no f16 GEMM feed, and no null dereference")
        } else {
            t |> skip("a build without dasVulkan has no Vulkan tower")
        }
    }
}
