options gen2
options indenting = 4

require dastest/testing_boost
require strudel/strudel_sfx
require math

let SR = 44100

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
def test_render_voice_both_kinds(t : T?) {
    var seed = 7u
    let s = render_voice(VoiceParams(sfxr = sfxr_pickup_coin(seed)))
    let m = render_voice(VoiceParams(modal = ModalVoice()))
    check_sound(t, s, "sfxr voice")
    check_sound(t, m, "modal voice")
}

[test]
def test_two_layer_mix(t : T?) {
    // ball-on-table: low thud + hard click, one sound
    let sound <- [
        modal_layer("thud", ModalVoice(pitch = 170.0, n_modes = 2, ring = 0.05, duration = 0.4)),
        modal_layer("click", ModalVoice(pitch = 1500.0, duration = 0.4))
    ]
    let buf <- render_sound(sound)
    check_sound(t, buf, "table mix")
}

[test]
def test_delay_extends_length(t : T?) {
    var sound : array<Layer>
    var lyr = modal_layer("d", ModalVoice(duration = 0.1))
    lyr.delay = 0.2
    sound |> push(lyr)
    let buf <- render_sound(sound)
    t |> success(length(buf) >= int(0.2 * float(SR)), "onset delay shifts the buffer out")
}

[test]
def test_disabled_layer_skipped(t : T?) {
    var sound : array<Layer>
    sound |> push(modal_layer("a", ModalVoice(duration = 0.1)))
    let one <- render_sound(sound)
    var extra = modal_layer("b", ModalVoice(duration = 0.1))
    extra.delay = 0.5
    extra.enabled = false
    sound |> push(extra)
    let with_disabled <- render_sound(sound)
    // a disabled far-delayed layer must not extend or change the mix
    t |> equal(length(one), length(with_disabled))
}

[test]
def test_link_mirrors_source(t : T?) {
    var sound : array<Layer>
    sound |> push(modal_layer("src", ModalVoice(pitch = 1234.0, ring = 0.08, brightness = 0.7)))
    var dup = modal_layer("bounce", ModalVoice())  // deliberately different params
    dup.delay = 0.12
    dup.link_source = 0
    sound |> push(dup)
    propagate_links(sound)
    // after propagation the linked layer's VOICE matches the source (its delay stays its own)
    let a <- render_voice(sound[0].voice)
    let b <- render_voice(sound[1].voice)
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
    t |> success(same, "linked layer mirrors source voice params")
    t |> success(sound[1].delay == 0.12, "linked layer keeps its own delay")
}
