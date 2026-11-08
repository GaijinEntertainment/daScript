options gen2

require dastest/testing_boost public

[test(description = "finally blocks run once on every exit of if, for, while, function and block bodies, including early outs and cmres returns")]
def test_finally(t : T?) {
    t |> run("test if finally") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@test_if_finally))
        t |> success(test_if_finally())
    }
    t |> run("test nested if") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@test_nested_if))
        t |> success(test_nested_if())
    }
    t |> run("test nested if else") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@test_nested_if_else))
        t |> success(test_nested_if_else())
    }
    t |> run("test if doulbe break") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@test_if_double_break))
        t |> success(test_if_double_break())
    }
    t |> run("test for finally") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@test_for_finally))
        t |> success(test_for_finally())
    }
    t |> run("test while finally") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@test_while_finally))
        t |> success(test_while_finally())
    }
    t |> run("test func finally") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@test_func_finally))
        t |> success(test_func_finally())
    }
    t |> run("test block finally") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@test_block_finally))
        t |> success(test_block_finally())
    }
    t |> run("cmres return without finally") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@cmres_return_without_finally))
        let bag = cmres_return_without_finally()
        t |> equal(bag.a, 1)
        t |> equal(bag.b, 2)
        t |> equal(bag.c, 3)
        t |> equal(bag.d, 4)
    }
    t |> run("for loop with early out") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@early_out))
        g_steps = 0
        early_out(t, array<int>(1, 2, 3, 4))
        // per-iter inner finally fires at aidx=1 and aidx=2 (before return); outer fires once = 3
        t |> equal(3, g_steps)
    }
}

var g_steps = 0

[jit, sideeffects]
def early_out(t : T?; var aclone : array<int>) {
    t |> equal(0, lock_count(aclone))
    for (aidx in aclone) {
        if (aidx == 2) {
            t |> equal(1, lock_count(aclone))
            return
        }
    } finally {
        g_steps ++
        // per-iteration finally fires while iterator still holds the lock
        t |> equal(1, lock_count(aclone))
    }
} finally {
    t |> equal(0, lock_count(aclone))
    g_steps ++
    delete aclone
}

[jit, sideeffects]
def test_if_double_break(max : int = 10) {
    var count = 0
    for (x in range(max)) {
        if (x == 1) {
            continue
        } finally {
            count += 10
        }
        if (x == 2) {
            continue
        } finally {
            count += 100
        }
        count ++
    }
    return count == 118
}

[jit, sideeffects]
def test_nested_if(max : int = 10) {
    var t = 0
    for (x in range(max)) {
        if (x == 5) {
            if (x > 4) {
                break
            } finally {
                t++
            }
        } finally {
            t += 10
        }
    }
    return t == 11
}

[jit, sideeffects]
def test_nested_if_else(max : int = 10) {
    var t = 0
    var p = false
    for (x in range(max)) {
        if (x == 5) {
            if (x < 4) {
                p = true
            } else {
                break
            } finally {
                t++
            }
        } finally {
            t += 10
        }
    }
    return t == 11 && !p
}

[jit, sideeffects]
def test_if_finally(max : int = 10) {
    var t : int[10]
    var count = 0
    for (x in range(max)) {
        t[x] = x + 1
        if (x == 5) {
            break
        } finally {
            t[9] = 13
            count ++
        }
    }
    return count == 1 && t[9] == 13
}

[jit, sideeffects]
def test_for_finally(max : int = 10) {
    var t : int[10]
    var count = 0
    for (x in range(max)) {
        let uno = one()
        t[x] = x + uno
        if (x == 5) {
            break
        }
    } finally {
        t[9] = 13
        count += uno
    }
    // per-iter finally: iterations 0..5 each run finally (break after x==5's finally)
    return count == 6 && t[9] == 13
}

var {
    ONE = 1
}

[jit, sideeffects]
def one {
    ONE = 1
    assert(ONE == 1)
    return ONE
}

[jit, sideeffects]
def test_while_finally(max : int = 10) {
    var t : int[10]
    var count = 0
    var x = 0
    while (x != max) {
        let uno = one()
        t[x] = x + uno
        if (x == 5) {
            break
        }
        ++x
    } finally {
        t[9] = 12 + uno
        count ++
    }
    // per-iter finally: iterations 0..5 each run finally (break after x==5's finally)
    return count == 6 && t[9] == 13
}

var {
    glob = 0
    glob_count = 0
}

[jit, sideeffects]
def func_finally {
    return 12
} finally {
    glob = 13
    glob_count ++
}

[jit, sideeffects]
def test_func_finally() {
    glob = 0
    glob_count = 0
    let t = func_finally()
    return t == 12 && glob == 13 && glob_count == 1
}

[jit, sideeffects]
def test_block_finally() {
    var t = 0
    invoke() $ { t = 1; } finally { t = 2; }
    return t == 2
}

struct CmresBag {
    a : int
    b : int
    c : int
    d : int
}

// CMRES return from inside a bare block with NO finally on any enclosing block —
// exercises has_any_finally_block's false-returning path (LLVM JIT must NOT spill
// the return value to a temp when no finally is present).
[jit]
def cmres_return_without_finally() : CmresBag {
    {
        return CmresBag(a = 1, b = 2, c = 3, d = 4)
    }
}

def try_recover(a : array<int>) {
    var res : int
    try {
        res = a[1]
    } recover {
        res = -1
    }
    return res
}

[test(description = "a JIT-compiled try/recover reaches the recover arm after a panic")]
def test_try_recover(t : T?) {
    t |> run("try recover") @(t : T?) {
        var a : array<int>
        t |> equal(-1, try_recover(a))
        a |> resize(2)
        a[1] = 13
        t |> equal(13, try_recover(a))
    }
}

def private apply_it(blk : block<(x : int) : int>) : int => invoke(blk, 1)

def scope_in_arm : int {
    return apply_it() $(x : int) : int {
        var res = 0
        try {
            var inscope arr <- [1, 2, 3]
            res = length(arr) + x
        } recover {
            res = -1
        }
        return res
    }
}

def scope_in_arm_recovering : int {
    return apply_it() $(x : int) : int {
        var res = 0
        try {
            var inscope arr <- [1, 2, 3]
            res = length(arr) / (x - 1)
        } recover {
            res = -1
        }
        return res
    }
}

[test(description = "an inscope variable in a try arm is unwound before the recover arm runs")]
def test_try_recover_scope(t : T?) {
    t |> run("inscope in a try arm inside a block") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@scope_in_arm))
        t |> equal(4, scope_in_arm())
    }
    t |> run("the recover arm runs with the arm's scope unwound") @(t : T?) {
        t |> equal(-1, scope_in_arm_recovering())
    }
}

def returns_from_a_block_inside_try : int {
    var r = 0
    try {
        let f = $() : int {
            return 3
        }
        r = invoke(f)
    } recover {
        pass
    }
    return r
}

[test(description = "a return inside a block nested in a try arm returns from that block and not from the enclosing function")]
def test_a_return_from_a_closure_inside_a_try_arm_is_the_closures_own(t : T?) {
    t |> equal(returns_from_a_block_inside_try(), 3)
}
