#include <miniaudio.h>
#include "volume_mixer.h"
#include "hrtf.h"
#include "reverb.h"

namespace das {
    bool dasAudio_init ( TFunc<void,TTemporary<TArray<float>>,int32_t,int32_t,float> mixer, int32_t rate, int32_t channels, Context & context );
    void dasAudio_finalize ( void );
    MA_API ma_result dasAudio_ma_resampler_init(const ma_resampler_config* pConfig, ma_resampler* pResampler);
    MA_API void dasAudio_ma_resampler_uninit(ma_resampler* pResampler);
    MA_API ma_result dasAudio_ma_channel_converter_init(const ma_channel_converter_config* pConfig, ma_channel_converter* pConverter);
    MA_API void dasAudio_ma_channel_converter_uninit(ma_channel_converter* pConverter);
    MA_API ma_uint64 dasAudio_ma_resampler_get_required_input_frame_count(const ma_resampler* pResampler, ma_uint64 outputFrameCount);
    MA_API ma_uint64 dasAudio_ma_resampler_get_expected_output_frame_count(const ma_resampler* pResampler, ma_uint64 inputFrameCount);
    MA_API ma_uint64 dasAudio_ma_decoder_get_length_in_pcm_frames(ma_decoder* pDecoder);
    MA_API ma_uint64 dasAudio_ma_decoder_read_pcm_frames(ma_decoder* pDecoder, void* pFramesOut, ma_uint64 frameCount);

    void dasAudio_setSampleRate ( I3DL2Reverb * reverb, float rate, Context * context, LineInfoArg * at );
    void dasAudio_setProperties ( I3DL2Reverb * reverb, const I3DL2ReverbProperties & props, Context * context, LineInfoArg * at );
    void dasAudio_process ( I3DL2Reverb * reverb, float * buffer, float * outBuffer, int nSamples, Context * context, LineInfoArg * at );
    void dasAudio_processMono ( I3DL2Reverb * reverb, float * buffer, float * outBuffer, int nSamples, Context * context, LineInfoArg * at );
    I3DL2ReverbProperties & dasAudio_getReverbPreset ( I3DL2Preset preset, Context * context, LineInfoArg * at );
}
