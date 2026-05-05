// Tutorial 54: Glob pattern matching
//
// This tutorial covers:
//   - match_glob: pathname-aware shell-style glob (* / ** / ? / [abc] / [a-z] / [!abc])
//   - glob: walk a directory tree and yield matching paths
//   - glob_filtered: include + exclude patterns, excludes win
//   - is_glob_pattern: detect literal vs glob to dispatch correctly
//   - expand_glob: pattern -> sorted file list (the higher-level helper)
//   - parse_file_list: comma/newline-separated list of files / dirs / globs
//   - When to use fio.match_glob vs strings_boost.glob_match (the two flavors)
//
// Prerequisites: Tutorial 07 (Strings), Tutorial 13 (Blocks)
//
// Run: daslang.exe tutorials/language/54_glob.das

options gen2

require daslib/fio
require daslib/strings_boost

[export]
def main() {
    // ============================================================
    // Section 1: match_glob — pathname-aware matching
    // ============================================================
    // `match_glob(pattern, path)` returns true if the path matches the
    // shell-style pattern. `*` matches any chars except `/`; `**` matches
    // across `/`; `?` matches one non-`/` char; `[abc]` is a character
    // class. The matcher is pure — no filesystem access.

    print("=== match_glob ===\n")

    let r1 = match_glob("*.das", "hello.das")
    let r2 = match_glob("*.das", "sub/hello.das")
    let r3 = match_glob("**/*.das", "sub/foo.das")
    let r4 = match_glob("**/*.das", "a/b/c/x.das")
    print("  *.das          vs hello.das       => {r1}\n")
    print("  *.das          vs sub/hello.das   => {r2}     // * stops at /\n")
    print("  **/*.das       vs sub/foo.das     => {r3}\n")
    print("  **/*.das       vs a/b/c/x.das     => {r4}\n")

    let r5 = match_glob("src/?ello.das", "src/hello.das")
    let r6 = match_glob("[a-z][0-9].txt", "m7.txt")
    let r7 = match_glob("[!abc].log", "x.log")
    print("  src/?ello.das  vs src/hello.das   => {r5}\n")
    print("  [a-z][0-9].txt vs m7.txt          => {r6}\n")
    print("  [!abc].log     vs x.log           => {r7}     // negated class\n")

    // ============================================================
    // Section 2: glob — walk + match in one shot
    // ============================================================
    // `glob(root, pattern, blk)` recursively walks `root` and invokes
    // `blk(filename, is_dir)` for entries that match `pattern`. Paths
    // are normalized to use `/` before matching, so the same pattern
    // works on Windows and POSIX.

    print("\n=== glob (filesystem walk) ===\n")

    // Use `create_temp_directory_result` so we never clobber a user-owned
    // path: the tutorial gets a fresh, OS-managed temp dir each run.
    let tmpdir_result = create_temp_directory_result("glob_tutorial_")
    if (!(tmpdir_result is value)) {
        print("  could not create temp directory: {tmpdir_result as error}\n")
        return
    }
    let tmpdir = tmpdir_result as value
    seed_tree(tmpdir)

    print("  built fixture tree under {tmpdir}/\n")

    var das_files : array<string>
    glob(tmpdir, "**/*.das") $(filename, is_dir) {
        if (!is_dir) {
            das_files |> push_clone(filename)
        }
    }
    sort(das_files)
    print("  glob ** matches all .das files (count={length(das_files)}):\n")
    for (f in das_files) {
        print("    {f}\n")
    }

    var pngs : array<string>
    glob(tmpdir, "assets/**/*.png") $(filename, is_dir) {
        if (!is_dir) {
            pngs |> push_clone(filename)
        }
    }
    sort(pngs)
    print("  glob assets/**/*.png finds nested PNGs (count={length(pngs)}):\n")
    for (f in pngs) {
        print("    {f}\n")
    }

    // ============================================================
    // Section 3: glob_filtered — includes + excludes
    // ============================================================
    // `glob_filtered(root, includes, excludes, blk)` yields entries that
    // match at least one include and zero excludes. Excludes win on conflict.

    print("\n=== glob_filtered ===\n")

    var hits : array<string>
    var includes : array<string>
    var excludes : array<string>
    includes |> push_clone("**/*.das")
    excludes |> push_clone("tests/**")
    glob_filtered(tmpdir, includes, excludes) $(filename, is_dir) {
        if (!is_dir) {
            hits |> push_clone(filename)
        }
    }
    sort(hits)
    print("  include **/*.das, exclude tests/** (count={length(hits)}):\n")
    for (f in hits) {
        print("    {f}\n")
    }

    // ============================================================
    // Section 4: is_glob_pattern — literal vs glob dispatch
    // ============================================================
    // When a tool accepts a user-supplied path, it's useful to know
    // whether to treat it as a literal file (and error if missing) or
    // as a glob (and warn if no matches). `is_glob_pattern` returns
    // true when the string contains `*`, `?`, or `[`.

    print("\n=== is_glob_pattern ===\n")
    let lit = is_glob_pattern("main.das")
    let g1 = is_glob_pattern("*.das")
    let g2 = is_glob_pattern("[ab]c.txt")
    let g3 = is_glob_pattern("data/v?.json")
    print("  main.das       => {lit}      // literal\n")
    print("  *.das          => {g1}     // glob\n")
    print("  [ab]c.txt      => {g2}     // glob (char class)\n")
    print("  data/v?.json   => {g3}     // glob (?)\n")

    // ============================================================
    // Section 5: Two flavors — fio.match_glob vs strings_boost.glob_match
    // ============================================================
    // daslib has two glob matchers. They look similar but differ in
    // how they treat `/`:
    //
    //   fio.match_glob          (pathname glob — POSIX FNM_PATHNAME)
    //     * stops at /          ** crosses /          [abc] classes
    //
    //   strings_boost.glob_match (filename glob — POSIX without FNM_PATHNAME)
    //     * crosses /           no **                  no classes
    //
    // Use `fio.match_glob` for path patterns. Use `glob_match` when
    // matching a single name component or any text where `/` is just
    // another character (e.g. tag matching, channel name matching).

    print("\n=== Two flavors ===\n")
    let pattern = "*.das"
    let path = "sub/hello.das"
    let path_aware = match_glob(pattern, path)
    let name_only = glob_match(pattern, path)
    print("  pattern={pattern}  path={path}\n")
    print("    fio.match_glob              => {path_aware}    // pathname-glob: false\n")
    print("    strings_boost.glob_match    => {name_only}     // filename-glob: true\n")

    // ============================================================
    // Section 6: to_generic_path — forward slashes regardless of host
    // ============================================================
    // `glob` and `glob_filtered` already pass `/`-normalized paths to
    // your callback. If you walk via `dir_rec` directly and need a
    // `/`-path for matching, call `to_generic_path` first.
    //
    // Note: `fio.normalize` is the OPPOSITE — it uses the platform-preferred
    // separator (`\` on Windows). Use `to_generic_path` when you want
    // stable `/`-output independent of host.

    print("\n=== to_generic_path ===\n")
    let mixed = "src\\sub/file.das"
    let generic = to_generic_path(mixed)
    print("  input  : {mixed}\n")
    print("  output : {generic}\n")

    // ============================================================
    // Section 7: expand_glob — pattern to sorted file list
    // ============================================================
    // `expand_glob(pattern, var result)` is the higher-level helper:
    // it splits the literal directory prefix from the glob remainder,
    // picks recursive vs shallow walk based on the pattern, filters out
    // directories, sorts locally, and APPENDS to `result`. Reach for it
    // whenever a CLI tool takes a single glob argument.

    print("\n=== expand_glob ===\n")
    var by_pattern : array<string>
    expand_glob("{tmpdir}/**/*.das", by_pattern)
    print("  expand_glob {tmpdir}/**/*.das (count={length(by_pattern)}):\n")
    for (f in by_pattern) {
        print("    {f}\n")
    }

    // ============================================================
    // Section 8: parse_file_list — comma/newline list of files/dirs/globs
    // ============================================================
    // `parse_file_list(file, var result)` is the canonical
    // "user-supplied paths argument -> expanded file list" expander.
    // Comma- or newline-separated; whitespace stripped; literal entries
    // pass through; glob entries go through `expand_glob`. Order is
    // preserved across plain entries — globs sort within their slice.

    print("\n=== parse_file_list ===\n")
    var inputs : array<string>
    let raw = "zzz_literal.das,{tmpdir}/src/*.das\n{tmpdir}/assets/**/*.png ,aaa_literal.das"
    parse_file_list(raw, inputs)
    print("  input: {raw}\n")
    print("  parsed (count={length(inputs)}; plain entries keep position):\n")
    for (f in inputs) {
        print("    {f}\n")
    }

    cleanup(tmpdir)
    print("\nDone.\n")
}

def seed_tree(root : string) {
    mkdir_rec("{root}/src")
    mkdir_rec("{root}/tests/inner")
    mkdir_rec("{root}/assets/sub")
    fwrite("{root}/main.das", "// main\n")
    fwrite("{root}/src/util.das", "// util\n")
    fwrite("{root}/tests/test_main.das", "// test\n")
    fwrite("{root}/tests/inner/test_inner.das", "// inner\n")
    fwrite("{root}/assets/icon.png", "PNG")
    fwrite("{root}/assets/sub/deep.png", "PNG")
}

def cleanup(root : string) {
    var error : string
    rmdir_rec(root, error)
}
