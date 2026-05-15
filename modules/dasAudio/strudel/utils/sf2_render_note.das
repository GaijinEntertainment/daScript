options gen2
options indenting = 4

// Render a single SF2 note to a WAV file for comparison with reference.
// Usage: daslang.exe sf2_render_note.das -- path/to/soundfont.sf2 [output_dir]
// Default output_dir: current directory

require audio
require strudel/strudel_sf2
require strudel/strudel_sf2_voice
require audio/audio_wav
require daslib/strings_boost
require math

[export]
def main() {
    let args <- get_command_line_arguments()
    var sf2_path = ""
    var output_dir = "."
    var pos = 0
    for (arg in args) {
        if (ends_with(arg, ".sf2") || ends_with(arg, ".SF2")) {
            sf2_path = arg
        } elif (pos > 0 && !ends_with(arg, ".sf2") && !ends_with(arg, ".SF2") && arg != "--") {
            output_dir = arg
        }
        if (arg == "--") {
            pos = 1
        }
    }
    if (empty(sf2_path)) {
        print("Usage: daslang.exe sf2_render_note.das -- path/to/soundfont.sf2 [output_dir]\n")
        return
    }

    var sf2 = sf2_load(sf2_path)
    if (sf2 == null) {
        print("Failed to load SF2: {sf2_path}\n")
        return
    }

    // render piano C4 at velocity 127
    render_note(*sf2, 0, 0, 60, 127, 2.0, "{output_dir}/das_piano_p0_k60_v127.wav")
    // render piano C2 at velocity 127
    render_note(*sf2, 0, 0, 36, 127, 2.0, "{output_dir}/das_piano_p0_k36_v127.wav")
    // render piano C6 at velocity 127
    render_note(*sf2, 0, 0, 84, 127, 2.0, "{output_dir}/das_piano_p0_k84_v127.wav")
    // render guitar
    render_note(*sf2, 0, 25, 64, 127, 2.0, "{output_dir}/das_guitar_p25_k64_v127.wav")
    // render drums
    render_note(*sf2, 128, 0, 36, 127, 1.0, "{output_dir}/das_drums_p128_k36_v127.wav")
    render_note(*sf2, 128, 0, 42, 127, 1.0, "{output_dir}/das_drums_p128_k42_v127.wav")
    render_note(*sf2, 128, 0, 38, 127, 1.0, "{output_dir}/das_drums_p128_k38_v127.wav")

    print("Done rendering test notes\n")
    unsafe { delete sf2; }
}

def render_note(sf2 : SF2File; bank, program, key, velocity : int; duration : float; output_path : string) {
    let sample_rate = 48000
    let total_frames = int(duration * float(sample_rate))
    let release_time = duration > 1.0 ? 0.5 : duration * 0.3
    let note_off_frame = int((duration - release_time) * float(sample_rate))

    let preset_idx = sf2_find_preset(sf2, bank, program)
    if (preset_idx < 0) {
        print("  preset bank={bank} program={program} not found\n")
        return
    }

    var zones <- sf2_find_zones(sf2, preset_idx, key, velocity)
    if (empty(zones)) {
        delete zones
        print("  no zones for preset={program} key={key} vel={velocity}\n")
        return
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
        print("  no voices created for preset={program} key={key}\n")
        return
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

    write_wav(output_path, output, [sample_rate = uint(sample_rate)])
    print("  rendered bank={bank} prog={program} key={key} vel={velocity} dur={duration}s -> {output_path} ({length(voices)} voices)\n")
    delete zones
}
