options gen2
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_common
require dasllama/dasllama_gpu_resident
require dasllama/dasllama_gpu_tier   // install_moe_gpu_resident_bias - the bias seat a served qwen2 needs
require dasllama/dasllama_metal_shapes   // moe_metal_ok - the Metal routed block's admission, portable
require strings
require daslib/strings_boost
require daslib/enum_trait   // the format-set drift guard walks the whole KqFmt enum

[unused_argument(rows)]
def private test_upload_bias(rows : array<float>) {}

[unused_argument(rows)]
def private test_upload_sinks(rows : array<float>) : bool => true

// The whole-model driver's decline reasons, decided from a Config or a synthetic Model shell
// (no weights on disk): every feature and layer shape the driver has no arm for is named in
// the text a user reads, and a shape it serves yields no text at all.

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

def private says(s : string; part : string) : bool => find(s, part) >= 0

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
        t |> success(says(s, "gated attention without qk-norm"), "gated attention named: {s}")
        t |> success(says(s, "partial rotary (64 of 256) without qk-norm"), "partial rotary named with its widths: {s}")
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
}

[test]
def test_resident_layer_decline(t : T?) {
    t |> run("a recurrent layer without deltanet planes is named by number; the attention layers around it pass") @(t : T?) {
        var m <- shell(3l, 2ul)
        t |> equal(resident_layer_decline(m, 0l), "")
        t |> success(says(resident_layer_decline(m, 1l), "layer 1 lacks a deltanet plane"), "layer 1 named")
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
        t |> success(!says(sq, "kq-tagged") && !says(sq, "do not serve"), "a K-quant qkv/z pair is no format decline: '{sq}'")
        m.dnout_fmt <- [KqFmt.q8, KqFmt.k4, KqFmt.q8]
        let so = resident_layer_decline(m, 1l)
        t |> success(!says(so, "out plane"), "a K-quant out plane is no format decline: '{so}'")
        //! an inner width off the format's block leaves the out plane untileable - the decline names it
        m.config.ssm_d_inner = 100l
        t |> success(says(resident_layer_decline(m, 1l), "layer 1's deltanet out plane carries a format"), "an untileable out plane is named")
        delete m
    }
    t |> run("shared K/V, a missing plane and a head size off the kernels' shape each name the layer; a differing FFN width or a sliding window is served") @(t : T?) {
        var m <- shell(3l, 0ul)
        m.kv_src[2] = 1l
        t |> equal(resident_layer_decline(m, 2l), "", "a layer sharing the K/V of a layer below it is served (Q only, the donor's rows)")
        m.kv_src[1] = 2l
        t |> success(says(resident_layer_decline(m, 1l), "layer 1 shares layer 2's K/V, a layer above it"), "a donor above the sharing layer is named")
        m.kv_src[1] = 1l
        m.kv_src[2] = 2l
        m.wq_offs[0] = -1l
        t |> success(says(resident_layer_decline(m, 0l), "layer 0 lacks a q/k plane"), "missing plane named")
        m.wq_offs[0] = 0l
        m.wv_offs[0] = -1l
        t |> success(says(resident_layer_decline(m, 0l), "layer 0 lacks a v plane"), "a missing v plane is named where the layer is not V-from-K")
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
        t |> success(says(resident_layer_decline(m, 1l), "layer 1 head size 544"), "a head past the kernels' 512 is named by layer")
        m.config.head_size = 40l
        t |> success(says(resident_layer_decline(m, 1l), "layer 1 head size 40"), "a head off the 32-multiple is named by layer")
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
        t |> success(says(resident_layer_decline(m, 2l), "layer 2 lacks an expert plane"), "a layer past the dense lead without its expert planes is named")
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
        t |> success(!says(s, "router bias") && says(s, "an expert selection bias"), "the router bias is served (the select adds it), the expert selection bias named: {s}")
        c.moe_router_bias = false
        c.moe_exp_probs = false
        c.n_expert = 300l
        c.n_expert_used = 70l
        let g = resident_unserved_features(c)
        t |> success(says(g, "300 experts") && says(g, "70 routed experts"), "the kernels' reach named with the counts: {g}")
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

[test]
def test_moe_metal_expert_formats(t : T?) {
    t |> run("the Metal routed block admits exactly the expert plane formats it has kernels for; every other format declines") @(t : T?) {
        for (f in [KqFmt.q8, KqFmt.q51, KqFmt.k4, KqFmt.k5, KqFmt.k6, KqFmt.k3, KqFmt.q40, KqFmt.iq4xs, KqFmt.iq3s, KqFmt.iq3xxs, KqFmt.iq2s, KqFmt.iq2xs, KqFmt.iq2xxs]) {
            var m <- moe_shell(f)
            t |> success(moe_metal_ok(m), "{f} expert planes are served")
            delete m
        }
        for (f in [KqFmt.iq4nl, KqFmt.k2]) {
            var m <- moe_shell(f)
            t |> success(!moe_metal_ok(m), "{f} expert planes decline the routed block")
            delete m
        }
        var mixed <- moe_shell(KqFmt.k4)
        mixed.we2_fmt[1] = KqFmt.k2
        t |> success(!moe_metal_ok(mixed), "one Q2_K down plane declines a q4_K model")
        delete mixed
    }
    t |> run("the resident driver names a biased expert stack feeding a K-quant down plane by layer; a per-32 down plane draws no such decline") @(t : T?) {
        var kb <- moe_shell(KqFmt.k6)
        kb.config.moe_exps_bias = true
        t |> success(says(resident_layer_decline(kb, 0l), "layer 0's biased expert stacks feed a K-quant down plane"), "the biased K-quant down is named")
        delete kb
        var qb <- moe_shell(KqFmt.q8)
        qb.config.moe_exps_bias = true
        t |> success(!says(resident_layer_decline(qb, 0l), "biased expert stacks"), "a biased stack over a Q8_0 down plane draws no bias decline")
        delete qb
    }
    t |> run("the kernel roster and the site-alignment gate name ONE format set - every KqFmt reads the same on both") @(t : T?) {
        // No format the site gate admits is refused by the roster: the two lists are identical,
        // so a roster that answered true for everything is caught only by walking the WHOLE enum -
        // iq4nl and k2 are the members that must read false on both. A format added to one
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
def test_resident_binding_ctx(t : T?) {
    t |> run("the KV mirror's binding cap counts the attention layers' rows, whatever layer 0 is") @(t : T?) {
        let bcap = 4_294_967_295l   //! the 4 GiB maxStorageBufferRange of the NV/AMD drivers
        //! 32 layers, attention every fourth (3, 7, ..., 31), kv width 8 x 128 in f16: one side is 8 x 2048 B per position
        var hy <- shell(32l, 0x7777_7777ul)
        hy.config.n_kv_heads = 8l
        hy.config.head_size = 128l
        t |> success(layer_is_recurrent(hy.config, 0l) && layer_kv_dim(hy.config, 0l) == 0l, "layer 0 is recurrent: its kv width reads 0")
        t |> equal(resident_binding_ctx(hy.config, KVDtype.f16, bcap), 262143l, "the hybrid's ctx is the range over eight attention rows, not skipped for layer 0's zero width")
        t |> equal(resident_binding_ctx(hy.config, KVDtype.f32, bcap), 131071l, "f32 rows halve it")
        t |> equal(resident_binding_ctx(hy.config, KVDtype.f16, 0l), 0l, "no cap known reads 0")
        delete hy
        var dense <- shell(32l, 0ul)
        dense.config.n_kv_heads = 8l
        dense.config.head_size = 128l
        t |> equal(resident_binding_ctx(dense.config, KVDtype.f16, bcap), 65535l, "the dense twin counts all 32 rows")
        delete dense
        var recr <- shell(4l, 0xFul)
        t |> equal(resident_binding_ctx(recr.config, KVDtype.f16, bcap), 0l, "no attention layer reads 0")
        delete recr
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
