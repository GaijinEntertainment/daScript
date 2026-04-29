options gen2
options indenting = 4

module strudel_event shared public
//! The `Event` payload — "what to play and how" — plus ADSR and delay-time resolvers.
//! Every pattern event carries one `Event` with sound, note, gain, filter, envelope,
//! and effect settings. `-1.0` in an ADSR field means "unset — resolver picks default".

require math

//! Payload of every pattern event. Defaults are "do nothing" values: `gain = 0.8` is
//! a sensible level, `lpf = 0` means "no filter", and ADSR fields of `-1.0` mean "unset".
struct Event {
    s       : string = ""       // sound name: "bd", "sd", "sawtooth", "sine"
        //! Sound/sample name (`"bd"`, `"sd"`, `"sawtooth"`, `"sine"`, ...).
    note    : float = 60.0      // MIDI note number (60 = middle C)
        //! MIDI note number (60 = middle C).
    n       : int = 0           // sample variation index (bd:0, bd:1, bd:2)
        //! Sample variation index within a bank (`bd:0`, `bd:1`, `bd:2`).
    gain    : float = 0.8       // amplitude 0.0–1.0 (applied after rendering)
        //! Amplitude 0.0–1.0, applied after rendering.
    velocity : float = 1.0     // expression 0.0–1.0 (scales gain, may affect timbre later)
        //! Expression 0.0–1.0; scales `gain` and may affect timbre.
    pan     : float = 0.5       // stereo pan: 0.0 = left, 0.5 = center, 1.0 = right
        //! Stereo pan: 0.0 = left, 0.5 = center, 1.0 = right.
    speed   : float = 1.0       // sample playback speed (1.0 = normal)
        //! Sample playback speed (1.0 = normal, 2.0 = double, 0.5 = half).
    lpf     : float = 0.0       // low-pass filter cutoff Hz (0 = no filter; set to activate)
        //! Low-pass filter cutoff in Hz (0 = disabled).
    lpq     : float = 1.0       // low-pass filter resonance Q (1.0 = gentle, >5 = peaky)
        //! Low-pass filter resonance Q (1.0 = gentle, >5 = peaky).
    hpf     : float = 0.0       // high-pass filter cutoff Hz (0 = off)
        //! High-pass filter cutoff in Hz (0 = disabled).
    hpq     : float = 1.0       // high-pass filter resonance Q (1.0 = gentle, >5 = peaky)
        //! High-pass filter resonance Q (1.0 = gentle, >5 = peaky).
    attack  : float = -1.0      // envelope attack time in seconds (-1 = unset → resolver picks default)
        //! Envelope attack time in seconds (-1 = unset, resolver picks default).
    decay   : float = -1.0      // envelope decay time (-1 = unset)
        //! Envelope decay time in seconds (-1 = unset).
    sustain : float = -1.0      // envelope sustain level (-1 = unset; full sustain if no ADSR set, percussion if decay set alone)
        //! Envelope sustain level 0.0–1.0 (-1 = unset).
    release : float = -1.0      // envelope release time (-1 = unset)
        //! Envelope release time in seconds (-1 = unset).
    cut     : int = 0           // cut group (0 = no cut, >0 = kill same group)
        //! Cut group (0 = no cut, >0 = kill any voice with the same cut id).
    room    : float = 0.0       // reverb send amount 0.0–1.0
        //! Reverb send amount 0.0–1.0.
    roomsize : float = 2.0      // reverb decay time in seconds (0.1–10.0)
        //! Reverb decay time in seconds (0.1–10.0).
    delay_amount : float = 0.0  // delay send amount 0.0–1.0
        //! Delay send amount 0.0–1.0.
    delaytime    : float = -1.0 // delay time in seconds (-1 = unset → derived from delaysync and cps)
        //! Delay time in seconds (-1 = unset, derived from `delaysync` and cps).
    delayfeedback : float = 0.5 // delay feedback 0.0–1.0
        //! Delay feedback 0.0–1.0.
    delaysync    : float = 0.1875 // delay time in cycles (3/16 cycle); used when delaytime is unset
        //! Delay time in cycles; used when `delaytime` is unset (default = 3/16 cycle, dotted-eighth feel).
    orbit   : int = 0           // effect bus routing (0 = default)
        //! Effect-bus routing id (0 = default bus).
    fm      : float = 0.0       // FM synthesis modulation depth
        //! FM synthesis modulation depth.
    fmh     : float = 1.0       // FM harmonicity ratio (modulator freq / carrier freq)
        //! FM harmonicity ratio (modulator frequency / carrier frequency).
    vowel   : string = ""      // vowel formant filter ("a", "e", "i", "o", "u", etc.)
        //! Vowel formant filter (`"a"`, `"e"`, `"i"`, `"o"`, `"u"`, ...).
    chorus  : float = 0.0       // chorus send amount 0.0–1.0
        //! Chorus send amount 0.0–1.0.
    // Sample region (0.0–1.0 of sample length)
    begin   : float = 0.0       // sample start position (0.0 = start)
        //! Sample region start (fraction of sample length, 0.0 = beginning).
    end_pos : float = 1.0       // sample end position (1.0 = end). Named end_pos to avoid keyword conflict.
        //! Sample region end (fraction of sample length, 1.0 = end). Named to avoid the `end` keyword.
    // Audio effects
    crush   : float = 0.0       // bit depth reduction (0 = off, 4–16 = bit depth)
        //! Bit-depth reduction (0 = off, 4–16 = target bit depth).
    coarse  : int = 0           // sample rate reduction factor (0 = off, 2 = half rate, etc.)
        //! Sample-rate reduction factor (0 = off, 2 = half rate).
    shape   : float = 0.0       // waveshaper drive (0 = off, >0 = saturated)
        //! Waveshaper drive (0 = off, >0 = saturated).
    djf     : float = 0.5       // DJ filter (0 = full LPF, 0.5 = off, 1 = full HPF)
        //! DJ filter (0 = full LPF, 0.5 = bypass, 1 = full HPF).
    bpf     : float = 0.0       // bandpass filter cutoff Hz (0 = off)
        //! Bandpass filter cutoff in Hz (0 = disabled).
    bpq     : float = 1.0       // bandpass filter Q
        //! Bandpass filter resonance Q.
    // Phaser (single notch biquad with LFO-modulated center)
    phaser        : float = 0.0     // phaser LFO rate in Hz (0 = off)
        //! Phaser LFO rate in Hz (0 = off).
    phaserdepth   : float = 0.75    // phaser depth 0..1 (notch Q + sweep scale)
        //! Phaser depth 0.0–1.0 (controls notch Q and sweep range).
    phasercenter  : float = 1000.0  // phaser center frequency Hz
        //! Phaser center frequency in Hz.
    phasersweep   : float = 2000.0  // phaser sweep range Hz
        //! Phaser sweep range in Hz.
    // Tremolo (amplitude modulation)
    tremolo       : float = 0.0     // tremolo LFO rate in Hz (0 = off)
        //! Tremolo LFO rate in Hz (0 = off).
    tremolodepth  : float = 1.0     // tremolo depth 0..1
        //! Tremolo depth 0.0–1.0.
    // Compressor (threshold in dB; >=0 = bypass)
    compressor    : float = 1.0     // threshold dB (>=0 = bypass; typical -20..-3)
        //! Compressor threshold in dB (>=0 = bypass; typical -20..-3).
    compressor_ratio   : float = 10.0   // compression ratio (>=1)
        //! Compression ratio (>=1).
    compressor_knee    : float = 10.0   // soft-knee width in dB
        //! Compressor soft-knee width in dB.
    compressor_attack  : float = 0.005  // attack time seconds
        //! Compressor attack time in seconds.
    compressor_release : float = 0.05   // release time seconds
        //! Compressor release time in seconds.
    // HRTF positional override — when active, the equal-power stereo pan render is replaced by a
    // binaural-stereo HRTF (two ma_hrtf instances per voice; pan still controls source width).
    // Shares the ma_hrtf engine used by audio_boost; CIPIC dataset is baked in at compile time.
    hrtf_active    : bool = false   // explicit "is HRTF position set?" flag (azimuth=0 alone is ambiguous — means "in front")
        //! True when HRTF positioning is active for this event; setters flip this on.
    hrtf_azimuth   : float = 0.0    // azimuth in degrees: -180..180 (0 = directly ahead, +90 = right, -90 = left)
        //! HRTF azimuth in degrees (-180..180; 0 = ahead, +90 = right, -90 = left). Ignored unless `hrtf_active`.
    hrtf_elevation : float = 0.0    // elevation in degrees: -90..90 (0 = horizontal plane)
        //! HRTF elevation in degrees (-90..90; 0 = horizontal). Ignored unless `hrtf_active`.
    // SF2 SoundFont
    sf2_program    : int = -1   // SF2 GM program number (-1 = not SF2, use sample/oscillator)
        //! SF2 GM program number (-1 = not SF2; use sample/oscillator instead).
    sf2_bank       : int = 0    // SF2 bank (0 = melodic, 128 = percussion)
        //! SF2 bank (0 = melodic, 128 = percussion).
    sf2_expression : float = -1.0 // SF2 expression CC11 (0.0–1.0, -1 = use default)
        //! SF2 expression (CC11) 0.0–1.0 (-1 = use default).
    sf2_mod_wheel  : float = -1.0 // SF2 mod wheel CC1 (0.0–1.0, -1 = use default)
        //! SF2 mod wheel (CC1) 0.0–1.0 (-1 = use default).
    sf2_pitch_bend : float = -1.0 // SF2 pitch bend (0.0–1.0 where 0.5 = center, -1 = use default)
        //! SF2 pitch bend 0.0–1.0 (0.5 = center, -1 = use default).
}

// Resolve ADSR fields from their "user set / unset" form (-1 = unset) into concrete
// envelope parameters. The rules are designed so that intuitive shorthand sounds right:
//
//   • Nothing set           → a=0.001, d=0.001, s=1.0, r=0.01 (a held tone with tiny envelope edges)
//   • Only decay set        → sustain drops to 0.001 (percussion: attack-then-decay to silence)
//   • Only attack or only release set (no decay) → sustain stays at 1.0 (held tone)
//   • Sustain set explicitly → use the given value, whatever else is set
//
// All values are clamped to envmin/releaseMin floors so a stray 0 never produces a click.
def resolve_adsr(attack, decay, sustain, release : float) : tuple<float; float; float; float> {
    //! Resolve ADSR fields from their "user set / unset" form (-1 = unset) into concrete
    //! envelope parameters. Rules: nothing set -> tiny edges + full sustain; only decay
    //! set -> percussion (sustain drops to zero); only attack/release set -> held tone;
    //! sustain set explicitly -> uses the given value. All values are clamped to floors
    //! so a stray 0 never produces a click.
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
    //! Resolve delay time: use `delaytime` if set (>= 0), otherwise derive from
    //! `delaysync` (in cycles) and the current `cps` so the echo locks to tempo.
    //! The 3/16-cycle default yields a dotted-eighth feel that tracks tempo changes.
    if (delaytime >= 0.0) {
        return delaytime
    }
    let safe_cps = cps > 0.0001 ? cps : 0.5
    return delaysync / safe_cps
}
