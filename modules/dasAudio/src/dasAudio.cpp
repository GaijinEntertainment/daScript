#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_typefactory_bind.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/simulate/bind_enum.h"

// include vorbis extras before miniaudio
#define STB_VORBIS_HEADER_ONLY
#ifdef __forceinline
#undef __forceinline
#define __forceinline inline
#endif
#include <extras/stb_vorbis.c>    /* Enables Vorbis decoding. */

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4755)
#pragma warning(disable:4701)
#endif

#ifdef __APPLE__
    #define MA_NO_RUNTIME_LINKING
#endif
#define MINIAUDIO_IMPLEMENTATION
#define MA_USE_STDINT
#include <miniaudio.h>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include "volume_mixer.h"
#include "hrtf.h"

#define I3DL32_REVERB_IMPLEMENTATION    1
#include "reverb.h"

#define MA_CHORUS_IMPLEMENTATION
#include "chorus.h"

#define CONVOLUTION_REVERB_IMPLEMENTATION
#include "convolution_reverb.h"

#define MA_EFFECTS_IMPLEMENTATION
#include "effects.h"

#define MA_PHASER_IMPLEMENTATION
#include "phaser.h"

#define MA_TREMOLO_IMPLEMENTATION
#include "tremolo.h"

#define MA_COMPRESSOR_IMPLEMENTATION
#include "compressor.h"

#include "dasAudio.h"

#ifndef HRTF_SAMPLE_RATE
#define HRTF_SAMPLE_RATE 48000
#endif

// Non-static wrapper for ma_sf2_biquad_tick (static in sf2_voice.h)
float das_ma_sf2_biquad_tick ( ma_sf2_biquad * bq, float input ) {
    return ma_sf2_biquad_tick(bq, input);
}

MAKE_EXTERNAL_TYPE_FACTORY(Context,Context);

das::Context* get_clone_context( das::Context * ctx, uint32_t category );//link time resolved dependencies

MAKE_TYPE_FACTORY(ma_resampler_config,ma_resampler_config);
MAKE_TYPE_FACTORY(ma_resampler,ma_resampler);

MAKE_TYPE_FACTORY(ma_channel_converter_config,ma_channel_converter_config);
MAKE_TYPE_FACTORY(ma_channel_converter,ma_channel_converter);

MAKE_TYPE_FACTORY(ma_volume_mixer,ma_volume_mixer);

MAKE_TYPE_FACTORY(ma_decoder_config,ma_decoder_config);
MAKE_TYPE_FACTORY(ma_decoder,ma_decoder);

MAKE_TYPE_FACTORY(ma_limiter,ma_limiter);

MAKE_TYPE_FACTORY(ma_sf2_envelope,ma_sf2_envelope);
MAKE_TYPE_FACTORY(ma_sf2_biquad,ma_sf2_biquad);
MAKE_TYPE_FACTORY(ma_sf2_voice,ma_sf2_voice);

MAKE_TYPE_FACTORY(ma_chorus_config,ma_chorus_config);
MAKE_TYPE_FACTORY(ma_chorus,ma_chorus);

MAKE_TYPE_FACTORY(ma_delay,ma_delay);

MAKE_TYPE_FACTORY(ma_bitcrush,ma_bitcrush);
MAKE_TYPE_FACTORY(ma_waveshaper,ma_waveshaper);
MAKE_TYPE_FACTORY(ma_djfilter,ma_djfilter);
MAKE_TYPE_FACTORY(ma_bandpass,ma_bandpass);

MAKE_TYPE_FACTORY(ma_phaser,ma_phaser);
MAKE_TYPE_FACTORY(ma_tremolo,ma_tremolo);
MAKE_TYPE_FACTORY(ma_compressor,ma_compressor);

DAS_BASE_BIND_ENUM ( ma_format, ma_format, \
    ma_format_unknown, \
    ma_format_u8, \
    ma_format_s16, \
    ma_format_s24, \
    ma_format_s32, \
    ma_format_f32 \
);
DAS_BIND_ENUM_CAST ( ma_format );

DAS_BASE_BIND_ENUM ( ma_resample_algorithm, ma_resample_algorithm, \
    ma_resample_algorithm_linear, \
    ma_resample_algorithm_custom \
);
DAS_BIND_ENUM_CAST ( ma_resample_algorithm );

DAS_BASE_BIND_ENUM ( ma_channel_mix_mode, ma_channel_mix_mode, \
    ma_channel_mix_mode_rectangular, \
    ma_channel_mix_mode_simple, \
    ma_channel_mix_mode_custom_weights, \
    ma_channel_mix_mode_default \
);
DAS_BIND_ENUM_CAST ( ma_channel_mix_mode );

DAS_BASE_BIND_ENUM ( ma_dither_mode, ma_dither_mode, \
    ma_dither_mode_none, \
    ma_dither_mode_rectangle, \
    ma_dither_mode_triangle \
);
DAS_BIND_ENUM_CAST ( ma_dither_mode );

DAS_BASE_BIND_ENUM ( ma_result, ma_result, \
    MA_SUCCESS                        ,\
    MA_ERROR                          ,\
    MA_INVALID_ARGS                   ,\
    MA_INVALID_OPERATION              ,\
    MA_OUT_OF_MEMORY                  ,\
    MA_OUT_OF_RANGE                   ,\
    MA_ACCESS_DENIED                  ,\
    MA_DOES_NOT_EXIST                 ,\
    MA_ALREADY_EXISTS                 ,\
    MA_TOO_MANY_OPEN_FILES            ,\
    MA_INVALID_FILE                   ,\
    MA_TOO_BIG                        ,\
    MA_PATH_TOO_LONG                  ,\
    MA_NAME_TOO_LONG                  ,\
    MA_NOT_DIRECTORY                  ,\
    MA_IS_DIRECTORY                   ,\
    MA_DIRECTORY_NOT_EMPTY            ,\
    MA_AT_END                         ,\
    MA_NO_SPACE                       ,\
    MA_BUSY                           ,\
    MA_IO_ERROR                       ,\
    MA_INTERRUPT                      ,\
    MA_UNAVAILABLE                    ,\
    MA_ALREADY_IN_USE                 ,\
    MA_BAD_ADDRESS                    ,\
    MA_BAD_SEEK                       ,\
    MA_BAD_PIPE                       ,\
    MA_DEADLOCK                       ,\
    MA_TOO_MANY_LINKS                 ,\
    MA_NOT_IMPLEMENTED                ,\
    MA_NO_MESSAGE                     ,\
    MA_BAD_MESSAGE                    ,\
    MA_NO_DATA_AVAILABLE              ,\
    MA_INVALID_DATA                   ,\
    MA_TIMEOUT                        ,\
    MA_NO_NETWORK                     ,\
    MA_NOT_UNIQUE                     ,\
    MA_NOT_SOCKET                     ,\
    MA_NO_ADDRESS                     ,\
    MA_BAD_PROTOCOL                   ,\
    MA_PROTOCOL_UNAVAILABLE           ,\
    MA_PROTOCOL_NOT_SUPPORTED         ,\
    MA_PROTOCOL_FAMILY_NOT_SUPPORTED  ,\
    MA_ADDRESS_FAMILY_NOT_SUPPORTED   ,\
    MA_SOCKET_NOT_SUPPORTED           ,\
    MA_CONNECTION_RESET               ,\
    MA_ALREADY_CONNECTED              ,\
    MA_NOT_CONNECTED                  ,\
    MA_CONNECTION_REFUSED             ,\
    MA_NO_HOST                        ,\
    MA_IN_PROGRESS                    ,\
    MA_CANCELLED                      ,\
    MA_MEMORY_ALREADY_MAPPED          ,\
    MA_FORMAT_NOT_SUPPORTED           ,\
    MA_DEVICE_TYPE_NOT_SUPPORTED      ,\
    MA_SHARE_MODE_NOT_SUPPORTED       ,\
    MA_NO_BACKEND                     ,\
    MA_NO_DEVICE                      ,\
    MA_API_NOT_FOUND                  ,\
    MA_INVALID_DEVICE_CONFIG          ,\
    MA_LOOP                           ,\
    MA_DEVICE_NOT_INITIALIZED         ,\
    MA_DEVICE_ALREADY_INITIALIZED     ,\
    MA_DEVICE_NOT_STARTED             ,\
    MA_DEVICE_NOT_STOPPED             ,\
    MA_FAILED_TO_INIT_BACKEND         ,\
    MA_FAILED_TO_OPEN_BACKEND_DEVICE  ,\
    MA_FAILED_TO_START_BACKEND_DEVICE ,\
    MA_FAILED_TO_STOP_BACKEND_DEVICE  \
);
DAS_BIND_ENUM_CAST ( ma_result );

DAS_BASE_BIND_ENUM( das::I3DL2Preset, I3DL2Preset, \
    Generic, \
    PaddedCell, \
    Room, \
    Bathroom, \
    LivingRoom, \
    StoneRoom, \
    Auditorium, \
    ConcertHall, \
    Cave, \
    Arena, \
    Hangar, \
    CarpetedHallway, \
    Hallway, \
    StoneCorridor, \
    Alley, \
    Forest, \
    City, \
    Mountains, \
    Quarry, \
    Plain, \
    ParkingLot, \
    SewerPipe, \
    Underwater \
);
DAS_BIND_ENUM_CAST ( das::I3DL2Preset );

MAKE_TYPE_FACTORY(I3DL2ReverbProperties,I3DL2ReverbProperties);
MAKE_TYPE_FACTORY(I3DL2Reverb,I3DL2Reverb);


MAKE_TYPE_FACTORY(ma_hrtf,ma_hrtf);
MAKE_TYPE_FACTORY(ConvolutionReverb,ConvolutionReverb);

namespace das {

static ma_device g_device;
static ma_log g_ma_log_struct;
static shared_ptr<Context> g_mixer_context;
static daScriptEnvironment * g_mixer_env = nullptr;
static Func g_mixer_function = (void *) nullptr;
static bool g_mixer_initialized = false;
static int g_rate = 0;
static int g_channels = 0;

void on_error_log ( void * , ma_uint32 level, const char * message ) {
    if (level <= 1) {
        LOG(LogLevel::error) << message;
    } else {
        LOG(LogLevel::info) << message;
    }
}

void data_callback(ma_device*, void* pOutput, const void*, ma_uint32 frameCount) {
    float fdt = 1.0f / float(g_rate);
    Array buffer;
    array_mark_locked(buffer, pOutput, frameCount * g_channels);
    lock_guard<recursive_mutex> guard(*g_mixer_context->contextMutex);
    auto saved = daScriptEnvironment::exchangeBound(g_mixer_env);
    g_mixer_context->restart();
    if ( !g_mixer_context.get()->runWithCatch([&](){
        das_invoke_function<void>::invoke<Array&,int32_t,int32_t>(g_mixer_context.get(),nullptr,g_mixer_function,buffer,g_channels,g_rate,fdt);
    }) ) {
        g_mixer_context->to_err(&g_mixer_context->exceptionAt, g_mixer_context->getException());
        g_mixer_context->clearException();
    }
    daScriptEnvironment::setBound(saved);
}

Context & dasAudio_mixerContext ( Context * context, LineInfoArg * at ) {
    if ( !g_mixer_context ) context->throw_error_at(at,"sound mixer is not initialized");
    return *g_mixer_context;
}

bool dasAudio_init ( TFunc<void,TTemporary<TArray<float>>,int32_t,int32_t,float> mixer, int32_t rate, int32_t channels, Context & context ) {
    g_mixer_initialized = false;
    g_rate = rate;
    g_channels = channels;
    // log
    ma_log_init(nullptr, &g_ma_log_struct);
    ma_log_register_callback(&g_ma_log_struct, {on_error_log, nullptr});
    // device
    ma_device_config deviceConfig;
    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format   = ma_format_f32;
    deviceConfig.playback.channels = g_channels;
    deviceConfig.sampleRate        = g_rate;
    deviceConfig.dataCallback      = data_callback;
    deviceConfig.pUserData         = NULL;
    if ( ma_device_init(nullptr, &deviceConfig, &g_device) != MA_SUCCESS ) {
        LOG(LogLevel::error) << "failed to open playback device.\n";
        return false;
    }
    g_mixer_context.reset(get_clone_context(&context,uint32_t(ContextCategory::audio_context)));
    g_mixer_function = mixer;
    g_mixer_env = daScriptEnvironment::getBound();
    if ( ma_device_start(&g_device) != MA_SUCCESS ) {
        ma_device_uninit(&g_device);
        g_mixer_context.reset();
        return false;
    }
    g_mixer_initialized = true;
    return true;
}

void dasAudio_finalize ( void ) {
    if ( g_mixer_initialized ) {
        ma_device_uninit(&g_device);
        g_mixer_context.reset();
        g_mixer_initialized = false;
    }
}

MA_API ma_result dasAudio_ma_resampler_init(const ma_resampler_config* pConfig, ma_resampler* pResampler) {
    return ma_resampler_init(pConfig, nullptr, pResampler);
}

MA_API void dasAudio_ma_resampler_uninit(ma_resampler* pResampler) {
    return ma_resampler_uninit(pResampler,nullptr);
}

MA_API ma_result dasAudio_ma_channel_converter_init(const ma_channel_converter_config* pConfig, ma_channel_converter* pConverter) {
    return ma_channel_converter_init(pConfig, nullptr, pConverter);
}

MA_API void dasAudio_ma_channel_converter_uninit(ma_channel_converter* pConverter) {
    return ma_channel_converter_uninit(pConverter, nullptr);
}

MA_API ma_uint64 dasAudio_ma_resampler_get_required_input_frame_count(const ma_resampler* pResampler, ma_uint64 outputFrameCount) {
    ma_uint64 inputFrameCount = 0ul;
    ma_resampler_get_required_input_frame_count(pResampler, outputFrameCount, &inputFrameCount);
    return inputFrameCount;
}

MA_API ma_uint64 dasAudio_ma_resampler_get_expected_output_frame_count(const ma_resampler* pResampler, ma_uint64 inputFrameCount) {
    ma_uint64 outputFrameCount = 0ul;
    ma_resampler_get_expected_output_frame_count(pResampler, inputFrameCount, &outputFrameCount);
    return outputFrameCount;
}

MA_API ma_result dasAudio_ma_decoder_init_memory(const void* pData, ma_uint64 dataSize, const ma_decoder_config* pConfig, ma_decoder* pDecoder) {
    return ma_decoder_init_memory(pData, (size_t)dataSize, pConfig, pDecoder);
}

MA_API ma_uint64 dasAudio_ma_decoder_get_length_in_pcm_frames(ma_decoder* pDecoder) {
    ma_uint64 frameCount = 0;
    ma_decoder_get_length_in_pcm_frames(pDecoder, &frameCount);
    return frameCount;
}

MA_API ma_uint64 dasAudio_ma_decoder_read_pcm_frames(ma_decoder* pDecoder, void* pFramesOut, ma_uint64 frameCount) {
    ma_uint64 framesRead = 0;
    ma_decoder_read_pcm_frames(pDecoder, pFramesOut, frameCount, &framesRead);
    return framesRead;
}

struct MAResamplerConfigAnnotation : ManagedStructureAnnotation<ma_resampler_config> {
    MAResamplerConfigAnnotation ( ModuleLibrary & mlib )
        : ManagedStructureAnnotation("ma_resampler_config", mlib, "ma_resampler_config") {
        addField<DAS_BIND_MANAGED_FIELD(format)>("format","format");
        addField<DAS_BIND_MANAGED_FIELD(channels)>("channels","channels");
        addField<DAS_BIND_MANAGED_FIELD(sampleRateIn)>("sampleRateIn","sampleRateIn");
        addField<DAS_BIND_MANAGED_FIELD(sampleRateOut)>("sampleRateOut","sampleRateOut");
        /*
        addField<DAS_BIND_MANAGED_FIELD(lpfOrder)>("lpfOrder","lpfOrder");
        addField<DAS_BIND_MANAGED_FIELD(lpfNyquistFactor)>("lpfNyquistFactor","lpfNyquistFactor");
        addField<DAS_BIND_MANAGED_FIELD(quality)>("quality","quality");
        */
    }
};

struct MAResamplerAnnotation : ManagedStructureAnnotation<ma_resampler> {
    MAResamplerAnnotation ( ModuleLibrary & mlib )
        : ManagedStructureAnnotation("ma_resampler", mlib, "ma_resampler") {
        addField<DAS_BIND_MANAGED_FIELD(format)>("format","format");
        addField<DAS_BIND_MANAGED_FIELD(channels)>("channels","channels");
        addField<DAS_BIND_MANAGED_FIELD(sampleRateIn)>("sampleRateIn","sampleRateIn");
        addField<DAS_BIND_MANAGED_FIELD(sampleRateOut)>("sampleRateOut","sampleRateOut");
        // addField<DAS_BIND_MANAGED_FIELD(state)>("state","state");
    }
};

struct MAChannelConvertorConfigAnnotation : ManagedStructureAnnotation<ma_channel_converter_config> {
    MAChannelConvertorConfigAnnotation ( ModuleLibrary & mlib )
        : ManagedStructureAnnotation("ma_channel_converter_config", mlib, "ma_channel_converter_config") {
        addField<DAS_BIND_MANAGED_FIELD(format)>("format","format");
        addField<DAS_BIND_MANAGED_FIELD(channelsIn)>("channelsIn","channelsIn");
        addField<DAS_BIND_MANAGED_FIELD(channelsOut)>("channelsOut","channelsOut");
        addField<DAS_BIND_MANAGED_FIELD(pChannelMapIn)>("channelMapIn","channelMapIn");
        addField<DAS_BIND_MANAGED_FIELD(pChannelMapOut)>("channelMapOut","channelMapOut");
        addField<DAS_BIND_MANAGED_FIELD(mixingMode)>("mixingMode","mixingMode");
        addField<DAS_BIND_MANAGED_FIELD(ppWeights)>("weights","weights");
    }
};

struct MAChannelConverterAnnotation : ManagedStructureAnnotation<ma_channel_converter> {
    MAChannelConverterAnnotation ( ModuleLibrary & mlib )
        : ManagedStructureAnnotation("ma_channel_converter", mlib, "ma_channel_converter") {
    }
};

struct MAVolumeMixerAnnotation : ManagedStructureAnnotation<ma_volume_mixer> {
    MAVolumeMixerAnnotation ( ModuleLibrary & mlib )
        : ManagedStructureAnnotation("ma_volume_mixer", mlib, "ma_volume_mixer") {
        addField<DAS_BIND_MANAGED_FIELD(volume)>("volume","volume");
        addField<DAS_BIND_MANAGED_FIELD(dvolume)>("dvolume","dvolume");
        addField<DAS_BIND_MANAGED_FIELD(tvolume)>("tvolume","tvolume");
        addField<DAS_BIND_MANAGED_FIELD(pan)>("pan","pan");
        addField<DAS_BIND_MANAGED_FIELD(nChannels)>("channels","nChannels");
    }
};

struct MADecoderConfigAnnotation : ManagedStructureAnnotation<ma_decoder_config> {
    MADecoderConfigAnnotation ( ModuleLibrary & mlib )
        : ManagedStructureAnnotation("ma_decoder_config", mlib, "ma_decoder_config") {
        addField<DAS_BIND_MANAGED_FIELD(format)>("format","format");
        addField<DAS_BIND_MANAGED_FIELD(channels)>("channels","channels");
        addField<DAS_BIND_MANAGED_FIELD(pChannelMap)>("channelMap","channelMap");
        addField<DAS_BIND_MANAGED_FIELD(channelMixMode)>("channelMixMode","channelMixMode");
        addField<DAS_BIND_MANAGED_FIELD(ditherMode)>("ditherMode","ditherMode");
    }
};

struct MADecoderAnnotation : ManagedStructureAnnotation<ma_decoder> {
    MADecoderAnnotation ( ModuleLibrary & mlib )
        : ManagedStructureAnnotation("ma_decoder", mlib, "ma_decoder") {
        addField<DAS_BIND_MANAGED_FIELD(outputFormat)>("outputFormat","outputFormat");
        addField<DAS_BIND_MANAGED_FIELD(outputChannels)>("outputChannels","outputChannels");
        addField<DAS_BIND_MANAGED_FIELD(outputSampleRate)>("outputSampleRate","outputSampleRate");
    }
};

struct MALimiterAnnotation : ManagedStructureAnnotation<ma_limiter> {
    MALimiterAnnotation ( ModuleLibrary & mlib )
        : ManagedStructureAnnotation("ma_limiter", mlib, "ma_limiter") {
        addField<DAS_BIND_MANAGED_FIELD(gain)>("gain","gain");
        addField<DAS_BIND_MANAGED_FIELD(nChannels)>("nChannels","nChannels");
        addField<DAS_BIND_MANAGED_FIELD(attack_samples)>("attack_samples","attack_samples");
        addField<DAS_BIND_MANAGED_FIELD(threshold)>("threshold","threshold");
        addField<DAS_BIND_MANAGED_FIELD(attack_coeff)>("attack_coeff","attack_coeff");
        addField<DAS_BIND_MANAGED_FIELD(release_coeff)>("release_coeff","release_coeff");
        addField<DAS_BIND_MANAGED_FIELD(linear_limiter)>("linear_limiter","linear_limiter");
    }
};

// SF2 voice
struct MASF2EnvelopeAnnotation : ManagedStructureAnnotation<ma_sf2_envelope> {
    MASF2EnvelopeAnnotation ( ModuleLibrary & mlib )
        : ManagedStructureAnnotation("ma_sf2_envelope", mlib, "ma_sf2_envelope") {
        addField<DAS_BIND_MANAGED_FIELD(stage)>("stage","stage");
        addField<DAS_BIND_MANAGED_FIELD(level)>("level","level");
        addField<DAS_BIND_MANAGED_FIELD(release_level)>("release_level","release_level");
        addField<DAS_BIND_MANAGED_FIELD(slope)>("slope","slope");
        addField<DAS_BIND_MANAGED_FIELD(is_exponential)>("is_exponential","is_exponential");
        addField<DAS_BIND_MANAGED_FIELD(is_amp_env)>("is_amp_env","is_amp_env");
        addField<DAS_BIND_MANAGED_FIELD(midi_velocity)>("midi_velocity","midi_velocity");
        addField<DAS_BIND_MANAGED_FIELD(samples_until_next)>("samples_until_next","samples_until_next");
        addField<DAS_BIND_MANAGED_FIELD(delay_sec)>("delay_sec","delay_sec");
        addField<DAS_BIND_MANAGED_FIELD(attack_sec)>("attack_sec","attack_sec");
        addField<DAS_BIND_MANAGED_FIELD(hold_sec)>("hold_sec","hold_sec");
        addField<DAS_BIND_MANAGED_FIELD(decay_sec)>("decay_sec","decay_sec");
        addField<DAS_BIND_MANAGED_FIELD(sustain_level)>("sustain_level","sustain_level");
        addField<DAS_BIND_MANAGED_FIELD(release_sec)>("release_sec","release_sec");
    }
};

struct MASF2BiquadAnnotation : ManagedStructureAnnotation<ma_sf2_biquad> {
    MASF2BiquadAnnotation ( ModuleLibrary & mlib )
        : ManagedStructureAnnotation("ma_sf2_biquad", mlib, "ma_sf2_biquad") {
        addField<DAS_BIND_MANAGED_FIELD(q_inv)>("q_inv","q_inv");
        addField<DAS_BIND_MANAGED_FIELD(a0)>("a0","a0");
        addField<DAS_BIND_MANAGED_FIELD(a1)>("a1","a1");
        addField<DAS_BIND_MANAGED_FIELD(b1)>("b1","b1");
        addField<DAS_BIND_MANAGED_FIELD(b2)>("b2","b2");
        addField<DAS_BIND_MANAGED_FIELD(z1)>("z1","z1");
        addField<DAS_BIND_MANAGED_FIELD(z2)>("z2","z2");
        addField<DAS_BIND_MANAGED_FIELD(active)>("active","active");
    }
};

struct MASF2VoiceAnnotation : ManagedStructureAnnotation<ma_sf2_voice> {
    MASF2VoiceAnnotation ( ModuleLibrary & mlib )
        : ManagedStructureAnnotation("ma_sf2_voice", mlib, "ma_sf2_voice") {
        addField<DAS_BIND_MANAGED_FIELD(sample_start)>("sample_start","sample_start");
        addField<DAS_BIND_MANAGED_FIELD(sample_end)>("sample_end","sample_end");
        addField<DAS_BIND_MANAGED_FIELD(loop_start)>("loop_start","loop_start");
        addField<DAS_BIND_MANAGED_FIELD(loop_end)>("loop_end","loop_end");
        addField<DAS_BIND_MANAGED_FIELD(loop_mode)>("loop_mode","loop_mode");
        addField<DAS_BIND_MANAGED_FIELD(sample_start_r)>("sample_start_r","sample_start_r");
        addField<DAS_BIND_MANAGED_FIELD(sample_end_r)>("sample_end_r","sample_end_r");
        addField<DAS_BIND_MANAGED_FIELD(loop_start_r)>("loop_start_r","loop_start_r");
        addField<DAS_BIND_MANAGED_FIELD(loop_end_r)>("loop_end_r","loop_end_r");
        addField<DAS_BIND_MANAGED_FIELD(stereo)>("stereo","stereo");
        addField<DAS_BIND_MANAGED_FIELD(position)>("position","position");
        addField<DAS_BIND_MANAGED_FIELD(phase_inc)>("phase_inc","phase_inc");
        addField<DAS_BIND_MANAGED_FIELD(vol_env)>("vol_env","vol_env");
        addField<DAS_BIND_MANAGED_FIELD(mod_env)>("mod_env","mod_env");
        addField<DAS_BIND_MANAGED_FIELD(filter)>("filter","filter");
        addField<DAS_BIND_MANAGED_FIELD(filter_r)>("filter_r","filter_r");
        addField<DAS_BIND_MANAGED_FIELD(initial_filter_fc)>("initial_filter_fc","initial_filter_fc");
        addField<DAS_BIND_MANAGED_FIELD(initial_filter_q)>("initial_filter_q","initial_filter_q");
        addField<DAS_BIND_MANAGED_FIELD(mod_env_to_pitch)>("mod_env_to_pitch","mod_env_to_pitch");
        addField<DAS_BIND_MANAGED_FIELD(mod_lfo_to_pitch)>("mod_lfo_to_pitch","mod_lfo_to_pitch");
        addField<DAS_BIND_MANAGED_FIELD(vib_lfo_to_pitch)>("vib_lfo_to_pitch","vib_lfo_to_pitch");
        addField<DAS_BIND_MANAGED_FIELD(mod_env_to_filter_fc)>("mod_env_to_filter_fc","mod_env_to_filter_fc");
        addField<DAS_BIND_MANAGED_FIELD(mod_lfo_to_filter_fc)>("mod_lfo_to_filter_fc","mod_lfo_to_filter_fc");
        addField<DAS_BIND_MANAGED_FIELD(mod_lfo_to_volume)>("mod_lfo_to_volume","mod_lfo_to_volume");
        addField<DAS_BIND_MANAGED_FIELD(mod_lfo_phase)>("mod_lfo_phase","mod_lfo_phase");
        addField<DAS_BIND_MANAGED_FIELD(mod_lfo_freq)>("mod_lfo_freq","mod_lfo_freq");
        addField<DAS_BIND_MANAGED_FIELD(mod_lfo_delay)>("mod_lfo_delay","mod_lfo_delay");
        addField<DAS_BIND_MANAGED_FIELD(mod_lfo_elapsed)>("mod_lfo_elapsed","mod_lfo_elapsed");
        addField<DAS_BIND_MANAGED_FIELD(vib_lfo_phase)>("vib_lfo_phase","vib_lfo_phase");
        addField<DAS_BIND_MANAGED_FIELD(vib_lfo_freq)>("vib_lfo_freq","vib_lfo_freq");
        addField<DAS_BIND_MANAGED_FIELD(vib_lfo_delay)>("vib_lfo_delay","vib_lfo_delay");
        addField<DAS_BIND_MANAGED_FIELD(vib_lfo_elapsed)>("vib_lfo_elapsed","vib_lfo_elapsed");
        addField<DAS_BIND_MANAGED_FIELD(pitch_bend_cents)>("pitch_bend_cents","pitch_bend_cents");
        addField<DAS_BIND_MANAGED_FIELD(attenuation)>("attenuation","attenuation");
        addField<DAS_BIND_MANAGED_FIELD(pan)>("pan","pan");
        addField<DAS_BIND_MANAGED_FIELD(released)>("released","released");
        addField<DAS_BIND_MANAGED_FIELD(finished)>("finished","finished");
        addField<DAS_BIND_MANAGED_FIELD(sample_rate)>("sample_rate","sample_rate");
    }
};

struct I3DL2ReverbPropertiesAnnotation : ManagedStructureAnnotation<I3DL2ReverbProperties> {
    I3DL2ReverbPropertiesAnnotation ( ModuleLibrary & mlib )
        : ManagedStructureAnnotation("I3DL2ReverbProperties", mlib, "I3DL2ReverbProperties") {
        addField<DAS_BIND_MANAGED_FIELD(lRoom)>("lRoom","lRoom");
        addField<DAS_BIND_MANAGED_FIELD(lRoomHF)>("lRoomHF","lRoomHF");
        addField<DAS_BIND_MANAGED_FIELD(flDecayTime)>("flDecayTime","flDecayTime");
        addField<DAS_BIND_MANAGED_FIELD(flDecayHFRatio)>("flDecayHFRatio","flDecayHFRatio");
        addField<DAS_BIND_MANAGED_FIELD(lReflections)>("lReflections","lReflections");
        addField<DAS_BIND_MANAGED_FIELD(flReflectionsDelay)>("flReflectionsDelay","flReflectionsDelay");
        addField<DAS_BIND_MANAGED_FIELD(lReverb)>("lReverb","lReverb");
        addField<DAS_BIND_MANAGED_FIELD(flReverbDelay)>("flReverbDelay","flReverbDelay");
        addField<DAS_BIND_MANAGED_FIELD(flDiffusion)>("flDiffusion","flDiffusion");
        addField<DAS_BIND_MANAGED_FIELD(flDensity)>("flDensity","flDensity");
    }
};

struct I3DL2ReverbAnnotation : ManagedStructureAnnotation<I3DL2Reverb,true,true> {
    I3DL2ReverbAnnotation ( ModuleLibrary & mlib )
        : ManagedStructureAnnotation("I3DL2Reverb", mlib, "I3DL2Reverb") {
    }
};

// ─── Chorus ───

struct MaChorusConfigAnnotation : ManagedStructureAnnotation<ma_chorus_config> {
    MaChorusConfigAnnotation ( ModuleLibrary & mlib )
        : ManagedStructureAnnotation("ma_chorus_config", mlib, "ma_chorus_config") {
        addField<DAS_BIND_MANAGED_FIELD(rate)>("rate","rate");
        addField<DAS_BIND_MANAGED_FIELD(depth)>("depth","depth");
        addField<DAS_BIND_MANAGED_FIELD(feedback)>("feedback","feedback");
        addField<DAS_BIND_MANAGED_FIELD(delay_ms)>("delay_ms","delay_ms");
        addField<DAS_BIND_MANAGED_FIELD(wet)>("wet","wet");
    }
};

struct MaChorusAnnotation : ManagedStructureAnnotation<ma_chorus,true,true> {
    MaChorusAnnotation ( ModuleLibrary & mlib )
        : ManagedStructureAnnotation("ma_chorus", mlib, "ma_chorus") {
    }
};

void dasAudio_chorusInit ( ma_chorus * chorus, float sample_rate, Context * context, LineInfoArg * at ) {
    if ( !chorus ) context->throw_error_at(at,"chorus is null");
    ma_chorus_init(chorus, sample_rate);
}

void dasAudio_chorusProcess ( ma_chorus * chorus, float * input, float * output, int nSamples, Context * context, LineInfoArg * at ) {
    if ( !chorus ) context->throw_error_at(at,"chorus is null");
    ma_chorus_process(chorus, input, output, nSamples);
}

void dasAudio_chorusSetConfig ( ma_chorus * chorus, const ma_chorus_config & config, Context * context, LineInfoArg * at ) {
    if ( !chorus ) context->throw_error_at(at,"chorus is null");
    ma_chorus_set_config(chorus, &config);
}

ma_chorus_config dasAudio_chorusConfigDefault ( ) {
    return ma_chorus_config_default();
}

// ─── Effects (bitcrush, waveshaper, djfilter, bandpass) ───

struct MaBitcrushAnnotation : ManagedStructureAnnotation<ma_bitcrush,true,true> {
    MaBitcrushAnnotation ( ModuleLibrary & mlib )
        : ManagedStructureAnnotation("ma_bitcrush", mlib, "ma_bitcrush") {
        addField<DAS_BIND_MANAGED_FIELD(bits)>("bits","bits");
        addField<DAS_BIND_MANAGED_FIELD(coarse)>("coarse","coarse");
    }
};

struct MaWaveshaperAnnotation : ManagedStructureAnnotation<ma_waveshaper,true,true> {
    MaWaveshaperAnnotation ( ModuleLibrary & mlib )
        : ManagedStructureAnnotation("ma_waveshaper", mlib, "ma_waveshaper") {
        addField<DAS_BIND_MANAGED_FIELD(drive)>("drive","drive");
    }
};

struct MaDjfilterAnnotation : ManagedStructureAnnotation<ma_djfilter,true,true> {
    MaDjfilterAnnotation ( ModuleLibrary & mlib )
        : ManagedStructureAnnotation("ma_djfilter", mlib, "ma_djfilter") {
        addField<DAS_BIND_MANAGED_FIELD(position)>("position","position");
        addField<DAS_BIND_MANAGED_FIELD(sample_rate)>("sample_rate","sample_rate");
    }
};

struct MaBandpassAnnotation : ManagedStructureAnnotation<ma_bandpass,true,true> {
    MaBandpassAnnotation ( ModuleLibrary & mlib )
        : ManagedStructureAnnotation("ma_bandpass", mlib, "ma_bandpass") {
        addField<DAS_BIND_MANAGED_FIELD(freq)>("freq","freq");
        addField<DAS_BIND_MANAGED_FIELD(q)>("q","q");
        addField<DAS_BIND_MANAGED_FIELD(sample_rate)>("sample_rate","sample_rate");
    }
};

void dasAudio_bitcrushInit ( ma_bitcrush * bc, Context * context, LineInfoArg * at ) {
    if ( !bc ) context->throw_error_at(at,"bitcrush is null");
    ma_bitcrush_init(bc);
}
void dasAudio_bitcrushProcess ( ma_bitcrush * bc, float * buf, int nFrames, Context * context, LineInfoArg * at ) {
    if ( !bc ) context->throw_error_at(at,"bitcrush is null");
    ma_bitcrush_process(bc, buf, nFrames);
}

void dasAudio_waveshaperInit ( ma_waveshaper * ws, Context * context, LineInfoArg * at ) {
    if ( !ws ) context->throw_error_at(at,"waveshaper is null");
    ma_waveshaper_init(ws);
}
void dasAudio_waveshaperProcess ( ma_waveshaper * ws, float * buf, int nFrames, Context * context, LineInfoArg * at ) {
    if ( !ws ) context->throw_error_at(at,"waveshaper is null");
    ma_waveshaper_process(ws, buf, nFrames);
}

void dasAudio_djfilterInit ( ma_djfilter * dj, float sample_rate, Context * context, LineInfoArg * at ) {
    if ( !dj ) context->throw_error_at(at,"djfilter is null");
    ma_djfilter_init(dj, sample_rate);
}
void dasAudio_djfilterProcess ( ma_djfilter * dj, float * buf, int nFrames, Context * context, LineInfoArg * at ) {
    if ( !dj ) context->throw_error_at(at,"djfilter is null");
    ma_djfilter_process(dj, buf, nFrames);
}

void dasAudio_bandpassInit ( ma_bandpass * bp, float sample_rate, Context * context, LineInfoArg * at ) {
    if ( !bp ) context->throw_error_at(at,"bandpass is null");
    ma_bandpass_init(bp, sample_rate);
}
void dasAudio_bandpassSetup ( ma_bandpass * bp, float freq, float q, Context * context, LineInfoArg * at ) {
    if ( !bp ) context->throw_error_at(at,"bandpass is null");
    ma_bandpass_setup(bp, freq, q);
}
void dasAudio_bandpassProcess ( ma_bandpass * bp, float * buf, int nFrames, Context * context, LineInfoArg * at ) {
    if ( !bp ) context->throw_error_at(at,"bandpass is null");
    ma_bandpass_process(bp, buf, nFrames);
}

// ─── Phaser / Tremolo / Compressor ───

struct MaPhaserAnnotation : ManagedStructureAnnotation<ma_phaser,true,true> {
    MaPhaserAnnotation ( ModuleLibrary & mlib )
        : ManagedStructureAnnotation("ma_phaser", mlib, "ma_phaser") {
        addField<DAS_BIND_MANAGED_FIELD(rate)>("rate","rate");
        addField<DAS_BIND_MANAGED_FIELD(depth)>("depth","depth");
        addField<DAS_BIND_MANAGED_FIELD(center)>("center","center");
        addField<DAS_BIND_MANAGED_FIELD(sweep)>("sweep","sweep");
        addField<DAS_BIND_MANAGED_FIELD(sample_rate)>("sample_rate","sample_rate");
    }
};

struct MaTremoloAnnotation : ManagedStructureAnnotation<ma_tremolo,true,true> {
    MaTremoloAnnotation ( ModuleLibrary & mlib )
        : ManagedStructureAnnotation("ma_tremolo", mlib, "ma_tremolo") {
        addField<DAS_BIND_MANAGED_FIELD(rate)>("rate","rate");
        addField<DAS_BIND_MANAGED_FIELD(depth)>("depth","depth");
        addField<DAS_BIND_MANAGED_FIELD(sample_rate)>("sample_rate","sample_rate");
    }
};

struct MaCompressorAnnotation : ManagedStructureAnnotation<ma_compressor,true,true> {
    MaCompressorAnnotation ( ModuleLibrary & mlib )
        : ManagedStructureAnnotation("ma_compressor", mlib, "ma_compressor") {
        addField<DAS_BIND_MANAGED_FIELD(threshold_db)>("threshold_db","threshold_db");
        addField<DAS_BIND_MANAGED_FIELD(ratio)>("ratio","ratio");
        addField<DAS_BIND_MANAGED_FIELD(knee_db)>("knee_db","knee_db");
        addField<DAS_BIND_MANAGED_FIELD(attack)>("attack","attack");
        addField<DAS_BIND_MANAGED_FIELD(release)>("release","release");
        addField<DAS_BIND_MANAGED_FIELD(sample_rate)>("sample_rate","sample_rate");
    }
};

void dasAudio_phaserInit ( ma_phaser * ph, float sample_rate, Context * context, LineInfoArg * at ) {
    if ( !ph ) context->throw_error_at(at,"phaser is null");
    ma_phaser_init(ph, sample_rate);
}
void dasAudio_phaserProcess ( ma_phaser * ph, float * buf, int nFrames, Context * context, LineInfoArg * at ) {
    if ( !ph ) context->throw_error_at(at,"phaser is null");
    ma_phaser_process(ph, buf, nFrames);
}

void dasAudio_tremoloInit ( ma_tremolo * tr, float sample_rate, Context * context, LineInfoArg * at ) {
    if ( !tr ) context->throw_error_at(at,"tremolo is null");
    ma_tremolo_init(tr, sample_rate);
}
void dasAudio_tremoloProcess ( ma_tremolo * tr, float * buf, int nFrames, Context * context, LineInfoArg * at ) {
    if ( !tr ) context->throw_error_at(at,"tremolo is null");
    ma_tremolo_process(tr, buf, nFrames);
}

void dasAudio_compressorInit ( ma_compressor * c, float sample_rate, Context * context, LineInfoArg * at ) {
    if ( !c ) context->throw_error_at(at,"compressor is null");
    ma_compressor_init(c, sample_rate);
}
void dasAudio_compressorProcess ( ma_compressor * c, float * buf, int nFrames, Context * context, LineInfoArg * at ) {
    if ( !c ) context->throw_error_at(at,"compressor is null");
    ma_compressor_process(c, buf, nFrames);
}

struct MAHrtfAnnotation : ManagedStructureAnnotation<ma_hrtf> {
    MAHrtfAnnotation ( ModuleLibrary & mlib )
        : ManagedStructureAnnotation("ma_hrtf", mlib, "ma_hrtf") {
        addField<DAS_BIND_MANAGED_FIELD(taps)>("taps","taps");
        addField<DAS_BIND_MANAGED_FIELD(azimuth)>("azimuth","azimuth");
        addField<DAS_BIND_MANAGED_FIELD(elevation)>("elevation","elevation");
        addField<DAS_BIND_MANAGED_FIELD(sampleRate)>("sampleRate","sampleRate");
        addField<DAS_BIND_MANAGED_FIELD(leftfip)>("leftfip","leftfip");
        addField<DAS_BIND_MANAGED_FIELD(rightfip)>("rightfip","rightfip");
        addField<DAS_BIND_MANAGED_FIELD(fft_size)>("fft_size","fft_size");
    }
};

void dasAudio_setSampleRate ( I3DL2Reverb * reverb, float rate, Context * context, LineInfoArg * at ) {
    if ( !reverb ) context->throw_error_at(at,"reverb is null");
    reverb->SetSampleRate(rate);
}

void dasAudio_setProperties ( I3DL2Reverb * reverb, const I3DL2ReverbProperties & props, Context * context, LineInfoArg * at ) {
    if ( !reverb ) context->throw_error_at(at,"reverb is null");
    reverb->SetReverbProperties(props);
}

void dasAudio_process ( I3DL2Reverb * reverb, float * buffer, float * outBuffer, int nSamples, Context * context, LineInfoArg * at ) {
    if ( !reverb ) context->throw_error_at(at,"reverb is null");
    reverb->Process(buffer,outBuffer,nSamples);
}

void dasAudio_processMono ( I3DL2Reverb * reverb, float * buffer, float * outBuffer, int nSamples, Context * context, LineInfoArg * at ) {
    if ( !reverb ) context->throw_error_at(at,"reverb is null");
    vector<float> inbuf(nSamples*2);
    for ( int i=0; i!=nSamples; ++i ) {
        inbuf[i*2] = inbuf[i*2+1] = buffer[i];
    }
    vector<float> outbuf(nSamples*2);
    reverb->Process(&inbuf[0],&outbuf[0],nSamples);
    for ( int i=0; i!=nSamples; ++i ) {
        outBuffer[i] = outbuf[i*2]; // left channel only and hope its the same as right
    }
}

I3DL2ReverbProperties & dasAudio_getReverbPreset ( I3DL2Preset preset, Context * context, LineInfoArg * at ) {
    auto pi = int(preset);
    if ( pi<0 || pi>int(I3DL2Preset::Underwater) ) context->throw_error_at(at,"invalid reverb preset");
    return ReverbPresets[preset];
}

// ─── Convolution Reverb ───

struct ConvolutionReverbAnnotation : ManagedStructureAnnotation<ConvolutionReverb,true,true> {
    ConvolutionReverbAnnotation ( ModuleLibrary & mlib )
        : ManagedStructureAnnotation("ConvolutionReverb", mlib, "ConvolutionReverb") {
        addField<DAS_BIND_MANAGED_FIELD(decayTime)>("decayTime","decayTime");
        addField<DAS_BIND_MANAGED_FIELD(lpFreqStart)>("lpFreqStart","lpFreqStart");
        addField<DAS_BIND_MANAGED_FIELD(lpFreqEnd)>("lpFreqEnd","lpFreqEnd");
        addField<DAS_BIND_MANAGED_FIELD(fadeIn)>("fadeIn","fadeIn");
        addField<DAS_BIND_MANAGED_FIELD(sampleRate)>("sampleRate","sampleRate");
        addField<DAS_BIND_MANAGED_FIELD(fft_size)>("fft_size","fft_size");
        addField<DAS_BIND_MANAGED_FIELD(ir_length)>("ir_length","ir_length");
        addField<DAS_BIND_MANAGED_FIELD(num_partitions)>("num_partitions","num_partitions");
    }
};

void dasAudio_convReverbInit ( ConvolutionReverb * rev, int sample_rate, float decay_time,
                                float lp_freq_start, float lp_freq_end, float fade_in,
                                Context * context, LineInfoArg * at ) {
    if ( !rev ) context->throw_error_at(at,"convolution reverb is null");
    conv_reverb_init(rev, (uint32_t)sample_rate, decay_time, lp_freq_start, lp_freq_end, fade_in);
}

void dasAudio_convReverbProcess ( ConvolutionReverb * rev, float * input, float * output, int nFrames,
                                   Context * context, LineInfoArg * at ) {
    if ( !rev ) context->throw_error_at(at,"convolution reverb is null");
    conv_reverb_process(rev, input, output, (uint32_t)nFrames);
}

void dasAudio_convReverbProcessMono ( ConvolutionReverb * rev, float * input, float * output, int nSamples,
                                       Context * context, LineInfoArg * at ) {
    if ( !rev ) context->throw_error_at(at,"convolution reverb is null");
    conv_reverb_process_mono(rev, input, output, (uint32_t)nSamples);
}

void dasAudio_convReverbUninit ( ConvolutionReverb * rev, Context * context, LineInfoArg * at ) {
    if ( !rev ) context->throw_error_at(at,"convolution reverb is null");
    conv_reverb_uninit(rev);
}

// ─── Delay (miniaudio ma_delay) ───

struct MaDelayAnnotation : ManagedStructureAnnotation<ma_delay,true,true> {
    MaDelayAnnotation ( ModuleLibrary & mlib )
        : ManagedStructureAnnotation("ma_delay", mlib, "ma_delay") {
    }
};

// Init a stereo delay with given delay time and feedback (decay).
// Allocates the internal buffer on the heap.
void dasAudio_delayInit ( ma_delay * d, int sample_rate, float delay_time_sec, float feedback, Context * context, LineInfoArg * at ) {
    if ( !d ) context->throw_error_at(at,"delay is null");
    ma_uint32 delayInFrames = (ma_uint32)(delay_time_sec * sample_rate);
    if ( delayInFrames < 1 ) delayInFrames = 1;
    auto config = ma_delay_config_init(2, sample_rate, delayInFrames, feedback);
    config.wet = 1.0f;  // output the delayed signal
    config.dry = 1.0f;  // feed input into the delay buffer
    config.delayStart = MA_TRUE;  // delay the start (echo mode)
    auto result = ma_delay_init(&config, nullptr, d);
    if ( result != MA_SUCCESS ) context->throw_error_at(at,"ma_delay_init failed");
}

void dasAudio_delayUninit ( ma_delay * d, Context * context, LineInfoArg * at ) {
    if ( !d ) context->throw_error_at(at,"delay is null");
    ma_delay_uninit(d, nullptr);
}

// Re-init with new delay time (requires free + realloc of internal buffer).
void dasAudio_delaySetParams ( ma_delay * d, int sample_rate, float delay_time_sec, float feedback, Context * context, LineInfoArg * at ) {
    if ( !d ) context->throw_error_at(at,"delay is null");
    ma_delay_uninit(d, nullptr);
    ma_uint32 delayInFrames = (ma_uint32)(delay_time_sec * sample_rate);
    if ( delayInFrames < 1 ) delayInFrames = 1;
    auto config = ma_delay_config_init(2, sample_rate, delayInFrames, feedback);
    config.wet = 1.0f;
    config.dry = 1.0f;
    config.delayStart = MA_TRUE;
    auto result = ma_delay_init(&config, nullptr, d);
    if ( result != MA_SUCCESS ) context->throw_error_at(at,"ma_delay_init failed (set_params)");
}

void dasAudio_delayProcess ( ma_delay * d, float * input, float * output, int nFrames, Context * context, LineInfoArg * at ) {
    if ( !d ) context->throw_error_at(at,"delay is null");
    ma_delay_process_pcm_frames(d, output, input, nFrames);
}

void dasAudio_disableLinearResamplerFiltering ( ma_resampler_config * config ) {
    config->linear.lpfOrder = 0;
}

class Module_Audio : public das::Module {
protected:
    bool initialized = false;
public:
    Module_Audio() : Module("audio") {}
    bool initDependencies() override {
        if ( initialized ) return true;
        if ( !Module::require("rtti_core") ) return false;
        initialized = true;
        // now, initialize
        ModuleLibrary lib(this);
        lib.addBuiltInModule();
        addBuiltinDependency(lib, Module::require("rtti_core"));
        // audio constants
        addConstant<int>(*this, "MA_SAMPLE_RATE", HRTF_SAMPLE_RATE);
        // reverb
        addEnumeration(new EnumerationI3DL2Preset());
        addAnnotation(new I3DL2ReverbPropertiesAnnotation(lib));
        addAnnotation(new I3DL2ReverbAnnotation(lib));
        addExtern<DAS_BIND_FUN(dasAudio_setSampleRate)>(*this, lib, "set_sample_rate",
            SideEffects::modifyArgumentAndExternal, "dasAudio_setSampleRate")->args({"reverb", "rate", "context", "at"});
        addExtern<DAS_BIND_FUN(dasAudio_setProperties)>(*this, lib, "set_properties",
            SideEffects::modifyArgumentAndExternal, "dasAudio_setProperties")->args({"reverb", "props", "context", "at"});
        addExtern<DAS_BIND_FUN(dasAudio_process)>(*this, lib, "process_stereo",
            SideEffects::modifyArgumentAndExternal, "dasAudio_process")->args({"reverb", "input", "output", "nSamples", "context", "at"});
        addExtern<DAS_BIND_FUN(dasAudio_processMono)>(*this, lib, "process_mono",
            SideEffects::modifyArgumentAndExternal, "dasAudio_processMono")->args({"reverb", "input", "output", "nSamples", "context", "at"});
        addExtern<DAS_BIND_FUN(dasAudio_getReverbPreset),SimNode_ExtFuncCallRef>(*this, lib, "get_preset",
            SideEffects::modifyArgumentAndExternal, "dasAudio_getReverbPreset")->args({"preset", "context", "at"});
        // convolution reverb
        addAnnotation(new ConvolutionReverbAnnotation(lib));
        addExtern<DAS_BIND_FUN(dasAudio_convReverbInit)>(*this, lib, "conv_reverb_init",
            SideEffects::modifyArgumentAndExternal, "dasAudio_convReverbInit")
                ->args({"reverb", "sample_rate", "decay_time", "lp_freq_start", "lp_freq_end", "fade_in", "context", "at"});
        addExtern<DAS_BIND_FUN(dasAudio_convReverbProcess)>(*this, lib, "conv_reverb_process",
            SideEffects::modifyArgumentAndExternal, "dasAudio_convReverbProcess")
                ->args({"reverb", "input", "output", "nFrames", "context", "at"});
        addExtern<DAS_BIND_FUN(dasAudio_convReverbProcessMono)>(*this, lib, "conv_reverb_process_mono",
            SideEffects::modifyArgumentAndExternal, "dasAudio_convReverbProcessMono")
                ->args({"reverb", "input", "output", "nSamples", "context", "at"});
        addExtern<DAS_BIND_FUN(dasAudio_convReverbUninit)>(*this, lib, "conv_reverb_uninit",
            SideEffects::modifyArgumentAndExternal, "dasAudio_convReverbUninit")
                ->args({"reverb", "context", "at"});
        addExtern<DAS_BIND_FUN(conv_reverb_set_max_ir)>(*this, lib, "conv_reverb_set_max_ir",
            SideEffects::modifyExternal, "conv_reverb_set_max_ir")
                ->args({"seconds"});
        addExtern<DAS_BIND_FUN(conv_reverb_get_max_ir)>(*this, lib, "conv_reverb_get_max_ir",
            SideEffects::accessGlobal, "conv_reverb_get_max_ir");
        // chorus
        addAnnotation(new MaChorusConfigAnnotation(lib));
        addAnnotation(new MaChorusAnnotation(lib));
        addExtern<DAS_BIND_FUN(dasAudio_chorusInit)>(*this, lib, "chorus_init",
            SideEffects::modifyArgumentAndExternal, "dasAudio_chorusInit")->args({"chorus", "sample_rate", "context", "at"});
        addExtern<DAS_BIND_FUN(dasAudio_chorusProcess)>(*this, lib, "chorus_process",
            SideEffects::modifyArgumentAndExternal, "dasAudio_chorusProcess")->args({"chorus", "input", "output", "nSamples", "context", "at"});
        addExtern<DAS_BIND_FUN(dasAudio_chorusSetConfig)>(*this, lib, "chorus_set_config",
            SideEffects::modifyArgumentAndExternal, "dasAudio_chorusSetConfig")->args({"chorus", "config", "context", "at"});
        addExtern<DAS_BIND_FUN(dasAudio_chorusConfigDefault),SimNode_ExtFuncCallAndCopyOrMove>(*this, lib, "chorus_config_default",
            SideEffects::none, "dasAudio_chorusConfigDefault");
        // effects (bitcrush, waveshaper, djfilter, bandpass)
        addAnnotation(new MaBitcrushAnnotation(lib));
        addExtern<DAS_BIND_FUN(dasAudio_bitcrushInit)>(*this, lib, "bitcrush_init",
            SideEffects::modifyArgumentAndExternal, "dasAudio_bitcrushInit")->args({"bc", "context", "at"});
        addExtern<DAS_BIND_FUN(dasAudio_bitcrushProcess)>(*this, lib, "bitcrush_process",
            SideEffects::modifyArgumentAndExternal, "dasAudio_bitcrushProcess")->args({"bc", "buf", "nFrames", "context", "at"});
        addAnnotation(new MaWaveshaperAnnotation(lib));
        addExtern<DAS_BIND_FUN(dasAudio_waveshaperInit)>(*this, lib, "waveshaper_init",
            SideEffects::modifyArgumentAndExternal, "dasAudio_waveshaperInit")->args({"ws", "context", "at"});
        addExtern<DAS_BIND_FUN(dasAudio_waveshaperProcess)>(*this, lib, "waveshaper_process",
            SideEffects::modifyArgumentAndExternal, "dasAudio_waveshaperProcess")->args({"ws", "buf", "nFrames", "context", "at"});
        addAnnotation(new MaDjfilterAnnotation(lib));
        addExtern<DAS_BIND_FUN(dasAudio_djfilterInit)>(*this, lib, "djfilter_init",
            SideEffects::modifyArgumentAndExternal, "dasAudio_djfilterInit")->args({"dj", "sample_rate", "context", "at"});
        addExtern<DAS_BIND_FUN(dasAudio_djfilterProcess)>(*this, lib, "djfilter_process",
            SideEffects::modifyArgumentAndExternal, "dasAudio_djfilterProcess")->args({"dj", "buf", "nFrames", "context", "at"});
        addAnnotation(new MaBandpassAnnotation(lib));
        addExtern<DAS_BIND_FUN(dasAudio_bandpassInit)>(*this, lib, "bandpass_init",
            SideEffects::modifyArgumentAndExternal, "dasAudio_bandpassInit")->args({"bp", "sample_rate", "context", "at"});
        addExtern<DAS_BIND_FUN(dasAudio_bandpassSetup)>(*this, lib, "bandpass_setup",
            SideEffects::modifyArgumentAndExternal, "dasAudio_bandpassSetup")->args({"bp", "freq", "q", "context", "at"});
        addExtern<DAS_BIND_FUN(dasAudio_bandpassProcess)>(*this, lib, "bandpass_process",
            SideEffects::modifyArgumentAndExternal, "dasAudio_bandpassProcess")->args({"bp", "buf", "nFrames", "context", "at"});
        // phaser / tremolo / compressor
        addAnnotation(new MaPhaserAnnotation(lib));
        addExtern<DAS_BIND_FUN(dasAudio_phaserInit)>(*this, lib, "phaser_init",
            SideEffects::modifyArgumentAndExternal, "dasAudio_phaserInit")->args({"ph", "sample_rate", "context", "at"});
        addExtern<DAS_BIND_FUN(dasAudio_phaserProcess)>(*this, lib, "phaser_process",
            SideEffects::modifyArgumentAndExternal, "dasAudio_phaserProcess")->args({"ph", "buf", "nFrames", "context", "at"});
        addAnnotation(new MaTremoloAnnotation(lib));
        addExtern<DAS_BIND_FUN(dasAudio_tremoloInit)>(*this, lib, "tremolo_init",
            SideEffects::modifyArgumentAndExternal, "dasAudio_tremoloInit")->args({"tr", "sample_rate", "context", "at"});
        addExtern<DAS_BIND_FUN(dasAudio_tremoloProcess)>(*this, lib, "tremolo_process",
            SideEffects::modifyArgumentAndExternal, "dasAudio_tremoloProcess")->args({"tr", "buf", "nFrames", "context", "at"});
        addAnnotation(new MaCompressorAnnotation(lib));
        addExtern<DAS_BIND_FUN(dasAudio_compressorInit)>(*this, lib, "compressor_init",
            SideEffects::modifyArgumentAndExternal, "dasAudio_compressorInit")->args({"c", "sample_rate", "context", "at"});
        addExtern<DAS_BIND_FUN(dasAudio_compressorProcess)>(*this, lib, "compressor_process",
            SideEffects::modifyArgumentAndExternal, "dasAudio_compressorProcess")->args({"c", "buf", "nFrames", "context", "at"});
        // delay
        addAnnotation(new MaDelayAnnotation(lib));
        addExtern<DAS_BIND_FUN(dasAudio_delayInit)>(*this, lib, "delay_init",
            SideEffects::modifyArgumentAndExternal, "dasAudio_delayInit")->args({"delay", "sample_rate", "delay_time_sec", "feedback", "context", "at"});
        addExtern<DAS_BIND_FUN(dasAudio_delayUninit)>(*this, lib, "delay_uninit",
            SideEffects::modifyArgumentAndExternal, "dasAudio_delayUninit")->args({"delay", "context", "at"});
        addExtern<DAS_BIND_FUN(dasAudio_delaySetParams)>(*this, lib, "delay_set_params",
            SideEffects::modifyArgumentAndExternal, "dasAudio_delaySetParams")->args({"delay", "sample_rate", "delay_time_sec", "feedback", "context", "at"});
        addExtern<DAS_BIND_FUN(dasAudio_delayProcess)>(*this, lib, "delay_process",
            SideEffects::modifyArgumentAndExternal, "dasAudio_delayProcess")->args({"delay", "input", "output", "nFrames", "context", "at"});
        // mixer
        addExtern<DAS_BIND_FUN(dasAudio_init)>(*this, lib, "sound_initalize",
            SideEffects::modifyExternal, "dasAudio_init")->args({"mixer", "rate", "channels","context"});
        addExtern<DAS_BIND_FUN(dasAudio_finalize)>(*this, lib, "sound_finalize",
            SideEffects::modifyExternal, "dasAudio_finalize");
        addExtern<DAS_BIND_FUN(dasAudio_mixerContext),SimNode_ExtFuncCallRef>(*this, lib, "mixer_context",
            SideEffects::modifyExternal, "dasAudio_mixerContext");
        // enums
        addEnumeration(new Enumerationma_format());
        addEnumeration(new Enumerationma_resample_algorithm());
        addEnumeration(new Enumerationma_channel_mix_mode());
        addEnumeration(new Enumerationma_dither_mode());
        addEnumeration(new Enumerationma_result());
        // resampler
        addAnnotation(new MAResamplerConfigAnnotation(lib));
        addAnnotation(new MAResamplerAnnotation(lib));
        addExtern<DAS_BIND_FUN(ma_resampler_config_init),SimNode_ExtFuncCallAndCopyOrMove>(*this, lib, "ma_resampler_config_init",
            SideEffects::none, "ma_resampler_config_init")->args({"format", "channels", "sampleRateIn", "sampleRateOut","algorithm"});
        addExtern<DAS_BIND_FUN(dasAudio_ma_resampler_init)>(*this, lib, "ma_resampler_init",
            SideEffects::modifyArgumentAndExternal, "dasAudio_ma_resampler_init")->args({"config", "resampler"});
        addExtern<DAS_BIND_FUN(dasAudio_ma_resampler_uninit)>(*this, lib, "ma_resampler_uninit",
            SideEffects::modifyArgumentAndExternal, "dasAudio_ma_resampler_uninit")->args({"resampler"});
        addExtern<DAS_BIND_FUN(ma_resampler_process_pcm_frames)>(*this, lib, "ma_resampler_process_pcm_frames",
            SideEffects::modifyArgument, "ma_resampler_process_pcm_frames")->args({"resampler", "pFramesIn", "pFrameCountIn", "pFramesOut", "pFrameCountOut"});
        addExtern<DAS_BIND_FUN(ma_resampler_set_rate)>(*this, lib, "ma_resampler_set_rate",
            SideEffects::modifyArgument, "ma_resampler_set_rate")->args({"resampler", "sampleRateIn", "sampleRateOut"});
        addExtern<DAS_BIND_FUN(ma_resampler_set_rate_ratio)>(*this, lib, "ma_resampler_set_rate_ratio",
            SideEffects::modifyArgument, "ma_resampler_set_rate_ratio")->args({"resampler", "ratioInOut"});
        addExtern<DAS_BIND_FUN(dasAudio_ma_resampler_get_required_input_frame_count)>(*this, lib, "ma_resampler_get_required_input_frame_count",
            SideEffects::none, "dasAudio_ma_resampler_get_required_input_frame_count")->args({"resampler", "outputFrameCount"});
        addExtern<DAS_BIND_FUN(dasAudio_ma_resampler_get_expected_output_frame_count)>(*this, lib, "ma_resampler_get_expected_output_frame_count",
            SideEffects::none, "dasAudio_ma_resampler_get_expected_output_frame_count")->args({"resampler", "inputFrameCount"});
        addExtern<DAS_BIND_FUN(ma_resampler_get_input_latency)>(*this, lib, "ma_resampler_get_input_latency",
            SideEffects::none, "ma_resampler_get_input_latency")->args({"resampler"});
        addExtern<DAS_BIND_FUN(ma_resampler_get_output_latency)>(*this, lib, "ma_resampler_get_output_latency",
            SideEffects::none, "ma_resampler_get_output_latency")->args({"resampler"});
        addExtern<DAS_BIND_FUN(dasAudio_disableLinearResamplerFiltering)>(*this, lib, "ma_resampler_disable_linear_filtering",
            SideEffects::modifyArgument, "dasAudio_disableLinearResamplerFiltering")->args({"config"});
        // channel converter
        addAnnotation(new MAChannelConvertorConfigAnnotation(lib));
        addAnnotation(new MAChannelConverterAnnotation(lib));
        addExtern<DAS_BIND_FUN(ma_channel_converter_config_init),SimNode_ExtFuncCallAndCopyOrMove>(*this, lib, "ma_channel_converter_config_init",
            SideEffects::none, "ma_channel_converter_config_init")->args({"format", "channelsIn", "channelsOut", "channelMapIn", "channelMapOut", "mixingMode"});
        addExtern<DAS_BIND_FUN(dasAudio_ma_channel_converter_init)>(*this, lib, "ma_channel_converter_init",
            SideEffects::modifyArgumentAndExternal, "dasAudio_ma_channel_converter_init")->args({"config", "converter"});
        addExtern<DAS_BIND_FUN(dasAudio_ma_channel_converter_uninit)>(*this, lib, "ma_channel_converter_uninit",
            SideEffects::modifyArgumentAndExternal, "dasAudio_ma_channel_converter_uninit")->args({"converter"});
        addExtern<DAS_BIND_FUN(ma_channel_converter_process_pcm_frames)>(*this, lib, "ma_channel_converter_process_pcm_frames",
            SideEffects::modifyArgument, "ma_channel_converter_process_pcm_frames")->args({"converter", "pFramesOut", "pFramesIn", "frameCount"});
        // volume mixer
        addAnnotation(new MAVolumeMixerAnnotation(lib));
        addExtern<DAS_BIND_FUN(ma_volume_mixer_init)>(*this, lib, "ma_volume_mixer_init",
            SideEffects::modifyArgument, "ma_volume_mixer_init")->args({"mixer","nChannels"});
        addExtern<DAS_BIND_FUN(ma_volume_mixer_uninit)>(*this, lib, "ma_volume_mixer_uninit",
            SideEffects::modifyArgument, "ma_volume_mixer_uninit")->args({"mixer"});
        addExtern<DAS_BIND_FUN(ma_volume_mixer_set_channels)>(*this, lib, "ma_volume_mixer_set_channels",
            SideEffects::modifyArgument, "ma_volume_mixer_set_channels")->args({"mixer","nChannels"});
        addExtern<DAS_BIND_FUN(ma_volume_mixer_set_volume)>(*this, lib, "ma_volume_mixer_set_volume",
            SideEffects::modifyArgument, "ma_volume_mixer_set_volume")->args({"mixer", "volume"});
        addExtern<DAS_BIND_FUN(ma_volume_mixer_set_volume_over_time)>(*this, lib, "ma_volume_mixer_set_volume_over_time",
            SideEffects::modifyArgument, "ma_volume_mixer_set_volume_over_time")->args({"mixer", "volume", "nFrames"});
        addExtern<DAS_BIND_FUN(ma_volume_mixer_set_pan)>(*this, lib, "ma_volume_mixer_set_pan",
            SideEffects::modifyArgument, "ma_volume_mixer_set_pan")->args({"mixer", "pan"});
        addExtern<DAS_BIND_FUN(ma_volume_mixer_set_pan_immediate)>(*this, lib, "ma_volume_mixer_set_pan_immediate",
            SideEffects::modifyArgument, "ma_volume_mixer_set_pan_immediate")->args({"mixer", "pan"});
        addExtern<DAS_BIND_FUN(ma_volume_mixer_set_linear_pan)>(*this, lib, "ma_volume_mixer_set_linear_pan",
            SideEffects::modifyArgument, "ma_volume_mixer_set_linear_pan")->args({"mixer", "linearPan"});
        addExtern<DAS_BIND_FUN(ma_volume_mixer_process_pcm_frames)>(*this, lib, "ma_volume_mixer_process_pcm_frames",
            SideEffects::modifyArgument, "ma_volume_mixer_process_pcm_frames")->args({"mixer", "pFramesOut", "pFramesIn", "frameCount"});
        // sf2 voice
        addAnnotation(new MASF2EnvelopeAnnotation(lib));
        addAnnotation(new MASF2BiquadAnnotation(lib));
        addAnnotation(new MASF2VoiceAnnotation(lib));
        addExtern<DAS_BIND_FUN(ma_sf2_voice_init)>(*this, lib, "ma_sf2_voice_init",
            SideEffects::modifyArgument, "ma_sf2_voice_init")->args({"voice", "sample_rate"});
        addExtern<DAS_BIND_FUN(ma_sf2_voice_note_off)>(*this, lib, "ma_sf2_voice_note_off",
            SideEffects::modifyArgument, "ma_sf2_voice_note_off")->args({"voice"});
        addExtern<DAS_BIND_FUN(ma_sf2_voice_end_quick)>(*this, lib, "ma_sf2_voice_end_quick",
            SideEffects::modifyArgument, "ma_sf2_voice_end_quick")->args({"voice"});
        addExtern<DAS_BIND_FUN(ma_sf2_voice_render)>(*this, lib, "ma_sf2_voice_render",
            SideEffects::modifyArgument, "ma_sf2_voice_render")->args({"voice", "sample_data", "sample_data_len", "output", "output_offset", "frame_count"});
        addExtern<DAS_BIND_FUN(ma_sf2_voice_render_send)>(*this, lib, "ma_sf2_voice_render_send",
            SideEffects::modifyArgument, "ma_sf2_voice_render_send")->args({"voice", "sample_data", "sample_data_len", "dry_output", "reverb_output", "output_offset", "frame_count", "dry_gain", "wet_gain"});
        addExtern<DAS_BIND_FUN(ma_sf2_voice_render_send2)>(*this, lib, "ma_sf2_voice_render_send2",
            SideEffects::modifyArgument, "ma_sf2_voice_render_send2")->args({"voice", "sample_data", "sample_data_len", "dry_output", "reverb_output", "chorus_output", "output_offset", "frame_count", "dry_gain", "reverb_gain", "chorus_gain"});
        addExtern<DAS_BIND_FUN(ma_sf2_voice_render_send3)>(*this, lib, "ma_sf2_voice_render_send3",
            SideEffects::modifyArgument, "ma_sf2_voice_render_send3")->args({"voice", "sample_data", "sample_data_len", "dry_output", "reverb_output", "chorus_output", "delay_output", "output_offset", "frame_count", "dry_gain", "reverb_gain", "chorus_gain", "delay_gain"});
        addExtern<DAS_BIND_FUN(ma_sf2_voice_is_finished)>(*this, lib, "ma_sf2_voice_is_finished",
            SideEffects::none, "ma_sf2_voice_is_finished")->args({"voice"});
        addExtern<DAS_BIND_FUN(ma_sf2_envelope_init)>(*this, lib, "ma_sf2_envelope_init",
            SideEffects::modifyArgument, "ma_sf2_envelope_init")->args({"env"});
        addExtern<DAS_BIND_FUN(ma_sf2_envelope_start)>(*this, lib, "ma_sf2_envelope_start",
            SideEffects::modifyArgument, "ma_sf2_envelope_start")->args({"env", "sample_rate"});
        addExtern<DAS_BIND_FUN(ma_sf2_biquad_setup)>(*this, lib, "ma_sf2_biquad_setup",
            SideEffects::modifyArgument, "ma_sf2_biquad_setup")->args({"bq", "fc_normalized"});
        addExtern<DAS_BIND_FUN(ma_sf2_biquad_setup_hpf)>(*this, lib, "ma_sf2_biquad_setup_hpf",
            SideEffects::modifyArgument, "ma_sf2_biquad_setup_hpf")->args({"bq", "fc_normalized"});
        addExtern<DAS_BIND_FUN(das_ma_sf2_biquad_tick)>(*this, lib, "ma_sf2_biquad_tick",
            SideEffects::modifyArgument, "das_ma_sf2_biquad_tick")->args({"bq", "input"});
        // decoder
        addAnnotation(new MADecoderConfigAnnotation(lib));
        addAnnotation(new MADecoderAnnotation(lib));
        addExtern<DAS_BIND_FUN(ma_decoder_config_init),SimNode_ExtFuncCallAndCopyOrMove>(*this, lib, "ma_decoder_config_init",
            SideEffects::none, "ma_decoder_config_init")->args({"outputFormat", "outputChannels", "outputSampleRate"});
        addExtern<DAS_BIND_FUN(ma_decoder_config_init_default),SimNode_ExtFuncCallAndCopyOrMove>(*this, lib, "ma_decoder_config_init_default",
            SideEffects::none, "ma_decoder_config_init_default");
        addExtern<DAS_BIND_FUN(dasAudio_ma_decoder_init_memory)>(*this, lib, "ma_decoder_init_memory",
            SideEffects::modifyArgumentAndExternal, "dasAudio_ma_decoder_init_memory")->args({"pData", "dataSize", "config", "decoder"});
        addExtern<DAS_BIND_FUN(ma_decoder_init_file)>(*this, lib, "ma_decoder_init_file",
            SideEffects::modifyArgumentAndExternal, "ma_decoder_init_file")->args({"pFilePath", "config", "decoder"});
        addExtern<DAS_BIND_FUN(ma_decoder_uninit)>(*this, lib, "ma_decoder_uninit",
            SideEffects::modifyArgumentAndExternal, "ma_decoder_uninit")->args({"decoder"});
        addExtern<DAS_BIND_FUN(ma_decoder_get_cursor_in_pcm_frames)>(*this, lib, "ma_decoder_get_cursor_in_pcm_frames",
            SideEffects::none, "ma_decoder_get_cursor_in_pcm_frames")->args({"decoder", "pCursor"});
        addExtern<DAS_BIND_FUN(dasAudio_ma_decoder_get_length_in_pcm_frames)>(*this, lib, "ma_decoder_get_length_in_pcm_frames",
            SideEffects::none, "dasAudio_ma_decoder_get_length_in_pcm_frames")->args({"decoder"});
        addExtern<DAS_BIND_FUN(dasAudio_ma_decoder_read_pcm_frames)>(*this, lib, "ma_decoder_read_pcm_frames",
            SideEffects::modifyArgument, "dasAudio_ma_decoder_read_pcm_frames")->args({"decoder", "pFramesOut", "frameCount"});
        addExtern<DAS_BIND_FUN(ma_decoder_seek_to_pcm_frame)>(*this, lib, "ma_decoder_seek_to_pcm_frame",
            SideEffects::modifyArgument, "ma_decoder_seek_to_pcm_frame")->args({"decoder", "frameIndex"});
        addExtern<DAS_BIND_FUN(ma_decoder_get_available_frames)>(*this, lib, "ma_decoder_get_available_frames",
            SideEffects::none, "ma_decoder_get_available_frames")->args({"decoder", "pAvailableFrames"});
        // limiter
        addAnnotation(new MALimiterAnnotation(lib));
        addExtern<DAS_BIND_FUN(ma_limiter_init)>(*this, lib, "ma_limiter_init",
            SideEffects::modifyArgument, "ma_limiter_init")->args({"limiter", "threshold", "attack_time", "release_time", "sample_rate", "nChannels"});
        addExtern<DAS_BIND_FUN(ma_limiter_init_linear)>(*this, lib, "ma_limiter_init_linear",
            SideEffects::modifyArgument, "ma_limiter_init_linear")->args({"limiter", "nChannels"});
        addExtern<DAS_BIND_FUN(ma_limiter_process_pcm_frames)>(*this, lib, "ma_limiter_process_pcm_frames",
            SideEffects::modifyArgument, "ma_limiter_process_pcm_frames")->args({"limiter", "InFames", "OutFrames", "nFrames"});
        addExtern<DAS_BIND_FUN(ma_limiter_get_required_input_frame_count)>(*this, lib, "ma_limiter_get_required_input_frame_count",
            SideEffects::none, "ma_limiter_get_required_input_frame_count")->args({"limiter", "out_len"});
        addExtern<DAS_BIND_FUN(ma_limiter_uninit)>(*this, lib, "ma_limiter_uninit",
            SideEffects::modifyArgument, "ma_limiter_uninit")->args({"limiter"});
        // hrtf
        addAnnotation(new MAHrtfAnnotation(lib));
        addExtern<DAS_BIND_FUN(ma_hrtf_init)>(*this, lib, "ma_hrtf_init",
            SideEffects::modifyArgument, "ma_hrtf_init")->args({"hrtf", "sampleRate"});
        addExtern<DAS_BIND_FUN(ma_hrtf_process_frames)>(*this, lib, "ma_hrtf_process_frames",
            SideEffects::modifyArgument, "ma_hrtf_process_frames")->args({"hrtf", "pOut", "pIn", "nChannels", "frameCount"});
        addExtern<DAS_BIND_FUN(ma_hrtf_set_direction)>(*this, lib, "ma_hrtf_set_direction",
            SideEffects::modifyArgument, "ma_hrtf_set_direction")->args({"hrtf", "azimuth", "elevation"});
        addExtern<DAS_BIND_FUN(ma_hrtf_uninit)>(*this, lib, "ma_hrtf_uninit",
            SideEffects::modifyArgument, "ma_hrtf_uninit")->args({"hrtf"});
        return true;
    }
    virtual ModuleAotType aotRequire ( TextWriter & tw ) const override {
        tw << "#include \"../modules/dasAudio/src/dasAudio.h\"\n";
        return ModuleAotType::cpp;
    }
};

REGISTER_DYN_MODULE(Module_Audio,Module_Audio);

} // namespace das

REGISTER_MODULE_IN_NAMESPACE(Module_Audio, das);

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4701) // stb_vorbis.c(4758) : warning C4701: potentially uninitialized local variable 'mid' used
#endif

#undef STB_VORBIS_HEADER_ONLY
#include <extras/stb_vorbis.c>    /* Enables Vorbis decoding. */

#ifdef _MSC_VER
#pragma warning(pop)
#endif
