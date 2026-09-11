options gen2
options indenting = 4

require strings
require daslib/strings_boost
require daslib/fio
require daslib/json_boost
require math
require dastest/review_gate

// The mechanical half of utils/internal/bench-stand/REVIEW.md (contract: REVIEW_COMMON.md at
// the repo root). Run from the repo root: bin/daslang utils/internal/bench-stand/REVIEW.das -
// exit 0 clean, 1 with findings.

let TOOL = "utils/internal/bench-stand"

def private tool_path(rel : string) : string {
    return path_join(TOOL, rel)
}

// Every committed file of the tool. A dot-prefixed path is build or cache output the tree
// ignores (`.jitted_scripts/`), never something a rule reaches.
def private files_here() : array<string> {
    var out : array<string>
    dir_rec(TOOL) $(name, is_dir) {
        return if (is_dir)
        let rel = to_generic_path(name)
        return if (starts_with(rel, ".") || find(rel, "/.") >= 0)
        out |> push(rel)
    }
    out |> sort
    return <- out
}

// Every `.das` here is one module family: a sibling is required by its bare module name, never
// by a path, so the files stay movable as a set.
def private check_sibling_requires() {
    let names = files_here()
    var modules : table<string>
    for (rel in names) {
        if (extension(rel) == ".das" && !starts_with(base_name(rel), "test_") && base_name(rel) != "REVIEW.das") {
            modules |> insert(stem(rel))
        }
    }
    for (rel in names) {
        continue if (extension(rel) != ".das")
        let path = tool_path(rel)
        for (req in das_requires(path)) {
            let leaf = stem(base_name(req))
            continue if (req == leaf || !key_exists(modules, leaf))
            gate_finding(path, find_line(fread(path), req),
                "requires the sibling module '{leaf}' as '{req}' - spell a sibling by its bare module name")
        }
    }
}

// suite.json, SuiteConfig and README section 4 are three spellings of one key set; a key that
// loses any of the three is a knob nobody can set, type or find.
def private check_suite_keys() {
    let suite_path = tool_path("suite.json")
    let config_path = tool_path("bench_suite.das")
    let readme_path = tool_path("README.md")
    var error = ""
    var root = read_json(fread(suite_path), error)
    if (root == null || !empty(error)) {
        gate_finding(suite_path, "does not parse as JSON: {error}")
        return
    }
    let config_text = fread(config_path)
    let struct_at = find(config_text, "struct SuiteConfig \{")
    if (struct_at < 0) {
        gate_finding(config_path, "no `struct SuiteConfig \{` - the suite config's field list is what suite.json is checked against")
        return
    }
    let struct_end = find(config_text, "\n\}", struct_at)
    let struct_body = slice(config_text, struct_at, struct_end < 0 ? length(config_text) : struct_end)
    let readme_section = readme_section_of(readme_path, "## 4.")
    if (empty(readme_section)) {
        gate_finding(readme_path, "no `## 4.` section - the suite keys are documented there")
    }
    var json_keys : table<string>
    if (!(root.value is _object)) {
        gate_finding(suite_path, "is not a JSON object")
        return
    }
    unsafe {
        assume obj = root.value as _object
        for (key in keys(obj)) {
            json_keys |> insert(key)
            if (find(struct_body, "\n    {key} ") < 0) {
                gate_finding(suite_path, "key '{key}' is not a SuiteConfig field in bench_suite.das")
            }
            if (!empty(readme_section) && find(readme_section, key) < 0) {
                gate_finding(readme_path, "section 4 does not mention the suite key '{key}'")
            }
        }
    }
    for (line in split(struct_body, "\n")) {
        let trimmed = strip(line)
        continue if (empty(trimmed) || starts_with(trimmed, "//") || starts_with(trimmed, "struct"))
        let colon = find(trimmed, " :")
        continue if (colon <= 0)
        let field = slice(trimmed, 0, colon)
        continue if (find(field, " ") >= 0)
        if (!key_exists(json_keys, field)) {
            gate_finding(suite_path, "SuiteConfig field '{field}' has no key here - a knob with no line in suite.json is one nobody finds")
        }
    }
}

// A skipped benchmark is debt the site shows every night; a skip with no reason hides what it
// is standing in for.
def private check_skip_reasons() {
    let suite_path = tool_path("suite.json")
    var error = ""
    var root = read_json(fread(suite_path), error)
    return if (root == null)
    var files = root?["files"]
    return if (files == null || !(files.value is _object))
    unsafe {
        assume entries = files.value as _object
        for (name in keys(entries)) {
            var over = files?[name]
            continue if (over == null)
            let skip = over?["skip"]
            // a key the entry does not carry comes back as a non-null _null node, never as a
            // pointer that tests false - the distinction is what keeps a plain override quiet
            continue if (skip == null || skip.value is _null)
            if (empty(strip(over?["skip"] ?? ""))) {
                gate_finding(suite_path, "the skip of '{name}' states no reason - name the failure it hides")
            }
        }
    }
}

// The viewer renders series names, commit subjects and failure messages that come from a run
// record; every one of them reaches the DOM as text, never as markup.
def private check_viewer_no_markup() {
    for (rel in files_here()) {
        continue if (dir_name(rel) != "site" || extension(rel) != ".js")
        let path = tool_path(rel)
        let text = strip_line_comments(fread(path))
        for (needle in ["innerHTML", "outerHTML", "insertAdjacentHTML", "document.write"]) {
            let at = find(text, needle)
            continue if (at < 0)
            gate_finding(path, find_line(fread(path), needle),
                "writes markup with {needle} - a label from a run record reaches the DOM through textContent")
        }
    }
}

// nightly.sh's knobs are invisible: one list in its own header, one in README section 3. A knob
// missing from either is one an operator cannot discover, and a documented knob nothing reads is
// one they would set in vain.
def private check_env_knobs() {
    let script_path = tool_path("nightly.sh")
    let readme_path = tool_path("README.md")
    let text = fread(script_path)
    let header = slice(text, 0, max(find(text, "\nset -"), 0))
    let body = slice(text, max(find(text, "\nset -"), 0))
    let readme_section = readme_section_of(readme_path, "## 3.")
    if (empty(readme_section)) {
        gate_finding(readme_path, "no `## 3.` section - nightly.sh's knobs are documented there")
    }
    var read_names : table<string>
    for (name in keys(prefixed_tokens(body, "BENCH_STAND_"))) {
        read_names |> insert(name)
        if (find(header, name) < 0) {
            gate_finding(script_path, "reads {name} without naming it in the script's own header")
        }
        if (!empty(readme_section) && find(readme_section, name) < 0) {
            gate_finding(readme_path, "section 3 does not document {name}, which nightly.sh reads")
        }
    }
    for (name in keys(prefixed_tokens(header, "BENCH_STAND_"))) {
        if (!key_exists(read_names, name)) {
            gate_finding(script_path, "the header documents {name}, which the script never reads")
        }
    }
}

// caddy.snippet is the one place the stand's public routes are written down; the deploy script
// derives its checks from it, so a route literal here is a second copy that can go stale.
def private check_no_route_literals() {
    let path = tool_path("bench-stand-deploy.sh")
    var in_extractor = false
    var lineno = 0
    for (raw in split(fread(path), "\n")) {
        lineno++
        let line = strip(raw)
        // snippet_routes is the extractor: the directive names ARE its subject, so its body is
        // the one place in the script licensed to spell them.
        if (starts_with(line, "snippet_routes() \{")) {
            in_extractor = true
            continue
        }
        if (in_extractor) {
            in_extractor = line != "\}"
            continue
        }
        continue if (empty(line) || starts_with(line, "#"))
        let code = find(line, " # ") >= 0 ? slice(line, 0, find(line, " # ")) : line
        for (needle in ["handle_path", "redir ", "file_server", "reverse_proxy"]) {
            continue if (find(code, needle) < 0)
            gate_finding(path, lineno,
                "spells the route directive '{strip(needle)}' - read the routes from caddy.snippet instead (snippet_routes)")
        }
    }
}

// The placement block is the folder's inventory. A file with no line is a file no rule reaches.
def private check_placement_inventory() {
    let review_path = tool_path("REVIEW.md")
    let text = fread(review_path)
    let block_at = find(text, "**Placement - one file, one line")
    if (block_at < 0) {
        gate_finding(review_path, "no placement block - the folder's file inventory lives there")
        return
    }
    let inventory = slice(text, block_at)
    for (rel in files_here()) {
        let leaf = base_name(rel)
        continue if (leaf == "REVIEW.md" || leaf == "README.md")
        let listed = dir_name(rel) == "site" ? find(inventory, "`site/`") >= 0 : find(inventory, "`{leaf}`") >= 0
        if (!listed) {
            gate_finding(review_path, "the placement block has no line for {rel}")
        }
    }
}

// A `## N.` section of a markdown file, up to the next same-level heading.
def private readme_section_of(path : string; heading : string) : string {
    let text = fread(path)
    let at = find(text, heading)
    return "" if (at < 0)
    let next = find(text, "\n## ", at + 1)
    return next < 0 ? slice(text, at) : slice(text, at, next)
}

[export]
def main() : int {
    if (!stat(TOOL).is_dir) {
        print("run from the repo root: {TOOL} not found\n")
        return 1
    }
    check_sibling_requires()
    check_suite_keys()
    check_skip_reasons()
    check_viewer_no_markup()
    check_env_knobs()
    check_no_route_literals()
    check_placement_inventory()
    return gate_verdict("bench-stand")
}
