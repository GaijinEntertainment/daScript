options gen2
options indenting = 4

module strudel_scheduler shared

require strudel/strudel_event
require strudel/strudel_time
require strudel/strudel_pattern
require strudel/strudel_synth
require strudel/strudel_samples
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

// The scheduler bridges the pattern engine and audio output.
// Maintains a pool of active voices for continuous audio.
[safe_when_uninitialized]
struct Scheduler {
    cps          : double = 0.5lf   // cycles per second (0.5 = 120 BPM at 4 beats/cycle)
    startTime    : double = 0.0lf   // wall-clock time when playback started
    lastQueryEnd : double = 0.0lf   // cycle position of last query end
    voices       : array<Voice?>    // active voices
    mixer        : ma_volume_mixer  // reused across ticks
    mixer_ready  : bool = false
}

// Convert wall-clock time to cycle position.
def current_cycle(sched : Scheduler; wall_time : double) : double {
    return (wall_time - sched.startTime) * sched.cps
}

// Tick the scheduler: query pattern for new events, render them,
// mix all active voices into a fixed-size stereo buffer.
//
// chunk_seconds: how much audio to produce this tick (e.g. 0.05 for 50ms)
// lookAhead: how far ahead in time to query for new events (in seconds)
def tick(var sched : Scheduler; var pat : Pattern; bank : SampleBank; wall_time : double; chunk_seconds : float = 0.05; lookAhead : float = 0.1) : array<float> {
    let cycleNow = current_cycle(sched, wall_time)
    let cycleEnd = current_cycle(sched, wall_time + double(lookAhead))

    // query for new events
    let queryStart = max(cycleNow, sched.lastQueryEnd)
    if (queryStart < cycleEnd) {
        var haps <- pat(TimeSpan(start = queryStart, stop = cycleEnd))
        for (h in haps) {
            // only trigger at onset
            if (h.has_whole && abs(h.whole.start - h.part.start) < 0.0001lf) {
                let durCycles = h.whole.stop - h.whole.start
                let durSec = float(durCycles / sched.cps)
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
                            unsafe {
                                delete sched.voices[k]
                            }
                            sched.voices |> erase(k)
                        }
                        k -= 1
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
        delete haps
        sched.lastQueryEnd = cycleEnd
    }

    // produce output: fixed-size stereo buffer for this tick
    let chunkFrames = int(chunk_seconds * float(SAMPLE_RATE))
    let chunkSamples = chunkFrames * 2  // stereo
    var output : array<float>
    output |> resize(chunkSamples)

    // mix all active voices into the output using ma_volume_mixer (gain + pan per voice)
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
        i -= 1
    }
    return <- output
}

// Set tempo in BPM (assumes 4 beats per cycle).
def set_bpm(var sched : Scheduler&; bpm : double) {
    sched.cps = bpm / 240.0lf
}

// Set cycles per second directly.
def setcps(var sched : Scheduler&; cps : double) {
    sched.cps = cps
}
