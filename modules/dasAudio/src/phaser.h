#pragma once

// Per-voice phaser: classic multi-stage 1-pole all-pass topology (MXR Phase 90 style).
// 4 cascaded all-pass sections produce 2 swept notches when summed with dry.
// depth controls wet/dry mix; sweep controls LFO range in cents; center is the
// geometric mean of the sweep; rate is the LFO Hz. Triangle LFO.
// Header-only; define MA_PHASER_IMPLEMENTATION in exactly one .cpp.

#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define MA_PHASER_STAGES 4

struct ma_phaser {
    float rate;            // LFO rate Hz
    float depth;           // wet/dry mix 0..1 (deeper notch at higher values)
    float center;          // center frequency Hz
    float sweep;           // sweep range cents (one-sided)
    float sample_rate;
    float lfo_phase;       // 0..1
    // 1-pole all-pass coef (shared across stages; modulated by LFO)
    float a;
    // state per stage, per channel
    float x_l[MA_PHASER_STAGES], y_l[MA_PHASER_STAGES];
    float x_r[MA_PHASER_STAGES], y_r[MA_PHASER_STAGES];
    // feedback tap (last-stage output from previous sample, per channel)
    float fb_l, fb_r;
    int update_counter;    // refresh coef every N samples
};

void ma_phaser_init(ma_phaser * ph, float sample_rate);
void ma_phaser_process(ma_phaser * ph, float * buf, int frame_count);

#ifdef MA_PHASER_IMPLEMENTATION

static void ma_phaser_setup_coef(ma_phaser * ph, float freq) {
    if (freq < 20.0f) freq = 20.0f;
    float nyq = ph->sample_rate * 0.49f;
    if (freq > nyq) freq = nyq;
    // 1-pole all-pass: y[n] = -a*x[n] + x[n-1] + a*y[n-1]
    // where a = (1 - tan(w/2)) / (1 + tan(w/2)), w = 2*pi*freq/sr.
    float w = (float)M_PI * freq / ph->sample_rate;  // = w/2 when w=2πf/sr
    float t = tanf(w);
    ph->a = (1.0f - t) / (1.0f + t);
}

void ma_phaser_init(ma_phaser * ph, float sample_rate) {
    memset(ph, 0, sizeof(*ph));
    ph->sample_rate = sample_rate;
    ph->rate = 1.0f;
    ph->depth = 0.75f;
    ph->center = 1000.0f;
    ph->sweep = 2000.0f;
    ma_phaser_setup_coef(ph, ph->center);
}

void ma_phaser_process(ma_phaser * ph, float * buf, int frame_count) {
    if (ph->rate <= 0.0f || ph->depth <= 0.0f) return;
    float phase_inc = ph->rate / ph->sample_rate;
    float a = ph->a;
    int counter = ph->update_counter;
    const int UPDATE_EVERY = 16;
    // triangle LFO modulates all-pass cutoff in cents around center.
    const float CENTS_INV = 1.0f / 1200.0f;
    float depth = ph->depth; if (depth > 1.0f) depth = 1.0f; if (depth < 0.0f) depth = 0.0f;
    // Classic phaser: notch depth comes from cancellation at frequencies where the
    // all-pass chain phase-inverts the signal. With feedback, the notches resonate.
    // depth modulates both wet/dry mix AND feedback for a bigger audible swing.
    float feedback = 0.9f * depth;   // up to 90% feedback → deep resonant notches
    float wet = depth;
    float norm = 1.0f / (1.0f + depth);
    float fb_l = ph->fb_l, fb_r = ph->fb_r;
    for (int f = 0; f < frame_count; f++) {
        if (counter <= 0) {
            float lfo = 4.0f * fabsf(ph->lfo_phase - 0.5f) - 1.0f;
            float freq = ph->center * powf(2.0f, lfo * ph->sweep * CENTS_INV);
            ma_phaser_setup_coef(ph, freq);
            a = ph->a;
            counter = UPDATE_EVERY;
        }
        counter--;
        ph->lfo_phase += phase_inc;
        if (ph->lfo_phase >= 1.0f) ph->lfo_phase -= 1.0f;

        float xl = buf[f * 2];
        float xr = buf[f * 2 + 1];
        // Mix input with last-stage output fed back (creates resonance at notches)
        float in_l = xl + fb_l * feedback;
        float in_r = xr + fb_r * feedback;
        // Cascade N all-pass stages
        for (int s = 0; s < MA_PHASER_STAGES; s++) {
            float yl = -a * in_l + ph->x_l[s] + a * ph->y_l[s];
            ph->x_l[s] = in_l;
            ph->y_l[s] = yl;
            in_l = yl;
            float yr = -a * in_r + ph->x_r[s] + a * ph->y_r[s];
            ph->x_r[s] = in_r;
            ph->y_r[s] = yr;
            in_r = yr;
        }
        fb_l = in_l;
        fb_r = in_r;
        // Soft-clip feedback to prevent runaway at very high depth
        if (fb_l > 2.0f) fb_l = 2.0f; if (fb_l < -2.0f) fb_l = -2.0f;
        if (fb_r > 2.0f) fb_r = 2.0f; if (fb_r < -2.0f) fb_r = -2.0f;
        buf[f * 2]     = (xl + in_l * wet) * norm;
        buf[f * 2 + 1] = (xr + in_r * wet) * norm;
    }
    ph->update_counter = counter;
    ph->fb_l = fb_l;
    ph->fb_r = fb_r;
}

#endif // MA_PHASER_IMPLEMENTATION
