require tbot
require daslib/fio
require strings
require math
require openai/openai

def read_bot_token(tokenfile:string)
    var token = ""
    fopen(tokenfile,"r") <| $ (f)
        if f != null
            token = fgets(f) |> replace("\n","") |> replace("\r","")
    if token |> empty
        panic("bot token is empty")
    return token

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
        print("{cmd}\nffmpeg failed with exit code {exit_code}\n")
    return exit_code==0

def sanitize_translation ( text : string )
    return text |> replace("\n"," ") |> replace("\r"," ") |> replace("\t"," ")

[export]
def main
    print("Godfather is listening...\n")
    if !openai_key_is_set()
        panic("openai key is not set")
    telegram_set_configuration([[configuration
        token = read_bot_token("D:/Work/telegramm/GG19781978_bot/token.txt")
    ]])
    var last_update = 0l
    while true
        unsafe(heap_collect(true,true))
        var res <- telegram_getupdates([[getupdates
            offset = last_update,
            limit = 100l,
            timeout = 5l,
            allowed_updates <- [{auto "message"; "edited_message"}]
        ]])
        for r in res
            print("update {r.update_id}\n")
            last_update = max(last_update, r.update_id+1l)
            if r.message != null
                print("<{r.message.message_id}> {convert_time(r.message.date)} {get_user_group_name(r.message.from)}: {r.message.text}\n")
                if r.message.voice!=null
                    print("\tvoice: {r.message.voice.file_id}\n")
                    var inscope file <- telegram_getFile([[getfile
                        file_id = r.message.voice.file_id
                    ]])
                    var inscope ptf <- telegram_download(file)
                    if ptf |> empty
                        print("download failed: {telegram_get_last_error()}\n")
                    else
                        let temp_file_name = get_temp_file_name(file.file_path |> replace("/","_"))
                        fopen(temp_file_name,"wb") <| $ (f)
                            if f != null
                                fwrite(f,ptf)
                        print("downloaded {ptf|>length} bytes to {temp_file_name}\n")
                        let temp_mp3_file_name = "{temp_file_name}.mp3"
                        if convert_to_mp3(temp_file_name,temp_mp3_file_name)
                            print("converted to mp3 into {temp_mp3_file_name}\n")
                            var inscope ru = openai_create_transcription([[Transcription() file = temp_mp3_file_name, language = "ru"]])
                            if !empty(ru.error)
                                print("transcription failed\n")
                            else
                                print("transcription:\n{ru.text}\n\n")
                                telegram_sendMessage([[sendmessage
                                    chat_id = "{r.message.chat.id}",
                                    reply_to_message_id = r.message.message_id,
                                    text = sanitize_translation(ru.text)
                                ]])
                                print("sending to {r.message.chat.id} done\n")
                                let prompt = "{ru.text}\n----------------\nDon Carleone would answer sarcastically: "
                                print("generating completion for {prompt}\n")
                                var inscope completion <- openai_create_completion([[Completion()
                                    model = "text-davinci-001",
                                    prompt = prompt,
                                    max_tokens = 256,
                                    temperature = 1.
                                ]])
                                if completion |> is_valid
                                    print("completion is: {completion.choices[0].text}\n")
                                    telegram_sendMessage([[sendmessage
                                        chat_id = "{r.message.chat.id}",
                                        reply_to_message_id = r.message.message_id,
                                        text = sanitize_translation("Godfather: {completion.choices[0].text}")
                                    ]])
                                else
                                    print("completion failed\n")
                        else
                            print("conversion failed\n")
        delete res

