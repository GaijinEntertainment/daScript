options gen2
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_common
require dasllama/dasllama_gpu_resident
require dasllama/dasllama_gpu_tier   // install_moe_gpu_resident_bias - the bias seat a served qwen2 needs
require strings
require daslib/strings_boost

[unused_argument(rows)]
def private test_upload_bias(rows : array<float>) {}

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
        c.attn_sinks = true
        t |> equal(resident_unserved_features(c), "attention sinks")
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
        t |> equal(resident_unserved_features(c), "a non-softmax router")
        c.moe_gate = MoeGate.softmax
        c.moe_router_bias = true
        c.moe_exp_probs = true
        let s = resident_unserved_features(c)
        t |> success(says(s, "a router bias") && says(s, "an expert selection bias"), "the two biases named together: {s}")
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
        let e = resident_unserved_features(c)
        t |> success(says(e, "biased expert stacks") && says(e, "the clamped swiglu expert activation") && !says(e, "output scale"),
            "the expert stack's bias and the clamped activation named together, the layer output scale served: {e}")
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
