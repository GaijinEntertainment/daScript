options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

// OLD das restrictions
options unsafe_table_lookup = false
options strict_smart_pointers = false
options relaxed_pointer_const = true


module tbot shared private

require tbotapi public

require dashv/dashv_boost
require daslib/json_boost
require fio
require strings

struct public configuration
    token : string

def public telegram_set_configuration ( conf:configuration )
    BOT_TOKEN = conf.token

var private BOT_TOKEN : string
var private LAST_ERROR = ""
var private LOG_API_CALLS = false

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
    if LOG_API_CALLS
        to_log(LOG_INFO, "API CALL: {url}\n{headers}\n")
    POST(url,headers,{{"Content-Type" => "application/json"}}) <| $ (resp)
        if resp.status_code == http_status OK
            peek(resp.body) <| $ ( text )
                var JV = read_json(text,LAST_ERROR)
                if JV != null
                    if  (JV as _object)["ok"] as _bool
                        static_if typeinfo(can_copy type<RES_TYPE>)
                            res = from_JV((JV as _object)["result"],type<RES_TYPE>)
                        else
                            res <- from_JV((JV as _object)["result"],type<RES_TYPE>)
                    else
                        LAST_ERROR = "API returned not OK\n{resp.body}"
                else
                    LAST_ERROR = "FAILED\n{resp.status_code}\n{resp.body}\n"
                unsafe
                    delete JV
        else
            LAST_ERROR = "FAILED\n{resp.status_code}\n{resp.body}\n"

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

def public telegram_sendLongMessage ( _msg:sendmessage; maxlength:int=4096; prefix:string=""; suffix:string="" )
    //! send long message to telegram char or user
    var msg := _msg
    var res : array<message>
    var text = msg.text
    LAST_ERROR = ""
    while !empty(text)
        if length(text) > maxlength
            msg.text = text |> slice(0, 4096)
            text = text |> slice(4096)
        else
            msg.text = text
            text = ""
        msg.text = "{prefix}{msg.text}{suffix}"
        res |> emplace <| telegram_sendMessage(msg)
        if !telegram_get_last_error() |> empty
            break
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
    var bytes : array<uint8>
    GET(url) <| $ ( resp )
        if resp.status_code != http_status OK
            LAST_ERROR = "GET failed with status {resp.status_code}\n{resp.body}"
            return
        bytes <- get_body_bytes(resp)
    return <- bytes

def public telegram_getMe
    //! get information about bot
    var res : user
    telegram_call("getMe",[[getme]],res)
    return <- res

struct public sendphoto
    chat_id : string
    file : string
    reply_to_message_id : int64
    caption : string
    parse_mode : string

def public telegram_sendPhoto ( photo : tbot::sendphoto )
    //! send photo to telegram char or user
    var settings <-{{"photo" => "@{photo.file}"; "chat_id" => "{photo.chat_id}"}}
    if photo.reply_to_message_id != 0l
        settings["reply_to_message_id"] ="{photo.reply_to_message_id}"
    if !empty(photo.caption)
        settings["caption"] = "{photo.caption}"
        if !empty(photo.parse_mode)
            settings["parse_mode"] = "{photo.parse_mode}"
    LAST_ERROR = ""
    POST("https://api.telegram.org/bot{BOT_TOKEN}/sendPhoto", "", {{"Content-Type" => "multipart/form-data"}}, settings ) <| $ ( resp )
        if resp.status_code != http_status OK
            LAST_ERROR = "HTTPS POST failed with status {resp.status_code}\n{resp.body}"

def public telegram_setMyCommands ( commands : setmycommands )
    var res : bool
    telegram_call("setMyCommands",commands,res)
    return res

/*
Underscore (_)  Asterisk (*)    Backtick (`)    Left square bracket ([)
Right square bracket (])    Left parenthesis (()    Right parenthesis ())
Tilde (~)   Greater than sign (>)   Hash (#)    Plus sign (+)   Minus sign (-)
Equals sign (=) Vertical bar (|)    Left curly brace ({)    Right curly brace (})
Period (.)  Exclamation mark (!)
*/
def public telegram_escapeMarkdownV2 ( text:string )
    return build_string <| $ ( writer )
        for Ch in text
            if Ch=='_' || Ch=='*' || Ch=='`' || Ch=='[' || Ch==']' || Ch=='(' || Ch==')' || Ch=='~' || Ch=='>' || Ch=='#' || Ch=='+' || Ch=='-' || Ch=='=' || Ch=='|' || Ch=='{' || Ch=='}' || Ch=='.' || Ch=='!'
                writer |> write_char('\\')
            writer |> write_char(Ch)