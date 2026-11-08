options gen2
options max_framed_call_depth = 256
options stack = 1048576

require dastest/testing_boost public
require daslib/ast
require daslib/rtti
require daslib/debugger
require daslib/fio
require strings
require _framed_call_depth_recursers

//! The recursers live in a no_aot module, so this file runs under AOT
//! and JIT as well: each sweep exercises its own recover handler (the
//! try node, jit_try_recover, das_try_recover). The simulated stack
//! is sized never to run out before the cap: the cap is the
//! diagnostic under test, `stack overflow while calling` would be the
//! other one.

let DEPTH_LIMIT = 256

//! The framed counter counts every interpreted framed call, the
//! harness's own included, so the levels a recurser has left under
//! the cap depend on the tier running this file: rec_count measures
//! them, and every recurser below has the same shape - one framed
//! entry and n + 1 frames of recursion - so `reach` passes and
//! `reach + 1` panics for each of them.
def deepest_level() : int {
    g_reached = -1
    try {
        g_sink = framed_rec_count()
    } recover {
    }
    return g_reached
}

[test(description = "framed recursion at the deepest level the cap allows passes and one call past it panics naming the option, for one, two and nine arguments and a struct result")]
def test_call_depth_panics(t : T?) {
    g_no_fold = 0
    t |> run("unbounded framed recursion panics naming the option") @(t : T?) {
        let why = panic_text() $ {
            g_sink = rec_forever(g_no_fold)
        }
        let hit = find(why, "max_framed_call_depth 256 exceeded while calling") >= 0
        t |> success(hit, why)
    }
    let reach = deepest_level()
    t |> success(reach > 16 && reach < DEPTH_LIMIT,
        "the probe reaches a level under the cap: {reach}")
    t |> run("one argument: at the deepest level passes, one call past it panics") @(t : T?) {
        t |> equal(framed_rec_sum(reach + g_no_fold), triangular(reach))
        let past = panic_text() $ { g_sink = framed_rec_sum(reach + 1 + g_no_fold) }
        t |> success(past != "", "one call past the cap must panic")
    }
    t |> run("two plain arguments, the fused shape") @(t : T?) {
        t |> equal(framed_rec_sum2(reach + g_no_fold), triangular(reach))
        let past = panic_text() $ { g_sink = framed_rec_sum2(reach + 1 + g_no_fold) }
        t |> success(past != "", "one call past the cap must panic")
    }
    t |> run("nine arguments, the variadic node") @(t : T?) {
        t |> equal(framed_rec_nine(reach + g_no_fold), 36)
        let past = panic_text() $ { g_sink = framed_rec_nine(reach + 1 + g_no_fold) }
        t |> success(past != "", "one call past the cap must panic")
    }
    t |> run("a struct result, the copy-or-move-on-return node") @(t : T?) {
        t |> equal(framed_rec_pair(reach + g_no_fold), reach * 1000 + triangular(reach))
        let past = panic_text() $ { g_sink = framed_rec_pair(reach + 1 + g_no_fold) }
        t |> success(past != "", "one call past the cap must panic")
    }
}

[test(description = "a thousand recovered framed overflows leave the depth counter where it was, so the probe reaches the same level after them")]
def test_recovered_overflow_leaves_no_drift(t : T?) {
    g_no_fold = 0
    t |> run("a thousand recovered overflows leave the probe's level unchanged") @(t : T?) {
        let before = deepest_level()
        for (_i in range(1000)) {
            try {
                g_sink = rec_forever(g_no_fold)
            } recover {
            }
        }
        t |> equal(deepest_level(), before)
    }
}

//! A program with no options line, capped at 16 by the host's
//! CodeOfPolicies: at_cap nests 16 calls, past_cap 17, and overflow
//! never returns; each body keeps a local so it stays framed.
let HOST_CAPPED_SOURCE = (
    "options gen2\n" +
    "var g_no_fold = 0\n" +
    "var g_sink = 0\n" +
    "def rec_sum(n : int) : int \{\n" +
    "    var here = n\n" +
    "    if (here == 0) \{\n" +
    "        return 0\n" +
    "    \}\n" +
    "    return here + rec_sum(n - 1)\n" +
    "\}\n" +
    "def rec_forever(n : int) : int \{\n" +
    "    var next = n + 1\n" +
    "    if (next < 0) \{\n" +
    "        return 0\n" +
    "    \}\n" +
    "    return rec_forever(next)\n" +
    "\}\n" +
    "[export]\n" +
    "def at_cap \{\n" +
    "    g_sink = rec_sum(15 + g_no_fold)\n" +
    "\}\n" +
    "[export]\n" +
    "def past_cap \{\n" +
    "    g_sink = rec_sum(16 + g_no_fold)\n" +
    "\}\n" +
    "[export]\n" +
    "def overflow \{\n" +
    "    g_sink = rec_forever(g_no_fold)\n" +
    "\}\n")

[test(description = "a host CodeOfPolicies cap limits framed call depth in a program with no options line, and a host-side catch of the overflow restores the counter")]
def test_host_policy_and_host_catch(t : T?) {
    t |> run("the host policy caps the program and a host catch restores the counter") @(t : T?) {
        let path = path_join(test_temp_dir(), "host_capped_framed.das")
        fwrite(path, HOST_CAPPED_SOURCE)
        var inscope access <- make_file_access("")
        using() $(var mg : ModuleGroup) {
            using() $(var cop : CodeOfPolicies) {
                cop.max_framed_call_depth = 16u
                cop.threadlock_context = true
                compile_file(path, access, unsafe(addr(mg)), cop) $(ok; program; issues) {
                    t |> success(ok, "the capped program compiles: {issues}")
                    return if (!ok)
                    simulate(program) $(sok; ctx; serrors) {
                        t |> success(sok, "the capped program simulates: {serrors}")
                        return if (!sok)
                        unsafe {
                            t |> equal(panic_text() $ { invoke_in_context(ctx, "at_cap") }, "")
                            let past = panic_text() $ { invoke_in_context(ctx, "past_cap") }
                            t |> success(find(past, "max_framed_call_depth 16 exceeded") >= 0,
                                "the host policy is the cap: {past}")
                            let escaped = panic_text() $ { invoke_in_context(ctx, "overflow") }
                            t |> success(find(escaped, "max_framed_call_depth 16 exceeded") >= 0,
                                "the overflow escapes to the caller: {escaped}")
                            t |> equal(panic_text() $ { invoke_in_context(ctx, "at_cap") }, "")
                        }
                    }
                }
            }
        }
    }
}
