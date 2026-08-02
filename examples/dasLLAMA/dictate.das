options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options fast_math = true   // ggml-parity FP laxity (non-bit-exact, ~+10%)
options persistent_heap

require dasllama/dasllama       // ASR (load_asr_model/transcribe) + Silero-VAD (load_vad_model/VadIter)
require audio/audio_record      // microphone capture (sound_record_*)
require daslib/jobque_boost
require daslib/fio              // sleep
require strings
require math

// Live dictation: microphone -> Silero-VAD endpointing -> Parakeet-TDT. The VAD cuts the mic
// stream into utterances at silence boundaries; each finished utterance is transcribed EXACTLY
// ONCE (vs the old whole-take re-decode every 2 s), and the take ends automatically after a
// short trailing silence. Takes a ggml-bin ASR model (Parakeet or Whisper — the single-arg
// load_asr_model route); Parakeet is the latency pick (no 30 s window). The VAD weights (~1.2 MB)
// are checked in and MIT-licensed (Silero Team).
//
//   bin/daslang -jit examples/dasLLAMA/dictate.das -- <asr-model.bin> [max-seconds]
//   e.g. bin/daslang -jit examples/dasLLAMA/dictate.das -- ggml-parakeet-tdt-0.6b-v2-f32.bin 30

let RATE = 16000
let MAX_SECONDS_DEFAULT = 30.0
let END_SILENCE_S = 1.5    // stop the take after this much trailing silence past the last utterance

def vad_bin_default() : string {
    return "{get_das_root()}/modules/dasLLAMA/models/silero_vad.bin"
}

// slice [s,e) out of the running take and transcribe it once
def transcribe_slice(var m : AsrModel; lang : string; take : array<float>; s, e : int64) : string {
    if (e <= s) {
        return ""
    }
    var slice : array<float>
    slice |> resize(e - s)
    for (i in range64(e - s)) {
        slice[i] = take[s + i]
    }
    var sess <- create_session(m, lang)
    let text = transcribe(m, sess, slice)
    delete sess     // per-utterance session: free it, else the take leaks one session per segment
    delete slice
    return text
}

[export]
def main {  // nolint:STYLE038 - one linear record/VAD/transcribe session
    let cmd <- get_command_line_arguments()
    var model_path = ""
    var max_seconds = MAX_SECONDS_DEFAULT
    for (a in cmd) {
        if (ends_with(a, ".bin")) {   // ggml-bin ASR only; GGUF decoders need the 2-arg load_asr_model
            if (empty(model_path)) {
                model_path = a
            }
        } elif (!empty(model_path) && to_float(a) > 0.0) {
            max_seconds = to_float(a)
        }
    }
    if (empty(model_path)) {
        print("usage: daslang -jit examples/dasLLAMA/dictate.das -- <asr-model.bin> [max-seconds]\n")
        return
    }

    print("loading ASR  {model_path}\n")
    var m <- load_asr_model(model_path)
    let c <- caps(m)
    let lang = !empty(c.languages) && length(c.languages) == 1 ? c.languages[0] : "auto"
    print("loading VAD  {vad_bin_default()}\n")
    var vm <- load_vad_model(vad_bin_default())

    if (!sound_record_start(RATE, 1, int(max_seconds * float(RATE)) + RATE * 5, -1)) {
        panic("could not open the capture device (no microphone, or access denied)")
    }
    print("listening — speak; stops after {END_SILENCE_S} s of silence (max {max_seconds} s)\n\n")

    with_job_que() {
        setup_dasllama_jobque()
        var vit : VadIter
        vad_iter_reset(vit, default_vad_opts())

        var take : array<float>
        take |> reserve(int(max_seconds * float(RATE)) + RATE)
        var scratch : array<float>
        scratch |> resize(RATE / 4)
        var newbuf : array<float>
        newbuf |> reserve(RATE / 4)

        var seg_start = 0l
        var open = false          // a speech segment is currently in progress
        var any = false           // at least one segment has started
        var last_end = -1l        // absolute sample of the last utterance end
        var transcript = ""

        while (float(length(take)) < max_seconds * float(RATE)) {
            let n = sound_record_read(scratch)
            if (n > 0) {
                for (i in range(n)) {
                    take |> push(scratch[i])
                }
                newbuf |> resize(n)
                for (i in range(n)) {
                    newbuf[i] = scratch[i]
                }
                vad_iter_feed(vm, vit, newbuf) $(ev : VadEvent) {
                    if (ev.kind == VadEventKind.speech_start) {
                        seg_start = max(0l, ev.sample)
                        open = true
                        any = true
                        print("  [listening…]\n")
                    } else {
                        let e = min(ev.sample, long_length(take))
                        let text = transcribe_slice(m, lang, take, seg_start, e)
                        transcript += (empty(transcript) ? "" : " ") + text
                        print("  {text}\n")
                        open = false
                        last_end = e
                    }
                }
                // endpoint on a long trailing silence past the last utterance
                if (any && !open && last_end >= 0l && vit.cur - last_end > int64(END_SILENCE_S * float(RATE))) {
                    break
                }
            } else {
                sleep(5u)
            }
        }
        sound_record_stop()
        if (sound_record_overflow_frames() > 0l) {
            print("(dropped {sound_record_overflow_frames()} frames — the decode loop fell behind)\n")
        }
        // a segment still open at stop (speech ongoing) — transcribe its tail
        if (open) {
            let text = transcribe_slice(m, lang, take, seg_start, long_length(take))
            transcript += (empty(transcript) ? "" : " ") + text
        }
        print("\n=== transcript ===\n{transcript}\n")
    }
}
