options gen2
options indenting = 4

require dastest/testing_boost
require strudel/strudel_sfx
require math
require strings

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

[test]
def test_save_load_roundtrip(t : T?) {
    var sd = 9u
    var doc = Sound()
    doc.mimic_target = "/tmp/ref.wav"
    doc.layers |> push(modal_layer("thud", ModalVoice(pitch = 170.0, n_modes = 2)))
    var zap = sfxr_layer("zap", sfxr_pickup_coin(sd))
    zap.gain = 0.5
    zap.delay = 0.25
    zap.solo = true
    doc.layers |> push(zap)

    let path = "/tmp/_sfx_roundtrip_test.sfx"
    t |> success(save_sound(path, doc), "save ok")
    var back = Sound()
    t |> success(load_sound(path, back), "load ok")
    t |> equal(length(back.layers), 2)
    t |> equal(back.mimic_target, "/tmp/ref.wav")
    t |> success(back.layers[0].voice is modal, "layer 0 is modal")
    if (back.layers[0].voice is modal) {
        t |> equal((back.layers[0].voice as modal).pitch, 170.0)
    }
    t |> success(back.layers[1].voice is sfxr, "layer 1 is sfxr")
    t |> equal(back.layers[1].gain, 0.5)
    t |> equal(back.layers[1].delay, 0.25)
    t |> success(back.layers[1].solo, "solo flag round-trips")
    let buf <- render_sound(back.layers)
    t |> success(peak(buf) > 0.02, "loaded sound renders audible")
}

[test]
def test_sfx_conform_resample_pad_fade(t : T?) {
    // 0.1s constant tone, upsampled 44100 -> 48000 at half gain
    var buf : array<float>
    buf |> resize(4410)
    for (i in range(4410)) {
        buf[i] = 1.0
    }
    let DST = 48000
    let RLEN = 4800   // 4410 * 48000 / 44100, exact

    // duration 0: full natural length (a one-shot rings out — never truncated)
    let a <- sfx_conform(buf, SR, DST, 0.0, 0.5)
    t |> equal(length(a), RLEN)
    t |> success(abs(a[0]) < 1.0e-5, "fade-in zeroes the first sample (no onset click)")
    t |> success(abs(a[RLEN - 1]) < 1.0e-5, "fade-out zeroes the last natural sample (no release click)")
    t |> success(abs(a[2400] - 0.5) < 1.0e-3, "interior = const * gain after resample")
    t |> success(all_finite_in_range(a), "conformed samples in [-1,1]")

    // duration SHORTER than natural must NOT truncate (the cutoff/click bugfix)
    let short <- sfx_conform(buf, SR, DST, 0.05, 0.5)   // asks for 2400 < 4800 natural
    t |> equal(length(short), RLEN)

    // duration LONGER than natural extends via zero-pad
    let long <- sfx_conform(buf, SR, DST, 0.2, 0.5)     // asks for 9600 > 4800 natural
    t |> equal(length(long), 9600)
    t |> success(abs(long[9599]) < 1.0e-5, "tail beyond natural length is zero-padded")

    // empty input -> empty output
    let none : array<float>
    let e <- sfx_conform(none, SR, DST, 0.1, 1.0)
    t |> equal(length(e), 0)
}

[test]
def test_emit_das_shape(t : T?) {
    var sd = 5u
    var doc = Sound()
    doc.master_gain = 0.8
    doc.layers |> push(modal_layer("thud", ModalVoice(pitch = 170.0, n_modes = 2)))
    var zap = sfxr_layer("zap", sfxr_pickup_coin(sd))
    zap.gain = 0.5
    doc.layers |> push(zap)
    doc.post.reverb.enabled = true

    // standard one-shot form: def render_test(duration, gain : float) : array<float>
    let code = emit_das(doc, "render_test", false)
    t |> success(find(code, "require strudel/strudel_sfx") >= 0, "emits the require hint")
    t |> success(find(code, "def render_test(duration, gain : float) : array<float>") >= 0, "standard one-shot signature")
    t |> success(find(code, "modal_layer(\"thud\"") >= 0, "modal layer ctor")
    t |> success(find(code, "pitch = 170") >= 0, "emits the non-default modal pitch")
    t |> success(find(code, "n_modes = 2") >= 0, "emits the non-default mode count")
    t |> success(find(code, "sfxr_layer(\"zap\"") >= 0, "sfxr layer ctor")
    t |> success(find(code, ".gain = 0.5") >= 0, "emits the non-default layer gain")
    t |> success(find(code, "render_sound(layers)") >= 0, "calls the engine")
    t |> success(find(code, "apply_post(") >= 0, "applies the post chain (reverb enabled)")
    t |> success(find(code, "sfx_conform(out, 44100, 48000, duration, gain * 0.8") >= 0, "conforms to 48k with master_gain folded in")

    // parametric (tom) form: adds base_freq and scales the modal pitch by it
    let tom = emit_das(doc, "render_tom", true)
    t |> success(find(tom, "def render_tom(duration, gain, base_freq : float) : array<float>") >= 0, "parametric signature")
    t |> success(find(tom, "base_freq") >= 0, "modal pitch scaled by base_freq")
}
