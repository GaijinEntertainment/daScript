options gen2
options indenting = 4

module strudel_scheduler shared public
//! Voice allocation, effect bus routing, and per-tick mixing for strudel playback.
//! Maintains pre-rendered sample voices, streaming oscillator voices, and real-time SF2 voices,
//! routing each through per-orbit reverb/delay/chorus buses (one independent FX chain per orbit).

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

//! An active voice: a pre-rendered stereo buffer being played back.
struct Voice {
    samples : array<float>   // interleaved stereo PCM (raw, no gain/pan)
        //! Interleaved stereo PCM buffer (raw, without gain/pan applied).
    position : int = 0       // current read position (in floats, not frames)
        //! Current read position in floats (not frames); negative values delay playback start.
    cut : int = 0            // cut group (0 = no cut)
        //! Cut group — new voices in the same group kill older ones (0 = no cut).
    gain : float = 0.8       // volume for this voice
        //! Linear gain applied during mixing.
    pan : float = 0.0        // pan for this voice (-1=left, 0=center, 1=right)
        //! Stereo pan (-1 = left, 0 = center, 1 = right).
    reverb_send : float = 0.0  // reverb send amount from Event.room (0.0–1.0)
        //! Reverb send amount (0.0–1.0) from Event.room.
    chorus_send : float = 0.0  // chorus send amount from Event.chorus (0.0–1.0)
        //! Chorus send amount (0.0–1.0) from Event.chorus.
    delay_send : float = 0.0   // delay send amount from Event.delay_amount (0.0–1.0)
        //! Delay send amount (0.0–1.0) from Event.delay_amount.
    orbit : int = 0            // effect bus routing
        //! Orbit index — selects which per-orbit effect bus receives this voice's sends.
    hrtf_active    : bool  = false   // HRTF positional override
        //! True when this voice uses HRTF positioning instead of equal-power stereo pan.
    hrtf_azimuth   : float = 0.0
        //! HRTF azimuth in degrees (-180..180; 0 = ahead).
    hrtf_elevation : float = 0.0
        //! HRTF elevation in degrees (-90..90; 0 = horizontal).
    hrtf_l         : ma_hrtf?
        //! HRTF state for the left input channel (virtual source at azimuth - 30°); allocated lazily.
    hrtf_r         : ma_hrtf?
        //! HRTF state for the right input channel (virtual source at azimuth + 30°); allocated lazily.
}

//! Per-voice metadata for SF2 voices (companion to the C ma_sf2_voice struct).
[safe_when_uninitialized]
struct StrudelSF2Meta {
    note             : int = 0       // MIDI note (for re-trigger matching)
        //! MIDI note number (used for re-trigger matching).
    note_off_sample  : int = -1      // sample position to fire note-off (-1 = don't auto note-off)
        //! Sample index at which to fire note-off (-1 = never auto note-off).
    samples_elapsed  : int = 0       // how many samples rendered so far
        //! Total samples rendered for this voice so far.
    reverb_send      : float = 0.0   // per-voice reverb amount from SF2 generators
        //! Per-voice reverb send (from SF2 generators).
    chorus_send      : float = 0.0   // per-voice chorus amount from SF2 generators
        //! Per-voice chorus send (from SF2 generators).
    delay_send       : float = 0.0   // per-voice delay amount from Event.delay_amount
        //! Per-voice delay send (from Event.delay_amount).
    exclusive_class  : int = 0       // SF2 exclusive class (hi-hat choke etc.)
        //! SF2 exclusive class — voices in the same non-zero class choke each other (e.g. hi-hats).
    cut              : int = 0       // strudel cut group
        //! Strudel cut group (0 = no cut).
    gain             : float = 1.0   // event gain * velocity (applied as attenuation scale)
        //! Event gain × velocity, applied as an attenuation scale factor.
    orbit            : int = 0       // effect bus routing
        //! Orbit index — selects which per-orbit effect bus receives this voice's sends.
    @safe_when_uninitialized fx : VoiceFX  // per-voice audio effects
        //! Per-voice audio effects (crush/shape/filter/etc.), applied post-render.
    pan              : float = 0.0   // stereo pan (-1..1) used when fx routes through temp buffer
        //! Stereo pan (-1..1), used when FX routing goes through a temp buffer.
    hrtf_active      : bool  = false // HRTF positional override
        //! True when this SF2 voice uses HRTF positioning. Pan still controls stereo width / image.
    hrtf_azimuth     : float = 0.0
        //! HRTF azimuth in degrees (-180..180; 0 = ahead).
    hrtf_elevation   : float = 0.0
        //! HRTF elevation in degrees (-90..90; 0 = horizontal).
    hrtf_l           : ma_hrtf?
        //! HRTF state for the left input channel (virtual source at azimuth - 30°); allocated lazily.
    hrtf_r           : ma_hrtf?
        //! HRTF state for the right input channel (virtual source at azimuth + 30°); allocated lazily.
}

//! Per-orbit effect bus. Each orbit has independent reverb + delay + chorus instances.
//! Orbits are created on demand when a voice with that orbit index is spawned;
//! each FX instance within the bus is allocated lazily on first send.
[safe_when_uninitialized]
struct OrbitBus {
    reverb            : ConvolutionReverb?
        //! Convolution reverb instance (null until a voice requests reverb).
    delay_proc        : ma_delay?
        //! Stereo delay instance (null until a voice requests delay).
    chorus            : ma_chorus?
        //! Chorus instance (null until a voice requests chorus).
    reverb_send_buf   : array<float>
        //! Scratch buffer accumulating reverb sends from all voices routed to this orbit.
    reverb_out_buf    : array<float>
        //! Scratch buffer receiving the reverb's wet output for mixing into the master.
    delay_send_buf    : array<float>
        //! Scratch buffer accumulating delay sends from all voices routed to this orbit.
    delay_out_buf     : array<float>
        //! Scratch buffer receiving the delay's wet output for mixing into the master.
    chorus_send_buf   : array<float>
        //! Scratch buffer accumulating chorus sends from all voices routed to this orbit.
    chorus_out_buf    : array<float>
        //! Scratch buffer receiving the chorus's wet output for mixing into the master.
    current_roomsize      : float = -1.0
        //! Most recently applied reverb roomsize (-1 = uninitialized).
    current_delaytime     : float = -1.0
        //! Most recently applied delay time in seconds (-1 = uninitialized).
    current_delayfeedback : float = -1.0
        //! Most recently applied delay feedback amount (-1 = uninitialized).
}

//! Bridges the pattern engine and audio output.
//! Maintains three voice pools (pre-rendered samples, streaming oscillators, real-time SF2)
//! and routes each voice's sends through per-orbit effect buses (independent reverb / delay / chorus per orbit).
[safe_when_uninitialized]
struct Scheduler {
    cps          : double = 0.5lf   // cycles per second (0.5 = 120 BPM at 4 beats/cycle)
        //! Cycles per second (0.5 = 120 BPM at 4 beats per cycle).
    startTime    : double = 0.0lf   // wall-clock time when playback started
        //! Wall-clock time when playback started (seconds).
    lastQueryEnd : double = 0.0lf   // cycle position of last query end
        //! Cycle position of the last pattern-query end, used to avoid duplicate spawning.
    voices       : array<Voice?>    // pre-rendered sample/oscillator voices
        //! Pool of pre-rendered sample voices.
    osc_voices   : array<OscVoice> // streaming oscillator voices
        //! Pool of streaming oscillator voices.
    mixer        : ma_volume_mixer  // reused across ticks
        //! Miniaudio volume mixer, reused across ticks for gain/pan application.
    mixer_ready  : bool = false
        //! True once the mixer has been initialized.
    output       : array<float>    // reusable output buffer (avoids per-tick allocation)
        //! Reusable stereo output buffer for the current tick.
    // SF2 real-time voice pool
    sf2           : SF2File const?        // loaded soundfont (null = SF2 disabled)
        //! Loaded SoundFont (null = SF2 disabled for this scheduler).
    sf2_voices    : array<ma_sf2_voice>   // real-time SF2 voices (C structs)
        //! Pool of real-time SF2 voices (C structs).
    sf2_meta      : array<StrudelSF2Meta> // parallel metadata
        //! Parallel metadata array for each SF2 voice.
    // Per-orbit effect buses (created on demand)
    orbit_buses   : array<OrbitBus?>
        //! Per-orbit effect buses, indexed by orbit number and allocated on demand.
    // Shared scratch buffer for per-voice FX processing (sized per-tick)
    voice_fx_buf    : array<float>
        //! Scratch buffer for per-voice FX processing, sized on demand each tick.
    // Shared HRTF scratch buffers, sized per-tick when any HRTF voice is active.
    // Stereo input is split L/R; each channel runs through its own HRTF instance and the two
    // stereo outputs are summed to produce a binaural-stereo virtual-speaker render.
    hrtf_mono_buf      : array<float>
        //! Mono input scratch for HRTF processing; one element per frame.
    hrtf_stereo_buf    : array<float>
        //! Stereo output scratch (left-channel HRTF result); interleaved L/R per frame.
    hrtf_stereo_buf_r  : array<float>
        //! Stereo output scratch (right-channel HRTF result); interleaved L/R per frame.
}

// Convert wall-clock time to cycle position.
def private current_cycle(sched : Scheduler; wall_time : double) : double {
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
def private init_orbit_reverb(var bus : OrbitBus; roomsize : float = 2.0) {
    if (bus.reverb == null) {
        let rs = clamp(roomsize, 0.1, 10.0)
        bus.reverb = new ConvolutionReverb
        conv_reverb_init(bus.reverb, SAMPLE_RATE, rs, 15000.0, 1000.0, 0.1)
        bus.current_roomsize = rs
    }
}

// Update reverb decay time on an orbit bus. Only reconfigures if changed.
def private update_orbit_reverb_roomsize(var bus : OrbitBus; roomsize : float) {
    let rs = clamp(roomsize, 0.1, 10.0)
    if (abs(rs - bus.current_roomsize) < 0.01) {
        return
    }
    bus.current_roomsize = rs
    // Reinitialize with new decay time
    conv_reverb_uninit(bus.reverb)
    conv_reverb_init(bus.reverb, SAMPLE_RATE, rs, 15000.0, 1000.0, 0.1)
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

def init_reverb(var sched : Scheduler) {
    //! Initialize reverb on the scheduler's default orbit (bus 0). Convenience wrapper used by older API.
    ensure_orbit(sched, 0)
    init_orbit_reverb(*sched.orbit_buses[0])
}

// Initialize chorus on an orbit bus (no-op if already initialized).
def private init_orbit_chorus(var bus : OrbitBus) {
    if (bus.chorus == null) {
        bus.chorus = new ma_chorus
        chorus_init(bus.chorus, float(SAMPLE_RATE))
        let cfg = chorus_config_default()
        chorus_set_config(bus.chorus, cfg)
    }
}

def init_delay(var sched : Scheduler; delaytime : float = 0.5; delayfeedback : float = 0.5) {
    //! Initialize delay on the scheduler's default orbit (bus 0) with the given time/feedback.
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
        let dt = resolve_delaytime(event.delaytime, event.delaysync, float(sched.cps))
        if (bus.delay_proc == null) {
            init_orbit_delay(*bus, dt, event.delayfeedback)
        } else {
            update_orbit_delay_params(*bus, dt, event.delayfeedback)
        }
    }
    if (event.chorus > 0.0) {
        init_orbit_chorus(*bus)
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
    var zones <- sf2_find_zones(*sched.sf2, preset_idx, note, velocity)
    if (length(zones) == 0) {
        delete zones
        return
    }
    let preset = sched.sf2.presets[preset_idx]
    let note_off_sample = int(durSec * float(SAMPLE_RATE))
    let cutGroup = event.cut
    let eventGain = event.gain * event.velocity
    var atten_state = SF2VoiceAttenState()
    var cc_defaults : int[128]
    sf2_default_cc_values(cc_defaults)
    // per-event CC overrides from pattern combinators
    if (event.sf2_expression >= 0.0) { cc_defaults[11] = clamp(int(event.sf2_expression * 127.0), 0, 127); }
    if (event.sf2_mod_wheel >= 0.0)  { cc_defaults[1]  = clamp(int(event.sf2_mod_wheel * 127.0), 0, 127); }
    if (event.sf2_pitch_bend >= 0.0) { cc_defaults[127] = clamp(int(event.sf2_pitch_bend * 16383.0), 0, 16383); }
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
        delete atten_state.atten_mods  // free previous iteration's mods (<- is memcpy+memzero, not finalize)
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
            let csend = max(sf2_get_chorus_send(*sched.sf2, z.x, z.y, pzone, preset.global_zone), event.chorus)
            if (csend > 0.0 && sched.orbit_buses[event.orbit].chorus == null) {
                init_orbit_chorus(*sched.orbit_buses[event.orbit])
            }
            sched.sf2_voices |> push(voice)
            var meta = StrudelSF2Meta(
                note = note,
                note_off_sample = note_off_sample,
                reverb_send = rsend,
                chorus_send = csend,
                delay_send = event_delay_send,
                exclusive_class = excl,
                cut = cutGroup,
                gain = eventGain,
                orbit = event.orbit,
                pan = event.pan * 2.0 - 1.0,
                hrtf_active = event.hrtf_active,
                hrtf_azimuth = event.hrtf_azimuth,
                hrtf_elevation = event.hrtf_elevation
            )
            fx_init_from_event(meta.fx, event, float(SAMPLE_RATE))
            sched.sf2_meta |> push(meta)
        }
    }
    delete atten_state.atten_mods
    delete zones
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

// Get the chorus send buffer for a given orbit. Returns null if no chorus on that orbit.
def private get_orbit_chorus_send(var sched : Scheduler; orbit : int) : array<float>? {
    if (orbit < length(sched.orbit_buses) && sched.orbit_buses[orbit] != null && sched.orbit_buses[orbit].chorus != null) {
        return unsafe(addr(sched.orbit_buses[orbit].chorus_send_buf))
    }
    return null
}

// Ensure the scheduler-level HRTF mono / stereo scratch buffers are sized for a tick of the given length.
def private prepare_hrtf_scratch(var sched : Scheduler; chunkFrames : int) {
    let chunkSamples_ = chunkFrames * 2
    if (length(sched.hrtf_mono_buf) < chunkFrames) {
        sched.hrtf_mono_buf |> resize(chunkFrames)
    }
    if (length(sched.hrtf_stereo_buf) < chunkSamples_) {
        sched.hrtf_stereo_buf |> resize(chunkSamples_)
    }
    if (length(sched.hrtf_stereo_buf_r) < chunkSamples_) {
        sched.hrtf_stereo_buf_r |> resize(chunkSamples_)
    }
}

// Render a stereo voice contribution through the binaural-stereo HRTF.
// Treats the L input channel as a virtual source at (azimuth - 30°) and the R input channel
// as a virtual source at (azimuth + 30°), runs each through its own HRTF instance, and sums
// the two stereo outputs into `sched.hrtf_stereo_buf`. This makes pan and HRTF orthogonal:
// pan controls the inherent stereo width / image of the source, HRTF positions it in 3D.
// Lazily creates both HRTF instances on first use.
def private hrtf_render_voice(
                              var sched : Scheduler;
                              var voice_hrtf_l : ma_hrtf?&;// L-channel HRTF — virtual source at az - 30°
                              var voice_hrtf_r : ma_hrtf?&;// R-channel HRTF — virtual source at az + 30°
                              azimuth, elevation : float;
                              var stereo_in : array<float>;
                              src_start : int;// source offset (interleaved sample index)
                              voice_gain : float;
                              chunkFrames : int
                              ) {
    if (voice_hrtf_l == null) {
        voice_hrtf_l = new ma_hrtf
        ma_hrtf_init(voice_hrtf_l, uint(SAMPLE_RATE))
    }
    if (voice_hrtf_r == null) {
        voice_hrtf_r = new ma_hrtf
        ma_hrtf_init(voice_hrtf_r, uint(SAMPLE_RATE))
    }
    let chunkSamples = chunkFrames * 2
    let limit = min(chunkSamples, length(stereo_in) - src_start)
    let nf = max(0, limit / 2)
    // L channel at (azimuth - 30°): build mono L into hrtf_mono_buf, process into hrtf_stereo_buf.
    ma_hrtf_set_direction(voice_hrtf_l, int(azimuth - 30.0), int(elevation))
    for (f in range(nf)) {
        let si = src_start + f * 2
        sched.hrtf_mono_buf[f] = stereo_in[si] * voice_gain
    }
    for (f in range(nf, chunkFrames)) {
        sched.hrtf_mono_buf[f] = 0.0
    }
    unsafe {
        ma_hrtf_process_frames(addr(*voice_hrtf_l),
            addr(sched.hrtf_stereo_buf[0]),
            addr(sched.hrtf_mono_buf[0]),
            1u,
            uint(chunkFrames))
    }
    // R channel at (azimuth + 30°): build mono R into hrtf_mono_buf, process into hrtf_stereo_buf_r.
    ma_hrtf_set_direction(voice_hrtf_r, int(azimuth + 30.0), int(elevation))
    for (f in range(nf)) {
        let si = src_start + f * 2
        sched.hrtf_mono_buf[f] = stereo_in[si + 1] * voice_gain
    }
    for (f in range(nf, chunkFrames)) {
        sched.hrtf_mono_buf[f] = 0.0
    }
    unsafe {
        ma_hrtf_process_frames(addr(*voice_hrtf_r),
            addr(sched.hrtf_stereo_buf_r[0]),
            addr(sched.hrtf_mono_buf[0]),
            1u,
            uint(chunkFrames))
    }
    // Sum the right-channel HRTF output into hrtf_stereo_buf so the caller sees one combined buffer.
    for (s in range(chunkSamples)) {
        sched.hrtf_stereo_buf[s] += sched.hrtf_stereo_buf_r[s]
    }
}

// Dispatch an osc voice to osc_render_chunk, picking each per-orbit send buffer when present
// and falling back to a caller-provided empty array otherwise (osc_render_chunk bounds-checks each).
def private osc_render_chunk_routed(
                                    var voice : OscVoice;
                                    var output : array<float>;
                                    var reverb_buf : array<float>?;
                                    var delay_buf : array<float>?;
                                    var chorus_buf : array<float>?;
                                    var empty_r : array<float>;
                                    var empty_d : array<float>;
                                    var empty_c : array<float>;
                                    chunkFrames : int
                                    ) {
    if (reverb_buf != null) {
        if (delay_buf != null) {
            if (chorus_buf != null) {
                osc_render_chunk(voice, output, *reverb_buf, *delay_buf, *chorus_buf, chunkFrames)
            } else {
                osc_render_chunk(voice, output, *reverb_buf, *delay_buf, empty_c, chunkFrames)
            }
        } else {
            if (chorus_buf != null) {
                osc_render_chunk(voice, output, *reverb_buf, empty_d, *chorus_buf, chunkFrames)
            } else {
                osc_render_chunk(voice, output, *reverb_buf, empty_d, empty_c, chunkFrames)
            }
        }
    } else {
        if (delay_buf != null) {
            if (chorus_buf != null) {
                osc_render_chunk(voice, output, empty_r, *delay_buf, *chorus_buf, chunkFrames)
            } else {
                osc_render_chunk(voice, output, empty_r, *delay_buf, empty_c, chunkFrames)
            }
        } else {
            if (chorus_buf != null) {
                osc_render_chunk(voice, output, empty_r, empty_d, *chorus_buf, chunkFrames)
            } else {
                osc_render_chunk(voice, output, empty_r, empty_d, empty_c, chunkFrames)
            }
        }
    }
}

// Render all active SF2 voices into the output buffer. Handles note-off scheduling,
// reverb/chorus/delay send, and voice cleanup.
// Reverb, delay, and chorus sends all accumulate into per-orbit bus buffers.
def private sf2_render_voices(var sched : Scheduler; var output : array<float>; chunkFrames : int) {
    if (length(sched.sf2_voices) == 0 || sched.sf2 == null) {
        return
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
            if (meta.hrtf_active) {
                // HRTF path: render dry into voice_fx_buf, optionally apply per-voice FX,
                // then HRTF the result and accumulate stereo into output + per-orbit sends.
                let chunkSamples = chunkFrames * 2
                if (length(sched.voice_fx_buf) < chunkSamples) {
                    sched.voice_fx_buf |> resize(chunkSamples)
                }
                for (s in range(chunkSamples)) {
                    sched.voice_fx_buf[s] = 0.0
                }
                unsafe {
                    ma_sf2_voice_render(
                        addr(sched.sf2_voices[si]),
                        addr(sched.sf2.sample_data[0]),
                        length(sched.sf2.sample_data),
                        addr(sched.voice_fx_buf[0]),
                        0, chunkFrames
                    )
                }
                if (meta.fx.active) {
                    fx_apply(sched.sf2_meta[si].fx, sched.voice_fx_buf, chunkFrames)
                }
                prepare_hrtf_scratch(sched, chunkFrames)
                hrtf_render_voice(sched, sched.sf2_meta[si].hrtf_l, sched.sf2_meta[si].hrtf_r,
                    meta.hrtf_azimuth, meta.hrtf_elevation,
                    sched.voice_fx_buf, 0, 1.0, chunkFrames)
                var reverb_buf = get_orbit_reverb_send(sched, meta.orbit)
                var delay_buf = get_orbit_delay_send(sched, meta.orbit)
                var chorus_buf = get_orbit_chorus_send(sched, meta.orbit)
                let mix_reverb = reverb_buf != null && reverb_send > 0.001
                let mix_delay  = delay_buf != null && delay_send_amt > 0.001
                let mix_chorus = chorus_buf != null && chorus_send > 0.001
                let dry_gain = has_sends ? max(0.0, 1.0 - reverb_send - chorus_send) : 1.0
                for (s in range(chunkSamples)) {
                    let v = sched.hrtf_stereo_buf[s]
                    output[s] += v * dry_gain
                    if (mix_reverb) { (*reverb_buf)[s] += v * reverb_send; }
                    if (mix_delay)  { (*delay_buf)[s]  += v * delay_send_amt; }
                    if (mix_chorus) { (*chorus_buf)[s] += v * chorus_send; }
                }
            } elif (meta.fx.active) {
                // FX path: render dry into scratch, apply effects, then mix post-FX
                // into output + sends. Uses the same temp buffer as osc voices.
                let chunkSamples = chunkFrames * 2
                if (length(sched.voice_fx_buf) < chunkSamples) {
                    sched.voice_fx_buf |> resize(chunkSamples)
                }
                for (s in range(chunkSamples)) {
                    sched.voice_fx_buf[s] = 0.0
                }
                unsafe {
                    ma_sf2_voice_render(
                        addr(sched.sf2_voices[si]),
                        addr(sched.sf2.sample_data[0]),
                        length(sched.sf2.sample_data),
                        addr(sched.voice_fx_buf[0]),
                        0, chunkFrames
                    )
                }
                fx_apply(sched.sf2_meta[si].fx, sched.voice_fx_buf, chunkFrames)
                var reverb_buf = get_orbit_reverb_send(sched, meta.orbit)
                var delay_buf = get_orbit_delay_send(sched, meta.orbit)
                var chorus_buf = get_orbit_chorus_send(sched, meta.orbit)
                let mix_reverb = reverb_buf != null && reverb_send > 0.001
                let mix_delay  = delay_buf != null && delay_send_amt > 0.001
                let mix_chorus = chorus_buf != null && chorus_send > 0.001
                let dry_gain = has_sends ? max(0.0, 1.0 - reverb_send - chorus_send) : 1.0
                for (s in range(chunkSamples)) {
                    let v = sched.voice_fx_buf[s]
                    output[s] += v * dry_gain
                    if (mix_reverb) { (*reverb_buf)[s] += v * reverb_send; }
                    if (mix_delay)  { (*delay_buf)[s]  += v * delay_send_amt; }
                    if (mix_chorus) { (*chorus_buf)[s] += v * chorus_send; }
                }
            } elif (has_sends) {
                var reverb_buf = get_orbit_reverb_send(sched, meta.orbit)
                var delay_buf = get_orbit_delay_send(sched, meta.orbit)
                var chorus_buf = get_orbit_chorus_send(sched, meta.orbit)
                let has_reverb = reverb_buf != null && reverb_send > 0.001
                let has_delay = delay_buf != null && delay_send_amt > 0.001
                let has_chorus = chorus_buf != null && chorus_send > 0.001
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
                                addr((*chorus_buf)[0]),
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
                                addr((*chorus_buf)[0]),
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
                                addr((*chorus_buf)[0]),
                                0, chunkFrames,
                                max(0.0, 1.0 - chorus_send), chorus_send
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
            if (sched.sf2_meta[si].hrtf_l != null) {
                ma_hrtf_uninit(sched.sf2_meta[si].hrtf_l)
                unsafe { delete sched.sf2_meta[si].hrtf_l; }
                sched.sf2_meta[si].hrtf_l = null
            }
            if (sched.sf2_meta[si].hrtf_r != null) {
                ma_hrtf_uninit(sched.sf2_meta[si].hrtf_r)
                unsafe { delete sched.sf2_meta[si].hrtf_r; }
                sched.sf2_meta[si].hrtf_r = null
            }
            sched.sf2_voices |> erase(si)
            sched.sf2_meta |> erase(si)
        }
        si --
    }
}

// Render all active streaming oscillator voices into the output buffer.
// Sends (reverb / delay / chorus) accumulate into per-orbit bus buffers based on each voice's orbit.
// When a voice has active FX, render dry (no sends) into voice_fx_buf,
// apply effects, then mix post-FX into output + sends.
def private osc_render_voices(var sched : Scheduler; var output : array<float>; chunkFrames : int) {
    let chunkSamples = chunkFrames * 2
    var i = length(sched.osc_voices) - 1
    while (i >= 0) {
        let orb = sched.osc_voices[i].orbit
        let has_fx = sched.osc_voices[i].fx.active
        let hrtf_active = sched.osc_voices[i].hrtf_active
        var reverb_buf : array<float>?
        var delay_buf : array<float>?
        var chorus_buf : array<float>?
        if (sched.osc_voices[i].reverb_send > 0.0) {
            reverb_buf = get_orbit_reverb_send(sched, orb)
        }
        if (sched.osc_voices[i].delay_send > 0.0) {
            delay_buf = get_orbit_delay_send(sched, orb)
        }
        if (sched.osc_voices[i].chorus_send > 0.0) {
            chorus_buf = get_orbit_chorus_send(sched, orb)
        }
        if (hrtf_active) {
            // HRTF: render dry into voice_fx_buf (no sends), optionally apply per-voice FX,
            // then HRTF the result and add the stereo HRTF output into master + per-orbit sends.
            if (length(sched.voice_fx_buf) < chunkSamples) {
                sched.voice_fx_buf |> resize(chunkSamples)
            }
            for (s in range(chunkSamples)) {
                sched.voice_fx_buf[s] = 0.0
            }
            var empty1 : array<float>
            var empty2 : array<float>
            var empty3 : array<float>
            osc_render_chunk(sched.osc_voices[i], sched.voice_fx_buf, empty1, empty2, empty3, chunkFrames)
            if (has_fx) {
                fx_apply(sched.osc_voices[i].fx, sched.voice_fx_buf, chunkFrames)
            }
            prepare_hrtf_scratch(sched, chunkFrames)
            // gain + pan are already baked into voice_fx_buf via osc_render_chunk's lGain/rGain.
            // Pan now controls source width (L/R virtual-speaker spread); HRTF positions in 3D.
            hrtf_render_voice(sched, sched.osc_voices[i].hrtf_l, sched.osc_voices[i].hrtf_r,
                sched.osc_voices[i].hrtf_azimuth, sched.osc_voices[i].hrtf_elevation,
                sched.voice_fx_buf, 0, 1.0, chunkFrames)
            let rev_send = sched.osc_voices[i].reverb_send
            let del_send = sched.osc_voices[i].delay_send
            let cho_send = sched.osc_voices[i].chorus_send
            let mix_reverb = reverb_buf != null && rev_send > 0.0
            let mix_delay  = delay_buf != null && del_send > 0.0
            let mix_chorus = chorus_buf != null && cho_send > 0.0
            for (s in range(chunkSamples)) {
                let v = sched.hrtf_stereo_buf[s]
                output[s] += v
                if (mix_reverb) { (*reverb_buf)[s] += v * rev_send; }
                if (mix_delay)  { (*delay_buf)[s]  += v * del_send; }
                if (mix_chorus) { (*chorus_buf)[s] += v * cho_send; }
            }
        } elif (has_fx) {
            // route through scratch buffer: render dry (no sends), apply FX, mix into output + sends
            if (length(sched.voice_fx_buf) < chunkSamples) {
                sched.voice_fx_buf |> resize(chunkSamples)
            }
            for (s in range(chunkSamples)) {
                sched.voice_fx_buf[s] = 0.0
            }
            var empty1 : array<float>
            var empty2 : array<float>
            var empty3 : array<float>
            osc_render_chunk(sched.osc_voices[i], sched.voice_fx_buf, empty1, empty2, empty3, chunkFrames)
            fx_apply(sched.osc_voices[i].fx, sched.voice_fx_buf, chunkFrames)
            let rev_send = sched.osc_voices[i].reverb_send
            let del_send = sched.osc_voices[i].delay_send
            let cho_send = sched.osc_voices[i].chorus_send
            let mix_reverb = reverb_buf != null && rev_send > 0.0
            let mix_delay  = delay_buf != null && del_send > 0.0
            let mix_chorus = chorus_buf != null && cho_send > 0.0
            for (s in range(chunkSamples)) {
                let v = sched.voice_fx_buf[s]
                output[s] += v
                if (mix_reverb) { (*reverb_buf)[s] += v * rev_send; }
                if (mix_delay)  { (*delay_buf)[s]  += v * del_send; }
                if (mix_chorus) { (*chorus_buf)[s] += v * cho_send; }
            }
        } else {
            // osc_render_chunk needs non-null array refs — pass per-orbit buffers when present,
            // empty locals otherwise (osc_render_chunk bounds-checks each).
            var empty_r : array<float>
            var empty_d : array<float>
            var empty_c : array<float>
            osc_render_chunk_routed(sched.osc_voices[i], output, reverb_buf, delay_buf, chorus_buf, empty_r, empty_d, empty_c, chunkFrames)
        }
        if (sched.osc_voices[i].finished) {
            if (sched.osc_voices[i].hrtf_l != null) {
                ma_hrtf_uninit(sched.osc_voices[i].hrtf_l)
                unsafe { delete sched.osc_voices[i].hrtf_l; }
                sched.osc_voices[i].hrtf_l = null
            }
            if (sched.osc_voices[i].hrtf_r != null) {
                ma_hrtf_uninit(sched.osc_voices[i].hrtf_r)
                unsafe { delete sched.osc_voices[i].hrtf_r; }
                sched.osc_voices[i].hrtf_r = null
            }
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
        if (bus.chorus != null) {
            if (length(bus.chorus_send_buf) < chunkSamples) {
                bus.chorus_send_buf |> resize(chunkSamples)
                bus.chorus_out_buf |> resize(chunkSamples)
            }
            for (s in range(chunkSamples)) {
                bus.chorus_send_buf[s] = 0.0
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
        // Always process reverb when it exists — convolver needs continuous input
        if (bus.reverb != null) {
            for (s in range(chunkSamples)) {
                bus.reverb_out_buf[s] = 0.0
            }
            unsafe {
                conv_reverb_process(bus.reverb, addr(bus.reverb_send_buf[0]), addr(bus.reverb_out_buf[0]), chunkFrames)
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
        // Process chorus when it exists — gated by send activity since chorus is delay-line based and dry input is silent
        if (bus.chorus != null) {
            var has_send = false
            for (s in range(chunkSamples)) {
                if (abs(bus.chorus_send_buf[s]) > 0.00001) {
                    has_send = true
                    break
                }
            }
            if (has_send) {
                for (s in range(chunkSamples)) {
                    bus.chorus_out_buf[s] = 0.0
                }
                unsafe {
                    chorus_process(bus.chorus, addr(bus.chorus_send_buf[0]), addr(bus.chorus_out_buf[0]), chunkFrames)
                }
                for (s in range(chunkSamples)) {
                    output[s] += bus.chorus_out_buf[s]
                }
            }
        }
    }
}

def tick(var sched : Scheduler; var pat : Pattern; bank : SampleBank; wall_time : double; chunk_seconds : float = 0.05; lookAhead : float = 0.1) : void {
    //! Advance one audio block: query the pattern for new events, spawn voices, mix all voices through the per-orbit reverb/delay/chorus buses into sched.output.
    //! chunk_seconds controls how much audio is produced this tick; lookAhead is how far ahead in time to query for new events.
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
                            if (sched.voices[k].hrtf_l != null) {
                                ma_hrtf_uninit(sched.voices[k].hrtf_l)
                                unsafe { delete sched.voices[k].hrtf_l; }
                                sched.voices[k].hrtf_l = null
                            }
                            if (sched.voices[k].hrtf_r != null) {
                                ma_hrtf_uninit(sched.voices[k].hrtf_r)
                                unsafe { delete sched.voices[k].hrtf_r; }
                                sched.voices[k].hrtf_r = null
                            }
                            unsafe { delete sched.voices[k]; }
                            sched.voices |> erase(k)
                        }
                        k --
                    }
                    var ko = length(sched.osc_voices) - 1
                    while (ko >= 0) {
                        if (sched.osc_voices[ko].cut == cutGroup) {
                            if (sched.osc_voices[ko].hrtf_l != null) {
                                ma_hrtf_uninit(sched.osc_voices[ko].hrtf_l)
                                unsafe { delete sched.osc_voices[ko].hrtf_l; }
                                sched.osc_voices[ko].hrtf_l = null
                            }
                            if (sched.osc_voices[ko].hrtf_r != null) {
                                ma_hrtf_uninit(sched.osc_voices[ko].hrtf_r)
                                unsafe { delete sched.osc_voices[ko].hrtf_r; }
                                sched.osc_voices[ko].hrtf_r = null
                            }
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
                    let adsr = resolve_adsr(h.value.attack, h.value.decay, h.value.sustain, h.value.release)
                    var oscv = OscVoice(
                        osc_type = oscType,
                        freq = note_to_freq(h.value.note),
                        duration = durSec,
                        attack = adsr._0,
                        decay = adsr._1,
                        sustain = adsr._2,
                        release_sec = adsr._3,
                        gain = h.value.gain * h.value.velocity,
                        pan = h.value.pan * 2.0 - 1.0,
                        cut = cutGroup,
                        offset_frames = offsetFrames,
                        fm_depth = h.value.fm,
                        fm_harmonicity = h.value.fmh,
                        reverb_send = h.value.room,
                        chorus_send = h.value.chorus,
                        delay_send = h.value.delay_amount,
                        orbit = h.value.orbit,
                        hrtf_active = h.value.hrtf_active,
                        hrtf_azimuth = h.value.hrtf_azimuth,
                        hrtf_elevation = h.value.hrtf_elevation
                    )
                    osc_voice_init_filters(oscv, h.value.lpf, h.value.hpf, h.value.lpq, h.value.hpq)
                    if (!empty(h.value.vowel)) {
                        vowel_filter_init(oscv.vowel_filter, h.value.vowel)
                    }
                    if (oscType == OscType.osc_supersaw) {
                        osc_voice_init_supersaw(oscv)
                    }
                    fx_init_from_event(oscv.fx, h.value, float(SAMPLE_RATE))
                    sched.osc_voices |> push(oscv)
                } else {
                    // sample/drum path: pre-render to buffer
                    var stereo <- render_event_stereo(h.value, durSec, bank)
                    // per-voice effects (crush/coarse/shape/djf/bpf) are applied
                    // once post-render; state is local to this call
                    fx_apply_to_samples(h.value, stereo, float(SAMPLE_RATE))
                    let offsetSamples = max(int(offsetSec * float(SAMPLE_RATE)) * 2, 0)
                    sched.voices |> push(new Voice(
                        samples <- stereo,
                        position = -offsetSamples,
                        cut = cutGroup,
                        gain = h.value.gain * h.value.velocity,
                        pan = h.value.pan * 2.0 - 1.0,
                        reverb_send = h.value.room,
                        chorus_send = h.value.chorus,
                        delay_send = h.value.delay_amount,
                        orbit = h.value.orbit,
                        hrtf_active = h.value.hrtf_active,
                        hrtf_azimuth = h.value.hrtf_azimuth,
                        hrtf_elevation = h.value.hrtf_elevation
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
            var reverb_buf = get_orbit_reverb_send(sched, voice.orbit)
            var delay_buf = get_orbit_delay_send(sched, voice.orbit)
            var chorus_buf = get_orbit_chorus_send(sched, voice.orbit)
            let nf = count / 2
            if (voice.hrtf_active) {
                // HRTF-positioned dry path: each input channel becomes a virtual source at
                // azimuth -/+ 30°; the two HRTF outputs sum to a binaural-stereo render.
                // Sends are also spatialised — the resulting stereo is reused for output and per-orbit sends.
                // Pass `nf` (not chunkFrames): the per-voice HRTF state must advance only over the
                // frames actually emitted this tick, otherwise leading-silence delays or end-of-sample
                // truncation would feed silent frames through the filter and skew its history.
                // Bake equal-power pan + voice gain into voice_fx_buf before HRTF (consistent with the
                // osc/sf2 paths). `voice.samples` is raw PCM with no pan/gain applied, so without this
                // step pan(...) would be silently ignored on the sample-voice HRTF path.
                let chunk_samples = nf * 2
                if (length(sched.voice_fx_buf) < chunk_samples) {
                    sched.voice_fx_buf |> resize(chunk_samples)
                }
                let pan = clamp(voice.pan, -1.0, 1.0)
                let angle = (pan + 1.0) * 0.25 * float(TWO_PI / 2.0)
                let lGain = voice.gain * cos(angle)
                let rGain = voice.gain * sin(angle)
                for (f in range(nf)) {
                    let si = srcStart + f * 2
                    let di = f * 2
                    sched.voice_fx_buf[di]     = voice.samples[si]     * lGain
                    sched.voice_fx_buf[di + 1] = voice.samples[si + 1] * rGain
                }
                prepare_hrtf_scratch(sched, nf)
                hrtf_render_voice(sched, voice.hrtf_l, voice.hrtf_r, voice.hrtf_azimuth, voice.hrtf_elevation,
                    sched.voice_fx_buf, 0, 1.0, nf)
                let mix_reverb = reverb_buf != null && voice.reverb_send > 0.0
                let mix_delay  = delay_buf  != null && voice.delay_send  > 0.0
                let mix_chorus = chorus_buf != null && voice.chorus_send > 0.0
                for (f in range(nf)) {
                    let di = dstStart + f * 2
                    let hl = sched.hrtf_stereo_buf[f * 2]
                    let hr = sched.hrtf_stereo_buf[f * 2 + 1]
                    sched.output[di]     += hl
                    sched.output[di + 1] += hr
                    if (mix_reverb) {
                        (*reverb_buf)[di]     += hl * voice.reverb_send
                        (*reverb_buf)[di + 1] += hr * voice.reverb_send
                    }
                    if (mix_delay) {
                        (*delay_buf)[di]      += hl * voice.delay_send
                        (*delay_buf)[di + 1]  += hr * voice.delay_send
                    }
                    if (mix_chorus) {
                        (*chorus_buf)[di]     += hl * voice.chorus_send
                        (*chorus_buf)[di + 1] += hr * voice.chorus_send
                    }
                }
            } else {
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
                let pan = clamp(voice.pan, -1.0, 1.0)
                let angle = (pan + 1.0) * 0.25 * float(TWO_PI / 2.0)
                // reverb send for pre-rendered voices
                if (voice.reverb_send > 0.0 && reverb_buf != null) {
                    let sL = voice.gain * voice.reverb_send * cos(angle)
                    let sR = voice.gain * voice.reverb_send * sin(angle)
                    for (f in range(nf)) {
                        let si = srcStart + f * 2
                        let di = dstStart + f * 2
                        (*reverb_buf)[di]     += voice.samples[si]     * sL
                        (*reverb_buf)[di + 1] += voice.samples[si + 1] * sR
                    }
                }
                // delay send for pre-rendered voices
                if (voice.delay_send > 0.0 && delay_buf != null) {
                    let dL = voice.gain * voice.delay_send * cos(angle)
                    let dR = voice.gain * voice.delay_send * sin(angle)
                    for (f in range(nf)) {
                        let si = srcStart + f * 2
                        let di = dstStart + f * 2
                        (*delay_buf)[di]     += voice.samples[si]     * dL
                        (*delay_buf)[di + 1] += voice.samples[si + 1] * dR
                    }
                }
                // chorus send for pre-rendered voices (per-orbit)
                if (voice.chorus_send > 0.0 && chorus_buf != null) {
                    let cL = voice.gain * voice.chorus_send * cos(angle)
                    let cR = voice.gain * voice.chorus_send * sin(angle)
                    for (f in range(nf)) {
                        let si = srcStart + f * 2
                        let di = dstStart + f * 2
                        (*chorus_buf)[di]     += voice.samples[si]     * cL
                        (*chorus_buf)[di + 1] += voice.samples[si + 1] * cR
                    }
                }
            }
        }
        voice.position += chunkSamples
        if (voice.position >= voiceLen) {
            if (sched.voices[i].hrtf_l != null) {
                ma_hrtf_uninit(sched.voices[i].hrtf_l)
                unsafe { delete sched.voices[i].hrtf_l; }
                sched.voices[i].hrtf_l = null
            }
            if (sched.voices[i].hrtf_r != null) {
                ma_hrtf_uninit(sched.voices[i].hrtf_r)
                unsafe { delete sched.voices[i].hrtf_r; }
                sched.voices[i].hrtf_r = null
            }
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

    // process all orbit bus effects (reverb / delay / chorus) and mix into output
    process_orbit_buses(sched, sched.output, chunkFrames)

    // output stays in sched.output — caller reads it directly
}

def get_orbit_reverb(var sched : Scheduler; orbit : int = 0) : ConvolutionReverb? {
    //! Fetch the reverb instance for the given orbit (for inspection or tests). Returns null if no bus or no reverb configured.
    if (orbit < length(sched.orbit_buses) && sched.orbit_buses[orbit] != null) {
        return sched.orbit_buses[orbit].reverb
    }
    return null
}

def get_orbit_delay(var sched : Scheduler; orbit : int = 0) : ma_delay? {
    //! Fetch the delay instance for the given orbit (for inspection or tests). Returns null if no bus or no delay configured.
    if (orbit < length(sched.orbit_buses) && sched.orbit_buses[orbit] != null) {
        return sched.orbit_buses[orbit].delay_proc
    }
    return null
}

def get_orbit_chorus(var sched : Scheduler; orbit : int = 0) : ma_chorus? {
    //! Fetch the chorus instance for the given orbit (for inspection or tests). Returns null if no bus or no chorus configured.
    if (orbit < length(sched.orbit_buses) && sched.orbit_buses[orbit] != null) {
        return sched.orbit_buses[orbit].chorus
    }
    return null
}

def shutdown_scheduler(var sched : Scheduler) {
    //! Release every resource owned by the scheduler: pre-rendered sample voices (each carrying
    //! a large PCM buffer), SF2/oscillator voice pools, per-orbit effect buses (reverb / delay /
    //! chorus) and their scratch buffers, and the per-tick output buffer.
    // pre-rendered sample voices — their `samples` arrays are the biggest leaked allocations
    for (i in range(length(sched.voices))) {
        if (sched.voices[i] != null) {
            if (sched.voices[i].hrtf_l != null) {
                ma_hrtf_uninit(sched.voices[i].hrtf_l)
                unsafe { delete sched.voices[i].hrtf_l; }
                sched.voices[i].hrtf_l = null
            }
            if (sched.voices[i].hrtf_r != null) {
                ma_hrtf_uninit(sched.voices[i].hrtf_r)
                unsafe { delete sched.voices[i].hrtf_r; }
                sched.voices[i].hrtf_r = null
            }
            unsafe { delete sched.voices[i]; }
            sched.voices[i] = null
        }
    }
    delete sched.voices
    // streaming oscillator voices — release any HRTF state
    for (i in range(length(sched.osc_voices))) {
        if (sched.osc_voices[i].hrtf_l != null) {
            ma_hrtf_uninit(sched.osc_voices[i].hrtf_l)
            unsafe { delete sched.osc_voices[i].hrtf_l; }
            sched.osc_voices[i].hrtf_l = null
        }
        if (sched.osc_voices[i].hrtf_r != null) {
            ma_hrtf_uninit(sched.osc_voices[i].hrtf_r)
            unsafe { delete sched.osc_voices[i].hrtf_r; }
            sched.osc_voices[i].hrtf_r = null
        }
    }
    delete sched.osc_voices
    // SF2 voices — release HRTF state from parallel meta
    for (i in range(length(sched.sf2_meta))) {
        if (sched.sf2_meta[i].hrtf_l != null) {
            ma_hrtf_uninit(sched.sf2_meta[i].hrtf_l)
            unsafe { delete sched.sf2_meta[i].hrtf_l; }
            sched.sf2_meta[i].hrtf_l = null
        }
        if (sched.sf2_meta[i].hrtf_r != null) {
            ma_hrtf_uninit(sched.sf2_meta[i].hrtf_r)
            unsafe { delete sched.sf2_meta[i].hrtf_r; }
            sched.sf2_meta[i].hrtf_r = null
        }
    }
    delete sched.sf2_voices
    delete sched.sf2_meta
    // per-orbit buses — free native effect instances then send/output scratch buffers
    for (bus in sched.orbit_buses) {
        if (bus != null) {
            if (bus.delay_proc != null) {
                delay_uninit(bus.delay_proc)
                unsafe { delete bus.delay_proc; }
            }
            if (bus.reverb != null) {
                conv_reverb_uninit(bus.reverb)
                unsafe { delete bus.reverb; }
            }
            if (bus.chorus != null) {
                unsafe { delete bus.chorus; }
                bus.chorus = null
            }
            delete bus.reverb_send_buf
            delete bus.reverb_out_buf
            delete bus.delay_send_buf
            delete bus.delay_out_buf
            delete bus.chorus_send_buf
            delete bus.chorus_out_buf
        }
    }
    for (i in range(length(sched.orbit_buses))) {
        if (sched.orbit_buses[i] != null) {
            unsafe { delete sched.orbit_buses[i]; }
            sched.orbit_buses[i] = null
        }
    }
    delete sched.orbit_buses
    // per-tick scratch buffers (including HRTF mono/stereo)
    delete sched.hrtf_mono_buf
    delete sched.hrtf_stereo_buf
    delete sched.hrtf_stereo_buf_r
    delete sched.voice_fx_buf
    delete sched.output
    // mixer — symmetric teardown to ma_volume_mixer_init in tick()
    if (sched.mixer_ready) {
        ma_volume_mixer_uninit(unsafe(addr(sched.mixer)))
        sched.mixer_ready = false
    }
}

// Set tempo in BPM (assumes 4 beats per cycle).
def private set_bpm(var sched : Scheduler; bpm : double) {
    sched.cps = bpm / 240.0lf
}

// Set cycles per second directly.
def private setcps(var sched : Scheduler; cps : double) {
    sched.cps = cps
}
