options persistent_heap
options gc

require dashv/dashv_boost
require daslib/json_boost

require fio

let private LOG_RESPONSES = false

var private OPENAI_API_KEY = get_env_variable("OPENAI_API_KEY")

struct public ListEnginesResponse
    error : string
    object : string
    data : array<Model>

def public is_valid(object:ListEnginesResponse)
    return object.object=="list"

struct public Model
    error : string
    id : string
    object : string
    created : int
    owned_by : string
    permission : array<ModelPermission>
    root : string
    // parent : null

def public is_valid ( object:Model )
    return object.object=="model"

struct public ModelPermission
    id : string
    object : string
    created : int
    allow_create_engine : bool
    allow_sampling : bool
    allow_logprobs : bool
    allow_search_indices : bool
    allow_view : bool
    allow_fine_tuning : bool
    organization : string
    // group : null,
    is_blocking : bool


def private log_resp ( resp : HttpResponse? )
    if LOG_RESPONSES
        if resp!=null
            print("GET {resp.status_code} {resp.body}\n")
        else
            print("GET null\n")

def public openai_api_key
    if empty(OPENAI_API_KEY)
        panic("OpenAI API key is not set")
    return OPENAI_API_KEY

def public openai_list
    var list : ListEnginesResponse
    var valid = false
    GET("https://api.openai.com/v1/models",
        {{"Authorization" => "Bearer {OPENAI_API_KEY}"}}
    ) <| $ ( var resp:HttpResponse? )
        log_resp(resp)
        if resp != null
            peek(resp.body) <| $ ( text )
                var JV = read_json(text,list.error)
                if is_valid_response(JV)
                    list <- from_JV(JV,type<ListEnginesResponse>)
                else
                    list.error = collect_error(JV)
        else
            list.error = "HTTPS GET failed"
    return <- list

def collect_error ( var json:JsonValue?; default_message:string = "invalid response" ) : string
    if json!=null
        if json is _object
            if (json as _object) |> key_exists("error")
                var error = (json as _object)["error"]
                if (error is _object) && ((error as _object) |> key_exists("message"))
                    var message = (error as _object)["message"]
                    if message is _string
                        return message as _string
    return default_message

def is_valid_response ( json:JsonValue? ) : bool
    if json!=null
        if json is _object
            if (json as _object) |> key_exists("object")
                return true
    return false

def public openai_retrive_model ( model_id:string )
    // retrive a model by id
    var model : Model
    var valid = false
    GET("https://api.openai.com/v1/models/{model_id}",
        {{"Authorization" => "Bearer {OPENAI_API_KEY}"}}
    ) <| $ ( var resp:HttpResponse? )
        log_resp(resp)
        if resp != null
            peek(resp.body) <| $ ( text )
                var JV = read_json(text,model.error)
                if is_valid_response(JV)
                    model <- from_JV(JV,type<Model>)
                else
                    model.error = collect_error(JV)
        else
            model.error = "HTTPS GET failed"
    return <- model

struct public Completion
    model:string
    prompt:string = ""
    suffix:string = ""
    max_tokens:int = 16
    temperature:float = 1.0             // 0..2
    top_p:float = 1.0                   // 0..1
    n:int = 1                           // how many completions to generate
    stop:array<string>                  // stop sequence (up to 4)
    presence_penalty:float = 0.0        // Number between -2.0 and 2.0. Positive values penalize new tokens based on whether they appear in the text so far, increasing the model's likelihood to talk about new topics.
    frequency_penalty:float = 0.0       // Number between -2.0 and 2.0. Positive values penalize new tokens based on their existing frequency in the text so far, decreasing the model's likelihood to repeat the same line verbatim.
    best_of:int = 1                     // Number of completions to consider when averaging logprobs or returning samples
    user:string = ""                    // User ID for logged metrics

def private write_request_json ( JV:JsonValue? )
    var ontz = set_no_trailing_zeros(true)
    var nea = set_no_empty_arrays(true)
    var text = write_json(JV)
    set_no_trailing_zeros(ontz)
    set_no_empty_arrays(nea)
    return text

struct public CreateCompletionResponse
    error : string
    id : string
    object : string
    created : int
    model : string
    choices : array<CreateCompletionResponseChoicesInner>
    usage : CreateCompletionResponseUsage

struct public CreateCompletionResponseUsage
    prompt_tokens : int
    completion_tokens : int
    total_tokens : int

def is_valid ( object:CreateCompletionResponse )
    return object.object=="text_completion"

struct public CreateCompletionResponseChoicesInner
    text : string
    index : int
    finish_reason : string

def public openai_create_completion ( completion:Completion )
    var req_jv <- JV(completion)
    if empty(completion.suffix)
        req_jv as _object |> erase("suffix")
    var req = write_request_json(req_jv)
    var ccr : CreateCompletionResponse
    POST("https://api.openai.com/v1/completions", req,
        {{"Authorization" => "Bearer {OPENAI_API_KEY}";
          "Content-Type" => "application/json"}}
    ) <| $ ( var resp:HttpResponse? )
        log_resp(resp)
        if resp != null
            peek(resp.body) <| $ ( text )
                var JV = read_json(text,ccr.error)
                if is_valid_response(JV)
                    ccr <- from_JV(JV,type<CreateCompletionResponse>)
                else
                    ccr.error = collect_error(JV)
        else
            ccr.error = "HTTPS POST failed"
    return <- ccr

[export]
def main
    // test list
    var list <- openai_list()
    if list |> is_valid
        debug(list)
    else
        print("error: {list.error}\n")
    // test retrive
    var model <- openai_retrive_model("text-davinci-001")
    if model |> is_valid
        debug(model)
    else
        print("error: {model.error}\n")
    // test completion
    var ccr <- openai_create_completion([[Completion()
        model = "text-davinci-001",
        prompt = "Say this is a test",
        max_tokens = 7,
        temperature = 0.
    ]])
    if ccr |> is_valid
        debug(ccr)
    else
        print("error: {ccr.error}\n")


