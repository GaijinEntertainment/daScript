options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module strudel_player shared public

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

variant StrudelCommand {
    shutdown : bool;
    set_bpm  : double;
    set_cps  : double;
    user_cmd : string;
}

// --- Track ---

[safe_when_uninitialized]
struct StrudelTrack {
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
var public g_wall_time : double = 0.0lf
var private g_wall_samples : int64 = 0l  // exact sample count — g_wall_time derived from this
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

// Per-track PCM output (populated by strudel_tick, main-thread mode only)
var public g_track_pcm : array<array<float>>
var public g_master_pcm : array<float>

// Rolling vis index — bumped by strudel_add_track, read by visualizer combinators
var public g_vis_index : int = 0

struct AudioChunk {
    samples : array<float>
}

struct PlaybackTime {
    wall_time : double
    cps       : double
}

// --- Playback time ---

// Get current playback position and tempo.
// In threaded mode reads from lockbox.
// In main-thread mode uses hardware playback position (smooth, matches what you hear).
// The playback position lags behind g_wall_time by the audio buffer depth — that's correct:
// the pianoroll shows upcoming notes, and the playhead shows where playback actually is.
def public strudel_get_playback_time() : tuple<double; double> {
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
    let sname = to_lower(name)
    if (key_exists(g_bank.sounds, sname)) {
        return
    }
    load_sound(path, name, g_bank)
}

def public strudel_load_sample_dir(root : string) {
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
    g_sf2 = sf2_load(path)
    if (g_sf2 != null) {
        g_sf2_path = path
    }
    return g_sf2 != null
}

def public strudel_get_sf2_path() : string {
    return g_sf2_path
}

def public strudel_has_sf2() : bool {
    return g_sf2 != null
}

// --- Volume / Pause ---

def public strudel_set_volume(volume : float; time : float = 0.0) {
    if (g_sid != 0ul) {
        set_volume(g_sid, volume, time)
    }
}

def public strudel_set_pause(paused : bool) {
    if (g_sid != 0ul) {
        set_pause(g_sid, paused)
    }
}

// --- Tempo (affects both threads) ---

def public strudel_set_bpm(bpm : double) {
    g_cps = bpm / 240.0lf
    if (g_cmd_channel != null) {
        g_cmd_channel |> push(new StrudelCommand(set_bpm = bpm))
    }
}

def public strudel_set_cps(cps : double) {
    g_cps = cps
    if (g_cmd_channel != null) {
        g_cmd_channel |> push(new StrudelCommand(set_cps = cps))
    }
}

def public strudel_set_look_ahead(look_ahead : float) {
    g_look_ahead = look_ahead
}

// --- Track management ---

// Add a track. Returns its index (matches g_vis_index for visualizer combinators).
def public strudel_add_track(var pat : Pattern; gain : float = 1.0) : int {
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
    if (g_sf2 != null) {
        init_reverb(track.sched)
        init_chorus(track.sched)
    }
    // ensure per-track PCM arrays are big enough
    while (length(g_track_pcm) <= idx) {
        var empty : array<float>
        g_track_pcm |> emplace(empty)
    }
    g_vis_index ++
    return idx
}

// Remove a track by index (sets slot to null).
def public strudel_remove_track(idx : int) {
    if (idx >= 0 && idx < length(g_tracks) && g_tracks[idx] != null) {
        unsafe { delete g_tracks[idx]; }
        g_tracks[idx] = null
    }
}

// Fade a track by index.
def public strudel_fade_track(idx : int; target_gain : float; time : float) {
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

// Create a PCM stream for main-thread playback. Call after audio_system_create().
def public strudel_create_channel() {
    if (g_sid == 0ul) {
        g_sid = play_sound_from_pcm_stream(MA_SAMPLE_RATE, MA_CHANNELS)
        g_tick_status_box = unsafe(lock_box_create())
        g_tick_status_box |> add_ref()
        set_status_update(g_sid, g_tick_status_box)
    }
}

// Single tick: queries all tracks, renders audio, mixes, appends to PCM stream.
// Call from your render loop. Self-regulating: skips when audio buffer has enough data.
def public strudel_tick() {
    if (g_sid == 0ul) {
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
    let CHUNK_SEC = g_chunk_seconds
    let chunkSamples = int(float(CHUNK_SEC) * float(SAMPLE_RATE)) * 2
    // prepare master output
    g_master_pcm |> resize(chunkSamples)
    for (s in g_master_pcm) {
        s = 0.0
    }
    // master mixer (static-like, init once)
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
        var chunk <- tick(track.sched, track.pat, g_bank, g_wall_time, float(CHUNK_SEC), g_look_ahead)
        // store per-track PCM
        if (i < length(g_track_pcm)) {
            g_track_pcm[i] <- chunk
        } else {
            delete chunk
            continue
        }
        // mix into master with track gain
        if (length(g_track_pcm[i]) > 0 && track.gain > 0.001) {
            ma_volume_mixer_set_volume(unsafe(addr(mixer)), track.gain)
            let nFrames = uint64(min(length(g_track_pcm[i]), chunkSamples) / 2)
            unsafe {
                ma_volume_mixer_process_pcm_frames(
                    addr(mixer),
                    addr(g_track_pcm[i][0]),
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
            unsafe { delete g_tracks[i]; }
            g_tracks[i] = null
        }
    }
    // submit audio
    if (length(g_master_pcm) > 0) {
        append_to_pcm(g_sid, g_master_pcm)
    }
    let chunkFrames = int(float(CHUNK_SEC) * float(SAMPLE_RATE))
    g_wall_samples += int64(chunkFrames)
    g_wall_time = double(g_wall_samples) / double(SAMPLE_RATE)
    ma_volume_mixer_uninit(unsafe(addr(mixer)))
}

// --- Threaded mode ---

// Blocking multi-track tick loop. Call from your strudel_main on the thread.
def public strudel_play(cmd_fn : function<(cmd : string) : void> = @@strudel_noop_cmd) {
    let CHUNK_SEC = g_chunk_seconds
    let TARGET_CHUNKS = 4
    let LOW_WATERMARK = 2
    var total_time = 0.lf
    var total_samples = 0ul
    var status_box <- unsafe(lock_box_create())
    set_status_update(g_sid, status_box)
    var mixer : ma_volume_mixer
    ma_volume_mixer_init(unsafe(addr(mixer)), 2u)
    while (strudel_process_commands(cmd_fn)) {
        let t0 = ref_time_ticks()
        let chunkSamples = int(float(CHUNK_SEC) * float(SAMPLE_RATE)) * 2
        var output : array<float>
        output |> resize(chunkSamples)
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
            var chunk <- tick(track.sched, track.pat, *g_bank_ptr, g_wall_time, float(CHUNK_SEC), g_look_ahead)
            // mix into output with track gain
            if (length(chunk) > 0 && track.gain > 0.001) {
                ma_volume_mixer_set_volume(unsafe(addr(mixer)), track.gain)
                let nFrames = uint64(min(length(chunk), length(output)) / 2)
                unsafe {
                    ma_volume_mixer_process_pcm_frames(
                        addr(mixer),
                        addr(chunk[0]),
                        addr(output[0]),
                        nFrames
                    )
                }
            }
            delete chunk
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
                unsafe { delete g_tracks[i]; }
                g_tracks[i] = null
            }
            i --
        }
        if (length(output) > 0) {
            append_to_pcm(g_sid, output)
        }
        total_time += double(get_time_usec(t0)) / 1000.lf
        total_samples += uint64(chunkSamples / 2)
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
    }
    if (total_samples > 0ul) {
        let SPEED = total_time / double(total_samples)
        let SPEED_OF_LIGHT = 1000.lf / 48000.lf
        let UTILIZATION = int(SPEED / SPEED_OF_LIGHT * 1000.lf)
        to_log(0, "strudel {UTILIZATION/10}.{UTILIZATION%10}% utilization\n")
    }
    ma_volume_mixer_uninit(unsafe(addr(mixer)))
    unset_status_update(g_sid)
    clear_status(status_box)
    status_box |> release()
}

// Spawn strudel thread. Captures bank/sf2 pointers from main thread.
// fn runs on the thread — add tracks and call strudel_play() there.
def public strudel_init(fn : function<() : void>; cmd_fn : function<(cmd : string) : void> = @@strudel_noop_cmd) {
    if (get_audio_command_channel() == null) {
        panic("strudel_init: audio system not initialized. Call audio_system_create() first.")
    }
    g_cmd_fn = cmd_fn
    // create playback time lockbox
    if (g_playback_box == null) {
        g_playback_box = unsafe(lock_box_create())
        g_playback_box |> add_ref()
    }
    // create channels
    g_cmd_channel = unsafe(channel_create())
    g_cmd_channel |> add_ref()
    g_done_channel = unsafe(channel_create())
    g_done_channel |> add_ref()
    // create PCM stream on main thread (audio globals are initialized here)
    let thread_sid = play_sound_from_pcm_stream(MA_SAMPLE_RATE, MA_CHANNELS)
    // capture shared resources from main thread
    var bank_ptr = unsafe(addr(g_bank))
    var sf2_ptr = g_sf2 != null ? unsafe(reinterpret<SF2File const?>(g_sf2)) : null
    var audio_ch = get_audio_command_channel()
    var cmd_ch = g_cmd_channel
    var done_ch = g_done_channel
    var pb_box = g_playback_box
    pb_box |> add_ref()
    cmd_ch |> add_ref()
    done_ch |> add_ref()
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
        done_ch |> push(new StrudelCommand(shutdown = true))
        done_ch |> release()
    }
}

def public strudel_shutdown() {
    if (g_cmd_channel == null) {
        return
    }
    g_cmd_channel |> push(new StrudelCommand(shutdown = true))
    var got_done = false
    while (!got_done) {
        _builtin_channel_pop(g_done_channel) $(data) {
            if (data != null) {
                got_done = true
            }
        }
    }
    g_cmd_channel |> release()
    g_cmd_channel = null
    g_done_channel |> release()
    g_done_channel = null
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
    if (length(data) == 0) {
        return
    }
    var ser = new MemSerializer(data)
    var arch = Archive(reading = true, stream = ser)
    serialize_sf2(arch, g_sf2)
    unsafe { delete ser; }
}

def public strudel_save_state() : array<uint8> {
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
