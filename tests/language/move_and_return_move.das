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

