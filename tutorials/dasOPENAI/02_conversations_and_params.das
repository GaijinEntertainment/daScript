// Tutorial OPENAI-02: Conversations and Parameters
//
// This tutorial covers:
//   - Multi-turn conversations: system / user / assistant message roles
//   - Optional request parameters (temperature, max_tokens, seed, stop)
//   - Reading token Usage from the response
//
// Prerequisites: Tutorial OPENAI-01 (client construction, chat())
//
// Run: daslang.exe tutorials/dasOPENAI/02_conversations_and_params.das

options gen2
options rtti
options persistent_heap
options gc

require openai/openai_chat
require tutorial_openai_server

let SERVER_PORT = 18191

// ──────────────────────────────────────────────────────────────────────────
// Section 1 — Multi-turn conversations
// ──────────────────────────────────────────────────────────────────────────
//
// A chat request carries the whole conversation as an ordered array of
// messages. The model sees all prior turns. Roles:
//   system    — instructions that set behavior
//   user      — what the human said
//   assistant — what the model previously replied
//   tool      — a tool/function result (see Tutorial OPENAI-04)

def example_conversation(base_url : string) {
    let client = openai_client(base_url)
    let req = ChatCompletionRequest(model = "mock-model", messages <- [
        ChatMessage(role = "system", content = "You are a terse assistant."),
        ChatMessage(role = "user", content = "Remember the number 7."),
        ChatMessage(role = "assistant", content = "Got it — 7."),
        ChatMessage(role = "user", content = "What number did I ask you to remember?")])
    let res = chat(client, req)
    if (res.ok) {
        print("assistant: {res.response.choices[0].message.content}\n")
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Section 2 — Optional parameters
// ──────────────────────────────────────────────────────────────────────────
//
// ChatCompletionRequest exposes the common knobs as @optional fields. Because
// `options rtti` is set, default-valued optionals are omitted from the wire —
// only the ones you set are sent. Set them with named arguments.

def example_parameters(base_url : string) {
    let client = openai_client(base_url)
    let req = ChatCompletionRequest(model = "mock-model",
        messages <- [ChatMessage(role = "user", content = "Write one word.")],
        temperature = 0.2,
        max_tokens = 16,
        seed = 42,
        stop <- ["\n\n"])
    let res = chat(client, req)
    if (res.ok) {
        print("reply: {res.response.choices[0].message.content}\n")
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Section 3 — Token usage
// ──────────────────────────────────────────────────────────────────────────
//
// Most responses include a Usage breakdown — prompt, completion, and total
// tokens. Useful for cost tracking and staying within context limits.

def example_usage(base_url : string) {
    let client = openai_client(base_url)
    let req = ChatCompletionRequest(model = "mock-model",
        messages <- [ChatMessage(role = "user", content = "Hello.")])
    let res = chat(client, req)
    if (res.ok) {
        let u = res.response.usage
        print("prompt={u.prompt_tokens} completion={u.completion_tokens} total={u.total_tokens}\n")
    }
}

[export]
def main() {
    with_openai_server(SERVER_PORT) $(base_url) {
        print("=== multi-turn conversation ===\n")
        example_conversation(base_url)

        print("\n=== optional parameters ===\n")
        example_parameters(base_url)

        print("\n=== token usage ===\n")
        example_usage(base_url)
    }
}
