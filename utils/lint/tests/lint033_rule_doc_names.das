options gen2
options indenting = 4
options no_unused_function_arguments = false

require dastest/testing_boost public

require strings
require daslib/strings_boost
require daslib/fio

// LINT032 and LINT033 are FOLDER rules the runner enforces with its own tree walk, not a
// compile pass, so like LINT027 they are driven through the CLI over a planted tree.
//
// The tree carries one folder holding a checklist and an architecture doc, a tests/ subfolder
// with two cells, and a sibling folder holding the file the checklist names by bare basename.
// The checklist cites: a live path, a dead path, a live cell, a dead cell, a bare basename that
// lives in the sibling folder, a wildcard span (skipped), and one rule by position. The
// architecture doc carries the same dead cites and a positional phrase - neither rule reads it.

var private _tmp_dir : string
var private _tmp_dir_inited = false


def private tmp_root() : string {
    if (!_tmp_dir_inited) {
        let r = create_temp_directory_result("lint033_rule_doc_names")
        if (!(r is value)) {
            panic("could not create temp dir for the LINT033 test: {unsafe(r.error)}")
        }
        _tmp_dir = unsafe(r.value)
        _tmp_dir_inited = true
    }
    return _tmp_dir
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


let private CHECKLIST = ("# Folder Code Review Checklist\n\n"
    + "**Read `REVIEW_COMMON.md` first.**\n\n"
    + "**A diff that changes `run.das` also runs `tests/test_thing_holds.das`.**\n\n"
    + "**A diff that changes `gone.das` runs `test_thing_holds` and `test_thing_vanished`.**\n\n"
    + "**A diff touching `sibling_tool.das` or `tests/test_gone*.das` names the run.**\n\n"
    + "**A hook the gate list above does not cover is a defect.**\n\n"
    + "**A diff that runs `test_thing_ghost` or names `far_tool.das` foresees below-average load.**\n")

let private ARCH = ("# Folder Architecture\n\n"
    + "`gone.das` holds the walk; see the rule above; `test_thing_vanished` pins it.\n")


def private build_tree() : string {
    let root = path_join(tmp_root(), "tree")
    let folder = path_join(root, "folder")
    let tests = path_join(folder, "tests")
    let sibling = path_join(root, "sibling")
    mkdir_rec(tests)
    mkdir_rec(sibling)
    plant(folder, "ok.das", "options gen2\n\n[export]\ndef main \{\n    print(\"ok\\n\")\n\}\n")
    plant(folder, "run.das", "options gen2\n\n[export]\ndef main \{\n    print(\"run\\n\")\n\}\n")
    plant(tests, "test_thing_holds.das", "options gen2\n\nrequire dastest/testing_boost public\n\n// def test_thing_ghost - a retired cell, named only in this comment\n\n[test]\ndef test_thing_holds(t : T?) \{\n    t |> equal(1, 1)\n\}\n")
    plant(sibling, "sibling_tool.das", "options gen2\n\n[export]\ndef main \{\n    print(\"tool\\n\")\n\}\n")
    plant(folder, "REVIEW.md", CHECKLIST)
    plant(folder, "ARCHITECTURE.md", ARCH)
    return root
}

//! a second scan root holding `far_tool.das` - the bare basename the first root's checklist names
def private build_far_tree() : string {
    let root = path_join(tmp_root(), "far")
    let other = path_join(root, "other")
    mkdir_rec(other)
    plant(other, "far_tool.das", "options gen2\n\n[export]\ndef main \{\n    print(\"far\\n\")\n\}\n")
    return root
}


def private das_exe() : string {
    let args <- get_command_line_arguments()
    return empty(args) ? "" : args[0]
}


def private run_lint_argv(extra : array<string>; roots : array<string>; var output : string&) : int {
    var argv <- [das_exe(), "-dasroot", get_das_root(), "{get_das_root()}/utils/lint/main.das", "--", "-q"]
    argv |> push_from(extra, roots)
    return unsafe(popen_argv(argv, 0.0, $(f) {
        if (f != null) {
            output = fread(f)
        }
    }))
}



[test]
def test_rule_doc_names(t : T?) {
    t |> run("a checklist's dead paths and dead cells fire once each; live, bare-basename and wildcard cites are silent") @(t : T?) {
        let root = build_tree()
        var out : string
        let rc = run_lint_argv(["--enable", "LINT032,LINT033"], [root, build_far_tree()], out)
        t |> equal(count(out, "LINT033"), 3, "one dead path, one dead cell, and one cell a comment names\n{out}")
        t |> success(find(out, "REVIEW.md:7: LINT033: cited path `gone.das`") >= 0, "the dead path is named with its line\n{out}")
        t |> success(find(out, "REVIEW.md:7: LINT033: cited test cell `test_thing_vanished`") >= 0, "the dead cell is named with its line\n{out}")
        t |> success(find(out, "REVIEW.md:13: LINT033: cited test cell `test_thing_ghost`") >= 0, "a `def test_` inside a comment defines no cell\n{out}")
        t |> success(find(out, "test_thing_holds`") < 0, "a cell the folder's tests define is silent\n{out}")
        t |> success(find(out, "sibling_tool.das") < 0, "a bare basename that lives in another tree is silent\n{out}")
        t |> success(find(out, "far_tool.das") < 0, "a bare basename that lives under another scan root is silent\n{out}")
        t |> success(find(out, "ARCHITECTURE.md") < 0, "an architecture doc is not read by either rule\n{out}")
        t |> equal(rc, 2, "a finding sets the issue exit code\n{out}")
    }
    t |> run("a checklist citing a rule by position fires once; a word that ends in a head does not") @(t : T?) {
        let root = build_tree()
        var out : string
        run_lint_argv(["--enable", "LINT032"], [root], out)
        t |> equal(count(out, "LINT032"), 1, "the one positional cite - `foresees below` is not `see below`\n{out}")
        t |> success(find(out, "REVIEW.md:11: LINT032") >= 0, "named with its line\n{out}")
    }
    t |> run("--enable LINT033 alone arms the folder walk with the other rules off") @(t : T?) {
        let root = build_tree()
        var out : string
        run_lint_argv(["--enable", "LINT033"], [root], out)
        t |> equal(count(out, "LINT033"), 4, "the names pass runs alone - with one scan root the far basename is dead too\n{out}")
        t |> equal(count(out, "LINT032"), 0, "the positional pass stays off\n{out}")
    }
}
