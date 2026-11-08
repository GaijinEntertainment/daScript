options gen2
options indenting = 4

require daslib/clargs
require daslib/fio
require daslib/strings_boost
require daslib/cmp
require dastest/file_census
require strings
require math

// The test-tree audit and the index generator, both over
// dastest/file_census. The audit answers how the tree packs: files
// per folder, tests per file, fence signatures, and what the folder
// rule would collapse the tree to. `--readme` writes
// `tests/README.md` from the census and `--verify` fails when the
// committed index differs from what the tree would generate.

[CommandLineArgs]
struct Config {
    @clarg_doc = "Root to scan (default: <das root>/tests)"
    root : string

    @clarg_doc = "Cap on [test] functions per file for the what-if projection and the generated index"
    cap : int = 100

    @clarg_doc = "Print every file (folder, class, tests, arms, lines, fence signature)"
    files : bool

    @clarg_doc = "Print merge candidates per folder (same fence signature, grouped by name prefix)"
    candidates : bool

    @clarg_doc = "Show at most this many folders in the folder table (0 = all)"
    top : int = 0

    @clarg_doc = "Write <root>/README.md from the census instead of printing the audit"
    readme : bool

    @clarg_doc = "Exit 1 when <root>/README.md differs from what --readme would write"
    verify : bool

    @clarg_doc = "Write or verify this file instead of <root>/README.md (a tool tree keeps its own README)"
    index : string

    @clarg_doc = "Tool tree: a .das with neither [test] nor expect is a source or fixture, left out of the audit and the index"
    tests_only : bool
}

struct Bucket {
    files : int
    suites : int
    fixtures : int
    scripts : int
    tests : int
    arms : int
    lines : int
    oneTestSuites : int
    twoTestSuites : int
    noAot : int
    undescribed : int
}

def add(var b : Bucket; c : TestFileCensus) {
    b.files++
    b.tests += length(c.tests)
    b.arms += c.arms
    b.lines += c.lines
    if (c.noAot) {
        b.noAot++
    }
    for (e in c.tests) {
        if (empty(e.description)) {
            b.undescribed++
        }
    }
    if (c.cls == TestFileClass.suite) {
        b.suites++
        if (length(c.tests) == 1) {
            b.oneTestSuites++
        } elif (length(c.tests) == 2) {
            b.twoTestSuites++
        }
    } elif (c.cls == TestFileClass.fixture) {
        b.fixtures++
    } else {
        b.scripts++
    }
}

// the last histogram bucket holds every file with this many tests or
// more
let HISTOGRAM_TOP = 21

def col(v : int; w : int) : string => pad_left("{v}", w)

def pct(a, b : int) : string => b == 0 ? "-" : "{(a * 100) / b}%"

def ceil_div(a, b : int) : int => (a + b - 1) / b

def name_prefix(name : string) : string {
    var n = name
    for (p in ["test_", "t_"]) {
        if (n |> starts_with(p)) {
            n = slice(n, length(p))
            break
        }
    }
    let us = find(n, "_")
    return us < 0 ? n : slice(n, 0, us)
}

//! the file count under the folder rule: a name-prefix cluster of
//! three or more files is one category file, everything else one
//! uncategorized file, both split at `cap`; fixtures and scripts stay
//! as they are
def project_folder_rule(files : array<TestFileCensus>; cap : int; folder : string = "") : int {
    var clusters : table<string; int>       // folder|sig|prefix -> file count
    var clusterTests : table<string; int>
    var projectedFiles = 0
    for (c in files) {
        continue if (!empty(folder) && c.folder != folder)
        if (c.cls != TestFileClass.suite) {
            projectedFiles++
            continue
        }
        let key = "{c.folder}|{fence_signature(c)}|{name_prefix(c.stem)}"
        clusters[key]++
        clusterTests[key] += length(c.tests)
    }
    var leftovers : table<string; int>      // folder|sig -> tests landing in uncategorized
    for (key, n in keys(clusters), values(clusters)) {
        let t = clusterTests[key]
        if (n >= 3) {
            projectedFiles += ceil_div(t, cap)
        } else {
            leftovers[slice(key, 0, rfind(key, "|"))] += t
        }
    }
    for (_k, t in keys(leftovers), values(leftovers)) {
        projectedFiles += ceil_div(t, cap)
    }
    return projectedFiles
}

def print_candidates(files : array<TestFileCensus>) {
    var groups : table<string; array<int>>     // cluster key -> indices into files
    for (i in range(length(files))) {
        continue if (files[i].cls != TestFileClass.suite)
        groups["{files[i].folder} | sig[{fence_signature(files[i])}] | " +
            "{name_prefix(files[i].stem)}"] |> push(i)
    }
    var keysSorted <- [for (k in keys(groups)); k]
    sort(keysSorted)
    print("\n== merge candidates (same folder, same fence signature, same name prefix; 2+ " +
        "files) ==\n")
    for (k in keysSorted) {
        continue if (length(groups[k]) < 2)
        var tests = 0
        for (i in groups[k]) {
            tests += length(files[i].tests)
        }
        print("{k}: {length(groups[k])} files, {tests} tests\n")
        for (i in groups[k]) {
            print("    {files[i].stem}  ({length(files[i].tests)} tests, {files[i].arms} " +
                "arms, {files[i].lines} lines)\n")
        }
    }
}

def print_folder_table(files : array<TestFileCensus>; var perFolder : table<string; Bucket>;
                       cfg : Config) {
    print("\n== per folder (sorted by file count) ==\n")
    let heads <- [pad_right("folder", 28), pad_left("files", 6), pad_left("suites", 7),
        pad_left("fixt", 5), pad_left("script", 7), pad_left("tests", 6), pad_left("arms", 6),
        pad_left("t/file", 7), pad_left("1-test", 7), pad_left("undesc", 7), pad_left("rule", 5)]
    print("{join(heads, " ")}\n")
    var folderNames <- [for (k in keys(perFolder)); k]
    sort(folderNames) <| cmp_by(-perFolder[_].files, _)
    var shown = 0
    for (name in folderNames) {
        break if (cfg.top > 0 && shown >= cfg.top)
        shown++
        let b = perFolder[name]
        let tpf = b.suites == 0 ? "-" : "{float(b.tests) / float(b.suites):.1f}"
        print("{pad_right(name, 28)} {col(b.files, 6)} {col(b.suites, 7)} " +
            "{col(b.fixtures, 5)} {col(b.scripts, 7)} {col(b.tests, 6)} {col(b.arms, 6)} " +
                "{pad_left(tpf, 7)} {col(b.oneTestSuites, 7)} {col(b.undescribed, 7)} " +
                    "{col(project_folder_rule(files, cfg.cap, name), 5)}\n")
    }
}

def print_signatures(files : array<TestFileCensus>) {
    print("\n== fence signatures across suite files ==\n")
    var sigs : table<string; int>
    for (c in files) {
        if (c.cls == TestFileClass.suite) {
            sigs[fence_signature(c)]++
        }
    }
    var sigNames <- [for (k in keys(sigs)); k]
    sort(sigNames) <| cmp_by(-sigs[_], _)
    for (s in sigNames) {
        print("{col(sigs[s], 6)}  [{s}]\n")
    }
}

def print_histogram(histo : table<int; int>; suites : int) {
    print("\n== [test] per suite file ==\n")
    var cum = 0
    for (i in range(1, HISTOGRAM_TOP + 1)) {
        let c = histo?[i] ?? 0
        continue if (c == 0)
        cum += c
        let bucketName = i == HISTOGRAM_TOP ? "{HISTOGRAM_TOP}+" : "{i}"
        print("{pad_left(bucketName, 4)} tests: {col(c, 5)} files   cumulative " +
            "{pct(cum, suites)}\n")
    }
}

def print_audit(files : array<TestFileCensus>; root : string; cfg : Config) {
    var total : Bucket
    var perFolder : table<string; Bucket>
    var histo : table<int; int>     // [test] count -> number of suite files
    for (c in files) {
        total |> add(c)
        perFolder[c.folder] |> add(c)
        if (c.cls == TestFileClass.suite) {
            histo[min(length(c.tests), HISTOGRAM_TOP)]++
        }
    }
    print("root: {root}\n")
    print("files: {total.files}  (suites {total.suites}, expect fixtures " +
        "{total.fixtures}, bare scripts {total.scripts})\n")
    print("[test] functions: {total.tests}   run() arms: {total.arms}   lines: " +
        "{total.lines}   tests without a description: {total.undescribed}\n")
    print("suite files with ONE [test]: {total.oneTestSuites} " +
        "({pct(total.oneTestSuites, total.suites)} of suites); with TWO: " +
            "{total.twoTestSuites} ({pct(total.twoTestSuites, total.suites)})\n")
    print("no_aot files: {total.noAot}\n")
    if (total.suites > 0) {
        print("mean [test] per suite file: " +
            "{float(total.tests) / float(total.suites):.1f}; mean lines per file: " +
                "{total.lines / total.files}\n")
    }
    print_histogram(histo, total.suites)
    print_folder_table(files, perFolder, cfg)
    let ruled = project_folder_rule(files, cfg.cap)
    print("\n== what-if: the folder rule (prefix cluster of 3+ files = category file, " +
        "rest = per-folder uncategorized, cap {cfg.cap}) ==\n")
    print("files now {total.files} -> {ruled}  (suites {total.suites} -> " +
        "{ruled - total.fixtures - total.scripts})\n")
    print_signatures(files)
    if (cfg.candidates) {
        print_candidates(files)
    }
    if (cfg.files) {
        print("\n== files ==\n")
        for (c in files) {
            print("{c.folder}\t{c.cls}\t{length(c.tests)}\t{c.arms}\t{c.lines}\t{c.path}" +
                "\t[{fence_signature(c)}]\n")
        }
    }
}

def write_file_block(var w : StringBuilderWriter; c : TestFileCensus) {
    // the path under the section's folder: a tool tree nests one
    // `uncategorized.das` per tool below one first-level folder
    let fileName = c.folder == "." ? c.path : slice(c.path, length(c.folder) + 1)
    if (c.cls == TestFileClass.suite) {
        w |> write("\n### {fileName}\n")
        if (!empty(c.header)) {
            w |> write("\n{c.header}\n")
        }
        w |> write("\n")
        for (e in c.tests) {
            let claim = empty(e.description) ? "*(no description)*" : e.description
            w |> write("- `{e.funcName}` - {claim}\n")
        }
    } else {
        let kind = c.cls == TestFileClass.fixture ? "compile-fail fixture" : "script"
        let claim = empty(c.header) ? "*(no header)*" : c.header
        w |> write("\n### {fileName}\n\n{kind}: {claim}\n")
    }
}

//! the index: one section per folder, one block per file; every line
//! comes from the census
def render_readme(files : array<TestFileCensus>) : string {
    // folder -> indices into files, which arrive sorted by folder and
    // name
    var folders : table<string; array<int>>
    for (i in range(length(files))) {
        folders[files[i].folder] |> push(i)    // nolint:PERF006 - a few hundred indices
    }
    var folderNames <- [for (k in keys(folders)); k]
    sort(folderNames)
    var total : Bucket
    for (c in files) {
        total |> add(c)
    }
    return build_string() $(var w) {
        w |> write("# Test Suite Index\n\n")
        w |> write("<!-- GENERATED by utils/internal/test-audit/main.das --readme from " +
            "the [test] annotations; edit the tests, not this file -->\n\n")
        w |> write("{total.files} files: {total.suites} test suites holding {total.tests} " +
            "`[test]` functions, {total.fixtures} compile-fail fixtures, {total.scripts} " +
                "scripts. How a test tree is laid out and what each class of file is: " +
                    "`tests/ARCHITECTURE.md` at the repo root.\n")
        for (folder in folderNames) {
            w |> write("\n## {folder}/\n")
            for (i in folders[folder]) {
                write_file_block(w, files[i])
            }
        }
    }
}

//! the lines one side has and the other lacks, a few of each, so a
//! CI log says what moved without a checkout to diff against
def private print_index_diff(have, want : string) {
    var inscope haveLines <- split(have, "\n")
    var inscope wantLines <- split(want, "\n")
    var inscope haveSet, wantSet : table<string>
    for (l in haveLines) {
        haveSet |> insert(l)
    }
    for (l in wantLines) {
        wantSet |> insert(l)
    }
    var shown = 0
    for (l in haveLines) {
        break if (shown >= 12)
        if (!empty(l) && !(wantSet |> key_exists(l))) {
            print("  index only:  {l}\n")
            shown++
        }
    }
    shown = 0
    for (l in wantLines) {
        break if (shown >= 12)
        if (!empty(l) && !(haveSet |> key_exists(l))) {
            print("  census only: {l}\n")
            shown++
        }
    }
    // the same lines in another order or count show up here alone
    print("  index {length(haveLines)} lines, census {length(wantLines)} lines\n")
    let common = min(length(haveLines), length(wantLines))
    for (i in range(common)) {
        if (haveLines[i] != wantLines[i]) {
            print("  first difference at line {i + 1}:\n    index:  {haveLines[i]}\n" +
                "    census: {wantLines[i]}\n")
            break
        }
    }
}

[export]
def main : int {
    var cfg = Config()
    let rc = parse_args_with_help(cfg, "test-audit")
    return rc if (rc >= 0)
    let root = empty(cfg.root) ? path_join(get_das_root(), "tests") : cfg.root
    var files : array<TestFileCensus>
    census_tree(root, files)
    if (cfg.tests_only) {
        var inscope judged <- [for (c in files); c; where c.cls != TestFileClass.script]
        delete files
        files <- judged
    }
    // by path, not stem: a tool tree holds one `uncategorized.das` per
    // tool under the same first-level folder, and an unstable tie
    // would order the index differently on each platform
    sort(files) <| cmp_by(_.folder, _.path)
    if (cfg.readme || cfg.verify) {
        let readmePath = empty(cfg.index) ? path_join(root, "README.md") : cfg.index
        let want = render_readme(files)
        if (cfg.verify) {
            // a CRLF checkout (autocrlf=true) must read as the same index
            let have = fread(readmePath) |> replace("\r", "")
            if (have != want |> replace("\r", "")) {
                let rootArg = empty(cfg.root) ? "" : " --root {cfg.root}"
                let indexArg = empty(cfg.index) ? "" : " --index {cfg.index}"
                let onlyArg = cfg.tests_only ? " --tests-only" : ""
                print("{readmePath} differs from the census; regenerate it: daslang " +
                    "utils/internal/test-audit/main.das --{rootArg}{indexArg}{onlyArg} --readme\n")
                print_index_diff(have, want)
                return 1
            }
            print("{readmePath} matches the census\n")
            return 0
        }
        if (!fwrite(readmePath, want)) {
            print("cannot write {readmePath}\n")
            return 1
        }
        print("wrote {readmePath}\n")
        return 0
    }
    print_audit(files, root, cfg)
    return 0
}
