options gen2
options indenting = 4

module samples_store public

require daslib/sql
require sqlite/sqlite_migrate public
require daslib/sql_linq
require daslib/sha_256 public
require strings

//! The playground sample store: schema, migrations, store operations, and
//! every policy decision (size cap, rate ceiling, listing). Zero HTTP --
//! the server module translates transport to these calls and nothing else.

let RATE_WINDOW_SECONDS = 3600l

[sql_table(name = "samples"),
 sql_index(fields = ("ClientIp", "CreatedAt"))]
struct Sample {
    @sql_primary_key Hash : string
    Source : string
    Size : int
    CreatedAt : int64
    ClientIp : string
    Listed : int = 0   // 0/1; bool lacks a bind adapter on the predicate/update rails
    Title : string = ""
    Origin : string = "user"
    Category : string = ""
    PrimaryPath : string = ""   // curated: files[0] from data.json — the UI derives slug/assets from it
    Slug : string = ""          // curated: explicit deep-link slug (?example=) when the manifest has one
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
    //! Strict UTF-8 validation: rejects overlongs, surrogates, and > U+10FFFF.
    var i = 0
    let n = length(data)
    while (i < n) {
        let b0 = uint(data[i])
        if (b0 < 0x80) {
            i ++
            continue
        }
        var need = 0
        var lo = 0x80u
        var hi = 0xBFu
        if (b0 >= 0xC2 && b0 <= 0xDF) {
            need = 1
        } elif (b0 == 0xE0) {
            need = 2
            lo = 0xA0u
        } elif (b0 >= 0xE1 && b0 <= 0xEC) {
            need = 2
        } elif (b0 == 0xED) {
            need = 2
            hi = 0x9Fu
        } elif (b0 >= 0xEE && b0 <= 0xEF) {
            need = 2
        } elif (b0 == 0xF0) {
            need = 3
            lo = 0x90u
        } elif (b0 >= 0xF1 && b0 <= 0xF3) {
            need = 3
        } elif (b0 == 0xF4) {
            need = 3
            hi = 0x8Fu
        } else {
            return false    // 0x80-0xC1 (bare continuation / overlong), 0xF5-0xFF
        }
        return false if (i + need >= n)
        for (k in range(need)) {
            let b = uint(data[i + 1 + k])
            let bLo = k == 0 ? lo : 0x80u
            let bHi = k == 0 ? hi : 0xBFu
            return false if (b < bLo || b > bHi)
        }
        i += need + 1
    }
    return true
}

def is_valid_utf8(s : string) : bool {
    var ok = true
    peek_data(s) $(bytes) {
        ok = is_valid_utf8(bytes)
    }
    return ok
}

def put_sample(db : SqlRunner; source : string; client_ip : string; policy : StorePolicy; now : int64) : PutResult {
    //! Validate, dedup by content hash, and store. First writer wins: a dedup
    //! hit changes nothing about the existing row.
    if (empty(source)) {
        return PutResult(status = PutStatus.empty_source)
    }
    if (length(source) > policy.max_source_bytes) {
        return PutResult(status = PutStatus.too_large)
    }
    if (!is_valid_utf8(source)) {
        return PutResult(status = PutStatus.invalid_utf8)
    }
    let hash = sha256_hex(source)
    if (sample_exists(db, hash)) {
        return PutResult(status = PutStatus.ok, hash = hash, created = false)
    }
    let cutoff = now - RATE_WINDOW_SECONDS
    let recent <- _sql(db |> select_from(type<Sample>)
        |> _where(_.ClientIp == client_ip && _.CreatedAt >= cutoff)
        |> _select(_.Hash))
    if (length(recent) >= policy.max_inserts_per_ip_hour) {
        return PutResult(status = PutStatus.rate_limited)
    }
    db |> insert(Sample(
        Hash = hash,
        Source = source,
        Size = length(source),
        CreatedAt = now,
        ClientIp = client_ip))
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
    //! The curated dropdown, category-then-title ordered.
    let rows <- _sql(db |> select_from(type<Sample>)
        |> _where(_.Listed != 0)
        |> _order_by((_.Category, _.Title))
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
    }
    return hash
}

def list_curated_sample(db : SqlRunner; hash : string; title : string; category : string; path : string; slug : string) {
    db |> _sql_update(
        type<Sample>,
        _.Hash == hash,
        (Listed = 1, Title = title, Category = category, PrimaryPath = path, Slug = slug))
}

def demote_stale_curated(db : SqlRunner; keep : table<string>) : int {
    //! Unlist curated rows whose hash is not in `keep` (the set the importer
    //! just minted). User-promoted samples (Origin != "curated") are untouched;
    //! rows and permalinks always survive. Returns the demoted count.
    let listed <- _sql(db |> select_from(type<Sample>)
        |> _where(_.Listed != 0 && _.Origin == "curated")
        |> _select(_.Hash))
    var demoted = 0
    for (h in listed) {
        if (!key_exists(keep, h)) {
            db |> _sql_update(type<Sample>, _.Hash == h, (Listed = 0))
            demoted ++
        }
    }
    return demoted
}

def promote_sample(db : SqlRunner; hash : string; title : string; category : string) : bool {
    //! List an existing sample in the dropdown. False when the hash is unknown.
    if (!sample_exists(db, hash)) {
        return false
    }
    db |> _sql_update(
        type<Sample>,
        _.Hash == hash,
        (Listed = 1, Title = title, Category = category))
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
    return true
}
