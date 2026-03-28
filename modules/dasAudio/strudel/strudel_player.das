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
}

// --- Shared state (passed to thread function) ---

struct StrudelState {
    bank          : SampleBank?   // pointer to g_bank (immutable after load)
    sid           : uint64        // PCM stream handle
    wall_time     : double        // playback position (thread updates this)
    cps           : double        // tempo
    cmd_channel   : Channel?      // command pipe (main -> thread)
    done_channel  : Channel?      // thread -> main (done signal)
    audio_channel : void?         // audio command channel (for append_to_pcm on thread)
    tracks        : array<StrudelTrack?> // active tracks
}

// --- Module globals ---

var private g_bank : SampleBank
var private g_sid : uint64 = 0ul
var private g_wall_time : double = 0.0lf
var private g_cps : double = 0.5lf
var private g_cmd_channel : Channel?
var private g_done_channel : Channel?
var private g_state : StrudelState
var private g_cmd_fn : function<(var state : StrudelState?; cmd : string) : void>

// --- Public API ---

def public strudel_load_sound(path, name : string) {
    let sname = to_lower(name)
    if (key_exists(g_bank.sounds, sname)) {
        return
    }
    load_sound(path, name, g_bank)
}

def public strudel_load_sample_dir(root : string) {
    dir(root) <| $(entry) {
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

def public strudel_set_bpm(bpm : double) {
    if (g_cmd_channel != null) {
        g_cmd_channel |> push(new StrudelCommand(set_bpm = bpm))
    }
}

def public strudel_set_cps(cps : double) {
    if (g_cmd_channel != null) {
        g_cmd_channel |> push(new StrudelCommand(set_cps = cps))
    }
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
        _builtin_channel_pop(g_done_channel) <| $(data) {
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
    // create channels
    g_cmd_channel = unsafe(channel_create())
    g_cmd_channel |> add_ref()
    g_done_channel = unsafe(channel_create())
    g_done_channel |> add_ref()
    // set up shared state
    g_state.bank = unsafe(addr(g_bank))
    g_state.sid = g_sid
    g_state.wall_time = g_wall_time
    g_state.cps = g_cps
    g_state.cmd_channel = g_cmd_channel
    g_state.cmd_channel |> add_ref()
    g_state.done_channel = g_done_channel
    g_state.done_channel |> add_ref()
    g_state.audio_channel = get_audio_command_channel()
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
    state.tracks |> push(new StrudelTrack(
        name = name,
        pat <- pat,
        gain = gain,
        target_gain = gain
    ))
    var track = state.tracks[length(state.tracks) - 1]
    track.sched.cps = state.cps
    track.sched.lastQueryEnd = state.wall_time * state.cps
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
    state.cmd_channel |> gather() <| $(cmd : StrudelCommand#) {
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
    var sched = Scheduler()
    sched.cps = state.cps
    sched.lastQueryEnd = state.wall_time * state.cps
    var status_box <- unsafe(lock_box_create())
    set_status_update(state.sid, status_box)
    while (strudel_process_commands(state)) {
        if (sched.cps != state.cps) {
            sched.lastQueryEnd = state.wall_time * state.cps
            sched.cps = state.cps
        }
        var chunk <- tick(sched, pat, *state.bank, state.wall_time, float(CHUNK_SEC))
        if (length(chunk) > 0) {
            append_to_pcm(state.sid, chunk)
        }
        state.wall_time += CHUNK_SEC
        var que_len = 0
        status_box |> get() <| $(st : AudioChannelStatus#) {
            que_len = st.stream_que_length
        }
        if (que_len >= TARGET_CHUNKS) {
            sleep(50u)
        } elif (que_len >= LOW_WATERMARK) {
            sleep(10u)
        }
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
    var status_box <- unsafe(lock_box_create())
    set_status_update(state.sid, status_box)
    var mixer : ma_volume_mixer
    ma_volume_mixer_init(unsafe(addr(mixer)), 2u)
    while (strudel_process_commands_ex(state, cmd_fn)) {
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
            var chunk <- tick(track.sched, track.pat, *state.bank, state.wall_time, float(CHUNK_SEC))
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
            append_to_pcm(state.sid, output)
        }
        state.wall_time += CHUNK_SEC
        // buffer level feedback
        var que_len = 0
        status_box |> get() <| $(st : AudioChannelStatus#) {
            que_len = st.stream_que_length
        }
        if (que_len >= TARGET_CHUNKS) {
            sleep(50u)
        } elif (que_len >= LOW_WATERMARK) {
            sleep(10u)
        }
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
