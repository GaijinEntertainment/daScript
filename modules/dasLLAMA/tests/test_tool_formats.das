options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dastest/testing_boost public
require dasllama/dasllama_tools   // the per-ToolMode wire codecs under test — pure string+JSON, model-free
require daslib/strings_boost

// The per-family tool wire codecs against their verbatim formats (tool_formats_plan.md):
// Harmony channel-structure calls, Mistral [TOOL_CALLS] arrays, llama-3 whole-reply JSON.
// Model-free and deterministic — the live legs live in the server test file.

let WEATHER_TOOL = "\{\"type\": \"function\", \"function\": \{\"name\": \"get_weather\", \"description\": \"Get current weather\", \"parameters\": \{\"type\": \"object\", \"properties\": \{\"city\": \{\"type\": \"string\", \"description\": \"The city name\"}, \"unit\": \{\"type\": \"string\", \"enum\": [\"celsius\", \"fahrenheit\"]}}, \"required\": [\"city\"]}}}"

[test]
def test_harmony_namespace(t : T?) {
    t |> run("the developer-turn TS namespace renders canonically (trailing commas included)") @(t : T?) {
        var tools : array<string>
        tools |> push(WEATHER_TOOL)
        let got = harmony_tools_namespace(tools)
        let want = ("## functions\n\nnamespace functions \{\n\n" +
                    "// Get current weather\n" +
                    "type get_weather = (_: \{\n" +
                    "  // The city name\n" +
                    "  city: string,\n" +
                    "  unit?: \"celsius\" | \"fahrenheit\",\n" +
                    "}) => any;\n\n" +
                    "} // namespace functions")
        t |> equal(got, want)
        delete tools
    }
    t |> run("a no-parameter tool renders the empty signature") @(t : T?) {
        var tools : array<string>
        tools |> push("\{\"type\": \"function\", \"function\": \{\"name\": \"ping\"}}")
        let got = harmony_tools_namespace(tools)
        t |> success(find(got, "type ping = () => any;") >= 0, "empty-arg form")
        delete tools
    }
}

[test]
def test_harmony_parse(t : T?) {   // nolint:STYLE038 — one arm per wire shape from the PEG spec
    t |> run("analysis -> final splits into reasoning + content") @(t : T?) {
        var r <- harmony_parse("<|channel|>analysis<|message|>THINK<|end|><|start|>assistant<|channel|>final<|message|>ANSWER")
        t |> equal(r.reasoning, "THINK")
        t |> equal(r.content, "ANSWER")
        t |> equal(length(r.calls), 0)
        delete r
    }
    t |> run("channel-header recipient call after analysis") @(t : T?) {
        var r <- harmony_parse("<|channel|>analysis<|message|>T<|end|><|start|>assistant<|channel|>commentary to=functions.get_weather <|constrain|>json<|message|>\{\"city\": \"Paris\"}")
        t |> equal(r.reasoning, "T")
        t |> equal(length(r.calls), 1)
        if (length(r.calls) == 1) {
            t |> equal(r.calls[0].name, "get_weather")
            t |> equal(r.calls[0].args, "\{\"city\": \"Paris\"}")
        }
        delete r
    }
    t |> run("role-header recipient call (the reply opens with ` to=`)") @(t : T?) {
        var r <- harmony_parse(" to=functions.ping<|channel|>commentary json<|message|>\{}")
        t |> equal(length(r.calls), 1)
        if (length(r.calls) == 1) {
            t |> equal(r.calls[0].name, "ping")
            t |> equal(r.calls[0].args, "\{}")
        }
        delete r
    }
    t |> run("bare commentary is a content preamble; unknown recipients drop") @(t : T?) {
        var r <- harmony_parse("<|channel|>commentary<|message|>Preamble.<|end|><|start|>assistant<|channel|>final<|message|>Done.")
        t |> equal(r.content, "Preamble.Done.")
        delete r
        var r2 <- harmony_parse(" to=python<|channel|>commentary<|message|>import x")
        t |> equal(length(r2.calls), 0)
        t |> equal(r2.content, "")
        delete r2
    }
    t |> run("headerless text passes through as content") @(t : T?) {
        var r <- harmony_parse("bare text reply")
        t |> equal(r.content, "bare text reply")
        delete r
    }
}

[test]
def test_mistral_parse(t : T?) {
    t |> run("[TOOL_CALLS] + array with id parses; content before the marker survives") @(t : T?) {
        var r <- mistral_parse("Let me check.[TOOL_CALLS][\{\"name\": \"get_weather\", \"arguments\": \{\"city\": \"Paris\"}, \"id\": \"abc123def\"}]")
        t |> equal(r.content, "Let me check.")
        t |> equal(length(r.calls), 1)
        if (length(r.calls) == 1) {
            t |> equal(r.calls[0].name, "get_weather")
            t |> equal(r.calls[0].id, "abc123def")
            t |> success(find(r.calls[0].args, "Paris") >= 0, "args carry the city")
        }
        delete r
    }
    t |> run("id is genuinely optional and a bare object is tolerated") @(t : T?) {
        var r <- mistral_parse("[TOOL_CALLS][\{\"name\": \"f\", \"arguments\": \{}}]")
        t |> equal(length(r.calls), 1)
        t |> equal(r.calls[0].id, "")
        delete r
        var r2 <- mistral_parse("[TOOL_CALLS]\{\"name\": \"g\", \"arguments\": \{\"a\": 1}}")
        t |> equal(length(r2.calls), 1)
        t |> equal(r2.calls[0].name, "g")
        delete r2
    }
    t |> run("no marker => pure content; replay text round-trips") @(t : T?) {
        var r <- mistral_parse("just words")
        t |> equal(r.content, "just words")
        t |> equal(length(r.calls), 0)
        let txt = mistral_calls_text(r.calls)
        t |> equal(txt, "[TOOL_CALLS][]")
        delete r
        var calls : array<ToolCall>
        calls |> push(ToolCall(name = "f", args = "\{\"a\": 1}", id = "abc123def"))
        t |> equal(mistral_calls_text(calls), "[TOOL_CALLS][\{\"name\": \"f\", \"arguments\": \{\"a\": 1}, \"id\": \"abc123def\"}]")
        delete calls
    }
}

[test]
def test_llama_json_parse(t : T?) {
    t |> run("a whole-reply JSON object is a call; prose before it survives") @(t : T?) {
        var r <- llama_json_parse("\{\"name\": \"get_weather\", \"parameters\": \{\"city\": \"Paris\"}}")
        t |> equal(length(r.calls), 1)
        if (length(r.calls) == 1) {
            t |> equal(r.calls[0].name, "get_weather")
            t |> success(find(r.calls[0].args, "Paris") >= 0, "parameters become args")
        }
        t |> equal(r.content, "")
        delete r
        var r2 <- llama_json_parse("Checking. \{\"name\": \"f\", \"parameters\": \{}}")
        t |> equal(r2.content, "Checking.")
        t |> equal(length(r2.calls), 1)
        delete r2
    }
    t |> run("the tolerated type-prefix wrapper and non-call braces") @(t : T?) {
        var r <- llama_json_parse("\{\"type\": \"function\", \"name\": \"f\", \"parameters\": \{}}")
        t |> equal(length(r.calls), 1)
        t |> equal(r.calls[0].name, "f")
        delete r
        // prose containing a brace that is not a call object stays content (llama.cpp throws here; we stay lenient)
        var r2 <- llama_json_parse("use \{braces} freely")
        t |> equal(r2.content, "use \{braces} freely")
        t |> equal(length(r2.calls), 0)
        delete r2
    }
}
