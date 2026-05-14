options gen2
require dastest/testing_boost public

struct Foo {
    a : int
}

def float(h : Foo) : int {
    return h.a + 1
}

def int(h : Foo) : float {
    return float(h.a) * 2.0
}

def double(h : Foo) : int {
    return h.a + h.a
}

def do(h : Foo) : int {
    return h.a * 10
}

[test]
def test_cpp_keyword_function_names(t : T?) {
    let h = Foo(a = 21)
    t |> run("def float") @(t : T?) {
        t |> equal(float(Foo(a = 41)), 42)
    }
    t |> run("def int") @(t : T?) {
        t |> equal(int(Foo(a = 3)), 6.0)
    }
    t |> run("def double") @(t : T?) {
        t |> equal(double(h), 42)
    }
    t |> run("def do") @(t : T?) {
        t |> equal(do(Foo(a = 4)), 40)
    }
}
