options gen2
options no_unused_function_arguments = false
require dastest/testing_boost public
require daslib/random
require daslib/if_not_null
require daslib/defer

// --- helpers for postfix if ---

def returns_13_if_less_10(a : int) : int {
    return 13 if (a < 10)
    return a
}

def returns_with_else(a : int) : int {
    return 13 if (a < 10) else return 42
}

// --- helpers for prefix if ---

def prefix_early_return(a : int) : int {
    if (a < 10) return 13
    return a
}

// --- helpers for multi-line one-liner shapes (previously rejected)
// --- expression_if_one_liner accepts
// return/yield/break/continue/expr_no_bracket; these helpers exercise
// the now-legal "body on the next line" layout.

def if_body_on_next_line(a : int) : int {
    if (a < 10)
        return 1
    return 0
}

def else_body_on_next_line(a : int) : int {
    if (a < 10) {
        return 1
    } else
        return 2
}

def if_and_else_bodies_on_next_lines(a : int) : int {
    if (a < 10)
        return 1
    else
        return 2
}

def elif_body_on_next_line(a : int) : int {
    if (a == 1) {
        return 1
    } elif (a == 2)
        return 2
    return 0
}

def static_if_true_body_on_next_line() : int {
    static_if (true)
        return 7
    return 0
}

def static_if_false_body_on_next_line() : int {
    static_if (false)
        return 7
    return 0
}

def postfix_if_split_across_lines(a : int) : int {
    // historically rejected: parses as `return 13` (unconditional)
    // followed by `if (a < 10) return a` (prefix-if, body on next line).
    // Compiles cleanly now; always returns 13.
    return 13
        if (a < 10)     // nolint:LINT001 - the dead tail is exactly what this function pins
    return a
}

// --- tests ---

[test(description = "a postfix `return x if (cond)` returns only when its condition holds")]
def test_postfix_if(t : T?) {
    t |> run("return when condition is true") @(t : T?) {
        t |> equal(returns_13_if_less_10(5), 13)
    }
    t |> run("fall through when condition is false") @(t : T?) {
        t |> equal(returns_13_if_less_10(20), 20)
    }
    t |> run("boundary at 10") @(t : T?) {
        t |> equal(returns_13_if_less_10(10), 10)
    }
}

[test(description = "a postfix if with an else returns the matching branch's value")]
def test_postfix_if_else(t : T?) {
    t |> run("true branch") @(t : T?) {
        t |> equal(returns_with_else(5), 13)
    }
    t |> run("else branch") @(t : T?) {
        t |> equal(returns_with_else(20), 42)
    }
}

[test(description = "a braceless prefix `if (cond) return x` returns early only when its condition holds")]
def test_prefix_if_oneliner(t : T?) {
    t |> run("return early when condition met") @(t : T?) {
        t |> equal(prefix_early_return(5), 13)
    }
    t |> run("fall through when condition not met") @(t : T?) {
        t |> equal(prefix_early_return(20), 20)
    }
}

[test(description = "a braceless prefix if runs its one-statement body only when its condition holds")]
def test_prefix_if_oneliner_body(t : T?) {
    t |> run("executes body when true") @(t : T?) {
        var x = 0
        if (true) { x = 42; }   // nolint:STYLE010 - the literal-true one-liner body is the tested construct
        t |> equal(x, 42)
    }
    t |> run("skips body when false") @(t : T?) {
        var x = 0
        if (false) { x = 42; }
        t |> equal(x, 0)
    }
}

[test(description = "braceless if with break or continue as its body steers the loop")]
def test_oneliner_break_continue(t : T?) {
    t |> run("break in prefix if one-liner") @(t : T?) {
        var count = 0
        for (i in range(10)) {
            if (i == 3) break
            count++
        }
        t |> equal(count, 3)
    }
    t |> run("continue in prefix if one-liner") @(t : T?) {
        var count = 0
        for (i in range(6)) {
            if (i % 2 == 0) continue
            count++
        }
        t |> equal(count, 3)
    }
}

[test(description = "a braceless if body written on the line after the condition parses as that if's body")]
def test_if_body_on_next_line(t : T?) {
    t |> run("body executes when condition true") @(t : T?) {
        t |> equal(if_body_on_next_line(5), 1)
    }
    t |> run("body skipped when condition false") @(t : T?) {
        t |> equal(if_body_on_next_line(20), 0)
    }
}

[test(description = "a braceless else body written on the line after the else parses as the else body")]
def test_else_body_on_next_line(t : T?) {
    t |> run("if branch taken") @(t : T?) {
        t |> equal(else_body_on_next_line(5), 1)
    }
    t |> run("else branch taken") @(t : T?) {
        t |> equal(else_body_on_next_line(20), 2)
    }
}

[test(description = "braceless if and else bodies both on their own next lines pair up correctly")]
def test_if_and_else_bodies_on_next_lines(t : T?) {
    t |> run("if branch taken") @(t : T?) {
        t |> equal(if_and_else_bodies_on_next_lines(5), 1)
    }
    t |> run("else branch taken") @(t : T?) {
        t |> equal(if_and_else_bodies_on_next_lines(20), 2)
    }
}

[test(description = "a braceless elif body on the next line belongs to that elif")]
def test_elif_body_on_next_line(t : T?) {
    t |> run("if branch taken") @(t : T?) {
        t |> equal(elif_body_on_next_line(1), 1)
    }
    t |> run("elif branch taken") @(t : T?) {
        t |> equal(elif_body_on_next_line(2), 2)
    }
    t |> run("neither branch taken") @(t : T?) {
        t |> equal(elif_body_on_next_line(3), 0)
    }
}

[test(description = "a braceless static_if body on the next line is kept or dropped by the constant condition")]
def test_static_if_body_on_next_line(t : T?) {
    t |> run("static_if (true) body on next line returns 7") @(t : T?) {
        t |> equal(static_if_true_body_on_next_line(), 7)
    }
    t |> run("static_if (false) elides next-line body, returns 0") @(t : T?) {
        t |> equal(static_if_false_body_on_next_line(), 0)
    }
}

[test(description = "a statement followed by an if on the next line parses as two statements - an unconditional return, then a prefix if - not as a postfix if")]
def test_postfix_if_split_across_lines(t : T?) {
    t |> run("split layout compiles and returns 13") @(t : T?) {
        t |> equal(postfix_if_split_across_lines(5), 13)
        t |> equal(postfix_if_split_across_lines(20), 13)
    }
}

[test(description = "a for loop over a constant fixed_array literal visits its elements")]
def test_for_const_array(t : T?) {
    t |> run("iterate fixed_array constant") @(t : T?) {
        for (x in fixed_array<int>(1, 1)) {
            t |> equal(x, 1)
        }
    }
}

def test_while_continue(t : T?) {
    var x = 0
    var y = 0
    while (x < 10) {
        x++
        if ((x & 1) == 0) {
            continue
        }
        y += x
    }
    t |> equal(y, 1 + 3 + 5 + 7 + 9)
}

def test_for_continue_impl(t : T?) {
    var y = 0
    for (x in range(10)) {
        if ((x & 1) == 0) {
            continue
        }
        y += x
    }
    t |> equal(y, 1 + 3 + 5 + 7 + 9)
}

def test_complex_for_continue(t : T?) {
    var y = 0
    let arr = fixed_array<int>(0, 1, 2, 3, 4, 5, 6, 7, 8, 9)
    for (x, s in range(10), arr) {
        t |> equal(x, s)
        if ((x & 1) == 0) {
            continue
        }
        y += x
    }
    t |> equal(y, 1 + 3 + 5 + 7 + 9)
}

[test(description = "continue skips the rest of the iteration in while, for and zipped for loops")]
def test_continue(t : T?) {
    t |> run("while continue") @(t : T?) {
        test_while_continue(t)
    }
    t |> run("for continue") @(t : T?) {
        test_for_continue_impl(t)
    }
    t |> run("complex for continue") @(t : T?) {
        test_complex_for_continue(t)
    }
}

def test_from_gte_to(t : T?) {
    for (_x in range(3, 2)) {
        t |> failure("loop body should not execute")
    }
    for (_x in urange(3, 2)) {
        t |> failure("loop body should not execute")
    }
    for (_x in range(3, 2)) {
        t |> failure("loop body should not execute")
    }
    for (_x in urange(3, 2)) {
        t |> failure("loop body should not execute")
    }
    for (_x in range(3, 2)) {
        break
    }
    for (_x in urange(3, 2)) {
        break
    }
    for (_x in range(3, 2)) {
        t |> failure("loop body should not execute")
        break
    }
    for (_x in urange(3, 2)) {
        t |> failure("loop body should not execute")
        break
    }
    for (_x, _y in range(3, 2), urange(3, 2)) {
        t |> failure("loop body should not execute")
    }
    for (_x, _y in urange(3, 2), urange(3, 2)) {
        t |> failure("loop body should not execute")
    }
    for (_x in each(range(3, 2))) {
        t |> failure("loop body should not execute")
    }
}

def test_for64(t : T?) {
    {
        var val = 0l
        for (x, y in range64(100), count()) {
            t |> equal(val, x)
            t |> equal(val, int64(y))
            val ++
        }
        t |> equal(val, 100l)
    }
    {
        var val = 0ul
        for (x in urange64(100)) {
            t |> equal(val, x)
            val ++
        }
        t |> equal(val, 100ul)
    }
}

[test(description = "for loops over fixed arrays, dynamic arrays, ranges, urange and 64-bit ranges, one or two variables, write and read every element, stop at the shorter source, and skip an empty range")]
def test_for_loop(t : T?) {     // nolint:STYLE038 - one flat arm per loop shape
    t |> run("1 variable fixed array") @(t : T?) {
        var a : int[10]
        var i = 0
        for (l in a) {
            l = i
            i++
        }
        t |> equal(i, 10)
        i = 0
        for (l in a) {
            t |> equal(l, i)
            i++
        }
    }
    t |> run("1 variable dynamic array") @(t : T?) {
        var a : array<int>
        var i = 0
        resize(a, 13)
        for (l in a) {
            l = i++
        }
        t |> equal(i, 13)
        i = 0
        for (l in a) {
            t |> equal(l, i)
            i++
        }
    }
    t |> run("2 variables fixed array") @(t : T?) {
        var a, b : int[10]
        var i = 0
        for (l, r in a, b) {
            r = i * 2 - 5
            l = i++
        }
        t |> equal(i, 10)
        i = 0
        for (l, r in a, b) {
            t |> equal(r, i * 2 - 5)
            t |> equal(l, i)
            i++
        }
        i = 0
        for (l, r in a, b) {
            if (l > r) {
                i++
            }
        }
        t |> equal(i, 5)
    }
    t |> run("2 variables hybrid array") @(t : T?) {
        var a : array<int>
        var b : int[10]
        var i = 0
        resize(a, 4)
        for (l, r in a, b) {
            t |> success(i < 4)
            r = i * 2 - 5
            l = i++
        }
        t |> equal(i, 4)
        i = 0
        for (l, r in a, b) {
            t |> equal(r, i * 2 - 5)
            t |> equal(l, i)
            i++
        }
        t |> equal(i, 4)
        i = 0
        for (l, r in a, b) {
            if (l > r) {
                i++
            }
        }
        t |> equal(i, 4)
    }
    t |> run("1 variable range") @(t : T?) {
        var i = 0
        for (a in range(10)) {
            t |> equal(a, i)
            i++
        }
        t |> equal(i, 10)
    }
    t |> run("2 variables range hybrid") @(t : T?) {
        var b : int[10]
        var i = 0
        for (_l, r in range(10), b) {
            r = i++ * 2 - 5
        }
        t |> equal(i, 10)
        i = 0
        for (l, r in range(10), b) {
            t |> equal(r, i * 2 - 5)
            t |> equal(l, i)
            i++
        }
        i = 0
        for (l, r in range(10), b) {
            if (l > r) {
                i++
            }
        }
        t |> equal(i, 5)
    }
    t |> run("urange") @(t : T?) {
        var u = 0u
        for (tt in urange(0u, 3u)) {
            u += tt
        }
        t |> equal(u, 3u)
    }
    t |> run("reverse range (from >= to)") @(t : T?) {
        test_from_gte_to(t)
    }
    t |> run("64-bit ranges") @(t : T?) {
        test_for64(t)
    }
}

[test(description = "a for loop over a one-element array literal visits that element")]
def test_for_single_element_array(t : T?) {
    t |> run("iterate single-element dim") @(t : T?) {
        let a = [1234]
        for (x in a) {
            t |> equal(x, 1234)
        }
    }
}

// each_ref turns a lambda into a reference iterator; generator<T&>
// lowers to an each_ref call
var g_src : array<int>

def make_lam() : lambda<(var a : int?&) : bool> {
    var idx = 0  // nolint:LINT003 - the lambda's captured copy mutates
    return <- @(var a : int?&) : bool {
        if (idx >= length(g_src)) {
            return false
        }
        a = unsafe(addr(g_src[idx]))
        idx++
        return true
    }
}

[test(description = "each_ref over a pointer-yielding lambda, and a generator<int&> that lowers to it, iterate genuine references, so writes reach the source array")]
def test_each_ref(t : T?) {
    t |> run("user lambda yields writable refs") @(t : T?) {
        g_src := [10, 20, 30]
        let lam <- make_lam()
        var sum = 0
        for (x in each_ref(lam)) {
            sum += x
            x = x * 10
        }
        t |> equal(60, sum)
        t |> equal(100, g_src[0])
        t |> equal(200, g_src[1])
        t |> equal(300, g_src[2])
    }
    t |> run("ref generator lowers to each_ref") @(t : T?) {
        g_src := [1, 2, 3]
        var gen <- generator<int&> {
            for (i in range(length(g_src))) {
                yield g_src[i]
            }
            return false
        }
        for (x in gen) {
            x += 5
        }
        t |> equal(6, g_src[0])
        t |> equal(7, g_src[1])
        t |> equal(8, g_src[2])
    }
}

var lr_seed : int4

typedef LrFoo = tuple<int; float3>

def lr_foo(var attached_fires : array<LrFoo>) {
    let tf = random_int(lr_seed) % 100
    for (x in attached_fires) {
        if (x._0 == tf) {
            return
        }
    }
    push(attached_fires, (tf, float3(0)))
}

var lr_polls = 0

[never_inline]
def lr_more() : bool {
    lr_polls++
    return lr_polls < 100
}

def lr_return_from_while() : int {
    while (lr_more()) {
        if (lr_polls >= 3) {
            return lr_polls
        }
        var inscope seen : array<int>
        seen |> push(lr_polls)
    }
    return -1
}

[test(description = "a return from inside a for or while loop leaves the function at once, also when the loop body has a finalized local")]
def test_loop_ret(t : T?) {
    t |> run("loop early return fills unique entries") @(t : T?) {
        var a : array<LrFoo>
        for (_x in range(1000)) {
            lr_foo(a)
        }
        t |> equal(length(a), 100)
    }
    t |> run("return leaves a while loop whose body has a finalizer") @(t : T?) {
        t |> equal(lr_return_from_while(), 3)
        t |> equal(lr_polls, 3)
    }
}

[sideeffects]
def basic_goto(tt : T?) {
    var fail = false
    var arr : int[10]
    var i = 0
    label 0:
    arr[i] = i
    i ++
    if (i < 10) {
        goto label 0
        fail = true
        tt |> failure("unreachable code")
    }
    for (t in range(10)) {
        tt |> equal(arr[t], t)
    }
    tt |> success(!fail)
}

[sideeffects]
def nested_goto(tt : T?) {
    var fail = false
    var arr : int[10]
    var i = 0
    label 0:
    arr[i] = i
    i ++
    if (i < 10) {
        if (i < 10) {
            goto label 0
            fail = true
        }
        fail = true
    }
    for (t in range(10)) {
        tt |> equal(arr[t], t)
    }
}

[sideeffects]
def for_goto(tt : T?) {
    var fail = false
    var arr : int[10]
    for (j in range(10)) {
        var i = 0
        label 0:
        arr[i] = i + j
        i ++
        if (i < 10) {
            goto label 0
            fail = true
        }
        for (t in range(10)) {
            tt |> equal(arr[t], t + j)
        }
        tt |> success(!fail)
    }
    for (t in range(10)) {
        tt |> equal(arr[t], t + 9)
    }
    tt |> success(!fail)
}

[sideeffects]
def while_goto(tt : T?) {
    var fail = false
    var arr : int[10]
    var j = 0
    while (j < 10) {
        var i = 0
        label 0:
        arr[i] = i + j
        i ++
        if (i < 10) {
            goto label 0
            fail = true
        }
        for (t in range(10)) {
            tt |> equal(arr[t], t + j)
        }
        tt |> success(!fail)
        j ++
    }
    for (t in range(10)) {
        tt |> equal(arr[t], t + 9)
    }
    tt |> success(!fail)
}

[sideeffects, no_jit]
def finally_goto(tt : T?; yes : bool = true) {
    var fail = true
    if (yes) {
        fail = true
        goto label 0
    } finally {
        fail = false
    }
    label 0:
    tt |> success(!fail)
}

[sideeffects]
def gotoN(n : int) {
    var res = -1
    goto n
    label 0:
    res = 0
    goto label 3
    label 1:
    res = 1
    goto label 3
    label 2:
    res = 2
    goto label 3
    label 3:
    return res
}

[test(description = "goto label loops back in plain code, nested ifs, for and while bodies, runs the finally it jumps out of, and a computed goto lands on its numbered label")]
def test_labels(t : T?) {
    t |> run("basic goto") @(t : T?) { basic_goto(t); }
    t |> run("nested goto") @(t : T?) { nested_goto(t); }
    t |> run("goto inside for-loop") @(t : T?) { for_goto(t); }
    t |> run("goto inside while-loop") @(t : T?) { while_goto(t); }
    t |> run("goto with finally") @(t : T?) { finally_goto(t); }
    t |> run("computed goto") @(t : T?) {
        t |> equal(gotoN(0), 0)
        t |> equal(gotoN(1), 1)
        t |> equal(gotoN(2), 2)
        t |> equal(gotoN(3), -1)
    }
}

def bw(c : int) : int {
    var r = 0
    var n = c
    label 1:
    r++
    while (true) {
        if (n > 0) {
            n--
            goto label 1
        }
        label 2:
        r += 100
        break
    }
    return r
}

[test(description = "a goto from a loop body to an outer label numbered below the body's own labels lands on the outer label")]
def test_a_goto_from_a_loop_body_to_an_outer_label_numbered_below_the_bodys_labels(t : T?) {
    t |> equal(bw(2), 103)
}

[export]
def skip_init(n : int) : int {
    if (n > 1) {
        goto label 1
    }
    var g = n * 2
    label 1:
    g = n
    return g
}

[export]
def skip_always(n : int) : int {
    goto label 1
    var g = n * 2
    label 1:
    g = n + 1
    return g
}

[never_inline]
def call_it(blk : block<(x : int) : int>; v : int) : int => invoke(blk, v)

[export]
def skip_block_let(n : int) : int {
    if (n > 1) {
        goto label 1
    }
    let b = $(x : int) : int => x + n
    label 1:
    return n > 1 ? n : call_it(b, 1)
}

[test(description = "a goto past a var or block-typed let declaration runs on every tier, and the skipped declaration still exists after the label")]
def test_a_goto_past_a_var_declaration_runs_on_every_tier(t : T?) {
    t |> equal(skip_init(3), 3)
    t |> equal(skip_init(1), 1)
    t |> equal(skip_always(3), 4, "the declaration survives the unreachable region")
    t |> equal(skip_block_let(1), 2, "a block-typed let after a conditional goto")
    t |> equal(skip_block_let(3), 3)
}

var g_gl_r = 0

def gl_b(c : int) : int {
    var q = 0
    if (c > 0) {
        if (c == 1) {
            goto label 3
        }
        q += 100
        label 3:
    }
    q += 7
    return q
}

def gl_f(c : int) {
    if (c == 1) {
        goto label 3
    }
    g_gl_r += 100
    label 3:
}

def gl_o(c : int) : int {
    var k = 0
    var n = c
    label 1:
    k++
    if (n > 0) {
        label 2:
        n--
        goto label 1
    }
    return k
}

[test(description = "a goto to a label that ends a block finishes the block and runs the statement after it")]
def test_a_goto_to_a_label_that_ends_a_block(t : T?) {
    t |> equal(gl_b(1), 7, "the block finishes and the statement after it runs")
    t |> equal(gl_b(2), 107)
    gl_f(1)
    gl_f(2)
    t |> equal(g_gl_r, 100)
}

[test(description = "a goto from inside a labelled if body to an outer label loops back correctly")]
def test_a_goto_from_a_labelled_block_to_an_outer_label(t : T?) {
    t |> equal(gl_o(2), 3)
}

def cg_run(c : int) : int {
    var r = 0
    while (true) {
        goto c + 1000
        label 1000:
        if (c == 0) {
            r = 10
            break
        }
        label 1001:
        if (c == 1) {
            r = 11
            break
        }
        r = -1
        break
    }
    return r
}

[sideeffects]
def jump_to(target : int) : int {
    var r = 0
    try {
        goto target
        label 1:
        r = 1
    } recover {
        r = -1
    }
    return r
}

[test(description = "a computed goto into a while body lands on the label its runtime value names")]
def test_a_computed_goto_into_a_while_body_lands_on_its_label(t : T?) {
    let k = 1
    t |> equal(11, cg_run(k), "the target held in a variable")
    for (j in range(2)) {
        t |> equal(10 + j, cg_run(j), "the target from a loop variable {j}")
    }
}

[sideeffects]
def jump_in_block(target : int) : int {
    return invoke($(n : int) : int {
        var q = 0
        try {
            goto n
            label 3:
            q = 3
        } recover {
            q = -1
        }
        return q
    }, target)
}

[sideeffects]
def jump_past_a_block_label(target : int) : int {
    var r = 0
    try {
        goto target
        label 1:
        r = 1
    } recover {
        r = -1
    }
    return r + invoke($() : int {
        label 3:
        return 3
    })
}

[test(description = "a computed goto resolves only against the labels of its own function or block literal, never another's")]
def test_a_computed_goto_resolves_against_the_labels_of_its_own_block(t : T?) {
    let three = 3
    t |> equal(3, jump_in_block(three), "a block literal's computed goto lands on its own label")
    let one = 1
    t |> equal(-1, jump_in_block(one),
               "a block literal's computed goto does not see the function's labels")
    t |> equal(4, jump_past_a_block_label(one))
    t |> equal(2, jump_past_a_block_label(three),
               "a label only a block literal declares is not the function's")
}

[sideeffects]
def nine : int => 9

var g_init_jump = invoke($() : int {
    var r = 0  // nolint:LINT003 - written inside the try arms of a block literal
    try {
        goto nine()
        label 1:
        r = 1
    } recover {
        r = -2
    }
    return r
})

[test(description = "a computed goto to a number no label declares raises a recoverable run-time error, also in a global initializer")]
def test_a_computed_goto_to_a_label_nothing_declares_throws(t : T?) {
    let one = 1
    t |> equal(1, jump_to(one))
    let missing = 77
    t |> equal(-1, jump_to(missing), "a label no block declares is a run-time error")
    let negative = -5
    t |> equal(-1, jump_to(negative))
    let unused = 0
    t |> equal(-1, jump_to(unused), "a number below the highest label that no label uses")
    t |> equal(-2, g_init_jump, "a global initializer's computed goto is checked too")
}

struct IfnnFoo {
    a : int
}

var g_ifnn_called = false

[sideeffects]
def ifnn_take3(pfoo : IfnnFoo; b, c : int) {
    assert(pfoo.a == 1 && b == 2 && c == 3)
    g_ifnn_called = true
}

[test(description = "if_not_null calls the piped function with the dereferenced pointer only when the pointer is not null")]
def test_if_not_null(t : T?) {
    t |> run("null pointer skips block") @(t : T?) {
        var pfoo : IfnnFoo?
        g_ifnn_called = false
        pfoo |> if_not_null() <| ifnn_take3(0, 0)
        t |> equal(g_ifnn_called, false)
    }
    t |> run("non-null pointer invokes block") @(t : T?) {
        var pfoo = new IfnnFoo(a = 1)
        g_ifnn_called = false
        pfoo |> if_not_null() <| ifnn_take3(2, 3)
        t |> equal(g_ifnn_called, true)
        unsafe { delete pfoo; }
    }
}

[test(description = "an if over a false bool variable skips its body")]
def test_bool_condition(t : T?) {
    t |> run("false condition skips body") @(t : T?) {
        let a = false
        if (a) {
            t |> failure("should not reach here")
        }
        t |> success(true)
    }
}

def defer_check(var x : int&) {
    defer() {
        // 2nd
        assert(x == 2)
        x *= 2
    }
    defer() {
        // 1st
        assert(x == 3)
        x--
    }
    x += 2
}

def defer_check_del(var a : array<int>) : int {
    a <- [for (x in range(10)); x * x]   // nolint:PERF030 - the parameter arrives empty; this is the fixture the defer-delete test needs
    var inscope tmp <- a
    a <- tmp                             // nolint:PERF030 - a was just emptied by the move into tmp; the round trip is the tested construct
    defer() {
        delete a
    }
    return length(a)
}

def defer_delete_check(var a : array<int>) : int {
    a <- [for (x in range(5)); x]   // nolint:PERF030 - the parameter arrives empty; this is the fixture the defer_delete arm needs
    defer_delete(a)
    return length(a)
}

[sideeffects]
def defer_check_short_pipe() {
    var x = 1
    {
        x++
        defer <| $() {      // nolint:STYLE002 - the short pipe spelling is what this arm covers
            x--
        }
    }
    assert(x == 1)
}

def defer_in_lambda_bare_block() : int2 {
    var count = 0   // nolint:LINT003 - `let` makes the lambda's captured copy const, breaking count++
    var lam <- @ capture(copy(count)) () : int {
        {
            defer() {
                count++
            }
            count += 10
        }
        return count
    }
    let first = invoke(lam)
    let second = invoke(lam)
    unsafe { delete lam; }
    return int2(first, second)
}

[test(description = "defer blocks run at scope exit in reverse order, defer_delete deletes at scope exit, and a defer in a bare block inside a lambda fires on each call")]
def test_defer(t : T?) {
    t |> run("defer execution order") @(t : T?) {
        var x = 1
        defer_check(x)
        t |> equal(x, 4)
    }
    t |> run("defer_delete cleans up array") @(t : T?) {
        var a : array<int>
        let len = defer_check_del(a)
        t |> equal(len, 10)
        t |> equal(length(a), 0)
    }
    t |> run("defer_delete (deprecated spelling) still deletes at scope exit") @(t : T?) {
        var a : array<int>
        t |> equal(defer_delete_check(a), 5)
        t |> equal(length(a), 0)
    }
    t |> run("defer with short pipe syntax") @(t : T?) {
        defer_check_short_pipe()
        t |> success(true)
    }
    t |> run("defer in a bare block inside a lambda fires per call, not on delete") @(t : T?) {
        t |> equal(defer_in_lambda_bare_block(), int2(11, 22))
    }
}

var ONE = 1

[sideeffects]
def one(tt : T?) {
    ONE = 1
    tt |> equal(ONE, 1)
    return ONE
}

[sideeffects]
def test_if_finally(tt : T?) {
    var t : int[10]
    var count = 0
    for (x in range(10)) {
        t[x] = x + 1
        if (x == 5) {
            break
        } finally {
            t[9] = 13
            count ++
        }
    }
    tt |> equal(count, 1)
    tt |> equal(t[9], 13)
}

[sideeffects]
def test_for_finally(tt : T?) {
    var t : int[10]
    var count = 0
    for (x in range(10)) {
        let uno = one(tt)
        t[x] = x + uno
        if (x == 5) {
            break
        }
    } finally {
        t[9] = 13
        count += uno
    }
    // per-iteration finally: iterations 0..5 each run finally (break
    // after x==5's finally)
    tt |> equal(count, 6)
    tt |> equal(t[9], 13)
}

[sideeffects]
def test_while_finally(tt : T?) {
    var t : int[10]
    var count = 0
    var x = 0
    while (x != 10) {
        let uno = one(tt)
        t[x] = x + uno
        if (x == 5) {
            break
        }
        ++x
    } finally {
        t[9] = 12 + uno
        count ++
    }
    // per-iteration finally: iterations 0..5 each run finally (break
    // after x==5's finally)
    tt |> equal(count, 6)
    tt |> equal(t[9], 13)
}

var glob = 0
var glob_count = 0

def func_finally {
    return 12
} finally {
    glob = 13
    glob_count ++
}

def test_func_finally(tt : T?) {
    glob = 0
    glob_count = 0
    let t = func_finally()
    tt |> equal(t, 12)
    tt |> equal(glob, 13)
    tt |> equal(glob_count, 1)
}

[sideeffects]
def test_block_finally(tt : T?) {
    var t = 0
    invoke() ${ t = 1; } finally { t = 2; }
    tt |> equal(t, 2)
}

[test(description = "a finally runs once on an if that breaks, once per iteration of a for or while loop, after a function's return, and after a block")]
def test_finally(t : T?) {
    t |> run("if-break finally") @(t : T?) { test_if_finally(t); }
    t |> run("for-loop finally") @(t : T?) { test_for_finally(t); }
    t |> run("while-loop finally") @(t : T?) { test_while_finally(t); }
    t |> run("function finally") @(t : T?) { test_func_finally(t); }
    t |> run("block finally") @(t : T?) { test_block_finally(t); }
}

[never_inline]
def private call_with_block_argument(value : int;
                                     blk : block<(arg : int) : int2>
                                     ) : int2 => invoke(blk, value)
[never_inline]
def private throw_with_block_argument(blk : block<(arg : int) : void>) {
    invoke(blk, 91)
}
def private block_arguments_after_recovery(value : int) : int2 {
    return (call_with_block_argument(value) $(outer : int) : int2 {
        var recovered = 0
        try {
            throw_with_block_argument($(inner : int) {
                panic("nested {inner}")
            })
        } recover {
            recovered = outer
        }
        return int2(recovered, outer)
    })
}

[test(description = "recovering from a panic raised in a nested block leaves the outer block's argument intact")]
def test_try_recover_restores_block_arguments(t : T?) {
    t |> equal(block_arguments_after_recovery(17), int2(17))
}

[test(description = "try/recover catches a null deref and an explicit panic and runs the recover block")]
def test_try_recover(t : T?) {
    t |> run("deref null recovers") @(t : T?) {
        var a = 1
        let b : int?
        try {
            a = deref(b)
        } recover {
            a = 13
        }
        t |> equal(a, 13)
    }
    t |> run("panic recovers") @(t : T?) {
        var a = 1
        try {
            panic("now panic")
        } recover {
            a = 14
        }
        t |> equal(a, 14)
    }
}

var g_rounds = 5

[export, no_jit]
def break_in_for : int {
    var seen = 0
    for (i in range(0, g_rounds)) {
        try {
            break if (i == 2)
            seen ++
        } recover {
        }
        seen += 10
    }
    return seen
}

[export, no_jit]
def continue_in_while : int {
    var seen = 0
    var i = 0
    while (i < g_rounds) {
        i ++
        try {
            continue if (i % 2 == 0)
            seen ++
        } recover {
        }
        seen += 10
    }
    return seen
}

[export, no_jit]
def break_in_recover : int {
    var seen = 0
    for (i in range(0, g_rounds)) {
        try {
            panic("stop") if (i == 2)
            seen ++
        } recover {
            break
        }
        seen += 10
    }
    return seen
}

[export, no_jit]
def nested_try : int {
    var seen = 0
    for (i in range(0, g_rounds)) {
        try {
            try {
                continue if (i == 1)
                break if (i == 3)
            } recover {
            }
            seen ++
        } recover {
        }
        seen += 10
    }
    return seen
}

[export, no_jit]
def inner_loop_break : int {
    var seen = 0
    for (i in range(0, g_rounds)) {
        try {
            for (j in range(0, 4)) {
                break if (j == 1)
                seen ++
            }
            break if (i == 2)
        } recover {
        }
        seen += 10
    }
    return seen
}

[export]
def inner_only_break : int {
    var seen = 0
    try {
        for (j in range(0, 4)) {
            break if (j == 2)
            seen ++
        }
        seen += 100
    } recover {
    }
    return seen
}

[export]
def try_without_exit : int {
    var seen = 0
    for (_i in range(0, g_rounds)) {
        try {
            seen ++
        } recover {
        }
    }
    return seen
}

[test(description = "break and continue inside try or recover, nested tries and inner loops leave exactly the loop they name")]
def test_try_recover_control_flow(t : T?) {
    t |> equal(break_in_for(), 22, "break leaves the for loop")
    t |> equal(continue_in_while(), 33, "continue skips the rest of the while iteration")
    t |> equal(break_in_recover(), 22, "break in the recover block leaves the loop")
    t |> equal(nested_try(), 22, "nested try chains the exit outwards")
    t |> equal(inner_loop_break(), 23, "break in a loop inside the try stays inner")
    t |> equal(inner_only_break(), 102, "a loop break with no loop around the try needs no exit")
    t |> equal(try_without_exit(), 5, "try with no exit is unaffected")
}

struct WithPoint {
    x, y : int = 0
}

[test(description = "a with (struct) block names the struct's fields directly, so assignments inside write them")]
def test_with_statement(t : T?) {
    t |> run("with block sets fields") @(t : T?) {
        var p = WithPoint()
        with (p) {
            x = 1
            y = 2
        }
        t |> equal(p.x, 1)
        t |> equal(p.y, 2)
    }
}
