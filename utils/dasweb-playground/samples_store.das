options gen2
options indenting = 4

module samples_store public

require daslib/sql
require sqlite/sqlite_migrate public
require daslib/sql_linq
require daslib/sha_256 public
require daslib/utf8_utils
require daslib/logger
require daslib/json_boost
require strings

//! The playground sample store: schema, migrations, store operations, and
//! every policy decision (size cap, rate ceiling, listing). Zero HTTP --
//! the server module translates transport to these calls and nothing else.

let RATE_WINDOW_SECONDS = 3600l

//! Sort key for a listing the importer did not order (an admin promotion):
//! after every manifest entry, in a store the dropdown renders top to bottom.
let LISTING_POSITION_LAST = 999999

[sql_table(name = "samples"),
 sql_index(fields = ("ClientIp", "CreatedAt"))]
struct Sample {
    @sql_primary_key Hash : string
    Source : string
    Size : int
    CreatedAt : int64
    ClientIp : string
    Listed : int = 0   // 0/1
    Title : string = ""
    Origin : string = "user"
    Category : string = ""
    PrimaryPath : string = ""   // curated: files[0] from data.json — the UI derives slug/assets from it
    Slug : string = ""          // curated: explicit deep-link slug (?example=) when the manifest has one
    Position : int = LISTING_POSITION_LAST   // manifest order; unset sorts last
}

[sql_migration(version = 1, description = "create playground samples")]
def samples_migration_001(db : SqlRunner) {
    db |> exec(
        "CREATE TABLE samples (" +
        "Hash TEXT PRIMARY KEY," +
        "Source TEXT NOT NULL," +
        "Size INTEGER NOT NULL," +
        "CreatedAt INTEGER NOT NULL," +
        "ClientIp TEXT NOT NULL," +
        "Listed INTEGER NOT NULL DEFAULT 0," +
        "Title TEXT NOT NULL DEFAULT ''," +
        "Origin TEXT NOT NULL DEFAULT 'user')")
    db |> exec("CREATE INDEX idx_samples_client_created ON samples (ClientIp, CreatedAt)")
}

[sql_migration(version = 2, description = "curated listing: category, primary path, slug")]
def samples_migration_002(db : SqlRunner) {
    db |> exec("ALTER TABLE samples ADD COLUMN Category TEXT NOT NULL DEFAULT ''")
    db |> exec("ALTER TABLE samples ADD COLUMN PrimaryPath TEXT NOT NULL DEFAULT ''")
    db |> exec("ALTER TABLE samples ADD COLUMN Slug TEXT NOT NULL DEFAULT ''")
}

[sql_migration(version = 3, description = "curated listing keeps manifest order")]
def samples_migration_003(db : SqlRunner) {
    db |> exec("ALTER TABLE samples ADD COLUMN Position INTEGER NOT NULL DEFAULT 999999")
}

struct StorePolicy {
    //! Every limit the store enforces; the launcher fills it from config.
    max_source_bytes : int = 262144
    max_inserts_per_ip_hour : int = 100
}

enum PutStatus {
    ok
    empty_source
    too_large
    invalid_utf8
    rate_limited
}

struct PutResult {
    status : PutStatus
    hash : string       // set when status == ok
    created : bool      // false = dedup hit (the sample already existed)
}

def is_valid_utf8(data : array<uint8> const# implicit) : bool {
    //! Strict UTF-8: rejects overlongs, surrogates, and > U+10FFFF.
    return is_utf8_string_valid(data)
}

def is_valid_utf8(s : string) : bool {
    //! Validates the BYTES, not the characters: the utf8_utils string overload
    //! iterates decoded characters, which is not what a stored source is.
    var ok = true
    peek_data(s) $(bytes) {
        ok = is_utf8_string_valid(bytes)
    }
    return ok
}

def private rate_exceeded(db : SqlRunner; client_ip : string; policy : StorePolicy; now : int64) : bool {
    let cutoff = now - RATE_WINDOW_SECONDS
    let recent <- _sql(db |> select_from(type<Sample>)
        |> _where(_.ClientIp == client_ip && _.CreatedAt >= cutoff)
        |> _select(_.Hash))
    return length(recent) >= policy.max_inserts_per_ip_hour
}

def put_sample(db : SqlRunner; source : string; client_ip : string; policy : StorePolicy; now : int64) : PutResult {
    //! Validate, dedup by content hash, and store. First writer wins: a dedup
    //! hit changes nothing about the existing row.
    //! Checks run cheapest-first, and the rate ceiling comes BEFORE validation
    //! and hashing: it counts rows created in the window, so a throttled caller
    //! must not be able to spend a full UTF-8 scan and SHA-256 pass per request
    //! on work that can never be stored — including for a body that would have
    //! deduped.
    if (empty(source)) {
        return PutResult(status = PutStatus.empty_source)
    }
    if (length(source) > policy.max_source_bytes) {
        return PutResult(status = PutStatus.too_large)
    }
    if (rate_exceeded(db, client_ip, policy, now)) {
        return PutResult(status = PutStatus.rate_limited)
    }
    if (!is_valid_utf8(source)) {
        return PutResult(status = PutStatus.invalid_utf8)
    }
    let hash = sha256_hex(source)
    if (sample_exists(db, hash)) {
        return PutResult(status = PutStatus.ok, hash = hash, created = false)
    }
    db |> insert(Sample(
        Hash = hash,
        Source = source,
        Size = length(source),
        CreatedAt = now,
        ClientIp = client_ip))
    logger_info("playground.store", "insert user sample",
        JV((hash = hash, size = length(source), ip = client_ip)))
    return PutResult(status = PutStatus.ok, hash = hash, created = true)
}

def sample_exists(db : SqlRunner; hash : string) : bool {
    let hit <- _sql(db |> select_from(type<Sample>)
        |> _where(_.Hash == hash)
        |> _select(_.Hash)
        |> take(1))
    return !empty(hit)
}

def get_source(db : SqlRunner; hash : string) : string {
    //! The stored source, or "" when the hash is unknown (an empty source can
    //! never be stored, so "" is unambiguous).
    let hit <- _sql(db |> select_from(type<Sample>)
        |> _where(_.Hash == hash)
        |> _select(_.Source)
        |> take(1))
    return !empty(hit) ? hit[0] : ""
}

struct ListedSample {
    hash : string
    title : string
    category : string
    path : string    // primary file path (slug/assets derivation in the UI); "" for user samples
    slug : string    // explicit deep-link slug; "" = derive from path
}

def listed_samples(db : SqlRunner) : array<ListedSample> {
    //! The curated dropdown in manifest order — the order the site authored is
    //! what the dropdown shows, and entry zero is what the playground opens
    //! with. Category/title only break ties among unordered rows.
    let rows <- _sql(db |> select_from(type<Sample>)
        |> _where(_.Listed != 0)
        |> _order_by((_.Position, _.Category, _.Title))
        |> _select((Hash = _.Hash, Title = _.Title, Category = _.Category,
            PrimaryPath = _.PrimaryPath, Slug = _.Slug)))
    return <- [for (r in rows); ListedSample(hash = r.Hash, title = r.Title, category = r.Category,
        path = r.PrimaryPath, slug = r.Slug)]
}

def upsert_curated_source(db : SqlRunner; source : string; now : int64) : string {
    //! Importer entry: store a curated body bypassing user policy (no rate
    //! ceiling, no size cap — the deploy tree is trusted). Returns the hash;
    //! an existing row (either origin) is left untouched.
    let hash = sha256_hex(source)
    if (!sample_exists(db, hash)) {
        db |> insert(Sample(
            Hash = hash,
            Source = source,
            Size = length(source),
            CreatedAt = now,
            ClientIp = "importer",
            Origin = "curated"))
        logger_info("playground.store", "insert curated sample",
            JV((hash = hash, size = length(source))))
    }
    return hash
}

def list_curated_sample(db : SqlRunner; hash : string; title : string; category : string; path : string; slug : string; position : int) {
    db |> _sql_update(
        type<Sample>,
        _.Hash == hash,
        (Listed = 1, Title = title, Category = category, PrimaryPath = path, Slug = slug, Position = position))
    logger_info("playground.store", "list curated sample",
        JV((hash = hash, title = title, category = category)))
}

def demote_stale_curated(db : SqlRunner; keep : table<string>) : int {
    //! Unlist importer-owned listings whose hash is not in `keep` (the set the
    //! importer just minted). Importer ownership = PrimaryPath set (only
    //! list_curated_sample sets it) — NOT Origin: a curated entry that dedups
    //! onto a pre-existing user row keeps Origin "user" yet must still demote
    //! when it leaves the manifest. Admin promotions (PrimaryPath "") are
    //! untouched; rows and permalinks always survive. Returns the demoted count.
    let listed <- _sql(db |> select_from(type<Sample>)
        |> _where(_.Listed != 0 && _.PrimaryPath != "")
        |> _select(_.Hash))
    var demoted = 0
    for (h in listed) {
        if (!key_exists(keep, h)) {
            db |> _sql_update(type<Sample>, _.Hash == h, (Listed = 0))
            logger_info("playground.store", "demote stale curated", JV((hash = h)))
            demoted ++
        }
    }
    return demoted
}

def promote_sample(db : SqlRunner; hash : string; title : string; category : string) : bool {
    //! List an existing sample in the dropdown. False when the hash is unknown.
    //! Clears PrimaryPath: that field marks importer ownership, so a sample the
    //! manifest once carried would otherwise be re-demoted by the next import.
    if (!sample_exists(db, hash)) {
        return false
    }
    db |> _sql_update(
        type<Sample>,
        _.Hash == hash,
        (Listed = 1, Title = title, Category = category, PrimaryPath = "", Position = LISTING_POSITION_LAST))
    logger_info("playground.store", "promote sample",
        JV((hash = hash, title = title, category = category)))
    return true
}

def demote_sample(db : SqlRunner; hash : string) : bool {
    //! Remove a sample from the dropdown; the row and its permalink stay.
    if (!sample_exists(db, hash)) {
        return false
    }
    db |> _sql_update(
        type<Sample>,
        _.Hash == hash,
        (Listed = 0))
    logger_info("playground.store", "demote sample", JV((hash = hash)))
    return true
}
