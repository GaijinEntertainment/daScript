options gen2
options indenting = 4
options rtti  // honor @optional / @rename (consumer roots must also set it)

//! OpenAI-compatible API — model listing and retrieval.

module openai_models shared public

require openai/openai_common public

struct Model {
    //! A single model entry.
    id : string                          //!< Model id (e.g. "gpt-4o-mini").
    @rename = "object" _object : string  //!< Object type ("model").
    @optional created : int64            //!< Unix creation time, when provided.
    @optional owned_by : string          //!< Owning organization, when provided.
}

struct private ModelList {
    @rename = "object" _object : string
    data : array<Model>
}

struct ModelsResult {
    //! Outcome of ``list_models``: ``models`` valid when ``ok`` is true.
    ok : bool             //!< True when ``models`` is valid.
    models : array<Model>  //!< Available models on success.
    error : OpenAIError   //!< Error detail on failure.
}

struct ModelResult {
    //! Outcome of ``retrieve_model``: ``model`` valid when ``ok`` is true.
    ok : bool            //!< True when ``model`` is valid.
    model : Model        //!< The retrieved model on success.
    error : OpenAIError  //!< Error detail on failure.
}

def list_models(client : OpenAIClient) : ModelsResult {
    //! Lists available models (GET /models).
    var list : ModelList
    let err = get_and_decode(client, "/models", list)
    if (!empty(err.kind)) return ModelsResult(error = err)
    return ModelsResult(ok = true, models <- list.data)
}

def retrieve_model(client : OpenAIClient; model : string) : ModelResult {
    //! Retrieves a single model by id (GET /models/{id}).
    var m : Model
    let err = get_and_decode(client, "/models/{percent_encode(model)}", m)
    return ModelResult(ok = empty(err.kind), model = m, error = err)
}
