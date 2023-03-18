options persistent_heap
options gc

require daslib/openai

[export]
def main
    // test list
    if true
        var list <- openai_list()
        if list |> is_valid
            debug(list)
        else
            print("error: {openai_get_last_error()}\n")
    // test retrive
    if true
        var model <- openai_retrive_model("text-davinci-001")
        if model |> is_valid
            debug(model)
        else
            print("error: {openai_get_last_error()}\n")
    // test completion
    if true
        var ccr <- openai_create_completion([[Completion()
            model = "text-davinci-001",
            prompt = "Say this is a test",
            max_tokens = 7,
            temperature = 0.
        ]])
        if ccr |> is_valid
            debug(ccr)
        else
            print("error: {openai_get_last_error()}\n")
    if true
        var ccr <- openai_create_chat_completion([[ChatCompletion()
            model = "gpt-3.5-turbo",
            messages <- [{ChatCompletionMessage
                role = "user",
                content = "Hello!"
            }]
        ]])
        if ccr |> is_valid
            debug(ccr)
        else
            print("error: {openai_get_last_error()}\n")

