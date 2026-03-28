#ifndef sf2_voice_h
#define sf2_voice_h

// SF2 voice renderer — single-header, ma_ style
// Handles the per-sample hot loop: interpolation + envelope + filter + pan
// Zone lookup, MIDI routing, and voice management stay in daScript

#include <math.h>
#include <string.h>

#ifndef MA_PI
#define MA_PI 3.14159265358979323846
#endif

// ─── Envelope ───

enum {
    MA_SF2_ENV_DELAY,
    MA_SF2_ENV_ATTACK,
    MA_SF2_ENV_HOLD,
    MA_SF2_ENV_DECAY,
    MA_SF2_ENV_SUSTAIN,
    MA_SF2_ENV_RELEASE,
    MA_SF2_ENV_FINISHED
};

struct ma_sf2_envelope {
    int     stage;
    float   level;
    float   release_level;
    float   slope;
    int     is_exponential;
    int     is_amp_env;
    int     midi_velocity;
    float   samples_until_next;     // countdown for delay/hold
    // parameters (in seconds)
    float   delay_sec;
    float   attack_sec;
    float   hold_sec;
    float   decay_sec;
    float   sustain_level;
    float   release_sec;
};

void ma_sf2_envelope_init ( ma_sf2_envelope * env );
void ma_sf2_envelope_start ( ma_sf2_envelope * env, float sample_rate );
float ma_sf2_envelope_tick ( ma_sf2_envelope * env, int released, float sample_rate );
void ma_sf2_envelope_release ( ma_sf2_envelope * env, float sample_rate );

// ─── Biquad Filter (transposed direct form II) ───

struct ma_sf2_biquad {
    double  q_inv;
    double  a0, a1, b1, b2;
    double  z1, z2;
    int     active;
};

void ma_sf2_biquad_setup ( ma_sf2_biquad * bq, float fc_normalized );

// ─── Voice ───

struct ma_sf2_voice {
    // sample addressing (left / mono channel)
    int     sample_start;
    int     sample_end;
    int     loop_start;
    int     loop_end;
    int     loop_mode;          // 0=none, 1=continuous, 3=sustain-loop
    // right channel (stereo-linked SF2 samples only; all zero when mono)
    int     sample_start_r;
    int     sample_end_r;
    int     loop_start_r;
    int     loop_end_r;
    int     stereo;             // 0=mono, 1=stereo-linked pair
    // playback
    double  position;
    double  phase_inc;          // base pitch (computed from root key + tuning)
    // envelopes
    ma_sf2_envelope vol_env;
    ma_sf2_envelope mod_env;
    // filter
    ma_sf2_biquad   filter;
    float   initial_filter_fc;  // in cents
    float   initial_filter_q;   // in dB
    // modulation routing amounts
    float   mod_env_to_pitch;
    float   mod_lfo_to_pitch;
    float   vib_lfo_to_pitch;
    float   mod_env_to_filter_fc;
    float   mod_lfo_to_filter_fc;
    float   mod_lfo_to_volume;
    // LFO state
    float   mod_lfo_phase;
    float   mod_lfo_freq;
    float   mod_lfo_delay;
    float   mod_lfo_elapsed;
    float   vib_lfo_phase;
    float   vib_lfo_freq;
    float   vib_lfo_delay;
    float   vib_lfo_elapsed;
    // real-time channel modulation (set by MIDI player per chunk, not saved/restored)
    float   pitch_bend_cents;   // pitch bend in cents (e.g. ±200 for ±2 semitones)
    // gain
    float   attenuation;        // linear gain (includes velocity)
    float   pan;                // -1 to 1
    // state
    int     released;
    int     finished;
    float   sample_rate;
};

void ma_sf2_voice_init ( ma_sf2_voice * voice, float sample_rate );
void ma_sf2_voice_note_off ( ma_sf2_voice * voice );
void ma_sf2_voice_end_quick ( ma_sf2_voice * voice );
void ma_sf2_voice_render ( ma_sf2_voice * voice, const short * sample_data, int sample_data_len,
                           float * output, int output_offset, int frame_count );
int  ma_sf2_voice_is_finished ( const ma_sf2_voice * voice );
// Render voice, split output into dry + reverb send buffers (additive).
// dry_gain = 1.0 - reverb_send, wet_gain = reverb_send.
void ma_sf2_voice_render_send ( ma_sf2_voice * voice, const short * sample_data, int sample_data_len,
                                float * dry_output, float * reverb_output, int output_offset, int frame_count,
                                float dry_gain, float wet_gain );
// Render voice, split output into dry + reverb + chorus send buffers (additive).
void ma_sf2_voice_render_send2 ( ma_sf2_voice * voice, const short * sample_data, int sample_data_len,
                                 float * dry_output, float * reverb_output, float * chorus_output,
                                 int output_offset, int frame_count,
                                 float dry_gain, float reverb_gain, float chorus_gain );

// ─── Implementation ───

#ifdef MINIAUDIO_IMPLEMENTATION

static float ma_sf2_cents_to_hz ( int cents ) {
    return 8.176f * powf(2.0f, (float)cents / 1200.0f);
}

// ─── Envelope Implementation ───

void ma_sf2_envelope_init ( ma_sf2_envelope * env ) {
    memset(env, 0, sizeof(*env));
    env->stage = MA_SF2_ENV_DELAY;
    env->sustain_level = 1.0f;
    env->attack_sec = 0.001f;
    env->decay_sec = 0.001f;
    env->release_sec = 0.001f;
}

static void ma_sf2_envelope_next_segment ( ma_sf2_envelope * env, float sample_rate ) {
    switch ( env->stage ) {
    case MA_SF2_ENV_DELAY: {
        float attack_samples = env->attack_sec * sample_rate;
        if ( env->is_amp_env )
            attack_samples *= (145 - env->midi_velocity) / 144.0f;
        env->stage = MA_SF2_ENV_ATTACK;
        env->is_exponential = 0;
        env->level = 0.0f;
        env->slope = attack_samples > 0.0f ? 1.0f / attack_samples : 1.0f;
        break;
    }
    case MA_SF2_ENV_ATTACK:
        env->stage = MA_SF2_ENV_HOLD;
        env->is_exponential = 0;
        env->level = 1.0f;
        env->slope = 0.0f;
        env->samples_until_next = env->hold_sec * sample_rate;
        break;
    case MA_SF2_ENV_HOLD: {
        float decay_samples = env->decay_sec * sample_rate;
        env->stage = MA_SF2_ENV_DECAY;
        if ( env->is_amp_env && decay_samples > 0.0f ) {
            env->slope = expf(-9.226f / decay_samples);
            env->is_exponential = 1;
        } else {
            env->is_exponential = 0;
            env->slope = decay_samples > 0.0f ? -1.0f / decay_samples : -1.0f;
        }
        break;
    }
    case MA_SF2_ENV_DECAY:
        env->stage = MA_SF2_ENV_SUSTAIN;
        env->level = env->sustain_level;
        env->slope = 0.0f;
        env->is_exponential = 0;
        break;
    case MA_SF2_ENV_SUSTAIN: {
        float release_samples = env->release_sec * sample_rate;
        env->stage = MA_SF2_ENV_RELEASE;
        if ( env->is_amp_env && release_samples > 0.0f ) {
            env->slope = expf(-9.226f / release_samples);
            env->is_exponential = 1;
        } else {
            env->is_exponential = 0;
            env->slope = release_samples > 0.0f ? -env->level / release_samples : -1.0f;
        }
        break;
    }
    case MA_SF2_ENV_RELEASE:
        env->stage = MA_SF2_ENV_FINISHED;
        env->level = 0.0f;
        env->slope = 0.0f;
        env->is_exponential = 0;
        break;
    }
}

void ma_sf2_envelope_start ( ma_sf2_envelope * env, float sample_rate ) {
    env->stage = MA_SF2_ENV_DELAY;
    env->level = 0.0f;
    float delay_samples = env->delay_sec * sample_rate;
    if ( delay_samples <= 0.0f ) {
        ma_sf2_envelope_next_segment(env, sample_rate);
    } else {
        env->slope = 0.0f;
        env->is_exponential = 0;
        env->samples_until_next = delay_samples;
    }
}

void ma_sf2_envelope_release ( ma_sf2_envelope * env, float sample_rate ) {
    if ( env->stage >= MA_SF2_ENV_RELEASE ) return;
    env->release_level = env->level;
    float release_samples = env->release_sec * sample_rate;
    env->stage = MA_SF2_ENV_RELEASE;
    if ( env->is_amp_env && release_samples > 0.0f ) {
        env->slope = expf(-9.226f / release_samples);
        env->is_exponential = 1;
    } else {
        env->is_exponential = 0;
        env->slope = release_samples > 0.0f ? -env->level / release_samples : -1.0f;
    }
}

float ma_sf2_envelope_tick ( ma_sf2_envelope * env, int released, float sample_rate ) {
    if ( released && env->stage < MA_SF2_ENV_RELEASE )
        ma_sf2_envelope_release(env, sample_rate);
    if ( env->stage == MA_SF2_ENV_FINISHED ) {
        env->level = 0.0f;
        return 0.0f;
    }
    // advance
    if ( env->is_exponential )
        env->level *= env->slope;
    else
        env->level += env->slope;
    if ( env->level < 0.0f ) env->level = 0.0f;
    if ( env->level > 1.0f ) env->level = 1.0f;
    // transitions
    switch ( env->stage ) {
    case MA_SF2_ENV_DELAY:
        env->samples_until_next -= 1.0f;
        if ( env->samples_until_next <= 0.0f )
            ma_sf2_envelope_next_segment(env, sample_rate);
        break;
    case MA_SF2_ENV_ATTACK:
        if ( env->level >= 1.0f ) {
            env->level = 1.0f;
            ma_sf2_envelope_next_segment(env, sample_rate);
            if ( env->stage == MA_SF2_ENV_HOLD && env->hold_sec <= 0.0f )
                ma_sf2_envelope_next_segment(env, sample_rate);
        }
        break;
    case MA_SF2_ENV_HOLD:
        env->samples_until_next -= 1.0f;
        if ( env->samples_until_next <= 0.0f )
            ma_sf2_envelope_next_segment(env, sample_rate);
        break;
    case MA_SF2_ENV_DECAY:
        if ( env->level <= env->sustain_level ) {
            env->level = env->sustain_level;
            ma_sf2_envelope_next_segment(env, sample_rate);
        }
        break;
    case MA_SF2_ENV_RELEASE:
        if ( env->level <= 0.001f ) {
            env->level = 0.0f;
            env->stage = MA_SF2_ENV_FINISHED;
        }
        break;
    }
    return env->level;
}

// ─── Biquad Implementation ───

void ma_sf2_biquad_setup ( ma_sf2_biquad * bq, float fc_normalized ) {
    if ( fc_normalized >= 0.499f ) {
        bq->active = 0;
        return;
    }
    bq->active = 1;
    double K = tan(MA_PI * (double)fc_normalized);
    double KK = K * K;
    double norm = 1.0 / (1.0 + K * bq->q_inv + KK);
    bq->a0 = KK * norm;
    bq->a1 = 2.0 * bq->a0;
    bq->b1 = 2.0 * (KK - 1.0) * norm;
    bq->b2 = (1.0 - K * bq->q_inv + KK) * norm;
}

// Note: caller must check filter.active before calling — no redundant branch here
static float ma_sf2_biquad_tick ( ma_sf2_biquad * bq, float input ) {
    double inp = (double)input;
    double out = inp * bq->a0 + bq->z1;
    bq->z1 = inp * bq->a1 + bq->z2 - bq->b1 * out;
    bq->z2 = inp * bq->a0 - bq->b2 * out;
    return (float)out;
}

// ─── LFO helpers ───

static float ma_sf2_lfo_tick ( float * phase, float * elapsed, float freq, float delay, float dt ) {
    *elapsed += dt;
    if ( *elapsed < delay ) return 0.0f;
    *phase += freq * dt;
    if ( *phase > 1.0f ) *phase -= floorf(*phase);
    return fabsf(*phase * 4.0f - 2.0f) - 1.0f;
}

// ─── Voice Implementation ───

void ma_sf2_voice_init ( ma_sf2_voice * voice, float sample_rate ) {
    memset(voice, 0, sizeof(*voice));
    voice->sample_rate = sample_rate;
    voice->attenuation = 1.0f;
    ma_sf2_envelope_init(&voice->vol_env);
    ma_sf2_envelope_init(&voice->mod_env);
}

void ma_sf2_voice_note_off ( ma_sf2_voice * voice ) {
    voice->released = 1;
}

void ma_sf2_voice_end_quick ( ma_sf2_voice * voice ) {
    voice->vol_env.release_sec = 0.01f;
    voice->mod_env.release_sec = 0.01f;
    voice->released = 1;
}

int ma_sf2_voice_is_finished ( const ma_sf2_voice * voice ) {
    return voice->finished;
}

#define MA_SF2_BLOCK_SIZE 64
#define MA_SF2_CROSSFADE_LEN 64

void ma_sf2_voice_render ( ma_sf2_voice * voice, const short * sample_data, int sample_data_len,
                           float * output, int output_offset, int frame_count ) {
    if ( voice->finished ) return;

    const float sr = voice->sample_rate;
    const float iSr = 1.0f / sr;

    // pan law: sqrt(0.5 ± pan) for mono; for stereo-linked, pan acts as balance
    float l_gain, r_gain;
    if ( voice->stereo ) {
        // stereo-linked: both channels play, pan acts as balance (0 = neutral)
        // SF2 zones set pan=-500(L)/ +500(R); we take from the L zone, so override to center
        l_gain = voice->attenuation;
        r_gain = voice->attenuation;
    } else {
        float pan05 = voice->pan * 0.5f;
        if ( pan05 <= -0.5f )       { l_gain = voice->attenuation; r_gain = 0.0f; }
        else if ( pan05 >= 0.5f )   { l_gain = 0.0f; r_gain = voice->attenuation; }
        else { l_gain = sqrtf(0.5f - pan05) * voice->attenuation;
               r_gain = sqrtf(0.5f + pan05) * voice->attenuation; }
    }

    // hoist invariants
    const int has_mod_env_pitch = voice->mod_env_to_pitch != 0.0f;
    const int has_mod_lfo_pitch = voice->mod_lfo_to_pitch != 0.0f;
    const int has_vib_lfo_pitch = voice->vib_lfo_to_pitch != 0.0f;
    const int has_any_pitch_mod = has_mod_env_pitch | has_mod_lfo_pitch | has_vib_lfo_pitch;
    const int has_mod_env_fc = voice->mod_env_to_filter_fc != 0.0f;
    const int has_mod_lfo_fc = voice->mod_lfo_to_filter_fc != 0.0f;
    const int dynamic_filter = has_mod_lfo_fc | has_mod_env_fc;
    const int has_tremolo = voice->mod_lfo_to_volume != 0.0f;
    const int has_modulation = has_any_pitch_mod | dynamic_filter | has_tremolo;
    const int loop_mode = voice->loop_mode;
    const int sample_start = voice->sample_start;
    const int sample_end = voice->sample_end;
    const int loop_start = voice->loop_start;
    const int loop_end = voice->loop_end;
    const int loop_len = loop_end - loop_start;
    const int do_loop = (loop_mode == 1 || loop_mode == 3) && loop_len > 0;
    const int crossfade_len = (do_loop && loop_len > MA_SF2_CROSSFADE_LEN * 2) ? MA_SF2_CROSSFADE_LEN : 0;
    const float inv_crossfade = crossfade_len > 0 ? 1.0f / (float)crossfade_len : 0.0f;
    const double base_phase_inc = voice->phase_inc;
    const float inv_32768 = 1.0f / 32768.0f;
    // stereo-linked right channel invariants
    const int is_stereo = voice->stereo;
    const int sample_start_r = voice->sample_start_r;
    const int sample_end_r = voice->sample_end_r;
    const int loop_start_r = voice->loop_start_r;
    const int loop_end_r = voice->loop_end_r;
    const int loop_len_r = loop_end_r - loop_start_r;
    const int crossfade_len_r = (is_stereo && do_loop && loop_len_r > MA_SF2_CROSSFADE_LEN * 2) ? MA_SF2_CROSSFADE_LEN : 0;
    const float inv_crossfade_r = crossfade_len_r > 0 ? 1.0f / (float)crossfade_len_r : 0.0f;

    int f = 0;
    while ( f < frame_count ) {
        int blk = frame_count - f;
        if ( blk > MA_SF2_BLOCK_SIZE ) blk = MA_SF2_BLOCK_SIZE;

        // ── per-block: envelope + LFO + modulation ──
        float vol = 0.0f;
        for ( int i = 0; i < blk; i++ )
            vol = ma_sf2_envelope_tick(&voice->vol_env, voice->released, sr);
        if ( voice->vol_env.stage == MA_SF2_ENV_FINISHED ) {
            voice->finished = 1;
            return;
        }
        if ( vol < 0.00001f ) {
            voice->position += base_phase_inc * (double)blk;
            f += blk;
            continue;
        }

        float mod = 0.0f, mlfo = 0.0f, vlfo = 0.0f;
        if ( has_modulation ) {
            for ( int i = 0; i < blk; i++ )
                mod = ma_sf2_envelope_tick(&voice->mod_env, voice->released, sr);
            float lfo_dt = iSr * (float)blk;
            mlfo = ma_sf2_lfo_tick(&voice->mod_lfo_phase, &voice->mod_lfo_elapsed,
                                    voice->mod_lfo_freq, voice->mod_lfo_delay, lfo_dt);
            vlfo = ma_sf2_lfo_tick(&voice->vib_lfo_phase, &voice->vib_lfo_elapsed,
                                    voice->vib_lfo_freq, voice->vib_lfo_delay, lfo_dt);
        }

        // pitch for this block
        double phase_inc = base_phase_inc;
        {
            float pitch_mod_cents = voice->pitch_bend_cents;
            if ( has_mod_env_pitch ) pitch_mod_cents += voice->mod_env_to_pitch * mod;
            if ( has_mod_lfo_pitch ) pitch_mod_cents += voice->mod_lfo_to_pitch * mlfo;
            if ( has_vib_lfo_pitch ) pitch_mod_cents += voice->vib_lfo_to_pitch * vlfo;
            if ( pitch_mod_cents != 0.0f )
                phase_inc = base_phase_inc * (double)powf(2.0f, pitch_mod_cents / 1200.0f);
        }

        // filter coefficients for this block
        if ( voice->filter.active || dynamic_filter ) {
            float fres = voice->initial_filter_fc;
            if ( has_mod_env_fc ) fres += voice->mod_env_to_filter_fc * mod;
            if ( has_mod_lfo_fc ) fres += voice->mod_lfo_to_filter_fc * mlfo;
            float fc_norm = fres <= 13500.0f ? ma_sf2_cents_to_hz((int)fres) / sr : 1.0f;
            voice->filter.active = fc_norm < 0.499f;
            if ( voice->filter.active ) ma_sf2_biquad_setup(&voice->filter, fc_norm);
        }

        // gain for this block
        float gain_l = vol * l_gain;
        float gain_r = vol * r_gain;
        if ( has_tremolo ) {
            float trem = powf(10.0f, voice->mod_lfo_to_volume * mlfo / -200.0f);
            gain_l *= trem;
            gain_r *= trem;
        }

        // ── per-sample inner loop ──
        const int use_filter = voice->filter.active;
        const int looping_now = do_loop && !(loop_mode == 3 && voice->released);

        for ( int s = 0; s < blk; s++ ) {
            int pos_i = (int)voice->position;
            float frac = (float)(voice->position - (double)pos_i);

            // ── left / mono channel interpolation ──
            int idx = sample_start + pos_i;
            float sample_l = 0.0f;
            if ( idx >= 0 && idx + 1 < sample_data_len && idx + 1 < sample_end ) {
                float sm1 = (idx - 1 >= 0) ? (float)sample_data[idx - 1] * inv_32768 : (float)sample_data[idx] * inv_32768;
                float s0  = (float)sample_data[idx] * inv_32768;
                float s1  = (float)sample_data[idx + 1] * inv_32768;
                float s2  = (idx + 2 < sample_data_len) ? (float)sample_data[idx + 2] * inv_32768 : s1;
                float c0  = s0;
                float c1  = 0.5f * (s1 - sm1);
                float c2  = sm1 - 2.5f * s0 + 2.0f * s1 - 0.5f * s2;
                float c3  = 0.5f * (s2 - sm1) + 1.5f * (s0 - s1);
                sample_l = ((c3 * frac + c2) * frac + c1) * frac + c0;
                // crossfade near loop_end
                if ( crossfade_len > 0 && looping_now ) {
                    int dist = loop_end - idx;
                    if ( dist >= 0 && dist < crossfade_len ) {
                        float fade = (float)(crossfade_len - dist) * inv_crossfade;
                        int wi = loop_start + (idx - (loop_end - crossfade_len));
                        if ( wi >= loop_start && wi + 1 < sample_data_len ) {
                            float wm1 = (wi - 1 >= 0) ? (float)sample_data[wi - 1] * inv_32768 : (float)sample_data[wi] * inv_32768;
                            float w0  = (float)sample_data[wi] * inv_32768;
                            float w1  = (float)sample_data[wi + 1] * inv_32768;
                            float w2  = (wi + 2 < sample_data_len) ? (float)sample_data[wi + 2] * inv_32768 : w1;
                            float wc0 = w0;
                            float wc1 = 0.5f * (w1 - wm1);
                            float wc2 = wm1 - 2.5f * w0 + 2.0f * w1 - 0.5f * w2;
                            float wc3 = 0.5f * (w2 - wm1) + 1.5f * (w0 - w1);
                            float wsample = ((wc3 * frac + wc2) * frac + wc1) * frac + wc0;
                            sample_l = sample_l * (1.0f - fade) + wsample * fade;
                        }
                    }
                }
            } else if ( !looping_now ) {
                voice->finished = 1;
                goto done;
            }

            // ── right channel interpolation (stereo-linked only) ──
            float sample_r;
            if ( is_stereo ) {
                int idx_r = sample_start_r + pos_i;
                sample_r = 0.0f;
                if ( idx_r >= 0 && idx_r + 1 < sample_data_len && idx_r + 1 < sample_end_r ) {
                    float sm1 = (idx_r - 1 >= 0) ? (float)sample_data[idx_r - 1] * inv_32768 : (float)sample_data[idx_r] * inv_32768;
                    float s0  = (float)sample_data[idx_r] * inv_32768;
                    float s1  = (float)sample_data[idx_r + 1] * inv_32768;
                    float s2  = (idx_r + 2 < sample_data_len) ? (float)sample_data[idx_r + 2] * inv_32768 : s1;
                    float c0  = s0;
                    float c1  = 0.5f * (s1 - sm1);
                    float c2  = sm1 - 2.5f * s0 + 2.0f * s1 - 0.5f * s2;
                    float c3  = 0.5f * (s2 - sm1) + 1.5f * (s0 - s1);
                    sample_r = ((c3 * frac + c2) * frac + c1) * frac + c0;
                    // crossfade near loop_end_r
                    if ( crossfade_len_r > 0 && looping_now ) {
                        int dist = loop_end_r - idx_r;
                        if ( dist >= 0 && dist < crossfade_len_r ) {
                            float fade = (float)(crossfade_len_r - dist) * inv_crossfade_r;
                            int wi = loop_start_r + (idx_r - (loop_end_r - crossfade_len_r));
                            if ( wi >= loop_start_r && wi + 1 < sample_data_len ) {
                                float wm1 = (wi - 1 >= 0) ? (float)sample_data[wi - 1] * inv_32768 : (float)sample_data[wi] * inv_32768;
                                float w0  = (float)sample_data[wi] * inv_32768;
                                float w1  = (float)sample_data[wi + 1] * inv_32768;
                                float w2  = (wi + 2 < sample_data_len) ? (float)sample_data[wi + 2] * inv_32768 : w1;
                                float wc0 = w0;
                                float wc1 = 0.5f * (w1 - wm1);
                                float wc2 = wm1 - 2.5f * w0 + 2.0f * w1 - 0.5f * w2;
                                float wc3 = 0.5f * (w2 - wm1) + 1.5f * (w0 - w1);
                                float wsample = ((wc3 * frac + wc2) * frac + wc1) * frac + wc0;
                                sample_r = sample_r * (1.0f - fade) + wsample * fade;
                            }
                        }
                    }
                }
            } else {
                sample_r = sample_l;  // mono: same sample to both channels
            }

            if ( use_filter ) {
                sample_l = ma_sf2_biquad_tick(&voice->filter, sample_l);
                if ( is_stereo ) sample_r = ma_sf2_biquad_tick(&voice->filter, sample_r);
            }

            int ofs = output_offset + (f + s) * 2;
            output[ofs]     += sample_l * gain_l;
            output[ofs + 1] += sample_r * gain_r;
            voice->position += phase_inc;

            // loop management (driven by left channel's loop points)
            if ( looping_now ) {
                if ( sample_start + (int)voice->position >= loop_end )
                    voice->position -= (double)loop_len;
            } else if ( sample_start + (int)voice->position >= sample_end ) {
                voice->finished = 1;
                goto done;
            }
        }

        f += blk;
    }
done:;
}

void ma_sf2_voice_render_send ( ma_sf2_voice * voice, const short * sample_data, int sample_data_len,
                                float * dry_output, float * reverb_output, int output_offset, int frame_count,
                                float dry_gain, float wet_gain ) {
    // Render to a stack-local temp buffer, then split into dry + wet.
    // Stack buffer for typical chunk sizes (up to 4800 stereo samples = 38.4KB).
    float temp[9600];
    const int n = frame_count * 2;
    if ( n > 9600 ) return;  // safety: skip absurdly large chunks
    for ( int i = 0; i < n; i++ ) temp[i] = 0.0f;
    ma_sf2_voice_render(voice, sample_data, sample_data_len, temp, 0, frame_count);
    for ( int i = 0; i < n; i++ ) {
        dry_output[output_offset + i]    += temp[i] * dry_gain;
        reverb_output[output_offset + i] += temp[i] * wet_gain;
    }
}

void ma_sf2_voice_render_send2 ( ma_sf2_voice * voice, const short * sample_data, int sample_data_len,
                                 float * dry_output, float * reverb_output, float * chorus_output,
                                 int output_offset, int frame_count,
                                 float dry_gain, float reverb_gain, float chorus_gain ) {
    float temp[9600];
    const int n = frame_count * 2;
    if ( n > 9600 ) return;
    for ( int i = 0; i < n; i++ ) temp[i] = 0.0f;
    ma_sf2_voice_render(voice, sample_data, sample_data_len, temp, 0, frame_count);
    for ( int i = 0; i < n; i++ ) {
        dry_output[output_offset + i]    += temp[i] * dry_gain;
        reverb_output[output_offset + i] += temp[i] * reverb_gain;
        chorus_output[output_offset + i] += temp[i] * chorus_gain;
    }
}

#endif // MINIAUDIO_IMPLEMENTATION

#endif // sf2_voice_h
