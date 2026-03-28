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
    name        : string
    pat         : Pattern
    @safe_when_uninitialized sched : Scheduler // per-track scheduler (tracks lastQueryEnd independently)
    gain        : float = 1.0
    target_gain : float = 1.0
    fade_speed  : float = 0.0     // gain change per second (0 = no fade)
    audio_vis_ptr : void?          // per-track PCM channel as void* (lifetime owned elsewhere)
}

// --- Shared state (passed to thread function) ---

[safe_when_uninitialized]
struct StrudelState {
    bank          : SampleBank?   // pointer to g_bank (immutable after load)
    sf2           : SF2File const? // pointer to g_sf2 (immutable after load)
    sid           : uint64        // PCM stream handle
    wall_time     : double        // playback position (thread updates this)
    cps           : double        // tempo
    cmd_channel   : Channel?      // command pipe (main -> thread)
    done_channel  : Channel?      // thread -> main (done signal)
    audio_channel : void?         // audio command channel (for append_to_pcm on thread)
    tracks        : array<StrudelTrack?> // active tracks
    playback_box  : LockBox?      // thread → main: current PlaybackTime
    vis_channels  : array<Channel?> // hap channels (main creates, thread pushes)
    audio_vis_channel : Channel?    // optional: master PCM chunks for spectrum analyzer
    track_audio_channels : array<Channel?> // per-track PCM channels
    vis_names     : array<string>      // parallel to vis_channels: name at index i
    vis_look_ahead : float = 0.1      // how far ahead scheduler queries (seconds)
}

// --- Module globals ---

var private g_bank : SampleBank
var private g_sid : uint64 = 0ul
var private g_wall_time : double = 0.0lf
var private g_cps : double = 0.5lf
var private g_cmd_channel : Channel?
var private g_done_channel : Channel?
var private g_sf2 : SF2File?
var private g_sf2_path : string   // path to loaded SF2 (for live-reload re-read)
var private g_state : StrudelState
var private g_cmd_fn : function<(var state : StrudelState?; cmd : string) : void>
var private g_playback_box : LockBox?

struct AudioChunk {
    samples : array<float>
}

struct PlaybackTime {
    wall_time : double
    cps       : double
}

// Get current playback position and tempo from the audio thread.
// Returns (wall_time, cps). Safe to call from the main/GL thread.
def public strudel_get_playback_time() : tuple<double; double> {
    var wt = 0.0lf
    var cp = 0.5lf
    if (g_playback_box != null) {
        g_playback_box |> get() $(pt : PlaybackTime#) {
            wt = pt.wall_time
            cp = pt.cps
        }
    } else {
        wt = g_wall_time
        cp = g_cps
    }
    return wt => cp
}

// --- Public API ---

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

// Load an SF2 soundfont for use with set_sf2() patterns.
def public strudel_load_sf2(path : string) : bool {
    g_sf2 = sf2_load(path)
    if (g_sf2 != null) {
        g_sf2_path = path
    }
    return g_sf2 != null
}

// Get the path of the currently loaded SF2 (empty if none).
def public strudel_get_sf2_path() : string {
    return g_sf2_path
}

// Check if an SF2 soundfont is loaded (e.g. restored from live store).
def public strudel_has_sf2() : bool {
    return g_sf2 != null
}

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

// Set tempo — works from main thread (before or after init) and from strudel thread.
// From main thread: sets g_cps and sends command to thread.
// From strudel thread: pass the state pointer to set tempo directly.
def public strudel_set_bpm(bpm : double) {
    g_cps = bpm / 240.0lf
    if (g_cmd_channel != null) {
        g_cmd_channel |> push(new StrudelCommand(set_bpm = bpm))
    }
}

def public strudel_set_bpm(var state : StrudelState?; bpm : double) {
    state.cps = bpm / 240.0lf
}

def public strudel_set_cps(cps : double) {
    g_cps = cps
    if (g_cmd_channel != null) {
        g_cmd_channel |> push(new StrudelCommand(set_cps = cps))
    }
}

def public strudel_set_cps(var state : StrudelState?; cps : double) {
    state.cps = cps
}

def public strudel_add_vis_channel(var ch : Channel?) {
    ch |> add_ref()
    g_state.vis_channels |> push(ch)
}

def public strudel_set_audio_vis_channel(var ch : Channel?) {
    ch |> add_ref()
    g_state.audio_vis_channel = ch
    g_state.audio_vis_channel |> add_ref()
}

def public strudel_add_track_audio_channel(var ch : Channel?) {
    ch |> add_ref()
    g_state.track_audio_channels |> push(ch)
}

// Register a named visualization: hap channel + audio channel + registry entry.
// Called by create_track_vis on the main thread.
def public strudel_register_vis(name : string; vis_index : int; var hap_ch, audio_ch : Channel?) {
    hap_ch |> add_ref()
    g_state.vis_channels |> push(hap_ch)
    audio_ch |> add_ref()
    g_state.track_audio_channels |> push(audio_ch)
    g_state.vis_names |> push(name)
}

def public strudel_set_look_ahead(look_ahead : float) {
    g_state.vis_look_ahead = look_ahead
}

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
    g_wall_time = g_state.wall_time
    g_cps = g_state.cps
    g_cmd_channel |> release()
    g_cmd_channel = null
    g_done_channel |> release()
    g_done_channel = null
}

def public strudel_noop_cmd(var state : StrudelState?; cmd : string) {
    pass
}

def public strudel_init(fn : function<(var state : StrudelState?) : void>; cmd_fn : function<(var state : StrudelState?; cmd : string) : void> = @@strudel_noop_cmd) {
    if (get_audio_command_channel() == null) {
        panic("strudel_init: audio system not initialized. Call audio_system_create() first.")
    }
    if (g_sid == 0ul) {
        g_sid = play_sound_from_pcm_stream(MA_SAMPLE_RATE, MA_CHANNELS)
    }
    g_cmd_fn = cmd_fn
    // create playback time lockbox (shared via StrudelState pointer)
    if (g_playback_box == null) {
        g_playback_box = unsafe(lock_box_create())
        g_playback_box |> add_ref()
    }
    // create channels
    g_cmd_channel = unsafe(channel_create())
    g_cmd_channel |> add_ref()
    g_done_channel = unsafe(channel_create())
    g_done_channel |> add_ref()
    // set up shared state
    g_state.bank = unsafe(addr(g_bank))
    g_state.sf2 = g_sf2 != null ? unsafe(reinterpret<SF2File const?>(g_sf2)) : null
    g_state.sid = g_sid
    g_state.wall_time = g_wall_time
    g_state.cps = g_cps
    g_state.cmd_channel = g_cmd_channel
    g_state.cmd_channel |> add_ref()
    g_state.done_channel = g_done_channel
    g_state.done_channel |> add_ref()
    g_state.audio_channel = get_audio_command_channel()
    g_state.playback_box = g_playback_box
    g_state.playback_box |> add_ref()
    // spawn thread
    var state = unsafe(addr(g_state))
    var cmd_ch = g_cmd_channel
    var done_ch = g_done_channel
    new_thread() <| @capture(= fn, = cmd_fn, = state, = cmd_ch, = done_ch) {
        unsafe {
            set_audio_thread_command_channel(state.audio_channel)
        }
        invoke(fn, state)
        unsafe {
            set_audio_thread_command_channel(null)
        }
        cmd_ch |> release()
        done_ch |> push(new StrudelCommand(shutdown = true))
        done_ch |> release()
    }
}

// --- Track management (called on strudel thread) ---

def public strudel_add_track(var state : StrudelState?; name : string; var pat : Pattern; gain : float = 1.0) {
    // remove existing track with same name
    strudel_remove_track(state, name)
    // auto-attach audio vis channel if name is in vis_registry
    var audio_ptr : void?
    for (vi in range(length(state.vis_names))) {
        if (state.vis_names[vi] == name && vi < length(state.track_audio_channels)) {
            audio_ptr = unsafe(reinterpret<void?>(state.track_audio_channels[vi]))
            break
        }
    }
    state.tracks |> push(new StrudelTrack(
        name = name,
        pat <- pat,
        gain = gain,
        target_gain = gain,
        audio_vis_ptr = audio_ptr
    ))
    var track = state.tracks[length(state.tracks) - 1]
    track.sched.cps = state.cps
    track.sched.lastQueryEnd = state.wall_time * state.cps
    track.sched.sf2 = state.sf2
    if (state.sf2 != null) {
        init_reverb(track.sched)
        init_chorus(track.sched)
    }
}

def public strudel_add_track_with_vis(var state : StrudelState?; name : string; var pat : Pattern; var audio_ch : Channel?; gain : float = 1.0) {
    strudel_remove_track(state, name)
    state.tracks |> push(new StrudelTrack(
        name = name,
        pat <- pat,
        gain = gain,
        target_gain = gain,
        audio_vis_ptr = unsafe(reinterpret<void?>(audio_ch))
    ))
    var track = state.tracks[length(state.tracks) - 1]
    track.sched.cps = state.cps
    track.sched.lastQueryEnd = state.wall_time * state.cps
    track.sched.sf2 = state.sf2
    if (state.sf2 != null) {
        init_reverb(track.sched)
        init_chorus(track.sched)
    }
}

def public strudel_remove_track(var state : StrudelState?; name : string) {
    var i = length(state.tracks) - 1
    while (i >= 0) {
        if (state.tracks[i].name == name) {
            unsafe { delete state.tracks[i]; }
            state.tracks |> erase(i)
        }
        i --
    }
}

def public strudel_fade_track(var state : StrudelState?; name : string; target_gain : float; time : float) {
    for (track in state.tracks) {
        if (track.name == name) {
            track.target_gain = target_gain
            if (time > 0.0) {
                track.fade_speed = abs(track.target_gain - track.gain) / time
            } else {
                track.gain = target_gain
                track.fade_speed = 0.0
            }
        }
    }
}

// --- Command processing ---

def private strudel_process_commands_ex(var state : StrudelState?; cmd_fn : function<(var state : StrudelState?; cmd : string) : void>) : bool {
    var running = true
    state.cmd_channel |> gather() $(cmd : StrudelCommand#) {
        if (cmd is shutdown) {
            running = false
        } elif (cmd is set_bpm) {
            state.cps = (cmd as set_bpm) / 240.0lf
        } elif (cmd is set_cps) {
            state.cps = cmd as set_cps
        } elif (cmd is user_cmd) {
            let ucmd = string(cmd as user_cmd)
            invoke(cmd_fn, state, ucmd)
        }
    }
    return running
}

// Keep old API working
def public strudel_process_commands(var state : StrudelState?) : bool {
    return strudel_process_commands_ex(state, @@strudel_noop_cmd)
}

// --- Tick loops ---

// Single-pattern tick loop (backwards compatible)
def public strudel_play_pattern(var state : StrudelState?; var pat : Pattern) {
    let CHUNK_SEC = 0.05lf
    let TARGET_CHUNKS = 4
    let LOW_WATERMARK = 2
    var total_time = 0.lf
    var total_samples = 0ul
    var sched = Scheduler()
    sched.cps = state.cps
    sched.lastQueryEnd = state.wall_time * state.cps
    sched.sf2 = state.sf2
    if (state.sf2 != null) {
        init_reverb(sched)
        init_chorus(sched)
    }
    var status_box <- unsafe(lock_box_create())
    set_status_update(state.sid, status_box)
    while (strudel_process_commands(state)) {
        if (sched.cps != state.cps) {
            sched.lastQueryEnd = state.wall_time * state.cps
            sched.cps = state.cps
        }
        let t0 = ref_time_ticks()
        var chunk <- tick(sched, pat, *state.bank, state.wall_time, float(CHUNK_SEC), state.vis_look_ahead)
        if (length(chunk) > 0) {
            total_time += double(get_time_usec(t0)) / 1000.lf
            total_samples += uint64(length(chunk) / 2)
            // send PCM to spectrum visualizer
            if (state.audio_vis_channel != null) {
                state.audio_vis_channel |> push_clone(AudioChunk(samples := chunk))
            }
            append_to_pcm(state.sid, chunk)
        }
        state.wall_time += CHUNK_SEC
        // update playback time for visualization
        if (state.playback_box != null) {
            state.playback_box |> set(PlaybackTime(wall_time = state.wall_time, cps = state.cps))
        }
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
    unset_status_update(state.sid)
    clear_status(status_box)
    status_box |> release()
}

// Multi-track tick loop with command callback
def public strudel_play(var state : StrudelState?; cmd_fn : function<(var state : StrudelState?; cmd : string) : void> = @@strudel_noop_cmd) {
    let CHUNK_SEC = 0.05lf
    let TARGET_CHUNKS = 4
    let LOW_WATERMARK = 2
    var total_time = 0.lf
    var total_samples = 0ul
    var status_box <- unsafe(lock_box_create())
    set_status_update(state.sid, status_box)
    var mixer : ma_volume_mixer
    ma_volume_mixer_init(unsafe(addr(mixer)), 2u)
    while (strudel_process_commands_ex(state, cmd_fn)) {
        let t0 = ref_time_ticks()
        let chunkSamples = int(float(CHUNK_SEC) * float(SAMPLE_RATE)) * 2
        var output : array<float>
        output |> resize(chunkSamples)
        // tick all active tracks
        var i = length(state.tracks) - 1
        while (i >= 0) {
            var track = state.tracks[i]
            // sync tempo
            if (track.sched.cps != state.cps) {
                track.sched.lastQueryEnd = state.wall_time * state.cps
                track.sched.cps = state.cps
            }
            var chunk <- tick(track.sched, track.pat, *state.bank, state.wall_time, float(CHUNK_SEC), state.vis_look_ahead)
            // per-track PCM for visualization (pre-mix)
            if (track.audio_vis_ptr != null && length(chunk) > 0) {
                var vis_ch = unsafe(reinterpret<Channel?>(track.audio_vis_ptr))
                vis_ch |> push_clone(AudioChunk(samples := chunk))
            }
            // mix into output with track gain via ma_volume_mixer
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
                unsafe { delete state.tracks[i]; }
                state.tracks |> erase(i)
            }
            i --
        }
        if (length(output) > 0) {
            // send PCM to spectrum visualizer
            if (state.audio_vis_channel != null) {
                state.audio_vis_channel |> push_clone(AudioChunk(samples := output))
            }
            append_to_pcm(state.sid, output)
        }
        total_time += double(get_time_usec(t0)) / 1000.lf
        total_samples += uint64(chunkSamples / 2)
        state.wall_time += CHUNK_SEC
        // update playback time for visualization
        if (state.playback_box != null) {
            state.playback_box |> set(PlaybackTime(wall_time = state.wall_time, cps = state.cps))
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
    unset_status_update(state.sid)
    clear_status(status_box)
    status_box |> release()
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

// Serialize player state to bytes (for live-reload persistence).
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

// Restore player state from bytes.
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
