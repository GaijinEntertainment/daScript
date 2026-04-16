options gen2
options indenting = 4

module strudel_event shared

// The payload of every pattern event — "what to play and how".
// All defaults are "do nothing" values: gain 0.8 is a sensible level,
// lpf 20000 means filter wide open, pan 0.5 is center, etc.
struct Event {
    s       : string = ""       // sound name: "bd", "sd", "sawtooth", "sine"
    note    : float = 60.0      // MIDI note number (60 = middle C)
    n       : int = 0           // sample variation index (bd:0, bd:1, bd:2)
    gain    : float = 0.8       // amplitude 0.0–1.0 (applied after rendering)
    velocity : float = 1.0     // expression 0.0–1.0 (scales gain, may affect timbre later)
    pan     : float = 0.5       // stereo pan: 0.0 = left, 0.5 = center, 1.0 = right
    speed   : float = 1.0       // sample playback speed (1.0 = normal)
    lpf     : float = 20000.0   // low-pass filter cutoff Hz (20kHz = wide open)
    lpq     : float = 1.0       // low-pass filter resonance Q (1.0 = gentle, >5 = peaky)
    hpf     : float = 0.0       // high-pass filter cutoff Hz (0 = off)
    hpq     : float = 1.0       // high-pass filter resonance Q (1.0 = gentle, >5 = peaky)
    attack  : float = 0.001     // envelope attack time in seconds
    decay   : float = 0.05      // envelope decay time
    sustain : float = 0.0       // envelope sustain level (default: no sustain hold)
    release : float = 0.01      // envelope release time
    cut     : int = 0           // cut group (0 = no cut, >0 = kill same group)
    room    : float = 0.0       // reverb send amount 0.0–1.0
    roomsize : float = 2.0      // reverb decay time in seconds (0.1–10.0)
    delay_amount : float = 0.0  // delay send amount 0.0–1.0
    delaytime    : float = 0.5  // delay time in seconds
    delayfeedback : float = 0.5 // delay feedback 0.0–1.0
    orbit   : int = 0           // effect bus routing (0 = default)
    fm      : float = 0.0       // FM synthesis modulation depth
    fmh     : float = 1.0       // FM harmonicity ratio (modulator freq / carrier freq)
    vowel   : string = ""      // vowel formant filter ("a", "e", "i", "o", "u", etc.)
    sf2_program : int = -1      // SF2 GM program number (-1 = not SF2, use sample/oscillator)
    sf2_bank    : int = 0       // SF2 bank (0 = melodic, 128 = percussion)
}
