options gen2
options rtti  // required so sprint_json/sscan_json honor @optional / @rename
require openai/openai_embeddings

[export]
def main {
    let client = openai_client("http://localhost:11434/v1")

    // convenience: single string -> vector
    let v = embed(client, "nomic-embed-text", "The quick brown fox.")
    print("embedding dim: {length(v)}\n")
    if (length(v) >= 3) {
        print("first 3: {v[0]}, {v[1]}, {v[2]}\n")
    }

    // full API: batch of inputs
    let req = EmbeddingRequest(model = "nomic-embed-text", input <- ["hello", "world", "embeddings"])
    let res = embeddings(client, req)
    if (res.ok) {
        print("batch count: {length(res.response.data)} total_tokens: {res.response.usage.total_tokens}\n")
    } else {
        print("error [{res.error.kind}] {res.error.message}\n")
    }
}
