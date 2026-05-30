options gen2
options rtti  // required so sprint_json/sscan_json honor @optional / @rename
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost public
require openai/openai_audio
require strings

// ===== Unit: serialization (no backend needed) =====

[test]
def test_speech_request_serialization(t : T?) {
    t |> run("speech request includes fields, omits unset speed") @(t : T?) {
        let req = SpeechRequest(model = "kokoro", input = "hi", voice = "af_heart", response_format = "wav")
        let js = sprint_json(req, false)
        t |> success(find(js, "\"model\":\"kokoro\"") >= 0, "model present")
        t |> success(find(js, "\"input\":\"hi\"") >= 0, "input present")
        t |> success(find(js, "\"voice\":\"af_heart\"") >= 0, "voice present")
        t |> success(find(js, "\"response_format\":\"wav\"") >= 0, "format present")
        t |> success(find(js, "\"speed\"") == -1, "unset speed omitted")
    }
}

// ===== Live integration (requires the Kokoro TTS server on :8880) =====

[test]
def test_speech_live_kokoro(t : T?) {
    t |> run("live TTS against the Kokoro server") @(t : T?) {
        let client = openai_client("http://localhost:8880/v1")
        let req = SpeechRequest(model = "kokoro", input = "The quick brown fox.", voice = "af_heart", response_format = "wav")
        let res <- speech(client, req)
        t |> success(res.ok, "speech ok (kind={res.error.kind} status={res.error.status} {res.error.message})")
        if (res.ok) {
            t |> success(length(res.audio) > 1000, "got audio bytes ({length(res.audio)})")
            if (length(res.audio) >= 4) {
                let riff = (res.audio[0] == uint8(82) && res.audio[1] == uint8(73) &&
                    res.audio[2] == uint8(70) && res.audio[3] == uint8(70))
                t |> success(riff, "WAV RIFF header present")
            }
        }
    }
}
