options gen2
options persistent_heap
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost
require daslib/decs_boost

[decs_template]
struct BenchEntity {
    pos : float3
    vel : float3
    lifetime : float
    radius : float
}

let SMALL = 10
let MEDIUM = 100
let LARGE = 1000

// --- Bulk create_entities ---

[benchmark]
def bulk_create(b : B?) {
    b |> run("create_entities/{SMALL}", SMALL) {
        restart()
        create_entities(SMALL) <| $(eid : EntityId; i : int; var cmp : ComponentMap) {
            apply_decs_template(cmp, BenchEntity(
                pos = float3(float(i), 0.0, 0.0),
                vel = float3(0.0, 1.0, 0.0),
                lifetime = 1.0,
                radius = 0.1
            ))
        }
    }
    b |> run("create_entities/{MEDIUM}", MEDIUM) {
        restart()
        create_entities(MEDIUM) <| $(eid : EntityId; i : int; var cmp : ComponentMap) {
            apply_decs_template(cmp, BenchEntity(
                pos = float3(float(i), 0.0, 0.0),
                vel = float3(0.0, 1.0, 0.0),
                lifetime = 1.0,
                radius = 0.1
            ))
        }
    }
    b |> run("create_entities/{LARGE}", LARGE) {
        restart()
        create_entities(LARGE) <| $(eid : EntityId; i : int; var cmp : ComponentMap) {
            apply_decs_template(cmp, BenchEntity(
                pos = float3(float(i), 0.0, 0.0),
                vel = float3(0.0, 1.0, 0.0),
                lifetime = 1.0,
                radius = 0.1
            ))
        }
    }
}

// --- Bulk create_entities direct (bypasses ComponentMap) ---

[benchmark]
def bulk_create_direct(b : B?) {
    b |> run("create_entities`direct/{SMALL}", SMALL) {
        restart()
        create_entities`BenchEntity(SMALL) <| $(eid : EntityId; i : int; var e : BenchEntity) {
            e.pos = float3(float(i), 0.0, 0.0)
            e.vel = float3(0.0, 1.0, 0.0)
            e.lifetime = 1.0
            e.radius = 0.1
        }
    }
    b |> run("create_entities`direct/{MEDIUM}", MEDIUM) {
        restart()
        create_entities`BenchEntity(MEDIUM) <| $(eid : EntityId; i : int; var e : BenchEntity) {
            e.pos = float3(float(i), 0.0, 0.0)
            e.vel = float3(0.0, 1.0, 0.0)
            e.lifetime = 1.0
            e.radius = 0.1
        }
    }
    b |> run("create_entities`direct/{LARGE}", LARGE) {
        restart()
        create_entities`BenchEntity(LARGE) <| $(eid : EntityId; i : int; var e : BenchEntity) {
            e.pos = float3(float(i), 0.0, 0.0)
            e.vel = float3(0.0, 1.0, 0.0)
            e.lifetime = 1.0
            e.radius = 0.1
        }
    }
}

// --- Individual create_entity + commit ---

[benchmark]
def individual_create(b : B?) {
    b |> run("create_entity+commit/{SMALL}", SMALL) {
        restart()
        for (i in range(SMALL)) {
            create_entity <| @(eid, cmp) {
                apply_decs_template(cmp, BenchEntity(
                    pos = float3(float(i), 0.0, 0.0),
                    vel = float3(0.0, 1.0, 0.0),
                    lifetime = 1.0,
                    radius = 0.1
                ))
            }
        }
        commit()
    }
    b |> run("create_entity+commit/{MEDIUM}", MEDIUM) {
        restart()
        for (i in range(MEDIUM)) {
            create_entity <| @(eid, cmp) {
                apply_decs_template(cmp, BenchEntity(
                    pos = float3(float(i), 0.0, 0.0),
                    vel = float3(0.0, 1.0, 0.0),
                    lifetime = 1.0,
                    radius = 0.1
                ))
            }
        }
        commit()
    }
    b |> run("create_entity+commit/{LARGE}", LARGE) {
        restart()
        for (i in range(LARGE)) {
            create_entity <| @(eid, cmp) {
                apply_decs_template(cmp, BenchEntity(
                    pos = float3(float(i), 0.0, 0.0),
                    vel = float3(0.0, 1.0, 0.0),
                    lifetime = 1.0,
                    radius = 0.1
                ))
            }
        }
        commit()
    }
}
