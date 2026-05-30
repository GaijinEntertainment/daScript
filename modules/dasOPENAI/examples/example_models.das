options gen2
options rtti  // required so sprint_json/sscan_json honor @optional / @rename
require openai/openai_models

[export]
def main {
    let client = openai_client("http://localhost:11434/v1")

    let res = list_models(client)
    if (res.ok) {
        print("Ollama models ({length(res.models)}):\n")
        for (m in res.models) {
            print("  {m.id} (owned_by={m.owned_by})\n")
        }
    } else {
        print("error [{res.error.kind}] {res.error.message}\n")
    }

    let one = retrieve_model(client, "nomic-embed-text")
    if (one.ok) {
        print("retrieved: {one.model.id} object={one.model._object} owned_by={one.model.owned_by}\n")
    }
}
