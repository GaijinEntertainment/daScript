options gen2
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_common
require dasllama/dasllama_gpu_resident
require strings
require daslib/strings_boost

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

def private names(s : string; part : string) : bool => find(s, part) >= 0

[test]
def test_resident_unserved_features(t : T?) {
    t |> run("a plain dense config has no unserved feature") @(t : T?) {
        let c = Config()
        t |> equal(resident_unserved_features(c), "")
    }
    t |> run("the qwen35 shape names every feature the driver lacks, in one list") @(t : T?) {
        var c = Config()
        c.q_gated = true
        c.n_layer_nextn = 1l
        c.head_size = 256l
        c.rope_dim = 64l
        let s = resident_unserved_features(c)
        t |> success(names(s, "in-file nextn (MTP) block"), "nextn named: {s}")
        t |> success(names(s, "gated attention"), "gated attention named: {s}")
        t |> success(names(s, "partial rotary (64 of 256)"), "partial rotary named with its widths: {s}")
        t |> equal(length(split(s, ", ")), 3)
    }
    t |> run("one feature alone is one name, no separator") @(t : T?) {
        var c = Config()
        c.attn_sinks = true
        t |> equal(resident_unserved_features(c), "attention sinks")
    }
}

[test]
def test_attn_chain_unserved_features(t : T?) {
    t |> run("interleaved rope is the one thing a plain config lacks; NEOX or partial rope serves") @(t : T?) {
        var c = Config()
        t |> equal(attn_chain_unserved_features(c), "interleaved (non-NEOX) rope")
        c.rope_neox = true
        t |> equal(attn_chain_unserved_features(c), "")
        var p = Config()
        p.head_size = 256l
        p.rope_dim = 64l
        t |> equal(attn_chain_unserved_features(p), "")
    }
}

[test]
def test_resident_layer_decline(t : T?) {
    t |> run("a recurrent layer is named by number; the attention layers around it pass") @(t : T?) {
        var m <- shell(3l, 2ul)
        let hid0 = layer_hidden(m, 0l)
        let hs = layer_head_size(m.config, 0l)
        let kvd = layer_kv_dim(m.config, 0l)
        t |> equal(resident_layer_decline(m, 0l, hid0, hs, kvd), "")
        t |> success(names(resident_layer_decline(m, 1l, hid0, hs, kvd), "layer 1 is recurrent"), "layer 1 named")
        t |> equal(resident_layer_decline(m, 2l, hid0, hs, kvd), "")
        delete m
    }
    t |> run("shared K/V, a missing plane and a differing FFN width each name the layer") @(t : T?) {
        var m <- shell(3l, 0ul)
        let hid0 = layer_hidden(m, 0l)
        let hs = layer_head_size(m.config, 0l)
        let kvd = layer_kv_dim(m.config, 0l)
        m.kv_src[2] = 1l
        t |> success(names(resident_layer_decline(m, 2l, hid0, hs, kvd), "layer 2 shares layer 1's K/V"), "shared K/V named")
        m.wq_offs[0] = -1l
        t |> success(names(resident_layer_decline(m, 0l, hid0, hs, kvd), "layer 0 lacks a q/k/v plane"), "missing plane named")
        m.ffn_w <- [64l, 96l, 64l]
        t |> success(names(resident_layer_decline(m, 1l, hid0, hs, kvd), "layer 1 FFN width 96"), "FFN width named")
        delete m
    }
}
