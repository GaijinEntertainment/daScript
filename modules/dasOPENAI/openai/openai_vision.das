options gen2
options indenting = 4
options rtti  // honor @optional / @rename (consumer roots must also set it)

//! OpenAI-compatible API — vision (image input) chat helper.
//! Multimodal messages use an array-of-parts content shape that the typed ChatMessage
//! (content : string) can't express, so the request is built as a JsonValue tree.

module openai_vision shared public

require openai/openai_chat public
require daslib/base64
require daslib/fio

def private vision_text_part(text : string) : JsonValue? {
    var tab <- { "type" => JV("text"), "text" => JV(text) }
    return JV(tab)
}

def private vision_image_part(image_url : string) : JsonValue? {
    var inner : table<string; JsonValue?>
    inner["url"] = JV(image_url)
    var tab <- { "type" => JV("image_url"), "image_url" => JV(inner) }
    return JV(tab)
}

def image_file_data_uri(path : string) : string {
    //! The image file at ``path`` as the ``data:`` URL a browser would send (``image/png`` for a ``.png``
    //! name, ``image/jpeg`` otherwise); "" for a file that cannot be read.
    var bytes : array<uint8>
    fopen(path, "rb") $(f) {
        if (f != null) {
            bytes |> resize(int64(fstat(f).size))
            fread(f, bytes)
        }
    }
    let uri = empty(bytes) ? "" : "data:image/{ends_with(to_lower(path), ".png") ? "png" : "jpeg"};base64,{base64_encode(bytes)}"
    delete bytes
    return uri
}

def vision_request_body(model, text, image_url : string; max_tokens : int = 300; system : string = "") : string {
    //! Builds the JSON body for a one-shot vision request (a user turn with text + one image), under a
    //! system message when ``system`` is not empty.
    var content <- [vision_text_part(text), vision_image_part(image_url)]
    var msg <- { "role" => JV("user"), "content" => JV(content) }
    var msgs : array<JsonValue?>
    if (!empty(system)) {
        var sys <- { "role" => JV("system"), "content" => JV(system) }
        msgs |> push(JV(sys))
    }
    msgs |> push(JV(msg))
    var body_tree <- {
        "model" => JV(model),
        "messages" => JV(msgs),
        "max_tokens" => JV(max_tokens)
    }
    return write_json_compact(JV(body_tree))
}

def chat_vision(client : OpenAIClient; model, text, image_url : string; max_tokens : int = 300; system : string = "") : ChatResult {
    //! One-shot vision request: a single user turn with text plus an image (http(s) URL or
    //! a ``data:`` URL). Returns the normal chat result (assistant text in choices[0].message.content).
    var response : ChatCompletionResponse
    let err = post_and_decode(client, "/chat/completions",
        vision_request_body(model, text, image_url, max_tokens, system), response)
    return ChatResult(ok = empty(err.kind), response <- response, error = err)
}
