options gen2
options force_inscope_pod = true

require dastest/testing_boost public
require daslib/decs_boost

[test(description = "set moves an array into its component, so the scope-exit collect of the source leaves the component intact")]
def test_set_moves_container(t : T?) {
    restart()
    for (i in range(4)) {
        create_entity() @(eid, cmp) {
            var values <- [for (x in range(i)); int2(x, x + 13)]
            cmp |> set("values", values)
            t |> equal(length(values), 0, "the source is empty after set")
            cmp.i := i
        }
    }
    commit()
    var churn : array<int>
    for (k in range(1000)) {
        churn |> push(k * 7)
    }
    var seen = 0
    query() $(values : array<int2>; i : int) {
        t |> equal(length(values), i)
        for (x, v in range(i), values) {
            t |> equal(v, int2(x, x + 13))
        }
        seen ++
    }
    t |> equal(seen, 4)
}

def churn_heap() {
    var churn : array<int>
    for (k in range(1000)) {
        churn |> push(k * 7)
    }
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

[decs_template(prefix = "moved_")]
struct MovedTemplate {
    values : array<int2>
    i : int
}

[test(description = "a template's bulk create moves each entity's array field into archetype storage")]
def test_template_bulk_create_moves_container(t : T?) {
    restart()
    create_entities`MovedTemplate(4) $(eid : EntityId; i : int; var entity : MovedTemplate) {
        entity.values <- [for (x in range(i + 1)); int2(x, x + 13)]
        entity.i = i
    }
    churn_heap()
    expect_moved_values(t, 4)
}
