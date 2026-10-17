options gen2
options _comment_hygiene = true
options _dasllama_internal = true
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require dasllama/dasllama_common   // mtp_depth_pin / restore_mtp_depth, the decode-override registry
require _override_probes   // decode_declines: the fake override's step
require ?das_metal dasllama/dasllama_metal_decode   // get_metal_decode_fuse, metal_mtp_carry_pre
require ?das_metal dasllama/dasllama_metal_prefill   // get_metal_prefill_lastrow
require ?das_metal dasllama/dasllama_metal_mtp_gemma   // gemma_trace, gemma_carry_pre_norm

// The driver knobs' getters and the registry's removal verb, model-free and device-free: each setter's value read back through
// its getter, then the value found put back and read back as found - the form every cell that pins a knob restores through.
// The Metal knobs' cells skip on a build without das_metal.

let private PROBE = "knob-getters-probe"   //! a decode override name no backend registers

[unused_argument(t, ws, sessions, nrows)]
def private probe_batch(t : Model; var ws : BatchWorkspace; var sessions : array<Session?>; nrows : int64) : bool => false

[test]
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

[test]
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
        register_decode_override(PROBE, @@probe_decode)
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

[test]
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
