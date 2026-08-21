options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_audio_io shared public

require dasllama/dasllama_lint public
require audio   // miniaudio: decode any audio container to PCM

//! Decode any format miniaudio handles (wav / mp3 / flac / ogg …) to 16 kHz mono f32 PCM —
//! the sample layout dasLLAMA's ASR facade (`transcribe`, `add_user_audio`) consumes. The
//! facade takes samples, not a path, so audio-file IO stays out of the core module; this
//! optional companion is the decode rail apps require when they have a file. Empty array on
//! a decode failure (missing / unreadable / unsupported file).
def load_audio_16k_mono(path : string) : array<float> {
    var samples : array<float>
    var decoder = new ma_decoder
    var config <- ma_decoder_config_init(ma_format.ma_format_f32, 1u, 16000u)
    if (ma_decoder_init_file(path, unsafe(addr(config)), decoder) != ma_result.MA_SUCCESS) {
        unsafe {
            delete decoder
        }
        return <- samples
    }
    let chunk = 16000
    var total = 0
    while (true) {
        samples |> ensure_capacity(total + chunk)
        samples |> resize(total + chunk)   // grow by a chunk, read straight into the tail
        let got = int(ma_decoder_read_pcm_frames(decoder, unsafe(addr(samples[total])), uint64(chunk)))
        total += got
        if (got < chunk) {
            break
        }
    }
    samples |> resize(total)
    ma_decoder_uninit(decoder)
    unsafe {
        delete decoder
    }
    return <- samples
}

//! ``load_audio_16k_mono``'s in-memory twin: decode an audio FILE's bytes (wav / mp3 / flac /
//! ogg …) to 16 kHz mono f32 PCM without touching disk — the shape a server holding an upload
//! or a base64 part wants. Empty array on a decode failure (corrupt / unsupported bytes).
def decode_audio_16k_mono(bytes : array<uint8> | #) : array<float> {
    var samples : array<float>
    if (empty(bytes)) {
        return <- samples
    }
    var decoder = new ma_decoder
    var config <- ma_decoder_config_init(ma_format.ma_format_f32, 1u, 16000u)
    var rc = ma_result.MA_ERROR
    unsafe {
        rc = ma_decoder_init_memory(addr(bytes[0]), uint64(length(bytes)), addr(config), decoder)
    }
    if (rc != ma_result.MA_SUCCESS) {
        unsafe {
            delete decoder
        }
        return <- samples
    }
    let chunk = 16000
    var total = 0
    while (true) {
        samples |> ensure_capacity(total + chunk)
        samples |> resize(total + chunk)   // grow by a chunk, read straight into the tail
        let got = int(ma_decoder_read_pcm_frames(decoder, unsafe(addr(samples[total])), uint64(chunk)))
        total += got
        if (got < chunk) {
            break
        }
    }
    samples |> resize(total)
    ma_decoder_uninit(decoder)
    unsafe {
        delete decoder
    }
    return <- samples
}
