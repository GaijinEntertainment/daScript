options gen2
options rtti  // required so sprint_json/sscan_json honor @optional / @rename
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost public
require openai/openai_moderations
require strings

// Fixture-only: no local backend implements /moderations (OpenAI cloud only).

[test]
def test_moderation_request_serialization(t : T?) {
    t |> run("moderation request serializes input array") @(t : T?) {
        var req : ModerationRequest
        req.model = "omni-moderation-latest"
        req.input <- ["some text"]
        let js = sprint_json(req, false)
        t |> success(find(js, "\"model\":\"omni-moderation-latest\"") >= 0, "model present")
        t |> success(find(js, "\"input\":[\"some text\"]") >= 0, "input array present")
    }
}

[test]
def test_moderation_response_decode(t : T?) {
    t |> run("moderation response round-trips with table-keyed categories") @(t : T?) {
        var resp : ModerationResponse
        resp.id = "modr-1"
        resp.model = "omni-moderation-latest"
        var r : ModerationCategoryResult
        r.flagged = true
        r.categories["violence"] = false
        r.categories["hate"] = true
        r.category_scores["violence"] = 0.01lf
        r.category_scores["hate"] = 0.93lf
        resp.results |> emplace(r)

        let js = sprint_json(resp, false)
        var back : ModerationResponse
        t |> success(sscan_json(js, back), "decode ok")
        t |> equal(length(back.results), 1)
        t |> success(back.results[0].flagged, "flagged is true")
        t |> success(back.results[0].categories["hate"], "category hate decoded true")
        t |> success(!back.results[0].categories["violence"], "category violence decoded false")
        t |> success(back.results[0].category_scores["hate"] > 0.9lf, "hate score decoded")
    }
}
