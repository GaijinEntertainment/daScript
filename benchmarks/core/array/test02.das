options gen2
options persistent_heap

require dastest/testing_boost

struct Foo {
    a : int
    b : float
    c : string
}

struct Bar {
    a, b, c : Foo
}

let N = 10000

[benchmark]
def push_vs_push_clone_int(b : B?) {
    b |> run("push_int/{N}", N) {
        var arr : array<int>
        reserve(arr, N)
        for (i in range(N)) {
            arr |> push(i)
        }
    }
    b |> run("push_clone_int/{N}", N) {
        var arr : array<int>
        reserve(arr, N)
        for (i in range(N)) {
            arr |> push_clone(i)
        }
    }
}

[benchmark]
def push_vs_push_clone_struct(b : B?) {
    b |> run("push_struct/{N}", N) {
        var arr : array<Foo>
        reserve(arr, N)
        for (i in range(N)) {
            arr |> push(Foo(a = i, b = float(i), c = "hello"))
        }
    }
    b |> run("push_clone_struct/{N}", N) {
        var arr : array<Foo>
        reserve(arr, N)
        for (i in range(N)) {
            arr |> push_clone(Foo(a = i, b = float(i), c = "hello"))
        }
    }
}

[benchmark]
def push_vs_push_clone_large_struct(b : B?) {
    b |> run("push_struct/{N}", N) {
        var arr : array<Bar>
        reserve(arr, N)
        for (i in range(N)) {
            arr |> push(Bar(a = Foo(a = i, b = float(i), c = "hello")))
        }
    }
    b |> run("push_clone_struct/{N}", N) {
        var arr : array<Bar>
        reserve(arr, N)
        for (i in range(N)) {
            arr |> push_clone(Bar(a = Foo(a = i, b = float(i), c = "hello")))
        }
    }
}
