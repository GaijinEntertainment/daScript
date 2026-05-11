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
        if (!empty(questions)) {
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
    if (empty(root)) return
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
    if (empty(root)) return
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
    if (empty(root)) return
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
    if (empty(root)) return
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
    if (empty(root)) return
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
    if (empty(root)) return
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
    if (empty(root)) return
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
    if (empty(root)) return
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
    if (empty(root)) return
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
    if (empty(root)) return
    let no_links : array<string>
    let qs <- ["sample"]
    seed_doc(t, root, "alpha", "Tab Match", "alpha gamma sample body", no_links, qs)
    var hits_count = -1
    var top_slug = ""
    with_index_db(root) $(db) {
        rebuild(db, root)
        let hits <- search(db, "alpha\tgamma", 5)
        hits_count = length(hits)
        if (!empty(hits)) {
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
    if (empty(root)) return
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
    if (!empty(links)) {
        t |> equal(links[0].from_slug, "doc-a")
        t |> equal(links[0].to_slug, "other-slug")
    }
    cleanup_root(root)
}

[test]
def test_reverse_index(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) return
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
    if (empty(root)) return
    let no_links : array<string>
    let qs <- ["how to write typefunction macros"]
    seed_doc(t, root, "doc-a", "Writing typefunction macros", "details", no_links, qs)
    var n_similar = -1
    var top_slug : string
    var top_sim = 0.0f
    with_index_db(root) $(db) {
        rebuild(db, root)
        let similar <- dupe_check(db, "typefunction", 5)
        n_similar = length(similar)
        if (n_similar > 0) {
            top_slug = similar[0].slug
            top_sim = similar[0].similarity
        }
    }
    t |> success(n_similar > 0, "expected at least one match")
    t |> equal(top_slug, "doc-a")
    // single rare keyword in a 3-word title => Jaccard = 1/3 ≈ 0.33
    t |> success(top_sim >= 0.3f, "similarity should be set: got {top_sim}")
    cleanup_root(root)
}

[test]
def test_dupe_check_no_match(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) return
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
def test_add_dupe_gate_blocks_high_similarity(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) return
    let no_links : array<string>
    let qs <- ["how to write typefunction macros"]
    // Title and incoming question share 5 of 6 content tokens after stop-word
    // filtering ⇒ Jaccard ≈ 0.83, well above the 0.7 hard-block threshold.
    seed_doc(t, root, "doc-a", "How to write typefunction macros", "body", no_links, qs)
    var created = true
    var n_similar = -1
    var top_sim = 0.0f
    var n_files_after = -1
    with_index_db(root) $(db) {
        rebuild(db, root)
        let outcome <- add_doc(db, root, "how to write typefunction macros for daslang", "answer body", "", false)
        created = outcome.created
        n_similar = length(outcome.similar)
        if (n_similar > 0) {
            top_sim = outcome.similar[0].similarity
        }
        let files <- list_doc_files(root)
        n_files_after = length(files)
    }
    t |> success(!created, "high-similarity dupe should block create")
    t |> success(n_similar > 0, "should return the similar doc")
    t |> success(top_sim >= 0.7f, "top similarity above hard block: got {top_sim}")
    t |> equal(n_files_after, 1)  // only doc-a; no new file written
    cleanup_root(root)
}

[test]
def test_add_dupe_gate_allows_low_similarity(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) return
    let no_links : array<string>
    let qs <- ["how to typefunction"]
    seed_doc(t, root, "doc-a", "Typefunction", "typefunction body", no_links, qs)
    var created = false
    var n_similar = -1
    var top_sim = -1.0f
    var n_files_after = -1
    with_index_db(root) $(db) {
        rebuild(db, root)
        // "typefunction question paraphrase" vs title "Typefunction" =>
        // Jaccard 1/3 ≈ 0.33, BELOW the 0.7 hard block. Add proceeds.
        let outcome <- add_doc(db, root, "typefunction question paraphrase", "answer body", "", false)
        created = outcome.created
        n_similar = length(outcome.similar)
        if (n_similar > 0) {
            top_sim = outcome.similar[0].similarity
        }
        let files <- list_doc_files(root)
        n_files_after = length(files)
    }
    t |> success(created, "low-similarity match should NOT block; add must proceed")
    t |> success(n_similar > 0, "advisory similar list still returned")
    t |> success(top_sim < 0.7f, "top similarity below hard block: got {top_sim}")
    t |> equal(n_files_after, 2)  // doc-a + the new doc
    cleanup_root(root)
}

// Common glue words ("how", "do", "I") shouldn't drag rare-content queries
// across the threshold. After stop-word filtering, the rare-token overlap
// here is empty.
[test]
def test_dupe_check_jaccard_filters_common_words(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) return
    let no_links : array<string>
    let qs <- ["how to write tests"]
    seed_doc(t, root, "doc-a", "how do I write tests", "body", no_links, qs)
    var n_similar = -1
    var top_sim = 0.0f
    with_index_db(root) $(db) {
        rebuild(db, root)
        let similar <- dupe_check(db, "how do I add git tracking", 5)
        n_similar = length(similar)
        if (n_similar > 0) {
            top_sim = similar[0].similarity
        }
    }
    // Even if FTS5 returns the doc (matches "how"/"do"), the Jaccard score
    // after stop-word filter is 0 — and dupe_check drops zero-similarity
    // hits — so n_similar may be 0. Either way, we must not look like a
    // duplicate.
    t |> success(top_sim < 0.2f, "common-word overlap should not look like a duplicate: n_similar={n_similar}, top_sim={top_sim}")
    cleanup_root(root)
}

[test]
def test_add_doc_rejects_traversal_slug_hint(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) return
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
    if (empty(root)) return
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
    if (empty(root)) return
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
    if (empty(root)) return
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

// `cmd_get`/`tool_get` build the path as `<docs>/{slug}.md`, never reading
// `DocRow.path`. A file whose basename diverges from its frontmatter slug
// (rename without re-frontmatter, or hand-edited slug) would be searchable
// but unfetchable. Skip+warn during rebuild so search results never point
// at unfetchable docs.
[test]
def test_rebuild_skips_basename_slug_mismatch(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) return
    let no_links : array<string>
    let no_q : array<string>
    seed_doc(t, root, "good", "Good", "good body", no_links, no_q)
    // Frontmatter slug `actual-slug` written into a file named `weird.md`.
    let fm = Frontmatter(
        slug = "actual-slug",
        title = "Mismatch",
        created = "2026-05-08",
        last_verified = "2026-05-08")
    var werr : string
    let weird_path = path_join(path_join(root, "docs"), "weird.md")
    let wrote = write_doc(weird_path, fm, "weird body\n", werr)
    t |> success(wrote, "seed mismatched basename: {werr}")
    var n = -1
    var slugs : array<string>
    with_index_db(root) $(db) {
        n = rebuild(db, root)
        let rows <- _sql(db |> select_from(type<DocRow>) |> _select(_.slug))
        for (s in rows) {
            slugs |> push(s)
        }
    }
    t |> equal(n, 1)  // only `good`; mismatched-basename doc skipped
    t |> equal(length(slugs), 1)
    if (!empty(slugs)) {
        t |> equal(slugs[0], "good")
    }
    cleanup_root(root)
}

// FTS5 keywords (OR/AND/NOT/NEAR) are uppercase-only operators, and tokens
// are OR-joined raw (no quoting). `foo OR bar` would tokenize to
// [foo, OR, bar] and join to `foo OR OR OR bar` — invalid syntax, caught,
// empty result. Downcasing every token makes user-typed keywords inert.
[test]
def test_search_handles_uppercase_fts_keywords(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) return
    let no_links : array<string>
    let qs <- ["alpha"]
    seed_doc(t, root, "alpha", "Alpha title", "alpha bravo body", no_links, qs)
    var hits_count = -1
    var top_slug = ""
    with_index_db(root) $(db) {
        rebuild(db, root)
        let hits <- search(db, "alpha OR bravo", 5)
        hits_count = length(hits)
        if (!empty(hits)) {
            top_slug = hits[0].slug
        }
    }
    t |> success(hits_count >= 1, "uppercase OR token must not break query")
    t |> equal(top_slug, "alpha")
    cleanup_root(root)
}

// A hand-edited `.md` with an invalid frontmatter slug (uppercase, dot,
// leading underscore, etc.) would otherwise be indexed but unretrievable
// via cmd_get/tool_get since they enforce is_valid_slug. Skip+warn so
// search results never point at unfetchable docs.
[test]
def test_rebuild_skips_invalid_frontmatter_slug(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) return
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
    if (!empty(slugs)) {
        t |> equal(slugs[0], "good")
    }
    cleanup_root(root)
}

[test]
def test_add_doc_accepts_valid_slug_hint(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) return
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
    if (empty(root)) return
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
    if (!empty(rows)) {
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
    if (empty(root)) return
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
    if (!empty(rows)) {
        t |> equal(rows[0].match_count, 0)
        t |> equal(rows[0].top_slug, "")
        t |> equal(rows[0].source, "mcp")
    }
    cleanup_root(root)
}

[test]
def test_rebuild_preserves_log(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) return
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
    if (empty(root)) return
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
        let all_rows <- recent_queries(db, 10, LogMode.All)
        let miss_rows <- recent_queries(db, 10, LogMode.Misses)
        n_all = length(all_rows)
        n_misses = length(miss_rows)
    }
    t |> equal(n_all, 2)
    t |> equal(n_misses, 1)
    cleanup_root(root)
}

// ─── useful flag (mark_no_match + Bad/Review filters) ────────────────

[test]
def test_log_query_returns_increasing_ids(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) return
    let no_links : array<string>
    let qs <- ["alpha question"]
    seed_doc(t, root, "alpha", "Alpha", "alpha body", no_links, qs)
    var id1 = -1l
    var id2 = -1l
    with_index_db(root) $(db) {
        rebuild(db, root)
        let h1 <- search(db, "alpha", 5)
        id1 = log_query(db, "alpha", h1, "cli")
        let h2 <- search(db, "alpha", 5)
        id2 = log_query(db, "alpha", h2, "cli")
    }
    t |> success(id1 > 0l, "first id positive: {id1}")
    t |> success(id2 > id1, "second id strictly greater than first: {id1} -> {id2}")
    cleanup_root(root)
}

[test]
def test_mark_no_match_sets_useful_zero(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) return
    let no_links : array<string>
    let qs <- ["alpha question"]
    seed_doc(t, root, "alpha", "Alpha", "alpha body", no_links, qs)
    var id = -1l
    var n_marked = -1
    var useful_before : Option<int>
    var useful_after : Option<int>
    with_index_db(root) $(db) {
        rebuild(db, root)
        let h1 <- search(db, "alpha", 5)
        id = log_query(db, "alpha", h1, "cli")
        let row_before <- _sql(db |> select_from(type<QueryLog>) |> _where(_.id == id) |> _first_opt())
        useful_before = (row_before |> unwrap).useful
        n_marked = mark_no_match(db, id)
        let row_after <- _sql(db |> select_from(type<QueryLog>) |> _where(_.id == id) |> _first_opt())
        useful_after = (row_after |> unwrap).useful
    }
    t |> success(useful_before |> is_none, "useful starts NULL on insert")
    t |> equal(n_marked, 1)
    t |> success(useful_after |> is_some, "useful is non-NULL after mark")
    t |> equal(useful_after |> unwrap_or(-1), 0)
    cleanup_root(root)
}

[test]
def test_mark_no_match_nonexistent_id_returns_zero(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) return
    var n_marked = -1
    with_index_db(root) $(db) {
        rebuild(db, root)
        n_marked = mark_no_match(db, 99999l)
    }
    t |> equal(n_marked, 0)
    cleanup_root(root)
}

[test]
def test_mark_no_match_idempotent(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) return
    let no_links : array<string>
    let qs <- ["alpha question"]
    seed_doc(t, root, "alpha", "Alpha", "alpha body", no_links, qs)
    var n1 = -1
    var n2 = -1
    var useful_after : Option<int>
    with_index_db(root) $(db) {
        rebuild(db, root)
        let h1 <- search(db, "alpha", 5)
        let id = log_query(db, "alpha", h1, "cli")
        n1 = mark_no_match(db, id)
        n2 = mark_no_match(db, id)
        let row <- _sql(db |> select_from(type<QueryLog>) |> _where(_.id == id) |> _first_opt())
        useful_after = (row |> unwrap).useful
    }
    t |> equal(n1, 1)
    t |> equal(n2, 1)  // SQLite UPDATE matches the row even if SET to same value
    t |> equal(useful_after |> unwrap_or(-1), 0)
    cleanup_root(root)
}

[test]
def test_recent_queries_filters_bad(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) return
    let no_links : array<string>
    let qs <- ["alpha question"]
    seed_doc(t, root, "alpha", "Alpha", "alpha body", no_links, qs)
    var n_bad = -1
    var n_review = -1
    with_index_db(root) $(db) {
        rebuild(db, root)
        // 3 hit rows: mark one bad, leave two unrated → 1 bad, 2 review.
        let h1 <- search(db, "alpha", 5)
        let id1 = log_query(db, "alpha", h1, "cli")
        let h2 <- search(db, "alpha", 5)
        log_query(db, "alpha 2", h2, "cli")
        let h3 <- search(db, "alpha", 5)
        log_query(db, "alpha 3", h3, "cli")
        // 1 miss row: should NOT show up under bad or review (match_count == 0).
        let h_miss <- search(db, "zzqxxgibberish", 5)
        log_query(db, "zzqxxgibberish", h_miss, "cli")
        mark_no_match(db, id1)
        let bad_rows <- recent_queries(db, 10, LogMode.Bad)
        let review_rows <- recent_queries(db, 10, LogMode.Review)
        n_bad = length(bad_rows)
        n_review = length(review_rows)
    }
    t |> equal(n_bad, 1)
    t |> equal(n_review, 2)
    cleanup_root(root)
}

[test]
def test_recent_queries_review_excludes_misses(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) return
    let no_links : array<string>
    let qs <- ["alpha question"]
    seed_doc(t, root, "alpha", "Alpha", "alpha body", no_links, qs)
    var n_review = -1
    with_index_db(root) $(db) {
        rebuild(db, root)
        // Only misses → review queue must be empty (Review needs match_count > 0).
        let h_miss <- search(db, "zzqxxgibberish", 5)
        log_query(db, "zzqxxgibberish", h_miss, "cli")
        let h_miss2 <- search(db, "another gibberish", 5)
        log_query(db, "another gibberish", h_miss2, "cli")
        let review_rows <- recent_queries(db, 10, LogMode.Review)
        n_review = length(review_rows)
    }
    t |> equal(n_review, 0)
    cleanup_root(root)
}

[test]
def test_add_dupe_gate_force(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) return
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

// ─── auto-reindex via signature ───────────────────────────────────────

[test]
def test_ensure_index_fresh_picks_up_new_md(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) return
    let no_links : array<string>
    let qs1 <- ["alpha question"]
    seed_doc(t, root, "alpha", "Alpha title", "alpha body", no_links, qs1)
    var n_initial = -1
    var n_after = -1
    var found = false
    with_index_db(root) $(db) {
        ensure_index_fresh(db, root)
        let rows1 <- _sql(db |> select_from(type<DocRow>))
        n_initial = length(rows1)
        // Drop a new .md straight onto disk, simulating a `git pull`.
        let qs2 <- ["beta question"]
        seed_doc(t, root, "beta", "Beta title", "beta body", no_links, qs2)
        ensure_index_fresh(db, root)
        let rows2 <- _sql(db |> select_from(type<DocRow>))
        n_after = length(rows2)
        let hits <- search(db, "beta", 5)
        for (h in hits) {
            if (h.slug == "beta") {
                found = true
            }
        }
    }
    t |> equal(n_initial, 1)
    t |> equal(n_after, 2)
    t |> success(found, "newly-dropped doc must be retrievable after ensure_index_fresh")
    cleanup_root(root)
}

[test]
def test_ensure_index_fresh_noop_when_unchanged(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) return
    let no_links : array<string>
    let qs <- ["alpha q"]
    seed_doc(t, root, "alpha", "Alpha", "body", no_links, qs)
    var sig1 : string
    var sig2 : string
    var rebuild_count = -1
    with_index_db(root) $(db) {
        ensure_index_fresh(db, root)
        sig1 = get_index_signature(db)
        // No filesystem changes — second call should hit the cache and
        // return 0, AND the persisted signature should still equal the first.
        rebuild_count = ensure_index_fresh(db, root)
        sig2 = get_index_signature(db)
    }
    t |> success(!empty(sig1), "signature populated on first ensure")
    t |> equal(sig1, sig2)
    t |> equal(rebuild_count, 0)  // 0 = cache hit, no rebuild
    cleanup_root(root)
}

// ─── --raw-query FTS5 passthrough ─────────────────────────────────────

[test]
def test_search_raw_query(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) return
    let no_links : array<string>
    let qs1 <- ["title alpha question"]
    let qs2 <- ["title bravo question"]
    seed_doc(t, root, "alpha", "alphakeyword content", "alpha body", no_links, qs1)
    seed_doc(t, root, "bravo", "bravokeyword content", "bravo body", no_links, qs2)
    var raw_n = 0
    var sanitized_n = 0
    with_index_db(root) $(db) {
        rebuild(db, root)
        // Raw-query path: explicit FTS5 OR — both docs match.
        let raw_hits <- search(db, "alphakeyword OR bravokeyword", 10, true)
        raw_n = length(raw_hits)
        // Sanitized path lowercases "OR" to "or" (a 2-char regular token);
        // it gets joined with the keywords via OR, so still matches both.
        // What we really want to assert is that the raw path works; the
        // sanitized path's exact behavior is tested elsewhere.
        let san_hits <- search(db, "alphakeyword OR bravokeyword", 10, false)
        sanitized_n = length(san_hits)
    }
    t |> equal(raw_n, 2, "raw FTS5 OR returns both docs")
    t |> success(sanitized_n >= 2, "sanitized path also returns both via OR-join: got {sanitized_n}")
    cleanup_root(root)
}

[test]
def test_search_raw_query_phrase(t : T?) {
    let root = make_temp_root(t)
    if (empty(root)) return
    let no_links : array<string>
    let qs <- ["the alpha bravo charlie sequence"]
    seed_doc(t, root, "alpha", "alpha bravo charlie", "the order matters: alpha bravo charlie", no_links, qs)
    var quoted_hits = -1
    with_index_db(root) $(db) {
        rebuild(db, root)
        // Phrase queries are valuable raw-query users. Sanitizer would strip
        // the quotes — raw_query=true preserves them.
        let hits <- search(db, "\"alpha bravo\"", 10, true)
        quoted_hits = length(hits)
    }
    t |> equal(quoted_hits, 1)
    cleanup_root(root)
}
