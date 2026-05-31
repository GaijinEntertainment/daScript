// Tutorial OPENAI-03: Structured Outputs (JSON mode)
//
// This tutorial covers:
//   - Asking the model for JSON with json_object_format()
//   - Setting req.response_format
//   - Decoding the returned JSON content into a typed struct with sscan_json
//
// Prerequisites: Tutorial OPENAI-01, OPENAI-02
//
// JSON mode tells the server to emit valid JSON as the message content. You
// then decode that content string into your own struct — the same sscan_json
// machinery the client uses internally.
//
// Run: daslang.exe tutorials/dasOPENAI/03_structured_outputs.das

options gen2
options rtti
options persistent_heap
options gc

require openai/openai_chat
require tutorial_openai_server

let SERVER_PORT = 18192

// The shape we expect the model to return. With `options rtti`, sscan_json
// maps JSON object keys onto these fields.
struct Answer {
    answer : int
}

// ──────────────────────────────────────────────────────────────────────────
// Section 1 — Requesting JSON
// ──────────────────────────────────────────────────────────────────────────
//
// json_object_format() returns the response_format value for JSON mode.
// Assign it to the request's response_format field. (The field is @embed, so
// the helper hands you the exact JSON the server expects.)

def example_json_mode(base_url : string) {
    let client = openai_client(base_url)
    var req = ChatCompletionRequest(model = "mock-model",
        messages <- [ChatMessage(role = "user",
            content = "Return the answer to 6*7 as JSON: \{\"answer\": N\}.")])
    req.response_format = json_object_format()

    let res = chat(client, req)
    if (!res.ok) {
        print("error: {res.error.message}\n")
        return
    }
    let content = res.response.choices[0].message.content
    print("raw JSON content: {content}\n")

    // ── Section 2 — decode the content into our struct ──
    var parsed : Answer
    if (sscan_json(content, parsed)) {
        print("decoded answer = {parsed.answer}\n")
    } else {
        print("could not decode content as Answer\n")
    }
}

[export]
def main() {
    with_openai_server(SERVER_PORT) $(base_url) {
        print("=== JSON mode ===\n")
        example_json_mode(base_url)
    }
}
