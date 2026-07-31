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
require daslib/fio
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
    prefix : string
    source : string
}

[safe_when_uninitialized]
struct private DasCompleteResult {
    token : int
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

def public text_edit_parse_das_issues(issues, file : string;
                                      var out : array<TextEditDiagnostic>) {
    //! Compiler issue text -> per-position findings: a `file:LINE:COL` line
    //! pins the preceding error/warning message. Printed lines are 1-based
    //! but columns 0-BASED (probe-verified); both store 0-based.
    var message = ""
    var inscope lines <- split(issues, "\n")
    let anchor = file + ":"
    for (line in lines) {
        if (starts_with(line, "error[") || starts_with(line, "warning[")) {
            let sep = find(line, "]: ")
            message = sep >= 0 ? slice(line, sep + 3, length(line)) : line
        } elif (starts_with(line, anchor) && !empty(message)) {
            var inscope parts <- split(slice(line, length(anchor), length(line)), ":")
            if (length(parts) >= 2) {
                out |> push(TextEditDiagnostic(
                    line = max(to_int(parts[0]) - 1, 0),
                    column = max(to_int(parts[1]), 0),
                    message = message))
                message = ""
            }
        }
    }
}

def private collect_compiled(source, prefix : string; // nolint:STYLE037
                             var result : DasCompleteResult) {
    var inscope access <- make_file_access("")
    // the parser wants a final newline; mid-edit buffers rarely have one
    set_file_source(access, COMPLETION_OVERLAY, source + "\n")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            // introspection model; gen2 default lives in the CLI, not the struct
            cop.version_2_syntax = true
            cop.threadlock_context = true
            cop.ignore_shared_modules = true
            cop.export_all = true
            cop.no_optimizations = true
            cop.no_infer_time_folding = true
            cop.auto_inline_functions = false
            compile_file(COMPLETION_OVERLAY, access, unsafe(addr(mg)), cop) $(ok; program; issues) {
                result.ok = ok && program != null
                if (!result.ok) {
                    text_edit_parse_das_issues("{issues}", COMPLETION_OVERLAY,
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
            var result = DasCompleteResult(token = job.token)
            collect_compiled(job.source, job.prefix, result)
            results |> push_clone(result)
            delete result
            delete job
        }
    }
    jobs |> release
    results |> release
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
    var stop_job = DasCompleteJob(stop = true)
    provider.jobs |> push_clone(stop_job)
    delete stop_job
    // let the worker pop the stop sentinel and drop its refs (50ms pop
    // timeout) before the channels go away under it
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
    provider.sent_token = -1
}

def public text_edit_das_provider_sync(var ed : TextEditState;
                                       var comp : TextEditCompletion;
                                       var provider : TextEditDasProvider) {
    //! Frame tick: poll finished compiles into the offer rail (stale tokens
    //! drop there) and hand the worker new work when the request moved.
    return if (ed.language != "daslang")
    if (provider.started) {
        var draining = true
        while (draining) {
            draining = provider.results |> try_pop_clone() $(res : DasCompleteResult#) {
                var local : DasCompleteResult
                local := res
                text_edit_complete_offer(ed, comp, local.token, local.names)
                // the freshest compile always wins the diagnostics + status
                text_edit_set_diagnostics(ed, local.diagnostics,
                    local.ok ? TextEditCompileStatus.ok : TextEditCompileStatus.failed)
                delete local
            }
        }
    }
    if (comp.request_revision >= 0 && comp.token != provider.sent_token) {
        let _ = text_edit_das_provider_start(provider)
        provider.sent_token = comp.token
        provider.compiled_revision = ed.revision
        ed.compile_status = TextEditCompileStatus.compiling
        var job = DasCompleteJob(token = comp.token, prefix = comp.prefix,
            source = text_edit_text(ed))
        provider.jobs |> push_clone(job)
        delete job
        return
    }
    // no completion request pending: keep the diagnostics fresh on idle
    if (provider.compiled_revision != ed.revision && ed.syntax_idle >= 0.3f) {
        let _ = text_edit_das_provider_start(provider)
        provider.compiled_revision = ed.revision
        ed.compile_status = TextEditCompileStatus.compiling
        var job = DasCompleteJob(token = -1, prefix = "",
            source = text_edit_text(ed))
        provider.jobs |> push_clone(job)
        delete job
    }
}
