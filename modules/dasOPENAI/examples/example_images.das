options gen2
options rtti  // required so sprint_json/sscan_json honor @optional / @rename
require openai/openai_images
require daslib/fio

// Image generation has no local backend — run against OpenAI with OPENAI_API_KEY set.
[export]
def main {
    if (!has_env_variable("OPENAI_API_KEY")) {
        print("set OPENAI_API_KEY to run this example (image generation is OpenAI-cloud only)\n")
        return
    }
    let client = openai_client("https://api.openai.com/v1", get_env_variable("OPENAI_API_KEY"))
    let req = ImageRequest(prompt = "a friendly robot reading a book, flat illustration",
        model = "dall-e-3", size = "1024x1024", n = 1)
    let res = generate_image(client, req)
    if (res.ok && !empty(res.response.data)) {
        print("image url: {res.response.data[0].url}\n")
    } else {
        print("error [{res.error.kind}] {res.error.message}\n")
    }
}
