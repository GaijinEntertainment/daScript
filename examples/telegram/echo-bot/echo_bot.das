options gen2
options rtti
options persistent_heap
options gc

require telegram/tbot
require daslib/fio
require live/live_gc
require live_host
require strings

// Lifecycle state is global: JIT heap collection cannot see stack locals as GC
// roots, so main keeps no collectable value alive across maybe_collect_gc().
var g_offset = 0l

[export]
def init() {
    // read token from env var or .bot_token file
    var token = get_env_variable("TELEGRAM_BOT_TOKEN")
    if (empty(token)) {
        fopen(".bot_token", "rb") $(f) {
            if (f != null) {
                token = fread(f)
                token = strip(token)
            }
        }
    }
    if (empty(token)) {
        print("No bot token found. Set TELEGRAM_BOT_TOKEN or create .bot_token file\n")
        request_exit()
        return
    }
    telegram_set_configuration(configuration(token = token))
    print("Echo bot started. Send me a message!\n")
    g_offset = 0l
}

// One long poll (the 30-second timeout is the pacing) + echo every new message.
[export]
def update() {
    var updates <- telegram_getUpdates(get_updates(offset = g_offset, timeout = 30l, limit = 100l))
    let err = telegram_get_last_error()
    if (!empty(err)) {
        print("ERROR: {err}\n")
        request_exit()
        return
    }
    for (u in updates) {
        g_offset = u.update_id + 1l
        if (u.message != null) {
            assume msg = *u.message
            let chat_id = "{msg.chat.id}"
            if (!empty(msg.text)) {
                print("{msg._from.first_name}: {msg.text}\n")
                telegram_sendMessage(send_message(chat_id = chat_id, text = msg.text))
                let send_err = telegram_get_last_error()
                if (!empty(send_err)) {
                    print("Send error: {send_err}\n")
                }
            }
        }
    }
}

[export]
def shutdown() {
    pass
}

[export]
def main() {
    init()
    while (!exit_requested()) {
        update()
        maybe_collect_gc()
    }
    shutdown()
}
