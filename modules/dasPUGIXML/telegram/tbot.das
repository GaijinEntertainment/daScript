require daslib/curl
require daslib/json_boost
require tbotapi

var private BOT_TOKEN = "5991183368:AAGxwJ2ZMaVxH0Rq2__j8CE0UTUw4iyZ5cE"

var private BOT_TIMEOUT = 10.0

var private LAST_ERROR = ""

def private write_request_json ( JV:JsonValue? )
    var ontz = set_no_trailing_zeros(true)
    var nea = set_no_empty_arrays(true)
    var text = write_json(JV)
    set_no_trailing_zeros(ontz)
    set_no_empty_arrays(nea)
    return text

def telegram_getupdates ( updates:getupdates )
    var req_jv <- JV(updates)
    let headers = write_request_json(req_jv)
    unsafe
        delete req_jv
    let url = "https://api.telegram.org/bot{BOT_TOKEN}/getupdates"
    var res : array<update>
    LAST_ERROR = ""
    POST(url,BOT_TIMEOUT,headers) <| $ (resp)
        if resp.status_code == 200
            var JV = read_json(resp.text,LAST_ERROR)
            if JV != null
                if  (JV as _object)["ok"] as _bool
                    res <- from_JV((JV as _object)["result"],type<array<update>>)
                else
                    LAST_ERROR = "API returned not OK\n{resp.text}"
            else
                LAST_ERROR = "FAILED\n{resp.status_code}\n{resp.error}\n"
            unsafe
                delete JV
        else
            LAST_ERROR = "FAILED\n{resp.status_code}\n{resp.error}\n"
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
        offset = 0,
        limit = 100,
        timeout = 0,
        allowed_updates <- [{auto "message"; "edited_message"}]
    ]])
    for r in res
        if r.message != null
            print("{get_user_group_name(r.message.from)}: {r.message.text}\n")
        debug(r)
