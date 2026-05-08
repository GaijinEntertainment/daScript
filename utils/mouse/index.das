// blind-mouse — SQLite/FTS5 index over the .md doc corpus.
// .md files under <root>/docs/ are the source of truth. The DB is a
// rebuildable retrieval index — `rebuild` repopulates it from disk.

options gen2

require daslib/sql public
require sqlite/sqlite_boost public
require sqlite/sqlite_linq public
require sqlite/sqlite_migrate public
require daslib/fio public
require strings public
require math
require store public

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
    if (!is_directory_path(dd)) {
        return <- out
    }
    dir(dd) $(filename) {
        if (filename == "." || filename == "..") {
            return
        }
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
// "questions asked but no doc covers them yet".
[sql_table(name = "query_log")]
struct QueryLog {
    @sql_primary_key id : int64
    asked_at : string
    question : string
    match_count : int
    top_slug : string
    source : string  // "cli" | "mcp"
}

[sql_migration(version = 1, description = "create docs/links/search_idx")]
def migration_001(db : SqlRunner) {
    db |> create_table(type<DocRow>)
    db |> create_table(type<LinkRow>)
    db |> create_table(type<SearchRow>)
}

[sql_migration(version = 2, description = "add query_log")]
def migration_002(db : SqlRunner) {
    db |> create_table(type<QueryLog>)
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
    rank : float
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
            n += 1
        }
    }
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
            let is_safe = ((c >= '0' && c <= '9') ||
                           (c >= 'a' && c <= 'z') ||
                           (c >= 'A' && c <= 'Z') ||
                           c == ' ' || c == '*')
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

def search(db : SqlRunner; query : string; k : int) : array<SearchHit> {
    var out : array<SearchHit>
    if (k <= 0) {
        return <- out
    }
    let kk = min(k, MAX_SEARCH_K)
    let cleaned = strip(sanitize_fts5_query(query))
    if (empty(cleaned)) {
        return <- out
    }
    // Free-form queries → OR-joined so BM25 ranks docs by how many of the
    // user's words appear, rather than failing when one word is missing.
    // FTS5 default is whitespace-AND, which is wrong for a "find related"
    // search. Users who want strict AND/phrase have to wait for
    // vNext --raw-query.
    let words <- split(cleaned, " ")
    var meaningful : array<string>
    meaningful |> reserve(length(words))
    for (w in words) {
        if (length(w) >= 2) {
            meaningful |> push(w)
        }
    }
    if (length(meaningful) == 0) {
        return <- out
    }
    let or_query = meaningful |> join(" OR ")
    let raw_hits <- _try_sql(db |> select_from(type<SearchRow>)
                                |> _where(_.Text |> text_match(or_query))
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
        if (!(drow_opt |> is_some)) {
            continue
        }
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

def dupe_check(db : SqlRunner; question : string; k : int = 5) : array<DupeMatch> {
    let hits <- search(db, question, k)
    var out : array<DupeMatch>
    out |> reserve(length(hits))
    for (h in hits) {
        out |> push(DupeMatch(slug = h.slug, title = h.title, rank = h.rank))
    }
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
              hits : array<SearchHit>; source : string) {
    let top = length(hits) > 0 ? hits[0].slug : ""
    db |> insert(QueryLog(
        asked_at = now_iso(db),
        question = question,
        match_count = length(hits),
        top_slug = top,
        source = source))
}

def recent_queries(db : SqlRunner; n : int; misses_only : bool) : array<QueryLog> {
    if (misses_only) {
        var rows <- _sql(db |> select_from(type<QueryLog>)
                           |> _where(_.match_count == 0)
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
        if (length(similar) > 0) {
            outcome.similar <- similar
            outcome.created = false
            return <- outcome
        }
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
    outcome.slug = slug
    outcome.created = true
    outcome.written_path = path
    return <- outcome
}
