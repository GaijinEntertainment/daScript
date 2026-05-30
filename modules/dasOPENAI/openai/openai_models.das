options gen2
options indenting = 4
options rtti  // honor @optional / @rename (consumer roots must also set it)

//! OpenAI-compatible API — model listing and retrieval.

module openai_models shared public

require openai/openai_common public

struct Model {
    //! A single model entry.
    id : string
    @rename = "object" _object : string
    @optional created : int64
    @optional owned_by : string
}

struct private ModelList {
    @rename = "object" _object : string
    data : array<Model>
}

struct ModelsResult {
    //! Outcome of ``list_models``: ``models`` valid when ``ok`` is true.
    ok : bool
    models : array<Model>
    error : OpenAIError
}

struct ModelResult {
    //! Outcome of ``retrieve_model``: ``model`` valid when ``ok`` is true.
    ok : bool
    model : Model
    error : OpenAIError
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
