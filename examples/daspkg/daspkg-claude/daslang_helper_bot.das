options gen2
options rtti
options persistent_heap
options no_unused_block_arguments = false

require telegram/tbot
require anthropic/anthropic
require daslib/json_boost
require fio
require strings

var system_prompt : string

def load_system_prompt() {
    let prompt_file = "examples/daslang_helper/system_prompt.md"
    fopen(prompt_file, "rb") <| $(f) {
        if (f != null) {
            system_prompt = fread(f)
        }
    }
    if (empty(system_prompt)) {
        panic("Failed to load system prompt from {prompt_file}")
    }
    print("Loaded system prompt ({length(system_prompt)} bytes)\n")
}

// ============================================================================
// compile_check tool — compiles daslang code in-process
// ============================================================================

def compile_check_tool() : Tool {
    return <- Tool(
        name = "compile_check",
        description = "Compile daslang code and return diagnostics. Use this to verify code before showing it to the user.",
        input_schema = "\{\"type\":\"object\",\"properties\":\{\"code\":\{\"type\":\"string\",\"description\":\"daslang source code to compile\"\}\},\"required\":[\"code\"]\}"
    )
}

def compile_check(code : string) : string {
    let file_name = "__check__.das"
    var inscope access <- make_file_access("sandbox.das_project")
    set_file_source(access, file_name, code)
    var result : string
    using() <| $(var mg : ModuleGroup) {
        using() <| $(var cop : CodeOfPolicies) {
            cop.threadlock_context = true
            cop.ignore_shared_modules = true
            compile_file(file_name, access, unsafe(addr(mg)), cop) <| $(ok; program; issues) {
                if (!ok) {
                    result = "Compilation FAILED:\n{issues}"
                } else {
                    var warnings = ""
                    if (!empty(issues)) {
                        warnings = "Warnings:\n{issues}\n"
                    }
                    result = "{warnings}Compilation OK."
                }
            }
        }
    }
    return result
}

// ============================================================================
// tool dispatch + narration
// ============================================================================

def html_escape(text : string) : string {
    return build_string() $(var w) {
        for (ch in text) {
            if (ch == '&') {
                w |> write("&amp;")
            } elif (ch == '<') {
                w |> write("&lt;")
            } elif (ch == '>') {
                w |> write("&gt;")
            } else {
                w |> write_char(int(ch))
            }
        }
    }
}

// convert standard markdown to Telegram HTML
// handles: ```code blocks```, **bold**, `inline code`
def markdown_to_html(md : string) : string {
    return build_string() $(var w) {
        var i = 0
        let n = length(md)
        var in_code_block = false
        while (i < n) {
            // fenced code block: ```
            if (i + 2 < n && character_at(md, i) == '`' && character_at(md, i + 1) == '`' && character_at(md, i + 2) == '`') {
                if (!in_code_block) {
                    in_code_block = true
                    i += 3
                    // skip optional language tag (e.g. "das\n")
                    while (i < n && character_at(md, i) != '\n') {
                        i++
                    }
                    if (i < n) {
                        i++ // skip \n
                    }
                    w |> write("<pre>")
                } else {
                    in_code_block = false
                    i += 3
                    // skip trailing newline after closing ```
                    if (i < n && character_at(md, i) == '\n') {
                        i++
                    }
                    w |> write("</pre>")
                }
                continue
            }
            if (in_code_block) {
                let ch = character_at(md, i)
                if (ch == '&') {
                    w |> write("&amp;")
                } elif (ch == '<') {
                    w |> write("&lt;")
                } elif (ch == '>') {
                    w |> write("&gt;")
                } else {
                    w |> write_char(ch)
                }
                i++
                continue
            }
            // **bold**
            if (i + 1 < n && character_at(md, i) == '*' && character_at(md, i + 1) == '*') {
                let close = find(md, "**", i + 2)
                if (close >= 0) {
                    w |> write("<b>")
                    w |> write(html_escape(slice(md, i + 2, close)))
                    w |> write("</b>")
                    i = close + 2
                    continue
                }
            }
            // `inline code`
            if (character_at(md, i) == '`') {
                let close = find(md, "`", i + 1)
                if (close >= 0) {
                    w |> write("<code>")
                    w |> write(html_escape(slice(md, i + 1, close)))
                    w |> write("</code>")
                    i = close + 1
                    continue
                }
            }
            // regular character — escape
            let ch = character_at(md, i)
            if (ch == '&') {
                w |> write("&amp;")
            } elif (ch == '<') {
                w |> write("&lt;")
            } elif (ch == '>') {
                w |> write("&gt;")
            } else {
                w |> write_char(ch)
            }
            i++
        }
    }
}

def send_html(chat_id, text : string) {
    telegram_sendLongMessage(send_message(chat_id = chat_id, text = text, parse_mode = "HTML"))
}

def get_code_from_tool(tc : ToolUseBlock) : string {
    let code_val = tc.input != null ? (tc.input.value as _object)?["code"] ?? default<JsonValue?> : null
    if (code_val != null && code_val.value is _string) {
        return code_val.value as _string
    }
    return ""
}

def preview(code : string; max_lines : int = 6) : string {
    var lines : int
    var pos = 0
    while (pos < length(code) && lines < max_lines) {
        let nl = find(code, "\n", pos)
        if (nl < 0) {
            break
        }
        pos = nl + 1
        lines++
    }
    if (pos < length(code)) {
        return slice(code, 0, pos) + "..."
    }
    return code
}

// ============================================================================
// message handling
// ============================================================================

def handle_message(chat_id, user_name, text : string) {
    print("{user_name}: {text}\n")
    var config <- claude_config_from_env()
    var params : MessageCreateParams
    params.system = system_prompt
    params.messages |> emplace(user_message(text))
    params.tools |> emplace(compile_check_tool())
    var attempt = 0
    var error : string
    var msg = run_tool_loop(config, params, $(tc) {
        if (tc.name != "compile_check") {
            return "Error: unknown tool '{tc.name}'"
        }
        let code = get_code_from_tool(tc)
        if (empty(code)) {
            return "Error: missing 'code' parameter"
        }
        attempt++
        print("  [tool] compile_check attempt {attempt} ({length(code)} bytes)\n")
        let code_html = "<pre>{html_escape(preview(code))}</pre>"
        if (attempt == 1) {
            send_html(chat_id, "Verifying code...\n{code_html}")
        } else {
            send_html(chat_id, "Attempt {attempt} — fixing...\n{code_html}")
        }
        let result = compile_check(code)
        let ok = find(result, "Compilation OK") >= 0
        if (ok) {
            send_html(chat_id, "<b>Compiles OK!</b>")
        } else {
            send_html(chat_id, "Compilation error:\n<pre>{html_escape(result)}</pre>")
        }
        return result
    }, error)
    var reply : string
    if (!empty(error)) {
        reply = "Error: {error}"
        print("  ERROR: {error}\n")
    } else {
        reply = get_text(msg)
        print("  Reply: {length(reply)} chars, {msg.usage.input_tokens}+{msg.usage.output_tokens} tokens\n")
    }
    send_html(chat_id, markdown_to_html(reply))
    let send_err = telegram_get_last_error()
    if (!empty(send_err)) {
        print("  Send error: {send_err}\n")
    }
}

[export]
def main() {
    load_system_prompt()
    var token = ""
    unsafe {
        token = get_env_variable("TELEGRAM_BOT_TOKEN")
    }
    if (empty(token)) {
        fopen(".bot_token", "rb") <| $(f) {
            if (f != null) {
                token = fread(f)
                token = strip(token)
            }
        }
    }
    if (empty(token)) {
        panic("No bot token. Set TELEGRAM_BOT_TOKEN or create .bot_token file")
    }
    if (!has_env_variable("ANTHROPIC_API_KEY")) {
        panic("ANTHROPIC_API_KEY environment variable is not set")
    }
    telegram_set_configuration(configuration(token = token))
    print("daslang helper bot started. Waiting for messages...\n")
    var offset = 0l
    while (true) {
        var updates <- telegram_getUpdates(get_updates(offset = offset, timeout = 30l, limit = 100l))
        let err = telegram_get_last_error()
        if (!empty(err)) {
            print("Poll error: {err}\n")
            break
        }
        for (u in updates) {
            offset = u.update_id + 1l
            if (u.message != null) {
                assume msg = *u.message
                let chat_id = "{msg.chat.id}"
                if (!empty(msg.text)) {
                    handle_message(chat_id, msg._from.first_name, msg.text)
                }
            }
        }
    }
}
