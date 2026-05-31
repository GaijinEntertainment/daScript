options gen2
options indenting = 4
options rtti  // honor @optional / @rename (consumer roots must also set it)

//! OpenAI-compatible API — image generation.

module openai_images shared public

require openai/openai_common public

struct ImageRequest {
    //! Request body for ``/images/generations``.
    prompt : string                      //!< Text description of the desired image.
    @optional model : string             //!< Image model id.
    @optional n : int                    //!< Number of images to generate.
    @optional size : string              //!< Image dimensions (e.g. "1024x1024").
    @optional quality : string           //!< Quality hint (e.g. "standard", "hd").
    @optional style : string             //!< Style hint (e.g. "vivid", "natural").
    @optional response_format : string   //!< "url" or "b64_json".
    @optional user : string              //!< Optional end-user id for abuse monitoring.
}

struct ImageData {
    //! One generated image.
    @optional url : string            //!< Image URL (when ``response_format`` is "url").
    @optional b64_json : string       //!< Base64-encoded image (when ``response_format`` is "b64_json").
    @optional revised_prompt : string //!< The prompt the model actually used, if it revised yours.
}

struct ImageResponse {
    //! Response body for ``/images/generations``.
    created : int64           //!< Unix creation time.
    data : array<ImageData>   //!< Generated images.
}

struct ImageResult {
    //! Outcome of ``generate_image``: ``response`` valid when ``ok`` is true.
    ok : bool              //!< True when ``response`` is valid.
    response : ImageResponse  //!< Decoded response on success.
    error : OpenAIError    //!< Error detail on failure.
}

def generate_image(client : OpenAIClient; req : ImageRequest) : ImageResult {
    //! Generates images from a prompt (POST /images/generations).
    var response : ImageResponse
    let err = post_and_decode(client, "/images/generations", sprint_json(req, false), response)
    return ImageResult(ok = empty(err.kind), response <- response, error = err)
}
