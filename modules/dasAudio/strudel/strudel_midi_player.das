options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module strudel_midi_player shared public

require strudel/strudel_midi
require strudel/strudel_synth
require strudel/strudel_samples
require audio/audio_boost
require audio
require daslib/jobque_boost
require daslib/fio
require math

// ─── GM Instrument Presets ───

struct GmPreset {
    osc   : OscType = OscType.osc_sine
    att   : float = 0.005
    dec   : float = 0.1
    sus   : float = 0.5
    rel   : float = 0.15
}

def gm_preset(program : int) : GmPreset {
    if (program < 8) {         // Piano — percussive, no sustain
        return GmPreset(osc = OscType.osc_triangle, att = 0.002, dec = 1.5, sus = 0.0, rel = 0.1)
    } elif (program < 16) {    // Chromatic Percussion
        return GmPreset(osc = OscType.osc_sine, att = 0.001, dec = 0.8, sus = 0.0, rel = 0.05)
    } elif (program < 24) {    // Organ — sustained
        return GmPreset(osc = OscType.osc_square, att = 0.005, dec = 0.05, sus = 0.7, rel = 0.1)
    } elif (program < 32) {    // Guitar — plucked, fast decay
        return GmPreset(osc = OscType.osc_triangle, att = 0.002, dec = 0.8, sus = 0.0, rel = 0.05)
    } elif (program < 40) {    // Bass — plucked
        return GmPreset(osc = OscType.osc_sawtooth, att = 0.002, dec = 0.6, sus = 0.0, rel = 0.05)
    } elif (program < 48) {    // Strings — bowed, sustained
        return GmPreset(osc = OscType.osc_sawtooth, att = 0.08, dec = 0.2, sus = 0.55, rel = 0.15)
    } elif (program < 56) {    // Ensemble / Synth Strings
        return GmPreset(osc = OscType.osc_sawtooth, att = 0.05, dec = 0.15, sus = 0.5, rel = 0.1)
    } elif (program < 64) {    // Brass — breath buildup
        return GmPreset(osc = OscType.osc_square, att = 0.06, dec = 0.15, sus = 0.6, rel = 0.12)
    } elif (program < 72) {    // Reed
        return GmPreset(osc = OscType.osc_square, att = 0.01, dec = 0.1, sus = 0.5, rel = 0.12)
    } elif (program < 80) {    // Pipe
        return GmPreset(osc = OscType.osc_sine, att = 0.02, dec = 0.05, sus = 0.6, rel = 0.15)
    } elif (program < 88) {    // Synth Lead — snappy, tight
        return GmPreset(osc = OscType.osc_sawtooth, att = 0.005, dec = 0.15, sus = 0.45, rel = 0.08)
    } elif (program < 96) {    // Synth Pad — slow swell
        return GmPreset(osc = OscType.osc_sawtooth, att = 0.2, dec = 0.2, sus = 0.6, rel = 0.25)
    } elif (program < 104) {   // FX (rain, soundtrack, crystal, etc.)
        return GmPreset(osc = OscType.osc_sine, att = 0.05, dec = 0.3, sus = 0.4, rel = 0.2)
    } elif (program < 112) {   // Ethnic (sitar, banjo, shamisen, koto)
        return GmPreset(osc = OscType.osc_triangle, att = 0.002, dec = 0.7, sus = 0.0, rel = 0.05)
    } elif (program < 120) {   // Percussive (tinkle bell, agogo, steel drums)
        return GmPreset(osc = OscType.osc_sine, att = 0.001, dec = 0.5, sus = 0.0, rel = 0.05)
    } elif (program < 128) {   // SFX (guitar fret, breath, seashore, etc.)
        return GmPreset(osc = OscType.osc_sine, att = 0.01, dec = 0.3, sus = 0.3, rel = 0.1)
    }
    return GmPreset()
}

// ─── Channel 10 Drum Map ───

def render_drum_hit(note : int; velocity : float) : array<float> {
    let gain = velocity
    if (note == 35 || note == 36) {
        return <- render_bd(0.2, gain)
    } elif (note == 38 || note == 40) {
        return <- render_sd(0.15, gain)
    } elif (note == 42 || note == 44 || note == 46) {
        return <- render_hh(0.05, gain)
    } elif (note == 39 || note == 49 || note == 51 || note == 52 || note == 57) {
        return <- render_cp(0.1, gain)
    }
    // fallback: short noise burst
    return <- render_hh(0.04, gain * 0.5)
}

// ─── GM Drum Note → Sample Bank Name ───

def gm_drum_name(note : int) : string {
    if (note == 35 || note == 36) { return "bd"; }
    if (note == 37) { return "sidestick"; }
    if (note == 38 || note == 40) { return "sd"; }
    if (note == 42) { return "hh"; }
    if (note == 44) { return "hh_pedal"; }
    if (note == 46) { return "hh_open"; }
    if (note == 43) { return "tom_low"; }
    if (note == 45 || note == 47) { return "tom_high"; }
    if (note == 49 || note == 52 || note == 55 || note == 57) { return "cp"; }
    if (note == 51) { return "ride"; }
    if (note == 53) { return "ride_bell"; }
    return ""
}

// ─── Piano Note → Sample Mapping ───

let PIANO_PITCH_CENTERS = fixed_array(29, 35, 40, 45, 50, 55, 59, 62, 65, 71, 76, 81, 86, 91, 96)

def piano_sample_name(midi_note : int) : tuple<name : string; speed : float> {
    var best = 62
    var bestDist = 200
    for (n in PIANO_PITCH_CENTERS) {
        let dist = abs(midi_note - n)
        if (dist < bestDist) {
            bestDist = dist
            best = n
        }
    }
    let semitones = float(midi_note - best)
    return (name = "note_{best}", speed = pow(2.0, semitones / 12.0))
}

// ─── MIDI ADSR (sustain holds until note-off) ───

def midi_adsr(elapsed, note_off_elapsed, attack, decay, sustain, release : float) : float {
    if (elapsed < 0.0) {
        return 0.0
    }
    var env = 0.0
    if (elapsed < attack) {
        env = elapsed / attack
    } elif (elapsed < attack + decay) {
        let t = elapsed - attack
        env = 1.0 - (1.0 - sustain) * (t / decay)
    } else {
        // natural decay during sustain — even held notes fade over time
        let sustain_time = elapsed - attack - decay
        env = sustain * exp(-sustain_time * 1.2)  // ~0.58 second half-life
    }
    // note still held
    if (note_off_elapsed < 0.0) {
        return env
    }
    // release phase
    if (note_off_elapsed < release) {
        return env * (1.0 - note_off_elapsed / release)
    }
    return 0.0
}

// ─── MIDI Voice ───

struct MidiVoice {
    channel     : int
    note        : int
    freq        : float
    phase       : float = 0.0    // oscillator phase [0,1)
    osc_type    : OscType = OscType.osc_sine
    velocity    : float = 1.0
    note_on_t   : float = 0.0   // playback time when note started
    note_off_t  : float = -1.0  // playback time when note-off received (-1 = still held)
    attack      : float = 0.005
    decay       : float = 0.1
    sustain_lev : float = 0.5
    rel         : float = 0.15
    finished    : bool = false
    sustain_held : bool = false  // note-off received but sustain pedal is on
    // for drums: pre-rendered buffer
    is_drum     : bool = false
    samples     : array<float>   // pre-rendered mono for drums
    position    : int = 0        // read position in drum buffer
}

// ─── Channel State ───

struct MidiChannelState {
    program    : int = 0
    volume     : float = 1.0    // CC7
    pan        : float = 0.0    // CC10 (-1=left, 0=center, 1=right)
    expression : float = 1.0    // CC11
    sustain    : bool = false   // CC64 (sustain pedal)
}

// ─── Playback State (per track) ───

struct MidiPlaybackState {
    name           : string
    events         : array<MidiEvent>   // merged, sorted by tick
    @safe_when_uninitialized channels : MidiChannelState[16]
    voices         : array<MidiVoice?>
    bank           : SampleBank const?  // optional sample bank (pointer to main context)
    ticks_per_qn   : int = 480          // from MIDI file
    event_index    : int = 0
    current_tick   : double = 0.0lf
    ticks_per_sec  : double = 0.0lf
    tempo_usec     : int = 500000       // default 120 BPM
    playback_time  : double = 0.0lf
    looping        : bool = false
    gain           : float = 1.0
    target_gain    : float = 1.0
    fade_speed     : float = 0.0
    done           : bool = false
    mixer          : ma_volume_mixer
    mixer_ready    : bool = false
}

def init_playback(var state : MidiPlaybackState; midi : MidiFile const?; name : string; gain : float; looping : bool; bank : SampleBank const? = null) {
    state.name = name
    state.events <- merge_tracks(*midi)
    state.gain = gain
    state.target_gain = gain
    state.looping = looping
    state.bank = bank
    state.ticks_per_qn = midi.ticks_per_qn
    state.tempo_usec = 500000
    state.ticks_per_sec = double(state.ticks_per_qn) * 1000000.0lf / double(state.tempo_usec)
    for (ch in state.channels) {
        ch = MidiChannelState()
    }
}

// ─── Event Processing ───

def process_events(var state : MidiPlaybackState) {
    while (state.event_index < length(state.events)) {
        let ev & = unsafe(state.events[state.event_index])
        if (double(ev.tick) > state.current_tick) {
            break
        }
        let ch = ev.channel
        if (ev.kind == MidiEventKind.midi_note_on) {
            let vel = float(ev.data2) / 127.0
            // kill existing voice on same channel+note (re-trigger)
            for (voice in state.voices) {
                if (voice != null && voice.channel == ch && voice.note == ev.data1 && !voice.is_drum) {
                    if (voice.note_off_t < 0.0) {
                        voice.note_off_t = float(state.playback_time)
                    }
                }
            }
            if (ch == 9) {
                // drum channel — try sample bank first, fall back to synthesis
                var drum_stereo : array<float>
                if (state.bank != null) {
                    let drum_name = gm_drum_name(ev.data1)
                    if (!empty(drum_name) && key_exists(state.bank.sounds, drum_name)) {
                        drum_stereo <- render_sample(*state.bank, drum_name, ev.data1 % 3, 1.0)
                    }
                }
                if (length(drum_stereo) == 0) {
                    let drum_mono <- render_drum_hit(ev.data1, vel)
                    drum_stereo <- mono_to_stereo(drum_mono)
                }
                state.voices |> push(new MidiVoice(
                    channel = ch,
                    note = ev.data1,
                    velocity = vel,
                    is_drum = true,
                    samples <- drum_stereo,
                    note_on_t = float(state.playback_time)
                ))
            } else {
                // melodic channel — use piano samples for piano programs (0-7), else synthesize
                var use_sample = false
                if (state.bank != null && state.channels[ch].program < 8) {
                    let pn = piano_sample_name(ev.data1)
                    if (key_exists(state.bank.sounds, pn.name)) {
                        var sample_stereo <- render_sample(*state.bank, pn.name, ev.data2 % 3, pn.speed)
                        state.voices |> push(new MidiVoice(
                            channel = ch,
                            note = ev.data1,
                            velocity = vel,
                            is_drum = true,  // pre-rendered like drums
                            samples <- sample_stereo,
                            note_on_t = float(state.playback_time)
                        ))
                        use_sample = true
                    }
                }
                if (!use_sample) {
                    let preset = gm_preset(state.channels[ch].program)
                    state.voices |> push(new MidiVoice(
                        channel = ch,
                        note = ev.data1,
                        freq = note_to_freq(float(ev.data1)),
                        osc_type = preset.osc,
                        velocity = vel,
                        note_on_t = float(state.playback_time),
                        attack = preset.att,
                        decay = preset.dec,
                        sustain_lev = preset.sus,
                        rel = preset.rel
                    ))
                }
            }
        } elif (ev.kind == MidiEventKind.midi_note_off) {
            // find matching voice and trigger release (or defer if sustain pedal is on)
            for (voice in state.voices) {
                if (voice != null && voice.channel == ch && voice.note == ev.data1 && voice.note_off_t < 0.0 && !voice.sustain_held) {
                    if (state.channels[ch].sustain) {
                        voice.sustain_held = true  // defer until pedal release
                    } else {
                        voice.note_off_t = float(state.playback_time)
                    }
                    break
                }
            }
        } elif (ev.kind == MidiEventKind.midi_program_change) {
            state.channels[ch].program = ev.data1
        } elif (ev.kind == MidiEventKind.midi_control_change) {
            if (ev.data1 == 7) {
                state.channels[ch].volume = float(ev.data2) / 127.0
            } elif (ev.data1 == 10) {
                state.channels[ch].pan = (float(ev.data2) / 63.5) - 1.0
            } elif (ev.data1 == 11) {
                state.channels[ch].expression = float(ev.data2) / 127.0
            } elif (ev.data1 == 64) {
                let was_on = state.channels[ch].sustain
                state.channels[ch].sustain = ev.data2 >= 64
                // pedal released — trigger note-off for all held voices on this channel
                if (was_on && !state.channels[ch].sustain) {
                    for (voice in state.voices) {
                        if (voice != null && voice.channel == ch && voice.note_off_t < 0.0 && voice.sustain_held) {
                            voice.note_off_t = float(state.playback_time)
                            voice.sustain_held = false
                        }
                    }
                }
            }
        } elif (ev.kind == MidiEventKind.midi_tempo) {
            state.tempo_usec = ev.tempo
            state.ticks_per_sec = double(state.ticks_per_qn) * 1000000.0lf / double(ev.tempo)
        }
        state.event_index ++
    }
}

// ─── Render One Tick ───

def midi_tick(var state : MidiPlaybackState; chunk_seconds : float) : array<float> {
    process_events(state)
    let chunkFrames = int(chunk_seconds * float(SAMPLE_RATE))
    let chunkSamples = chunkFrames * 2  // stereo
    var output : array<float>
    output |> resize(chunkSamples)
    if (!state.mixer_ready) {
        ma_volume_mixer_init(unsafe(addr(state.mixer)), 2u)
        state.mixer_ready = true
    }
    let iSr = 1.0 / float(SAMPLE_RATE)
    let tp = float(TWO_PI)
    var i = length(state.voices) - 1
    while (i >= 0) {
        var voice = state.voices[i]
        if (voice.is_drum) {
            // mix pre-rendered stereo samples via ma_volume_mixer
            let srcLen = length(voice.samples)
            let ch_state & = unsafe(state.channels[voice.channel])
            let vol = voice.velocity * ch_state.volume * ch_state.expression * state.gain
            let srcStart = voice.position
            let available = srcLen - srcStart
            let space = chunkSamples
            let count = min(available, space)
            if (count > 0) {
                ma_volume_mixer_set_volume(unsafe(addr(state.mixer)), vol)
                ma_volume_mixer_set_pan(unsafe(addr(state.mixer)), ch_state.pan)
                let nFrames = uint64(count / 2)
                unsafe {
                    ma_volume_mixer_process_pcm_frames(
                        addr(state.mixer),
                        addr(voice.samples[srcStart]),
                        addr(output[0]),
                        nFrames
                    )
                }
                voice.position += count
            }
            if (voice.position + 1 >= srcLen) {
                voice.finished = true
            }
        } else {
            let ch_state & = unsafe(state.channels[voice.channel])
            let vol = voice.velocity * ch_state.volume * ch_state.expression * state.gain
            let phaseInc = voice.freq * iSr
            let pan = ch_state.pan
            let lGain = (1.0 - max(pan, 0.0)) * vol
            let rGain = (1.0 + min(pan, 0.0)) * vol
            let t0 = float(state.playback_time) - voice.note_on_t
            let noff0 = voice.note_off_t >= 0.0 ? float(state.playback_time) - voice.note_off_t : -1.0
            let att = voice.attack
            let dec = voice.decay
            let sus = voice.sustain_lev
            let rel = voice.rel
            if (voice.osc_type == OscType.osc_sine) {
                for (f in range(chunkFrames)) {
                    let dt = float(f) * iSr
                    let env = midi_adsr(t0 + dt, noff0 < 0.0 ? -1.0 : noff0 + dt, att, dec, sus, rel)
                    if (env < 0.001 && t0 + dt > 0.5) { voice.finished = true; break; }
                    let sample = sin(voice.phase * tp) * env
                    output[f * 2] += sample * lGain
                    output[f * 2 + 1] += sample * rGain
                    voice.phase += phaseInc
                    voice.phase -= floor(voice.phase)
                }
            } elif (voice.osc_type == OscType.osc_sawtooth) {
                for (f in range(chunkFrames)) {
                    let dt = float(f) * iSr
                    let env = midi_adsr(t0 + dt, noff0 < 0.0 ? -1.0 : noff0 + dt, att, dec, sus, rel)
                    if (env < 0.001 && t0 + dt > 0.5) { voice.finished = true; break; }
                    let sample = (voice.phase * 2.0 - 1.0) * env
                    output[f * 2] += sample * lGain
                    output[f * 2 + 1] += sample * rGain
                    voice.phase += phaseInc
                    voice.phase -= floor(voice.phase)
                }
            } elif (voice.osc_type == OscType.osc_square) {
                for (f in range(chunkFrames)) {
                    let dt = float(f) * iSr
                    let env = midi_adsr(t0 + dt, noff0 < 0.0 ? -1.0 : noff0 + dt, att, dec, sus, rel)
                    if (env < 0.001 && t0 + dt > 0.5) { voice.finished = true; break; }
                    let sample = (voice.phase < 0.5 ? 1.0 : -1.0) * env
                    output[f * 2] += sample * lGain
                    output[f * 2 + 1] += sample * rGain
                    voice.phase += phaseInc
                    voice.phase -= floor(voice.phase)
                }
            } elif (voice.osc_type == OscType.osc_triangle) {
                for (f in range(chunkFrames)) {
                    let dt = float(f) * iSr
                    let env = midi_adsr(t0 + dt, noff0 < 0.0 ? -1.0 : noff0 + dt, att, dec, sus, rel)
                    if (env < 0.001 && t0 + dt > 0.5) { voice.finished = true; break; }
                    let sample = (abs(voice.phase * 4.0 - 2.0) - 1.0) * env
                    output[f * 2] += sample * lGain
                    output[f * 2 + 1] += sample * rGain
                    voice.phase += phaseInc
                    voice.phase -= floor(voice.phase)
                }
            }
        }
        if (voice.finished) {
            unsafe { delete state.voices[i]; }
            state.voices |> erase(i)
        }
        i --
    }
    // advance time
    state.playback_time += double(chunk_seconds)
    state.current_tick += state.ticks_per_sec * double(chunk_seconds)
    // check if done
    if (state.event_index >= length(state.events) && length(state.voices) == 0) {
        if (state.looping) {
            state.event_index = 0
            state.current_tick = 0.0lf
            state.playback_time = 0.0lf
            for (ch in state.channels) {
                ch = MidiChannelState()
            }
            state.tempo_usec = 500000
            // recalculate from original tpqn (approximate — we don't store it separately)
            // For looping, this is fine since the tempo event at tick 0 will re-set it
        } else {
            state.done = true
        }
    }
    return <- output
}

// ─── Command Channel ───

variant MidiCommand {
    shutdown     : bool;
    add_track    : MidiTrackCmd;
    remove_track : string;
    set_volume   : tuple<name : string; volume : float; fade : float>;
    set_pause    : bool;
}

struct MidiTrackCmd {
    name    : string
    file    : MidiFile const?   // pointer to main context's g_midi_files entry
    bank    : SampleBank const? // pointer to main context's sample bank (optional)
    gain    : float = 1.0
    looping : bool = false
}

// ─── Module Globals ───

var private g_midi_sid : uint64 = 0ul
var private g_midi_cmd_channel : Channel?
var private g_midi_done_channel : Channel?
var private g_midi_files : table<string; MidiFile>  // owns parsed data
var private g_midi_bank : SampleBank                // module-global sample bank
var private g_midi_running : bool = false

// ─── Audio Thread Main Loop ───

def private midi_thread_main(sid : uint64; var cmd_channel : Channel?; var done_channel : Channel?; audio_channel : void?) {
    unsafe {
        set_audio_thread_command_channel(audio_channel)
    }
    let CHUNK_SEC = 0.05lf
    let TARGET_CHUNKS = 4
    let LOW_WATERMARK = 2
    var tracks : array<MidiPlaybackState?>
    var status_box <- unsafe(lock_box_create())
    set_status_update(sid, status_box)
    // pending add-track commands (extracted from gather to avoid # lifetime issues)
    var pending_adds : array<MidiTrackCmd>
    var running = true
    while (running) {
        // process commands
        cmd_channel |> gather() <| $(cmd : MidiCommand#) {
            if (cmd is shutdown) {
                running = false
            } elif (cmd is add_track) {
                let tc & = cmd as add_track
                // extract pointer via reinterpret to strip # temp lifetime
                let file_ptr = unsafe(reinterpret<MidiFile const?>(tc.file))
                let bank_ptr = unsafe(reinterpret<SampleBank const?>(tc.bank))
                pending_adds |> emplace(MidiTrackCmd(
                    name := tc.name,
                    file = file_ptr,
                    bank = bank_ptr,
                    gain = tc.gain,
                    looping = tc.looping
                ))
            } elif (cmd is remove_track) {
                let name = string(cmd as remove_track)
                var k = length(tracks) - 1
                while (k >= 0) {
                    if (tracks[k].name == name) {
                        tracks[k].target_gain = 0.0
                        tracks[k].fade_speed = 2.0  // fade out over 0.5s
                    }
                    k --
                }
            } elif (cmd is set_volume) {
                let sv & = unsafe(cmd as set_volume)
                for (track in tracks) {
                    if (track.name == sv._0) {
                        track.target_gain = sv._1
                        if (sv._2 > 0.0) {
                            track.fade_speed = abs(track.target_gain - track.gain) / sv._2
                        } else {
                            track.gain = sv._1
                            track.fade_speed = 0.0
                        }
                    }
                }
            } elif (cmd is set_pause) {
                // simple pause — just stop ticking (audio stream continues with silence)
            }
        }
        // process pending add-track commands (outside gather callback)
        tracks |> reserve(length(tracks) + length(pending_adds))
        for (tc in pending_adds) {
            // remove existing track with same name
            var k = length(tracks) - 1
            while (k >= 0) {
                if (tracks[k].name == tc.name) {
                    unsafe { delete tracks[k]; }
                    tracks |> erase(k)
                }
                k --
            }
            var ps = new MidiPlaybackState()
            init_playback(*ps, tc.file, tc.name, tc.gain, tc.looping, tc.bank)
            tracks |> push(ps)
        }
        pending_adds |> clear()
        if (!running) {
            break
        }
        // wait for tracks before producing audio
        if (length(tracks) == 0) {
            sleep(10u)
            continue
        }
        // render all tracks
        let chunkSamples = int(float(CHUNK_SEC) * float(SAMPLE_RATE)) * 2
        var output : array<float>
        output |> resize(chunkSamples)
        var ii = length(tracks) - 1
        while (ii >= 0) {
            var track = tracks[ii]
            var chunk <- midi_tick(*track, float(CHUNK_SEC))
            if (length(chunk) > 0 && track.gain > 0.001) {
                let n = min(length(chunk), length(output))
                for (j in range(n)) {
                    output[j] += chunk[j] * track.gain
                }
            }
            delete chunk
            // fade envelope
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
            // remove done or faded-out tracks
            if (track.done || (track.gain <= 0.001 && track.target_gain <= 0.001 && track.fade_speed == 0.0)) {
                unsafe { delete tracks[ii]; }
                tracks |> erase(ii)
            }
            ii --
        }
        if (length(output) > 0) {
            append_to_pcm(sid, output)
        }
        delete output
        // adaptive buffering
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
    unset_status_update(sid)
    clear_status(status_box)
    status_box |> release()
    unsafe {
        set_audio_thread_command_channel(null)
    }
    cmd_channel |> release()
    done_channel |> push(new MidiCommand(shutdown = true))
    done_channel |> release()
}

// ─── Public API ───

def public midi_init() {
    if (g_midi_running) {
        return
    }
    if (get_audio_command_channel() == null) {
        panic("midi_init: audio system not initialized. Call audio_system_create() first.")
    }
    if (g_midi_sid == 0ul) {
        g_midi_sid = play_sound_from_pcm_stream(MA_SAMPLE_RATE, MA_CHANNELS)
    }
    g_midi_cmd_channel = unsafe(channel_create())
    g_midi_cmd_channel |> add_ref()
    g_midi_done_channel = unsafe(channel_create())
    g_midi_done_channel |> add_ref()
    let sid = g_midi_sid
    var cmd_ch = g_midi_cmd_channel
    var done_ch = g_midi_done_channel
    let audio_ch = get_audio_command_channel()
    new_thread() <| @capture(= sid, = cmd_ch, = done_ch, = audio_ch) {
        midi_thread_main(sid, cmd_ch, done_ch, audio_ch)
        cmd_ch = null
        done_ch = null
    }
    g_midi_running = true
}

// Load standard piano + drum samples into the module-global bank.
def public midi_load_samples(media_path : string) {
    // piano samples
    for (n in PIANO_PITCH_CENTERS) {
        load_sound("{media_path}/piano/note_{n}", "note_{n}", g_midi_bank)
    }
    // drum samples
    let drum_dirs = fixed_array("bd", "sd", "hh", "hh_pedal", "hh_open", "cp", "ride", "ride_bell", "sidestick", "tom_low", "tom_high")
    for (d in drum_dirs) {
        load_sound("{media_path}/drums/{d}", d, g_midi_bank)
    }
}

def public midi_play(name : string; path : string; gain : float = 1.0; looping : bool = false; bank : SampleBank const? = null) : bool {
    g_midi_files[name] <- load_midi(path)
    if (length(g_midi_files[name].tracks) == 0) {
        g_midi_files |> erase(name)
        return false
    }
    if (!g_midi_running) {
        midi_init()
    }
    let file_ptr = unsafe(addr(g_midi_files[name]))
    var cmd = new MidiCommand(add_track = MidiTrackCmd(
        name = name,
        file = file_ptr,
        bank = bank != null ? bank : unsafe(reinterpret<SampleBank const?>(unsafe(addr(g_midi_bank)))),
        gain = gain,
        looping = looping
    ))
    g_midi_cmd_channel |> push(cmd)
    return true
}

def public midi_play_data(name : string; data : array<uint8>; gain : float = 1.0; looping : bool = false; bank : SampleBank const? = null) : bool {
    g_midi_files[name] <- parse_midi(data)
    if (length(g_midi_files[name].tracks) == 0) {
        g_midi_files |> erase(name)
        return false
    }
    if (!g_midi_running) {
        midi_init()
    }
    let file_ptr = unsafe(addr(g_midi_files[name]))
    var cmd = new MidiCommand(add_track = MidiTrackCmd(
        name = name,
        file = file_ptr,
        bank = bank != null ? bank : unsafe(reinterpret<SampleBank const?>(unsafe(addr(g_midi_bank)))),
        gain = gain,
        looping = looping
    ))
    g_midi_cmd_channel |> push(cmd)
    return true
}

def public midi_stop(name : string = "") {
    if (!g_midi_running) {
        return
    }
    if (empty(name)) {
        // stop all — shutdown the thread
        midi_shutdown()
    } else {
        var cmd = new MidiCommand(shutdown = false)
        unsafe {
            set_variant_index(*cmd, 2)  // remove_track
            cmd.remove_track = name
        }
        g_midi_cmd_channel |> push(cmd)
    }
}

def public midi_set_volume(name : string; volume : float; fade : float = 0.0) {
    if (!g_midi_running || g_midi_cmd_channel == null) {
        return
    }
    var cmd = new MidiCommand(shutdown = false)
    unsafe {
        set_variant_index(*cmd, 3)  // set_volume
        cmd.set_volume = (name, volume, fade)
    }
    g_midi_cmd_channel |> push(cmd)
}

def public midi_set_pause(paused : bool) {
    if (!g_midi_running || g_midi_cmd_channel == null) {
        return
    }
    g_midi_cmd_channel |> push(new MidiCommand(set_pause = paused))
}

def public midi_shutdown() {
    if (!g_midi_running || g_midi_cmd_channel == null) {
        return
    }
    g_midi_cmd_channel |> push(new MidiCommand(shutdown = true))
    var got_done = false
    while (!got_done) {
        _builtin_channel_pop(g_midi_done_channel) <| $(data) {
            if (data != null) {
                got_done = true
            }
        }
    }
    g_midi_cmd_channel |> release()
    g_midi_cmd_channel = null
    g_midi_done_channel |> release()
    g_midi_done_channel = null
    g_midi_running = false
}

def public midi_is_playing() : bool {
    return g_midi_running
}
