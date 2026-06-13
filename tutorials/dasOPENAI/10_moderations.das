// Tutorial OPENAI-10: Moderations
//
// This tutorial covers the content-moderation endpoint:
//   - Building a ModerationRequest (model + an array of input strings)
//   - moderations and the ModerationResult / per-input verdicts
//   - Reading the `flagged` boolean and the per-category score tables
//
// Prerequisites: OPENAI-01 (client + result handling)
//
// Every dasOPENAI script needs `options rtti` — see OPENAI-01. The mock flags any
// input containing the sentinel phrase "flagged-example" so this runs offline;
// against a real backend the model decides.
//
// Run: daslang.exe tutorials/dasOPENAI/10_moderations.das

options gen2
options rtti
options persistent_heap
options gc

require openai/openai_moderations
require tutorial_openai_server

let SERVER_PORT = 18193

// Classify one string and return whether it was flagged.
def classify(client : OpenAIClient; text : string) : bool {
    let req = ModerationRequest(model = "text-moderation-latest", input <- [text])
    let res = moderations(client, req)
    assert(res.ok, "moderation request failed")
    assert(!empty(res.response.results), "no results returned")
    // `categories` / `category_scores` are tables keyed by category name, so use
    // safe lookup (?[] ?? default) rather than assuming a key is present.
    var flagged = false
    for (verdict in res.response.results) {
        flagged = verdict.flagged
        let violence = verdict.category_scores?["violence"] ?? 0.0lf
        print("  \"{text}\"\n    flagged={verdict.flagged}, violence score={violence}\n")
    }
    return flagged
}

[export]
def main() {
    with_openai_server(SERVER_PORT) $(base_url) {
        let client = openai_client(base_url)
        print("=== Moderations ===\n")
        let clean_flagged = classify(client, "I love sunny days at the park.")
        let bad_flagged = classify(client, "flagged-example: a violent threat")
        assert(!clean_flagged, "benign text should not be flagged")
        assert(bad_flagged, "sentinel text should be flagged")
    }
}
