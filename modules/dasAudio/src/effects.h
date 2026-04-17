#pragma once

// Bitcrusher, waveshaper, DJ filter, and bandpass effects.
// Header-only; define MA_EFFECTS_IMPLEMENTATION in exactly one .cpp before including.

#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct ma_bitcrush {
    float bits;           // target bit depth (4-16, 0=off)
    int coarse;           // sample rate reduction factor (0=off, 2=half, 4=quarter)
    float held_l, held_r; // sample-and-hold state for coarse
    int hold_counter;     // countdown for coarse
};

struct ma_waveshaper {
    float drive;          // drive amount (0=off, 1=mild, 5+=harsh)
};

struct ma_djfilter {
    float position;       // 0=full LPF, 0.5=bypass, 1=full HPF
    float z1_l, z1_r;    // one-pole filter state (left, right)
    float sample_rate;
};

struct ma_bandpass {
    float freq;           // center frequency Hz
    float q;              // quality factor
    float sample_rate;
    // biquad coefficients
    float a0, a1, a2, b1, b2;
    // state per channel
    float z1_l, z2_l, z1_r, z2_r;
};

void ma_bitcrush_init(ma_bitcrush * bc);
void ma_bitcrush_process(ma_bitcrush * bc, float * buf, int frame_count);

void ma_waveshaper_init(ma_waveshaper * ws);
void ma_waveshaper_process(ma_waveshaper * ws, float * buf, int frame_count);

void ma_djfilter_init(ma_djfilter * dj, float sample_rate);
void ma_djfilter_process(ma_djfilter * dj, float * buf, int frame_count);

void ma_bandpass_init(ma_bandpass * bp, float sample_rate);
void ma_bandpass_setup(ma_bandpass * bp, float freq, float q);
void ma_bandpass_process(ma_bandpass * bp, float * buf, int frame_count);

// ─── Implementation ───

#ifdef MA_EFFECTS_IMPLEMENTATION

void ma_bitcrush_init(ma_bitcrush * bc) {
    memset(bc, 0, sizeof(*bc));
}

void ma_bitcrush_process(ma_bitcrush * bc, float * buf, int frame_count) {
    for (int f = 0; f < frame_count; f++) {
        float l = buf[f * 2];
        float r = buf[f * 2 + 1];

        // bit depth reduction
        if (bc->bits > 0.0f) {
            float levels = powf(2.0f, bc->bits);
            l = floorf(l * levels + 0.5f) / levels;
            r = floorf(r * levels + 0.5f) / levels;
        }

        // sample rate reduction (sample-and-hold)
        if (bc->coarse > 0) {
            if (bc->hold_counter <= 0) {
                bc->held_l = l;
                bc->held_r = r;
                bc->hold_counter = bc->coarse;
            }
            bc->hold_counter--;
            l = bc->held_l;
            r = bc->held_r;
        }

        buf[f * 2]     = l;
        buf[f * 2 + 1] = r;
    }
}

void ma_waveshaper_init(ma_waveshaper * ws) {
    memset(ws, 0, sizeof(*ws));
}

void ma_waveshaper_process(ma_waveshaper * ws, float * buf, int frame_count) {
    if (ws->drive <= 0.0f) return;
    float gain = 1.0f + ws->drive * 10.0f;
    float norm = 1.0f / tanhf(gain);
    for (int f = 0; f < frame_count; f++) {
        buf[f * 2]     = tanhf(buf[f * 2]     * gain) * norm;
        buf[f * 2 + 1] = tanhf(buf[f * 2 + 1] * gain) * norm;
    }
}

void ma_djfilter_init(ma_djfilter * dj, float sample_rate) {
    memset(dj, 0, sizeof(*dj));
    dj->position = 0.5f;
    dj->sample_rate = sample_rate;
}

void ma_djfilter_process(ma_djfilter * dj, float * buf, int frame_count) {
    float pos = dj->position;
    if (pos < 0.0f) pos = 0.0f;
    if (pos > 1.0f) pos = 1.0f;

    // bypass
    if (pos > 0.499f && pos < 0.501f) return;

    float cutoff;
    int is_hpf;
    if (pos < 0.5f) {
        // LPF: cutoff sweeps from 200Hz (pos=0) to 20000Hz (pos=0.5)
        float t = pos / 0.5f;
        cutoff = 200.0f * powf(20000.0f / 200.0f, t);
        is_hpf = 0;
    } else {
        // HPF: cutoff sweeps from 20Hz (pos=0.5) to 4000Hz (pos=1)
        float t = (pos - 0.5f) / 0.5f;
        cutoff = 20.0f * powf(4000.0f / 20.0f, t);
        is_hpf = 1;
    }

    float alpha = 1.0f - expf(-2.0f * (float)M_PI * cutoff / dj->sample_rate);
    float z1_l = dj->z1_l;
    float z1_r = dj->z1_r;

    for (int f = 0; f < frame_count; f++) {
        float in_l = buf[f * 2];
        float in_r = buf[f * 2 + 1];

        // one-pole LPF
        z1_l += alpha * (in_l - z1_l);
        z1_r += alpha * (in_r - z1_r);

        if (is_hpf) {
            buf[f * 2]     = in_l - z1_l;
            buf[f * 2 + 1] = in_r - z1_r;
        } else {
            buf[f * 2]     = z1_l;
            buf[f * 2 + 1] = z1_r;
        }
    }

    dj->z1_l = z1_l;
    dj->z1_r = z1_r;
}

void ma_bandpass_init(ma_bandpass * bp, float sample_rate) {
    memset(bp, 0, sizeof(*bp));
    bp->sample_rate = sample_rate;
    bp->freq = 1000.0f;
    bp->q = 1.0f;
    ma_bandpass_setup(bp, bp->freq, bp->q);
}

void ma_bandpass_setup(ma_bandpass * bp, float freq, float q) {
    bp->freq = freq;
    bp->q = q;
    if (bp->q < 0.01f) bp->q = 0.01f;

    float w0 = 2.0f * (float)M_PI * freq / bp->sample_rate;
    float sin_w0 = sinf(w0);
    float cos_w0 = cosf(w0);
    float alpha = sin_w0 / (2.0f * bp->q);

    float norm = 1.0f / (1.0f + alpha);
    bp->a0 =  alpha * norm;
    bp->a1 =  0.0f;
    bp->a2 = -alpha * norm;
    bp->b1 = -2.0f * cos_w0 * norm;
    bp->b2 =  (1.0f - alpha) * norm;
}

void ma_bandpass_process(ma_bandpass * bp, float * buf, int frame_count) {
    float a0 = bp->a0, a1 = bp->a1, a2 = bp->a2;
    float b1 = bp->b1, b2 = bp->b2;
    float z1_l = bp->z1_l, z2_l = bp->z2_l;
    float z1_r = bp->z1_r, z2_r = bp->z2_r;

    for (int f = 0; f < frame_count; f++) {
        float x_l = buf[f * 2];
        float x_r = buf[f * 2 + 1];

        // transposed direct form II
        float y_l = a0 * x_l + z1_l;
        z1_l = a1 * x_l - b1 * y_l + z2_l;
        z2_l = a2 * x_l - b2 * y_l;

        float y_r = a0 * x_r + z1_r;
        z1_r = a1 * x_r - b1 * y_r + z2_r;
        z2_r = a2 * x_r - b2 * y_r;

        buf[f * 2]     = y_l;
        buf[f * 2 + 1] = y_r;
    }

    bp->z1_l = z1_l; bp->z2_l = z2_l;
    bp->z1_r = z1_r; bp->z2_r = z2_r;
}

#endif // MA_EFFECTS_IMPLEMENTATION
