options gen2
options stack = 524288   // three Model shells live in one cell's frame - the default test stack overflows on them
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_common
require dasllama/dasllama_sampling   // sample_: the consumer of a landed pick
require dasllama/dasllama_gpu_resident
require dasllama/dasllama_gpu_tier   // install_moe_gpu_resident_bias - the bias seat a served qwen2 needs
require ?vulkan dasllama/dasllama_vulkan_decode   // rd_routed_slots_fit - the N-row command's slot cap, where the module exists
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
def test_resident_binding_ctx(t : T?) {
    t |> run("the KV mirror's binding cap counts the attention layers' rows, whatever layer 0 is") @(t : T?) {
        let bcap = 4_294_967_295l   //! the 4 GiB maxStorageBufferRange of the NV/AMD drivers
        //! 32 layers, attention every fourth (3, 7, ..., 31), kv width 8 x 128 in f16: one side is 8 x 2048 B per position
        var hy <- shell(32l, 0x7777_7777ul)
        hy.config.n_kv_heads = 8l
        hy.config.head_size = 128l
        t |> success(layer_is_recurrent(hy.config, 0l) && layer_kv_dim(hy.config, 0l) == 0l, "layer 0 is recurrent: its kv width reads 0")
        t |> equal(resident_binding_ctx(hy, KVDtype.f16, bcap), 262143l, "the hybrid's ctx is the range over eight attention rows, not skipped for layer 0's zero width")
        t |> equal(resident_binding_ctx(hy, KVDtype.f32, bcap), 131071l, "f32 rows halve it")
        t |> equal(resident_binding_ctx(hy, KVDtype.f16, 0l), 0l, "no cap known reads 0")
        delete hy
        var dense <- shell(32l, 0ul)
        dense.config.n_kv_heads = 8l
        dense.config.head_size = 128l
        t |> equal(resident_binding_ctx(dense, KVDtype.f16, bcap), 65535l, "the dense twin counts all 32 rows")
        for (k in range64(8l)) {   //! every fourth layer shares the K/V of the layer below it: eight sharers, twenty-four owners
            dense.kv_src[4l * k + 1l] = 4l * k
        }
        t |> equal(resident_binding_ctx(dense, KVDtype.f16, bcap), 87381l, "a shared-KV layer adds no row of its own: the cap divides by the twenty-four rows the mirror allocates, not the thirty-two layers")
        delete dense
        var recr <- shell(4l, 0xFul)
        t |> equal(resident_binding_ctx(recr, KVDtype.f16, bcap), 0l, "no attention layer reads 0")
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
}

[test]
def test_routed_slots_fit(t : T?) {
    t |> run("the routed planes' slot cap: every row's picks fit the 64 slots or the N-row command declines the model") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            t |> success(rd_routed_slots_fit(8l, 8l), "eight rows of eight picks fill the slots")
            t |> success(!rd_routed_slots_fit(8l, 9l), "eight rows of nine picks pass them")
            t |> success(rd_routed_slots_fit(2l, 32l), "two rows of thirty-two picks fit")
            t |> success(!rd_routed_slots_fit(3l, 32l), "three rows of thirty-two pass them")
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

let private PROBE_OWNED = "mtp_seat_probe"   //! a fake decode override whose seats carry an owner the probe answers
let private PROBE_UNOWNED = "mtp_seat_probe_unowned"   //! ... and one whose seats carry none
var private g_probe_owns = false
var private g_probe_seat_calls = 0l

[unused_argument(t, s, token, pos)]
def private probe_decode(t : Model; var s : Session; token, pos : int64) : bool => false

[unused_argument(t)]
def private probe_owner(t : Model) : bool => g_probe_owns

[unused_argument(t, s, tok, accepted)]
def private probe_spec(t : Model; var s : Session; tok : int64; var accepted : int64&) : bool {
    g_probe_seat_calls++
    return false
}

[unused_argument(t, s, tok, k)]
def private probe_round(t : Model; var s : Session; tok : int64; k : int64) : int64 {
    g_probe_seat_calls++
    return 0l
}

[unused_argument(t, sessions, toks, accepted)]
def private probe_batch(t : Model; var sessions : array<Session?>; toks : array<int64>; var accepted : array<int64>) : int64 {
    g_probe_seat_calls++
    return 7l
}

//! how many of `mtp_spec_eval`, `mtp_spec_round` and `mtp_spec_eval_batch` reached a seat, how many opened the CPU round, and the batch tick's answer
struct private SeatDispatch {
    seat : int64
    cpu : int64
    batch : int64
}

//! the three seat-taking entries once each under the fake override `name` (its owner answering `owns`) on a Model shell;
//! the session carries a parked token, so the CPU round's opening refuses it before any forward and counts as a CPU call
def private seat_dispatch(name : string; owns : bool) : SeatDispatch {
    for (nm in [PROBE_OWNED, PROBE_UNOWNED]) {
        register_decode_override(nm, @@probe_decode)
        register_mtp_spec_override(nm, @@probe_spec)
        register_mtp_round_override(nm, @@probe_round)
        register_mtp_spec_batch_override(nm, @@probe_batch)
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
    select_decode_override(prev)
    out.seat = g_probe_seat_calls
    delete s
    delete m
    delete accepted
    return out
}

[test]
def test_mtp_seat_owner(t : T?) {
    t |> run("the speculative seats serve only a model the active decode override's owner claims; otherwise the CPU round runs") @(t : T?) {
        let refused = seat_dispatch(PROBE_OWNED, false)
        t |> equal(refused.seat, 0l, "an owner refusing the model reaches no seat")
        t |> equal(refused.cpu, 2l, "mtp_spec_eval and mtp_spec_round open the CPU round")
        t |> equal(refused.batch, 0l, "the batch tick steps each stream alone")
        let unowned = seat_dispatch(PROBE_UNOWNED, true)
        t |> equal(unowned.seat, 0l, "seats registered with no owner never take a round")
        t |> equal(unowned.cpu, 2l, "... and the CPU round runs")
        let owned = seat_dispatch(PROBE_OWNED, true)
        t |> equal(owned.seat, 3l, "an owner claiming the model hands every entry to its seat")
        t |> equal(owned.cpu, 0l, "no entry opened the CPU round")
        t |> equal(owned.batch, 7l, "the batch tick answers the seat's count")
    }
}

//! the CPU round's opening on a session carrying an unconsumed landed pick, under the unowned probe override
def private round_refuses_pick() : bool {
    register_decode_override(PROBE_UNOWNED, @@probe_decode)
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
