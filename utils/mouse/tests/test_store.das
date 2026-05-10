options gen2
options no_unused_function_arguments = false

require dastest/testing_boost public
require strings
require daslib/fio
require ../store.das


def fixture_path(name : string) : string {
    return "utils/mouse/tests/fixtures/{name}"
}

def load_fixture(t : T?; name : string) : ParsedDoc {
    var doc : ParsedDoc
    var error : string
    let ok = read_doc(fixture_path(name), doc, error)
    t |> success(ok, "fixture {name} loads: {error}")
    return <- doc
}

// ─── frontmatter parsing ──────────────────────────────────────────────

[test]
def test_parse_frontmatter_full(t : T?) {
    let doc <- load_fixture(t, "sample-doc.md")
    t |> equal(doc.frontmatter.slug, "sample-doc")
    t |> equal(doc.frontmatter.title, "Canonical full doc fixture")
    t |> equal(doc.frontmatter.created, "2026-05-08")
    t |> equal(doc.frontmatter.last_verified, "2026-05-08")
    t |> equal(length(doc.frontmatter.links), 2)
    if (length(doc.frontmatter.links) >= 2) {
        t |> equal(doc.frontmatter.links[0], "other-slug")
        t |> equal(doc.frontmatter.links[1], "another-slug")
    }
    t |> success(empty(doc.parse_error), "no parse error: {doc.parse_error}")
}

[test]
def test_parse_frontmatter_minimal(t : T?) {
    let doc <- load_fixture(t, "sample-doc-minimal.md")
    t |> equal(doc.frontmatter.slug, "sample-doc-minimal")
    t |> equal(doc.frontmatter.title, "Minimal frontmatter fixture")
    t |> equal(doc.frontmatter.created, "")
    t |> equal(doc.frontmatter.last_verified, "")
    t |> equal(length(doc.frontmatter.links), 0)
    t |> success(empty(doc.parse_error), "no parse error: {doc.parse_error}")
}

[test]
def test_parse_frontmatter_missing_fence(t : T?) {
    let doc <- load_fixture(t, "no-frontmatter.md")
    t |> equal(doc.frontmatter.slug, "")
    t |> equal(doc.frontmatter.title, "")
    t |> success(empty(doc.parse_error), "no fence is not an error: {doc.parse_error}")
    // Body should contain the whole file content.
    t |> success(find(doc.body, "no frontmatter at all") >= 0, "body has the file content")
}

[test]
def test_parse_frontmatter_malformed(t : T?) {
    let doc <- load_fixture(t, "malformed-frontmatter.md")
    t |> success(!empty(doc.parse_error), "missing closing fence sets parse_error")
    // Per locked-in behavior: body is the whole text on parse error
    t |> success(find(doc.body, "Frontmatter without closing fence") >= 0, "body contains the original text on error")
}

[test]
def test_parse_frontmatter_inline_links(t : T?) {
    let text = "---\nslug: x\nlinks: [a, b, c]\n---\nbody"
    let doc <- parse_doc_text(text)
    t |> equal(length(doc.frontmatter.links), 3)
    if (length(doc.frontmatter.links) >= 3) {
        t |> equal(doc.frontmatter.links[0], "a")
        t |> equal(doc.frontmatter.links[1], "b")
        t |> equal(doc.frontmatter.links[2], "c")
    }
}

[test]
def test_parse_frontmatter_empty_inline_links(t : T?) {
    let text = "---\nslug: x\nlinks: []\n---\nbody"
    let doc <- parse_doc_text(text)
    t |> equal(length(doc.frontmatter.links), 0)
}

// ─── ## Questions section ─────────────────────────────────────────────

[test]
def test_parse_questions_section(t : T?) {
    let doc <- load_fixture(t, "sample-doc.md")
    t |> equal(length(doc.questions), 3)
    if (length(doc.questions) >= 3) {
        t |> equal(doc.questions[0], "How do I do the canonical thing?")
        t |> equal(doc.questions[1], "canonical fixture")
        t |> equal(doc.questions[2], "sample doc question")
    }
}

[test]
def test_parse_questions_missing(t : T?) {
    let doc <- load_fixture(t, "sample-doc-minimal.md")
    t |> equal(length(doc.questions), 0)
}

[test]
def test_parse_questions_stops_at_next_heading(t : T?) {
    let text = "## Questions\n- one\n- two\n## Other\n- three\n"
    let qs <- parse_questions(text)
    t |> equal(length(qs), 2)
    if (length(qs) >= 2) {
        t |> equal(qs[0], "one")
        t |> equal(qs[1], "two")
    }
}

// ─── slug generation ─────────────────────────────────────────────────

[test]
def test_slug_from_title_basic(t : T?) {
    t |> equal(slug_from_title("How do I X?"), "how-do-i-x")
    t |> equal(slug_from_title("What's the pattern for foo?"), "what-s-the-pattern-for-foo")
    t |> equal(slug_from_title("UPPERCASE"), "uppercase")
    t |> equal(slug_from_title("multi   spaces"), "multi-spaces")
    t |> equal(slug_from_title("trim-trailing-"), "trim-trailing")
    t |> equal(slug_from_title("-leading"), "leading")
}

[test]
def test_slug_from_title_empty_returns_empty(t : T?) {
    t |> equal(slug_from_title(""), "")
    t |> equal(slug_from_title("???"), "")
    t |> equal(slug_from_title("   "), "")
}

[test]
def test_slug_dedupe_collision(t : T?) {
    var existing : table<string>
    existing |> insert("foo")
    existing |> insert("foo-2")
    let s = slug_from_title_dedupe("Foo", existing)
    t |> equal(s, "foo-3")
}

[test]
def test_slug_dedupe_no_collision(t : T?) {
    let existing : table<string>
    let s = slug_from_title_dedupe("Bar", existing)
    t |> equal(s, "bar")
}

[test]
def test_dedupe_slug_preserves_underscore(t : T?) {
    var existing : table<string>
    existing |> insert("my_doc")
    let s = dedupe_slug("my_doc", existing)
    t |> equal(s, "my_doc-2")
}

[test]
def test_dedupe_slug_no_collision_returns_verbatim(t : T?) {
    let existing : table<string>
    let s = dedupe_slug("any-shape_keeps", existing)
    t |> equal(s, "any-shape_keeps")
}

// Long titles otherwise produce slugs that `is_valid_slug` rejects, making
// the doc unfetchable via cmd_get/tool_get.
[test]
def test_slug_from_title_caps_at_max_len(t : T?) {
    let s = slug_from_title(repeat("a", MAX_SLUG_LEN * 2))
    t |> equal(length(s), MAX_SLUG_LEN)
    t |> success(is_valid_slug(s), "capped slug passes is_valid_slug")
}

// If the cap lands on a `-` (dash inserted between word boundaries), the
// trailing dash must be trimmed so the slug doesn't end with `-`.
[test]
def test_slug_from_title_cap_trims_trailing_dash(t : T?) {
    // "aaa...(MAX-1) bbb...(200)" → raw has `-` exactly at position MAX-1.
    let prefix = repeat("a", MAX_SLUG_LEN - 1)
    let suffix = repeat("b", 200)
    let title = "{prefix} {suffix}"
    let s = slug_from_title(title)
    t |> success(length(s) <= MAX_SLUG_LEN, "len {length(s)} <= {MAX_SLUG_LEN}")
    t |> success(!ends_with(s, "-"), "no trailing dash after cap: {s}")
}

// On collision, base must be capped enough to leave room for `-N` suffix
// so candidate stays within MAX_SLUG_LEN.
[test]
def test_dedupe_slug_caps_long_base_when_suffixing(t : T?) {
    let huge = repeat("a", MAX_SLUG_LEN)
    let huge_capped = repeat("a", MAX_SLUG_LEN - 6)
    var existing : table<string>
    existing |> insert(huge)
    existing |> insert(huge_capped)
    let s = dedupe_slug(huge, existing)
    t |> success(length(s) <= MAX_SLUG_LEN, "deduped len {length(s)} <= {MAX_SLUG_LEN}")
    t |> success(is_valid_slug(s), "deduped slug passes is_valid_slug: {s}")
    t |> equal(s, "{huge_capped}-2")
}

// ─── question sanitization ──────────────────────────────────────────

[test]
def test_sanitize_question_replaces_newlines(t : T?) {
    t |> equal(sanitize_question("hello\nworld"), "hello world")
    t |> equal(sanitize_question("a\r\nb"), "a  b")
    t |> equal(sanitize_question("  trim\nme  "), "trim me")
}

[test]
def test_sanitize_question_blank_returns_empty(t : T?) {
    t |> equal(sanitize_question("   \n\r  "), "")
    t |> equal(sanitize_question(""), "")
}

// ─── slug validation ─────────────────────────────────────────────────

[test]
def test_is_valid_slug_accepts(t : T?) {
    t |> success(is_valid_slug("alpha"), "alpha")
    t |> success(is_valid_slug("alpha-beta"), "alpha-beta")
    t |> success(is_valid_slug("a_b"), "a_b")
    t |> success(is_valid_slug("a1"), "a1")
    t |> success(is_valid_slug("why-darken-bd"), "why-darken-bd")
    t |> success(is_valid_slug("0starts-with-digit"), "digit-start")
}

[test]
def test_is_valid_slug_rejects(t : T?) {
    t |> success(!is_valid_slug(""), "empty")
    t |> success(!is_valid_slug("../etc"), "path traversal")
    t |> success(!is_valid_slug("foo/bar"), "forward slash")
    t |> success(!is_valid_slug("foo\\bar"), "back slash")
    t |> success(!is_valid_slug("-leading"), "leading dash")
    t |> success(!is_valid_slug("_leading"), "leading underscore")
    t |> success(!is_valid_slug("foo.md"), "contains dot")
    t |> success(!is_valid_slug("foo bar"), "space")
    t |> success(!is_valid_slug("FOO"), "uppercase")
    let huge = repeat("a", 200)
    t |> success(!is_valid_slug(huge), "over 128 chars")
}

// ─── slug normalization (input leniency) ─────────────────────────────

[test]
def test_normalize_slug_input_passthrough(t : T?) {
    t |> equal(normalize_slug_input("foo"), "foo")
    t |> equal(normalize_slug_input("alpha-beta_gamma"), "alpha-beta_gamma")
}

[test]
def test_normalize_slug_input_strips_md(t : T?) {
    t |> equal(normalize_slug_input("foo.md"), "foo")
    t |> equal(normalize_slug_input("alpha-beta.md"), "alpha-beta")
}

[test]
def test_normalize_slug_input_strips_path(t : T?) {
    t |> equal(normalize_slug_input("./mouse-data/docs/foo.md"), "foo")
    t |> equal(normalize_slug_input("docs/foo"), "foo")
    t |> equal(normalize_slug_input("a\\b\\foo.md"), "foo")
}

[test]
def test_normalize_slug_input_trims_whitespace(t : T?) {
    t |> equal(normalize_slug_input("  foo  "), "foo")
    t |> equal(normalize_slug_input("\tfoo.md\n"), "foo")
}

[test]
def test_normalize_slug_input_empty(t : T?) {
    t |> equal(normalize_slug_input(""), "")
    t |> equal(normalize_slug_input("   "), "")
}

// ─── read/write error contracts ──────────────────────────────────────

[test]
def test_read_doc_clears_stale_error(t : T?) {
    let r = create_temp_directory_result("mouse_test")
    if (!(r is value)) {
        t |> failure("could not create temp dir: {unsafe(r.error)}")
        return
    }
    let root = unsafe(r.value)
    let fm = Frontmatter(slug = "alpha", title = "Alpha", created = "2026-05-08", last_verified = "2026-05-08")
    let path = path_join(root, "alpha.md")
    var werr : string
    let wrote = write_doc(path, fm, "body content\n", werr)
    t |> success(wrote, "seed write: {werr}")
    var doc : ParsedDoc
    var err = "leftover from previous call"
    let ok = read_doc(path, doc, err)
    t |> success(ok, "read_doc succeeded")
    t |> equal(err, "")
    t |> equal(doc.frontmatter.slug, "alpha")
    rmdir_rec(root)
}

[test]
def test_read_doc_sets_error_on_missing(t : T?) {
    var doc : ParsedDoc
    var err : string
    let ok = read_doc("/nonexistent-mouse-test-path/missing.md", doc, err)
    t |> success(!ok, "read of missing path returns false")
    t |> success(!empty(err), "error message populated")
    t |> success(find(err, "missing.md") >= 0, "error mentions the path: {err}")
}

// ─── body hash ───────────────────────────────────────────────────────

[test]
def test_body_hash_stable(t : T?) {
    let h1 = hash_body("hello world")
    let h2 = hash_body("hello world")
    t |> equal(h1, h2)
    let h3 = hash_body("hello worlD")  // different last char
    t |> success(h1 != h3, "different bodies produce different hashes")
}

[test]
def test_body_hash_empty(t : T?) {
    let h = hash_body("")
    t |> success(!empty(h), "empty body still produces a hash string")
}

// ─── round trip serialize/parse ──────────────────────────────────────

[test]
def test_serialize_round_trip(t : T?) {
    var fm = Frontmatter(
        slug = "x",
        title = "test title",
        created = "2026-01-02",
        last_verified = "2026-05-08")
    fm.links |> push("a")
    fm.links |> push("b")
    let text = serialize_doc(fm, "body content here\n")
    let doc <- parse_doc_text(text)
    t |> equal(doc.frontmatter.slug, "x")
    t |> equal(doc.frontmatter.title, "test title")
    t |> equal(doc.frontmatter.created, "2026-01-02")
    t |> equal(doc.frontmatter.last_verified, "2026-05-08")
    t |> equal(length(doc.frontmatter.links), 2)
    t |> success(find(doc.body, "body content here") >= 0, "body round-tripped")
}
