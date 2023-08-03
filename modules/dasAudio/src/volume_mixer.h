#ifndef volume_mixer_h
#define volume_mixer_h

// i keep it in ma_ style, and potentially will make a PR to miniaudio at some point
// #include <miniaudio.h>

// volume mixer

struct ma_volume_mixer {
    float volume;
    float dvolume;
    float tvolume;
    float pan;
    uint32_t nChannels;
};

void ma_volume_mixer_init ( ma_volume_mixer * mixer, uint32_t nChannels );
void ma_volume_mixer_uninit ( ma_volume_mixer * mixer );
void ma_volume_mixer_set_channels ( ma_volume_mixer * mixer, uint32_t nChannels );
void ma_volume_mixer_set_volume ( ma_volume_mixer * mixer, float volume );
void ma_volume_mixer_set_volume_over_time ( ma_volume_mixer * mixer, float volume, uint64_t nFrames );
void ma_volume_mixer_set_pan ( ma_volume_mixer * mixer, float pan );
void ma_volume_mixer_process_pcm_frames ( ma_volume_mixer * mixer, float * InFrames, float * OutFrames, uint64_t nFrames );

// look-ahead limiter

struct ma_limiter {
    float gain[MA_MAX_CHANNELS];
    uint32_t nChannels;
    uint32_t attack_samples;
    float    threshold;
    float    attack_coeff;
    float    release_coeff;
};

void ma_limiter_init ( ma_limiter * limiter, float threshold, float attack_time, float release_time, float sample_rate, uint32_t nChannels );
void ma_limiter_process_pcm_frames ( ma_limiter * limiter, float * InFames, float * OutFrames, uint64_t nFrames );
uint64_t ma_limiter_get_required_input_frame_count ( ma_limiter * limiter, uint64_t out_len );
void ma_limiter_uninit ( ma_limiter * );

#ifdef MINIAUDIO_IMPLEMENTATION

void ma_volume_mixer_init ( ma_volume_mixer * mixer, uint32_t nChannels ) {
    mixer->volume = 1.0f;
    mixer->dvolume = 0.0f;
    mixer->tvolume = 1.0f;
    mixer->pan = 0.0f;
    mixer->nChannels = nChannels;
}

void ma_volume_mixer_uninit ( ma_volume_mixer * ) {
    // do nothing
}

void ma_volume_mixer_set_channels ( ma_volume_mixer * mixer, uint32_t nChannels ) {
    mixer->nChannels = nChannels;
}

void ma_volume_mixer_set_volume ( ma_volume_mixer * mixer, float volume ) {
    mixer->volume = volume;
    mixer->dvolume = 0.0f;
    mixer->tvolume = volume;
}

void ma_volume_mixer_set_pan ( ma_volume_mixer * mixer, float pan ) {
    mixer->pan = pan;
}

void ma_volume_mixer_set_volume_over_time ( ma_volume_mixer * mixer, float volume, uint64_t nFrames ) {
    mixer->dvolume = (volume - mixer->volume) / float(nFrames);
    mixer->tvolume = volume;
}

void ma_volume_mixer_process_pcm_frames_linear ( ma_volume_mixer * mixer, float * InFrames, float * OutFrames, uint64_t nFrames ) {
    float volume = mixer->volume;
    uint32_t nChannels = mixer->nChannels;
    if ( nChannels==1 ) {
        for ( uint64_t i=0; i!=nFrames; ++i ) {
            OutFrames[i] += InFrames[i] * volume;
        }
    } else if ( nChannels==2 ) {
        float pan = ma_max(ma_min(mixer->pan,1.0f),-1.0f);
        float volumeL = volume * ma_min(1.0f + pan, 1.0f);
        float volumeR = volume * ma_min(1.0f - pan, 1.0f);
        for ( uint64_t i=0; i!=nFrames; ++i ) {
            OutFrames[i*2+0] += InFrames[i*2+0] * volumeR;
            OutFrames[i*2+1] += InFrames[i*2+1] * volumeL;
        }
    } else if ( nChannels==4 ) {
        for ( uint64_t i=0; i!=nFrames; ++i ) {
            OutFrames[i*4+0] += InFrames[i*4+0] * volume;
            OutFrames[i*4+1] += InFrames[i*4+1] * volume;
            OutFrames[i*4+2] += InFrames[i*4+2] * volume;
            OutFrames[i*4+3] += InFrames[i*4+3] * volume;
        }
    } else {
        for ( uint64_t i=0; i!=nFrames; ++i ) {
            for ( uint32_t j=0; j!=nChannels; ++j ) {
                OutFrames[i*nChannels+j] += InFrames[i*nChannels+j] * volume;
            }
        }
    }
}

void ma_volume_mixer_process_pcm_frames_descending ( ma_volume_mixer * mixer, float * InFrames, float * OutFrames, uint64_t nFrames ) {
    uint32_t nChannels = mixer->nChannels;
    float volume = mixer->volume;
    float dvolume = mixer->dvolume;
    float tvolume = mixer->tvolume;
    float pan = ma_max(ma_min(mixer->pan,1.0f),-1.0f);
    if ( nChannels==1 ) {
        for ( uint64_t i=0; i!=nFrames; ++i ) {
            OutFrames[i] += InFrames[i] * volume;
            volume = ma_max(volume+dvolume,tvolume);
        }
    } else if ( nChannels==2 ) {
        for ( uint64_t i=0; i!=nFrames; ++i ) {
            float volumeL = volume * ma_min(1.0f + pan, 1.0f);
            float volumeR = volume * ma_min(1.0f - pan, 1.0f);
            OutFrames[i*2+0] += InFrames[i*2+0] * volumeR;
            OutFrames[i*2+1] += InFrames[i*2+1] * volumeL;
            volume = ma_max(volume+dvolume,tvolume);
        }
    } else if ( nChannels==4 ) {
        for ( uint64_t i=0; i!=nFrames; ++i ) {
            OutFrames[i*4+0] += InFrames[i*4+0] * volume;
            OutFrames[i*4+1] += InFrames[i*4+1] * volume;
            OutFrames[i*4+2] += InFrames[i*4+2] * volume;
            OutFrames[i*4+3] += InFrames[i*4+3] * volume;
            volume = ma_max(volume+dvolume,tvolume);
        }
    } else {
        for ( uint64_t i=0; i!=nFrames; ++i ) {
            for ( uint32_t j=0; j!=nChannels; ++j ) {
                OutFrames[i*nChannels+j] += InFrames[i*nChannels+j] * volume;
                volume = ma_max(volume+dvolume,tvolume);
            }
        }
    }
    mixer->volume = volume;
    mixer->dvolume = volume==tvolume ? 0.0f : dvolume;
}

void ma_volume_mixer_process_pcm_frames_ascending ( ma_volume_mixer * mixer, float * InFrames, float * OutFrames, uint64_t nFrames ) {
    uint32_t nChannels = mixer->nChannels;
    float volume = mixer->volume;
    float dvolume = mixer->dvolume;
    float tvolume = mixer->tvolume;
    float pan = ma_max(ma_min(mixer->pan,1.0f),-1.0f);
    if ( nChannels==1 ) {
        for ( uint64_t i=0; i!=nFrames; ++i ) {
            OutFrames[i] += InFrames[i] * volume;
            volume = ma_min(volume+dvolume,tvolume);
        }
    } else if ( nChannels==2 ) {
        for ( uint64_t i=0; i!=nFrames; ++i ) {
            float volumeL = volume * ma_min(1.0f + pan, 1.0f);
            float volumeR = volume * ma_min(1.0f - pan, 1.0f);
            OutFrames[i*2+0] += InFrames[i*2+0] * volumeR;
            OutFrames[i*2+1] += InFrames[i*2+1] * volumeL;
            volume = ma_min(volume+dvolume,tvolume);
        }
    } else if ( nChannels==4 ) {
        for ( uint64_t i=0; i!=nFrames; ++i ) {
            OutFrames[i*4+0] += InFrames[i*4+0] * volume;
            OutFrames[i*4+1] += InFrames[i*4+1] * volume;
            OutFrames[i*4+2] += InFrames[i*4+2] * volume;
            OutFrames[i*4+3] += InFrames[i*4+3] * volume;
            volume = ma_min(volume+dvolume,tvolume);
        }
    } else {
        for ( uint64_t i=0; i!=nFrames; ++i ) {
            for ( uint32_t j=0; j!=nChannels; ++j ) {
                OutFrames[i*nChannels+j] += InFrames[i*nChannels+j] * volume;
                volume = ma_min(volume+dvolume,tvolume);
            }
        }
    }
    mixer->volume = volume;
    mixer->dvolume = volume==tvolume ? 0.0f : dvolume;
}

void ma_volume_mixer_process_pcm_frames ( ma_volume_mixer * mixer, float * InFrames, float * OutFrames, uint64_t nFrames ) {
    if ( mixer->dvolume==0.0f ) {
        ma_volume_mixer_process_pcm_frames_linear(mixer,InFrames,OutFrames,nFrames);
    } else if ( mixer->dvolume<0.0f ) {
        ma_volume_mixer_process_pcm_frames_descending(mixer,InFrames,OutFrames,nFrames);
    } else {
        ma_volume_mixer_process_pcm_frames_ascending(mixer,InFrames,OutFrames,nFrames);
    }
}

void ma_limiter_init ( ma_limiter * limiter, float threshold, float attack_time, float release_time, float sample_rate, uint32_t nChannels ) {
    limiter->nChannels = nChannels;
    limiter->threshold = threshold;
    limiter->attack_samples = (int)(attack_time * sample_rate);
    limiter->attack_coeff = expf(-1.0f / (limiter->attack_samples));
    limiter->release_coeff = expf(-1.0f / (release_time));
    for ( uint32_t i=0; i!=MA_MAX_CHANNELS; ++i ) {
        limiter->gain[i] = 1.0f;
    }
}

void ma_limiter_uninit ( ma_limiter * ) {
    // do nothing
}

uint64_t ma_limiter_get_required_input_frame_count ( ma_limiter * limiter, uint64_t out_len ) {
    return out_len + limiter->attack_samples;
}

void ma_limiter_porcess_pcm_frames_mono ( ma_limiter * limiter, float * InFames, float * OutFrames, uint64_t nFrames ) {
    float gain = limiter->gain[0];
    float attack_coeff = limiter->attack_coeff;
    float release_coeff = limiter->release_coeff;
    float threshold = limiter->threshold;
    uint32_t attack_samples = limiter->attack_samples;
    for ( uint64_t i=0; i!=nFrames; ++i ) {
        float lookahead_sample = InFames[i + attack_samples];
        if (fabs(lookahead_sample) * gain > threshold) {
            float desired_gain = threshold / fabs(lookahead_sample);
            gain = gain * attack_coeff + desired_gain * (1 - attack_coeff);
        } else {
            gain = gain * release_coeff + 1.0f * (1 - release_coeff);
        }
        OutFrames[i] = InFames[i] * gain;
    }
    limiter->gain[0] = gain;
}

void ma_limiter_porcess_pcm_frames_stereo ( ma_limiter * limiter, float * InFames, float * OutFrames, uint64_t nFrames ) {
    float gain[2] = { limiter->gain[0], limiter->gain[1] };
    float attack_coeff = limiter->attack_coeff;
    float release_coeff = limiter->release_coeff;
    float threshold = limiter->threshold;
    uint32_t attack_samples = limiter->attack_samples;
    for ( uint64_t i=0; i!=nFrames; ++i ) {
        float lookahead_sample = InFames[i*2 + attack_samples*2];
        if (fabs(lookahead_sample) * gain[0] > threshold) {
            float desired_gain = threshold / fabs(lookahead_sample);
            gain[0] = gain[0] * attack_coeff + desired_gain * (1 - attack_coeff);
        } else {
            gain[0] = gain[0] * release_coeff + 1.0f * (1 - release_coeff);
        }
        lookahead_sample = InFames[i*2 + attack_samples*2 + 1];
        if (fabs(lookahead_sample) * gain[1] > threshold) {
            float desired_gain = threshold / fabs(lookahead_sample);
            gain[1] = gain[1] * attack_coeff + desired_gain * (1 - attack_coeff);
        } else {
            gain[1] = gain[1] * release_coeff + 1.0f * (1 - release_coeff);
        }
        OutFrames[i*2+0] = InFames[i*2+0] * gain[0];
        OutFrames[i*2+1] = InFames[i*2+1] * gain[1];
    }
    limiter->gain[0] = gain[0];
    limiter->gain[1] = gain[1];
}

void ma_limiter_porcess_pcm_frames_any ( ma_limiter * limiter, float * InFames, float * OutFrames, uint64_t nFrames ) {
    float gain[MA_MAX_CHANNELS];
    for ( uint32_t i=0; i!=limiter->nChannels; ++i ) {
        gain[i] = limiter->gain[i];
    }
    float attack_coeff = limiter->attack_coeff;
    float release_coeff = limiter->release_coeff;
    float threshold = limiter->threshold;
    uint32_t attack_samples = limiter->attack_samples;
    for ( uint64_t i=0; i!=nFrames; ++i ) {
        for ( uint32_t j=0; j!=limiter->nChannels; ++j ) {
            float lookahead_sample = InFames[i*limiter->nChannels + j + attack_samples*limiter->nChannels];
            if (fabs(lookahead_sample) * gain[j] > threshold) {
                float desired_gain = threshold / fabs(lookahead_sample);
                gain[j] = gain[j] * attack_coeff + desired_gain * (1 - attack_coeff);
            } else {
                gain[j] = gain[j] * release_coeff + 1.0f * (1 - release_coeff);
            }
            OutFrames[i*limiter->nChannels+j] = InFames[i*limiter->nChannels+j] * gain[j];
        }
    }
    for ( uint32_t i=0; i!=limiter->nChannels; ++i ) {
        limiter->gain[i] = gain[i];
    }
}

void ma_limiter_process_pcm_frames ( ma_limiter * limiter, float * InFames, float * OutFrames, uint64_t nFrames ) {
    if ( limiter->nChannels==1 ) {
        ma_limiter_porcess_pcm_frames_mono(limiter, InFames, OutFrames, nFrames);
    } else if ( limiter->nChannels==2 ) {
        ma_limiter_porcess_pcm_frames_stereo(limiter, InFames, OutFrames, nFrames);
    } else {
        ma_limiter_porcess_pcm_frames_any(limiter, InFames, OutFrames, nFrames);
    }
}

#endif

#endif
