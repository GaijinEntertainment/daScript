options gen2
options indenting = 4
options rtti  // honor @optional / @rename (consumer roots must also set it)

//! OpenAI-compatible API — vision (image input) chat helper.
//! Multimodal messages use an array-of-parts content shape that the typed ChatMessage
//! (content : string) can't express, so the request is built as a JsonValue tree.

module openai_vision shared public

require openai/openai_chat public

def private vision_text_part(text : string) : JsonValue? {
    var tab : table<string; JsonValue?>
    tab["type"] = JV("text")
    tab["text"] = JV(text)
    return JV(tab)
}

def private vision_image_part(image_url : string) : JsonValue? {
    var inner : table<string; JsonValue?>
    inner["url"] = JV(image_url)
    var tab : table<string; JsonValue?>
    tab["type"] = JV("image_url")
    tab["image_url"] = JV(inner)
    return JV(tab)
}

def vision_request_body(model, text, image_url : string; max_tokens : int = 300) : string {
    //! Builds the JSON body for a one-shot vision request (a user turn with text + one image).
    var content <- [vision_text_part(text), vision_image_part(image_url)]
    var msg : table<string; JsonValue?>
    msg["role"] = JV("user")
    msg["content"] = JV(content)
    var msgs : array<JsonValue?>
    msgs |> push(JV(msg))
    var body_tree : table<string; JsonValue?>
    body_tree["model"] = JV(model)
    body_tree["messages"] = JV(msgs)
    body_tree["max_tokens"] = JV(max_tokens)
    return write_json(JV(body_tree))
}

def chat_vision(client : OpenAIClient; model, text, image_url : string; max_tokens : int = 300) : ChatResult {
    //! One-shot vision request: a single user turn with text plus an image (http(s) URL or
    //! a ``data:`` URL). Returns the normal chat result (assistant text in choices[0].message.content).
    var response : ChatCompletionResponse
    let err = post_and_decode(client, "/chat/completions",
        vision_request_body(model, text, image_url, max_tokens), response)
    return ChatResult(ok = empty(err.kind), response <- response, error = err)
}
