#define MINIAUDIO_IMPLEMENTATION
#define MA_NO_DECODING
#define MA_NO_ENCODING
#define MA_NO_RESOURCE_MANAGER
#define MA_NO_NODE_GRAPH
#define MA_NO_ENGINE
#include "miniaudio.h"
#include <emscripten.h>
#include <stdatomic.h>

static ma_device device;
static atomic_uint calls;

static void output(ma_device* d, void* out, const void* in, ma_uint32 frames)
{
    (void)in;
    atomic_fetch_add(&calls, 1);
    for (unsigned i = 0; out != NULL && i < frames * d->playback.channels; i++) {
        ((float*)out)[i] = 0;
    }
}

EMSCRIPTEN_KEEPALIVE int start_probe(int channels, int type)
{
    ma_device_config cfg = ma_device_config_init((ma_device_type)type);
    cfg.capture.format = ma_format_f32;
    cfg.capture.channels = (ma_uint32)channels;
    cfg.playback.format = ma_format_f32;
    cfg.playback.channels = (ma_uint32)channels;
    cfg.sampleRate = 48000;
    cfg.dataCallback = output;
    ma_result result = ma_device_init(NULL, &cfg, &device);
    if (result != MA_SUCCESS) return result;
    return ma_device_start(&device);
}

EMSCRIPTEN_KEEPALIVE int callback_count(void)
{
    return (int)atomic_load(&calls);
}

int main(void)
{
    return 0;
}
