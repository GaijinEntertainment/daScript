// Tutorial 14: Lambdas and Closures
//
// This tutorial covers:
//   - Lambda declaration with @() syntax
//   - How lambdas differ from blocks and functions
//   - Call syntax: fn(x) — invoke as an alternative
//   - Capture: by copy (default), by move, by clone
//   - Storing lambdas in variables (move semantics)
//   - Simplified => syntax
//   - Lambda vs block vs function
//
// Run: daslang.exe tutorials/language/14_lambdas.das

options gen2

[export]
def main {

    // === Lambda basics ===
    // Lambdas are declared with @ instead of $ (blocks).
    // Unlike blocks, lambdas can be stored, moved, and outlive the scope.
    // Unlike anonymous functions (@@), lambdas capture variables.
    // Lambdas capture variables by COPY by default.

    var multiplier = 10
    var mul_lambda <- @(x : int) : int {
        return x * multiplier
    }
    print("mul_lambda(5) = {mul_lambda(5)}\n")   // 50

    // Changing the original variable does NOT affect the lambda (captured by copy).
    multiplier = 999
    print("mul_lambda(5) still = {mul_lambda(5)}\n")  // still 50

    // === Simplified => syntax ===
    // For single-expression lambdas.
    var doubler <- @(x : int) : int => x * 2
    print("doubler(7) = {doubler(7)}\n")

    // === Call syntax ===
    // Lambda variables support call syntax — call them like regular functions.
    // invoke() is the explicit alternative — same behavior.
    print("call syntax: {doubler(3)}\n")
    print("invoke:      {invoke(doubler, 3)}\n")

    // === Storing lambdas ===
    // Lambdas MUST be moved with <- (they cannot be copied).
    // var copy = doubler      // ERROR: lambdas can't be copied
    // var moved <- doubler    // OK but doubler becomes null

    // === Passing lambdas to functions ===
    // Functions that take lambda<> accept only @ lambdas (not @@ functions).
    print("apply(doubler, 21) = {apply_lambda(doubler, 21)}\n")

    // === Capture by copy (default) ===
    // Each lambda gets its own copy of captured variables.
    var counter = 0
    var inc <- @() : int {
        return ++counter    // this is the lambda's OWN copy of counter
    }
    print("inc() = {inc()}\n")    // 1
    print("inc() = {inc()}\n")    // 2
    print("inc() = {inc()}\n")    // 3
    print("original counter = {counter}\n")  // still 0

    // === Capture by move ===
    // Use capture(move(var)) to transfer ownership into the lambda.
    // The original variable becomes empty/zeroed after the move.
    var data <- [10, 20, 30]
    unsafe {
        var summer <- @capture(move(data)) () : int {
            var s = 0
            for (v in data) {
                s += v
            }
            return s
        }
        print("sum of moved data = {summer()}\n")     // 60
    }
    print("data length after move = {length(data)}\n") // 0

    // === Capture by clone ===
    // Use capture(clone(var)) to deep-copy a container into the lambda.
    // The original remains intact.
    var items <- [1, 2, 3]
    var count_items <- @capture(clone(items)) () : int {
        return length(items)
    }
    print("cloned items count = {count_items()}\n")  // 3
    print("original items still = {length(items)}\n") // 3

    // === Lambda vs Block vs Function ===
    //
    // FUNCTION (@@):
    //   - No capture at all. Same as a named function.
    //   - Type: function<>
    //   - See tutorial 12 for details.
    //
    // BLOCK ($):
    //   - Stack-allocated, very fast
    //   - Captures by reference automatically
    //   - Cannot be stored or returned — only passed as argument
    //   - Type: block<>
    //
    // LAMBDA (@):
    //   - Heap-allocated, captures by copy (default), move, clone, or ref
    //   - Can be stored in variables (moved), passed around
    //   - Type: lambda<>
    //
    // Each type has its own parameter type:
    //   function<(x:int):int>  — accepts only @@
    //   lambda<(x:int):int>    — accepts only @
    //   block<(x:int):int>     — accepts all three (most flexible)

    // === Practical example: stateful counter factory ===
    var make_counter <- @() : lambda<():int> {
        var n = 0
        return <- @() : int => ++n
    }
    var c1 <- invoke(make_counter)
    print("c1: {c1()}, {c1()}, {c1()}\n")   // 1, 2, 3
}

def apply_lambda(fn : lambda<(x:int):int>; value : int) : int {
    return fn(value)
}

// output:
// mul_lambda(5) = 50
// mul_lambda(5) still = 50
// doubler(7) = 14
// call syntax: 6
// invoke:      6
// apply(doubler, 21) = 42
// inc() = 1
// inc() = 2
// inc() = 3
// original counter = 0
// sum of moved data = 60
// data length after move = 0
// cloned items count = 3
// original items still = 3
// c1: 1, 2, 3
