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
    token : int
    source : string
    compiler : string
}

[safe_when_uninitialized]
struct private CppCheckResult {
    token : int
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
        let candidate = "{at}/libclang/bin/clang.exe"
        return candidate if (fexist(candidate))
    }
    return "clang"
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
    let scratch = "{get_das_root()}/_dasimgui_cpp_check_{job.token}.cpp"
    let capture_path = "{scratch}.out.txt"
    fopen(scratch, "wb") $(f) {
        if (f != null) {
            f |> fwrite(job.source + "\n")
        }
    }
    var full = "\"{job.compiler}\" -fsyntax-only -x c++ \"{scratch}\" > \"{capture_path}\" 2>&1"
    if (get_platform_name() == "windows") {
        // sacrificial outer quotes for cmd.exe (see daspkg run_cmd)
        full = "\"{full}\""
    }
    let exit_code = unsafe(popen(full) $(f) {
        if (f != null) {
            let _ = fread(f)
        }
    })
    result.ok = exit_code == 0
    if (fexist(capture_path)) {
        fopen(capture_path, "rb") $(f) {
            if (f != null) {
                text_edit_parse_clang_output(fread(f), scratch, result.diagnostics)
            }
        }
        remove(capture_path)
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
            var result = CppCheckResult(token = job.token)
            run_cpp_check(job, result)
            results |> push_clone(result)
            delete result
            delete job
        }
    }
    jobs |> release
    results |> release
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
    var stop_job = CppCheckJob(stop = true)
    provider.jobs |> push_clone(stop_job)
    delete stop_job
    // let the worker pop the stop sentinel and drop its refs before removal
    sleep(150u)
    unsafe {
        if (provider.jobs != null) {
            provider.jobs |> channel_remove
        }
        if (provider.results != null) {
            provider.results |> channel_remove
        }
    }
    provider.jobs = null
    provider.results = null
    provider.started = false
    provider.compiled_revision = -1
}

def public text_edit_cpp_provider_sync(var ed : TextEditState;
                                       var provider : TextEditCppProvider) {
    //! Frame tick: poll finished checks into diagnostics/status and hand the
    //! worker a fresh check when the buffer sits idle at an unseen revision.
    return if (ed.language != "cpp" && ed.language != "c")
    if (!provider.discovered && empty(provider.compiler)) {
        provider.compiler = text_edit_cpp_compiler_discover()
        provider.discovered = true
    }
    if (provider.started) {
        var draining = true
        while (draining) {
            draining = provider.results |> try_pop_clone() $(res : CppCheckResult#) {
                var local : CppCheckResult
                local := res
                text_edit_set_diagnostics(ed, local.diagnostics,
                    local.ok ? TextEditCompileStatus.ok : TextEditCompileStatus.failed)
                delete local
            }
        }
    }
    if (provider.compiled_revision != ed.revision && ed.syntax_idle >= 0.3f) {
        let _ = text_edit_cpp_provider_start(provider)
        provider.compiled_revision = ed.revision
        ed.compile_status = TextEditCompileStatus.compiling
        var job = CppCheckJob(token = ed.revision,
            source = text_edit_text(ed), compiler = provider.compiler)
        provider.jobs |> push_clone(job)
        delete job
    }
}
