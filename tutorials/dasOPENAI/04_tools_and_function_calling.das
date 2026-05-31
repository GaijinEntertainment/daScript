// Tutorial OPENAI-04: Tools and Function Calling
//
// This tutorial covers:
//   - Declaring a Tool (a function the model may call)
//   - Sending tools with a chat request
//   - Reading back the model's ToolCall (name + JSON arguments)
//   - Completing the round trip: feed the tool result back as a "tool" message
//
// Prerequisites: Tutorial OPENAI-01, OPENAI-02
//
// Function calling lets the model ask your program to run a function. The model
// does not run anything itself — it returns a tool_calls request, you execute
// it, and you send the result back so the model can use it.
//
// Run: daslang.exe tutorials/dasOPENAI/04_tools_and_function_calling.das

options gen2
options rtti
options persistent_heap
options gc

require openai/openai_chat
require tutorial_openai_server

let SERVER_PORT = 18193

// A weather tool. `parameters` is a raw JSON-schema string describing the
// function's arguments — the model fills it in when it decides to call.
def weather_tool() : Tool {
    let params = ("\{\"type\":\"object\"," +
        "\"properties\":\{\"location\":\{\"type\":\"string\"\}\}," +
        "\"required\":[\"location\"]\}")
    return Tool(_type = "function",
        _function = FunctionDef(name = "get_weather",
            description = "Get the current weather for a location",
            parameters = params))
}

[export]
def main() {
    with_openai_server(SERVER_PORT) $(base_url) {
        let client = openai_client(base_url)

        // ── Section 1 — send the tools, read the tool call ──
        print("=== model requests a tool call ===\n")
        let req = ChatCompletionRequest(model = "mock-model",
            messages <- [ChatMessage(role = "user", content = "What's the weather in Paris?")],
            tools <- [weather_tool()])
        let res = chat(client, req)
        if (!res.ok) {
            print("error: {res.error.message}\n")
            return
        }
        // When the model wants a tool, content is empty and tool_calls is set.
        // (Some servers send content:null here — sscan_json decodes that fine.)
        if (empty(res.response.choices[0].message.tool_calls)) {
            print("model replied directly: {res.response.choices[0].message.content}\n")
            return
        }
        let tcall = res.response.choices[0].message.tool_calls[0]
        print("tool call id={tcall.id} name={tcall._function.name}\n")
        print("arguments: {tcall._function.arguments}\n")

        // ── Section 2 — run the tool, send the result back ──
        // We "execute" get_weather locally and return its result as a message
        // with role "tool", tagged with the tool_call_id the model gave us.
        print("\n=== send the tool result back ===\n")
        let req2 = ChatCompletionRequest(model = "mock-model", messages <- [
            ChatMessage(role = "user", content = "What's the weather in Paris?"),
            ChatMessage(role = "assistant", content = "",
                tool_calls <- [ToolCall(id = tcall.id, _type = "function",
                    _function = ToolCallFunction(name = tcall._function.name,
                        arguments = tcall._function.arguments))]),
            ChatMessage(role = "tool", tool_call_id = tcall.id,
                content = "\{\"temp_c\":18,\"sky\":\"clear\"\}")])
        let res2 = chat(client, req2)
        if (res2.ok) {
            print("final answer: {res2.response.choices[0].message.content}\n")
        }
    }
}
