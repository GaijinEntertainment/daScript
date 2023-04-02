require fio

require daslib/algorithm
require daslib/stringify
require daslib/strings_boost
require daslib/json_boost

require openai/openai

require chat_log
require emoji
require godfather_chat

[export]
def main
    if !openai_key_is_set()
        panic("openai key is not set")
