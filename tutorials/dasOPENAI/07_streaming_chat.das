// Tutorial OPENAI-07: Streaming Chat
//
// This tutorial covers:
//   - Streaming a chat response with chat_stream
//   - The on_delta block: handling text increments as they arrive
//   - The accumulated content and finish_reason in ChatStreamResult
//   - Reassembled streamed tool calls (result.tool_calls)
//
// Prerequisites: Tutorial OPENAI-01, OPENAI-02, OPENAI-04
//
// Streaming delivers the assistant's reply token-by-token over Server-Sent
// Events instead of waiting for the whole response. chat_stream invokes your
// on_delta block for each text increment and also accumulates the full text.
//
// It also reassembles streamed tool-call fragments (by index) into
// result.tool_calls — see Section 2 — so you can stream tool-calling models too.
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

// ──────────────────────────────────────────────────────────────────────────
// Section 2 — Streamed tool calls
// ──────────────────────────────────────────────────────────────────────────
//
// When a model streams a tool call, the id and function name arrive in the
// first frame and the JSON arguments accrete across later frames. chat_stream
// reassembles them by index into result.tool_calls. on_delta fires for text
// only, so a pure tool-call stream produces no text — the result is in
// result.tool_calls and finish_reason == "tool_calls".

def example_stream_tool_calls(base_url : string) {
    let client = openai_client(base_url)
    var req = ChatCompletionRequest(model = "mock-model",
        messages <- [ChatMessage(role = "user", content = "What's the weather in Paris?")],
        tools <- [Tool(_type = "function",
            _function = FunctionDef(name = "get_weather", description = "Look up the weather for a location."))])

    let result = chat_stream(client, req) $(delta : string) {
        print(delta)   // no text arrives for a pure tool-call stream
    }

    if (!result.ok) {
        print("error [{result.error.kind}/{result.error.status}]: {result.error.message}\n")
        return
    }
    print("finish_reason: {result.finish_reason}\n")
    for (tc in result.tool_calls) {
        print("tool call: {tc._function.name}({tc._function.arguments}) [id={tc.id}]\n")
    }
}

[export]
def main() {
    with_openai_server(SERVER_PORT) $(base_url) {
        print("=== streaming chat ===\n")
        example_stream(base_url)
        print("\n=== streaming tool calls ===\n")
        example_stream_tool_calls(base_url)
    }
}
