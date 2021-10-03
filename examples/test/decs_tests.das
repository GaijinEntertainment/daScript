options log_compile_time

require daslib/decs_boost
require daslib/apply
require daslib/static_let

[export]
def main
    test_create_delete()
    test_update()
    test_type_mismatch()
    test_query()
    test_query_macro()

// can specify prefix="string", prefix for nothing, or not specify for Particle_
[decs_template(prefix)]
struct Particle
    pos, vel : float3

def test_query_macro
    print("\n\nTEST QUERY MACRO\n")
    restart()
    for i in range(3)
        create_entity <| @ ( eid, cmp )
            cmp |> set ( "pos", float3(i))
            cmp |> set ( "vel", float3(.1,.2,.3))
    for i in range(2)
        create_entity <| @ ( eid, cmp )
            cmp |> set ( "pos", float3(i+3))
            cmp |> set ( "vel", float3(.1,.2,.3))
            cmp |> set ( "col", 0xffffffff)
    commit()
    var count = 0
    var count_ff = 0
    query <| $ ( var pos:float3&; vel:float3; col:uint=0x12345678 )
        pos += vel
        if col==0x12345678
            count_ff ++
        count ++
    assert(count==5)
    assert(count_ff==3)
    debug_dump()
    var count_nc = 0
    query <| $ [decs_require_not(col)] ( var pos:float3& )
        pos = float3(13)
        count_nc ++
    assert(count_nc==3)
    debug_dump()
    var count_par = 0
    query <| $ ( var p : Particle )
        p.pos += p.vel
        count_par ++
    assert(count_par==5)
    debug_dump()
    var count_npar = 0
    query <| $ ( [[prefix="nothere_"]] var p : Particle )
        p.pos += p.vel
        count_npar ++
    assert(count_npar==0)
    debug_dump()

def test_query
    print("\n\nTEST QUERY\n")
    restart()
    for i in range(3)
        create_entity <| @ ( eid, cmp )
            cmp |> set ( "pos", float3(i))
            cmp |> set ( "vel", float3(.1,.2,.3))
    for i in range(3)
        create_entity <| @ ( eid, cmp )
            cmp |> set ( "pos", float3(i+100))
            cmp |> set ( "vel", float3(.1,.2,.3))
            cmp |> set ( "col", 0xffffffff)
    commit()
    // query 1
    static_let <|
        var erq1 <- [[EcsRequest
            req <- [{string "pos"; "vel"}]
        ]]
    var total1 = 0
    erq1 |> for_each_archetype <| $ ( arch )
        for pos, vel in get(arch,"pos",type<float3>), get(arch,"vel",type<float3>)
            pos += vel
            total1 ++
    assert(total1==6)
    // query 2
    var total2 = 0
    static_let <|
        var erq2 <- [[EcsRequest
            req <- [{string "pos"; "col"}]
        ]]
    erq2 |> for_each_archetype <| $ ( arch )
        for col,pos in get(arch,"col",type<uint>),get_ro(arch,"pos",type<float3>)
            if pos.x > 101.
                col =  0xff00ffff
            // pos += float3(1) // this will fail due to "ro"
            total2 ++
    assert(total2==3)
    // query 3
    var total3 = 0
    var found3 = 0
    static_let <|
        var erq3 <- [[EcsRequest
            req <- [{string "pos"}]
        ]]
    erq3 |> for_each_archetype <| $ ( arch )
        for col,pos in get_default_ro(arch,"col",0x12345678), get(arch,"pos",type<float3>)
            if col==0xff00ffff
                pos = float3(13)
                found3 ++
            total3 ++
    assert(total3==6)
    assert(found3==2)
    debug_dump()
    var total4 = 0
    static_let <|
        var erq4 <- [[EcsRequest
            req <- [{string "pos"}],
            reqn <- [{string "col"}]
        ]]
    erq4 |> for_each_archetype <| $ ( arch )
        for pos in get(arch,"pos",type<float3>)
            pos.y -= 0.001
            total4 ++
    assert(total4==3)
    debug_dump()

def test_type_mismatch
    print("\n\nTEST TYPE MISMATCH\n")
    restart()
    create_entity <| @ ( eid, cmp )
        cmp |> set("pos", float3(1,2,3))
        try
            cmp |> set("pos", 13)
        recover
            print("mismatch set\n")
        try
            cmp |> get("pos", type<int>)
        recover
            print("mismatch get\n")
        let f123 = cmp |> get("pos", float3(13))
        print("get[pos] = {f123}\n")
    commit()
    debug_dump()

def test_update
    print("\n\nTEST UPDATE\n")
    restart()
    static_let <|
        var migr : EntityId
    print("before:\n")
    create_entity <| @ ( eid, cmp )
        cmp |> set("pos", float3(1,2,3))
        cmp |> set("vel", float3(0.,-9.8,0.))
        migr = eid
    commit()
    debug_dump()
    print("after add:\n")
    migr |> update_entity <| @ ( eid, cmp )
        cmp |> set("color", float4(1,0,0,1))
        migr = eid
    commit()
    debug_dump()
    print("after remove:\n")
    migr |> update_entity <| @ ( eid, cmp )
        let t = cmp |> has("vel")
        print("t = {t}\n")
        verify(cmp |> has("vel"))
        cmp |> remove("vel")
        verify(!(cmp |> has("vel")))
        migr = eid
    commit()
    debug_dump()
    print("without change:\n")
    migr |> update_entity <| @ ( eid, cmp )
        print("nothing\n")
        migr = eid
    commit()
    debug_dump()

struct Foo
    pos : float3
    name : string
    eid : EntityId
    foobar : int

def test_create_delete
    print("\n\nTEST CREATE-DELETE\n")
    restart()
    print("creating:\n")
    create_entity <| @ ( eid, cmp )
        print("eid = {eid}\n")
        cmp |> set("pos",float3(1.,2.,3.))
        cmp |> set("name","entity_once")
    create_entity <| @ ( eid, cmp )
        var foo = [[Foo pos=float3(1.), name="from template",eid=eid]]
        apply(foo) <| $ ( n, v )
            cmp |> set ( n, v)

    static_let <|
        var de : array<EntityId>
    for i in range(3)
        create_entity <| @ ( eid, cmp )
            print("i={i} eid = {eid}\n")
            if i==1 || i==2
                de |> push(eid)
            cmp |> set("pos",float3(1+i,2+i,3+i))
            cmp |> set("name","entity{i}")
            cmp |> set("id", i)
    commit()
    debug_dump()

    print("after delete:\n")
    print("deleting {de}\n")
    for d in de
        delete_entity(d)
    commit()
    debug_dump()

options debugger
require daslib/debug
