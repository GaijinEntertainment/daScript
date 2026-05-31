// Tutorial OPENAI-06: Audio (speech and transcription)
//
// This tutorial covers:
//   - Text-to-speech: speak() returns raw audio bytes
//   - Writing audio bytes to a file
//   - Speech-to-text: transcribe() uploads an audio file and returns text
//
// Prerequisites: Tutorial OPENAI-01
//
// Run: daslang.exe tutorials/dasOPENAI/06_audio.das

options gen2
options rtti
options persistent_heap
options gc

require openai/openai_audio
require tutorial_openai_server
require daslib/fio

let SERVER_PORT = 18195

// ──────────────────────────────────────────────────────────────────────────
// Section 1 — Text to speech
// ──────────────────────────────────────────────────────────────────────────
//
// speak() synthesizes text into audio and returns the raw bytes
// (array<uint8>, empty on error). Write them to a file to play later, or
// stream them onward. The byte format is whatever the server produced.

def example_tts(base_url : string) {
    let client = openai_client(base_url)
    var audio <- speak(client, "mock-tts", "alloy", "Hello from daslang.")
    print("received {length(audio)} audio bytes\n")

    let path = "tutorial_tts_out.wav"
    fopen(path, "wb") $(f) {
        fwrite(f, audio)
    }
    print("wrote {path}\n")
    remove(path)
    delete audio
}

// ──────────────────────────────────────────────────────────────────────────
// Section 2 — Speech to text
// ──────────────────────────────────────────────────────────────────────────
//
// transcribe() uploads an audio file (multipart) and returns the recognized
// text. Here we make a throwaway input file; against a real server you would
// pass a genuine recording.

def example_stt(base_url : string) {
    let client = openai_client(base_url)
    let path = "tutorial_stt_in.wav"
    fopen(path, "wb") $(f) {
        fwrite(f, "fake wav bytes for the mock server")
    }
    let res = transcribe(client, "whisper-1", path)
    remove(path)
    if (res.ok) {
        print("transcript: {res.text}\n")
    } else {
        print("error [{res.error.kind}/{res.error.status}]: {res.error.message}\n")
    }
}

[export]
def main() {
    with_openai_server(SERVER_PORT) $(base_url) {
        print("=== text to speech ===\n")
        example_tts(base_url)

        print("\n=== speech to text ===\n")
        example_stt(base_url)
    }
}
