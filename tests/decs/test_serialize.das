options persistent_heap = true
options gc

require daslib/decs_boost
require dastest/testing_boost public

[test]
def test_serialize( var t: T? )
    restart()
    for i in range (3)
        create_entity <| @ ( eid, cmp )
            cmp |> set("i", i)
    for i in range (3)
        create_entity <| @ ( eid, cmp )
            cmp |> set("i", i)
            cmp |> set("s", "__{i}__")
    commit()
    var check_it <- @ <|
        query <| $ ( i:int )
            if i<0 || i>=3
                t->fatal("wrong size")
        query <| $ ( i:int; s:string )
            equal(t, s, "__{i}__" )
    invoke(check_it)
    var data <- mem_archive_save(decsState)
    restart()
    query <| $ ( i:int )
        panic("something is up")

    data |> mem_archive_load(decsState)
    invoke(check_it)
