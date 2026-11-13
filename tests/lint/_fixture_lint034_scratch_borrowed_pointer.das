options gen2
options auto_inline_functions = false
// lint-skip-file: test_lint034_sites expects each marked line

struct Session {
    n_past : int
}

struct Seg {
    s : Session?
    npos : int
}

struct Runner {
    @scratch segs : array<Seg>
    @scratch owned : array<Session?>
}

struct Node {
    next : Node?
    @scratch held : array<Session?>
}

var private @scratch g_segs : array<Seg>
var private @scratch g_ptrs : array<Session?>
var private @scratch g_counts : array<int>
var private @scratch g_by_id : table<int; Session?>
var private @scratch g_pairs : array<tuple<Session?; int>>
var private g_registry : array<Session?>
var private g_runner : Runner
var private g_runners : array<Runner>
var private g_runner_ptr : Runner?
var private g_head : Node

def consume(segs : array<Seg>) : int {
    var total = 0
    for (sg in segs) {
        total += sg.npos
    }
    return total
}

// ===== bad: a store an exit leaves in place =====

def bad_fall_off_the_end(var sessions : array<Session?>) : int {
    g_segs |> clear()
    for (s in sessions) {
        g_segs |> push(Seg(s = s, npos = 1))        // LINT034
    }
    return consume(g_segs)
}

def bad_early_return(var sessions : array<Session?>; quick : bool) : int {
    for (s in sessions) {
        g_segs |> push(Seg(s = s, npos = 1))        // LINT034
    }
    return 0 if (quick)
    let total = consume(g_segs)
    g_segs |> clear()
    return total
}

def bad_one_branch_clears(var sessions : array<Session?>; tidy : bool) : void {
    for (s in sessions) {
        g_segs |> push(Seg(s = s, npos = 1))        // LINT034
    }
    if (tidy) {
        g_segs |> clear()
    }
}

def bad_fill_in_one_branch(var sp : Session?; fill : bool) : void {
    if (fill) {
        g_ptrs |> push(sp)                          // LINT034
    }
}

def bad_slot_store(var sp : Session?) : void {
    g_segs |> resize(1)
    g_segs[0].s = sp                                // LINT034
}

def bad_move_store(var sp : Session?) : void {
    g_segs |> resize(1)
    g_segs[0].s <- sp                               // LINT034
}

def bad_two_slots_one_nulled(var a, b : Session?) : void {
    g_ptrs |> resize(2)
    g_ptrs[0] = a
    g_ptrs[1] = b                                   // LINT034
    g_ptrs[0] = null
}

def bad_resize_one(var sp : Session?) : void {
    g_ptrs |> push(sp)                              // LINT034
    g_ptrs |> resize(1)
}

def bad_second_round(var sessions : array<Session?>) : void {
    for (s in sessions) {
        return if (s == null)
        g_segs |> push(Seg(s = s, npos = 1))        // LINT034
    }
    g_segs |> clear()
}

def bad_while_second_round(var sessions : array<Session?>) : void {
    var i = 0
    while (i < length(sessions)) {
        return if (sessions[i] == null)
        g_ptrs |> push(sessions[i])                 // LINT034
        i ++
    }
    g_ptrs |> clear()
}

def bad_clear_only_in_loop(var sp : Session?; n : int) : void {
    g_ptrs |> push(sp)                              // LINT034
    for (_i in range(n)) {
        g_ptrs |> clear()
    }
}

def bad_break_skips_clear(var sessions : array<Session?>) : void {
    for (s in sessions) {
        g_ptrs |> push(s)                           // LINT034
        break if (s == null)
        g_ptrs |> clear()
    }
}

def bad_clear_only_in_try(var sp : Session?) : void {
    g_ptrs |> push(sp)                              // LINT034
    try {
        g_ptrs |> clear()
    } recover {
        pass
    }
}

def bad_return_in_unsafe(var sp : Session?; quick : bool) : void {
    g_ptrs |> push(sp)                              // LINT034
    unsafe {
        return if (quick)
    }
    g_ptrs |> clear()
}

def bad_fill_in_finally(var sp : Session?) : int {
    g_ptrs |> clear()
    return 0
} finally {
    g_ptrs |> push(sp)                              // LINT034
}

def bad_conditional_finally(var sp : Session?; tidy : bool) : int {
    g_ptrs |> push(sp)                              // LINT034
    return 0
} finally {
    if (tidy) {
        g_ptrs |> clear()
    }
}

def bad_inner_finally_then_fill(var sp : Session?) : void {
    if (sp != null) {
        pass
    } finally {
        g_ptrs |> clear()
    }
    g_ptrs |> push(sp)                              // LINT034
}

def bad_table_insert(var sp : Session?) : void {
    g_by_id |> insert(1, sp)                        // LINT034
}

def bad_tuple(var sp : Session?) : void {
    g_pairs |> push((sp, 1))                        // LINT034
}

def bad_emplace(var sp : Session?) : void {
    var seg = Seg(s = sp, npos = 1)
    g_segs |> emplace(seg)                          // LINT034
}

def bad_push_from(var sessions : array<Session?>) : void {
    g_ptrs |> push_from(sessions)                   // LINT034
}

def bad_push_clone(var sp : Session?) : void {
    g_ptrs |> push_clone(sp)                        // LINT034
}

def bad_runner_array(var sp : Session?) : void {
    g_runners[0].segs |> push(Seg(s = sp, npos = 1))    // LINT034
}

def bad_ptr_field(var sp : Session?) : void {
    g_runner_ptr.segs |> push(Seg(s = sp, npos = 1))    // LINT034
}

def bad_self_referential_type(var sp : Session?) : void {
    g_head.held |> push(sp)                         // LINT034
}

def bad_ref_alias(var sp : Session?) : void {
    var segs & = unsafe(g_segs)
    segs |> push(Seg(s = sp, npos = 1))             // LINT034
}

def bad_block_arg_value(var sp : Session?) : void {
    invoke($(var p : Session?) {
        g_segs |> push(Seg(s = p, npos = 1))        // LINT034
    }, sp)
}

def bad_recursive(var sp : Session?; n : int) : void {
    g_ptrs |> push(sp)                              // LINT034
    if (n > 0) {
        bad_recursive(sp, n - 1)
    }
}

// a helper that fills its caller's container is not the finding
def fill_runner(var r : Runner; var sessions : array<Session?>) : void {
    for (s in sessions) {
        r.segs |> push(Seg(s = s, npos = 1))
    }
}

def fill_runner_once_removed(var r : Runner; var sessions : array<Session?>) : void {
    fill_runner(r, sessions)
}

def bad_caller_keeps(var sessions : array<Session?>) : int {
    fill_runner(g_runner, sessions)                 // LINT034
    return consume(g_runner.segs)
}

def bad_caller_two_calls_down(var sessions : array<Session?>) : int {
    fill_runner_once_removed(g_runner, sessions)    // LINT034
    return consume(g_runner.segs)
}

// a private function's global is its callers' to empty
def private queue_seg(var s : Session?) : void {
    g_segs |> push(Seg(s = s, npos = 1))
}

def private flush_segs : int {
    let total = consume(g_segs)
    g_segs |> clear()
    return total
}

def private flush_segs_if(tidy : bool) : void {
    if (tidy) {
        g_segs |> clear()
    }
}

def bad_queue_without_flush(var sessions : array<Session?>) : void {
    for (s in sessions) {
        queue_seg(s)                                // LINT034
    }
}

def bad_helper_clears_on_one_path(var sp : Session?; tidy : bool) : void {
    queue_seg(sp)                                   // LINT034
    flush_segs_if(tidy)
}

// ===== good =====

def good_clear_on_every_exit(var sessions : array<Session?>; quick : bool) : int {
    for (s in sessions) {
        g_segs |> push(Seg(s = s, npos = 1))
    }
    if (quick) {
        g_segs |> clear()
        return 0
    }
    let total = consume(g_segs)
    g_segs |> clear()
    return total
}

def good_finally(var sessions : array<Session?>; quick : bool) : int {
    for (s in sessions) {
        g_segs |> push(Seg(s = s, npos = 1))
    }
    return 0 if (quick)
    return consume(g_segs)
} finally {
    g_segs |> clear()
}

def good_finally_fall_off(var sp : Session?) : void {
    g_ptrs |> push(sp)
} finally {
    g_ptrs |> clear()
}

def good_slot_nulled(var sp : Session?) : int {
    g_segs |> resize(1)
    g_segs[0].s = sp
    let total = consume(g_segs)
    g_segs[0].s = null
    return total
}

def good_resize_zero(var sp : Session?) : void {
    g_ptrs |> push(sp)
    g_ptrs |> resize(0)
}

def good_delete(var sp : Session?) : void {
    g_ptrs |> push(sp)
    unsafe {
        delete g_ptrs
    }
}

def good_queue_then_flush(var sessions : array<Session?>) : int {
    for (s in sessions) {
        queue_seg(s)
    }
    return flush_segs()
}

def good_caller_clears(var sessions : array<Session?>) : int {
    fill_runner(g_runner, sessions)
    let total = consume(g_runner.segs)
    g_runner.segs |> clear()
    return total
}

def good_caller_local(var sessions : array<Session?>) : int {
    g_ptrs |> clear()
    var r : Runner
    fill_runner(r, sessions)
    return consume(r.segs)
}

def good_calls_reported_public(var sessions : array<Session?>) : int {
    return bad_fall_off_the_end(sessions)
}

def make_session(var like : Session?) : Session? {
    return new Session(n_past = like.n_past)
}

def good_call_result(var like : Session?) : void {
    g_segs |> push(Seg(s = make_session(like), npos = 1))
}

def good_owned(var r : Runner) : void {
    r.owned |> push(new Session(n_past = 1))
}

def good_owned_new_local : void {
    var p = new Session()
    g_ptrs |> push(p)
}

def good_owned_new_local_with_fields(n : int) : void {
    var p = new Session(n_past = n)
    g_ptrs |> push(p)
}

def good_global_pointer(at : int) : void {
    g_ptrs |> push(g_registry[at])
}

def good_no_pointers(var sp : Session?) : void {
    g_counts |> clear()
    g_counts |> push(sp == null ? 0 : 1)
}

def good_not_scratch(var sp : Session?) : void {
    g_ptrs |> clear()
    g_registry |> push(sp)
}

def good_local_scratch(var sp : Session?) : int {
    g_ptrs |> clear()
    var @scratch mine : array<Session?>
    mine |> push(sp)
    return length(mine)
}

def good_block_arg_dest(var sp : Session?) : void {
    invoke($(var rr : Runner) {
        rr.segs |> push(Seg(s = sp, npos = 1))
    }, g_runner)
}

[export]
def main {
    var sessions : array<Session?>
    var r : Runner
    bad_fall_off_the_end(sessions)
    bad_early_return(sessions, false)
    bad_one_branch_clears(sessions, true)
    bad_fill_in_one_branch(null, false)
    bad_slot_store(null)
    bad_move_store(null)
    bad_two_slots_one_nulled(null, null)
    bad_resize_one(null)
    bad_second_round(sessions)
    bad_while_second_round(sessions)
    bad_clear_only_in_loop(null, 1)
    bad_break_skips_clear(sessions)
    bad_clear_only_in_try(null)
    bad_return_in_unsafe(null, false)
    bad_fill_in_finally(null)
    bad_conditional_finally(null, true)
    bad_inner_finally_then_fill(null)
    bad_table_insert(null)
    bad_tuple(null)
    bad_emplace(null)
    bad_push_from(sessions)
    bad_push_clone(null)
    bad_runner_array(null)
    bad_ptr_field(null)
    bad_self_referential_type(null)
    bad_ref_alias(null)
    bad_block_arg_value(null)
    bad_recursive(null, 0)
    bad_caller_keeps(sessions)
    bad_caller_two_calls_down(sessions)
    bad_queue_without_flush(sessions)
    bad_helper_clears_on_one_path(null, true)
    good_clear_on_every_exit(sessions, false)
    good_finally(sessions, false)
    good_finally_fall_off(null)
    good_slot_nulled(null)
    good_resize_zero(null)
    good_delete(null)
    good_queue_then_flush(sessions)
    good_caller_clears(sessions)
    good_caller_local(sessions)
    good_calls_reported_public(sessions)
    good_call_result(null)
    good_owned(r)
    good_owned_new_local()
    good_owned_new_local_with_fields(1)
    good_global_pointer(0)
    good_no_pointers(null)
    good_not_scratch(null)
    good_local_scratch(null)
    good_block_arg_dest(null)
    g_segs |> clear()
    g_ptrs |> clear()
    g_by_id |> clear()
    g_pairs |> clear()
    g_runner.segs |> clear()
    g_runners |> clear()
    g_head.held |> clear()
}
