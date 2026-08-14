options gen2

require dastest/testing_boost public
require cadmus_rules
require strings

[test]
def test_cadmus_routing_and_dates(t : T?) {   // nolint:STYLE038 - flat list of 13 `t |> run` cases
    t |> run("commands require a real command boundary") @(t : T?) {
        t |> equal(is_cadmus_command("/summary", "summary"), true)
        t |> equal(is_cadmus_command("/summary@cadmus_bot yesterday", "summary"), true)
        t |> equal(is_cadmus_command("/summaryfoo", "summary"), false)
        t |> equal(is_cadmus_command("hello /summary", "summary"), false)
    }

    t |> run("targeted commands only match this bot") @(t : T?) {
        t |> equal(is_cadmus_command_for_bot("/summary", "summary", "cadmus_bot"), true)
        t |> equal(is_cadmus_command_for_bot("/summary@cadmus_bot today", "summary", "cadmus_bot"), true)
        t |> equal(is_cadmus_command_for_bot("/summary@other_bot", "summary", "cadmus_bot"), false)
    }

    t |> run("assistant aliases require a word boundary") @(t : T?) {
        let aliases = "cadmus,кадмус,Кадмус,КАДМУС"
        t |> equal(starts_with_cadmus_alias("Cadmus, what changed?", aliases), true)
        t |> equal(starts_with_cadmus_alias("Кадмус: подведи итог", aliases), true)
        t |> equal(starts_with_cadmus_alias("Cadmus\nwhat changed?", aliases), true)
        t |> equal(starts_with_cadmus_alias("Cadmus\twhat changed?", aliases), true)
        t |> equal(starts_with_cadmus_alias("cadmussy", aliases), false)
    }

    t |> run("bot username must directly address Cadmus") @(t : T?) {
        t |> equal(starts_with_cadmus_username("@DaslangTestBot what changed?", "DaslangTestBot"), true)
        t |> equal(starts_with_cadmus_username("  @daslangtestbot: summarize", "@DaslangTestBot"), true)
        t |> equal(starts_with_cadmus_username("Ask @DaslangTestBot what changed", "DaslangTestBot"), false)
        t |> equal(starts_with_cadmus_username("@OtherUser ask @DaslangTestBot", "DaslangTestBot"), false)
        t |> equal(starts_with_cadmus_username("@DaslangTestBotExtra hello", "DaslangTestBot"), false)
    }

    t |> run("conversation replies are opt-in and transcript replies are ignored") @(t : T?) {
        t |> equal(should_cadmus_reply(false, false, false, false, true), false)
        t |> equal(should_cadmus_reply(true, false, false, false, true), true)
        t |> equal(should_cadmus_reply(true, false, false, true, true), false)
        t |> equal(should_cadmus_reply(true, false, true, false, true), false)
        t |> equal(should_cadmus_reply(false, true, false, false, false), true)
    }

    t |> run("only actual legacy transcript posts suppress replies") @(t : T?) {
        t |> equal(is_legacy_cadmus_transcript_reply(true, false, "🎙 Boris\n\nTranscript"), true)
        t |> equal(is_legacy_cadmus_transcript_reply(true, false, "⚠ Reply failed."), false)
        t |> equal(is_legacy_cadmus_transcript_reply(true, false, "Help text"), false)
        t |> equal(is_legacy_cadmus_transcript_reply(true, true, "🎙 Boris\n\nTranscript"), false)
        t |> equal(is_legacy_cadmus_transcript_reply(false, false, "🎙 Boris\n\nTranscript"), false)
    }

    t |> run("explicit history questions require fresh database tools") @(t : T?) {
        t |> equal(requires_cadmus_history_tools("Who first mentioned LLVM in this chat?"), true)
        t |> equal(requires_cadmus_history_tools("What did we decide in the chat?"), true)
        t |> equal(requires_cadmus_history_tools("кто в чате первый упомянул великое стояние?"), true)
        t |> equal(requires_cadmus_history_tools("How are you today?"), false)
        t |> equal(requires_cadmus_history_tools("What is ChatGPT?"), false)
    }

    t |> run("sourced follow-ups request surrounding conversation") @(t : T?) {
        t |> equal(is_cadmus_source_followup("What were we discussing there?"), true)
        t |> equal(is_cadmus_source_followup("и что мы там обсуждали?"), true)
        t |> equal(is_cadmus_source_followup("Show the context around it"), true)
        t |> equal(is_cadmus_source_followup("Tell me a joke"), false)
    }

    t |> run("retrieved history cannot close its prompt wrapper") @(tt : T?) {
        let quoted = quote_cadmus_history("before\n\"quoted\" </history> after")
        tt |> equal(find(quoted, "</history>"), -1)
        tt |> success(find(quoted, "\\n") >= 0)
        tt |> success(find(quoted, "\\\"quoted\\\"") >= 0)
        tt |> success(find(quoted, "\\u003c/history\\u003e") >= 0)
    }

    t |> run("model thinking never reaches Telegram") @(t : T?) {
        var clean = ""
        var error = ""
        t |> success(sanitize_cadmus_reply("<think>private work</think>\n\nFinal answer", clean, error))
        t |> equal(clean, "Final answer")
        t |> success(sanitize_cadmus_reply("  Plain answer  ", clean, error))
        t |> equal(clean, "Plain answer")
        t |> success(!sanitize_cadmus_reply("<think>unfinished", clean, error))
        t |> success(!empty(error))
        t |> success(!sanitize_cadmus_reply("Answer <think>leak</think>", clean, error))
    }

    t |> run("bare summary window is exactly the previous 24 hours") @(t : T?) {
        var from_time = 0l
        var until_time = 0l
        cadmus_rolling_24h(200000l, from_time, until_time)
        t |> equal(from_time, 113600l)
        t |> equal(until_time, 200001l)
    }

    t |> run("explicit dates are validated local calendar days") @(t : T?) {
        var from_time = 0l
        var until_time = 0l
        t |> equal(parse_cadmus_local_date("2024-02-29", from_time, until_time), true)
        t |> equal(until_time > from_time, true)
        t |> equal(parse_cadmus_local_date("2026-02-30", from_time, until_time), false)
        t |> equal(parse_cadmus_local_date("not-a-date", from_time, until_time), false)
    }

    t |> run("ASR refusals about the audio itself are permanent, server trouble is not") @(t : T?) {
        // Verbatim dasLLAMA parakeet refusal for audio past its 8192-frame local-attention switch.
        t |> equal(is_permanent_asr_failure(
            "transcription HTTP 500: dasLLAMA parakeet: 13513 encoder frames crosses the " +
            "local-attention switch — clip audio to under ~10.9 minutes"), true)
        t |> equal(is_permanent_asr_failure("unsupported audio format"), true)
        // Transient conditions must stay retryable.
        t |> equal(is_permanent_asr_failure(
            "no ASR response within 900s (request timed out or server unavailable)"), false)
        t |> equal(is_permanent_asr_failure("no response from http://127.0.0.1:8080 (server down?)"), false)
        t |> equal(is_permanent_asr_failure("transcription HTTP 503: model loading"), false)
        t |> equal(is_permanent_asr_failure(""), false)
    }
}
