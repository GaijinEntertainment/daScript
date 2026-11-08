options gen2
options persistent_heap = true
options gc
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require daslib/decs_boost
require dastest/testing_boost public

// === decs_template with default prefix (StructName_) ===

[decs_template]
struct Particle {
    pos : float3
    vel : float3
    life : int
}

[test(description = "a decs_template component names get the struct-name prefix by default")]
def test_template_default_prefix(t : T?) {
    restart()
    let eid = create_entity() @(eid, cmp) {
        apply_decs_template(cmp, Particle(pos = float3(1, 2, 3), vel = float3(4, 5, 6), life = 100))
    }
    commit()
    // components stored with "Particle_" prefix
    var found = false
    query(eid) $(Particle_pos : float3; Particle_vel : float3; Particle_life : int) {
        t |> equal(Particle_pos, float3(1, 2, 3))
        t |> equal(Particle_vel, float3(4, 5, 6))
        t |> equal(Particle_life, 100)
        found = true
    }
    t |> success(found)
}

// === decs_template with custom prefix ===

[decs_template(prefix = "p_")]
struct Bullet {
    speed : float
    damage : int
}

[test(description = "a decs_template with a custom prefix names its components with it")]
def test_template_custom_prefix(t : T?) {
    restart()
    let eid = create_entity() @(eid, cmp) {
        apply_decs_template(cmp, Bullet(speed = 100.0, damage = 50))
    }
    commit()
    var found = false
    query(eid) $(p_speed : float; p_damage : int) {
        t |> equal(p_speed, 100.0)
        t |> equal(p_damage, 50)
        found = true
    }
    t |> success(found)
}

// === decs_template with empty prefix ===

[decs_template(prefix)]
struct Vehicle {
    speed : float
    fuel : int
}

[test(description = "a decs_template with an empty prefix names its components by field")]
def test_template_empty_prefix(t : T?) {
    restart()
    let eid = create_entity() @(eid, cmp) {
        apply_decs_template(cmp, Vehicle(speed = 60.0, fuel = 100))
    }
    commit()
    // no prefix — component names are "speed" and "fuel" directly
    var found = false
    query(eid) $(speed : float; fuel : int) {
        t |> equal(speed, 60.0)
        t |> equal(fuel, 100)
        found = true
    }
    t |> success(found)
}

// === Query with template struct ===

[test(description = "a query can read a template's components back as a struct")]
def test_query_with_template_struct(t : T?) {
    restart()
    for (i in range(3)) {
        create_entity() @(eid, cmp) {
            apply_decs_template(cmp, Particle(pos = float3(i), vel = float3(1), life = 10 * i))
        }
    }
    commit()
    var count = 0
    query() $(p : Particle) {
        count++
        t |> equal(p.vel, float3(1))
    }
    t |> equal(count, 3)
}

// === Mutable query with template struct ===

[test(description = "a query can modify the components of a template")]
def test_query_mutate_template(t : T?) {
    restart()
    let eid = create_entity() @(eid, cmp) {
        apply_decs_template(cmp, Particle(pos = float3(0), vel = float3(1, 0, 0), life = 10))
    }
    commit()
    // mutate via template
    query() $(var p : Particle) {
        p.pos += p.vel
        p.life --
    }
    // verify mutation
    var found = false
    query(eid) $(Particle_pos : float3; Particle_life : int) {
        t |> equal(Particle_pos, float3(1, 0, 0))
        t |> equal(Particle_life, 9)
        found = true
    }
    t |> success(found)
}

// === remove_decs_template ===

[test(description = "removing a template removes its components and keeps the others")]
def test_remove_template(t : T?) {
    restart()
    let eid = create_entity() @(eid, cmp) {
        apply_decs_template(cmp, Vehicle(speed = 60.0, fuel = 100))
        cmp.name := "car"
    }
    commit()
    // remove the template components
    update_entity(eid) @(eid, cmp) {
        cmp |> remove("speed")
        cmp |> remove("fuel")
    }
    commit()
    // "speed" and "fuel" gone, but "name" remains
    var found = false
    query(eid) $(name : string) {
        t |> equal(name, "car")
        found = true
    }
    t |> success(found)

    var foundSpeed = false
    query(eid) $(speed : float) {
        foundSpeed = true
    }
    t |> success(!foundSpeed)
}

// === Multiple templates on same entity ===

[test(description = "one entity can carry several templates")]
def test_multiple_templates(t : T?) {
    restart()
    let eid = create_entity() @(eid, cmp) {
        apply_decs_template(cmp, Particle(pos = float3(1), vel = float3(2), life = 5))
        apply_decs_template(cmp, Bullet(speed = 300.0, damage = 75))
    }
    commit()
    // both templates' components present
    var found = false
    query(eid) $(Particle_pos : float3; p_speed : float; p_damage : int) {
        t |> equal(Particle_pos, float3(1))
        t |> equal(p_speed, 300.0)
        t |> equal(p_damage, 75)
        found = true
    }
    t |> success(found)
}

[decs_template]
struct DefaultParticle {
    live = 13
}

[test(description = "a template built with default values creates an entity with those defaults")]
def test_create_from_default(t : T?) {
    restart()
    create_entity() @(eid, cmp) {
        apply_decs_template(cmp, DefaultParticle())
    }
    commit()
    query() $(var p : DefaultParticle) {
        p.live ++
    }
    query() $(var p : DefaultParticle) {
        t |> equal(p.live, 14)
    }
}

[test(description = "a query argument with a default value gets it when the component is missing")]
def test_default_values(t : T?) {
    restart()
    create_entity() @(eid, cmp) {
        cmp.a := 1
    }
    create_entity() @(eid, cmp) {
        cmp.a := 2
        cmp.b := 10
    }
    commit()
    var count = 0
    query() $(a : int; b : int = -1) {
        if (b == -1) {
            t |> equal(a, 1)
        } elif (b == 10) {
            t |> equal(a, 2)
        }
        count ++
    }
    t |> equal(count, 2)
}
