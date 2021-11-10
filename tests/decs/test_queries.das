options persistent_heap = true
options gc

require daslib/decs_boost
require dastest/testing_boost public

[test]
def test_queries ( t: T? )
    restart()
    for i in range(3)
        create_entity <| @ ( eid, cmp )
            cmp |> set("pos", float3(1+i,2+i,3+i))
            cmp |> set("vel", float3(1,1,1))
            cmp |> set("i", i)
    commit()
    query <| $ ( pos:float3; vel:float3; i:int )    // check if we set the right values
        equal(t, pos, float3(1+i,2+i,3+i) )
        equal(t, vel, float3(1,1,1))
    t->run("nested") <| @@ ( t : T? )
        query <| $ ( pos:float3; i:int )
            let spos = pos
            let si = i
            query <| $ ( pos:float3; i:int )
                if spos==pos
                    equal(t, si, i)                 // if position is the same, index should be the same
    t->run("nested query one") <| @@ ( t : T? )
        query <| $ ( i:int; eid:EntityId )
            let si = i
            query(eid) <| $ ( i:int )               // this query does lookup by eid
                equal(t, si, i)                     //  we check if it can find the same thing
    t->run("nested with break") <| @@ ( t : T? )
        query <| $ ( i:int )
            for j in range(0,i)
                if j % 3 == 0
                    break                           // break is allowed, but only inside the loops - not query itself
    t->run("nested with continue") <| @@ ( t : T? )
        query <| $ ( i:int; var pos:float3 )
            for j in range(0,i)
                if j % 3 == 0
                    continue                        // continue is only allowed inside the loop, same as break
                pos.x = pos.x


[test]
def test_contracts ( t: T? )
    restart()
    for i in range(3)
        create_entity <| @ ( eid, cmp )
            cmp.pos := float3(i)
            cmp.vel := float3(.1,.2,.3)
    for i in range(2)
        create_entity <| @ ( eid, cmp )
            cmp.pos := float3(i+3)
            cmp.vel := float3(.1,.2,.3)
            cmp.col := 0xffffffff
    commit()
    t->run("require not") <| @@ ( t : T?)
        var count_nc = 0
        query <| $ [REQUIRE_NOT(col)] ( var pos:float3& )
            pos = float3(13)
            count_nc ++
        equal(t, count_nc, 3)
    t->run("require") <| @@ ( t : T?)
        var count_c = 0
        query <| $ [REQUIRE(col)] ( var pos:float3& )
            pos = float3(13)
            count_c ++
        equal(t, count_c, 2)

[export]
def main
    pass

