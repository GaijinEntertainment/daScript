options gen2
options indenting = 4

module strudel_sfxr shared public
//! Faithful sfxr-style procedural one-shot SFX generator (DrPetter's sfxr model).
//! One parameterized synth (`render_sfxr`) covers square/saw/sine/triangle/noise voices with
//! envelope (attack/sustain/punch/decay), pitch slide + arpeggio + vibrato, square duty, retrigger,
//! phaser and a low/high-pass filter. The classic 7 generators (pickup/laser/explosion/powerup/
//! hit/jump/blip) plus randomize/mutate produce a fresh `SfxrSound` you can render, tweak and play.
//! Output is mono `array<float>` in [-1,1] at `SFXR_SAMPLE_RATE` (44100, the model's native rate).

require math

let SFXR_SAMPLE_RATE = 44100
let private TWO_PI = 6.28318530717958647692

enum SfxrWave {
    //! Base oscillator shape. Values 0..4 match the slot order sfxr presets randomize over.
    square
    sawtooth
    sine
    noise
    triangle
}

struct SfxrSound {
    //! sfxr synthesizer parameters. A freshly-constructed value is sfxr's neutral "reset" patch.
    //! Most fields are normalized: [0,1] unless noted [-1,1]. Magic scaling matches the original sfxr.
    wave : SfxrWave = SfxrWave.square
    sound_vol : float = 0.5             //!< master gain
    env_attack : float = 0.0            //!< attack time
    env_sustain : float = 0.3           //!< sustain time
    env_punch : float = 0.0             //!< initial amplitude overshoot during sustain
    env_decay : float = 0.4             //!< decay time
    base_freq : float = 0.3             //!< base pitch
    freq_limit : float = 0.0            //!< pitch floor; sound ends if the slide crosses it (0 = none)
    freq_ramp : float = 0.0             //!< pitch slide [-1,1]
    freq_dramp : float = 0.0            //!< pitch slide acceleration [-1,1]
    vib_strength : float = 0.0          //!< vibrato depth
    vib_speed : float = 0.0             //!< vibrato rate
    arp_mod : float = 0.0               //!< one-shot pitch jump amount [-1,1]
    arp_speed : float = 0.0             //!< time until the pitch jump (1 = disabled)
    duty : float = 0.0                  //!< square duty cycle
    duty_ramp : float = 0.0             //!< square duty sweep [-1,1]
    repeat_speed : float = 0.0          //!< retrigger rate (0 = disabled)
    pha_offset : float = 0.0            //!< phaser offset [-1,1]
    pha_ramp : float = 0.0              //!< phaser sweep [-1,1]
    lpf_freq : float = 1.0             //!< low-pass cutoff (1 = fully open / disabled)
    lpf_ramp : float = 0.0             //!< low-pass cutoff sweep [-1,1]
    lpf_resonance : float = 0.0        //!< low-pass resonance
    hpf_freq : float = 0.0             //!< high-pass cutoff
    hpf_ramp : float = 0.0             //!< high-pass cutoff sweep [-1,1]
}

// ===== RNG (deterministic; same seed => same sound) =====

def private frnd(var seed : uint&; mx : float) : float {
    seed = seed * 1664525u + 1013904223u
    return float((seed >> 9u) & 0x7FFFFFu) / 8388607.0 * mx
}

def private rnd_i(var seed : uint&; n : int) : int {
    seed = seed * 1664525u + 1013904223u
    return int((seed >> 9u) & 0x7FFFFFu) % (n + 1)
}

// sign-preserving integer powers (sfxr uses these on possibly-negative bases)
def private p3(x : float) : float => x * x * x
def private p5(x : float) : float => x * x * x * x * x

def sfxr_wave_of(i : int) : SfxrWave {
    //! Map an integer index (0..4) to its SfxrWave, for UI combos / serialization.
    if (i == 1) return SfxrWave.sawtooth
    if (i == 2) return SfxrWave.sine
    if (i == 3) return SfxrWave.noise
    if (i == 4) return SfxrWave.triangle
    return SfxrWave.square
}

// ===== Synthesis =====

def render_sfxr(snd : SfxrSound; sample_rate : int = SFXR_SAMPLE_RATE) : array<float> {
    //! Render a sound to a mono buffer in [-1,1]. Deterministic: identical params yield identical output.
    var seed = 0x9E3779B9u

    // ---- reset (sfxr ResetSample, full) ----
    var fperiod = 100.0 / (snd.base_freq * snd.base_freq + 0.001)
    var period = int(fperiod)
    let fmaxperiod = 100.0 / (snd.freq_limit * snd.freq_limit + 0.001)
    var fslide = 1.0 - p3(snd.freq_ramp) * 0.01
    let fdslide = -p3(snd.freq_dramp) * 0.000001
    var square_duty = 0.5 - snd.duty * 0.5
    let square_slide = -snd.duty_ramp * 0.00005

    var arp_mod : float
    if (snd.arp_mod >= 0.0) {
        arp_mod = 1.0 - snd.arp_mod * snd.arp_mod * 0.9
    } else {
        arp_mod = 1.0 + snd.arp_mod * snd.arp_mod * 10.0
    }
    var arp_time = 0
    var arp_limit = int((1.0 - snd.arp_speed) * (1.0 - snd.arp_speed) * 20000.0 + 32.0)
    if (snd.arp_speed == 1.0) { arp_limit = 0; }

    // filter
    var fltp = 0.0
    var fltdp = 0.0
    var fltw = p3(snd.lpf_freq) * 0.1
    let fltw_d = 1.0 + snd.lpf_ramp * 0.0001
    var fltdmp = 5.0 / (1.0 + snd.lpf_resonance * snd.lpf_resonance * 20.0) * (0.01 + fltw)
    if (fltdmp > 0.8) { fltdmp = 0.8; }
    var fltphp = 0.0
    var flthp = snd.hpf_freq * snd.hpf_freq * 0.1
    let flthp_d = 1.0 + snd.hpf_ramp * 0.0003

    // vibrato
    var vib_phase = 0.0
    let vib_speed = snd.vib_speed * snd.vib_speed * 0.01
    let vib_amp = snd.vib_strength * 0.5

    // envelope
    var env_vol = 0.0
    var env_stage = 0
    var env_time = 0
    var env_length : int[3]
    env_length[0] = int(snd.env_attack * snd.env_attack * 100000.0)
    env_length[1] = int(snd.env_sustain * snd.env_sustain * 100000.0)
    env_length[2] = int(snd.env_decay * snd.env_decay * 100000.0)

    // phaser
    var fphase = snd.pha_offset * snd.pha_offset * 1020.0
    if (snd.pha_offset < 0.0) { fphase = -fphase; }
    var fdphase = snd.pha_ramp * snd.pha_ramp * 1.0
    if (snd.pha_ramp < 0.0) { fdphase = -fdphase; }
    var iphase = abs(int(fphase))
    var ipp = 0
    var phaser_buffer : float[1024]
    for (i in range(1024)) { phaser_buffer[i] = 0.0; }

    var phase = 0
    var noise_buffer : float[32]
    for (i in range(32)) { noise_buffer[i] = frnd(seed, 2.0) - 1.0; }

    var rep_time = 0
    var rep_limit = int((1.0 - snd.repeat_speed) * (1.0 - snd.repeat_speed) * 20000.0 + 32.0)
    if (snd.repeat_speed == 0.0) { rep_limit = 0; }

    // master gain: square base is +-0.5, so /8 (supersample avg) * 2 * sound_vol peaks near sound_vol.
    let total = env_length[0] + env_length[1] + env_length[2]
    var samples : array<float>
    if (total <= 0) return <- samples
    samples |> resize(total)
    var written = 0

    for (sidx in range(total)) {
        // retrigger: re-run the pitch/duty/arp part of reset (sfxr ResetSample(restart=true))
        rep_time++
        if (rep_limit != 0 && rep_time >= rep_limit) {
            rep_time = 0
            fperiod = 100.0 / (snd.base_freq * snd.base_freq + 0.001)
            period = int(fperiod)
            fslide = 1.0 - p3(snd.freq_ramp) * 0.01
            square_duty = 0.5 - snd.duty * 0.5
            arp_time = 0
            arp_limit = int((1.0 - snd.arp_speed) * (1.0 - snd.arp_speed) * 20000.0 + 32.0)
            if (snd.arp_speed == 1.0) { arp_limit = 0; }
        }

        // pitch slide + arpeggio
        arp_time++
        if (arp_limit != 0 && arp_time >= arp_limit) {
            arp_limit = 0
            fperiod *= arp_mod
        }
        fslide += fdslide
        fperiod *= fslide
        var freq_done = false
        if (fperiod > fmaxperiod) {
            fperiod = fmaxperiod
            if (snd.freq_limit > 0.0) { freq_done = true; }
        }
        var rfperiod = fperiod
        if (vib_amp > 0.0) {
            vib_phase += vib_speed
            rfperiod = fperiod * (1.0 + sin(vib_phase) * vib_amp)
        }
        period = int(rfperiod)
        if (period < 8) { period = 8; }
        square_duty = clamp(square_duty + square_slide, 0.0, 0.5)

        // envelope advance
        env_time++
        if (env_time > env_length[env_stage]) {
            env_time = 0
            env_stage++
            if (env_stage == 3) break
        }
        if (env_stage == 0) {
            env_vol = float(env_time) / float(max(env_length[0], 1))
        } elif (env_stage == 1) {
            let d = 1.0 - float(env_time) / float(max(env_length[1], 1))
            env_vol = 1.0 + d * 2.0 * snd.env_punch
        } else {
            env_vol = 1.0 - float(env_time) / float(max(env_length[2], 1))
        }

        // phaser advance
        fphase += fdphase
        iphase = abs(int(fphase))
        if (iphase > 1023) { iphase = 1023; }
        if (flthp_d != 0.0) {
            flthp = clamp(flthp * flthp_d, 0.00001, 0.1)
        }

        // 8x supersampling
        var ssample = 0.0
        for (_si in range(8)) {
            phase++
            if (phase >= period) {
                phase = phase % period
                if (snd.wave == SfxrWave.noise) {
                    for (j in range(32)) { noise_buffer[j] = frnd(seed, 2.0) - 1.0; }
                }
            }
            let fp = float(phase) / float(period)
            var sample = 0.0
            if (snd.wave == SfxrWave.square) {
                sample = fp < square_duty ? 0.5 : -0.5
            } elif (snd.wave == SfxrWave.sawtooth) {
                sample = 1.0 - fp * 2.0
            } elif (snd.wave == SfxrWave.sine) {
                sample = sin(fp * TWO_PI)
            } elif (snd.wave == SfxrWave.triangle) {
                sample = fp < 0.5 ? 4.0 * fp - 1.0 : 3.0 - 4.0 * fp
            } else {
                sample = noise_buffer[(phase * 32 / period) & 31]
            }

            // low-pass
            let pp = fltp
            fltw = clamp(fltw * fltw_d, 0.0, 0.1)
            if (snd.lpf_freq != 1.0) {
                fltdp += (sample - fltp) * fltw
                fltdp -= fltdp * fltdmp
            } else {
                fltp = sample
                fltdp = 0.0
            }
            fltp += fltdp
            // high-pass
            fltphp += fltp - pp
            fltphp -= fltphp * flthp
            sample = fltphp
            // phaser
            phaser_buffer[ipp & 1023] = sample
            sample += phaser_buffer[(ipp - iphase + 1024) & 1023]
            ipp = (ipp + 1) & 1023
            ssample += sample * env_vol
        }
        ssample = ssample / 8.0 * 2.0 * snd.sound_vol
        samples[sidx] = clamp(ssample, -1.0, 1.0)
        written++
        if (freq_done) break
    }

    if (written < total) { samples |> resize(written); }
    return <- samples
}

// ===== Preset generators (sfxr's classic buttons; thread a seed for variety) =====

def sfxr_pickup_coin(var seed : uint&) : SfxrSound {
    //! Coin / pickup chime: bright base pitch, snappy decay, optional arpeggio sparkle.
    let base = 0.4 + frnd(seed, 0.5)
    let sus = frnd(seed, 0.1)
    let dec = 0.1 + frnd(seed, 0.4)
    let pun = 0.3 + frnd(seed, 0.3)
    var s = SfxrSound(base_freq = base, env_attack = 0.0, env_sustain = sus, env_decay = dec, env_punch = pun)
    if (rnd_i(seed, 1) != 0) {
        s.arp_speed = 0.5 + frnd(seed, 0.2)
        s.arp_mod = 0.2 + frnd(seed, 0.4)
    }
    return s
}

def sfxr_laser_shoot(var seed : uint&) : SfxrSound {
    //! Laser / shoot: fast downward pitch slide, often square/saw with duty motion.
    var s = SfxrSound()
    s.wave = sfxr_wave_of(rnd_i(seed, 2))
    if (s.wave == SfxrWave.sine && rnd_i(seed, 1) != 0) {
        s.wave = sfxr_wave_of(rnd_i(seed, 1))
    }
    s.base_freq = 0.5 + frnd(seed, 0.5)
    s.freq_limit = s.base_freq - 0.2 - frnd(seed, 0.6)
    if (s.freq_limit < 0.2) { s.freq_limit = 0.2; }
    s.freq_ramp = -0.15 - frnd(seed, 0.2)
    if (rnd_i(seed, 2) == 0) {
        s.base_freq = 0.3 + frnd(seed, 0.6)
        s.freq_limit = frnd(seed, 0.1)
        s.freq_ramp = -0.35 - frnd(seed, 0.3)
    }
    if (rnd_i(seed, 1) != 0) {
        s.duty = frnd(seed, 0.5)
        s.duty_ramp = frnd(seed, 0.2)
    } else {
        s.duty = 0.4 + frnd(seed, 0.5)
        s.duty_ramp = -frnd(seed, 0.7)
    }
    s.env_attack = 0.0
    s.env_sustain = 0.1 + frnd(seed, 0.2)
    s.env_decay = frnd(seed, 0.4)
    if (rnd_i(seed, 1) != 0) { s.env_punch = frnd(seed, 0.3); }
    if (rnd_i(seed, 2) == 0) {
        s.pha_offset = frnd(seed, 0.2)
        s.pha_ramp = -frnd(seed, 0.2)
    }
    if (rnd_i(seed, 1) != 0) { s.hpf_freq = frnd(seed, 0.3); }
    return s
}

def sfxr_explosion(var seed : uint&) : SfxrSound {
    //! Explosion: noise voice with a downward sweep, punch and optional vibrato/retrigger rumble.
    var s = SfxrSound()
    s.wave = SfxrWave.noise
    if (rnd_i(seed, 1) != 0) {
        s.base_freq = 0.1 + frnd(seed, 0.4)
        s.freq_ramp = -0.1 + frnd(seed, 0.4)
    } else {
        s.base_freq = 0.2 + frnd(seed, 0.7)
        s.freq_ramp = -0.2 - frnd(seed, 0.2)
    }
    s.base_freq *= s.base_freq
    if (rnd_i(seed, 4) == 0) { s.freq_ramp = 0.0; }
    if (rnd_i(seed, 2) == 0) { s.repeat_speed = 0.3 + frnd(seed, 0.5); }
    s.env_attack = 0.0
    s.env_sustain = 0.1 + frnd(seed, 0.3)
    s.env_decay = frnd(seed, 0.5)
    if (rnd_i(seed, 1) == 0) {
        s.pha_offset = -0.3 + frnd(seed, 0.9)
        s.pha_ramp = -frnd(seed, 0.3)
    }
    s.env_punch = 0.2 + frnd(seed, 0.6)
    if (rnd_i(seed, 1) != 0) {
        s.vib_strength = frnd(seed, 0.7)
        s.vib_speed = frnd(seed, 0.6)
    }
    if (rnd_i(seed, 2) == 0) {
        s.arp_speed = 0.6 + frnd(seed, 0.3)
        s.arp_mod = 0.8 - frnd(seed, 1.6)
    }
    return s
}

def sfxr_powerup(var seed : uint&) : SfxrSound {
    //! Power-up: rising pitch, often with retrigger or vibrato shimmer.
    var s = SfxrSound()
    if (rnd_i(seed, 1) != 0) {
        s.wave = SfxrWave.sawtooth
    } else {
        s.duty = frnd(seed, 0.6)
    }
    if (rnd_i(seed, 1) != 0) {
        s.base_freq = 0.2 + frnd(seed, 0.3)
        s.freq_ramp = 0.1 + frnd(seed, 0.4)
        s.repeat_speed = 0.4 + frnd(seed, 0.4)
    } else {
        s.base_freq = 0.2 + frnd(seed, 0.3)
        s.freq_ramp = 0.05 + frnd(seed, 0.2)
        if (rnd_i(seed, 1) != 0) {
            s.vib_strength = frnd(seed, 0.7)
            s.vib_speed = frnd(seed, 0.6)
        }
    }
    s.env_attack = 0.0
    s.env_sustain = frnd(seed, 0.4)
    s.env_decay = 0.1 + frnd(seed, 0.4)
    return s
}

def sfxr_hit_hurt(var seed : uint&) : SfxrSound {
    //! Hit / hurt: short noisy or square burst with a steep downward slide.
    var s = SfxrSound()
    s.wave = sfxr_wave_of(rnd_i(seed, 2))
    if (s.wave == SfxrWave.sine) { s.wave = SfxrWave.noise; }
    if (s.wave == SfxrWave.square) { s.duty = frnd(seed, 0.6); }
    s.base_freq = 0.2 + frnd(seed, 0.6)
    s.freq_ramp = -0.3 - frnd(seed, 0.4)
    s.env_attack = 0.0
    s.env_sustain = frnd(seed, 0.1)
    s.env_decay = 0.1 + frnd(seed, 0.2)
    if (rnd_i(seed, 1) != 0) { s.hpf_freq = frnd(seed, 0.3); }
    return s
}

def sfxr_jump(var seed : uint&) : SfxrSound {
    //! Jump: square voice with a quick upward slide.
    let duty = frnd(seed, 0.6)
    let base = 0.3 + frnd(seed, 0.3)
    let ramp = 0.1 + frnd(seed, 0.2)
    let sus = 0.1 + frnd(seed, 0.3)
    let dec = 0.1 + frnd(seed, 0.2)
    var s = SfxrSound(wave = SfxrWave.square, duty = duty, base_freq = base, freq_ramp = ramp,
        env_attack = 0.0, env_sustain = sus, env_decay = dec)
    if (rnd_i(seed, 1) != 0) { s.hpf_freq = frnd(seed, 0.3); }
    if (rnd_i(seed, 1) != 0) { s.lpf_freq = 1.0 - frnd(seed, 0.6); }
    return s
}

def sfxr_blip_select(var seed : uint&) : SfxrSound {
    //! Blip / select: tiny UI tick, square or saw with a high-pass edge.
    var s = SfxrSound()
    s.wave = sfxr_wave_of(rnd_i(seed, 1))
    if (s.wave == SfxrWave.square) { s.duty = frnd(seed, 0.6); }
    s.base_freq = 0.2 + frnd(seed, 0.4)
    s.env_attack = 0.0
    s.env_sustain = 0.1 + frnd(seed, 0.1)
    s.env_decay = frnd(seed, 0.2)
    s.hpf_freq = 0.1
    return s
}

def sfxr_randomize(var seed : uint&) : SfxrSound {
    //! Fully random patch across all parameters (sfxr's "Randomize" button).
    var s = SfxrSound()
    s.base_freq = (frnd(seed, 2.0) - 1.0) * (frnd(seed, 2.0) - 1.0)
    if (rnd_i(seed, 1) != 0) { s.base_freq = p3(frnd(seed, 2.0) - 1.0) + 0.5; }
    s.freq_limit = 0.0
    s.freq_ramp = p5(frnd(seed, 2.0) - 1.0)
    if (s.base_freq > 0.7 && s.freq_ramp > 0.2) { s.freq_ramp = -s.freq_ramp; }
    if (s.base_freq < 0.2 && s.freq_ramp < -0.05) { s.freq_ramp = -s.freq_ramp; }
    s.freq_dramp = p3(frnd(seed, 2.0) - 1.0)
    s.duty = frnd(seed, 2.0) - 1.0
    s.duty_ramp = p3(frnd(seed, 2.0) - 1.0)
    s.vib_strength = p3(frnd(seed, 2.0) - 1.0)
    s.vib_speed = frnd(seed, 2.0) - 1.0
    s.env_attack = p3(frnd(seed, 2.0) - 1.0)
    s.env_sustain = (frnd(seed, 2.0) - 1.0) * (frnd(seed, 2.0) - 1.0)
    s.env_decay = frnd(seed, 2.0) - 1.0
    s.env_punch = frnd(seed, 0.8) * frnd(seed, 0.8)
    if (s.env_attack + s.env_sustain + s.env_decay < 0.2) {
        s.env_sustain += 0.2 + frnd(seed, 0.3)
        s.env_decay += 0.2 + frnd(seed, 0.3)
    }
    s.lpf_resonance = frnd(seed, 2.0) - 1.0
    s.lpf_freq = 1.0 - p3(frnd(seed, 1.0))
    s.lpf_ramp = p3(frnd(seed, 2.0) - 1.0)
    if (s.lpf_freq < 0.1 && s.lpf_ramp < -0.05) { s.lpf_ramp = -s.lpf_ramp; }
    s.hpf_freq = p5(frnd(seed, 1.0))
    s.hpf_ramp = p5(frnd(seed, 2.0) - 1.0)
    s.pha_offset = p3(frnd(seed, 2.0) - 1.0)
    s.pha_ramp = p3(frnd(seed, 2.0) - 1.0)
    s.repeat_speed = frnd(seed, 2.0) - 1.0
    s.arp_speed = frnd(seed, 2.0) - 1.0
    s.arp_mod = frnd(seed, 2.0) - 1.0
    return s
}

def sfxr_mutate(snd : SfxrSound; var seed : uint&; amount : float = 0.05) : SfxrSound {
    //! Nudge each parameter by a small random delta — sfxr's "Mutate" for exploring nearby sounds.
    var s = snd
    if (rnd_i(seed, 1) != 0) { s.base_freq += frnd(seed, amount * 2.0) - amount; }
    if (rnd_i(seed, 1) != 0) { s.freq_ramp += frnd(seed, amount * 2.0) - amount; }
    if (rnd_i(seed, 1) != 0) { s.freq_dramp += frnd(seed, amount * 2.0) - amount; }
    if (rnd_i(seed, 1) != 0) { s.duty += frnd(seed, amount * 2.0) - amount; }
    if (rnd_i(seed, 1) != 0) { s.duty_ramp += frnd(seed, amount * 2.0) - amount; }
    if (rnd_i(seed, 1) != 0) { s.vib_strength += frnd(seed, amount * 2.0) - amount; }
    if (rnd_i(seed, 1) != 0) { s.vib_speed += frnd(seed, amount * 2.0) - amount; }
    if (rnd_i(seed, 1) != 0) { s.env_attack += frnd(seed, amount * 2.0) - amount; }
    if (rnd_i(seed, 1) != 0) { s.env_sustain += frnd(seed, amount * 2.0) - amount; }
    if (rnd_i(seed, 1) != 0) { s.env_decay += frnd(seed, amount * 2.0) - amount; }
    if (rnd_i(seed, 1) != 0) { s.env_punch += frnd(seed, amount * 2.0) - amount; }
    if (rnd_i(seed, 1) != 0) { s.lpf_resonance += frnd(seed, amount * 2.0) - amount; }
    if (rnd_i(seed, 1) != 0) { s.lpf_freq += frnd(seed, amount * 2.0) - amount; }
    if (rnd_i(seed, 1) != 0) { s.lpf_ramp += frnd(seed, amount * 2.0) - amount; }
    if (rnd_i(seed, 1) != 0) { s.hpf_freq += frnd(seed, amount * 2.0) - amount; }
    if (rnd_i(seed, 1) != 0) { s.hpf_ramp += frnd(seed, amount * 2.0) - amount; }
    if (rnd_i(seed, 1) != 0) { s.pha_offset += frnd(seed, amount * 2.0) - amount; }
    if (rnd_i(seed, 1) != 0) { s.pha_ramp += frnd(seed, amount * 2.0) - amount; }
    if (rnd_i(seed, 1) != 0) { s.repeat_speed += frnd(seed, amount * 2.0) - amount; }
    if (rnd_i(seed, 1) != 0) { s.arp_speed += frnd(seed, amount * 2.0) - amount; }
    if (rnd_i(seed, 1) != 0) { s.arp_mod += frnd(seed, amount * 2.0) - amount; }
    return s
}
