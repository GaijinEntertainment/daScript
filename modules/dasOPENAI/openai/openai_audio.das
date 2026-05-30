options gen2
options indenting = 4
options rtti  // honor @optional / @rename (consumer roots must also set it)

//! OpenAI-compatible API — audio: speech synthesis (TTS) and transcription / translation (STT).

module openai_audio shared public

require openai/openai_common public

// ===== Speech (TTS) =====

struct SpeechRequest {
    //! Request body for ``/audio/speech``. The response is raw audio bytes, not JSON.
    model : string
    input : string
    voice : string
    @optional response_format : string
    @optional speed : float
}

struct SpeechResult {
    //! Outcome of ``speech``: ``audio`` holds the raw bytes when ``ok`` is true.
    ok : bool
    audio : array<uint8>
    error : OpenAIError
}

def speech(client : OpenAIClient; req : SpeechRequest) : SpeechResult {
    //! Synthesizes speech (POST /audio/speech); returns the raw audio bytes.
    var result : SpeechResult
    var out <- post_for_bytes(client, "/audio/speech", sprint_json(req, false))
    result.ok = out.ok
    if (!out.ok) {
        result.error = out.error
        return <- result
    }
    result.audio <- out.bytes
    return <- result
}

def speak(client : OpenAIClient; model, voice, text : string) : array<uint8> {
    //! Convenience: synthesize ``text`` with ``voice``, returns audio bytes (empty on error).
    var bytes : array<uint8>
    let req = SpeechRequest(model = model, voice = voice, input = text)
    var res <- speech(client, req)
    if (res.ok) {
        bytes <- res.audio
    }
    return <- bytes
}

// ===== Transcriptions / translations (STT) =====

struct TranscriptionResult {
    //! Outcome of ``transcribe`` / ``translate``: ``text`` valid when ``ok`` is true.
    ok : bool
    text : string
    error : OpenAIError
}

struct private TranscriptionResponse {
    text : string
}

def private post_audio_file(client : OpenAIClient; path, file_path, model : string) : HttpOutcome {
    var out : HttpOutcome
    with_http_request() $(var hreq : HttpRequest?#) {
        hreq.method = http_method.POST
        hreq.url := "{client.base_url}{path}"
        configure_request(hreq, client)
        set_form_file(hreq, "file", file_path)
        set_form_data(hreq, "model", model)
        set_form_data(hreq, "response_format", "json")
        request(hreq) $(var resp : HttpResponse?) {
            if (resp == null) {
                out.error = OpenAIError(kind = "transport", message = "no response from {client.base_url}{path}")
                return
            }
            out.status = int(resp.status_code)
            out.body = "{resp.body}"
            if (out.status >= 200 && out.status < 300) {
                out.ok = true
            } else {
                out.error = parse_error_body(out.status, out.body)
            }
        }
    }
    return out
}

def private decode_transcription(out : HttpOutcome) : TranscriptionResult {
    var result : TranscriptionResult
    result.ok = out.ok
    if (!out.ok) {
        result.error = out.error
        return result
    }
    var resp : TranscriptionResponse
    if (sscan_json(out.body, resp)) {
        result.text = resp.text
    } else {
        result.ok = false
        result.error = OpenAIError(kind = "decode", status = out.status, message = "failed to decode transcription")
    }
    return result
}

def transcribe(client : OpenAIClient; model : string; file_path : string) : TranscriptionResult {
    //! Transcribes the audio file at ``file_path`` (POST /audio/transcriptions).
    return decode_transcription(post_audio_file(client, "/audio/transcriptions", file_path, model))
}

def translate(client : OpenAIClient; model : string; file_path : string) : TranscriptionResult {
    //! Translates the audio file at ``file_path`` into English (POST /audio/translations).
    return decode_transcription(post_audio_file(client, "/audio/translations", file_path, model))
}
