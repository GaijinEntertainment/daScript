#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_typefactory_bind.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/simulate/bind_enum.h"

#include "dasAudio.h"

#include <opl3.h>

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>
#include <volume_mixer.h>

MAKE_TYPE_FACTORY(Opl3Chip,opl3_chip)
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
    ma_resample_algorithm_speex \
);
DAS_BIND_ENUM_CAST ( ma_resample_algorithm );

DAS_BASE_BIND_ENUM ( ma_channel_mix_mode, ma_channel_mix_mode, \
    ma_channel_mix_mode_rectangular, \
    ma_channel_mix_mode_simple, \
    ma_channel_mix_mode_custom_weights, \
    ma_channel_mix_mode_planar_blend, \
    ma_channel_mix_mode_default \
);
DAS_BIND_ENUM_CAST ( ma_channel_mix_mode );

DAS_BASE_BIND_ENUM ( ma_dither_mode, ma_dither_mode, \
    ma_dither_mode_none, \
    ma_dither_mode_rectangle, \
    ma_dither_mode_triangle \
);
DAS_BIND_ENUM_CAST ( ma_dither_mode );

namespace das {

struct Opl3ChipAnnotation : ManagedStructureAnnotation<opl3_chip> {
    Opl3ChipAnnotation ( ModuleLibrary & mlib ) : ManagedStructureAnnotation("Opl3Chip", mlib, "opl3_chip") {
    }
};

static ma_device g_device;
static ma_log g_ma_log_struct;
static Context * g_mixer_context = nullptr;
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
    das_invoke_function<void>::invoke<Array&,int32_t,int32_t>(g_mixer_context,nullptr,g_mixer_function,buffer,g_channels,g_rate,fdt);
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
    g_mixer_context = get_clone_context(&context,uint32_t(ContextCategory::audio_context));
    g_mixer_function = mixer;
    g_mixer_env = daScriptEnvironment::bound;
    if ( ma_device_start(&g_device) != MA_SUCCESS ) {
        ma_device_uninit(&g_device);
        delete g_mixer_context;
        g_mixer_context = nullptr;
        return false;
    }
    g_mixer_initialized = true;
    return true;
}

void dasAudio_finalize ( void ) {
    if ( g_mixer_initialized ) {
        ma_device_uninit(&g_device);
        delete g_mixer_context;
        g_mixer_context = nullptr;
        g_mixer_initialized = false;
    }
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
        addField<DAS_BIND_MANAGED_FIELD(config)>("config","config");
    }
};

struct MAChannelConvertorConfigAnnotation : ManagedStructureAnnotation<ma_channel_converter_config> {
    MAChannelConvertorConfigAnnotation ( ModuleLibrary & mlib )
        : ManagedStructureAnnotation("ma_channel_converter_config", mlib, "ma_channel_converter_config") {
        addField<DAS_BIND_MANAGED_FIELD(format)>("format","format");
        addField<DAS_BIND_MANAGED_FIELD(channelsIn)>("channelsIn","channelsIn");
        addField<DAS_BIND_MANAGED_FIELD(channelsOut)>("channelsOut","channelsOut");
        addField<DAS_BIND_MANAGED_FIELD(channelMapIn)>("channelMapIn","channelMapIn");
        addField<DAS_BIND_MANAGED_FIELD(channelMapOut)>("channelMapOut","channelMapOut");
        addField<DAS_BIND_MANAGED_FIELD(mixingMode)>("mixingMode","mixingMode");
        addField<DAS_BIND_MANAGED_FIELD(weights)>("weights","weights");
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
        addField<DAS_BIND_MANAGED_FIELD(nChannels)>("channels","nChannels");
    }
};

struct MADecoderConfigAnnotation : ManagedStructureAnnotation<ma_decoder_config> {
    MADecoderConfigAnnotation ( ModuleLibrary & mlib )
        : ManagedStructureAnnotation("ma_decoder_config", mlib, "ma_decoder_config") {
        addField<DAS_BIND_MANAGED_FIELD(format)>("format","format");
        addField<DAS_BIND_MANAGED_FIELD(channels)>("channels","channels");
        addField<DAS_BIND_MANAGED_FIELD(channelMap)>("channelMap","channelMap");
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
    }
};

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
        ModuleLibrary lib;
        lib.addModule(this);
        lib.addBuiltInModule();
        // opl3
        addAnnotation(make_smart<Opl3ChipAnnotation>(lib));
        addExtern<DAS_BIND_FUN(OPL3_Generate)>(*this, lib, "OPL3_Generate",
            SideEffects::worstDefault, "OPL3_Generate")->args({"chip", "buf"});
        addExtern<DAS_BIND_FUN(OPL3_GenerateResampled)>(*this, lib, "OPL3_GenerateResampled",
            SideEffects::worstDefault, "OPL3_GenerateResampled")->args({"chip", "buf"});
        addExtern<DAS_BIND_FUN(OPL3_Reset)>(*this, lib, "OPL3_Reset",
            SideEffects::worstDefault, "OPL3_Reset")->args({"chip", "sampleRate"});
        addExtern<DAS_BIND_FUN(OPL3_WriteReg)>(*this, lib, "OPL3_WriteReg",
            SideEffects::worstDefault, "OPL3_WriteReg")->args({"chip", "reg", "v"});
        addExtern<DAS_BIND_FUN(OPL3_WriteRegBuffered)>(*this, lib, "OPL3_WriteRegBuffered",
            SideEffects::worstDefault, "OPL3_WriteRegBuffered")->args({"chip", "reg", "v"});
        addExtern<DAS_BIND_FUN(OPL3_GenerateStream)>(*this, lib, "OPL3_GenerateStream",
            SideEffects::worstDefault, "OPL3_GenerateStream")->args({"OPL3_GenerateStream", "sndptr", "numsamples"});
        addExtern<DAS_BIND_FUN(OPL3_Generate4Ch)>(*this, lib, "OPL3_Generate4Ch",
            SideEffects::worstDefault, "OPL3_Generate4Ch")->args({"chip", "buf"});
        addExtern<DAS_BIND_FUN(OPL3_Generate4ChResampled)>(*this, lib, "OPL3_Generate4ChResampled",
            SideEffects::worstDefault, "OPL3_Generate4ChResampled")->args({"chip", "buf"});
        addExtern<DAS_BIND_FUN(OPL3_Generate4ChStream)>(*this, lib, "OPL3_Generate4ChStream",
            SideEffects::worstDefault, "OPL3_Generate4ChStream")->args({"chip", "sndptr1", "sndptr2", "numsamples"});
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
        // resampler
        addAnnotation(make_smart<MAResamplerConfigAnnotation>(lib));
        addAnnotation(make_smart<MAResamplerAnnotation>(lib));
        addExtern<DAS_BIND_FUN(ma_resampler_config_init),SimNode_ExtFuncCallAndCopyOrMove>(*this, lib, "ma_resampler_config_init",
            SideEffects::none, "ma_resampler_config_init")->args({"format", "channels", "sampleRateIn", "sampleRateOut","algorithm"});
        addExtern<DAS_BIND_FUN(ma_resampler_init)>(*this, lib, "ma_resampler_init",
            SideEffects::modifyArgumentAndExternal, "ma_resampler_init")->args({"config", "resampler"});
        addExtern<DAS_BIND_FUN(ma_resampler_uninit)>(*this, lib, "ma_resampler_uninit",
            SideEffects::modifyArgumentAndExternal, "ma_resampler_uninit")->args({"resampler"});
        addExtern<DAS_BIND_FUN(ma_resampler_process_pcm_frames)>(*this, lib, "ma_resampler_process_pcm_frames",
            SideEffects::modifyArgument, "ma_resampler_process_pcm_frames")->args({"resampler", "pFramesIn", "pFrameCountIn", "pFramesOut", "pFrameCountOut"});
        addExtern<DAS_BIND_FUN(ma_resampler_set_rate)>(*this, lib, "ma_resampler_set_rate",
            SideEffects::modifyArgument, "ma_resampler_set_rate")->args({"resampler", "sampleRateIn", "sampleRateOut"});
        addExtern<DAS_BIND_FUN(ma_resampler_set_rate_ratio)>(*this, lib, "ma_resampler_set_rate_ratio",
            SideEffects::modifyArgument, "ma_resampler_set_rate_ratio")->args({"resampler", "ratioInOut"});
        addExtern<DAS_BIND_FUN(ma_resampler_get_required_input_frame_count)>(*this, lib, "ma_resampler_get_required_input_frame_count",
            SideEffects::none, "ma_resampler_get_required_input_frame_count")->args({"resampler", "outputFrameCount"});
        addExtern<DAS_BIND_FUN(ma_resampler_get_expected_output_frame_count)>(*this, lib, "ma_resampler_get_expected_output_frame_count",
            SideEffects::none, "ma_resampler_get_expected_output_frame_count")->args({"resampler", "inputFrameCount"});
        addExtern<DAS_BIND_FUN(ma_resampler_get_input_latency)>(*this, lib, "ma_resampler_get_input_latency",
            SideEffects::none, "ma_resampler_get_input_latency")->args({"resampler"});
        addExtern<DAS_BIND_FUN(ma_resampler_get_output_latency)>(*this, lib, "ma_resampler_get_output_latency",
            SideEffects::none, "ma_resampler_get_output_latency")->args({"resampler"});
        // channel converter
        addAnnotation(make_smart<MAChannelConvertorConfigAnnotation>(lib));
        addAnnotation(make_smart<MAChannelConverterAnnotation>(lib));
        addExtern<DAS_BIND_FUN(ma_channel_converter_config_init),SimNode_ExtFuncCallAndCopyOrMove>(*this, lib, "ma_channel_converter_config_init",
            SideEffects::none, "ma_channel_converter_config_init")->args({"format", "channelsIn", "channelsOut", "channelMapIn", "channelMapOut", "mixingMode"});
        addExtern<DAS_BIND_FUN(ma_channel_converter_init)>(*this, lib, "ma_channel_converter_init",
            SideEffects::modifyArgumentAndExternal, "ma_channel_converter_init")->args({"config", "converter"});
        addExtern<DAS_BIND_FUN(ma_channel_converter_uninit)>(*this, lib, "ma_channel_converter_uninit",
            SideEffects::modifyArgumentAndExternal, "ma_channel_converter_uninit")->args({"converter"});
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
        addExtern<DAS_BIND_FUN(ma_decoder_get_length_in_pcm_frames)>(*this, lib, "ma_decoder_get_length_in_pcm_frames",
            SideEffects::none, "ma_decoder_get_length_in_pcm_frames")->args({"decoder"});
        addExtern<DAS_BIND_FUN(ma_decoder_read_pcm_frames)>(*this, lib, "ma_decoder_read_pcm_frames",
            SideEffects::modifyArgument, "ma_decoder_read_pcm_frames")->args({"decoder", "pFramesOut", "frameCount"});
        addExtern<DAS_BIND_FUN(ma_decoder_seek_to_pcm_frame)>(*this, lib, "ma_decoder_seek_to_pcm_frame",
            SideEffects::modifyArgument, "ma_decoder_seek_to_pcm_frame")->args({"decoder", "frameIndex"});
        addExtern<DAS_BIND_FUN(ma_decoder_get_available_frames)>(*this, lib, "ma_decoder_get_available_frames",
            SideEffects::none, "ma_decoder_get_available_frames")->args({"decoder", "pAvailableFrames"});
        // limiter
        addAnnotation(make_smart<MALimiterAnnotation>(lib));
        addExtern<DAS_BIND_FUN(ma_limiter_init)>(*this, lib, "ma_limiter_init",
            SideEffects::modifyArgument, "ma_limiter_init")->args({"limiter", "threshold", "attack_time", "release_time", "sample_rate", "nChannels"});
        addExtern<DAS_BIND_FUN(ma_limiter_process_pcm_frames)>(*this, lib, "ma_limiter_process_pcm_frames",
            SideEffects::modifyArgument, "ma_limiter_process_pcm_frames")->args({"limiter", "InFames", "OutFrames", "nFrames"});
        addExtern<DAS_BIND_FUN(ma_limiter_get_required_input_frame_count)>(*this, lib, "ma_limiter_get_required_input_frame_count",
            SideEffects::none, "ma_limiter_get_required_input_frame_count")->args({"limiter", "out_len"});
        addExtern<DAS_BIND_FUN(ma_limiter_uninit)>(*this, lib, "ma_limiter_uninit",
            SideEffects::modifyArgument, "ma_limiter_uninit")->args({"limiter"});
        return true;
    }
    virtual ModuleAotType aotRequire ( TextWriter & tw ) const override {
        tw << "#include \"../modules/dasAudio/src/dasAudio.h\"\n";
        tw << "#include <miniaudio.h>\n";
        tw << "#include <volume_mixer.h>\n";
        return ModuleAotType::cpp;
    }
};

} // namespace das

REGISTER_MODULE_IN_NAMESPACE(Module_Audio, das);
