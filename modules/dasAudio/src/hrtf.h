#ifndef hrtf_h
#define hrtf_h

#include <mit_hrtf_lib.h>

#define HRTF_OVERLAP  64

struct ma_hrtf {
    uint32_t taps;
    float *  left;
    float *  right;
    int32_t  azimuth;
    int32_t  elevation;
    uint32_t sampleRate;
    float *  mixbuffer;
    float *  inbuffer;
    uint32_t mixsize;
    short *  leftfip;
    short *  rightfip;
    float    leftOverlap[HRTF_OVERLAP];
    float    rightOverlap[HRTF_OVERLAP];
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
    int localAzimuth = hrtf->azimuth;
    int localElevation = hrtf->elevation;
    mit_hrtf_get(&localAzimuth, &localElevation, hrtf->sampleRate, HRTF_DIFFUSED, hrtf->leftfip, hrtf->rightfip);
}

void ma_hrtf_process_channel ( ma_hrtf* hrtf, float* pOut, const float* pInBuffer, float * history, short * pFilter, float * overlap, ma_uint32 frameCount) {
    uint32_t taps = hrtf->taps;
    uint32_t hist = taps + HRTF_OVERLAP;
    uint32_t mixsize = hist + frameCount;
    if ( mixsize > hrtf->mixsize ) {
        if ( hrtf->mixbuffer ) {
            free(hrtf->mixbuffer);
        }
        if ( hrtf->inbuffer ) {
            free(hrtf->inbuffer);
        }
        hrtf->mixbuffer = (float *) malloc((mixsize + taps) * sizeof(float));
        hrtf->inbuffer = (float *) malloc(mixsize * sizeof(float));
        hrtf->mixsize = mixsize;
    }
    float * pIn = (float *) hrtf->inbuffer;
    for ( uint32_t i=0; i!=hist; ++i ) {
        pIn[i] = history[i];
    }
    for ( uint32_t i=0; i!=frameCount; ++i ) {
        pIn[i+hist] = pInBuffer[i];
    }
    for ( ma_uint32 i=0; i!=hist; i++ ) {
        history[i] = pInBuffer[frameCount + i - hist];
    }

    float * outBuf = hrtf->mixbuffer;
    for ( ma_uint32 i = 0; i != mixsize; i++ ) {
        outBuf[i] = 0.0f;
    }
    for ( ma_uint32 m = 0; m != taps; ++m ) {
        float filter_m = pFilter[m] / 32768.0f;
        for ( ma_uint32 n = 0; n != (frameCount + hist); n++ ) {
            outBuf[n + m] += pIn[n] * filter_m;
        }
    }
    for ( uint32_t i=0; i!=HRTF_OVERLAP; ++i ) {
        float k = ((float)( i + 1)) / ((float)(HRTF_OVERLAP + 1));
        outBuf[taps + i] = outBuf[taps + i] * k + overlap[i] * (1.0f - k);
    }
    for ( ma_uint32 i = 0; i != frameCount; i++ ) {
        pOut[i*2] = outBuf[i + taps];
    }
    for ( uint32_t i=0; i!=HRTF_OVERLAP; ++i ) {
        overlap[i] = outBuf[frameCount + taps + i];
    }
}

void ma_hrtf_process_pcm_frames_mono(ma_hrtf* hrtf, float* pOut, const float* pIn, ma_uint32 frameCount) {
    ma_hrtf_process_channel(hrtf, pOut, pIn, hrtf->left, hrtf->leftfip, hrtf->leftOverlap, frameCount);
    ma_hrtf_process_channel(hrtf, pOut+1, pIn, hrtf->right, hrtf->rightfip, hrtf->rightOverlap, frameCount);
}

void ma_hrtf_process_frames(ma_hrtf * hrtf, float * pOut, const float * pIn, ma_uint32 nChannels, ma_uint32 frameCount) {
    if ( hrtf->taps==0 ) return;
    if ( nChannels == 1 ) {
        ma_hrtf_process_pcm_frames_mono(hrtf, pOut, pIn, frameCount);
    } else {
        float * pInMono = (float *) malloc(frameCount * sizeof(float));
        float rChannels = 1.0f / nChannels;
        for ( ma_uint32 i = 0; i != frameCount; i++ ) {
            float m = 0.0f;
            for ( ma_uint32 j = 0; j != nChannels; j++ ) {
                m += pIn[i*nChannels+j];
            }
            pInMono[i] = m * rChannels;
        }
        ma_hrtf_process_pcm_frames_mono(hrtf, pOut, pInMono, frameCount);
        free(pInMono);
    }
}

void ma_hrtf_init(ma_hrtf* hrtf, ma_uint32 sampleRate) {
    hrtf->taps = mit_hrtf_availability(0, 0, sampleRate, HRTF_DIFFUSED); // diffused vs regular
    if ( hrtf->taps ) {
        uint32_t history = hrtf->taps + HRTF_OVERLAP;
        hrtf->left = (float *) malloc(history * sizeof(float));
        hrtf->right = (float *) malloc(history * sizeof(float));
        hrtf->leftfip = (short *) malloc(hrtf->taps * sizeof(short));
        hrtf->rightfip = (short *) malloc(hrtf->taps * sizeof(short));
        for ( uint32_t i = 0; i != history; ++i ) {
            hrtf->left[i] = 0.0f;
            hrtf->right[i] = 0.0f;
        }
        for ( uint32_t i = 0; i != hrtf->taps; ++i ) {
            hrtf->leftfip[i] = 0;
            hrtf->rightfip[i] = 0;
        }
    } else {
        hrtf->left = nullptr;
        hrtf->right = nullptr;
    }
    hrtf->azimuth = 0;
    hrtf->elevation = 0;
    hrtf->sampleRate = sampleRate;
    hrtf->mixbuffer = nullptr;
    hrtf->inbuffer = nullptr;
    hrtf->mixsize = 0;
    for ( uint32_t i=0; i!=HRTF_OVERLAP; i++ ) {
        hrtf->leftOverlap[i] = 0.0f;
        hrtf->rightOverlap[i] = 0.0f;
    }
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
    if ( hrtf->mixbuffer ) {
        free(hrtf->mixbuffer);
        hrtf->mixbuffer = nullptr;
    }
    if ( hrtf->inbuffer ) {
        free(hrtf->inbuffer);
        hrtf->inbuffer = nullptr;
    }
    if ( hrtf->leftfip ) {
        free(hrtf->leftfip);
        hrtf->leftfip = nullptr;
    }
    if ( hrtf->rightfip ) {
        free(hrtf->rightfip);
        hrtf->rightfip = nullptr;
    }
}

#endif

#endif

