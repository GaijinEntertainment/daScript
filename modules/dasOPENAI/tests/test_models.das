options gen2
options rtti  // required so sprint_json/sscan_json honor @optional / @rename
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost public
require openai/openai_models
require strings
require daslib/fio

// ===== Unit: serialization (no backend needed) =====

[test]
def test_model_decode(t : T?) {
    t |> run("model entry round-trips through sprint_json/sscan_json") @(t : T?) {
        var m : Model
        m.id = "gpt-test"
        m._object = "model"
        m.created = 1700000000
        m.owned_by = "openai"
        let js = sprint_json(m, false)
        t |> success(find(js, "\"object\":\"model\"") >= 0, "renamed object key emitted")
        var back : Model
        t |> success(sscan_json(js, back), "decode ok")
        t |> equal(back.id, "gpt-test")
        t |> equal(back._object, "model")
        t |> equal(back.owned_by, "openai")
    }
}

// ===== Live integration =====

[test]
def test_models_live_ollama(t : T?) {
    t |> run("live list_models against Ollama") @(t : T?) {
        let client = openai_client("http://localhost:11434/v1")
        let res = list_models(client)
        t |> success(res.ok, "list ok (kind={res.error.kind} status={res.error.status} {res.error.message})")
        if (res.ok) {
            t |> success(!empty(res.models), "models non-empty ({length(res.models)})")
        }
    }
}

[test]
def test_models_retrieve_ollama(t : T?) {
    t |> run("live retrieve_model against Ollama") @(t : T?) {
        let client = openai_client("http://localhost:11434/v1")
        let res = retrieve_model(client, "nomic-embed-text")
        t |> success(res.ok, "retrieve ok (kind={res.error.kind} status={res.error.status} {res.error.message})")
        if (res.ok) {
            t |> equal(res.model.id, "nomic-embed-text")
            t |> equal(res.model._object, "model")
        }
    }
}

[test]
def test_models_live_openrouter(t : T?) {
    t |> run("live list_models against OpenRouter (validates GET Bearer auth)") @(t : T?) {
        if (!has_env_variable("OPENROUTER_API_KEY")) {
            t |> success(true, "OPENROUTER_API_KEY not set - skipped")
            return
        }
        let client = openai_client("https://openrouter.ai/api/v1", get_env_variable("OPENROUTER_API_KEY"))
        let res = list_models(client)
        if (!res.ok && res.error.status == 429) {
            t |> success(true, "OpenRouter free-tier rate-limited (429) - skipped")
            return
        }
        t |> success(res.ok, "list ok (kind={res.error.kind} status={res.error.status} {res.error.message})")
        if (res.ok) {
            t |> success(length(res.models) > 10, "many models ({length(res.models)})")
        }
    }
}
