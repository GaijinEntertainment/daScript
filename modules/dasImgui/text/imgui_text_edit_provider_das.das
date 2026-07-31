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
    names : array<string>
}

struct public TextEditDasProvider {
    jobs : Channel?
    results : Channel?
    started : bool = false
    //! The completion token the worker last received work for.
    sent_token : int = -1
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

def private collect_compiled_symbols(source, prefix : string; // nolint:STYLE037
                                     var names : array<string>) {
    var inscope access <- make_file_access("")
    // the parser wants a final newline; mid-edit buffers rarely have one
    set_file_source(access, COMPLETION_OVERLAY, source + "\n")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.threadlock_context = true
            cop.ignore_shared_modules = true
            cop.export_all = true
            cop.no_optimizations = true
            cop.no_infer_time_folding = true
            compile_file(COMPLETION_OVERLAY, access, unsafe(addr(mg)), cop) $(ok; program; _issues) {
                if (ok && program != null) {
                    let this_mod = get_this_module(program)
                    ast::for_each_function(this_mod, "") $(var func : FunctionPtr) {
                        if (!func.flags.generated) {
                            offer_name(names, string(func.name), prefix)
                        }
                    }
                    ast::for_each_structure(this_mod) $(var st : StructurePtr) {
                        if (!st.flags.generated && !st.flags.isLambda) {
                            offer_name(names, string(st.name), prefix)
                        }
                    }
                    ast::for_each_enumeration(this_mod) $(var en : EnumerationPtr) {
                        offer_name(names, string(en.name), prefix)
                        for (entry in en.list) {
                            offer_name(names, string(entry.name), prefix)
                        }
                    }
                    ast::for_each_global(this_mod) $(var value : VariablePtr) {
                        if (!value.flags.generated) {
                            offer_name(names, string(value.name), prefix)
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
            collect_compiled_symbols(job.source, job.prefix, result.names)
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
                delete local
            }
        }
    }
    return if (comp.request_revision < 0 || comp.token == provider.sent_token)
    let _ = text_edit_das_provider_start(provider)
    provider.sent_token = comp.token
    var job = DasCompleteJob(token = comp.token, prefix = comp.prefix,
        source = text_edit_text(ed))
    provider.jobs |> push_clone(job)
    delete job
}
