require dastest/testing_boost public

def iliketomoveitmoveit ( var a )
    return <- a

[test]
def test_moves ( t : T? )
    t |> run("basic move it") <| @@ ( t : T? )
        var a <- [{for x in range(10); x}]
        a <- a
        if t |> equal(length(a), 10)
            for i,j in a,range(10)
                t |> equal(i, j)
    t |> run("function move it") <| @@ ( t : T? )
        var a <- [{for x in range(10); x}]
        a <- iliketomoveitmoveit(a)
        if t |> equal(length(a), 10)
            for i,j in a,range(10)
                t |> equal(i, j)

def consume ( var a )
    delete a

[test]
def test_failed_moves ( t : T? )
    t |> run("basic move it") <| @@ ( t : T? )
        var a : array<array<int>>
        var b : array<array<int>>
        resize(a, 10)
        a[0] |> push(1)
        var failed = false
        try
            for t in a[0]
                b <- a
        recover
            failed = true
        t |> success(failed)
    t |> run("function move it") <| @@ ( t : T? )
        var a : array<array<int>>
        var b : array<array<int>>
        resize(a, 10)
        a[0] |> push(1)
        var failed = false
        try
            for t in a[0]
                b <- iliketomoveitmoveit(a)
        recover
            failed = true
        t |> success(failed)

    t |> run("consume it") <| @@ ( t : T? )
        var a : array<array<int>>
        resize(a, 10)
        a[0] |> push(1)
        var failed = false
        try
            for t in a[0]
                consume(iliketomoveitmoveit(a))
        recover
            failed = true
        t |> success(failed)