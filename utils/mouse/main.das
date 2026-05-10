// blind-mouse — entry point. Subcommand-dispatched CLI; `serve` runs a
// stdio JSON-RPC MCP server.
//
//   daslang utils/mouse/main.das -- ask "how do I X" --root ./mouse-data
//   daslang utils/mouse/main.das -- add "question" --body "answer body"
//   daslang utils/mouse/main.das -- get <slug>
//   daslang utils/mouse/main.das -- rebuild
//   daslang utils/mouse/main.das -- log [-n N] [--misses]
//   daslang utils/mouse/main.das -- serve            # MCP stdio loop

options gen2
options persistent_heap
options gc
options multiple_contexts
options rtti
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require daslib/clargs
require daslib/json_boost
require daslib/fio
require daslib/strings_boost
require strings
require store
require index

// ─── argv ────────────────────────────────────────────────────────────

[CommandLineArgs]
struct MouseArgs {
    @clarg_positional
    @clarg_doc = "Subcommand: ask | add | get | rebuild | log | serve"
    command : Option<string>

    @clarg_positional
    @clarg_doc = "Argument: question text (ask, add) or slug (get)"
    arg : Option<string>

    @clarg_short = "r"
    @clarg_doc = "Corpus root (default ./mouse-data, env: MOUSE_ROOT)"
    root : string

    @clarg_short = "k"
    @clarg_doc = "Top-K results (ask)"
    k : int = 5

    @clarg_doc = "Slug for add (overrides auto-generated)"
    slug : string

    @clarg_short = "b"
    @clarg_doc = "Body content (add)"
    body : string

    @clarg_doc = "Force add even if similar exists"
    force : bool

    @clarg_short = "n"
    @clarg_doc = "Limit (log)"
    limit : int = 20

    @clarg_doc = "Show only queries with no result (log)"
    misses : bool

    @clarg_doc = "Treat the question as raw FTS5 query syntax (no sanitization, no OR-join). See https://www.sqlite.org/fts5.html#full_text_query_syntax"
    raw_query : bool

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

def resolve_root(arg_root : string) : string {
    if (!empty(arg_root)) {
        return arg_root
    }
    if (has_env_variable("MOUSE_ROOT")) {
        return get_env_variable("MOUSE_ROOT")
    }
    return "./mouse-data"
}

def positional_value(opt : Option<string>) : string {
    return opt ?? ""
}

// ─── CLI commands ────────────────────────────────────────────────────

def cmd_ask(args : MouseArgs) {
    let root = resolve_root(args.root)
    let query = positional_value(args.arg)
    if (empty(query)) {
        to_log(LOG_ERROR, "ask: missing question; pass as positional arg")
        return
    }
    with_index_db(root) $(db) {
        ensure_index_fresh(db, root)
        let hits <- search(db, query, args.k, args.raw_query)
        log_query(db, query, hits, "cli")
        if (empty(hits)) {
            print("(no results){HINT_NO_MATCH}\n")
            return
        }
        let qtok = tokenize_for_jaccard(query)
        print("Top {length(hits)} best match(es):\n")
        for (h in hits) {
            let ttok = tokenize_for_jaccard(h.title)
            print(fmt_search_hit(h, jaccard(qtok, ttok)))
        }
        print("{HINT_HIT}\n")
    }
}

def cmd_get(args : MouseArgs) {
    let root = resolve_root(args.root)
    let slug = positional_value(args.arg)
    if (empty(slug)) {
        to_log(LOG_ERROR, "get: missing slug")
        return
    }
    if (!is_valid_slug(slug)) {
        to_log(LOG_ERROR, "get: invalid slug '{slug}'")
        return
    }
    with_index_db(root) $(db) {
        ensure_index_fresh(db, root)
        let path = path_join(docs_dir(root), "{slug}.md")
        var doc : ParsedDoc
        var err : string
        if (!read_doc(path, doc, err)) {
            to_log(LOG_ERROR, "could not read {slug}: {err}")
            return
        }
        // rebuild already skips parse_error docs from the index, so they
        // can only reach get via an explicit slug. Warn-and-continue
        // (rather than fail) — a partial header is still better than nothing.
        if (!empty(doc.parse_error)) {
            to_log(LOG_WARNING, "{slug}: {doc.parse_error}")
        }
        print("# {doc.frontmatter.title}\n")
        print("slug: {doc.frontmatter.slug}\n")
        if (!empty(doc.frontmatter.created)) {
            print("created: {doc.frontmatter.created}\n")
        }
        if (!empty(doc.frontmatter.last_verified)) {
            print("last_verified: {doc.frontmatter.last_verified}\n")
        }
        if (!empty(doc.frontmatter.links)) {
            let links_str = doc.frontmatter.links |> join(", ")
            print("links: {links_str}\n")
        }
        let froms <- linked_from(db, slug)
        if (!empty(froms)) {
            let froms_str = froms |> join(", ")
            print("linked from: {froms_str}\n")
        }
        print("\n---\n\n")
        print(doc.body)
        print("\n\nHint: if this answer is stale or wrong, edit this .md directly and bump `last_verified`. Delete it if the answer is no longer correct or the question no longer comes up.\n")
    }
}

def cmd_add(args : MouseArgs) {
    let root = resolve_root(args.root)
    let question = positional_value(args.arg)
    if (empty(question)) {
        to_log(LOG_ERROR, "add: missing question")
        return
    }
    if (empty(args.body)) {
        to_log(LOG_ERROR, "add: missing --body")
        return
    }
    with_index_db(root) $(db) {
        ensure_index_fresh(db, root)
        let outcome <- add_doc(db, root, question, args.body, args.slug, args.force)
        if (!empty(outcome.error)) {
            to_log(LOG_ERROR, outcome.error)
            return
        }
        if (outcome.created) {
            print("Created: {outcome.slug}\n")
            print("path: {outcome.written_path}\n")
            if (!empty(outcome.similar)) {
                print("\nTop {length(outcome.similar)} similar document(s) (none above the {JACCARD_HARD_BLOCK:.2f} block threshold; surfaced for awareness):\n")
                for (s in outcome.similar) {
                    print(fmt_dupe_match(s))
                }
                print("\nHint: similarity > 0.5 may overlap meaningfully — consider whether the new doc could fold into one of these instead. Edit `last_verified` on the merged doc and `mouse rebuild` to drop this one.\n")
            } else {
                print("\nHint: if you discover this answer is wrong later, edit the .md at the path above and bump `last_verified`.\n")
            }
        } else {
            print("Did NOT create — top {length(outcome.similar)} similar document(s) (similarity ≥ {JACCARD_HARD_BLOCK:.2f}, looks like a duplicate):\n")
            for (s in outcome.similar) {
                print(fmt_dupe_match(s))
            }
            print("\nSuggestion: prefer extending an existing doc (run `mouse get <slug>` to find the path).\nRe-call with --force if this really is a different topic — the caller's judgment overrides the threshold.\n")
        }
    }
}

def cmd_rebuild(args : MouseArgs) {
    let root = resolve_root(args.root)
    var n = 0
    with_index_db(root) $(db) {
        n = rebuild(db, root)
        set_index_signature(db, compute_docs_signature(root))
    }
    print("rebuilt: {n} doc(s) in {root}\n")
}

def cmd_log(args : MouseArgs) {
    let root = resolve_root(args.root)
    let n = args.limit > 0 ? args.limit : 20
    with_index_db(root) $(db) {
        ensure_index_fresh(db, root)
        let rows <- recent_queries(db, n, args.misses)
        if (empty(rows)) {
            print("(no queries logged yet)\n")
            return
        }
        for (r in rows) {
            if (r.match_count == 0) {
                print("[{r.asked_at}] ({r.source}) {r.question} -> MISS\n")
            } else {
                print("[{r.asked_at}] ({r.source}) {r.question} -> {r.match_count} hit(s) (top: {r.top_slug})\n")
            }
        }
    }
}

// ─── MCP stdio server ────────────────────────────────────────────────

let MCP_PROTOCOL_VERSION = "2024-11-05"
let MCP_SERVER_NAME = "blind-mouse"
let MCP_SERVER_VERSION = "0.1.0"

struct ContentItem {
    @rename = "type" _type : string
    text : string
}

struct ToolResult {
    content : array<ContentItem>
    isError : bool
}

def make_tool_result(text : string; is_error : bool = false) : string {
    var result : ToolResult
    result.isError = is_error
    result.content |> emplace(ContentItem(_type = "text", text = text))
    return sprint_json(result, false)
}

def jsonrpc_response(id : string; result_body : string) : string {
    return "\{\"jsonrpc\":\"2.0\",\"id\":{id},\"result\":{result_body}\}"
}

def jsonrpc_error(id : string; code : int; msg : string) : string {
    let escaped = sprint_json(msg, false)
    return "\{\"jsonrpc\":\"2.0\",\"id\":{id},\"error\":\{\"code\":{code},\"message\":{escaped}\}\}"
}

def get_string_arg(args : JsonValue?; key : string) : string {
    if (args == null) {
        return ""
    }
    let v = args?[key]
    if (v == null) {
        return ""
    }
    if (v.value is _string) {
        return v.value as _string
    }
    return ""
}

def get_int_arg(args : JsonValue?; key : string; dflt : int) : int {
    if (args == null) {
        return dflt
    }
    let v = args?[key]
    if (v == null) {
        return dflt
    }
    if (v.value is _number) {
        return int(v.value as _number)
    }
    return dflt
}

def get_bool_arg(args : JsonValue?; key : string) : bool {
    if (args == null) {
        return false
    }
    let v = args?[key]
    if (v == null) {
        return false
    }
    if (v.value is _bool) {
        return v.value as _bool
    }
    return false
}

def jsonvalue_id_to_string(id : JsonValue?) : string {
    if (id == null) {
        return "null"
    }
    return write_json(id)
}

def handle_initialize(id : string) : string {
    let body = "\{\"protocolVersion\":\"{MCP_PROTOCOL_VERSION}\",\"capabilities\":\{\"tools\":\{\}\},\"serverInfo\":\{\"name\":\"{MCP_SERVER_NAME}\",\"version\":\"{MCP_SERVER_VERSION}\"\}\}"
    return jsonrpc_response(id, body)
}

def tools_list_body() : string {
    return build_string() $(var w) {
        w |> write("\{\"tools\":[")
        w |> write("\{\"name\":\"mouse__ask\",\"description\":\"Search the blind-mouse Q&A cache. Returns top-K matches ranked by FTS5 BM25, each annotated with a Jaccard title-similarity (sim 0..1) so you can judge relevance at a glance. Set rawQuery=true to pass raw FTS5 syntax (https://www.sqlite.org/fts5.html#full_text_query_syntax) — quoted phrases, NEAR, explicit AND/OR.\",\"inputSchema\":\{\"type\":\"object\",\"properties\":\{\"question\":\{\"type\":\"string\"\},\"k\":\{\"type\":\"integer\",\"default\":5\},\"rawQuery\":\{\"type\":\"boolean\",\"default\":false\},\"root\":\{\"type\":\"string\"\}\},\"required\":[\"question\"]\}\},")
        w |> write("\{\"name\":\"mouse__add\",\"description\":\"Add a Q&A to the cache. Runs a Jaccard similarity check on the question; with force=false (default), hard-blocks creation only when the top match scores >= 0.7 (a near-paraphrase) and returns the similar list without writing. Below the threshold, the doc is created and the similar list is returned for awareness. Set force=true to override the hard block.\",\"inputSchema\":\{\"type\":\"object\",\"properties\":\{\"question\":\{\"type\":\"string\"\},\"body\":\{\"type\":\"string\"\},\"slug\":\{\"type\":\"string\"\},\"force\":\{\"type\":\"boolean\",\"default\":false\},\"root\":\{\"type\":\"string\"\}\},\"required\":[\"question\",\"body\"]\}\},")
        w |> write("\{\"name\":\"mouse__get\",\"description\":\"Fetch a doc by slug. Returns body, frontmatter, and reverse-link footer (which docs link to this one).\",\"inputSchema\":\{\"type\":\"object\",\"properties\":\{\"slug\":\{\"type\":\"string\"\},\"root\":\{\"type\":\"string\"\}\},\"required\":[\"slug\"]\}\},")
        w |> write("\{\"name\":\"mouse__rebuild\",\"description\":\"Rescan <root>/docs/ and rebuild the SQLite index from disk. .md files are the source of truth.\",\"inputSchema\":\{\"type\":\"object\",\"properties\":\{\"root\":\{\"type\":\"string\"\}\}\}\}")
        w |> write("]\}")
    }
}

def handle_tools_list(id : string) : string {
    return jsonrpc_response(id, tools_list_body())
}

// FTS5 BM25 rank (negative; smaller = better) shown alongside Jaccard
// title-similarity (0..1) so the caller can read both signals at a glance.
def fmt_search_hit(h : SearchHit; sim : float) : string {
    return "  [BM25 {h.rank:.3f} | sim {sim:.3f}] {h.slug} — {h.title}\n      path: {h.path}\n      last_verified: {h.last_verified}\n"
}

def fmt_dupe_match(d : DupeMatch) : string {
    return "  [sim {d.similarity:.3f}] {d.slug} — {d.title}\n"
}

let HINT_NO_MATCH = "\nHint: nothing matched. Do the research yourself, then call mouse__add(question, body) so the next session doesn't redo this work."
let HINT_HIT = "\nHint: if any answer above is stale or wrong, edit the .md at the `path:` shown and bump `last_verified` (or delete the file if it's no longer relevant). To fetch one of the answers in full, call mouse__get with the slug shown next to the `BM25`/`sim` scores."

def tool_ask(args : JsonValue?) : string {
    let question = get_string_arg(args, "question")
    if (empty(question)) {
        return make_tool_result("missing 'question' argument", true)
    }
    let k = get_int_arg(args, "k", 5)
    let raw_query = get_bool_arg(args, "rawQuery")
    let root = resolve_root(get_string_arg(args, "root"))
    var output : string
    with_index_db(root) $(db) {
        ensure_index_fresh(db, root)
        let hits <- search(db, question, k, raw_query)
        log_query(db, question, hits, "mcp")
        if (empty(hits)) {
            output = "(no results for: {question}){HINT_NO_MATCH}"
            return
        }
        let qtok = tokenize_for_jaccard(question)
        output = build_string() $(var w) {
            w |> write("Top {length(hits)} best match(es):\n")
            for (h in hits) {
                let ttok = tokenize_for_jaccard(h.title)
                w |> write(fmt_search_hit(h, jaccard(qtok, ttok)))
            }
            w |> write(HINT_HIT)
        }
    }
    return make_tool_result(output, false)
}

def tool_add(args : JsonValue?) : string {
    let question = get_string_arg(args, "question")
    if (empty(question)) {
        return make_tool_result("missing 'question' argument", true)
    }
    let body = get_string_arg(args, "body")
    if (empty(body)) {
        return make_tool_result("missing 'body' argument", true)
    }
    let slug_hint = get_string_arg(args, "slug")
    let force = get_bool_arg(args, "force")
    let root = resolve_root(get_string_arg(args, "root"))
    var output : string
    var is_error = false
    with_index_db(root) $(db) {
        ensure_index_fresh(db, root)
        let outcome <- add_doc(db, root, question, body, slug_hint, force)
        if (!empty(outcome.error)) {
            output = outcome.error
            is_error = true
            return
        }
        if (outcome.created) {
            if (!empty(outcome.similar)) {
                output = build_string() $(var w) {
                    w |> write("Created: {outcome.slug}\n")
                    w |> write("path: {outcome.written_path}\n\n")
                    w |> write("Top {length(outcome.similar)} similar document(s) (none above the {JACCARD_HARD_BLOCK:.2f} block threshold; surfaced for awareness):\n")
                    for (s in outcome.similar) {
                        w |> write(fmt_dupe_match(s))
                    }
                    w |> write("\nHint: similarity > 0.5 may overlap meaningfully — consider whether the new doc could fold into one of these instead. Edit `last_verified` on the merged doc and call mouse__rebuild to drop this one.")
                }
            } else {
                output = "Created: {outcome.slug}\npath: {outcome.written_path}\n\nHint: if you discover this answer is wrong later, edit the .md at the path above and bump `last_verified`."
            }
        } else {
            output = build_string() $(var w) {
                w |> write("Did NOT create — top {length(outcome.similar)} similar document(s) (similarity ≥ {JACCARD_HARD_BLOCK:.2f}, looks like a duplicate):\n")
                for (s in outcome.similar) {
                    w |> write(fmt_dupe_match(s))
                }
                w |> write("\nSuggestion: prefer extending an existing doc (call mouse__get with one of the slugs above to find the path).\nRe-call with force=true if this really is a different topic — the caller's judgment overrides the threshold.")
            }
        }
    }
    return make_tool_result(output, is_error)
}

def tool_get(args : JsonValue?) : string {
    let slug = get_string_arg(args, "slug")
    if (empty(slug)) {
        return make_tool_result("missing 'slug' argument — pass the slug shown next to the `BM25`/`sim` scores in mouse__ask output (the bare-token form, e.g. 'how-do-i-foo-bar'), NOT the `path:` value", true)
    }
    if (!is_valid_slug(slug)) {
        let looks_like_path = find(slug, "/") >= 0 || find(slug, "\\") >= 0 || ends_with(slug, ".md")
        let hint = looks_like_path ? " — looks like a file path; pass just the slug (the bare-token form shown next to the `BM25`/`sim` scores in mouse__ask output)" : ""
        return make_tool_result("invalid slug '{slug}'{hint}", true)
    }
    let root = resolve_root(get_string_arg(args, "root"))
    var output : string
    var is_error = false
    with_index_db(root) $(db) {
        ensure_index_fresh(db, root)
        let path = path_join(docs_dir(root), "{slug}.md")
        var doc : ParsedDoc
        var err : string
        if (!read_doc(path, doc, err)) {
            output = "could not read {slug}: {err}"
            is_error = true
            return
        }
        // rebuild already skips parse_error docs from the index. Surface a
        // warning prefix so MCP callers see the diagnostic without losing
        // the partial body.
        var parse_warning : string
        if (!empty(doc.parse_error)) {
            parse_warning = "[W] {slug}: {doc.parse_error}\n\n"
        }
        let froms <- linked_from(db, slug)
        output = build_string() $(var w) {
            if (!empty(parse_warning)) {
                w |> write(parse_warning)
            }
            w |> write("# {doc.frontmatter.title}\n")
            w |> write("slug: {doc.frontmatter.slug}\n")
            if (!empty(doc.frontmatter.created)) {
                w |> write("created: {doc.frontmatter.created}\n")
            }
            if (!empty(doc.frontmatter.last_verified)) {
                w |> write("last_verified: {doc.frontmatter.last_verified}\n")
            }
            if (!empty(doc.frontmatter.links)) {
                let links_str = doc.frontmatter.links |> join(", ")
                w |> write("links: {links_str}\n")
            }
            if (!empty(froms)) {
                let froms_str = froms |> join(", ")
                w |> write("linked from: {froms_str}\n")
            }
            w |> write("\n---\n\n")
            w |> write(doc.body)
            w |> write("\n\nHint: if this answer is stale or wrong, edit this .md directly and bump `last_verified`. Delete it if the answer is no longer correct or the question no longer comes up.")
        }
    }
    return make_tool_result(output, is_error)
}

def tool_rebuild(args : JsonValue?) : string {
    let root = resolve_root(get_string_arg(args, "root"))
    var n = 0
    with_index_db(root) $(db) {
        n = rebuild(db, root)
        set_index_signature(db, compute_docs_signature(root))
    }
    return make_tool_result("rebuilt: {n} doc(s) in {root}", false)
}

def handle_tools_call(id : string; params : JsonValue?) : string {
    if (params == null) {
        return jsonrpc_error(id, -32602, "missing params")
    }
    let name_v = params?.name
    if (name_v == null || !(name_v.value is _string)) {
        return jsonrpc_error(id, -32602, "missing tool name")
    }
    let name = name_v.value as _string
    let args = params?.arguments
    var result : string
    if (name == "mouse__ask") {
        result = tool_ask(args)
    } elif (name == "mouse__add") {
        result = tool_add(args)
    } elif (name == "mouse__get") {
        result = tool_get(args)
    } elif (name == "mouse__rebuild") {
        result = tool_rebuild(args)
    } else {
        return jsonrpc_error(id, -32602, "unknown tool: {name}")
    }
    return jsonrpc_response(id, result)
}

def dispatch_jsonrpc(body : string) : string {
    var err : string
    var parsed = read_json(body, err)
    if (parsed == null) {
        return jsonrpc_error("null", -32700, "parse error: {err}")
    }
    let id = jsonvalue_id_to_string(parsed?.id)
    let method_v = parsed?.method
    if (method_v == null || !(method_v.value is _string)) {
        unsafe {
            delete parsed
        }
        return jsonrpc_error(id, -32600, "missing method")
    }
    let method = method_v.value as _string
    var result : string
    if (method == "initialize") {
        result = handle_initialize(id)
    } elif (method == "notifications/initialized" || method == "initialized") {
        unsafe {
            delete parsed
        }
        return ""
    } elif (method == "tools/list") {
        result = handle_tools_list(id)
    } elif (method == "tools/call") {
        result = handle_tools_call(id, parsed?.params)
    } elif (method == "ping") {
        result = jsonrpc_response(id, "\{\}")
    } else {
        result = jsonrpc_error(id, -32601, "method not found: {method}")
    }
    unsafe {
        delete parsed
    }
    return result
}

let HEAP_COLLECT_THRESHOLD = uint64(1024 * 1024)

def cmd_serve(args : MouseArgs) {
    let sin = fstdin()
    let sout = fstdout()
    while (!feof(sin)) {
        let msg = build_string() $(var w) {
            while (!feof(sin)) {
                let chunk = fgets(sin)
                let n = length(chunk)
                if (n == 0) {
                    break
                }
                if (character_at(chunk, n - 1) == '\n') {  // nolint:PERF003
                    if (n > 1 && character_at(chunk, n - 2) == '\r') {  // nolint:PERF003
                        w |> write(slice(chunk, 0, n - 2))
                    } else {
                        w |> write(slice(chunk, 0, n - 1))
                    }
                    break
                }
                w |> write(chunk)
            }
        }
        if (empty(msg)) {
            continue
        }
        let response = dispatch_jsonrpc(msg)
        if (!empty(response)) {
            fprint(sout, response)
            fprint(sout, "\n")
            fflush(sout)
        }
        let heap_used = heap_bytes_allocated()
        let str_used = string_heap_bytes_allocated()
        if (heap_used > HEAP_COLLECT_THRESHOLD || str_used > HEAP_COLLECT_THRESHOLD) {
            unsafe {
                heap_collect(str_used > HEAP_COLLECT_THRESHOLD, false)
            }
        }
    }
}

// ─── main ────────────────────────────────────────────────────────────

[export]
def main() {
    var args_r <- parse_args(type<MouseArgs>)
    if (args_r |> is_err) {
        to_log(LOG_ERROR, "error: {args_r |> unwrap_err}")
        print_help(get_command_info(type<MouseArgs>), "mouse")
        return
    }
    let args <- args_r |> move_unwrap
    if (args.help) {
        print_help(get_command_info(type<MouseArgs>), "mouse")
        return
    }
    let cmd = positional_value(args.command)
    if (empty(cmd)) {
        print_help(get_command_info(type<MouseArgs>), "mouse")
        return
    }
    if (cmd == "ask") {
        cmd_ask(args)
    } elif (cmd == "add") {
        cmd_add(args)
    } elif (cmd == "get") {
        cmd_get(args)
    } elif (cmd == "rebuild") {
        cmd_rebuild(args)
    } elif (cmd == "log") {
        cmd_log(args)
    } elif (cmd == "serve") {
        cmd_serve(args)
    } else {
        to_log(LOG_ERROR, "unknown command: {cmd}")
        print_help(get_command_info(type<MouseArgs>), "mouse")
    }
}
