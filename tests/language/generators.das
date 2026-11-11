options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false
require dastest/testing_boost public
require daslib/coroutines

[sideeffects]
def checkIt(t : T?; var gen : iterator<int>) {
    var count = 0
    for (x, y in gen, count()) {
        if (y >= 10) {
            t |> failure("output too long")
        }
        t |> equal(x, y)
        count ++
    }
    t |> equal(count, 10)
}

[sideeffects]
def checkSeq(t : T?; var gen : iterator<int>; expected : array<int>) {
    var i = 0
    for (x in gen) {
        if (i >= length(expected)) {
            t |> failure("output too long")
            return
        }
        t |> equal(x, expected[i])
        i ++
    }
    t |> equal(i, length(expected))
}

def test_yield(t : T?) {
    var gen <- generator<int> {
        yield 0
        yield 1
        yield 2
        yield 3
        yield 4
        yield 5
        yield 6
        yield 7
        yield 8
        yield 9
        return false
    }
    checkIt(t, gen)
}

def test_local(t : T?) {
    var gen <- generator<int> {
        var t = 0
        yield t++
        yield t++
        yield t++
        yield t++
        yield t++
        yield t++
        yield t++
        yield t++
        yield t++
        yield t++
        return false
    }
    checkIt(t, gen)
}

def test_local_ref(t : T?) {
    var gen <- generator<int> {
        var t = 0
        var T : int& = t
        yield t++
        yield T++
        yield t++
        yield T++
        yield t++
        yield T++
        yield t++
        yield T++
        yield t++
        yield T++
        return false
    }
    checkIt(t, gen)
}

def test_if(t : T?) {
    var gen <- generator<int> {
        var t = 0
        yield t++
        if (t == 1) {
            yield t++
        }
        if (t == 2) {
            yield t++
        } else {
            yield 13
        }
        if (t == 3) {
            if (t < 10) {
                yield t++
            } else {
                yield 13
            }
        } else {
            yield 12
        }
        if (t == 13) {
            yield 0
        } elif (t == 4) {
            yield t++
        } else {
            yield -1
        }
        yield t++
        yield t++
        yield t++
        yield t++
        yield t++
        return false
    }
    checkIt(t, gen)
}

def test_while(t : T?) {
    var gen <- generator<int> {
        var t = 0
        while (t < 10) {
            yield t ++
        }
        return false
    }
    checkIt(t, gen)
}

def test_while_break(t : T?) {
    var gen <- generator<int> {
        var t = 0
        while (t < 100) {
            if (t == 10) {
                break
            }
            yield t ++
        }
        return false
    }
    checkIt(t, gen)
}

def test_while_continue(t : T?) {
    var gen <- generator<int> {
        var t = 0
        while (t < 100) {
            if (t >= 10) {
                t ++
                continue
            }
            yield t ++
        }
        return false
    }
    checkIt(t, gen)
}

def test_while_nested(t : T?) {
    var gen <- generator<int> {
        var q = 0
        while (q < 2) {
            var t = 0
            while (t < 5) {
                yield q * 5 + t ++
            }
            q ++
        }
        return false
    }
    checkIt(t, gen)
}

def test_while_finally(t : T?) {
    // per-iteration semantics: finally runs at the end of every iteration
    // of the body, NOT once after the loop. `yield 9` fires interleaved
    // after each body yield. cond becomes false -> finally does NOT run
    // for that phantom iteration.
    var gen <- generator<int> {
        var t = 0
        while (t < 9) {
            yield t ++
        } finally {
            yield 9
        }
        return false
    }
    checkSeq(t, gen, [0, 9, 1, 9, 2, 9, 3, 9, 4, 9, 5, 9, 6, 9, 7, 9, 8, 9])
}

def test_while_finally_break(t : T?) {
    // break routes through finally once (current iteration's finally),
    // then exits
    var gen <- generator<int> {
        var t = 0
        while (t < 100) {
            yield t ++
            if (t == 3) {
                break
            }
        } finally {
            yield 99
        }
        return false
    }
    // iter 0: yield 0 -> t=1, no break, finally yields 99
    // iter 1: yield 1 -> t=2, no break, finally yields 99
    // iter 2: yield 2 -> t=3, break, finally yields 99, exit
    checkSeq(t, gen, [0, 99, 1, 99, 2, 99])
}

def test_while_finally_continue(t : T?) {
    // continue routes through finally (same as normal end of iter), then
    // loops back
    var gen <- generator<int> {
        var t = 0
        while (t < 5) {
            t ++
            if (t % 2 == 0) {
                continue
            }
            yield t
        } finally {
            yield 99
        }
        return false
    }
    // t=1: odd, yield 1, finally 99
    // t=2: even, continue, finally 99
    // t=3: odd, yield 3, finally 99
    // t=4: even, continue, finally 99
    // t=5: odd, yield 5, finally 99
    // t<5 false, exit without finally
    checkSeq(t, gen, [1, 99, 99, 3, 99, 99, 5, 99])
}

def test_while_finally_empty(t : T?) {
    // empty loop: body never enters, finally never runs
    var gen <- generator<int> {
        while (false) {
            yield 1
        } finally {
            yield 99
        }
        return false
    }
    checkSeq(t, gen, [])
}

def test_for(t : T?) {
    var gen <- generator<int> {
        for (t in range(10)) {
            yield t
        }
        return false
    }
    checkIt(t, gen)
}

def test_for_break(t : T?) {
    var gen <- generator<int> {
        for (t in range(100)) {
            if (t == 10) {
                break
            }
            yield t
        }
        return false
    }
    checkIt(t, gen)
}

def test_for_continue(t : T?) {
    var gen <- generator<int> {
        for (t in range(100)) {
            if (t >= 10) {
                continue
            }
            yield t
        }
        return false
    }
    checkIt(t, gen)
}

def test_for_nested(t : T?) {
    var gen <- generator<int> {
        for (q in range(2)) {
            for (t in range(5)) {
                yield q * 5 + t
            }
        }
        return false
    }
    checkIt(t, gen)
}

def test_for_finally(t : T?) {
    // per-iteration semantics: finally runs at mid_loop (after body,
    // before iterator_next) for N iterations we get N body yields and N
    // finally yields, interleaved. On iterator exhaustion: initial !loop
    // check jumps past finally (skipped for empty or post-last).
    var gen <- generator<int> {
        for (t in range(9)) {
            yield t
        } finally {
            yield 9
        }
        return false
    }
    checkSeq(t, gen, [0, 9, 1, 9, 2, 9, 3, 9, 4, 9, 5, 9, 6, 9, 7, 9, 8, 9])
}

def test_for_finally_break(t : T?) {
    var gen <- generator<int> {
        for (t in range(100)) {
            yield t
            if (t == 2) {
                break
            }
        } finally {
            yield 99
        }
        return false
    }
    // iter 0: yield 0, t!=2, finally 99
    // iter 1: yield 1, t!=2, finally 99
    // iter 2: yield 2, t==2 break, finally 99, exit
    checkSeq(t, gen, [0, 99, 1, 99, 2, 99])
}

def test_for_finally_continue(t : T?) {
    var gen <- generator<int> {
        for (t in range(5)) {
            if (t % 2 == 0) {
                continue
            }
            yield t
        } finally {
            yield 99
        }
        return false
    }
    // t=0 even continue, finally 99
    // t=1 odd, yield 1, finally 99
    // t=2 even continue, finally 99
    // t=3 odd, yield 3, finally 99
    // t=4 even continue, finally 99
    checkSeq(t, gen, [99, 1, 99, 99, 3, 99, 99])
}

def test_for_range_flavors(t : T?) {
    var gen <- generator<int> {
        for (u in urange(3u)) {
            yield int(u)
        }
        for (l in range64(3l, 7l)) {
            yield int(l)
        }
        for (ul in urange64(7ul, 10ul)) {
            yield int(ul)
        }
        return false
    }
    checkIt(t, gen)
}

def test_for_two_ranges(t : T?) {
    var gen <- generator<int> {
        for (a, _b in range(10), range(100, 200)) {
            yield a
        }
        return false
    }
    checkIt(t, gen)
}

def test_for_range_and_iterator(t : T?) {
    var gen <- generator<int> {
        for (c, _i in "0123456789", range(10)) {
            yield c - 48
        }
        return false
    }
    checkIt(t, gen)
}

def test_for_finally_empty(t : T?) {
    // empty iterator: body never enters, finally never runs
    var gen <- generator<int> {
        for (t in range(0)) {
            yield t
        } finally {
            yield 99
        }
        return false
    }
    checkSeq(t, gen, [])
}

[test(description = "a raw generator yields the right sequence through locals, refs, if/elif, while and for loops with break, continue, nesting and finally, and every range flavor")]
def test_generators(t : T?) {
    t |> run("yield explicit values") @(t : T?) { test_yield(t); }
    t |> run("local variable in generator") @(t : T?) { test_local(t); }
    t |> run("local ref in generator") @(t : T?) { test_local_ref(t); }
    t |> run("if/elif/else in generator") @(t : T?) { test_if(t); }
    t |> run("while loop in generator") @(t : T?) { test_while(t); }
    t |> run("while break in generator") @(t : T?) { test_while_break(t); }
    t |> run("while continue in generator") @(t : T?) { test_while_continue(t); }
    t |> run("nested while in generator") @(t : T?) { test_while_nested(t); }
    t |> run("while finally in generator") @(t : T?) { test_while_finally(t); }
    t |> run("while finally + break in generator") @(t : T?) { test_while_finally_break(t); }
    t |> run("while finally + continue in generator") @(t : T?) { test_while_finally_continue(t); }
    t |> run("while finally empty in generator") @(t : T?) { test_while_finally_empty(t); }
    t |> run("for loop in generator") @(t : T?) { test_for(t); }
    t |> run("for break in generator") @(t : T?) { test_for_break(t); }
    t |> run("for continue in generator") @(t : T?) { test_for_continue(t); }
    t |> run("nested for in generator") @(t : T?) { test_for_nested(t); }
    t |> run("for finally in generator") @(t : T?) { test_for_finally(t); }
    t |> run("for finally + break in generator") @(t : T?) { test_for_finally_break(t); }
    t |> run("for finally + continue in generator") @(t : T?) { test_for_finally_continue(t); }
    t |> run("for finally empty in generator") @(t : T?) { test_for_finally_empty(t); }
    t |> run("for over every range flavor") @(t : T?) { test_for_range_flavors(t); }
    t |> run("for over two ranges") @(t : T?) { test_for_two_ranges(t); }
    t |> run("for over range and iterator") @(t : T?) { test_for_range_and_iterator(t); }
}

// the inliner binds a block-literal argument to a local holder when
// the callee reads it more than once or under a loop (ast_inline.cpp,
// makeArgTemp); a generator lifts its locals into its capture
// structure, where a block can never live, so the site declines
// inside a generator
def private transform_sum(n : int; blk : block<(v : int) : int>) : int {
    // reads the block under a loop, which is what forces the holder path
    var total = 0
    for (v in range(n)) {
        total += invoke(blk, v)
    }
    return total
}

def private pick(blk : block<(v : int) : int>) : int {
    // reads the block twice, the other holder trigger
    return invoke(blk, 1) + invoke(blk, 2)
}

[test(description = "a generator body calls a function taking a block literal, read under a loop or twice, and yields its results")]
def test_generator_block_argument(t : T?) {
    t |> run("block literal under a callee loop") @(t : T?) {
        var g <- generator<int> {
            yield transform_sum(4, $(v : int) { return v * 2; })
            yield 7
            return false
        }
        let got <- [for (x in g); x]
        t |> equal(got, [12, 7])
    }
    t |> run("block literal read twice by the callee") @(t : T?) {
        var g <- generator<int> {
            yield pick($(v : int) { return v * 10; })
            return false
        }
        let got <- [for (x in g); x]
        t |> equal(got, [30])
    }
    t |> run("block argument inside a generator loop") @(t : T?) {
        var g <- generator<int> {
            for (i in range(3)) {
                yield transform_sum(i, $(v : int) { return v + 1; })
            }
            return false
        }
        let got <- [for (x in g); x]
        t |> equal(got, [0, 1, 3])
    }
}

var g_iter_count : int = 0
var g_arr_count : int = 0
var g_bump_count : int = 0

def noisy_iter() : iterator<int> {
    g_iter_count ++
    return <- generator<int> {
        yield 1
        yield 2
        yield 3
        return false
    }
}

def noisy_arr() : array<int> {
    g_arr_count ++
    return <- [10, 20, 30]
}

def bump(n : int) : int {
    g_bump_count ++
    return n
}

[coroutine]
def sub_task(_n : int) {
    co_continue()
}

[coroutine]
def awaiting_task() {
    co_await <| sub_task(bump(7))
}

[coroutine]
def delegating_gen() : int {
    yeild_from <| noisy_iter()
    return false
}

[coroutine]
def yieldless_for_sum() : int {
    var total = 0
    for (x in noisy_iter()) {
        total += x
    }
    yield total
    return false
}

[test(description = "a for source is evaluated exactly once - in a yielding generator loop, a zip, co_await, yeild_from, a yieldless coroutine and a comprehension")]
def test_generator_for_source_eval_once(t : T?) {
    t |> run("iterator call in a yielding for-loop runs once") @(t : T?) {
        g_iter_count = 0
        var total = 0
        var gen <- generator<int> {
            for (x in noisy_iter()) {
                yield x
            }
            return false
        }
        for (v in gen) {
            total += v
        }
        t |> equal(g_iter_count, 1)
        t |> equal(total, 6)
    }
    t |> run("zip sources each run once") @(t : T?) {
        g_iter_count = 0
        g_arr_count = 0
        var total = 0
        var gen <- generator<int> {
            for (a, b in noisy_iter(), noisy_arr()) {
                yield a + b
            }
            return false
        }
        for (v in gen) {
            total += v
        }
        t |> equal(g_iter_count, 1)
        t |> equal(g_arr_count, 1)
        t |> equal(total, 66)
    }
    t |> run("co_await argument runs once") @(t : T?) {
        g_bump_count = 0
        var c <- awaiting_task()
        cr_run(c)
        t |> equal(g_bump_count, 1)
    }
    t |> run("yeild_from source runs once") @(t : T?) {
        g_iter_count = 0
        var total = 0
        for (v in delegating_gen()) {
            total += v
        }
        t |> equal(g_iter_count, 1)
        t |> equal(total, 6)
    }
    t |> run("array-returning call runs once") @(t : T?) {
        g_arr_count = 0
        var total = 0
        var gen <- generator<int> {
            for (x in noisy_arr()) {
                yield x
            }
            return false
        }
        for (v in gen) {
            total += v
        }
        t |> equal(g_arr_count, 1)
        t |> equal(total, 60)
    }
    t |> run("for-loop in a yieldless coroutine runs the source once") @(t : T?) {
        g_iter_count = 0
        var res = 0
        for (v in yieldless_for_sum()) {
            res = v
        }
        t |> equal(g_iter_count, 1)
        t |> equal(res, 6)
    }
    t |> run("comprehension source runs once") @(t : T?) {
        g_iter_count = 0
        let vals <- [for (x in noisy_iter()); x]
        t |> equal(g_iter_count, 1)
        t |> equal(length(vals), 3)
    }
}

// the end-of-loop path closes ALL zipped sources, so first() must run
// on every source even after an earlier one comes up empty: closing a
// never-opened container iterator unlocks a container whose lock
// magic was already cleared
[test(description = "a generator zipping array or table iterators where one source is empty yields nothing and closes every source cleanly")]
def test_generator_zip_empty_source(t : T?) {
    t |> run("two array iterators, first empty") @(t : T?) {
        let a : array<int>
        var b : array<int>
        b |> push(1)
        var ait <- unsafe(each(a))
        var bit <- unsafe(each(b))
        var g <- generator<int -const> capture(<- ait, <- bit) {
            for (x, y in ait, bit) {
                yield x + y
            }
            return false
        }
        var n = 0
        for (_v in g) {
            n++
        }
        t |> equal(n, 0)
    }
    t |> run("two array iterators, second empty") @(t : T?) {
        var a : array<int>
        let b : array<int>
        a |> push(1)
        var ait <- unsafe(each(a))
        var bit <- unsafe(each(b))
        var g <- generator<int -const> capture(<- ait, <- bit) {
            for (x, y in ait, bit) {
                yield x + y
            }
            return false
        }
        var n = 0
        for (_v in g) {
            n++
        }
        t |> equal(n, 0)
    }
    t |> run("two table iterators, table empty") @(t : T?) {
        let tab : table<int; int>
        var kit <- unsafe(keys(tab))
        var vit <- unsafe(values(tab))
        var g <- generator<tuple<k : int; v : int> -const> capture(<- kit, <- vit) {
            for (k, v in kit, vit) {
                yield (k = k, v = v)
            }
            return false
        }
        var n = 0
        for (_kv in g) {
            n++
        }
        t |> equal(n, 0)
    }
}

[coroutine]
def count_up(n : int) : int {
    for (i in range(1, n + 1)) {
        yield i
    }
    return false
}

[coroutine]
def count_down(n : int) : int {
    var i = n
    while (i > 0) {
        yield i
        i --
    }
    return false
}

[test(description = "a typed [coroutine] yields its values to a for loop counting up, counting down, or not at all")]
def test_typed_coroutine(t : T?) {
    t |> run("count_up via for loop") @(t : T?) {
        let vals <- [for (v in count_up(3)); v]
        t |> equal(length(vals), 3)
        t |> equal(vals[0], 1)
        t |> equal(vals[1], 2)
        t |> equal(vals[2], 3)
    }
    t |> run("count_down via for loop") @(t : T?) {
        let vals <- [for (v in count_down(3)); v]
        t |> equal(length(vals), 3)
        t |> equal(vals[0], 3)
        t |> equal(vals[1], 2)
        t |> equal(vals[2], 1)
    }
    t |> run("empty coroutine") @(t : T?) {
        let vals <- [for (v in count_up(0)); v]
        t |> equal(length(vals), 0)
    }
}

[coroutine]
def yielder() : int {
    yield 10
    yield 20
    yield 30
    return false
}

[test(description = "a coroutine of straight-line yields produces exactly those values in order")]
def test_yielder(t : T?) {
    t |> run("yielder produces exact values") @(t : T?) {
        let vals <- [for (v in yielder()); v]
        t |> equal(length(vals), 3)
        t |> equal(vals[0], 10)
        t |> equal(vals[1], 20)
        t |> equal(vals[2], 30)
    }
}

var g_step_count : int = 0

[coroutine]
def stepper(n : int) {
    for (i in range(n)) {
        g_step_count ++
        co_continue()
    }
}

[test(description = "cr_run drives a void coroutine through every co_continue to completion")]
def test_cr_run(t : T?) {
    t |> run("cr_run drives void coroutine to completion") @(t : T?) {
        g_step_count = 0
        var c <- stepper(5)
        cr_run(c)
        t |> equal(g_step_count, 5)
    }
}

var g_a_count : int = 0
var g_b_count : int = 0

[coroutine]
def worker_a() {
    g_a_count ++
    co_continue()
    g_a_count ++
}

[coroutine]
def worker_b() {
    g_b_count ++
    co_continue()
    g_b_count ++
    co_continue()
    g_b_count ++
}

[test(description = "cr_run_all runs several coroutines of different lengths each to completion")]
def test_cr_run_all(t : T?) {
    t |> run("cr_run_all runs multiple coroutines") @(t : T?) {
        g_a_count = 0
        g_b_count = 0
        var tasks : Coroutines <- [worker_a(), worker_b()]
        cr_run_all(tasks)
        t |> equal(g_a_count, 2)
        t |> equal(g_b_count, 3)
    }
}

[coroutine]
def sub_gen() : int {
    yield 100
    yield 200
    return false
}

[coroutine]
def parent_gen() : int {
    yield 1
    yeild_from <| sub_gen()
    yield 2
    return false
}

[test(description = "yeild_from splices a sub-generator's values into the parent's sequence in place")]
def test_yeild_from(t : T?) {
    t |> run("yeild_from delegates to sub-generator") @(t : T?) {
        let vals <- [for (v in parent_gen()); v]
        t |> equal(length(vals), 4)
        t |> equal(vals[0], 1)
        t |> equal(vals[1], 100)
        t |> equal(vals[2], 200)
        t |> equal(vals[3], 2)
    }
}

var g_main_log : array<string>

[coroutine]
def co_sub_task() {
    g_main_log |> push("sub1")
    co_continue()
    g_main_log |> push("sub2")
}

[coroutine]
def main_task() {
    g_main_log |> push("main_start")
    co_await <| co_sub_task()
    g_main_log |> push("main_end")
}

[test(description = "co_await suspends the caller until the awaited coroutine finishes, then resumes it")]
def test_co_await(t : T?) {
    t |> run("co_await waits for sub-coroutine") @(t : T?) {
        g_main_log |> clear()
        var c <- main_task()
        cr_run(c)
        t |> equal(length(g_main_log), 4)
        t |> equal(g_main_log[0], "main_start")
        t |> equal(g_main_log[1], "sub1")
        t |> equal(g_main_log[2], "sub2")
        t |> equal(g_main_log[3], "main_end")
    }
}

def unsafe_block_locals(n : int) {
    return <- generator<int> {
        for (index in range(n)) {
            unsafe {
                var local = index * 10
                yield local
                local ++
                yield local
            }
        }
        return false
    }
}

[test(description = "a local declared in an unsafe block of a generator keeps its value across a yield")]
def test_generator_unsafe_block_local(t : T?) {
    let got <- [for (x in unsafe_block_locals(3)); x]
    t |> equal(length(got), 6)
    for (i, x in range(6), got) {
        t |> equal(x, (i / 2) * 10 + i % 2)
    }
}
