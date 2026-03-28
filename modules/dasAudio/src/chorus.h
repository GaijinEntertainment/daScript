#pragma once

// 4-tap stereo chorus with modulated delay lines.
// Header-only; define MA_CHORUS_IMPLEMENTATION in exactly one .cpp before including.

#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// 4800 samples = 100ms at 48kHz — enough for max delay (50ms) + depth (20ms)
#define MA_CHORUS_BUF_SIZE 4800

struct ma_chorus_config {
    float rate;       // LFO frequency in Hz (0.1 - 5.0), default 1.0
    float depth;      // LFO excursion in ms (0.0 - 10.0), default 3.0
    float feedback;   // feedback coefficient (0.0 - 0.9), default 0.3
    float delay_ms;   // center delay in ms (1.0 - 50.0), default 7.0
    float wet;        // wet mix (0.0 - 1.0), default 0.5
};

struct ma_chorus {
    ma_chorus_config config;
    float sample_rate;
    float delay_buf_l[MA_CHORUS_BUF_SIZE];
    float delay_buf_r[MA_CHORUS_BUF_SIZE];
    int buf_size;       // actual usable size (<= MA_CHORUS_BUF_SIZE)
    int write_pos;
    float lfo_phase;    // 0..1
};

static inline ma_chorus_config ma_chorus_config_default(void) {
    ma_chorus_config c;
    c.rate     = 0.3f;
    c.depth    = 8.0f;
    c.feedback = 0.0f;
    c.delay_ms = 10.0f;
    c.wet      = 0.5f;
    return c;
}

void ma_chorus_init(ma_chorus * chorus, float sample_rate);
void ma_chorus_process(ma_chorus * chorus, const float * input, float * output, int frame_count);
void ma_chorus_set_config(ma_chorus * chorus, const ma_chorus_config * config);

// ─── Implementation ───

#ifdef MA_CHORUS_IMPLEMENTATION

void ma_chorus_init(ma_chorus * chorus, float sample_rate) {
    memset(chorus, 0, sizeof(*chorus));
    chorus->config = ma_chorus_config_default();
    chorus->sample_rate = sample_rate;
    chorus->buf_size = (int)(sample_rate * 0.1f) + 1;
    if ( chorus->buf_size > MA_CHORUS_BUF_SIZE ) chorus->buf_size = MA_CHORUS_BUF_SIZE;
    chorus->write_pos = 0;
    chorus->lfo_phase = 0.0f;
}

void ma_chorus_set_config(ma_chorus * chorus, const ma_chorus_config * config) {
    chorus->config = *config;
    // clamp
    if (chorus->config.rate < 0.01f) chorus->config.rate = 0.01f;
    if (chorus->config.rate > 10.0f) chorus->config.rate = 10.0f;
    if (chorus->config.depth < 0.0f) chorus->config.depth = 0.0f;
    if (chorus->config.depth > 20.0f) chorus->config.depth = 20.0f;
    if (chorus->config.feedback < 0.0f) chorus->config.feedback = 0.0f;
    if (chorus->config.feedback > 0.95f) chorus->config.feedback = 0.95f;
    if (chorus->config.delay_ms < 0.5f) chorus->config.delay_ms = 0.5f;
    if (chorus->config.delay_ms > 50.0f) chorus->config.delay_ms = 50.0f;
    if (chorus->config.wet < 0.0f) chorus->config.wet = 0.0f;
    if (chorus->config.wet > 1.0f) chorus->config.wet = 1.0f;
}

void ma_chorus_process(ma_chorus * chorus, const float * input, float * output, int frame_count) {
    const float sr = chorus->sample_rate;
    const float rate = chorus->config.rate;
    const float depth_samples = chorus->config.depth * 0.001f * sr;
    const float center_samples = chorus->config.delay_ms * 0.001f * sr;
    const float feedback = chorus->config.feedback;
    const float wet = chorus->config.wet;
    const float dry = 1.0f - wet;
    const int buf_size = chorus->buf_size;
    const float phase_inc = rate / sr;

    // 4 tap phase offsets (0, 0.25, 0.5, 0.75) in 0..1 range
    static const float tap_offsets[4] = { 0.0f, 0.25f, 0.5f, 0.75f };

    float * dl = chorus->delay_buf_l;
    float * dr = chorus->delay_buf_r;
    int wp = chorus->write_pos;
    float phase = chorus->lfo_phase;

    for (int f = 0; f < frame_count; f++) {
        float in_l = input[f * 2];
        float in_r = input[f * 2 + 1];

        // write input + feedback into delay lines
        dl[wp] = in_l;
        dr[wp] = in_r;

        // sum 4 taps with modulated delay
        float wet_l = 0.0f, wet_r = 0.0f;
        for (int t = 0; t < 4; t++) {
            float tap_phase = phase + tap_offsets[t];
            if (tap_phase >= 1.0f) tap_phase -= 1.0f;
            float lfo = sinf(tap_phase * (float)(2.0 * M_PI));
            float delay = center_samples + depth_samples * lfo;
            if (delay < 0.5f) delay = 0.5f;
            if (delay > (float)(buf_size - 1)) delay = (float)(buf_size - 1);

            // fractional read position with linear interpolation
            float read_pos = (float)wp - delay;
            if (read_pos < 0.0f) read_pos += (float)buf_size;
            int idx0 = (int)read_pos;
            int idx1 = idx0 + 1;
            if (idx1 >= buf_size) idx1 = 0;
            float frac = read_pos - (float)idx0;

            wet_l += dl[idx0] + (dl[idx1] - dl[idx0]) * frac;
            wet_r += dr[idx0] + (dr[idx1] - dr[idx0]) * frac;
        }
        wet_l *= 0.25f;  // average 4 taps
        wet_r *= 0.25f;

        // feedback: add wet signal back into delay
        dl[wp] += wet_l * feedback;
        dr[wp] += wet_r * feedback;

        // output: dry + wet mix
        output[f * 2]     = in_l * dry + wet_l * wet;
        output[f * 2 + 1] = in_r * dry + wet_r * wet;

        wp++;
        if (wp >= buf_size) wp = 0;
        phase += phase_inc;
        if (phase >= 1.0f) phase -= 1.0f;
    }

    chorus->write_pos = wp;
    chorus->lfo_phase = phase;
}

#endif // MA_CHORUS_IMPLEMENTATION
