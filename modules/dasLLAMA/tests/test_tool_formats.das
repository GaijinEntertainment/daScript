options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama   // the facade fires the arch [init] registrations arch_chat reads
require dasllama/dasllama_tools   // the per-ToolMode wire codecs under test — pure string+JSON, model-free
require dasllama/dasllama_chat   // parse_calls_ — the family-dispatching reply parse over the registered templates
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
def test_harmony_system_body(t : T?) {
    t |> run("the harmony system body carries the reasoning level it is asked for, and the channel lines around it") @(t : T?) {
        for (level in ["low", "medium", "high"]) {
            let body = harmony_system_body(level)
            t |> success(find(body, "\nReasoning: {level}\n") >= 0, "the Reasoning line reads {level}: {body}")
            t |> success(find(body, "# Valid channels: analysis, commentary, final.") >= 0, "the channel declaration stands")
            t |> success(find(body, "commentary channel: 'functions'") >= 0, "... and the routing line")
        }
        t |> success(find(harmony_system_body("low"), "medium") < 0, "one level, not two")
        let bare = harmony_system_body("medium", false)
        t |> success(find(bare, "commentary channel: 'functions'") < 0, "without tools the routing line is absent: {bare}")
        t |> success(find(bare, "# Valid channels: analysis, commentary, final. Channel must be included for every message.") >= 0, "... and the channel declaration ends the body")
    }
}

[test]
def test_harmony_parse(t : T?) {
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
        // prose containing a brace that is not a call object stays content (upstream throws here; we stay lenient)
        var r2 <- llama_json_parse("use \{braces} freely")
        t |> equal(r2.content, "use \{braces} freely")
        t |> equal(length(r2.calls), 0)
        delete r2
    }
}

[test]
def test_gemma4_codecs(t : T?) {
    t |> run("the call DSL parses to JSON args (quote tokens, bare keys, nesting)") @(t : T?) {
        var r <- gemma4_parse("I will check.<|tool_call>call:get_weather\{city:<|\"|>London<|\"|>,days:3,opts:\{fast:true}}<tool_call|> Done.")
        t |> equal(r.content, "I will check. Done.")
        t |> equal(length(r.calls), 1)
        if (length(r.calls) == 1) {
            t |> equal(r.calls[0].name, "get_weather")
            t |> equal(r.calls[0].args, "\{\"city\":\"London\",\"days\":3,\"opts\":\{\"fast\":true}}")
        }
        delete r
    }
    t |> run("adjacent blocks are parallel calls; arrays and null parse") @(t : T?) {
        var r <- gemma4_parse("<|tool_call>call:a\{xs:[1,2],n:null}<tool_call|><|tool_call>call:b\{}<tool_call|>")
        t |> equal(length(r.calls), 2)
        if (length(r.calls) == 2) {
            t |> equal(r.calls[0].args, "\{\"xs\":[1,2],\"n\":null}")
            t |> equal(r.calls[1].name, "b")
        }
        delete r
    }
    t |> run("replay + result DSL round-trips (dictsorted, quote-token strings)") @(t : T?) {
        t |> equal(gemma4_call_dsl("get_weather", "\{\"unit\": \"C\", \"city\": \"Paris\"}"),
                   "call:get_weather\{city:<|\"|>Paris<|\"|>,unit:<|\"|>C<|\"|>}")
        t |> equal(gemma4_response_dsl("get_weather", "\{\"temp\": 21}"), "response:get_weather\{temp:21}")
        t |> equal(gemma4_response_dsl("get_weather", "sunny"), "response:get_weather\{value:<|\"|>sunny<|\"|>}")
    }
    t |> run("the declaration DSL: fixed key order, UPPER types, dictsorted properties") @(t : T?) {
        let got = gemma4_tool_declaration(WEATHER_TOOL)
        let want = ("declaration:get_weather\{description:<|\"|>Get current weather<|\"|>," +
                    "parameters:\{properties:\{city:\{description:<|\"|>The city name<|\"|>,type:<|\"|>STRING<|\"|>}," +
                    "unit:\{enum:[<|\"|>celsius<|\"|>,<|\"|>fahrenheit<|\"|>],type:<|\"|>STRING<|\"|>}}," +
                    "required:[<|\"|>city<|\"|>],type:<|\"|>OBJECT<|\"|>}}")
        t |> equal(got, want)
    }
}

[test]
def test_mistral_markerless(t : T?) {
    t |> run("a bare call array parses (the SPM stream suppresses the [TOOL_CALLS] piece)") @(t : T?) {
        var r <- mistral_parse(" [\{\"name\": \"get_weather\", \"arguments\": \{\"city\": \"Paris\"}}]")
        t |> equal(length(r.calls), 1)
        if (length(r.calls) == 1) {
            t |> equal(r.calls[0].name, "get_weather")
        }
        t |> equal(r.content, "")
        delete r
        // an array that is NOT call-shaped stays content
        var r2 <- mistral_parse("Here is a list: [1, 2, 3]")
        t |> equal(length(r2.calls), 0)
        t |> equal(r2.content, "Here is a list: [1, 2, 3]")
        delete r2
    }
}

[test]
def test_shared_codec_helpers(t : T?) {
    t |> run("tool_call_of_json normalizes the replay shape; garbage yields the empty call") @(t : T?) {
        let c = tool_call_of_json("\{\"name\": \"f\", \"arguments\": \{\"a\":1}, \"id\": \"xyz\"}")
        t |> equal(c.name, "f")
        t |> equal(c.args, "\{\"a\": 1}")   // write_json_compact keeps one space after : and ,
        t |> equal(c.id, "xyz")
        let bad = tool_call_of_json("not json")
        t |> equal(bad.name, "")
    }
    t |> run("json_quoted escapes; json_value_or_quoted passes JSON through and quotes prose") @(t : T?) {
        t |> equal(json_quoted("say \"hi\"\n"), "\"say \\\"hi\\\"\\n\"")
        t |> equal(json_value_or_quoted("\{\"temp\":21}"), "\{\"temp\": 21}")
        t |> equal(json_value_or_quoted("[1,2]"), "[1, 2]")
        t |> equal(json_value_or_quoted("sunny, mild"), "\"sunny, mild\"")
        // a JSON STRING input re-quotes: the wire carries it as a JSON string literal either way
        t |> equal(json_value_or_quoted("\"x\""), "\"\\\"x\\\"\"")
    }
    t |> run("the wire-text builders: mistral defs, llama defs + call, harmony headers") @(t : T?) {
        var tools <- ["\{\"a\": 1}", "\{\"b\": 2}"]
        t |> equal(mistral_defs_text(tools), "[AVAILABLE_TOOLS][\{\"a\": 1}, \{\"b\": 2}][/AVAILABLE_TOOLS]")
        t |> success(find(llama_json_defs_text(tools), "\{\"a\": 1}\n\n\{\"b\": 2}") >= 0, "defs joined")
        t |> success(starts_with(llama_json_defs_text(tools), "Given the following functions"), "canonical prompt text")
        delete tools
        t |> equal(llama_json_call_text("f", "\{\"x\": 1}"), "\{\"name\": \"f\", \"parameters\": \{\"x\": 1}}")
        t |> equal(llama_json_call_text("f", ""), "\{\"name\": \"f\", \"parameters\": \{}}")
        t |> equal(harmony_call_header("get_weather"), " to=functions.get_weather")
        t |> equal(harmony_result_header("get_weather"), "functions.get_weather to=assistant")
    }
}

[test]
def test_review_round_hardening(t : T?) {
    t |> run("harmony recipient names truncate at space and at <| in both header arms") @(t : T?) {
        // role-header arm with no <|channel|>: the recipient runs to <|message|> and carries junk
        var r <- harmony_parse(" to=functions.ping json<|message|>\{}")
        t |> equal(length(r.calls), 1)
        t |> equal(r.calls[0].name, "ping")
        delete r
        // channel-header arm: <|constrain|> butts against the name with no space
        var r2 <- harmony_parse("<|channel|>commentary to=functions.lookup<|constrain|>json<|message|>\{\"q\": 1}")
        t |> equal(length(r2.calls), 1)
        t |> equal(r2.calls[0].name, "lookup")
        delete r2
    }
    t |> run("a malformed gemma4 block never grabs a later block's brace") @(t : T?) {
        // first block has NO args dict — it must drop, and block b must still parse cleanly
        var r <- gemma4_parse("<|tool_call>call:a<tool_call|><|tool_call>call:b\{x:1}<tool_call|>")
        t |> equal(length(r.calls), 1)
        if (length(r.calls) == 1) {
            t |> equal(r.calls[0].name, "b")
            t |> equal(r.calls[0].args, "\{\"x\":1}")
        }
        delete r
        // a key with no separator resyncs at the dict closer instead of scanning past the block
        var r2 <- gemma4_parse("<|tool_call>call:c\{flag}<tool_call|> tail")
        t |> equal(length(r2.calls), 1)
        t |> equal(r2.content, "tail")
        delete r2
    }
    t |> run("post-final framing drops: a bare <|start|> tail after the final channel") @(t : T?) {
        var r <- harmony_parse("<|channel|>final<|message|>Done.<|start|>assistant<|channel|>analysis<|message|>stray")
        t |> equal(r.content, "Done.")
        t |> equal(r.reasoning, "stray")   // segmented, not streamed raw into content
        delete r
    }
}

[test]
def test_harmony_no_end_token(t : T?) {
    t |> run("the live wire: <|end|> suppressed, segments split on bare <|start|>assistant") @(t : T?) {
        var r <- harmony_parse("<|channel|>analysis<|message|>We need to call get_weather.<|start|>assistant<|channel|>commentary to=functions.get_weather <|constrain|>json<|message|>\{\"city\":\"Paris\"}")
        t |> equal(r.reasoning, "We need to call get_weather.")
        t |> equal(length(r.calls), 1)
        if (length(r.calls) == 1) {
            t |> equal(r.calls[0].name, "get_weather")
            t |> equal(r.calls[0].args, "\{\"city\":\"Paris\"}")
        }
        delete r
    }
}

[test]
def test_marker_parse(t : T?) {
    t |> run("no markers - everything is content") @(t : T?) {
        var r <- marker_parse("Just an answer.", "<tool_call>", "</tool_call>")
        t |> equal(r.content, "Just an answer.")
        t |> equal(length(r.calls), 0)
        delete r
    }
    t |> run("empty open mark - no tool format, everything is content") @(t : T?) {
        var r <- marker_parse("<tool_call>looks like one</tool_call>", "", "")
        t |> equal(r.content, "<tool_call>looks like one</tool_call>")
        t |> equal(length(r.calls), 0)
        delete r
    }
    t |> run("one call, content before the opener") @(t : T?) {
        var r <- marker_parse("Let me check.\n<tool_call>\n\{\"name\": \"get_weather\", \"arguments\": \{\"city\": \"Paris\"}}\n</tool_call>",
            "<tool_call>", "</tool_call>")
        t |> equal(r.content, "Let me check.")
        t |> equal(length(r.calls), 1)
        if (length(r.calls) == 1) {
            t |> equal(r.calls[0].name, "get_weather")
            t |> success(find(r.calls[0].args, "Paris") >= 0, "args carry the city")
        }
        delete r
    }
    t |> run("two calls parse in order") @(t : T?) {
        var r <- marker_parse(("<tool_call>\{\"name\": \"a\", \"arguments\": \{}}</tool_call>" +
                               "<tool_call>\{\"name\": \"b\", \"arguments\": \{}}</tool_call>"),
            "<tool_call>", "</tool_call>")
        t |> equal(length(r.calls), 2)
        if (length(r.calls) == 2) {
            t |> equal(r.calls[0].name, "a")
            t |> equal(r.calls[1].name, "b")
        }
        delete r
    }
    t |> run("unterminated span ends the scan; nothing parsed keeps the whole reply as content") @(t : T?) {
        var r <- marker_parse("Half a call<tool_call>\{\"name\": \"x\"", "<tool_call>", "</tool_call>")
        t |> equal(length(r.calls), 0)
        t |> equal(r.content, "Half a call<tool_call>\{\"name\": \"x\"")
        delete r
    }
    t |> run("a nameless/garbage span skips; nothing parsed keeps the whole reply") @(t : T?) {
        var r <- marker_parse("A<tool_call>not json</tool_call>B", "<tool_call>", "</tool_call>")
        t |> equal(length(r.calls), 0)
        t |> equal(r.content, "A<tool_call>not json</tool_call>B")
        delete r
    }
}

[test]
def test_parse_calls_dispatch(t : T?) {
    t |> run("hermes: think span + content + marker call classify into all three") @(t : T?) {
        var c = ChatSession()
        c.tmpl <- arch_chat("qwen3")
        var r <- parse_calls_(c, ("<think>plan the call</think>Sure.\n" +
            "<tool_call>\n\{\"name\": \"get_weather\", \"arguments\": \{\"city\": \"Paris\"}}\n</tool_call>"))
        t |> equal(r.reasoning, "plan the call")
        t |> equal(r.content, "Sure.")
        t |> equal(length(r.calls), 1)
        if (length(r.calls) == 1) {
            t |> equal(r.calls[0].name, "get_weather")
        }
        delete r
        delete c
    }
    t |> run("none mode: a plain-chat family returns the reply as content") @(t : T?) {
        var c = ChatSession()
        c.tmpl <- arch_chat("gemma2")   // no tool format declared
        var r <- parse_calls_(c, "Just words.")
        t |> equal(r.reasoning, "")
        t |> equal(r.content, "Just words.")
        t |> equal(length(r.calls), 0)
        delete r
        delete c
    }
}

[test]
def test_string_arguments_passthrough(t : T?) {
    t |> run("arguments emitted as a JSON string pass through verbatim (the shared normalizer)") @(t : T?) {
        let c = tool_call_of_json("\{\"name\": \"a\", \"arguments\": \"\{\\\"x\\\": 1}\"}")
        t |> equal(c.name, "a")
        t |> equal(c.args, "\{\"x\": 1}")
    }
    t |> run("explicit-null and empty-string arguments normalize to the empty object") @(t : T?) {
        t |> equal(tool_call_of_json("\{\"name\": \"a\", \"arguments\": null}").args, "\{}")
        t |> equal(tool_call_of_json("\{\"name\": \"a\", \"arguments\": \"\"}").args, "\{}")
    }
}

// ===== Qwen3.6 — <function=NAME> blocks: the template's own text is the fixture =====

[test]
def test_xml_function_format(t : T?) {
    t |> run("a replayed call renders the template's block: string values verbatim, others as one-line JSON") @(t : T?) {
        t |> equal("<tool_call>\n<function=control>\n<parameter=point>\nlight.1\n</parameter>\n<parameter=value>\nfalse\n</parameter>\n</function>\n</tool_call>",
            xml_function_call_text("control", "\{\"point\":\"light.1\",\"value\":\"false\"}"))
        t |> equal("<tool_call>\n<function=remind>\n<parameter=when>\n\{\"h\": 8, \"m\": 30}\n</parameter>\n<parameter=n>\n3\n</parameter>\n</function>\n</tool_call>",
            xml_function_call_text("remind", "\{\"when\":\{\"h\":8,\"m\":30},\"n\":3}"))
        t |> equal("<tool_call>\n<function=not_for_me>\n</function>\n</tool_call>", xml_function_call_text("not_for_me", "\{}"))
    }
    t |> run("a reply parses: content before the first block, each block a call, values strings unless they are JSON objects or arrays") @(t : T?) {
        var tr <- xml_function_parse("Switching it off.\n\n<tool_call>\n<function=control>\n<parameter=point>\nlight.1\n</parameter>\n<parameter=value>\nfalse\n</parameter>\n</function>\n</tool_call>\n<tool_call>\n<function=remind>\n<parameter=when>\n\{\"h\": 8\}\n</parameter>\n<parameter=text>\nline one\nline two\n</parameter>\n</function>\n</tool_call>")
        t |> equal("Switching it off.", tr.content)
        t |> equal(2, length(tr.calls))
        t |> equal("control", tr.calls[0].name)
        t |> equal("\{\"point\": \"light.1\", \"value\": \"false\"}", tr.calls[0].args, "a value that reads as a bool stays the string the template wrote")
        t |> equal("remind", tr.calls[1].name)
        t |> equal("\{\"when\": \{\"h\": 8}, \"text\": \"line one\\nline two\"}", tr.calls[1].args, "an object value stays JSON, a multi-line value keeps its inner newline")
        delete tr
        var none <- xml_function_parse("Nothing to do here.")
        t |> equal("Nothing to do here.", none.content)
        t |> equal(0, length(none.calls))
        delete none
        var bare <- xml_function_parse("<tool_call>\n<function=not_for_me>\n</function>\n</tool_call>")
        t |> equal("", bare.content)
        t |> equal(1, length(bare.calls))
        t |> equal("\{}", bare.calls[0].args, "a parameterless call is the empty object")
        delete bare
        var cut <- xml_function_parse("Hold on.\n<tool_call>\n<function=control>\n<parameter=point>\nlight")
        t |> equal(0, length(cut.calls), "an unterminated block is no call")
        t |> equal("Hold on.\n<tool_call>\n<function=control>\n<parameter=point>\nlight", cut.content, "... and the reply stays whole as content")
        delete cut
        var nameless <- xml_function_parse("<tool_call>\n<function=>\n</function>\n</tool_call>\n<tool_call>\n<function=remind>\n</function>\n</tool_call>")
        t |> equal(1, length(nameless.calls), "a nameless block is skipped")
        t |> equal("remind", nameless.calls[0].name)
        delete nameless
        var odd <- xml_function_parse("<tool_call>\n<function=f>\n<parameter=a</parameter>\n<parameter=b>\n\{not json\n</parameter>\n<parameter=c>\n1\n</parameter>\n<parameter=c>\n2\n</parameter>\n</function>\n</tool_call>")
        t |> equal(1, length(odd.calls))
        t |> equal("\{\"b\": \"\{not json\", \"c\": \"2\"}", odd.calls[0].args, "an opener with no `>` of its own is skipped, a `\{` that is no JSON stays a string, a key written twice keeps its last value")
        delete odd
    }
    t |> run("a reply's values are typed by the declared parameters: a string stays a string whatever it looks like, a declared number or boolean goes bare") @(t : T?) {
        var tools <- ["\{\"type\":\"function\",\"function\":\{\"name\":\"write\",\"parameters\":\{\"type\":\"object\",\"properties\":\{\"content\":\{\"type\":\"string\"},\"n\":\{\"type\":\"integer\"},\"ok\":\{\"type\":\"boolean\"},\"when\":\{\"type\":\"object\"}}}}}"]
        var types <- tool_param_types(tools)
        t |> equal("string", types?["write"]?["content"] ?? "")
        let reply = "<tool_call>\n<function=write>\n<parameter=content>\n\{\"a\": 1}\n</parameter>\n<parameter=n>\n5\n</parameter>\n<parameter=ok>\ntrue\n</parameter>\n<parameter=when>\n\{\"h\": 8}\n</parameter>\n<parameter=free>\n7\n</parameter>\n</function>\n</tool_call>"
        var typed <- xml_function_parse(reply, types)
        t |> equal("\{\"content\": \"\{\\\"a\\\": 1}\", \"n\": 5, \"ok\": true, \"when\": \{\"h\": 8}, \"free\": \"7\"}", typed.calls[0].args, "the declared string keeps its braces as text, the number and the boolean go bare, the object parses, an undeclared number stays text")
        delete typed
        var untyped <- xml_function_parse(reply)
        t |> equal("\{\"content\": \{\"a\": 1}, \"n\": \"5\", \"ok\": \"true\", \"when\": \{\"h\": 8}, \"free\": \"7\"}", untyped.calls[0].args, "with no declarations a value is typed by its own text alone")
        delete untyped
        var bad <- xml_function_parse("<tool_call>\n<function=write>\n<parameter=n>\nfive\n</parameter>\n</function>\n</tool_call>", types)
        t |> equal("\{\"n\": \"five\"}", bad.calls[0].args, "a declared number whose text is no number stays a string")
        delete bad
        delete types
        delete tools
    }
}
