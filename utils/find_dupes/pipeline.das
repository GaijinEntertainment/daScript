options gen2

require daslib/ast
require daslib/ast_boost
require strings
require canonical
require cluster
require exchange


def public normalize_path(p : string) : string {
    return p |> replace("\\", "/")
}


// Walk one compiled program and append a FuncRecord per user
// function (lambdas included; only builtIn / generated skipped).
//
// Generic-template functions (e.g. dastest's `equal<T>`) get reified
// into every caller's module, so without filtering we see hundreds of
// copies of the same library helpers. The file-source filter drops
// any function whose `at.fileInfo` points outside the file we just
// compiled, leaving only what was actually written in that file.
def public collect_from_program(program : ProgramPtr; source_file : string; var records : array<FuncRecord>; var seen_loc : table<string; void?>; lambdas_only : bool; want_sigs : bool; min_tokens : int) {
    let thisMod = get_this_module(program)
    let source_norm = normalize_path(source_file)
    for_each_function(thisMod, "") $(func) {
        if (func.flags.builtIn) {
            return
        }
        // Lambdas are emitted as `generated` synthesized functions
        // (`__lambda_function_at_line_xxx`). Default mode skips them via
        // the generated filter (the dispatcher already references them
        // via ADDR). Lambda-only mode lets them through and drops the
        // top-level functions instead.
        if (lambdas_only) {
            if (!func.flags._lambda) {
                return
            }
        } else {
            if (func.flags.generated) {
                return
            }
        }
        let fn_file = (func.at.fileInfo != null) ? string(func.at.fileInfo.name) : ""
        // Normalize slash direction — Windows runners can mix `\` and
        // `/`, and a literal mismatch silently drops every function
        // from a file that compiled fine.
        if (normalize_path(fn_file) != source_norm) {
            return
        }
        // Generic templates get reified per type (e.g. `test_conversions`
        // reified for int8, uint16, uint, uint64). Each reification is a
        // distinct FunctionPtr but they all point at the same source
        // (file, line). Keep the first; the rest are reported as
        // duplicates against themselves which is just noise.
        let loc = "{fn_file}:{int(func.at.line)}"
        if (key_exists(seen_loc, loc)) {
            return
        }
        seen_loc |> insert(loc, null)
        let canon = canonicalize_function(func)
        if (empty(canon)) {
            return
        }
        var rec = FuncRecord()
        rec.ref.file = fn_file
        rec.ref.name = string(func.name)
        rec.ref.line = int(func.at.line)
        rec.ref.is_lambda = func.flags._lambda
        rec.canonical := canon
        derive_sig_and_count(rec, want_sigs, min_tokens)
        records |> emplace(rec)
    }
}


def public compile_and_collect(file : string; var records : array<FuncRecord>; var seen_loc : table<string; void?>; verbose : bool; lambdas_only : bool; want_sigs : bool; min_tokens : int) : bool {
    var inscope access <- make_file_access("")
    var ok_compile = true
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.ignore_shared_modules = true
            cop.export_all = true
            // Leave optimisations and infer-time folding ON: dastest macros
            // (e.g. unroll) need constant folding to compile, and folded-equal
            // expressions are exactly the kind of duplicate we want to catch.
            compile_file(file, access, unsafe(addr(mg)), cop) $(ok; program; issues) {
                if (!ok) {
                    ok_compile = false
                    if (verbose) {
                        print("FAIL {file}\n  {issues}\n")
                    } else {
                        print("FAIL {file}\n")
                    }
                    return
                }
                collect_from_program(program, file, records, seen_loc, lambdas_only, want_sigs, min_tokens)
            }
        }
    }
    return ok_compile
}
