options gen2
options rtti  // required so sprint_json/sscan_json honor @optional / @rename
require openai/openai_audio
require daslib/fio

[export]
def main {
    // Point base_url at any local OpenAI-compatible TTS server (e.g. Kokoro-FastAPI on :8880).
    let client = openai_client("http://localhost:8880/v1")

    let audio = speak(client, "kokoro", "af_heart",
        "Welcome to daslang. This narration was synthesized locally through the OpenAI compatible module.")
    print("audio bytes: {length(audio)}\n")
    if (!empty(audio)) {
        fopen("tts_example.wav", "wb") $(f) {
            fwrite(f, audio)
        }
        print("wrote tts_example.wav\n")
    }
}
