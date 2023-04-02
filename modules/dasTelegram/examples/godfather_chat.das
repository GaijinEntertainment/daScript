options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module godfather_chat shared private

require fio
require math

require daslib/algorithm
require daslib/stringify
require daslib/strings_boost
require daslib/json_boost
require daslib/utf8_utils

require openai/openai

require chat_log
require emoji

let BOT_USE_CHAT = false

var BOT_ID = 5991183368l
var BOT_NAME = "GG19781978_bot"
var BOT_NICKNAME = "Godfather"

let BOT_TOTAL_CONTEXT_MESSAGES = 100
let BOT_TOTAL_MESSAGES = 20

let BOT_RETRY_ATTEMPTS = 3
let BOT_RETRY_DELAY = 20        // in seconds

let BOT_SUMMARY_SIZE = 4096
let BOT_SHORTCUT_SUMMARY_SIZE = 1024+512

let BOT_TEST_SKIP_LAST_N_MESSAGES = 0

let BOT_CHAT_PROMPT_PREFIX = %stringify~
Pretend to be Vito Carleone aka Godfather from the movie The Godfather.
You can draw pictures and write code.
Answer in detail in the same language as the question.
You reply in well formed JPEG encoded in utf8. Examples:
Q. when u need to draw
JSON: {"result":"draw","draw":"very detailed description of whatever user asked to draw","message":"you answer followed by emoji"}
Q. when u need to write code
JSON: {"result":"code","code":"text of the code goes here","message":"you answer followed by emoji"}
Q. general questions
JSON: {"result":"nothing","message":"you answer followed by emoji"}
%%

let BOT_CHAT_PROMPT_SUFFIX = "JSON:"

def public sanitize_content ( text : string )
    return text |> fix_broken_utf32 |> strip

struct public GodfatherMessage
    result : string
    message : string
    draw : string
    code : string

def generate_chat_completion_result  ( messages : array<ChatCompletionMessage> ) : GodfatherMessage
    //! generate completion result given chat completion messages
    to_log(LOG_INFO,"\n\n\ngenerating chat completion:\n")
    for msg in messages
        to_log(LOG_INFO,"{msg.role} <{msg.name}>:  {msg.content}\n")
    var chat : ChatCompletionResponse
    for i in range(BOT_RETRY_ATTEMPTS)
        chat <- openai_create_chat_completion([[ChatCompletion()
            model = "gpt-3.5-turbo",
            max_tokens = 512,
            temperature = 1.0,
            top_p = 1.0,
            messages := messages
        ]])
        if chat |> is_valid
            break
        if !(openai_get_last_error() |> starts_with("curl timeout") || openai_get_last_error() |> starts_with("HTTPS POST failed"))
            break
        to_log(LOG_ERROR, "retrying, reason: {openai_get_last_error()}\n")
        delete chat
        sleep(uint(BOT_RETRY_DELAY)*1000u)
    if !chat|> is_valid
        to_log(LOG_ERROR, "openai_create_chat_completion failed: {openai_get_last_error()}\n")
        return [[GodfatherMessage result="error", message=openai_get_last_error()]]
    to_log(LOG_INFO, "chat completion: {chat.choices[0].message.content}\n")
    to_log(LOG_INFO, "usage:\n")
    to_log(LOG_INFO, "- prompt tokens {chat.usage.prompt_tokens}\n")
    to_log(LOG_INFO, "- completion tokens {chat.usage.completion_tokens}\n")
    to_log(LOG_INFO, "- total tokens {chat.usage.total_tokens}\n")
    return [[GodfatherMessage result="ok", message=chat.choices[0].message.content]]

def get_message_content ( entry : ChatLogEntry; use_js : bool = true )
    let isBot = entry.from_id==BOT_ID
    if isBot
        var content = entry.text
        var error : string
        var json = content |> read_json(error)
        if json==null || !empty(error)
            if use_js
                content = "\{ \"result\":\"nothing\", \"message\":\"{escape(content)}\" \}"
            else
                content = content |> strip()
        else
            if use_js
                if json is _object
                    var obj & = unsafe(json as _object)
                    if obj |> key_exists("raw")
                        obj |> erase("raw")
                    if obj |> key_exists("mood")
                        obj |> erase("mood")
                content = write_request_json(json) |> sanitize_json_reply |> strip()
            else
                if json is _object
                    var obj & = unsafe(json as _object)
                    if obj |> key_exists("message")
                        var msg & = unsafe(obj["message"])
                        if msg is _string
                            content = (msg as _string) |> strip
        unsafe
            delete json
        return content
    else
        return "{entry.text |> sanitize_content} {entry.transcription |> sanitize_content}" |> strip

def generate_completion_request ( chat_id : int64 )
    //! generate completion request given chat id
    // lets get previous conversation summary first
    let summary_so_far = generate_previous_conversation_summary(chat_id,"in english")
    // build user lookup
    var users : array<UserEntry>
    list_chat_users(chat_id) <| $ ( user )
        users |> push(user)
    var user_lookup : table<int64; UserEntry>
    for user in users
        user_lookup[user.user_id] = user
    // get chat entries
    var entries : array<ChatLogEntry>
    entries |> reserve(BOT_TOTAL_MESSAGES)
    list_conversation_log(chat_id,BOT_ID,BOT_NAME,BOT_TOTAL_MESSAGES) <| $ ( msg )
        entries |> push(msg)
    entries |> reverse
    // this is here for debugging only
    for i in range(BOT_TEST_SKIP_LAST_N_MESSAGES)
        entries |> pop
    // fill completion messages
    var messages : array<ChatCompletionMessage>
    messages |> push([[ChatCompletionMessage role="system",
        content="It is {get_clock()}\n{BOT_CHAT_PROMPT_PREFIX}"
    ]])
    if !empty(summary_so_far)
        messages |> push([[ChatCompletionMessage role="system", content="Summary of the conversation so far: {summary_so_far}"]])
    for entry in entries
        let isBot = entry.from_id==BOT_ID
        let content = get_message_content(entry)
        if isBot
            messages |> push([[ChatCompletionMessage role="assistant", content=content]])
        else
            let user = user_lookup[entry.from_id]
            let clean_content = content |> sanitize_content |> replace("@{BOT_NAME}","")
            messages |> push([[ChatCompletionMessage role="user", name=user.user_name, content = "{user.first_name} {user.last_name}: {clean_content}"]])
    messages |> push([[ChatCompletionMessage role="system", name="", content=BOT_CHAT_PROMPT_SUFFIX]])
    return <- messages

def flatten_json_reply ( var writer:StringBuilderWriter; var reply : JsonValue? )
    //! this function flattens the json reply into a string
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

def fix_malformed_completion ( var json : JsonValue? )
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

def try_parse_json ( var json_str:string; var error:string& ) : JsonValue?
    //! this is a workaround for the broken json, which bot produces all the time
    let ofs = json_str |> find("\{")
    if ofs==-1
        // there is no opening brace, so its just answer. let see
        var resp : GodfatherMessage
        resp.result = "nothing"
        resp.message = json_str
        error = ""
        return JV(resp)
    json_str = json_str |> slice(ofs)
    var old_adc = set_allow_duplicate_keys(true)
    var json <- read_json(json_str,error)
    set_allow_duplicate_keys(old_adc)
    if json != null
        error = ""
        return json
    to_log(LOG_ERROR, "read_json failed: {error}; trying to fix\n")
    let fixed_json = try_fixing_broken_json(json_str)
    to_log(LOG_ERROR, "fixed_json: {fixed_json}\n")
    old_adc = set_allow_duplicate_keys(true)
    var new_json <- read_json(fixed_json,error)
    set_allow_duplicate_keys(old_adc)
    if new_json != null
        to_log(LOG_ERROR, "that fixed it\n")
        return new_json
    to_log(LOG_ERROR, "that didn't fix it\n")
    return null

def parse_chat_reply ( reply : GodfatherMessage )
    //! parse chat reply
    let json_str = reply.message
    to_log(LOG_INFO, "parse_chat_reply: {json_str}\n")
    if empty(json_str)
        to_log(LOG_WARNING, "empty json_str\n")
        return [[GodfatherMessage result="error", message="says nothing in return"]]
    var error : string
    var json <- try_parse_json(json_str,error)
    if !empty(error) || json==null
        unsafe
            delete json
        to_log(LOG_ERROR, "read_json failed: {error}\n")
        return [[GodfatherMessage result="error", message=error]]
    fix_malformed_completion(json)
    var res = from_JV(json,type<GodfatherMessage>)
    unsafe
        delete json
    return res

def public generate_chat_reply ( chat_id : int64 )
    //! given chat_id generate bot reply
    var messags <- generate_completion_request(chat_id)
    var result = generate_chat_completion_result(messags)
    if result.result == "error"
        return result
    return parse_chat_reply(result)

def public convert_completion ( messages : array<ChatCompletionMessage> )
    return build_string <| $ ( writer )
        for message in messages
            writer |> write(message.content)
            if message.role=="system"
                writer |> write("\n")
            writer |> write("\n")

def public generate_completion_reply ( chat_id : int64 )
    //! given chat_id generate bot reply
    var messags <- generate_completion_request(chat_id)
    let summary <- convert_completion(messags)
    let completion = generate_completion(summary)
    var result = [[GodfatherMessage result="ok", message="{completion}"]]
    if result.result == "error"
        return result
    return parse_chat_reply(result)

def public generate_bot_reply ( chat_id : int64 )
    if BOT_USE_CHAT
        return generate_chat_reply(chat_id)
    else
        return generate_completion_reply(chat_id)

def public setup_chat ( botid : int64; botname, botNickName : string )
    //! setup chat
    BOT_ID = botid
    BOT_NAME = botname
    BOT_NICKNAME = botNickName
    to_log(LOG_INFO, "setup_chat: {BOT_ID} {BOT_NAME}\n")

def sanitize_json_reply ( reply : string )
    //! take json reply, remove all \n,\r,\t and replace them with spaces. remove \n\r\t from inbetween quotes when broken
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
                elif str[i] == '\r'u8 || str[i] == '\n'u8
                    if instring
                        writer |> write_char(' ')
                    i ++
                    continue
                writer |> write_char(int(str[i]))   // we write everything else
                i ++

def get_summary_message ( entry:ChatLogEntry; var user_lookup:table<int64; UserEntry>)
    let user = user_lookup[entry.from_id]
    var text = user.is_bot ? BOT_NICKNAME : "{user.first_name} {user.last_name}"
    let content = get_message_content(entry, false)
    return "{text}: {content}"

def public generate_previous_conversation_summary ( chat_id:int64; extra_prompt:string )
    var users : array<UserEntry>
    list_chat_users(chat_id) <| $ ( user )
        users |> push(user)
    var user_lookup : table<int64; UserEntry>
    for user in users
        user_lookup[user.user_id] = user
    // get chat entries
    var entries : array<ChatLogEntry>
    entries |> reserve(BOT_TOTAL_MESSAGES)
    list_conversation_log(chat_id,BOT_ID,BOT_NAME,BOT_TOTAL_CONTEXT_MESSAGES) <| $ ( msg )
        entries |> push(msg)
    if entries |> empty
        return ""
    to_log(LOG_INFO, "total entries: {length(entries)}\n")
    entries |> reverse
    to_log(LOG_INFO, "total entries after the reverse: {length(entries)}\n")
    // latest are at the end, so we need to remove some
    entries |> resize ( max(0,length(entries)-(BOT_TOTAL_MESSAGES-1)) )
    var dayZ = -1L
    if !empty(entries)
        dayZ = (entries |> back).id
        entries |> pop
    // let see if we need to skip some entires, for we already have summary
    var latest = get_first_summary(chat_id, dayZ )
    dayZ = latest.id
    // now, lets start building completion request
    var llen = length(entries)
    var prev_summary = latest.text
    var i = 0
    var all_perfect = true
    var last_id = 0l
    while i < llen
        if entries[i].id <= dayZ
            i ++
            continue
        var text = get_summary_message(entries[i],user_lookup)
        i ++
        while i<llen && length(text)<BOT_SUMMARY_SIZE
            text = "{text}\n{get_summary_message(entries[i],user_lookup)}"
            last_id = entries[i].id
            i ++
        if !empty(text)
            if i >= llen
                // if its the last summary, and we can just fit it in the small limit, we will return as is
                let shortcut = "{prev_summary}\n{text}\n"
                if length(shortcut) < BOT_SHORTCUT_SUMMARY_SIZE
                    to_log(LOG_INFO, "Shortcut summary:\n{shortcut}\n\n\n")
                    return shortcut
            var summary_prompt = "Previous conversation:\n\n{prev_summary}\n\n---\n\n{text}\n\n---\nCombined detailed summary of both conversations {extra_prompt}:"
            let summary = generate_completion(summary_prompt)
            if !empty(summary) && all_perfect
                write_summary([[SummaryEntry
                    chat_id = chat_id,
                    id = last_id,
                    text = summary
                ]])
            else
                all_perfect = false // we will not write summary if we had any errors
            prev_summary = summary
    to_log(LOG_INFO, "Overall summary:\n{prev_summary}\n\n\n")
    return prev_summary

def public generate_completion ( summary_prompt:string )
    to_log(LOG_INFO, "generate_completion for prompt:\n{summary_prompt}\n\n")
    var completion : CreateCompletionResponse
    for i in range(BOT_RETRY_ATTEMPTS)
        completion <- openai_create_completion([[Completion()
            model = "text-davinci-003",
            prompt = summary_prompt |> fix_broken_escaping,
            max_tokens = 1024,
            temperature = 0.
        ]])
        if completion |> is_valid
            break
        if !(openai_get_last_error() |> starts_with("curl timeout") || openai_get_last_error() |> starts_with("HTTPS POST failed"))
            break
        to_log(LOG_ERROR, "retrying, reason: {openai_get_last_error()}\n")
        delete completion
        sleep(uint(BOT_RETRY_DELAY)*1000u)
    if !completion|> is_valid
        to_log(LOG_ERROR, "openai_create_completion failed: {openai_get_last_error()}\n")
        return ""
    let completion_text = completion.choices[0].text |> replace_multiple([{auto"\r" => ""; "\n" => " "}]) |> strip
    to_log(LOG_INFO, "completion:\n{completion_text}\n")
    to_log(LOG_INFO, "usage:\n")
    to_log(LOG_INFO, "      prompt_tokens={completion.usage.prompt_tokens}\n")
    to_log(LOG_INFO, "      completion_tokens={completion.usage.completion_tokens}\n")
    to_log(LOG_INFO, "      total_tokens={completion.usage.total_tokens}\n\n")
    return completion_text
