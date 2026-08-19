options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options fast_math = true   // ggml-parity FP laxity (non-bit-exact, ~+10%); tests stay bit-exact

require dasllama/dasllama       // the public facade: engine + chat layer + audio tower
require daslib/jobque_boost
require daslib/fio
require strings
require math

// Audio chat over the whisper-tower families: create a chat that OWNS an audio tower, queue
// audio + text for a turn, respond. The soft-token splice and the per-model prompt shape
// (audio markers, template) all come from the chat layer — the same demo runs Qwen2-Audio,
// Qwen2.5-Omni, Ultravox (stock Llama-3 decoders), and Voxtral pairs, mirroring
// llama.cpp's mtmd-cli token for token:
//   qwen2a/qwen2.5o: <|im_start|>user\n<|audio_bos|>[audio]<|audio_eos|>{prompt}<|im_end|>\n<|im_start|>assistant\n
//   ultravox:        <|begin_of_text|><|start_header_id|>user<|end_header_id|>\n\n[audio]{prompt}<|eot_id|><|start_header_id|>assistant<|end_header_id|>\n\n
//   voxtral:         <s>[INST][BEGIN_AUDIO][audio]{prompt}[/INST]
//
//   bin/daslang -jit examples/dasLLAMA/audio_chat.das -- <decoder.gguf> <mmproj.gguf> <audio-file> [prompt]

let DEFAULT_PROMPT = "What is being said in this audio?"
let N_GEN = 128l
let CTX = 4096l

[export]
def main {
    let cmd <- get_command_line_arguments()
    var gguf_at = -1
    for (i in range(length(cmd))) {
        if (ends_with(cmd[i], ".gguf")) {
            gguf_at = i
            break
        }
    }
    if (gguf_at < 0 || gguf_at + 2 >= length(cmd)) {
        print("usage: daslang -jit examples/dasLLAMA/audio_chat.das -- <decoder.gguf> <mmproj.gguf> <audio-file> [prompt]\n")
        return
    }
    let model_path = cmd[gguf_at]
    let mmproj_path = cmd[gguf_at + 1]
    let audio_path = cmd[gguf_at + 2]
    let prompt = gguf_at + 3 < length(cmd) ? cmd[gguf_at + 3] : DEFAULT_PROMPT

    print("loading {model_path}\n")
    var m <- load_model(model_path, QuantMode.q8)
    m.config.seq_len = min(m.config.seq_len, CTX)
    print("loading {mmproj_path}\n")
    var tower <- load_audio_tower(mmproj_path)
    allow_cpu_prefill()   // CPU inference, said explicitly — on a Mac, set_metal_mode(MetalMode.when_available) picks the GPU rails instead
    let samples <- load_audio_16k_mono(audio_path)
    if (empty(samples)) {
        print("could not decode audio: {audio_path}\n")
        return
    }
    print("audio: {length(samples)} samples ({float(length(samples)) / float(int(AUDIO_SAMPLE_RATE))} s)\n")

    with_job_que() {
        setup_dasllama_jobque()
        var chat <- create_chat(m, tower, "", N_GEN)   // the chat takes ownership of the tower
        let t_enc = ref_time_ticks()
        add_user_audio(chat, samples)                  // encoded to soft tokens here
        print("encoded {chat.n_audio_rows} soft tokens in {get_time_usec(t_enc) / 1000} ms\n---\n")
        add_user(chat, prompt)
        respond(m, chat, SamplingParams()) $(piece) {
            fprint(fstdout(), piece)
            fflush(fstdout())
            return true
        }
        print("\n")
        let st = stats(chat.session)
        print("prefill {st.n_prompt} pos in {float(st.n_prompt) / float(st.prefill_tps)} s; {st.gen_tps} tok/s\n")
    }
}
