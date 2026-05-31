options gen2
options indenting = 4
options rtti  // honor @optional / @rename (consumer roots must also set it)

//! OpenAI-compatible API — text embeddings.

module openai_embeddings shared public

require openai/openai_common public

struct EmbeddingRequest {
    //! Request body for ``/embeddings``. ``input`` is always sent as an array of strings
    //! (a single string is just a one-element array).
    model : string                 //!< Embedding model id.
    input : array<string>          //!< Texts to embed, one element per string.
    @optional encoding_format : string  //!< "float" (default) or "base64".
    @optional dimensions : int     //!< Optional output dimensionality, for models that support it.
    @optional user : string        //!< Optional end-user id for abuse monitoring.
}

struct EmbeddingUsage {
    //! Token accounting for an embeddings request.
    prompt_tokens : int   //!< Tokens in the input.
    total_tokens : int    //!< Total tokens billed.
}

struct EmbeddingData {
    //! One input's embedding.
    @rename = "object" _object : string  //!< Object type ("embedding").
    index : int                          //!< Position in the input array.
    embedding : array<float>             //!< The embedding vector.
}

struct EmbeddingResponse {
    //! Response body for ``/embeddings``.
    @rename = "object" _object : string  //!< Object type ("list").
    data : array<EmbeddingData>          //!< One ``EmbeddingData`` per input, in order.
    model : string                       //!< Model that produced the embeddings.
    usage : EmbeddingUsage               //!< Token accounting.
}

struct EmbeddingResult {
    //! Outcome of ``embeddings``: when ``ok`` is true ``response`` is valid, else inspect ``error``.
    ok : bool                  //!< True when ``response`` is valid.
    response : EmbeddingResponse  //!< Decoded response on success.
    error : OpenAIError        //!< Error detail on failure.
}

def embeddings(client : OpenAIClient; req : EmbeddingRequest) : EmbeddingResult {
    //! Sends an ``/embeddings`` request and decodes the response.
    var response : EmbeddingResponse
    let err = post_and_decode(client, "/embeddings", sprint_json(req, false), response)
    return EmbeddingResult(ok = empty(err.kind), response <- response, error = err)
}

def embed(client : OpenAIClient; model : string; text : string) : array<float> {
    //! Convenience: embed a single string, returns the vector (empty on error).
    var out : array<float>
    let req = EmbeddingRequest(model = model, input <- [text])
    let res = embeddings(client, req)
    if (res.ok && !empty(res.response.data)) {
        out := res.response.data[0].embedding
    }
    return <- out
}
