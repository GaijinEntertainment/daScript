options gen2

module telegram_history_import public

require pugixml/PUGIXML_boost
require sqlite/sqlite_migrate
require daslib/defer
require daslib/fio
require daslib/json_boost
require daslib/strings_boost
require daslib/strings_convert
require cadmus_history

struct TelegramExportMessage {
    MessageId : int64
    SenderName : string
    MessageDate : int64
    ReplyToMessageId : int64
    Text : string
    ForwardedFrom : string
    MediaPath : string
    SourceKind : string
}

struct TelegramImportPageResult {
    Messages : int
    Media : int
    MissingSender : int
    InvalidDate : int
}

struct TelegramRawAsrConfig {
    Server : string = "http://127.0.0.1:8080"
    Language : string = "auto"
    Ffmpeg : string = "ffmpeg"
    Curl : string = "curl"
}

def private parse_required_int(text, field_name : string; var value : int&; var error : string&) : bool {
    let parsed = try_to_int(text)
    if (parsed |> is_err) {
        error = "invalid {field_name}: {text}"
        return false
    }
    value = parsed |> unwrap
    return true
}

// Telegram Desktop emits e.g. "14.06.2022 15:21:58 UTC-08:00". Convert civil time directly
// rather than using host-local mktime, so imports are reproducible in every timezone.
def parse_telegram_export_timestamp(text : string; var epoch : int64&; var error : string&) : bool {
    let parts <- split(strip(text), " ")
    if (length(parts) != 3) {
        error = "unexpected Telegram timestamp: {text}"
        return false
    }
    let date_parts <- split(parts[0], ".")
    let time_parts <- split(parts[1], ":")
    if (length(date_parts) != 3 || length(time_parts) != 3 || !starts_with(parts[2], "UTC") ||
        length(parts[2]) != 9) {
        error = "unexpected Telegram timestamp: {text}"
        return false
    }
    var day = 0
    var month = 0
    var year = 0
    var hour = 0
    var minute = 0
    var second = 0
    var zone_hour = 0
    var zone_minute = 0
    if (!parse_required_int(date_parts[0], "day", day, error) ||
        !parse_required_int(date_parts[1], "month", month, error) ||
        !parse_required_int(date_parts[2], "year", year, error) ||
        !parse_required_int(time_parts[0], "hour", hour, error) ||
        !parse_required_int(time_parts[1], "minute", minute, error) ||
        !parse_required_int(time_parts[2], "second", second, error) ||
        !parse_required_int(slice(parts[2], 4, 6), "timezone hour", zone_hour, error) ||
        !parse_required_int(slice(parts[2], 7, 9), "timezone minute", zone_minute, error)) {
        return false
    }
    let zone_sign = slice(parts[2], 3, 4)
    if ((zone_sign != "+" && zone_sign != "-") || month < 1 || month > 12 || day < 1 || day > 31 ||
        hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 60 ||
        zone_hour < 0 || zone_hour > 23 || zone_minute < 0 || zone_minute > 59) {
        error = "out-of-range Telegram timestamp: {text}"
        return false
    }
    let adjusted_year = year - (month <= 2 ? 1 : 0)
    let era = (adjusted_year >= 0 ? adjusted_year : adjusted_year - 399) / 400
    let year_of_era = adjusted_year - era * 400
    let shifted_month = month + (month > 2 ? -3 : 9)
    let day_of_year = (153 * shifted_month + 2) / 5 + day - 1
    let day_of_era = year_of_era * 365 + year_of_era / 4 - year_of_era / 100 + day_of_year
    let days = int64(era * 146097 + day_of_era - 719468)
    let zone_seconds = int64(zone_hour * 3600 + zone_minute * 60) * (zone_sign == "-" ? -1l : 1l)
    epoch = days * 86400l + int64(hour * 3600 + minute * 60 + second) - zone_seconds
    return true
}

def telegram_export_sender_id(name : string) : int64 {
    var hash = 1469598103934665603ul
    peek_data(to_lower(strip(name))) $(bytes) {
        for (byte in bytes) {
            hash = (hash ^ uint64(byte)) * 1099511628211ul
        }
    }
    let positive = int64(hash & 0x3FFFFFFFFFFFFFFFul)
    return -(positive + 1l)
}

def private node_text_with_breaks(node : xml_node) : string {
    return build_string() $(var writer) {
        for (child in each_child(node)) {
            if (child._type == xml_node_type.node_pcdata || child._type == xml_node_type.node_cdata) {
                writer |> write(child.value)
            } elif (child._type == xml_node_type.node_element && child.name == "br") {
                writer |> write("\n")
            } elif (child._type == xml_node_type.node_element) {
                writer |> write(node_text_with_breaks(child))
            }
        }
    }
}

def private selected_node_text(node : xml_node; xpath : string) : string {
    let selected = node.select_node(xpath)
    return selected.ok ? strip(node_text_with_breaks(selected.node)) : ""
}

def private selected_attribute(node : xml_node; xpath : string) : string {
    let selected = node.select_node(xpath)
    return selected.ok ? clone_string(selected.attribute.value) : ""
}

def private parse_message_id(value : string; prefix : string) : int64 {
    if (!starts_with(value, prefix)) {
        return 0l
    }
    let parsed = try_to_int64(slice(value, length(prefix)))
    return parsed |> is_ok ? parsed |> unwrap : 0l
}

def private media_kind(path : string) : string {
    return "imported_voice" if (find(to_generic_path(path), "voice_messages/") >= 0)
    return "imported_round_video" if (find(to_generic_path(path), "round_video_messages/") >= 0)
    return empty(path) ? "imported_text" : "imported_media"
}

def parse_telegram_export_page(
                               html : string;
                               var current_sender : string&;
                               var error : string&
                               ) : array<TelegramExportMessage> {
    var result : array<TelegramExportMessage>
    // Telegram Desktop can emit `<a ="" href="...">` for bot buttons. An empty attribute name is
    // invalid XML; removing that exact invalid token preserves the anchor and its useful href.
    let xml = replace(
        replace(replace(replace(html, " =\"\"", ""), "&laquo;", "&#171;"), "&raquo;", "&#187;"),
        "<br>", "<br/>")
    parse_xml(xml) $(doc, ok) {
        if (!ok) {
            error = "Telegram HTML is incomplete or malformed"
            return
        }
        let root = doc as xml_node
        root |> for_each_select("//div[starts-with(@id,'message')]") $(selected) {
            let node = selected.node
            let message_id = parse_message_id(node.attribute("id").value, "message")
            let classes = " {strip(node.attribute("class").value)} "
            if (message_id <= 0l || find(classes, " service ") >= 0) {
                return
            }
            let explicit_sender = selected_node_text(
                node, ".//div[contains(concat(' ',normalize-space(@class),' '),' from_name ')][1]")
            if (!empty(explicit_sender)) {
                current_sender = explicit_sender
            }
            let timestamp = selected_attribute(
                node, ".//div[contains(concat(' ',normalize-space(@class),' '),' date ')][1]/@title")
            var message_date = 0l
            var date_error = ""
            if (!parse_telegram_export_timestamp(timestamp, message_date, date_error)) {
                // A bad timestamp is local to this message. Keep the page importable and let the
                // caller's InvalidDate accounting report MessageDate == 0.
                message_date = 0l
            }
            let reply_href = selected_attribute(
                node, ".//div[contains(concat(' ',normalize-space(@class),' '),' reply_to ')]//a[1]/@href")
            let media_path = selected_attribute(
                node, ".//a[contains(@href,'voice_messages/') or " +
                      "contains(@href,'round_video_messages/')][1]/@href")
            result |> emplace(TelegramExportMessage(
                MessageId = message_id,
                SenderName = current_sender,
                MessageDate = message_date,
                ReplyToMessageId = parse_message_id(trim_prefix(reply_href, "#go_to_"), "message"),
                Text = selected_node_text(
                    node, ".//div[contains(concat(' ',normalize-space(@class),' '),' text ')][1]"),
                ForwardedFrom = selected_node_text(
                    node, ".//div[contains(concat(' ',normalize-space(@class),' '),' forwarded ')][1]"),
                MediaPath = media_path,
                SourceKind = media_kind(media_path)))
        }
    }
    return <- result
}

def import_telegram_export_page(
                                db : SqlRunner;
                                root : string;
                                chat_id : int64;
                                messages : array<TelegramExportMessage>
                                ) : TelegramImportPageResult {
    var report = TelegramImportPageResult()
    db |> with_transaction(SqliteTxnMode.Immediate) {
        for (item in messages) {
            report.Messages++
            if (empty(item.SenderName)) {
                report.MissingSender++
            }
            if (item.MessageDate == 0l) {
                report.InvalidDate++
            }
            let forwarded = empty(item.ForwardedFrom) ? "" : "[Forwarded: {item.ForwardedFrom}]"
            upsert_cadmus_message_db(db, CadmusMessage(
                Id = 0,
                ChatId = chat_id,
                TelegramMessageId = item.MessageId,
                SenderId = telegram_export_sender_id(item.SenderName),
                SenderName = empty(item.SenderName) ? "Unknown export sender" : item.SenderName,
                SenderUsername = "",
                Role = CADMUS_ROLE_USER,
                SourceKind = item.SourceKind,
                Content = item.Text,
                Caption = forwarded,
                RawTranscript = "",
                MessageDate = item.MessageDate,
                EditedAt = 0l,
                ReplyToMessageId = item.ReplyToMessageId,
                Deleted = false))
            if (!empty(item.MediaPath)) {
                report.Media++
                let full_path = path_join(root, item.MediaPath)
                let info = stat(full_path)
                upsert_cadmus_import_media_db(db, CadmusImportMedia(
                    Id = 0,
                    ChatId = chat_id,
                    TelegramMessageId = item.MessageId,
                    RelativePath = to_generic_path(item.MediaPath),
                    FileSize = info.is_valid ? int64(info.size) : 0l,
                    ModifiedAt = info.is_valid ? unsafe(reinterpret<int64>(info.mtime)) : 0l,
                    Status = "pending",
                    Attempts = 0,
                    LastError = info.is_valid ? "" : "media file is missing",
                    PipelineVersion = 1))
            }
        }
    }
    return report
}

def private transcode_import_audio(cfg : TelegramRawAsrConfig; source, wav : string) : bool {
    let argv = [cfg.Ffmpeg, "-y", "-hide_banner", "-loglevel", "error",
                "-i", source, "-ar", "16000", "-ac", "1", wav]
    let rc = unsafe(popen_argv(argv, 300.0) $(file) {
        if (file != null) {
            fread(file)
        }
    })
    return rc == 0
}

def private request_raw_transcription(
                                      cfg : TelegramRawAsrConfig;
                                      wav : string;
                                      var transcript : string&;
                                      var error : string&
                                      ) : bool {
    var temp_error = ""
    let response_path = create_temp_file("cadmus_asr_", ".json", temp_error)
    if (empty(response_path)) {
        error = "could not create temporary ASR response: {temp_error}"
        return false
    }
    defer() {
        remove(response_path)
    }
    let argv = [cfg.Curl, "--silent", "--show-error", "--fail-with-body",
                "--connect-timeout", "10", "--max-time", "3600",
                "--output", response_path,
                "--form", "file=@{wav}",
                "--form", "language={cfg.Language}",
                "--form", "response_format=json",
                "{cfg.Server}/v1/audio/transcriptions"]
    let rc = unsafe(popen_argv(argv, 3660.0) $(file) {
        if (file != null) {
            fread(file)
        }
    })
    let response = fread(response_path)
    if (rc != 0) {
        error = (empty(strip(response)) ? "ASR request failed with curl exit code {rc}" :
                                          "ASR request failed: {response}")
        return false
    }
    var parse_error = ""
    var json = read_json(response, parse_error)
    if (json == null) {
        error = "bad ASR JSON: {parse_error}"
        return false
    }
    transcript = clone_string(json?["text"] ?? "")
    unsafe {
        delete json
    }
    if (empty(strip(transcript))) {
        error = "ASR returned an empty transcript"
        return false
    }
    return true
}

def raw_transcribe_cadmus_import_media(
                                       cfg : TelegramRawAsrConfig;
                                       root : string;
                                       item : CadmusImportMedia;
                                       var transcript : string&;
                                       var error : string&
                                       ) : bool {
    let relative = to_generic_path(item.RelativePath)
    if (is_absolute(item.RelativePath) || relative == ".." || starts_with(relative, "../")) {
        error = "unsafe media path: {item.RelativePath}"
        return false
    }
    let source = normalize(path_join(root, item.RelativePath))
    if (!stat(source).is_reg) {
        error = "media file not found: {source}"
        return false
    }
    var temp_error = ""
    let wav = create_temp_file("cadmus_import_", ".wav", temp_error)
    if (empty(wav)) {
        error = "could not create temporary WAV: {temp_error}"
        return false
    }
    defer() {
        remove(wav)
    }
    if (!transcode_import_audio(cfg, source, wav)) {
        error = "ffmpeg could not decode {item.RelativePath}"
        return false
    }
    return request_raw_transcription(cfg, wav, transcript, error)
}
