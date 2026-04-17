#pragma once

// Per-voice feed-forward compressor with envelope follower and soft knee.
// Parameters: threshold/ratio/knee in dB, attack/release in seconds.
// Threshold >= 0 means bypass.
// Header-only; define MA_COMPRESSOR_IMPLEMENTATION in exactly one .cpp.

#include <math.h>
#include <string.h>

struct ma_compressor {
    float threshold_db;   // threshold in dB (>= 0 = bypass)
    float ratio;          // compression ratio (1+ : 1)
    float knee_db;        // soft knee width in dB
    float attack;         // attack time seconds
    float release;        // release time seconds
    float sample_rate;
    float envelope;       // envelope follower state (linear)
    float attack_coef;    // cached 1-exp(-1/(attack*sr))
    float release_coef;   // cached 1-exp(-1/(release*sr))
    float cached_attack;
    float cached_release;
};

void ma_compressor_init(ma_compressor * c, float sample_rate);
void ma_compressor_process(ma_compressor * c, float * buf, int frame_count);

#ifdef MA_COMPRESSOR_IMPLEMENTATION

static void ma_compressor_update_coefs(ma_compressor * c) {
    if (c->attack != c->cached_attack) {
        c->attack_coef = c->attack > 0.0f ? 1.0f - expf(-1.0f / (c->attack * c->sample_rate)) : 1.0f;
        c->cached_attack = c->attack;
    }
    if (c->release != c->cached_release) {
        c->release_coef = c->release > 0.0f ? 1.0f - expf(-1.0f / (c->release * c->sample_rate)) : 1.0f;
        c->cached_release = c->release;
    }
}

void ma_compressor_init(ma_compressor * c, float sample_rate) {
    memset(c, 0, sizeof(*c));
    c->sample_rate = sample_rate;
    c->threshold_db = 1.0f;   // bypass
    c->ratio = 10.0f;
    c->knee_db = 10.0f;
    c->attack = 0.005f;
    c->release = 0.05f;
    c->envelope = 0.0f;
    c->cached_attack = -1.0f;
    c->cached_release = -1.0f;
    ma_compressor_update_coefs(c);
}

void ma_compressor_process(ma_compressor * c, float * buf, int frame_count) {
    if (c->threshold_db >= 0.0f) return;   // bypass
    ma_compressor_update_coefs(c);
    float ratio = c->ratio < 1.0f ? 1.0f : c->ratio;
    float thr_db = c->threshold_db;
    float knee = c->knee_db < 0.0f ? 0.0f : c->knee_db;
    float half_knee = knee * 0.5f;
    float inv_ratio = 1.0f / ratio;
    float env = c->envelope;
    float ac = c->attack_coef;
    float rc = c->release_coef;
    const float EPS = 1.0e-9f;
    for (int f = 0; f < frame_count; f++) {
        float l = buf[f * 2];
        float r = buf[f * 2 + 1];
        // peak detector over max(|L|, |R|)
        float peak = fabsf(l) > fabsf(r) ? fabsf(l) : fabsf(r);
        // envelope follower
        float coef = peak > env ? ac : rc;
        env += coef * (peak - env);
        if (env < EPS) env = EPS;
        // gain computer (soft-knee)
        float level_db = 20.0f * log10f(env);
        float over = level_db - thr_db;
        float gain_reduction_db;
        if (knee > 0.0f && over > -half_knee && over < half_knee) {
            // soft knee: quadratic interpolation
            float x = over + half_knee;
            gain_reduction_db = (1.0f - inv_ratio) * x * x / (2.0f * knee);
        } else if (over >= half_knee) {
            gain_reduction_db = (1.0f - inv_ratio) * over;
        } else {
            gain_reduction_db = 0.0f;
        }
        float gain = powf(10.0f, -gain_reduction_db / 20.0f);
        buf[f * 2]     = l * gain;
        buf[f * 2 + 1] = r * gain;
    }
    c->envelope = env;
}

#endif // MA_COMPRESSOR_IMPLEMENTATION
