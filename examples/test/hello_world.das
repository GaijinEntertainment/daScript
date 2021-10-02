options log_compile_time

require daslib/decs
require daslib/apply
require daslib/static_let

struct Foo
    pos : float3
    name : string
    id : EntityId
    foobar : int

[export]
def main
    print("creating:\n")
    create_entity <| @ ( eid, cmp )
        print("eid = {eid}\n")
        cmp |> set("pos",float3(1.,2.,3.))
        cmp |> set("pos",13)
        cmp |> set("name","entity_once")
    create_entity <| @ ( eid, cmp )
        var foo = [[Foo pos=float3(1.), name="from template", id=eid]]
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

    print("\n\nafter delete:\n")
    print("deleting {de}\n")
    for d in de
        delete_entity(d)
    commit()
    debug_dump()


options debugger
require daslib/debug



