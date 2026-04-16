#ifndef convolution_reverb_h
#define convolution_reverb_h

// Partitioned convolution reverb using synthetic impulse response.
// Based on the strudel-cc approach: random noise with exponential decay and lowpass sweep.
// Uses uniform partitioned overlap-save with minfft for FFT.
//
// The IR is split into fixed-size blocks (CONV_BLOCK_SIZE). Each block is pre-transformed
// to the frequency domain. A frequency-domain delay line (FDDL) stores recent input spectra.
// Per audio chunk: 1 FFT + P complex multiply-accumulates + 1 IFFT, where P = num partitions.
// This is O(P * B * log(B)) vs O(N * log(N)) for monolithic convolution — ~5x faster for 2s IRs.

#define CONV_BLOCK_SIZE 1024   // partition size in samples (~21ms at 48kHz)
#define CONV_DEFAULT_MAX_IR 8.0f  // default max IR length in seconds

struct ConvolutionReverb {
    // Impulse response parameters (exposed to daScript)
    float    decayTime;        // decay time in seconds (time to -60dB)
    float    lpFreqStart;      // lowpass start frequency (Hz)
    float    lpFreqEnd;        // lowpass end frequency at -60dB (Hz)
    float    fadeIn;           // fade-in time in seconds
    uint32_t sampleRate;
    // Partitioned convolution state
    void *   fft_aux;          // minfft_aux* (opaque for AOT)
    uint32_t fft_size;         // N = 2 * CONV_BLOCK_SIZE
    uint32_t ir_length;        // original IR length in samples
    uint32_t num_partitions;   // P = ceil(ir_length / CONV_BLOCK_SIZE)
    // Pre-FFT'd IR partitions: [P][freq_size] per channel
    float *  ir_parts_left;
    float *  ir_parts_right;
    // Frequency-domain delay line: [P][freq_size] (circular buffer of input spectra)
    float *  fddl;
    uint32_t fddl_pos;         // current write position in circular FDDL
    // Working buffers
    float *  fft_input_buf;    // N floats — time-domain input for FFT
    float *  fft_freq_scratch; // freq_size floats — scratch for FFT output
    float *  fft_accum;        // freq_size floats — accumulation buffer
    float *  fft_time_scratch; // N floats — IFFT output
    // Input history (previous CONV_BLOCK_SIZE samples for overlap-save)
    float *  input_hist_left;
    float *  input_hist_right;
    // Input accumulation buffer (collects samples until a full block is ready)
    float *  input_accum;
    uint32_t input_accum_pos;  // how many samples accumulated so far
    // Pending output from blocks that completed with carried-over samples
    float *  pending_left;
    float *  pending_right;
    uint32_t pending_count;    // how many pending output samples
    uint32_t pending_offset;   // read position in pending buffer
    void *   arena;            // single allocation for all buffers
};

void conv_reverb_init(ConvolutionReverb * rev, uint32_t sampleRate, float decayTime,
                      float lpFreqStart, float lpFreqEnd, float fadeIn);
void conv_reverb_process(ConvolutionReverb * rev, const float * input, float * output, uint32_t nFrames);
void conv_reverb_process_mono(ConvolutionReverb * rev, const float * input, float * output, uint32_t nSamples);
void conv_reverb_uninit(ConvolutionReverb * rev);
void conv_reverb_set_max_ir(float seconds);
float conv_reverb_get_max_ir();

#ifdef CONVOLUTION_REVERB_IMPLEMENTATION

#include "minfft.h"
#include <math.h>
#include <string.h>

static float g_conv_reverb_max_ir = CONV_DEFAULT_MAX_IR;

void conv_reverb_set_max_ir(float seconds) {
    if (seconds > 0.0f) g_conv_reverb_max_ir = seconds;
}

float conv_reverb_get_max_ir() {
    return g_conv_reverb_max_ir;
}
#include <stdlib.h>

#define CONV_FFT_AUX(rev) ((minfft_aux*)(rev)->fft_aux)

// Simple PRNG (xorshift32)
static uint32_t conv_reverb_xorshift(uint32_t * state) {
    uint32_t x = *state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    *state = x;
    return x;
}

static float conv_reverb_rand(uint32_t * state) {
    return (float)(conv_reverb_xorshift(state)) / (float)0xFFFFFFFFu * 2.0f - 1.0f;
}

// Generate synthetic impulse response: random noise with exponential decay.
// Creates stereo decorrelation by using different random seeds for L/R.
static void conv_reverb_generate_ir(ConvolutionReverb * rev, float * ir_left, float * ir_right) {
    uint32_t N = rev->ir_length;
    float sr = (float)rev->sampleRate;
    float iSr = 1.0f / sr;

    float decayRate = logf(1000.0f) / rev->decayTime;

    uint32_t fadeInSamples = (uint32_t)(rev->fadeIn * sr);
    if (fadeInSamples > N) fadeInSamples = N;

    uint32_t seedL = 0x12345678u;
    uint32_t seedR = 0x87654321u;
    for (uint32_t i = 0; i < N; i++) {
        float t = (float)i * iSr;
        float envelope = expf(-t * decayRate);
        if (i < fadeInSamples) {
            envelope *= (float)(i + 1) / (float)(fadeInSamples + 1);
        }
        ir_left[i]  = conv_reverb_rand(&seedL) * envelope;
        ir_right[i] = conv_reverb_rand(&seedR) * envelope;
    }

    // Fade-out window over last 10% to avoid hard cutoff when IR is capped
    uint32_t fadeOutLen = N / 10;
    if (fadeOutLen < 64) fadeOutLen = 64;
    if (fadeOutLen > N) fadeOutLen = N;
    uint32_t fadeOutStart = N - fadeOutLen;
    for (uint32_t i = fadeOutStart; i < N; i++) {
        float fade = (float)(N - 1 - i) / (float)fadeOutLen;
        ir_left[i]  *= fade;
        ir_right[i] *= fade;
    }

    // Gradient lowpass filter (frequency sweeps from lpFreqStart to lpFreqEnd)
    if (rev->lpFreqStart < sr * 0.499f || rev->lpFreqEnd < sr * 0.499f) {
        float prevL = 0.0f, prevR = 0.0f;
        for (uint32_t i = 0; i < N; i++) {
            float t = (float)i * iSr;
            float progress = t / rev->decayTime;
            if (progress > 1.0f) progress = 1.0f;
            float logStart = logf(rev->lpFreqStart);
            float logEnd   = logf(rev->lpFreqEnd);
            float fc = expf(logStart + (logEnd - logStart) * progress);
            float alpha = 1.0f - expf(-6.2831853f * fc / sr);
            if (alpha > 1.0f) alpha = 1.0f;
            prevL = alpha * ir_left[i]  + (1.0f - alpha) * prevL;
            prevR = alpha * ir_right[i] + (1.0f - alpha) * prevR;
            ir_left[i]  = prevL;
            ir_right[i] = prevR;
        }
    }

    // RMS normalization — consistent output level across decay times
    float targetRMS = 0.005f;
    for (int ch = 0; ch < 2; ch++) {
        float * ir = (ch == 0) ? ir_left : ir_right;
        float energy = 0.0f;
        for (uint32_t i = 0; i < N; i++) {
            energy += ir[i] * ir[i];
        }
        float rms = sqrtf(energy / (float)N);
        if (rms > 0.0f) {
            float scale = targetRMS / rms;
            for (uint32_t i = 0; i < N; i++) {
                ir[i] *= scale;
            }
        }
    }
}

static void conv_reverb_complex_multiply_acc(float * acc, const float * a, const float * b, uint32_t count) {
    for (uint32_t i = 0; i < count; i++) {
        float ar = a[i*2],   ai = a[i*2+1];
        float br = b[i*2],   bi = b[i*2+1];
        acc[i*2]   += ar * br - ai * bi;
        acc[i*2+1] += ar * bi + ai * br;
    }
}

// FFT one block of input, store spectrum in the FDDL. Called once per block (shared across L/R).
static void conv_reverb_fft_input(ConvolutionReverb * rev, const float * input_block) {
    uint32_t B = CONV_BLOCK_SIZE;
    uint32_t N = rev->fft_size;
    uint32_t freq_size = N + 2;

    // Build overlap-save input: [previous B samples | current B samples]
    memcpy(rev->fft_input_buf, rev->input_hist_left, B * sizeof(float));
    memcpy(rev->fft_input_buf + B, input_block, B * sizeof(float));

    // Save current block as history
    memcpy(rev->input_hist_left, input_block, B * sizeof(float));

    // FFT
    minfft_realdft((minfft_real*)rev->fft_input_buf, (minfft_cmpl*)rev->fft_freq_scratch, CONV_FFT_AUX(rev));

    // Store in FDDL
    memcpy(rev->fddl + rev->fddl_pos * freq_size, rev->fft_freq_scratch, freq_size * sizeof(float));
}

// Convolve the FDDL with one channel's IR partitions. Called once per channel.
static void conv_reverb_convolve_channel(ConvolutionReverb * rev, float * out_buf, float * ir_parts) {
    uint32_t B = CONV_BLOCK_SIZE;
    uint32_t N = rev->fft_size;
    uint32_t P = rev->num_partitions;
    uint32_t freq_size = N + 2;
    uint32_t freq_count = N / 2 + 1;
    float inv_n = 1.0f / (float)N;

    // Accumulate: Y[k] = sum over p of FDDL[(pos - p) mod P] * H[p][k]
    memset(rev->fft_accum, 0, freq_size * sizeof(float));
    for (uint32_t p = 0; p < P; p++) {
        uint32_t fddl_idx = (rev->fddl_pos + P - p) % P;
        float * input_spec = rev->fddl + fddl_idx * freq_size;
        float * ir_spec = ir_parts + p * freq_size;
        conv_reverb_complex_multiply_acc(rev->fft_accum, input_spec, ir_spec, freq_count);
    }
    // IFFT
    minfft_invrealdft((minfft_cmpl*)rev->fft_accum, rev->fft_time_scratch, CONV_FFT_AUX(rev));

    // Overlap-save: output is the last B samples (discard first B)
    float * valid = rev->fft_time_scratch + B;
    for (uint32_t i = 0; i < B; i++) {
        out_buf[i] = valid[i] * inv_n;
    }
}

void conv_reverb_init(ConvolutionReverb * rev, uint32_t sampleRate, float decayTime,
                      float lpFreqStart, float lpFreqEnd, float fadeIn) {
    memset(rev, 0, sizeof(ConvolutionReverb));
    rev->sampleRate = sampleRate;
    rev->decayTime = decayTime;
    rev->lpFreqStart = lpFreqStart;
    rev->lpFreqEnd = lpFreqEnd;
    rev->fadeIn = fadeIn;

    // IR extends to 1.5x decay time (reaches ~-90dB), capped at configurable max
    float irTime = decayTime * 1.5f;
    if (irTime > g_conv_reverb_max_ir) irTime = g_conv_reverb_max_ir;
    rev->ir_length = (uint32_t)(irTime * (float)sampleRate);
    if (rev->ir_length < 64) rev->ir_length = 64;

    uint32_t B = CONV_BLOCK_SIZE;
    uint32_t N = B * 2;
    rev->fft_size = N;
    rev->num_partitions = (rev->ir_length + B - 1) / B;
    uint32_t P = rev->num_partitions;
    uint32_t freq_size = N + 2;

    rev->fft_aux = (void*)minfft_mkaux_realdft_1d(N);

    // Single arena allocation:
    // IR generation (temporary): ir_left(ir_length) + ir_right(ir_length)
    // IR partitions: 2 channels * P * freq_size
    // FDDL: P * freq_size
    // Working: fft_input_buf(N) + fft_freq_scratch(freq) + fft_accum(freq) + fft_time_scratch(N)
    // History: input_hist_left(B) + input_hist_right(B)
    // Input accum: input_accum(B)
    // Output overlap: output_overlap_left(B) + output_overlap_right(B)
    uint32_t ir_len = rev->ir_length;
    size_t ir_temp = (size_t)ir_len * 2;
    size_t parts = (size_t)P * freq_size * 2;
    size_t fddl = (size_t)P * freq_size;
    size_t work = (size_t)N * 2 + (size_t)freq_size * 2;
    size_t hist = (size_t)B * 2;
    size_t accum_buf = (size_t)B;
    size_t overlap = (size_t)B * 2;  // pending_left + pending_right
    // ir_temp is reused after init, but we need it during init — allocate max
    size_t total_floats = ir_temp + parts + fddl + work + hist + accum_buf + overlap;
    rev->arena = calloc(total_floats, sizeof(float));

    float * fptr = (float *)rev->arena;
    float * ir_left  = fptr; fptr += ir_len;
    float * ir_right = fptr; fptr += ir_len;
    rev->ir_parts_left     = fptr; fptr += P * freq_size;
    rev->ir_parts_right    = fptr; fptr += P * freq_size;
    rev->fddl              = fptr; fptr += P * freq_size;
    rev->fft_input_buf     = fptr; fptr += N;
    rev->fft_freq_scratch  = fptr; fptr += freq_size;
    rev->fft_accum         = fptr; fptr += freq_size;
    rev->fft_time_scratch  = fptr; fptr += N;
    rev->input_hist_left   = fptr; fptr += B;
    rev->input_hist_right  = fptr; fptr += B;
    rev->input_accum       = fptr; fptr += B;
    rev->pending_left  = fptr; fptr += B;
    rev->pending_right = fptr; fptr += B;

    rev->fddl_pos = 0;
    rev->input_accum_pos = 0;
    // Pre-fill pending with silence — introduces one block of latency (~21ms)
    // but ensures continuous output regardless of chunk size alignment.
    memset(rev->pending_left, 0, B * sizeof(float));
    memset(rev->pending_right, 0, B * sizeof(float));
    rev->pending_count = B;
    rev->pending_offset = 0;

    // Generate IR
    conv_reverb_generate_ir(rev, ir_left, ir_right);

    // Partition and FFT each block of the IR
    for (uint32_t p = 0; p < P; p++) {
        uint32_t offset = p * B;
        uint32_t len = ir_len - offset;
        if (len > B) len = B;

        // Left channel partition
        memset(rev->fft_input_buf, 0, N * sizeof(float));
        memcpy(rev->fft_input_buf, ir_left + offset, len * sizeof(float));
        minfft_realdft((minfft_real*)rev->fft_input_buf,
                       (minfft_cmpl*)(rev->ir_parts_left + p * freq_size),
                       CONV_FFT_AUX(rev));

        // Right channel partition
        memset(rev->fft_input_buf, 0, N * sizeof(float));
        memcpy(rev->fft_input_buf, ir_right + offset, len * sizeof(float));
        minfft_realdft((minfft_real*)rev->fft_input_buf,
                       (minfft_cmpl*)(rev->ir_parts_right + p * freq_size),
                       CONV_FFT_AUX(rev));
    }
    // ir_left/ir_right memory is part of arena but no longer needed
}

// Process interleaved stereo input to interleaved stereo output.
void conv_reverb_process(ConvolutionReverb * rev, const float * input, float * output, uint32_t nFrames) {
    if (!rev->fft_aux || nFrames == 0) return;

    uint32_t B = CONV_BLOCK_SIZE;
    uint32_t written = 0;

    // 1. Flush pending output from previous call
    while (rev->pending_count > 0 && written < nFrames) {
        uint32_t idx = rev->pending_offset;
        output[written * 2]     = rev->pending_left[idx];
        output[written * 2 + 1] = rev->pending_right[idx];
        rev->pending_offset++;
        rev->pending_count--;
        written++;
    }

    // 2. Process input, producing output blocks
    uint32_t read_pos = 0;
    while (read_pos < nFrames) {
        // Accumulate mono-downmixed input
        uint32_t space = B - rev->input_accum_pos;
        uint32_t avail = nFrames - read_pos;
        uint32_t to_copy = (avail < space) ? avail : space;

        for (uint32_t i = 0; i < to_copy; i++) {
            rev->input_accum[rev->input_accum_pos + i] =
                (input[(read_pos + i) * 2] + input[(read_pos + i) * 2 + 1]) * 0.5f;
        }
        rev->input_accum_pos += to_copy;
        read_pos += to_copy;

        if (rev->input_accum_pos >= B) {
            // Process full block
            conv_reverb_fft_input(rev, rev->input_accum);
            conv_reverb_convolve_channel(rev, rev->pending_left, rev->ir_parts_left);
            conv_reverb_convolve_channel(rev, rev->pending_right, rev->ir_parts_right);
            rev->fddl_pos = (rev->fddl_pos + 1) % rev->num_partitions;
            rev->input_accum_pos = 0;

            // Write as much as fits into output
            rev->pending_offset = 0;
            rev->pending_count = B;
            while (rev->pending_count > 0 && written < nFrames) {
                uint32_t idx = rev->pending_offset;
                output[written * 2]     = rev->pending_left[idx];
                output[written * 2 + 1] = rev->pending_right[idx];
                rev->pending_offset++;
                rev->pending_count--;
                written++;
            }
            // Any remaining pending_count will be flushed on the next call
        }
    }
}

// Process mono input to mono output (convolves with left IR only).
void conv_reverb_process_mono(ConvolutionReverb * rev, const float * input, float * output, uint32_t nSamples) {
    if (!rev->fft_aux || nSamples == 0) return;

    uint32_t B = CONV_BLOCK_SIZE;
    uint32_t written = 0;

    // Flush pending
    while (rev->pending_count > 0 && written < nSamples) {
        output[written] = rev->pending_left[rev->pending_offset];
        rev->pending_offset++;
        rev->pending_count--;
        written++;
    }

    uint32_t read_pos = 0;
    while (read_pos < nSamples) {
        uint32_t space = B - rev->input_accum_pos;
        uint32_t avail = nSamples - read_pos;
        uint32_t to_copy = (avail < space) ? avail : space;

        memcpy(rev->input_accum + rev->input_accum_pos, input + read_pos, to_copy * sizeof(float));
        rev->input_accum_pos += to_copy;
        read_pos += to_copy;

        if (rev->input_accum_pos >= B) {
            conv_reverb_fft_input(rev, rev->input_accum);
            conv_reverb_convolve_channel(rev, rev->pending_left, rev->ir_parts_left);
            rev->fddl_pos = (rev->fddl_pos + 1) % rev->num_partitions;
            rev->input_accum_pos = 0;

            rev->pending_offset = 0;
            rev->pending_count = B;
            while (rev->pending_count > 0 && written < nSamples) {
                output[written] = rev->pending_left[rev->pending_offset];
                rev->pending_offset++;
                rev->pending_count--;
                written++;
            }
        }
    }
}

void conv_reverb_uninit(ConvolutionReverb * rev) {
    if (rev->fft_aux) { minfft_free_aux(CONV_FFT_AUX(rev)); rev->fft_aux = nullptr; }
    if (rev->arena) { free(rev->arena); rev->arena = nullptr; }
}

#endif // CONVOLUTION_REVERB_IMPLEMENTATION

#endif // convolution_reverb_h
