require daslib/curl
require daslib/json_boost
require tbotapi
require daslib/fio
require strings

def read_bot_token
    var token = ""
    fopen("D:/Work/telegramm/GG19781978_bot/token.txt","r") <| $ (f)
        if f != null
            token = fgets(f) |> replace("\n","") |> replace("\r","")
    if token |> empty
        panic("bot token is empty")
    return token

var private BOT_TOKEN = read_bot_token()

var private BOT_TIMEOUT = 10.0

var private LAST_ERROR = ""

def private write_request_json ( JV:JsonValue? )
    var ontz = set_no_trailing_zeros(true)
    var nea = set_no_empty_arrays(true)
    var text = write_json(JV)
    set_no_trailing_zeros(ontz)
    set_no_empty_arrays(nea)
    return text

def public telegram_get_last_error
    //! return error made via last request
    return LAST_ERROR

def telegram_call ( METHOD:string; updates:auto; var res:auto(RES_TYPE) )
    var req_jv <- JV(updates)
    let headers = write_request_json(req_jv)
    unsafe
        delete req_jv
    let url = "https://api.telegram.org/bot{BOT_TOKEN}/{METHOD}"
    LAST_ERROR = ""
    POST(url,BOT_TIMEOUT,headers,[{auto "Content-Type" => "application/json"}]) <| $ (resp)
        if resp.status_code == 200
            var JV = read_json(resp.text,LAST_ERROR)
            if JV != null
                if  (JV as _object)["ok"] as _bool
                    res <- from_JV((JV as _object)["result"],type<RES_TYPE>)
                else
                    LAST_ERROR = "API returned not OK\n{resp.text}"
            else
                LAST_ERROR = "FAILED\n{resp.status_code}\n{resp.error}\n"
            unsafe
                delete JV
        else
            LAST_ERROR = "FAILED\n{resp.status_code}\n{resp.error}\n"

def telegram_getupdates ( updates:getupdates )
    var res : array<update>
    telegram_call("getupdates",updates,res)
    return <- res

def telegram_sendMessage ( msg:sendmessage )
    var res : message
    telegram_call("sendMessage",msg,res)
    return <- res

def telegram_getFile ( msg:getfile )
    var res : tbotapi::file
    telegram_call("getFile",msg,res)
    return <- res

def telegram_download ( file:tbotapi::file )
    LAST_ERROR = ""
    let url = "https://api.telegram.org/file/bot{BOT_TOKEN}/{file.file_path}"
    var resp <- DOWNLOAD(url)
    if resp.status_code == HTTP_CURL_FAILED
        LAST_ERROR = resp.error
        return <- resp.bytes
    elif resp.status_code != 200
        LAST_ERROR = "DOWNLOAD failed with status {resp.status_code}"
        return <- resp.bytes
    return <- resp.bytes

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

[export]
def main
    var inscope res <- telegram_getupdates([[getupdates
        offset = 0l,
        limit = 100l,
        timeout = 5l,
        allowed_updates <- [{auto "message"; "edited_message"}]
    ]])
    var last_chat_id = 0l
    for r in res
        if r.message != null
            print("<{r.message.message_id}> {convert_time(r.message.date)} {get_user_group_name(r.message.from)}: {r.message.text}\n")
            if r.message.voice!=null
                print("\tvoice: {r.message.voice.file_id}\n")
                var inscope file <- telegram_getFile([[getfile
                    file_id = r.message.voice.file_id
                ]])
                // file_path
                debug(r.message.voice, "voice is")
                debug(file,"file is")
                let ptf <- telegram_download(file)
                if ptf |> empty
                    print("download failed: {telegram_get_last_error()}\n")
                else
                    let temp_file_name = get_temp_file_name(file.file_path |> replace("/","_"))
                    fopen(temp_file_name,"wb") <| $ (f)
                        if f != null
                            fwrite(f,ptf)
                    print("downloaded {ptf|>length} bytes to {temp_file_name}\n")
        last_chat_id = r.message.chat.id
        // debug(r)

    /*
    var inscope msg <- telegram_sendMessage([[sendmessage
        chat_id = "{last_chat_id}}",
        text = "Bot message at {get_clock()}"
    ]])
    print("Message sent: {msg.message_id}\n")
    */
