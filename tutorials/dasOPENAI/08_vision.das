// Tutorial OPENAI-08: Vision (image input)
//
// This tutorial covers multimodal chat — sending an image alongside text:
//   - chat_vision: one user turn with text + an image URL
//   - The multimodal request body (an array of text / image_url parts)
//   - Reading the assistant's reply (same ChatResult shape as a text chat)
//
// Prerequisites: OPENAI-01 (chat basics)
//
// Every dasOPENAI script needs `options rtti` — see OPENAI-01.
//
// The image can be an http(s) URL or a `data:` URL (base64-inlined bytes). To
// run against a real vision-capable backend, point base_url at it; Ollama with a
// vision model (e.g. `ollama run llava`) serves the same /chat/completions shape.
//
// Run: daslang.exe tutorials/dasOPENAI/08_vision.das

options gen2
options rtti
options persistent_heap
options gc

require openai/openai_vision
require tutorial_openai_server

let SERVER_PORT = 18191

// ──────────────────────────────────────────────────────────────────────────
// Section 1 — chat_vision
// ──────────────────────────────────────────────────────────────────────────
//
// chat_vision sends a single user turn (text + one image) and returns the
// normal ChatResult — the assistant's text is in choices[0].message.content.

def example_vision(base_url : string) {
    let client = openai_client(base_url)
    let res = chat_vision(client, "mock-model", "What is in this image?",
        "https://example.com/cat.png", [max_tokens = 100])
    assert(res.ok, "vision request failed")
    assert(!empty(res.response.choices), "no choices returned")
    print("vision reply: {res.response.choices[0].message.content}\n")
}

// ──────────────────────────────────────────────────────────────────────────
// Section 2 — The multimodal request shape
// ──────────────────────────────────────────────────────────────────────────
//
// A vision message can't use the plain `content : string` shape — its content
// is an ARRAY of parts (a text part and an image_url part). vision_request_body
// builds that JSON; printing it shows the wire format.

def example_request_shape() {
    let body = vision_request_body("gpt-4o", "Describe this", "https://example.com/cat.png")
    print("request body:\n{body}\n")
}

[export]
def main() {
    with_openai_server(SERVER_PORT) $(base_url) {
        print("=== Section 1: chat_vision ===\n")
        example_vision(base_url)

        print("\n=== Section 2: request shape ===\n")
        example_request_shape()
    }
}
