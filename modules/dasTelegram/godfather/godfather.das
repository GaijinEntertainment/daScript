require daslib/curl

require fio
require daslib/strings_boost
require math
require openai/openai
require daslib/json_boost
require daslib/base64
require daslib/algorithm
require daslib/utf8_utils

require telegram/tbotlog
require telegram/tbot

require godfather_chat
require godfather_admin
require chat_db
require emoji

let BOT_DEBUG_MODE = false
let BOT_DOES_NOT_ANSWER = false

let BOT_OPENAI_TIMEOUT = 30.0
let BOT_UPDATE_TIMEOUT = 5.0
let BOT_UPDATE_RETRY_TIMEOUT = 20.0

struct BotConfig : tbot::configuration
    id : string
    name : string
    path_to_ffmpeg : string
    path_to_chatlog : string
    admin : string

var g_botConfig : BotConfig
var g_userGodfather : user
var g_adminId : int64

def read_bot_config(configfile:string)
    var config : BotConfig
    fopen(configfile,"rb") <| $ (f)
        if f != null
            var data = fread(f)
            var error = ""
            var json = read_json(data, error)
            if json == null
                panic("failed to parse bot config, {error}\n")
            config = json |> from_JV(type<BotConfig>)
            unsafe
                delete json
    return <- config

def get_user_name ( user:user? )
    if user == null
        return "unknown"
    if user.username |> empty
        return "{user.first_name} {user.last_name}"
    else
        return user.username

def convert_time ( t:int64 )
    unsafe
        return "{*reinterpret<clock?> addr(t)}"

def get_temp_file_name(name:string)
    let temp_dir = "{get_das_root()}/temp"
    mkdir(temp_dir)
    return "{temp_dir}/{name}"

def convert_to_mp3 ( fin,fout:string )
    to_log(LOG_INFO, "converting {fin} to {fout}\n")
    let cmd = "{g_botConfig.path_to_ffmpeg}/ffmpeg -loglevel quiet -y -i {fin} -ar 22050 {fout}"
    let exit_code = unsafe(popen(cmd)) <| $ (f)
        pass
    if exit_code != 0
        to_log(LOG_ERROR, "convert to mp3 failed\n{cmd}\nffmpeg failed with exit code {exit_code}\n")
    return exit_code==0

def decode_voice_message(message:tbotapi::message ) : string
    to_log(LOG_INFO, "decoding voice message {message.voice.file_id}\n")
    var inscope waveBytes : array<uint8>
    var inscope file <- telegram_getFile([[getfile
        file_id = message.voice.file_id
    ]])
    if !telegram_get_last_error() |> empty
        to_log(LOG_ERROR, "telegram_getFile failed: {telegram_get_last_error()}\n")
        return ""
    waveBytes <- telegram_download(file)
    if waveBytes |> empty
        to_log(LOG_ERROR, "download failed: {telegram_get_last_error()}\n")
        return ""
    let temp_file_name = get_temp_file_name(file.file_path |> replace("/","_"))
    var saved = false
    fopen(temp_file_name,"wb") <| $ (f)
        if f != null
            to_log(LOG_INFO,"saving voice message to {temp_file_name}\n")
            fwrite(f,waveBytes)
            saved = true
    if !saved
        to_log(LOG_ERROR, "failed to save {temp_file_name}\n")
        return ""
    let temp_mp3_file_name = "{temp_file_name}.mp3"
    if !convert_to_mp3(temp_file_name,temp_mp3_file_name)
        return ""
    to_log(LOG_INFO, "creating transcription for {temp_mp3_file_name}\n")
    var inscope ru : TranscriptionResult
    for i in range(BOT_RETRY_ATTEMPTS)
        ru <- openai_create_transcription([[Transcription() file = temp_mp3_file_name, language = ""]])
        if empty(ru.error)
            break
        if ! (ru.error |> starts_with("curl timeout") || ru.error |> starts_with("HTTPS post failed"))
            break
        to_log(LOG_ERROR, "retrying, reason: {ru.error}\n")
        delete ru
        sleep(uint(BOT_RETRY_DELAY)*1000u)
    if !empty(ru.error)
        to_log(LOG_ERROR,"transcription failed {ru.error}\n")
        return ""
    let transcription_text = sanitize_content(ru.text)
    if transcription_text |> empty
        to_log(LOG_INFO,"nothing to work with - empty text\n")
        return ""
    to_log(LOG_INFO, "transcription: {transcription_text}\n")
    to_log(LOG_INFO, "sending transcription to {message.chat.id}, replying to {message.message_id}\n")
    telegram_sendLongMessage([[sendmessage
        chat_id = "{message.chat.id}",
        reply_to_message_id = message.message_id,
        text = transcription_text
    ]])
    if !telegram_get_last_error() |> empty
        to_log(LOG_ERROR, "telegram_sendLongMessage failed: {telegram_get_last_error()}\n")
        return ""
    return transcription_text

def is_someone_asking_bot_to_say_something ( message:tbotapi::message  )
    if message.text |> find("@{g_botConfig.id}") != -1
        return true
    if message.reply_to_message?.from?.id ?? 0l == g_userGodfather.id
        return true
    return false

def process_error_message ( message:tbotapi::message; whatGodfatherSaid:GodfatherMessage )
    to_log(LOG_ERROR, "generate_chat_reply failed: {whatGodfatherSaid.message}\n{whatGodfatherSaid}\n")
    telegram_sendLongMessage([[sendmessage
        chat_id = "{message.chat.id}",
        reply_to_message_id = message.message_id,
        text = "{g_botConfig.name} (debug message):```\n{whatGodfatherSaid.message}\n{whatGodfatherSaid}\n```" |> telegram_escapeMarkdownV2,
        parse_mode = "MarkdownV2"
    ]])
    if !telegram_get_last_error() |> empty
        to_log(LOG_ERROR, "telegram_sendLongMessage failed: {telegram_get_last_error()}\n")

def send_system_message ( message:tbotapi::message; text:string )
    to_log(LOG_INFO, "text\n")
    print("{text}\n")
    telegram_sendLongMessage([[sendmessage
        chat_id = "{message.chat.id}",
        reply_to_message_id = message.message_id,
        text = text
    ]])
    if !telegram_get_last_error() |> empty
        to_log(LOG_ERROR, "telegram_sendLongMessage failed: {telegram_get_last_error()}\n")

def fix_message ( text:string )
    return text |> fix_emoji

def write_user_to_log ( user:tbotapi::user )
    write_user([[UserEntry
                user_id = user.id,
                is_bot = user.is_bot,
                first_name = user.first_name,
                last_name = user.last_name,
                user_name = user.username]])

def write_message_to_log ( message:tbotapi::message; transcription:string )
    write_to_log([[ChatLogEntry
                chat_id = message.chat.id,
                message_id = message.message_id,
                date = message.date,
                from_id = message.from?.id ?? 0l,
                reply_to_message = message.reply_to_message?.from?.id ?? 0l,
                text = message.text,
                transcription = transcription]])
    if message.from != null
        write_user_to_log(*message.from)
    if message.reply_to_message != null && message.reply_to_message.from != null
        write_user_to_log(*message.reply_to_message.from)

def write_message_to_log ( message:tbotapi::message )
    write_message_to_log(message,"")

def process_chat_reply ( message:tbotapi::message; whatGodfatherSaid:GodfatherMessage; rights:ChannelRights )
    to_log(LOG_INFO, "{g_botConfig.name}: {whatGodfatherSaid}\n")
    var inscope messages <- telegram_sendLongMessage([[sendmessage
        chat_id = "{message.chat.id}",
        reply_to_message_id = message.message_id,
        text = whatGodfatherSaid.message |> sanitize_content |> fix_message
    ]])
    if !telegram_get_last_error() |> empty
        to_log(LOG_ERROR, "telegram_sendLongMessage failed: {telegram_get_last_error()}\n")
        return
    if !empty(messages)
        // we save what godfather said to the log as a raw JSON
        var json = JV(whatGodfatherSaid)
        var reply_json = write_json(json)
        unsafe
            delete json
        write_to_log([[ChatLogEntry
            chat_id = message.chat.id,
            message_id = messages[0].message_id,
            date = messages[0].date,
            from_id = g_userGodfather.id,
            reply_to_message = message.from?.id ?? 0l,
            text = reply_json,
            transcription = ""
        ]])
    process_chat_reply_draw ( message, whatGodfatherSaid, rights )
    process_char_reply_code ( message, whatGodfatherSaid, rights )

def process_char_reply_code ( message:tbotapi::message; whatGodfatherSaid:GodfatherMessage; rights:ChannelRights )
    if !empty(whatGodfatherSaid.code)
        to_log(LOG_INFO, "writing code:\n{whatGodfatherSaid.code}\n")
        telegram_sendLongMessage([[sendmessage
            chat_id = "{message.chat.id}",
            reply_to_message_id = message.message_id,
            text = whatGodfatherSaid.code,
            parse_mode = "MarkdownV2"
        ]], 4096-10,"```\n","\n```")

def process_chat_reply_draw ( message:tbotapi::message; whatGodfatherSaid:GodfatherMessage; rights:ChannelRights )
    let draw_prompt = whatGodfatherSaid.draw |> strip
    if !empty(draw_prompt)
        if !rights.draw_pictures
            to_log(LOG_INFO, "drawing image denied\n")
            return
        to_log(LOG_INFO, "drawing image {whatGodfatherSaid.draw}\n")
        var inscope image <- openai_create_image([[Image()
            prompt = draw_prompt,
            n = 1,
            size = 256,
            response_format = "b64_json"    // url
        ]])
        if !image |> is_valid
            to_log(LOG_ERROR, "openai_create_image failed: {openai_get_last_error()}\n")
            return
        if image.data |> length != 1
            to_log(LOG_ERROR, "openai_create_image failed: invalid image data\n")
            return
        // save last one to image.png
        var photo_ok = false
        let photo_file_name = get_temp_file_name("image.{message.message_id}.png")
        fopen(photo_file_name, "wb") <| $ ( f )
            if f != null
                var inscope bytes : array<uint8>
                let res = base64_decode(image.data[0].b64_json, bytes)
                f |> fwrite(bytes)
                photo_ok = true
                to_log(LOG_INFO, "image written to \"{photo_file_name}\"\n")
        if !photo_ok
            to_log(LOG_ERROR, "failed to write image to \"{photo_file_name}\"\n")
            return
        // send image to telegram
        to_log(LOG_INFO, "sending image to telegram {message.chat.id} reply to {message.message_id}\n")
        telegram_sendPhoto([[tbot::sendphoto
            chat_id = "{message.chat.id}",
            reply_to_message_id = message.message_id,
            file = photo_file_name,
            caption = whatGodfatherSaid.draw |> sanitize_content |> fix_message
        ]])
        if !telegram_get_last_error() |> empty
            to_log(LOG_ERROR, "telegram_sendLongMessage failed: {telegram_get_last_error()}\n")
            return

def generate_chat_summary ( chat_id:int64; last_n_messages:int; extp:string)
    to_log(LOG_INFO, "Generating chat summary for {chat_id} last {last_n_messages} messages\n\n\n")
    var inscope log <- collect_chat_log(chat_id, last_n_messages)
    var i = 0
    var llen = length(log)
    var total_summary = ""
    var prev_summary = ""
    while i < llen
        var text = log[i]
        while i<llen && length(text)<BOT_SUMMARY_SIZE
            text = "{text}\n{log[i]}"
            i ++
        if !empty(text)
            let summary_prompt = "Previous section summary:\n{prev_summary}\n---\nText:\n{text}\n---\nSummary of the text above {extp}:"
            to_log(LOG_INFO, "Generating summary for {length(summary_prompt)} bytes\n{summary_prompt}\n\n\n")
            let summary = generate_summary(summary_prompt)
            to_log(LOG_INFO, "Summary: {summary}\n\n")
            total_summary = "{total_summary}\n{summary}"
            prev_summary = summary
    to_log(LOG_INFO, "Overall summary: {total_summary}\n\n\n")
    return total_summary


def setup_bot_commands
    telegram_setMyCommands([[setmycommands
        commands <- [{botcommand
            command="summary", description="show chat log summary for the last n messages";
            command="samara", description="сводка по логу чата за последние n сообщений"
        }]
    ]])
    if !telegram_get_last_error() |> empty
        to_log(LOG_ERROR, "telegram_setMyCommands failed: {telegram_get_last_error()}\n")
        return

def process_bot_commands ( message:message; rights:ChannelRights )
    //! process bot commands. currently supported
    //! /summary - generate summary for the last n messages
    //! /samara - сгенерировать сводку по последним n сообщениям
    var response : string
    let summary = message.text |> starts_with("/summary@{g_botConfig.id}")
    let samara = message.text |> starts_with("/samara@{g_botConfig.id}")
    if summary || samara
        if rights.summary
            let len_cmd = length("/summary@{g_botConfig.id}")
            let num_substr = message.text |> slice(len_cmd+1)
            var max_messages = 100
            if !empty(num_substr)
                max_messages = int(num_substr)
            response = generate_chat_summary(message.chat.id, max_messages,
                summary ? "in english language" : "in russian language")
        else
            response = "Sorry, you don't have permission to generate chat summary"
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

[export]
def main
    var args <- get_command_line_arguments()
    var configPath = ""
    for arg,argi in args,count()
        if arg=="-bot-config"
            if argi == length(args)-1
                panic("expecting missing bot config file name")
            else
                configPath = args[argi+1]
    if configPath |> empty
        panic("bot config file name is not set. use -bot-config <config file name> command line argument")
    if !openai_key_is_set()
        panic("openai key is not set")
    set_use_temp_file(true) // sometimes curl removes \n from the command line input. don't know why. for now this is a workaround
    g_botConfig <- read_bot_config(configPath)
    g_adminId = int64(g_botConfig.admin)
    telegram_set_configuration(g_botConfig)
    openai_set_connection_timeout(BOT_OPENAI_TIMEOUT)
    if !create_chat_log(g_botConfig.path_to_chatlog)
        panic("failed to create chat log at {g_botConfig.path_to_chatlog}")
    var last_update = 0l
    g_userGodfather = telegram_getMe()
    if !telegram_get_last_error() |> empty
        panic("telegram_getMe failed: {telegram_get_last_error()}")
    setup_chat(g_userGodfather.id, g_botConfig.id, g_botConfig.name)
    print("{g_botConfig.name} {g_userGodfather.username} ({g_userGodfather.id}) is listening...\n")
    while true
        unsafe(heap_collect(true,true))
        if true
            var inscope res <- telegram_getupdates([[getupdates
                offset = last_update,
                limit = 100l,
                timeout = int64(BOT_UPDATE_TIMEOUT),
                allowed_updates <- [{auto "message"; "edited_message"}]
            ]])
            if !telegram_get_last_error() |> empty
                to_log(LOG_ERROR, "telegram_getupdates failed: {telegram_get_last_error()}\nwaiting {BOT_UPDATE_RETRY_TIMEOUT} seconds...")
                sleep(uint(BOT_UPDATE_RETRY_TIMEOUT))
                continue
            for r in res
                last_update = max(last_update, r.update_id+1l)
                if r.message != null
                    if process_admin_commands(*r.message, g_botConfig.id, g_adminId)
                        continue
                    var rights = get_channel_rights(r.message.chat.id)
                    if rights.chat_id==0l
                        continue
                    to_log(LOG_INFO, "message from {get_user_name(r.message.from)} at {r.message.chat.id}: {r.message.text |> sanitize_content}\n")
                    if BOT_DEBUG_MODE
                        print("message from {get_user_name(r.message.from)} at {r.message.chat.id}: {r.message.text |> sanitize_content}\n")
                        continue
                    if process_bot_commands(*r.message, rights)
                        continue
                    if r.message.reply_to_message != null
                        to_log(LOG_INFO, "reply to {get_user_name(r.message.reply_to_message.from)}: {r.message.reply_to_message.text}\n")
                    if r.message.voice!=null
                        if rights.decode_voice
                            to_log(LOG_INFO, "contains voice message\n")
                            write_message_to_log(*r.message, decode_voice_message(*r.message))
                        else
                            to_log(LOG_INFO, "ignore voice message\n")
                            write_message_to_log(*r.message)
                    else
                        write_message_to_log(*r.message)
                    if is_someone_asking_bot_to_say_something(*r.message)
                        if BOT_DOES_NOT_ANSWER
                            send_system_message(*r.message, "[THE GODFATHER IS SLEEPING]")
                            continue
                        if !rights.bot_answers
                            to_log(LOG_INFO, "ignore bot conversation request\n")
                            continue
                        to_log(LOG_INFO, "someone is asking me to say something\n")
                        let reply = generate_bot_reply(r.message.chat.id)
                        if reply.result == "debug"
                            print("debug: {reply.message}\n")
                        if reply.result != "error"
                            process_chat_reply(*r.message,reply,rights)
                        else
                            process_error_message(*r.message,reply)

