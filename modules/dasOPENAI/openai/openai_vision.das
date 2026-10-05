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

def file_base64(path : string) : string {
    //! The bytes of the file at ``path`` as base64 - a byte read, since an image or a clip carries interior NULs a
    //! string read stops at; "" for a file that cannot be read.
    var inscope bytes : array<uint8>
    fopen(path, "rb") $(f) {
        if (f != null) {
            bytes |> resize(int64(fstat(f).size))
            fread(f, bytes)
        }
    }
    return empty(bytes) ? "" : base64_encode(bytes)
}

def image_file_data_uri(path : string) : string {
    //! The image file at ``path`` as the ``data:`` URL a browser would send (``image/png`` for a ``.png``
    //! name, ``image/jpeg`` otherwise); "" for a file that cannot be read.
    let b64 = file_base64(path)
    return empty(b64) ? "" : "data:image/{ends_with(to_lower(path), ".png") ? "png" : "jpeg"};base64,{b64}"
}

def private audio_part(audio_base64, format : string) : JsonValue? {
    var inner <- { "data" => JV(audio_base64), "format" => JV(format) }
    var tab <- { "type" => JV("input_audio"), "input_audio" => JV(inner) }
    return JV(tab)
}

//! the JSON body of a one-shot request: a user turn of `text` and one media part, the media ahead of the text under
//! `media_first`, under a system message when `system` is not empty
def private media_request_body(model, text : string; var media : JsonValue?; max_tokens : int; system : string; media_first, no_think : bool) : string {
    var content <- media_first ? [media, vision_text_part(text)] : [vision_text_part(text), media]
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
    if (no_think) {   // both spellings: a server reads the flat field or the template's keyword arguments
        var kwargs <- { "enable_thinking" => JV(false) }
        body_tree["enable_thinking"] = JV(false)
        body_tree["chat_template_kwargs"] = JV(kwargs)
    }
    return write_json_compact(JV(body_tree))
}

def vision_request_body(model, text, image_url : string; max_tokens : int = 300; system : string = ""; image_first : bool = false; no_think : bool = false) : string {
    //! Builds the JSON body for a one-shot vision request (a user turn with text + one image, the image ahead of
    //! the text under ``image_first``), under a system message when ``system`` is not empty; ``no_think`` asks a
    //! thinking model to answer directly.
    return media_request_body(model, text, vision_image_part(image_url), max_tokens, system, image_first, no_think)
}

def audio_request_body(model, text, audio_base64 : string; max_tokens : int = 300; system : string = ""; audio_first : bool = false; format : string = "wav"; no_think : bool = false) : string {
    //! Builds the JSON body for a one-shot audio request: a user turn with text + one ``input_audio`` part carrying
    //! the base64 of an audio file, the clip ahead of the text under ``audio_first``.
    return media_request_body(model, text, audio_part(audio_base64, format), max_tokens, system, audio_first, no_think)
}

def chat_audio(client : OpenAIClient; model, text, audio_base64 : string; max_tokens : int = 300; system : string = ""; audio_first : bool = false; no_think : bool = false) : ChatResult {
    //! One-shot audio request: a single user turn with text plus an audio clip (the base64 of its file).
    //! Returns the normal chat result (assistant text in choices[0].message.content).
    var response : ChatCompletionResponse
    let err = post_and_decode(client, "/chat/completions",
        audio_request_body(model, text, audio_base64, max_tokens, system, audio_first, "wav", no_think), response)
    return ChatResult(ok = empty(err.kind), response <- response, error = err)
}

def chat_vision(client : OpenAIClient; model, text, image_url : string; max_tokens : int = 300; system : string = ""; image_first : bool = false; no_think : bool = false) : ChatResult {
    //! One-shot vision request: a single user turn with text plus an image (http(s) URL or
    //! a ``data:`` URL). Returns the normal chat result (assistant text in choices[0].message.content).
    var response : ChatCompletionResponse
    let err = post_and_decode(client, "/chat/completions",
        vision_request_body(model, text, image_url, max_tokens, system, image_first, no_think), response)
    return ChatResult(ok = empty(err.kind), response <- response, error = err)
}
