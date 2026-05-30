options gen2
options rtti  // required so sprint_json/sscan_json honor @optional / @rename
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost public
require openai/openai_embeddings
require strings

// ===== Unit: serialization (no backend needed) =====

[test]
def test_embeddings_request_serialization(t : T?) {
    t |> run("embeddings request includes input array, omits unset optionals") @(t : T?) {
        var req : EmbeddingRequest
        req.model = "m"
        req.input <- ["a", "b"]
        let js = sprint_json(req, false)
        t |> success(find(js, "\"model\":\"m\"") >= 0, "model present")
        t |> success(find(js, "\"input\":[\"a\",\"b\"]") >= 0, "input array present")
        t |> success(find(js, "\"dimensions\"") == -1, "unset dimensions omitted")
    }
}

[test]
def test_embeddings_response_decode(t : T?) {
    t |> run("embeddings response round-trips through sprint_json/sscan_json") @(t : T?) {
        var resp : EmbeddingResponse
        resp._object = "list"
        resp.model = "test-embed"
        resp.data <- [EmbeddingData(_object = "embedding", index = 0, embedding <- [0.1f, 0.2f, 0.3f])]
        resp.usage = EmbeddingUsage(prompt_tokens = 4, total_tokens = 4)
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

// ===== Live integration (requires Ollama with nomic-embed-text) =====

[test]
def test_embeddings_live_ollama(t : T?) {
    t |> run("live embeddings against Ollama (nomic-embed-text)") @(t : T?) {
        let client = openai_client("http://localhost:11434/v1")
        let v = embed(client, "nomic-embed-text", "hello world")
        t |> success(!empty(v), "embedding vector non-empty")
        if (!empty(v)) {
            t |> equal(length(v), 768)
        }
    }
}
