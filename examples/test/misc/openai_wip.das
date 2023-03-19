options persistent_heap
options gc

require daslib/openai
require daslib/base64
require fio

let TEST_LIST =                     true
let TEST_RETREIVE =                 true
let TEST_COMPLETION =               true
let TEST_CHAT_COMPLETION =          true
let TEST_EDIT_COMPLETION =          true
let TEST_CREATE_IMAGE =             true
let TEST_CREATE_IMAGE_VARIATION =   true

[export]
def main
    // test list
    if TEST_LIST
        print("testing list\n")
        var list <- openai_list()
        if list |> is_valid
            debug(list)
            print("ok - testing list\n")
        else
            print("error: {openai_get_last_error()}\n")
    // test retrive
    if TEST_RETREIVE
        print("testing retrive\n")
        var model <- openai_retrive_model("text-davinci-001")
        if model |> is_valid
            debug(model)
            print("ok - testing retrive\n")
        else
            print("error: {openai_get_last_error()}\n")
    // test completion
    if TEST_COMPLETION
        print("testing completion\n")
        var ccr <- openai_create_completion([[Completion()
            model = "text-davinci-001",
            prompt = "Say this is a test",
            max_tokens = 7,
            temperature = 0.
        ]])
        if ccr |> is_valid
            debug(ccr)
            print("ok - testing completion\n")
        else
            print("error: {openai_get_last_error()}\n")
    // test chat completion
    if TEST_CHAT_COMPLETION
        print("testing chat completion\n")
        var ccr <- openai_create_chat_completion([[ChatCompletion()
            model = "gpt-3.5-turbo",
            messages <- [{ChatCompletionMessage
                role = "user",
                content = "Hello!"
            }]
        ]])
        if ccr |> is_valid
            debug(ccr)
            print("ok - testing chat completion\n")
        else
            print("error: {openai_get_last_error()}\n")
    // test edit completion
    if TEST_EDIT_COMPLETION
        print("testing edit\n")
        var ccr <- openai_create_edit([[Edit()
            model = "text-davinci-edit-001",
            input = "What day of the wek is it?",
            instruction = "Fix the spelling mistakes"
        ]])
        if ccr |> is_valid
            debug(ccr)
            print("ok - testing edit\n")
        else
            print("error: {openai_get_last_error()}\n")
    // test create image
    if TEST_CREATE_IMAGE
        let old_timeout = openai_set_connection_timeout(60.)    // this may take time
        print("testing create image\n")
        var ccr <- openai_create_image([[Image()
            prompt = "Draw a cat",
            n = 1,
            size = 256,
            response_format = "b64_json"    // url
        ]])
        if ccr |> is_valid
            debug(ccr)
            print("ok - testing create image\n")
            fopen("image.png", "wb") <| $ ( f )
                if f != null
                    var inscope bytes : array<uint8>
                    let res = base64_decode(ccr.data[0].b64_json, bytes)
                    f |> fwrite(bytes)
                    print("written to image.png\n")
                else
                    print("failed to write to image.png\n")
        else
            print("error: {openai_get_last_error()}\n")
        openai_set_connection_timeout(old_timeout)
    if TEST_CREATE_IMAGE_VARIATION
        let old_timeout = openai_set_connection_timeout(60.)    // this may take time
        print("testing create image\n")
        var ccr <- openai_create_image_variation([[ImageVariation()
            image = "image.png",
            n = 1,
            size = 256,
            response_format = "b64_json"    // url
        ]])
        if ccr |> is_valid
            debug(ccr)
            print("ok - testing create image variation\n")
            fopen("image_variation.png", "wb") <| $ ( f )
                if f != null
                    var inscope bytes : array<uint8>
                    let res = base64_decode(ccr.data[0].b64_json, bytes)
                    f |> fwrite(bytes)
                    print("written to image_vriation.png\n")
                else
                    print("failed to write to image_vriation.png\n")
        else
            print("error: {openai_get_last_error()}\n")
        openai_set_connection_timeout(old_timeout)
