options gen2
options rtti  // required so sprint_json/sscan_json honor @optional / @rename
require openai/openai_audio
require daslib/fio

[export]
def main {
    let client = openai_client("http://localhost:8880/v1")

    // synthesize, then transcribe back — a full audio round-trip
    let audio = speak(client, "kokoro", "af_heart", "Daslang transcription round trip.")
    let path = "rt_example.wav"
    fopen(path, "wb") $(f) {
        fwrite(f, audio)
    }

    let res = transcribe(client, "whisper-1", path)
    if (res.ok) {
        print("transcript: {res.text}\n")
    } else {
        print("error [{res.error.kind}] {res.error.message}\n")
    }
}
