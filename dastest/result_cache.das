options gen2
options indenting = 4

module result_cache shared

require daslib/fio
require daslib/option
require daslib/result
require daslib/strings_boost
require daslib/strings_convert
require strings

require suite_result public

var private content_hashes : table<string; string>

def private content_hash(path : string) : string {
    let known = content_hashes?[path] ?? ""
    return known if (!empty(known))
    var st : FStat
    let digest = stat(path, st) && st.is_reg ? "{hash(fread(path))}" : "-"
    content_hashes[path] = digest
    return digest
}

let private REPORT_ONLY_FLAGS <- {
    "--isolated-mode", "--failures-only", "--uri-paths", "--verbose", "--color", "--no-color"
}

let private REPORT_ONLY_OPTIONS <- {
    "--test", "--exclude", "--test-list", "--test-only", "--json-file", "--timeout", "--timing-outliers",
    "--max-file-time", "--isolated-mode-threads", "--batch", "--test-timeout", "--worker-index", "--cache-dir"
}

def cache_key_args(args : array<string>) : string {
    return build_string() $(var w) {
        var i = 0
        while (i < length(args)) {
            let a = args[i]
            if (REPORT_ONLY_OPTIONS |> key_exists(a)) {
                i += 2
                continue
            }
            if (!(REPORT_ONLY_FLAGS |> key_exists(a))) {
                w |> write(a)
                w |> write(" ")
            }
            i ++
        }
    }
}

def runner_inputs(dastest_root : string) : array<string> {
    var inputs : array<string>
    dir(dastest_root) $(name) {
        inputs |> push(path_join(dastest_root, name)) if (ends_with(name, ".das"))
    }
    return <- inputs
}

def cache_entry(cache_dir : string; file : string; key_args : string) : string {
    let key = "{normalize(file)}|{key_args}"
    return path_join(cache_dir, "{hash(key)}")
}

def private parse_counts(line : string) : Option<SuiteResult> {
    let parts <- split(line, " ")
    return none(type<SuiteResult>) if (length(parts) != 4 || parts[0] != "counts")
    let total = try_to_int(parts[1])
    let passed = try_to_int(parts[2])
    let skipped = try_to_int(parts[3])
    return none(type<SuiteResult>) if (is_err(total) || is_err(passed) || is_err(skipped))
    return some(SuiteResult(total = unwrap(total), passed = unwrap(passed), skipped = unwrap(skipped)))
}

def cache_lookup(entry : string) : Option<SuiteResult> {
    let text = fread(entry)
    return none(type<SuiteResult>) if (empty(text))
    let lines <- split(text, "\n")
    let counts = parse_counts(lines[0])
    return none(type<SuiteResult>) if (is_none(counts))
    for (i in range(1, length(lines))) {
        let line = lines[i]
        continue if (empty(line))
        let sp = find(line, " ")
        return none(type<SuiteResult>) if (sp < 0 || content_hash(slice(line, sp + 1)) != slice(line, 0, sp))
    }
    return some(unwrap(counts))
}

def cache_store(entry : string; res : SuiteResult; inputs : array<string>) {
    mkdir_rec(dir_name(entry))
    fwrite(entry, build_string() $(var w) {
        w |> write("counts {res.total} {res.passed} {res.skipped}\n")
        for (path in inputs) {
            w |> write("{content_hash(path)} {path}\n")
        }
    })
}
