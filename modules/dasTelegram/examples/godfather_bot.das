require daslib/curl

require fio
require daslib/strings_boost
require math
require openai/openai
require daslib/json_boost
require daslib/base64
require daslib/regex_boost
require daslib/utf8_utils

require telegram/tbotlog
require telegram/tbot

let replacing_emoji <- [{auto
// regular smiles
    ":)" => "🙂";
    ":)" => "🙂";
    ":(" => "☹️";
    ":D" => "😀";
    ";)" => "😉";
    ":-|" => "😐";
// :foo: format
    ":smirk" => "😏";
    ":smile" => "😊";
    ":wink" => "😉";
    ":angry:" => "😠";
    ":neutral_face:" => "😐";
    ":grin:" => "😁";
    ":innocent:" => "😇";
    ":heart_eyes:" => "😍";
    ":kissing_heart:" => "😘";
// bad unicode?
    "ðŸ˜" => "😃";
    "ðŸ˜Ž" => "😄";
    "ðŸ§" => "👀";
    "ðŸ¤‘" => "😂";
    "ðŸ•¶ï¸" => "👍";
    "ðŸ˜’" => "😀"
}]

var broken_utf16 <- %regex~ud[89ab][0-9a-fA-F][0-9a-fA-F]ud[c-f][0-9a-fA-F][0-9a-fA-F]?%%

def fix_broken_utf16 ( str:string )
	return broken_utf16 |> regex_replace(str) <| $ ( codePoint )
		let hs = "0x{codePoint |> slice(1, 5)}"
		let ls = "0x{codePoint |> slice(6, 10)}"
		let utf32 = utf16_to_utf32(to_uint(hs, true), to_uint(ls, true))
		return string(utf8_encode(utf32))

var broken_utf32 <- %regex~u[0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F]u[0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F]u[0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F]%%

def fix_broken_utf32 ( str:string )
	return broken_utf32 |> regex_replace(str) <| $ ( codePoint )
		let l1 = "0x{codePoint |> slice(1, 5)}"
		let l2 = "0x{codePoint |> slice(6, 10)}"
		let l3 = "0x{codePoint |> slice(11, 15)}"
		let enc <- [{uint
			to_uint(l1, true);
			to_uint(l2, true);
			to_uint(l3, true)
		}]
		return string(utf8_encode(enc))

var broken_html_hex <- %regex~&#x[0-9a-fA-F]+;%%

def fix_broken_html_hex ( str:string )
	return broken_html_hex |> regex_replace(str) <| $ ( codePoint )
		let code = "0x{codePoint |> slice(3, -1)}"
		let utf32 = to_uint(code, true)
		return string(utf8_encode(utf32))

struct BotConfig : tbot::configuration
    id : string
    name : string
    openai_timeout : float = 30.0
    update_timeout : float = 5.0
    retry_timeout : float = 20.0
    max_chat_messages : int = 10
    chat_completion_attempts : int = 3
    chat_prompt : string
    chat_extra_prompt : string

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
    var inscope ru : TranscriptionResult
    for i in 0..g_botConfig.chat_completion_attempts
        ru <- openai_create_transcription([[Transcription() file = temp_mp3_file_name, language = ""]])
        if empty(ru.error)
            break
        if ! (ru.error |> starts_with("curl timeout") || ru.error |> starts_with("HTTPS post failed"))
            break
        to_log(LOG_ERROR, "retrying, reason: {ru.error}\n")
        delete ru
    if !empty(ru.error)
        to_log(LOG_ERROR,"transcription failed {ru.error}\n")
        return ""
    let transcription_text = sanitize_translation(ru.text)
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

struct ChatMessage
    message_text : string
    author : string
    answered : bool = false

[skip_lock_check]
struct ChatLog
    messages : array<ChatMessage>
    summary : string
    total_tokens : int

var g_chat_messages : table<string; ChatLog>

def reset_chat_log ( chat_id:string )
    delete g_chat_messages[chat_id]
    g_chat_messages[chat_id] <- ChatLog()

def get_chat_summary( var log:ChatLog; nmessages:int=INT_MAX )
    // now, the fallback
    let summary_prompt = build_string <| $ ( writer )
        if !empty(log.summary)
            writer |> write("Conversation so far:\n")
            writer |> write(log.summary)
        for msg,i in log.messages,range(nmessages)
            writer |> write("\n")
            writer |> write(msg.author==g_botConfig.name ? "assistant" : "user")
            writer |> write(": ")
            writer |> write(msg.message_text)
            writer |> write("\n")
        writer |> write("---\nSummary of the entire conversation:")
    // now, we make a summary
    to_log(LOG_INFO, "summary prompt:\n{summary_prompt}\n")
    var inscope completion : CreateCompletionResponse
    for i in 0..g_botConfig.chat_completion_attempts
        completion <- openai_create_completion([[Completion()
            model = "text-davinci-003",
            prompt = summary_prompt,
            max_tokens = 512,
            temperature = 0.
        ]])
        if completion |> is_valid
            break
        if !(openai_get_last_error() |> starts_with("curl timeout") || openai_get_last_error() |> starts_with("HTTPS POST failed"))
            break
        to_log(LOG_ERROR, "retrying, reason: {openai_get_last_error()}\n")
        delete completion
    if !completion|> is_valid
        to_log(LOG_ERROR, "openai_create_completion failed: {openai_get_last_error()}\n")
        return ""
    return completion.choices[0].text |> replace("\\n","") |> replace("\n","")

def prune_chat ( var log:ChatLog )
    // first lets try to get rid of message, that is not answered (i.e. was not addressed to bot)
    // and not written by bot
    var index = -1
    for msg,i in log.messages,count()
        if !msg.answered && msg.author!=g_botConfig.name
            index = i
            break
    if index != -1
        to_log(LOG_INFO, "pruning chat, removing unansered message {index} from {log.messages[index].author}\n")
        to_log(LOG_INFO, "message was:\n{log.messages[index].message_text}\n")
        log.messages |> erase(index)
        return true
    let new_summary = get_chat_summary(log,1)
    if new_summary |> empty
        return false
    to_log(LOG_INFO, "pruning chat, removing first message from {log.messages[0].author}\n")
    to_log(LOG_INFO, "message was:\n{log.messages[0].message_text}\n")
    to_log(LOG_INFO, "new summary is:\n{new_summary}\n")
    log.summary = new_summary
    log.messages |> erase(0)
    return true

def accept_chat_message ( chat_id:string; message_text:string; author:string )
    var text = message_text
    while !empty(text)
        if length(text) > 4096
            let part = text |> slice(0,4096)
            accept_single_chat_message(chat_id,part,author)
            text = text |> slice(4096)
        else
            accept_single_chat_message(chat_id,text,author)
            text = ""

def accept_single_chat_message ( chat_id:string; message_text:string; author:string )
    to_log(LOG_INFO, "accept_chat_message {chat_id} {message_text} {author}\n")
    assume chat = g_chat_messages[chat_id]
    while length(chat.messages) > g_botConfig.max_chat_messages
        if !prune_chat ( chat )
            break
    if !chat.messages |> empty
        var last_message & = unsafe(chat.messages |> back)
        if !last_message.answered && last_message.author == author
            let combined_text = "{last_message.message_text}\n{message_text}"
            if length(combined_text) < 4096
                last_message.message_text = combined_text
            return
    chat.messages |> push([[ChatMessage message_text=message_text, author=author]])

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
    draw : string
    code : string
    mood : string
    raw : string

def sanitize_chat_message ( message : string )
    return (message
        |> replace("@{g_userGodfather.username}","")
        |> replace_multiple([{auto
            "\\n" => " ";
            "\\r" => " ";
            "\\t" => " "}])
        |> strip())

def sanitize_json_reply ( reply : string )
    return build_string <| $ ( writer )
        peek_data(reply) <| $ ( str )
            var i = 0
            let len = length(str)
            var instring = false
            while i < len
                if str[i] == '"'u8
                    instring = !instring
                elif str[i] == '\\'u8
                    if i+1 < len
                        let nc = str[i+1]
                        if instring // we stay in string if we have escaped quote
                            writer |> write_char('\\')
                            writer |> write_char(int(nc))
                            i += 2
                            continue
                        if nc=='n'u8 || nc=='r'u8 || nc=='t'u8  // we replace \n,\r,\t with ' '
                            writer |> write_char(' ')
                            i += 2
                            continue
                writer |> write_char(int(str[i]))   // we write everything else
                i ++

def flatten_json_reply ( var writer:StringBuilderWriter; var reply : JsonValue? )
    if reply is _object
        for key in (reply as _object) |> keys
            flatten_json_reply(writer, (reply as _object)[key])
    elif reply is _array
        for item in reply as _array
            flatten_json_reply(writer, item)
    elif reply is _string
        writer |> write(reply as _string)
        writer |> write(" ")
    elif reply is _number
        writer |> write("{reply as _number}")
        writer |> write(" ")
    elif reply is _bool
        writer |> write("{reply as _bool}")
        writer |> write(" ")

def adjust_completion ( var json : JsonValue? )
    //! here is collection of workarounds for the mallformed output
    if json == null
        return
    // this workaround is when it starts providing object after the draw, instead of description
    assume obj = json as _object
    if obj |> key_exists("draw")
        if obj["draw"] is _object
            assume draw = obj["draw"] as _object
            var req = build_string <| $ ( writer )
                flatten_json_reply(writer, obj["draw"])
            obj |> erase("draw")
            obj["draw"] = JV(req)
            to_log(LOG_INFO, "adjusting draw to {req}\n")

def generate_chat_reply ( chat_id : string; message:tbotapi::message; var tokens:int& )
    to_log(LOG_INFO, "generate_chat_reply {chat_id} {message.text}\n")
    tokens = 0
    var messages : array<ChatCompletionMessage>
    to_log(LOG_INFO, "system: {g_botConfig.chat_prompt}\n")
    messages |> push([[ChatCompletionMessage role="system", content=g_botConfig.chat_prompt, name=g_botConfig.name]])
    let summary = g_chat_messages[chat_id].summary
    if !empty(summary)
        to_log(LOG_INFO, "system: {summary}\n")
        messages |> push([[ChatCompletionMessage role="system", content="Summary of the conversation so far: {summary}", name=g_botConfig.name]])
    for cm in g_chat_messages[chat_id].messages
        let role = cm.author==g_botConfig.name ? "assistant" : "user"
        let content = sanitize_chat_message(cm.message_text)
        to_log(LOG_INFO,"{role}: {content}\n")
        messages |> push([[ChatCompletionMessage
            role = role,
            name = cm.author==g_botConfig.name ? "" : cm.author,    // we are skipping this, because it goes bonkers otherwise
            content = content]])
    messages |> push([[ChatCompletionMessage
        role = "system",
        content = g_botConfig.chat_extra_prompt]])
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
        if !(openai_get_last_error() |> starts_with("curl timeout") || openai_get_last_error() |> starts_with("HTTPS POST failed"))
            break
        to_log(LOG_ERROR, "retrying, reason: {openai_get_last_error()}\n")
        delete chat
    if !chat|> is_valid
        to_log(LOG_ERROR, "openai_create_chat_completion failed: {openai_get_last_error()}\n")
        return [[GodfatherMessage result = "error", message = openai_get_last_error()]]
    to_log(LOG_INFO, "chat completion: {chat.choices[0].message.content}\n")
    to_log(LOG_INFO, "usage:\n")
    to_log(LOG_INFO, "- prompt tokens {chat.usage.prompt_tokens}\n")
    to_log(LOG_INFO, "- completion tokens {chat.usage.completion_tokens}\n")
    to_log(LOG_INFO, "- total tokens {chat.usage.total_tokens}\n")
    tokens = chat.usage.total_tokens
    let fnd = chat.choices[0].message.content |> find("\{")
    if fnd == -1
        to_log(LOG_ERROR, "there is no JSON there\n")
        return [[GodfatherMessage result = "error", message = "there is no JSON there", raw = chat.choices[0].message.content]]
    let json_str = chat.choices[0].message.content |> slice(fnd) |> sanitize_json_reply
    to_log(LOG_INFO, "json_str: {json_str}\n")
    var error : string
    let old_adc = set_allow_duplicate_keys(true)
    var json <- read_json(json_str,error)
    if json == null
        to_log(LOG_ERROR, "read_json failed: {error}; trying to fix\n")
        let fixed_json = try_fixing_broken_json(json_str)
        to_log(LOG_ERROR, "fixed_json: {fixed_json}\n")
        var fixed_error : string
        var new_json <- read_json(fixed_json,error)
        if new_json != null
            to_log(LOG_ERROR, "that fixed it\n")
            json <- new_json
            error = fixed_error
    set_allow_duplicate_keys(old_adc)
    if !empty(error)
        unsafe
            delete json
        to_log(LOG_ERROR, "read_json failed: {error}\n")
        return [[GodfatherMessage result = "error", message = error, raw = chat.choices[0].message.content]]
    adjust_completion(json)
    var res <- from_JV(json,type<GodfatherMessage>)
    (g_chat_messages[chat_id].messages |> back).answered = true
    res.raw = write_json(json) // save raw JSON for chat completion
    unsafe
        delete json
    return <- res

def process_error_message ( message:tbotapi::message; whatGodfatherSaid:GodfatherMessage )
    to_log(LOG_ERROR, "generate_chat_reply failed: {whatGodfatherSaid.message}\n{whatGodfatherSaid.raw}\n")
    telegram_sendLongMessage([[sendmessage
        chat_id = "{message.chat.id}",
        reply_to_message_id = message.message_id,
        text = "{g_botConfig.name} (debug message):```\n{whatGodfatherSaid.message}\n{whatGodfatherSaid.raw}\n```" |> telegram_escapeMarkdownV2,
        parse_mode = "MarkdownV2"
    ]])
    if !telegram_get_last_error() |> empty
        to_log(LOG_ERROR, "telegram_sendLongMessage failed: {telegram_get_last_error()}\n")

def fix_mood ( mood:string )
    return mood |> replace_multiple(replacing_emoji) |> fix_broken_utf16 |> fix_broken_utf32 |> fix_broken_html_hex

def fix_message ( text:string )
    return text |> replace_multiple(replacing_emoji) |> fix_broken_utf16 |> fix_broken_utf32 |> fix_broken_html_hex

def process_chat_reply ( message:tbotapi::message; whatGodfatherSaid:GodfatherMessage )
    to_log(LOG_INFO, "{g_botConfig.name}: {whatGodfatherSaid}\n")
    let reply = whatGodfatherSaid.message |> sanitize_translation |> fix_message
    let mood = whatGodfatherSaid.mood |> fix_mood
    telegram_sendLongMessage([[sendmessage
        chat_id = "{message.chat.id}",
        reply_to_message_id = message.message_id,
        text = "{g_botConfig.name}: {reply} {mood}"
    ]])
    if !telegram_get_last_error() |> empty
        to_log(LOG_ERROR, "telegram_sendLongMessage failed: {telegram_get_last_error()}\n")
        return
    if whatGodfatherSaid.raw != ""
        accept_chat_message("{message.chat.id}",whatGodfatherSaid.raw,"Godfather")
    process_chat_reply_draw ( message, whatGodfatherSaid )
    process_char_reply_code ( message, whatGodfatherSaid )

def process_char_reply_code ( message:tbotapi::message; whatGodfatherSaid:GodfatherMessage )
    if whatGodfatherSaid.result=="code" && !empty(whatGodfatherSaid.code)
        to_log(LOG_INFO, "writing code:\n{whatGodfatherSaid.code}\n")
        telegram_sendLongMessage([[sendmessage
            chat_id = "{message.chat.id}",
            reply_to_message_id = message.message_id,
            text = whatGodfatherSaid.code,
            parse_mode = "MarkdownV2"
        ]], 4096-10,"```\n","\n```")

def process_chat_reply_draw ( message:tbotapi::message; whatGodfatherSaid:GodfatherMessage )
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
            caption = "{g_botConfig.name}: {whatGodfatherSaid.draw}"
        ]])
        if !telegram_get_last_error() |> empty
            to_log(LOG_ERROR, "telegram_sendLongMessage failed: {telegram_get_last_error()}\n")
            return

def accept_godfather_message ( chat_id:string; message_text:string )
    var msg = [[GodfatherMessage result="nothing", summary="i should say something", message=message_text, mood="sarcastic"]]
    var json <- JV(msg)
    var json_str = write_json(json)
    unsafe
        delete json
    accept_chat_message(chat_id,json_str,g_botConfig.name)

def setup_bot_commands
    telegram_setMyCommands([[setmycommands
        commands <- [{botcommand
            command="test", description="verify one of the internal bot features";
            command="reset", description="reset conversation in this chat";
            command="summary", description="get a summary of the chat";
            command="completion_summary", description="show completion summary";
            command="status", description="show current chat status"
        }]
    ]])
    if !telegram_get_last_error() |> empty
        to_log(LOG_ERROR, "telegram_setMyCommands failed: {telegram_get_last_error()}\n")
        return

def dump_chat_status ( chat:ChatLog )
    return build_string <| $ ( writer )
        writer |> write("```\n")
        writer |> write(chat.summary)
        writer |> write("\n")
        for msg in chat.messages
            writer |> write(msg.author)
            writer |> write(": ")
            writer |> write(msg.message_text)
            writer |> write("\n")
        writer |> write("```\n")
        writer |> write("in {chat.total_tokens} tokens\n")

def process_bot_commands ( message:message )
    var response : string
    if message.text == "/test@{g_botConfig.id}"
        response = build_string <| $ ( writer )
            writer |> write("```\n\n code block \n\n line 2 \n\n line 3 \n\n```")
    elif message.text == "/summary@{g_botConfig.id}"
        response = get_chat_summary(g_chat_messages["{message.chat.id}"])
    elif message.text == "/completion_summary@{g_botConfig.id}"
        response = g_chat_messages["{message.chat.id}"].summary
        if response |> empty
            response = "no completion summary available"
    elif message.text == "/status@{g_botConfig.id}"
        response = dump_chat_status(g_chat_messages["{message.chat.id}"])
    elif message.text == "/reset@{g_botConfig.id}"
        reset_chat_log("{message.chat.id}")
        response = "conversation history has been reset"
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
    telegram_set_configuration(g_botConfig)
    openai_set_connection_timeout(g_botConfig.openai_timeout)
    var last_update = 0l
    g_userGodfather = telegram_getMe()
    if !telegram_get_last_error() |> empty
        panic("telegram_getMe failed: {telegram_get_last_error()}")
    print("{g_botConfig.name} {g_userGodfather.username} ({g_userGodfather.id}) is listening...\n")
    setup_bot_commands()
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
                    if process_bot_commands(*r.message)
                        continue
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
                        var tokens = 0
                        let reply = generate_chat_reply("{r.message.chat.id}",*r.message, tokens)
                        if reply.result != "error"
                            g_chat_messages["{r.message.chat.id}"].total_tokens = tokens
                            process_chat_reply(*r.message,reply)
                        else
                            process_error_message(*r.message,reply)


