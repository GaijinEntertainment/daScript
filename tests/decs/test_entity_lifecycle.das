options gen2
options persistent_heap = true
options gc
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require daslib/decs_boost
require dastest/testing_boost public
require daslib/static_let
require daslib/apply
require daslib/strings_boost

[test(description = "every created entity gets a distinct id")]
def test_entity_id_uniqueness(t : T?) {
    restart()
    var ids : array<EntityId>
    ids |> reserve(10)
    for (i in range(10)) {
        let eid = create_entity() @(eid, cmp) {
            cmp.value := i
        }
        ids |> push(eid)
    }
    commit()
    // all ids unique
    for (i in range(length(ids))) {
        for (j in range(i + 1, length(ids))) {
            t |> success(ids[i] != ids[j])
        }
    }
    // all ids valid
    for (eid in ids) {
        t |> success(eid != INVALID_ENTITY_ID)
    }
}

[test(description = "the default entity id is invalid and compares as the invalid id")]
def test_invalid_entity_id(t : T?) {
    restart()
    t |> run("default is invalid") @(t : T?) {
        let eid : EntityId
        t |> equal(eid, INVALID_ENTITY_ID)
    }
    t |> run("invalid entity id equality") @(t : T?) {
        t |> equal(INVALID_ENTITY_ID, INVALID_ENTITY_ID)
    }
}

[test(description = "a deleted entity's slot is reused with a bumped generation and the old id becomes dead")]
def test_entity_id_recycling(t : T?) {
    restart()
    let eid1 = create_entity() @(eid, cmp) {
        cmp.tag := 1
    }
    commit()
    let gen1 = eid1.generation
    let id1 = eid1.id
    // delete and commit
    delete_entity(eid1)
    commit()
    // create again — should reuse the id slot with incremented generation
    let eid2 = create_entity() @(eid, cmp) {
        cmp.tag := 2
    }
    commit()
    t |> run("reuses id slot") @(t : T?) {
        t |> equal(eid2.id, id1)
    }
    t |> run("generation increments") @(t : T?) {
        t |> success(eid2.generation > gen1)
    }
    t |> run("old eid no longer valid") @(t : T?) {
        t |> success(eid1 != eid2)
    }
    t |> run("new entity has correct data") @(t : T?) {
        var found = false
        query(eid2) $(tag : int) {
            t |> equal(tag, 2)
            found = true
        }
        t |> success(found)
    }
    t |> run("old eid query is empty") @(t : T?) {
        var found = false
        query(eid1) $(tag : int) {
            found = true
        }
        t |> success(!found)
    }
}

[test(description = "repeated create and delete cycles keep recycling the slot with a rising generation")]
def test_multiple_recycle_cycles(t : T?) {
    restart()
    var lastId : uint
    for (cycle in range(5)) {
        let eid = create_entity() @(eid, cmp) {
            cmp.cycle := cycle
        }
        commit()
        if (cycle > 0) {
            t |> equal(eid.id, lastId)
        }
        lastId = eid.id
        delete_entity(eid)
        commit()
    }
    t |> success(true)
}

[test(description = "deleting an already deleted entity is harmless")]
def test_delete_already_deleted(t : T?) {
    restart()
    let eid = create_entity() @(eid, cmp) {
        cmp.x := 1
    }
    commit()
    delete_entity(eid)
    commit()
    // delete again — should be a no-op (generation mismatch)
    delete_entity(eid)
    commit()
    t |> success(true)
}

[test(description = "an entity with a single component is created and readable")]
def test_entity_minimal(t : T?) {
    // entity with a single component — the simplest useful entity
    restart()
    let eid = create_entity() @(eid, cmp) {
        cmp.tag := 1
    }
    commit()
    var found = false
    query(eid) $(tag : int) {
        t |> equal(tag, 1)
        found = true
    }
    t |> success(found)
}

[test(description = "an entity can hold components of many value types")]
def test_entity_many_component_types(t : T?) {
    restart()
    let eid = create_entity() @(eid, cmp) {
        cmp.i  := 42
        cmp.f  := 3.14
        cmp.s  := "hello"
        cmp.v3 := float3(1, 2, 3)
        cmp.v4 := float4(1, 2, 3, 4)
        cmp.b  := true
        cmp.u  := 0xdeadbeefu
    }
    commit()
    query(eid) $(i : int; f : float; s : string; v3 : float3; v4 : float4; b : bool; u : uint) {
        t |> equal(i, 42)
        t |> equal(f, 3.14)
        t |> equal(s, "hello")
        t |> equal(v3, float3(1, 2, 3))
        t |> equal(v4, float4(1, 2, 3, 4))
        t |> equal(b, true)
        t |> equal(u, 0xdeadbeefu)
    }
}

[test(description = "entity_count sums entities over all archetypes")]
def test_entity_count_across_archetypes(t : T?) {
    restart()
    // 3 entities with {pos}
    for (i in range(3)) {
        create_entity() @(eid, cmp) {
            cmp.pos := float3(i)
        }
    }
    // 2 entities with {pos, vel}
    for (i in range(2)) {
        create_entity() @(eid, cmp) {
            cmp.pos := float3(i)
            cmp.vel := float3(1)
        }
    }
    commit()
    var total = 0
    query() $(pos : float3) {
        total++
    }
    t |> equal(total, 5)
    var velCount = 0
    query() $(vel : float3) {
        velCount++
    }
    t |> equal(velCount, 2)
}

[test(description = "create, update, modify, empty update and remove each change the entity as requested")]
def test_create_update_remove(t : T?) {
    restart()
    static_let() {
        var migr : EntityId
    }
    t |> run("create entity") @(t : T?) {
        create_entity() @(eid, cmp) {
            cmp.pos := float3(1, 2, 3)
            cmp.vel := float3(0., -9.8, 0.)
            verify(cmp |> has("pos"))
            verify(cmp |> has("vel"))
            migr = eid
        }
        commit()
        query(migr) $(pos, vel : float3) {
            assert(pos == float3(1, 2, 3))
            assert(vel == float3(0., -9.8, 0.))
        }
    }
    t |> run("update entity") @(t : T?) {
        migr |> update_entity() @(eid, cmp) {
            verify(cmp |> has("pos"))
            verify(cmp |> has("vel"))
            verify(!cmp |> has("color"))
            cmp.color := float4(1, 0, 0, 1)
            verify(cmp |> has("color"))
            migr = eid
        }
        commit()
        query(migr) $(pos, vel : float3; color : float4) {
            assert(pos == float3(1, 2, 3))
            assert(color == float4(1, 0, 0, 1))
            assert(vel == float3(0., -9.8, 0.))
        }
    }
    t |> run("modify entity") @(t : T?) {
        migr |> update_entity() @(eid, cmp) {
            verify(cmp |> has("vel"))
            cmp |> remove("vel")
            verify(!cmp |> has("vel"))
            migr = eid
        }
        commit()
        query(migr) $(pos : float3; color : float4) {
            assert(pos == float3(1, 2, 3))
            assert(color == float4(1, 0, 0, 1))
        }
    }
    t |> run("empty update") @(t : T?) {
        migr |> update_entity() @(eid, cmp) {
            migr = eid
        }
        commit()
    }
    t |> run("remove") @(t : T?) {
        migr |> delete_entity
        commit()
        query(migr) $(pos : float3; color : float4) {
            panic("somehwow did not remove")
        }
    }
}

[test(description = "a created entity exists after commit and is gone after delete")]
def test_create_delete(t : T?) {
    restart()
    create_entity() @(eid, cmp) {
        cmp.pos  := float3(1., 2., 3.)
        cmp.name := "entity_once"
    }
    static_let() {
        var de : array<EntityId>
    }
    for (i in range(3)) {
        create_entity() @(eid, cmp) {
            if (i == 1 || i == 2) {
                de |> push(eid)
            }
            cmp.pos  := float3(1 + i, 2 + i, 3 + i)
            cmp.name := "entity{i}"
            cmp.id   := i
        }
    }
    commit()
    t |> run("verify create") @(t : T?) {
        var total = 0
        query() $(name : string) {
            success(t, name |> starts_with("entity"))
            total ++
        }
        t |> equal(total, 4)
    }
    for (d in de) {
        delete_entity(d)
    }
    commit()
    t |> run("verify delete") @(t : T?) {
        var total = 0
        query() $(name : string) {
            if (name != "entity0" && name != "entity_once") {
                t->fatal("deleted something else {name}")
            }
            total ++
        }
        t |> equal(total, 2)
    }
}
