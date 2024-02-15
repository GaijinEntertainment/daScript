require dastest/testing_boost

require jit


require UnitTest

[test,jit]
def test_table ( t:T? )
    t |> run("table") <| @ ( t : T? )

        t |> success ( is_jit_function(@@test_table) )

        var tab : table<EntityId; string>
        tab[EntityId(1)] = "hello"
        tab[EntityId(2)] = "world"
        tab[EntityId(3)] = "!"
        t |> equal (3, length(tab))
        tab |> erase(EntityId(3))
        t |> equal (2, length(tab))

        tab |> find(EntityId(2)) <| $ ( val )
            t |> success(val!=null && *val=="world")

        tab |> get(EntityId(1)) <| $ ( val )
            t |> equal(val, "hello")

        if !tab |> key_exists(EntityId(1))
            t |> failure("key does not exist (1)")
        if tab |> key_exists(EntityId(3))
            t |> failure("key exists (3)")

        let qq = tab ?[EntityId(1)] ?? "goodbye"
        t |> equal(qq, "hello")
        var ptab = unsafe(addr(tab))
        let q = ptab ?[EntityId(2)] ?? "goodbye"
        t |> equal(q, "world")

        var count = 0
        for k,v in keys(tab),values(tab)
            if int(k) == 1
                t |> equal(v, "hello")
            if int(k) == 2
                t |> equal(v, "world")
            count ++
        assert(count==2)

    t |> run("set") <| @ ( t : T? )
        var set : table<EntityId>
        set |> insert(EntityId(1))
        set |> insert(EntityId(2))
        set |> insert(EntityId(3))
        t |> equal(3, length(set))
        if !set |> key_exists(EntityId(3))
            t |> failure("key does not exist (3)")
        if set |> key_exists(EntityId(4))
            t |> failure("key exists (4)")



