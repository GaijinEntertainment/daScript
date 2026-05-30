options gen2
options rtti  // required so sprint_json/sscan_json honor @optional / @rename
require openai/openai_completions

[export]
def main {
    let client = openai_client("http://localhost:11434/v1")
    let text = complete(client, "hf.co/LiquidAI/LFM2.5-1.2B-Instruct-GGUF:Q4_K_M",
        "The capital of France is")
    print("completion: {text}\n")
}
