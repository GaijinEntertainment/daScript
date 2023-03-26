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

// options max_infer_passes = 100

def telegram_sendMessage ( msg:sendmessage )
    var res : message
    telegram_call("sendMessage",msg,res)
    return <- res

def get_user_group_name ( user:user? )
    if user == null
        return "unknown"
    if user.username |> empty
        return "{user.first_name} {user.last_name}"
    else
        return user.username

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
            print("<{r.message.message_id}> {get_user_group_name(r.message.from)}: {r.message.text}\n")
        last_chat_id = r.message.chat.id
        // debug(r)

    var inscope msg <- telegram_sendMessage([[sendmessage
        chat_id = "{last_chat_id}}",
        text = "Bot message at {get_clock()}"
    ]])
    print("Message sent: {msg.message_id}\n")
