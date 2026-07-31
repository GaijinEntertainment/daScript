options gen2
options _comment_hygiene = true
options indenting = 4
options rtti

module imgui_text_edit_provider_das shared

require imgui/imgui_text_edit_complete public
require daslib/rtti
require daslib/ast
require daslib/ast_boost
require daslib/jobque_boost
require daslib/strings_boost
require math
require strings

//! The .das completion provider: calls the compiler infrastructure directly
//! (the utils/lsp approach minus the protocol) on a worker thread. Minimal
//! but real - symbol names arrive through the offer rail only when the
//! buffer actually compiles; the instant document-words tier always runs.

let private COMPLETION_OVERLAY = "text_edit_completion_overlay.das"

[safe_when_uninitialized]
struct private DasCompleteJob {
    stop : bool = false
    token : int
    revision : int
    prefix : string
    source : string
    //! Overlay filename the buffer compiles under (its display name).
    overlay : string
}

[safe_when_uninitialized]
struct private DasCompleteResult {
    token : int
    //! Buffer revision the compile ran on; a result only lands on that revision.
    revision : int
    ok : bool = false
    names : array<string>
    diagnostics : array<TextEditDiagnostic>
}

struct public TextEditDasProvider {
    jobs : Channel?
    results : Channel?
    started : bool = false
    //! The completion token the worker last received work for.
    sent_token : int = -1
    //! Buffer revision last handed to the worker (diagnostics keep fresh
    //! even without completion requests).
    compiled_revision : int = -1
}

def private ident_name(name : string) : bool {
    return false if (empty(name))
    let b = first_character(name)
    return is_alpha(b) || b == '_'
}

def private offer_name(var names : array<string>; name, prefix : string) {
    return if (!ident_name(name) || name == prefix || !starts_with(name, prefix)
        || find_index(names, name) >= 0)
    names |> push(name)
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

def private parse_location(line, anchor : string;
                           var out : array<TextEditDiagnostic>;
                           message : string) : bool {
    // FILE:LINE:COL split from the right - paths keep their colons
    let c2 = rfind(line, ":")
    return false if (c2 <= 0)
    let c1 = rfind(slice(line, 0, c2), ":")
    return false if (c1 <= 0)
    let line_text = slice(line, c1 + 1, c2)
    let column_text = slice(line, c2 + 1, length(line))
    return false if (!all_digits(line_text) || !all_digits(column_text))
    let source_file = slice(line, 0, c1)
    let from_line = max(to_int(line_text) - 1, 0)
    let from_column = max(to_int(column_text), 0)
    out |> push(TextEditDiagnostic(line = from_line, column = from_column,
        end_line = from_line, end_column = from_column,
        file = source_file == anchor ? "" : source_file, message = message))
    return true
}

def public text_edit_parse_das_issues(issues, file : string; // nolint:STYLE037
                                      var out : array<TextEditDiagnostic>) {
    //! Issue text -> findings: the first column-0 `FILE:LINE:COL` after an
    //! error/warning pins its message (1-based lines, 0-BASED columns); the
    //! ^^^ caret line supplies the width; foreign files keep their path.
    var message = ""
    var pending_caret = 0
    var inscope lines <- split(issues, "\n")
    for (line in lines) {
        if (starts_with(line, "error[") || starts_with(line, "warning[")) {
            let sep = find(line, "]: ")
            message = sep >= 0 ? slice(line, sep + 3, length(line)) : line
            pending_caret = 0
        } elif (!empty(message) && !starts_with(line, "\t")
                && !starts_with(line, " ")) {
            if (parse_location(line, file, out, message)) {
                message = ""
                pending_caret = 2 // source echo next, then the caret line
            }
        } elif (pending_caret == 2) {
            pending_caret = 1
        } elif (pending_caret == 1) {
            pending_caret = 0
            // the ^^^ width under the source echo IS the finding's width
            var carets = 0
            var caret_line = true
            peek_data(line) $(bytes) {
                for (b in bytes) {
                    if (int(b) == '^') {
                        carets++
                    } elif (int(b) != ' ' && int(b) != '\t') {
                        caret_line = false
                    }
                }
            }
            if (caret_line && carets > 0 && !empty(out)) {
                let last = length(out) - 1
                out[last].end_column = out[last].column + carets
            }
        }
    }
}

def private collect_compiled(source, prefix, overlay : string; // nolint:STYLE037
                             var result : DasCompleteResult) {
    var inscope access <- make_file_access("")
    // the parser wants a final newline; mid-edit buffers rarely have one
    set_file_source(access, overlay, source + "\n")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            // introspection model; explicit gen2 keeps this correct on pre-flip cores
            cop.version_2_syntax = true
            cop.threadlock_context = true
            cop.ignore_shared_modules = true
            cop.export_all = true
            cop.no_optimizations = true
            cop.no_infer_time_folding = true
            cop.auto_inline_functions = false
            compile_file(overlay, access, unsafe(addr(mg)), cop) $(ok; program; issues) {
                result.ok = ok && program != null
                if (!result.ok) {
                    text_edit_parse_das_issues("{issues}", overlay,
                        result.diagnostics)
                }
                if (result.ok) {
                    let this_mod = get_this_module(program)
                    ast::for_each_function(this_mod, "") $(var func : FunctionPtr) {
                        if (!func.flags.generated) {
                            offer_name(result.names, string(func.name), prefix)
                        }
                    }
                    ast::for_each_structure(this_mod) $(var st : StructurePtr) {
                        if (!st.flags.generated && !st.flags.isLambda) {
                            offer_name(result.names, string(st.name), prefix)
                        }
                    }
                    ast::for_each_enumeration(this_mod) $(var en : EnumerationPtr) {
                        offer_name(result.names, string(en.name), prefix)
                        for (entry in en.list) {
                            offer_name(result.names, string(entry.name), prefix)
                        }
                    }
                    ast::for_each_global(this_mod) $(var value : VariablePtr) {
                        if (!value.flags.generated) {
                            offer_name(result.names, string(value.name), prefix)
                        }
                    }
                }
            }
        }
    }
}

def private das_complete_worker(var jobs : Channel?&; var results : Channel?&) {
    this_context().name := "dasimgui_text_edit_das_complete"
    var stopping = false
    while (!stopping) {
        jobs |> pop_with_timeout_clone(50) $(first : DasCompleteJob#) {
            var job : DasCompleteJob
            job := first
            // only the newest request matters - drain rapid typing churn
            var draining = true
            while (draining) {
                draining = jobs |> try_pop_clone() $(next : DasCompleteJob#) {
                    delete job
                    job := next
                }
            }
            if (job.stop) {
                stopping = true
                delete job
                return
            }
            var result = DasCompleteResult(token = job.token, revision = job.revision)
            collect_compiled(job.source, job.prefix,
                empty(job.overlay) ? COMPLETION_OVERLAY : job.overlay, result)
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

def public text_edit_das_provider_start(var provider : TextEditDasProvider) : bool {
    return false if (provider.started)
    provider.jobs = unsafe(channel_create())
    provider.results = unsafe(channel_create())
    provider.started = true
    var jobs_capture = provider.jobs
    var results_capture = provider.results
    new_thread() <| @capture(= jobs_capture, = results_capture) {
        das_complete_worker(jobs_capture, results_capture)
    }
    return true
}

def public text_edit_das_provider_stop(var provider : TextEditDasProvider) {
    return if (!provider.started)
    // Drop OUR refs before the sentinel, then let the worker remove the
    // channels when it exits: a compile in flight can outrun any wait here,
    // and removing a channel the worker still holds panics.
    let jobs_channel = provider.jobs
    provider.jobs |> release
    provider.results |> release
    var stop_job = DasCompleteJob(stop = true)
    jobs_channel |> push_clone(stop_job)
    delete stop_job
    provider.jobs = null
    provider.results = null
    provider.started = false
    provider.sent_token = -1
    provider.compiled_revision = -1
}

def public text_edit_das_provider_sync(var ed : TextEditState;
                                       var comp : TextEditCompletion;
                                       var provider : TextEditDasProvider) {
    //! Frame tick: poll finished compiles into the offer rail (stale tokens
    //! drop there) and hand the worker new work when the request moved.
    //! Results the buffer moved past (or switched language away from) drop.
    let active = ed.language == "daslang"
    if (provider.started) {
        var draining = true
        while (draining) {
            draining = provider.results |> try_pop_clone() $(res : DasCompleteResult#) {
                var local : DasCompleteResult
                local := res
                if (active && local.revision == ed.revision) {
                    text_edit_complete_offer(ed, comp, local.token, local.names, 0)
                    // the freshest compile always wins the diagnostics + status
                    text_edit_set_diagnostics(ed, local.diagnostics,
                        local.ok ? TextEditCompileStatus.ok : TextEditCompileStatus.failed)
                } else {
                    // dropped: recompile from scratch when this buffer is current again
                    provider.compiled_revision = -1
                }
                delete local
            }
        }
    }
    return if (!active)
    if (comp.request_revision >= 0 && comp.token != provider.sent_token) {
        let _ = text_edit_das_provider_start(provider)
        provider.sent_token = comp.token
        provider.compiled_revision = ed.revision
        ed.compile_status = TextEditCompileStatus.compiling
        var job = DasCompleteJob(token = comp.token, revision = ed.revision,
            prefix = comp.prefix, source = text_edit_text(ed),
            overlay = ed.file_name)
        provider.jobs |> push_clone(job)
        delete job
        return
    }
    // no completion request pending: keep the diagnostics fresh on idle
    if (provider.compiled_revision != ed.revision && ed.syntax_idle >= 0.3f) {
        let _ = text_edit_das_provider_start(provider)
        provider.compiled_revision = ed.revision
        ed.compile_status = TextEditCompileStatus.compiling
        var job = DasCompleteJob(token = -1, revision = ed.revision, prefix = "",
            source = text_edit_text(ed), overlay = ed.file_name)
        provider.jobs |> push_clone(job)
        delete job
    }
}
