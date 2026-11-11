options gen2
module test_shape shared

require strings
require math
require daslib/strings_boost
require daslib/fio
require dastest/review_gate
require dastest/file_census

//! The shape rules every test root's `REVIEW.das` gate applies over
//! the file census: a category file holds three or more `[test]`
//! functions, a folder's loose tests sit in its `uncategorized.das`,
//! a fenced uncategorized file is named by its fence signature,
//! continuations follow a full file, every test states its claim, a
//! compile-fail fixture states its claim in a `//!` line and carries
//! a fixture name, and no file exceeds the cap. The rules and their
//! reasons: `tests/ARCHITECTURE.md`. Findings land through
//! `review_gate`; the caller prints the verdict.

def private is_uncategorized(name : string) : bool =>
    name == "uncategorized" || name |> starts_with("uncategorized_")

//! the file name a fence signature spells: its options joined by `_`,
//! every value other than true joining its option's name
//! (`optimize_false`, `stack_524288`, `unit_test_1_23`)
def canonical_fenced_stem(signature : string) : string {
    let parts <- [for (option in split(signature, ","));
                  replace(replace(option, "=", "_"), ".", "_")]
    return "uncategorized_" + join(parts, "_")
}

//! the index a stem carries past `base`: 1 for the base itself, N for
//! `<base>_N`, 0 when the stem is neither
def continuation_index(stem : string; base : string) : int {
    return 1 if (stem == base)
    return 0 if (!(stem |> starts_with(base + "_")))
    let suffix = slice(stem, length(base) + 1)
    let index = to_int(suffix)
    return index >= 2 && "{index}" == suffix ? index : 0
}

//! `tests_only` is for a tool tree, where a `.das` with neither
//! `[test]` nor `expect` is a source or a fixture the suite feeds to
//! the tool, not a test that forgot its annotation, and a
//! compile-fail
//! fixture is named by the tool's own convention (a lint rule's code)
def check_test_shape(root : string; cap : int;  // nolint:STYLE037,STYLE038 - flat
                     tests_only : bool = false) {
    var files : array<TestFileCensus>
    census_tree(root, files)
    // for the continuation rule
    // keyed by the file's own directory: under a tool tree one folder
    // holds many `uncategorized.das`, one per tool
    let testCounts <- {for (c in files); "{dir_name(c.path)}|{c.stem}" => length(c.tests)}
    for (c in files) {
        let fullPath = "{root}/{c.path}"
        for (line in c.unparsedAnnotationLines) {
            gate_finding(fullPath, line, "the `[test(` annotation does not close on its " +
                "line; keep the whole annotation on one line so the census can read it")
        }
        if (c.cls == TestFileClass.fixture) {
            if (empty(c.header)) {
                gate_finding(fullPath, "a compile-fail fixture states its claim as a " +
                    "`//!` line above its first declaration")
            }
            if (!tests_only && !is_fixture_name(c.stem)) {
                gate_finding(fullPath, "a compile-fail fixture's name starts with " +
                    "`failed_`, `cant_` or `invalid_`, so the tools that skip files with " +
                        "no compilable AST can tell it apart")
            }
            continue
        }
        if (c.cls == TestFileClass.script) {
            if (!tests_only) {
                gate_finding(fullPath, "a run file with no `[test]` and no `expect` proves " +
                    "nothing by name; wrap its body in a `[test]` and give it a description")
            }
            continue
        }
        for (e in c.tests) {
            if (empty(e.description)) {
                gate_finding(fullPath, e.line, "`{e.funcName}` has no description; write " +
                    "`[test(description = \"what it proves\")]`")
            }
        }
        if (!is_uncategorized(c.stem)) {
            if (length(c.tests) > cap) {
                gate_finding(fullPath, "{length(c.tests)} `[test]` functions, over the " +
                    "cap of {cap}; split the file into files of three or more tests each")
            }
            var anyStandalone = false
            for (e in c.tests) {
                anyStandalone ||= !empty(e.standalone)
            }
            if (length(c.tests) < 3 && !anyStandalone) {
                gate_finding(fullPath, "a category file holds three or more `[test]` " +
                    "functions; this one holds {length(c.tests)} - move them into the " +
                        "folder's `uncategorized.das`, or into the category file they " +
                            "belong to; a file that cannot share a program says why in " +
                            "`[test(standalone = \"...\")]`")
            }
            continue
        }
        let signature = fence_signature(c)
        let canonical = empty(signature) ? "uncategorized" : canonical_fenced_stem(signature)
        let index = continuation_index(c.stem, canonical)
        if (length(c.tests) > cap) {
            gate_finding(fullPath, "{length(c.tests)} `[test]` functions, over the cap of " +
                "{cap}; continue the file in `{canonical}_{max(index, 1) + 1}.das`")
        }
        if (index == 0) {
            if (empty(signature)) {
                gate_finding(fullPath, "`{c.stem}.das` carries no fence option, so its " +
                    "tests belong in `uncategorized.das`")
            } elif (continuation_index(c.stem, "uncategorized") > 0) {
                gate_finding(fullPath, "`{c.stem}.das` carries fence options " +
                    "[{signature}], so a plain test cannot join it; a fenced " +
                        "uncategorized file is named by its signature, `{canonical}.das`")
            } else {
                gate_finding(fullPath, "`{c.stem}.das` is named apart from its fence " +
                    "signature [{signature}]; a fenced uncategorized file is named by its " +
                        "signature, `{canonical}.das`")
            }
            continue
        }
        if (index > 1) {
            let previous = index == 2 ? canonical : "{canonical}_{index - 1}"
            let previousTests = testCounts?["{dir_name(c.path)}|{previous}"] ?? -1
            if (previousTests < 0) {
                gate_finding(fullPath, "`{c.stem}.das` continues `{previous}.das`, which " +
                    "does not exist; rename it `{previous}.das`")
            } elif (previousTests < cap) {
                gate_finding(fullPath, "`{c.stem}.das` continues `{previous}.das`, which " +
                    "holds {previousTests} `[test]` functions, under the cap of {cap}; " +
                        "add the tests there")
            }
        }
    }
}
