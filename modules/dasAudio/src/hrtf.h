#ifndef hrtf_h
#define hrtf_h

#include <cipic_hrtf_lib.h>
#include "minfft.h"

#define HRTF_CROSSFADE 256

struct ma_hrtf {
    uint32_t taps;
    int32_t  azimuth;
    int32_t  elevation;
    uint32_t sampleRate;
    short *  leftfip;
    short *  rightfip;
    float *  monoBuffer;
    uint32_t monoBufferSize;
    minfft_aux * fft_aux;
    void *       arena;                   // single allocation for all buffers below
    float *      filter_left_freq;
    float *      filter_right_freq;
    float *      filter_left_freq_prev;
    float *      filter_right_freq_prev;
    float *      crossfade_buf;
    float *      fft_input_buf;
    float *      fft_input_buf2;
    float *      fft_freq_scratch;
    float *      fft_time_scratch;
    float *      left_input_hist;
    float *      right_input_hist;
    uint32_t     fft_size;
    int          needs_crossfade;
};

void ma_hrtf_init(ma_hrtf* hrtf, ma_uint32 sampleRate);
void ma_hrtf_process_frames(ma_hrtf * hrtf, float * pOut, const float * pIn, ma_uint32 nChannels, ma_uint32 frameCount);
void ma_hrtf_set_direction ( ma_hrtf* hrtf, int32_t azimuth, int32_t elevation );
void ma_hrtf_uninit(ma_hrtf* hrtf);

#ifdef MINIAUDIO_IMPLEMENTATION

#define HRTF_DIFFUSED 0

static uint32_t ma_hrtf_next_pow2(uint32_t v) {
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}

static void ma_hrtf_compute_filter_freq(ma_hrtf* hrtf, short* pFilter, float* filter_freq) {
    uint32_t N = hrtf->fft_size;
    memset(hrtf->fft_input_buf, 0, N * sizeof(float));
    for (uint32_t i = 0; i < hrtf->taps; i++) {
        hrtf->fft_input_buf[i] = pFilter[i] / 32768.0f;
    }
    minfft_realdft((minfft_real*)hrtf->fft_input_buf, (minfft_cmpl*)filter_freq, hrtf->fft_aux);
}

void ma_hrtf_set_direction ( ma_hrtf* hrtf, int32_t azimuth, int32_t elevation ) {
    hrtf->azimuth = azimuth;
    hrtf->elevation = elevation;
    int localAzimuth = hrtf->azimuth;
    int localElevation = hrtf->elevation;
    // Save previous filter before computing new one
    short oldLeft[512], oldRight[512];
    memcpy(oldLeft, hrtf->leftfip, hrtf->taps * sizeof(short));
    memcpy(oldRight, hrtf->rightfip, hrtf->taps * sizeof(short));
    mit_hrtf_get(&localAzimuth, &localElevation, hrtf->sampleRate, HRTF_DIFFUSED, hrtf->leftfip, hrtf->rightfip);
    if (hrtf->fft_aux) {
        // Check if filter actually changed (MIT lib snaps to grid)
        int changed = 0;
        for (uint32_t i = 0; i < hrtf->taps; i++) {
            if (oldLeft[i] != hrtf->leftfip[i] || oldRight[i] != hrtf->rightfip[i]) {
                changed = 1;
                break;
            }
        }
        if (changed) {
            // Save old filter in freq domain for crossfade
            ma_hrtf_compute_filter_freq(hrtf, oldLeft, hrtf->filter_left_freq_prev);
            ma_hrtf_compute_filter_freq(hrtf, oldRight, hrtf->filter_right_freq_prev);
            hrtf->needs_crossfade = 1;
        }
        ma_hrtf_compute_filter_freq(hrtf, hrtf->leftfip, hrtf->filter_left_freq);
        ma_hrtf_compute_filter_freq(hrtf, hrtf->rightfip, hrtf->filter_right_freq);
    }
}

static void ma_hrtf_complex_multiply(float* out, const float* a, const float* b, uint32_t count) {
    for (uint32_t i = 0; i < count; i++) {
        float ar = a[i*2];
        float ai = a[i*2+1];
        float br = b[i*2];
        float bi = b[i*2+1];
        out[i*2]   = ar * br - ai * bi;
        out[i*2+1] = ar * bi + ai * br;
    }
}

// Core overlap-save convolution for one block. Writes to pOut at stride 2 (interleaved stereo).
static void ma_hrtf_convolve_block(ma_hrtf* hrtf, float* pOut, const float* input_buf,
                                    float* filter_freq, uint32_t block, uint32_t out_offset) {
    uint32_t N = hrtf->fft_size;
    uint32_t overlap_len = hrtf->taps - 1;
    float inv_n = 1.0f / (float)N;

    minfft_realdft((minfft_real*)input_buf, (minfft_cmpl*)hrtf->fft_freq_scratch, hrtf->fft_aux);
    ma_hrtf_complex_multiply(hrtf->fft_freq_scratch, hrtf->fft_freq_scratch, filter_freq, N / 2 + 1);
    minfft_invrealdft((minfft_cmpl*)hrtf->fft_freq_scratch, hrtf->fft_time_scratch, hrtf->fft_aux);

    float * valid_output = hrtf->fft_time_scratch + overlap_len;
    for (uint32_t i = 0; i < block; i++) {
        pOut[(out_offset + i) * 2] = valid_output[i] * inv_n;
    }
}

void ma_hrtf_process_channel ( ma_hrtf* hrtf, float* pOut, const float* pInBuffer,
                                float * input_hist, float * filter_freq, float * filter_freq_prev,
                                int do_crossfade, ma_uint32 frameCount ) {
    uint32_t N = hrtf->fft_size;
    uint32_t M = hrtf->taps;
    uint32_t overlap_len = M - 1;
    float * input_buf = hrtf->fft_input_buf;

    uint32_t L = N - overlap_len;
    uint32_t processed = 0;

    while (processed < frameCount) {
        uint32_t block = frameCount - processed;
        if (block > L) block = L;

        // Build overlap-save input buffer
        memcpy(input_buf, input_hist, overlap_len * sizeof(float));
        memcpy(input_buf + overlap_len, pInBuffer + processed, block * sizeof(float));
        if (overlap_len + block < N) {
            memset(input_buf + overlap_len + block, 0, (N - overlap_len - block) * sizeof(float));
        }

        if (do_crossfade && processed == 0) {
            // minfft_realdft destroys its input, so copy before first convolution
            memcpy(hrtf->fft_input_buf2, input_buf, N * sizeof(float));
            // Process with OLD filter into crossfade_buf
            ma_hrtf_convolve_block(hrtf, hrtf->crossfade_buf, input_buf, filter_freq_prev, block, 0);
            // Process with NEW filter into pOut (using the saved copy)
            ma_hrtf_convolve_block(hrtf, pOut, hrtf->fft_input_buf2, filter_freq, block, processed);
            // Crossfade over HRTF_CROSSFADE samples
            uint32_t fade_len = HRTF_CROSSFADE;
            if (fade_len > block) fade_len = block;
            for (uint32_t i = 0; i < fade_len; i++) {
                float k = (float)(i + 1) / (float)(fade_len + 1);
                float old_val = hrtf->crossfade_buf[i * 2];
                float new_val = pOut[(processed + i) * 2];
                pOut[(processed + i) * 2] = old_val * (1.0f - k) + new_val * k;
            }
        } else {
            // Normal processing with current filter
            ma_hrtf_convolve_block(hrtf, pOut, input_buf, filter_freq, block, processed);
        }

        // Update input history
        if (block >= overlap_len) {
            memcpy(input_hist, pInBuffer + processed + block - overlap_len, overlap_len * sizeof(float));
        } else {
            uint32_t keep = overlap_len - block;
            memmove(input_hist, input_hist + block, keep * sizeof(float));
            memcpy(input_hist + keep, pInBuffer + processed, block * sizeof(float));
        }

        processed += block;
    }
}

void ma_hrtf_process_pcm_frames_mono(ma_hrtf* hrtf, float* pOut, const float* pIn, ma_uint32 frameCount) {
    int xfade = hrtf->needs_crossfade;
    hrtf->needs_crossfade = 0;
    ma_hrtf_process_channel(hrtf, pOut, pIn, hrtf->left_input_hist,
        hrtf->filter_left_freq, hrtf->filter_left_freq_prev, xfade, frameCount);
    ma_hrtf_process_channel(hrtf, pOut+1, pIn, hrtf->right_input_hist,
        hrtf->filter_right_freq, hrtf->filter_right_freq_prev, xfade, frameCount);
}

void ma_hrtf_process_frames(ma_hrtf * hrtf, float * pOut, const float * pIn, ma_uint32 nChannels, ma_uint32 frameCount) {
    if ( hrtf->taps==0 ) return;
    if ( nChannels == 1 ) {
        ma_hrtf_process_pcm_frames_mono(hrtf, pOut, pIn, frameCount);
    } else {
        if ( frameCount > hrtf->monoBufferSize ) {
            if ( hrtf->monoBuffer ) free(hrtf->monoBuffer);
            hrtf->monoBuffer = (float *) malloc(frameCount * sizeof(float));
            hrtf->monoBufferSize = frameCount;
        }
        float rChannels = 1.0f / nChannels;
        for ( ma_uint32 i = 0; i != frameCount; i++ ) {
            float m = 0.0f;
            for ( ma_uint32 j = 0; j != nChannels; j++ ) {
                m += pIn[i*nChannels+j];
            }
            hrtf->monoBuffer[i] = m * rChannels;
        }
        ma_hrtf_process_pcm_frames_mono(hrtf, pOut, hrtf->monoBuffer, frameCount);
    }
}

void ma_hrtf_init(ma_hrtf* hrtf, ma_uint32 sampleRate) {
    memset(hrtf, 0, sizeof(ma_hrtf));
    hrtf->taps = mit_hrtf_availability(0, 0, sampleRate, HRTF_DIFFUSED);
    hrtf->sampleRate = sampleRate;
    if ( hrtf->taps ) {
        uint32_t M = hrtf->taps;
        uint32_t N = ma_hrtf_next_pow2(M * 2);
        if (N < 256) N = 256;
        hrtf->fft_size = N;
        hrtf->fft_aux = minfft_mkaux_realdft_1d(N);
        uint32_t freq_size = N + 2;
        uint32_t overlap_len = M - 1;
        uint32_t L = N - overlap_len;
        // Single allocation for all buffers
        // Shorts: leftfip(M) + rightfip(M)
        // Floats: filter_left_freq(freq) + filter_right_freq(freq) +
        //         filter_left_freq_prev(freq) + filter_right_freq_prev(freq) +
        //         fft_input_buf(N) + fft_input_buf2(N) + fft_freq_scratch(freq) +
        //         fft_time_scratch(N) + crossfade_buf(L*2) +
        //         left_input_hist(overlap) + right_input_hist(overlap)
        size_t short_bytes = (size_t)(M * 2) * sizeof(short);
        // Round up to float alignment
        size_t short_bytes_aligned = (short_bytes + sizeof(float) - 1) & ~(sizeof(float) - 1);
        size_t float_count = (size_t)(freq_size * 4) + N * 3 + freq_size + L * 2 + overlap_len * 2;
        size_t total = short_bytes_aligned + float_count * sizeof(float);
        hrtf->arena = calloc(1, total);
        char * ptr = (char *)hrtf->arena;
        hrtf->leftfip  = (short *)ptr; ptr += M * sizeof(short);
        hrtf->rightfip = (short *)ptr; ptr += M * sizeof(short);
        // Align to float boundary
        ptr = (char *)hrtf->arena + short_bytes_aligned;
        float * fptr = (float *)ptr;
        hrtf->filter_left_freq       = fptr; fptr += freq_size;
        hrtf->filter_right_freq      = fptr; fptr += freq_size;
        hrtf->filter_left_freq_prev  = fptr; fptr += freq_size;
        hrtf->filter_right_freq_prev = fptr; fptr += freq_size;
        hrtf->fft_input_buf          = fptr; fptr += N;
        hrtf->fft_input_buf2         = fptr; fptr += N;
        hrtf->fft_freq_scratch       = fptr; fptr += freq_size;
        hrtf->fft_time_scratch       = fptr; fptr += N;
        hrtf->crossfade_buf          = fptr; fptr += L * 2;
        hrtf->left_input_hist        = fptr; fptr += overlap_len;
        hrtf->right_input_hist       = fptr; fptr += overlap_len;
    }
}

void ma_hrtf_uninit(ma_hrtf* hrtf) {
    if ( hrtf->monoBuffer ) { free(hrtf->monoBuffer); hrtf->monoBuffer = nullptr; }
    if ( hrtf->fft_aux ) { minfft_free_aux(hrtf->fft_aux); hrtf->fft_aux = nullptr; }
    if ( hrtf->arena ) { free(hrtf->arena); hrtf->arena = nullptr; }
}

#endif

#endif
