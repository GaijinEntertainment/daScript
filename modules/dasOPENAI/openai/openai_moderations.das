options gen2
options indenting = 4
options rtti  // honor @optional / @rename (consumer roots must also set it)

//! OpenAI-compatible API — content moderation.

module openai_moderations shared public

require openai/openai_common public

struct ModerationRequest {
    //! Request body for ``/moderations``. ``input`` is always sent as an array of strings.
    model : string         //!< Moderation model id.
    input : array<string>  //!< Texts to classify, one element per string.
}

struct ModerationCategoryResult {
    //! One moderation verdict. ``categories`` and ``category_scores`` are keyed by category name
    //! (e.g. "violence", "hate/threatening"), so no per-category field declarations are needed.
    flagged : bool                          //!< True if the input was flagged in any category.
    categories : table<string; bool>        //!< Per-category boolean verdicts, keyed by category name.
    category_scores : table<string; double> //!< Per-category confidence scores, keyed by category name.
}

struct ModerationResponse {
    //! Response body for ``/moderations``.
    id : string                              //!< Moderation request id.
    model : string                           //!< Model that produced the verdicts.
    results : array<ModerationCategoryResult> //!< One verdict per input.
}

struct ModerationResult {
    //! Outcome of ``moderations``: ``response`` valid when ``ok`` is true.
    ok : bool                   //!< True when ``response`` is valid.
    response : ModerationResponse  //!< Decoded response on success.
    error : OpenAIError         //!< Error detail on failure.
}

def moderations(client : OpenAIClient; req : ModerationRequest) : ModerationResult {
    //! Classifies text for policy violations (POST /moderations).
    var response : ModerationResponse
    let err = post_and_decode(client, "/moderations", sprint_json(req, false), response)
    return ModerationResult(ok = empty(err.kind), response <- response, error = err)
}
