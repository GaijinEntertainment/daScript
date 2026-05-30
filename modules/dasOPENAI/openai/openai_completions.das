options gen2
options indenting = 4
options rtti  // honor @optional / @rename (consumer roots must also set it)

//! OpenAI-compatible API — legacy text completions.

module openai_completions shared public

require openai/openai_common public

struct CompletionRequest {
    //! Request body for the legacy ``/completions`` endpoint.
    model : string
    prompt : string
    @optional max_tokens : int
    @optional temperature : float
    @optional top_p : float
    @optional n : int
    @optional stop : array<string>
    @optional seed : int
    @optional user : string
}

struct CompletionChoice {
    text : string
    index : int
    finish_reason : string
}

struct CompletionResponse {
    //! Response body for ``/completions``.
    id : string
    @rename = "object" _object : string
    created : int64
    model : string
    choices : array<CompletionChoice>
    usage : Usage
}

struct CompletionResult {
    //! Outcome of ``completions``: ``response`` valid when ``ok`` is true.
    ok : bool
    response : CompletionResponse
    error : OpenAIError
}

def completions(client : OpenAIClient; req : CompletionRequest) : CompletionResult {
    //! Sends a legacy ``/completions`` request and decodes the response.
    var response : CompletionResponse
    let err = post_and_decode(client, "/completions", sprint_json(req, false), response)
    return CompletionResult(ok = empty(err.kind), response <- response, error = err)
}

def complete(client : OpenAIClient; model : string; prompt : string) : string {
    //! Convenience: completes ``prompt``, returns the text ("" on error).
    let req = CompletionRequest(model = model, prompt = prompt, max_tokens = 64)
    let res = completions(client, req)
    if (!res.ok || empty(res.response.choices)) return ""
    return res.response.choices[0].text
}
