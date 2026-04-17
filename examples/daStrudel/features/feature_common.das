options gen2
options persistent_heap
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module feature_common shared public

require strudel/strudel public
require strudel/strudel_player public
require audio/audio_boost
require audio/audio_wav
require daslib/fio
require strings

var public default_feature_duration = 10.0

// Parse --duration N, --track-memory, and --wav PATH from command line args.
// Returns (duration_seconds, track_memory, wav_path).
struct FeatureArgs {
    duration : float = 10.0
    track_memory : bool = false
    wav_path : string
}

def parse_feature_args() : FeatureArgs {
    var res = FeatureArgs(duration = default_feature_duration)
    let args <- get_command_line_arguments()
    var i = 0
    while (i < length(args)) {
        if (args[i] == "--duration" && i + 1 < length(args)) {
            res.duration = float(to_float(args[i + 1]))
            i += 2
        } elif (args[i] == "--track-memory") {
            res.track_memory = true
            i ++
        } elif (args[i] == "--wav" && i + 1 < length(args)) {
            res.wav_path = args[i + 1]
            i += 2
        } else {
            i ++
        }
    }
    return res
}

// Main-thread mode playback: init audio, create channel, tick in loop, shutdown.
// Uses main-thread mode (strudel_create_channel + strudel_tick) so we stay in
// one context — memory tracking sees everything.
// Optional setup block runs after channel creation (for loading samples, etc.).
// If --wav PATH is given, renders offline to WAV file instead of playing audio.
def play_feature_cps(var pat : Pattern; cps : double = 0.5lf; setup : block = $ {}) {
    let args = parse_feature_args()
    let duration = args.duration
    let track_memory = args.track_memory
    let wav_path = args.wav_path
    if (!empty(wav_path)) {
        // --- Offline WAV render (no audio driver) ---
        strudel_set_cps(cps)
        strudel_debug_memory(track_memory)
        invoke(setup)
        strudel_add_track(pat)
        strudel_reset_memory_baseline()
        var all_samples : array<float>
        let total_frames = int(duration * 48000.0)
        all_samples |> reserve(total_frames * 2)
        to_log(0, "rendering {duration} seconds to {wav_path}...\n")
        while (g_wall_time < double(duration)) {
            strudel_tick_offline()
            for (s in g_master_pcm) {
                all_samples |> push(s)
            }
        }
        write_wav(wav_path, all_samples, 48000u, 2)
        to_log(0, "wrote {wav_path} ({length(all_samples) / 2} frames)\n")
        delete all_samples
        strudel_shutdown()
        if (track_memory) {
            to_log(0, "=== HEAP AFTER SHUTDOWN ===\n")
            heap_report()
        }
    } else {
        // --- Audio playback ---
        with_audio_system() {
            strudel_create_channel()
            strudel_set_cps(cps)
            strudel_debug_memory(track_memory)
            invoke(setup)
            strudel_add_track(pat)
            strudel_reset_memory_baseline()  // baseline after all setup; shutdown frees before measuring
            if (track_memory) {
                to_log(0, "=== HEAP BEFORE PLAYBACK ===\n")
                heap_report()
            }
            let t0 = ref_time_ticks()
            while (float(get_time_usec(t0)) / 1000000.0 < duration) {
                strudel_tick()
                sleep(5u)
            }
            strudel_shutdown()
            if (track_memory) {
                to_log(0, "=== HEAP AFTER SHUTDOWN ===\n")
                heap_report()
            }
        }
    }
}

// Same as play_feature_cps but takes BPM (converts to cps = bpm / 60 / 2).
def play_feature(var pat : Pattern; bpm : double = 120.0lf; setup : block = $ {}) {
    play_feature_cps(pat, bpm / 120.0lf, setup)
}

// Same as play_feature but with cycles per minute.
def play_feature_cpm(var pat : Pattern; cpm : double = 15.0lf) {
    play_feature_cps(pat, cpm / 60.0lf)
}
