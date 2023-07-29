options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module godfather_admin shared private

require daslib/strings_boost
require daslib/algorithm

require telegram/tbot

require godfather_chat
require chat_db
require emoji

var public BOT_SHUTDOWN = false

def public collect_chat_log ( chat_id:int64; last_n_messages:int )
    var users : table<int64; UserEntry>
    list_chat_users(chat_id) <| $ ( userEntry )
        users[userEntry.user_id] := userEntry
    var log : array<string>
    list_chat_log(chat_id, last_n_messages) <| $ ( msg )
        let user_name = "{users[msg.from_id].first_name} {users[msg.from_id].last_name}"
        if !empty(msg.transcription)
            log |> push ("{user_name}: {msg.transcription}")
        log |> push ("{user_name}: {msg.text |> fix_broken_utf16 |> fix_broken_utf32}")
    log |> reverse
    return <- log

def dump_chat_users ( chat_id:int64 )
    return build_string <| $ ( writer )
        writer |> write("```\n")
        list_chat_users(chat_id) <| $ ( userEntry )
            writer |> write(userEntry.user_id)
            if userEntry.is_bot
                writer |> write("<bot>")
            writer |> write(": ")
            writer |> write(userEntry.user_name)
            writer |> write(" aka ")
            writer |> write(userEntry.first_name)
            writer |> write(" ")
            writer |> write(userEntry.last_name)
            writer |> write("\n")
        writer |> write("```")

def dump_chat_log ( chat_id:int64; last_n_messages:int )
    var inscope log <- collect_chat_log(chat_id, last_n_messages)
    return build_string <| $ ( writer )
        writer |> write("```\n")
        for l in log
            writer |> write(l)
            writer |> write("\n")
        writer |> write("```")

def dump_api_usage ( chat_id : int64 )
    var users : table<int64; UserEntry>
    if chat_id==0l
        list_all_users <| $ ( userEntry )
            users[userEntry.user_id] := userEntry
    else
        list_chat_users(chat_id) <| $ ( userEntry )
            users[userEntry.user_id] := userEntry
    return build_string <| $ ( writer)
        writer |> write("```\n")
        get_all_api_use <| $ ( use )
            if chat_id==0l || users |> key_exists(use.user_id)
                let user_name = "{users[use.user_id].first_name} {users[use.user_id].last_name}"
                writer |> write(user_name)
                writer |> write(": tokens=")
                writer |> write(use.tokens)
                writer |> write(" voice_bytes=")
                writer |> write(use.voice_bytes)
                writer |> write("\n")
        writer |> write("```")

def modify_right ( var rights:ChannelRights; which:string; value:bool )
    if which == "decode_voice"
        rights.decode_voice = value
    elif which == "draw_pictures"
        rights.draw_pictures = value
    elif which == "bot_answers"
        rights.bot_answers = value
    elif which == "summary"
        rights.summary = value
    else
        to_log(LOG_ERROR, "unknown right {which}\n")
        return false
    return true

def public process_admin_commands ( message:message; botId:string; adminId:int64 )
    if message.from?.id ?? 0l != adminId
        return false
    var response : string
    if message.text == "/test@{botId}"
        response = build_string <| $ ( writer )
            writer |> write("```\n\n code block \n\n line 2 \n\n line 3 \n\n```")
    elif message.text == "/users@{botId}"
        response = dump_chat_users(message.chat.id)
    elif message.text == "/reset@{botId}"
        response = "reset chat {message.chat.id}: {reset_chat(message.chat.id)}";
    elif message.text |> starts_with("/chatlog@{botId}")
        let len_cmd = length("/chatlog@{botId}")
        let num_substr = message.text |> slice(len_cmd+1)
        var max_messages = 100
        if !empty(num_substr)
            max_messages = int(num_substr)
        response = dump_chat_log(message.chat.id, max_messages)
        response = "last {max_messages} messages in {message.chat.id}\n{response}"
    elif message.text=="/addchannel@{botId}" || message.text=="/addsuperchannel@{botId}"
        let superchannel = message.text=="/addsuperchannel@{botId}"
        set_channel_rights([[ChannelRights
            chat_id = message.chat.id,
            description = message.chat.title,
            decode_voice = superchannel,
            draw_pictures = superchannel,
            bot_answers = superchannel,
            summary = superchannel
        ]])
        response = "add channel {message.chat.id} with all rights"
    elif message.text == "/delchannel@{botId}"
        response = "del channel {message.chat.id} remove channel rights: {delete_channel_rights(message.chat.id)}"
    elif message.text |> starts_with("/allow@{botId}")
        var rights = get_channel_rights(message.chat.id)
        if rights.chat_id!=0l
            let what = message.text |> slice(length("/allow@{botId}")+1)
            response = "allow right {what}: {modify_right(rights, what, true)}"
            set_channel_rights(rights)
        else
            response = "no rights for channel {message.chat.id}"
    elif message.text |> starts_with("/disallow@{botId}")
        var rights = get_channel_rights(message.chat.id)
        if rights.chat_id!=0l
            let what = message.text |> slice(length("/disallow@{botId}")+1)
            response = "disallow right {what}: {modify_right(rights, what, false)}"
            set_channel_rights(rights)
        else
            response = "no rights for channel {message.chat.id}"
    elif message.text == "/status@{botId}"
        var rights = get_channel_rights(message.chat.id)
        if rights.chat_id!=0l
            response = build_string <| $ ( writer )
                writer |> write("```\n")
                writer |> write("chat_id: {rights.chat_id}\n")
                writer |> write("description: {rights.description |> sanitize_content}\n")
                writer |> write("decode_voice: {rights.decode_voice}\n")
                writer |> write("draw_pictures: {rights.draw_pictures}\n")
                writer |> write("bot_answers: {rights.bot_answers}\n")
                writer |> write("summary: {rights.summary}\n")
                writer |> write("```")
        else
            response = "no rights for channel {message.chat.id}"
    elif message.text == "/api@{botId}"
        response = dump_api_usage(message.chat.id)
    elif message.text == "/apiall@{botId}"
        response = dump_api_usage(0l)
    elif message.text == "/shutdown@{botId}"
        response = "shutdown"
        BOT_SHUTDOWN = true
    elif message.text == "/hethinks@{botId}"
        response = get_first_summary(message.chat.id, LONG_MAX).text
    elif message.text == "/help@{botId}"
        response = build_string <| $ ( writer )
            writer |> write("```\n")
            writer |> write("/test@{botId} - test command\n")
            writer |> write("/users@{botId} - list chat users\n")
            writer |> write("/reset@{botId} - reset chat\n")
            writer |> write("/chatlog@{botId} - dump chat log\n")
            writer |> write("/addchannel@{botId} - add channel\n")
            writer |> write("/delchannel@{botId} - del channel\n")
            writer |> write("/allow@{botId} <right> - allow right\n")
            writer |> write("/disallow@{botId} <right> - disallow right\n")
            writer |> write("/status@{botId} - show channel status\n")
            writer |> write("/api@{botId} - show api usage in this chat\n")
            writer |> write("/apiall@{botId} - show api usage for all chats\n")
            writer |> write("/shutdown@{botId} - shutdown bot\n")
            writer |> write("/hethinks@{botId} - show first summary of what is on bot's mind\n")
            writer |> write("/help@{botId} - this help\n")
            writer |> write("```")
    else
        return false
    if !empty(response)
        telegram_sendLongMessage([[sendmessage
            chat_id = "{message.chat.id}",
            reply_to_message_id = message.message_id,
            text = response |> telegram_escapeMarkdownV2,
            parse_mode = "MarkdownV2"
        ]])
        if !telegram_get_last_error() |> empty
            to_log(LOG_ERROR, "telegram_sendLongMessage failed: {telegram_get_last_error()}\n")
        return true
    return false
