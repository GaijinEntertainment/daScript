options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dasllama/dasllama_pocket
require dasllama/dasllama_tts_types
require dasllama/dasllama_math
require dasllama/dasllama_metal_tower
require ../performance/profile_common.das   // tune_gate + apply_box_profile_runtime, the standing pre-measure stack
require daslib/clargs
require daslib/jobque_boost
require math

// The Pocket TTS stage probe: one chunk's frame loop alone and the codec alone over that
// chunk's latents, each `--reps` passes on the CPU chain and on the Metal tower in one process
// (the first tower pass pays the pipeline library), one row an arm a pass and a best-of row an
// arm at the end. The codec arm compares the tower's samples against the CPU chain's within
// `--bar`; the frame loop's arms are timing-only - the parity cells of tests/test_tts_pocket.das
// hold that loop's latents.
// ATTRIBUTION SWEEP: the frames arms attribute a chunk's wall between the loop and the codec.
//
//   bin/daslang -jit modules/dasLLAMA/harness/pocket_stage_probe.das -- \
//       -m ~/Work/llama.cpp/models/pocket-tts-en-q8.gguf \
//       -t "The quick brown fox jumps over the lazy dog." --voice alba

[CommandLineArgs]
struct ProbeArgs {
    @clarg_required
    @clarg_short = "m"
    @clarg_doc = "Pocket TTS GGUF"
    model : string

    @clarg_required
    @clarg_short = "t"
    @clarg_doc = "The chunk the frame loop speaks and the codec decodes"
    text : string

    @clarg_doc = "The roster voice the frames arm speaks with (default alba)"
    voice : string

    @clarg_short = "r"
    @clarg_doc = "Passes each way (default 3)"
    reps : int

    @clarg_doc = "The codec compare's rel-rms bar (default 5e-2, the served-lane bar; the f32 lane reads under 1e-6)"
    bar : float

    @clarg_doc = "Pin the Q8_0 lane (set_pocket_q8(true)); the default lane is the file's"
    q8 : bool

    @clarg_doc = "Pin the f32 lane (set_pocket_q8(false)) - the reference lane"
    f32 : bool

    @clarg_short = "h"
    @clarg_doc = "Show this help"
    help : bool
}

def private rel_rms(a, b : array<float>) : double {
    var num = 0.0lf
    var den = 0.0lf
    for (i in range64(min(long_length(a), long_length(b)))) {
        let d = double(a[i]) - double(b[i])
        num += d * d
        den += double(a[i]) * double(a[i])
    }
    return sqrt(num / max(den, 1e-30lf))
}

//! The codec over the chunk's latents: one row an arm a pass, the tower's compared within the bar;
//! false when a tower pass did not serve or missed the bar.
def private codec_arm(m : PocketModel; frames : int64; reps : int; bar : float; var sc : PocketScratch) : bool {
    var inscope lat : array<float>
    lat |> resize(frames * m.latent_dim)
    for (i in range64(frames * m.latent_dim)) {
        lat[i] = sc.latents[i]
    }
    var ok = true
    var best_cpu = 0
    var best_gpu = 0
    for (rep in range(reps)) {
        set_metal_tower(false)
        var inscope a : array<float>
        var t0 = ref_time_ticks()
        pocket_decode_latents(m, lat, frames, sc, a)
        let cpu_us = get_time_usec(t0)
        print("codec pass {rep} cpu: {frames} frames, {length(a)} samples, {cpu_us} us\n")
        set_metal_tower(true)
        let s0 = pocket_gpu_stats("codec")
        var inscope b : array<float>
        t0 = ref_time_ticks()
        pocket_decode_latents(m, lat, frames, sc, b)
        let gpu_us = get_time_usec(t0)
        let s1 = pocket_gpu_stats("codec")
        let served = s1.served - s0.served == 1
        let d = rel_rms(a, b)
        let hit = served && length(a) == length(b) && d <= double(bar)
        ok = ok && hit
        let verdict = hit ? "PASS" : "FAIL"
        let declined = served ? "" : " (declines {metal_tower_declines()})"
        print("codec pass {rep} tower: {frames} frames, {length(b)} samples, {gpu_us} us, served {s1.served - s0.served}/{s1.calls - s0.calls}, rel-rms {d} against the CPU chain, bar {bar}: {verdict}{declined}\n")
        best_cpu = rep == 0 ? cpu_us : min(best_cpu, cpu_us)
        best_gpu = rep == 0 ? gpu_us : min(best_gpu, gpu_us)
    }
    print("codec best-of cpu: {best_cpu} us over {reps} passes\n")
    print("codec best-of tower: {best_gpu} us over {reps} passes\n")
    return ok
}

//! The frame loop over the chunk: one row an arm a pass, timing-only - the tower's frame count
//! and whether it served are read, its latents are held by the parity cells; the frames made
//! on the tower's last pass, or -1 when a tower pass did not serve.
def private frames_arm(var m : PocketModel; cfg : ProbeArgs; var sc : PocketScratch) : int64 {
    let voice = empty(cfg.voice) ? "alba" : cfg.voice
    var frames = 0l
    var ok = true
    var best_cpu = 1.0e30lf
    var best_gpu = 1.0e30lf
    for (rep in range(cfg.reps)) {
        set_metal_tower(false)
        var tc = TtsTimings()
        let fc = pocket_speak(m, cfg.text, voice, 0.7, 0x5eedul, tc, sc)
        let cpu_frame = double(tc.backbone_us + tc.head_us) / 1000.0lf / double(max(fc, 1l))
        print("frames pass {rep} cpu timing-only: {fc} frames, {cpu_frame} ms a frame ({timings_line(tc, m.sample_rate)})\n")
        set_metal_tower(true)
        let s0 = pocket_gpu_stats("frames")
        var tg = TtsTimings()
        let fg = pocket_speak(m, cfg.text, voice, 0.7, 0x5eedul, tg, sc)
        let s1 = pocket_gpu_stats("frames")
        let served = s1.served - s0.served == 1
        ok = ok && served
        let gpu_frame = double(tg.backbone_us + tg.head_us) / 1000.0lf / double(max(fg, 1l))
        let declined = served ? "" : " (declines {metal_tower_declines()})"
        print("frames pass {rep} tower timing-only: {fg} frames, {gpu_frame} ms a frame, served {s1.served - s0.served}/{s1.calls - s0.calls} ({timings_line(tg, m.sample_rate)}){declined}\n")
        best_cpu = min(best_cpu, cpu_frame)
        best_gpu = min(best_gpu, gpu_frame)
        frames = fg
    }
    print("frames best-of cpu timing-only: {best_cpu} ms a frame over {cfg.reps} passes\n")
    print("frames best-of tower timing-only: {best_gpu} ms a frame over {cfg.reps} passes\n")
    return ok ? frames : -1l
}

[export, cold_path]
def main() : int {
    var r <- parse_args(type<ProbeArgs>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<ProbeArgs>), "pocket_stage_probe")
        return 1
    }
    var cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<ProbeArgs>), "pocket_stage_probe")
        return 0
    }
    if (cfg.reps <= 0) {
        cfg.reps = 3
    }
    if (cfg.bar <= 0.0) {
        cfg.bar = 5e-2
    }
    if (cfg.q8 || cfg.f32) {
        set_pocket_q8(cfg.q8)
    }
    apply_box_profile_runtime()
    if (!tune_gate()) {   // never measure on fallback winners
        panic("pocket_stage_probe: tune_gate refused - point DAS_TUNE_MANIFEST at a complete sidecar")
    }
    var rc = 0
    with_job_que() {
        setup_dasllama_jobque_()
        var inscope m <- load_pocket(cfg.model)
        var inscope sc = PocketScratch()
        let frames = frames_arm(m, cfg, sc)
        if (frames < 0l) {
            print("the tower declined a frames pass: set_metal_tower is inert on this box\n")
            rc = 2
        }
        if (!codec_arm(m, max(frames, 1l), cfg.reps, cfg.bar, sc)) {
            print("a codec tower pass did not serve, or missed the bar\n")
            rc = 2
        }
    }
    return rc
}
