require tbot
require fio
require strings
require math
require openai/openai
require daslib/json_boost

struct BotConfig : tbot::configuration
    update_timeout : float = 5.0
    retry_timeout : float = 20.0

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

def get_user_group_name ( user:user? )
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
            fwrite(f,waveBytes)
            saved = true
    if !saved
        to_log(LOG_ERROR, "failed to save {temp_file_name}\n")
        return ""
    let temp_mp3_file_name = "{temp_file_name}.mp3"
    if !convert_to_mp3(temp_file_name,temp_mp3_file_name)
        return ""
    var inscope ru = openai_create_transcription([[Transcription() file = temp_mp3_file_name, language = "ru"]])
    if !empty(ru.error)
        to_log(LOG_ERROR,"transcription failed {ru.error}\n")
        return ""
    let transcription_text = sanitize_translation(ru.text)
    if transcription_text |> empty
        to_log(LOG_INFO,"nothing to work with - empty text\n")
        return ""
    telegram_sendMessage([[sendmessage
        chat_id = "{message.chat.id}",
        reply_to_message_id = message.message_id,
        text = transcription_text
    ]])
    if !telegram_get_last_error() |> empty
        to_log(LOG_ERROR, "telegram_sendMessage failed: {telegram_get_last_error()}\n")
        return ""
    return transcription_text

def generate_sarcastic_summary ( message : tbotapi::message; sanitized_text:string )
    let prompt = "User said: {sanitized_text}\nDon Carleone answered with a sarcastic remark  in russian:"
    var inscope completion <- openai_create_completion([[Completion()
        model = "text-davinci-003",
        prompt = prompt,
        max_tokens = 256,
        temperature = 0.7
    ]])
    if !completion |> is_valid
        to_log(LOG_ERROR, "openai_create_completion failed: {openai_get_last_error()}\n")
        return
    let whatGodfatherSaid = sanitize_translation(completion.choices[0].text)
    if whatGodfatherSaid |> empty
        to_log(LOG_INFO,"Godfather answer is nothing to work with - empty text\n")
        return
    telegram_sendMessage([[sendmessage
        chat_id = "{message.chat.id}",
        reply_to_message_id = message.message_id,
        text = "Godfather: {whatGodfatherSaid}"
    ]])
    if !telegram_get_last_error() |> empty
        debug(message)
        to_log(LOG_ERROR, "telegram_sendMessage failed: {telegram_get_last_error()}\n")
        return

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
    var botConfig <- read_bot_config(configPath)
    telegram_set_configuration(botConfig)
    var last_update = 0l
    print("Godfather is listening...\n")
    while true
        unsafe(heap_collect(true,true))
        if true
            var inscope res <- telegram_getupdates([[getupdates
                offset = last_update,
                limit = 100l,
                timeout = int64(botConfig.update_timeout),
                allowed_updates <- [{auto "message"; "edited_message"}]
            ]])
            if !telegram_get_last_error() |> empty
                let waitSeconds = uint(max(5.0,botConfig.retry_timeout*1000.0))
                to_log(LOG_ERROR, "telegram_getupdates failed: {telegram_get_last_error()}\nwaiting {waitSeconds} seconds...")
                sleep(waitSeconds)
                continue
            for r in res
                last_update = max(last_update, r.update_id+1l)
                if r.message != null
                    if r.message.voice!=null
                        let voiceMessageText = decode_voice_message(*r.message)
                        if !empty(voiceMessageText)
                            generate_sarcastic_summary(*r.message,voiceMessageText)

