#ifdef NDEBUG
#error Audio playback checks require assertions
#endif
#define MA_NO_DEVICE_IO
#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>
#include "playback_buffer.h"
#include <cassert>
#include <cstdio>
#include <initializer_list>

static void check_batched_callbacks() {
    for (ma_uint32 capacity : {960u, playback_aligned_frames(960u)}) {
        ma_pcm_rb ring;
        assert(ma_pcm_rb_init(ma_format_f32, 2, capacity, nullptr, nullptr, &ring) == MA_SUCCESS);
        ma_uint32 count = capacity; void * data = nullptr;
        assert(ma_pcm_rb_acquire_write(&ring, &count, &data) == MA_SUCCESS && count == capacity);
        for (ma_uint32 i = 0; i < count * 2; ++i) ((float *)data)[i] = .25f;
        ma_pcm_rb_commit_write(&ring, count);
        PlaybackRecoveryState state; float out[256]; ma_uint32 missing = 0;
        for (int i = 0; i < 8; ++i) missing += read_playback_pcm(ring, state, out, 128, 2, capacity, []{}).missing;
        assert(missing == (capacity == 960 ? 64u : 0u));
        ma_pcm_rb_uninit(&ring);
    }
}

static void check_split_ramps() {
    ma_pcm_rb ring;
    assert(ma_pcm_rb_init(ma_format_f32, 2, 16, nullptr, nullptr, &ring) == MA_SUCCESS);
    auto fill = [&](ma_uint32 frames) {
        while (frames) { ma_uint32 n = frames; void * p = nullptr;
            assert(ma_pcm_rb_acquire_write(&ring, &n, &p) == MA_SUCCESS && n);
            for (ma_uint32 i = 0; i < n; ++i) { ((float *)p)[i*2] = .5f; ((float *)p)[i*2+1] = -.25f; }
            ma_pcm_rb_commit_write(&ring, n); frames -= n;
        }
    };
    PlaybackRecoveryState state; float out[64];
    fill(1);
    auto gap = read_playback_pcm(ring, state, out, 4, 2, 16, []{});
    assert(gap.missing == 3 && out[0] == .5f && out[1] == -.25f);
    assert(out[6] == .5f * 5.f / 8.f && out[7] == -.25f * 5.f / 8.f);
    auto wait = read_playback_pcm(ring, state, out, 2, 2, 16, []{});
    assert(wait.recovery_silence == 2 && out[2] == .5f * 3.f / 8.f);
    fill(16);
    auto resumed = read_playback_pcm(ring, state, out, 2, 2, 16, []{});
    assert(resumed.recovered && state.fade_in == 6);
    assert(out[0] == .5f * 3.f / 8.f * 7.f / 8.f + .5f / 8.f);
    assert(out[1] == -.25f * 3.f / 8.f * 7.f / 8.f - .25f / 8.f);
    auto next = read_playback_pcm(ring, state, out, 18, 2, 16, []{});
    assert(next.missing == 4 && out[12] == .5f && out[13] == -.25f);
    assert(out[34] == .25f && out[35] == -.125f);
    ma_pcm_rb_uninit(&ring);
}

int main() {
    check_batched_callbacks();
    check_split_ramps();
    ma_pcm_rb ring;
    assert(ma_pcm_rb_init(ma_format_f32, 2, 960, nullptr, nullptr, &ring) == MA_SUCCESS);
    auto produce = [&](ma_uint32 frames, float signal) {
        while (frames) {
            ma_uint32 count = frames; void * data = nullptr;
            assert(ma_pcm_rb_acquire_write(&ring, &count, &data) == MA_SUCCESS && count);
            for (ma_uint32 i = 0; i < count * 2; ++i) ((float *)data)[i] = signal;
            ma_pcm_rb_commit_write(&ring, count); frames -= count;
        }
    };
    PlaybackRecoveryState state;
    int wakes = 0;
    float out[256];
    auto consume = [&] { return read_playback_pcm(ring, state, out, 128, 2, 960, [&]{ ++wakes; }); };
    produce(64, .25f);
    auto first = consume();
    assert(first.missing == 64 && !first.recovered && !first.recovery_silence);
    for (int i = 0; i < 256; ++i) {
        int frame = i / 2;
        float expected = frame < 64 ? .25f : (frame < 72 ? .25f * float(71 - frame) / 8.f : 0.f);
        assert(out[i] == expected);
    }
    produce(128, .5f);
    auto waiting = consume();
    assert(waiting.recovery_silence == 128 && !waiting.missing);
    assert(ma_pcm_rb_available_read(&ring) == 128);
    for (float sample : out) assert(sample == 0.f);
    produce(832, .5f);
    auto resumed = consume();
    assert(resumed.recovered && !resumed.missing && !resumed.recovery_silence);
    for (int i = 0; i < 256; ++i) assert(out[i] == .5f * float(i / 2 < 8 ? i / 2 + 1 : 8) / 8.f);
    for (int i = 0; i < 1000; ++i) {
        produce(128, .5f);
        auto r = consume();
        assert(!r.missing && !r.recovery_silence && !r.recovered);
        for (float sample : out) assert(sample == .5f);
    }
    assert(wakes > 1000);
    ma_pcm_rb_uninit(&ring);
    puts("PASS aligned callback bursts, stereo split ramps, shortage, recovery and wraparound");
}
