options gen2
options rtti  // required so sprint_json/sscan_json honor @optional / @rename
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost public
require _openai_live
require openai/openai_completions
require openai/openai_embeddings
require openai/openai_moderations
require openai/openai_models
require strings
require daslib/fio

//! The text endpoints beside chat: completions, embeddings,
//! moderations and the model list. No local backend serves
//! /moderations, so it is checked offline only.

[test(description = "A completion request serializes the model and prompt and omits an unset temperature.")]
def test_completion_request_serialization(t : T?) {
    t |> run("completion request includes fields, omits unset optionals") @(t : T?) {
        let req = CompletionRequest(model = "m", prompt = "hello")
        let js = sprint_json(req, false)
        t |> success(find(js, "\"model\":\"m\"") >= 0, "model present")
        t |> success(find(js, "\"prompt\":\"hello\"") >= 0, "prompt present")
        t |> success(find(js, "\"temperature\"") == -1, "unset temperature omitted")
    }
}

[test(description = "A text_completion response round-trips through sprint_json and sscan_json with its object, choice text and usage intact.")]
def test_completion_response_decode(t : T?) {
    t |> run("completion response round-trips through sprint_json/sscan_json") @(t : T?) {
        let resp = CompletionResponse(
            id = "cmpl-1",
            _object = "text_completion",
            model = "m",
            choices <- [CompletionChoice(text = "Paris", index = 0, finish_reason = "stop")],
            usage = Usage(prompt_tokens = 3, completion_tokens = 1, total_tokens = 4))
        let js = sprint_json(resp, false)
        var back : CompletionResponse
        t |> success(sscan_json(js, back), "decode ok")
        t |> equal(back._object, "text_completion")
        t |> equal(back.choices[0].text, "Paris")
        t |> equal(back.usage.total_tokens, 4)
    }
}

[test(description = "A live complete() against Ollama returns non-empty text; skipped when no Ollama server answers.", timeout = 1200)]
def test_completion_live_ollama(t : T?) {
    t |> run("live completion against Ollama") @(t : T?) {
        return if (!live_server(t, "http://localhost:11434/v1"))
        let client = openai_client("http://localhost:11434/v1")
        let text = complete(client, "hf.co/LiquidAI/LFM2.5-1.2B-Instruct-GGUF:Q4_K_M",
            "The capital of France is")
        t |> success(!empty(text), "got completion text")
    }
}

[test(description = "An embeddings request serializes its input as a JSON array and omits unset dimensions.")]
def test_embeddings_request_serialization(t : T?) {
    t |> run("embeddings request includes input array, omits unset optionals") @(t : T?) {
        let req = EmbeddingRequest(model = "m", input <- ["a", "b"])
        let js = sprint_json(req, false)
        t |> success(find(js, "\"model\":\"m\"") >= 0, "model present")
        t |> success(find(js, "\"input\":[\"a\",\"b\"]") >= 0, "input array present")
        t |> success(find(js, "\"dimensions\"") == -1, "unset dimensions omitted")
    }
}

[test(description = "An embeddings response round-trips through sprint_json and sscan_json with its vector length, index and usage intact.")]
def test_embeddings_response_decode(t : T?) {
    t |> run("embeddings response round-trips through sprint_json/sscan_json") @(t : T?) {
        let resp = EmbeddingResponse(
            _object = "list",
            model = "test-embed",
            data <- [EmbeddingData(_object = "embedding", index = 0, embedding <- [0.1f, 0.2f, 0.3f])],
            usage = EmbeddingUsage(prompt_tokens = 4, total_tokens = 4))
        let js = sprint_json(resp, false)
        var back : EmbeddingResponse
        t |> success(sscan_json(js, back), "decode ok")
        t |> equal(back._object, "list")
        t |> equal(length(back.data), 1)
        t |> equal(back.data[0].index, 0)
        t |> equal(length(back.data[0].embedding), 3)
        t |> equal(back.usage.total_tokens, 4)
    }
}

[test(description = "A live embed() against Ollama's nomic-embed-text returns a 768-wide vector; skipped when no Ollama server answers.", timeout = 1200)]
def test_embeddings_live_ollama(t : T?) {
    t |> run("live embeddings against Ollama (nomic-embed-text)") @(t : T?) {
        return if (!live_server(t, "http://localhost:11434/v1"))
        let client = openai_client("http://localhost:11434/v1")
        let v = embed(client, "nomic-embed-text", "hello world")
        t |> success(!empty(v), "embedding vector non-empty")
        if (!empty(v)) {
            t |> equal(length(v), 768)
        }
    }
}

[test(description = "A moderation request serializes the model and its input as a JSON array.")]
def test_moderation_request_serialization(t : T?) {
    t |> run("moderation request serializes input array") @(t : T?) {
        let req = ModerationRequest(model = "omni-moderation-latest", input <- ["some text"])
        let js = sprint_json(req, false)
        t |> success(find(js, "\"model\":\"omni-moderation-latest\"") >= 0, "model present")
        t |> success(find(js, "\"input\":[\"some text\"]") >= 0, "input array present")
    }
}

[test(description = "A moderation response round-trips through sprint_json and sscan_json with its flag and its table-keyed categories and scores intact.")]
def test_moderation_response_decode(t : T?) {
    t |> run("moderation response round-trips with table-keyed categories") @(t : T?) {
        var resp = ModerationResponse(id = "modr-1", model = "omni-moderation-latest")
        var r : ModerationCategoryResult
        r.flagged = true
        r.categories["violence"] = false
        r.categories["hate"] = true
        r.category_scores["violence"] = 0.01lf
        r.category_scores["hate"] = 0.93lf
        resp.results |> emplace(r)

        let js = sprint_json(resp, false)
        var back : ModerationResponse
        t |> success(sscan_json(js, back), "decode ok")
        t |> equal(length(back.results), 1)
        t |> success(back.results[0].flagged, "flagged is true")
        t |> success(back.results[0].categories["hate"], "category hate decoded true")
        t |> success(!back.results[0].categories["violence"], "category violence decoded false")
        t |> success(back.results[0].category_scores["hate"] > 0.9lf, "hate score decoded")
    }
}

[test(description = "A model entry round-trips through sprint_json and sscan_json, its _object field written under the JSON key object.")]
def test_model_decode(t : T?) {
    t |> run("model entry round-trips through sprint_json/sscan_json") @(t : T?) {
        let m = Model(id = "gpt-test", _object = "model", created = 1700000000, owned_by = "openai")
        let js = sprint_json(m, false)
        t |> success(find(js, "\"object\":\"model\"") >= 0, "renamed object key emitted")
        var back : Model
        t |> success(sscan_json(js, back), "decode ok")
        t |> equal(back.id, "gpt-test")
        t |> equal(back._object, "model")
        t |> equal(back.owned_by, "openai")
    }
}

[test(description = "A live list_models against Ollama returns a non-empty list; skipped when no Ollama server answers.", timeout = 1200)]
def test_models_live_ollama(t : T?) {
    t |> run("live list_models against Ollama") @(t : T?) {
        return if (!live_server(t, "http://localhost:11434/v1"))
        let client = openai_client("http://localhost:11434/v1")
        let res = list_models(client)
        t |> success(res.ok, "list ok (kind={res.error.kind} status={res.error.status} {res.error.message})")
        if (res.ok) {
            t |> success(!empty(res.models), "models non-empty ({length(res.models)})")
        }
    }
}

[test(description = "A live retrieve_model against Ollama returns the requested model by id; skipped when no Ollama server answers.", timeout = 1200)]
def test_models_retrieve_ollama(t : T?) {
    t |> run("live retrieve_model against Ollama") @(t : T?) {
        return if (!live_server(t, "http://localhost:11434/v1"))
        let client = openai_client("http://localhost:11434/v1")
        let res = retrieve_model(client, "nomic-embed-text")
        t |> success(res.ok, "retrieve ok (kind={res.error.kind} status={res.error.status} {res.error.message})")
        if (res.ok) {
            t |> equal(res.model.id, "nomic-embed-text")
            t |> equal(res.model._object, "model")
        }
    }
}

[test(description = "A live list_models against OpenRouter authenticates a GET with the Bearer key and lists more than ten models; skipped without OPENROUTER_API_KEY, passed on a 429 rate limit.", timeout = 1200)]
def test_models_live_openrouter(t : T?) {
    t |> run("live list_models against OpenRouter (validates GET Bearer auth)") @(t : T?) {
        return if (!live_key(t, "OPENROUTER_API_KEY"))
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
