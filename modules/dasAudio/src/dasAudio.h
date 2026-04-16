#define MA_USE_STDINT
#include <miniaudio.h>
#include "volume_mixer.h"
#include "sf2_voice.h"
#include "hrtf.h"
#include "reverb.h"
#include "chorus.h"

// AOT-visible declarations for global-scope functions called from namespace das
float das_ma_sf2_biquad_tick ( ma_sf2_biquad * bq, float input );

namespace das {
    // pull global-scope functions into namespace das for AOT-generated code
    using ::ma_volume_mixer_init;
    using ::ma_volume_mixer_set_pan;
    using ::ma_volume_mixer_set_pan_immediate;
    using ::ma_volume_mixer_set_linear_pan;
    using ::ma_volume_mixer_process_pcm_frames;
    using ::ma_sf2_biquad_setup;
    using ::ma_sf2_biquad_setup_hpf;
    using ::das_ma_sf2_biquad_tick;
    using ::ma_sf2_voice_init;
    using ::ma_sf2_voice_note_off;
    using ::ma_sf2_voice_end_quick;
    using ::ma_sf2_voice_render;
    using ::ma_sf2_voice_render_send;
    using ::ma_sf2_voice_render_send2;
    using ::ma_sf2_voice_render_send3;
    using ::ma_sf2_voice_is_finished;
    using ::ma_sf2_envelope_init;
    using ::ma_sf2_envelope_start;
    using ::ma_sf2_envelope_tick;
    using ::ma_sf2_envelope_release;

    bool dasAudio_init ( TFunc<void,TTemporary<TArray<float>>,int32_t,int32_t,float> mixer, int32_t rate, int32_t channels, Context & context );
    void dasAudio_finalize ( void );
    MA_API ma_result dasAudio_ma_resampler_init(const ma_resampler_config* pConfig, ma_resampler* pResampler);
    MA_API void dasAudio_ma_resampler_uninit(ma_resampler* pResampler);
    MA_API ma_result dasAudio_ma_channel_converter_init(const ma_channel_converter_config* pConfig, ma_channel_converter* pConverter);
    MA_API void dasAudio_ma_channel_converter_uninit(ma_channel_converter* pConverter);
    MA_API ma_uint64 dasAudio_ma_resampler_get_required_input_frame_count(const ma_resampler* pResampler, ma_uint64 outputFrameCount);
    MA_API ma_uint64 dasAudio_ma_resampler_get_expected_output_frame_count(const ma_resampler* pResampler, ma_uint64 inputFrameCount);
    void dasAudio_disableLinearResamplerFiltering ( ma_resampler_config * config );
    MA_API ma_result dasAudio_ma_decoder_init_memory(const void* pData, ma_uint64 dataSize, const ma_decoder_config* pConfig, ma_decoder* pDecoder);
    MA_API ma_uint64 dasAudio_ma_decoder_get_length_in_pcm_frames(ma_decoder* pDecoder);
    MA_API ma_uint64 dasAudio_ma_decoder_read_pcm_frames(ma_decoder* pDecoder, void* pFramesOut, ma_uint64 frameCount);

    void dasAudio_setSampleRate ( I3DL2Reverb * reverb, float rate, Context * context, LineInfoArg * at );
    void dasAudio_setProperties ( I3DL2Reverb * reverb, const I3DL2ReverbProperties & props, Context * context, LineInfoArg * at );
    void dasAudio_process ( I3DL2Reverb * reverb, float * buffer, float * outBuffer, int nSamples, Context * context, LineInfoArg * at );
    void dasAudio_processMono ( I3DL2Reverb * reverb, float * buffer, float * outBuffer, int nSamples, Context * context, LineInfoArg * at );
    I3DL2ReverbProperties & dasAudio_getReverbPreset ( I3DL2Preset preset, Context * context, LineInfoArg * at );

    Context & dasAudio_mixerContext ( Context * context, LineInfoArg * at );

    void dasAudio_chorusInit ( ma_chorus * chorus, float sample_rate, Context * context, LineInfoArg * at );
    void dasAudio_chorusProcess ( ma_chorus * chorus, float * input, float * output, int nSamples, Context * context, LineInfoArg * at );
    void dasAudio_chorusSetConfig ( ma_chorus * chorus, const ma_chorus_config & config, Context * context, LineInfoArg * at );
    ma_chorus_config dasAudio_chorusConfigDefault ( );

    void dasAudio_delayInit ( ma_delay * d, int sample_rate, float delay_time_sec, float feedback, Context * context, LineInfoArg * at );
    void dasAudio_delayUninit ( ma_delay * d, Context * context, LineInfoArg * at );
    void dasAudio_delaySetParams ( ma_delay * d, int sample_rate, float delay_time_sec, float feedback, Context * context, LineInfoArg * at );
    void dasAudio_delayProcess ( ma_delay * d, float * input, float * output, int nFrames, Context * context, LineInfoArg * at );
}
