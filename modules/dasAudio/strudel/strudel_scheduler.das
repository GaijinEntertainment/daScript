options gen2
options indenting = 4

module strudel_scheduler shared

require strudel/strudel_event
require strudel/strudel_time
require strudel/strudel_pattern
require strudel/strudel_synth
require strudel/strudel_samples
require strudel/strudel_sf2
require strudel/strudel_sf2_voice
require audio/audio_boost
require audio
require math

// An active voice: a pre-rendered stereo buffer being played back.
struct Voice {
    samples : array<float>   // interleaved stereo PCM (raw, no gain/pan)
    position : int = 0       // current read position (in floats, not frames)
    cut : int = 0            // cut group (0 = no cut)
    gain : float = 0.8       // volume for this voice
    pan : float = 0.0        // pan for this voice (-1=left, 0=center, 1=right)
    reverb_send : float = 0.0 // reverb send amount from Event.room (0.0–1.0)
    delay_send : float = 0.0  // delay send amount from Event.delay_amount (0.0–1.0)
    orbit : int = 0           // effect bus routing
}

// Per-voice metadata for SF2 voices (not stored in the C ma_sf2_voice struct).
struct StrudelSF2Meta {
    note             : int = 0       // MIDI note (for re-trigger matching)
    note_off_sample  : int = -1      // sample position to fire note-off (-1 = don't auto note-off)
    samples_elapsed  : int = 0       // how many samples rendered so far
    reverb_send      : float = 0.0   // per-voice reverb amount from SF2 generators
    chorus_send      : float = 0.0   // per-voice chorus amount from SF2 generators
    delay_send       : float = 0.0   // per-voice delay amount from Event.delay_amount
    exclusive_class  : int = 0       // SF2 exclusive class (hi-hat choke etc.)
    cut              : int = 0       // strudel cut group
    gain             : float = 1.0   // event gain * velocity (applied as attenuation scale)
    orbit            : int = 0       // effect bus routing
}

// Per-orbit effect bus. Each orbit has independent reverb + delay instances.
// Orbits are created on demand when a voice with that orbit index is spawned.
[safe_when_uninitialized]
struct OrbitBus {
    reverb            : I3DL2Reverb?
    delay_proc        : ma_delay?
    reverb_send_buf   : array<float>
    reverb_out_buf    : array<float>
    delay_send_buf    : array<float>
    delay_out_buf     : array<float>
    current_roomsize      : float = -1.0
    current_delaytime     : float = -1.0
    current_delayfeedback : float = -1.0
}

// The scheduler bridges the pattern engine and audio output.
// Maintains two pools: pre-rendered sample voices and real-time SF2 voices.
// Effects are routed through per-orbit buses (each orbit has its own reverb + delay).
[safe_when_uninitialized]
struct Scheduler {
    cps          : double = 0.5lf   // cycles per second (0.5 = 120 BPM at 4 beats/cycle)
    startTime    : double = 0.0lf   // wall-clock time when playback started
    lastQueryEnd : double = 0.0lf   // cycle position of last query end
    voices       : array<Voice?>    // pre-rendered sample/oscillator voices
    osc_voices   : array<OscVoice> // streaming oscillator voices
    mixer        : ma_volume_mixer  // reused across ticks
    mixer_ready  : bool = false
    output       : array<float>    // reusable output buffer (avoids per-tick allocation)
    // SF2 real-time voice pool
    sf2           : SF2File const?        // loaded soundfont (null = SF2 disabled)
    sf2_voices    : array<ma_sf2_voice>   // real-time SF2 voices (C structs)
    sf2_meta      : array<StrudelSF2Meta> // parallel metadata
    // Per-orbit effect buses (created on demand)
    orbit_buses   : array<OrbitBus?>
    // Chorus (SF2-only, not orbit-routed)
    chorus          : ma_chorus?
    chorus_send_buf : array<float>
    chorus_out_buf  : array<float>
}

// Convert wall-clock time to cycle position.
def current_cycle(sched : Scheduler; wall_time : double) : double {
    return (wall_time - sched.startTime) * sched.cps
}

// Ensure orbit bus exists at given index, creating it if needed.
def private ensure_orbit(var sched : Scheduler; orbit : int) {
    while (length(sched.orbit_buses) <= orbit) {
        sched.orbit_buses |> push(null)
    }
    if (sched.orbit_buses[orbit] == null) {
        sched.orbit_buses[orbit] = new OrbitBus()
    }
}

// Initialize reverb on an orbit bus.
def private init_orbit_reverb(var bus : OrbitBus) {
    if (bus.reverb == null) {
        bus.reverb = new I3DL2Reverb
        set_sample_rate(bus.reverb, float(SAMPLE_RATE))
        let props = get_preset(I3DL2Preset.Bathroom)
        set_properties(bus.reverb, props)
        bus.current_roomsize = -1.0
    }
}

// Update reverb decay time on an orbit bus. Only reconfigures if changed.
def private update_orbit_reverb_roomsize(var bus : OrbitBus; roomsize : float) {
    let rs = clamp(roomsize, 0.1, 10.0)
    if (abs(rs - bus.current_roomsize) < 0.01) {
        return
    }
    bus.current_roomsize = rs
    var props = get_preset(I3DL2Preset.Bathroom)
    props.flDecayTime = rs
    set_properties(bus.reverb, props)
}

// Initialize delay on an orbit bus.
def private init_orbit_delay(var bus : OrbitBus; delaytime : float = 0.5; delayfeedback : float = 0.5) {
    if (bus.delay_proc == null) {
        bus.delay_proc = new ma_delay
    } else {
        delay_uninit(bus.delay_proc)
    }
    let dt = clamp(delaytime, 0.001, 2.0)
    let fb = clamp(delayfeedback, 0.0, 0.98)
    delay_init(bus.delay_proc, SAMPLE_RATE, dt, fb)
    bus.current_delaytime = dt
    bus.current_delayfeedback = fb
}

// Update delay parameters on an orbit bus if changed.
def private update_orbit_delay_params(var bus : OrbitBus; delaytime, delayfeedback : float) {
    let dt = clamp(delaytime, 0.001, 2.0)
    let fb = clamp(delayfeedback, 0.0, 0.98)
    if (abs(dt - bus.current_delaytime) < 0.0001 && abs(fb - bus.current_delayfeedback) < 0.001) {
        return
    }
    bus.current_delaytime = dt
    bus.current_delayfeedback = fb
    delay_set_params(bus.delay_proc, SAMPLE_RATE, dt, fb)
}

// Initialize reverb on the scheduler's default orbit (backwards compat).
def init_reverb(var sched : Scheduler) {
    ensure_orbit(sched, 0)
    init_orbit_reverb(*sched.orbit_buses[0])
}

// Initialize chorus on the scheduler (SF2-only, not orbit-routed).
def init_chorus(var sched : Scheduler) {
    if (sched.chorus == null) {
        sched.chorus = new ma_chorus
        chorus_init(sched.chorus, float(SAMPLE_RATE))
        let cfg = chorus_config_default()
        chorus_set_config(sched.chorus, cfg)
    }
}

// Initialize delay on the scheduler's default orbit (backwards compat).
def init_delay(var sched : Scheduler; delaytime : float = 0.5; delayfeedback : float = 0.5) {
    ensure_orbit(sched, 0)
    init_orbit_delay(*sched.orbit_buses[0], delaytime, delayfeedback)
}

// Ensure orbit bus has reverb/delay initialized as needed for the event.
def private setup_orbit_for_event(var sched : Scheduler; event : Event) {
    let orb = event.orbit
    ensure_orbit(sched, orb)
    var bus = sched.orbit_buses[orb]
    if (event.room > 0.0) {
        if (bus.reverb == null) {
            init_orbit_reverb(*bus)
        }
        update_orbit_reverb_roomsize(*bus, event.roomsize)
    }
    if (event.delay_amount > 0.0) {
        if (bus.delay_proc == null) {
            init_orbit_delay(*bus, event.delaytime, event.delayfeedback)
        } else {
            update_orbit_delay_params(*bus, event.delaytime, event.delayfeedback)
        }
    }
}

// Spawn SF2 voices for an event onset.
def private sf2_spawn_voices(var sched : Scheduler; event : Event; durSec : float; event_delay_send : float) {
    let note = int(event.note)
    let velocity = clamp(int(event.velocity * 127.0), 1, 127)
    let preset_idx = sf2_find_preset(*sched.sf2, event.sf2_bank, event.sf2_program)
    if (preset_idx < 0) {
        return
    }
    let zones = sf2_find_zones(*sched.sf2, preset_idx, note, velocity)
    if (length(zones) == 0) {
        return
    }
    let preset = sched.sf2.presets[preset_idx]
    let note_off_sample = int(durSec * float(SAMPLE_RATE))
    let cutGroup = event.cut
    let eventGain = event.gain * event.velocity
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
        let excl = sf2_get_exclusive_class(*sched.sf2, z.x, z.y, pzone, preset.global_zone)
        var voice : ma_sf2_voice
        if (sf2_create_c_voice(*sched.sf2, z.x, z.y, pzone, preset.global_zone, note, velocity, 0, cc_defaults, voice, atten_state)) {
            // exclusive class: quick-kill existing SF2 voices with same class
            if (excl > 0) {
                for (i in range(length(sched.sf2_voices))) {
                    if (sched.sf2_meta[i].exclusive_class == excl && sched.sf2_voices[i].finished == 0) {
                        unsafe { ma_sf2_voice_end_quick(addr(sched.sf2_voices[i])); }
                    }
                }
            }
            // cut groups: kill existing SF2 voices in same cut group
            if (cutGroup != 0) {
                for (i in range(length(sched.sf2_voices))) {
                    if (sched.sf2_meta[i].cut == cutGroup && sched.sf2_voices[i].finished == 0) {
                        unsafe { ma_sf2_voice_end_quick(addr(sched.sf2_voices[i])); }
                    }
                }
            }
            let rsend = sf2_get_reverb_send(*sched.sf2, z.x, z.y, pzone, preset.global_zone)
            let csend = sf2_get_chorus_send(*sched.sf2, z.x, z.y, pzone, preset.global_zone)
            sched.sf2_voices |> push(voice)
            sched.sf2_meta |> push(StrudelSF2Meta(
                note = note,
                note_off_sample = note_off_sample,
                reverb_send = rsend,
                chorus_send = csend,
                delay_send = event_delay_send,
                exclusive_class = excl,
                cut = cutGroup,
                gain = eventGain,
                orbit = event.orbit
            ))
        }
    }
}

// Get the reverb send buffer for a given orbit. Returns empty array ref if no reverb on that orbit.
def private get_orbit_reverb_send(var sched : Scheduler; orbit : int) : array<float>? {
    if (orbit < length(sched.orbit_buses) && sched.orbit_buses[orbit] != null && sched.orbit_buses[orbit].reverb != null) {
        return unsafe(addr(sched.orbit_buses[orbit].reverb_send_buf))
    }
    return null
}

// Get the delay send buffer for a given orbit. Returns null if no delay on that orbit.
def private get_orbit_delay_send(var sched : Scheduler; orbit : int) : array<float>? {
    if (orbit < length(sched.orbit_buses) && sched.orbit_buses[orbit] != null && sched.orbit_buses[orbit].delay_proc != null) {
        return unsafe(addr(sched.orbit_buses[orbit].delay_send_buf))
    }
    return null
}

// Render all active SF2 voices into the output buffer. Handles note-off scheduling,
// reverb/chorus/delay send, and voice cleanup.
// Reverb and delay sends accumulate into per-orbit bus buffers.
// Chorus send is self-contained (SF2-only, not orbit-routed).
def private sf2_render_voices(var sched : Scheduler; var output : array<float>; chunkFrames : int) {
    if (length(sched.sf2_voices) == 0 || sched.sf2 == null) {
        return
    }
    let buf_samples = chunkFrames * 2
    // prepare chorus send bus (SF2-only, self-contained)
    var has_chorus_send = false
    if (sched.chorus != null) {
        if (length(sched.chorus_send_buf) < buf_samples) {
            sched.chorus_send_buf |> resize(buf_samples)
            sched.chorus_out_buf |> resize(buf_samples)
        }
        for (s in range(buf_samples)) {
            sched.chorus_send_buf[s] = 0.0
        }
    }
    var si = length(sched.sf2_voices) - 1
    while (si >= 0) {
        if (sched.sf2_voices[si].finished == 0) {
            let meta & = unsafe(sched.sf2_meta[si])
            if (meta.note_off_sample >= 0 && meta.samples_elapsed >= meta.note_off_sample && sched.sf2_voices[si].released == 0) {
                unsafe { ma_sf2_voice_note_off(addr(sched.sf2_voices[si])); }
            }
            let saved_atten = sched.sf2_voices[si].attenuation
            sched.sf2_voices[si].attenuation = saved_atten * meta.gain
            let reverb_send = meta.reverb_send
            let chorus_send = meta.chorus_send
            let delay_send_amt = meta.delay_send
            let has_sends = reverb_send > 0.001 || chorus_send > 0.001 || delay_send_amt > 0.001
            if (has_sends) {
                var reverb_buf = get_orbit_reverb_send(sched, meta.orbit)
                var delay_buf = get_orbit_delay_send(sched, meta.orbit)
                let has_reverb = reverb_buf != null && reverb_send > 0.001
                let has_delay = delay_buf != null && delay_send_amt > 0.001
                let has_chorus = sched.chorus != null && chorus_send > 0.001
                if (has_reverb || has_chorus || has_delay) {
                    let dry_gain = max(0.0, 1.0 - reverb_send - chorus_send)
                    if (has_delay && has_reverb && has_chorus) {
                        unsafe {
                            ma_sf2_voice_render_send3(
                                addr(sched.sf2_voices[si]),
                                addr(sched.sf2.sample_data[0]),
                                length(sched.sf2.sample_data),
                                addr(output[0]),
                                addr((*reverb_buf)[0]),
                                addr(sched.chorus_send_buf[0]),
                                addr((*delay_buf)[0]),
                                0, chunkFrames,
                                dry_gain, reverb_send, chorus_send, delay_send_amt
                            )
                        }
                    } elif (has_reverb && has_chorus) {
                        unsafe {
                            ma_sf2_voice_render_send2(
                                addr(sched.sf2_voices[si]),
                                addr(sched.sf2.sample_data[0]),
                                length(sched.sf2.sample_data),
                                addr(output[0]),
                                addr((*reverb_buf)[0]),
                                addr(sched.chorus_send_buf[0]),
                                0, chunkFrames,
                                dry_gain, reverb_send, chorus_send
                            )
                        }
                    } elif (has_reverb) {
                        let actual_dry = max(0.0, 1.0 - reverb_send)
                        unsafe {
                            ma_sf2_voice_render_send(
                                addr(sched.sf2_voices[si]),
                                addr(sched.sf2.sample_data[0]),
                                length(sched.sf2.sample_data),
                                addr(output[0]),
                                addr((*reverb_buf)[0]),
                                0, chunkFrames,
                                actual_dry, reverb_send
                            )
                        }
                    } elif (has_delay) {
                        unsafe {
                            ma_sf2_voice_render_send(
                                addr(sched.sf2_voices[si]),
                                addr(sched.sf2.sample_data[0]),
                                length(sched.sf2.sample_data),
                                addr(output[0]),
                                addr((*delay_buf)[0]),
                                0, chunkFrames,
                                max(0.0, 1.0 - delay_send_amt), delay_send_amt
                            )
                        }
                    } elif (has_chorus) {
                        unsafe {
                            ma_sf2_voice_render_send(
                                addr(sched.sf2_voices[si]),
                                addr(sched.sf2.sample_data[0]),
                                length(sched.sf2.sample_data),
                                addr(output[0]),
                                addr(sched.chorus_send_buf[0]),
                                0, chunkFrames,
                                max(0.0, 1.0 - chorus_send), chorus_send
                            )
                        }
                    }
                    if (chorus_send > 0.001) { has_chorus_send = true; }
                } else {
                    unsafe {
                        ma_sf2_voice_render(
                            addr(sched.sf2_voices[si]),
                            addr(sched.sf2.sample_data[0]),
                            length(sched.sf2.sample_data),
                            addr(output[0]),
                            0, chunkFrames
                        )
                    }
                }
            } else {
                unsafe {
                    ma_sf2_voice_render(
                        addr(sched.sf2_voices[si]),
                        addr(sched.sf2.sample_data[0]),
                        length(sched.sf2.sample_data),
                        addr(output[0]),
                        0, chunkFrames
                    )
                }
            }
            sched.sf2_voices[si].attenuation = saved_atten
            sched.sf2_meta[si].samples_elapsed += chunkFrames
        }
        if (sched.sf2_voices[si].finished != 0) {
            sched.sf2_voices |> erase(si)
            sched.sf2_meta |> erase(si)
        }
        si --
    }
    // process chorus send bus and mix into output (SF2-only)
    if (has_chorus_send && sched.chorus != null) {
        for (s in range(buf_samples)) {
            sched.chorus_out_buf[s] = 0.0
        }
        unsafe {
            chorus_process(sched.chorus, addr(sched.chorus_send_buf[0]), addr(sched.chorus_out_buf[0]), chunkFrames)
        }
        for (s in range(buf_samples)) {
            output[s] += sched.chorus_out_buf[s]
        }
    }
}

// Render all active streaming oscillator voices into the output buffer.
// Sends accumulate into per-orbit bus buffers based on each voice's orbit.
def private osc_render_voices(var sched : Scheduler; var output : array<float>; chunkFrames : int) {
    var i = length(sched.osc_voices) - 1
    while (i >= 0) {
        let orb = sched.osc_voices[i].orbit
        var reverb_buf : array<float>?
        var delay_buf : array<float>?
        if (sched.osc_voices[i].reverb_send > 0.0) {
            reverb_buf = get_orbit_reverb_send(sched, orb)
        }
        if (sched.osc_voices[i].delay_send > 0.0) {
            delay_buf = get_orbit_delay_send(sched, orb)
        }
        // osc_render_chunk needs non-null array refs — use empty statics if no bus
        if (reverb_buf != null && delay_buf != null) {
            osc_render_chunk(sched.osc_voices[i], output, *reverb_buf, *delay_buf, chunkFrames)
        } elif (reverb_buf != null) {
            var empty : array<float>
            osc_render_chunk(sched.osc_voices[i], output, *reverb_buf, empty, chunkFrames)
        } elif (delay_buf != null) {
            var empty : array<float>
            osc_render_chunk(sched.osc_voices[i], output, empty, *delay_buf, chunkFrames)
        } else {
            var empty1 : array<float>
            var empty2 : array<float>
            osc_render_chunk(sched.osc_voices[i], output, empty1, empty2, chunkFrames)
        }
        if (sched.osc_voices[i].finished) {
            sched.osc_voices |> erase(i)
        }
        i --
    }
}

// Prepare orbit bus send buffers for a tick (resize + zero).
def private prepare_orbit_buses(var sched : Scheduler; chunkSamples : int) {
    for (bus in sched.orbit_buses) {
        if (bus == null) {
            continue
        }
        if (bus.reverb != null) {
            if (length(bus.reverb_send_buf) < chunkSamples) {
                bus.reverb_send_buf |> resize(chunkSamples)
                bus.reverb_out_buf |> resize(chunkSamples)
            }
            for (s in range(chunkSamples)) {
                bus.reverb_send_buf[s] = 0.0
            }
        }
        if (bus.delay_proc != null) {
            if (length(bus.delay_send_buf) < chunkSamples) {
                bus.delay_send_buf |> resize(chunkSamples)
                bus.delay_out_buf |> resize(chunkSamples)
            }
            for (s in range(chunkSamples)) {
                bus.delay_send_buf[s] = 0.0
            }
        }
    }
}

// Process all orbit bus effects and mix into output.
def private process_orbit_buses(var sched : Scheduler; var output : array<float>; chunkFrames : int) {
    let chunkSamples = chunkFrames * 2
    for (bus in sched.orbit_buses) {
        if (bus == null) {
            continue
        }
        // Always process reverb when it exists — internal delay lines need to decay
        if (bus.reverb != null) {
            for (s in range(chunkSamples)) {
                bus.reverb_out_buf[s] = 0.0
            }
            unsafe {
                process_stereo(bus.reverb, addr(bus.reverb_send_buf[0]), addr(bus.reverb_out_buf[0]), chunkFrames)
            }
            // Swap L/R reverb output to match expected spatial image.
            // I3DL2 Hadamard decorrelation produces the mirror of ConvolverNode's random-noise IR.
            for (f in range(chunkFrames)) {
                let tmp = bus.reverb_out_buf[f * 2]
                bus.reverb_out_buf[f * 2] = bus.reverb_out_buf[f * 2 + 1]
                bus.reverb_out_buf[f * 2 + 1] = tmp
            }
            for (s in range(chunkSamples)) {
                output[s] += bus.reverb_out_buf[s]
            }
        }
        // Always process delay when it exists — internal buffer needs to decay
        if (bus.delay_proc != null) {
            for (s in range(chunkSamples)) {
                bus.delay_out_buf[s] = 0.0
            }
            unsafe {
                delay_process(bus.delay_proc, addr(bus.delay_send_buf[0]), addr(bus.delay_out_buf[0]), chunkFrames)
            }
            for (s in range(chunkSamples)) {
                output[s] += bus.delay_out_buf[s]
            }
        }
    }
}

// Tick the scheduler: query pattern for new events, render them,
// mix all active voices into a fixed-size stereo buffer.
//
// chunk_seconds: how much audio to produce this tick (e.g. 0.05 for 50ms)
// lookAhead: how far ahead in time to query for new events (in seconds)
def tick(var sched : Scheduler; var pat : Pattern; bank : SampleBank; wall_time : double; chunk_seconds : float = 0.05; lookAhead : float = 0.1) : void {
    let cycleNow = current_cycle(sched, wall_time)
    let cycleEnd = current_cycle(sched, wall_time + double(lookAhead))
    let spawnEnd = current_cycle(sched, wall_time + double(chunk_seconds) + 0.01lf)

    // query for new events
    let queryStart = max(cycleNow, sched.lastQueryEnd)
    if (queryStart < cycleEnd) {
        var haps <- pat(TimeSpan(start = queryStart, stop = cycleEnd))
        for (h in haps) {
            if (h.has_whole && abs(h.whole.start - h.part.start) < 0.0001lf && h.whole.start < spawnEnd) {
                let durCycles = h.whole.stop - h.whole.start
                let durSec = float(durCycles / sched.cps)
                let cutGroup = h.value.cut
                let onsetCycle = h.whole.start
                let onsetWall = sched.startTime + onsetCycle / sched.cps
                let offsetSec = float(onsetWall - wall_time)
                // cut groups: kill voices in all pools
                if (cutGroup != 0) {
                    var k = length(sched.voices) - 1
                    while (k >= 0) {
                        if (sched.voices[k].cut == cutGroup) {
                            unsafe { delete sched.voices[k]; }
                            sched.voices |> erase(k)
                        }
                        k --
                    }
                    var ko = length(sched.osc_voices) - 1
                    while (ko >= 0) {
                        if (sched.osc_voices[ko].cut == cutGroup) {
                            sched.osc_voices |> erase(ko)
                        }
                        ko --
                    }
                }
                // setup orbit bus (auto-init reverb/delay as needed)
                setup_orbit_for_event(sched, h.value)
                // SF2 path: spawn real-time voices
                if (h.value.sf2_program >= 0 && sched.sf2 != null) {
                    sf2_spawn_voices(sched, h.value, durSec, h.value.delay_amount)
                } elif (to_osc_type(h.value.s) != OscType.osc_unknown) {
                    // streaming oscillator path
                    let offsetFrames = max(int(offsetSec * float(SAMPLE_RATE)), 0)
                    let oscType = to_osc_type(h.value.s)
                    var oscv = OscVoice(
                        osc_type = oscType,
                        freq = note_to_freq(h.value.note),
                        duration = durSec,
                        attack = h.value.attack,
                        decay = h.value.decay,
                        sustain = h.value.sustain,
                        release_sec = h.value.release,
                        gain = h.value.gain * h.value.velocity,
                        pan = h.value.pan * 2.0 - 1.0,
                        cut = cutGroup,
                        offset_frames = offsetFrames,
                        fm_depth = h.value.fm,
                        fm_harmonicity = h.value.fmh,
                        reverb_send = h.value.room,
                        delay_send = h.value.delay_amount,
                        orbit = h.value.orbit
                    )
                    osc_voice_init_filters(oscv, h.value.lpf, h.value.hpf, h.value.lpq, h.value.hpq)
                    if (!empty(h.value.vowel)) {
                        vowel_filter_init(oscv.vowel_filter, h.value.vowel)
                    }
                    if (oscType == OscType.osc_supersaw) {
                        osc_voice_init_supersaw(oscv)
                    }
                    sched.osc_voices |> push(oscv)
                } else {
                    // sample/drum path: pre-render to buffer
                    var stereo <- render_event_stereo(h.value, durSec, bank)
                    let offsetSamples = max(int(offsetSec * float(SAMPLE_RATE)) * 2, 0)
                    sched.voices |> push(new Voice(
                        samples <- stereo,
                        position = -offsetSamples,
                        cut = cutGroup,
                        gain = h.value.gain * h.value.velocity,
                        pan = h.value.pan * 2.0 - 1.0,
                        reverb_send = h.value.room,
                        delay_send = h.value.delay_amount,
                        orbit = h.value.orbit
                    ))
                }
            }
        }
        delete haps
        sched.lastQueryEnd = spawnEnd
    }

    // produce output
    let chunkFrames = int(chunk_seconds * float(SAMPLE_RATE))
    let chunkSamples = chunkFrames * 2
    sched.output |> resize(chunkSamples)
    for (s in sched.output) {
        s = 0.0
    }

    // prepare per-orbit send buffers
    prepare_orbit_buses(sched, chunkSamples)

    // mix all pre-rendered voices into the output using ma_volume_mixer (gain + pan per voice)
    if (!sched.mixer_ready) {
        ma_volume_mixer_init(unsafe(addr(sched.mixer)), 2u)
        sched.mixer_ready = true
    }
    var i = length(sched.voices) - 1
    while (i >= 0) {
        var voice = sched.voices[i]
        let voiceLen = length(voice.samples)
        let dstStart = max(0, -voice.position)
        let srcStart = max(0, voice.position)
        let available = voiceLen - srcStart
        let space = chunkSamples - dstStart
        let count = min(available, space)
        if (count > 0) {
            ma_volume_mixer_set_volume(unsafe(addr(sched.mixer)), voice.gain)
            ma_volume_mixer_set_pan_immediate(unsafe(addr(sched.mixer)), voice.pan)
            let nFrames = uint64(count / 2)
            unsafe {
                ma_volume_mixer_process_pcm_frames(
                    addr(sched.mixer),
                    addr(voice.samples[srcStart]),
                    addr(sched.output[dstStart]),
                    nFrames
                )
            }
            // reverb send for pre-rendered voices
            var reverb_buf = get_orbit_reverb_send(sched, voice.orbit)
            if (voice.reverb_send > 0.0 && reverb_buf != null) {
                let pan = clamp(voice.pan, -1.0, 1.0)
                let angle = (pan + 1.0) * 0.25 * float(TWO_PI / 2.0)
                let sL = voice.gain * voice.reverb_send * cos(angle)
                let sR = voice.gain * voice.reverb_send * sin(angle)
                let nf = count / 2
                for (f in range(nf)) {
                    let si = srcStart + f * 2
                    let di = dstStart + f * 2
                    (*reverb_buf)[di]     += voice.samples[si]     * sL
                    (*reverb_buf)[di + 1] += voice.samples[si + 1] * sR
                }
            }
            // delay send for pre-rendered voices
            var delay_buf = get_orbit_delay_send(sched, voice.orbit)
            if (voice.delay_send > 0.0 && delay_buf != null) {
                let pan = clamp(voice.pan, -1.0, 1.0)
                let angle = (pan + 1.0) * 0.25 * float(TWO_PI / 2.0)
                let dL = voice.gain * voice.delay_send * cos(angle)
                let dR = voice.gain * voice.delay_send * sin(angle)
                let nf = count / 2
                for (f in range(nf)) {
                    let si = srcStart + f * 2
                    let di = dstStart + f * 2
                    (*delay_buf)[di]     += voice.samples[si]     * dL
                    (*delay_buf)[di + 1] += voice.samples[si + 1] * dR
                }
            }
        }
        voice.position += chunkSamples
        if (voice.position >= voiceLen) {
            unsafe {
                delete sched.voices[i]
            }
            sched.voices |> erase(i)
        }
        i --
    }

    // render SF2 real-time voices (accumulates into per-orbit reverb/delay send bufs)
    sf2_render_voices(sched, sched.output, chunkFrames)

    // render streaming oscillator voices (accumulates into per-orbit send bufs)
    osc_render_voices(sched, sched.output, chunkFrames)

    // process all orbit bus effects and mix into output
    process_orbit_buses(sched, sched.output, chunkFrames)

    // output stays in sched.output — caller reads it directly
}

// Accessors for orbit bus effects (for tests and backwards compat).
def get_orbit_reverb(var sched : Scheduler; orbit : int = 0) : I3DL2Reverb? {
    if (orbit < length(sched.orbit_buses) && sched.orbit_buses[orbit] != null) {
        return sched.orbit_buses[orbit].reverb
    }
    return null
}

def get_orbit_delay(var sched : Scheduler; orbit : int = 0) : ma_delay? {
    if (orbit < length(sched.orbit_buses) && sched.orbit_buses[orbit] != null) {
        return sched.orbit_buses[orbit].delay_proc
    }
    return null
}

// Release native resources owned by the scheduler.
def shutdown_scheduler(var sched : Scheduler) {
    for (bus in sched.orbit_buses) {
        if (bus != null) {
            if (bus.delay_proc != null) {
                delay_uninit(bus.delay_proc)
                unsafe { delete bus.delay_proc; }
            }
            if (bus.reverb != null) {
                unsafe { delete bus.reverb; }
            }
        }
    }
    if (sched.chorus != null) {
        unsafe { delete sched.chorus; }
    }
}

// Set tempo in BPM (assumes 4 beats per cycle).
def set_bpm(var sched : Scheduler; bpm : double) {
    sched.cps = bpm / 240.0lf
}

// Set cycles per second directly.
def setcps(var sched : Scheduler; cps : double) {
    sched.cps = cps
}
