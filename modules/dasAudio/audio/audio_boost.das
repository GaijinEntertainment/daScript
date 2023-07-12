options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module audio_boost shared private

require audio
require math
require rtti
require debugapi

require daslib/array_boost
require daslib/jobque_boost
require daslib/apply_in_context

// audio config
let public MA_SAMPLE_RATE = 48000
let public MA_CHANNELS = 2

// limiter config
let MA_LIMITER_THRESHOLD = 1.0
let MA_LIMITER_ATTACK_TIME = 0.005
let MA_LIMITER_RELEASE_TIME = 0.100

typedef SID = uint64

class AudioSource
    bitrate : int = MA_SAMPLE_RATE  // samples per second
    channels : int = 1              // 1 or 2
    def ready : bool
        return true
    def abstract get_samples ( nsamples:int ) : array<float>
    def abstract append ( var data : array<float> ) : bool

class AudioChannel
    sid    : SID = 0ul
    paused : bool = false
    stop   : bool = false
    pitch  : float = 1.
    source : AudioSource?
    resampler : ma_resampler
    channel_converter : ma_channel_converter
    volume_mixer : ma_volume_mixer
    playback_position : uint64 = 0ul
    def AudioChannel ( src:AudioSource? )
        source = src
        // resampler
        var resampler_config <- ma_resampler_config_init(
            ma_format ma_format_f32,
            uint(source.channels),
            uint(source.bitrate),
            uint(MA_SAMPLE_RATE),
            ma_resample_algorithm ma_resample_algorithm_linear
        )
        ma_resampler_init(unsafe(addr(resampler_config)), unsafe(addr(resampler)))
        // channel converter
        var channel_converter_config <- ma_channel_converter_config_init(
            ma_format ma_format_f32,
            uint(source.channels),
            null,
            uint(MA_CHANNELS),
            null,
            ma_channel_mix_mode ma_channel_mix_mode_default
        )
        ma_channel_converter_init(unsafe(addr(channel_converter_config)), unsafe(addr(channel_converter)))
        // volume mixer
        ma_volume_mixer_init(unsafe(addr(volume_mixer)),1u)
    def finalize
        unsafe
            delete source
        ma_volume_mixer_uninit(unsafe(addr(volume_mixer)))
        ma_channel_converter_uninit(unsafe(addr(channel_converter)))
        ma_resampler_uninit(unsafe(addr(resampler)))
    def mix ( var data:array<float>#; channels,rate:int; dt:float ) : bool
        if paused || !source->ready()
            return true
        if stop && volume_mixer.volume==0.
            return false
        let inputRate = uint(float(source.bitrate) * pitch)
        ma_resampler_set_rate(unsafe(addr(resampler)), inputRate, uint(rate))
        var outputSamples = uint64(data|>length/channels)
        var inputSamples = ma_resampler_get_required_input_frame_count(unsafe(addr(resampler)), outputSamples)
        var samples <- source->get_samples(int(inputSamples))
        if length(samples)==0
            return false    // reached end of stream
        if length(samples) < int(inputSamples)
            samples |> resize(int(inputSamples))
        if inputSamples != outputSamples
            // resample
            var temp : array<float>
            temp |> resize(int(outputSamples))
            ma_resampler_process_pcm_frames(unsafe(addr(resampler)),
                unsafe(addr(samples[0])),
                unsafe(addr(inputSamples)),
                unsafe(addr(temp[0])),
                unsafe(addr(outputSamples)))
            delete samples
            samples <- temp
        // convert channels
        var channel_data : array<float>
        channel_data |> resize(data |> length)
        ma_channel_converter_process_pcm_frames(unsafe(addr(channel_converter)),
            unsafe(addr(channel_data[0])),
            unsafe(addr(samples[0])),
            outputSamples)
        unsafe
            delete samples
        // apply volume envelope
        ma_volume_mixer_set_channels(unsafe(addr(volume_mixer)), uint(channels))
        ma_volume_mixer_process_pcm_frames(
            unsafe(addr(volume_mixer)),
            unsafe(addr(channel_data[0])),
            unsafe(addr(data[0])),
            outputSamples)
        delete channel_data
        // update position
        playback_position += inputSamples
        return true

class AudioChannel2D : AudioChannel
    pan : float = 0.     // -1 left to 1 right

class AudioChannel3D : AudioChannel
    position : float3       // meters
    velocity : float3       // meters per second
    attenuation : float

var g_channels : array<AudioChannel?>
var g_sid_2_channel : table<uint64; AudioChannel?>
var g_mixer_total_time = 0.lf
var g_mixer_total_samples = 0ul

class AudioSourcePCM : AudioSource
    samples : array<float>
    def AudioSourcePCM ( ch,rate:int; var smp:array<float> )
        bitrate = rate
        channels = ch
        samples <- smp
    def override append ( var data : array<float> ) : bool
        let ofs = length(samples)
        let nsmp = length(data)
        samples |> resize(ofs+nsmp)
        unsafe
            memcpy ( unsafe(addr(samples[ofs])), unsafe(addr(data[0])), nsmp * 4 )
        delete data
        return true
    def finalize
        delete samples
    def override get_samples ( nsamples:int ) : array<float>
        let nsmp = min(length(samples), nsamples * channels)
        var data : array<float>
        if nsmp!=0
            data |> resize(nsmp)
            unsafe
                memcpy ( unsafe(addr(data[0])), unsafe(addr(samples[0])), nsmp * 4 )
            samples |> erase(0,nsmp)
        return <- data

let MA_RESULT_OK = 0

class AudioSourceDecoder : AudioSource
    decoder : ma_decoder?
    isReady : bool = false
    def AudioSourceDecoder
        pass
    def initFromFile ( filename:string; rate,nChannels:int )
        decoder = new ma_decoder
        var config <- ma_decoder_config_init(ma_format ma_format_f32, uint(channels), uint(rate))
        let result = ma_decoder_init_file(filename, unsafe(addr(config)), decoder)
        if result == MA_RESULT_OK
            initFromDecoder(decoder, rate, nChannels)
    def initFromDecoder ( dec:ma_decoder?; rate,nChannels:int )
        bitrate = rate
        channels = nChannels
        decoder = dec
        isReady = true
    def finalize
        if isReady
            ma_decoder_uninit(decoder)
        unsafe
            delete decoder
    def override ready : bool
        return isReady
    def override get_samples ( nsamples:int ) : array<float>
        var samples : array<float>
        samples |> resize(nsamples * channels)
        let nframes = ma_decoder_read_pcm_frames(
            decoder,
            unsafe(addr(samples[0])),
            uint64(nsamples))
        samples |> resize(int(nframes))
        return <- samples
    def override append ( var data : array<float> ) : bool
        return false

def remove_channel ( srci:int )
    let sid = g_channels[srci].sid
    if sid != 0ul
        g_sid_2_channel |> erase(sid)
    unsafe
        delete g_channels[srci]
    g_channels |> erase(srci)

variant AudioCommand
    shutdown    : bool
    add_decoder : tuple<sid:uint64; decoder:ma_decoder?; rate:int; channels:int>
    add_pcm     : tuple<sid:uint64; rate:int; channels:int; samples:array<float>>
    append_pcm  : tuple<sid:uint64; samples:array<float>>
    pause       : tuple<sid:uint64; paused:bool>
    volume      : tuple<sid:uint64; volume:float; time:float>
    stop        : tuple<sid:uint64; time:float>

var g_command_channel : Channel?

[pinvoke, export]
def setup_command_processor ( ch:Channel? )
    g_command_channel = ch
    g_command_channel |> add_ref

def command_processor
    return if g_command_channel==null
    var commands : array<AudioCommand>
    g_command_channel |> gather <| $ ( cmd:AudioCommand# )
        commands |> push_clone(cmd)
    for cmd in commands
        if cmd is shutdown
            delete g_channels
            g_command_channel |> notify_and_release
        elif cmd is add_decoder
            var dcmd = cmd as add_decoder
            var decoder  = new AudioSourceDecoder()
            decoder->initFromDecoder(unsafe(reinterpret<ma_decoder?> dcmd.decoder),dcmd.rate,dcmd.channels)
            var channel = new AudioChannel(decoder)
            g_channels |> push(channel)
            if dcmd.sid != 0ul
                channel.sid = dcmd.sid
                g_sid_2_channel[dcmd.sid] = channel
        elif cmd is add_pcm
            assume pcmd = cmd as add_pcm
            var decoder  = new AudioSourcePCM(pcmd.channels, pcmd.rate, clone_to_move(pcmd.samples))
            var channel = new AudioChannel(decoder)
            g_channels |> push(channel)
            if pcmd.sid != 0ul
                channel.sid = pcmd.sid
                g_sid_2_channel[pcmd.sid] = channel
        elif cmd is append_pcm
            assume pcmd = cmd as append_pcm
            g_sid_2_channel |> get(pcmd.sid) <| $ ( var ch:AudioChannel?& )
                ch.source->append(clone_to_move(pcmd.samples))
        elif cmd is pause
            var pcmd = cmd as pause
            g_sid_2_channel |> get(pcmd.sid) <| $ ( var ch:AudioChannel?& )
                ch.paused = pcmd.paused
        elif cmd is volume
            var vcmd = cmd as volume
            g_sid_2_channel |> get(vcmd.sid) <| $ ( var ch:AudioChannel?& )
                if vcmd.time > 0.
                    let nFrames = uint64(vcmd.time * float(MA_SAMPLE_RATE))
                    ma_volume_mixer_set_volume_over_time(unsafe(addr(ch.volume_mixer)), vcmd.volume, nFrames)
                else
                    ma_volume_mixer_set_volume(unsafe(addr(ch.volume_mixer)), vcmd.volume)
        elif cmd is stop
            var scmd = cmd as stop
            g_sid_2_channel |> get(scmd.sid) <| $ ( var ch:AudioChannel?& )
                if scmd.time > 0.
                    let nFrames = uint64(scmd.time * float(MA_SAMPLE_RATE))
                    ma_volume_mixer_set_volume_over_time(unsafe(addr(ch.volume_mixer)), 0., nFrames)
                    ch.stop = true
                else
                    ma_volume_mixer_set_volume(unsafe(addr(ch.volume_mixer)), 0.)
                    ch.stop = true
        else
            panic("unknown audio command {cmd}")

var g_limiter : ma_limiter
var g_mix_buffer : array<float>

[export]
def mixer ( var data:array<float>#; channels,rate:int; dt:float )
    let t0 = ref_time_ticks()
    command_processor()
    let output_samples = length(data)/channels
    let input_samples = int(ma_limiter_get_required_input_frame_count(unsafe(addr(g_limiter)),uint64(output_samples)))
    let current_samples = length(g_mix_buffer)/channels
    let missing_samples = input_samples - current_samples
    if missing_samples < 0
        panic("{input_samples} - {current_samples}, need {output_samples}; mixer: missing_samples < 0")
    g_mix_buffer |> resize(input_samples*channels)
    array_view(g_mix_buffer, current_samples * channels, missing_samples * channels) <| $ ( mix_data )
        let srct = g_channels |> length
        for i in range(srct)
            let srci = srct - i - 1
            if !g_channels[srci]->mix(mix_data,channels,rate,dt)
                remove_channel(srci)
    ma_limiter_process_pcm_frames(unsafe(addr(g_limiter)),
        unsafe(addr(g_mix_buffer[0])),
        unsafe(addr(data[0])),
        uint64(output_samples))
    g_mix_buffer |> erase(0, output_samples * channels)
    g_mixer_total_time += double(get_time_usec(t0)) / 1000.lf
    g_mixer_total_samples += uint64(length(data)/channels)

[init]
def initialize_mixer
    if this_context().category.audio
        this_context().name := "audio_mixer"
        ma_limiter_init(unsafe(addr(g_limiter)),
            MA_LIMITER_THRESHOLD,
            MA_LIMITER_ATTACK_TIME,
            MA_LIMITER_RELEASE_TIME,
            float(MA_SAMPLE_RATE),
            uint(MA_CHANNELS))

[finalize]
def finalize_mixer
    if this_context().category.audio
        ma_limiter_uninit(unsafe(addr(g_limiter)))
        delete g_mix_buffer
        let SPEED = g_mixer_total_time / double(g_mixer_total_samples)
        let SPEED_OF_LIGHT = 1000.lf / 48000.lf
        let UTILIZATION = int(SPEED / SPEED_OF_LIGHT * 1000.lf)
        to_log(LOG_INFO, "mixer {UTILIZATION/10}.{UTILIZATION%10}% utilization\n")

def public with_audio_system ( blk:block )
    sound_initalize(@@mixer,MA_SAMPLE_RATE,MA_CHANNELS,this_context())
    with_channel(1) <| $ ( channel )
        g_command_channel = channel
        unsafe
            invoke_in_context(mixer_context(),"setup_command_processor",channel)
        invoke(blk)
        channel |> push_clone <| [[AudioCommand shutdown=true]]
        g_command_channel = null
        channel |> join
    sound_finalize()

var private g_sound_sid = 1ul

def private generate_sid
    //! generate unique ID for sound
    return g_sound_sid ++

def public play_sound_from_file ( filename:string; rate,channels:int )
    //! plays sound from file
    //! note - this function is blocking for the duration of the decoder creation
    var decoder = new ma_decoder
    var config <- ma_decoder_config_init(ma_format ma_format_f32, uint(channels), uint(rate))
    let result = ma_decoder_init_file(filename, unsafe(addr(config)), decoder)
    if result != MA_RESULT_OK
        return 0ul
    let sid = generate_sid()
    g_command_channel |> push_clone <| [[AudioCommand add_decoder=[[auto sid, decoder, rate, channels]]]]
    return sid

def public play_sound_from_pcm ( rate,channels:int; var samples:array<float> )
    //! plays sound from PCM data
    var sid = generate_sid()
    g_command_channel |> push_clone <| [[AudioCommand add_pcm<-[[auto sid, rate, channels, samples]]]]
    return sid

def public append_to_pcm ( sid:SID; var samples:array<float> )
    //! append samples to PCM stream
    g_command_channel |> push_clone <| [[AudioCommand append_pcm<-[[auto sid, samples]]]]

def public set_pause ( sid:SID; paused:bool )
    //! pause or unpause sound
    g_command_channel |> push_clone <| [[AudioCommand pause=[[auto sid, paused]]]]

def public set_volume ( sid:SID; volume:float; time:float = 0.0f )
    //! set volume of sound
    g_command_channel |> push_clone <| [[AudioCommand volume=[[auto sid, volume, time]]]]

def public stop ( sid:SID; time:float = 0.0f )
    //! stop sound
    g_command_channel |> push_clone <| [[AudioCommand stop=[[auto sid, time]]]]

def public decode_audio ( data:array<uint8> | #; var channels,rate:int& ) : array<float>
    var decoder = new ma_decoder
    var config <- ma_decoder_config_init(ma_format ma_format_f32, 0u, 0u)
    let result = ma_decoder_init_memory(unsafe(addr(data[0])), uint64(length(data)), unsafe(addr(config)), decoder)
    channels = int(decoder.outputChannels)
    rate = int(decoder.outputSampleRate)
    let nframes = ma_decoder_get_length_in_pcm_frames(decoder)
    var samples : array<float>
    samples |> resize(int(nframes))
    let rframes = ma_decoder_read_pcm_frames(
        decoder,
        unsafe(addr(samples[0])),
        nframes)
    samples |> resize(int(rframes))
    ma_decoder_uninit(decoder)
    return <- samples

