options gen2
options indenting = 4
options rtti  // honor @optional / @rename (consumer roots must also set it)

//! OpenAI-compatible API — text embeddings.

module openai_embeddings shared public

require openai/openai_common public

struct EmbeddingRequest {
    //! Request body for ``/embeddings``. ``input`` is always sent as an array of strings
    //! (a single string is just a one-element array).
    model : string
    input : array<string>
    @optional encoding_format : string
    @optional dimensions : int
    @optional user : string
}

struct EmbeddingUsage {
    prompt_tokens : int
    total_tokens : int
}

struct EmbeddingData {
    @rename = "object" _object : string
    index : int
    embedding : array<float>
}

struct EmbeddingResponse {
    //! Response body for ``/embeddings``.
    @rename = "object" _object : string
    data : array<EmbeddingData>
    model : string
    usage : EmbeddingUsage
}

struct EmbeddingResult {
    //! Outcome of ``embeddings``: when ``ok`` is true ``response`` is valid, else inspect ``error``.
    ok : bool
    response : EmbeddingResponse
    error : OpenAIError
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
