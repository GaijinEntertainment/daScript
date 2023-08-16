#ifndef hrtf_h
#define hrtf_h

#include <mit_hrtf_lib.h>

struct ma_hrtf {
    uint32_t taps;
    float *  left;
    float *  right;
    int32_t  azimuth;
    int32_t  elevation;
    uint32_t sampleRate;
};

void ma_hrtf_init(ma_hrtf* hrtf, ma_uint32 sampleRate);
void ma_hrtf_process_frames(ma_hrtf * hrtf, float * pOut, const float * pIn, ma_uint32 nChannels, ma_uint32 frameCount);
void ma_hrtf_set_direction ( ma_hrtf* hrtf, int32_t azimuth, int32_t elevation );
void ma_hrtf_uninit(ma_hrtf* hrtf);

#ifdef MINIAUDIO_IMPLEMENTATION

#define HRTF_DIFFUSED 0

void ma_hrtf_set_direction ( ma_hrtf* hrtf, int32_t azimuth, int32_t elevation ) {
    hrtf->azimuth = azimuth;
    hrtf->elevation = elevation;
}

void ma_hrtf_process_channel ( ma_hrtf* hrtf, float* pOut, const float* pIn, float * history, short * pFilter, ma_uint32 frameCount) {
    uint32_t taps = hrtf->taps;
    float * outBuf = (float *) malloc((taps + frameCount)*sizeof(float));
    for ( ma_uint32 i = 0; i != (taps + frameCount); i++ ) {
        outBuf[i] = 0.0f;
    }
    for ( ma_uint32 i = 0; i != taps; i++ ) {
        outBuf[i] = history[i];
    }
    // convolution. todo: replace via fft?
    for ( ma_uint32 m = 0; m != taps; ++m ) {
        float filter_m = pFilter[m] / 32768.0f;
        for ( ma_uint32 n = 0; n != frameCount; n++ ) {
            outBuf[n + m] += pIn[n] * filter_m;
        }
    }
    // copy history
    for ( ma_uint32 i = 0; i != taps; i++ ) {
        history[i] = outBuf[frameCount + i];
    }
    // copy result
    for ( ma_uint32 i = 0; i != frameCount; i++ ) {
        pOut[i*2] = outBuf[i];
    }
    free(outBuf);
}

bool ma_hrtf_process_pcm_frames_mono(ma_hrtf* hrtf, float* pOut, const float* pIn, ma_uint32 frameCount) {
    if ( hrtf->taps == 0 ) {
        for ( ma_uint32 i = 0; i < frameCount; i++ ) {
            pOut[i*2+0] = pIn[i];
            pOut[i*2+1] = pIn[i];
        }
        return false;
    }
    // right, then left?
    int localAzimuth = hrtf->azimuth;
    int localElevation = hrtf->elevation;
    short * pLeft = (short *) malloc(hrtf->taps * sizeof(short));
    short * pRight = (short *) malloc(hrtf->taps * sizeof(short));
    mit_hrtf_get(&localAzimuth, &localElevation, hrtf->sampleRate, HRTF_DIFFUSED, pLeft, pRight);
    ma_hrtf_process_channel(hrtf, pOut, pIn, hrtf->left, pLeft, frameCount);
    ma_hrtf_process_channel(hrtf, pOut+1, pIn, hrtf->right, pRight, frameCount);
    free(pLeft);
    free(pRight);
    return true;
}

bool ma_hrtf_process_pcm_frames_stereo(ma_hrtf* hrtf, float* pOut, const float* pIn, ma_uint32 frameCount) {
    if ( hrtf->taps == 0 ) {
        for ( ma_uint32 i = 0; i < frameCount; i++ ) {
            pOut[i*2+0] = pIn[i];
            pOut[i*2+1] = pIn[i];
        }
        return false;
    }
    // right, then left?
    int localAzimuth = hrtf->azimuth;
    int localElevation = hrtf->elevation;
    short * pLeft = (short *) malloc(hrtf->taps * sizeof(short));
    short * pRight = (short *) malloc(hrtf->taps * sizeof(short));
    mit_hrtf_get(&localAzimuth, &localElevation, hrtf->sampleRate, HRTF_DIFFUSED, pLeft, pRight);
    float * pInMono = (float *) malloc(frameCount * sizeof(float));
    for ( ma_uint32 i = 0; i != frameCount; i++ ) {
        pInMono[i] = (pIn[i*2+0] + pIn[i*2+1]) * 0.5f;
    }
    ma_hrtf_process_channel(hrtf, pOut, pInMono, hrtf->left, pLeft, frameCount);
    ma_hrtf_process_channel(hrtf, pOut+1, pInMono, hrtf->right, pRight, frameCount);
    free(pInMono);
    free(pLeft);
    free(pRight);
    return true;
}

void ma_hrtf_process_frames(ma_hrtf * hrtf, float * pOut, const float * pIn, ma_uint32 nChannels, ma_uint32 frameCount) {
    if ( nChannels == 1 ) {
        ma_hrtf_process_pcm_frames_mono(hrtf, pOut, pIn, frameCount);
    } else if ( nChannels == 2 ) {
        ma_hrtf_process_pcm_frames_stereo(hrtf, pOut, pIn, frameCount);
    } else {
        for ( ma_uint32 i = 0; i < frameCount; i++ ) {
            for ( ma_uint32 c = 0; c < nChannels; c++ ) {
                pOut[i*nChannels+c] = pIn[i*nChannels+c];
            }
        }
    }
}

void ma_hrtf_init(ma_hrtf* hrtf, ma_uint32 sampleRate) {
    hrtf->taps = mit_hrtf_availability(0, 0, sampleRate, HRTF_DIFFUSED); // diffused vs regular
    if ( hrtf->taps ) {
        hrtf->left = (float *) malloc(hrtf->taps * sizeof(float));
        hrtf->right = (float *) malloc(hrtf->taps * sizeof(float));
        for ( uint32_t i = 0; i != hrtf->taps; ++i ) {
            hrtf->left[i] = 0.0f;
            hrtf->right[i] = 0.0f;
        }
    } else {
        hrtf->left = nullptr;
        hrtf->right = nullptr;
    }
    hrtf->azimuth = 0;
    hrtf->elevation = 0;
    hrtf->sampleRate = sampleRate;
}

void ma_hrtf_uninit(ma_hrtf* hrtf) {
    if ( hrtf->left ) {
        free(hrtf->left);
        hrtf->left = nullptr;
    }
    if ( hrtf->right ) {
        free(hrtf->right);
        hrtf->right = nullptr;
    }
}

#endif

#endif

