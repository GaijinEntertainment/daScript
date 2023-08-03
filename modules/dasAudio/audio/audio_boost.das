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

let public INVALID_SID : SID = 0ul

enum public AudioChannelState
    stopped
    stopping
    playing
    paused

struct public AudioChannelStatus
    sid : SID
    state : AudioChannelState
    playback_position : uint64
    stream_que_length : int

class AudioSource
    bitrate : int = MA_SAMPLE_RATE  // samples per second
    channels : int = 1              // 1 or 2
    def ready : bool
        return true
    def abstract get_samples(nframes : int) : array<float>
    def abstract append(var data : array<float>) : bool
    def update_status(var status : AudioChannelStatus)
        status.stream_que_length = 0

class AudioChannel
    sid    : SID = INVALID_SID
    paused : bool = false
    stop   : bool = false
    pitch  : float = 1.
    volume : float = 1.
    source : AudioSource?
    resampler : ma_resampler
    channel_converter : ma_channel_converter
    volume_mixer : ma_volume_mixer
    playback_position : uint64 = 0ul
    position3d : float3
    radius : float = 1.
    is3D : bool = false
    [[do_not_delete]] status : LockBox? = null
    [[do_not_delete]] status_channel : Channel? = null
    def AudioChannel(src : AudioSource?)
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
        ma_volume_mixer_init(unsafe(addr(volume_mixer)), 1u)
    def finalize
        unsafe
            delete source
        ma_volume_mixer_uninit(unsafe(addr(volume_mixer)))
        ma_channel_converter_uninit(unsafe(addr(channel_converter)))
        ma_resampler_uninit(unsafe(addr(resampler)))
        if status != null
            status |> notify_and_release
            status = null
        if status_channel != null
            status_channel |> notify_and_release
            status_channel = null
    def report_eos
        if status != null
            status |> set <| [[AudioChannelStatus sid=sid, state=AudioChannelState stopped]]
    def mix(var data : array<float>#; channels, rate : int; dt : float) : bool
        if paused || !source->ready()
            return true
        if stop && volume_mixer.volume == 0.
            return false
        let inputRate = uint(float(source.bitrate) * pitch)
        ma_resampler_set_rate(unsafe(addr(resampler)), inputRate, uint(rate))
        var outputFrames = uint64(data |> length / channels)
        var inputFrames = ma_resampler_get_required_input_frame_count(unsafe(addr(resampler)), outputFrames)
        var samples <- source->get_samples(int(inputFrames))
        if length(samples) == 0
            return false    // reached end of stream
        if length(samples) < int(inputFrames) * channels
            samples |> resize(int(inputFrames) * channels)
        if inputFrames != outputFrames
            // resample
            var temp : array<float>
            temp |> resize(int(outputFrames) * channels)
            ma_resampler_process_pcm_frames(unsafe(addr(resampler)),
                unsafe(addr(samples[0])),
                unsafe(addr(inputFrames)),
                unsafe(addr(temp[0])),
                unsafe(addr(outputFrames)))
            delete samples
            samples <- temp
        // convert channels
        var channel_data : array<float>
        channel_data |> resize(data |> length)
        ma_channel_converter_process_pcm_frames(unsafe(addr(channel_converter)),
            unsafe(addr(channel_data[0])),
            unsafe(addr(samples[0])),
            outputFrames)
        unsafe
            delete samples
        // apply volume envelope
        ma_volume_mixer_set_channels(unsafe(addr(volume_mixer)), uint(channels))
        ma_volume_mixer_process_pcm_frames(
            unsafe(addr(volume_mixer)),
            unsafe(addr(channel_data[0])),
            unsafe(addr(data[0])),
            outputFrames)
        delete channel_data
        // update position
        playback_position += inputFrames
        // update status
        if status != null || status_channel != null
            var status_data : AudioChannelStatus
            status_data.sid = sid
            status_data.state = paused ? AudioChannelState paused : (stop ? AudioChannelState stopping : AudioChannelState playing)
            status_data.playback_position = playback_position
            source->update_status(status_data)
            if status != null
                status |> set(status_data)
            if status_channel != null
                status_channel |> push_clone(status_data)
        return true

var g_channels : array<AudioChannel?>
var g_sid_2_channel : table<SID; AudioChannel?>
var g_mixer_total_time = 0.lf
var g_mixer_total_samples = 0ul

class AudioSourcePCM : AudioSource
    samples : array<float>
    def AudioSourcePCM(ch, rate : int; var smp : array<float>)
        bitrate = rate
        channels = ch
        samples <- smp
    def override append(var data : array<float>) : bool
        let ofs = length(samples)
        let nsmp = length(data)
        samples |> resize(ofs + nsmp)
        unsafe
            memcpy(unsafe(addr(samples[ofs])), unsafe(addr(data[0])), nsmp * 4)
        delete data
        return true
    def finalize
        delete samples
    def override get_samples(nframes : int) : array<float>
        var data : array<float>
        let nsmp = min(length(samples), nframes * channels)
        if nsmp != 0
            data |> resize(nsmp)
            unsafe
                memcpy(unsafe(addr(data[0])), unsafe(addr(samples[0])), nsmp * 4)
            samples |> erase(0, nsmp)
        return <- data

[skip_field_lock_check]
struct StreamChunk
    samples : array<float>

class AudioSourcePCMStream : AudioSource
    chunks : array<StreamChunk>
    def AudioSourcePCMStream(ch, rate : int)
        bitrate = rate
        channels = ch
    def override append(var data : array<float>) : bool
        chunks |> emplace([[StreamChunk samples <- data]])
        return true
    def finalize
        delete chunks
    def override get_samples(nframes : int) : array<float>
        var data : array<float>
        let nsmp = nframes * channels
        data |> resize(nsmp)
        var ofs = 0
        while ofs < nsmp && length(chunks) != 0
            var ncopy = min(length(chunks[0].samples), nsmp - ofs)
            if ncopy != 0
                unsafe
                    memcpy(unsafe(addr(data[ofs])), unsafe(addr(chunks[0].samples[0])), ncopy * 4)
                chunks[0].samples |> erase(0, ncopy)
                if chunks[0].samples |> empty
                    delete chunks[0].samples
                    chunks |> erase(0)
                ofs += ncopy
            else
                break
        return <- data
    def override update_status(var status : AudioChannelStatus)
        status.stream_que_length = length(chunks)

class AudioSourcePCMLoop : AudioSource
    samples : array<float>
    offset  : int
    def AudioSourcePCMLoop(ch, rate : int; var smp : array<float>)
        bitrate = rate
        channels = ch
        samples <- smp
        offset = 0
    def finalize
        delete samples
    def override get_samples(nframes : int) : array<float>
        var data : array<float>
        let lsamples = length(samples)
        let wsmp = nframes * channels
        data |> resize(wsmp)
        var dofs = 0
        while dofs < wsmp
            let lsmp = min(lsamples, offset + wsmp - dofs)
            let nsmp = lsmp - offset
            if nsmp != 0
                unsafe
                    memcpy(unsafe(addr(data[dofs])), unsafe(addr(samples[offset])), nsmp * 4)
                offset += nsmp
            else
                offset = 0
            dofs += nsmp
        return <- data

class AudioSourceDecoder : AudioSource
    decoder : ma_decoder?
    isReady : bool = false
    def AudioSourceDecoder
        pass
    def initFromFile(filename : string; rate, nChannels : int)
        decoder = new ma_decoder
        var config <- ma_decoder_config_init(ma_format ma_format_f32, uint(channels), uint(rate))
        let result = ma_decoder_init_file(filename, unsafe(addr(config)), decoder)
        if result == ma_result MA_SUCCESS
            initFromDecoder(decoder, rate, nChannels)
    def initFromDecoder(dec : ma_decoder?; rate, nChannels : int)
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
    def override get_samples(nframes : int) : array<float>
        var samples : array<float>
        samples |> resize(nframes * channels)
        let rframes = ma_decoder_read_pcm_frames(
            decoder,
            unsafe(addr(samples[0])),
            uint64(nframes))
        samples |> resize(int(rframes) * channels)
        return <- samples
    def override append(var data : array<float>) : bool
        return false

def remove_channel(srci : int)
    g_channels[srci]->report_eos()
    let sid = g_channels[srci].sid
    if sid != INVALID_SID
        g_sid_2_channel |> erase(sid)
    unsafe
        delete g_channels[srci]
    g_channels |> erase(srci)

var g_head_position : float3
var g_head_direction : float3 = float3(0., 1., 0.)

def update_hrtf
    for ch in g_channels
        if ch.is3D
            var rxy = ch.position3d.xy - g_head_position.xy
            rxy = float2(rxy.x * g_head_direction.x + rxy.y * g_head_direction.y,
                        -rxy.x * g_head_direction.y + rxy.y * g_head_direction.x)
            // linear volume attenuation???
            let attn = max(0.0f, 1.0f - length(rxy) / ch.radius)
            ma_volume_mixer_set_volume(unsafe(addr(ch.volume_mixer)), ch.volume * attn)
            // panning
            let nrxy = normalize(rxy)
            ma_volume_mixer_set_pan(unsafe(addr(ch.volume_mixer)), nrxy.y)

struct AudioCommandAddDecoder
    sid : SID
    [[do_not_delete]] decoder : ma_decoder?
    rate : int
    channels : int

struct AudioCommandAddDecoder3D : AudioCommandAddDecoder
    position : float3
    radius : float

struct AudioCommandAddPCM
    sid : SID
    samples : array<float>
    rate : int
    channels : int
    loop : bool

struct AudioCommandAddPCM3D : AudioCommandAddPCM
    position : float3
    radius : float

struct AudioCommandAddPCMStream
    sid : SID
    rate : int
    channels : int
    loop : bool

struct AudioCommandAddPCMStream3D : AudioCommandAddPCMStream
    position : float3
    radius : float

struct AudioCommandStatus
    sid : SID
    [[do_not_delete]] status : LockBox?
    [[do_not_delete]] status_channel : Channel?

variant AudioCommand
    shutdown            : bool
    add_decoder         : AudioCommandAddDecoder
    add_decoder_3d      : AudioCommandAddDecoder3D
    add_pcm             : AudioCommandAddPCM
    add_pcm_3d          : AudioCommandAddPCM3D
    add_pcm_stream      : AudioCommandAddPCMStream
    add_pcm_stream_3d   : AudioCommandAddPCMStream3D
    append_pcm          : tuple<sid : SID; samples : array<float>>
    pause               : tuple<sid : SID; paused : bool>
    volume              : tuple<sid : SID; volume : float; time : float>
    pan                 : tuple<sid : SID; pan : float>
    stop                : tuple<sid : SID; time : float>
    head_transform      : tuple<head_position : float3; head_direction : float3>
    sound_position      : tuple<sid : SID; position : float3>
    status              : AudioCommandStatus

var g_command_channel : Channel?

[pinvoke, export]
def setup_command_processor(ch : Channel?)
    g_command_channel = ch
    g_command_channel |> add_ref

def add_channel(sid : SID; var channel : AudioChannel?)
    g_channels |> push(channel)
    if sid != INVALID_SID
        channel.sid = sid
        g_sid_2_channel[sid] = channel

def add_channel_3d(sid : SID; position : float3; radius : float; var channel : AudioChannel?)
    channel.position3d = position
    channel.radius = radius
    channel.is3D = true
    print("make it 3D\n")
    add_channel(sid, channel)

def command_processor
    return if g_command_channel == null
    var commands : array<AudioCommand>
    g_command_channel |> gather <| $(cmd : AudioCommand#)
        commands |> push_clone(cmd)
    for cmd in commands
        if cmd is shutdown
            unsafe
                delete g_channels
            g_command_channel |> notify_and_release
        elif cmd is add_decoder
            assume dcmd = cmd as add_decoder
            var decoder  = new AudioSourceDecoder()
            decoder->initFromDecoder(unsafe(reinterpret<ma_decoder?> dcmd.decoder), dcmd.rate, dcmd.channels)
            add_channel(dcmd.sid) <| new AudioChannel(decoder)
        elif cmd is add_decoder_3d
            assume dcmd = cmd as add_decoder_3d
            var decoder  = new AudioSourceDecoder()
            decoder->initFromDecoder(unsafe(reinterpret<ma_decoder?> dcmd.decoder), dcmd.rate, dcmd.channels)
            add_channel_3d(dcmd.sid, dcmd.position, dcmd.radius) <| new AudioChannel(decoder)
        elif cmd is add_pcm_stream
            assume pcmd = cmd as add_pcm_stream
            var decoder = new AudioSourcePCMStream(pcmd.channels, pcmd.rate)
            add_channel(pcmd.sid) <| new AudioChannel(decoder)
        elif cmd is add_pcm_stream_3d
            assume pcmd = cmd as add_pcm_stream_3d
            var decoder = new AudioSourcePCMStream(pcmd.channels, pcmd.rate)
            add_channel_3d(pcmd.sid, pcmd.position, pcmd.radius) <| new AudioChannel(decoder)
        elif cmd is add_pcm
            assume pcmd = cmd as add_pcm
            var decoder : audio_boost::AudioSource?
            if pcmd.loop
                decoder = new AudioSourcePCMLoop(pcmd.channels, pcmd.rate, pcmd.samples)
            else
                decoder = new AudioSourcePCM(pcmd.channels, pcmd.rate, pcmd.samples)
            add_channel(pcmd.sid) <| new AudioChannel(decoder)
        elif cmd is add_pcm_3d
            assume pcmd = cmd as add_pcm_3d
            var decoder : audio_boost::AudioSource?
            if pcmd.loop
                decoder = new AudioSourcePCMLoop(pcmd.channels, pcmd.rate, pcmd.samples)
            else
                decoder = new AudioSourcePCM(pcmd.channels, pcmd.rate, pcmd.samples)
            add_channel_3d(pcmd.sid, pcmd.position, pcmd.radius) <| new AudioChannel(decoder)
        elif cmd is append_pcm
            assume pcmd = cmd as append_pcm
            g_sid_2_channel |> get(pcmd.sid) <| $(var ch : AudioChannel?&)
                ch.source->append(pcmd.samples)
        elif cmd is pause
            var pcmd = cmd as pause
            g_sid_2_channel |> get(pcmd.sid) <| $(var ch : AudioChannel?&)
                ch.paused = pcmd.paused
        elif cmd is volume
            var vcmd = cmd as volume
            g_sid_2_channel |> get(vcmd.sid) <| $(var ch : AudioChannel?&)
                ch.volume = vcmd.volume
                if vcmd.time > 0.
                    let nFrames = uint64(vcmd.time * float(MA_SAMPLE_RATE))
                    ma_volume_mixer_set_volume_over_time(unsafe(addr(ch.volume_mixer)), vcmd.volume, nFrames)
                else
                    ma_volume_mixer_set_volume(unsafe(addr(ch.volume_mixer)), vcmd.volume)
        elif cmd is pan
            var vcmd = cmd as pan
            g_sid_2_channel |> get(vcmd.sid) <| $(var ch : AudioChannel?&)
                ma_volume_mixer_set_pan(unsafe(addr(ch.volume_mixer)), vcmd.pan)
        elif cmd is stop
            var scmd = cmd as stop
            g_sid_2_channel |> get(scmd.sid) <| $(var ch : AudioChannel?&)
                if scmd.time > 0.
                    let nFrames = uint64(scmd.time * float(MA_SAMPLE_RATE))
                    ma_volume_mixer_set_volume_over_time(unsafe(addr(ch.volume_mixer)), 0., nFrames)
                    ch.stop = true
                else
                    ma_volume_mixer_set_volume(unsafe(addr(ch.volume_mixer)), 0.)
                    ch.stop = true
        elif cmd is head_transform
            assume hcmd = cmd as head_transform
            g_head_position = hcmd.head_position
            g_head_direction = normalize(hcmd.head_direction)
        elif cmd is sound_position
            assume scmd = cmd as sound_position
            g_sid_2_channel |> get(scmd.sid) <| $(var ch : AudioChannel?&)
                ch.position3d = scmd.position
        elif cmd is status
            assume scmd = cmd as status
            let found = g_sid_2_channel |> get(scmd.sid) <| $(var ch : AudioChannel?&)
                if ch.status != null
                    ch.status |> notify_and_release
                ch.status = scmd.status
                if ch.status_channel != null
                    ch.status_channel |> notify_and_release
                ch.status_channel = scmd.status_channel
            if !found
                if scmd.status != null
                    scmd.status |> set <| [[AudioChannelStatus sid=scmd.sid, state=AudioChannelState stopped]]
                    scmd.status |> notify_and_release
                if scmd.status_channel != null
                    scmd.status_channel |> push_clone <| [[AudioChannelStatus sid=scmd.sid, state=AudioChannelState stopped]]
                    scmd.status_channel |> notify_and_release
        else
            panic("unknown audio command {cmd}")
    delete commands

var g_limiter : ma_limiter
var g_mix_buffer : array<float>

[export]
def mixer(var data : array<float>#; channels, rate : int; dt : float)
    let t0 = ref_time_ticks()
    command_processor()
    update_hrtf()
    let output_samples = length(data) / channels
    let input_samples = int(ma_limiter_get_required_input_frame_count(unsafe(addr(g_limiter)), uint64(output_samples)))
    let current_samples = length(g_mix_buffer) / channels
    let missing_samples = input_samples - current_samples
    if missing_samples < 0
        panic("{input_samples} - {current_samples}, need {output_samples}; mixer: missing_samples < 0")
    g_mix_buffer |> resize(input_samples * channels)
    array_view(g_mix_buffer, current_samples * channels, missing_samples * channels) <| $(mix_data)
        let srct = g_channels |> length
        for i in range(srct)
            let srci = srct - i - 1
            assume ch = g_channels[srci]
            if !ch->mix(mix_data, channels, rate, dt)
                remove_channel(srci)
    ma_limiter_process_pcm_frames(unsafe(addr(g_limiter)),
        unsafe(addr(g_mix_buffer[0])),
        unsafe(addr(data[0])),
        uint64(output_samples))
    g_mix_buffer |> erase(0, output_samples * channels)
    g_mixer_total_time += double(get_time_usec(t0)) / 1000.lf
    g_mixer_total_samples += uint64(length(data) / channels)

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

def public audio_system_create() : Channel?
    sound_initalize(@@mixer, MA_SAMPLE_RATE, MA_CHANNELS, this_context())
    var channel = channel_create()
    g_command_channel = channel
    unsafe
        invoke_in_context(mixer_context(), "setup_command_processor", channel)
    return channel

def public audio_system_finalize(var channel : Channel?&)
    channel |> push_clone <| [[AudioCommand shutdown=true]]
    g_command_channel = null
    sound_finalize()
    channel |> join
    channel |> release
    channel = null

def public with_audio_system(blk : block)
    var channel = audio_system_create()
    invoke(blk)
    audio_system_finalize(channel)

var private g_sound_sid = 1ul

def public generate_sound_sid
    //! generate unique ID for sound
    return g_sound_sid ++

def make_decoder(filename : string; rate, channels : int)
    var decoder = new ma_decoder
    var config <- ma_decoder_config_init(ma_format ma_format_f32, uint(channels), uint(rate))
    let result = ma_decoder_init_file(filename, unsafe(addr(config)), decoder)
    if result != ma_result MA_SUCCESS
        return null
    return decoder

def public play_sound_from_file(filename : string; rate, channels : int)
    //! plays sound from file
    //! note - this function is blocking for the duration of the decoder creation
    var decoder = make_decoder(filename, rate, channels)
    if decoder == null
        return INVALID_SID
    let sid = generate_sound_sid()
    g_command_channel |> push_clone <| [[AudioCommand add_decoder=[[AudioCommandAddDecoder sid=sid, decoder=decoder, rate=rate, channels=channels]]]]
    return sid

def public play_3d_sound_from_file(filename : string; position : float3; radius : float; rate, channels : int)
    //! plays 3D sound from file
    //! note - this function is blocking for the duration of the decoder creation
    var decoder = make_decoder(filename, rate, channels)
    if decoder == null
        return INVALID_SID
    let sid = generate_sound_sid()
    g_command_channel |> push_clone <| [[AudioCommand add_decoder_3d=
        [[AudioCommandAddDecoder3D sid=sid, decoder=decoder, rate=rate, channels=channels, position=position, radius=radius]]]]
    return sid

def public play_sound_from_pcm_stream(rate, channels : int; sid : SID = generate_sound_sid())
    //! plays sound from PCM data
    g_command_channel |> push_clone <| [[AudioCommand add_pcm_stream <- [[AudioCommandAddPCMStream sid=sid, rate=rate, channels=channels]]]]
    return sid

def public play_3d_sound_from_pcm_stream(position : float3; radius : float; rate, channels : int)
    //! plays sound from PCM data
    var sid = generate_sound_sid()
    g_command_channel |> push_clone <| [[AudioCommand add_pcm_stream_3d <-
        [[AudioCommandAddPCMStream3D sid=sid, rate=rate, channels=channels, position=position, radius=radius]]]]
    return sid

def public play_sound_from_pcm(rate, channels : int; var samples : array<float>)
    //! plays sound from PCM data
    var sid = generate_sound_sid()
    g_command_channel |> push_clone <| [[AudioCommand add_pcm <- [[AudioCommandAddPCM sid=sid, rate=rate, channels=channels, samples <- samples, loop=false]]]]
    return sid

def public play_sound_loop_from_pcm(rate, channels : int; var samples : array<float>)
    //! plays looping sound from PCM data
    var sid = generate_sound_sid()
    g_command_channel |> push_clone <| [[AudioCommand add_pcm <- [[AudioCommandAddPCM sid=sid, rate=rate, channels=channels, samples <- samples, loop=true]]]]
    return sid

def public play_3d_sound_from_pcm(position : float3; radius : float; rate, channels : int; var samples : array<float>)
    //! plays 3D sound from PCM data
    var sid = generate_sound_sid()
    g_command_channel |> push_clone <| [[AudioCommand add_pcm_3d <-
        [[AudioCommandAddPCM3D sid=sid, rate=rate, channels=channels, samples <- samples, loop=false, position=position, radius=radius]]]]
    return sid

def public play_3d_sound_loop_from_pcm(position : float3; radius : float; rate, channels : int; var samples : array<float>)
    //! plays 3D looping sound from PCM data
    var sid = generate_sound_sid()
    g_command_channel |> push_clone <| [[AudioCommand add_pcm_3d <-
        [[AudioCommandAddPCM3D sid=sid, rate=rate, channels=channels, samples <- samples, loop=true, position=position, radius=radius]]]]
    return sid

def public append_to_pcm(sid : SID; var samples : array<float>)
    //! append samples to PCM stream
    g_command_channel |> push_clone <| [[AudioCommand append_pcm <- [[auto sid, samples]]]]

def public set_pause(sid : SID; paused : bool)
    //! pause or unpause sound
    g_command_channel |> push_clone <| [[AudioCommand pause=[[auto sid, paused]]]]

def public set_volume(sid : SID; volume : float; time : float = 0.0f)
    //! set volume of sound
    g_command_channel |> push_clone <| [[AudioCommand volume=[[auto sid, volume, time]]]]

def public set_pan(sid : SID; pan : float)
    //! set volume of sound
    g_command_channel |> push_clone <| [[AudioCommand pan=[[auto sid, pan]]]]

def public stop(sid : SID; time : float = 0.0f)
    //! stop sound
    g_command_channel |> push_clone <| [[AudioCommand stop=[[auto sid, time]]]]

def public set_head_position(pos, dir : float3)
    //! set head position for 3D sound
    g_command_channel |> push_clone <| [[AudioCommand pos, dir]]

def public set_position(sid : SID; pos : float3)
    //! set sound position for 3D sound
    g_command_channel |> push_clone <| [[AudioCommand sid, pos]]

def public set_status_update(sid : SID; var status : LockBox?)
    //! set status for sound
    status |> add_ref
    status |> append(1)
    g_command_channel |> push_clone <| [[AudioCommand status=[[AudioCommandStatus sid=sid, status=status, status_channel=null]]]]

def public set_status_update(sid : SID; var status_channel : Channel?)
    //! set status for sound
    status_channel |> add_ref
    g_command_channel |> push_clone <| [[AudioCommand status=[[AudioCommandStatus sid=sid, status=null, status_channel=status_channel]]]]

def public decode_audio(data : array<uint8> |#; var channels, rate : int&) : array<float>
    var decoder = new ma_decoder
    var config <- ma_decoder_config_init(ma_format ma_format_f32, 0u, 0u)
    let result = ma_decoder_init_memory(unsafe(addr(data[0])), uint64(length(data)), unsafe(addr(config)), decoder)
    channels = int(decoder.outputChannels)
    rate = int(decoder.outputSampleRate)
    let nframes = ma_decoder_get_length_in_pcm_frames(decoder)
    var samples : array<float>
    samples |> resize(int(nframes) * channels)
    let rframes = ma_decoder_read_pcm_frames(
        decoder,
        unsafe(addr(samples[0])),
        nframes)
    samples |> resize(int(rframes) * channels)
    ma_decoder_uninit(decoder)
    return <- samples

def public get_audio_command_channel : void ?
    return g_command_channel

[unsafe_operation]
def public set_audio_thread_command_channel(ch : void?)
    g_command_channel = unsafe(reinterpret<Channel?> ch)



