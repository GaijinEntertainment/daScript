// blind-mouse — frontmatter/body parsing, slug generation, body hash, file IO.
// No DB knowledge here. See index.das for SQLite + retrieval.

options gen2
options persistent_heap

require strings public
require daslib/strings_boost public
require daslib/fio public

// Slug length cap. Shared by `is_valid_slug`, `slug_from_title`, and the
// suffix-fits-too math in `dedupe_slug`. Untrusted callers (MCP/CLI) hit
// `is_valid_slug` so any auto-generated slug must stay within this limit
// or the resulting doc would be unfetchable.
let MAX_SLUG_LEN = 128

struct Frontmatter {
    slug : string
    title : string
    created : string
    last_verified : string
    links : array<string>
}

struct ParsedDoc {
    frontmatter : Frontmatter
    body : string
    questions : array<string>
    body_hash : string
    parse_error : string  // empty on success
}

// ─── frontmatter ─────────────────────────────────────────────────────

def private parse_inline_list(value : string) : array<string> {
    var out : array<string>
    let trimmed = strip(value)
    // The trailing-`]` check uses character_at to keep the guard a one-liner.
    if (length(trimmed) < 2 || first_character(trimmed) != '[' || character_at(trimmed, length(trimmed) - 1) != ']') {  // nolint:PERF003
        return <- out
    }
    let inner = slice(trimmed, 1, length(trimmed) - 1)
    if (empty(strip(inner))) {
        return <- out
    }
    for (item in split(inner, ",")) {
        let cleaned = strip(item)
        if (!empty(cleaned)) {
            out |> push(cleaned)
        }
    }
    return <- out
}

def private parse_kv_line(line : string; var key : string&; var value : string&) : bool {
    let colon = find(line, ":")
    if (colon < 0) {
        return false
    }
    key = strip(slice(line, 0, colon))
    value = strip(slice(line, colon + 1))
    return true
}

// Returns body + parse_error. On no frontmatter at all, returns the
// whole input as body with empty error. On opening fence without
// closing fence, returns whole input with parse_error set.
def parse_frontmatter_text(text : string; var fm : Frontmatter) : tuple<body : string; parse_error : string> {
    let lines <- split(text, "\n")
    if (empty(lines)) {
        return (body = "", parse_error = "")
    }
    if (strip(lines[0]) != "---") {
        return (body = text, parse_error = "")
    }
    var close_idx = -1
    for (i in range(1, length(lines))) {
        if (strip(lines[i]) == "---") {
            close_idx = i
            break
        }
    }
    if (close_idx < 0) {
        return (body = text, parse_error = "missing closing frontmatter fence")
    }
    for (i in range(1, close_idx)) {
        let stripped = strip(lines[i])
        if (empty(stripped)) {
            continue
        }
        var key : string
        var value : string
        if (!parse_kv_line(stripped, key, value)) {
            continue
        }
        if (key == "slug") {
            fm.slug = value
        } elif (key == "title") {
            fm.title = value
        } elif (key == "created") {
            fm.created = value
        } elif (key == "last_verified") {
            fm.last_verified = value
        } elif (key == "links") {
            fm.links <- parse_inline_list(value)
        }
    }
    let body = build_string() $(var w) {
        for (i in range(close_idx + 1, length(lines))) {
            if (i > close_idx + 1) {
                w |> write("\n")
            }
            w |> write(lines[i])
        }
    }
    return (body = body, parse_error = "")
}

// ─── ## Questions section ─────────────────────────────────────────────

def parse_questions(body : string) : array<string> {
    var out : array<string>
    let lines <- split(body, "\n")
    var in_section = false
    for (i in range(length(lines))) {
        let stripped = strip(lines[i])
        if (!in_section) {
            if (to_lower(stripped) == "## questions") {
                in_section = true
            }
            continue
        }
        if (starts_with(stripped, "#")) {
            break
        }
        if (starts_with(stripped, "- ")) {
            let q = strip(slice(stripped, 2))
            if (!empty(q)) {
                out |> push(q)
            }
        }
    }
    return <- out
}

// ─── slug generation ─────────────────────────────────────────────────

def slug_from_title(title : string) : string {
    let lower = to_lower(title)
    let raw = build_string() $(var w) {
        var prev_dash = true
        let n = length(lower)
        for (i in range(n)) {
            let c = character_at(lower, i)  // nolint:PERF003
            // `lower` is already lowercased, so is_alnum's uppercase half can never match — equivalent to the explicit ranges.
            if (is_alnum(c)) {
                w |> write(slice(lower, i, i + 1))
                prev_dash = false
            } elif (!prev_dash) {
                w |> write("-")
                prev_dash = true
            }
        }
    }
    var s = 0
    var e = length(raw)
    while (s < e && character_at(raw, s) == '-') {  // nolint:PERF003
        s ++
    }
    while (e > s && character_at(raw, e - 1) == '-') {  // nolint:PERF003
        e --
    }
    // Cap to MAX_SLUG_LEN so a long title can't produce a slug that
    // `is_valid_slug` later rejects (making the doc unfetchable). Re-trim
    // trailing dashes since the cut may land on a `-`.
    if (e - s > MAX_SLUG_LEN) {
        e = s + MAX_SLUG_LEN
        while (e > s && character_at(raw, e - 1) == '-') {  // nolint:PERF003
            e --
        }
    }
    return s < e ? slice(raw, s, e) : ""
}

// A slug is the on-disk filename and SQL primary key. Untrusted input
// (MCP JSON-RPC, CLI args) flows here, so reject anything that could
// escape <root>/docs/ or break path joins. Shape: lowercase alnum, dash,
// underscore; must start with alnum; capped at MAX_SLUG_LEN chars.
def is_valid_slug(s : string) : bool {
    let n = length(s)
    if (n == 0 || n > MAX_SLUG_LEN) {
        return false
    }
    var ok = true
    peek_data(s) $(bytes) {
        // Slugs must be lowercase-only — using is_alnum here would silently allow
        // uppercase letters and break the on-disk filename round-trip on
        // case-sensitive filesystems. The explicit ranges intentionally reject
        // uppercase, so the broader is_alnum() helper is wrong here.
        let first = int(bytes[0])
        let first_ok = (first >= '0' && first <= '9') || (first >= 'a' && first <= 'z')  // nolint:PERF014
        if (!first_ok) {
            ok = false
            return
        }
        for (b in bytes) {
            let c = int(b)
            let valid = (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || c == '-' || c == '_'  // nolint:PERF014
            if (!valid) {
                ok = false
                return
            }
        }
    }
    return ok
}

// Append `-2`, `-3`, ... to `base` until a free slot is found. Preserves
// `base` verbatim when there's no collision — does NOT re-slugify, so a
// valid hint with `_` keeps its `_`. On collision, truncates `base` only
// as much as needed to fit the suffix within MAX_SLUG_LEN.
def dedupe_slug(base : string; existing : table<string>) : string {
    if (empty(base)) {
        return ""
    }
    if (!key_exists(existing, base)) {
        return base
    }
    // Suffix is at most `-99999` (6 chars). Cap base to leave room.
    let max_base = MAX_SLUG_LEN - 6
    var capped = length(base) > max_base ? slice(base, 0, max_base) : base
    while (!empty(capped) && character_at(capped, length(capped) - 1) == '-') {  // nolint:PERF003
        capped = slice(capped, 0, length(capped) - 1)
    }
    if (empty(capped)) {
        return ""
    }
    if (capped != base && !key_exists(existing, capped)) {
        return capped
    }
    var n = 2
    while (n < 100000) {
        let candidate = "{capped}-{n}"
        if (!key_exists(existing, candidate)) {
            return candidate
        }
        n++
    }
    panic("slug collision overflow for {capped}")
    return ""
}

def slug_from_title_dedupe(title : string; existing : table<string>) : string {
    return dedupe_slug(slug_from_title(title), existing)
}

// Strip CR/LF (and surrounding whitespace) from a question. Without this,
// MCP-supplied newlines in `question` would split the YAML `title:` line
// across multiple lines and could inject sibling keys into frontmatter.
// Callers must treat empty-after-sanitize as a rejection.
def sanitize_question(q : string) : string {
    let cleaned = build_string() $(var w) {
        let n = length(q)
        for (i in range(n)) {
            let c = character_at(q, i)  // nolint:PERF003
            if (c == '\n' || c == '\r') {
                w |> write(" ")
            } else {
                w |> write(slice(q, i, i + 1))
            }
        }
    }
    return strip(cleaned)
}

// ─── body hash (FNV-1a 64-bit, decimal) ──────────────────────────────

def hash_body(body : string) : string {
    var h = 0xcbf29ce484222325ul
    let prime = 0x100000001b3ul
    peek_data(body) $(bytes) {
        for (b in bytes) {
            h ^= uint64(b)
            h *= prime
        }
    }
    return "{h}"
}

// ─── parse + read + write ────────────────────────────────────────────

def parse_doc_text(text : string) : ParsedDoc {
    var doc : ParsedDoc
    let r <- parse_frontmatter_text(text, doc.frontmatter)
    doc.body = r.body
    doc.parse_error = r.parse_error
    doc.questions <- parse_questions(doc.body)
    doc.body_hash = hash_body(doc.body)
    return <- doc
}

def read_doc_text(path : string; var error : string&) : string {
    error = ""
    var content : string
    var ok = false
    fopen(path, "rb") $(f) {
        if (f == null) {
            return
        }
        content := unsafe(fread_to_eof(f))
        ok = true
    }
    if (!ok) {
        error = "could not open {path}"
    }
    return content
}

def read_doc(path : string; var doc : ParsedDoc; var error : string&) : bool {
    error = ""
    let text = read_doc_text(path, error)
    if (!empty(error)) {
        return false
    }
    doc <- parse_doc_text(text)
    return true
}

def serialize_doc(fm : Frontmatter; body : string) : string {
    return build_string() $(var w) {
        w |> write("---\n")
        if (!empty(fm.slug)) {
            w |> write("slug: {fm.slug}\n")
        }
        if (!empty(fm.title)) {
            w |> write("title: {fm.title}\n")
        }
        if (!empty(fm.created)) {
            w |> write("created: {fm.created}\n")
        }
        if (!empty(fm.last_verified)) {
            w |> write("last_verified: {fm.last_verified}\n")
        }
        w |> write("links: [")
        for (i in range(length(fm.links))) {
            if (i > 0) {
                w |> write(", ")
            }
            w |> write(fm.links[i])
        }
        w |> write("]\n")
        w |> write("---\n\n")
        w |> write(body)
        if (!empty(body) && character_at(body, length(body) - 1) != '\n') {  // nolint:PERF003
            w |> write("\n")
        }
    }
}

def write_doc(path : string; fm : Frontmatter; body : string; var error : string&) : bool {
    error = ""
    let text = serialize_doc(fm, body)
    var ok = false
    fopen(path, "wb") $(f) {
        if (f == null) {
            return
        }
        f |> fprint(text)
        ok = true
    }
    if (!ok) {
        error = "could not write {path}"
    }
    return ok
}
