options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_audio_io shared public

require dasllama/dasllama_lint public
require daslib/defer
require math
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

//! ``load_audio_16k_mono``'s in-memory twin: decode an audio FILE's bytes to 16 kHz mono f32 PCM
//! without touching disk. ``max_frames`` (> 0) hard-caps the DECODED length — compressed audio inflates
//! hundreds-fold over its bytes, so untrusted callers MUST pass one (0 = unbounded, trusted files only).
def decode_audio_16k_mono(bytes : array<uint8> | #; max_frames : int = 0) : array<float> {
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
    defer() {
        ma_decoder_uninit(decoder)
        unsafe {
            delete decoder
        }
    }
    if (max_frames > 0) {
        let declared_frames_at_16k = int64(ma_decoder_get_length_in_pcm_frames(decoder))
        if (declared_frames_at_16k > int64(max_frames)) {
            return <- samples
        }
    }
    let chunk = 16000
    var total = 0
    while (true) {
        let want = max_frames > 0 ? min(chunk, max_frames - total) : chunk
        if (want <= 0) {
            break
        }
        samples |> ensure_capacity(total + want)
        samples |> resize(total + want)   // grow by a chunk, read straight into the tail
        let got = int(ma_decoder_read_pcm_frames(decoder, unsafe(addr(samples[total])), uint64(want)))
        total += got
        if (got < want) {
            break
        }
    }
    samples |> resize(total)
    return <- samples
}
