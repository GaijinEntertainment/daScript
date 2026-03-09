options gen2
options rtti
options persistent_heap
options no_unused_block_arguments = false

require telegram/tbot
require anthropic/anthropic
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

def handle_message(chat_id, user_name, text : string) {
    print("{user_name}: {text}\n")
    // send "typing" indicator would be nice, but not in tbot API yet
    var config <- claude_config_from_env()
    var params : MessageCreateParams
    params.system = system_prompt
    params.messages |> emplace(user_message(text))
    var error : string
    var msg = create_message(config, params, error)
    var reply : string
    if (!empty(error)) {
        reply = "Error: {error}"
        print("  ERROR: {error}\n")
    } else {
        reply = get_text(msg)
        print("  Reply: {length(reply)} chars, {msg.usage.input_tokens}+{msg.usage.output_tokens} tokens\n")
    }
    telegram_sendLongMessage(send_message(chat_id = chat_id, text = reply))
    let send_err = telegram_get_last_error()
    if (!empty(send_err)) {
        print("  Send error: {send_err}\n")
    }
}

[export]
def main() {
    // load system prompt
    load_system_prompt()
    // configure telegram
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
    // verify anthropic key
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
