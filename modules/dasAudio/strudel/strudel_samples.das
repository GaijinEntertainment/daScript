options gen2
options indenting = 4

module strudel_samples shared public
//! Sample bank and audio file loading for the strudel pattern engine.
//! Decodes WAV/MP3/FLAC/OGG via miniaudio (from disk or memory), organises
//! them into named banks with indexed variations, and resamples playback.

require audio/audio_boost
require audio
require daslib/fio
require daslib/strings_boost
require math

//! A decoded PCM audio sample.
[safe_when_uninitialized]
struct Sample {
    data        : array<float>   // interleaved PCM
        //! Interleaved PCM float samples (left, right, left, right, ...).
    channels    : int = 1
        //! Channel count (1 = mono, 2 = stereo).
    sample_rate : int = 48000
        //! Native sample rate of the decoded audio in Hz.
}

//! A named bank of samples, each mapped to an array of variations.
struct SampleBank {
    sounds : table<string; array<Sample>>
        //! Map from lowercase sound name to its indexed variations (bd:0, bd:1, ...).
}

def load_audio_file(path : string) : Sample {
    //! Decode a single audio file (WAV/MP3/FLAC/OGG) from disk into a Sample.
    //! Keeps the native channel count (mono or stereo); returns an empty Sample on failure.
    var result : Sample
    var decoder = new ma_decoder
    var config <- ma_decoder_config_init(ma_format.ma_format_f32, 0u, 0u)
    let res = ma_decoder_init_file(path, unsafe(addr(config)), decoder)
    if (res != ma_result.MA_SUCCESS) {
        unsafe {
            delete decoder
        }
        return <- result
    }
    result.channels = int(decoder.outputChannels)
    result.sample_rate = int(decoder.outputSampleRate)
    let totalFrames = int(ma_decoder_get_length_in_pcm_frames(decoder))
    if (totalFrames > 0) {
        result.data |> resize(totalFrames * result.channels)
        ma_decoder_read_pcm_frames(decoder, unsafe(addr(result.data[0])), uint64(totalFrames))
    }
    ma_decoder_uninit(decoder)
    unsafe {
        delete decoder
    }
    return <- result
}

// Check if a filename looks like an audio file.
def private is_audio_file(name : string) : bool {
    let lower = to_lower(name)
    return ends_with(lower, ".wav") || ends_with(lower, ".mp3") || ends_with(lower, ".flac") || ends_with(lower, ".ogg")
}

def load_sound(path : string; name : string; var bank : SampleBank) {
    //! Load a single named sound from a directory of audio files into the bank.
    //! Files are sorted alphabetically so index :0, :1, ... matches strudel's sample indexing.
    var files : array<string>
    dir(path) $(file) {
        if (file == "." || file == "..") return
        if (is_audio_file(file)) {
            files |> push(file)
        }
    }
    sort(files)
    var variations : array<Sample>
    for (file in files) {
        var sample <- load_audio_file("{path}/{file}")
        if (!empty(sample.data)) {
            variations |> emplace <| sample
        }
    }
    if (!empty(variations)) {
        let soundName = to_lower(name)
        bank.sounds[soundName] <- variations
    } else {
        to_log(LOG_WARNING, "load_sound: no audio files found in '{path}' for '{name}'\n")
    }
    delete files
}

// Load a Dirt-Samples style directory structure.
def private load_sample_dir(root : string; var bank : SampleBank) {
    dir(root) $(entry) {
        if (entry == "." || entry == "..") return
        let subdir = "{root}/{entry}"
        var variations : array<Sample>
        dir(subdir) $(file) {
            if (file == "." || file == "..") return
            if (is_audio_file(file)) {
                var sample <- load_audio_file("{subdir}/{file}")
                if (!empty(sample.data)) {
                    variations |> emplace <| sample
                }
            }
        }
        if (!empty(variations)) {
            let soundName = to_lower(entry)
            bank.sounds[soundName] <- variations
        }
    }
}

def load_audio_memory(data : array<uint8>) : Sample {
    //! Decode audio bytes (WAV/MP3/FLAC/OGG) from memory into a Sample.
    //! Uses miniaudio's memory decoder with explicit error checking; returns an empty Sample on failure.
    var result : Sample
    if (empty(data)) return <- result
    // decode_audio crashes on invalid data (array index out of range)
    // so we do our own decoding with proper error checking
    var decoder = new ma_decoder
    var config <- ma_decoder_config_init(ma_format.ma_format_f32, 0u, 0u)
    let res = ma_decoder_init_memory(unsafe(addr(data[0])), uint64(length(data)), unsafe(addr(config)), decoder)
    if (res != ma_result.MA_SUCCESS) {
        unsafe { delete decoder; }
        return <- result
    }
    result.channels = int(decoder.outputChannels)
    result.sample_rate = int(decoder.outputSampleRate)
    let totalFrames = int(ma_decoder_get_length_in_pcm_frames(decoder))
    if (totalFrames > 0) {
        result.data |> resize(totalFrames * result.channels)
        ma_decoder_read_pcm_frames(decoder, unsafe(addr(result.data[0])), uint64(totalFrames))
    }
    ma_decoder_uninit(decoder)
    unsafe { delete decoder; }
    return <- result
}

def load_sound_memory(data : array<uint8>; name : string; var bank : SampleBank) {
    //! Decode audio bytes and add the result as a single-variation sound in the bank.
    var sample <- load_audio_memory(data)
    if (!empty(sample.data)) {
        let soundName = to_lower(name)
        var variations : array<Sample>
        variations |> emplace <| sample
        bank.sounds[soundName] <- variations
    }
}

def render_sample(bank : SampleBank; name : string; variation : int; speed : float;
                  begin : float = 0.0; end_pos : float = 1.0) : array<float> {
    //! Render a sample slice at the given playback speed as interleaved stereo.
    //! begin/end_pos are normalized [0.0..1.0] slice bounds used by striate/chop/loop_at;
    //! speed resamples via miniaudio and mono input is upmixed to stereo. No gain/pan applied.
    var stereo : array<float>
    bank.sounds |> get(name) $(variations) {
        if (empty(variations)) return
        let idx = variation % length(variations)
        let srcChannels = uint(variations[idx].channels)
        let srcLen = length(variations[idx].data)
        let srcFrames = srcLen / int(srcChannels)
        if (srcFrames == 0) return
        // clamp begin/end to [0,1], compute slice frame range
        let b = clamp(begin, 0.0, 1.0)
        let e = clamp(end_pos, 0.0, 1.0)
        let sliceStart = int(b * float(srcFrames))
        let sliceStop = clamp(int(e * float(srcFrames)), sliceStart + 1, srcFrames)
        let sliceFrames = sliceStop - sliceStart
        if (sliceFrames <= 0) return
        // resample: source rate * speed → target rate (MA_SAMPLE_RATE)
        // e.g. speed=2.0 means play at double speed = half the output frames
        let srcRate = variations[idx].sample_rate
        var resampler : ma_resampler
        var resampler_config <- ma_resampler_config_init(
            ma_format.ma_format_f32,
            srcChannels,
            uint(float(srcRate) * abs(speed)),
            uint(MA_SAMPLE_RATE),
            ma_resample_algorithm.ma_resample_algorithm_linear
        )
        ma_resampler_init(unsafe(addr(resampler_config)), unsafe(addr(resampler)))
        var outFrames = uint64(ma_resampler_get_expected_output_frame_count(unsafe(addr(resampler)), uint64(sliceFrames)))
        var resampled : array<float>
        resampled |> resize(int(outFrames) * int(srcChannels))
        var inFrames = uint64(sliceFrames)
        unsafe {
            ma_resampler_process_pcm_frames(
                addr(resampler),
                addr(variations[idx].data[sliceStart * int(srcChannels)]),
                addr(inFrames),
                addr(resampled[0]),
                addr(outFrames)
            )
        }
        ma_resampler_uninit(unsafe(addr(resampler)))
        resampled |> resize(int(outFrames) * int(srcChannels))
        // convert to stereo if needed
        if (srcChannels == 2u) {
            stereo <- resampled
        } else {
            var converter : ma_channel_converter
            var converter_config <- ma_channel_converter_config_init(
                ma_format.ma_format_f32,
                srcChannels,
                null,
                2u,
                null,
                ma_channel_mix_mode.ma_channel_mix_mode_default
            )
            ma_channel_converter_init(unsafe(addr(converter_config)), unsafe(addr(converter)))
            stereo |> resize(int(outFrames) * 2)
            unsafe {
                ma_channel_converter_process_pcm_frames(
                    addr(converter),
                    addr(stereo[0]),
                    addr(resampled[0]),
                    outFrames
                )
            }
            ma_channel_converter_uninit(unsafe(addr(converter)))
        }
    }
    return <- stereo
}
