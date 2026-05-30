options gen2
options rtti  // required so sprint_json/sscan_json honor @optional / @rename
require openai/openai_moderations
require daslib/fio

// Moderations has no local backend — run against OpenAI with OPENAI_API_KEY set.
[export]
def main {
    if (!has_env_variable("OPENAI_API_KEY")) {
        print("set OPENAI_API_KEY to run this example (moderations is OpenAI-cloud only)\n")
        return
    }
    let client = openai_client("https://api.openai.com/v1", get_env_variable("OPENAI_API_KEY"))
    let req = ModerationRequest(model = "omni-moderation-latest", input <- ["I want to hurt someone."])
    let res = moderations(client, req)
    if (res.ok && !empty(res.response.results)) {
        print("flagged: {res.response.results[0].flagged} (across {length(res.response.results[0].categories)} categories)\n")
    } else {
        print("error [{res.error.kind}] {res.error.message}\n")
    }
}
