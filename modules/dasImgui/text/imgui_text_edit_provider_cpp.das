options gen2
options _comment_hygiene = true
options indenting = 4

module imgui_text_edit_provider_cpp shared

require imgui/imgui_text_edit_complete public
require daslib/jobque_boost
require daslib/fio
require daslib/strings_boost
require math
require strings

//! The .cpp diagnostics provider: an honest clang round-trip per idle
//! revision — the buffer syntax-checks through an EXTERNAL compiler process
//! (the out-of-process half of the provider contract). Errors and warnings
//! land as squiggles + Issues rows; completion waits for E6 / clangd.

[safe_when_uninitialized]
struct private CppCheckJob {
    stop : bool = false
    revision : int
    source : string
    compiler : string
    //! Buffer language ("cpp" or "c") - it picks the clang dialect.
    language : string
}

[safe_when_uninitialized]
struct private CppCheckResult {
    //! Buffer revision the check ran on; a result only lands on that revision.
    revision : int
    ok : bool = false
    diagnostics : array<TextEditDiagnostic>
}

struct public TextEditCppProvider {
    jobs : Channel?
    results : Channel?
    started : bool = false
    compiled_revision : int = -1
    //! Compiler command; "" = discover once (the libclang SDK sibling
    //! convention `<ancestor>/libclang/bin/clang.exe`, else PATH clang).
    compiler : string
    discovered : bool = false
}

def public text_edit_cpp_compiler_discover : string {
    //! First clang found by the dasClangBind convention — `libclang/` as a
    //! sibling of an ancestor of the das root — falling back to PATH.
    var at = get_das_root()
    for (_step in range(6)) {
        at = dir_name(at)
        break if (empty(at) || at == "/" || ends_with(at, ":"))
        // .exe first for Windows, then the POSIX name
        for (exe in ["clang.exe", "clang"]) {
            let candidate = "{at}/libclang/bin/{exe}"
            return candidate if (fexist(candidate))
        }
    }
    return "clang"
}

def public text_edit_cpp_compiler_available(compiler : string) : bool {
    //! Is this compiler usable? A discovered path answers by `fexist`, a bare
    //! PATH name (the fallback, and the norm off Windows) only by running it.
    return false if (empty(compiler))
    return true if (fexist(compiler))
    var ok = false
    let exit_code = unsafe(popen_argv([compiler, "--version"], 30.0) $(f) {
        if (f != null) {
            ok = find(unsafe(fread_to_eof(f)), "clang") >= 0
        }
    })
    return ok && exit_code == 0
}

def public text_edit_cpp_scratch_suffix(language : string) : string {
    //! The scratch extension must agree with the `-x` dialect below.
    return language == "c" ? ".c" : ".cpp"
}

def public text_edit_cpp_check_argv(compiler, language, scratch : string) : array<string> {
    //! `c` buffers must check as C — `-x c++` rejects valid C (`void*` assigns
    //! to any `T*` in C, not in C++).
    return [compiler, "-fsyntax-only", "-x", language == "c" ? "c" : "c++",
        scratch]
}

def private all_digits(text : string) : bool {
    return false if (empty(text))
    var digits = true
    peek_data(text) $(bytes) {
        for (b in bytes) {
            if (!is_number(int(b))) {
                digits = false
                break
            }
        }
    }
    return digits
}

def private parse_clang_line(line, overlay : string;
                             var out : array<TextEditDiagnostic>) : bool {
    var sep = find(line, ": error: ")
    var msg_offset = 9
    if (sep < 0) {
        sep = find(line, ": warning: ")
        msg_offset = 11
    }
    if (sep < 0) {
        // a missing #include reports as ": fatal error: " and nothing else
        sep = find(line, ": fatal error: ")
        msg_offset = 15
    }
    return false if (sep < 0)
    let location = slice(line, 0, sep)
    let c2 = rfind(location, ":")
    return false if (c2 <= 0)
    let c1 = rfind(slice(location, 0, c2), ":")
    return false if (c1 <= 0)
    let line_text = slice(location, c1 + 1, c2)
    let column_text = slice(location, c2 + 1, length(location))
    return false if (!all_digits(line_text) || !all_digits(column_text))
    let source_file = slice(location, 0, c1)
    let from_line = max(to_int(line_text) - 1, 0)
    let from_column = max(to_int(column_text) - 1, 0) // clang is 1-based BOTH
    out |> push(TextEditDiagnostic(line = from_line, column = from_column,
        end_line = from_line, end_column = from_column,
        file = source_file == overlay ? "" : source_file,
        message = slice(line, sep + msg_offset, length(line))))
    return true
}

def public text_edit_parse_clang_output(output, overlay : string; // nolint:STYLE037
                                        var out : array<TextEditDiagnostic>) {
    //! Clang text -> findings: `FILE:LINE:COL: severity: message` one-liners
    //! (1-based both); the |-gutter caret line (`^~~~`) supplies the width.
    var inscope lines <- split(output, "\n")
    for (line in lines) {
        continue if (parse_clang_line(line, overlay, out))
        // a caret line: nothing but spaces, ^ and ~ after the | gutter
        let bar = find(line, "|")
        continue if (bar < 0 || empty(out))
        let last = length(out) - 1
        continue if (out[last].end_column != out[last].column) // already sized
        var width = 0
        var caret_line = true
        peek_data(line) $(bytes) {
            for (at in range(bar + 1, length(bytes))) {
                let b = int(bytes[at])
                if (b == '^' || b == '~') {
                    width++
                } elif (b != ' ' && b != '\t' && b != 0x0d) {
                    caret_line = false
                }
            }
        }
        if (caret_line && width > 0) {
            out[last].end_column = out[last].column + width
        }
    }
}

def private run_cpp_check(job : CppCheckJob; var result : CppCheckResult) {
    let scratch = ("{get_das_root()}/_dasimgui_cpp_check_{job.revision}"
        + text_edit_cpp_scratch_suffix(job.language))
    fopen(scratch, "wb") $(f) {
        if (f != null) {
            f |> fwrite(job.source + "\n")
        }
    }
    // popen_argv merges stdout+stderr; a pipe has no stat size, so plain fread reads nothing
    var output = ""
    var inscope argv <- text_edit_cpp_check_argv(job.compiler, job.language, scratch)
    let exit_code = unsafe(popen_argv(argv, 60.0) $(f) {
        if (f != null) {
            output = unsafe(fread_to_eof(f))
        }
    })
    result.ok = exit_code == 0
    if (!empty(output)) {
        text_edit_parse_clang_output(output, scratch, result.diagnostics)
    }
    remove(scratch)
}

def private cpp_check_worker(var jobs : Channel?&; var results : Channel?&) {
    this_context().name := "dasimgui_text_edit_cpp_check"
    var stopping = false
    while (!stopping) {
        jobs |> pop_with_timeout_clone(50) $(first : CppCheckJob#) {
            var job : CppCheckJob
            job := first
            // only the newest buffer matters - drain typing churn
            var draining = true
            while (draining) {
                draining = jobs |> try_pop_clone() $(next : CppCheckJob#) {
                    delete job
                    job := next
                }
            }
            if (job.stop) {
                stopping = true
                delete job
                return
            }
            var result = CppCheckResult(revision = job.revision)
            run_cpp_check(job, result)
            results |> push_clone(result)
            delete result
            delete job
        }
    }
    // the host dropped its refs at stop: the worker is the last owner here
    unsafe {
        jobs |> channel_remove
        results |> channel_remove
    }
}

def public text_edit_cpp_provider_start(var provider : TextEditCppProvider) : bool {
    return false if (provider.started)
    provider.jobs = unsafe(channel_create())
    provider.results = unsafe(channel_create())
    provider.started = true
    var jobs_capture = provider.jobs
    var results_capture = provider.results
    new_thread() <| @capture(= jobs_capture, = results_capture) {
        cpp_check_worker(jobs_capture, results_capture)
    }
    return true
}

def public text_edit_cpp_provider_stop(var provider : TextEditCppProvider) {
    return if (!provider.started)
    // Drop OUR refs before the sentinel, then let the worker remove the
    // channels when it exits: a check in flight can outrun any wait here, and
    // removing a channel the worker still holds panics.
    let jobs_channel = provider.jobs
    provider.jobs |> release
    provider.results |> release
    var stop_job = CppCheckJob(stop = true)
    jobs_channel |> push_clone(stop_job)
    delete stop_job
    provider.jobs = null
    provider.results = null
    provider.started = false
    provider.compiled_revision = -1
}

def public text_edit_cpp_provider_sync(var ed : TextEditState;
                                       var provider : TextEditCppProvider) {
    //! Frame tick: poll finished checks into diagnostics/status and hand the
    //! worker a fresh check when the buffer sits idle at an unseen revision.
    //! Results the buffer moved past (or switched language away from) drop.
    let active = ed.language == "cpp" || ed.language == "c"
    if (provider.started) {
        var draining = true
        while (draining) {
            draining = provider.results |> try_pop_clone() $(res : CppCheckResult#) {
                var local : CppCheckResult
                local := res
                if (active && local.revision == ed.revision) {
                    text_edit_set_diagnostics(ed, local.diagnostics,
                        local.ok ? TextEditCompileStatus.ok : TextEditCompileStatus.failed)
                } else {
                    // dropped: re-check from scratch when this buffer is current again
                    provider.compiled_revision = -1
                }
                delete local
            }
        }
    }
    return if (!active)
    if (!provider.discovered && empty(provider.compiler)) {
        provider.compiler = text_edit_cpp_compiler_discover()
        provider.discovered = true
    }
    if (provider.compiled_revision != ed.revision && ed.syntax_idle >= 0.3f) {
        let _ = text_edit_cpp_provider_start(provider)
        provider.compiled_revision = ed.revision
        ed.compile_status = TextEditCompileStatus.compiling
        var job = CppCheckJob(revision = ed.revision, source = text_edit_text(ed),
            compiler = provider.compiler, language = ed.language)
        provider.jobs |> push_clone(job)
        delete job
    }
}
