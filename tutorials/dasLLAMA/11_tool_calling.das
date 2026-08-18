options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dasllama/dasllama
require daslib/jobque_boost
require daslib/fio
require strings

// Tutorial dasLLAMA-11: Tool Calling
//
// This tutorial covers:
//   - Declaring tools: set_tools with verbatim OpenAI tools[] JSON
//   - The model calls a tool: parse_calls on the reply
//   - Answering the call: add_tool_results + the second respond
//   - The per-family wire formats, and replaying a call turn
//
// Prerequisites: tutorials 02 and 10; a tool-capable GGUF (Qwen3-0.6B-Q8_0 works:
// https://huggingface.co/Qwen/Qwen3-0.6B-GGUF).
//
// Run: daslang.exe -jit tutorials/dasLLAMA/11_tool_calling.das -- <path/to/model.gguf>

let SYSTEM = "You are a helpful assistant."

// One tool, declared exactly as the OpenAI wire spells it — dasLLAMA passes these JSON
// objects through verbatim and renders them in the model family's own defs format.
let WEATHER_TOOL = ("\{\"type\": \"function\", \"function\": \{\"name\": \"get_weather\", " +
    "\"description\": \"Get the current weather in a city\", " +
    "\"parameters\": \{\"type\": \"object\", \"properties\": \{\"city\": \{\"type\": \"string\"}}, " +
    "\"required\": [\"city\"]}}}")

// Resolve the model path: last command-line argument ending in .gguf, else $DASLLAMA_MODEL.
def model_path() : string {
    let cmd <- get_command_line_arguments()
    if (!empty(cmd) && ends_with(cmd[length(cmd) - 1], ".gguf")) {
        return cmd[length(cmd) - 1]
    }
    return get_env_variable("DASLLAMA_MODEL")
}

// The host side of the tool: a real program would dispatch on c.name and parse c.args
// (always normalized JSON object text). Results are strings — JSON passes through cleanly.
def run_tool(c : ToolCall) : string {
    if (c.name == "get_weather") {
        return "\{\"temp_c\": 21, \"sky\": \"clear\"}"
    }
    return "\{\"error\": \"unknown tool\"}"
}

// Stream a reply to stdout and return it.
def ask_streaming(m : Model; var chat : ChatSession) : string {
    let reply = respond(m, chat, SamplingParams()) $(piece) {
        fprint(fstdout(), piece)
        fflush(fstdout())
        return true
    }
    print("\n")
    return reply
}

// ──────────────────────────────────────────────────────────────────────────
// Section 1 — Declare the tools
// ──────────────────────────────────────────────────────────────────────────
//
// set_tools takes the conversation's tools BEFORE the first turn renders — the
// system turn then carries the family's tool block (hermes puts JSON defs in
// the system prose; other families have their own framing). A family with no
// tool format ignores the declaration. render_turn shows the exact result.

def section_declare(m : Model; var chat : ChatSession) {
    var tools <- [WEATHER_TOOL]
    set_tools(chat, tools)
    add_user(chat, "What is the weather in Paris right now?")
    let turn <- render_turn(m, chat)   // read-only: the message stays queued
    print("first turn prefill: {length(turn)} tokens; the decoded text carries the tool defs:\n")
    print("{decode(m, turn)}\n")
}

// ──────────────────────────────────────────────────────────────────────────
// Section 2 — The model calls the tool; Section 3 — answer the call
// ──────────────────────────────────────────────────────────────────────────
//
// The reply to a tool-needing question is a CALL, spelled in the family's wire
// format (hermes: <tool_call>{"name":...,"arguments":...}</tool_call>).
// parse_calls splits any complete reply into (reasoning, content, calls) per
// that format — arguments come back normalized to JSON object text.
//
// Tool results are the reply TO the model: queue them with add_tool_results in
// place of add_user — one result per call, in call order — and respond again.
// The model reads the results and answers the original question.

def section_call_and_answer(m : Model; var chat : ChatSession) {
    print("model: ")
    let reply = ask_streaming(m, chat)
    var tr <- parse_calls(chat, reply)
    if (empty(tr.calls)) {
        print("(the model answered directly — tiny models sometimes skip the call; try again or use a bigger one)\n")
        return
    }
    var results : array<string>
    results |> reserve(length(tr.calls))
    for (c in tr.calls) {
        let out = run_tool(c)
        print("call: {c.name}({c.args}) -> {out}\n")
        results |> push(out)
    }
    add_tool_results(chat, results)
    print("model, with the result: ")
    ask_streaming(m, chat)
    // Out-of-order or id-keyed hosts use add_tool_results(chat, results, names) — the
    // name-spelling families (harmony, gemma-4) then pair results correctly.

    // A stateless server replays a past call turn without running the model:
    // render_assistant_calls appends the exact tokens that assistant turn prefilled —
    // its text plus the verbatim call objects — onto a renderer chat (tutorial 02's
    // render_assistant, for a turn that called tools).
    var replay = create_chat_renderer(m, "")
    var replay_tools <- [WEATHER_TOOL]
    set_tools(replay, replay_tools)
    add_user(replay, "What is the weather in Paris right now?")
    let call_objs <- [for (c in tr.calls); "\{\"name\":\"{c.name}\",\"arguments\":{c.args}}"]
    var toks : array<int64>
    render_assistant_calls(m, replay, tr.content, call_objs, toks)
    print("replayed call turn = {length(toks)} tokens, no model run\n")
}

[export]
def main() {
    let path = model_path()
    if (empty(path)) {
        print("usage: daslang -jit tutorials/dasLLAMA/11_tool_calling.das -- <model.gguf>\n")
        return
    }
    var m <- load_model(path, QuantMode.q8)
    allow_cpu_prefill()   // a CPU tutorial, said explicitly — on a Mac, set_metal_mode(MetalMode.when_available) picks the GPU rails instead
    with_job_que() {
        setup_dasllama_jobque()
        var chat = create_chat(m, SYSTEM, 512l)
        set_thinking(chat, false)   // keep the wire clean here; tutorial 10 is the thinking story
        section_declare(m, chat)
        section_call_and_answer(m, chat)
    }
}
