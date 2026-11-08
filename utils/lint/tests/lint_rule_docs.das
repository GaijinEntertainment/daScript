options gen2
options indenting = 4
options no_unused_function_arguments = false

require dastest/testing_boost public

require strings
require daslib/strings_boost
require daslib/fio
require _lint_cli

// LINT025, LINT027, LINT032 and LINT033 are FOLDER rules the runner
// enforces with its own
// tree walk, not a compile pass, so each is driven through the CLI
// over a planted tree.
//
// stray tree: `tagged` (rule_docs_only = true, one stray .md beside
// the five allowed names),
// `off` (rule_docs_only = false, one stray) and `plain` (no config,
// one stray).
// sized tree: `tagged`, `arch` (a routed checklist), `off` and
// `plain`, four oversized rule
// documents in all. names tree: a checklist and an architecture doc
// citing live and dead
// paths and cells, beside a sibling folder holding a bare-basename
// target.

var private _tmp_dir : string
var private _tmp_dir_inited = false


def private base_tmp() : string {
    if (!_tmp_dir_inited) {
        let r = create_temp_directory_result("lint_rule_docs")
        // Fail-fast: an empty path would resolve against the working directory and plant
        // the fixture tree in the worktree.
        if (!(r is value)) {
            panic("could not create temp dir for the rule-doc tests: {unsafe(r.error)}")
        }
        _tmp_dir = unsafe(r.value)
        _tmp_dir_inited = true
    }
    return _tmp_dir
}


def private tmp_root(topic : string) : string {
    let dir = path_join(base_tmp(), topic)
    mkdir_rec(dir)
    return dir
}


[finalize]
def cleanup_temp_dir {
    if (_tmp_dir_inited && !empty(_tmp_dir)) {
        rmdir_rec(_tmp_dir)
    }
}


def private plant(dir, name, content : string) {
    fopen(path_join(dir, name), "wb") $(f) {
        if (f != null) {
            fwrite(f, content)
        }
    }
}


// argv[0] is the running interpreter - dastest itself is spawned as
// `daslang(.exe) dastest/dastest.das ...`, so it names the binary to spawn.
def private run_lint_roots(extra : array<string>; roots : array<string>;
                           var output : string&) : int {
    var after <- ["-q"]
    after |> push_from(extra, roots)
    var argv <- lint_cli_argv([], after)
    return unsafe(popen_argv(argv, 0.0, $(f) {
        if (f != null) {
            output = fread(f)
        }
    }))
}


def private run_lint_argv(extra : array<string>; root : string; var output : string&) : int {
    return run_lint_roots(extra, [root], output)
}


def private run_lint(root : string; var output : string&) : int {
    let none : array<string>
    return run_lint_argv(none, root, output)
}


def private count_of(text, sub : string) : int {
    var n = 0
    var from = 0
    while (true) {
        let at = find(text, sub, from)
        break if (at < 0)
        n ++
        from = at + length(sub)
    }
    return n
}


// One folder: its own .lint_config (skipped when `config` is empty), a lint-clean .das so
// the run has something to lint, and the .md files the folder carries.
def private plant_stray_folder(root, name, config : string; docs : array<string>) : string {
    let dir = path_join(root, name)
    mkdir_rec(dir)
    if (!empty(config)) {
        plant(dir, ".lint_config", config)
    }
    plant(dir, "ok.das", "options gen2\n\n[export]\ndef main \{\n    print(\"ok\\n\")\n\}\n")
    for (doc in docs) {
        plant(dir, doc, "# {doc}\n")
    }
    return dir
}


def private build_stray_tree() : string {
    let root = path_join(tmp_root("stray"), "tree")
    mkdir_rec(root)
    plant_stray_folder(root, "tagged", "[docs]\nrule_docs_only = true\n",
        ["notes.md", "REVIEW.md", "REVIEW_LINQ.md", "ARCHITECTURE.md", "ARCHITECTURE_DEDUP.md"])
    plant_stray_folder(root, "off", "[docs]\nrule_docs_only = false\n", ["notes.md"])
    plant_stray_folder(root, "plain", "", ["notes.md"])
    return root
}


// A markdown body of exactly `n` lines, each newline-terminated.
def private md_of_lines(n : int) : string {
    return build_string() $(var w) {
        for (i in range(n)) {
            w |> write("line {i}\n")
        }
    }
}


// One folder: its own .lint_config (skipped when `config` is empty), a lint-clean .das so the
// run has something to lint, and the .md files the folder carries, each at its own length.
def private plant_sized_folder(root, name, config : string; docs : array<string>;
                               lengths : array<int>) : string {
    let dir = path_join(root, name)
    mkdir_rec(dir)
    if (!empty(config)) {
        plant(dir, ".lint_config", config)
    }
    plant(dir, "ok.das", "options gen2\n\n[export]\ndef main \{\n    print(\"ok\\n\")\n\}\n")
    for (doc, n in docs, lengths) {
        plant(dir, doc, md_of_lines(n))
    }
    return dir
}


def private build_sized_tree() : string {
    let root = path_join(tmp_root("sized"), "tree")
    mkdir_rec(root)
    // 301 fires, 300 is at the cap
    plant_sized_folder(root, "tagged", "[docs]\nrule_docs_only = true\n",
        ["ARCHITECTURE.md", "REVIEW.md"], [301, 300])
    plant_sized_folder(root, "arch", "", ["REVIEW_GPU.md"], [301])
    plant_sized_folder(root, "off", "[docs]\nrule_docs_only = false\n", ["ARCHITECTURE.md"], [400])
    plant_sized_folder(root, "plain", "", ["ARCHITECTURE.md"], [400])
    return root
}


let private CHECKLIST = ("# Folder Code Review Checklist\n\n"
    + "**Read `REVIEW_COMMON.md` first.**\n\n"
    + "**A diff that changes `run.das` also runs `tests/test_thing_holds.das`.**\n\n"
    + "**A diff that changes `gone.das` runs `test_thing_holds` and `test_thing_vanished`.**\n\n"
    + "**A diff touching `sibling_tool.das` or `tests/test_gone*.das` names the run.**\n\n"
    + "**A hook the gate list above does not cover is a defect.**\n\n"
    + "**A diff that runs `test_thing_ghost` or names `far_tool.das` must foresee below-average load; the clause below binds `test_aot`.**\n\n"
    + "**A diff touching `sibling/sibling_tool.das`, `run.das --help`, `$LCPP/src/unicode-data.cpp`, `./run.das` or `hook.sh` is silent; `above.das`, `nowhere/sibling_tool.das` and `stale.das` are dead.**\n")

let private ARCH = ("# Folder Architecture\n\n"
    + "`gone.das` holds the walk; see the rule above; `test_thing_vanished` pins it.\n")


def private build_names_tree() : string {
    let root = path_join(tmp_root("names"), "tree")
    let folder = path_join(root, "folder")
    let tests = path_join(folder, "tests")
    let sibling = path_join(root, "sibling")
    mkdir_rec(tests)
    mkdir_rec(sibling)
    let github = path_join(root, ".github")   // a skip dir the basename index still reads
    let build = path_join(root, "build")   // a build tree (CMakeCache.txt) the index prunes
    mkdir_rec(github)
    mkdir_rec(build)
    plant(github, "hook.sh", "#!/bin/sh\necho hook\n")
    plant(build, "CMakeCache.txt", "CMAKE_GENERATOR:INTERNAL=Ninja\n")
    plant(build, "stale.das", "options gen2\n")
    // above the scan root - the ancestor walk stops at the root
    plant(tmp_root("names"), "above.das", "options gen2\n")
    plant(folder, "ok.das", "options gen2\n\n[export]\ndef main \{\n    print(\"ok\\n\")\n\}\n")
    plant(folder, "run.das", "options gen2\n\n[export]\ndef main \{\n    print(\"run\\n\")\n\}\n")
    plant(tests, "test_thing_holds.das", "options gen2\n\nrequire dastest/testing_boost public\n\n// def test_thing_ghost - a retired cell, named only in this comment\n\n[test]\ndef test_thing_holds(t : T?) \{\n    t |> equal(1, 1)\n\}\n")
    plant(sibling, "sibling_tool.das", "options gen2\n\n[export]\ndef main \{\n    print(\"tool\\n\")\n\}\n")
    plant(folder, "REVIEW.md", CHECKLIST)
    plant(folder, "ARCHITECTURE.md", ARCH)
    return root
}

// a second scan root holding `far_tool.das` - the bare basename the
// first root's checklist names
def private build_far_tree() : string {
    let root = path_join(tmp_root("names"), "far")
    let other = path_join(root, "other")
    mkdir_rec(other)
    plant(other, "far_tool.das", "options gen2\n\n[export]\ndef main \{\n    print(\"far\\n\")\n\}\n")
    return root
}


[test(description = "LINT025 reports one stray .md in the tagged folder only, never a rule document, and a single-file run still arms its folder", timeout = 1200)]
def test_rule_docs_only_folder(t : T?) {
    t |> run("a stray .md is reported once, and only in the tagged folder") @(t : T?) {
        let root = build_stray_tree()
        var out : string
        let rc = run_lint(root, out)
        t |> equal(count_of(out, "LINT025"), 1, "exactly one stray .md in the planted tree\n{out}")
        t |> success(find(out, "tagged/notes.md: LINT025: stray .md beside shipped code") >= 0,
            "the finding names the tagged folder's stray\n{out}")
        t |> success(find(out, "off/notes.md") < 0, "rule_docs_only = false reports nothing\n{out}")
        t |> success(find(out, "plain/notes.md") < 0, "an untagged folder reports nothing\n{out}")
        t |> equal(rc, 2, "a finding sets the issue exit code\n{out}")
    }
    t |> run("the rule documents themselves are never findings") @(t : T?) {
        let root = build_stray_tree()
        var out : string
        run_lint(root, out)
        for (doc in ["REVIEW.md", "REVIEW_LINQ.md", "ARCHITECTURE.md", "ARCHITECTURE_DEDUP.md"]) {
            t |> success(find(out, "/{doc}: LINT025") < 0, "{doc} is a rule document\n{out}")
        }
    }
    t |> run("a file positional arms its folder - the PR lint shape") @(t : T?) {
        let root = build_stray_tree()
        var out : string
        let rc = run_lint(path_join(path_join(root, "tagged"), "ok.das"), out)
        t |> equal(count_of(out, "LINT025"), 1, "linting one file still checks its folder\n{out}")
        t |> success(find(out, "tagged/notes.md: LINT025") >= 0, "the finding names the stray\n{out}")
        t |> equal(rc, 2, "a finding sets the issue exit code\n{out}")
    }
    t |> run("the tag is a folder property - a subfolder does not inherit it") @(t : T?) {
        let root = build_stray_tree()
        let nested = path_join(path_join(root, "tagged"), "nested")
        mkdir_rec(nested)
        plant(nested, "stray.md", "# stray\n")
        var out : string
        run_lint(root, out)
        t |> success(find(out, "nested/stray.md") < 0, "no cascade into an untagged subfolder\n{out}")
    }
    t |> run("a tagged folder with no .das still gets its verdict") @(t : T?) {
        let root = build_stray_tree()
        let dir = path_join(root, "mdonly")
        mkdir_rec(dir)
        plant(dir, ".lint_config", "[docs]\nrule_docs_only = true\n")
        plant(dir, "stray.md", "# stray\n")
        var out : string
        let rc = run_lint(dir, out)
        t |> success(find(out, "no .das files found") < 0, "an armed folder is not an empty input\n{out}")
        t |> success(find(out, "mdonly/stray.md: LINT025") >= 0, "the finding names the stray\n{out}")
        t |> equal(rc, 2, "a finding sets the issue exit code\n{out}")
    }
}


[test(description = "LINT025 dedups overlapping roots, skips skipped directories, and obeys --enable and --disable", timeout = 1200)]
def test_rule_docs_only_modes(t : T?) {
    t |> run("overlapping scan roots report each stray once") @(t : T?) {
        let root = build_stray_tree()
        var out : string
        let argv <- lint_cli_argv([], ["-q", root, path_join(root, "tagged")])
        unsafe(popen_argv(argv, 0.0, $(f) {
            if (f != null) {
                out = fread(f)
            }
        }))
        t |> equal(count_of(out, "tagged/notes.md"), 1, "the seen set dedups overlapping roots\n{out}")
    }
    t |> run("a tagged folder under a skipped directory is not scanned") @(t : T?) {
        let root = build_stray_tree()
        let hidden = path_join(path_join(root, "_gen"), "inner")
        mkdir_rec(hidden)
        plant(hidden, ".lint_config", "[docs]\nrule_docs_only = true\n")
        plant(hidden, "stray.md", "# stray\n")
        var out : string
        run_lint(root, out)
        t |> success(find(out, "inner/stray.md") < 0, "the walk prunes _-prefixed dirs like the das scan\n{out}")
    }
    t |> run("a non-bool rule_docs_only value reads as false") @(t : T?) {
        let root = build_stray_tree()
        let dir = path_join(root, "strval")
        mkdir_rec(dir)
        plant(dir, ".lint_config", "[docs]\nrule_docs_only = \"true\"\n")
        plant(dir, "stray.md", "# stray\n")
        var out : string
        run_lint(root, out)
        t |> success(find(out, "strval/stray.md") < 0, "only a bool true arms the folder\n{out}")
    }
    t |> run("--enable whitelist mode silences an unlisted LINT025") @(t : T?) {
        let root = build_stray_tree()
        var out : string
        let argv <- lint_cli_argv([], ["-q", "--enable", "LINT001", root])
        unsafe(popen_argv(argv, 0.0, $(f) {
            if (f != null) {
                out = fread(f)
            }
        }))
        t |> equal(count_of(out, "LINT025"), 0, "whitelist mode without LINT025 reports nothing\n{out}")
    }
    t |> run("--disable LINT025 silences the pass") @(t : T?) {
        let root = build_stray_tree()
        var out : string
        let argv <- lint_cli_argv([], ["-q", "--disable", "LINT025", root])
        let rc = unsafe(popen_argv(argv, 0.0, $(f) {
            if (f != null) {
                out = fread(f)
            }
        }))
        t |> equal(count_of(out, "LINT025"), 0, "the disabled rule reports nothing\n{out}")
        t |> equal(rc, 0, "and the run is clean\n{out}")
    }
}


[test(description = "LINT027 reports each rule document past 300 lines once, in any folder, and obeys --enable and --disable", timeout = 1200)]
def test_rule_doc_line_gate(t : T?) {
    t |> run("an oversized rule document fires, once per document, in any folder") @(t : T?) {
        let root = build_sized_tree()
        var out : string
        let rc = run_lint(root, out)
        t |> equal(count_of(out, "LINT027"), 4, "one per oversized rule document, in any folder\n{out}")
        t |> success(find(out, "tagged/ARCHITECTURE.md: LINT027: rule document exceeds 300 lines (301)") >= 0,
            "the finding names the document and its size\n{out}")
        t |> success(find(out, "arch/REVIEW_GPU.md: LINT027") >= 0,
            "a folder holding a rule document is gated with no tag\n{out}")
        t |> equal(rc, 2, "a finding sets the issue exit code\n{out}")
    }
    t |> run("--enable LINT027 alone still arms the folder walk") @(t : T?) {
        let root = build_sized_tree()
        var out : string
        let rc = run_lint_argv(["--enable", "LINT027"], root, out)
        t |> equal(count_of(out, "LINT027"), 4, "the line gate runs with LINT025/026 whitelisted off\n{out}")
        t |> equal(rc, 2, "a finding sets the issue exit code\n{out}")
    }
    t |> run("the cap itself is silent") @(t : T?) {
        let root = build_sized_tree()
        var out : string
        run_lint(root, out)
        t |> success(find(out, "REVIEW.md: LINT027") < 0, "300 lines exactly is at the cap\n{out}")
    }
    t |> run("an untagged folder is gated like a tagged one") @(t : T?) {
        let root = build_sized_tree()
        var out : string
        run_lint(root, out)
        t |> success(find(out, "off/ARCHITECTURE.md") >= 0, "rule_docs_only = false does not disarm the line gate\n{out}")
        t |> success(find(out, "plain/ARCHITECTURE.md") >= 0, "no .lint_config does not disarm it either\n{out}")
    }
    t |> run("a nested folder holding a rule document is gated on its own") @(t : T?) {
        let root = build_sized_tree()
        let nested = path_join(path_join(root, "tagged"), "nested")
        mkdir_rec(nested)
        plant(nested, "ARCHITECTURE.md", md_of_lines(400))
        var out : string
        run_lint(root, out)
        t |> success(find(out, "nested/ARCHITECTURE.md") >= 0, "the subfolder's own rule document is gated\n{out}")
    }
    t |> run("--disable LINT027 silences the pass") @(t : T?) {
        let root = build_sized_tree()
        var out : string
        let rc = run_lint_argv(["--disable", "LINT027"], root, out)
        t |> equal(count_of(out, "LINT027"), 0, "the disabled rule reports nothing\n{out}")
        t |> equal(rc, 0, "and the run is clean\n{out}")
    }
    t |> run("--enable whitelist mode silences an unlisted LINT027") @(t : T?) {
        let root = build_sized_tree()
        var out : string
        run_lint_argv(["--enable", "LINT001"], root, out)
        t |> equal(count_of(out, "LINT027"), 0, "whitelist mode without LINT027 reports nothing\n{out}")
    }
}


[test(description = "LINT032 and LINT033 report dead paths, dead cells and positional rule cites in a checklist, and stay silent on live, bare-basename and wildcard cites", timeout = 1200)]
def test_rule_doc_names(t : T?) {
    t |> run("a checklist's dead paths and dead cells fire once each; live, bare-basename and wildcard cites are silent") @(t : T?) {
        let root = build_names_tree()
        var out : string
        let rc = run_lint_roots(["--enable", "LINT032,LINT033"], [root, build_far_tree()], out)
        t |> equal(count(out, "LINT033"), 6, "one dead path, one dead cell, one cell a comment names, a file above the root, a slashed path with a live basename, a build-tree file\n{out}")
        t |> success(find(out, "REVIEW.md:7: LINT033: cited path `gone.das`") >= 0, "the dead path is named with its line\n{out}")
        t |> success(find(out, "REVIEW.md:7: LINT033: cited test cell `test_thing_vanished`") >= 0, "the dead cell is named with its line\n{out}")
        t |> success(find(out, "REVIEW.md:13: LINT033: cited test cell `test_thing_ghost`") >= 0, "a `def test_` inside a comment defines no cell\n{out}")
        t |> success(find(out, "REVIEW.md:15: LINT033: cited path `above.das`") >= 0, "the ancestor walk stops at the scan root\n{out}")
        t |> success(find(out, "REVIEW.md:15: LINT033: cited path `nowhere/sibling_tool.das`") >= 0, "a slashed path gets no bare-basename fallback\n{out}")
        t |> success(find(out, "REVIEW.md:15: LINT033: cited path `stale.das`") >= 0, "a build tree is pruned from the basename index\n{out}")
        t |> success(find(out, "test_thing_holds`") < 0, "a cell the folder's tests define is silent\n{out}")
        t |> success(find(out, "`test_aot`") < 0, "a one-underscore name is a binary, not a cell\n{out}")
        t |> success(find(out, "`sibling_tool.das`") < 0, "a bare basename that lives in another tree is silent\n{out}")
        t |> success(find(out, "`sibling/sibling_tool.das`") < 0, "a path that resolves from an ancestor under the root is silent\n{out}")
        t |> success(find(out, "far_tool.das") < 0, "a bare basename that lives under another scan root is silent\n{out}")
        t |> success(find(out, "hook.sh") < 0, "a file under .github is indexed\n{out}")
        t |> success(find(out, "run.das") < 0, "the first word of a backticked span is the path\n{out}")
        t |> success(find(out, "unicode-data.cpp") < 0 && find(out, "LINT033: cited path `.") < 0, "a $-, ~-, /- or .-prefixed token is not a path cite\n{out}")
        t |> success(find(out, "ARCHITECTURE.md") < 0, "an architecture doc is not read by either rule\n{out}")
        t |> equal(rc, 2, "a finding sets the issue exit code\n{out}")
    }
    t |> run("a checklist citing a rule by position fires per cite, above or below; a word that ends in a head does not") @(t : T?) {
        let root = build_names_tree()
        var out : string
        run_lint_roots(["--enable", "LINT032"], [root], out)
        t |> equal(count(out, "LINT032"), 2, "the two positional cites - `foresee below` is not `see below`\n{out}")
        t |> success(find(out, "REVIEW.md:11: LINT032") >= 0, "the `above` cite, named with its line\n{out}")
        t |> success(find(out, "REVIEW.md:13: LINT032") >= 0, "the `below` cite, named with its line\n{out}")
    }
    t |> run("--enable LINT033 alone arms the folder walk with the other rules off") @(t : T?) {
        let root = build_names_tree()
        var out : string
        run_lint_roots(["--enable", "LINT033"], [root], out)
        t |> equal(count(out, "LINT033"), 7, "the names pass runs alone - with one scan root the far basename is dead too\n{out}")
        t |> equal(count(out, "LINT032"), 0, "the positional pass stays off\n{out}")
    }
}
