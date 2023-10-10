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

#include "dasAudio.h"

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

namespace das {

static ma_device g_device;
static ma_log g_ma_log_struct;
static shared_ptr<Context> g_mixer_context;
static daScriptEnvironment * g_mixer_env = nullptr;
static Func g_mixer_function = nullptr;
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
    buffer.data = (char *) pOutput;
    buffer.size = buffer.capacity = frameCount * g_channels;
    buffer.lock = 1;
    lock_guard<recursive_mutex> guard(*g_mixer_context->contextMutex);
    auto saved = daScriptEnvironment::bound;
    daScriptEnvironment::bound = g_mixer_env;
    g_mixer_context->restart();
    g_mixer_context.get()->runWithCatch([&](){
        das_invoke_function<void>::invoke<Array&,int32_t,int32_t>(g_mixer_context.get(),nullptr,g_mixer_function,buffer,g_channels,g_rate,fdt);
    });
    // TODO: i don't know what to do with exceptions here. im ignoring for now. better than crashing
    if ( const char* exp = g_mixer_context->getException() ) {
        g_mixer_context->to_err(&g_mixer_context->exceptionAt, exp);
    }
    daScriptEnvironment::bound = saved;
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
    g_mixer_env = daScriptEnvironment::bound;
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

struct MAHrtfAnnotation : ManagedStructureAnnotation<ma_hrtf> {
    MAHrtfAnnotation ( ModuleLibrary & mlib )
        : ManagedStructureAnnotation("ma_hrtf", mlib, "ma_hrtf") {
        addField<DAS_BIND_MANAGED_FIELD(taps)>("taps","taps");
        addField<DAS_BIND_MANAGED_FIELD(left)>("left","left");
        addField<DAS_BIND_MANAGED_FIELD(right)>("right","right");
        addField<DAS_BIND_MANAGED_FIELD(azimuth)>("azimuth","azimuth");
        addField<DAS_BIND_MANAGED_FIELD(elevation)>("elevation","elevation");
        addField<DAS_BIND_MANAGED_FIELD(sampleRate)>("sampleRate","sampleRate");
        addField<DAS_BIND_MANAGED_FIELD(mixbuffer)>("mixbuffer","mixbuffer");
        addField<DAS_BIND_MANAGED_FIELD(mixsize)>("mixsize","mixsize");
        addField<DAS_BIND_MANAGED_FIELD(leftfip)>("leftfip","leftfip");
        addField<DAS_BIND_MANAGED_FIELD(rightfip)>("rightfip","rightfip");
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
        if ( !Module::require("rtti") ) return false;
        initialized = true;
        // now, initialize
        ModuleLibrary lib(this);
        lib.addBuiltInModule();
        addBuiltinDependency(lib, Module::require("rtti"));
        // reverb
        addEnumeration(make_smart<EnumerationI3DL2Preset>());
        addAnnotation(make_smart<I3DL2ReverbPropertiesAnnotation>(lib));
        addAnnotation(make_smart<I3DL2ReverbAnnotation>(lib));
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
        // mixer
        addExtern<DAS_BIND_FUN(dasAudio_init)>(*this, lib, "sound_initalize",
            SideEffects::modifyExternal, "dasAudio_init")->args({"mixer", "rate", "channels","context"});
        addExtern<DAS_BIND_FUN(dasAudio_finalize)>(*this, lib, "sound_finalize",
            SideEffects::modifyExternal, "dasAudio_finalize");
        addExtern<DAS_BIND_FUN(dasAudio_mixerContext),SimNode_ExtFuncCallRef>(*this, lib, "mixer_context",
            SideEffects::modifyExternal, "dasAudio_mixerContext");
        // enums
        addEnumeration(make_smart<Enumerationma_format>());
        addEnumeration(make_smart<Enumerationma_resample_algorithm>());
        addEnumeration(make_smart<Enumerationma_channel_mix_mode>());
        addEnumeration(make_smart<Enumerationma_dither_mode>());
        addEnumeration(make_smart<Enumerationma_result>());
        // resampler
        addAnnotation(make_smart<MAResamplerConfigAnnotation>(lib));
        addAnnotation(make_smart<MAResamplerAnnotation>(lib));
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
        addAnnotation(make_smart<MAChannelConvertorConfigAnnotation>(lib));
        addAnnotation(make_smart<MAChannelConverterAnnotation>(lib));
        addExtern<DAS_BIND_FUN(ma_channel_converter_config_init),SimNode_ExtFuncCallAndCopyOrMove>(*this, lib, "ma_channel_converter_config_init",
            SideEffects::none, "ma_channel_converter_config_init")->args({"format", "channelsIn", "channelsOut", "channelMapIn", "channelMapOut", "mixingMode"});
        addExtern<DAS_BIND_FUN(dasAudio_ma_channel_converter_init)>(*this, lib, "ma_channel_converter_init",
            SideEffects::modifyArgumentAndExternal, "dasAudio_ma_channel_converter_init")->args({"config", "converter"});
        addExtern<DAS_BIND_FUN(dasAudio_ma_channel_converter_uninit)>(*this, lib, "ma_channel_converter_uninit",
            SideEffects::modifyArgumentAndExternal, "dasAudio_ma_channel_converter_uninit")->args({"converter"});
        addExtern<DAS_BIND_FUN(ma_channel_converter_process_pcm_frames)>(*this, lib, "ma_channel_converter_process_pcm_frames",
            SideEffects::modifyArgument, "ma_channel_converter_process_pcm_frames")->args({"converter", "pFramesOut", "pFramesIn", "frameCount"});
        // volume mixer
        addAnnotation(make_smart<MAVolumeMixerAnnotation>(lib));
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
        addExtern<DAS_BIND_FUN(ma_volume_mixer_process_pcm_frames)>(*this, lib, "ma_volume_mixer_process_pcm_frames",
            SideEffects::modifyArgument, "ma_volume_mixer_process_pcm_frames")->args({"mixer", "pFramesOut", "pFramesIn", "frameCount"});
        // decoder
        addAnnotation(make_smart<MADecoderConfigAnnotation>(lib));
        addAnnotation(make_smart<MADecoderAnnotation>(lib));
        addExtern<DAS_BIND_FUN(ma_decoder_config_init),SimNode_ExtFuncCallAndCopyOrMove>(*this, lib, "ma_decoder_config_init",
            SideEffects::none, "ma_decoder_config_init")->args({"outputFormat", "outputChannels", "outputSampleRate"});
        addExtern<DAS_BIND_FUN(ma_decoder_config_init_default),SimNode_ExtFuncCallAndCopyOrMove>(*this, lib, "ma_decoder_config_init_default",
            SideEffects::none, "ma_decoder_config_init_default");
        addExtern<DAS_BIND_FUN(ma_decoder_init_memory)>(*this, lib, "ma_decoder_init_memory",
            SideEffects::modifyArgumentAndExternal, "ma_decoder_init_memory")->args({"pData", "dataSize", "config", "decoder"});
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
        addAnnotation(make_smart<MALimiterAnnotation>(lib));
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
        addAnnotation(make_smart<MAHrtfAnnotation>(lib));
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
