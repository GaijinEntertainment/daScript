options persistent_heap = true
options gc

require daslib/decs_boost
require dastest/testing_boost public

[test]
def test_arrays( t: T?)
    restart()
    for i in range(3)
        create_entity <| @ ( eid, cmp )
            cmp |> set("values", [{for x in range(i+1); x}])
            cmp |> set("i", i)
    commit()
    query <| $ ( values:array<int>; i:int )
        t |> equal( length(values), i+1)
        for x,v in range(i+1),values
            t |> equal( x, v)

