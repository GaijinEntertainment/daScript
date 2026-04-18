options gen2
// STYLE012: array<T> variable initialized via push/emplace instead of literal
//
// Problem: Declaring an empty `array<T>` and then populating it with a
// contiguous run of TWO OR MORE `push`/`emplace` calls is a common
// "I-forgot-how-to-init-arrays" bug — and it's both harder to read and slower
// than using an array literal `var a <- [x, <-y]`. A single push stays silent
// because it is often more readable that way.
//
// Bad pattern:
//   var a : array<int>
//   a |> push(1)
//   a |> push(2)
//
// Good patterns:
//   var a <- [1, 2]                          // inferred element type
//   var a <- array<Shape?>(new Circle(),     // explicit element type,
//                          new Square())     //   useful for polymorphic upcasts
//
// `push_clone` is intentionally NOT flagged — no clean array-literal equivalent.

struct Foo {
    x : int
}

// bad: two pushes after empty array decl (1 warning)
def bad_push_sequence() {
    var a : array<int>
    a |> push(1)
    a |> push(2)
}

// good: single push — readability is fine, stays silent
def good_single_push() {
    var a : array<int>
    a |> push(42)
}

// bad: emplace sequence (1 warning)
def bad_emplace_sequence() {
    var f1 = Foo(x = 1)
    var f2 = Foo(x = 2)
    var a : array<Foo>
    a |> emplace(f1)
    a |> emplace(f2)
}

// bad: mixed push + emplace (1 warning)
def bad_mixed() {
    var f = Foo(x = 1)
    var a : array<Foo>
    a |> push(Foo(x = 0))
    a |> emplace(f)
}

// good: array literal
def good_literal() {
    var a <- [1, 2, 3]
    a |> push(4)  // the literal used initialization; this push is acceptable
}

// good: push_clone is intentionally ignored
def good_push_clone() {
    var s = "hi"
    var a : array<string>
    a |> push_clone(s)
}

// good: push inside a loop is fine (not a literal candidate)
def good_loop_push() {
    var a : array<int>
    for (i in range(5)) {
        a |> push(i)
    }
}

// good: push inside conditional is fine
def good_conditional_push(cond : bool) {
    var a : array<int>
    if (cond) {
        a |> push(1)
    }
}

// good: inscope variable (matches STYLE011 exclusion)
def good_inscope() {
    var inscope a : array<int>
    a |> push(1)
}

// good: var decl followed by other statement, then push (push not contiguous)
def good_interleaved() : int {
    var a : array<int>
    let x = 42
    a |> push(x)
    return x
}
