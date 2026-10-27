options gen2
// the bake tool's own reader: GGUFMeta is bake-side, not facade
options _dasllama_internal = true

module convert_sniff shared public

require dasllama/dasllama               // CANARY_MAGIC, rd_u32
require dasllama/dasllama_gguf          // GGUFMeta — bake-side, deliberately not facade
require dasllama/dasllama_nmt           // is_marian_model: the translation carrier's sniff
require daslib/fio

//! What a source file carries: a regular decoder GGUF (the Model
//! flow), an audio family, a vision mmproj, or a translation model
enum AsrCarrierKind {
    decoder
    whisper
    parakeet
    gigaam
    canary
    qwen3a
    gemma4a
    audio_tower   //!< the qwen2a / ultravox / voxtral mmproj
    vision
    bergamot      //!< a Marian translation student
}

//! The carrier kind of the file at `path`, off its magic and its GGUF
//! metadata; a file that is none of the audio or vision carriers - or
//! no file - is a decoder.
def sniff_asr_kind(path : string) : AsrCarrierKind {
    var st : FStat
    if (!stat(path, st) || st.is_dir || int64(st.size) < 8l) {
        return AsrCarrierKind.decoder
    }
    var kind = AsrCarrierKind.decoder
    let f = fopen(path, "rb")
    if (f == null) {
        return AsrCarrierKind.decoder
    }
    fmap(f) $(var bytes : array<uint8>#) {
        let magic = rd_u32(bytes, 0l)
        let ggml = asr_ggml_kind(magic, int(rd_u32(bytes, 4l)))
        if (is_marian_model(bytes)) {
            kind = AsrCarrierKind.bergamot
        } elif (ggml != AsrGgmlKind.none) {
            kind = ggml == AsrGgmlKind.parakeet ? AsrCarrierKind.parakeet : AsrCarrierKind.whisper
        } elif (magic == CANARY_MAGIC) {
            kind = AsrCarrierKind.canary
        } elif (magic == 0x46554747u) {   // GGUF: an audio mmproj or a plain decoder
            var inscope m <- parse_gguf_meta(bytes)
            var proj = ""
            if (gguf_has(m, "clip.audio.projector_type")) {
                proj = gguf_str(m, bytes, "clip.audio.projector_type")
            } elif (gguf_has(m, "clip.projector_type")) {
                proj = gguf_str(m, bytes, "clip.projector_type")
            }
            if (gguf_str_or(m, bytes, "general.architecture", "") == GIGAAM_ARCH) {
                kind = AsrCarrierKind.gigaam
            } elif (proj == "qwen3a") {
                kind = AsrCarrierKind.qwen3a
            } elif (proj == "gemma4a") {
                kind = AsrCarrierKind.gemma4a
            } elif (proj == "qwen2a" || proj == "qwen2.5o"
                    || proj == "ultravox" || proj == "voxtral") {
                kind = AsrCarrierKind.audio_tower
            } elif (proj != "") {
                kind = AsrCarrierKind.vision
            }
        }
    }
    fclose(f)
    return kind
}
