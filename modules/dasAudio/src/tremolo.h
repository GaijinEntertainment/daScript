#pragma once

// Per-voice tremolo — sine LFO modulating amplitude.
// Output = input * (1 - depth * 0.5 * (1 - sin(phase))), so gain sweeps
// between (1 - depth) and 1.0. At depth=0 no effect; at depth=1 full AM.
// Header-only; define MA_TREMOLO_IMPLEMENTATION in exactly one .cpp.

#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct ma_tremolo {
    float rate;           // LFO rate Hz (0 = off)
    float depth;          // modulation depth 0..1
    float sample_rate;
    float lfo_phase;      // 0..1
};

void ma_tremolo_init(ma_tremolo * tr, float sample_rate);
void ma_tremolo_process(ma_tremolo * tr, float * buf, int frame_count);

#ifdef MA_TREMOLO_IMPLEMENTATION

void ma_tremolo_init(ma_tremolo * tr, float sample_rate) {
    memset(tr, 0, sizeof(*tr));
    tr->sample_rate = sample_rate;
    tr->rate = 0.0f;
    tr->depth = 1.0f;
    tr->lfo_phase = 0.0f;
}

void ma_tremolo_process(ma_tremolo * tr, float * buf, int frame_count) {
    if (tr->rate <= 0.0f || tr->depth <= 0.0f) return;
    float d = tr->depth; if (d > 1.0f) d = 1.0f;
    float half_d = d * 0.5f;
    float min_gain = 1.0f - d;
    if (min_gain < 0.0f) min_gain = 0.0f;
    float phase_inc = tr->rate / tr->sample_rate;
    float phase = tr->lfo_phase;
    for (int f = 0; f < frame_count; f++) {
        // gain sweeps min_gain (sin=-1) .. 1.0 (sin=1)
        float lfo = sinf(phase * 2.0f * (float)M_PI);
        float gain = min_gain + (1.0f - min_gain) * (0.5f + 0.5f * lfo);
        buf[f * 2]     *= gain;
        buf[f * 2 + 1] *= gain;
        phase += phase_inc;
        if (phase >= 1.0f) phase -= 1.0f;
    }
    tr->lfo_phase = phase;
    (void)half_d;
}

#endif // MA_TREMOLO_IMPLEMENTATION
