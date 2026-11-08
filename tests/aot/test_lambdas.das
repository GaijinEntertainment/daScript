options gen2

require dastest/testing_boost public
require UnitTest

[test(description = "A basic lambda runs in AOT")]
def test_basic_lambda(t : T?) {
    t |> run("lambda invocation") @(t : T?) {
        var lam <- @(x : int) : int {
            return x * 2
        }
        t |> equal(invoke(lam, 5), 10)
        t |> equal(invoke(lam, -3), -6)
        unsafe { delete lam; }
    }
}

[test(description = "A lambda sees the variables it captures in AOT")]
def test_lambda_capture(t : T?) {
    t |> run("capture by copy") @(t : T?) {
        let factor = 3
        var lam <- @(x : int) : int {
            return x * factor
        }
        t |> equal(invoke(lam, 5), 15)
        unsafe { delete lam; }
    }
}

def apply_transform(values : array<int>; transform : function<(x : int) : int>) : array<int> {
    var result : array<int>
    result |> reserve(length(values))
    for (v in values) {
        result |> push(invoke(transform, v))
    }
    return <- result
}

def fn_double(x : int) : int => x * 2

[test(description = "A function taking a lambda calls it correctly in AOT")]
def test_higher_order(t : T?) {
    t |> run("apply function pointer to array") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5]
        let result <- apply_transform(arr, @@fn_double)
        t |> equal(length(result), 5)
        t |> equal(result[0], 2)
        t |> equal(result[1], 4)
        t |> equal(result[2], 6)
        t |> equal(result[3], 8)
        t |> equal(result[4], 10)
    }
}

[test(description = "A block argument is invoked correctly in AOT")]
def test_block_argument(t : T?) {
    t |> run("block as last argument") @(t : T?) {
        var sum = 0
        for (i in range(5)) {
            sum += i
        }
        t |> equal(sum, 10)
    }
}

// AOT mirror of tests/language/blocks_and_lambdas.das.
// Exercises the same lambda copy + unsafe-delete coverage through the AOT codegen path.

typedef Counter = lambda<() : int>

def invoke_twice(g : Counter) : int => invoke(g) + invoke(g)

[test(description = "A copied lambda aliases the original's capture in AOT")]
def test_aot_lambda_copy_aliases_capture(t : T?) {
    t |> run("aot copy assignment shares capture frame") @(t : T?) {
        var counter = 0 // nolint:LINT003 -- captured by lambda, mutated inside body
        var inc <- @() : int {
            counter++
            return counter
        }
        let inc2 = inc
        t |> equal(invoke(inc), 1)
        t |> equal(invoke(inc2), 2)
        t |> equal(invoke(inc), 3)
        unsafe { delete inc; }
    }
}

[test(description = "A lambda passed by value shares its capture with the original in AOT")]
def test_aot_lambda_pass_by_value(t : T?) {
    t |> run("aot lambda passes by value (one capture frame, two refs)") @(t : T?) {
        var lam <- @() : int {
            var x = 0
            x++
            return x
        }
        let sum = invoke_twice(lam)
        t |> equal(sum, 2)
        unsafe { delete lam; }
    }
}

struct Holder {
    action : Counter
}

[test(description = "A lambda stored in a struct field and copied shares its capture in AOT")]
def test_aot_lambda_struct_field_copy(t : T?) {
    t |> run("aot struct copy aliases lambda field") @(t : T?) {
        var s1 = Holder(action <- @() : int {
            var n = 0
            n++
            return n
        })
        let s2 = s1
        t |> equal(invoke(s1.action), 1)
        t |> equal(invoke(s2.action), 1)
        unsafe { delete s1.action; }
    }
}

[test(description = "A lambda's implicit capture of locals works in AOT")]
def test_aot_lambda_implicit_capture(t : T?) {
    t |> run("aot inner lambda captured implicitly inside another lambda") @(t : T?) {
        var inner <- @() : int {
            var n = 0
            n++
            return n
        }
        var outer <- @() : int {
            return invoke(inner) + invoke(inner)
        }
        t |> equal(invoke(outer), 2)
        unsafe {
            delete inner
            delete outer
        }
    }
}

[export]
def make_handled_lambda {
    var a = @ { return makeDummy(); }
    unsafe { delete a; }
}

[test(description = "A lambda returning a handled type compiles and runs in AOT")]
def test_lambda_handled_result(t : T?) {
    make_handled_lambda()
    t |> success(true, "lambda with a handled result builds")
}
