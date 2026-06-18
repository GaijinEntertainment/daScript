options gen2
options indenting = 4
options persistent_heap

// Headless: render every GM percussion drum from a soundfont (Stolen preferred)
// to a mono 48kHz .wav reference, for A/B against the procedural / .sfx voices.
// Usage: daslang examples/daStrudel/sfx_lab/render_drums.das

require audio/audio_boost
require strudel/strudel_sf2
require strudel/strudel_sf2_voice
require audio/audio_wav
require daslib/fio
require math

struct DrumDef {
    name : string   // output file stem (matches the render_* synth names)
    key  : int      // GM percussion key (bank 128)
    dur  : float    // render duration, seconds
}

let DRUMS = [
    DrumDef(name = "bd",         key = 36, dur = 2.0),
    DrumDef(name = "sd",         key = 38, dur = 2.0),
    DrumDef(name = "hh",         key = 42, dur = 2.0),
    DrumDef(name = "oh",         key = 46, dur = 2.0),
    DrumDef(name = "cp",         key = 39, dur = 2.0),
    DrumDef(name = "tom_low",    key = 41, dur = 2.0),
    DrumDef(name = "tom_high",   key = 50, dur = 2.0),
    DrumDef(name = "ride",       key = 51, dur = 5.0),
    DrumDef(name = "crash",      key = 49, dur = 5.0),
    DrumDef(name = "rimshot",    key = 37, dur = 2.0),
    DrumDef(name = "cowbell",    key = 56, dur = 2.0),
    DrumDef(name = "tambourine", key = 54, dur = 2.0)
]

def find_sf2() : string {
    let dir = "{get_das_root()}/examples/daStrudel/midi_sf2_demo"
    var st : FStat
    for (name in fixed_array("Stolen Soundfont v2.08.2 (rev.3).sf2", "Musyng Kite.sf2",
                             "GeneralUser GS v1.471.sf2", "FluidR3_GM.sf2")) {
        if (stat("{dir}/{name}", st)) {
            return "{dir}/{name}"
        }
    }
    return ""
}

def render_sf2_drum(sf2 : SF2File; key, velocity : int; duration : float) : array<float> {
    let sample_rate = 48000
    let total_frames = int(duration * float(sample_rate))
    let note_off_frame = int(duration * 0.7 * float(sample_rate))
    let preset_idx = sf2_find_preset(sf2, 128, 0)
    if (preset_idx < 0) {
        var no_samples : array<float>
        return <- no_samples
    }
    let zones = sf2_find_zones(sf2, preset_idx, key, velocity)
    if (empty(zones)) {
        var no_samples : array<float>
        return <- no_samples
    }
    let preset = sf2.presets[preset_idx]
    var voices : array<ma_sf2_voice>
    var excl_classes : array<int>
    var atten_state = SF2VoiceAttenState()
    var cc_defaults : int[128]
    sf2_default_cc_values(cc_defaults)
    for (z in zones) {
        var pzone : SF2Zone const?
        for (pz in preset.zones) {
            let inst_gen = get_generator(*pz, int(SF2GenOper.instrument), -1)
            if (inst_gen == z.x) {
                pzone = pz
                break
            }
        }
        var cv : ma_sf2_voice
        if (sf2_create_c_voice(sf2, z.x, z.y, pzone, preset.global_zone, key, velocity, 0, cc_defaults, cv, atten_state)) {
            let excl = sf2_get_exclusive_class(sf2, z.x, z.y, pzone, preset.global_zone)
            if (excl > 0) {
                for (i in range(length(voices))) {
                    if (excl_classes[i] == excl && voices[i].finished == 0) {
                        unsafe { ma_sf2_voice_end_quick(addr(voices[i])); }
                    }
                }
            }
            voices |> push(cv)
            excl_classes |> push(excl)
        }
    }
    if (empty(voices)) {
        var no_samples : array<float>
        return <- no_samples
    }
    var output : array<float>
    output |> resize(total_frames * 2)
    let chunk_size = 512
    var frame = 0
    var note_off_sent = false
    while (frame < total_frames) {
        let frames = min(chunk_size, total_frames - frame)
        if (!note_off_sent && frame >= note_off_frame) {
            for (v in voices) {
                unsafe { ma_sf2_voice_note_off(addr(v)); }
            }
            note_off_sent = true
        }
        for (v in voices) {
            unsafe {
                ma_sf2_voice_render(addr(v), addr(sf2.sample_data[0]), length(sf2.sample_data), addr(output[0]), frame * 2, frames)
            }
        }
        frame += frames
    }
    return <- output
}

def to_mono(stereo : array<float>) : array<float> {
    let n = length(stereo) / 2
    var mono : array<float>
    mono |> resize(n)
    for (i in range(n)) {
        mono[i] = (stereo[i * 2] + stereo[i * 2 + 1]) * 0.5
    }
    return <- mono
}

[export]
def main {
    let sf2_path = find_sf2()
    if (empty(sf2_path)) {
        to_log(LOG_ERROR, "no soundfont found in examples/daStrudel/midi_sf2_demo\n")
        return
    }
    to_log(LOG_INFO, "loading {sf2_path}\n")
    let sf2 = sf2_load(sf2_path)
    if (sf2 == null) {
        to_log(LOG_ERROR, "sf2 load failed\n")
        return
    }
    let out_dir = "{get_das_root()}/examples/daStrudel/sfx_lab/drum_refs"
    mkdir(out_dir)
    for (d in DRUMS) {
        let stereo <- render_sf2_drum(*sf2, d.key, 127, d.dur)
        if (empty(stereo)) {
            to_log(LOG_WARNING, "  {d.name}: no zones for key {d.key} — skipped\n")
            continue
        }
        let mono <- to_mono(stereo)
        let path = "{out_dir}/{d.name}.wav"
        write_wav(path, mono, 48000u, 1)
        to_log(LOG_INFO, "  {d.name}: {length(mono)} samples -> {path}\n")
    }
}
