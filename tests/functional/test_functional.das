require daslib/functional
require dastest/testing_boost public


[test]
def test_flatten ( t:T? )
    t |> run("flatten 1") <| @ ( t : T? )
        var qc = 0
        var q <- [[for x in range(5); x]]
        for i,j in flatten(q),range(100500)
            t |> equal(i,j)
            qc ++
        t |> equal(qc, 5)
    t |> run("flatten 2") <| @ ( t : T? )
        var qc = 0
        var qq <- [[for x in range(3); [[for y in range(4); x*4+y ]]  ]]
        for i,j in flatten(qq),range(100500)
            t |> equal(i,j)
            qc ++
        t |> equal(qc, 12)
    t |> run("flatten 3") <| @ ( t : T? )
        var qc = 0
        var qqq <- [[for x in range(3); [[for y in range(2); [[for z in range(2); x*2*2 + y*2 + z ]] ]]  ]]
        for i,j in flatten(qqq),range(100500)
            t |> equal(i,j)
            qc ++
        t |> equal(qc, 12)
