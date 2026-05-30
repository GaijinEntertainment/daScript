options gen2
options rtti  // required so sprint_json/sscan_json honor @optional / @rename
require openai/openai_chat

[export]
def main {
    let model = "hf.co/LiquidAI/LFM2.5-1.2B-Instruct-GGUF:Q4_K_M"
    let client = openai_client("http://localhost:11434/v1")

    var req = ChatCompletionRequest(model = model,
        messages <- [ChatMessage(role = "user", content = "Count from 1 to 5, one number per line.")])

    print("streaming: ")
    let res = chat_stream(client, req) $(delta : string) {
        print(delta)
    }
    print("\n--- done: ok={res.ok} finish={res.finish_reason} ---\n")
}
