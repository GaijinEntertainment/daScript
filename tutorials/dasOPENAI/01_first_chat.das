// Tutorial OPENAI-01: Your First Chat
//
// This tutorial covers:
//   - Constructing an OpenAIClient (base_url, optional API key)
//   - The one-liner chat_text helper
//   - The full chat() call: request, ChatResult, choices, usage
//   - Handling OpenAIError
//
// Prerequisites: none (the tutorial runs against a local mock server)
//
// Every dasOPENAI script needs `options rtti` — without it the @optional /
// @rename field annotations are ignored and requests serialize every field,
// which real OpenAI-compatible servers reject.
//
// To talk to a real server instead of the mock, point base_url at it and pass
// the key, e.g.:
//   let client = openai_client("https://api.openai.com/v1", get_env("OPENAI_API_KEY"))
//
// Run: daslang.exe tutorials/dasOPENAI/01_first_chat.das

options gen2
options rtti
options persistent_heap
options gc

require openai/openai_chat
require tutorial_openai_server

let SERVER_PORT = 18190

// ──────────────────────────────────────────────────────────────────────────
// Section 1 — The chat_text one-liner
// ──────────────────────────────────────────────────────────────────────────
//
// chat_text is the simplest entry point: one user turn in, the assistant's
// text out (empty string on error). Good for quick prompts and scripts.

def example_chat_text(base_url : string) {
    let client = openai_client(base_url)
    let reply = chat_text(client, "mock-model", "Say hello in one short sentence.")
    print("chat_text -> {reply}\n")
}

// ──────────────────────────────────────────────────────────────────────────
// Section 2 — The full chat() call
// ──────────────────────────────────────────────────────────────────────────
//
// chat() takes a ChatCompletionRequest and returns a ChatResult. When ok is
// true, response holds the decoded ChatCompletionResponse — choices, the
// assistant message, and token usage. Otherwise inspect error.

def example_chat_full(base_url : string) {
    let client = openai_client(base_url)
    let req = ChatCompletionRequest(model = "mock-model",
        messages <- [ChatMessage(role = "user", content = "What is 2+2?")])
    let res = chat(client, req)
    if (res.ok) {
        print("answer: {res.response.choices[0].message.content}\n")
        print("model:  {res.response.model}\n")
        print("tokens: {res.response.usage.total_tokens}\n")
    } else {
        print("error [{res.error.kind}/{res.error.status}]: {res.error.message}\n")
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Section 3 — Handling errors
// ──────────────────────────────────────────────────────────────────────────
//
// A client pointed at an unreachable server produces a transport error rather
// than crashing — ok is false and error.kind is "transport". The same shape
// surfaces HTTP ("http"), API ("api"), and decode ("decode") failures.

def example_error_handling() {
    let client = openai_client("http://127.0.0.1:1/v1")   // nothing listens here
    let reply = chat_text(client, "mock-model", "this will fail")
    print("unreachable server -> reply is empty: {empty(reply)}\n")
}

[export]
def main() {
    with_openai_server(SERVER_PORT) $(base_url) {
        print("=== chat_text ===\n")
        example_chat_text(base_url)

        print("\n=== chat (full result) ===\n")
        example_chat_full(base_url)

        print("\n=== error handling ===\n")
        example_error_handling()
    }
}
