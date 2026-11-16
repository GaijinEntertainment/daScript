options gen2
options _comment_hygiene = true
options _dasllama_internal = true
options stack = 524288   // the K/V start cell makes sessions: every dasLLAMA program root that does takes this budget
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require dasllama/dasllama_common   // mtp_depth_pin / restore_mtp_depth, the decode-override registry
require dasllama/dasllama_lever   // [lever]: the probe levers below
require dasllama/dasllama_gguf   // the source_release lever
require dasllama/dasllama_image   // the image_save and host_planes levers
require dasllama/dasllama_gpu_tier   // the vulkan_trim lever
require dasllama/dasllama_gpu_resident   // the lane_read and gpu_kv_want levers
require _override_probes   // decode_declines: the fake override's step
require dasllama/dasllama_kv_codec   // cvt_q8kv_to_f32: a poisoned q8_0 row read back
require daslib/linq   // all
require ?das_metal dasllama/dasllama_metal_decode   // get_metal_decode_fuse, metal_mtp_carry_pre
require ?das_metal dasllama/dasllama_metal_prefill   // get_metal_prefill_lastrow
require ?das_metal dasllama/dasllama_metal_mtp_gemma   // gemma_trace, gemma_carry_pre_norm

// The driver knobs' getters and the registry's removal verb, model-free and device-free: each setter's value read back through
// its getter, then the value found put back and read back as found - the form every cell that pins a knob restores through.
// The Metal knobs' cells skip on a build without das_metal.

let private PROBE = "knob-getters-probe"   //! a decode override name no backend registers

[unused_argument(t, ws, sessions, nrows)]
def private probe_batch(t : Model; var ws : BatchWorkspace; var sessions : array<Session?>; nrows : int64) : bool => false

[test(description = "`mtp_depth_pin` reads back whether and at what depth `set_mtp_depth` pinned, and `restore_mtp_depth` puts back both a cleared and a pinned state exactly as found")]
def test_mtp_depth_pin(t : T?) {
    t |> run("mtp_depth_pin reads whether set_mtp_depth pinned a depth and the depth, restore_mtp_depth puts both back") @(t : T?) {
        let found = mtp_depth_pin()
        set_mtp_depth(3l)
        let pinned = mtp_depth_pin()
        t |> success(pinned.pinned && pinned.depth == 3l, "an explicit depth reads pinned at 3 ({pinned.pinned}, {pinned.depth})")
        t |> equal(get_mtp_depth(), 3l, "the explicit depth is the round's depth")
        clear_mtp_depth()
        let cleared = mtp_depth_pin()
        t |> success(!cleared.pinned, "a cleared depth reads unpinned")
        set_mtp_depth(5l)
        restore_mtp_depth(cleared)
        t |> success(!mtp_depth_pin().pinned, "restoring an unpinned depth leaves no explicit depth standing")
        restore_mtp_depth(pinned)
        t |> success(mtp_depth_pin().pinned && get_mtp_depth() == 3l, "restoring the pin brings the explicit depth back")
        restore_mtp_depth(found)
        let back = mtp_depth_pin()
        t |> success(back.pinned == found.pinned && back.depth == found.depth, "the depth found is put back ({back.pinned}, {back.depth})")
    }
}

[test(description = "`unregister_decode_override` removes a probe override and its batched step, deselects it only when active, and a re-registered single-stream name arms no batch twin while a re-registered batch step does")]
def test_unregister_decode_override(t : T?) {
    t |> run("unregister_decode_override drops the override and its batched step, deselecting it where it is active") @(t : T?) {
        let found = active_decode_override()
        t |> success(!has_decode_override(PROBE), "the probe is not registered before the cell")
        register_decode_override(PROBE, @@decode_declines)
        register_batch_decode_override(PROBE, @@probe_batch)
        t |> success(has_decode_override(PROBE), "the probe registered")
        t |> success(select_decode_override(PROBE), "the probe selects")
        unregister_decode_override(PROBE)
        t |> success(!has_decode_override(PROBE), "the probe is gone from the registry")
        t |> equal(active_decode_override(), "", "an active override unregistered is deselected")
        t |> success(!select_decode_override(PROBE), "an unregistered name no longer selects")
        register_decode_override(PROBE, @@decode_declines)
        select_decode_override(found)
        unregister_decode_override(PROBE)
        t |> equal(active_decode_override(), found, "an inactive override unregistered leaves the selection as found")
        register_decode_override(PROBE, @@decode_declines)
        select_decode_override(PROBE)
        t |> success(!batch_decode_override_armed(), "the batched step went with the override: the name re-registered single-stream arms no batch twin")
        register_batch_decode_override(PROBE, @@probe_batch)
        select_decode_override(PROBE)
        t |> success(batch_decode_override_armed(), "the control: a batched step registered under the name arms with it")
        select_decode_override(found)
        unregister_decode_override(PROBE)
        t |> equal(active_decode_override(), found, "the selection found is put back")
        t |> success(!has_decode_override(PROBE), "the probe is unregistered after the cell")
    }
}

[test(description = "Each Metal driver knob getter (decode fuse, prefill last row, gemma trace, gemma carry pre-norm) reads back what its setter set and the value found restores, and the carry choice leaves the decode carry alone with no drafter attached")]
def test_metal_knob_getters(t : T?) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        t |> run("get_metal_decode_fuse reads the rail set_metal_decode_fuse set") @(t : T?) {
            let found = get_metal_decode_fuse()
            for (v in fixed_array(1, 0, -1)) {
                set_metal_decode_fuse(v)
                t |> equal(get_metal_decode_fuse(), v, "the fuse rail reads {v}")
            }
            set_metal_decode_fuse(found)
            t |> equal(get_metal_decode_fuse(), found, "the rail found is put back")
        }
        t |> run("get_metal_prefill_lastrow reads the tail set_metal_prefill_lastrow set") @(t : T?) {
            let found = get_metal_prefill_lastrow()
            for (v in fixed_array(!found, found)) {
                set_metal_prefill_lastrow(v)
                t |> equal(get_metal_prefill_lastrow(), v, "the last-row tail reads {v}")
            }
            set_metal_prefill_lastrow(found)
            t |> equal(get_metal_prefill_lastrow(), found, "the tail found is put back")
        }
        t |> run("gemma_trace and gemma_carry_pre_norm read the drafter levers their setters set") @(t : T?) {
            let trace = gemma_trace()
            let pre = gemma_carry_pre_norm()
            let carry = metal_mtp_carry_pre()
            set_gemma_trace(!trace)
            t |> equal(gemma_trace(), !trace, "the trace lever reads what was set")
            set_gemma_trace(trace)
            t |> equal(gemma_trace(), trace, "the trace lever found is put back")
            set_gemma_carry_pre_norm(!pre)
            t |> equal(gemma_carry_pre_norm(), !pre, "the h reading reads what was set")
            t |> equal(metal_mtp_carry_pre(), carry, "with no drafter attached the decode's carry is left alone - the choice applies at the attach")
            set_gemma_carry_pre_norm(pre)
            t |> equal(gemma_carry_pre_norm(), pre, "the h reading found is put back")
        }
    } else {
        t |> skip("das_metal is not built on this platform; the Metal drivers' knobs are not compiled")
    }
}

//! whether `buf` holds bytes and every one is `v`
def private bytes_all(buf : array<uint8>; v : uint8) : bool => !empty(buf) && all(buf, $(b) => b == v)

[test(description = "A session's K/V buffers start as `set_kv_init` asks: the unfilled start sizes them and writes no byte, zero and poison write every byte (0, and 0xFF - a NaN row through the f32 and the q8_0 codec), a paged pool's groups start the same way as they grow, a fill past its chunk covers its range and nothing beside it, `kv_init_of` reads each start's spelling and anything else as unfilled, and the pin restores as found")]
def test_kv_start(t : T?) {
    t |> run("unfilled, zero and poison starts of a flat session's slabs and a paged pool's groups") @(t : T?) {
        let c = Config(dim = 256l, hidden_dim = 32l, n_layers = 2l, n_heads = 4l, n_kv_heads = 2l, vocab_size = 8l, seq_len = 16l,
            head_size = 64l, kv_dim = 128l, shared_weights = true, rope_freq_base = 10000.0)
        let pin = kv_init_pin()
        set_kv_init(KvInit.unfilled)
        let b0 = kv_bytes_started()
        var u <- make_run_state(c)
        let nk = long_length(u.key_cache)
        let nv = long_length(u.value_cache)
        t |> success(nk > 0l && nv > 0l, "unfilled: the slabs are sized ({nk} K bytes, {nv} V bytes)")
        t |> equal(kv_bytes_started(), b0, "unfilled: no byte of them is written")
        delete u
        set_kv_init(KvInit.zero)
        t |> equal(kv_init(), KvInit.zero, "the pin decides the start")
        var z <- make_run_state(c)
        t |> success(bytes_all(z.key_cache, 0u8) && bytes_all(z.value_cache, 0u8), "zero: every byte 0")
        t |> equal(kv_bytes_started(), b0 + nk + nv, "zero: every byte of both slabs is written")
        delete z
        set_kv_init(KvInit.poison)
        var p <- make_run_state(c)
        t |> success(bytes_all(p.key_cache, 0xFFu8) && bytes_all(p.value_cache, 0xFFu8), "poison: every byte 0xFF")
        t |> success(!bytes_all(p.key_cache, 0u8), "the zero check tells a poisoned slab apart")
        t |> equal(long_length(p.key_cache), nk, "poison: the K slab's size")
        unsafe {
            let d = addr<float const?>(p.key_cache[0])
            t |> success(d[0] != d[0] && d[c.kv_dim - 1l] != d[c.kv_dim - 1l], "poison: an f32 row reads NaN")
        }
        delete p
        var q <- make_run_state(c, KVDtype.q8_0, KVDtype.q8_0)
        var row : array<float>
        row |> resize(c.kv_dim)
        unsafe {
            cvt_q8kv_to_f32(addr(row[0]), addr<uint8 const?>(q.value_cache[0]), c.kv_dim)
        }
        var nan_n = 0l
        for (x in row) {
            nan_n += x != x ? 1l : 0l
        }
        t |> equal(nan_n, c.kv_dim, "poison: a q8_0 row reads NaN at every element")
        delete row
        delete q
        var pool <- create_kv_pool_(c, 4l, KVDtype.f32, KVDtype.f32)
        let g0 = kv_pool_alloc_group(pool)
        t |> success(g0 >= 0 && bytes_all(pool.kblobs[0], 0xFFu8) && bytes_all(pool.vblobs[0], 0xFFu8), "poison: a paged pool's first groups start 0xFF as the pool grows")
        set_kv_init(KvInit.unfilled)
        let grown = long_length(pool.kblobs[0])
        for (_g in range(8)) {
            kv_pool_alloc_group(pool)
        }
        t |> success(long_length(pool.kblobs[0]) > grown, "the pool grew past its first groups")
        var first_all = true
        for (i in range64(grown)) {
            first_all &&= pool.kblobs[0][i] == 0xFFu8
        }
        t |> success(first_all, "... and growing it rewrote no byte of the groups it had")
        delete pool
        set_kv_init(KvInit.poison)
        var buf <- [for (_i in range(100)); 0x11u8]
        kv_bytes_start(buf, 10l, 90l, 7l)
        var inside = 0
        var outside = 0
        for (b, i in buf, count()) {
            inside += i >= 10 && i < 90 && b == 0xFFu8 ? 1 : 0
            outside += (i < 10 || i >= 90) && b == 0x11u8 ? 1 : 0
        }
        t |> equal(inside, 80, "a fill of 80 bytes in chunks of 7 covers its whole range")
        t |> equal(outside, 20, "... and no byte beside it")
        delete buf
        t |> success(kv_init_of("zero") == KvInit.zero && kv_init_of("poison") == KvInit.poison && kv_init_of("unfilled") == KvInit.unfilled, "the knob's spellings are the starts' names")
        t |> success(kv_init_of("") == KvInit.unfilled && kv_init_of("zeros") == KvInit.unfilled, "unset, and a spelling that names no start, read as unfilled")
        restore_kv_init(pin)
        let back = kv_init_pin()
        t |> success(back.pinned == pin.pinned && back.value == pin.value, "the pin is back as found")
    }
}

enum private ProbeStart {
    cold
    warm
    hot
}

var private g_probe_knob = true
var private g_probe_reads = 0   //! the times `probe_start_now`'s own body ran: a pinned lever answers without it

[lever]
def private probe_flag() : bool => g_probe_knob

[lever(name = probe_start)]
def private probe_start_now() : ProbeStart {
    g_probe_reads++
    return ProbeStart.warm
}

[test(description = "A `[lever]` getter answers its own body until `set_X` pins a value, `reset_X` lifts the pin, `X_pin` / `restore_X` carry a pinned and an unpinned state back exactly, `with_X` pins for its block alone and nests, and `name =` names the generated functions apart from the getter")]
def test_lever_macro(t : T?) {
    t |> run("an unpinned lever reads its knob, a pinned one the pin; reset, restore and with put the state back") @(t : T?) {
        t |> success(probe_flag() && !probe_flag_pin().pinned, "unpinned: the getter's own body answers")
        g_probe_knob = false
        t |> success(!probe_flag(), "... and follows the knob")
        set_probe_flag(true)
        t |> success(probe_flag() && probe_flag_pin().pinned && probe_flag_pin().value, "set pins the value over the knob")
        let pinned = probe_flag_pin()
        reset_probe_flag()
        t |> success(!probe_flag() && !probe_flag_pin().pinned, "reset lifts the pin: the knob decides again")
        let lifted = probe_flag_pin()
        restore_probe_flag(pinned)
        t |> success(probe_flag() && probe_flag_pin().pinned, "restore brings a pinned state back")
        restore_probe_flag(lifted)
        t |> success(!probe_flag() && !probe_flag_pin().pinned, "... and an unpinned one")
        var seen = 0
        with_probe_flag(true) {
            seen += probe_flag() ? 1 : 0
            with_probe_flag(false) {
                seen += probe_flag() ? 0 : 10
            }
            seen += probe_flag() && probe_flag_pin().pinned ? 100 : 0
        }
        t |> equal(seen, 111, "with pins for its block, a nested with for its own, the outer pin back between them")
        t |> success(!probe_flag() && !probe_flag_pin().pinned, "... and the state it found after it")
        g_probe_knob = true
    }
    t |> run("a lever over an enum, named apart from its getter") @(t : T?) {
        let r0 = g_probe_reads
        t |> equal(probe_start_now(), ProbeStart.warm, "unpinned: the body's value")
        t |> equal(g_probe_reads, r0 + 1, "... read through the body")
        set_probe_start(ProbeStart.hot)
        t |> equal(probe_start_now(), ProbeStart.hot, "pinned: the pin's value")
        t |> equal(g_probe_reads, r0 + 1, "... with the body not run")
        with_probe_start(ProbeStart.cold) {
            t |> equal(probe_start_now(), ProbeStart.cold, "with pins its own value")
        }
        t |> success(probe_start_pin().pinned && probe_start_pin().value == ProbeStart.hot, "... and puts the pin it found back")
        reset_probe_start()
        t |> success(!probe_start_pin().pinned && probe_start_pin().value == ProbeStart.cold, "reset leaves the type's default behind an unpinned lever")
    }
}

typedef BoolPin = tuple<pinned : bool; value : bool>

//! a bool lever of the engine through its five functions: the value against the knob pinned and read, the reset, `with` for a block,
//! and the pin found put back
def private bool_lever_walk(t : T?; name : string; get : function<() : bool>; set : function<(v : bool) : void>;
                            reset : function<() : void>; pin : function<() : BoolPin>; restore : function<(p : BoolPin) : void>;
                            wth : function<(v : bool; blk : block) : void>) {
    let found = invoke(pin)
    invoke(reset)
    let knob = invoke(get)
    t |> success(!invoke(pin).pinned, "{name}: reset leaves no pin, and the getter reads the knob ({knob})")
    invoke(set, !knob)
    t |> success(invoke(get) == !knob && invoke(pin).pinned, "{name}: set pins the value against the knob")
    var inside = false
    invoke(wth, knob) {
        inside = invoke(get) == knob
    }
    t |> success(inside && invoke(get) == !knob, "{name}: with pins for its block and puts the pin back")
    invoke(restore, found)
    let back = invoke(pin)
    t |> success(back.pinned == found.pinned && (!found.pinned || back.value == found.value), "{name}: the pin is back as found")
}

[test(description = "Each of the engine's levers - source_release, image_save, host_planes, vulkan_trim, lane_read, and the enum levers kv_init and gpu_kv_want - reads its knob unpinned, the pinned value once set, `with_`'s value inside its block alone, and comes back as found through `_pin` / `restore_`")]
def test_engine_levers(t : T?) {
    t |> run("the five bool levers") @(t : T?) {
        bool_lever_walk(t, "source_release", @@source_release, @@set_source_release, @@reset_source_release, @@source_release_pin, @@restore_source_release, @@with_source_release)
        bool_lever_walk(t, "image_save", @@image_save_enabled, @@set_image_save, @@reset_image_save, @@image_save_pin, @@restore_image_save, @@with_image_save)
        bool_lever_walk(t, "host_planes", @@host_planes, @@set_host_planes, @@reset_host_planes, @@host_planes_pin, @@restore_host_planes, @@with_host_planes)
        bool_lever_walk(t, "vulkan_trim", @@vulkan_trim, @@set_vulkan_trim, @@reset_vulkan_trim, @@vulkan_trim_pin, @@restore_vulkan_trim, @@with_vulkan_trim)
        bool_lever_walk(t, "lane_read", @@lane_read, @@set_lane_read, @@reset_lane_read, @@lane_read_pin, @@restore_lane_read, @@with_lane_read)
    }
    t |> run("the two enum levers") @(t : T?) {
        let start = kv_init_pin()
        with_kv_init(KvInit.zero) {
            t |> equal(kv_init(), KvInit.zero, "kv_init: with pins the start for its block")
        }
        let start_back = kv_init_pin()
        t |> success(start_back.pinned == start.pinned && start_back.value == start.value, "kv_init: ... and puts the pin back")
        let want = gpu_kv_want_pin()
        set_gpu_kv_want(KVDtype.tq4)
        t |> success(gpu_kv_want_pin().pinned && gpu_kv_want() == KVDtype.tq4, "gpu_kv_want: set pins the codec")
        with_gpu_kv_want(KVDtype.q8_0) {
            t |> equal(gpu_kv_want(), KVDtype.q8_0, "gpu_kv_want: with pins the codec for its block")
        }
        t |> equal(gpu_kv_want(), KVDtype.tq4, "gpu_kv_want: ... and puts the pin back")
        reset_gpu_kv_want()
        t |> success(!gpu_kv_want_pin().pinned && gpu_kv_want() == KVDtype.f16, "gpu_kv_want: unpinned, the getter's own f16")
        restore_gpu_kv_want(want)
        t |> equal(gpu_kv_want_pin().pinned, want.pinned, "gpu_kv_want: the pin is back as found")
    }
}
