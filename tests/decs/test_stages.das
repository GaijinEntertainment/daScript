options gen2
options persistent_heap = true
options gc
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require daslib/decs_boost
require daslib/apply
require daslib/static_let
require dastest/testing_boost public
require _test_stages_extra
require math

[test(description = "running a decs stage applies its system to every matching entity")]
def test_baisc_stage(t : T?) {
    restart()
    for (i in range(3)) {
        create_entity() @(eid, cmp) {
            cmp |> set("pos", float3(1 + i, 2 + i, 3 + i))
            cmp |> set("vel", float3(1, 1, 1))
            cmp |> set("i", i)
        }
    }
    commit()
    query() $(pos : float3; vel : float3; i : int) {
        t |> equal(pos, float3(1 + i, 2 + i, 3 + i))
        t |> equal(vel, float3(1, 1, 1))
    }

    t |> run("update") @(t : T?) {
        decs_stage("update")
        query() $(pos : float3; vel : float3; i : int) {
            t |> equal(pos, float3(1 + i, 2 + i, 3 + i) + vel)
        }
    }
}


[decs(stage = update)]
def update(var pos : float3&; vel : float3) {
    pos += vel
}

[decs(stage="make_visible")]
def make_visible(var visible : bool) {
    visible = true
}

[decs(stage="check_visible")]
def check_visible(visible : bool) {
    assert(visible)
}


[test(description = "stages declared in this file and in another module both run on the same entities")]
def test_two_stage(t : T?) {
    restart()
    for (_i in range(3)) {
        create_entity() @(eid, cmp) {
            cmp.visible := false
        }
    }
    commit()
    t |> run("make visible and check") @(t : T?) {
        decs_stage("make_visible")
        decs_stage("check_visible")
    }
    t |> run("check again") @(t : T?) {
        query() $(visible : bool) {
            t |> equal(visible, true)
        }
    }
    t |> run("make visible, which is in the other file") @(t : T?) {
        decs_stage("make_invisible")
    }
    t |> run("check one more time") @(t : T?) {
        query() $(visible : bool) {
            t |> equal(visible, false)
        }
    }
}

// === Stage function: physics ===

[decs(stage = physics)]
def apply_gravity(var vy : float&) {
    vy -= 9.8
}

[decs(stage = physics)]
def apply_velocity(var pos : float3&; vel : float3) {
    pos += vel
}

// === Stage function: scoring ===

[decs(stage = scoring, REQUIRE = scored)]
def add_score(var score : int&) {
    score ++
}

// === Tests ===

[test(description = "a stage system modifies component values in place")]
def test_stage_modifies_components(t : T?) {
    restart()
    let eid = create_entity() @(eid, cmp) {
        cmp.pos := float3(0, 0, 0)
        cmp.vel := float3(1, 0, 0)
        cmp.vy  := 100.0
    }
    commit()
    decs_stage("physics")
    var finalPos = float3(0)
    var finalVy = 0.0
    query(eid) $(pos : float3; vy : float) {
        finalPos = pos
        finalVy = vy
    }
    t |> equal(finalPos, float3(1, 0, 0))
    t |> equal(finalVy, 100.0 - 9.8)
}

[test(description = "a stage with a REQUIRE filter touches only entities that have the required component")]
def test_stage_with_require(t : T?) {
    restart()
    // entity WITH "scored" — should be affected
    let e1 = create_entity() @(eid, cmp) {
        cmp.score := 0
        cmp.scored := true
    }
    // entity WITHOUT "scored" — should NOT be affected
    let e2 = create_entity() @(eid, cmp) {
        cmp.score := 0
    }
    commit()
    decs_stage("scoring")
    var v1 = -1
    query(e1) $(score : int) {
        v1 = score
    }
    t |> equal(v1, 1)
    var v2 = -1
    query(e2) $(score : int) {
        v2 = score
    }
    t |> equal(v2, 0)
}

[test(description = "each decs_stage call applies the system once more")]
def test_stage_multiple_invocations(t : T?) {
    restart()
    let eid = create_entity() @(eid, cmp) {
        cmp.vy := 100.0
    }
    commit()
    // run physics 3 times
    decs_stage("physics")
    decs_stage("physics")
    decs_stage("physics")
    var v = 0.0
    query(eid) $(vy : float) {
        v = vy
    }
    // approximately 100 - 29.4
    t |> success(abs(v - (100.0 - 9.8 * 3.0)) < 0.01)
}

[test(description = "running an unknown stage does nothing")]
def test_nonexistent_stage_is_noop(t : T?) {
    restart()
    create_entity() @(eid, cmp) {
        cmp.val := 42
    }
    commit()
    // stage that doesn't exist does nothing
    decs_stage("does_not_exist")
    var v = 0
    query() $(val : int) {
        v = val
    }
    t |> equal(v, 42)
}

[test(description = "decs_stage commits pending changes before and after it runs")]
def test_stage_commits_before_and_after(t : T?) {
    // decs_stage calls commit() before and after
    restart()
    create_entity() @(eid, cmp) {
        cmp.vy := 50.0
    }
    // DON'T call commit — decs_stage should do it
    decs_stage("physics")
    var v = 0.0
    query() $(vy : float) {
        v = vy
    }
    t |> equal(v, 50.0 - 9.8)
}
