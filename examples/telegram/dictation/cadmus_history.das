options gen2

module cadmus_history public

require sqlite/sqlite_migrate
require daslib/sql_linq
require daslib/linq_das
require daslib/strings_boost
require math

let CADMUS_ROLE_USER = "user"
let CADMUS_ROLE_ASSISTANT = "assistant"

// Import ASR retries across bot restarts; past this many failures a row is left alone.
let IMPORT_MAX_ATTEMPTS = 5

[sql_table(name = "cadmus_messages"),
 sql_index(fields = ("ChatId", "TelegramMessageId"), unique = true),
 sql_index(fields = ("ChatId", "MessageDate")),
 sql_index(fields = ("ChatId", "SenderId", "MessageDate"))]
struct CadmusMessage {
    @sql_primary_key Id : int
    ChatId : int64
    TelegramMessageId : int64
    SenderId : int64
    SenderName : string
    SenderUsername : string
    Role : string
    SourceKind : string
    Content : string
    Caption : string
    RawTranscript : string
    MessageDate : int64
    EditedAt : int64
    ReplyToMessageId : int64
    Deleted : bool
}

[sql_table(name = "cadmus_state")]
struct CadmusState {
    @sql_primary_key Id : int
    UpdateOffset : int64
}

[sql_table(name = "cadmus_transcript_deliveries"),
 sql_index(fields = ("ChatId", "TelegramMessageId"), unique = true)]
struct CadmusTranscriptDelivery {
    @sql_primary_key Id : int
    ChatId : int64
    TelegramMessageId : int64
    SourceMessageId : int64
}

[sql_table(name = "cadmus_assistant_sources"),
 sql_index(fields = ("ChatId", "AssistantMessageId", "SourceMessageId"), unique = true),
 sql_index(fields = ("ChatId", "AssistantMessageId"))]
struct CadmusAssistantSource {
    @sql_primary_key Id : int
    ChatId : int64
    AssistantMessageId : int64
    SourceMessageId : int64
}

[sql_table(name = "cadmus_import_media"),
 sql_index(fields = ("ChatId", "TelegramMessageId"), unique = true),
 sql_index(fields = ("Status", "ChatId", "TelegramMessageId"))]
struct CadmusImportMedia {
    @sql_primary_key Id : int
    ChatId : int64
    TelegramMessageId : int64
    RelativePath : string
    FileSize : int64
    ModifiedAt : int64
    Status : string
    Attempts : int
    LastError : string
    PipelineVersion : int
}

[sql_table(name = "cadmus_live_audio"),
 sql_index(fields = ("ChatId", "TelegramMessageId"), unique = true),
 sql_index(fields = ("Status", "NextAttemptAt", "EnqueuedAt"))]
struct CadmusLiveAudio {
    @sql_primary_key Id : int
    ChatId : int64
    TelegramMessageId : int64
    MessageJson : string
    RawTranscript : string
    TranscriptLanguage : string
    Status : string
    Attempts : int
    LastError : string
    EnqueuedAt : int64
    UpdatedAt : int64
    NextAttemptAt : int64
}

[sql_fts5(name = "cadmus_message_fts")]
struct CadmusSearchRow {
    @sql_fts_unindexed ChatId : int64
    @sql_fts_unindexed TelegramMessageId : int64
    Body : string
    @sql_fts_rank Rank : float
}

struct CadmusMentionStats {
    Total : int
    FirstDate : int64
    LastDate : int64
}

// Frozen v1 snapshot: durable messages + Telegram poll state.
[sql_migration(version = 1, description = "create Cadmus conversation history")]
def cadmus_migration_001(db : SqlRunner) {
    db |> exec(
        "CREATE TABLE cadmus_messages (" +
        "Id INTEGER PRIMARY KEY AUTOINCREMENT," +
        "ChatId INTEGER NOT NULL," +
        "TelegramMessageId INTEGER NOT NULL," +
        "SenderId INTEGER NOT NULL," +
        "SenderName TEXT NOT NULL," +
        "SenderUsername TEXT NOT NULL," +
        "Role TEXT NOT NULL," +
        "SourceKind TEXT NOT NULL," +
        "Content TEXT NOT NULL," +
        "Caption TEXT NOT NULL," +
        "RawTranscript TEXT NOT NULL," +
        "MessageDate INTEGER NOT NULL," +
        "EditedAt INTEGER NOT NULL," +
        "ReplyToMessageId INTEGER NOT NULL," +
        "Deleted INTEGER NOT NULL," +
        "UNIQUE(ChatId, TelegramMessageId))")
    db |> exec("CREATE INDEX idx_cadmus_messages_chat_date ON cadmus_messages(ChatId, MessageDate)")
    db |> exec("CREATE INDEX idx_cadmus_messages_chat_sender_date ON cadmus_messages(ChatId, SenderId, MessageDate)")
    db |> exec("CREATE TABLE cadmus_state (Id INTEGER PRIMARY KEY, UpdateOffset INTEGER NOT NULL)")
    db |> exec("INSERT INTO cadmus_state (Id, UpdateOffset) VALUES (1, 0)")
}

// Frozen v2 snapshot: self-contained FTS5 index. FTS rows mirror live message text.
[sql_migration(version = 2, description = "add Cadmus full-text history index")]
def cadmus_migration_002(db : SqlRunner) {
    db |> exec(
        "CREATE VIRTUAL TABLE cadmus_message_fts USING fts5(" +
        "ChatId, TelegramMessageId, Body, tokenize='unicode61')")
}

// Outgoing transcript replies look like ordinary bot answers when somebody replies to them:
// Telegram deliberately omits nested reply metadata. Persist their message IDs so routing can
// distinguish transcript attribution from a question directed at Cadmus.
[sql_migration(version = 3, description = "track outgoing transcript delivery messages")]
def cadmus_migration_003(db : SqlRunner) {
    db |> exec(
        "CREATE TABLE cadmus_transcript_deliveries (" +
        "Id INTEGER PRIMARY KEY AUTOINCREMENT," +
        "ChatId INTEGER NOT NULL," +
        "TelegramMessageId INTEGER NOT NULL," +
        "SourceMessageId INTEGER NOT NULL," +
        "UNIQUE(ChatId, TelegramMessageId))")
}

// Desktop-export imports are intentionally resumable: text upserts are idempotent and media keeps
// a durable pending/done/failed checkpoint. Rebuild FTS once so older raw transcripts become
// searchable too; subsequent upserts maintain the same combined body.
[sql_migration(version = 4, description = "add resumable Telegram export media queue")]
def cadmus_migration_004(db : SqlRunner) {
    db |> exec(
        "CREATE TABLE cadmus_import_media (" +
        "Id INTEGER PRIMARY KEY AUTOINCREMENT," +
        "ChatId INTEGER NOT NULL," +
        "TelegramMessageId INTEGER NOT NULL," +
        "RelativePath TEXT NOT NULL," +
        "FileSize INTEGER NOT NULL," +
        "ModifiedAt INTEGER NOT NULL," +
        "Status TEXT NOT NULL," +
        "Attempts INTEGER NOT NULL," +
        "LastError TEXT NOT NULL," +
        "PipelineVersion INTEGER NOT NULL," +
        "UNIQUE(ChatId, TelegramMessageId))")
    db |> exec(
        "CREATE INDEX idx_cadmus_import_media_status " +
        "ON cadmus_import_media(Status, ChatId, TelegramMessageId)")
    db |> exec("DELETE FROM cadmus_message_fts")
    db |> exec(
        "INSERT INTO cadmus_message_fts(ChatId, TelegramMessageId, Body) " +
        "SELECT CAST(ChatId AS TEXT), CAST(TelegramMessageId AS TEXT), " +
        "trim(Content || char(10) || Caption || char(10) || RawTranscript) " +
        "FROM cadmus_messages WHERE Deleted = 0 AND " +
        "length(trim(Content || Caption || RawTranscript)) > 0")
}

// Grounded answers retain their database evidence independently of rendered citations. Follow-up
// questions can therefore retrieve the same anchors and expand them into surrounding conversation.
[sql_migration(version = 5, description = "track Cadmus answer source messages")]
def cadmus_migration_005(db : SqlRunner) {
    db |> exec(
        "CREATE TABLE cadmus_assistant_sources (" +
        "Id INTEGER PRIMARY KEY AUTOINCREMENT," +
        "ChatId INTEGER NOT NULL," +
        "AssistantMessageId INTEGER NOT NULL," +
        "SourceMessageId INTEGER NOT NULL," +
        "UNIQUE(ChatId, AssistantMessageId, SourceMessageId))")
    db |> exec(
        "CREATE INDEX idx_cadmus_assistant_sources_answer " +
        "ON cadmus_assistant_sources(ChatId, AssistantMessageId)")
}

// Live Telegram audio is acknowledged only after this durable record exists. The serialized
// message retains the file_id and reply/sender metadata needed to finish after a restart.
[sql_migration(version = 6, description = "add durable live Telegram audio queue")]
def cadmus_migration_006(db : SqlRunner) {
    db |> exec(
        "CREATE TABLE cadmus_live_audio (" +
        "Id INTEGER PRIMARY KEY AUTOINCREMENT," +
        "ChatId INTEGER NOT NULL," +
        "TelegramMessageId INTEGER NOT NULL," +
        "MessageJson TEXT NOT NULL," +
        "Status TEXT NOT NULL," +
        "Attempts INTEGER NOT NULL," +
        "LastError TEXT NOT NULL," +
        "EnqueuedAt INTEGER NOT NULL," +
        "UpdatedAt INTEGER NOT NULL," +
        "NextAttemptAt INTEGER NOT NULL," +
        "UNIQUE(ChatId, TelegramMessageId))")
    db |> exec(
        "CREATE INDEX idx_cadmus_live_audio_ready " +
        "ON cadmus_live_audio(Status, NextAttemptAt, EnqueuedAt)")
}

// ASR is the expensive half of a live job. Preserve a successful raw transcript so a cleanup,
// Telegram delivery, or bot restart failure never sends the same audio through ASR again.
[sql_migration(version = 7, description = "checkpoint live Telegram ASR results")]
def cadmus_migration_007(db : SqlRunner) {
    db |> exec("ALTER TABLE cadmus_live_audio ADD COLUMN RawTranscript TEXT NOT NULL DEFAULT ''")
    db |> exec("ALTER TABLE cadmus_live_audio ADD COLUMN TranscriptLanguage TEXT NOT NULL DEFAULT ''")
}

// FTS keys are relational metadata, not searchable prose. Rebuild once so new typed queries bind
// int64 directly and FTS5 stops tokenizing chat/message IDs into its inverted index.
[sql_migration(version = 8, description = "store Cadmus FTS keys as typed unindexed metadata")]
def cadmus_migration_008(db : SqlRunner) {
    db |> exec("DROP TABLE cadmus_message_fts")
    db |> exec(
        "CREATE VIRTUAL TABLE cadmus_message_fts USING fts5(" +
        "\"ChatId\" UNINDEXED, \"TelegramMessageId\" UNINDEXED, \"Body\", " +
        "tokenize='unicode61')")
    db |> exec(
        "INSERT INTO cadmus_message_fts(ChatId, TelegramMessageId, Body) " +
        "SELECT ChatId, TelegramMessageId, " +
        "trim(Content || char(10) || Caption || char(10) || RawTranscript) " +
        "FROM cadmus_messages WHERE Deleted = 0 AND " +
        "length(trim(Content || Caption || RawTranscript)) > 0")
}

def with_cadmus_database(path : string; blk : block<(db : SqlRunner) : void>) {
    with_latest_sqlite(path) $(db) {
        db |> set_pragma("busy_timeout", 120000l)
        invoke(blk, db)
    }
}

def ensure_cadmus_database(path : string) {
    with_cadmus_database(path) $(db) {
        db |> check_schema(type<CadmusMessage>)
        db |> check_schema(type<CadmusState>)
        db |> check_schema(type<CadmusTranscriptDelivery>)
        db |> check_schema(type<CadmusAssistantSource>)
        db |> check_schema(type<CadmusImportMedia>)
        db |> check_schema(type<CadmusLiveAudio>)
    }
}

def remember_cadmus_transcript_delivery(
                                        path : string;
                                        chat_id, message_id, source_message_id : int64
                                        ) {
    with_cadmus_database(path) $(db) {
        let existing <- %linq! from d : CadmusTranscriptDelivery in db
                               where d.ChatId == chat_id && d.TelegramMessageId == message_id
                               select d.Id %%
        if (empty(existing)) {
            db |> insert(CadmusTranscriptDelivery(
                Id = 0,
                ChatId = chat_id,
                TelegramMessageId = message_id,
                SourceMessageId = source_message_id))
        } else {
            db |> _sql_update(
                type<CadmusTranscriptDelivery>,
                _.Id == existing[0],
                (SourceMessageId = source_message_id))
        }
    }
}

def is_cadmus_transcript_delivery(path : string; chat_id, message_id : int64) : bool {
    var found = false
    with_cadmus_database(path) $(db) {
        let rows <- %linq! from d : CadmusTranscriptDelivery in db
                           where d.ChatId == chat_id && d.TelegramMessageId == message_id
                           select d.Id %%
        found = !empty(rows)
    }
    return found
}

def is_cadmus_assistant_message(path : string; chat_id, message_id : int64) : bool {
    var found = false
    with_cadmus_database(path) $(db) {
        let rows <- %linq! from m : CadmusMessage in db
                           where m.ChatId == chat_id && m.TelegramMessageId == message_id &&
                                 m.Role == CADMUS_ROLE_ASSISTANT && !m.Deleted
                           select m.Id %%
        found = !empty(rows)
    }
    return found
}

def remember_cadmus_assistant_sources(
                                      path : string;
                                      chat_id, assistant_message_id : int64;
                                      sources : array<CadmusMessage>
                                      ) {
    if (assistant_message_id == 0l || empty(sources)) {
        return
    }
    with_cadmus_database(path) $(db) {
        db |> with_transaction(SqliteTxnMode.Immediate) {
            for (source in sources) {
                if (source.ChatId != chat_id || source.TelegramMessageId == 0l) {
                    continue
                }
                let existing <- %linq! from item : CadmusAssistantSource in db
                                       where item.ChatId == chat_id &&
                                             item.AssistantMessageId == assistant_message_id &&
                                             item.SourceMessageId == source.TelegramMessageId
                                       select item.Id %%
                if (empty(existing)) {
                    db |> insert(CadmusAssistantSource(
                        Id = 0,
                        ChatId = chat_id,
                        AssistantMessageId = assistant_message_id,
                        SourceMessageId = source.TelegramMessageId))
                }
            }
        }
    }
}

def load_cadmus_assistant_sources(
                                  path : string;
                                  chat_id, assistant_message_id : int64
                                  ) : array<CadmusMessage> {
    var result : array<CadmusMessage>
    if (assistant_message_id == 0l) {
        return <- result
    }
    var source_ids : array<int64>
    with_cadmus_database(path) $(db) {
        source_ids <- %linq! from item : CadmusAssistantSource in db   // nolint:PERF030 - target is the fresh empty decl above; the fill is inside a callback, so it cannot fold into the declaration
                              where item.ChatId == chat_id &&
                                    item.AssistantMessageId == assistant_message_id
                              select item.SourceMessageId %%
    }
    for (source_id in source_ids) {
        let rows <- load_cadmus_message(path, chat_id, source_id)
        if (!empty(rows)) {
            result |> push(rows[0])
        }
    }
    delete source_ids
    return <- result
}

def private sync_cadmus_search(db : SqlRunner; msg : CadmusMessage) {
    db |> _sql_delete(
        type<CadmusSearchRow>,
        _.ChatId == msg.ChatId && _.TelegramMessageId == msg.TelegramMessageId)
    if (!msg.Deleted && !empty(strip("{msg.Content}{msg.Caption}{msg.RawTranscript}"))) {
        let body = build_string() $(var writer) {
            if (!empty(strip(msg.Content))) {
                writer |> write(msg.Content)
            }
            if (!empty(strip(msg.Caption))) {
                writer |> write("\n")
                writer |> write(msg.Caption)
            }
            if (!empty(strip(msg.RawTranscript)) && msg.RawTranscript != msg.Content) {
                writer |> write("\n")
                writer |> write(msg.RawTranscript)
            }
        }
        db |> insert(CadmusSearchRow(
            ChatId = msg.ChatId,
            TelegramMessageId = msg.TelegramMessageId,
            Body = body,
            Rank = 0.0))
    }
}

def upsert_cadmus_message_db(db : SqlRunner; msg : CadmusMessage) {
    let existing <- %linq! from m : CadmusMessage in db
                           where m.ChatId == msg.ChatId && m.TelegramMessageId == msg.TelegramMessageId
                           select m.Id %%
    if (empty(existing)) {
        db |> insert(msg)
    } else {
        db |> _sql_update(
            type<CadmusMessage>,
            _.Id == existing[0],
            (SenderId = msg.SenderId,
             SenderName = msg.SenderName,
             SenderUsername = msg.SenderUsername,
             Role = msg.Role,
             SourceKind = msg.SourceKind,
             Content = msg.Content,
             Caption = msg.Caption,
             RawTranscript = msg.RawTranscript,
             MessageDate = msg.MessageDate,
             EditedAt = msg.EditedAt,
             ReplyToMessageId = msg.ReplyToMessageId,
             Deleted = msg.Deleted))
    }
    sync_cadmus_search(db, msg)
}

def upsert_cadmus_message(path : string; msg : CadmusMessage) {
    with_cadmus_database(path) $(db) {
        db |> with_transaction(SqliteTxnMode.Immediate) {
            upsert_cadmus_message_db(db, msg)
        }
    }
}

def upsert_cadmus_import_media_db(db : SqlRunner; item : CadmusImportMedia) {
    let completed <- %linq! from old : CadmusImportMedia in db
                            where old.ChatId == item.ChatId &&
                                  old.TelegramMessageId == item.TelegramMessageId &&
                                  old.Status == "done" && old.RelativePath == item.RelativePath &&
                                  old.FileSize == item.FileSize && old.ModifiedAt == item.ModifiedAt &&
                                  old.PipelineVersion == item.PipelineVersion
                            select old.Id %%
    if (!empty(completed)) {
        return
    }
    var upsert_item = item
    upsert_item.Id = 0
    db |> _sql_upsert(
        upsert_item,
        tuple(_.ChatId, _.TelegramMessageId),
        (RelativePath = _excluded.RelativePath,
         FileSize = _excluded.FileSize,
         ModifiedAt = _excluded.ModifiedAt,
         Status = _excluded.Status,
         Attempts = _excluded.Attempts,
         LastError = _excluded.LastError,
         PipelineVersion = _excluded.PipelineVersion))
}

def load_cadmus_import_media(
                             path : string;
                             chat_id : int64;
                             include_failed : bool;
                             count : int
                             ) : array<CadmusImportMedia> {
    var result : array<CadmusImportMedia>
    with_cadmus_database(path) $(db) {
        result <- _sql(
            db |> select_from(type<CadmusImportMedia>)
               |> _where(
                    _.ChatId == chat_id &&
                    (_.Status == "pending" || (include_failed && _.Status == "failed")))
               |> _order_by((_.FileSize, _.TelegramMessageId))
               |> take(count <= 0 ? 1000000 : count))
    }
    return <- result
}

// The bot uses imported media only as idle work, across every chat represented in the export.
// Failed rows are explicitly requeued once at startup, so a permanent failure cannot spin here.
def load_next_cadmus_import_media(path : string) : array<CadmusImportMedia> {
    var result : array<CadmusImportMedia>
    with_cadmus_database(path) $(db) {
        result <- _sql(
            db |> select_from(type<CadmusImportMedia>)
               |> _where(_.Status == "pending")
               |> _order_by((_.FileSize, _.TelegramMessageId, _.Id))
               |> take(1))
    }
    return <- result
}

def has_pending_cadmus_import_media(path : string) : bool {
    var ready = false
    with_cadmus_database(path) $(db) {
        let rows <- _sql(
            db |> select_from(type<CadmusImportMedia>)
               |> _where(_.Status == "pending")
               |> take(1))
        ready = !empty(rows)
    }
    return ready
}

// Requeue only rows that still have attempts left. Without the bound a row that can never succeed
// (audio the ASR model refuses, a corrupt file) is retried once per bot start, forever.
def requeue_failed_cadmus_import_media(path : string; max_attempts : int) {
    with_cadmus_database(path) $(db) {
        db |> _sql_update(
            type<CadmusImportMedia>,
            _.Status == "failed" && _.Attempts < max_attempts,
            (Status = "pending", LastError = ""))
    }
}

def count_exhausted_cadmus_import_media(path : string; max_attempts : int) : int {
    var total = 0
    with_cadmus_database(path) $(db) {
        total = _sql(
            db |> select_from(type<CadmusImportMedia>)
               |> _where(_.Status == "failed" && _.Attempts >= max_attempts)
               |> count())
    }
    return total
}

def finish_cadmus_import_transcription(
                                       path : string;
                                       item : CadmusImportMedia;
                                       transcript : string
                                       ) {
    with_cadmus_database(path) $(db) {
        db |> with_transaction(SqliteTxnMode.Immediate) {
            let messages <- %linq! from m : CadmusMessage in db
                                   where m.ChatId == item.ChatId &&
                                         m.TelegramMessageId == item.TelegramMessageId
                                   select m %%
            if (!empty(messages)) {
                var updated = messages[0]
                updated.Content = transcript
                updated.RawTranscript = transcript
                upsert_cadmus_message_db(db, updated)
            }
            db |> _sql_update(
                type<CadmusImportMedia>,
                _.ChatId == item.ChatId && _.TelegramMessageId == item.TelegramMessageId,
                (Status = "done", Attempts = item.Attempts + 1, LastError = ""))
        }
    }
}

def fail_cadmus_import_transcription(
                                     path : string;
                                     item : CadmusImportMedia;
                                     error : string;
                                     attempts : int
                                     ) {
    with_cadmus_database(path) $(db) {
        db |> _sql_update(
            type<CadmusImportMedia>,
            _.ChatId == item.ChatId && _.TelegramMessageId == item.TelegramMessageId,
            (Status = "failed", Attempts = attempts, LastError = error))
    }
}

def enqueue_cadmus_live_audio(path : string; item : CadmusLiveAudio) {
    with_cadmus_database(path) $(db) {
        db |> with_transaction(SqliteTxnMode.Immediate) {
            let rows <- %linq! from old : CadmusLiveAudio in db
                               where old.ChatId == item.ChatId &&
                                     old.TelegramMessageId == item.TelegramMessageId
                               select old %%
            if (empty(rows)) {
                db |> insert(item)
            } elif (rows[0].Status != "done") {
                db |> _sql_update(
                    type<CadmusLiveAudio>,
                    _.Id == rows[0].Id,
                    (MessageJson = item.MessageJson,
                     Status = "pending",
                     LastError = "",
                     UpdatedAt = item.UpdatedAt,
                     NextAttemptAt = 0l))
            }
        }
    }
}

def has_ready_cadmus_live_audio(path : string; now : int64) : bool {
    var ready = false
    with_cadmus_database(path) $(db) {
        let rows <- _sql(
            db |> select_from(type<CadmusLiveAudio>)
               |> _where(
                    _.Status == "pending" ||
                    (_.Status == "failed" && _.NextAttemptAt <= now))
               |> take(1))
        ready = !empty(rows)
    }
    return ready
}

def claim_cadmus_live_audio(path : string; now : int64) : array<CadmusLiveAudio> {
    var result : array<CadmusLiveAudio>
    with_cadmus_database(path) $(db) {
        db |> with_transaction(SqliteTxnMode.Immediate) {
            result <- _sql(
                db |> select_from(type<CadmusLiveAudio>)
                   |> _where(
                        _.Status == "pending" ||
                        (_.Status == "failed" && _.NextAttemptAt <= now))
                   // Preserve conversation order: finish the earliest ready voice note first.
                   |> _order_by((_.EnqueuedAt, _.Id))
                   |> take(1))
            if (!empty(result)) {
                result[0].Status = "processing"
                result[0].Attempts++
                result[0].UpdatedAt = now
                let claimed_id = result[0].Id
                let claimed_status = result[0].Status
                let claimed_attempts = result[0].Attempts
                db |> _sql_update(
                    type<CadmusLiveAudio>,
                    _.Id == claimed_id,
                    (Status = claimed_status,
                     Attempts = claimed_attempts,
                     UpdatedAt = now))
            }
        }
    }
    return <- result
}

def retry_cadmus_live_audio(
                            path : string;
                            item : CadmusLiveAudio;
                            error : string;
                            now, next_attempt_at : int64
                            ) {
    with_cadmus_database(path) $(db) {
        db |> _sql_update(
            type<CadmusLiveAudio>,
            _.Id == item.Id,
            (Status = "failed", LastError = error,
             UpdatedAt = now, NextAttemptAt = next_attempt_at))
    }
}

def checkpoint_cadmus_live_transcript(
                                      path : string;
                                      item : CadmusLiveAudio;
                                      transcript, language : string;
                                      now : int64
                                      ) {
    with_cadmus_database(path) $(db) {
        db |> _sql_update(
            type<CadmusLiveAudio>,
            _.Id == item.Id,
            (RawTranscript = transcript, TranscriptLanguage = language, UpdatedAt = now))
    }
}

def finish_cadmus_live_audio(path : string; item : CadmusLiveAudio; now : int64) {
    with_cadmus_database(path) $(db) {
        db |> _sql_update(
            type<CadmusLiveAudio>,
            _.Id == item.Id,
            (Status = "done", LastError = "", UpdatedAt = now, NextAttemptAt = 0l))
    }
}

def recover_cadmus_live_audio(path : string; now : int64) {
    with_cadmus_database(path) $(db) {
        db |> _sql_update(
            type<CadmusLiveAudio>,
            _.Status == "processing",
            (Status = "pending", UpdatedAt = now, NextAttemptAt = 0l))
    }
}

def update_cadmus_audio_edit(
                             path : string;
                             chat_id, message_id : int64;
                             caption : string;
                             edited_at : int64
                             ) {
    with_cadmus_database(path) $(db) {
        db |> with_transaction(SqliteTxnMode.Immediate) {
            let rows <- %linq! from m : CadmusMessage in db
                               where m.ChatId == chat_id && m.TelegramMessageId == message_id
                               select m %%
            if (!empty(rows)) {
                db |> _sql_update(
                    type<CadmusMessage>,
                    _.Id == rows[0].Id,
                    (Caption = caption, EditedAt = edited_at))
                var updated = rows[0]
                updated.Caption = caption
                updated.EditedAt = edited_at
                sync_cadmus_search(db, updated)
            }
        }
    }
}

def mark_cadmus_message_deleted(path : string; chat_id, message_id, edited_at : int64) {
    with_cadmus_database(path) $(db) {
        db |> with_transaction(SqliteTxnMode.Immediate) {
            db |> _sql_update(
                type<CadmusMessage>,
                _.ChatId == chat_id && _.TelegramMessageId == message_id,
                (Deleted = true, EditedAt = edited_at))
            db |> _sql_delete(
                type<CadmusSearchRow>,
                _.ChatId == chat_id && _.TelegramMessageId == message_id)
        }
    }
}

def load_cadmus_offset(path : string) : int64 {
    var offset = 0l
    with_cadmus_database(path) $(db) {
        let rows <- %linq! from s : CadmusState in db where s.Id == 1 select s.UpdateOffset %%
        if (!empty(rows)) {
            offset = rows[0]
        }
    }
    return offset
}

def save_cadmus_offset(path : string; offset : int64) {
    with_cadmus_database(path) $(db) {
        db |> _sql_update(type<CadmusState>, _.Id == 1, (UpdateOffset = offset))
    }
}

def load_recent_cadmus_messages(path : string; chat_id : int64; count : int) : array<CadmusMessage> {
    var result : array<CadmusMessage>
    with_cadmus_database(path) $(db) {
        let newest <- _sql(
            db |> select_from(type<CadmusMessage>)
               |> _where(_.ChatId == chat_id && !_.Deleted)
               |> _order_by_descending((_.MessageDate, _.TelegramMessageId))
               |> take(count))
        result |> reserve(length(newest))
        for (i in range(length(newest))) {
            result |> push(newest[length(newest) - i - 1])
        }
    }
    return <- result
}

def private cadmus_fts_query(
                             text : string;
                             require_all : bool = false;
                             prefix_terms : bool = false
                             ) : string {
    let words <- split_by_chars(to_lower(text), " \t\r\n.,!?;:()[]<>/\\|@#$%^&*+=~")
    var terms : array<string>
    for (word in words) {
        let clean = strip(word)
        if (length(clean) >= 3 && length(terms) < 12) {
            let suffix = prefix_terms ? "*" : ""
            terms |> push("\"{replace(clean, "\"", "\"\"")}\"{suffix}")
        }
    }
    return terms |> join(require_all ? " AND " : " OR ")
}

def search_cadmus_messages(path : string; chat_id : int64; question : string; count : int) : array<CadmusMessage> {
    var result : array<CadmusMessage>
    let query_text = cadmus_fts_query(question)
    if (empty(query_text)) {
        return <- result
    }
    with_cadmus_database(path) $(db) {
        let hits <- _sql(
            db |> select_from(type<CadmusSearchRow>)
               |> _where(_.ChatId == chat_id && _.Body |> text_match(query_text))
               |> _order_by(_.Rank)
               |> take(count))
        for (hit in hits) {
            let rows <- %linq! from m : CadmusMessage in db
                               where m.ChatId == chat_id &&
                                     m.TelegramMessageId == hit.TelegramMessageId && !m.Deleted
                               select m %%
            if (!empty(rows)) {
                result |> push(rows[0])
            }
        }
    }
    return <- result
}

def private append_unique_cadmus_messages(
                                          rows : array<CadmusMessage>;
                                          count : int;
                                          var result : array<CadmusMessage>&
                                          ) {
    for (row in rows) {
        var known = false
        for (existing in result) {
            if (existing.ChatId == row.ChatId &&
                existing.TelegramMessageId == row.TelegramMessageId) {
                known = true
                break
            }
        }
        if (!known) {
            result |> push(row)
            if (length(result) >= count) {
                return
            }
        }
    }
}

def private search_cadmus_message_variant(
                                          path : string;
                                          chat_id : int64;
                                          query : string;
                                          sender_id, before_message_id : int64;
                                          count : int
                                          ) : array<CadmusMessage> {
    var result : array<CadmusMessage>
    let query_text = cadmus_fts_query(query, true, true)
    if (empty(query_text) || count <= 0) {
        return <- result
    }
    with_cadmus_database(path) $(db) {
        let candidate_count = clamp(count * 6, 24, 100)
        let hits <- _sql(
            db |> select_from(type<CadmusSearchRow>)
               |> _where(_.ChatId == chat_id && _.Body |> text_match(query_text))
               |> _order_by(_.Rank)
               |> take(candidate_count))
        for (hit in hits) {
            let rows <- %linq! from m : CadmusMessage in db
                               where m.ChatId == chat_id &&
                                     m.TelegramMessageId == hit.TelegramMessageId &&
                                     m.Role == CADMUS_ROLE_USER && !m.Deleted &&
                                     (sender_id == 0l || m.SenderId == sender_id) &&
                                     (before_message_id == 0l ||
                                      m.TelegramMessageId < before_message_id)
                               select m %%
            if (!empty(rows)) {
                result |> push(rows[0])
                if (length(result) >= count) {
                    break
                }
            }
        }
    }
    return <- result
}

def search_cadmus_message_variants(
                                   path : string;
                                   chat_id : int64;
                                   queries : array<string>;
                                   sender_id, before_message_id : int64;
                                   count : int
                                   ) : array<CadmusMessage> {
    var result : array<CadmusMessage>
    if (empty(queries) || count <= 0) {
        return <- result
    }
    let per_query = max(1, (count + length(queries) - 1) / length(queries))
    for (query in queries) {
        let rows <- search_cadmus_message_variant(
            path, chat_id, query, sender_id, before_message_id, per_query)
        append_unique_cadmus_messages(rows, count, result)
        if (length(result) >= count) {
            return <- result
        }
    }
    for (query in queries) {
        let rows <- search_cadmus_message_variant(
            path, chat_id, query, sender_id, before_message_id, count)
        append_unique_cadmus_messages(rows, count, result)
        if (length(result) >= count) {
            break
        }
    }
    return <- result
}

def search_cadmus_messages_chronological(
                                         path : string;
                                         chat_id : int64;
                                         question : string;
                                         count : int
                                         ) : array<CadmusMessage> {
    var result : array<CadmusMessage>
    let query_text = cadmus_fts_query(question, true)
    if (empty(query_text)) {
        return <- result
    }
    with_cadmus_database(path) $(db) {
        result <- _sql(
            db |> select_from(type<CadmusSearchRow>)
               |> _where(_.ChatId == chat_id && _.Body |> text_match(query_text))
               |> _join(
                    db |> select_from(type<CadmusMessage>)
                       |> _where(_.ChatId == chat_id && !_.Deleted),
                    $(f : CadmusSearchRow, m : CadmusMessage) =>
                        f.TelegramMessageId == m.TelegramMessageId,
                    $(f : CadmusSearchRow, m : CadmusMessage) => m)
               |> _order_by((_.MessageDate, _.TelegramMessageId))
               |> take(count))
    }
    return <- result
}

def cadmus_mention_stats(path : string; chat_id : int64; question : string) : CadmusMentionStats {
    var result = CadmusMentionStats()
    let query_text = cadmus_fts_query(question, true)
    if (empty(query_text)) {
        return result
    }
    with_cadmus_database(path) $(db) {
        let stats = _sql(
            (db |> select_from(type<CadmusSearchRow>)
                |> _where(_.ChatId == chat_id && _.Body |> text_match(query_text)))
            |> _join(
                db |> select_from(type<CadmusMessage>)
                   |> _where(_.ChatId == chat_id && !_.Deleted),
                $(f : CadmusSearchRow, m : CadmusMessage) =>
                    f.TelegramMessageId == m.TelegramMessageId,
                $(f : CadmusSearchRow, m : CadmusMessage) => m)
            |> _aggregate($(_rows) => (
                Total = _rows |> count,
                FirstDate = _rows |> _select(_.MessageDate) |> min,
                LastDate = _rows |> _select(_.MessageDate) |> max)))
        result = CadmusMentionStats(
            Total = stats.Total,
            FirstDate = stats.FirstDate,
            LastDate = stats.LastDate)
    }
    return result
}

def load_cadmus_message(path : string; chat_id, message_id : int64) : array<CadmusMessage> {
    var result : array<CadmusMessage>
    with_cadmus_database(path) $(db) {
        result <- %linq! from m : CadmusMessage in db
                         where m.ChatId == chat_id && m.TelegramMessageId == message_id && !m.Deleted
                         select m %%
    }
    return <- result
}

def load_cadmus_message_context(
                                path : string;
                                chat_id, message_id : int64;
                                before, after : int
                                ) : array<CadmusMessage> {
    var result : array<CadmusMessage>
    with_cadmus_database(path) $(db) {
        let target <- %linq! from m : CadmusMessage in db
                             where m.ChatId == chat_id && m.TelegramMessageId == message_id && !m.Deleted
                             select m %%
        if (empty(target)) {
            return
        }
        let earlier <- _sql(
            db |> select_from(type<CadmusMessage>)
               |> _where(
                    _.ChatId == chat_id && !_.Deleted &&
                    (_.MessageDate < target[0].MessageDate ||
                     (_.MessageDate == target[0].MessageDate && _.TelegramMessageId < message_id)))
               |> _order_by_descending((_.MessageDate, _.TelegramMessageId))
               |> take(before))
        result |> reserve(length(earlier) + 1 + after)
        for (i in range(length(earlier))) {
            result |> push(earlier[length(earlier) - i - 1])
        }
        result |> push(target[0])
        let later <- _sql(
            db |> select_from(type<CadmusMessage>)
               |> _where(
                    _.ChatId == chat_id && !_.Deleted &&
                    (_.MessageDate > target[0].MessageDate ||
                     (_.MessageDate == target[0].MessageDate && _.TelegramMessageId > message_id)))
               |> _order_by((_.MessageDate, _.TelegramMessageId))
               |> take(after))
        for (row in later) {
            result |> push(row)
        }
    }
    return <- result
}

def list_cadmus_messages_before(
                                path : string;
                                chat_id, before_message_id : int64;
                                count : int
                                ) : array<CadmusMessage> {
    var result : array<CadmusMessage>
    with_cadmus_database(path) $(db) {
        let newest <- _sql(
            db |> select_from(type<CadmusMessage>)
               |> _where(
                    _.ChatId == chat_id && !_.Deleted &&
                    (before_message_id == 0l || _.TelegramMessageId < before_message_id))
               |> _order_by_descending((_.MessageDate, _.TelegramMessageId))
               |> take(count))
        result |> reserve(length(newest))
        for (i in range(length(newest))) {
            result |> push(newest[length(newest) - i - 1])
        }
    }
    return <- result
}

def search_cadmus_user_messages(
                                path : string;
                                chat_id : int64;
                                question : string;
                                count : int
                                ) : array<CadmusMessage> {
    var result : array<CadmusMessage>
    let query_text = cadmus_fts_query(question)
    if (empty(query_text)) {
        return <- result
    }
    with_cadmus_database(path) $(db) {
        let candidate_count = count * 4 <= 100 ? count * 4 : 100
        let hits <- _sql(
            db |> select_from(type<CadmusSearchRow>)
               |> _where(_.ChatId == chat_id && _.Body |> text_match(query_text))
               |> _order_by(_.Rank)
               |> take(candidate_count))
        for (hit in hits) {
            let rows <- %linq! from m : CadmusMessage in db
                               where m.ChatId == chat_id &&
                                     m.TelegramMessageId == hit.TelegramMessageId &&
                                     m.Role == CADMUS_ROLE_USER && !m.Deleted
                               select m %%
            if (!empty(rows)) {
                result |> push(rows[0])
                if (length(result) >= count) {
                    break
                }
            }
        }
    }
    return <- result
}

def load_cadmus_messages(
                         path : string;
                         chat_id, from_time, until_time : int64;
                         count : int
                         ) : array<CadmusMessage> {
    var result : array<CadmusMessage>
    with_cadmus_database(path) $(db) {
        result <- _sql(
            db |> select_from(type<CadmusMessage>)
               |> _where(
                    _.ChatId == chat_id && !_.Deleted &&
                    _.MessageDate >= from_time && _.MessageDate < until_time)
               |> _order_by((_.MessageDate, _.TelegramMessageId))
               |> take(count))
    }
    return <- result
}

def load_cadmus_messages_for_sender(
                                    path : string;
                                    chat_id, sender_id, from_time, until_time : int64;
                                    count : int
                                    ) : array<CadmusMessage> {
    var result : array<CadmusMessage>
    with_cadmus_database(path) $(db) {
        result <- _sql(
            db |> select_from(type<CadmusMessage>)
               |> _where(
                    _.ChatId == chat_id && _.SenderId == sender_id && !_.Deleted &&
                    _.MessageDate >= from_time && _.MessageDate < until_time)
               |> _order_by((_.MessageDate, _.TelegramMessageId))
               |> take(count))
    }
    return <- result
}

def find_cadmus_sender(path : string; chat_id : int64; username : string) : int64 {
    var sender_id = 0l
    let identity = to_lower(strip(trim_prefix(strip(username), "@")))
    if (empty(identity)) {
        return 0l
    }
    with_cadmus_database(path) $(db) {
        let rows <- %linq! from m : CadmusMessage in db
                           where m.ChatId == chat_id &&
                                 (m.SenderUsername |> to_lower == identity ||
                                  m.SenderName |> to_lower == identity)
                           orderby m.MessageDate descending
                           select m.SenderId %%
        if (!empty(rows)) {
            sender_id = rows[0]
        }
    }
    return sender_id
}

def format_cadmus_message(msg : CadmusMessage) : string {
    let stamp = format_time(unsafe(reinterpret<clock>(msg.MessageDate)), "%Y-%m-%d %H:%M")
    let who = empty(msg.SenderName) ? msg.Role : msg.SenderName
    let body = empty(strip(msg.Caption)) ? msg.Content : "{msg.Content}\n[caption: {msg.Caption}]"
    return "[{stamp}] {who}: {body}"
}

def cadmus_message_link(chat_id, message_id : int64; chat_username : string = "") : string {
    let username = trim_prefix(strip(chat_username), "@")
    if (!empty(username)) {
        return "https://t.me/{username}/{message_id}"
    }
    let chat_key = "{chat_id}"
    if (starts_with(chat_key, "-100") && length(chat_key) > 4) {
        return "https://t.me/c/{slice(chat_key, 4)}/{message_id}"
    }
    return ""
}

def private has_cadmus_source_message(
                                      source : CadmusMessage;
                                      sources : array<CadmusMessage>
                                      ) : bool {
    for (known in sources) {
        if (known.ChatId == source.ChatId && known.TelegramMessageId == source.TelegramMessageId) {
            return true
        }
    }
    return false
}

def append_cadmus_sources(
                          reply : string;
                          sources : array<CadmusMessage>;
                          chat_username : string = "";
                          max_sources : int = 5
                          ) : string {
    var emitted : array<CadmusMessage>
    for (source in sources) {
        let link = cadmus_message_link(source.ChatId, source.TelegramMessageId, chat_username)
        if (length(emitted) >= max_sources || empty(link) || reply |> contains(link) ||
            has_cadmus_source_message(source, emitted)) {
            continue
        }
        emitted |> push(source)
    }
    if (empty(emitted)) {
        delete emitted
        return reply
    }
    let result = build_string() $(var writer) {
        writer |> write(reply)
        writer |> write("\n\nChat sources:\n")
        for (source in emitted) {
            let link = cadmus_message_link(source.ChatId, source.TelegramMessageId, chat_username)
            let stamp = format_time(unsafe(reinterpret<clock>(source.MessageDate)), "%Y-%m-%d %H:%M")
            let who = empty(source.SenderName) ? source.Role : source.SenderName
            writer |> write("- [{stamp}] {who}: {link}\n")
        }
    }
    delete emitted
    return result
}
