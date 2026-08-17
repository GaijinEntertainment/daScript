options gen2
options indenting = 4

module build_queue public

require daslib/sql
require sqlite/sqlite_migrate public
require daslib/sql_linq
require daslib/logger
require daslib/json_boost
require daslib/strings_boost
require strings

//! The wasm build queue: schema, migration, the job state machine, and queue
//! policy (claim timeout, attempt ceiling). A job is a (source hash x
//! toolchain) pair, so a toolchain bump re-enqueues lazily instead of
//! invalidating anything eagerly. Zero HTTP and zero filesystem — the server
//! module owns transport, build_artifacts owns the blobs directory.

let BUILD_STATE_QUEUED = "queued"
let BUILD_STATE_BUILDING = "building"
let BUILD_STATE_DONE = "done"
let BUILD_STATE_FAILED = "failed"

let BUILD_MODE_MODULE = "module"   // bare .wasm the playground runtime instantiates
let BUILD_MODE_PAGE = "page"       // standalone html+js+wasm page (an /examples card)

//! A failed build stores the compiler error for the user; cap what one job can hold.
let MAX_BUILD_ERROR_BYTES = 65536

//! Ceiling on how many files one upload may claim to carry. The per-mode set
//! below is the real limit; this bounds the work of parsing a manifest that
//! has not been checked against it yet.
let MAX_ARTIFACT_MANIFEST_ENTRIES = 16

def expected_artifact_names(mode : string) : array<string> {
    //! The EXACT file set a mode may publish. The builder enforces this too,
    //! but a build runs the user's own compile-time code, so the server does
    //! not take the builder's word for what a job produced: anything the build
    //! dropped beside its real output would be served publicly. Empty = the
    //! mode publishes nothing.
    if (mode == BUILD_MODE_MODULE) {
        return <- ["sample.wasm"]
    }
    if (mode == BUILD_MODE_PAGE) {
        return <- ["sample.html", "sample.js", "sample.wasm"]
    }
    return <- array<string>()
}

// Require-path roots that force the standalone-page artifact: their native halves
// (GL context, audio device, image decode) live in emscripten's JS glue, which a
// bare wasi module cannot carry. strudel and live_stub are here for what they
// require transitively (audio; a glfw window) — the scan only sees source text.
let private PAGE_REQUIRE_ROOTS : table<string> <- {
    "glfw", "opengl", "imgui", "audio", "minfft", "stbimage", "clipboard", "strudel", "live_stub"
}

def private text_requires_page(text : string) : bool {
    for (line in split(text, "\n")) {
        let t = strip(line)
        continue if (!((t |> starts_with("require ")) || (t |> starts_with("require\t"))))
        let rest = strip(replace(slice(t, 8), "\t", " "))
        let sp = find(rest, " ")
        var tok = sp >= 0 ? slice(rest, 0, sp) : rest
        if (tok |> starts_with("?")) {
            tok = slice(tok, 1)
        }
        let sl = find(tok, "/")
        let seg = sl >= 0 ? slice(tok, 0, sl) : tok
        if (key_exists(PAGE_REQUIRE_ROOTS, seg)) {
            return true
        }
    }
    return false
}

def detect_build_mode(source : string) : string {
    //! module vs page, from the stored source document (raw `.das` text or the
    //! `{files, active}` bundle): any require of a native graphics/audio
    //! namespace, in any file, makes the artifact a standalone page.
    var jerr = ""
    var doc = read_json(source, jerr)
    if (doc != null && doc.value is _object && key_exists(doc.value as _object, "files")) {
        let files = doc?["files"]
        if (files != null && files.value is _object) {
            for (v in values(files.value as _object)) {
                let text : string = v ?? ""
                if (text_requires_page(text)) {
                    return BUILD_MODE_PAGE
                }
            }
        }
        return BUILD_MODE_MODULE
    }
    return text_requires_page(source) ? BUILD_MODE_PAGE : BUILD_MODE_MODULE
}

[sql_table(name = "build_jobs")]
struct BuildJob {
    // Composite primary key (Hash, ToolchainId) lives in the migration DDL —
    // the linq rail has no composite @sql_primary_key.
    Hash : string
    ToolchainId : string
    State : string = BUILD_STATE_QUEUED
    Mode : string = BUILD_MODE_MODULE
    Attempts : int = 0
    QueuedAt : int64
    StartedAt : int64
    FinishedAt : int64
    Error : string = ""
    ArtifactPath : string = ""    // blobs-relative artifact dir ("<toolchain>/<hash>")
    ArtifactFiles : string = ""   // JSON array of artifact file names
}

[sql_table(name = "build_meta")]
struct BuildMeta {
    @sql_primary_key MetaKey : string
    MetaValue : string
}

[sql_migration(version = 4, description = "wasm build queue + build metadata")]
def build_queue_migration_004(db : SqlRunner) {
    db |> exec(
        "CREATE TABLE build_jobs (" +
        "Hash TEXT NOT NULL," +
        "ToolchainId TEXT NOT NULL," +
        "State TEXT NOT NULL DEFAULT 'queued'," +
        "Mode TEXT NOT NULL DEFAULT 'module'," +
        "Attempts INTEGER NOT NULL DEFAULT 0," +
        "QueuedAt INTEGER NOT NULL," +
        "StartedAt INTEGER NOT NULL DEFAULT 0," +
        "FinishedAt INTEGER NOT NULL DEFAULT 0," +
        "Error TEXT NOT NULL DEFAULT ''," +
        "ArtifactPath TEXT NOT NULL DEFAULT ''," +
        "ArtifactFiles TEXT NOT NULL DEFAULT ''," +
        "PRIMARY KEY (Hash, ToolchainId))")
    db |> exec("CREATE INDEX idx_build_jobs_state_queued ON build_jobs (State, QueuedAt)")
    db |> exec("CREATE TABLE build_meta (MetaKey TEXT PRIMARY KEY, MetaValue TEXT NOT NULL)")
}

struct BuildConfig {
    //! Queue policy; the launcher fills it from config.
    claim_timeout_seconds : int = 600
    max_build_attempts : int = 3
}

struct EnqueueResult {
    state : string   // the job's state after the call
    created : bool   // false = the job already existed and was left as-is
}

struct ClaimedJob {
    hash : string    // "" = nothing to claim
    toolchain : string
    mode : string
    attempts : int
}

struct BuildStatus {
    found : bool
    state : string
    mode : string
    attempts : int
    error : string
    artifact_path : string
    artifact_files : string   // JSON array text as stored
    position : int            // 1-based place among queued jobs; 0 unless queued
}

def current_toolchain(db : SqlRunner) : string {
    //! The toolchain new jobs enqueue under; "" until a builder announces one.
    let hit <- _sql(db |> select_from(type<BuildMeta>)
        |> _where(_.MetaKey == "current_toolchain")
        |> _select(_.MetaValue)
        |> take(1))
    return !empty(hit) ? hit[0] : ""
}

def set_current_toolchain(db : SqlRunner; toolchain : string) {
    //! The builder announces its toolchain after the ABI canary runs clean —
    //! this call IS the go-live for that toolchain id.
    if (empty(current_toolchain(db))) {
        db |> insert(BuildMeta(MetaKey = "current_toolchain", MetaValue = toolchain))
    } else {
        db |> _sql_update(type<BuildMeta>, _.MetaKey == "current_toolchain", (MetaValue = toolchain))
    }
    logger_info("playground.build", "current toolchain set", JV((toolchain = toolchain)))
}

def private job_rows(db : SqlRunner; hash : string; toolchain : string) : array<BuildJob> {
    return <- _sql(db |> select_from(type<BuildJob>)
        |> _where(_.Hash == hash && _.ToolchainId == toolchain)
        |> take(1))
}

def enqueue_build(db : SqlRunner; hash : string; toolchain : string; mode : string; now : int64; force : bool = false) : EnqueueResult {
    //! Idempotent: an existing job in any state is returned as-is. `force`
    //! re-queues a failed job (admin retry) — a failed build is deterministic,
    //! so a plain re-request must not burn attempts on it.
    let rows <- job_rows(db, hash, toolchain)
    if (!empty(rows)) {
        if (rows[0].State == BUILD_STATE_FAILED && force) {
            db |> _sql_update(type<BuildJob>, _.Hash == hash && _.ToolchainId == toolchain,
                (State = BUILD_STATE_QUEUED, Attempts = 0, Error = "", QueuedAt = now,
                    StartedAt = 0l, FinishedAt = 0l, ArtifactPath = "", ArtifactFiles = ""))
            logger_info("playground.build", "re-enqueue failed job", JV((hash = hash, toolchain = toolchain)))
            return EnqueueResult(state = BUILD_STATE_QUEUED, created = true)
        }
        return EnqueueResult(state = rows[0].State, created = false)
    }
    db |> insert(BuildJob(Hash = hash, ToolchainId = toolchain, Mode = mode, QueuedAt = now))
    logger_info("playground.build", "enqueue", JV((hash = hash, toolchain = toolchain, mode = mode)))
    return EnqueueResult(state = BUILD_STATE_QUEUED, created = true)
}

def requeue_stale_claims(db : SqlRunner; cfg : BuildConfig; now : int64) : int {
    //! A claim the builder never resolved (crash, kill, network) returns to the
    //! queue after the timeout; a job that already spent its attempts fails
    //! instead, so a build that wedges the builder cannot wedge the queue.
    let cutoff = now - int64(cfg.claim_timeout_seconds)
    let stale <- _sql(db |> select_from(type<BuildJob>)
        |> _where(_.State == BUILD_STATE_BUILDING && _.StartedAt <= cutoff))
    for (j in stale) {
        if (j.Attempts >= cfg.max_build_attempts) {
            db |> _sql_update(type<BuildJob>, _.Hash == j.Hash && _.ToolchainId == j.ToolchainId,
                (State = BUILD_STATE_FAILED, FinishedAt = now,
                    Error = "build timed out after {j.Attempts} attempt(s)"))
            logger_error("playground.build", "job failed: attempts exhausted",
                JV((hash = j.Hash, toolchain = j.ToolchainId, attempts = j.Attempts)))
        } else {
            db |> _sql_update(type<BuildJob>, _.Hash == j.Hash && _.ToolchainId == j.ToolchainId,
                (State = BUILD_STATE_QUEUED))
            logger_warning("playground.build", "requeue stale claim",
                JV((hash = j.Hash, toolchain = j.ToolchainId, attempts = j.Attempts)))
        }
    }
    return length(stale)
}

def claim_next_build(db : SqlRunner; cfg : BuildConfig; toolchain : string; now : int64) : ClaimedJob {
    //! Oldest queued job for the builder's own toolchain — a builder must never
    //! receive a job minted for a different toolchain id. Empty hash = nothing
    //! to claim. Stale claims are swept first, so a dead builder's jobs come
    //! back through here.
    requeue_stale_claims(db, cfg, now)
    let rows <- _sql(db |> select_from(type<BuildJob>)
        |> _where(_.State == BUILD_STATE_QUEUED && _.ToolchainId == toolchain)
        |> _order_by((_.QueuedAt, _.Hash))
        |> take(1))
    if (empty(rows)) {
        return ClaimedJob()
    }
    db |> _sql_update(type<BuildJob>, _.Hash == rows[0].Hash && _.ToolchainId == toolchain,
        (State = BUILD_STATE_BUILDING, StartedAt = now, Attempts = rows[0].Attempts + 1))
    logger_info("playground.build", "claim",
        JV((hash = rows[0].Hash, toolchain = toolchain, attempt = rows[0].Attempts + 1)))
    return ClaimedJob(hash = rows[0].Hash, toolchain = toolchain,
        mode = rows[0].Mode, attempts = rows[0].Attempts + 1)
}

def record_build_done(db : SqlRunner; hash : string; toolchain : string; artifact_path : string; artifact_files_json : string; now : int64) : bool {
    //! Only a job still in `building` may resolve — a result arriving after the
    //! claim was requeued is refused, and the builder discards its work (the
    //! artifact is content-addressed, so the next attempt finds it anyway).
    let rows <- job_rows(db, hash, toolchain)
    if (empty(rows) || rows[0].State != BUILD_STATE_BUILDING) {
        logger_error("playground.build", "refused done result",
            JV((hash = hash, toolchain = toolchain, state = empty(rows) ? "missing" : rows[0].State)))
        return false
    }
    db |> _sql_update(type<BuildJob>, _.Hash == hash && _.ToolchainId == toolchain,
        (State = BUILD_STATE_DONE, FinishedAt = now, Error = "",
            ArtifactPath = artifact_path, ArtifactFiles = artifact_files_json))
    logger_info("playground.build", "done",
        JV((hash = hash, toolchain = toolchain, artifact = artifact_path)))
    return true
}

def record_build_failed(db : SqlRunner; hash : string; toolchain : string; error : string; now : int64) : bool {
    //! A failed build is terminal (a compile error is deterministic); the error
    //! text is the user-facing product and is stored capped.
    let rows <- job_rows(db, hash, toolchain)
    if (empty(rows) || rows[0].State != BUILD_STATE_BUILDING) {
        logger_error("playground.build", "refused failure result",
            JV((hash = hash, toolchain = toolchain, state = empty(rows) ? "missing" : rows[0].State)))
        return false
    }
    db |> _sql_update(type<BuildJob>, _.Hash == hash && _.ToolchainId == toolchain,
        (State = BUILD_STATE_FAILED, FinishedAt = now, Error = capped_error(error)))
    logger_info("playground.build", "failed", JV((hash = hash, toolchain = toolchain)))
    return true
}

def private capped_error(error : string) : string {
    if (length(error) <= MAX_BUILD_ERROR_BYTES) {
        return error
    }
    return slice(error, 0, MAX_BUILD_ERROR_BYTES) + "\n... (truncated)"
}

def build_status(db : SqlRunner; hash : string; toolchain : string) : BuildStatus {
    //! Queue position counts this job and every queued job at or before its
    //! QueuedAt — ties count conservatively, which only ever overstates the wait.
    let rows <- job_rows(db, hash, toolchain)
    if (empty(rows)) {
        return BuildStatus()
    }
    var st = BuildStatus(found = true, state = rows[0].State, mode = rows[0].Mode,
        attempts = rows[0].Attempts, error = rows[0].Error,
        artifact_path = rows[0].ArtifactPath, artifact_files = rows[0].ArtifactFiles)
    if (rows[0].State == BUILD_STATE_QUEUED) {
        let qa = rows[0].QueuedAt
        st.position = _sql(db |> select_from(type<BuildJob>)
            |> _where(_.State == BUILD_STATE_QUEUED && _.ToolchainId == toolchain && _.QueuedAt <= qa)
            |> count())
    }
    return st
}
