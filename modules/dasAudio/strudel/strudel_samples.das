options gen2
options indenting = 4

module strudel_samples shared

require audio/audio_boost
require audio
require daslib/fio
require daslib/strings_boost
require math

// A sample: interleaved float data + channel count.
[safe_when_uninitialized]
struct Sample {
    data     : array<float>   // interleaved PCM
    channels : int = 1
}

// A sample bank: sound name → array of variations.
struct SampleBank {
    sounds : table<string; array<Sample>>
}

// Load a single audio file via miniaudio decoder.
// Keeps native channel count (mono or stereo).
def load_audio_file(path : string) : Sample {
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
def is_audio_file(name : string) : bool {
    let lower = to_lower(name)
    return ends_with(lower, ".wav") || ends_with(lower, ".mp3") || ends_with(lower, ".flac") || ends_with(lower, ".ogg")
}

// Load a single sound from a directory of audio files.
def load_sound(path : string; name : string; var bank : SampleBank) {
    var variations : array<Sample>
    dir(path) $(file) {
        if (file == "." || file == "..") {
            return
        }
        if (is_audio_file(file)) {
            var sample <- load_audio_file("{path}/{file}")
            if (length(sample.data) > 0) {
                variations |> emplace <| sample
            }
        }
    }
    if (length(variations) > 0) {
        let soundName = to_lower(name)
        bank.sounds[soundName] <- variations
    }
}

// Load a Dirt-Samples style directory structure.
def load_sample_dir(root : string; var bank : SampleBank) {
    dir(root) $(entry) {
        if (entry == "." || entry == "..") {
            return
        }
        let subdir = "{root}/{entry}"
        var variations : array<Sample>
        dir(subdir) $(file) {
            if (file == "." || file == "..") {
                return
            }
            if (is_audio_file(file)) {
                var sample <- load_audio_file("{subdir}/{file}")
                if (length(sample.data) > 0) {
                    variations |> emplace <| sample
                }
            }
        }
        if (length(variations) > 0) {
            let soundName = to_lower(entry)
            bank.sounds[soundName] <- variations
        }
    }
}

// Load audio from in-memory bytes (WAV, MP3, FLAC, OGG).
// Uses miniaudio's memory decoder. Returns empty Sample on failure.
def load_audio_memory(data : array<uint8>) : Sample {
    var result : Sample
    if (length(data) == 0) {
        return <- result
    }
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
    let totalFrames = int(ma_decoder_get_length_in_pcm_frames(decoder))
    if (totalFrames > 0) {
        result.data |> resize(totalFrames * result.channels)
        ma_decoder_read_pcm_frames(decoder, unsafe(addr(result.data[0])), uint64(totalFrames))
    }
    ma_decoder_uninit(decoder)
    unsafe { delete decoder; }
    return <- result
}

// Load a sound from in-memory bytes into a sample bank.
def load_sound_memory(data : array<uint8>; name : string; var bank : SampleBank) {
    var sample <- load_audio_memory(data)
    if (length(sample.data) > 0) {
        let soundName = to_lower(name)
        var variations : array<Sample>
        variations |> emplace <| sample
        bank.sounds[soundName] <- variations
    }
}

// Render a sample with speed control via miniaudio resampler + channel converter.
// Returns interleaved stereo, no gain/pan applied.
def render_sample(bank : SampleBank; name : string; variation : int; speed : float) : array<float> {
    var stereo : array<float>
    bank.sounds |> get(name) $(variations) {
        if (length(variations) == 0) {
            return
        }
        let idx = variation % length(variations)
        let srcChannels = uint(variations[idx].channels)
        let srcLen = length(variations[idx].data)
        let srcFrames = srcLen / int(srcChannels)
        if (srcFrames == 0) {
            return
        }
        // resample: source rate * speed → target rate (MA_SAMPLE_RATE)
        // e.g. speed=2.0 means play at double speed = half the output frames
        var resampler : ma_resampler
        var resampler_config <- ma_resampler_config_init(
            ma_format.ma_format_f32,
            srcChannels,
            uint(float(MA_SAMPLE_RATE) * abs(speed)),
            uint(MA_SAMPLE_RATE),
            ma_resample_algorithm.ma_resample_algorithm_linear
        )
        ma_resampler_init(unsafe(addr(resampler_config)), unsafe(addr(resampler)))
        var outFrames = uint64(ma_resampler_get_expected_output_frame_count(unsafe(addr(resampler)), uint64(srcFrames)))
        var resampled : array<float>
        resampled |> resize(int(outFrames) * int(srcChannels))
        var inFrames = uint64(srcFrames)
        unsafe {
            ma_resampler_process_pcm_frames(
                addr(resampler),
                addr(variations[idx].data[0]),
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
