options gen2
options persistent_heap = true
options gc

require daslib/decs_boost
require dastest/testing_boost public

[test(description = "array components iterate, modify and read back")]
def test_arrays(t : T?) {
    restart()
    for (i in range(3)) {
        create_entity() @(eid, cmp) {
            cmp |> set("values", [for (x in range(i + 1)); x])
            cmp |> set("i", i)
        }
    }
    commit()
    t |> run("iterate") @@(t : T?) {
        query() $(values : array<int>; i : int) {
            t |> equal(length(values), i + 1)
            for (x, v in range(i + 1), values) {
                t |> equal(x, v)
            }
        }
    }
    t |> run("modify") @@(t : T?) {
        query() $(var values : array<int>; i : int) {
            delete values
            for (j in range(5)) {
                values |> push(j + i)
            }
        }
    }
    t |> run("verify") @@(t : T?) {
        query() $(values : array<int>; i : int) {
            t |> equal(length(values), 5)
            for (j in range(5)) {
                t |> equal(values[j], i + j)
            }
        }
    }
}

[test(description = "calling commit inside a query is a run-time error")]
def test_boundaries(t : T?) {
    restart()
    create_entity() @(eid, cmp) {
        cmp |> set("i", 1)
    }
    commit()
    t |> run("commit inside query") @@(t : T?) {
        query() $(i : int) {
            var failed = false
            try {
                commit()
            } recover {
                failed = true
            }
            t |> success(failed)
        }
    }
    t |> run("restart inside query") @@(t : T?) {
        query() $(i : int) {
            var failed = false
            try {
                commit()
            } recover {
                failed = true
            }
            t |> success(failed)
        }
    }
}

[test(description = "entities survive a serialize and deserialize round trip")]
def test_serialize(var t : T?) {
    restart()
    let entitiesNum = 3
    for (i in range(entitiesNum)) {
        create_entity() @(eid, cmp) {
            cmp |> set("i", i)
        }
    }
    for (i in range(entitiesNum)) {
        create_entity() @(eid, cmp) {
            cmp |> set("i", i)
            cmp |> set("s", "__{i}__")
        }
    }
    commit()
    var check_it <- $ {
        var num = 0
        query() $(i : int) {
            num ++
            if (i < 0 || i >= 3) {
                t->fatal("wrong size")
            }
        }
        t |> equal(num, entitiesNum + entitiesNum)
        num = 0
        query() $(i : int; s : string) {
            num ++
            t |> equal(s, "__{i}__")
        }
        t |> equal(num, entitiesNum)
    }
    invoke(check_it)
    var data <- mem_archive_save(decsState)
    restart()
    query() $(i : int) {
        panic("something is up")
    }

    data |> mem_archive_load(decsState)
    invoke(check_it)
}
