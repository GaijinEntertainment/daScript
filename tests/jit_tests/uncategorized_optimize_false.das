options gen2
options optimize = false

require dastest/testing_boost

//! every shape here leaves a statement after a terminator inside one block list - the
//! optimizer would delete the tail, so the file compiles unoptimized to keep it

var g_tail_ran = 0

def bare_block_return() : int {
    {
        return 1
    }
    g_tail_ran++
    return 2
}

def constant_if_return() : int {
    if (true) {    // nolint:STYLE010 - the constant-condition if is the shape under test
        return 3
    }
    g_tail_ran++
    return 4
}

def break_in_nested_block() : int {
    var seen = 0
    for (_i in range(10)) {
        seen++
        {
            break
        }
        g_tail_ran++
    }
    return seen
}

def continue_in_nested_block() : int {
    var seen = 0
    for (_i in range(3)) {
        {
            seen++
            continue
        }
        g_tail_ran++
    }
    return seen
}

def void_bare_block_return() {
    {
        return
    }
    g_tail_ran++
}

[test(description = "statements after a nested block that returns, breaks or continues never run")]
def test_statements_after_a_terminated_nested_block(t : T?) {
    g_tail_ran = 0
    t |> equal(bare_block_return(), 1)
    t |> equal(constant_if_return(), 3)
    t |> equal(break_in_nested_block(), 1)
    t |> equal(continue_in_nested_block(), 3)
    void_bare_block_return()
    t |> equal(g_tail_ran, 0, "a statement after a terminator never runs")
}

[test(description = "a test that skips and returns early does not run the code after the return")]
def test_skip_shape_returns_early(t : T?) {
    if (true) {    // nolint:STYLE010 - the gated-skip shape a test file writes
        t |> skip("the early return is the shape under test")
        return
    }
    t |> failure("the tail after the early return ran")
}

// A local typedef nobody uses is never inferred, so the expressions in its dimension
// reach the JIT with no type.
[test(description = "an unused typedef over an untyped dimension compiles")]
def test_unused_typedef_with_an_untyped_dim(t : T?) {
    typedef Alias = Elem[Elem[Index]]
    t |> success(true)
}

[test(description = "an unused typedef over an untyped for source compiles")]
def test_unused_typedef_with_an_untyped_for_source(t : T?) {
    typedef Alias = int[[for(x in Elem); x]]
    t |> success(true)
}
