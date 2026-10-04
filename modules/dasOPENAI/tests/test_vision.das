options gen2
options rtti  // required so sprint_json/sscan_json honor @optional / @rename
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost public
require _openai_live
require openai/openai_vision
require strings
require daslib/fio

// 1x1 PNG data URL — enough to exercise the multimodal request plumbing.
let PIXEL_PNG = "data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAYAAAAfFcSJAAAADUlEQVR42mP8z8BQDwAEhQGAhKmMIQAAAABJRU5ErkJggg=="

[test]
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
    }
}

[test]
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
