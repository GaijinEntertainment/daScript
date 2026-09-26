#pragma once

struct PlaybackRecoveryState {
    static constexpr ma_uint32 ramp_frames = 8;
    bool recovering = false;
    ma_uint32 fade_out = 0, fade_in = 0;
    float last[MA_MAX_CHANNELS] = {};
    float resume_from[MA_MAX_CHANNELS] = {};
};
struct PlaybackReadResult {
    ma_uint32 missing = 0;
    ma_uint32 recovery_silence = 0;
    bool recovered = false;
};

// modules/dasAudio/ARCHITECTURE.md#threaded-webassembly-output
inline ma_uint32 playback_aligned_frames(ma_uint32 frames, ma_uint32 quantum = 128) {
    return ((frames + quantum - 1) / quantum) * quantum;
}

inline void playback_missing_pcm(PlaybackRecoveryState & state, float * output,
                                 ma_uint32 frames, ma_uint32 channels) {
    while (frames && state.fade_out) {
        const float gain = float(state.fade_out - 1) / float(state.fade_out);
        for (ma_uint32 c = 0; c < channels; ++c) {
            state.last[c] *= gain;
            *output++ = state.last[c];
        }
        --state.fade_out;
        --frames;
    }
    memset(output, 0, size_t(frames) * channels * sizeof(float));
}

template <typename Wake>
PlaybackReadResult read_playback_pcm(ma_pcm_rb & ring, PlaybackRecoveryState & state,
                                    float * output, ma_uint32 frames, ma_uint32 channels,
                                    ma_uint32 capacity, Wake wake) {
    PlaybackReadResult result;
    if (state.recovering) {
        if (ma_pcm_rb_available_read(&ring) < capacity) {
            playback_missing_pcm(state, output, frames, channels);
            result.recovery_silence = frames;
            wake();
            return result;
        }
        state.recovering = false;
        state.fade_in = PlaybackRecoveryState::ramp_frames;
        state.fade_out = 0;
        for (ma_uint32 c = 0; c < channels; ++c) state.resume_from[c] = state.last[c];
        result.recovered = true;
    }
    while (frames) {
        ma_uint32 count = frames;
        void * input = nullptr;
        if (ma_pcm_rb_acquire_read(&ring, &count, &input) != MA_SUCCESS || !count) {
            state.fade_out = PlaybackRecoveryState::ramp_frames;
            state.fade_in = 0;
            playback_missing_pcm(state, output, frames, channels);
            result.missing = frames;
            state.recovering = true;
            wake();
            break;
        }
        memcpy(output, input, size_t(count) * channels * sizeof(float));
        for (ma_uint32 f = 0; f < count && state.fade_in; ++f) {
            const float gain = float(PlaybackRecoveryState::ramp_frames - state.fade_in + 1)
                               / float(PlaybackRecoveryState::ramp_frames);
            for (ma_uint32 c = 0; c < channels; ++c) {
                auto & value = output[size_t(f) * channels + c];
                value = state.resume_from[c] * (1.f - gain) + value * gain;
            }
            --state.fade_in;
        }
        for (ma_uint32 c = 0; c < channels; ++c) state.last[c] = output[size_t(count - 1) * channels + c];
        ma_pcm_rb_commit_read(&ring, count);
        wake();
        output += size_t(count) * channels;
        frames -= count;
    }
    return result;
}
