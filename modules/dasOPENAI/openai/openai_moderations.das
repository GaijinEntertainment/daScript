options gen2
options indenting = 4
options rtti  // honor @optional / @rename (consumer roots must also set it)

//! OpenAI-compatible API — content moderation.

module openai_moderations shared public

require openai/openai_common public

struct ModerationRequest {
    //! Request body for ``/moderations``. ``input`` is always sent as an array of strings.
    model : string
    input : array<string>
}

struct ModerationCategoryResult {
    //! One moderation verdict. ``categories`` and ``category_scores`` are keyed by category name
    //! (e.g. "violence", "hate/threatening"), so no per-category field declarations are needed.
    flagged : bool
    categories : table<string; bool>
    category_scores : table<string; double>
}

struct ModerationResponse {
    //! Response body for ``/moderations``.
    id : string
    model : string
    results : array<ModerationCategoryResult>
}

struct ModerationResult {
    //! Outcome of ``moderations``: ``response`` valid when ``ok`` is true.
    ok : bool
    response : ModerationResponse
    error : OpenAIError
}

def moderations(client : OpenAIClient; req : ModerationRequest) : ModerationResult {
    //! Classifies text for policy violations (POST /moderations).
    var response : ModerationResponse
    let err = post_and_decode(client, "/moderations", sprint_json(req, false), response)
    return ModerationResult(ok = empty(err.kind), response <- response, error = err)
}
