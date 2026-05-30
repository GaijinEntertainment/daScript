options gen2
options rtti  // required so sprint_json/sscan_json honor @optional / @rename
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost public
require openai/openai_audio
require strings
require daslib/fio

def private synth_wav(t : T?; client : OpenAIClient; text, path : string) : bool {
    let sreq = SpeechRequest(model = "kokoro", input = text, voice = "af_heart", response_format = "wav")
    let sres <- speech(client, sreq)
    if (!sres.ok) {
        t |> failure("TTS setup failed: {sres.error.message}")
        return false
    }
    fopen(path, "wb") $(f) {
        fwrite(f, sres.audio)
    }
    return true
}

// ===== Live integration (requires the Kokoro+Whisper server on :8880) =====

[test]
def test_transcribe_live_round_trip(t : T?) {
    t |> run("TTS -> STT round trip against the local server") @(t : T?) {
        let client = openai_client("http://localhost:8880/v1")
        let path = "test_stt_rt.wav"
        if (synth_wav(t, client, "The quick brown fox.", path)) {
            let res = transcribe(client, "whisper-1", path)
            remove(path)
            t |> success(res.ok, "transcribe ok (kind={res.error.kind} status={res.error.status} {res.error.message})")
            if (res.ok) {
                t |> success(!empty(res.text), "got transcript ({res.text})")
                t |> success(find(res.text, "fox") >= 0, "transcript mentions 'fox'")
            }
        }
    }
}

[test]
def test_translate_live(t : T?) {
    t |> run("translation endpoint against the local server") @(t : T?) {
        let client = openai_client("http://localhost:8880/v1")
        let path = "test_translate_rt.wav"
        if (synth_wav(t, client, "Hello world.", path)) {
            let res = translate(client, "whisper-1", path)
            remove(path)
            t |> success(res.ok, "translate ok (kind={res.error.kind} status={res.error.status} {res.error.message})")
            if (res.ok) {
                t |> success(!empty(res.text), "got translation ({res.text})")
            }
        }
    }
}
