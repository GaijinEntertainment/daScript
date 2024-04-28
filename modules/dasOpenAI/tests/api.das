options persistent_heap = true
options gc

require dastest/testing_boost public

require openai/openai
require daslib/base64
require fio
require daslib/strings_boost

let COMPLETION_MODEL = "davinci-002"

def get_test_file_name(name:string)
    let test_dir = "{get_das_root()}/modules/dasOpenAI/tests"
    return "{test_dir}/{name}"

def get_temp_file_name(name:string)
    let temp_dir = "{get_das_root()}/temp"
    mkdir(temp_dir)
    return "{temp_dir}/{name}"

[test]
def test_openai_api( t: T?)
    if !openai_key_is_set()
        t |> failure("OpenAI key is not set. Please set the OPENAI_KEY environment variable.")
        return
    t |> run("list") <| @@ ( t:T? )
        var inscope list <- openai_list()
        t |> success ( is_valid(list), openai_get_last_error() )
    t |> run("retrive") <| @@ ( t:T? )
        var inscope model <- openai_retrive_model(COMPLETION_MODEL)
        t |> success ( is_valid(model), openai_get_last_error() )
    t |> run("completion") <| @@ ( t:T? )
        var inscope completion <- openai_create_completion([[Completion()
            model = COMPLETION_MODEL,
            prompt = "Say this is a test",
            max_tokens = 7,
            temperature = 0.
        ]])
        t |> success ( is_valid(completion), openai_get_last_error() )
        if completion |> is_valid
            t |> equal ( 1, completion.choices |> length )
            to_log(LOG_INFO, "completion result: \"{completion.choices[0].text}\"\n")
    t |> run("chat completion") <| @@ ( t:T? )
        var inscope chat <- openai_create_chat_completion([[ChatCompletion()
            model = "gpt-4-turbo",
            messages <- [{ChatCompletionMessage
                role = "user",
                content = "Hello!"
            }]
        ]])
        t |> success ( is_valid(chat), openai_get_last_error() )
        if chat |> is_valid
            t |> equal ( 1, chat.choices |> length )
            to_log(LOG_INFO, "chat completion result: \"{chat.choices[0].message.content}\"\n")

var is_image_valid = false

[test]
def test_openai_image_api( t: T?)
    if !openai_key_is_set()
        return
    t |> run("image completion") <| @@ ( t:T? )
        var inscope image <- openai_create_image([[Image()
            prompt = "Draw a cat",
            n = 1,
            size = 256,
            response_format = "b64_json"    // url
        ]])
        t |> success ( is_valid(image), openai_get_last_error() )
        if image |> is_valid
            t |> equal ( 1, image.data |> length, "expecting image" )
            if image.data |> length == 1
                fopen(get_temp_file_name("image.png"), "wb") <| $ ( f )
                    if f != null
                        var inscope bytes : array<uint8>
                        let res = base64_decode(image.data[0].b64_json, bytes)
                        f |> fwrite(bytes)
                        to_log(LOG_INFO, "image completion written to \"{get_temp_file_name("image.png")}\"\n")
                        is_image_valid = true
                    else
                        t |> failure("failed to open image file {get_temp_file_name("image.png")} for writing")
    if !is_image_valid
        t |> failure("image completion failed, skipping the rest of the image tests")
        return
    t |> run("image variation") <| @@ ( t:T? )
        var inscope image <- openai_create_image_variation([[ImageVariation()
            image = get_temp_file_name("image.png"),
            n = 1,
            size = 256,
            response_format = "b64_json"    // url
        ]])
        t |> success ( is_valid(image), openai_get_last_error() )
        if image |> is_valid
            t |> equal ( 1, image.data |> length, "expecting image" )
            if image.data |> length == 1
                fopen(get_temp_file_name("image_variation.png"), "wb") <| $ ( f )
                    if f != null
                        var inscope bytes : array<uint8>
                        let res = base64_decode(image.data[0].b64_json, bytes)
                        f |> fwrite(bytes)
                        to_log(LOG_INFO, "image variation written to \"{get_temp_file_name("image_variation.png")}\"\n")
                    else
                        t |> failure("failed to open image file {get_temp_file_name("image_variation.png")} for writing")

[test]
def test_openai_image_edit_api( t: T?)
    if !openai_key_is_set()
        return
    t |> run("image edit") <| @@ ( t:T? )
        var inscope image <- openai_create_image_edit([[ImageEdit()
            image = get_test_file_name("brick.png"),
            mask = get_test_file_name("brick_alpha.png"),
            prompt = "cast iron plate with keyhole",
            n = 1,
            size = 256,
            response_format = "b64_json"    // url
        ]])
        t |> success ( is_valid(image), openai_get_last_error() )
        if image |> is_valid
            t |> equal ( 1, image.data |> length, "expecting image" )
            if image.data |> length == 1
                fopen(get_temp_file_name("image_edit.png"), "wb") <| $ ( f )
                    if f != null
                        var inscope bytes : array<uint8>
                        let res = base64_decode(image.data[0].b64_json, bytes)
                        f |> fwrite(bytes)
                        to_log(LOG_INFO, "image edit written to \"{get_temp_file_name("image_edit.png")}\"\n")
                    else
                        t |> failure("failed to open image file {get_temp_file_name("image_edit.png")} for writing")

[test]
def test_opeanai_audio_api( t: T?)
    if !openai_key_is_set()
        return
    t |> run("transcription, english") <| @@ ( t:T? )
        let en = openai_create_transcription([[Transcription()
            file = get_test_file_name("english.wav")
        ]])
        t |> success ( empty(en.error), "transcription failed" )
        t |> success ( en.text |> starts_with("Hello, my name is Boris."), "transcription mismatch: {en.text}" )
    t |> run("transcription, russian") <| @@ ( t:T? )
        let ru = openai_create_transcription([[Transcription()
            file = get_test_file_name("russian.wav"),
            language = "ru"
        ]])
        t |> success ( empty(ru.error), "transcription failed" )
        t |> success ( ru.text |> starts_with("Здравствуйте, меня зовут Борис."), "transcription mismatch: {ru.text}" )
    t |> run("translation") <| @@ ( t:T? )
        let en = openai_create_translation([[Translation()
            file = get_test_file_name("russian.wav"),
            prompt = "introduction in russian"
        ]])
        t |> success ( empty(en.error), "transcription failed" )
        t |> success ( en.text |> starts_with("Hello, my name is Boris."), "translation mismatch: {en.text}" )
