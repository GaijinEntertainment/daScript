// blind-mouse — SQLite/FTS5 index over the .md doc corpus.
// .md files under <root>/docs/ are the source of truth. The DB is a
// rebuildable retrieval index — `rebuild` repopulates it from disk.

options gen2

require daslib/sql public
require sqlite/sqlite_boost public
require sqlite/sqlite_linq public
require sqlite/sqlite_migrate public
require daslib/fio public
require daslib/strings_boost public
require strings public
require math
require store public
require ../common/git_signature.das

// ─── on-disk layout ──────────────────────────────────────────────────

def docs_dir(root : string) : string {
    return path_join(root, "docs")
}

def db_path(root : string) : string {
    return path_join(root, "index.db")
}

def is_directory_path(path : string) : bool {
    let s = stat(path)
    return s.is_valid && s.is_dir
}

def ensure_root(root : string) {
    if (!is_directory_path(root)) {
        mkdir_rec(root)
    }
    let dd = docs_dir(root)
    if (!is_directory_path(dd)) {
        mkdir_rec(dd)
    }
}

def list_doc_files(root : string) : array<string> {
    var out : array<string>
    let dd = docs_dir(root)
    if (!is_directory_path(dd)) return <- out
    dir(dd) $(filename) {
        if (filename == "." || filename == "..") return
        if (ends_with(filename, ".md")) {
            out |> push(path_join(dd, filename))
        }
    }
    return <- out
}

// ─── schema ──────────────────────────────────────────────────────────

[sql_table(name = "docs")]
struct DocRow {
    @sql_primary_key
    slug : string
    path : string
    title : string
    created : string
    last_verified : string
    body_hash : string
}

[sql_table(name = "links"),
 sql_index(unique = true, fields = ("from_slug", "to_slug"))]
struct LinkRow {
    from_slug : string
    to_slug : string
}

[sql_fts5(name = "search_idx")]
struct SearchRow {
    Slug : string
    Text : string
    @sql_fts_rank Rank : float
}

// Append-only log of every ask. Survives rebuild() (which only wipes the
// doc cache). The miss list is the input signal for `mouse__add` —
// "questions asked but no doc covers them yet". `useful` captures the
// false-positive case: hit rows the caller marked as no-match (BM25
// returned cards but none addressed the question). NULL = unrated;
// 0 = no-match. We never write 1 — implicit positive avoids sycophancy.
[sql_table(name = "query_log")]
struct QueryLog {
    @sql_primary_key id : int64
    asked_at : string
    question : string
    match_count : int
    top_slug : string
    source : string  // "cli" | "mcp"
    @safe_when_uninitialized useful : Option<int>
}

[sql_migration(version = 1, description = "create docs/links/search_idx")]
def migration_001(db : SqlRunner) {
    db |> create_table(type<DocRow>)
    db |> create_table(type<LinkRow>)
    db |> create_table(type<SearchRow>)
}

[sql_migration(version = 2, description = "add query_log")]
def migration_002(db : SqlRunner) {
    // Raw SQL pinned to the v2-era schema. Originally this was
    // `db |> create_table(type<QueryLog>)`, but `create_table` regenerates
    // DDL from the current struct on every fresh-DB migration — so as soon
    // as a later migration evolves QueryLog (e.g. v4 adds `useful`), v2
    // would create the column too, and v4's add_column would duplicate.
    // Pinning v2's DDL here decouples migration history from struct shape.
    db |> exec("CREATE TABLE query_log (id INTEGER PRIMARY KEY, asked_at TEXT NOT NULL, question TEXT NOT NULL, match_count INTEGER NOT NULL, top_slug TEXT NOT NULL, source TEXT NOT NULL)")
}

// Persisted across cold opens: the staleness signature used by
// ensure_index_fresh. Without persistence every CLI invocation would see an
// empty in-memory signature and rebuild from scratch.
[sql_table(name = "index_meta")]
struct IndexMeta {
    @sql_primary_key key : string
    value : string
}

[sql_migration(version = 3, description = "add index_meta for staleness signature")]
def migration_003(db : SqlRunner) {
    db |> create_table(type<IndexMeta>)
}

[sql_migration(version = 4, description = "add useful flag to query_log")]
def migration_004(db : SqlRunner) {
    db |> add_column(type<QueryLog>, "useful")
}

def with_index_db(root : string; blk : block<(db : SqlRunner) : void>) {
    ensure_root(root)
    with_latest_sqlite(db_path(root)) $(db) {
        invoke(blk, db)
    }
}

// ─── result shapes ────────────────────────────────────────────────────

struct SearchHit {
    slug : string
    title : string
    path : string
    last_verified : string
    rank : float
}

struct DupeMatch {
    slug : string
    title : string
    rank : float        // FTS5 BM25; kept for power users / debugging
    similarity : float  // Jaccard, 0..1; primary ranking key for dedup decisions
}

struct AddOutcome {
    slug : string
    similar : array<DupeMatch>
    created : bool
    written_path : string
    error : string
}

// ─── rebuild ─────────────────────────────────────────────────────────

def rebuild(db : SqlRunner; root : string) : int {
    let files <- list_doc_files(root)
    var n = 0
    db |> with_transaction() {
        db |> exec("DELETE FROM docs")
        db |> exec("DELETE FROM links")
        db |> exec("DELETE FROM search_idx")
        for (f in files) {
            var doc : ParsedDoc
            var error : string
            if (!read_doc(f, doc, error)) {
                to_log(LOG_WARNING, "skipping {f}: {error}")
                continue
            }
            if (!empty(doc.parse_error)) {
                to_log(LOG_WARNING, "skipping {f}: {doc.parse_error}")
                continue
            }
            if (empty(doc.frontmatter.slug)) {
                to_log(LOG_WARNING, "skipping {f}: missing slug")
                continue
            }
            if (!is_valid_slug(doc.frontmatter.slug)) {
                to_log(LOG_WARNING, "skipping {f}: invalid slug `{doc.frontmatter.slug}` (would be unretrievable via get)")
                continue
            }
            // get rebuilds the path as <docs>/{slug}.md, so a hand-edited
            // file whose basename diverges from its frontmatter slug would
            // be searchable but unfetchable.
            let expected_basename = "{doc.frontmatter.slug}.md"
            if (base_name(f) != expected_basename) {
                to_log(LOG_WARNING, "skipping {f}: basename does not match slug (expected `{expected_basename}`, would be unretrievable via get)")
                continue
            }
            db |> insert(DocRow(
                slug = doc.frontmatter.slug,
                path = f,
                title = doc.frontmatter.title,
                created = doc.frontmatter.created,
                last_verified = doc.frontmatter.last_verified,
                body_hash = doc.body_hash))
            for (link in doc.frontmatter.links) {
                // insert_or_ignore: a doc accidentally listing the same
                // link twice in frontmatter would otherwise hit the
                // UNIQUE(from_slug, to_slug) constraint and abort the
                // whole rebuild transaction.
                db |> insert_or_ignore(LinkRow(
                    from_slug = doc.frontmatter.slug,
                    to_slug = link))
            }
            // doc.body already contains the `## Questions` section, so
            // we don't append doc.questions separately — that would
            // double-index aliases and skew BM25.
            let combined = build_string() $(var w) {
                if (!empty(doc.frontmatter.title)) {
                    w |> write(doc.frontmatter.title)
                    w |> write("\n")
                }
                w |> write(doc.body)
            }
            db |> insert(SearchRow(
                Slug = doc.frontmatter.slug,
                Text = combined,
                Rank = 0.0f))
            n ++
        }
    }
    return n
}

// ─── staleness signature & auto-reindex ──────────────────────────────

// Persisted (via index_meta) so the cost only pays out on real changes.
let SIGNATURE_KEY = "signature"

def get_index_signature(db : SqlRunner) : string {
    let opt <- _sql(db |> select_from(type<IndexMeta>)
                       |> _where(_.key == SIGNATURE_KEY)
                       |> _first_opt())
    if (opt |> is_some) return (opt |> unwrap).value
    return ""
}

def set_index_signature(db : SqlRunner; sig : string) {
    db |> _sql_upsert(
        IndexMeta(key = SIGNATURE_KEY, value = sig),
        _.key,
        (value = _excluded.value))
}

// The canonical staleness signature for a docs root — `compute_signature`
// over `*.md` under `docs/`. Use this everywhere we stamp a fresh signature
// so the input shape stays in lockstep with `ensure_index_fresh`'s read.
def public compute_docs_signature(root : string) : string {
    return compute_signature(root, @(p : string) => ends_with(p, ".md"), [docs_dir(root)])
}

// Reconcile the SQLite index against on-disk docs. If the staleness signature
// matches what's stored, this is a no-op. On mismatch, rebuilds and stores
// the new signature. Returns the number of docs (re)indexed (0 on cache hit).
//
// Signature compute is delegated to utils/common/git_signature: the predicate
// only narrows by file extension because the shared module pre-narrows status
// output and filesystem walks to paths under `search_dirs_abs = [docs_dir]`.
def ensure_index_fresh(db : SqlRunner; root : string) : int {
    let docs_abs = docs_dir(root)
    if (!is_directory_path(docs_abs)) return 0
    let sig = compute_docs_signature(root)
    if (empty(sig)) return 0
    let stored = get_index_signature(db)
    if (sig == stored) return 0
    let n = rebuild(db, root)
    set_index_signature(db, sig)
    return n
}

// ─── search ──────────────────────────────────────────────────────────

// Strip non-alphanumeric chars (except whitespace and `*`) so a free-form
// user question can't break FTS5's query parser. v0 trade-off: no
// quoted-phrase / OR / NEAR support for free-text queries; users who
// want those can pass them via raw FTS5 syntax (vNext: bypass sanitizer
// with a `--raw-query` flag or detect quoted forms).
def sanitize_fts5_query(q : string) : string {
    return build_string() $(var w) {
        let n = length(q)
        for (i in range(n)) {
            let c = character_at(q, i)  // nolint:PERF003
            let is_safe = is_alnum(c) || c == ' ' || c == '*'
            if (is_safe) {
                w |> write(slice(q, i, i + 1))
            } else {
                w |> write(" ")
            }
        }
    }
}

// Hit-count cap. Untrusted callers (CLI/MCP) supply `k`, and SQLite
// treats `LIMIT -1` as "no limit", so a negative or oversize `k` would
// scan the whole index. 50 is well above any sane top-k for this corpus.
let MAX_SEARCH_K = 50

def search(db : SqlRunner; query : string; k : int; raw_query : bool = false) : array<SearchHit> {
    var out : array<SearchHit>
    if (k <= 0) return <- out
    let kk = min(k, MAX_SEARCH_K)
    var fts_query : string
    if (raw_query) {
        // Power-user path: pass the user's string straight to FTS5. Bad
        // syntax surfaces via _try_sql + LOG_WARNING below.
        fts_query = strip(query)
        if (empty(fts_query)) return <- out
    } else {
        // to_lower: FTS5 keywords (OR/AND/NOT/NEAR) are uppercase-only operators.
        // A user query like "foo OR bar" would tokenize to [foo, OR, bar] and
        // OR-join to `foo OR OR OR bar` — invalid FTS syntax → empty result.
        // Downcasing every token makes user-typed keywords inert.
        let cleaned = to_lower(strip(sanitize_fts5_query(query)))
        if (empty(cleaned)) return <- out
        // Free-form queries → OR-joined so BM25 ranks docs by how many of the
        // user's words appear, rather than failing when one word is missing.
        // FTS5 default is whitespace-AND, which is wrong for a "find related"
        // search. Users who want strict AND/phrase pass raw_query=true.
        let words <- split(cleaned, " ")
        var meaningful : array<string>
        meaningful |> reserve(length(words))
        for (w in words) {
            if (length(w) >= 2) {
                meaningful |> push(w)
            }
        }
        if (empty(meaningful)) return <- out
        fts_query = meaningful |> join(" OR ")
    }
    let raw_hits <- _try_sql(db |> select_from(type<SearchRow>)
                                |> _where(_.Text |> text_match(fts_query))
                                |> _order_by(_.Rank)
                                |> take(kk))
    if (raw_hits |> is_err) {
        to_log(LOG_WARNING, "search failed: {raw_hits |> unwrap_err}")
        return <- out
    }
    for (h in raw_hits._value) {
        let drow_opt <- _sql(db |> select_from(type<DocRow>)
                               |> _where(_.slug == h.Slug)
                               |> _first_opt())
        if (!(drow_opt |> is_some)) continue
        let drow = drow_opt |> unwrap
        out |> push(SearchHit(
            slug = drow.slug,
            title = drow.title,
            path = drow.path,
            last_verified = drow.last_verified,
            rank = h.Rank))
    }
    return <- out
}

// ─── dedup scoring (Jaccard over titles, stop-words filtered) ────────

// add_doc treats a hit at or above this as "looks like a duplicate" and
// blocks the create unless force=true. Below this, hits are advisory only —
// shown to the caller, but the add proceeds. The caller (LLM or human) is
// the actual decider; the threshold just stops obvious near-paraphrases
// from sneaking in.
let JACCARD_HARD_BLOCK = 0.7f

// Conservative English stopword set. Kept small on purpose: filter common
// glue words that flood OR-queries, but bias toward false negatives (let
// borderline words like `do` through) over false positives (filtering rare
// words used semantically).
let STOP_WORDS : table<string> <- {
    "a", "an", "the", "is", "are", "was", "were", "be", "been", "being",
    "do", "does", "did", "doing", "can", "could", "should", "would", "will",
    "may", "might", "of", "in", "on", "at", "to", "for", "with", "from", "by",
    "as", "and", "or", "but", "not", "no", "if", "how", "what", "why", "when",
    "where", "which", "that", "this", "these", "those", "i", "you", "my"
}

def tokenize_for_jaccard(s : string) : table<string> {
    var out : table<string>
    let cleaned = to_lower(strip(sanitize_fts5_query(s)))
    if (empty(cleaned)) return <- out
    let parts <- split(cleaned, " ")
    for (w in parts) {
        if (length(w) >= 2 && !key_exists(STOP_WORDS, w)) {
            out |> insert(w)
        }
    }
    return <- out
}

// Score every BM25 hit by Jaccard on tokenized titles, filter out the
// zero-similarity noise, and return the rest sorted by similarity desc.
// dupe_check no longer makes the block decision — that moved into add_doc
// (and into the response messaging in main.das). Callers see the ranking.
def dupe_check(db : SqlRunner; question : string; k : int = 5) : array<DupeMatch> {
    let hits <- search(db, question, k)
    let qtok = tokenize_for_jaccard(question)
    var out : array<DupeMatch>
    out |> reserve(length(hits))
    for (h in hits) {
        let ttok = tokenize_for_jaccard(h.title)
        let s = jaccard(qtok, ttok)
        if (s > 0.0f) {
            out |> push(DupeMatch(
                slug = h.slug,
                title = h.title,
                rank = h.rank,
                similarity = s))
        }
    }
    sort(out) <| $(a, b) => a.similarity > b.similarity
    return <- out
}

// ─── reverse links ───────────────────────────────────────────────────

def linked_from(db : SqlRunner; slug : string) : array<string> {
    var froms <- _sql(db |> select_from(type<LinkRow>)
                        |> _where(_.to_slug == slug)
                        |> _order_by(_.from_slug)
                        |> _select(_.from_slug))
    return <- froms
}

// ─── query log ───────────────────────────────────────────────────────

def now_iso(db : SqlRunner) : string {
    return db |> query_scalar("SELECT datetime('now')", type<string>)
}

def log_query(db : SqlRunner; question : string;
              hits : array<SearchHit>; source : string) : int64 {
    let top = !empty(hits) ? hits[0].slug : ""
    db |> insert(QueryLog(
        asked_at = now_iso(db),
        question = question,
        match_count = length(hits),
        top_slug = top,
        source = source,
        useful = none(type<int>)))
    return db |> last_insert_rowid()
}

// Mark a query_log row as no-match (useful = 0). Returns rows affected
// (0 if id doesn't exist; 1 on success). Idempotent — re-marking is a
// no-op SET to the same value.
def mark_no_match(db : SqlRunner; id : int64) : int {
    return db |> _sql_update(type<QueryLog>, _.id == id, (useful = some(0)))
}

enum LogMode {
    All     //!< every row (newest first)
    Misses  //!< match_count == 0 (FTS5 returned nothing)
    Bad     //!< useful == 0 (caller marked the hit irrelevant)
    Review  //!< match_count > 0 AND useful IS NULL (queue for retrospective rating)
}

def recent_queries(db : SqlRunner; n : int; mode : LogMode) : array<QueryLog> {
    if (mode == LogMode.Misses) {
        var rows <- _sql(db |> select_from(type<QueryLog>)
                           |> _where(_.match_count == 0)
                           |> _order_by_descending(_.id)
                           |> take(n))
        return <- rows
    }
    if (mode == LogMode.Bad) {
        var rows <- _sql(db |> select_from(type<QueryLog>)
                           |> _where(_.useful |> unwrap_or(-1) == 0)
                           |> _order_by_descending(_.id)
                           |> take(n))
        return <- rows
    }
    if (mode == LogMode.Review) {
        var rows <- _sql(db |> select_from(type<QueryLog>)
                           |> _where(_.match_count > 0 && _.useful |> is_none)
                           |> _order_by_descending(_.id)
                           |> take(n))
        return <- rows
    }
    var rows <- _sql(db |> select_from(type<QueryLog>)
                       |> _order_by_descending(_.id)
                       |> take(n))
    return <- rows
}

// ─── add ─────────────────────────────────────────────────────────────

def today_iso(db : SqlRunner) : string {
    return db |> query_scalar("SELECT date('now')", type<string>)
}

def add_doc(db : SqlRunner; root : string;
            question : string; body : string;
            slug_hint : string; force : bool) : AddOutcome {
    var outcome : AddOutcome
    let clean_question = sanitize_question(question)
    if (empty(clean_question)) {
        outcome.error = "question is empty after sanitization"
        return <- outcome
    }
    if (!force) {
        var similar <- dupe_check(db, clean_question, 5)
        if (!empty(similar) && similar[0].similarity >= JACCARD_HARD_BLOCK) {
            outcome.similar <- similar
            outcome.created = false
            return <- outcome
        }
        // Below threshold: stash the list so the success path can surface
        // the closest matches as an awareness hint.
        outcome.similar <- similar
    }
    var existing : table<string>
    let all_slugs <- _sql(db |> select_from(type<DocRow>) |> _select(_.slug))
    for (s in all_slugs) {
        existing |> insert(s)
    }
    var slug : string
    if (!empty(slug_hint) && is_valid_slug(slug_hint)) {
        slug = dedupe_slug(slug_hint, existing)
    } else {
        slug = slug_from_title_dedupe(clean_question, existing)
    }
    if (empty(slug)) {
        slug = "doc-{ref_time_ticks()}"
    }
    let now = today_iso(db)
    let fm = Frontmatter(
        slug = slug,
        title = clean_question,
        created = now,
        last_verified = now)
    let composed = build_string() $(var w) {
        w |> write(body)
        if (!empty(body) && character_at(body, length(body) - 1) != '\n') {  // nolint:PERF003
            w |> write("\n")
        }
        w |> write("\n## Questions\n")
        w |> write("- {clean_question}\n")
    }
    let path = path_join(docs_dir(root), "{slug}.md")
    var werr : string
    if (!write_doc(path, fm, composed, werr)) {
        outcome.error = "could not write {path}: {werr}"
        return <- outcome
    }
    rebuild(db, root)
    set_index_signature(db, compute_docs_signature(root))
    outcome.slug = slug
    outcome.created = true
    outcome.written_path = path
    return <- outcome
}
