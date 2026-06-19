options gen2
options indenting = 4

require dastest/testing_boost
require strudel/strudel_sfxr
require math

def private peak(samples : array<float>) : float {
    var m = 0.0
    for (s in samples) {
        m = max(m, abs(s))
    }
    return m
}

// NaN/Inf are caught here too: any comparison with NaN is false, and clamp would have
// turned Inf into +-1, so a value outside [-1,1] means the synth produced garbage.
def private all_finite_in_range(samples : array<float>) : bool {
    for (s in samples) {
        if (!(s >= -1.0 && s <= 1.0)) {
            return false
        }
    }
    return true
}

def private check_sound(t : T?; buf : array<float>; name : string) {
    t |> success(!empty(buf), "{name}: non-empty")
    t |> success(peak(buf) > 0.02, "{name}: audible")
    t |> success(all_finite_in_range(buf), "{name}: samples in [-1,1]")
}

// ─── Presets ───

[test]
def test_preset_pickup_coin(t : T?) {
    var seed = 12345u
    let buf <- render_sfxr(sfxr_pickup_coin(seed))
    check_sound(t, buf, "pickup_coin")
}

[test]
def test_preset_laser_shoot(t : T?) {
    var seed = 22222u
    let buf <- render_sfxr(sfxr_laser_shoot(seed))
    check_sound(t, buf, "laser_shoot")
}

[test]
def test_preset_explosion(t : T?) {
    var seed = 33333u
    let buf <- render_sfxr(sfxr_explosion(seed))
    check_sound(t, buf, "explosion")
}

[test]
def test_preset_powerup(t : T?) {
    var seed = 44444u
    let buf <- render_sfxr(sfxr_powerup(seed))
    check_sound(t, buf, "powerup")
}

[test]
def test_preset_hit_hurt(t : T?) {
    var seed = 55555u
    let buf <- render_sfxr(sfxr_hit_hurt(seed))
    check_sound(t, buf, "hit_hurt")
}

[test]
def test_preset_jump(t : T?) {
    var seed = 66666u
    let buf <- render_sfxr(sfxr_jump(seed))
    check_sound(t, buf, "jump")
}

[test]
def test_preset_blip_select(t : T?) {
    var seed = 77777u
    let buf <- render_sfxr(sfxr_blip_select(seed))
    check_sound(t, buf, "blip_select")
}

// ─── Every waveform produces sound ───

[test]
def test_all_waveforms(t : T?) {
    let waves = fixed_array(SfxrWave.square, SfxrWave.sawtooth, SfxrWave.sine, SfxrWave.triangle, SfxrWave.noise)
    for (w in waves) {
        var s = SfxrSound()
        s.wave = w
        s.base_freq = 0.5
        s.env_sustain = 0.3
        s.env_decay = 0.3
        let buf <- render_sfxr(s)
        check_sound(t, buf, "wave {int(w)}")
    }
}

// ─── Render is deterministic (fixed internal noise seed) ───

[test]
def test_deterministic_render(t : T?) {
    var seed = 0xC0FFEEu
    let s = sfxr_explosion(seed)  // noise voice exercises the per-period noise refill
    let a <- render_sfxr(s)
    let b <- render_sfxr(s)
    t |> equal(length(a), length(b))
    var same = length(a) == length(b)
    if (same) {
        for (i in range(length(a))) {
            if (a[i] != b[i]) {
                same = false
                break
            }
        }
    }
    t |> success(same, "same params render identical buffers")
}

// ─── Envelope length scales with decay ───

[test]
def test_decay_controls_length(t : T?) {
    var short = SfxrSound()
    short.env_sustain = 0.0
    short.env_decay = 0.2
    var long = SfxrSound()
    long.env_sustain = 0.0
    long.env_decay = 0.6
    let a <- render_sfxr(short)
    let b <- render_sfxr(long)
    t |> success(length(b) > length(a), "longer decay yields a longer buffer")
}

// ─── Randomize / mutate stay renderable and well-formed ───

[test]
def test_randomize_renderable(t : T?) {
    var seed = 0xABCDEFu
    for (i in range(8)) {
        let s = sfxr_randomize(seed)
        let buf <- render_sfxr(s)
        t |> success(all_finite_in_range(buf), "randomize {i}: samples in [-1,1]")
    }
}

[test]
def test_mutate_renderable(t : T?) {
    var seed = 0x13572468u
    let base = sfxr_jump(seed)
    for (i in range(8)) {
        let s = sfxr_mutate(base, seed, 0.1)
        let buf <- render_sfxr(s)
        t |> success(all_finite_in_range(buf), "mutate {i}: samples in [-1,1]")
    }
}
