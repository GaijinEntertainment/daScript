options gen2
options indenting = 4

module strudel_event shared

require math

// The payload of every pattern event — "what to play and how".
// All defaults are "do nothing" values: gain 0.8 is a sensible level,
// lpf 0 means "no filter" (undefined = no filter).
struct Event {
    s       : string = ""       // sound name: "bd", "sd", "sawtooth", "sine"
    note    : float = 60.0      // MIDI note number (60 = middle C)
    n       : int = 0           // sample variation index (bd:0, bd:1, bd:2)
    gain    : float = 0.8       // amplitude 0.0–1.0 (applied after rendering)
    velocity : float = 1.0     // expression 0.0–1.0 (scales gain, may affect timbre later)
    pan     : float = 0.5       // stereo pan: 0.0 = left, 0.5 = center, 1.0 = right
    speed   : float = 1.0       // sample playback speed (1.0 = normal)
    lpf     : float = 0.0       // low-pass filter cutoff Hz (0 = no filter; set to activate)
    lpq     : float = 1.0       // low-pass filter resonance Q (1.0 = gentle, >5 = peaky)
    hpf     : float = 0.0       // high-pass filter cutoff Hz (0 = off)
    hpq     : float = 1.0       // high-pass filter resonance Q (1.0 = gentle, >5 = peaky)
    attack  : float = -1.0      // envelope attack time in seconds (-1 = unset → resolver picks default)
    decay   : float = -1.0      // envelope decay time (-1 = unset)
    sustain : float = -1.0      // envelope sustain level (-1 = unset; full sustain if no ADSR set, percussion if decay set alone)
    release : float = -1.0      // envelope release time (-1 = unset)
    cut     : int = 0           // cut group (0 = no cut, >0 = kill same group)
    room    : float = 0.0       // reverb send amount 0.0–1.0
    roomsize : float = 2.0      // reverb decay time in seconds (0.1–10.0)
    delay_amount : float = 0.0  // delay send amount 0.0–1.0
    delaytime    : float = -1.0 // delay time in seconds (-1 = unset → derived from delaysync and cps)
    delayfeedback : float = 0.5 // delay feedback 0.0–1.0
    delaysync    : float = 0.1875 // delay time in cycles (3/16 cycle); used when delaytime is unset
    orbit   : int = 0           // effect bus routing (0 = default)
    fm      : float = 0.0       // FM synthesis modulation depth
    fmh     : float = 1.0       // FM harmonicity ratio (modulator freq / carrier freq)
    vowel   : string = ""      // vowel formant filter ("a", "e", "i", "o", "u", etc.)
    chorus  : float = 0.0       // chorus send amount 0.0–1.0
    // Sample region (0.0–1.0 of sample length)
    begin   : float = 0.0       // sample start position (0.0 = start)
    end_pos : float = 1.0       // sample end position (1.0 = end). Named end_pos to avoid keyword conflict.
    // Audio effects
    crush   : float = 0.0       // bit depth reduction (0 = off, 4–16 = bit depth)
    coarse  : int = 0           // sample rate reduction factor (0 = off, 2 = half rate, etc.)
    shape   : float = 0.0       // waveshaper drive (0 = off, >0 = saturated)
    djf     : float = 0.5       // DJ filter (0 = full LPF, 0.5 = off, 1 = full HPF)
    bpf     : float = 0.0       // bandpass filter cutoff Hz (0 = off)
    bpq     : float = 1.0       // bandpass filter Q
    // Phaser (single notch biquad with LFO-modulated center)
    phaser        : float = 0.0     // phaser LFO rate in Hz (0 = off)
    phaserdepth   : float = 0.75    // phaser depth 0..1 (notch Q + sweep scale)
    phasercenter  : float = 1000.0  // phaser center frequency Hz
    phasersweep   : float = 2000.0  // phaser sweep range Hz
    // Tremolo (amplitude modulation)
    tremolo       : float = 0.0     // tremolo LFO rate in Hz (0 = off)
    tremolodepth  : float = 1.0     // tremolo depth 0..1
    // Compressor (threshold in dB; >=0 = bypass)
    compressor    : float = 1.0     // threshold dB (>=0 = bypass; typical -20..-3)
    compressor_ratio   : float = 10.0   // compression ratio (>=1)
    compressor_knee    : float = 10.0   // soft-knee width in dB
    compressor_attack  : float = 0.005  // attack time seconds
    compressor_release : float = 0.05   // release time seconds
    // SF2 SoundFont
    sf2_program    : int = -1   // SF2 GM program number (-1 = not SF2, use sample/oscillator)
    sf2_bank       : int = 0    // SF2 bank (0 = melodic, 128 = percussion)
    sf2_expression : float = -1.0 // SF2 expression CC11 (0.0–1.0, -1 = use default)
    sf2_mod_wheel  : float = -1.0 // SF2 mod wheel CC1 (0.0–1.0, -1 = use default)
    sf2_pitch_bend : float = -1.0 // SF2 pitch bend (0.0–1.0 where 0.5 = center, -1 = use default)
}

// Resolve ADSR fields from their "user set / unset" form (−1 = unset) into concrete
// envelope parameters. The rules are designed so that intuitive shorthand sounds right:
//
//   • Nothing set           → a=0.001, d=0.001, s=1.0, r=0.01 (a held tone with tiny envelope edges)
//   • Only decay set        → sustain drops to 0.001 (percussion: attack-then-decay to silence)
//   • Only attack or only release set (no decay) → sustain stays at 1.0 (held tone)
//   • Sustain set explicitly → use the given value, whatever else is set
//
// All values are clamped to envmin/releaseMin floors so a stray 0 never produces a click.
def resolve_adsr(attack, decay, sustain, release : float) : tuple<float; float; float; float> {
    let envmin = 0.001
    let envmax = 1.0
    let release_min = 0.01
    let a_set = attack >= 0.0
    let d_set = decay >= 0.0
    let s_set = sustain >= 0.0
    let r_set = release >= 0.0
    if (!a_set && !d_set && !s_set && !r_set) {
        return (envmin, envmin, envmax, release_min)
    }
    let sustain_resolved = s_set ? sustain : (d_set ? envmin : envmax)
    let a_final = max(a_set ? attack : 0.0, envmin)
    let d_final = max(d_set ? decay : 0.0, envmin)
    let r_final = max(r_set ? release : 0.0, release_min)
    let s_final = min(sustain_resolved, envmax)
    return (a_final, d_final, s_final, r_final)
}

// Resolve delay time: if delaytime was set explicitly (>= 0), use it; otherwise
// derive it from delaysync (in cycles) and the current cps so the echo locks to
// the tempo. The 3/16-cycle default yields a dotted-eighth feel that changes
// automatically when the user changes cps.
def resolve_delaytime(delaytime, delaysync, cps : float) : float {
    if (delaytime >= 0.0) {
        return delaytime
    }
    let safe_cps = cps > 0.0001 ? cps : 0.5
    return delaysync / safe_cps
}
