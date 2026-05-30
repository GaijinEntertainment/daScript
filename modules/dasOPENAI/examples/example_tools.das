options gen2
options rtti  // required so sprint_json/sscan_json honor @optional / @rename
require openai/openai_chat
require strings

[export]
def main {
    let client = openai_client("http://localhost:11434/v1")

    var tool : Tool
    tool._type = "function"
    tool._function.name = "get_weather"
    tool._function.description = "Get the current weather for a location"
    tool._function.parameters = write_json(%json~
        {"type":"object","properties":{"location":{"type":"string"}},"required":["location"]}
    %%)

    let req = ChatCompletionRequest(model = "qwen2.5-coder:7b",
        messages <- [ChatMessage(role = "user", content = "What is the weather in Paris?")],
        tools <- [tool], tool_choice = "auto")

    let res = chat(client, req)
    if (res.ok && !empty(res.response.choices)) {
        let n = length(res.response.choices[0].message.tool_calls)
        print("tool calls: {n}\n")
        if (n > 0) {
            print("  -> {res.response.choices[0].message.tool_calls[0]._function.name}" +
                "({res.response.choices[0].message.tool_calls[0]._function.arguments})\n")
        } else {
            print("  (model replied with text instead)\n")
        }
    } else {
        print("error [{res.error.kind}] {res.error.message}\n")
    }
}
