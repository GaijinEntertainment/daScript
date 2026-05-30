options gen2
options rtti  // required so sprint_json/sscan_json honor @optional / @rename
require openai/openai_chat

[export]
def main {
    let client = openai_client("http://localhost:11434/v1")
    let req = ChatCompletionRequest(model = "hf.co/LiquidAI/LFM2.5-1.2B-Instruct-GGUF:Q4_K_M",
        messages <- [ChatMessage(role = "user",
            content = "Give me a JSON object with name and city for a fictional person.")],
        response_format = json_object_format(), max_tokens = 128)
    let res = chat(client, req)
    if (res.ok && !empty(res.response.choices)) {
        print("json reply: {res.response.choices[0].message.content}\n")
    } else {
        print("error [{res.error.kind}] {res.error.message}\n")
    }
}
