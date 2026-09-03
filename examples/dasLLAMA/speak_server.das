options gen2
options stack = 524288   // the budget every program root here takes (options stack does not unify up from libs)

require dashv/dashv_boost     // POST + get_body_bytes — the outbound HTTP client
require daslib/clargs
require daslib/fio

// Speak a line through a running dasllama-server: POST the OpenAI speech shape to
// /v1/audio/speech, write the WAV body to a file, report how much audio came back and how
// long the round trip took. No model is loaded here — the server owns it, so this client
// starts instantly and any OpenAI speech client would work the same way.
//
// Start the server first — --tts is what arms the speech route:
//   bin/daslang -jit utils/dasllama-server/main.das -- --model <llm.gguf> --tts <kitten-nano.gguf> --port 8080
// then:
//   bin/daslang examples/dasLLAMA/speak_server.das -- --text "Hello from the server." --out hello.wav

[CommandLineArgs]
struct SpeakArgs {
    @clarg_short = "u"
    @clarg_doc = "Base URL of the dasllama-server (default http://127.0.0.1:8080)"
    url : string

    @clarg_short = "t"
    @clarg_doc = "The text to speak"
    text : string = "The server speaks so this program does not have to."

    @clarg_short = "o"
    @clarg_doc = "The WAV file to write"
    out : string = "speech.wav"

    @clarg_short = "v"
    @clarg_doc = "Voice name (default: the server's first voice)"
    voice : string

    @clarg_short = "s"
    @clarg_doc = "Speech speed, 0.25 to 4.0"
    speed : float = 1.0

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

def private le16(b : array<uint8>; o : int) : int {
    return int(b[o]) | (int(b[o + 1]) << 8)
}

def private le32(b : array<uint8>; o : int) : int {
    return le16(b, o) | (le16(b, o + 2) << 16)
}

// seconds of PCM behind a canonical 44-byte RIFF/WAVE header, or 0 when the body is not one
def private wav_seconds(b : array<uint8>) : float {
    return 0.0 if (length(b) < 44 || int(b[0]) != 'R' || int(b[8]) != 'W')
    let rate = le32(b, 24)
    let bytes_per_frame = le16(b, 22) * le16(b, 34) / 8
    return rate <= 0 || bytes_per_frame <= 0 ? 0.0 : float(le32(b, 40)) / float(rate * bytes_per_frame)
}

def private write_all(path : string; bytes : array<uint8>) : bool {
    let f = fopen(path, "wb")
    return false if (f == null)
    fwrite(f, bytes)
    fclose(f)
    return true
}

[export]
def main : int {
    var inscope r <- parse_args(type<SpeakArgs>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<SpeakArgs>), "speak_server")
        return 1
    }
    var inscope cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<SpeakArgs>), "speak_server")
        return 0
    }
    let base = empty(cfg.url) ? "http://127.0.0.1:8080" : cfg.url
    let body = "\{\"input\": \"{cfg.text}\", \"voice\": \"{cfg.voice}\", \"speed\": {cfg.speed}}"

    var status = 0
    var message = ""
    var n_bytes = 0
    var secs = 0.0
    var written = false
    let clock = ref_time_ticks()
    POST("{base}/v1/audio/speech", body) $(resp) {
        if (resp == null) {
            message = "no answer from {base} — is dasllama-server running?"
            return
        }
        status = int(resp.status_code)
        message = "{resp.body}"
        return if (status != 200)
        var inscope wav <- get_body_bytes(resp)
        n_bytes = length(wav)
        secs = wav_seconds(wav)
        written = write_all(cfg.out, wav)
    }
    let wall = float(get_time_usec(clock)) / 1.0e6

    if (status != 200) {
        print(status == 0 ? "{message}\n" : "HTTP {status}: {message}\n")
        return 1
    }
    if (!written) {
        print("cannot write {cfg.out}\n")
        return 1
    }
    print("{cfg.out}: {secs} s of audio, {n_bytes} bytes, in {wall} s")
    print(secs > 0.0 ? " (rtf {wall / secs})\n" : "\n")
    return 0
}
