// Tutorial OPENAI-07: Streaming Chat
//
// This tutorial covers:
//   - Streaming a chat response with chat_stream
//   - The on_delta block: handling text increments as they arrive
//   - The accumulated content and finish_reason in ChatStreamResult
//
// Prerequisites: Tutorial OPENAI-01, OPENAI-02
//
// Streaming delivers the assistant's reply token-by-token over Server-Sent
// Events instead of waiting for the whole response. chat_stream invokes your
// on_delta block for each text increment and also accumulates the full text.
//
// Scope: chat_stream accumulates text deltas (choices[0].delta.content) only.
// Streamed tool/function calls are a separate, later feature — for tool calls
// use the non-streaming chat() from Tutorial OPENAI-04.
//
// Run: daslang.exe tutorials/dasOPENAI/07_streaming_chat.das

options gen2
options rtti
options persistent_heap
options gc

require openai/openai_chat
require tutorial_openai_server

let SERVER_PORT = 18196

// ──────────────────────────────────────────────────────────────────────────
// Section 1 — Streaming with on_delta
// ──────────────────────────────────────────────────────────────────────────
//
// chat_stream sets stream=true for you. Each text increment is passed to the
// on_delta block (here we print it immediately, so the reply appears as it
// streams). The return value carries the full accumulated content.

def example_stream(base_url : string) {
    let client = openai_client(base_url)
    var req = ChatCompletionRequest(model = "mock-model",
        messages <- [ChatMessage(role = "user", content = "Stream a short greeting.")])

    print("live: ")
    let result = chat_stream(client, req) $(delta : string) {
        print(delta)   // each increment, as it arrives
    }
    print("\n")

    if (result.ok) {
        print("accumulated: {result.content}\n")
        print("finish_reason: {result.finish_reason}\n")
    } else {
        print("error [{result.error.kind}/{result.error.status}]: {result.error.message}\n")
    }
}

[export]
def main() {
    with_openai_server(SERVER_PORT) $(base_url) {
        print("=== streaming chat ===\n")
        example_stream(base_url)
    }
}
