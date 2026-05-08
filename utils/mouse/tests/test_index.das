options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost public
require strings
require daslib/fio
require ../store.das
require ../index.das

// ─── helpers ─────────────────────────────────────────────────────────

def make_temp_root(t : T?) : string {
    let r = create_temp_directory_result("mouse_test")
    if (!(r is value)) {
        t |> failure("could not create temp dir: {unsafe(r.error)}")
        return ""
    }
    let root = unsafe(r.value)
    mkdir_rec(path_join(root, "docs"))
    return root
}

def cleanup_root(root : string) {
    if (!empty(root)) {
        rmdir_rec(root)
    }
}

def seed_doc(t : T?; root : string;
             slug : string; title : string; body : string;
             links : array<string>; questions : array<string>) {
    var fm = Frontmatter(
        slug = slug,
        title = title,
        created = "2026-05-08",
        last_verified = "2026-05-08")
    fm.links |> reserve(length(links))
    for (l in links) {
        fm.links |> push(l)
    }
    let composed = build_string() $(var w) {
        w |> write(body)
        if (!empty(body) && character_at(body, length(body) - 1) != '\n') {  // nolint:PERF003
            w |> write("\n")
        }
        if (length(questions) > 0) {
            w |> write("\n## Questions\n")
            for (q in questions) {
                w |> write("- {q}\n")
            }
        }
    }
    let path = path_join(path_join(root, "docs"), "{slug}.md")
    var err : string
    let ok = write_doc(path, fm, composed, err)
    t |> success(ok, "seed {slug}: {err}")
}

// ─── rebuild ─────────────────────────────────────────────────────────

[test]
def test_rebuild_empty(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) {
        return
    }
    var n = -1
    with_index_db(root) $(db) {
        n = rebuild(db, root)
    }
    t |> equal(n, 0)
    cleanup_root(root)
}

[test]
def test_rebuild_picks_up_files(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) {
        return
    }
    let no_links : array<string>
    let qs1 <- ["one a", "one b"]
    let qs2 <- ["two a"]
    let qs3 <- ["three a"]
    seed_doc(t, root, "alpha", "Alpha title", "alpha body", no_links, qs1)
    seed_doc(t, root, "beta",  "Beta title",  "beta body",  no_links, qs2)
    seed_doc(t, root, "gamma", "Gamma title", "gamma body", no_links, qs3)
    var n = -1
    var n_docs = -1
    with_index_db(root) $(db) {
        n = rebuild(db, root)
        let rows <- _sql(db |> select_from(type<DocRow>))
        n_docs = length(rows)
    }
    t |> equal(n, 3)
    t |> equal(n_docs, 3)
    cleanup_root(root)
}

[test]
def test_rebuild_idempotent(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) {
        return
    }
    let no_links : array<string>
    let qs <- ["q one"]
    seed_doc(t, root, "alpha", "Alpha title", "alpha body", no_links, qs)
    var n1 = -1
    var n2 = -1
    var doc_count_after = -1
    with_index_db(root) $(db) {
        n1 = rebuild(db, root)
        n2 = rebuild(db, root)
        let rows <- _sql(db |> select_from(type<DocRow>))
        doc_count_after = length(rows)
    }
    t |> equal(n1, 1)
    t |> equal(n2, 1)
    t |> equal(doc_count_after, 1)
    cleanup_root(root)
}

[test]
def test_rebuild_removes_deleted(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) {
        return
    }
    let no_links : array<string>
    let qs1 <- ["a"]
    let qs2 <- ["b"]
    seed_doc(t, root, "alpha", "Alpha", "alpha body", no_links, qs1)
    seed_doc(t, root, "beta",  "Beta",  "beta body",  no_links, qs2)
    var n_first = -1
    var n_after_delete = -1
    var has_alpha_after = false
    var has_beta_after = false
    with_index_db(root) $(db) {
        n_first = rebuild(db, root)
        // Delete beta from disk and rebuild.
        remove(path_join(path_join(root, "docs"), "beta.md"))
        n_after_delete = rebuild(db, root)
        let rows <- _sql(db |> select_from(type<DocRow>) |> _select(_.slug))
        for (s in rows) {
            if (s == "alpha") {
                has_alpha_after = true
            }
            if (s == "beta") {
                has_beta_after = true
            }
        }
    }
    t |> equal(n_first, 2)
    t |> equal(n_after_delete, 1)
    t |> success(has_alpha_after, "alpha kept")
    t |> success(!has_beta_after, "beta removed")
    cleanup_root(root)
}

// ─── search ──────────────────────────────────────────────────────────

[test]
def test_search_returns_top_match(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) {
        return
    }
    let no_links : array<string>
    let qs1 <- ["how do typefunction macros work"]
    let qs2 <- ["how do I install daspkg"]
    seed_doc(t, root, "doc-a", "Typefunction macros", "details about typefunction", no_links, qs1)
    seed_doc(t, root, "doc-b", "Daspkg install",      "details about daspkg",      no_links, qs2)
    var top_slug : string
    var n_results = 0
    with_index_db(root) $(db) {
        rebuild(db, root)
        let hits <- search(db, "typefunction", 5)
        n_results = length(hits)
        if (n_results > 0) {
            top_slug = hits[0].slug
        }
    }
    t |> success(n_results > 0, "non-empty results for 'typefunction'")
    t |> equal(top_slug, "doc-a")
    cleanup_root(root)
}

[test]
def test_search_empty_corpus(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) {
        return
    }
    var n_results = -1
    with_index_db(root) $(db) {
        rebuild(db, root)
        let hits <- search(db, "anything", 5)
        n_results = length(hits)
    }
    t |> equal(n_results, 0)
    cleanup_root(root)
}

[test]
def test_search_no_match(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) {
        return
    }
    let no_links : array<string>
    let qs <- ["the canonical question"]
    seed_doc(t, root, "doc-a", "Some title", "some body", no_links, qs)
    var n_results = -1
    with_index_db(root) $(db) {
        rebuild(db, root)
        let hits <- search(db, "zzqxxgibberish", 5)
        n_results = length(hits)
    }
    t |> equal(n_results, 0)
    cleanup_root(root)
}

[test]
def test_search_respects_k(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) {
        return
    }
    let no_links : array<string>
    for (i in range(5)) {
        let qs <- ["foo bar question {i}"]
        seed_doc(t, root, "doc-{i}", "Title foo {i}", "body foo {i}", no_links, qs)
    }
    var n_results = -1
    with_index_db(root) $(db) {
        rebuild(db, root)
        let hits <- search(db, "foo", 2)
        n_results = length(hits)
    }
    t |> equal(n_results, 2)
    cleanup_root(root)
}

// SQLite treats LIMIT -1 as "no limit", so a non-positive k must short-
// circuit before reaching the SQL layer.
[test]
def test_search_k_zero_returns_empty(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) {
        return
    }
    let no_links : array<string>
    let qs <- ["foo question"]
    seed_doc(t, root, "alpha", "Foo title", "foo body", no_links, qs)
    var n_zero = -1
    var n_neg = -1
    with_index_db(root) $(db) {
        rebuild(db, root)
        let h0 <- search(db, "foo", 0)
        n_zero = length(h0)
        let hn <- search(db, "foo", -3)
        n_neg = length(hn)
    }
    t |> equal(n_zero, 0)
    t |> equal(n_neg, 0)
    cleanup_root(root)
}

// Tabs in the query previously survived sanitize then sat inside a single
// `split(" ")` token, breaking matches. Verify a tab-separated query
// still matches a doc whose body contains the words.
[test]
def test_search_handles_tab_in_query(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) {
        return
    }
    let no_links : array<string>
    let qs <- ["sample"]
    seed_doc(t, root, "alpha", "Tab Match", "alpha gamma sample body", no_links, qs)
    var hits_count = -1
    var top_slug = ""
    with_index_db(root) $(db) {
        rebuild(db, root)
        let hits <- search(db, "alpha\tgamma", 5)
        hits_count = length(hits)
        if (length(hits) > 0) {
            top_slug = hits[0].slug
        }
    }
    t |> success(hits_count >= 1, "tab-separated query produced a hit")
    t |> equal(top_slug, "alpha")
    cleanup_root(root)
}

// ─── cross-refs ──────────────────────────────────────────────────────

[test]
def test_links_table_populated(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) {
        return
    }
    let links_a <- ["other-slug"]
    let no_q : array<string>
    seed_doc(t, root, "doc-a", "Title A", "body A", links_a, no_q)
    var links : array<tuple<from_slug : string; to_slug : string>>
    with_index_db(root) $(db) {
        rebuild(db, root)
        let rows <- _sql(db |> select_from(type<LinkRow>) |> _select((from_slug = _.from_slug, to_slug = _.to_slug)))
        for (r in rows) {
            links |> push(r)
        }
    }
    t |> equal(length(links), 1)
    if (length(links) >= 1) {
        t |> equal(links[0].from_slug, "doc-a")
        t |> equal(links[0].to_slug, "other-slug")
    }
    cleanup_root(root)
}

[test]
def test_reverse_index(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) {
        return
    }
    let links_a <- ["target"]
    let links_b <- ["target"]
    let no_q : array<string>
    seed_doc(t, root, "alpha", "A", "a body", links_a, no_q)
    seed_doc(t, root, "beta",  "B", "b body", links_b, no_q)
    var froms : array<string>
    with_index_db(root) $(db) {
        rebuild(db, root)
        froms <- linked_from(db, "target")
    }
    t |> equal(length(froms), 2)
    if (length(froms) >= 2) {
        t |> equal(froms[0], "alpha")
        t |> equal(froms[1], "beta")
    }
    cleanup_root(root)
}

// ─── dupe-on-add gate ────────────────────────────────────────────────

[test]
def test_dupe_check_finds_similar(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) {
        return
    }
    let no_links : array<string>
    let qs <- ["how to write typefunction macros"]
    seed_doc(t, root, "doc-a", "Writing typefunction macros", "details", no_links, qs)
    var n_similar = -1
    var top_slug : string
    with_index_db(root) $(db) {
        rebuild(db, root)
        let similar <- dupe_check(db, "typefunction", 5)
        n_similar = length(similar)
        if (n_similar > 0) {
            top_slug = similar[0].slug
        }
    }
    t |> success(n_similar > 0, "expected at least one match")
    t |> equal(top_slug, "doc-a")
    cleanup_root(root)
}

[test]
def test_dupe_check_no_match(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) {
        return
    }
    let no_links : array<string>
    let qs <- ["alpha question"]
    seed_doc(t, root, "doc-a", "Alpha", "alpha body", no_links, qs)
    var n_similar = -1
    with_index_db(root) $(db) {
        rebuild(db, root)
        let similar <- dupe_check(db, "zzqxxgibberish", 5)
        n_similar = length(similar)
    }
    t |> equal(n_similar, 0)
    cleanup_root(root)
}

[test]
def test_add_dupe_gate_no_force(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) {
        return
    }
    let no_links : array<string>
    let qs <- ["how to typefunction"]
    seed_doc(t, root, "doc-a", "Typefunction", "typefunction body", no_links, qs)
    var created = true
    var n_similar = -1
    var n_files_after = -1
    with_index_db(root) $(db) {
        rebuild(db, root)
        let outcome <- add_doc(db, root, "typefunction question paraphrase", "answer body", "", false)
        created = outcome.created
        n_similar = length(outcome.similar)
        let files <- list_doc_files(root)
        n_files_after = length(files)
    }
    t |> success(!created, "should not create when similar exists")
    t |> success(n_similar > 0, "should return at least one similar")
    t |> equal(n_files_after, 1)  // only doc-a, no new file written
    cleanup_root(root)
}

[test]
def test_add_doc_rejects_traversal_slug_hint(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) {
        return
    }
    var created = false
    var produced_slug : string
    var written_under_docs = false
    var n_files_after = -1
    with_index_db(root) $(db) {
        rebuild(db, root)
        let outcome <- add_doc(db, root, "Some Question", "answer body", "../escape", true)
        created = outcome.created
        produced_slug = outcome.slug
        let docs = docs_dir(root)
        written_under_docs = starts_with(outcome.written_path, docs)
        let files <- list_doc_files(root)
        n_files_after = length(files)
    }
    t |> success(created, "fail-soft: bad slug_hint should still create from title")
    t |> equal(produced_slug, "some-question")
    t |> success(written_under_docs, "written path stays under docs dir: got {produced_slug}")
    t |> equal(n_files_after, 1)
    cleanup_root(root)
}

// Newlines in `question` would otherwise split the YAML `title:` line and
// could inject sibling frontmatter keys.
[test]
def test_add_doc_sanitizes_question_newlines(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) {
        return
    }
    var written_path : string
    var produced_slug : string
    with_index_db(root) $(db) {
        rebuild(db, root)
        let injected = "Hello\nslug: hijacked"
        let outcome <- add_doc(db, root, injected, "answer", "", true)
        written_path = outcome.written_path
        produced_slug = outcome.slug
    }
    var doc : ParsedDoc
    var err : string
    let ok = read_doc(written_path, doc, err)
    t |> success(ok, "read back: {err}")
    t |> success(find(doc.frontmatter.title, "\n") < 0, "no newlines in title: {doc.frontmatter.title}")
    t |> success(produced_slug != "hijacked", "slug not hijacked: {produced_slug}")
    t |> success(doc.frontmatter.slug != "hijacked", "frontmatter slug not hijacked: {doc.frontmatter.slug}")
    cleanup_root(root)
}

[test]
def test_add_doc_rejects_blank_question(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) {
        return
    }
    var created = false
    var error : string
    var n_files_after = -1
    with_index_db(root) $(db) {
        rebuild(db, root)
        let outcome <- add_doc(db, root, "  \n\r  ", "answer", "", true)
        created = outcome.created
        error = outcome.error
        let files <- list_doc_files(root)
        n_files_after = length(files)
    }
    t |> success(!created, "must not create with blank question")
    t |> success(!empty(error), "error message populated: {error}")
    t |> equal(n_files_after, 0)
    cleanup_root(root)
}

// A doc accidentally listing the same link twice in frontmatter must not
// abort the rebuild transaction on UNIQUE(from_slug, to_slug).
[test]
def test_rebuild_dedupes_duplicate_links(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) {
        return
    }
    let dup_links <- ["target", "target", "target"]
    let no_q : array<string>
    seed_doc(t, root, "alpha", "Alpha", "alpha body", dup_links, no_q)
    var n_links_after = -1
    var n_docs = -1
    with_index_db(root) $(db) {
        rebuild(db, root)
        let link_rows <- _sql(db |> select_from(type<LinkRow>))
        n_links_after = length(link_rows)
        let doc_rows <- _sql(db |> select_from(type<DocRow>))
        n_docs = length(doc_rows)
    }
    t |> equal(n_links_after, 1)
    t |> equal(n_docs, 1)  // doc itself still indexed
    cleanup_root(root)
}

// A hand-edited `.md` with an invalid frontmatter slug (uppercase, dot,
// leading underscore, etc.) would otherwise be indexed but unretrievable
// via cmd_get/tool_get since they enforce is_valid_slug. Skip+warn so
// search results never point at unfetchable docs.
[test]
def test_rebuild_skips_invalid_frontmatter_slug(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) {
        return
    }
    let no_links : array<string>
    let no_q : array<string>
    seed_doc(t, root, "good", "Good", "good body", no_links, no_q)
    let fm = Frontmatter(
        slug = "Bad.Slug",
        title = "Bad",
        created = "2026-05-08",
        last_verified = "2026-05-08")
    var werr : string
    let bad_path = path_join(path_join(root, "docs"), "bad.md")
    let wrote = write_doc(bad_path, fm, "bad body\n", werr)
    t |> success(wrote, "seed bad slug: {werr}")
    var n = -1
    var slugs : array<string>
    with_index_db(root) $(db) {
        n = rebuild(db, root)
        let rows <- _sql(db |> select_from(type<DocRow>) |> _select(_.slug))
        for (s in rows) {
            slugs |> push(s)
        }
    }
    t |> equal(n, 1)  // only `good` indexed; bad-slug doc skipped
    t |> equal(length(slugs), 1)
    if (length(slugs) >= 1) {
        t |> equal(slugs[0], "good")
    }
    cleanup_root(root)
}

[test]
def test_add_doc_accepts_valid_slug_hint(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) {
        return
    }
    var produced_slug : string
    with_index_db(root) $(db) {
        rebuild(db, root)
        let outcome <- add_doc(db, root, "Some Question", "answer body", "my-slug", true)
        produced_slug = outcome.slug
    }
    t |> equal(produced_slug, "my-slug")
    cleanup_root(root)
}

// ─── query log ───────────────────────────────────────────────────────

[test]
def test_log_query_records_hit(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) {
        return
    }
    let no_links : array<string>
    let qs <- ["how to typefunction"]
    seed_doc(t, root, "alpha", "Typefunction macros", "alpha body", no_links, qs)
    var rows : array<QueryLog>
    with_index_db(root) $(db) {
        rebuild(db, root)
        let hits <- search(db, "typefunction", 5)
        log_query(db, "typefunction", hits, "cli")
        rows <- _sql(db |> select_from(type<QueryLog>))
    }
    t |> equal(length(rows), 1)
    if (length(rows) >= 1) {
        t |> equal(rows[0].question, "typefunction")
        t |> equal(rows[0].source, "cli")
        t |> success(rows[0].match_count >= 1, "match_count populated")
        t |> equal(rows[0].top_slug, "alpha")
        t |> success(!empty(rows[0].asked_at), "asked_at populated")
    }
    cleanup_root(root)
}

[test]
def test_log_query_records_miss(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) {
        return
    }
    let no_links : array<string>
    let qs <- ["alpha question"]
    seed_doc(t, root, "alpha", "Alpha", "alpha body", no_links, qs)
    var rows : array<QueryLog>
    with_index_db(root) $(db) {
        rebuild(db, root)
        let hits <- search(db, "zzqxxgibberish", 5)
        log_query(db, "zzqxxgibberish", hits, "mcp")
        rows <- _sql(db |> select_from(type<QueryLog>))
    }
    t |> equal(length(rows), 1)
    if (length(rows) >= 1) {
        t |> equal(rows[0].match_count, 0)
        t |> equal(rows[0].top_slug, "")
        t |> equal(rows[0].source, "mcp")
    }
    cleanup_root(root)
}

[test]
def test_rebuild_preserves_log(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) {
        return
    }
    let no_links : array<string>
    let qs <- ["alpha question"]
    seed_doc(t, root, "alpha", "Alpha", "alpha body", no_links, qs)
    var n_before = -1
    var n_after = -1
    with_index_db(root) $(db) {
        rebuild(db, root)
        let hits <- search(db, "alpha", 5)
        log_query(db, "alpha", hits, "cli")
        let rows1 <- _sql(db |> select_from(type<QueryLog>))
        n_before = length(rows1)
        rebuild(db, root)
        let rows2 <- _sql(db |> select_from(type<QueryLog>))
        n_after = length(rows2)
    }
    t |> equal(n_before, 1)
    t |> equal(n_after, 1)
    cleanup_root(root)
}

[test]
def test_recent_queries_filters_misses(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) {
        return
    }
    let no_links : array<string>
    let qs <- ["alpha question"]
    seed_doc(t, root, "alpha", "Alpha", "alpha body", no_links, qs)
    var n_all = -1
    var n_misses = -1
    with_index_db(root) $(db) {
        rebuild(db, root)
        let h1 <- search(db, "alpha", 5)
        log_query(db, "alpha", h1, "cli")
        let h2 <- search(db, "zzqxxgibberish", 5)
        log_query(db, "zzqxxgibberish", h2, "cli")
        let all_rows <- recent_queries(db, 10, false)
        let miss_rows <- recent_queries(db, 10, true)
        n_all = length(all_rows)
        n_misses = length(miss_rows)
    }
    t |> equal(n_all, 2)
    t |> equal(n_misses, 1)
    cleanup_root(root)
}

[test]
def test_add_dupe_gate_force(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) {
        return
    }
    let no_links : array<string>
    let qs <- ["how to typefunction"]
    seed_doc(t, root, "doc-a", "Typefunction", "typefunction body", no_links, qs)
    var created = false
    var n_files_after = -1
    var found_via_search = false
    with_index_db(root) $(db) {
        rebuild(db, root)
        let outcome <- add_doc(db, root, "completely different question xyz", "new answer body", "", true)
        created = outcome.created
        let files <- list_doc_files(root)
        n_files_after = length(files)
        let hits <- search(db, "completely different xyz", 5)
        for (h in hits) {
            if (h.slug == outcome.slug) {
                found_via_search = true
            }
        }
    }
    t |> success(created, "should create with force=true")
    t |> equal(n_files_after, 2)
    t |> success(found_via_search, "newly added doc retrievable via search")
    cleanup_root(root)
}
