options gen2
options rtti  // required so sprint_json/sscan_json honor @optional / @rename
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost public
require _openai_live
require openai/openai_audio
require openai/openai_images
require openai/openai_vision
require strings
require daslib/fio

//! The media endpoints: image generation, speech, transcription and
//! vision. No local backend serves /images/generations, so it is
//! checked offline only; speech and transcription run live against
//! the Kokoro+Whisper server on :8880.

//! 1x1 PNG data URL - enough to exercise the multimodal request
//! plumbing.
let PIXEL_PNG = "data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAYAAAAfFcSJAAAADUlEQVR42mP8z8BQDwAEhQGAhKmMIQAAAABJRU5ErkJggg=="

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

[test(description = "An image request serializes the prompt, model and size and omits an unset quality.")]
def test_image_request_serialization(t : T?) {
    t |> run("image request includes prompt, omits unset optionals") @(t : T?) {
        let req = ImageRequest(prompt = "a red cube", model = "dall-e-3", size = "1024x1024")
        let js = sprint_json(req, false)
        t |> success(find(js, "\"prompt\":\"a red cube\"") >= 0, "prompt present")
        t |> success(find(js, "\"model\":\"dall-e-3\"") >= 0, "model present")
        t |> success(find(js, "\"size\":\"1024x1024\"") >= 0, "size present")
        t |> success(find(js, "\"quality\"") == -1, "unset quality omitted")
    }
}

[test(description = "An image response round-trips through sprint_json and sscan_json with each image's url and revised prompt intact.")]
def test_image_response_decode(t : T?) {
    t |> run("image response round-trips through sprint_json/sscan_json") @(t : T?) {
        let resp = ImageResponse(
            created = 1700000000,
            data <- [ImageData(url = "https://example.com/img.png", revised_prompt = "a bright red cube")])
        let js = sprint_json(resp, false)
        var back : ImageResponse
        t |> success(sscan_json(js, back), "decode ok")
        t |> equal(length(back.data), 1)
        t |> equal(back.data[0].url, "https://example.com/img.png")
        t |> equal(back.data[0].revised_prompt, "a bright red cube")
    }
}

[test(description = "A speech request serializes the model, input, voice and response format and omits an unset speed.")]
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

[test(description = "A live speech() against the Kokoro server returns more than 1000 bytes of WAV audio with a RIFF header; skipped when no server answers on :8880.", timeout = 1200)]
def test_speech_live_kokoro(t : T?) {
    t |> run("live TTS against the Kokoro server") @(t : T?) {
        return if (!live_server(t, "http://localhost:8880/v1"))
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

[test(description = "Speech synthesized by the local server and transcribed back by it yields a transcript that mentions the spoken word fox; skipped when no server answers on :8880.", timeout = 1200)]
def test_transcribe_live_round_trip(t : T?) {
    t |> run("TTS -> STT round trip against the local server") @(t : T?) {
        return if (!live_server(t, "http://localhost:8880/v1"))
        let client = openai_client("http://localhost:8880/v1")
        let path = path_join(test_temp_dir(), "test_stt_rt.wav")
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

[test(description = "Speech synthesized by the local server and sent to its translation endpoint yields non-empty text; skipped when no server answers on :8880.", timeout = 1200)]
def test_translate_live(t : T?) {
    t |> run("translation endpoint against the local server") @(t : T?) {
        return if (!live_server(t, "http://localhost:8880/v1"))
        let client = openai_client("http://localhost:8880/v1")
        let path = path_join(test_temp_dir(), "test_translate_rt.wav")
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

[test(description = "A vision request body carries the text and image_url parts with the text first, leads with a system message and the image or audio clip only when asked, and sends both no-thinking spellings only under no_think.")]
def test_vision_body_serialization(t : T?) {
    t |> run("vision request body carries text + image_url parts") @(t : T?) {
        let body = vision_request_body("m", "What is this?", "https://example.com/x.png")
        t |> success(find(body, "image_url") >= 0, "image_url part present")
        t |> success(find(body, "What is this?") >= 0, "text part present")
        t |> success(find(body, "https://example.com/x.png") >= 0, "image url present")
        t |> success(find(body, "\"system\"") < 0, "no system message unless one is given")
        let under = vision_request_body("m", "What is this?", "https://example.com/x.png", 8, "Be brief.")
        t |> success(find(under, "\"role\": \"system\"") >= 0 && find(under, "Be brief.") < find(under, "What is this?"), "the system message leads: {under}")
        t |> success(find(body, "What is this?") < find(body, "image_url"), "the text leads the image by default")
        let first = vision_request_body("m", "What is this?", "https://example.com/x.png", 8, "", true)
        t |> success(find(first, "image_url") < find(first, "What is this?"), "the image leads under image_first")
        let heard = audio_request_body("m", "What is said?", "QUJD", 8, "", true)
        t |> success(find(heard, "input_audio") >= 0 && find(heard, "QUJD") >= 0 && find(heard, "\"format\": \"wav\"") >= 0, "an audio body carries the clip as an input_audio part: {heard}")
        t |> success(find(heard, "input_audio") < find(heard, "What is said?"), "the clip leads under audio_first")
        t |> success(find(body, "enable_thinking") < 0, "no thinking field unless asked")
        let quiet = vision_request_body("m", "What is this?", "https://example.com/x.png", 8, "", false, true)
        t |> success(find(quiet, "\"enable_thinking\": false") >= 0 && find(quiet, "chat_template_kwargs") >= 0, "no_think sends both spellings: {quiet}")
    }
}

[test(description = "A live chat_vision against OpenRouter describes a 1x1 PNG with non-empty text; skipped without OPENROUTER_API_KEY, passed on a 429 rate limit.", timeout = 1200)]
def test_vision_live_openrouter(t : T?) {
    t |> run("live vision (image input) against OpenRouter") @(t : T?) {
        return if (!live_key(t, "OPENROUTER_API_KEY"))
        let client = openai_client("https://openrouter.ai/api/v1", get_env_variable("OPENROUTER_API_KEY"))
        let res = chat_vision(client, "google/gemma-4-31b-it:free", "Describe this image in one word.", PIXEL_PNG)
        if (!res.ok && res.error.status == 429) {
            t |> success(true, "OpenRouter free-tier rate-limited (429) - skipped")
            return
        }
        t |> success(res.ok, "vision chat ok (kind={res.error.kind} status={res.error.status} {res.error.message})")
        if (res.ok && !empty(res.response.choices)) {
            t |> success(!empty(res.response.choices[0].message.content),
                "got description ({res.response.choices[0].message.content})")
        }
    }
}
