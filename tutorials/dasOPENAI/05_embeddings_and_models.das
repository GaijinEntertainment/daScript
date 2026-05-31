// Tutorial OPENAI-05: Embeddings and Models
//
// This tutorial covers:
//   - Embedding text into vectors with embed()
//   - Comparing vectors with cosine similarity
//   - Listing models (list_models) and retrieving one (retrieve_model)
//   - Why model ids are percent-encoded in the URL path
//
// Prerequisites: Tutorial OPENAI-01
//
// Run: daslang.exe tutorials/dasOPENAI/05_embeddings_and_models.das

options gen2
options rtti
options persistent_heap
options gc

require openai/openai_embeddings
require openai/openai_models
require openai/openai_common   // percent_encode
require tutorial_openai_server
require math

let SERVER_PORT = 18194

// ──────────────────────────────────────────────────────────────────────────
// Section 1 — Embeddings and cosine similarity
// ──────────────────────────────────────────────────────────────────────────
//
// embed() turns a string into a vector of floats. Semantically similar texts
// produce vectors that point in similar directions — measured by cosine
// similarity (1.0 = identical direction, 0.0 = orthogonal).
//
// (The mock returns a fixed vector, so the similarity here is 1.0; against a
// real embeddings model the value reflects actual semantic closeness.)

def cosine(a, b : array<float>) : float {
    if (length(a) != length(b) || empty(a)) return 0.0
    var dot = 0.0
    var na = 0.0
    var nb = 0.0
    for (i in range(length(a))) {
        dot += a[i] * b[i]
        na += a[i] * a[i]
        nb += b[i] * b[i]
    }
    return dot / (sqrt(na) * sqrt(nb))
}

def example_embeddings(base_url : string) {
    let client = openai_client(base_url)
    let v1 = embed(client, "mock-embed", "a cat sat on the mat")
    let v2 = embed(client, "mock-embed", "the feline rested on the rug")
    print("vector length: {length(v1)}\n")
    print("cosine similarity: {cosine(v1, v2)}\n")
}

// ──────────────────────────────────────────────────────────────────────────
// Section 2 — Listing and retrieving models
// ──────────────────────────────────────────────────────────────────────────

def example_models(base_url : string) {
    let client = openai_client(base_url)
    let list = list_models(client)
    if (list.ok) {
        print("available models:\n")
        for (m in list.models) {
            print("  {m.id} (owned_by {m.owned_by})\n")
        }
    }
    let one = retrieve_model(client, "mock-model")
    if (one.ok) {
        print("retrieved: {one.model.id}\n")
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Section 3 — Percent-encoded model ids
// ──────────────────────────────────────────────────────────────────────────
//
// Many providers use model ids with slashes (e.g. "google/gemma-2:free").
// retrieve_model percent-encodes the id so the '/' is not parsed as extra
// path segments. percent_encode is the helper that does it.

def example_percent_encode() {
    let id = "google/gemma-2:free"
    print("{id} -> {percent_encode(id)}\n")
}

[export]
def main() {
    with_openai_server(SERVER_PORT) $(base_url) {
        print("=== embeddings ===\n")
        example_embeddings(base_url)

        print("\n=== models ===\n")
        example_models(base_url)

        print("\n=== percent-encoded ids ===\n")
        example_percent_encode()
    }
}
