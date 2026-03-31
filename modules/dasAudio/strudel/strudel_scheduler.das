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
}

// Per-voice metadata for SF2 voices (not stored in the C ma_sf2_voice struct).
struct StrudelSF2Meta {
    note             : int = 0       // MIDI note (for re-trigger matching)
    note_off_sample  : int = -1      // sample position to fire note-off (-1 = don't auto note-off)
    samples_elapsed  : int = 0       // how many samples rendered so far
    reverb_send      : float = 0.0   // per-voice reverb amount from SF2 generators
    chorus_send      : float = 0.0   // per-voice chorus amount from SF2 generators
    exclusive_class  : int = 0       // SF2 exclusive class (hi-hat choke etc.)
    cut              : int = 0       // strudel cut group
    gain             : float = 1.0   // event gain * velocity (applied as attenuation scale)
}

// The scheduler bridges the pattern engine and audio output.
// Maintains two pools: pre-rendered sample voices and real-time SF2 voices.
[safe_when_uninitialized]
struct Scheduler {
    cps          : double = 0.5lf   // cycles per second (0.5 = 120 BPM at 4 beats/cycle)
    startTime    : double = 0.0lf   // wall-clock time when playback started
    lastQueryEnd : double = 0.0lf   // cycle position of last query end
    voices       : array<Voice?>    // pre-rendered sample/oscillator voices
    mixer        : ma_volume_mixer  // reused across ticks
    mixer_ready  : bool = false
    // SF2 real-time voice pool
    sf2           : SF2File const?        // loaded soundfont (null = SF2 disabled)
    sf2_voices    : array<ma_sf2_voice>   // real-time SF2 voices (C structs)
    sf2_meta      : array<StrudelSF2Meta> // parallel metadata
    reverb        : I3DL2Reverb?          // optional reverb processor
    reverb_send_buf : array<float>        // reverb send bus (interleaved stereo)
    reverb_out_buf  : array<float>        // reverb output buffer
    chorus          : ma_chorus?          // optional chorus processor
    chorus_send_buf : array<float>        // chorus send bus (interleaved stereo)
    chorus_out_buf  : array<float>        // chorus output buffer
}

// Convert wall-clock time to cycle position.
def current_cycle(sched : Scheduler; wall_time : double) : double {
    return (wall_time - sched.startTime) * sched.cps
}

// Initialize reverb on the scheduler (call once before ticking).
def init_reverb(var sched : Scheduler) {
    if (sched.reverb == null) {
        sched.reverb = new I3DL2Reverb
        set_sample_rate(sched.reverb, float(SAMPLE_RATE))
        let props = get_preset(I3DL2Preset.ConcertHall)
        set_properties(sched.reverb, props)
    }
}

// Initialize chorus on the scheduler (call once before ticking).
def init_chorus(var sched : Scheduler) {
    if (sched.chorus == null) {
        sched.chorus = new ma_chorus
        chorus_init(sched.chorus, float(SAMPLE_RATE))
        let cfg = chorus_config_default()
        chorus_set_config(sched.chorus, cfg)
    }
}

// Spawn SF2 voices for an event onset.
def private sf2_spawn_voices(var sched : Scheduler; event : Event; durSec : float) {
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
                exclusive_class = excl,
                cut = cutGroup,
                gain = eventGain
            ))
        }
    }
}

// Render all active SF2 voices into the output buffer. Handles note-off scheduling,
// reverb/chorus send, and voice cleanup.
def private sf2_render_voices(var sched : Scheduler; var output : array<float>; chunkFrames : int) {
    if (length(sched.sf2_voices) == 0 || sched.sf2 == null) {
        return
    }
    let buf_samples = chunkFrames * 2
    // prepare reverb send bus
    var has_reverb_send = false
    if (sched.reverb != null) {
        if (length(sched.reverb_send_buf) < buf_samples) {
            sched.reverb_send_buf |> resize(buf_samples)
            sched.reverb_out_buf |> resize(buf_samples)
        }
        for (s in range(buf_samples)) {
            sched.reverb_send_buf[s] = 0.0
        }
    }
    // prepare chorus send bus
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
            // check if it's time to fire note-off
            let meta & = unsafe(sched.sf2_meta[si])
            if (meta.note_off_sample >= 0 && meta.samples_elapsed >= meta.note_off_sample && sched.sf2_voices[si].released == 0) {
                unsafe { ma_sf2_voice_note_off(addr(sched.sf2_voices[si])); }
            }
            // apply event gain by temporarily scaling attenuation
            let saved_atten = sched.sf2_voices[si].attenuation
            sched.sf2_voices[si].attenuation = saved_atten * meta.gain
            let reverb_send = meta.reverb_send
            let chorus_send = meta.chorus_send
            if ((reverb_send > 0.001 || chorus_send > 0.001) && (sched.reverb != null || sched.chorus != null)) {
                let dry_gain = max(0.0, 1.0 - reverb_send - chorus_send)
                unsafe {
                    ma_sf2_voice_render_send2(
                        addr(sched.sf2_voices[si]),
                        addr(sched.sf2.sample_data[0]),
                        length(sched.sf2.sample_data),
                        addr(output[0]),
                        addr(sched.reverb_send_buf[0]),
                        addr(sched.chorus_send_buf[0]),
                        0,
                        chunkFrames,
                        dry_gain,
                        reverb_send,
                        chorus_send
                    )
                }
                if (reverb_send > 0.001) { has_reverb_send = true; }
                if (chorus_send > 0.001) { has_chorus_send = true; }
            } else {
                unsafe {
                    ma_sf2_voice_render(
                        addr(sched.sf2_voices[si]),
                        addr(sched.sf2.sample_data[0]),
                        length(sched.sf2.sample_data),
                        addr(output[0]),
                        0,
                        chunkFrames
                    )
                }
            }
            // restore attenuation and advance elapsed counter
            sched.sf2_voices[si].attenuation = saved_atten
            sched.sf2_meta[si].samples_elapsed += chunkFrames
        }
        if (sched.sf2_voices[si].finished != 0) {
            sched.sf2_voices |> erase(si)
            sched.sf2_meta |> erase(si)
        }
        si --
    }
    // process reverb send bus and mix into output
    if (has_reverb_send && sched.reverb != null) {
        for (s in range(buf_samples)) {
            sched.reverb_out_buf[s] = 0.0
        }
        unsafe {
            process_stereo(sched.reverb, addr(sched.reverb_send_buf[0]), addr(sched.reverb_out_buf[0]), chunkFrames)
        }
        for (s in range(buf_samples)) {
            output[s] += sched.reverb_out_buf[s]
        }
    }
    // process chorus send bus and mix into output
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

// Tick the scheduler: query pattern for new events, render them,
// mix all active voices into a fixed-size stereo buffer.
//
// chunk_seconds: how much audio to produce this tick (e.g. 0.05 for 50ms)
// lookAhead: how far ahead in time to query for new events (in seconds)
def tick(var sched : Scheduler; var pat : Pattern; bank : SampleBank; wall_time : double; chunk_seconds : float = 0.05; lookAhead : float = 0.1) : array<float> {
    let cycleNow = current_cycle(sched, wall_time)
    let cycleEnd = current_cycle(sched, wall_time + double(lookAhead))
    // Voice spawning cutoff: only spawn voices within one chunk of now.
    // The full query range feeds the pianoroll combinator; we just skip spawning far-future voices.
    let spawnEnd = current_cycle(sched, wall_time + double(chunk_seconds) + 0.01lf)

    // query for new events
    let queryStart = max(cycleNow, sched.lastQueryEnd)
    if (queryStart < cycleEnd) {
        var haps <- pat(TimeSpan(start = queryStart, stop = cycleEnd))
        for (h in haps) {
            // only trigger at onset
            if (h.has_whole && abs(h.whole.start - h.part.start) < 0.0001lf && h.whole.start < spawnEnd) {
                let durCycles = h.whole.stop - h.whole.start
                let durSec = float(durCycles / sched.cps)
                // SF2 path: spawn real-time voices
                if (h.value.sf2_program >= 0 && sched.sf2 != null) {
                    // cut groups: also kill pre-rendered voices in same group
                    let cutGroup = h.value.cut
                    if (cutGroup != 0) {
                        var k = length(sched.voices) - 1
                        while (k >= 0) {
                            if (sched.voices[k].cut == cutGroup) {
                                unsafe { delete sched.voices[k]; }
                                sched.voices |> erase(k)
                            }
                            k --
                        }
                    }
                    sf2_spawn_voices(sched, h.value, durSec)
                } else {
                    // sample/oscillator path: pre-render to buffer
                    var stereo <- render_event_stereo(h.value, durSec, bank)
                    // calculate how many samples into the chunk this onset falls
                    let onsetCycle = h.whole.start
                    let onsetWall = sched.startTime + onsetCycle / sched.cps
                    let offsetSec = float(onsetWall - wall_time)
                    let offsetSamples = max(int(offsetSec * float(SAMPLE_RATE)) * 2, 0) // stereo offset
                    // cut groups: kill existing voices in same group
                    let cutGroup = h.value.cut
                    if (cutGroup != 0) {
                        var k = length(sched.voices) - 1
                        while (k >= 0) {
                            if (sched.voices[k].cut == cutGroup) {
                                unsafe { delete sched.voices[k]; }
                                sched.voices |> erase(k)
                            }
                            k --
                        }
                    }
                    sched.voices |> push(new Voice(
                        samples <- stereo,
                        position = -offsetSamples,
                        cut = cutGroup,
                        gain = h.value.gain * h.value.velocity,
                        pan = h.value.pan * 2.0 - 1.0
                    ))
                }
            }
        }
        delete haps
        sched.lastQueryEnd = spawnEnd
    }

    // produce output: fixed-size stereo buffer for this tick
    let chunkFrames = int(chunk_seconds * float(SAMPLE_RATE))
    let chunkSamples = chunkFrames * 2  // stereo
    var output : array<float>
    output |> resize(chunkSamples)

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
            ma_volume_mixer_set_pan(unsafe(addr(sched.mixer)), voice.pan)
            let nFrames = uint64(count / 2)
            unsafe {
                ma_volume_mixer_process_pcm_frames(
                    addr(sched.mixer),
                    addr(voice.samples[srcStart]),
                    addr(output[dstStart]),
                    nFrames
                )
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

    // render SF2 real-time voices into the same output buffer
    sf2_render_voices(sched, output, chunkFrames)

    return <- output
}

// Set tempo in BPM (assumes 4 beats per cycle).
def set_bpm(var sched : Scheduler; bpm : double) {
    sched.cps = bpm / 240.0lf
}

// Set cycles per second directly.
def setcps(var sched : Scheduler; cps : double) {
    sched.cps = cps
}
