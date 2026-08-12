module cadmus_rules public

options gen2

require strings
require daslib/strings_boost
require daslib/strings_convert

def is_cadmus_command(text, name : string) : bool {
    let lower = to_lower(strip(text))
    let tokens <- split_by_chars(lower, " \t\r\n")
    if (empty(tokens)) {
        return false
    }
    let command = tokens[0]
    return command == "/{name}" || starts_with(command, "/{name}@")
}

def is_cadmus_command_for_bot(text, name, bot_username : string) : bool {
    let lower = to_lower(strip(text))
    let tokens <- split_by_chars(lower, " \t\r\n")
    if (empty(tokens)) {
        return false
    }
    let command = tokens[0]
    if (command == "/{name}") {
        return true
    }
    let target = to_lower(strip(trim_prefix(bot_username, "@")))
    return !empty(target) && command == "/{name}@{target}"
}

def private has_alias_boundary(text, alias : string) : bool {
    return (text == alias || starts_with(text, "{alias} ") || starts_with(text, "{alias}\t") ||
            starts_with(text, "{alias}\r") || starts_with(text, "{alias}\n") || starts_with(text, "{alias},") ||
            starts_with(text, "{alias}:") || starts_with(text, "{alias}!") || starts_with(text, "{alias}?"))
}

def starts_with_cadmus_alias(text, aliases_csv : string) : bool {
    let original = strip(text)
    let lower = to_lower(strip(text))
    let aliases <- split(aliases_csv, ",")
    for (alias in aliases) {
        let exact_alias = strip(alias)
        let lower_alias = to_lower(exact_alias)
        if (!empty(exact_alias) &&
            (has_alias_boundary(original, exact_alias) || has_alias_boundary(lower, lower_alias))) {
            return true
        }
    }
    return false
}

def starts_with_cadmus_username(text, bot_username : string) : bool {
    let target = to_lower(strip(trim_prefix(bot_username, "@")))
    return !empty(target) && has_alias_boundary(to_lower(strip(text)), "@{target}")
}

def should_cadmus_reply(
                        replies_enabled, is_command, is_edit, is_transcript_reply, is_addressed : bool
                        ) : bool {
    if (is_edit) {
        return false
    }
    if (is_command) {
        return true
    }
    if (is_transcript_reply) {
        return false
    }
    return replies_enabled && is_addressed
}

def is_legacy_cadmus_transcript_reply(
                                      replied_by_bot, remembered_assistant : bool;
                                      replied_text : string
                                      ) : bool {
    return replied_by_bot && !remembered_assistant && starts_with(strip(replied_text), "🎙 ")
}

def requires_cadmus_history_tools(text : string) : bool {
    let lower = to_lower(strip(text))
    return (lower |> contains("in this chat") || lower |> contains("in the chat") ||
            lower |> contains("chat history") || lower |> contains("who first") ||
            lower |> contains("who last") || lower |> contains("when did we") ||
            lower |> contains("в чате") || lower |> contains("из чата") ||
            lower |> contains("переписк") || lower |> contains("кто первый") ||
            lower |> contains("кто последний") || lower |> contains("когда мы") ||
            lower |> contains("упомянул") || lower |> contains("упоминал"))
}

def is_cadmus_source_followup(text : string) : bool {
    let lower = to_lower(strip(text))
    return (lower |> contains("there") || lower |> contains("that message") ||
            lower |> contains("that discussion") || lower |> contains("around it") ||
            lower |> contains("what were we discussing") || lower |> contains("what was discussed") ||
            lower |> contains("the context") || lower |> contains("там") ||
            lower |> contains("в том сообщении") || lower |> contains("вокруг этого") ||
            lower |> contains("что мы обсуждали") || lower |> contains("что там обсуждали") ||
            lower |> contains("контекст"))
}

def quote_cadmus_history(text : string) : string {
    let json = sprint_json(text, false)
    return replace_multiple(json, [
        (text = "<", replacement = "\\u003c"),
        (text = ">", replacement = "\\u003e")])
}

def sanitize_cadmus_reply(text : string; var clean, error : string&) : bool {
    clean = ""
    error = ""
    var ok = true
    peek_data(text) $(d) {
        let n = length(d)
        // consuming a leading block advances the cursor - the reply is materialized once, at the end
        var pos = skip_white_space(d, 0)
        while (starts_with(d, pos, "<think>")) {
            let close_at = find(d, "</think>", pos)
            if (close_at < 0) {
                error = "model response contained an unterminated thinking block"
                ok = false
                return
            }
            pos = skip_white_space(d, close_at + length("</think>"))
        }
        if (find(d, "<think>", pos) >= 0 || find(d, "</think>", pos) >= 0) {
            error = "model response contained a thinking block outside the response preamble"
            ok = false
            return
        }
        clean = strip(slice(d, pos, n))
    }
    if (!ok) {
        return false
    }
    // peek_data never calls the block on an empty text, so clean is still "" there
    if (empty(clean)) {
        error = "model response contained thinking but no final answer"
        return false
    }
    return true
}

def cadmus_rolling_24h(now : int64; var from_time, until_time : int64&) {
    from_time = now - 86400l
    until_time = now + 1l
}

def parse_cadmus_local_date(value : string; var from_time, until_time : int64&) : bool {
    let parts <- split(value, "-")
    if (length(parts) != 3) {
        return false
    }
    let year_r = try_to_int(parts[0])
    let month_r = try_to_int(parts[1])
    let day_r = try_to_int(parts[2])
    if (year_r |> is_err || month_r |> is_err || day_r |> is_err) {
        return false
    }
    let year = year_r |> unwrap
    let month = month_r |> unwrap
    let day = day_r |> unwrap
    if (year < 1970 || month < 1 || month > 12 || day < 1 || day > 31) {
        return false
    }
    from_time = int64(mktime(year, month, day, 0, 0, 0))
    until_time = int64(mktime(year, month, day + 1, 0, 0, 0))
    return format_time(mktime(year, month, day, 12, 0, 0), "%Y-%m-%d") == value
}

// An ASR refusal that describes the input itself, not a transient server condition. Retrying one of
// these re-runs the same rejected audio through the same model for the same answer, so the queue
// spends its whole budget at once instead of re-attempting on every bot restart.
def is_permanent_asr_failure(error : string) : bool {
    let lower = to_lower(error)
    return (lower |> contains("crosses the local-attention switch") ||
            lower |> contains("clip audio to under") ||
            lower |> contains("unsupported audio format") ||
            lower |> contains("audio is empty"))
}
