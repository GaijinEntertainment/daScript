options gen2
options _comment_hygiene = true
options _dasllama_internal = true
options stack = 524288   // the K/V start cell makes sessions: every dasLLAMA program root that does takes this budget
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require dasllama/dasllama_common   // mtp_depth_pin / restore_mtp_depth, the decode-override registry
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
        t |> success(back.pinned == pin.pinned && back.how == pin.how, "the pin is back as found")
    }
}
