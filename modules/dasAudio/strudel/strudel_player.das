options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module strudel_player shared public
//! Top-level playback harness for strudel patterns.
//! Manages tracks, BPM/CPS, sample/SF2 banks, and a command channel for both
//! main-thread (strudel_tick) and dedicated-thread (strudel_play) playback modes.

require strudel/strudel_event
require strudel/strudel_time
require strudel/strudel_pattern
require strudel/strudel_synth
require strudel/strudel_samples
require strudel/strudel_scheduler
require strudel/strudel_sf2
require audio/audio_boost
require audio
require daslib/jobque_boost
require daslib/archive
require daslib/fio
require daslib/strings_boost
require math

// --- Command channel ---

variant private StrudelCommand {
    shutdown : bool;
    set_bpm  : double;
    set_cps  : double;
    user_cmd : string;
}

// --- Track ---

[safe_when_uninitialized]
struct private StrudelTrack {
    pat         : Pattern
    @safe_when_uninitialized sched : Scheduler
    gain        : float = 1.0
    target_gain : float = 1.0
    fade_speed  : float = 0.0
}

// --- Module globals ---

var private g_bank : SampleBank
var private g_bank_ptr : SampleBank?   // for threaded mode: points to main thread's bank
var private g_sid : uint64 = 0ul
//! Current playback wall-clock time in seconds. Derived from exact sample count.
//! Written by the playback thread (threaded mode) or strudel_tick (main-thread mode).
var public g_wall_time : double = 0.0lf
var private g_wall_samples : int64 = 0l  // exact sample count — g_wall_time derived from this
//! Current tempo in cycles per second (0.5 = 120 BPM at 4 beats/cycle).
var public g_cps : double = 0.5lf
var private g_cmd_channel : Channel?
var private g_done_channel : Channel?
var private g_sf2 : SF2File?
var private g_sf2_path : string
var private g_cmd_fn : function<(cmd : string) : void>
var private g_playback_box : LockBox?
var private g_tracks : array<StrudelTrack?>
var private g_look_ahead : float = 0.1
var private g_chunk_seconds : float = 0.05
var private g_tick_status_box : LockBox?  // buffer level feedback for strudel_tick
var private g_tick_pcm_box : LockBox?    // lock box for append_box_to_pcm in main-thread mode

//! Per-track PCM output written by strudel_tick (main-thread mode only), indexed by track index.
//! Visualizer combinators read this to render per-track waveforms.
var public g_track_pcm : array<array<float>>
//! Master mixed stereo PCM output for the most recent tick (main-thread mode only).
var public g_master_pcm : array<float>

//! Rolling visualizer index — incremented by strudel_add_track, read by visualizer combinators.
var public g_vis_index : int = 0

// --- Memory tracking ---
var private g_debug_memory : bool = false
var private g_mem_heap_baseline : uint64 = 0ul
var private g_mem_str_baseline : uint64 = 0ul
var private g_mem_heap_max : uint64 = 0ul
var private g_mem_str_max : uint64 = 0ul
var private g_mem_tick_count : int = 0  // for periodic logging in main-thread mode

// --- Tick timing ---
var private g_tick_total_time : double = 0.lf       // total tick time in ms
var private g_tick_total_samples : uint64 = 0ul     // total samples rendered
var private g_tick_max_chunk_usec : double = 0.lf   // peak chunk time in usec

//! A PCM audio block passed between the playback thread and the visualizer or saver.
struct AudioChunk {
    samples : array<float>
        //! Interleaved stereo PCM samples for one chunk.
}

struct private PlaybackTime {
    wall_time : double
    cps       : double
}

// --- Playback time ---

def public strudel_get_playback_time() : tuple<double; double> {
    //! Get current playback wall-clock time and tempo as (wall_time, cps).
    //! In threaded mode reads from a lockbox; in main-thread mode uses the hardware
    //! consumed position (smooth, matches what you hear) and lags g_wall_time by the audio buffer depth.
    var wt = 0.0lf
    var cp = 0.5lf
    if (g_playback_box != null) {
        // threaded mode — read from lockbox
        g_playback_box |> get() $(pt : PlaybackTime#) {
            wt = pt.wall_time
            cp = pt.cps
        }
    } elif (g_tick_status_box != null) {
        // main-thread mode — use consumed_position once audio device starts,
        // fall back to g_wall_time during startup (before device begins consuming)
        var consumed = 0ul
        g_tick_status_box |> get() $(st : AudioChannelStatus#) {
            consumed = st.consumed_position
        }
        wt = consumed > 0ul ? double(consumed) / double(SAMPLE_RATE) : g_wall_time
        cp = g_cps
    } else {
        wt = g_wall_time
        cp = g_cps
    }
    return wt => cp
}

// --- Resource loading ---

def public strudel_load_sound(path, name : string) {
    //! Load a single audio sample from disk and register it under the given name. Duplicates are ignored.
    let sname = to_lower(name)
    if (key_exists(g_bank.sounds, sname)) {
        return
    }
    load_sound(path, name, g_bank)
}

def public strudel_load_sample_dir(root : string) {
    //! Load every audio file under root as a named sample (filename becomes the sound name).
    dir(root) $(entry) {
        if (entry == "." || entry == "..") {
            return
        }
        let sname = to_lower(entry)
        if (key_exists(g_bank.sounds, sname)) {
            return
        }
        load_sound("{root}/{entry}", entry, g_bank)
    }
}

def public strudel_load_sf2(path : string) : bool {
    //! Load a SoundFont (.sf2) file for SF2-based synthesis. Returns true on success.
    g_sf2 = sf2_load(path)
    if (g_sf2 != null) {
        g_sf2_path = path
    }
    return g_sf2 != null
}

def public strudel_get_sf2_path() : string {
    //! Return the path of the currently loaded SoundFont (empty string if none).
    return g_sf2_path
}

def public strudel_has_sf2() : bool {
    //! Return true if a SoundFont has been loaded.
    return g_sf2 != null
}

// --- Volume / Pause ---

def public strudel_set_volume(volume : float; time : float = 0.0) {
    //! Set the master playback volume, optionally ramping to it over the given time in seconds.
    if (g_sid != 0ul) {
        set_volume(g_sid, volume, time)
    }
}

def public strudel_set_pause(paused : bool) {
    //! Pause or resume audio output.
    if (g_sid != 0ul) {
        set_pause(g_sid, paused)
    }
}

// --- Tempo (affects both threads) ---

def public strudel_set_bpm(bpm : double) {
    //! Set tempo in beats per minute (assumes 4 beats per cycle). Safe to call from the main thread.
    g_cps = bpm / 240.0lf
    if (g_cmd_channel != null) {
        g_cmd_channel |> push(new StrudelCommand(set_bpm = bpm))
    }
}

def public strudel_set_cps(cps : double) {
    //! Set tempo directly in cycles per second. Safe to call from the main thread.
    g_cps = cps
    if (g_cmd_channel != null) {
        g_cmd_channel |> push(new StrudelCommand(set_cps = cps))
    }
}

def public strudel_set_look_ahead(look_ahead : float) {
    //! Set how far ahead (in seconds) to query the pattern for upcoming events.
    g_look_ahead = look_ahead
}

def public strudel_debug_memory(enabled : bool) {
    //! Enable or disable periodic heap/string-memory logging during playback.
    g_debug_memory = enabled
}

def public strudel_reset_memory_baseline() {
    //! Reset memory-tracking baseline to the current heap state.
    //! Call after all one-time setup (audio stream, pattern, tracks) so only playback growth is measured.
    g_mem_heap_baseline = heap_bytes_allocated()
    g_mem_str_baseline = string_heap_bytes_allocated()
    g_mem_heap_max = g_mem_heap_baseline
    g_mem_str_max = g_mem_str_baseline
    g_mem_tick_count = 0
}

def public strudel_debug_sample_peaks() {
    //! Log the peak amplitude of every loaded sample in the bank (debug helper).
    for (name in keys(g_bank.sounds)) {
        var samples <- render_sample(g_bank, name, 0, 1.0)
        var peak = 0.0
        for (i in range(length(samples))) {
            peak = max(peak, abs(samples[i]))
        }
        to_log(0, "sample '{name}': {length(samples)} samples, peak={peak}\n")
        delete samples
    }
}

def public strudel_debug_master_peak() {
    //! Log the peak amplitude of the current master PCM buffer (debug helper, main-thread mode).
    if (length(g_master_pcm) > 0) {
        var peak = 0.0
        for (i in range(length(g_master_pcm))) {
            peak = max(peak, abs(g_master_pcm[i]))
        }
        if (peak > 0.001) {
            to_log(0, "master pcm peak={peak}\n")
        }
    }
}

def public strudel_debug_output_peak() {
    //! Log the peak amplitude of each track's scheduler output (debug helper).
    for (i in range(length(g_tracks))) {
        var track = g_tracks[i]
        if (track != null && length(track.sched.output) > 0) {
            var peak = 0.0
            for (j in range(length(track.sched.output))) {
                peak = max(peak, abs(track.sched.output[j]))
            }
            if (peak > 0.001) {
                to_log(0, "track {i}: output peak={peak}\n")
            }
        }
    }
}

def public strudel_debug_voices() {
    //! Log the number of active sample/oscillator voices on each track (debug helper).
    for (i in range(length(g_tracks))) {
        var track = g_tracks[i]
        if (track != null) {
            to_log(0, "track {i}: voices={length(track.sched.voices)} osc={length(track.sched.osc_voices)}\n")
        }
    }
}

// --- Track management ---

def public strudel_add_track(var pat : Pattern; gain : float = 1.0) : int {
    //! Add a track playing the given pattern at the given gain. Returns its index (matches g_vis_index for visualizer combinators).
    let idx = g_vis_index
    g_tracks |> push(new StrudelTrack(
        pat <- pat,
        gain = gain,
        target_gain = gain
    ))
    var track = g_tracks[length(g_tracks) - 1]
    track.sched.cps = g_cps
    track.sched.lastQueryEnd = g_wall_time * g_cps
    track.sched.sf2 = g_sf2 != null ? unsafe(reinterpret<SF2File const?>(g_sf2)) : null
    init_chorus(track.sched)
    if (g_sf2 != null) {
        init_reverb(track.sched)
    }
    // ensure per-track PCM arrays are big enough
    while (length(g_track_pcm) <= idx) {
        var empty : array<float>
        g_track_pcm |> emplace(empty)
    }
    g_vis_index ++
    return idx
}

def public strudel_remove_track(idx : int) {
    //! Remove a track by index (shuts down its scheduler and sets the slot to null).
    if (idx >= 0 && idx < length(g_tracks) && g_tracks[idx] != null) {
        shutdown_scheduler(g_tracks[idx].sched)
        unsafe { delete g_tracks[idx]; }
        g_tracks[idx] = null
    }
}

def public strudel_fade_track(idx : int; target_gain : float; time : float) {
    //! Fade a track's gain to target_gain over the given time in seconds (time=0 snaps instantly).
    if (idx >= 0 && idx < length(g_tracks) && g_tracks[idx] != null) {
        var track = g_tracks[idx]
        track.target_gain = target_gain
        if (time > 0.0) {
            track.fade_speed = abs(track.target_gain - track.gain) / time
        } else {
            track.gain = target_gain
            track.fade_speed = 0.0
        }
    }
}

// Find track by name — NOT supported in new API. User maintains their own name->index mapping.
// Use the index returned by strudel_add_track.

// --- Commands (main -> thread) ---

def public strudel_command(cmd : string) {
    //! Send a user command string from the main thread to the strudel playback thread.
    //! The command is dispatched to the cmd_fn passed to strudel_init.
    if (g_cmd_channel != null) {
        var sc = new StrudelCommand(shutdown = false)
        unsafe {
            set_variant_index(*sc, 3)  // user_cmd
            sc.user_cmd = cmd
        }
        g_cmd_channel |> push(sc)
    }
}

def public strudel_noop_cmd(cmd : string) {
    //! No-op command handler — default argument when no user command dispatcher is needed.
    pass
}

// --- Command processing (called on strudel thread) ---

def private strudel_process_commands(cmd_fn : function<(cmd : string) : void>) : bool {
    if (g_cmd_channel == null) {
        return true
    }
    var running = true
    g_cmd_channel |> gather() $(cmd : StrudelCommand#) {
        if (cmd is shutdown) {
            running = false
        } elif (cmd is set_bpm) {
            g_cps = (cmd as set_bpm) / 240.0lf
        } elif (cmd is set_cps) {
            g_cps = cmd as set_cps
        } elif (cmd is user_cmd) {
            let ucmd = string(cmd as user_cmd)
            invoke(cmd_fn, ucmd)
        }
    }
    return running
}

// --- Main-thread mode ---

def public strudel_create_channel() {
    //! Create the PCM stream for main-thread playback. Call once after audio_system_create() and before strudel_tick.
    if (g_sid == 0ul) {
        g_sid = play_sound_from_pcm_stream(MA_SAMPLE_RATE, MA_CHANNELS)
        g_tick_status_box = unsafe(lock_box_create())
        g_tick_status_box |> add_ref()
        set_status_update(g_sid, g_tick_status_box)
        g_tick_pcm_box = unsafe(lock_box_create())
        // memory tracking baseline
        g_mem_heap_baseline = heap_bytes_allocated()
        g_mem_str_baseline = string_heap_bytes_allocated()
        g_mem_heap_max = g_mem_heap_baseline
        g_mem_str_max = g_mem_str_baseline
        g_mem_tick_count = 0
    }
}

def public strudel_tick() {
    //! Single main-thread tick: query all tracks, render audio, mix, append to the PCM stream.
    //! Call from your render loop; self-regulating — skips when the audio buffer has enough data queued.
    if (g_sid == 0ul) {
        return
    }
    // wait for previous pcm box to be consumed before overwriting the buffer
    if (g_tick_pcm_box != null && !g_tick_pcm_box.isReady) {
        return
    }
    // check buffer level — skip if we're ahead enough
    if (g_tick_status_box != null) {
        var que_len = 0
        g_tick_status_box |> get() $(st : AudioChannelStatus#) {
            que_len = st.stream_que_length
        }
        if (que_len >= 4) {
            return
        }
    }
    let t0 = ref_time_ticks()
    let CHUNK_SEC = g_chunk_seconds
    let chunkSamples = int(float(CHUNK_SEC) * float(SAMPLE_RATE)) * 2
    // prepare master output
    g_master_pcm |> resize(chunkSamples)
    for (s in g_master_pcm) {
        s = 0.0
    }
    // master mixer — linear pan (no cos/sin attenuation at center)
    var mixer : ma_volume_mixer
    ma_volume_mixer_init(unsafe(addr(mixer)), 2u)
    // tick all active tracks
    for (i in range(length(g_tracks))) {
        var track = g_tracks[i]
        if (track == null) {
            continue
        }
        // sync tempo
        if (track.sched.cps != g_cps) {
            track.sched.lastQueryEnd = g_wall_time * g_cps
            track.sched.cps = g_cps
        }
        tick(track.sched, track.pat, g_bank, g_wall_time, float(CHUNK_SEC), g_look_ahead)
        // mix scheduler output into master with track gain
        if (length(track.sched.output) > 0 && track.gain > 0.001) {
            // copy to per-track PCM for visualizer access
            if (i < length(g_track_pcm)) {
                g_track_pcm[i] := track.sched.output
            }
            ma_volume_mixer_set_volume(unsafe(addr(mixer)), track.gain)
            let nFrames = uint64(min(length(track.sched.output), chunkSamples) / 2)
            unsafe {
                ma_volume_mixer_process_pcm_frames(
                    addr(mixer),
                    addr(track.sched.output[0]),
                    addr(g_master_pcm[0]),
                    nFrames
                )
            }
        }
        // update fade envelope
        if (track.fade_speed > 0.0) {
            if (track.gain < track.target_gain) {
                track.gain = min(track.gain + track.fade_speed * float(CHUNK_SEC), track.target_gain)
            } elif (track.gain > track.target_gain) {
                track.gain = max(track.gain - track.fade_speed * float(CHUNK_SEC), track.target_gain)
            }
            if (abs(track.gain - track.target_gain) < 0.001) {
                track.gain = track.target_gain
                track.fade_speed = 0.0
            }
        }
        // remove faded-out tracks
        if (track.gain <= 0.001 && track.target_gain <= 0.001 && track.fade_speed == 0.0) {
            shutdown_scheduler(track.sched)
            unsafe { delete g_tracks[i]; }
            g_tracks[i] = null
        }
    }
    // submit audio via lock box (no allocation — reuses g_master_pcm buffer)
    if (length(g_master_pcm) > 0) {
        append_box_to_pcm(g_sid, g_tick_pcm_box, g_master_pcm)
    }
    let chunkFrames = int(float(CHUNK_SEC) * float(SAMPLE_RATE))
    g_wall_samples += int64(chunkFrames)
    g_wall_time = double(g_wall_samples) / double(SAMPLE_RATE)
    ma_volume_mixer_uninit(unsafe(addr(mixer)))
    // tick timing
    let chunk_usec = double(get_time_usec(t0))
    g_tick_total_time += chunk_usec / 1000.lf
    g_tick_total_samples += uint64(chunkSamples / 2)
    if (chunk_usec > g_tick_max_chunk_usec) { g_tick_max_chunk_usec = chunk_usec; }
    // memory tracking
    let mem_heap_cur = heap_bytes_allocated()
    let mem_str_cur = string_heap_bytes_allocated()
    g_mem_heap_max = max(g_mem_heap_max, mem_heap_cur)
    g_mem_str_max = max(g_mem_str_max, mem_str_cur)
    g_mem_tick_count ++
    let mem_log_interval = max(int(1.0 / CHUNK_SEC), 1)
    if (g_debug_memory && g_mem_tick_count % mem_log_interval == 0) {
        to_log(0, "strudel mem: heap {mem_heap_cur/1024ul}kb / str {mem_str_cur/1024ul}kb (max heap {g_mem_heap_max/1024ul}kb / str {g_mem_str_max/1024ul}kb)\n")
    }
}

def public strudel_tick_offline() {
    //! Offline tick: same as strudel_tick but without an audio driver.
    //! Renders into g_master_pcm and advances g_wall_time; call in a tight loop for offline WAV rendering.
    let t0 = ref_time_ticks()
    let CHUNK_SEC = g_chunk_seconds
    let chunkSamples = int(float(CHUNK_SEC) * float(SAMPLE_RATE)) * 2
    // prepare master output
    g_master_pcm |> resize(chunkSamples)
    for (s in g_master_pcm) {
        s = 0.0
    }
    // master mixer — linear pan (no cos/sin attenuation at center)
    var mixer : ma_volume_mixer
    ma_volume_mixer_init(unsafe(addr(mixer)), 2u)
    // tick all active tracks
    for (i in range(length(g_tracks))) {
        var track = g_tracks[i]
        if (track == null) {
            continue
        }
        // sync tempo
        if (track.sched.cps != g_cps) {
            track.sched.lastQueryEnd = g_wall_time * g_cps
            track.sched.cps = g_cps
        }
        tick(track.sched, track.pat, g_bank, g_wall_time, float(CHUNK_SEC), g_look_ahead)
        // mix scheduler output into master with track gain
        if (length(track.sched.output) > 0 && track.gain > 0.001) {
            // copy to per-track PCM for visualizer access
            if (i < length(g_track_pcm)) {
                g_track_pcm[i] := track.sched.output
            }
            ma_volume_mixer_set_volume(unsafe(addr(mixer)), track.gain)
            let nFrames = uint64(min(length(track.sched.output), chunkSamples) / 2)
            unsafe {
                ma_volume_mixer_process_pcm_frames(
                    addr(mixer),
                    addr(track.sched.output[0]),
                    addr(g_master_pcm[0]),
                    nFrames
                )
            }
        }
        // update fade envelope
        if (track.fade_speed > 0.0) {
            if (track.gain < track.target_gain) {
                track.gain = min(track.gain + track.fade_speed * float(CHUNK_SEC), track.target_gain)
            } elif (track.gain > track.target_gain) {
                track.gain = max(track.gain - track.fade_speed * float(CHUNK_SEC), track.target_gain)
            }
            if (abs(track.gain - track.target_gain) < 0.001) {
                track.gain = track.target_gain
                track.fade_speed = 0.0
            }
        }
        // remove faded-out tracks
        if (track.gain <= 0.001 && track.target_gain <= 0.001 && track.fade_speed == 0.0) {
            shutdown_scheduler(track.sched)
            unsafe { delete g_tracks[i]; }
            g_tracks[i] = null
        }
    }
    let chunkFrames = int(float(CHUNK_SEC) * float(SAMPLE_RATE))
    g_wall_samples += int64(chunkFrames)
    g_wall_time = double(g_wall_samples) / double(SAMPLE_RATE)
    ma_volume_mixer_uninit(unsafe(addr(mixer)))
    // tick timing
    let chunk_usec = double(get_time_usec(t0))
    g_tick_total_time += chunk_usec / 1000.lf
    g_tick_total_samples += uint64(chunkSamples / 2)
    if (chunk_usec > g_tick_max_chunk_usec) { g_tick_max_chunk_usec = chunk_usec; }
    // memory tracking
    let mem_heap_cur = heap_bytes_allocated()
    let mem_str_cur = string_heap_bytes_allocated()
    g_mem_heap_max = max(g_mem_heap_max, mem_heap_cur)
    g_mem_str_max = max(g_mem_str_max, mem_str_cur)
    g_mem_tick_count ++
    let mem_log_interval = max(int(1.0 / CHUNK_SEC), 1)
    if (g_debug_memory && g_mem_tick_count % mem_log_interval == 0) {
        to_log(0, "strudel mem: heap {mem_heap_cur/1024ul}kb / str {mem_str_cur/1024ul}kb (max heap {g_mem_heap_max/1024ul}kb / str {g_mem_str_max/1024ul}kb)\n")
    }
}

// --- Threaded mode ---

def public strudel_play(cmd_fn : function<(cmd : string) : void> = @@strudel_noop_cmd) {
    //! Blocking multi-track tick loop for threaded playback.
    //! Runs on the strudel thread (spawned by strudel_init); dispatches incoming user commands via cmd_fn until shutdown.
    let CHUNK_SEC = g_chunk_seconds
    let TARGET_CHUNKS = 4
    let LOW_WATERMARK = 2
    var total_time = 0.lf
    var total_samples = 0ul
    var max_chunk_usec = 0.0lf
    var status_box <- unsafe(lock_box_create())
    set_status_update(g_sid, status_box)
    var pcm_box <- unsafe(lock_box_create())
    var mixer : ma_volume_mixer
    ma_volume_mixer_init(unsafe(addr(mixer)), 2u)
    let chunkSamples = int(float(CHUNK_SEC) * float(SAMPLE_RATE)) * 2
    var output : array<float>
    output |> resize(chunkSamples)
    // memory tracking
    let mem_heap_base = heap_bytes_allocated()
    let mem_str_base = string_heap_bytes_allocated()
    var mem_heap_max = mem_heap_base
    var mem_str_max = mem_str_base
    let mem_log_interval = max(int(1.0 / CHUNK_SEC), 1)
    var mem_tick_count = 0
    while (strudel_process_commands(cmd_fn)) {
        if (!pcm_box.isReady) {
            sleep(1u)
            continue
        }
        let t0 = ref_time_ticks()
        for (v in output) { v = 0.0; }
        // tick all active tracks
        var i = length(g_tracks) - 1
        while (i >= 0) {
            var track = g_tracks[i]
            if (track == null) {
                i --
                continue
            }
            // sync tempo
            if (track.sched.cps != g_cps) {
                track.sched.lastQueryEnd = g_wall_time * g_cps
                track.sched.cps = g_cps
            }
            tick(track.sched, track.pat, *g_bank_ptr, g_wall_time, float(CHUNK_SEC), g_look_ahead)
            // mix into output with track gain
            if (length(track.sched.output) > 0 && track.gain > 0.001) {
                ma_volume_mixer_set_volume(unsafe(addr(mixer)), track.gain)
                let nFrames = uint64(min(length(track.sched.output), length(output)) / 2)
                unsafe {
                    ma_volume_mixer_process_pcm_frames(
                        addr(mixer),
                        addr(track.sched.output[0]),
                        addr(output[0]),
                        nFrames
                    )
                }
            }
            // update fade envelope
            if (track.fade_speed > 0.0) {
                if (track.gain < track.target_gain) {
                    track.gain = min(track.gain + track.fade_speed * float(CHUNK_SEC), track.target_gain)
                } elif (track.gain > track.target_gain) {
                    track.gain = max(track.gain - track.fade_speed * float(CHUNK_SEC), track.target_gain)
                }
                if (abs(track.gain - track.target_gain) < 0.001) {
                    track.gain = track.target_gain
                    track.fade_speed = 0.0
                }
            }
            // remove faded-out tracks
            if (track.gain <= 0.001 && track.target_gain <= 0.001 && track.fade_speed == 0.0) {
                shutdown_scheduler(track.sched)
                unsafe { delete g_tracks[i]; }
                g_tracks[i] = null
            }
            i --
        }
        if (length(output) > 0) {
            append_box_to_pcm(g_sid, pcm_box, output)
        }
        let chunk_usec = double(get_time_usec(t0))
        total_time += chunk_usec / 1000.lf
        total_samples += uint64(chunkSamples / 2)
        if (chunk_usec > max_chunk_usec) { max_chunk_usec = chunk_usec; }
        let chunkFrames = chunkSamples / 2
        g_wall_samples += int64(chunkFrames)
        g_wall_time = double(g_wall_samples) / double(SAMPLE_RATE)
        // update playback time for main thread
        if (g_playback_box != null) {
            g_playback_box |> set(PlaybackTime(wall_time = g_wall_time, cps = g_cps))
        }
        // buffer level feedback
        var que_len = 0
        status_box |> get() $(st : AudioChannelStatus#) {
            que_len = st.stream_que_length
        }
        if (que_len >= TARGET_CHUNKS) {
            sleep(50u)
        } elif (que_len >= LOW_WATERMARK) {
            sleep(10u)
        }
        // memory tracking
        let mem_heap_cur = heap_bytes_allocated()
        let mem_str_cur = string_heap_bytes_allocated()
        mem_heap_max = max(mem_heap_max, mem_heap_cur)
        mem_str_max = max(mem_str_max, mem_str_cur)
        mem_tick_count ++
        if (g_debug_memory && mem_tick_count % mem_log_interval == 0) {
            to_log(0, "strudel mem: heap {mem_heap_cur/1024ul}kb / str {mem_str_cur/1024ul}kb (max heap {mem_heap_max/1024ul}kb / str {mem_str_max/1024ul}kb)\n")
        }
    }
    if (total_samples > 0ul) {
        let SPEED = total_time / double(total_samples)
        let SPEED_OF_LIGHT = 1000.lf / 48000.lf
        let UTILIZATION = int(SPEED / SPEED_OF_LIGHT * 1000.lf)
        let chunk_budget_usec = double(CHUNK_SEC) * 1000000.lf
        let MAX_UTIL = int(max_chunk_usec / chunk_budget_usec * 1000.lf)
        let delta_heap = int64(mem_heap_max) - int64(mem_heap_base)
        let delta_str = int64(mem_str_max) - int64(mem_str_base)
        to_log(0, "strudel {UTILIZATION/10}.{UTILIZATION%10}% avg, {MAX_UTIL/10}.{MAX_UTIL%10}% peak util, heap {mem_heap_max/1024ul}kb (delta {delta_heap}), str {mem_str_max/1024ul}kb (delta {delta_str})\n")
        if (delta_heap > 0l || delta_str > 0l) {
            to_log(0, "strudel WARNING: potential memory leak detected (delta heap={delta_heap}, str={delta_str})\n")
        }
    }
    pcm_box |> join()
    unsafe(lock_box_remove(pcm_box))
    ma_volume_mixer_uninit(unsafe(addr(mixer)))
    unset_status_update(g_sid)
    clear_status(status_box)
    status_box |> join()
    unsafe(lock_box_remove(status_box))
}

def public strudel_init(fn : function<() : void>; cmd_fn : function<(cmd : string) : void> = @@strudel_noop_cmd) {
    //! Spawn the strudel playback thread and inject shared resources (sample bank, SF2, audio channel).
    //! fn runs on the new thread — it should add tracks and call strudel_play(). Call from main thread only.
    if (get_audio_command_channel() == null) {
        panic("strudel_init: audio system not initialized. Call audio_system_create() first.")
    }
    g_cmd_fn = cmd_fn
    // create playback time lockbox
    if (g_playback_box == null) {
        g_playback_box = unsafe(lock_box_create())
    }
    // create channels
    g_cmd_channel = unsafe(channel_create())
    g_done_channel = unsafe(channel_create())
    // create PCM stream on main thread (audio globals are initialized here)
    let thread_sid = play_sound_from_pcm_stream(MA_SAMPLE_RATE, MA_CHANNELS)
    // capture shared resources from main thread
    var bank_ptr = unsafe(addr(g_bank))
    var sf2_ptr = g_sf2 != null ? unsafe(reinterpret<SF2File const?>(g_sf2)) : null
    var audio_ch = get_audio_command_channel()
    var cmd_ch = g_cmd_channel
    var done_ch = g_done_channel
    var pb_box = g_playback_box
    let wt = g_wall_time
    let cps = g_cps
    let la = g_look_ahead
    new_thread() <| @capture(= fn, = bank_ptr, = sf2_ptr, = audio_ch, = cmd_ch, = done_ch, = pb_box, = wt, = cps, = la, = thread_sid) {
        // inject shared resources into thread context
        g_bank_ptr = bank_ptr
        unsafe {
            set_audio_thread_command_channel(audio_ch)
        }
        g_sf2 = unsafe(reinterpret<SF2File?>(sf2_ptr))
        g_sid = thread_sid
        g_cmd_channel = cmd_ch
        g_done_channel = done_ch
        g_playback_box = pb_box
        g_wall_time = wt
        g_cps = cps
        g_look_ahead = la
        // run user setup + play loop
        invoke(fn)
        // cleanup
        unsafe {
            set_audio_thread_command_channel(null)
        }
        cmd_ch |> release()
        pb_box |> release()
        done_ch |> push(new StrudelCommand(shutdown = true))
        done_ch |> release()
    }
}

def public strudel_shutdown() {
    //! Shut down playback: signal the strudel thread to stop, release channels/lockboxes, free all tracks.
    //! Safe to call from the main thread; also finalizes memory/utilization stats.
    if (g_cmd_channel != null) {
        g_cmd_channel |> push(new StrudelCommand(shutdown = true))
        var got_done = false
        while (!got_done) {
            _builtin_channel_pop(g_done_channel) $(data) {
                if (data != null) {
                    got_done = true
                }
            }
        }
        // command channel
        unsafe(channel_remove(g_cmd_channel))
        g_cmd_channel = null
        // done channel
        unsafe(channel_remove(g_done_channel))
        g_done_channel = null
        // player box
        unsafe(lock_box_remove(g_playback_box))
        g_playback_box = null
    }
    // pcm box (main-thread mode)
    if (g_tick_pcm_box != null) {
        g_tick_pcm_box |> join()
        unsafe(lock_box_remove(g_tick_pcm_box))
        g_tick_pcm_box = null
    }
    // status box (main-thread mode: created in strudel_create_channel)
    if (g_tick_status_box != null) {
        unset_status_update(g_sid)
        clear_status(g_tick_status_box)
        var tick_ptr = g_tick_status_box
        g_tick_status_box |> release()
        tick_ptr |> join()
        unsafe(lock_box_remove(tick_ptr))
        g_tick_status_box = null
    }
    // release tracks and buffers before measuring memory
    for (i in range(length(g_tracks))) {
        if (g_tracks[i] != null) {
            shutdown_scheduler(g_tracks[i].sched)
            unsafe { delete g_tracks[i]; }
            g_tracks[i] = null
        }
    }
    delete g_tracks
    delete g_track_pcm
    delete g_master_pcm
    g_sid = 0ul
    // utilization stats
    if (g_tick_total_samples > 0ul) {
        let SPEED = g_tick_total_time / double(g_tick_total_samples)
        let SPEED_OF_LIGHT = 1000.lf / 48000.lf
        let UTILIZATION = int(SPEED / SPEED_OF_LIGHT * 1000.lf)
        let chunk_budget_usec = double(g_chunk_seconds) * 1000000.lf
        let MAX_UTIL = int(g_tick_max_chunk_usec / chunk_budget_usec * 1000.lf)
        to_log(0, "strudel {UTILIZATION/10}.{UTILIZATION%10}% avg, {MAX_UTIL/10}.{MAX_UTIL%10}% peak utilization\n")
    }
    g_tick_total_time = 0.lf
    g_tick_total_samples = 0ul
    g_tick_max_chunk_usec = 0.lf
    // memory stats (main-thread mode)
    if (g_mem_heap_baseline > 0ul) {
        let mem_heap_now = heap_bytes_allocated()
        let mem_str_now = string_heap_bytes_allocated()
        let leak_heap = int64(mem_heap_now) - int64(g_mem_heap_baseline)
        let leak_str = int64(mem_str_now) - int64(g_mem_str_baseline)
        to_log(0, "strudel mem: heap {int(mem_heap_now)/1024}kb (peak {int(g_mem_heap_max)/1024}kb, leak {leak_heap/1024l}kb), str {int(mem_str_now)/1024}kb (leak {leak_str/1024l}kb)\n")
        if (leak_heap > 65536l || leak_str > 0l) {
            to_log(0, "strudel WARNING: potential memory leak detected (heap={leak_heap}, str={leak_str})\n")
        }
        g_mem_heap_baseline = 0ul
    }
}

// --- Persistence ---

def private serialize_bank(var arch : Archive; var bank : SampleBank) {
    var num_sounds = length(bank.sounds)
    arch |> serialize(num_sounds)
    for (name, variations in keys(bank.sounds), values(bank.sounds)) {
        var sname = name
        arch |> serialize(sname)
        var nv = length(variations)
        arch |> serialize(nv)
        for (s in variations) {
            var ch = s.channels
            arch |> serialize(ch)
            var nd = length(s.data)
            arch |> serialize(nd)
            for (d in s.data) {
                var v = d
                arch |> serialize(v)
            }
        }
    }
}

def private deserialize_bank(var arch : Archive; var bank : SampleBank) {
    var num_sounds : int
    arch |> serialize(num_sounds)
    for (_ in range(num_sounds)) {
        var name : string
        arch |> serialize(name)
        var nv : int
        arch |> serialize(nv)
        var variations : array<Sample>
        variations |> reserve(nv)
        for (_v in range(nv)) {
            var sample : Sample
            arch |> serialize(sample.channels)
            var nd : int
            arch |> serialize(nd)
            sample.data |> resize(nd)
            for (d in sample.data) {
                arch |> serialize(d)
            }
            variations |> emplace(sample)
        }
        bank.sounds[name] <- variations
    }
}

// --- SF2 Persistence ---

def private serialize_zone(var arch : Archive; var zone : SF2Zone?&) {
    var has = zone != null
    arch |> serialize(has)
    if (has) {
        if (arch.reading) {
            zone = new SF2Zone()
        }
        arch |> serialize(zone.key_lo)
        arch |> serialize(zone.key_hi)
        arch |> serialize(zone.vel_lo)
        arch |> serialize(zone.vel_hi)
        arch |> serialize(zone.generators)
    }
}

def private serialize_sf2(var arch : Archive; var sf2 : SF2File?&) {
    var has = sf2 != null
    arch |> serialize(has)
    if (!has) {
        return
    }
    if (arch.reading) {
        sf2 = new SF2File()
    }
    arch |> serialize(sf2.name)
    arch |> serialize(sf2.sample_data)
    // samples
    var ns = length(sf2.samples)
    arch |> serialize(ns)
    if (arch.reading) {
        sf2.samples |> resize(ns)
    }
    for (sh in sf2.samples) {
        arch |> serialize(sh.name)
        arch |> serialize(sh.start)
        arch |> serialize(sh.end)
        arch |> serialize(sh.loop_start)
        arch |> serialize(sh.loop_end)
        arch |> serialize(sh.sample_rate)
        arch |> serialize(sh.original_pitch)
        arch |> serialize(sh.pitch_correction)
        arch |> serialize(sh.sample_link)
        arch |> serialize(sh.sample_type)
    }
    // instruments
    var ni = length(sf2.instruments)
    arch |> serialize(ni)
    if (arch.reading) {
        sf2.instruments |> resize(ni)
    }
    for (inst in sf2.instruments) {
        var has_inst = inst != null
        arch |> serialize(has_inst)
        if (has_inst) {
            if (arch.reading) {
                inst = new SF2InstrumentDef()
            }
            arch |> serialize(inst.name)
            serialize_zone(arch, inst.global_zone)
            var nz = length(inst.zones)
            arch |> serialize(nz)
            if (arch.reading) {
                inst.zones |> resize(nz)
            }
            for (z in inst.zones) {
                serialize_zone(arch, z)
            }
        }
    }
    // presets
    var np = length(sf2.presets)
    arch |> serialize(np)
    if (arch.reading) {
        sf2.presets |> resize(np)
    }
    for (pr in sf2.presets) {
        var has_pr = pr != null
        arch |> serialize(has_pr)
        if (has_pr) {
            if (arch.reading) {
                pr = new SF2PresetDef()
            }
            arch |> serialize(pr.name)
            arch |> serialize(pr.preset)
            arch |> serialize(pr.bank)
            serialize_zone(arch, pr.global_zone)
            var nz = length(pr.zones)
            arch |> serialize(nz)
            if (arch.reading) {
                pr.zones |> resize(nz)
            }
            for (z in pr.zones) {
                serialize_zone(arch, z)
            }
        }
    }
    // preset_lookup
    arch |> serialize(sf2.preset_lookup)
}

def public strudel_save_sf2_state() : array<uint8> {
    //! Serialize the currently loaded SoundFont to a byte array (empty if none loaded).
    if (g_sf2 == null) {
        var empty : array<uint8>
        return <- empty
    }
    var ser = new MemSerializer()
    var arch = Archive(reading = false, stream = ser)
    serialize_sf2(arch, g_sf2)
    var data <- ser->extractData()
    unsafe { delete ser; }
    return <- data
}

def public strudel_restore_sf2_state(data : array<uint8>) {
    //! Restore a previously serialized SoundFont from a byte array (no-op on empty data).
    if (length(data) == 0) {
        return
    }
    var ser = new MemSerializer(data)
    var arch = Archive(reading = true, stream = ser)
    serialize_sf2(arch, g_sf2)
    unsafe { delete ser; }
}

def public strudel_save_state() : array<uint8> {
    //! Serialize playback state (wall time, CPS, sample bank) into a byte array; shuts down playback first.
    if (g_sid == 0ul) {
        var empty : array<uint8>
        return <- empty
    }
    strudel_shutdown()
    var ser = new MemSerializer()
    var arch = Archive(reading = false, stream = ser)
    arch |> serialize(g_wall_time)
    arch |> serialize(g_cps)
    arch |> serialize(g_sid)
    serialize_bank(arch, g_bank)
    var data <- ser->extractData()
    unsafe { delete ser; }
    return <- data
}

def public strudel_restore_state(data : array<uint8>) {
    //! Restore previously saved playback state (wall time, CPS, sample bank) from a byte array.
    if (length(data) == 0) {
        return
    }
    var ser = new MemSerializer(data)
    var arch = Archive(reading = true, stream = ser)
    arch |> serialize(g_wall_time)
    arch |> serialize(g_cps)
    arch |> serialize(g_sid)
    deserialize_bank(arch, g_bank)
    unsafe { delete ser; }
}
