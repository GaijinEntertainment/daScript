options gen2
options rtti  // required so sprint_json/sscan_json honor @optional / @rename
require openai/openai_vision
require daslib/fio

// Vision needs a vision-capable model — run against OpenRouter with OPENROUTER_API_KEY set.
[export]
def main {
    if (!has_env_variable("OPENROUTER_API_KEY")) {
        print("set OPENROUTER_API_KEY to run this vision example\n")
        return
    }
    let client = openai_client("https://openrouter.ai/api/v1", get_env_variable("OPENROUTER_API_KEY"))
    let image = "https://upload.wikimedia.org/wikipedia/commons/thumb/3/3a/Cat03.jpg/240px-Cat03.jpg"
    let res = chat_vision(client, "google/gemma-4-31b-it:free", "What animal is in this image? Answer in one word.", image)
    if (res.ok && !empty(res.response.choices)) {
        print("vision reply: {res.response.choices[0].message.content}\n")
    } else {
        print("error [{res.error.kind}] {res.error.message}\n")
    }
}
