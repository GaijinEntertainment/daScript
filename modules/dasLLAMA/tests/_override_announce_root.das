options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dasllama/dasllama_common
require dasllama/dasllama_gpu_resident
require ?das_metal dasllama/dasllama_metal_common
require ?das_metal dasllama/dasllama_metal_decode

// The fixture test_override_announces.das spawns: the override announces print through to_log, which no in-process cell
// reads, and DASLLAMA_METAL_FUSE is read at context init. Every case opens with a `CASE <name>` record on the same log
// stream, so the cell reads each case's lines between its marker and the next.

def private mark(name : string) {
    to_log(LOG_INFO, "CASE {name}\n")
}

//! a four-layer shell, every other layer sliding at `window` (0 = no sliding layer), one 64-wide K/V head a layer
def private ring_shell(window : int64) : Model {
    var t = Model()
    t.config.n_layers = 4l
    t.config.n_heads = 1l
    t.config.n_kv_heads = 1l
    t.config.head_size = 64l
    t.config.dim = 64l
    t.config.sliding_window = window
    t.config.swa_pattern = 2l
    t.kv_row_prefix <- [0l, 64l, 128l, 192l]
    return <- t
}

def private ring_cases {
    var sliding <- ring_shell(512l)
    var flat <- ring_shell(0l)
    let pin = gpu_kv_ring_pin()
    mark("ring_on")
    set_gpu_kv_ring(1)
    kv_ring_announce(sliding.config, "probe home", "the probe's rows")
    mark("ring_off")
    set_gpu_kv_ring(0)
    kv_ring_announce(sliding.config, "probe home", "the probe's rows")
    mark("ring_none")
    set_gpu_kv_ring(1)
    kv_ring_announce(flat.config, "probe home", "the probe's rows")
    set_gpu_kv_ring(0)
    kv_ring_announce(flat.config, "probe home", "the probe's rows")
    static_if (typeinfo builtin_module_exists(das_metal)) {
        mark("metal_ring_twice")
        set_gpu_kv_ring(1)
        mirror_rings(sliding)
        mirror_rings(sliding)
        mark("metal_ring_flat")
        set_gpu_kv_ring(0)
        mirror_rings(sliding)
    }
    set_gpu_kv_ring(pin)
    delete flat
    delete sliding
}

def private embed_cases {
    var c = Config()
    let pin = gpu_resident_embed_pin()
    mark("embed_pinned_off")
    set_gpu_resident_embed(0)
    resident_emb_pin_say(c, true)
    mark("embed_pinned_off_no_seat")
    resident_emb_pin_say(c, false)
    mark("embed_pinned_off_hc")
    c.hyper_conn = true
    resident_emb_pin_say(c, true)
    c.hyper_conn = false
    mark("embed_on")
    set_gpu_resident_embed(1)
    resident_emb_pin_say(c, true)
    mark("embed_gather_lines")
    resident_emb_gather_say("the table's exact f32 rows")
    resident_emb_gather_say("the table's own Q4_1 plane")
    set_gpu_resident_embed(pin)
}

def private metal_cases {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        mark("metal_gather_off")
        set_metal_batch_gather(false)
        mark("metal_gather_on")
        set_metal_batch_gather(true)
        mark("metal_g4_unfused")
        let prior = set_metal_moe_g4_unfused(true)
        mark("metal_g4_fused")
        set_metal_moe_g4_unfused(prior)
        mark("metal_fuse_restore")
        set_metal_decode_fuse(-1)
        mark("metal_fuse_off")
        set_metal_decode_fuse(0)
        set_metal_decode_fuse(0)
        mark("metal_fuse_on_off")
        set_metal_decode_fuse(1)
        set_metal_decode_fuse(0)
        set_metal_decode_fuse(-1)
    }
}

//! no module of this root registers the assistant drafter's sidecar resolver (dasllama/dasllama_mtp_gemma does)
def private sidecar_cases {
    mark("sidecar_no_resolver")
    let side = mtp_drafter_sidecar_("gemma-4-26B-A4B-it-Q4_K_M.gguf")
    to_log(LOG_INFO, "SIDECAR '{side}'\n")
}

[export]
def main() {
    ring_cases()
    embed_cases()
    sidecar_cases()
    metal_cases()
    mark("end")
    print("ROOT_DONE\n")
}
