options gen2

require dastest/testing_boost public
require daslib/fio
require daslib/sql_linq
require daslib/linq_das
require sqlite/sqlite_migrate
require cadmus_history

def private test_db_path() : string {
    var error : string
    return path_join(temp_directory(error), "cadmus_history_test.db")
}

def private test_message(chat_id, message_id, sender_id, date : int64; text : string) : CadmusMessage {
    return CadmusMessage(
        Id = 0,
        ChatId = chat_id,
        TelegramMessageId = message_id,
        SenderId = sender_id,
        SenderName = "Tester",
        SenderUsername = "tester",
        Role = CADMUS_ROLE_USER,
        SourceKind = "text",
        Content = text,
        Caption = "",
        RawTranscript = "",
        MessageDate = date,
        EditedAt = 0l,
        ReplyToMessageId = 0l,
        Deleted = false)
}

[test]
def test_history_migrations_linq_and_fts(t : T?) {   // nolint:STYLE038 - flat list of 18 `t |> run` cases against one migrated database
    let path = test_db_path()
    remove(path)
    ensure_cadmus_database(path)

    t |> run("starts at migration eight with unindexed typed FTS keys") @(t : T?) {
        with_sqlite(path) $(db) {
            t |> equal(db |> current_schema_version(), 8)
            let ddl = db |> query_scalar(
                "SELECT sql FROM sqlite_master WHERE name = 'cadmus_message_fts'",
                type<string>)
            t |> success(find(ddl, "\"ChatId\" UNINDEXED") >= 0)
            t |> success(find(ddl, "\"TelegramMessageId\" UNINDEXED") >= 0)
        }
    }

    t |> run("Cadmus connections tolerate long-running import writers") @(t : T?) {
        with_cadmus_database(path) $(db) {
            t |> equal(db |> query_scalar("PRAGMA busy_timeout", type<int64>), 120000l)
        }
    }

    t |> run("upsert is idempotent and edits replace FTS text") @(t : T?) {
        upsert_cadmus_message(path, test_message(10l, 1l, 7l, 1000l, "alpha original"))
        upsert_cadmus_message(path, test_message(10l, 1l, 7l, 1000l, "beta edited"))
        let recent <- load_recent_cadmus_messages(path, 10l, 10)
        t |> equal(length(recent), 1)
        t |> equal(recent[0].Content, "beta edited")
        t |> equal(length(search_cadmus_messages(path, 10l, "alpha", 10)), 0)
        t |> equal(length(search_cadmus_messages(path, 10l, "beta", 10)), 1)
    }

    t |> run("audio caption edits preserve transcripts and refresh FTS") @(tt : T?) {
        var audio = test_message(10l, 3l, 7l, 1200l, "cleaned transcript")
        audio.SourceKind = "voice"
        audio.Caption = "original caption"
        upsert_cadmus_message(path, audio)
        update_cadmus_audio_edit(path, 10l, 3l, "replacement caption", 1300l)
        let recent <- load_recent_cadmus_messages(path, 10l, 10)
        var edited = -1
        for (i in range(length(recent))) {
            if (recent[i].TelegramMessageId == 3l) {
                edited = i
                break
            }
        }
        if (edited < 0) {
            tt |> failure("edited audio message was not loaded")
            return
        }
        tt |> equal(recent[edited].Content, "cleaned transcript")
        tt |> equal(recent[edited].Caption, "replacement caption")
        tt |> equal(recent[edited].EditedAt, 1300l)
        tt |> equal(length(search_cadmus_messages(path, 10l, "original", 10)), 0)
        tt |> equal(length(search_cadmus_messages(path, 10l, "replacement", 10)), 1)
    }

    t |> run("chat and sender filters stay local") @(t : T?) {
        upsert_cadmus_message(path, test_message(10l, 2l, 8l, 1100l, "first chat"))
        upsert_cadmus_message(path, test_message(20l, 1l, 7l, 1100l, "second chat"))
        let chat_rows <- load_cadmus_messages(path, 10l, 0l, 2000l, 100)
        let user_rows <- load_cadmus_messages_for_sender(path, 10l, 7l, 0l, 2000l, 100)
        t |> equal(length(chat_rows), 3)
        t |> equal(length(user_rows), 2)
        t |> equal(user_rows[0].ChatId, 10l)
    }

    t |> run("tool search returns only user messages from the requested chat") @(t : T?) {
        var assistant = test_message(10l, 20l, 100l, 1500l, "private assistant needle")
        assistant.Role = CADMUS_ROLE_ASSISTANT
        upsert_cadmus_message(path, assistant)
        t |> equal(is_cadmus_assistant_message(path, 10l, 20l), true)
        t |> equal(is_cadmus_assistant_message(path, 20l, 20l), false)
        upsert_cadmus_message(path, test_message(20l, 21l, 7l, 1501l, "other chat needle"))
        upsert_cadmus_message(path, test_message(10l, 22l, 8l, 1502l, "local user needle"))
        let rows <- search_cadmus_user_messages(path, 10l, "needle", 20)
        t |> equal(length(rows), 1)
        if (length(rows) == 1) {
            t |> equal(rows[0].TelegramMessageId, 22l)
            t |> equal(rows[0].Role, CADMUS_ROLE_USER)
            t |> equal(rows[0].ChatId, 10l)
        }
    }

    t |> run("transcript delivery ids persist and stay chat-local") @(t : T?) {
        remember_cadmus_transcript_delivery(path, 10l, 500l, 3l)
        t |> equal(is_cadmus_transcript_delivery(path, 10l, 500l), true)
        t |> equal(is_cadmus_transcript_delivery(path, 20l, 500l), false)
        t |> equal(is_cadmus_transcript_delivery(path, 10l, 501l), false)
        remember_cadmus_transcript_delivery(path, 10l, 500l, 4l)
        t |> equal(is_cadmus_transcript_delivery(path, 10l, 500l), true)
    }

    t |> run("assistant source provenance persists and stays chat-local") @(t : T?) {
        let sources <- load_cadmus_message(path, 10l, 22l)
        remember_cadmus_assistant_sources(path, 10l, 20l, sources)
        remember_cadmus_assistant_sources(path, 10l, 20l, sources)
        let loaded <- load_cadmus_assistant_sources(path, 10l, 20l)
        t |> equal(length(loaded), 1)
        if (length(loaded) == 1) {
            t |> equal(loaded[0].TelegramMessageId, 22l)
        }
        t |> equal(length(load_cadmus_assistant_sources(path, 20l, 20l)), 0)
    }

    t |> run("timeline context and links stay chat-local") @(t : T?) {
        let stats = cadmus_mention_stats(path, 10l, "needle")
        t |> equal(stats.Total, 2)
        t |> equal(stats.FirstDate, 1500l)
        t |> equal(stats.LastDate, 1502l)
        let context <- load_cadmus_message_context(path, 10l, 20l, 1, 1)
        t |> equal(length(context), 3)
        if (length(context) == 3) {
            t |> equal(context[1].TelegramMessageId, 20l)
        }
        t |> equal(cadmus_message_link(-1001234567890l, 42l), "https://t.me/c/1234567890/42")
        t |> equal(cadmus_message_link(-1001234567890l, 42l, "@public_chat"),
                   "https://t.me/public_chat/42")
        t |> equal(cadmus_message_link(123l, 42l), "")
        let chronological <- search_cadmus_messages_chronological(path, 10l, "needle", 10)
        t |> equal(length(chronological), 2)
        if (length(chronological) == 2) {
            t |> equal(chronological[0].TelegramMessageId, 20l)
            t |> equal(chronological[1].TelegramMessageId, 22l)
        }
        let sources <- [
            test_message(-1001234567890l, 42l, 7l, 1500l, "first"),
            test_message(-1001234567890l, 42l, 7l, 1500l, "duplicate")]
        let source_stamp = format_time(unsafe(reinterpret<clock>(1500l)), "%Y-%m-%d %H:%M")
        t |> equal(
            append_cadmus_sources("Answer.", sources),
            "Answer.\n\nChat sources:\n- [{source_stamp}] Tester: https://t.me/c/1234567890/42\n")
        t |> equal(
            append_cadmus_sources("Already linked: https://t.me/c/1234567890/42", sources),
            "Already linked: https://t.me/c/1234567890/42")
    }

    t |> run("timeline matching requires every query term") @(t : T?) {
        upsert_cadmus_message(path, test_message(30l, 1l, 7l, 100l, "alpha only"))
        upsert_cadmus_message(path, test_message(30l, 2l, 7l, 200l, "alpha and beta"))
        upsert_cadmus_message(path, test_message(30l, 3l, 7l, 300l, "beta only"))
        let stats = cadmus_mention_stats(path, 30l, "alpha beta")
        t |> equal(stats.Total, 1)
        t |> equal(stats.FirstDate, 200l)
        t |> equal(stats.LastDate, 200l)
        let chronological <- search_cadmus_messages_chronological(path, 30l, "alpha beta", 10)
        t |> equal(length(chronological), 1)
        if (length(chronological) == 1) {
            t |> equal(chronological[0].TelegramMessageId, 2l)
        }
    }

    t |> run("multilingual strict search preserves authorship and excludes current answers") @(t : T?) {
        var russian = test_message(40l, 1l, 11l, 100l, "шведский маузер и оптика")
        russian.SenderName = "Boris"
        russian.SenderUsername = "boris"
        upsert_cadmus_message(path, russian)
        var english = test_message(40l, 2l, 12l, 200l, "Swedish Mauser optics")
        english.SenderName = "Greg Cohen"
        english.SenderUsername = "greg"
        upsert_cadmus_message(path, english)
        var partial_name = test_message(40l, 3l, 12l, 300l, "Swedish rifle")
        partial_name.SenderName = "Greg Cohen"
        partial_name.SenderUsername = "greg"
        upsert_cadmus_message(path, partial_name)
        upsert_cadmus_message(path, test_message(40l, 4l, 13l, 400l, "Mauser optics"))
        var assistant = test_message(40l, 5l, 100l, 500l, "Swedish Mauser answer")
        assistant.Role = CADMUS_ROLE_ASSISTANT
        upsert_cadmus_message(path, assistant)
        upsert_cadmus_message(path, test_message(40l, 6l, 14l, 600l, "Swedish Mauser?"))

        let queries <- ["шведск маузер", "swedish mauser"]
        let rows <- search_cadmus_message_variants(path, 40l, queries, 0l, 6l, 10)
        t |> equal(length(rows), 2)
        if (length(rows) == 2) {
            t |> equal(rows[0].TelegramMessageId, 1l)
            t |> equal(rows[1].TelegramMessageId, 2l)
        }
        let greg_id = find_cadmus_sender(path, 40l, "Greg Cohen")
        t |> equal(greg_id, 12l)
        let greg_rows <- search_cadmus_message_variants(path, 40l, queries, greg_id, 6l, 10)
        t |> equal(length(greg_rows), 1)
        if (length(greg_rows) == 1) {
            t |> equal(greg_rows[0].TelegramMessageId, 2l)
        }
    }

    t |> run("strict prefix variants require every topic term") @(t : T?) {
        upsert_cadmus_message(path, test_message(41l, 1l, 7l, 100l, "alpha only"))
        upsert_cadmus_message(path, test_message(41l, 2l, 8l, 200l, "beta only"))
        upsert_cadmus_message(path, test_message(41l, 3l, 9l, 300l, "обсуждали оптику"))
        let mixed <- search_cadmus_message_variants(path, 41l, ["alpha beta"], 0l, 0l, 10)
        t |> equal(length(mixed), 0)
        let inflected <- search_cadmus_message_variants(path, 41l, ["оптик"], 0l, 0l, 10)
        t |> equal(length(inflected), 1)
        if (length(inflected) == 1) {
            t |> equal(inflected[0].TelegramMessageId, 3l)
        }
    }

    t |> run("import media checkpoint preserves unchanged completed work") @(t : T?) {
        with_cadmus_database(path) $(db) {
            db |> with_transaction(SqliteTxnMode.Immediate) {
                upsert_cadmus_import_media_db(db, CadmusImportMedia(
                    Id = 0, ChatId = 10l, TelegramMessageId = 700l,
                    RelativePath = "voice_messages/audio.ogg", FileSize = 100l,
                    ModifiedAt = 200l, Status = "done", Attempts = 1,
                    LastError = "", PipelineVersion = 1))
                upsert_cadmus_import_media_db(db, CadmusImportMedia(
                    Id = 0, ChatId = 10l, TelegramMessageId = 700l,
                    RelativePath = "voice_messages/audio.ogg", FileSize = 100l,
                    ModifiedAt = 200l, Status = "pending", Attempts = 0,
                    LastError = "", PipelineVersion = 1))
            }
            let rows <- %linq! from media : CadmusImportMedia in db
                               where media.ChatId == 10l && media.TelegramMessageId == 700l
                               select media %%
            t |> equal(length(rows), 1)
            if (length(rows) == 1) {
                t |> equal(rows[0].Status, "done")
                t |> equal(rows[0].Attempts, 1)
            }
        }
    }

    t |> run("idle import work spans chats and requeues prior failures once") @(t : T?) {
        with_cadmus_database(path) $(db) {
            upsert_cadmus_import_media_db(db, CadmusImportMedia(
                Id = 0, ChatId = 10l, TelegramMessageId = 710l,
                RelativePath = "voice_messages/failed.ogg", FileSize = 50l,
                ModifiedAt = 200l, Status = "failed", Attempts = 1,
                LastError = "server unavailable", PipelineVersion = 1))
            upsert_cadmus_import_media_db(db, CadmusImportMedia(
                Id = 0, ChatId = 20l, TelegramMessageId = 711l,
                RelativePath = "voice_messages/pending.ogg", FileSize = 100l,
                ModifiedAt = 200l, Status = "pending", Attempts = 0,
                LastError = "", PipelineVersion = 1))
        }
        let pending <- load_next_cadmus_import_media(path)
        t |> equal(length(pending), 1)
        if (length(pending) == 1) {
            t |> equal(pending[0].TelegramMessageId, 711l)
        }
        requeue_failed_cadmus_import_media(path, IMPORT_MAX_ATTEMPTS)
        let retried <- load_next_cadmus_import_media(path)
        t |> equal(length(retried), 1)
        if (length(retried) == 1) {
            t |> equal(retried[0].TelegramMessageId, 710l)
        }
    }

    t |> run("import rows that exhausted their attempts are never requeued again") @(t : T?) {
        with_cadmus_database(path) $(db) {
            upsert_cadmus_import_media_db(db, CadmusImportMedia(
                Id = 0, ChatId = 30l, TelegramMessageId = 720l,
                RelativePath = "voice_messages/too_long.ogg", FileSize = 50l,
                ModifiedAt = 200l, Status = "failed", Attempts = IMPORT_MAX_ATTEMPTS,
                LastError = "clip audio to under ~10.9 minutes", PipelineVersion = 1))
        }
        t |> equal(count_exhausted_cadmus_import_media(path, IMPORT_MAX_ATTEMPTS), 1)
        requeue_failed_cadmus_import_media(path, IMPORT_MAX_ATTEMPTS)
        with_cadmus_database(path) $(db) {
            let rows <- %linq! from media : CadmusImportMedia in db
                               where media.ChatId == 30l && media.TelegramMessageId == 720l
                               select media %%
            t |> equal(length(rows), 1)
            if (length(rows) == 1) {
                // Still failed: a requeue must not resurrect a row that spent its budget.
                t |> equal(rows[0].Status, "failed")
                t |> equal(rows[0].Attempts, IMPORT_MAX_ATTEMPTS)
            }
        }
    }

    t |> run("empty username never resolves to an anonymous sender") @(t : T?) {
        var anonymous = test_message(10l, 4l, 99l, 1400l, "anonymous message")
        anonymous.SenderUsername = ""
        upsert_cadmus_message(path, anonymous)
        t |> equal(find_cadmus_sender(path, 10l, "@"), 0l)
        t |> equal(find_cadmus_sender(path, 10l, " @ "), 0l)
    }

    t |> run("Telegram offset persists") @(t : T?) {
        save_cadmus_offset(path, 42l)
        t |> equal(load_cadmus_offset(path), 42l)
    }

    t |> run("live audio queue claims, retries, and recovers durably") @(t : T?) {
        enqueue_cadmus_live_audio(path, CadmusLiveAudio(
            Id = 0, ChatId = 10l, TelegramMessageId = 799l,
            MessageJson = "\{\"message_id\":799}", Status = "pending", Attempts = 0,
            LastError = "", EnqueuedAt = 90l, UpdatedAt = 90l, NextAttemptAt = 0l))
        enqueue_cadmus_live_audio(path, CadmusLiveAudio(
            Id = 0, ChatId = 10l, TelegramMessageId = 800l,
            MessageJson = "\{\"message_id\":800}", Status = "pending", Attempts = 0,
            LastError = "", EnqueuedAt = 100l, UpdatedAt = 100l, NextAttemptAt = 0l))
        enqueue_cadmus_live_audio(path, CadmusLiveAudio(
            Id = 0, ChatId = 10l, TelegramMessageId = 800l,
            MessageJson = "\{\"message_id\":800,\"caption\":\"new\"}", Status = "pending", Attempts = 0,
            LastError = "", EnqueuedAt = 101l, UpdatedAt = 101l, NextAttemptAt = 0l))
        t |> success(has_ready_cadmus_live_audio(path, 1000l))
        let first <- claim_cadmus_live_audio(path, 1000l)
        t |> equal(length(first), 1)
        if (length(first) == 1) {
            t |> equal(first[0].TelegramMessageId, 799l)
            t |> equal(first[0].Attempts, 1)
            t |> equal(first[0].Status, "processing")
            checkpoint_cadmus_live_transcript(path, first[0], "raw words", "English", 1900l)
            retry_cadmus_live_audio(path, first[0], "server down", 2000l, 2600l)
        }
        let older <- claim_cadmus_live_audio(path, 2001l)
        t |> equal(length(older), 1)
        if (length(older) == 1) {
            t |> equal(older[0].MessageJson, "\{\"message_id\":800,\"caption\":\"new\"}")
            finish_cadmus_live_audio(path, older[0], 2002l)
        }
        t |> success(!has_ready_cadmus_live_audio(path, 2599l))
        t |> success(has_ready_cadmus_live_audio(path, 2600l))
        let second <- claim_cadmus_live_audio(path, 2600l)
        t |> equal(length(second), 1)
        if (length(second) == 1) {
            t |> equal(second[0].Attempts, 2)
            t |> equal(second[0].RawTranscript, "raw words")
            t |> equal(second[0].TranscriptLanguage, "English")
            finish_cadmus_live_audio(path, second[0], 2700l)
        }
        t |> success(!has_ready_cadmus_live_audio(path, 9999l))

        enqueue_cadmus_live_audio(path, CadmusLiveAudio(
            Id = 0, ChatId = 10l, TelegramMessageId = 801l,
            MessageJson = "\{\"message_id\":801}", Status = "pending", Attempts = 0,
            LastError = "", EnqueuedAt = 200l, UpdatedAt = 200l, NextAttemptAt = 0l))
        let interrupted <- claim_cadmus_live_audio(path, 3000l)
        t |> equal(length(interrupted), 1)
        recover_cadmus_live_audio(path, 3100l)
        t |> success(has_ready_cadmus_live_audio(path, 3100l))
    }

    remove(path)
}

[test]
def test_history_v8_rebuild_preserves_existing_search_rows(t : T?) {
    var error : string
    let path = path_join(temp_directory(error), "cadmus_history_v7_upgrade_test.db")
    remove(path)
    with_sqlite(path) $(db) {
        cadmus_migration_001(db)
        cadmus_migration_002(db)
        cadmus_migration_003(db)
        cadmus_migration_004(db)
        cadmus_migration_005(db)
        cadmus_migration_006(db)
        cadmus_migration_007(db)
        db |> baseline(7)
        db |> insert(test_message(77l, 1l, 5l, 100l, "legacy migration needle"))
        db |> exec(
            "INSERT INTO cadmus_message_fts(ChatId, TelegramMessageId, Body) VALUES (?, ?, ?)",
            "77", "1", "legacy migration needle")
    }

    ensure_cadmus_database(path)
    let rows <- search_cadmus_messages(path, 77l, "migration needle", 10)
    t |> equal(length(rows), 1)
    if (length(rows) == 1) {
        t |> equal(rows[0].TelegramMessageId, 1l)
    }
    with_sqlite(path) $(db) {
        t |> equal(db |> current_schema_version(), 8)
        t |> equal(
            db |> query_scalar(
                "SELECT typeof(ChatId) FROM cadmus_message_fts WHERE TelegramMessageId = 1",
                type<string>),
            "integer")
    }
    remove(path)
}
