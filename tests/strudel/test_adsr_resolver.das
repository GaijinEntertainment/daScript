options gen2
options indenting = 4

require dastest/testing_boost
require strudel/strudel
require strudel/strudel_event
require math

[test(description = "resolve_adsr with nothing set returns a full-sustain held tone with tiny attack, decay and release edges")]
def test_resolve_adsr_none_set(t : T?) {
    // Nothing set: full-sustain held tone with tiny edges.
    let r = resolve_adsr(-1.0, -1.0, -1.0, -1.0)
    t |> equal(r._0, 0.001)
    t |> equal(r._1, 0.001)
    t |> equal(r._2, 1.0)
    t |> equal(r._3, 0.01)
}

[test(description = "a decay alone makes the envelope percussive by dropping sustain to the envelope minimum")]
def test_resolve_adsr_only_decay_percussion(t : T?) {
    // Decay alone → percussion: sustain drops to envmin (0.001).
    let r = resolve_adsr(-1.0, 0.2, -1.0, -1.0)
    t |> equal(r._0, 0.001)
    t |> equal(r._1, 0.2)
    t |> equal(r._2, 0.001)
    t |> equal(r._3, 0.01)
}

[test(description = "an attack alone yields a held tone with the slow attack and default decay, sustain and release")]
def test_resolve_adsr_only_attack(t : T?) {
    // Attack alone, no decay → held tone with slow attack.
    let r = resolve_adsr(0.3, -1.0, -1.0, -1.0)
    t |> equal(r._0, 0.3)
    t |> equal(r._1, 0.001)
    t |> equal(r._2, 1.0)
    t |> equal(r._3, 0.01)
}

[test(description = "a release alone yields a held tone with the long release tail")]
def test_resolve_adsr_only_release(t : T?) {
    // Release alone → held tone with long release tail.
    let r = resolve_adsr(-1.0, -1.0, -1.0, 0.5)
    t |> equal(r._0, 0.001)
    t |> equal(r._1, 0.001)
    t |> equal(r._2, 1.0)
    t |> equal(r._3, 0.5)
}

[test(description = "an explicit sustain overrides the percussion default even when a decay is set")]
def test_resolve_adsr_sustain_explicit_wins(t : T?) {
    // Sustain set explicitly overrides percussion default even when decay is set.
    let r = resolve_adsr(-1.0, 0.1, 0.5, -1.0)
    t |> equal(r._0, 0.001)
    t |> equal(r._1, 0.1)
    t |> equal(r._2, 0.5)
    t |> equal(r._3, 0.01)
}

[test(description = "an explicit sustain of zero stays zero and is distinct from sustain unset")]
def test_resolve_adsr_sustain_zero_explicit(t : T?) {
    // sustain=0 explicitly (percussion) is distinct from sustain unset.
    let r = resolve_adsr(-1.0, -1.0, 0.0, -1.0)
    t |> equal(r._0, 0.001)
    t |> equal(r._1, 0.001)
    t |> equal(r._2, 0.0)
    t |> equal(r._3, 0.01)
}

[test(description = "resolve_adsr passes all four explicit values through unchanged")]
def test_resolve_adsr_all_set(t : T?) {
    let r = resolve_adsr(0.05, 0.1, 0.8, 0.5)
    t |> equal(r._0, 0.05)
    t |> equal(r._1, 0.1)
    t |> equal(r._2, 0.8)
    t |> equal(r._3, 0.5)
}

[test(description = "zero attack, decay and release clamp up to their minimum floors")]
def test_resolve_adsr_clamp_floors(t : T?) {
    // Zero or tiny values clamp up to envmin/release_min floors.
    let r = resolve_adsr(0.0, 0.0, 0.5, 0.0)
    t |> equal(r._0, 0.001)
    t |> equal(r._1, 0.001)
    t |> equal(r._2, 0.5)
    t |> equal(r._3, 0.01)
}

[test(description = "a sustain above one clamps down to one")]
def test_resolve_adsr_sustain_clamp_max(t : T?) {
    // Sustain >1 clamps down to 1.
    let r = resolve_adsr(-1.0, -1.0, 2.5, -1.0)
    t |> equal(r._2, 1.0)
}

// ─── Delay time resolver ───

[test(description = "an explicit delaytime is returned verbatim regardless of delaysync and cps")]
def test_resolve_delaytime_explicit_wins(t : T?) {
    // Explicit delaytime is returned verbatim regardless of delaysync/cps.
    t |> equal(resolve_delaytime(0.4, 0.1875, 0.5), 0.4)
    t |> equal(resolve_delaytime(0.0, 0.1875, 0.5), 0.0)
    t |> equal(resolve_delaytime(1.25, 0.1875, 2.0), 1.25)
}

[test(description = "an unset delaytime is derived as delaysync divided by cps")]
def test_resolve_delaytime_tempo_synced(t : T?) {
    // Unset delaytime derives from delaysync / cps.
    // 3/16 cycle at cps=0.5 → 0.375s (dotted-eighth at 120 BPM when beats=4/cycle).
    t |> success(abs(resolve_delaytime(-1.0, 0.1875, 0.5) - 0.375) < 1e-6)
    // 3/16 cycle at cps=1.0 → 0.1875s.
    t |> success(abs(resolve_delaytime(-1.0, 0.1875, 1.0) - 0.1875) < 1e-6)
    // 1/4 cycle at cps=2.0 → 0.125s.
    t |> success(abs(resolve_delaytime(-1.0, 0.25, 2.0) - 0.125) < 1e-6)
}

[test(description = "a zero or near-zero cps falls back to 0.5 so the delay time never divides by zero")]
def test_resolve_delaytime_degenerate_cps(t : T?) {
    // Zero/near-zero cps falls back to cps=0.5 so we never divide by zero.
    t |> success(abs(resolve_delaytime(-1.0, 0.1875, 0.0) - 0.375) < 1e-6)
    t |> success(abs(resolve_delaytime(-1.0, 0.1875, 0.00001) - 0.375) < 1e-6)
}
