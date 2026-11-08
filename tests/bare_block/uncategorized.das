options gen2

require dastest/testing_boost public
require daslib/defer

// Regression: a `finally` (here via daslib/defer) runs on EVERY block exit, including
// an early `return` that lexically precedes a later variable's declaration. The
// interpreter zeroes such locals at block entry, but the stack-reuse optimization
// could hand a finally-referenced local's slot to an earlier sub-scope and clobber
// the zero -- so the finally saw garbage (and crashed when it dereferenced a pointer
// local). The stack allocator now disables slot reuse for blocks that have a `finally`,
// so those locals keep distinct, zeroed slots when the finally fires before their
// initializer ran.

var g_seen_int = -1

def private trigger_int(early : bool) {
    if (early) {
        // sub-scope locals occupying the slot `x` would otherwise reuse on this path
        var junk : int[16]
        for (i in range(16)) {
            junk[i] = 0x40000000 + i
        }
        return
    }
    var x = 12345 // nolint:LINT003 - must stay a slot-backed local; let would const-fold it
    defer() {
        g_seen_int = x   // on the early path x is never initialized; must read as 0, not garbage
    }
}

[test(description = "a finally block sees a zeroed local, not stack garbage, when an early return precedes the local's declaration")]
def test_finally_local_zeroed_on_early_exit(t : T?) {
    t |> run("finally sees a zeroed local on an early exit before its declaration") @(t : T?) {
        g_seen_int = -1
        trigger_int(true)
        t |> equal(g_seen_int, 0)
    }
}

var private g_trace = 0

def private block_used_in_defer() {
    let bump = $ {
        g_trace ++
    }
    defer() {
        invoke(bump)    // block invoked from finally - hoisted with its backing in AOT
    }
    invoke(bump)
}

def private block_after_defer() {
    defer() {
        g_trace ++
    }
    let bump = $ {      // block declared after the finally was armed
        g_trace ++
    }
    invoke(bump)
}

def private block_after_early_return(early : bool) {
    defer() {
        invoke(bump)
    }
    return if (early)
    let bump = $ {
        g_trace ++
    }
}

def private block_in_plain_finally() {
    {
        let bump = $ {
            g_trace ++
        }
        invoke(bump)
    } finally {
        invoke(bump)
    }
}

def private block_in_loop() {
    for (i in range(3)) {
        {
            let bump = $ {
                g_trace += i + 1    // captures the loop iterator
            }
            defer() {
                invoke(bump)
            }
            invoke(bump)
        }
    }
}

def private block_sees_later_mutation() {
    var x = 1
    let bump = $ {
        g_trace += x    // blocks capture by reference - reads x at invoke time
    }
    defer() {
        invoke(bump)
    }
    x = 10
}

[test(description = "a block variable invoked from a defer or plain finally works whether it is declared before or after the defer, in a loop, and sees later mutations")]
def test_block_in_finally(t : T?) {
    t |> run("block invoked from defer") @(t : T?) {
        g_trace = 0
        block_used_in_defer()
        t |> equal(g_trace, 2)
    }
    t |> run("block declared after defer") @(t : T?) {
        g_trace = 0
        block_after_defer()
        t |> equal(g_trace, 2)
    }
    t |> run("block declared after an early-return guard") @(t : T?) {
        g_trace = 0
        block_after_early_return(false)
        t |> equal(g_trace, 1)
    }
    t |> run("block invoked from plain finally") @(t : T?) {
        g_trace = 0
        block_in_plain_finally()
        t |> equal(g_trace, 2)
    }
    t |> run("block in a loop invoked from defer") @(t : T?) {
        g_trace = 0
        block_in_loop()
        t |> equal(g_trace, 12)
    }
    t |> run("finally-invoked block sees later mutations") @(t : T?) {
        g_trace = 0
        block_sees_later_mutation()
        t |> equal(g_trace, 10)
    }
}
