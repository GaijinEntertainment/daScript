options gen2
options no_aot = true  // macro module — compile-time only, never AOT-compiled (like jobque_boost)
options _comment_hygiene = true

module dasllama_par shared public

require jobque public
require daslib/jobque_boost public  // emitted code uses parallel_for / invoke
require math public                 // emitted code uses min (indexed fifo arm)
require daslib/ast
require daslib/rtti
require daslib/ast_boost
require daslib/templates_boost
require daslib/macro_boost

//! `maybe_parallel_for(parallel, range_begin, range_end, num_jobs) $(rb, re) { ... }` —
//! parallel_for with a leading runtime `parallel` flag. When `parallel` is true it threads the
//! work across `num_jobs` chunks exactly like parallel_for; when false it runs the SAME block once,
//! inline on the calling thread (no job dispatch, no wait group). Lets a kernel gate on a runtime
//! size threshold without duplicating its loop body across an if/else. The block takes just the
//! chunk bounds `(rb, re)` — no `wg`/`notify_and_release` ceremony; the parallel arm adds it.
[tag_function(tag_maybe_parallel_for), unused_argument(parallel, range_begin, range_end, num_jobs, blk)]
def maybe_parallel_for(parallel : bool; range_begin, range_end : int; num_jobs : int;
                       blk : block<(rb : int; re : int) : void>) {
    //! stub — rewritten by MaybeParallelForMacro into a runtime if/else
}

//! `maybe_parallel_for(range_begin, range_end, num_jobs) $(rb, re) { ... }` — work-proportional
//! form: `num_jobs` evaluated ONCE; > 1 dispatches like the bool form's parallel arm, <= 1 runs inline.
//! Pair with matmul_chunks / matmul_chunks_gemv shapers (return 1 when not worth a dispatch).
[tag_function(tag_maybe_parallel_for_nj), unused_argument(range_begin, range_end, num_jobs, blk)]
def maybe_parallel_for(range_begin, range_end : int; num_jobs : int;
                       blk : block<(rb : int; re : int) : void>) {
    //! stub — rewritten by MaybeParallelForNjMacro into a runtime if/else
}

//! `maybe_parallel_for_indexed(range_begin, range_end, num_jobs) $(slot, rb, re) { ... }` —
//! slot-indexed dispatch: block also gets a slot index guaranteed unique across in-flight
//! invocations (size scratch to `get_dispatch_slot_bound()`); `num_jobs` <= 1 runs inline as slot 0.
[tag_function(tag_maybe_parallel_for_indexed), unused_argument(range_begin, range_end, num_jobs, blk)]
def maybe_parallel_for_indexed(range_begin, range_end : int; num_jobs : int;
                               blk : block<(slot : int; rb : int; re : int) : void>) {
    //! stub — rewritten by MaybeParallelForIndexedMacro into a runtime if/else
}

// Shared arm bodies — ONE source of truth for both lowerings so the arms can't drift.
// parallel: team mode → team_parallel_for (self-serve off one atomic); off → parallel_for (fifo
// job/chunk + notify). inline: invoke on the calling thread; wrapper names avoid shadowing user params.
def private mpf_par_arm(beginE, endE, njE, blkE : ExpressionPtr; at : LineInfo) : array<ExpressionPtr> {
    let rbN = make_unique_private_name("mpf_rb", at)
    let reN = make_unique_private_name("mpf_re", at)
    let wgN = make_unique_private_name("mpf_wg", at)
    let rbT = make_unique_private_name("mpf_trb", at)
    let reT = make_unique_private_name("mpf_tre", at)
    return <- qmacro_block_to_array() {
        // num_jobs evaluates ONCE — callers pass shaper CALLS, so a separate count call would re-run it and could profile a different value than dispatched
        if (get_jobque_team_mode()) {
            team_parallel_for($e(beginE), $e(endE), count_parallel_dispatch_thru($e(njE))) <| @($i(rbT) : int; $i(reT) : int) {
                invoke($e(blkE), $i(rbT), $i(reT))
            }
        } else {
            parallel_for($e(beginE), $e(endE), count_parallel_dispatch_thru($e(njE))) <| $($i(rbN), $i(reN), $i(wgN)) {
                invoke($e(blkE), $i(rbN), $i(reN))
                $i(wgN) |> notify_and_release
            }
        }
    }
}

[unused_argument(at)]  // symmetry with mpf_par_arm — callers pass call.at uniformly
def private mpf_inline_arm(beginE, endE, blkE : ExpressionPtr; at : LineInfo) : array<ExpressionPtr> {
    return <- qmacro_block_to_array() {
        count_inline_run()
        invoke($e(blkE), $e(beginE), $e(endE))
    }
}

[tag_function_macro(tag="tag_maybe_parallel_for")]
class private MaybeParallelForMacro : AstFunctionAnnotation {
    def override transform(var call : ExprCallFunc?; var errors : das_string) : ExpressionPtr {
        let li = call.arguments.length() - 1
        macro_verify(li == 4 && call.arguments[li] is ExprMakeBlock, compiling_program(), call.at,
            "maybe_parallel_for expects (parallel, range_begin, range_end, num_jobs) and a $(rb, re) block")
        // num_jobs stays lazily evaluated in the parallel arm only (the bool already decided the arm).
        var parArm <- mpf_par_arm(call.arguments[1], call.arguments[2], call.arguments[3],
            call.arguments[4], call.at)
        var inlArm <- mpf_inline_arm(call.arguments[1], call.arguments[2], call.arguments[4], call.at)
        var qb = qmacro_block() {
            if ($e(call.arguments[0])) {
                $b(parArm)
            } else {
                $b(inlArm)
            }
        }
        qb |> force_at(call.at)
        return <- qb
    }
}

// Indexed arm bodies. team: team_parallel_for_indexed passes the claiming worker's slot straight through.
// fifo: no worker identity exists, so the range collapses to min(num_jobs, pool+1) sub-ranges —
// the sub-range INDEX is the slot (one fifo job each). inline: one invoke as slot 0.
def private mpfi_team_arm(beginE, endE, njE, blkE : ExpressionPtr; at : LineInfo) : array<ExpressionPtr> {
    let slT = make_unique_private_name("mpfi_slot", at)
    let jbT = make_unique_private_name("mpfi_jb", at)
    let jeT = make_unique_private_name("mpfi_je", at)
    return <- qmacro_block_to_array() {
        team_parallel_for_indexed($e(beginE), $e(endE), count_parallel_dispatch_thru($e(njE))) <| @($i(slT) : int; $i(jbT) : int; $i(jeT) : int) {
            invoke($e(blkE), $i(slT), $i(jbT), $i(jeT))
        }
    }
}

def private mpfi_fifo_arm(beginE, endE, njE, blkE : ExpressionPtr; at : LineInfo) : array<ExpressionPtr> {
    let nsN = make_unique_private_name("mpfi_ns", at)
    let sbN = make_unique_private_name("mpfi_sb", at)
    let seN = make_unique_private_name("mpfi_se", at)
    let wgN = make_unique_private_name("mpfi_wg", at)
    let slN = make_unique_private_name("mpfi_sl", at)
    let jbN = make_unique_private_name("mpfi_jb", at)
    let jeN = make_unique_private_name("mpfi_je", at)
    let capN = make_unique_private_name("mpfi_cap", at)
    return <- qmacro_block_to_array() {
        let $i(capN) = is_job_que_available() ? get_total_hw_jobs() + 1 : 1
        let $i(nsN) = min(min($e(njE), $e(endE) - $e(beginE)), $i(capN))
        if ($i(nsN) > 1) {
            parallel_for(0, $i(nsN), count_parallel_dispatch_thru($i(nsN))) <| $($i(sbN), $i(seN), $i(wgN)) {
                for ($i(slN) in range($i(sbN), $i(seN))) {
                    let $i(jbN) = $e(beginE) + int(int64($i(slN)) * int64($e(endE) - $e(beginE)) / int64($i(nsN)))
                    let $i(jeN) = $e(beginE) + int(int64($i(slN) + 1) * int64($e(endE) - $e(beginE)) / int64($i(nsN)))
                    invoke($e(blkE), $i(slN), $i(jbN), $i(jeN))
                }
                $i(wgN) |> notify_and_release
            }
        } else {
            count_inline_run()
            invoke($e(blkE), 0, $e(beginE), $e(endE))
        }
    }
}

[tag_function_macro(tag="tag_maybe_parallel_for_indexed")]
class private MaybeParallelForIndexedMacro : AstFunctionAnnotation {
    def override transform(var call : ExprCallFunc?; var errors : das_string) : ExpressionPtr {
        let li = call.arguments.length() - 1
        macro_verify(li == 3 && call.arguments[li] is ExprMakeBlock, compiling_program(), call.at,
            "maybe_parallel_for_indexed expects (range_begin, range_end, num_jobs) and a $(slot, rb, re) block")
        // all three scalars bind ONCE — the arms (fifo sub-range math) read them repeatedly
        let rbN = make_unique_private_name("mpfi_rb0", call.at)
        let reN = make_unique_private_name("mpfi_re0", call.at)
        let njN = make_unique_private_name("mpfi_nj", call.at)
        var rbRead = qmacro($i(rbN))
        var reRead = qmacro($i(reN))
        var njRead = qmacro($i(njN))
        var teamArm <- mpfi_team_arm(rbRead, reRead, njRead, call.arguments[3], call.at)
        var fifoArm <- mpfi_fifo_arm(rbRead, reRead, njRead, call.arguments[3], call.at)
        var qb = qmacro_block() {
            let $i(rbN) = $e(call.arguments[0])
            let $i(reN) = $e(call.arguments[1])
            let $i(njN) = $e(call.arguments[2])
            if ($i(njN) > 1 && is_job_que_available() && get_jobque_team_mode()) {
                $b(teamArm)
            } else {
                $b(fifoArm)
            }
        }
        qb |> force_at(call.at)
        return <- qb
    }
}

[tag_function_macro(tag="tag_maybe_parallel_for_nj")]
class private MaybeParallelForNjMacro : AstFunctionAnnotation {
    def override transform(var call : ExprCallFunc?; var errors : das_string) : ExpressionPtr {
        let li = call.arguments.length() - 1
        macro_verify(li == 3 && call.arguments[li] is ExprMakeBlock, compiling_program(), call.at,
            "maybe_parallel_for expects (range_begin, range_end, num_jobs) and a $(rb, re) block")
        // num_jobs binds ONCE — it both decides which arm runs and feeds the dispatch.
        let njN = make_unique_private_name("mpf_nj", call.at)
        var njRead = qmacro($i(njN))
        var parArm <- mpf_par_arm(call.arguments[0], call.arguments[1], njRead,
            call.arguments[3], call.at)
        var inlArm <- mpf_inline_arm(call.arguments[0], call.arguments[1], call.arguments[3], call.at)
        var qb = qmacro_block() {
            let $i(njN) = $e(call.arguments[2])
            if ($i(njN) > 1) {
                $b(parArm)
            } else {
                $b(inlArm)
            }
        }
        qb |> force_at(call.at)
        return <- qb
    }
}
