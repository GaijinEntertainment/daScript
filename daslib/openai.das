options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4

module openai shared private

require strings
require fio
require daslib/curl
require daslib/json_boost

let private LOG_RESPONSES = false

var private OPENAI_API_KEY = get_env_variable("OPENAI_API_KEY")
var private LAST_ERROR = ""

struct public ListEnginesResponse
    //! list of models
    object : string
    data : array<Model>

def public is_valid(object:ListEnginesResponse)
    return object.object=="list"

def public openai_get_last_error
    //! return error made via last request
    return LAST_ERROR

struct public Model
    //! model description
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
    //! model permission
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

def public openai_key_is_set
    //! return true if API key is set
    return !empty(OPENAI_API_KEY)

def public openai_api_key
    //! return API authentication key
    if empty(OPENAI_API_KEY)
        panic("OpenAI API key is not set")
    return OPENAI_API_KEY

def public openai_list
    //! list all models
    var list : ListEnginesResponse
    var valid = false
    LAST_ERROR = ""
    GET("https://api.openai.com/v1/models",
        {{"Authorization" => "Bearer {OPENAI_API_KEY}"}}
    ) <| $ ( resp )
        log_resp(resp)
        if resp != null && resp.status_code==200
            var JV = read_json(resp.text,LAST_ERROR)
            if is_valid_response(JV)
                list <- from_JV(JV,type<ListEnginesResponse>)
            else
                LAST_ERROR = collect_error(JV)
            unsafe
                delete JV
        else
            LAST_ERROR = "HTTPS GET failed"
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
    //! retrive a model by id
    var model : Model
    var valid = false
    LAST_ERROR = ""
    GET("https://api.openai.com/v1/models/{model_id}",
        {{"Authorization" => "Bearer {OPENAI_API_KEY}"}}
    ) <| $ ( resp )
        log_resp(resp)
        if resp!=null && resp.status_code==200
            var JV = read_json(resp.text,LAST_ERROR)
            if is_valid_response(JV)
                model <- from_JV(JV,type<Model>)
            else
                LAST_ERROR = collect_error(JV)
            unsafe
                delete JV
        else
            LAST_ERROR = "HTTPS GET failed"
    return <- model

struct public Completion
    //! completion request
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
    //! completion response
    id : string
    object : string
    created : int
    model : string
    choices : array<CreateCompletionResponseChoicesInner>
    usage : CreateCompletionResponseUsage

struct public CreateCompletionResponseUsage
    //! completion response usage
    prompt_tokens : int
    completion_tokens : int
    total_tokens : int

def public is_valid ( object:CreateCompletionResponse )
    return object.object=="text_completion"

struct public CreateCompletionResponseChoicesInner
    //! completion response choices
    text : string
    index : int
    finish_reason : string

def public openai_create_completion ( completion:Completion )
    //! create a completion
    var req_jv <- JV(completion)
    if empty(completion.suffix)
        req_jv as _object |> erase("suffix")
    var req = write_request_json(req_jv)
    unsafe
        delete req_jv
    var ccr : CreateCompletionResponse
    LAST_ERROR = ""
    POST("https://api.openai.com/v1/completions", req,
        {{"Authorization" => "Bearer {OPENAI_API_KEY}";
          "Content-Type" => "application/json"}}
    ) <| $ ( resp )
        log_resp(resp)
        if resp!=null
            var JV <- read_json(resp.text,LAST_ERROR)
            if is_valid_response(JV)
                ccr <- from_JV(JV,type<CreateCompletionResponse>)
            else
                LAST_ERROR = collect_error(JV)
            unsafe
                delete JV
        else
            LAST_ERROR = "HTTPS POST failed"
    return <- ccr
