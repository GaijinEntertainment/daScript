options gen2
options indenting = 4

require dastest/testing_boost
require strudel/strudel_modal
require math

def private peak(samples : array<float>) : float {
    var m = 0.0
    for (s in samples) {
        m = max(m, abs(s))
    }
    return m
}

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

[test]
def test_default_voice(t : T?) {
    let buf <- render_modal_voice(ModalVoice())
    check_sound(t, buf, "default")
}

[test]
def test_pitch_range(t : T?) {
    let lo <- render_modal_voice(ModalVoice(pitch = 180.0))
    let hi <- render_modal_voice(ModalVoice(pitch = 3200.0))
    check_sound(t, lo, "low pitch")
    check_sound(t, hi, "high pitch")
}

[test]
def test_mode_count_extremes(t : T?) {
    // n_modes is clamped to [1,8] internally — both extremes must render cleanly
    let one <- render_modal_voice(ModalVoice(n_modes = 1))
    let many <- render_modal_voice(ModalVoice(n_modes = 8))
    let oob <- render_modal_voice(ModalVoice(n_modes = 0))  // clamps to 1
    check_sound(t, one, "1 mode")
    check_sound(t, many, "8 modes")
    check_sound(t, oob, "0 modes (clamped)")
}

[test]
def test_echo_renders(t : T?) {
    let dry <- render_modal_voice(ModalVoice(echo_amount = 0.0, duration = 0.4))
    let wet <- render_modal_voice(ModalVoice(echo_amount = 0.5, echo_delay = 0.05, duration = 0.4))
    check_sound(t, dry, "dry")
    check_sound(t, wet, "echo")
}

[test]
def test_deterministic(t : T?) {
    let v = ModalVoice(pitch = 1500.0, transient = 0.6, echo_amount = 0.3)
    let a <- render_modal_voice(v)
    let b <- render_modal_voice(v)
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

[test]
def test_ring_affects_tail(t : T?) {
    // a longer ring should leave more energy late in the buffer (same duration)
    let dur = 0.4
    let short_ring <- render_modal_voice(ModalVoice(ring = 0.02, echo_amount = 0.0, transient = 0.0, duration = dur))
    let long_ring <- render_modal_voice(ModalVoice(ring = 0.25, echo_amount = 0.0, transient = 0.0, duration = dur))
    let half = length(short_ring) / 2
    var late_short = 0.0
    var late_long = 0.0
    for (i in range(half, length(short_ring))) {
        late_short += abs(short_ring[i])
        late_long += abs(long_ring[i])
    }
    t |> success(late_long > late_short, "longer ring retains more late energy")
}
