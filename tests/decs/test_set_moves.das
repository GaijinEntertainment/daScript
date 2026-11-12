options gen2
options force_inscope_pod = true

require dastest/testing_boost public
require daslib/decs_boost

def churn_heap() {
    var churn <- [for (k in range(1000)); k * 7]
}

def expect_moved_values(t : T?; count : int) {
    var seen = 0
    query() $(moved_values : array<int2>; moved_i : int) {
        t |> equal(length(moved_values), moved_i + 1)
        for (x, v in range(moved_i + 1), moved_values) {
            t |> equal(v, int2(x, x + 13))
        }
        seen ++
    }
    t |> equal(seen, count)
}

[test(description = "set moves an array into its component, so the scope-exit collect of the source leaves the component intact")]
def test_set_moves_container(t : T?) {
    restart()
    for (i in range(4)) {
        create_entity() @(eid, cmp) {
            var values <- [for (x in range(i + 1)); int2(x, x + 13)]
            if (i % 2 == 0) {
                cmp |> set("moved_values", values)
            } else {
                set(cmp.moved_values, values)
            }
            t |> equal(length(values), 0, "the source is empty after set")
            var index = 0
            index = i
            cmp |> set("moved_i", index)
            t |> equal(index, i, "a copyable source keeps its value")
        }
    }
    commit()
    churn_heap()
    expect_moved_values(t, 4)
}

[test(description = "set_direct and set_direct_at move an array into archetype storage and leave the source empty")]
def test_set_direct_moves_container(t : T?) {
    restart()
    var cmp : ComponentMap
    var shape : array<int2>
    cmp |> set("moved_values", shape)
    cmp |> set("moved_i", 0)
    create_entities_from_cmp(4, cmp) $(var arch : Archetype; eidx : int; eid : EntityId; i : int) {
        var values <- [for (x in range(i + 1)); int2(x, x + 13)]
        arch |> set_direct("eid", eidx, eid)
        if (i % 2 == 0) {
            arch |> set_direct("moved_values", eidx, values)
        } else {
            arch |> set_direct_at(find_component_index(arch, "moved_values"), eidx, values)
        }
        t |> equal(length(values), 0, "the source is empty after the write")
        arch |> set_direct("moved_i", eidx, i)
    }
    churn_heap()
    expect_moved_values(t, 4)
}

struct Tracked {
    values : array<int>
}

var released = 0

def finalize(var value : Tracked) {
    if (!empty(value.values)) {
        released ++
    }
    delete value.values
}

[test(description = "a direct write over a slot that holds a value releases that value, and the slot keeps the last one written")]
def test_set_direct_releases_the_held_value(t : T?) {
    restart()
    var cmp : ComponentMap
    var shape : Tracked
    cmp |> set("tracked", shape)
    released = 0
    create_entities_from_cmp(1, cmp) $(var arch : Archetype; eidx : int; eid : EntityId; _i : int) {
        arch |> set_direct("eid", eidx, eid)
        let slot = find_component_index(arch, "tracked")
        for (round in range(4)) {
            var value = Tracked(values <- [for (x in range(round + 1)); x + 13])
            if (round % 2 == 0) {
                arch |> set_direct("tracked", eidx, value)
            } else {
                arch |> set_direct_at(slot, eidx, value)
            }
            t |> equal(length(value.values), 0, "the source is empty after the write")
            t |> equal(released, round, "each write past the first releases what the slot held")
        }
    }
    churn_heap()
    var seen = 0
    query() $(tracked : Tracked) {
        t |> equal(length(tracked.values), 4)
        for (x, v in range(4), tracked.values) {
            t |> equal(v, x + 13)
        }
        seen ++
    }
    t |> equal(seen, 1)
}

[decs_template(prefix = "moved_")]
struct MovedTemplate {
    values : array<int2>
    i : int
}

[test(description = "a template's bulk create moves each entity's array field into archetype storage")]
def test_template_bulk_create_moves_container(t : T?) {
    restart()
    create_entities`MovedTemplate(4) $(_eid : EntityId; i : int; var entity : MovedTemplate) {
        entity.values <- [for (x in range(i + 1)); int2(x, x + 13)]
        entity.i = i
    }
    churn_heap()
    expect_moved_values(t, 4)
}
