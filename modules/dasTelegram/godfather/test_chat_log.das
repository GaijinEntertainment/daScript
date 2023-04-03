require fio

require daslib/algorithm
require daslib/stringify
require daslib/strings_boost
require daslib/json_boost

require openai/openai

require chat_db
require emoji

require godfather_chat

// let chat_id = -1001849665593l // in bots we trust
let chat_id = -1001666598976l; // remote work

[export]
def main
    if !openai_key_is_set()
        panic("openai key is not set")
    if !create_chat_log("d:/Work/telegramm/GG19781978_bot/chat_log.db")
        panic("failed to create chat log")
    /*
    var res = generate_chat_reply(chat_id)
    debug(res,"res = ")
    */

    let summary_so_far = generate_previous_conversation_summary(chat_id,"in english")
    print("{summary_so_far}\n")
