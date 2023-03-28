require tbot
require fio
require strings
require math
require openai/openai
require daslib/json_boost
require daslib/base64
require tbot_log

struct BotConfig : tbot::configuration
    id : string
    openai_timeout : float = 30.0
    update_timeout : float = 5.0
    retry_timeout : float = 20.0
    max_chat_messages : int = 10
    chat_completion_attempts : int = 3
    chat_prompt : string

var g_botConfig : BotConfig
var g_userGodfather : user

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
    let cmd = "D:/ffmpeg/bin/ffmpeg -loglevel quiet -y -i {fin} -ar 22050 {fout}"
    let exit_code = unsafe(popen(cmd)) <| $ (f)
        pass
    if exit_code != 0
        to_log(LOG_ERROR, "convert to mp3 failed\n{cmd}\nffmpeg failed with exit code {exit_code}\n")
    return exit_code==0

def sanitize_translation ( text : string )
    return text |> replace("\n"," ") |> replace("\r"," ") |> replace("\t"," ") |> replace("\\n", " ") |> strip()

def decode_voice_message(message:tbotapi::message)
    to_log(LOG_INFO, "decoding voice message {message.voice.file_id}\n")
    var inscope file <- telegram_getFile([[getfile
        file_id = message.voice.file_id
    ]])
    if !telegram_get_last_error() |> empty
        to_log(LOG_ERROR, "telegram_getFile failed: {telegram_get_last_error()}\n")
        return ""
    var inscope waveBytes <- telegram_download(file)
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
    var inscope ru = openai_create_transcription([[Transcription() file = temp_mp3_file_name, language = "ru"]])
    if !empty(ru.error)
        to_log(LOG_ERROR,"transcription failed {ru.error}\n")
        return ""
    let transcription_text = sanitize_translation(ru.text)
    if transcription_text |> empty
        to_log(LOG_INFO,"nothing to work with - empty text\n")
        return ""
    to_log(LOG_INFO, "transcription: {transcription_text}\n")
    to_log(LOG_INFO, "sending transcription to {message.chat.id}, replying to {message.message_id}\n")
    telegram_sendMessage([[sendmessage
        chat_id = "{message.chat.id}",
        reply_to_message_id = message.message_id,
        text = transcription_text
    ]])
    if !telegram_get_last_error() |> empty
        to_log(LOG_ERROR, "telegram_sendMessage failed: {telegram_get_last_error()}\n")
        return ""
    return transcription_text

struct ChatMessage
    message_text : string
    author : string
    answered : bool = false

[skip_lock_check]
struct ChatLog
    messages : array<ChatMessage>

var g_chat_messages : table<string; ChatLog>

def accept_chat_message ( chat_id:string; message_text:string; author:string )
    to_log(LOG_INFO, "accept_chat_message {chat_id} {message_text} {author}\n")
    assume chat = g_chat_messages[chat_id]
    if !chat.messages |> empty
        var last_message & = unsafe(chat.messages |> back)
        if !last_message.answered && last_message.author == author
            last_message.message_text = "{last_message.message_text}\n{message_text}"
            return
    chat.messages |> push([[ChatMessage message_text=message_text, author=author]])
    if length(chat.messages) > g_botConfig.max_chat_messages
        chat.messages |> erase(0)

def is_someone_asking_bot_to_say_something ( message:tbotapi::message  )
    if message.text |> find("@{g_botConfig.id}") != -1
        return true
    if message.reply_to_message?.from?.id ?? 0l == g_userGodfather.id
        return true
    return false

struct GodfatherMessage
    result : string
    message : string
    summary : string
    question : string
    search : string
    draw : string
    order : string
    mood : string
    raw : string

def sanitize_chat_message ( message : string )
    return message |> replace("@{g_userGodfather.username}","") |> strip()

def sanitize_json_reply ( reply : string )
    return reply |> replace("\\n"," ") |> replace("\\r"," ") |> replace("\\t"," ") |> replace("\\n", " ") |> strip()

def generate_chat_reply ( chat_id : string; message:tbotapi::message )
    to_log(LOG_INFO, "generate_chat_reply {chat_id} {message.text}\n")
    var messages : array<ChatCompletionMessage>
    to_log(LOG_INFO, "system: {g_botConfig.chat_prompt}\n")
    messages |> push([[ChatCompletionMessage role = "system", content = g_botConfig.chat_prompt, name = "Godfather"]])
    for cm in g_chat_messages[chat_id].messages
        let role = cm.author=="Godfather" ? "assistant" : "user"
        let content = sanitize_chat_message(cm.message_text)
        to_log(LOG_INFO,"{role}: {content}\n")
        messages |> push([[ChatCompletionMessage
            role = role,
            content = content]])
    to_log(LOG_INFO,"generating chat completion\n")
    var inscope chat : ChatCompletionResponse
    for i in 0..g_botConfig.chat_completion_attempts
        chat <- openai_create_chat_completion([[ChatCompletion()
            model = "gpt-3.5-turbo",
            max_tokens = 512,
            temperature = 1.0,
            top_p = 1.0,
            messages <- messages
        ]])
        if chat |> is_valid
            break
        if !openai_get_last_error() |> starts_with("curl timeout")
            break
        to_log(LOG_ERROR, "retrying, reason: {openai_get_last_error()}\n")
        delete chat
    if !chat|> is_valid
        to_log(LOG_ERROR, "openai_create_completion failed: {openai_get_last_error()}\n")
        return [[GodfatherMessage result = "error", message = openai_get_last_error()]]
    to_log(LOG_INFO, "chat completion: {chat.choices[0].message.content}\n")
    let fnd = chat.choices[0].message.content |> find("\{")
    if fnd == -1
        to_log(LOG_ERROR, "there is no JSON there\n")
        return [[GodfatherMessage result = "error", message = "there is no JSON there", raw = chat.choices[0].message.content]]
    let json_str = chat.choices[0].message.content |> slice(fnd) |> sanitize_json_reply
    var error : string
    var json <- read_json(json_str,error)
    if !empty(error)
        unsafe
            delete json
        to_log(LOG_ERROR, "read_json failed: {error}\n")
        return [[GodfatherMessage result = "error", message = error, raw = chat.choices[0].message.content]]
    var res <- from_JV(json,type<GodfatherMessage>)
    (g_chat_messages[chat_id].messages |> back).answered = true
    res.raw = write_json(json) // save raw JSON for chat completion
    unsafe
        delete json
    return <- res

def process_chat_reply ( message:tbotapi::message; whatGodfatherSaid:GodfatherMessage )
    to_log(LOG_INFO, "Godfather: {whatGodfatherSaid}\n")
    let reply = sanitize_translation(whatGodfatherSaid.message)
    telegram_sendMessage([[sendmessage
        chat_id = "{message.chat.id}",
        reply_to_message_id = message.message_id,
        text = "Godfather: {reply}"
    ]])
    if !telegram_get_last_error() |> empty
        to_log(LOG_ERROR, "telegram_sendMessage failed: {telegram_get_last_error()}\n")
        return
    if whatGodfatherSaid.raw != ""
        accept_chat_message("{message.chat.id}",whatGodfatherSaid.raw,"Godfather")
    if whatGodfatherSaid.result=="draw" && !empty(whatGodfatherSaid.draw)
        to_log(LOG_INFO, "drawing image {whatGodfatherSaid.draw}\n")
        var inscope image <- openai_create_image([[Image()
            prompt = whatGodfatherSaid.draw,
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
            caption = "Godfather: {whatGodfatherSaid.draw}"
        ]])
        if !telegram_get_last_error() |> empty
            to_log(LOG_ERROR, "telegram_sendMessage failed: {telegram_get_last_error()}\n")
            return

def accept_godfather_message ( chat_id:string; message_text:string )
    var msg = [[GodfatherMessage result="nothing", summary="i should say something", message=message_text, mood="sarcastic"]]
    var json <- JV(msg)
    var json_str = write_json(json)
    unsafe
        delete json
    accept_chat_message(chat_id,json_str,"Godfather")

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
    g_botConfig <- read_bot_config(configPath)
    telegram_set_configuration(g_botConfig)
    openai_set_connection_timeout(g_botConfig.openai_timeout)
    var last_update = 0l
    g_userGodfather = telegram_getMe()
    if !telegram_get_last_error() |> empty
        panic("telegram_getMe failed: {telegram_get_last_error()}")
    print("Godfather {g_userGodfather.username} ({g_userGodfather.id}) is listening...\n")
    while true
        unsafe(heap_collect(true,true))
        if true
            var inscope res <- telegram_getupdates([[getupdates
                offset = last_update,
                limit = 100l,
                timeout = int64(g_botConfig.update_timeout),
                allowed_updates <- [{auto "message"; "edited_message"}]
            ]])
            if !telegram_get_last_error() |> empty
                let waitSeconds = uint(max(5.0,g_botConfig.retry_timeout*1000.0))
                to_log(LOG_ERROR, "telegram_getupdates failed: {telegram_get_last_error()}\nwaiting {waitSeconds} seconds...")
                sleep(waitSeconds)
                continue
            for r in res
                last_update = max(last_update, r.update_id+1l)
                if r.message != null
                    to_log(LOG_INFO, "message from {get_user_name(r.message.from)}: {r.message.text}\n")
                    if r.message.reply_to_message != null
                        to_log(LOG_INFO, "reply to {get_user_name(r.message.reply_to_message.from)}: {r.message.reply_to_message.text}\n")
                    if r.message.voice!=null
                        to_log(LOG_INFO, "contains voice message\n")
                        let voiceMessageText = decode_voice_message(*r.message)
                        if !empty(voiceMessageText)
                            accept_chat_message("{r.message.chat.id}",voiceMessageText,get_user_name(r.message.from))
                    else
                        accept_chat_message("{r.message.chat.id}","{r.message.text}",get_user_name(r.message.from))
                    if is_someone_asking_bot_to_say_something(*r.message)
                        to_log(LOG_INFO, "someone is asking me to say something\n")
                        let reply = generate_chat_reply("{r.message.chat.id}",*r.message)
                        if reply.result != "error"
                            process_chat_reply(*r.message,reply)
                        else
                            to_log(LOG_ERROR, "generate_chat_reply failed: {reply.message}\n{reply.raw}\n")

