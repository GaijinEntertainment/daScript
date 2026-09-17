options gen2
options persistent_heap
options stack = 524288
options _dasllama_internal = true
require dastest/testing_boost public
require dasllama/dasllama_tts
require daslib/json_boost
require daslib/math_bits

def private prompt_test_model() : TtsModel {
    var model = TtsModel(kind = TtsKind.pocket)
    model.pocket.language = "english_2026-04"
    model.pocket.lang = "en"
    model.pocket.sample_rate = 24000
    model.pocket.latent_dim = 32l
    model.pocket.frame_samples = 1920l
    return <- model
}

def private prompt_test_value() : TtsVoicePrompt {
    var prompt = TtsVoicePrompt(language = "english_2026-04", sample_rate = 24000, latent_dim = 32l, frame_samples = 1920l)
    resize(prompt.latents, 64)
    return <- prompt
}

[test]
def test_prepared_voice_registration_is_owned_and_transactional(t : T?) {
    var inscope model <- prompt_test_model()
    var inscope source_prompt <- prompt_test_value()
    t |> success(tts_register_voice_prompt(model, "extra", source_prompt), "encoder-free registration")
    source_prompt.latents[0] = .5
    t |> equal(model.pocket.voice_latents["extra"][0], 0.0, "caller retains independent storage")
    model.pocket.voices["extra"] <- [1.0]
    model.pocket.voice_states["extra"].len = 2l
    t |> success(tts_register_voice_prompt(model, "extra", source_prompt))
    t |> equal(length(model.pocket.voice_names), 1)
    t |> equal(model.pocket.voice_latents["extra"][0], .5)
    t |> success(!key_exists(model.pocket.voices, "extra"), "old PCM evicted")
    t |> success(!key_exists(model.pocket.voice_states, "extra"), "old conditioning state evicted")
    var inscope available <- caps(model)
    t |> equal(available.voices[0], "extra", "registered voice reaches public roster")
    t |> equal(tts_voice_lang(model, "extra"), "en", "registered voice reaches public lookup")
    var inscope restored_prompt = TtsVoicePrompt()
    t |> success(sscan_json(sprint_json(source_prompt, false), restored_prompt))
    t |> success(tts_voice_prompt_valid(model, restored_prompt))
    t |> success(!tts_register_voice_prompt(model, "", restored_prompt), "empty names refused")
    restored_prompt.language = "french"
    t |> success(!tts_register_voice_prompt(model, "extra", restored_prompt))
    t |> equal(model.pocket.voice_latents["extra"][0], .5, "rejection preserves the existing voice")
    t |> equal(length(model.pocket.voice_names), 1)
}

[test]
def test_prepared_voice_prompt_rejects_incompatible_data(t : T?) {
    for (invalid in ["kind", "version", "family", "language", "rate", "width", "stride", "empty", "partial", "too_long", "nan", "positive_inf", "negative_inf", "positive_bound", "negative_bound"]) {
        var inscope model <- prompt_test_model()
        var inscope prompt <- prompt_test_value()
        if (invalid == "kind") { model.kind = TtsKind.kitten }
        elif (invalid == "version") { prompt.version = 2 }
        elif (invalid == "family") { prompt.family = "other" }
        elif (invalid == "language") { prompt.language = "other" }
        elif (invalid == "rate") { prompt.sample_rate = 16000 }
        elif (invalid == "width") { prompt.latent_dim = 16l }
        elif (invalid == "stride") { prompt.frame_samples = 960l }
        elif (invalid == "empty") { resize(prompt.latents, 0) }
        elif (invalid == "partial") { resize(prompt.latents, 63) }
        elif (invalid == "too_long") { resize(prompt.latents, 24032) }
        elif (invalid == "nan") { prompt.latents[0] = uint_bits_to_float(0x7fc00000u) }
        elif (invalid == "positive_inf") { prompt.latents[0] = uint_bits_to_float(0x7f800000u) }
        elif (invalid == "negative_inf") { prompt.latents[0] = uint_bits_to_float(0xff800000u) }
        elif (invalid == "positive_bound") { prompt.latents[0] = 1000001.0 }
        else { prompt.latents[0] = -1000001.0 }
        t |> success(!tts_voice_prompt_valid(model, prompt), invalid)
        t |> success(!tts_register_voice_prompt(model, "invalid", prompt), invalid)
        t |> equal(length(model.pocket.voice_names), 0, "{invalid}: no half voice")
    }
    var inscope model <- prompt_test_model()
    var inscope prompt <- prompt_test_value()
    resize(prompt.latents, 24000)
    t |> success(tts_voice_prompt_valid(model, prompt), "exactly 60 seconds is accepted")
}

[test]
def test_prepared_voice_requires_valid_reference(t : T?) {
    for (invalid in ["kind", "encoder", "rate", "empty", "too_long"]) {
        var inscope model <- prompt_test_model()
        model.pocket.cloning = true
        var inscope pcm <- [0.0]
        var rate = 24000
        if (invalid == "kind") { model.kind = TtsKind.kitten }
        elif (invalid == "encoder") { model.pocket.cloning = false }
        elif (invalid == "rate") { rate = 16000 }
        elif (invalid == "empty") { resize(pcm, 0) }
        else { resize(pcm, 1440001) }
        var refused = false
        try {
            var inscope prompt <- tts_prepare_voice_prompt(model, pcm, rate)
        } recover {
            refused = true
        }
        t |> success(refused, invalid)
    }
}
