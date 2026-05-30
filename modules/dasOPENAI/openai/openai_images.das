options gen2
options indenting = 4
options rtti  // honor @optional / @rename (consumer roots must also set it)

//! OpenAI-compatible API — image generation.

module openai_images shared public

require openai/openai_common public

struct ImageRequest {
    //! Request body for ``/images/generations``.
    prompt : string
    @optional model : string
    @optional n : int
    @optional size : string
    @optional quality : string
    @optional style : string
    @optional response_format : string
    @optional user : string
}

struct ImageData {
    @optional url : string
    @optional b64_json : string
    @optional revised_prompt : string
}

struct ImageResponse {
    //! Response body for ``/images/generations``.
    created : int64
    data : array<ImageData>
}

struct ImageResult {
    //! Outcome of ``generate_image``: ``response`` valid when ``ok`` is true.
    ok : bool
    response : ImageResponse
    error : OpenAIError
}

def generate_image(client : OpenAIClient; req : ImageRequest) : ImageResult {
    //! Generates images from a prompt (POST /images/generations).
    var response : ImageResponse
    let err = post_and_decode(client, "/images/generations", sprint_json(req, false), response)
    return ImageResult(ok = empty(err.kind), response <- response, error = err)
}
