options gen2
options rtti  // required so sprint_json/sscan_json honor @optional / @rename
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost public
require openai/openai_images
require strings

// Fixture-only: no local backend implements /images/generations.

[test]
def test_image_request_serialization(t : T?) {
    t |> run("image request includes prompt, omits unset optionals") @(t : T?) {
        var req : ImageRequest
        req.prompt = "a red cube"
        req.model = "dall-e-3"
        req.size = "1024x1024"
        let js = sprint_json(req, false)
        t |> success(find(js, "\"prompt\":\"a red cube\"") >= 0, "prompt present")
        t |> success(find(js, "\"model\":\"dall-e-3\"") >= 0, "model present")
        t |> success(find(js, "\"size\":\"1024x1024\"") >= 0, "size present")
        t |> success(find(js, "\"quality\"") == -1, "unset quality omitted")
    }
}

[test]
def test_image_response_decode(t : T?) {
    t |> run("image response round-trips through sprint_json/sscan_json") @(t : T?) {
        var resp : ImageResponse
        resp.created = 1700000000
        resp.data <- [ImageData(url = "https://example.com/img.png", revised_prompt = "a bright red cube")]
        let js = sprint_json(resp, false)
        var back : ImageResponse
        t |> success(sscan_json(js, back), "decode ok")
        t |> equal(length(back.data), 1)
        t |> equal(back.data[0].url, "https://example.com/img.png")
        t |> equal(back.data[0].revised_prompt, "a bright red cube")
    }
}
