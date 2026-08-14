options gen2
options indenting = 4

// Automated A/B comparison: daScript SF2 renderer vs reference WAVs
// Usage: daslang.exe sf2_compare.das -- path/to/soundfont.sf2 path/to/ref_dir

require audio
require strudel/strudel_sf2
require strudel/strudel_sf2_voice
require audio/audio_wav
require daslib/strings_boost
require math

struct CompareResult {
    name : string
    preset : int
    key : int
    velocity : int
    rms_error : float       // normalized RMS of difference
    peak_error : float      // max absolute sample difference
    env_corr : float        // envelope correlation (RMS-in-windows)
    passed : bool
}

struct TestCase {
    preset : int
    bank : int
    name : string
    key : int
    velocity : int
    duration : float
}

let RMS_THRESHOLD = 0.25        // 25% RMS error (allows quick-release timing differences)
let PEAK_THRESHOLD = 1.0        // peak error not a useful threshold (phase-dependent)
let ENV_CORR_THRESHOLD = 0.95   // 95% envelope correlation

def private parse_args(var sf2_path : string&; var ref_dir : string&) : bool {
    let args <- get_command_line_arguments()
    for (arg in args) {
        if (ends_with(arg, ".sf2") || ends_with(arg, ".SF2")) {
            sf2_path = arg
        } elif (arg != "--" && !empty(sf2_path) && empty(ref_dir)) {
            ref_dir = arg
        }
    }
    return !empty(sf2_path) && !empty(ref_dir)
}

def private sf2_test_cases() : array<TestCase> {
    return <- [
        // Piano
        TestCase(preset = 0, bank = 0, name = "piano", key = 36, velocity = 127, duration = 2.0),
        TestCase(preset = 0, bank = 0, name = "piano", key = 60, velocity = 127, duration = 2.0),
        TestCase(preset = 0, bank = 0, name = "piano", key = 84, velocity = 127, duration = 2.0),
        TestCase(preset = 0, bank = 0, name = "piano", key = 60, velocity = 96, duration = 2.0),
        TestCase(preset = 0, bank = 0, name = "piano", key = 60, velocity = 32, duration = 2.0),
        // Guitar
        TestCase(preset = 25, bank = 0, name = "guitar", key = 64, velocity = 127, duration = 2.0),
        TestCase(preset = 25, bank = 0, name = "guitar", key = 52, velocity = 64, duration = 2.0),
        // Cello
        TestCase(preset = 42, bank = 0, name = "cello", key = 48, velocity = 127, duration = 2.0),
        // Trumpet
        TestCase(preset = 56, bank = 0, name = "trumpet", key = 60, velocity = 127, duration = 2.0),
        // Flute
        TestCase(preset = 73, bank = 0, name = "flute", key = 72, velocity = 127, duration = 2.0),
        // Drums
        TestCase(preset = 128, bank = 128, name = "drums", key = 36, velocity = 127, duration = 1.0),
        TestCase(preset = 128, bank = 128, name = "drums", key = 38, velocity = 127, duration = 1.0),
        TestCase(preset = 128, bank = 128, name = "drums", key = 42, velocity = 127, duration = 1.0)
    ]
}

// Renders one case, prints its row, and reports the metrics. A skipped case comes
// back with `rms_error` negative — the same marker `print_row` uses.
def private compare_case(sf2 : SF2File; tc : TestCase; ref_dir : string) : CompareResult {
    let skipped = CompareResult(name = tc.name, preset = tc.preset, key = tc.key,
        velocity = tc.velocity, rms_error = -1.0, peak_error = -1.0, env_corr = -1.0, passed = false)
    let ref_path = "{ref_dir}/ref_{tc.name}_p{tc.preset}_k{tc.key}_v{tc.velocity}.wav"
    var ref_data : array<float>
    var ref_rate = 0
    var ref_channels = 0
    if (!read_wav(ref_path, ref_data, ref_rate, ref_channels)) {
        print_row(tc.name, tc.preset, tc.key, tc.velocity, -1.0, -1.0, -1.0, "SKIP(ref)")
        return skipped
    }

    var das_data : array<float>
    render_note(sf2, tc.bank, tc.preset, tc.key, tc.velocity, tc.duration, ref_rate, das_data)
    if (empty(das_data)) {
        print_row(tc.name, tc.preset, tc.key, tc.velocity, -1.0, -1.0, -1.0, "SKIP(voice)")
        return skipped
    }

    let n = min(length(ref_data), length(das_data))
    let rms_err = compute_rms_error(ref_data, das_data, n)
    let peak_err = compute_peak_error(ref_data, das_data, n)
    let env_corr = compute_envelope_correlation(ref_data, das_data, n, ref_rate)
    let passed = rms_err < RMS_THRESHOLD && peak_err < PEAK_THRESHOLD && env_corr > ENV_CORR_THRESHOLD
    print_row(tc.name, tc.preset, tc.key, tc.velocity, rms_err, peak_err, env_corr, passed ? "PASS" : "FAIL")
    return CompareResult(name = tc.name, preset = tc.preset, key = tc.key, velocity = tc.velocity,
        rms_error = rms_err, peak_error = peak_err, env_corr = env_corr, passed = passed)
}

[export]
def main() {
    var sf2_path = ""
    var ref_dir = ""
    if (!parse_args(sf2_path, ref_dir)) {
        print("Usage: daslang.exe sf2_compare.das -- path/to/soundfont.sf2 path/to/ref_dir\n")
        return
    }

    var sf2 = sf2_load(sf2_path)
    if (sf2 == null) {
        print("Failed to load SF2: {sf2_path}\n")
        return
    }

    var cases <- sf2_test_cases()
    var results : array<CompareResult>
    var total_pass = 0
    var total_fail = 0

    print("name          preset  key  vel   rms_err  peak_err  env_corr  result\n")
    print("------------------------------------------------------------------------\n")

    for (tc in cases) {
        let r = compare_case(*sf2, tc, ref_dir)
        continue if (r.rms_error < 0.0)
        if (r.passed) {
            total_pass ++
        } else {
            total_fail ++
        }
        results |> push(r)
    }

    print("------------------------------------------------------------------------\n")
    print("{total_pass + total_fail} tests: {total_pass} passed, {total_fail} failed\n")

    unsafe { delete sf2; }
}

// ─── Render ───

def render_note(sf2 : SF2File; bank, preset_num, key, velocity : int; duration : float; sample_rate : int; var output : array<float>&) {
    let total_frames = int(duration * float(sample_rate))
    let release_time = duration > 1.0 ? 0.5 : duration * 0.3
    let note_off_frame = int((duration - release_time) * float(sample_rate))

    let actual_bank = bank >= 128 ? 128 : 0
    let actual_preset = bank >= 128 ? 0 : preset_num
    let preset_idx = sf2_find_preset(sf2, actual_bank, actual_preset)
    if (preset_idx < 0) return

    var zones <- sf2_find_zones(sf2, preset_idx, key, velocity)
    if (empty(zones)) {
        delete zones
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
                for (v, ec in voices, excl_classes) {
                    if (ec == excl && v.finished == 0) {
                        unsafe { ma_sf2_voice_end_quick(addr(v)); }
                    }
                }
            }
            voices |> push(cv)
            excl_classes |> push(excl)
        }
    }

    if (empty(voices)) return

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
    delete zones
}

// ─── Output ───

def print_row(name : string; preset, key, velocity : int; rms_err, peak_err, env_corr : float; tag : string) {
    if (rms_err < 0.0) {
        print("{name}\t p={preset}\t k={key}\t v={velocity}\t {tag}\n")
    } else {
        print("{name}\t p={preset}\t k={key}\t v={velocity}\t rms={rms_err}\t peak={peak_err}\t env={env_corr}\t {tag}\n")
    }
}

// ─── Metrics ───

def compute_rms_error(ref_data, das_data : array<float>; n : int) : float {
    var sum_diff_sq = 0.0lf
    var sum_ref_sq = 0.0lf
    for (i in range(n)) {
        let d = double(das_data[i] - ref_data[i])
        sum_diff_sq += d * d
        let r = double(ref_data[i])
        sum_ref_sq += r * r
    }
    if (sum_ref_sq < 1e-10lf) return sum_diff_sq > 1e-10lf ? 1.0 : 0.0
    return float(sqrt(sum_diff_sq / sum_ref_sq))
}

def compute_peak_error(ref_data, das_data : array<float>; n : int) : float {
    var max_diff = 0.0
    var max_ref = 0.0
    for (i in range(n)) {
        let d = abs(das_data[i] - ref_data[i])
        if (d > max_diff) {
            max_diff = d
        }
        let r = abs(ref_data[i])
        if (r > max_ref) {
            max_ref = r
        }
    }
    if (max_ref < 1e-6) return max_diff > 1e-6 ? 1.0 : 0.0
    return max_diff / max_ref
}

def compute_envelope_correlation(ref_data, das_data : array<float>; n : int; sample_rate : int) : float {
    let window_size = sample_rate / 100 * 2  // 10ms in stereo samples
    if (window_size <= 0 || n < window_size) return 0.0
    let num_windows = n / window_size

    var ref_env : array<float>
    var das_env : array<float>
    ref_env |> resize(num_windows)
    das_env |> resize(num_windows)

    for (w in range(num_windows)) {
        var ref_sum = 0.0lf
        var das_sum = 0.0lf
        let base = w * window_size
        for (i in range(window_size)) {
            let rv = double(ref_data[base + i])
            let dv = double(das_data[base + i])
            ref_sum += rv * rv
            das_sum += dv * dv
        }
        ref_env[w] = float(sqrt(ref_sum / double(window_size)))
        das_env[w] = float(sqrt(das_sum / double(window_size)))
    }

    // Pearson correlation
    var mean_ref = 0.0lf
    var mean_das = 0.0lf
    for (w in range(num_windows)) {
        mean_ref += double(ref_env[w])
        mean_das += double(das_env[w])
    }
    mean_ref /= double(num_windows)
    mean_das /= double(num_windows)

    var cov = 0.0lf
    var var_ref = 0.0lf
    var var_das = 0.0lf
    for (w in range(num_windows)) {
        let dr = double(ref_env[w]) - mean_ref
        let dd = double(das_env[w]) - mean_das
        cov += dr * dd
        var_ref += dr * dr
        var_das += dd * dd
    }

    let denom = sqrt(var_ref * var_das)
    if (denom < 1e-10lf) return 0.0
    return float(cov / denom)
}
