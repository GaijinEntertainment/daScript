options persistent_heap
options gc

require dashv/dashv_boost
require daslib/json_boost

require fio

let private LOG_RESPONSES = false

var private OPENAI_API_KEY = get_env_variable("OPENAI_API_KEY")

struct public ListEnginesResponse
    object : string
    data : array<Model>

def public is_valid(object:ListEnginesResponse)
    return object.object=="list"

struct public Model
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
                var error : string
                var JV = read_json(text,error)
                if JV!=null
                    list <- from_JV(JV,type<ListEnginesResponse>)
                else
                    list.object = "read_json failed\n{error}}"
        else
            list.object = "HHTPS GET failed"
    return <- list

[export]
def main
    var list <- openai_list()
    if list |> is_valid
        debug(list)
    else
        print("error: {list.object}\n")

