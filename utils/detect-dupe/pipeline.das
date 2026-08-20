options gen2

require daslib/ast
require daslib/ast_boost
require daslib/fio
require daslib/strings_boost
require strings
require canonical
require cluster
require exchange
require patterns





def public read_path_lines(f : FILE const?; var paths : array<string>) {
    if (f == null) return
    while (!feof(f)) {
        let line = fgets(f) |> strip
        if (empty(line) || line |> starts_with("#")) continue
        paths |> push(line)
    }
}


def public read_path_lines_from_file(path : string; var paths : array<string>;
                                     var error : string&) : bool {
    var ok = false
    fopen(path, "rb") $(f) {
        if (f == null) return
        ok = true
        read_path_lines(f, paths)
    }
    if (!ok) {
        error = "could not open paths-from file: {path}"
    }
    return ok
}


def public is_daslib_skip(path : string; basename : string) : bool {
    let normalized = normalize_path(path)
    let prefix = "daslib/{basename}"
    return normalized == prefix || normalized |> ends_with("/{prefix}")
}


//! Files no scan should compile: builtin.das, daslib debugger/profiler
//! (thread-local agents abort a second in-process install), and the
//! ast-fuzz selftest victims (deliberately broken; test_ast_fuzz.das
//! owns them — same exclusion as CI's ast-verify sweep).
def public is_skip_file(path : string; basename : string) : bool {
    if (basename == "builtin.das" || normalize_path(path) |> contains("ast-fuzz/selftest/")) return true
    return (is_daslib_skip(path, "debugger.das")
        || is_daslib_skip(path, "profiler.das"))
}


def public is_skip_dir(name : string) : bool {
    return name |> starts_with(".")
}


def public has_expect_directive(file : string) : bool {
    let text = fread(file)
    if (empty(text)) return false
    let lines <- text |> split("\n")
    for (line in lines) {
        let trimmed = line |> strip
        if (trimmed |> starts_with("expect ")) return true
    }
    return false
}


def public scan_das_files(path : string; var files : array<string>; var cache : table<string; void?>) {
    let st = stat(path)
    if (!st.is_valid) return
    if (st.is_reg) {
        if (path |> ends_with(".das")) {
            let np = normalize_path(path)
            if (!cache |> key_exists(np)) {
                let fname = base_name(np)
                if (!is_skip_file(np, fname)) {
                    cache |> insert(np, null)
                    files |> push(np)
                }
            }
        }
        return
    }
    if (!st.is_dir) return
    fio::dir(path) $(name) {
        if (name == "." || name == ".." || name |> starts_with("_") || is_skip_dir(name)) return
        let full = path_join(path, name)
        let fst = stat(full)
        if (!fst.is_valid) return
        if (fst.is_dir) {
            scan_das_files(full, files, cache)
        } elif (fst.is_reg && name |> ends_with(".das") && !is_skip_file(full, name)) {
            let np = normalize_path(full)
            if (!cache |> key_exists(np)) {
                cache |> insert(np, null)
                files |> push(np)
            }
        }
    }
}


def private collect_one_function(var func : FunctionPtr;
                                 source_norm : string;
                                 var records : array<FuncRecord>;
                                 var seen_loc : table<string; void?>;
                                 lambdas_only : bool;
                                 want_sigs : bool;
                                 min_tokens : int) {
    if (func.flags.builtIn) return
    if (lambdas_only) {
        if (!func.flags._lambda) return
    } else {
        if (func.flags.generated) return
    }
    let fn_file = (func.at.fileInfo != null) ? string(func.at.fileInfo.name) : ""
    if (normalize_path(fn_file) != source_norm) return
    let loc = "{fn_file}:{int(func.at.line)}"
    if (key_exists(seen_loc, loc)) return
    seen_loc |> insert(loc, null)
    let canon = canonicalize_function(func)
    if (empty(canon)) return
    var rec = FuncRecord()
    rec.ref.file = fn_file
    rec.ref.name = string(func.name)
    rec.ref.line = int(func.at.line)
    if (func.body != null) {
        rec.ref.end_line = int(func.body.at.last_line)
    } else {
        rec.ref.end_line = int(func.at.last_line)
    }
    rec.ref.is_lambda = func.flags._lambda
    rec.canonical = canon
    derive_sig_and_count(rec, want_sigs, min_tokens)
    records |> emplace(rec)
}


def public collect_from_program(program : ProgramPtr; source_file : string; var records : array<FuncRecord>; var seen_loc : table<string; void?>; lambdas_only : bool; want_sigs : bool; min_tokens : int) {
    let thisMod = get_this_module(program)
    let source_norm = normalize_path(source_file)
    if (!lambdas_only) {
        for_each_generic(thisMod, "") $(func) {
            collect_one_function(func, source_norm, records, seen_loc,
                                 lambdas_only, want_sigs, min_tokens)
        }
    }
    for_each_function(thisMod, "") $(func) {
        collect_one_function(func, source_norm, records, seen_loc,
                             lambdas_only, want_sigs, min_tokens)
    }
}


def public drop_records_under_files(var records : array<FuncRecord>;
                                    against_files : table<string; void?>) : int {
    var kept : array<FuncRecord>
    kept |> reserve(length(records))
    var dropped = 0
    for (r in records) {
        if (key_exists(against_files, normalize_path(r.ref.file))) {
            dropped++
            continue
        }
        kept |> push_clone(r)
    }
    delete records
    records <- kept
    return dropped
}


def public apply_pattern_filter(var records : array<FuncRecord>;
                                kept : table<string; void?>;
                                keep_all : bool;
                                verbose : bool) : table<string; int> {
    var skip_counts : table<string; int>
    if (keep_all) return <- skip_counts
    var kept_records : array<FuncRecord>
    kept_records |> reserve(length(records))
    for (r in records) {
        let hit = classify(r.ref.name, r.canonical)
        if (empty(hit.name) || key_exists(kept, hit.name)) {
            kept_records |> push_clone(r)
            continue
        }
        if (verbose) {
            to_log(LOG_INFO, "  pattern-skip [{hit.name}]: {r.ref.file}:{r.ref.line} {r.ref.name} ({hit.note})\n")
        }
        if (key_exists(skip_counts, hit.name)) {
            skip_counts[hit.name] = skip_counts[hit.name] + 1
        } else {
            skip_counts |> insert(hit.name, 1)
        }
    }
    delete records
    records <- kept_records
    return <- skip_counts
}


def public collect_candidate_refs(records : array<FuncRecord>) : array<MemberRef> {
    return <- [for (r in records); r.ref; where r.is_candidate]
}


def public compile_and_collect(file : string; var records : array<FuncRecord>;
                               var seen_loc : table<string; void?>;
                               verbose : bool; lambdas_only : bool;
                               want_sigs : bool; min_tokens : int;
                               quiet : bool = false;
                               project : string = "") : bool {
    var inscope access <- make_file_access(project)
    var ok_compile = true
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.ignore_shared_modules = true
            cop.export_all = true
            cop.version_2_syntax = true
            compile_file(file, access, unsafe(addr(mg)), cop) $(ok; program; issues) {
                if (!ok) {
                    ok_compile = false
                    if (!quiet) {
                        if (verbose) {
                            to_log(LOG_ERROR, "FAIL {file}\n  {issues}\n")
                        } else {
                            to_log(LOG_ERROR, "FAIL {file}\n")
                        }
                    }
                    return
                }
                collect_from_program(program, file, records, seen_loc, lambdas_only, want_sigs, min_tokens)
            }
        }
    }
    return ok_compile
}
