options gen2
options indenting = 4
options no_global_variables = false
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module audio_boost shared private

//! Audio playback, mixing, 3D spatial audio, and effects.
//!
//! Provides a high-level API for playing sounds from files, PCM arrays, and PCM streams.
//! Supports volume, pan, pitch, 3D positioning with HRTF, distance attenuation,
//! Doppler effect, I3DL2 reverb presets, and look-ahead limiting.
//!
//! All audio commands are archive-serialized and shipped to the audio thread through a
//! byte ``Stream``. Stream bytes are owned C++-side, so cleanup is automatic: either
//! side can free without coordinating with the other (no "collect channel" needed).
//! Use `with_audio_system` to initialize and finalize the audio system.

require audio public
require math
require daslib/rtti

require daslib/array_boost
require daslib/jobque_boost
require daslib/apply_in_context
require daslib/archive

let public SPEED_OF_SOUND = 343.3f
//! Speed of sound in m/s, used for Doppler calculations.
let public MAX_SPEED_OF_OBJECT = SPEED_OF_SOUND / 2.
//! Maximum object velocity for Doppler effect (half speed of sound).
let private LOG2 = log(2.0)

// audio config
// MA_SAMPLE_RATE is defined in the C++ audio module (from HRTF_SAMPLE_RATE CMake define, default 48000)
let public MA_CHANNELS = 2
//! Number of output audio channels (always 2, stereo).
let public MA_HRTF = true
//! Whether HRTF spatial audio is enabled.

// limiter config
let MA_LIMITER_THRESHOLD = 0.95
let MA_LIMITER_ATTACK_TIME = 0.0001
let MA_LIMITER_RELEASE_TIME = .01

//! Sound ID — a unique handle for every playing sound. Returned by all play functions.
typedef public SID = uint64

let public INVALID_SID : SID = 0ul
//! Sentinel value meaning "no sound" or "invalid sound ID".

//! Playback state of an audio channel.
enum public AudioChannelState {
    stopped
    //! Channel has finished playing.
    stopping
    //! Channel is fading out and will stop.
    playing
    //! Channel is actively playing audio.
    paused
    //! Channel is paused.
    starting
    //! Channel has been created but has not started playing yet.
}

//! Concurrent status snapshot for a playing sound, readable via LockBox.
struct public AudioChannelStatus {
    state : AudioChannelState
    //! Current playback state.
    playback_position : uint64
    //! Current playback position in PCM frames.
    consumed_position : uint64
    //! Frames of actual audio consumed (excludes silence padding).
    stream_que_length : int
    //! Number of pending PCM chunks in the stream queue.
}

//! Distance attenuation coefficients for 3D audio.
//! Use the helper functions (inverse_distance_attenuation, linear_attenuation, etc.) to create instances.
[safe_when_uninitialized]
struct public Attenuation {
    A, B, C, D : float
    E : float = 1.
    F : float
}

def public compute_attenuation(attn : Attenuation; d : float) {
    //! compute attenuation given distance
    let d2 = d * d
    let denom = attn.D * d2 + attn.E * d + attn.F
    if (denom == 0.) {
        return 1.
    }
    return clamp((attn.A * d2 + attn.B * d + attn.C) / denom, 0., 1.)
}

def public inverse_distance_attenuation(dmin : float) {
    //! DEFAULT ATTENUATION MODEL
    //! fades with inverse distance, i.e. dmin / (d + dmin)
    // ( dmin / (d + dmin) )
    return Attenuation(A = 0., B = 0., C = dmin, D = 0., E = 1., F = dmin)
}

def public linear_attenuation(dmax : float) {
    //! fades linearly with distance, i.e 1 - d / dmax
    // ( -1/dmax * d + 1 ) / 1
    return Attenuation(A = 0., B = -1. / dmax, C = 1., D = 0., E = 0., F = 1.)
}

def public quadratic_attenuation(dmax : float) {
    //! fades quadratically with distance, i.e 1 - d^2 / dmax^2
    // ( -1/dmax^2 * d^2 + 1 ) / 1
    return Attenuation(A = -1. / dmax / dmax, B = 0., C = 1., D = 0., E = 0., F = 1.)
}

def public inverse_square_attenuation(dmin : float) {
    //! fades with inverse square distance, i.e. dmin^2 / (d^2 + dmin^2)
    // ( dmin^2 / (d^2 + dmin^2) )
    let dmin2 = dmin * dmin
    return Attenuation(A = 0., B = 0., C = dmin2, D = 1., E = 0., F = dmin2)
}

def public default_attenuation {
    //! default attenuation
    //! 1 / (d + 1)
    return inverse_distance_attenuation(1.)
}

class AudioSource {
    bitrate : int = MA_SAMPLE_RATE  // samples per second
    channels : int = 1              // 1 or 2
    def ready : bool {
        return true
    }
    def abstract get_samples(nframes : int) : array<float>
    def abstract append(var data : array<float>) : bool
    def abstract append_temporary(data : array<float>) : bool
    def set_position(pos : uint64) : uint64 {
        return 0ul
    }
    def update_status(var status : AudioChannelStatus#) {
        status.stream_que_length = 0
    }
}

class AudioChannel {
    sid    : SID = INVALID_SID
    paused : bool = false
    pause_fade : float = 1.0        // current fade level (0=silent, 1=full)
    pause_fade_target : float = 1.0 // 0=fading to pause, 1=fading to play
    stop   : bool = false
    pitch  : float = 1.
    volume : float = 1.
    source : AudioSource?
    resampler : ma_resampler
    channel_converter : ma_channel_converter
    volume_mixer : ma_volume_mixer
    playback_position : uint64 = 0ul
    position3d : float3
    velocity3d : float3
    doppler : float = 1.
    attenuation : Attenuation = default_attenuation()
    is3D : bool = false
    @do_not_delete status : LockBox? = null
    @do_not_delete reverb : I3DL2Reverb?
    @do_not_delete chorus : ma_chorus?
    hrtf : ma_hrtf
    def AudioChannel(var src : AudioSource?) {
        source = src
        // resampler
        var resampler_config <- ma_resampler_config_init(
            ma_format.ma_format_f32,
            uint(source.channels),
            uint(source.bitrate),
            uint(MA_SAMPLE_RATE),
            ma_resample_algorithm.ma_resample_algorithm_linear
        )
        ma_resampler_disable_linear_filtering(unsafe(addr(resampler_config)))
        ma_resampler_init(unsafe(addr(resampler_config)), unsafe(addr(resampler)))
        // channel converter
        var channel_converter_config <- ma_channel_converter_config_init(
            ma_format.ma_format_f32,
            uint(source.channels),
            null,
            uint(MA_CHANNELS),
            null,
            ma_channel_mix_mode.ma_channel_mix_mode_default
        )
        ma_channel_converter_init(unsafe(addr(channel_converter_config)), unsafe(addr(channel_converter)))
        // volume mixer
        ma_volume_mixer_init(unsafe(addr(volume_mixer)), 1u)
    }
    def set3D {
        is3D = true
        if (MA_HRTF) {
            ma_hrtf_init(unsafe(addr(hrtf)), uint(MA_SAMPLE_RATE))
            ma_channel_converter_uninit(unsafe(addr(channel_converter)))
            var channel_converter_config <- ma_channel_converter_config_init(
                ma_format.ma_format_f32,
                2u,
                null,
                uint(MA_CHANNELS),
                null,
                ma_channel_mix_mode.ma_channel_mix_mode_default
            )
            ma_channel_converter_init(unsafe(addr(channel_converter_config)), unsafe(addr(channel_converter)))
        }
    }
    def finalize {
        unsafe {
            delete source
        }
        ma_volume_mixer_uninit(unsafe(addr(volume_mixer)))
        ma_channel_converter_uninit(unsafe(addr(channel_converter)))
        ma_resampler_uninit(unsafe(addr(resampler)))
        if (status != null) {
            status |> notify_and_release
            status = null
        }
        if (reverb != null) {
            unsafe {
                delete reverb
            }
        }
        if (chorus != null) {
            unsafe {
                delete chorus
            }
        }
        if (MA_HRTF && is3D) {
            ma_hrtf_uninit(unsafe(addr(hrtf)))
        }
    }
    def report_eos {
        if (status != null) {
            status |> update() $(var data : AudioChannelStatus#) {
                data = AudioChannelStatus(state = AudioChannelState.stopped)
            }
        }
    }
    def mix(var data : array<float>#; channels, rate : int; dt : float) : bool {
        if (!source->ready() || g_pitch == 0.) {
            return true
        }
        if (paused) {
            if (status != null) {
                status |> update() $(var status_channel : AudioChannelStatus#) {
                    status_channel.state = AudioChannelState.paused
                    status_channel.playback_position = playback_position
                    source->update_status(status_channel)
                }
            }
            return true
        }
        if (stop && volume_mixer.volume == 0.) {
            return false
        }
        let inputRate = uint(float(source.bitrate) * pitch * doppler * g_pitch)
        ma_resampler_set_rate(unsafe(addr(resampler)), inputRate, uint(rate))
        var outputFrames = uint64(data |> length / channels)
        var inputFrames = ma_resampler_get_required_input_frame_count(unsafe(addr(resampler)), outputFrames)
        var samples <- source->get_samples(int(inputFrames))
        if (length(samples) == 0) {
            return false    // reached end of stream
        }
        if (length(samples) < int(inputFrames) * channels) {
            samples |> resize(int(inputFrames) * channels)
        }
        if (inputFrames != outputFrames) {
            // resample
            var temp : array<float>
            temp |> resize(int(outputFrames) * source.channels)
            ma_resampler_process_pcm_frames(unsafe(addr(resampler)),
                unsafe(addr(samples[0])),
                unsafe(addr(inputFrames)),
                unsafe(addr(temp[0])),
                unsafe(addr(outputFrames)))
            delete samples
            samples <- temp
        }
        // hrtf
        var nSoundChannels = source.channels
        if (is3D && MA_HRTF) {
            // void ma_hrtf_process_frames(ma_hrtf * hrtf, float * pOut, const float * pIn, ma_uint32 nChannels, ma_uint32 frameCount)
            var temp : array<float>
            temp |> resize(int(outputFrames) * 2)
            ma_hrtf_process_frames(unsafe(addr(hrtf)),
                unsafe(addr(temp[0])),
                unsafe(addr(samples[0])),
                uint(source.channels),
                uint(outputFrames))
            nSoundChannels = 2      // HRTF always produces stereo
            delete samples
            samples <- temp
        }
        // reverb
        if (reverb != null) {
            var temp : array<float>
            temp |> resize(int(outputFrames) * channels)
            if (nSoundChannels == 1) {
                reverb |> process_mono(unsafe(addr(samples[0])), unsafe(addr(temp[0])), int(outputFrames))
            } elif (nSoundChannels == 2) {
                reverb |> process_stereo(unsafe(addr(samples[0])), unsafe(addr(temp[0])), int(outputFrames))
            } else {
                panic("unsupported channel count {channels}")
            }
            delete samples
            samples <- temp
        }
        // chorus
        if (chorus != null && nSoundChannels == 2) {
            var temp : array<float>
            temp |> resize(int(outputFrames) * 2)
            chorus_process(chorus, unsafe(addr(samples[0])), unsafe(addr(temp[0])), int(outputFrames))
            delete samples
            samples <- temp
        }
        // convert channels
        var channel_data : array<float>
        channel_data |> resize(data |> length)
        ma_channel_converter_process_pcm_frames(unsafe(addr(channel_converter)),
            unsafe(addr(channel_data[0])),
            unsafe(addr(samples[0])),
            outputFrames)
        unsafe {
            delete samples
        }
        // apply volume envelope
        ma_volume_mixer_set_channels(unsafe(addr(volume_mixer)), uint(channels))
        ma_volume_mixer_process_pcm_frames(
            unsafe(addr(volume_mixer)),
            unsafe(addr(channel_data[0])),
            unsafe(addr(data[0])),
            outputFrames)
        delete channel_data
        // apply pause fade (2ms ramp to avoid clicks)
        if (pause_fade != pause_fade_target) {
            let fade_step = 1.0 / 96.0  // ~2ms at 48kHz
            for (f in range(int(outputFrames))) {
                let fade = pause_fade
                for (c in range(channels)) {
                    data[f * channels + c] *= fade
                }
                if (pause_fade_target < pause_fade) {
                    pause_fade = max(pause_fade - fade_step, 0.0)
                } else {
                    pause_fade = min(pause_fade + fade_step, 1.0)
                }
            }
            if (pause_fade <= 0.001 && pause_fade_target == 0.0) {
                paused = true
                pause_fade = 0.0
            } elif (pause_fade >= 0.999 && pause_fade_target == 1.0) {
                pause_fade = 1.0
            }
        }
        // update position
        playback_position += inputFrames
        // update status
        if (status != null) {
            status |> update() $(var status_channel : AudioChannelStatus#) {
                status_channel.state = paused ? AudioChannelState.paused : (stop ? AudioChannelState.stopping : AudioChannelState.playing)
                status_channel.playback_position = playback_position
                source->update_status(status_channel)
            }
        }
        return true
    }
}

var g_channels : array<AudioChannel?>
var g_sid_2_channel : table<SID; AudioChannel?>
var g_mixer_total_time = 0.lf
var g_mixer_total_samples = 0ul
var g_pitch = 1.

class AudioSourcePCM : AudioSource {
    samples : array<float>
    offsetFrames : int
    def AudioSourcePCM(ch, rate : int; var smp : array<float>) {
        bitrate = rate
        channels = ch
        samples <- smp
        offsetFrames = 0
    }
    def override append(var data : array<float>) : bool {
        append_temporary(data)
        delete data
        return true
    }
    def override append_temporary(data : array<float>) : bool {
        let ofs = length(samples)
        let nsmp = length(data)
        samples |> resize(ofs + nsmp)
        unsafe {
            memcpy(addr(samples[ofs]), addr(data[0]), nsmp * 4)
        }
        return true
    }
    def finalize {
        delete samples
    }
    def override get_samples(nframes : int) : array<float> {
        var data : array<float>
        let psmp = offsetFrames * channels
        let nsmp = min(length(samples) - psmp, nframes * channels)
        if (nsmp > 0) {
            data |> resize(nsmp)
            unsafe {
                memcpy(addr(data[0]), addr(samples[psmp]), nsmp * 4)
            }
            offsetFrames += nsmp / channels
        }
        return <- data
    }
    def override set_position(pos : uint64) : uint64 {
        offsetFrames = clamp(int(pos), 0, length(samples) / channels)
        return uint64(offsetFrames)
    }
}

struct StreamChunk {
    samples : array<float>
}

class AudioSourcePCMStream : AudioSource {
    chunks : array<StreamChunk>
    consumed : uint64 = 0ul  // frames of actual audio consumed (excludes silence)
    def AudioSourcePCMStream(ch, rate : int) {
        bitrate = rate
        channels = ch
    }
    def override append(var data : array<float>) : bool {
        chunks |> emplace(StreamChunk(samples <- data))
        return true
    }
    def override append_temporary(data : array<float>) : bool {
        chunks |> emplace(StreamChunk(samples <- clone_to_move(data)))
        return true
    }
    def finalize {
        delete chunks
    }
    def override get_samples(nframes : int) : array<float> {
        var data : array<float>
        let nsmp = nframes * channels
        data |> resize(nsmp)
        var ofs = 0
        while (ofs < nsmp && length(chunks) != 0) {
            let ncopy = min(length(chunks[0].samples), nsmp - ofs)
            if (ncopy != 0) {
                unsafe {
                    memcpy(unsafe(addr(data[ofs])), unsafe(addr(chunks[0].samples[0])), ncopy * 4)
                }
                chunks[0].samples |> erase(0, ncopy)
                if (chunks[0].samples |> empty) {
                    delete chunks[0].samples
                    chunks |> erase(0)
                }
                ofs += ncopy
            } else {
                break
            }
        }
        consumed += uint64(ofs / channels)
        return <- data
    }
    def override update_status(var status : AudioChannelStatus#) {
        status.stream_que_length = length(chunks)
        status.consumed_position = consumed
    }
}

class AudioSourcePCMLoop : AudioSource {
    samples : array<float>
    offset  : int
    def AudioSourcePCMLoop(ch, rate : int; var smp : array<float>) {
        bitrate = rate
        channels = ch
        samples <- smp
        offset = 0
    }
    def finalize {
        delete samples
    }
    def override get_samples(nframes : int) : array<float> {
        var data : array<float>
        let lsamples = length(samples)
        let wsmp = nframes * channels
        data |> resize(wsmp)
        var dofs = 0
        while (dofs < wsmp) {
            let lsmp = min(lsamples, offset + wsmp - dofs)
            let nsmp = lsmp - offset
            if (nsmp != 0) {
                unsafe {
                    memcpy(unsafe(addr(data[dofs])), unsafe(addr(samples[offset])), nsmp * 4)
                }
                offset += nsmp
            } else {
                offset = 0
            }
            dofs += nsmp
        }
        return <- data
    }
    def override append(var data : array<float>) : bool {
        return false
    }
    def override append_temporary(data : array<float>) : bool {
        return false
    }
    def override set_position(pos : uint64) : uint64 {
        let offsetFrames = clamp(int(pos), 0, length(samples) / channels)
        offset = offsetFrames * channels
        return uint64(offsetFrames)
    }
}

class AudioSourceDecoder : AudioSource {
    decoder : ma_decoder?
    isReady : bool = false
    def AudioSourceDecoder {
        pass
    }
    def initFromFile(filename : string; rate, nChannels : int) {
        decoder = new ma_decoder
        var config <- ma_decoder_config_init(ma_format.ma_format_f32, uint(channels), uint(rate))
        let result = ma_decoder_init_file(filename, unsafe(addr(config)), decoder)
        if (result == ma_result.MA_SUCCESS) {
            initFromDecoder(decoder, rate, nChannels)
        }
    }
    def initFromDecoder(var dec : ma_decoder?; rate, nChannels : int) {
        bitrate = rate
        channels = nChannels
        decoder = dec
        isReady = true
    }
    def finalize {
        if (isReady) {
            ma_decoder_uninit(decoder)
        }
        unsafe {
            delete decoder
        }
    }
    def override ready : bool {
        return isReady
    }
    def override get_samples(nframes : int) : array<float> {
        var samples : array<float>
        samples |> resize(nframes * channels)
        let rframes = ma_decoder_read_pcm_frames(
            decoder,
            unsafe(addr(samples[0])),
            uint64(nframes))
        samples |> resize(int(rframes) * channels)
        return <- samples
    }
    def override append(var data : array<float>) : bool {
        return false
    }
    def override append_temporary(data : array<float>) : bool {
        return false
    }
    def override set_position(pos : uint64) : uint64 {
        ma_decoder_seek_to_pcm_frame(decoder, pos)
        var cursor : uint64
        ma_decoder_get_cursor_in_pcm_frames(decoder, unsafe(addr(cursor)))
        return cursor
    }
}


def remove_channel(srci : int) {
    g_channels[srci]->report_eos()
    let sid = g_channels[srci].sid
    if (sid != INVALID_SID) {
        g_sid_2_channel |> erase(sid)
    }
    unsafe {
        delete g_channels[srci]
    }
    g_channels |> erase(srci)
}

var g_head_position : float3
var g_head_direction : float3 = float3(0., 1., 0.)
var g_head_velocity : float3

def update_hrtf(dt : float; nFrames : uint64) {
    for (ch in g_channels) {
        if (ch.is3D && !ch.stop) {
            var rxy = ch.position3d.xy - g_head_position.xy
            rxy = float2(rxy.x * g_head_direction.x + rxy.y * g_head_direction.y,
                        -rxy.x * g_head_direction.y + rxy.y * g_head_direction.x)
            let nrxy = normalize(rxy)
            static_if (MA_HRTF) {
                let asimuth = atan2(nrxy.y, nrxy.x)
                let elevation = atan2(ch.position3d.z - g_head_position.z, length(rxy))
                let iasimuth = int(asimuth * 180. / PI)
                let ielevation = int(elevation * 180. / PI)
                ma_hrtf_set_direction(unsafe(addr(ch.hrtf)), iasimuth, ielevation)
            } else {
                // panning
                ma_volume_mixer_set_pan(unsafe(addr(ch.volume_mixer)), nrxy.y)
            }
            // volume attenuation — ramp over frame to avoid clicks
            let distance = length(ch.position3d - g_head_position)
            let attn = compute_attenuation(ch.attenuation, distance)
            ma_volume_mixer_set_volume_over_time(unsafe(addr(ch.volume_mixer)), ch.volume * attn, nFrames)
            // doppler
            let vrel = g_head_velocity - ch.velocity3d
            let r = normalize(ch.position3d - g_head_position)
            let v = clamp(dot(vrel, r), -MAX_SPEED_OF_OBJECT, MAX_SPEED_OF_OBJECT)
            let doppler = 1. + v / SPEED_OF_SOUND
            ch.doppler = lerp(ch.doppler, doppler, 1.0 - exp(-max(LOG2 * dt, FLT_MIN) / 0.15))
        }
    }
}

struct AudioCommandAddDecoder {
    sid : SID
    decoder : uint64      //! opaque ma_decoder? (archive-safe)
    rate : int
    channels : int
}

struct AudioCommandAddDecoder3D : AudioCommandAddDecoder {
    position : float3
    attenuation : Attenuation
}

struct AudioCommandAddPCM {
    sid : SID
    samples : array<float>
    rate : int
    channels : int
    loop : bool
}

struct AudioCommandAddPCM3D : AudioCommandAddPCM {
    position : float3
    attenuation : Attenuation
}

struct AudioCommandAddPCMStream {
    sid : SID
    rate : int
    channels : int
    loop : bool
}

struct AudioCommandAddPCMStream3D : AudioCommandAddPCMStream {
    position : float3
    attenuation : Attenuation
}

struct AudioCommandStatus {
    sid : SID
    status : uint64       //! opaque LockBox? (archive-safe)
}

struct AudioCommandSoundPosition3D {
    sid : SID
    position : float3
    velocity : float3
}

struct AudioCommandHeadPosition3D {
    position : float3
    direction : float3
    velocity : float3
}

// Archive overloads for C++ ManagedStructureAnnotation types — POD, serialize raw.
def serialize(var arch : Archive; var value : ma_chorus_config) {
    arch |> serialize_raw(value)
}

def serialize(var arch : Archive; var value : I3DL2ReverbProperties) {
    arch |> serialize_raw(value)
}

variant AudioCommand {
    shutdown                : bool
    add_decoder             : AudioCommandAddDecoder
    add_decoder_3d          : AudioCommandAddDecoder3D
    add_pcm                 : AudioCommandAddPCM
    add_pcm_3d              : AudioCommandAddPCM3D
    add_pcm_stream          : AudioCommandAddPCMStream
    add_pcm_stream_3d       : AudioCommandAddPCMStream3D
    append_pcm              : tuple<sid : SID; samples : array<float>>
    append_box_pcm          : tuple<sid : SID; box : uint64>  //! opaque LockBox? (archive-safe)
    pause                   : tuple<sid : SID; paused : bool>
    volume                  : tuple<sid : SID; volume : float; time : float>
    pan                     : tuple<sid : SID; pan : float>
    pitch                   : tuple<sid : SID; pitch : float>
    global_pitch            : float
    stop                    : tuple<sid : SID; time : float>
    head_transform          : AudioCommandHeadPosition3D
    sound_position          : AudioCommandSoundPosition3D
    status                  : AudioCommandStatus
    reverb                  : tuple<sid : SID; properties : I3DL2ReverbProperties>
    chorus_cmd              : tuple<sid : SID; config : ma_chorus_config>
    set_playback_position   : tuple<sid : SID; position : uint64>
    global_pause            : bool
}

var g_command_stream : Stream?

[pinvoke, export]
def setup_command_processor(var s : Stream?) {
    g_command_stream = s
    g_command_stream |> add_ref
}

def add_channel(sid : SID; var channel : AudioChannel?) {
    g_channels |> push(channel)
    if (sid != INVALID_SID) {
        channel.sid = sid
        g_sid_2_channel |> insert(sid, channel)
    }
}

def add_channel_3d(sid : SID; position : float3; attenuation : Attenuation; var channel : AudioChannel?) {
    channel.position3d = position
    channel.attenuation = attenuation
    channel->set3D()
    add_channel(sid, channel)
}

def public audio_system_release_context {
    //! Release the mixer context reference. Commands buffered in the stream are drained on the mixer context.
    if (g_command_stream != null) {
        unsafe(invoke_in_context(mixer_context(), "command_processor"))
    }
}

[pinvoke]
def command_processor {
    return if (g_command_stream == null)
    g_command_stream |> gather_archive() $(var cmd : AudioCommand) {
        if (cmd is shutdown) {
            unsafe {
                delete g_channels
            }
            g_sid_2_channel |> clear()
            g_command_stream |> release
        } elif (cmd is add_decoder) {
            assume dcmd = cmd as add_decoder
            var decoder  = new AudioSourceDecoder()
            decoder->initFromDecoder(unsafe(reinterpret<ma_decoder?> dcmd.decoder), dcmd.rate, dcmd.channels)
            add_channel(dcmd.sid) <| new AudioChannel(decoder)
        } elif (cmd is add_decoder_3d) {
            assume dcmd = cmd as add_decoder_3d
            var decoder  = new AudioSourceDecoder()
            decoder->initFromDecoder(unsafe(reinterpret<ma_decoder?> dcmd.decoder), dcmd.rate, dcmd.channels)
            add_channel_3d(dcmd.sid, dcmd.position, dcmd.attenuation) <| new AudioChannel(decoder)
        } elif (cmd is add_pcm_stream) {
            assume pcmd = cmd as add_pcm_stream
            var decoder = new AudioSourcePCMStream(pcmd.channels, pcmd.rate)
            add_channel(pcmd.sid) <| new AudioChannel(decoder)
        } elif (cmd is add_pcm_stream_3d) {
            assume pcmd = cmd as add_pcm_stream_3d
            var decoder = new AudioSourcePCMStream(pcmd.channels, pcmd.rate)
            add_channel_3d(pcmd.sid, pcmd.position, pcmd.attenuation) <| new AudioChannel(decoder)
        } elif (cmd is add_pcm) {
            assume pcmd = cmd as add_pcm
            var decoder : audio_boost::AudioSource?
            if (pcmd.loop) {
                decoder = new AudioSourcePCMLoop(pcmd.channels, pcmd.rate, pcmd.samples)
            } else {
                decoder = new AudioSourcePCM(pcmd.channels, pcmd.rate, pcmd.samples)
            }
            add_channel(pcmd.sid) <| new AudioChannel(decoder)
        } elif (cmd is add_pcm_3d) {
            assume pcmd = cmd as add_pcm_3d
            var decoder : audio_boost::AudioSource?
            if (pcmd.loop) {
                decoder = new AudioSourcePCMLoop(pcmd.channels, pcmd.rate, pcmd.samples)
            } else {
                decoder = new AudioSourcePCM(pcmd.channels, pcmd.rate, pcmd.samples)
            }
            add_channel_3d(pcmd.sid, pcmd.position, pcmd.attenuation) <| new AudioChannel(decoder)
        } elif (cmd is append_pcm) {
            assume pcmd = cmd as append_pcm
            g_sid_2_channel |> get(pcmd.sid) $(var ch : AudioChannel?&) {
                ch.source->append(pcmd.samples)
            }
        } elif (cmd is append_box_pcm) {
            assume pcmd = cmd as append_box_pcm
            var box = unsafe(reinterpret<LockBox?>(pcmd.box))
            box |> grab() $(var samples : array<float>) {
                g_sid_2_channel |> get(pcmd.sid) $(var ch : AudioChannel?&) {
                    ch.source->append_temporary(samples)
                }
            }
        } elif (cmd is pause) {
            let pcmd = cmd as pause
            g_sid_2_channel |> get(pcmd.sid) $(var ch : AudioChannel?&) {
                if (pcmd.paused) {
                    ch.pause_fade_target = 0.0  // fade out to pause
                } else {
                    ch.paused = false
                    ch.pause_fade_target = 1.0  // fade in from pause
                }
            }
        } elif (cmd is volume) {
            let vcmd = cmd as volume
            g_sid_2_channel |> get(vcmd.sid) $(var ch : AudioChannel?&) {
                ch.volume = vcmd.volume
                if (vcmd.time > 0.) {
                    let nFrames = uint64(vcmd.time * float(MA_SAMPLE_RATE))
                    ma_volume_mixer_set_volume_over_time(unsafe(addr(ch.volume_mixer)), vcmd.volume, nFrames)
                } else {
                    ma_volume_mixer_set_volume(unsafe(addr(ch.volume_mixer)), vcmd.volume)
                }
            }
        } elif (cmd is pan) {
            let vcmd = cmd as pan
            g_sid_2_channel |> get(vcmd.sid) $(var ch : AudioChannel?&) {
                ma_volume_mixer_set_pan(unsafe(addr(ch.volume_mixer)), vcmd.pan)
            }
        } elif (cmd is pitch) {
            let pcmd = cmd as pitch
            g_sid_2_channel |> get(pcmd.sid) $(var ch : AudioChannel?&) {
                ch.pitch = pcmd.pitch
            }
        } elif (cmd is global_pitch) {
            g_pitch = cmd as global_pitch
        } elif (cmd is global_pause) {
            g_pause = cmd as global_pause // todo: envelope?
        } elif (cmd is stop) {
            let scmd = cmd as stop
            g_sid_2_channel |> get(scmd.sid) $(var ch : AudioChannel?&) {
                if (scmd.time > 0.) {
                    let nFrames = uint64(scmd.time * float(MA_SAMPLE_RATE))
                    ma_volume_mixer_set_volume_over_time(unsafe(addr(ch.volume_mixer)), 0., nFrames)
                    ch.stop = true
                } else {
                    ma_volume_mixer_set_volume(unsafe(addr(ch.volume_mixer)), 0.)
                    ch.stop = true
                }
            }
        } elif (cmd is head_transform) {
            assume hcmd = cmd as head_transform
            g_head_position = hcmd.position
            g_head_direction = normalize(hcmd.direction)
            g_head_velocity = hcmd.velocity
        } elif (cmd is sound_position) {
            assume scmd = cmd as sound_position
            g_sid_2_channel |> get(scmd.sid) $(var ch : AudioChannel?&) {
                ch.position3d = scmd.position
                ch.velocity3d = scmd.velocity
            }
        } elif (cmd is status) {
            assume scmd = cmd as status
            var scmd_status = unsafe(reinterpret<LockBox?>(scmd.status))
            let found = g_sid_2_channel |> get(scmd.sid) $(var ch : AudioChannel?&) {
                if (ch.status != null) {
                    if (scmd_status != null) {
                        panic("audio: status already set for {scmd.sid}")
                    }
                    ch.status |> notify_and_release
                }
                ch.status = scmd_status
            }
            if (!found) {
                if (scmd_status != null) {
                    scmd_status |> update() $(var data : AudioChannelStatus#) {
                        data = AudioChannelStatus(state = AudioChannelState.stopped)
                    }
                    scmd_status |> notify_and_release
                }
            }
        } elif (cmd is reverb) {
            assume rcmd = cmd as reverb
            g_sid_2_channel |> get(rcmd.sid) $(var ch : AudioChannel?&) {
                if (ch.reverb == null) {
                    ch.reverb = new I3DL2Reverb
                }
                ch.reverb |> set_sample_rate(float(MA_SAMPLE_RATE))
                ch.reverb |> set_properties(rcmd.properties)
            }
        } elif (cmd is chorus_cmd) {
            assume ccmd = cmd as chorus_cmd
            g_sid_2_channel |> get(ccmd.sid) $(var ch : AudioChannel?&) {
                if (ch.chorus == null) {
                    ch.chorus = new ma_chorus
                    chorus_init(ch.chorus, float(MA_SAMPLE_RATE))
                }
                chorus_set_config(ch.chorus, ccmd.config)
            }
        } elif (cmd is set_playback_position) {
            assume scmd = cmd as set_playback_position
            g_sid_2_channel |> get(scmd.sid) $(var ch : AudioChannel?&) {
                ch.playback_position = ch.source->set_position(scmd.position)
            }
        } else {
            panic("unknown audio command {cmd}")
        }
    }
}

var g_limiter : ma_limiter
var g_mix_buffer : array<float>
var g_pause : bool = false

[export]
def mixer(var data : array<float>#; channels, rate : int; dt : float) {
    let t0 = ref_time_ticks()
    command_processor()
    if (g_pause) {
        unsafe {
            memset32(addr(data[0]), 0u, length(data))
        }
    } else {
        let output_samples = length(data) / channels
        update_hrtf(dt, uint64(output_samples))
        let input_samples = int(ma_limiter_get_required_input_frame_count(unsafe(addr(g_limiter)), uint64(output_samples)))
        let current_samples = length(g_mix_buffer) / channels
        let missing_samples = input_samples - current_samples
        if (missing_samples < 0) {
            panic("{input_samples} - {current_samples}, need {output_samples}; mixer: missing_samples < 0")
        }
        g_mix_buffer |> resize(input_samples * channels)
        array_view(g_mix_buffer, current_samples * channels, missing_samples * channels) $(mix_data) {
            let srct = g_channels |> length
            for (i in range(srct)) {
                let srci = srct - i - 1
                assume ch = g_channels[srci]
                if (!ch->mix(mix_data, channels, rate, dt)) {
                    remove_channel(srci)
                }
            }
        }
        ma_limiter_process_pcm_frames(unsafe(addr(g_limiter)),
            unsafe(addr(g_mix_buffer[0])),
            unsafe(addr(data[0])),
            uint64(output_samples))
        g_mix_buffer |> erase(0, output_samples * channels)
    }
    g_mixer_total_time += double(get_time_usec(t0)) / 1000.lf
    g_mixer_total_samples += uint64(length(data) / channels)
}

[init]
def initialize_mixer {
    if (this_context().category.audio) {
        this_context().name := "audio_mixer"
        ma_limiter_init(unsafe(addr(g_limiter)),
            MA_LIMITER_THRESHOLD,
            MA_LIMITER_ATTACK_TIME,
            MA_LIMITER_RELEASE_TIME,
            float(MA_SAMPLE_RATE),
            uint(MA_CHANNELS))
    }
}
        /*
        ma_limiter_init_linear(unsafe(addr(g_limiter)),
            uint(MA_CHANNELS))
        */
        // print("limiter requires {int(g_limiter.attack_samples)} samples, {int(double(g_limiter.attack_samples)*1000.0lf/double(MA_SAMPLE_RATE))} ms\n")

[finalize]
def finalize_mixer {
    if (this_context().category.audio) {
        ma_limiter_uninit(unsafe(addr(g_limiter)))
        delete g_mix_buffer
        let SPEED = g_mixer_total_time / double(g_mixer_total_samples)
        let SPEED_OF_LIGHT = 1000.lf / 48000.lf
        let UTILIZATION = int(SPEED / SPEED_OF_LIGHT * 1000.lf)
        to_log(0, "mixer {UTILIZATION/10}.{UTILIZATION%10}% utilization\n")
    }
}

tuple public AudioSystemChannels {
    command : Stream ?
    next_sid : Atomic64 ?
}

def public audio_system_create : AudioSystemChannels {
    //! Low-level: creates the audio system. Prefer with_audio_system.
    sound_initalize(@@mixer, MA_SAMPLE_RATE, MA_CHANNELS, this_context())
    var s = unsafe(stream_create())
    s |> add_ref()
    g_command_stream = s
    g_sound_sid = atomic64_create()
    unsafe(invoke_in_context(mixer_context(), "setup_command_processor", s))
    return (command = s, next_sid = g_sound_sid)
}

def public audio_system_finalize(var s : Stream?&; var next_sid : Atomic64?) {
    //! Low-level: shuts down the audio system.
    s |> push_archive(AudioCommand(shutdown = true))
    unsafe(invoke_in_context(mixer_context(), "command_processor"))
    sound_finalize()
    g_command_stream |> release
    s |> join
    unsafe(stream_remove(s))
    unsafe(atomic64_remove(g_sound_sid))
}

def public with_audio_system(blk : block) {
    //! Initializes the audio system, runs the block, then shuts down. All audio API calls must happen inside this block.
    var channels = audio_system_create()
    invoke(blk)
    audio_system_finalize(channels.command, channels.next_sid)
}

var private g_sound_sid : Atomic64?

def public generate_sound_sid {
    //! generate unique ID for sound
    return g_sound_sid |> inc() |> uint64()
}

def make_decoder(filename : string; rate, channels : int) : ma_decoder? {
    var decoder = new ma_decoder
    var config <- ma_decoder_config_init(ma_format.ma_format_f32, uint(channels), uint(rate))
    let result = ma_decoder_init_file(filename, unsafe(addr(config)), decoder)
    if (result != ma_result.MA_SUCCESS) {
        return null
    }
    return decoder
}

// Batch: each command is archive-serialized once, accumulated into array<array<uint8>>,
// then pushed atomically as one Stream batch.
var global_batch : array<array<uint8>>?

def public begin_batch() {
    //! Begin batching audio commands. All commands until end_batch are sent atomically.
    if (global_batch != null) {
        panic("nested batch")
    }
    var tempBatch = new array<array<uint8>>()
    global_batch = tempBatch
}

def public end_batch() {
    //! End batching and send all batched commands atomically.
    if (global_batch == null) {
        panic("no batch")
    }
    g_command_stream |> push_batch(*global_batch)
    unsafe {
        delete * global_batch
    }
    global_batch = null
}

def public batch(cb : block) {
    //! Execute a block of audio commands as a single atomic batch.
    if (global_batch != null) {
        panic("nested batch")
    }
    var tempBatch : array<array<uint8>>

    global_batch = unsafe(addr(tempBatch))
    cb |> invoke()
    global_batch = null

    g_command_stream |> push_batch(tempBatch)
    unsafe {
        delete tempBatch
    }
}

def push_cmd(var cmd : AudioCommand) {
    var bytes <- mem_archive_save(cmd)
    if (global_batch != null) {
        *global_batch |> emplace(bytes)
    } else {
        g_command_stream |> push(bytes)
        unsafe {
            delete bytes
        }
    }
}

def public play_sound_from_file(filename : string; rate, channels : int) {
    //! plays sound from file
    //! note - this function is blocking for the duration of the decoder creation
    var decoder = make_decoder(filename, rate, channels)
    if (decoder == null) {
        return INVALID_SID
    }
    let sid = generate_sound_sid()
    push_cmd(AudioCommand(add_decoder = AudioCommandAddDecoder(sid = sid, decoder = intptr(decoder), rate = rate, channels = channels)))
    return sid
}

def public play_3d_sound_from_file(filename : string; position : float3; attenuation : Attenuation; rate, channels : int) {
    //! plays 3D sound from file
    //! note - this function is blocking for the duration of the decoder creation
    var decoder = make_decoder(filename, rate, channels)
    if (decoder == null) {
        return INVALID_SID
    }
    let sid = generate_sound_sid()
    push_cmd(AudioCommand(add_decoder_3d =
        AudioCommandAddDecoder3D(sid = sid, decoder = intptr(decoder), rate = rate, channels = channels, position = position, attenuation = attenuation)))
    return sid
}

def public play_sound_from_pcm_stream(rate, channels : int; sid : SID = generate_sound_sid()) {
    //! Create a PCM streaming channel. Feed it samples with append_to_pcm.
    push_cmd(AudioCommand(add_pcm_stream <- AudioCommandAddPCMStream(sid = sid, rate = rate, channels = channels)))
    return sid
}

def public play_3d_sound_from_pcm_stream(position : float3; attenuation : Attenuation; rate, channels : int) {
    //! Create a 3D PCM streaming channel. Feed it samples with append_to_pcm.
    let sid = generate_sound_sid()
    push_cmd(AudioCommand(add_pcm_stream_3d <-
        AudioCommandAddPCMStream3D(sid = sid, rate = rate, channels = channels, position = position, attenuation = attenuation)))
    return sid
}

def public play_sound_from_pcm(rate, channels : int; var samples : array<float>) {
    //! plays sound from PCM data
    let sid = generate_sound_sid()
    push_cmd(AudioCommand(add_pcm <- AudioCommandAddPCM(sid = sid, rate = rate, channels = channels, samples <- samples, loop = false)))
    return sid
}

def public play_sound_loop_from_pcm(rate, channels : int; var samples : array<float>) {
    //! plays looping sound from PCM data
    let sid = generate_sound_sid()
    push_cmd(AudioCommand(add_pcm <- AudioCommandAddPCM(sid = sid, rate = rate, channels = channels, samples <- samples, loop = true)))
    return sid
}

def public play_3d_sound_from_pcm(position : float3; attenuation : Attenuation; rate, channels : int; var samples : array<float>) {
    //! plays 3D sound from PCM data
    let sid = generate_sound_sid()
    push_cmd(AudioCommand(add_pcm_3d <-
        AudioCommandAddPCM3D(sid = sid, rate = rate, channels = channels, samples <- samples, loop = false, position = position, attenuation = attenuation)))
    return sid
}

def public play_3d_sound_loop_from_pcm(position : float3; attenuation : Attenuation; rate, channels : int; var samples : array<float>) {
    //! plays 3D looping sound from PCM data
    let sid = generate_sound_sid()
    push_cmd(AudioCommand(add_pcm_3d <-
        AudioCommandAddPCM3D(sid = sid, rate = rate, channels = channels, samples <- samples, loop = true, position = position, attenuation = attenuation)))
    return sid
}

def public append_to_pcm(sid : SID; var samples : array<float>) {
    //! append samples to PCM stream
    push_cmd(AudioCommand(append_pcm <- (sid = sid, samples <- samples)))
    return sid
}

def public append_box_to_pcm(sid : SID; box : LockBox?; var samples : array<float>) {
    //! append samples from lock box to PCM stream. Box is grabbed and released on audio thread.
    //! samples buffer must outlive the grab — caller keeps it alive until box.isReady.
    _builtin_lockbox_fill(box, unsafe(addr(samples)))
    push_cmd(AudioCommand(append_box_pcm = (sid = sid, box = intptr(box))))
    return sid
}

def public set_pause(sid : SID; paused : bool) {
    //! pause or unpause sound
    push_cmd(AudioCommand(pause = (sid = sid, paused = paused)))
    return sid
}

def public set_volume(sid : SID; volume : float; time : float = 0.002f) {
    //! set volume of sound
    push_cmd(AudioCommand(volume = (sid = sid, volume = volume, time = time)))
    return sid
}

def public set_pan(sid : SID; pan : float) {
    //! set pan of sound
    push_cmd(AudioCommand(pan = (sid = sid, pan = pan)))
    return sid
}

def public set_pitch(sid : SID; pitch : float) {
    //! set pitch of sound
    push_cmd(AudioCommand(pitch = (sid = sid, pitch = pitch)))
    return sid
}

def public set_global_pitch(pitch : float) {
    //! set global pitch of sounds
    push_cmd(AudioCommand(global_pitch = pitch))
}

def public set_global_pause(pause : bool) {
    //! set global pause of sounds
    push_cmd(AudioCommand(global_pause = pause))
}

def public stop(sid : SID; time : float = 0.0f) {
    //! stop sound
    push_cmd(AudioCommand(stop = (sid = sid, time = time)))
    return sid
}

def public set_head_position(pos, dir : float3; vel : float3 = float3(0.)) {
    //! set head position for 3D sound
    push_cmd(AudioCommand(head_transform = AudioCommandHeadPosition3D(position = pos, direction = dir, velocity = vel)))
}

def public set_position(sid : SID; pos : float3; dir : float3 = float3(0.)) {
    //! set sound position for 3D sound
    push_cmd(AudioCommand(sound_position = AudioCommandSoundPosition3D(sid = sid, position = pos, velocity = dir)))
    return sid
}

def public set_status_update(sid : SID; var status : LockBox?) {
    //! set status for sound
    status |> add_ref
    status |> append(1)
    status |> set() <| new AudioChannelStatus(state = AudioChannelState.starting)
    push_cmd(AudioCommand(status = AudioCommandStatus(sid = sid, status = intptr(status))))
    return sid
}

def public clear_status(var status : LockBox?) {
    //! clear status for sound
    status |> clear(type <AudioChannelStatus>)
}

def public unset_status_update(sid : SID) {
    //! unset status for sound
    push_cmd(AudioCommand(status = AudioCommandStatus(sid = sid, status = 0ul)))
    return sid
}

def public set_reverb(sid : SID; reverb : I3DL2ReverbProperties) {
    //! set reverb for sound
    push_cmd(AudioCommand(reverb = (sid = sid, properties = reverb)))
    return sid
}

//! I3DL2 reverb preset names.
enum public ReverbPreset {
    Generic
    PaddedCell
    Room
    Bathroom
    LivingRoom
    StoneRoom
    Auditorium
    ConcertHall
    Cave
    Arena
    Hangar
    CarpetedHallway
    Hallway
    StoneCorridor
    Alley
    Forest
    City
    Mountains
    Quarry
    Plain
    ParkingLot
    SewerPipe
    Underwater
}

def public set_reverb_preset(sid : SID; preset : ReverbPreset) {
    //! set reverb from preset for sound
    return set_reverb(sid, get_preset(unsafe(reinterpret<I3DL2Preset> preset)))
}

def public set_chorus(sid : SID; config : ma_chorus_config) {
    //! set chorus effect for sound
    push_cmd(AudioCommand(chorus_cmd = (sid = sid, config = config)))
    return sid
}

def public set_chorus_default(sid : SID) {
    //! set chorus effect with default settings for sound
    return set_chorus(sid, chorus_config_default())
}

def public set_playback_position(sid : SID; position : uint64) {
    //! set playback position for sound (in frames)
    push_cmd(AudioCommand(set_playback_position = (sid = sid, position = position)))
    return sid
}

def public decode_audio(data : array<uint8> | #; var channels, rate : int&) : array<float> {
    //! Decode an audio file (MP3, WAV, FLAC, Vorbis) from raw bytes to PCM float samples. Returns interleaved sample data; sets channels and rate.
    var decoder = new ma_decoder
    var config <- ma_decoder_config_init(ma_format.ma_format_f32, 0u, 0u)
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
}

def public get_audio_command_stream : void ? {
    return g_command_stream
}

[unsafe_operation]
def public set_audio_thread_command_stream(ch : void?) {
    var s = unsafe(reinterpret<Stream?> ch)
    if (s == g_command_stream) {
        return
    }
    if (g_command_stream != null) {
        g_command_stream |> release
    }
    g_command_stream = s
    if (g_command_stream != null) {
        g_command_stream |> add_ref
    }
}

def public get_sound_sid : void ? {
    return g_sound_sid
}

def public set_sound_sid(next_sid : void?) {
    var sid = unsafe(reinterpret<Atomic64?> next_sid)
    if (sid == g_sound_sid) {
        return
    }
    g_sound_sid = sid
}
