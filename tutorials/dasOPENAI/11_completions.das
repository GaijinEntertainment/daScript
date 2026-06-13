// Tutorial OPENAI-11: Legacy Completions
//
// This tutorial covers the older (pre-chat) text-completion endpoint:
//   - Building a CompletionRequest (model + prompt + sampling options)
//   - completions and the CompletionResult / CompletionResponse shape
//   - The complete() convenience wrapper (prompt in, text out)
//
// Prefer chat (OPENAI-01) for new code; this endpoint exists for older models
// and OpenAI-compatible backends that only expose /completions.
//
// Prerequisites: OPENAI-01 (client + result handling)
//
// Every dasOPENAI script needs `options rtti` — see OPENAI-01.
//
// Run: daslang.exe tutorials/dasOPENAI/11_completions.das

options gen2
options rtti
options persistent_heap
options gc

require openai/openai_completions
require tutorial_openai_server

let SERVER_PORT = 18200

[export]
def main() {
    with_openai_server(SERVER_PORT) $(base_url) {
        let client = openai_client(base_url)

        // ──────────────────────────────────────────────────────────────────
        // Section 1 — The full completions() call
        // ──────────────────────────────────────────────────────────────────
        //
        // completions() takes a CompletionRequest and returns a CompletionResult.
        // On success, response.choices holds the generated text and finish_reason.

        print("=== Section 1: completions ===\n")
        let req = CompletionRequest(model = "mock-model",
            prompt = "The capital of France is", max_tokens = 16)
        let res = completions(client, req)
        if (!res.ok) {
            print("error [{res.error.kind}/{res.error.status}]: {res.error.message}\n")
        }
        assert(res.ok, "completion failed")
        assert(!empty(res.response.choices), "no choices returned")
        print("text:          {res.response.choices[0].text}\n")
        print("finish_reason: {res.response.choices[0].finish_reason}\n")
        print("tokens:        {res.response.usage.total_tokens}\n")

        // ──────────────────────────────────────────────────────────────────
        // Section 2 — The complete() convenience wrapper
        // ──────────────────────────────────────────────────────────────────
        //
        // complete() is a one-liner: prompt in, text out (empty string on error).

        print("\n=== Section 2: complete() ===\n")
        let text = complete(client, "mock-model", "Once upon a time")
        print("complete(): {text}\n")
        assert(!empty(text), "complete() returned empty text")
    }
}
