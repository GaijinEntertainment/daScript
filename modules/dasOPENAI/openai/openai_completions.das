options gen2
options indenting = 4
options rtti  // honor @optional / @rename (consumer roots must also set it)

//! OpenAI-compatible API — legacy text completions.

module openai_completions shared public

require openai/openai_common public

struct CompletionRequest {
    //! Request body for the legacy ``/completions`` endpoint.
    model : string             //!< Completion model id.
    prompt : string            //!< The prompt to complete.
    @optional max_tokens : int //!< Maximum tokens to generate.
    @optional temperature : float  //!< Sampling temperature.
    @optional top_p : float    //!< Nucleus-sampling probability mass.
    @optional n : int          //!< Number of completions to generate.
    @optional stop : array<string>  //!< Stop sequences.
    @optional seed : int       //!< Seed for (best-effort) deterministic sampling.
    @optional user : string    //!< Optional end-user id for abuse monitoring.
}

struct CompletionChoice {
    //! One generated completion.
    text : string           //!< The completion text.
    index : int             //!< Index of this choice.
    finish_reason : string  //!< Why generation stopped (e.g. "stop", "length").
}

struct CompletionResponse {
    //! Response body for ``/completions``.
    id : string                          //!< Completion id.
    @rename = "object" _object : string  //!< Object type ("text_completion").
    created : int64                      //!< Unix creation time.
    model : string                       //!< Model that produced the completion.
    choices : array<CompletionChoice>    //!< Generated choices.
    usage : Usage                        //!< Token accounting.
}

struct CompletionResult {
    //! Outcome of ``completions``: ``response`` valid when ``ok`` is true.
    ok : bool                   //!< True when ``response`` is valid.
    response : CompletionResponse  //!< Decoded response on success.
    error : OpenAIError         //!< Error detail on failure.
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
