options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module tbot shared private

require tbotapi public

require daslib/curl
require daslib/json_boost
require fio
require strings

struct public configuration
    token : string
    api_timeout : float = 10.0

def public telegram_set_configuration ( conf:configuration )
    BOT_TOKEN = conf.token
    BOT_TIMEOUT = conf.api_timeout

var private BOT_TOKEN : string

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

def private telegram_call ( METHOD:string; updates:auto; var res:auto(RES_TYPE) )
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

def public telegram_getupdates ( updates:getupdates )
    //! get updates from telegram
    var res : array<update>
    telegram_call("getupdates",updates,res)
    return <- res

def public telegram_sendMessage ( msg:sendmessage )
    //! send message to telegram char or user
    var res : message
    telegram_call("sendMessage",msg,res)
    return <- res

def public telegram_getFile ( msg:getfile )
    //! get file information from telegram
    var res : tbotapi::file
    telegram_call("getFile",msg,res)
    return <- res

def public telegram_download ( file:tbotapi::file )
    //! download file from telegram
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
