options gen2
options rtti  // required so sprint_json/sscan_json honor @optional / @rename
require openai/openai_chat

[export]
def main {
    let model = "hf.co/LiquidAI/LFM2.5-1.2B-Instruct-GGUF:Q4_K_M"
    let client = openai_client("http://localhost:11434/v1")

    print("--- chat_text ---\n")
    let reply = chat_text(client, model, "Say hello in exactly one short sentence.")
    print("reply: {reply}\n")

    print("--- chat (full result) ---\n")
    let req = ChatCompletionRequest(model = model,
        messages <- [ChatMessage(role = "user", content = "What is 2+2? Reply with just the number.")])
    let res = chat(client, req)
    if (res.ok) {
        print("answer: {res.response.choices[0].message.content}\n")
        print("model:  {res.response.model}\n")
        print("tokens: {res.response.usage.total_tokens}\n")
    } else {
        print("error [{res.error.kind}/{res.error.status}] {res.error.message}\n")
    }
}
