options gen2
options indenting = 4
options _dasllama_internal = true   // the exchange SERVICE: implements the wire contract, so it reads the schema module directly

module ladder_store public

require dasllama/dasllama_exchange_schema public   // BoxIdentity/SidecarInfo are store API types
require daslib/sql
require sqlite/sqlite_migrate public
require daslib/sql_linq
require daslib/sha_256 public
require daslib/logger
require daslib/json_boost
require strings

//! The ladder store: schema, migrations, and every policy decision (size caps, rate
//! ceiling, source stamping, the sidecar lookup ladder). Zero HTTP — the server module
//! translates transport to these calls and nothing else. Engine-free like exchange_schema.

let RATE_WINDOW_SECONDS = 3600l

//! One accepted upload (community submit or official import), kept verbatim — runs
//! reference their submission by id, so a receipt render never re-normalizes.
[sql_table(name = "submissions"),
 sql_index(fields = ("ClientIp", "CreatedAt"))]
struct Submission {
    @sql_primary_key Id : int
    Kind : string               // "records" | "sidecar" | "official"
    @sql_unique DocSha : string // content hash of the verbatim upload — exact resubmits dedup
    Doc : string
    CreatedAt : int64
    ClientIp : string
}

//! One measured run, flattened for filtering; the verbatim BenchRun lives in the
//! submission Doc at (ModelIdx, RunIdx). Identity per METHODOLOGY.md: (Gguf, Box,
//! Engine, Backend, Flavor, Workload) — official re-imports replace on it.
[sql_table(name = "runs"),
 sql_index(fields = ("DasllamaVersion")),
 sql_index(fields = ("SubmissionId")),
 sql_index(fields = ("Gguf", "Box", "Engine", "Backend", "Flavor", "Workload"))]
struct Run {
    @sql_primary_key Id : int
    SubmissionId : int
    ModelIdx : int
    RunIdx : int
    Gguf : string
    ModelArch : string = ""
    Quant : string = ""
    ParamsB : double = 0.0lf
    ActiveB : double = 0.0lf
    SizeBytes : int64 = 0l
    Engine : string
    Backend : string
    Flavor : string
    Workload : string = ""
    Box : string
    Threads : int
    RunDate : string
    Version : string = ""
    DasllamaVersion : int = 0
    EngineSha : string = ""
    TuneSha : string = ""
    Source : string             // stamped by the store, never by the submitter
    Verified : int = 0
    CreatedAt : int64
}

//! One tune sidecar, content-addressed by its own sha256; the identity columns are
//! lifted by exchange_schema's parse_sidecar_info and drive the lookup ladder.
[sql_table(name = "sidecars"),
 sql_index(fields = ("DasllamaVersion", "Box")),
 sql_index(fields = ("DasllamaVersion", "Platform", "Arch", "Cpu"))]
struct SidecarRow {
    @sql_primary_key Sha : string
    Doc : string
    DasllamaVersion : int
    Box : string
    Platform : string
    Arch : string
    ModelId : string = ""
    Cpu : string
    EngineSha : string = ""
    Written : string = ""
    Noise : string = ""
    KernelCount : int = 0
    Verified : int = 0
    CreatedAt : int64
    ClientIp : string
}

[sql_migration(version = 1, description = "create ladder submissions, runs, sidecars")]
def ladder_migration_001(db : SqlRunner) {
    db |> exec(
        "CREATE TABLE submissions (" +
        "Id INTEGER PRIMARY KEY," +
        "Kind TEXT NOT NULL," +
        "DocSha TEXT NOT NULL," +
        "Doc TEXT NOT NULL," +
        "CreatedAt INTEGER NOT NULL," +
        "ClientIp TEXT NOT NULL)")
    db |> exec("CREATE UNIQUE INDEX uq_submissions_DocSha ON submissions (DocSha)")
    db |> exec("CREATE INDEX idx_submissions_ClientIp_CreatedAt ON submissions (ClientIp, CreatedAt)")
    db |> exec(
        "CREATE TABLE runs (" +
        "Id INTEGER PRIMARY KEY," +
        "SubmissionId INTEGER NOT NULL," +
        "ModelIdx INTEGER NOT NULL," +
        "RunIdx INTEGER NOT NULL," +
        "Gguf TEXT NOT NULL," +
        "ModelArch TEXT NOT NULL DEFAULT ''," +
        "Quant TEXT NOT NULL DEFAULT ''," +
        "ParamsB REAL NOT NULL DEFAULT 0," +
        "ActiveB REAL NOT NULL DEFAULT 0," +
        "SizeBytes INTEGER NOT NULL DEFAULT 0," +
        "Engine TEXT NOT NULL," +
        "Backend TEXT NOT NULL," +
        "Flavor TEXT NOT NULL," +
        "Workload TEXT NOT NULL DEFAULT ''," +
        "Box TEXT NOT NULL," +
        "Threads INTEGER NOT NULL," +
        "RunDate TEXT NOT NULL," +
        "Version TEXT NOT NULL DEFAULT ''," +
        "DasllamaVersion INTEGER NOT NULL DEFAULT 0," +
        "EngineSha TEXT NOT NULL DEFAULT ''," +
        "TuneSha TEXT NOT NULL DEFAULT ''," +
        "Source TEXT NOT NULL," +
        "Verified INTEGER NOT NULL DEFAULT 0," +
        "CreatedAt INTEGER NOT NULL)")
    db |> exec("CREATE INDEX idx_runs_DasllamaVersion ON runs (DasllamaVersion)")
    db |> exec("CREATE INDEX idx_runs_SubmissionId ON runs (SubmissionId)")
    db |> exec("CREATE INDEX idx_runs_Gguf_Box_Engine_Backend_Flavor_Workload ON runs (Gguf, Box, Engine, Backend, Flavor, Workload)")
    db |> exec(
        "CREATE TABLE sidecars (" +
        "Sha TEXT PRIMARY KEY," +
        "Doc TEXT NOT NULL," +
        "DasllamaVersion INTEGER NOT NULL," +
        "Box TEXT NOT NULL," +
        "Platform TEXT NOT NULL," +
        "Arch TEXT NOT NULL," +
        "ModelId TEXT NOT NULL DEFAULT ''," +
        "Cpu TEXT NOT NULL," +
        "EngineSha TEXT NOT NULL DEFAULT ''," +
        "Written TEXT NOT NULL DEFAULT ''," +
        "Noise TEXT NOT NULL DEFAULT ''," +
        "KernelCount INTEGER NOT NULL DEFAULT 0," +
        "Verified INTEGER NOT NULL DEFAULT 0," +
        "CreatedAt INTEGER NOT NULL," +
        "ClientIp TEXT NOT NULL)")
    db |> exec("CREATE INDEX idx_sidecars_DasllamaVersion_Box ON sidecars (DasllamaVersion, Box)")
    db |> exec("CREATE INDEX idx_sidecars_DasllamaVersion_Platform_Arch_Cpu ON sidecars (DasllamaVersion, Platform, Arch, Cpu)")
}

//! Every limit the store enforces; the launcher fills it from config.
struct LadderPolicy {
    max_records_bytes : int = 4194304
    max_sidecar_bytes : int = 1048576
    max_submissions_per_ip_hour : int = 20
    // total submit ATTEMPTS (accepted or rejected) per IP per hour — the transport-layer gate
    // that caps parse/validate work on the single tick thread; the accepted ceiling above
    // counts only stored rows, so it never throttles a flood of invalid documents
    max_attempts_per_ip_hour : int = 120
    // public submissions accepted only when true; default closed = read-only board. The server
    // flips this at runtime via the loopback /admin/submit route (no restart), and a restart
    // re-reads the config, so closed is the fail-safe resting state
    submit_open : bool = false
}

enum PutStatus {
    ok
    empty_doc
    too_large
    rate_limited
    invalid
}

struct PutSidecarResult {
    status : PutStatus
    sha : string        // set when status == ok
    created : bool      // false = content-identical sidecar already stored
    error : string      // first validation error when status == invalid
}

struct PutRecordsResult {
    status : PutStatus
    submission_id : int // set when status == ok and created
    runs_added : int
    created : bool      // false = byte-identical document already accepted
    error : string
}

struct ImportResult {
    ok : bool
    runs_added : int
    runs_replaced : int
    error : string
}

//! One rung of the sidecar lookup ladder. tier: "exact" (same box string),
//! "cpu" (same platform/arch/cpu on another machine), "arch" (same platform/arch only),
//! "" (a listing row — not part of a ladder).
struct SidecarHit {
    sha : string
    box : string
    tier : string
    verified : bool
    noise : string
    written : string
    kernel_count : int
    engine_sha : string
    dasllama_version : int
}

def private rate_exceeded(db : SqlRunner; client_ip : string; policy : LadderPolicy; now : int64) : bool {
    // one scalar on the (ClientIp, CreatedAt) index, ahead of any parse or hash work
    let cutoff = now - RATE_WINDOW_SECONDS
    let recent = _sql(db |> select_from(type<Submission>)
        |> _where(_.ClientIp == client_ip && _.CreatedAt >= cutoff)
        |> count())
    return recent >= policy.max_submissions_per_ip_hour
}

def private submission_id_by_sha(db : SqlRunner; doc_sha : string) : int {
    let hit <- _sql(db |> select_from(type<Submission>)
        |> _where(_.DocSha == doc_sha)
        |> _select(_.Id)
        |> take(1))
    return !empty(hit) ? hit[0] : 0
}

def private add_submission(db : SqlRunner; kind, doc_sha, doc, client_ip : string; now : int64) : int {
    return int(db |> insert(Submission(
        Kind = kind,
        DocSha = doc_sha,
        Doc = doc,
        CreatedAt = now,
        ClientIp = client_ip)))
}

//! Accept a community tune sidecar. Checks run cheapest-first (size, rate ceiling,
//! then parse/validate/hash), mirroring the playground store discipline: a throttled
//! caller must not spend a validation pass on work that can never be stored.
def put_sidecar(db : SqlRunner; var doc : string; client_ip : string; policy : LadderPolicy; now : int64) : PutSidecarResult {
    if (empty(doc)) {
        return PutSidecarResult(status = PutStatus.empty_doc)
    }
    if (length(doc) > policy.max_sidecar_bytes) {
        return PutSidecarResult(status = PutStatus.too_large)
    }
    if (rate_exceeded(db, client_ip, policy, now)) {
        return PutSidecarResult(status = PutStatus.rate_limited)
    }
    // server-side privacy strip, mirroring the submitting client (defense in depth): what a
    // stripped-at-the-client doc round-trips unchanged, an unstripped one loses here — the
    // STRIPPED text is what gets validated, content-hashed, stored, and served
    doc = exchange_strip_private(doc)
    if (empty(doc)) {
        return PutSidecarResult(status = PutStatus.invalid, error = "json parse failed")
    }
    var errors : array<string>
    if (!validate_sidecar_submission(doc, errors)) {
        let first = !empty(errors) ? errors[0] : "invalid"
        return PutSidecarResult(status = PutStatus.invalid, error = first)
    }
    var info : SidecarInfo
    if (!parse_sidecar_info(doc, info)) {
        return PutSidecarResult(status = PutStatus.invalid, error = "index fields do not parse")
    }
    let sha = sha256_hex(doc)
    let existing <- _sql(db |> select_from(type<SidecarRow>)
        |> _where(_.Sha == sha)
        |> _select(_.Sha)
        |> take(1))
    if (!empty(existing)) {
        return PutSidecarResult(status = PutStatus.ok, sha = sha, created = false)
    }
    add_submission(db, "sidecar", sha, doc, client_ip, now)
    db |> insert(SidecarRow(
        Sha = sha,
        Doc = doc,
        DasllamaVersion = info.dasllama_version,
        Box = info.box,
        Platform = info.identity.platform,
        Arch = info.identity.arch,
        ModelId = info.identity.model_id,
        Cpu = info.identity.cpu,
        EngineSha = info.engine_sha,
        Written = info.written,
        Noise = info.noise,
        KernelCount = info.kernel_count,
        CreatedAt = now,
        ClientIp = client_ip))
    logger_info("ladder.store", "insert sidecar",
        JV((sha = sha, version = info.dasllama_version, box = info.box, ip = client_ip)))
    return PutSidecarResult(status = PutStatus.ok, sha = sha, created = true)
}

// extract the flat filter columns of every run in a validated store document;
// the verbatim truth stays in the submission Doc at (ModelIdx, RunIdx)
def private insert_runs(db : SqlRunner; doc_text : string; submission_id : int; source : string;
                        verified : int; now : int64) : int {
    var jerr = ""
    var doc = read_json(doc_text, jerr)
    if (doc == null || !(doc is _array)) {
        return 0
    }
    var rows : array<Run>
    var total = 0
    for (m in doc as _array) {
        let mruns = m?["runs"]
        if (mruns != null && mruns is _array) {
            total += length(mruns as _array)
        }
    }
    rows |> reserve(total)
    var mi = 0
    for (m in doc as _array) {
        var ri = 0
        for (r in m?["runs"] as _array) {
            rows |> emplace(Run(
                SubmissionId = submission_id,
                ModelIdx = mi,
                RunIdx = ri,
                Gguf = m?["gguf"] ?? "",
                ModelArch = m?["arch"] ?? "",
                Quant = m?["quant"] ?? "",
                ParamsB = m?["params_b"] ?? 0.0lf,
                ActiveB = m?["active_b"] ?? 0.0lf,
                SizeBytes = m?["size_bytes"] ?? 0l,
                Engine = r?["engine"] ?? "",
                Backend = r?["backend"] ?? "",
                Flavor = r?["flavor"] ?? "",
                Workload = r?["workload"] ?? "",
                Box = r?["box"] ?? "",
                // read through double: the schema accepts a JSON number OR longint, and the int
                // `?? 0` coalesce silently yields 0 for a float form (3.0), which would drop the
                // row into the un-versioned bucket and escape every ?version=N filter
                Threads = int(r?["threads"] ?? 0.0lf),
                RunDate = r?["date"] ?? "",
                Version = r?["version"] ?? "",
                DasllamaVersion = int(r?["dasllama_version"] ?? 0.0lf),
                EngineSha = r?["sha"] ?? "",
                TuneSha = r?["tune_sha"] ?? "",
                Source = source,
                Verified = verified,
                CreatedAt = now))
            ri++
        }
        mi++
    }
    if (!empty(rows)) {
        db |> insert(rows)
    }
    return length(rows)
}

//! Accept a community bench-record submission (the lcpp_bench `-o json` document).
//! Source is stamped "community" and rows enter unverified regardless of the payload.
def put_records(db : SqlRunner; var doc : string; client_ip : string; policy : LadderPolicy; now : int64) : PutRecordsResult {
    if (empty(doc)) {
        return PutRecordsResult(status = PutStatus.empty_doc)
    }
    if (length(doc) > policy.max_records_bytes) {
        return PutRecordsResult(status = PutStatus.too_large)
    }
    if (rate_exceeded(db, client_ip, policy, now)) {
        return PutRecordsResult(status = PutStatus.rate_limited)
    }
    // redact home-layout paths from cmd/env/files server-side too (the submitting client does
    // it as well; idempotent, so a clean upload round-trips and dedups) — the STORED text is
    // the redacted one, validated/hashed below
    doc = redact_record_paths(doc)
    if (empty(doc)) {
        return PutRecordsResult(status = PutStatus.invalid, error = "json parse failed")
    }
    var errors : array<string>
    if (!validate_record_submission(doc, errors)) {
        let first = !empty(errors) ? errors[0] : "invalid"
        return PutRecordsResult(status = PutStatus.invalid, error = first)
    }
    let doc_sha = sha256_hex(doc)
    let dup = submission_id_by_sha(db, doc_sha)
    if (dup != 0) {
        return PutRecordsResult(status = PutStatus.ok, submission_id = dup, created = false)
    }
    let sid = add_submission(db, "records", doc_sha, doc, client_ip, now)
    let added = insert_runs(db, doc, sid, "community", 0, now)
    logger_info("ladder.store", "insert records submission",
        JV((id = sid, runs = added, ip = client_ip)))
    return PutRecordsResult(status = PutStatus.ok, submission_id = sid, runs_added = added, created = true)
}

//! Seed or refresh the official board from a repo record store (deploy-time, trusted).
//! Shape-level validation only — official history predates the release counter. Rows
//! replace on the METHODOLOGY identity, so a re-measure never duplicates.
def import_official_store(db : SqlRunner; doc : string; now : int64) : ImportResult {
    var errors : array<string>
    if (!validate_record_store(doc, errors)) {
        let first = !empty(errors) ? errors[0] : "invalid"
        return ImportResult(error = first)
    }
    let doc_sha = sha256_hex(doc)
    if (submission_id_by_sha(db, doc_sha) != 0) {
        return ImportResult(ok = true)   // byte-identical import: nothing to do
    }
    let sid = add_submission(db, "official", doc_sha, doc, "importer", now)
    // replace-by-identity: drop the official rows this document re-measures
    var jerr = ""
    var doc_js = read_json(doc, jerr)
    var replaced = 0
    if (doc_js != null && doc_js is _array) {
        for (m in doc_js as _array) {
            let gguf : string = m?["gguf"] ?? ""
            for (r in m?["runs"] as _array) {
                let engine : string = r?["engine"] ?? ""
                let backend : string = r?["backend"] ?? ""
                let flavor : string = r?["flavor"] ?? ""
                let workload : string = r?["workload"] ?? ""
                let box : string = r?["box"] ?? ""
                let n = db |> _sql_delete(type<Run>,
                    _.Source == "official" && _.Gguf == gguf && _.Box == box && _.Engine == engine
                        && _.Backend == backend && _.Flavor == flavor && _.Workload == workload)
                replaced += n
            }
        }
    }
    let added = insert_runs(db, doc, sid, "official", 1, now)
    logger_info("ladder.store", "import official store",
        JV((id = sid, runs = added, replaced = replaced)))
    return ImportResult(ok = true, runs_added = added, runs_replaced = replaced)
}

def private tier_hits(tier : string; var seen : table<string>;
                      var out : array<SidecarHit>; rows : array<SidecarRow>) {
    // verified rows surface before unverified within a tier; rows arrive newest-first
    for (round in range(2)) {
        let want_verified = round == 0
        for (r in rows) {
            if (!key_exists(seen, r.Sha) && (r.Verified != 0) == want_verified) {
                seen |> insert(r.Sha)
                out |> push(SidecarHit(
                    sha = r.Sha,
                    box = r.Box,
                    tier = tier,
                    verified = r.Verified != 0,
                    noise = r.Noise,
                    written = r.Written,
                    kernel_count = r.KernelCount,
                    engine_sha = r.EngineSha,
                    dasllama_version = r.DasllamaVersion))
            }
        }
    }
}

//! The lookup ladder: exact box, then same cpu on another machine of that platform/arch,
//! then same platform/arch. Within a tier verified sidecars come first, newest first.
def find_sidecars(db : SqlRunner; version : int; ident : BoxIdentity) : array<SidecarHit> {
    var out : array<SidecarHit>
    var seen : table<string>
    // exact = same platform/arch/model_id/cpu (the indexed columns) — NOT the full box string.
    // os_build is stored verbatim but deliberately out of the match key: the Windows UBR churns
    // monthly, and an OS-patch is not a hardware change, so matching on the raw box would demote
    // every pre-patch sidecar out of the exact tier. model_id is what separates same-cpu machines.
    let exact <- _sql(db |> select_from(type<SidecarRow>)
        |> _where(_.DasllamaVersion == version && _.Platform == ident.platform
            && _.Arch == ident.arch && _.ModelId == ident.model_id && _.Cpu == ident.cpu)
        |> _order_by_descending(_.CreatedAt))
    tier_hits("exact", seen, out, exact)
    let cpu <- _sql(db |> select_from(type<SidecarRow>)
        |> _where(_.DasllamaVersion == version && _.Platform == ident.platform
            && _.Arch == ident.arch && _.Cpu == ident.cpu)
        |> _order_by_descending(_.CreatedAt))
    tier_hits("cpu", seen, out, cpu)
    let arch <- _sql(db |> select_from(type<SidecarRow>)
        |> _where(_.DasllamaVersion == version && _.Platform == ident.platform
            && _.Arch == ident.arch)
        |> _order_by_descending(_.CreatedAt))
    tier_hits("arch", seen, out, arch)
    return <- out
}

def private listing_hits(rows : array<SidecarRow>) : array<SidecarHit> {
    return <- [for (r in rows); SidecarHit(
        sha = r.Sha,
        box = r.Box,
        tier = "",
        verified = r.Verified != 0,
        noise = r.Noise,
        written = r.Written,
        kernel_count = r.KernelCount,
        engine_sha = r.EngineSha,
        dasllama_version = r.DasllamaVersion)]
}

//! Every sidecar for one version (0 = all versions), newest first — the browse page
//! listing. tier stays "" on every row: a listing is not a lookup ladder.
def list_sidecars(db : SqlRunner; version : int) : array<SidecarHit> {
    if (version == 0) {
        let rows <- _sql(db |> select_from(type<SidecarRow>)
            |> _order_by_descending(_.CreatedAt))
        return <- listing_hits(rows)
    }
    let rows <- _sql(db |> select_from(type<SidecarRow>)
        |> _where(_.DasllamaVersion == version)
        |> _order_by_descending(_.CreatedAt))
    return <- listing_hits(rows)
}

//! The stored sidecar document, or "" when the sha is unknown (an empty document can
//! never be stored, so "" is unambiguous).
def get_sidecar_doc(db : SqlRunner; sha : string) : string {
    let hit <- _sql(db |> select_from(type<SidecarRow>)
        |> _where(_.Sha == sha)
        |> _select(_.Doc)
        |> take(1))
    return !empty(hit) ? hit[0] : ""
}

def set_sidecar_verified(db : SqlRunner; sha : string; verified : bool) : bool {
    let n = db |> _sql_update(type<SidecarRow>, _.Sha == sha, (Verified = verified ? 1 : 0))
    if (n > 0) {
        logger_info("ladder.store", "sidecar verified flag", JV((sha = sha, verified = verified)))
    }
    return n > 0
}

def delete_sidecar(db : SqlRunner; sha : string) : bool {
    let n = db |> _sql_delete(type<SidecarRow>, _.Sha == sha)
    // the sidecar is content-addressed, so its Sha IS the submission's DocSha: drop the
    // verbatim submission copy too, or GET /api/submission/:id keeps serving a "deleted"
    // sidecar AND its orphaned DocSha (unique) makes the content permanently unresubmittable
    db |> _sql_delete(type<Submission>, _.Kind == "sidecar" && _.DocSha == sha)
    if (n > 0) {
        logger_info("ladder.store", "delete sidecar", JV((sha = sha)))
    }
    return n > 0
}

//! Flip the verified flag on every run of a submission; returns the affected count.
def set_submission_verified(db : SqlRunner; submission_id : int; verified : bool) : int {
    let n = db |> _sql_update(type<Run>, _.SubmissionId == submission_id,
        (Verified = verified ? 1 : 0))
    if (n > 0) {
        logger_info("ladder.store", "submission verified flag",
            JV((id = submission_id, verified = verified, runs = n)))
    }
    return n
}

//! Remove a submission and every run it carried (the admin delete lever). A sidecar
//! submission's sidecar row is removed through its content sha.
def delete_submission(db : SqlRunner; submission_id : int) : int {
    let subs <- _sql(db |> select_from(type<Submission>)
        |> _where(_.Id == submission_id)
        |> take(1))
    if (empty(subs)) {
        return 0
    }
    var n = db |> _sql_delete(type<Run>, _.SubmissionId == submission_id)
    if (subs[0].Kind == "sidecar") {
        n += db |> _sql_delete(type<SidecarRow>, _.Sha == subs[0].DocSha)
    }
    db |> _sql_delete(type<Submission>, _.Id == submission_id)
    logger_info("ladder.store", "delete submission", JV((id = submission_id, rows = n)))
    return n
}

//! Every dasllama version the store has seen (runs and sidecars merged), newest first —
//! the site's version filter; entry zero is "latest".
def list_versions(db : SqlRunner) : array<int> {
    var vs <- _sql(db |> select_from(type<Run>)
        |> _select(_.DasllamaVersion)
        |> distinct())
    var sv <- _sql(db |> select_from(type<SidecarRow>)
        |> _select(_.DasllamaVersion)
        |> distinct())
    var seen : table<int>
    var out : array<int>
    for (v in vs) {
        if (!key_exists(seen, v)) {
            seen |> insert(v)
            out |> push(v)
        }
    }
    for (v in sv) {
        if (!key_exists(seen, v)) {
            seen |> insert(v)
            out |> push(v)
        }
    }
    sort(out) $(a, b) => a > b
    return <- out
}

//! The ladder rows for one version (0 = all versions), newest submissions first.
def list_runs(db : SqlRunner; version : int) : array<Run> {
    if (version == 0) {
        return <- _sql(db |> select_from(type<Run>)
            |> _order_by_descending(_.CreatedAt))
    }
    return <- _sql(db |> select_from(type<Run>)
        |> _where(_.DasllamaVersion == version)
        |> _order_by_descending(_.CreatedAt))
}

//! The verbatim submission document a run came from ("" when the id is unknown) —
//! the receipt render reads the BenchRun back at the run's (ModelIdx, RunIdx).
def get_submission_doc(db : SqlRunner; submission_id : int) : string {
    let hit <- _sql(db |> select_from(type<Submission>)
        |> _where(_.Id == submission_id)
        |> _select(_.Doc)
        |> take(1))
    return !empty(hit) ? hit[0] : ""
}
