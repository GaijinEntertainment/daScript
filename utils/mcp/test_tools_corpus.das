options gen2
options rtti

require dastest/testing_boost public
require daslib/json
require daslib/json_boost
require strings
require daslib/fio
require _tools_common

require tools/detect_duplicates public
require tools/export_corpus public

def make_temp_text_file(t : T?; prefix : string; ext : string; body : string) : string {
    let r = create_temp_file_result(prefix, ext)
    if (!(r is value)) {
        t |> failure("create_temp_file_result failed")
        return ""
    }
    let path = unsafe(r.value)
    fopen(path, "wb") $(fw) {
        if (fw != null) {
            fw |> fprint(body)
        }
    }
    return path
}

def make_temp_corpus_file(t : T?; body : string) : string =>
    make_temp_text_file(t, "detect_dupe_mcp_test", ".json", body)

def make_temp_paths_file(t : T?; content : string) : string =>
    make_temp_text_file(t, "export_corpus_paths", ".txt", content)

[test(description = "detect_duplicates rejects a call without a corpus argument.")]
def test_detect_duplicates_missing_corpus(t : T?) {
    t |> run("missing 'corpus' arg returns error envelope") <| @(t : T?) {
        var text : string
        var is_error = false
        let ok = parse_result(do_detect_duplicates("foo.das", "", "", "", "", ""), text, is_error)
        t |> success(ok, "parse result JSON")
        t |> success(is_error, "should be error")
        t |> success(find(text, "corpus") >= 0, "should mention corpus: {text}")
    }
}

[test(description = "detect_duplicates rejects a call without a paths argument.")]
def test_detect_duplicates_missing_paths(t : T?) {
    t |> run("missing 'paths' arg returns error envelope") <| @(t : T?) {
        let path = make_temp_corpus_file(t, "\{\"schema_version\":3,\"functions\":[]\}")
        if (empty(path)) return
        var text : string
        var is_error = false
        let ok = parse_result(do_detect_duplicates("", path, "", "", "", ""), text, is_error)
        t |> success(ok, "parse result JSON")
        t |> success(is_error, "should be error")
        t |> success(find(text, "paths") >= 0, "should mention paths: {text}")
        remove(path)
    }
}

[test(description = "detect_duplicates reports a corpus file that does not exist.")]
def test_detect_duplicates_corpus_not_found(t : T?) {
    t |> run("nonexistent corpus path is reported") <| @(t : T?) {
        let bogus = make_temp_corpus_file(t, "")
        if (empty(bogus)) return
        remove(bogus)
        var text : string
        var is_error = false
        let ok = parse_result(do_detect_duplicates(fixture_path("_fixture_valid.das"),
            bogus, "", "", "", ""), text, is_error)
        t |> success(ok, "parse result JSON")
        t |> success(is_error, "should be error")
    }
}

[test(description = "detect_duplicates against an empty corpus returns an envelope with exit code 0 and an embedded report.", timeout = 1200)]
def test_detect_duplicates_empty_corpus(t : T?) {
    t |> run("empty corpus + valid candidate yields envelope with embedded report") <| @(t : T?) {
        let path = make_temp_corpus_file(t, "\{\"schema_version\":3,\"functions\":[]\}")
        if (empty(path)) return
        var text : string
        var is_error = false
        let ok = parse_result(do_detect_duplicates(fixture_path("_fixture_valid.das"),
            path, "", "", "", ""), text, is_error)
        t |> success(ok, "parse result JSON")
        t |> success(!is_error, "should NOT be error: {text}")
        t |> success(find(text, "\"exit_code\":0") >= 0, "envelope has exit_code 0: {text}")
        t |> success(find(text, "\"report\"") >= 0, "envelope has nested report: {text}")
        t |> success(find(text, "candidates") >= 0, "report contains candidates array: {text}")
        remove(path)
    }
}

[test(description = "detect_duplicates rejects a corpus with an unsupported schema_version.", timeout = 1200)]
def test_detect_duplicates_bad_schema(t : T?) {
    t |> run("corpus with bad schema_version is rejected") <| @(t : T?) {
        let path = make_temp_corpus_file(t, "\{\"schema_version\":99,\"functions\":[]\}")
        if (empty(path)) return
        var text : string
        var is_error = false
        let ok = parse_result(do_detect_duplicates(fixture_path("_fixture_valid.das"),
            path, "", "", "", ""), text, is_error)
        t |> success(ok, "parse result JSON")
        t |> success(is_error, "should be error")
        t |> success(find(text, "schema_version") >= 0, "mentions schema_version: {text}")
        remove(path)
    }
}

[test(description = "export_corpus rejects a call with neither paths nor a paths file.")]
def test_export_corpus_missing_paths(t : T?) {
    t |> run("missing 'paths' / 'paths_file' returns error envelope") <| @(t : T?) {
        var text : string
        var is_error = false
        let ok = parse_result(do_export_corpus("", "", "export_corpus_test_out.json", "", ""), text, is_error)
        t |> success(ok, "parse result JSON")
        t |> success(is_error, "should be error")
        t |> success(find(text, "paths") >= 0, "should mention paths: {text}")
    }
}

[test(description = "export_corpus rejects a call without an output path.")]
def test_export_corpus_missing_out(t : T?) {
    t |> run("missing 'out' arg returns error envelope") <| @(t : T?) {
        var text : string
        var is_error = false
        let ok = parse_result(do_export_corpus("foo.das", "", "", "", ""), text, is_error)
        t |> success(ok, "parse result JSON")
        t |> success(is_error, "should be error")
        t |> success(find(text, "out") >= 0, "should mention out: {text}")
    }
}

[test(description = "export_corpus reports paths that match nothing as an error.")]
def test_export_corpus_no_match(t : T?) {
    t |> run("paths matching nothing yields error envelope") <| @(t : T?) {
        var text : string
        var is_error = false
        let ok = parse_result(do_export_corpus("does_not_exist_xyzzy.das", "", "export_corpus_test_out.json", "", ""), text, is_error)
        t |> success(ok, "parse result JSON")
        t |> success(is_error, "should be error: {text}")
    }
}

[test(description = "export_corpus writes a corpus file and returns an envelope with exit code, output path and subprocess stdout.", timeout = 1200)]
def test_export_corpus_writes_envelope(t : T?) {
    t |> run("valid fixture produces a corpus and a populated envelope") <| @(t : T?) {
        let r = create_temp_file_result("export_corpus_mcp_test", ".json")
        if (!(r is value)) {
            t |> failure("create_temp_file_result failed")
            return
        }
        let out_path = unsafe(r.value)
        var text : string
        var is_error = false
        let ok = parse_result(do_export_corpus(fixture_path("_fixture_valid.das"), "", out_path, "", ""), text, is_error)
        t |> success(ok, "parse result JSON")
        t |> success(!is_error, "should NOT be error: {text}")
        t |> success(find(text, "\"exit_code\":0") >= 0, "envelope has exit_code 0: {text}")
        t |> success(find(text, "\"out\":") >= 0, "envelope echoes out path: {text}")
        t |> success(find(text, "\"stdout\":") >= 0, "envelope has subprocess stdout: {text}")
        let body = fread(out_path)
        t |> success(!empty(body), "corpus file is non-empty")
        t |> success(find(body, "\"schema_version\"") >= 0, "corpus has schema_version: {body}")
        remove(out_path)
    }
}

[test(description = "export_corpus builds a corpus from a paths file alone.", timeout = 1200)]
def test_export_corpus_paths_file(t : T?) {
    t |> run("paths_file alone produces a corpus") <| @(t : T?) {
        let paths_body = "{fixture_path("_fixture_valid.das")}\n"
        let paths_file = make_temp_paths_file(t, paths_body)
        if (empty(paths_file)) return
        let r = create_temp_file_result("export_corpus_paths_file", ".json")
        if (!(r is value)) {
            t |> failure("create_temp_file_result failed")
            remove(paths_file)
            return
        }
        let out_path = unsafe(r.value)
        var text : string
        var is_error = false
        let ok = parse_result(do_export_corpus("", paths_file, out_path, "", ""), text, is_error)
        t |> success(ok, "parse result JSON")
        t |> success(!is_error, "should NOT be error: {text}")
        let body = fread(out_path)
        t |> success(!empty(body), "corpus file is non-empty")
        t |> success(find(body, "\"schema_version\"") >= 0, "corpus has schema_version")
        t |> success(find(body, "_fixture_valid.das") >= 0, "corpus references the fixture file")
        remove(paths_file)
        remove(out_path)
    }
}

[test(description = "export_corpus unions paths and a paths file without duplicating records.", timeout = 1200)]
def test_export_corpus_paths_and_paths_file_union(t : T?) {
    t |> run("paths and paths_file unioned (no duplicate records)") <| @(t : T?) {
        let paths_body = "{fixture_path("_fixture_test.das")}\n"
        let paths_file = make_temp_paths_file(t, paths_body)
        if (empty(paths_file)) return
        let r = create_temp_file_result("export_corpus_union", ".json")
        if (!(r is value)) {
            t |> failure("create_temp_file_result failed")
            remove(paths_file)
            return
        }
        let out_path = unsafe(r.value)
        var text : string
        var is_error = false
        let ok = parse_result(
            do_export_corpus(fixture_path("_fixture_valid.das"), paths_file,
                             out_path, "", ""),
            text, is_error)
        t |> success(ok, "parse result JSON")
        t |> success(!is_error, "should NOT be error: {text}")
        let body = fread(out_path)
        var jerr : string
        var parsed = read_json(body, jerr)
        t |> success(parsed != null, "envelope parses: {jerr}")
        if (parsed == null) {
            remove(paths_file)
            remove(out_path)
            return
        }
        let funcs = parsed?["functions"]
        t |> success(funcs != null && funcs.value is _array, "functions array present")
        var seen : table<string; int>
        var have_valid = false
        var have_test = false
        if (funcs != null && funcs.value is _array) {
            for (ef in funcs.value as _array) {
                if (ef == null) continue
                var f = ""
                var n = ""
                var ln = 0
                let fv = ef?["file"]
                if (fv != null && fv.value is _string) {
                    f = fv.value as _string
                }
                let nv = ef?["name"]
                if (nv != null && nv.value is _string) {
                    n = nv.value as _string
                }
                let lv = ef?["line"]
                if (lv != null && lv.value is _number) {
                    ln = int(lv.value as _number)
                }
                let key = "{f}:{n}:{ln}"
                seen[key] = (seen?[key] ?? 0) + 1
                if (find(f, "_fixture_valid.das") >= 0) {
                    have_valid = true
                }
                if (find(f, "_fixture_test.das") >= 0) {
                    have_test = true
                }
            }
        }
        t |> success(have_valid, "valid fixture contributed records")
        t |> success(have_test, "test fixture contributed records")
        var dupes = 0
        for (count in values(seen)) {
            if (count > 1) {
                dupes++
            }
        }
        t |> equal(dupes, 0)
        unsafe { delete parsed; }
        remove(paths_file)
        remove(out_path)
    }
}

[test(description = "export_corpus accepts a workers count and runs the sequential path.", timeout = 1200)]
def test_export_corpus_workers_one(t : T?) {
    t |> run("workers param parses and runs the sequential path") <| @(t : T?) {
        let r = create_temp_file_result("export_corpus_w1", ".json")
        if (!(r is value)) {
            t |> failure("create_temp_file_result failed")
            return
        }
        let out_path = unsafe(r.value)
        var text : string
        var is_error = false
        let ok = parse_result(
            do_export_corpus(fixture_path("_fixture_valid.das"), "",
                             out_path, "", "1"),
            text, is_error)
        t |> success(ok, "parse result JSON")
        t |> success(!is_error, "should NOT be error: {text}")
        let body = fread(out_path)
        t |> success(!empty(body), "corpus non-empty")
        t |> success(find(body, "\"schema_version\"") >= 0, "schema_version present")
        remove(out_path)
    }
}

[test(description = "export_corpus still writes a corpus when an input has a syntax error, even with workers.", timeout = 1200)]
def test_export_corpus_workers_parse_failure_still_exports(t : T?) {
    t |> run("syntax error in input still exports the survivors, even with workers") <| @(t : T?) {
        let paths_body = "{fixture_path("_fixture_syntax_error.das")}\n"
        let paths_file = make_temp_paths_file(t, paths_body)
        if (empty(paths_file)) return
        let r = create_temp_file_result("export_corpus_fail", ".json")
        if (!(r is value)) {
            t |> failure("create_temp_file_result failed")
            remove(paths_file)
            return
        }
        let out_path = unsafe(r.value)
        var text : string
        var is_error = false
        let ok = parse_result(
            do_export_corpus("", paths_file, out_path, "", "1"),
            text, is_error)
        t |> success(ok, "parse result JSON")
        t |> success(!is_error, "a parse failure is not a failed run: {text}")
        t |> success(find(text, "failed to parse") >= 0, "the envelope's stdout carries the failure count: {text}")
        t |> success(stat(out_path).is_valid, "the corpus file is written")
        remove(paths_file)
        remove(out_path)
    }
}
