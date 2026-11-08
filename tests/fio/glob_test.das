options gen2

require dastest/testing_boost
require daslib/fio
require strings

[test(description = "match_glob matches an exact literal")]
def test_match_glob_literal(t : T?) {
    t |> run("exact literal matches") @(t : T?) {
        t |> success(match_glob("foo", "foo"))
        t |> success(match_glob("foo/bar", "foo/bar"))
        t |> success(!match_glob("foo", "bar"))
        t |> success(!match_glob("foo", "foob"))
        t |> success(!match_glob("foo", "fo"))
        t |> success(!match_glob("foo/bar", "foo/baz"))
    }
}

[test(description = "A single star matches within a segment and does not cross a slash")]
def test_match_glob_star(t : T?) {
    t |> run("single star within segment") @(t : T?) {
        t |> success(match_glob("*.txt", "hello.txt"))
        t |> success(match_glob("*.txt", ".txt"))
        t |> success(!match_glob("*.txt", "hello.md"))
        t |> success(match_glob("a*c", "abc"))
        t |> success(match_glob("a*c", "ac"))
        t |> success(match_glob("a*c", "axxxc"))
        t |> success(!match_glob("a*c", "abd"))
    }
    t |> run("single star does not cross /") @(t : T?) {
        t |> success(!match_glob("*.txt", "dir/hello.txt"))
        t |> success(!match_glob("a*c", "a/c"))
        t |> success(match_glob("dir/*.txt", "dir/hello.txt"))
        t |> success(!match_glob("dir/*.txt", "dir/sub/hello.txt"))
    }
}

[test(description = "A double star matches across slashes and honors trailing, extension and boundary forms")]
def test_match_glob_double_star(t : T?) {
    t |> run("** matches across /") @(t : T?) {
        t |> success(match_glob("**", "anything"))
        t |> success(match_glob("**", "a/b/c"))
        t |> success(match_glob("**/foo.txt", "foo.txt"))
        t |> success(match_glob("**/foo.txt", "a/foo.txt"))
        t |> success(match_glob("**/foo.txt", "a/b/c/foo.txt"))
        t |> success(match_glob("a/**/b", "a/b"))
        t |> success(match_glob("a/**/b", "a/x/b"))
        t |> success(match_glob("a/**/b", "a/x/y/z/b"))
        t |> success(!match_glob("a/**/b", "x/a/b"))
    }
    t |> run("trailing /** requires at least one segment under root") @(t : T?) {
        t |> success(match_glob("a/**", "a/b"))
        t |> success(match_glob("a/**", "a/b/c"))
        t |> success(!match_glob("a/**", "a"))   // shell glob convention: a/** does NOT match a itself
    }
    t |> run("** combined with extension filter") @(t : T?) {
        t |> success(match_glob("assets/**/*.png", "assets/icon.png"))
        t |> success(match_glob("assets/**/*.png", "assets/sub/icon.png"))
        t |> success(match_glob("assets/**/*.png", "assets/a/b/c/icon.png"))
        t |> success(!match_glob("assets/**/*.png", "assets/icon.jpg"))
        t |> success(!match_glob("assets/**/*.png", "other/icon.png"))
    }
    t |> run("**/ requires a / boundary when ** consumes characters") @(t : T?) {
        // Round-5 regression: the `/` after `**` is optional only when `**`
        // matches empty. Pattern `a/**/b` must NOT match `a/xb` — there is
        // no `/` separator between the `**` consumption and `b`.
        t |> success(!match_glob("a/**/b", "a/xb"))
        t |> success(!match_glob("**/foo.txt", "afoo.txt"))
        t |> success(!match_glob("assets/**/*.png", "assetsicon.png"))
        // Zero-segment path still works (empty `**` match).
        t |> success(match_glob("**/foo.txt", "foo.txt"))
        t |> success(match_glob("a/**/b", "a/b"))
    }
}

[test(description = "A question mark matches exactly one non-slash character")]
def test_match_glob_question(t : T?) {
    t |> run("? matches exactly one non-/ char") @(t : T?) {
        t |> success(match_glob("?ello", "hello"))
        t |> success(match_glob("h?llo", "hello"))
        t |> success(match_glob("hell?", "hello"))
        t |> success(!match_glob("?ello", "ello"))         // zero chars
        t |> success(!match_glob("?ello", "hhello"))       // two chars
        t |> success(!match_glob("?", "/"))                // ? does not match /
    }
}

[test(description = "Character classes match lists, ranges, negations and literals the way POSIX fnmatch does")]
def test_match_glob_charclass(t : T?) {
    t |> run("[abc] matches any listed char") @(t : T?) {
        t |> success(match_glob("[abc]ar", "bar"))
        t |> success(match_glob("[abc]ar", "aar"))
        t |> success(match_glob("[abc]ar", "car"))
        t |> success(!match_glob("[abc]ar", "dar"))
    }
    t |> run("[a-z] matches a range") @(t : T?) {
        t |> success(match_glob("[a-z]", "m"))
        t |> success(match_glob("[a-z]", "a"))
        t |> success(match_glob("[a-z]", "z"))
        t |> success(!match_glob("[a-z]", "A"))
        t |> success(!match_glob("[a-z]", "0"))
        t |> success(match_glob("[0-9][0-9]", "42"))
    }
    t |> run("[!abc] / [^abc] negates") @(t : T?) {
        t |> success(match_glob("[!abc]", "x"))
        t |> success(!match_glob("[!abc]", "a"))
        t |> success(match_glob("[^abc]", "x"))
        t |> success(!match_glob("[^abc]", "b"))
    }
    t |> run("unterminated [ matches as a literal (POSIX fnmatch)") @(t : T?) {
        // POSIX: a `[` with no closing `]` is treated as a literal `[`.
        t |> success(match_glob("[abc", "[abc"))
        t |> success(match_glob("[", "["))
        t |> success(!match_glob("[", "x"))
        t |> success(!match_glob("[abc", "abc"))
    }
    t |> run("] right after [ or [! is a literal class member") @(t : T?) {
        // POSIX: the first `]` after `[` (or `[!` / `[^`) is in the class,
        // not the terminator.
        t |> success(match_glob("[]ab]", "]"))
        t |> success(match_glob("[]ab]", "a"))
        t |> success(!match_glob("[]ab]", "c"))
        t |> success(match_glob("[!]ab]", "x"))
        t |> success(!match_glob("[!]ab]", "]"))
    }
    t |> run("class can hold metacharacters as literals") @(t : T?) {
        // No backslash escape mechanism — `[*]` is the documented way to
        // match a literal `*` (and likewise for `?` and `[`).
        t |> success(match_glob("[*]", "*"))
        t |> success(!match_glob("[*]", "a"))
        t |> success(match_glob("[?]", "?"))
        t |> success(!match_glob("[?]", "a"))
    }
    t |> run("- at start or end of class is a literal") @(t : T?) {
        // POSIX: a `-` adjacent to `[` / `[!` / `]` is a literal `-`,
        // not a range marker.
        t |> success(match_glob("[a-]", "a"))
        t |> success(match_glob("[a-]", "-"))
        t |> success(!match_glob("[a-]", "b"))
        t |> success(match_glob("[-a]", "-"))
        t |> success(match_glob("[-a]", "a"))
    }
}

[test(description = "match_glob handles empty patterns and empty paths")]
def test_match_glob_edge_cases(t : T?) {
    t |> run("empty patterns and paths") @(t : T?) {
        t |> success(match_glob("", ""))
        t |> success(!match_glob("", "x"))
        t |> success(!match_glob("x", ""))
        t |> success(match_glob("*", ""))
        t |> success(match_glob("**", ""))
    }
}

[test(description = "is_glob_pattern detects glob metacharacters")]
def test_is_glob_pattern(t : T?) {
    t |> run("detects glob metacharacters") @(t : T?) {
        t |> success(is_glob_pattern("*.txt"))
        t |> success(is_glob_pattern("**/foo"))
        t |> success(is_glob_pattern("file?.txt"))
        t |> success(is_glob_pattern("[abc].das"))
        t |> success(!is_glob_pattern("plain.txt"))
        t |> success(!is_glob_pattern(""))
        t |> success(!is_glob_pattern("a/b/c.das"))
    }
}

def glob_fixture_tree(t : T?; root : string) {
    mkdir_rec("{root}/assets/sub")
    mkdir_rec("{root}/src")
    mkdir_rec("{root}/tests/inner")
    t |> success(fwrite("{root}/main.das", "// main\n"))
    t |> success(fwrite("{root}/README.md", "# readme\n"))
    t |> success(fwrite("{root}/assets/icon.png", "PNG"))
    t |> success(fwrite("{root}/assets/banner.jpg", "JPG"))
    t |> success(fwrite("{root}/assets/sub/deep.png", "PNG"))
    t |> success(fwrite("{root}/src/util.das", "// util\n"))
    t |> success(fwrite("{root}/tests/test_main.das", "// test\n"))
    t |> success(fwrite("{root}/tests/inner/test_inner.das", "// inner\n"))
}

def glob_cleanup_tree(root : string) {
    var error : string
    rmdir_rec(root, error)
}

[test(description = "glob and glob_filtered walk the file system, honor excludes and yield slash-separated paths")]
def test_glob_filesystem(t : T?) { // nolint:STYLE038 - flat list of independent glob arms over one fixture tree
    let tmp = create_temp_directory_result("glob_test_")
    if (!(tmp is value)) {
        t |> failure("could not create temp directory: {tmp as error}")
        return
    }
    let root = tmp as value
    glob_fixture_tree(t, root)
    t |> run("glob picks PNG files anywhere") @(t : T?) {
        var hits : array<string>
        glob(root, "**/*.png") $(filename, is_dir) {
            if (!is_dir) {
                hits |> push_clone(filename)
            }
        }
        t |> equal(length(hits), 2)
    }
    t |> run("glob picks .das files in src only") @(t : T?) {
        var hits : array<string>
        glob(root, "src/*.das") $(filename, is_dir) {
            if (!is_dir) {
                hits |> push_clone(filename)
            }
        }
        t |> equal(length(hits), 1)
    }
    t |> run("glob with ** picks all .das files") @(t : T?) {
        var hits : array<string>
        glob(root, "**/*.das") $(filename, is_dir) {
            if (!is_dir) {
                hits |> push_clone(filename)
            }
        }
        t |> equal(length(hits), 4)
    }
    t |> run("glob_filtered honors excludes") @(t : T?) {
        var hits, includes, excludes : array<string>
        includes |> push_clone("**/*.das")
        excludes |> push_clone("tests/**")
        glob_filtered(root, includes, excludes) $(filename, is_dir) {
            if (!is_dir) {
                hits |> push_clone(filename)
            }
        }
        t |> equal(length(hits), 2)
    }
    t |> run("glob callback always yields /-separated paths") @(t : T?) {
        // Round-2 fix: glob must invoke `blk` with the normalized path,
        // not the OS-native one. On macOS/Linux this is a tautology; on
        // Windows it's the load-bearing assertion.
        glob(root, "**/*.das") $(filename, is_dir) {
            t |> success(find(filename, "\\") < 0,
                "callback path '{filename}' must not contain backslashes")
        }
    }
    t |> run("glob_filtered callback always yields /-separated paths") @(t : T?) {
        var includes : array<string>
        let excludes : array<string>
        includes |> push_clone("**/*.das")
        glob_filtered(root, includes, excludes) $(filename, is_dir) {
            t |> success(find(filename, "\\") < 0,
                "callback path '{filename}' must not contain backslashes")
        }
    }
    t |> run("glob on non-existent root yields nothing, no error") @(t : T?) {
        // dir_rec on a missing path silently no-ops via the non-erroring
        // overload; glob/glob_filtered piggyback on that. Verify the contract:
        // no panic, no callback invocations.
        var hits : array<string>
        glob("{root}/_no_such_dir_{get_clock()}", "**/*") $(filename, is_dir) {
            hits |> push_clone(filename)
        }
        t |> equal(length(hits), 0)
    }
    t |> run("glob_filtered with empty includes yields nothing") @(t : T?) {
        // An empty include list means "match nothing", so the callback must
        // not fire even when entries exist under root.
        var hits : array<string>
        let includes, excludes : array<string>
        glob_filtered(root, includes, excludes) $(filename, is_dir) {
            hits |> push_clone(filename)
        }
        t |> equal(length(hits), 0)
    }
    glob_cleanup_tree(root)
}

[test(description = "to_generic_path turns backslashes into forward slashes and leaves generic paths alone")]
def test_to_generic_path(t : T?) {
    t |> run("backslashes become forward slashes") @(t : T?) {
        t |> equal(to_generic_path("a\\b\\c"), "a/b/c")
        t |> equal(to_generic_path("a/b\\c/d"), "a/b/c/d")
    }
    t |> run("already-generic paths pass through") @(t : T?) {
        t |> equal(to_generic_path("a/b/c"), "a/b/c")
        t |> equal(to_generic_path("plain"), "plain")
        t |> equal(to_generic_path(""), "")
    }
}

def make_fixture_tree(t : T?; root : string) {
    mkdir_rec("{root}/sub")
    mkdir_rec("{root}/sub/deep")
    mkdir_rec("{root}/empty_subdir")
    mkdir_rec("{root}/other")
    t |> success(fwrite("{root}/foo.das", "// foo\n"))
    t |> success(fwrite("{root}/bar.das", "// bar\n"))
    t |> success(fwrite("{root}/baz.txt", "baz"))
    t |> success(fwrite("{root}/sub/inner.das", "// inner\n"))
    t |> success(fwrite("{root}/sub/deep/deeper.das", "// deeper\n"))
    t |> success(fwrite("{root}/other/o1.das", "// o1\n"))
}

def cleanup_tree(root : string) {
    var error : string
    rmdir_rec(root, error)
}

def has_path(files : array<string>; suffix : string) : bool {
    for (f in files) {
        if (find(f, suffix) >= 0) {
            return true
        }
    }
    return false
}

[test(description = "A single plain file passes through parse_file_list unchanged")]
def test_parse_file_list_single_plain(t : T?) {
    t |> run("single plain file passes through unchanged") @(t : T?) {
        var files : array<string>
        parse_file_list("path/to/file.das", files)
        t |> equal(length(files), 1)
        t |> equal(files[0], "path/to/file.das")
    }
}

[test(description = "A single glob expands to its matches")]
def test_parse_file_list_single_glob(t : T?) {
    let tmp = create_temp_directory_result("expand_glob_single_")
    if (!(tmp is value)) {
        t |> failure("could not create temp directory: {tmp as error}")
        return
    }
    let root = tmp as value
    make_fixture_tree(t, root)
    t |> run("single glob expands to matches") @(t : T?) {
        var files : array<string>
        parse_file_list("{root}/*.das", files)
        t |> success(length(files) >= 2, "at least foo.das and bar.das")
        t |> success(has_path(files, "foo.das"), "foo.das matched")
        t |> success(has_path(files, "bar.das"), "bar.das matched")
    }
    cleanup_tree(root)
}

[test(description = "A comma-separated list of plain files is split")]
def test_parse_file_list_comma_plain(t : T?) {
    t |> run("comma list of plain files") @(t : T?) {
        var files : array<string>
        parse_file_list("a.das,b.das,c.das", files)
        t |> equal(length(files), 3)
        t |> equal(files[0], "a.das")
        t |> equal(files[1], "b.das")
        t |> equal(files[2], "c.das")
    }
}

[test(description = "A comma-separated list of globs expands each glob")]
def test_parse_file_list_comma_globs(t : T?) {
    let tmp = create_temp_directory_result("expand_glob_comma_")
    if (!(tmp is value)) {
        t |> failure("could not create temp directory: {tmp as error}")
        return
    }
    let root = tmp as value
    make_fixture_tree(t, root)
    t |> run("comma list of globs (issue #2498 repro)") @(t : T?) {
        var files : array<string>
        parse_file_list("{root}/*.das,{root}/other/*.das", files)
        t |> success(has_path(files, "foo.das"), "first glob expanded")
        t |> success(has_path(files, "o1.das"), "second glob expanded")
    }
    cleanup_tree(root)
}

[test(description = "A mixed list of plain files and globs expands in place")]
def test_parse_file_list_mixed(t : T?) {
    let tmp = create_temp_directory_result("expand_glob_mixed_")
    if (!(tmp is value)) {
        t |> failure("could not create temp directory: {tmp as error}")
        return
    }
    let root = tmp as value
    make_fixture_tree(t, root)
    t |> run("mixed plain + glob") @(t : T?) {
        var files : array<string>
        parse_file_list("literal.das,{root}/foo.das,{root}/sub/*.das", files)
        t |> success(has_path(files, "literal.das"), "plain entry preserved")
        t |> success(has_path(files, "foo.das"), "explicit file resolved")
        t |> success(has_path(files, "inner.das"), "glob expanded")
    }
    cleanup_tree(root)
}

[test(description = "A newline-delimited list is split")]
def test_parse_file_list_newline(t : T?) {
    t |> run("newline-delimited") @(t : T?) {
        var files : array<string>
        parse_file_list("a.das\nb.das\nc.das", files)
        t |> equal(length(files), 3)
        t |> equal(files[0], "a.das")
        t |> equal(files[1], "b.das")
        t |> equal(files[2], "c.das")
    }
}

[test(description = "Commas, newlines and whitespace all separate entries, and surrounding whitespace is stripped")]
def test_parse_file_list_whitespace(t : T?) {
    t |> run("mixed comma + newline + whitespace") @(t : T?) {
        var files : array<string>
        parse_file_list("  a.das , b.das \n c.das ", files)
        t |> equal(length(files), 3)
        t |> equal(files[0], "a.das")
        t |> equal(files[2], "c.das")
    }
    t |> run("single fast-path strips whitespace") @(t : T?) {
        var files : array<string>
        parse_file_list("  a.das  ", files)
        t |> equal(length(files), 1)
        t |> equal(files[0], "a.das")
    }
}

[test(description = "Empty and whitespace-only parts are skipped")]
def test_parse_file_list_empty_parts(t : T?) {
    t |> run("empty parts skipped") @(t : T?) {
        var files : array<string>
        parse_file_list("a.das,,\n,b.das", files)
        t |> equal(length(files), 2, "2 non-empty files")
    }
    t |> run("single fast-path skips empty/whitespace-only") @(t : T?) {
        var files : array<string>
        parse_file_list("   ", files)
        t |> equal(length(files), 0)
    }
}

[test(description = "expand_glob keeps earlier entries in order and does not re-sort globally")]
def test_parse_file_list_order_preserved(t : T?) {
    let tmp = create_temp_directory_result("expand_glob_order_")
    if (!(tmp is value)) {
        t |> failure("could not create temp directory: {tmp as error}")
        return
    }
    let root = tmp as value
    make_fixture_tree(t, root)
    t |> run("expand_glob preserves prior entries (no global re-sort)") @(t : T?) {
        // Regression Copilot caught: expand_glob's sort() once scrambled
        // entries pushed before it. Order must hold across (plain, glob, plain).
        var files : array<string>
        parse_file_list("zzz.das,{root}/*.das,aaa.das", files)
        t |> equal(files[0], "zzz.das", "first plain stays first")
        t |> equal(files[length(files) - 1], "aaa.das", "last plain stays last")
    }
    cleanup_tree(root)
}

[test(description = "A double star in the middle of a pattern walks from the literal prefix")]
def test_expand_glob_recursive(t : T?) {
    let tmp = create_temp_directory_result("expand_glob_recursive_")
    if (!(tmp is value)) {
        t |> failure("could not create temp directory: {tmp as error}")
        return
    }
    let root = tmp as value
    make_fixture_tree(t, root)
    t |> run("** in middle of pattern walks from literal prefix") @(t : T?) {
        // Pattern with `**` must walk from the literal prefix, not from a
        // literal `**` directory that does not exist.
        var files : array<string>
        parse_file_list("{root}/**/inner.das", files)
        t |> success(has_path(files, "inner.das"))
    }
    cleanup_tree(root)
}

[test(description = "A shallow pattern does not descend into subdirectories")]
def test_expand_glob_shallow(t : T?) {
    let tmp = create_temp_directory_result("expand_glob_shallow_")
    if (!(tmp is value)) {
        t |> failure("could not create temp directory: {tmp as error}")
        return
    }
    let root = tmp as value
    make_fixture_tree(t, root)
    t |> run("shallow pattern does not descend into subdirectories") @(t : T?) {
        // Patterns without `**` use the non-recursive walker. `{root}/*.das`
        // must NOT pick up files under `{root}/sub/` or `{root}/other/`.
        var files : array<string>
        parse_file_list("{root}/*.das", files)
        for (f in files) {
            t |> success(find(f, "/sub/") < 0, "shallow * did not descend into sub/")
            t |> success(find(f, "/other/") < 0, "shallow * did not descend into other/")
        }
    }
    cleanup_tree(root)
}

[test(description = "A multi-segment pattern without a double star still recurses through its segments")]
def test_expand_glob_multi_segment(t : T?) {
    let tmp = create_temp_directory_result("expand_glob_multiseg_")
    if (!(tmp is value)) {
        t |> failure("could not create temp directory: {tmp as error}")
        return
    }
    let root = tmp as value
    make_fixture_tree(t, root)
    t |> run("multi-segment pattern without ** still recurses") @(t : T?) {
        // A pattern like `{root}/*/inner.das` has no `**` but it crosses
        // a `/`, so the recursive walk must be used.
        var files : array<string>
        parse_file_list("{root}/*/inner.das", files)
        t |> success(has_path(files, "inner.das"),
            "multi-segment pattern matched sub/inner.das")
    }
    cleanup_tree(root)
}

[test(description = "A backslash-style pattern matches the same files as a forward-slash one")]
def test_expand_glob_backslash(t : T?) {
    let tmp = create_temp_directory_result("expand_glob_backslash_")
    if (!(tmp is value)) {
        t |> failure("could not create temp directory: {tmp as error}")
        return
    }
    let root = tmp as value
    make_fixture_tree(t, root)
    t |> run("backslash-style pattern matches the same as forward-slash") @(t : T?) {
        // The pattern is normalized to `/` inside expand_glob, so backslash
        // patterns produce the same result as POSIX `/` patterns. Build the
        // backslash pattern by replacing `/` with `\` in the same input.
        let fwd = "{root}/sub/*.das"
        let back = replace(fwd, "/", "\\")
        var slash, bs : array<string>
        parse_file_list(fwd, slash)
        parse_file_list(back, bs)
        sort(slash)
        sort(bs)
        t |> equal(length(slash), length(bs), "same number of matches")
        for (i in range(length(slash))) {
            t |> equal(slash[i], bs[i], "entry {i} matches")
        }
    }
    cleanup_tree(root)
}

[test(description = "An absolute pattern walks its own root and produces clean paths")]
def test_expand_glob_absolute(t : T?) {
    let tmp = create_temp_directory_result("expand_glob_abs_")
    if (!(tmp is value)) {
        t |> failure("could not create temp directory: {tmp as error}")
        return
    }
    let root = tmp as value
    make_fixture_tree(t, root)
    t |> run("absolute path patterns walk the root, not cwd") @(t : T?) {
        // _split_literal_prefix keeps the separator in dir_part when the
        // pattern starts at the filesystem root, so glob walks the right
        // place rather than an empty path.
        var files : array<string>
        parse_file_list("{root}/*.das", files)
        t |> success(!empty(files), "absolute pattern matched at least one file")
        t |> success(has_path(files, "foo.das"), "foo.das present")
    }
    t |> run("absolute root prefix produces clean paths (no double slash)") @(t : T?) {
        // When dir_part ends with `/` (POSIX root or Windows drive root),
        // expand_glob must not insert another `/` when concatenating with
        // the matched entry.
        var files : array<string>
        parse_file_list("{root}/*.das", files)
        for (f in files) {
            t |> success(find(f, "//") < 0, "no double slashes in '{f}'")
        }
    }
    cleanup_tree(root)
}

[test(description = "A character class triggers glob expansion")]
def test_expand_glob_charclass(t : T?) {
    let tmp = create_temp_directory_result("expand_glob_charclass_")
    if (!(tmp is value)) {
        t |> failure("could not create temp directory: {tmp as error}")
        return
    }
    let root = tmp as value
    make_fixture_tree(t, root)
    t |> run("character class triggers glob expansion") @(t : T?) {
        // parse_file_list dispatches via fio.is_glob_pattern, which
        // recognizes `[` (not just `*` / `?`).
        var files : array<string>
        parse_file_list("{root}/[fb]*.das", files)
        t |> success(has_path(files, "foo.das"), "[fb]* matched foo.das")
        t |> success(has_path(files, "bar.das"), "[fb]* matched bar.das")
    }
    cleanup_tree(root)
}

[test(description = "A shallow expand_glob filters out directories")]
def test_expand_glob_filters_dirs(t : T?) {
    let tmp = create_temp_directory_result("expand_glob_dirs_")
    if (!(tmp is value)) {
        t |> failure("could not create temp directory: {tmp as error}")
        return
    }
    let root = tmp as value
    make_fixture_tree(t, root)
    t |> run("shallow expand_glob filters out directories") @(t : T?) {
        // The shallow `dir(...)` branch must stat each match and skip dirs
        // — downstream tools (format/lint/compile_check) take regular files.
        var files : array<string>
        parse_file_list("{root}/*", files)
        for (f in files) {
            let st = stat(f)
            t |> success(!(st.is_valid && st.is_dir),
                "expand_glob must not emit directory '{f}'")
        }
    }
    cleanup_tree(root)
}
