options gen2
options persistent_heap

require rr_globals public
require river
require rr_audio

// --- Spawn helpers ---

def spawn_trail(origin, color : float3) {
    create_entity() @(eid, cmp) {
        apply_decs_template(cmp, Particle(
            pos = origin,
            vel = float3(0.0),
            color = color,
            lifetime = TRAIL_LIFETIME,
            max_life = TRAIL_LIFETIME,
            size = BULLET_SIZE * 1.1,
            spin_speed = 0.0,
            spin_phase = 0.0,
            kind = ParticleKind.trail,
            drag = 0.0,
            gravity = 0.0
        ))
    }
}

def bonus_color(t : BonusType) : float3 {
    return (t == BonusType.fuel ? float3(0.15, 0.9, 0.95)
        : (t == BonusType.multishot ? float3(1.0, 0.45, 0.12)
        : (t == BonusType.fastshot ? float3(1.0, 0.7, 0.05)
        : float3(0.95, 1.0, 0.3))))
}

def spawn_pickup_burst(origin, color : float3) {
    create_entities`Particle(14) $(_eid : EntityId; _i : int; var p : Particle) {
        let angle = random_f() * 2.0 * PI
        let elev = random_range(-0.25, 0.85)
        let dir = normalize(float3(cos(angle), sin(angle), elev))
        let speed = random_range(4.5, 8.5)
        p.pos = origin
        p.vel = dir * speed
        p.color = color
        p.lifetime = random_range(0.18, 0.32)
        p.max_life = p.lifetime
        p.size = random_range(0.06, 0.14)
        p.spin_speed = random_range(-15.0, 15.0)
        p.spin_phase = random_f() * 2.0 * PI
        p.kind = ParticleKind.ember
        p.drag = 2.2
        p.gravity = 5.0
    }
}

// --- Explosions -------------------------------------------------------------
//
// One blast is six layers, each with its own lifetime, motion and blend mode:
// a single white flash at the instant of the hit, a handful of fireballs that
// expand and cool, embers that arc under gravity, smoke that rises and outlives
// everything, solid debris, and a flat shockwave ring across the water. The
// layering is what turns "boxes flying" into an explosion; no single layer
// carries it alone.

def private spawn_flash(origin : float3; radius : float) {
    create_entities`Particle(1) $(_eid : EntityId; _i : int; var p : Particle) {
        p.pos = origin
        p.vel = float3(0.0)
        p.color = float3(1.0, 0.96, 0.86)
        p.lifetime = 0.075
        p.max_life = p.lifetime
        p.size = radius * 0.75
        p.spin_speed = 0.0
        p.spin_phase = 0.0
        p.kind = ParticleKind.flash
        p.drag = 0.0
        p.gravity = 0.0
    }
}

def private spawn_fireballs(origin, color : float3; radius : float; count : int) {
    create_entities`Particle(count) $(_eid : EntityId; _i : int; var p : Particle) {
        let angle = random_f() * 2.0 * PI
        let elev = random_range(-0.2, 0.9)
        let dir = normalize(float3(cos(angle), sin(angle), elev))
        p.pos = origin + dir * random_range(0.0, radius * 0.55)
        p.vel = dir * random_range(0.6, 2.6) + float3(0.0, 0.0, random_range(0.4, 1.6))
        p.color = color
        p.lifetime = random_range(0.26, 0.46)
        p.max_life = p.lifetime
        p.size = radius * random_range(0.22, 0.46)
        p.spin_speed = 0.0
        p.spin_phase = random_f() * 2.0 * PI
        p.kind = ParticleKind.fire
        p.drag = 3.4
        p.gravity = -1.2   // hot gas rises
    }
}

def private spawn_embers(origin, color : float3; radius : float; count : int) {
    create_entities`Particle(count) $(_eid : EntityId; _i : int; var p : Particle) {
        let angle = random_f() * 2.0 * PI
        let elev = random_range(0.1, 1.5)
        let dir = normalize(float3(cos(angle), sin(angle), elev))
        p.pos = origin + dir * radius * 0.3
        p.vel = dir * random_range(5.0, 15.0) * (0.6 + radius * 0.4)
        p.color = color
        p.lifetime = random_range(0.45, 1.15)
        p.max_life = p.lifetime
        p.size = random_range(0.035, 0.085)
        p.spin_speed = 0.0
        p.spin_phase = 0.0
        p.kind = ParticleKind.ember
        p.drag = 1.1
        p.gravity = 9.5
    }
}

def private spawn_smoke(origin : float3; radius : float; count : int) {
    create_entities`Particle(count) $(_eid : EntityId; _i : int; var p : Particle) {
        let angle = random_f() * 2.0 * PI
        let dir = float3(cos(angle), sin(angle), 0.0)
        p.pos = origin + dir * random_range(0.0, radius * 0.7) + float3(0.0, 0.0, random_range(0.0, radius * 0.4))
        p.vel = dir * random_range(0.4, 1.6) + float3(0.0, 0.0, random_range(1.1, 2.6))
        // Smoke takes the section's fog colour so it sits in the same air as
        // the rest of the scene instead of reading as a grey decal.
        p.color = lerp(float3(0.10, 0.09, 0.09), env_now.fog_color, 0.35)
        p.lifetime = random_range(1.3, 2.4)
        p.max_life = p.lifetime
        p.size = radius * random_range(0.24, 0.46)
        p.spin_speed = random_range(-1.2, 1.2)
        p.spin_phase = random_f() * 2.0 * PI
        p.kind = ParticleKind.smoke
        p.drag = 1.5
        p.gravity = -0.7
    }
}

def private spawn_debris(origin, color : float3; radius : float; count : int) {
    create_entities`Particle(count) $(_eid : EntityId; _i : int; var p : Particle) {
        let angle = random_f() * 2.0 * PI
        let elev = random_range(0.05, 1.1)
        let dir = normalize(float3(cos(angle), sin(angle), elev))
        p.pos = origin + dir * radius * 0.4
        p.vel = dir * random_range(3.0, 9.0) * (0.7 + radius * 0.4)
        p.color = color
        p.lifetime = random_range(0.6, 1.25)
        p.max_life = p.lifetime
        p.size = random_range(0.035, 0.10) * (0.7 + radius * 0.4)
        p.spin_speed = random_range(-13.0, 13.0)
        p.spin_phase = random_f() * 2.0 * PI
        p.kind = ParticleKind.debris
        p.drag = 0.55
        p.gravity = 11.0
    }
}

def private spawn_shockwave(origin : float3; radius : float) {
    create_entities`Particle(1) $(_eid : EntityId; _i : int; var p : Particle) {
        // Pinned just above the water so the ring reads as a surface wave
        // rather than a sphere seen edge-on.
        p.pos = float3(origin.x, origin.y, 0.06)
        p.vel = float3(0.0)
        p.color = float3(1.0, 0.92, 0.75)
        p.lifetime = 0.5
        p.max_life = p.lifetime
        p.size = radius * 3.0
        p.spin_speed = 0.0
        p.spin_phase = 0.0
        p.kind = ParticleKind.shock
        p.drag = 0.0
        p.gravity = 0.0
    }
}

// `scale` drives the whole blast: 1.0 is a boat, ~1.8 a fuel depot, and the
// player's own death runs at 2.2 so losing a life lands as an event.
def spawn_explosion(origin, color : float3; object_radius, scale : float) {
    let r = max(object_radius, 0.35) * scale
    spawn_flash(origin, r)
    spawn_fireballs(origin, color, r, int(3.0 + 3.0 * scale))
    spawn_embers(origin, lerp(color, float3(1.0, 0.85, 0.45), 0.5), r, int(10.0 + 12.0 * scale))
    spawn_smoke(origin, r, int(3.0 + 4.0 * scale))
    spawn_debris(origin, color * 0.55, r, int(4.0 + 5.0 * scale))
    spawn_shockwave(origin, r)
}

// Small sparks with no fireball -- bullet impacts and ricochets.
def spawn_particles(origin, color : float3; object_radius : float; count : int) {
    spawn_embers(origin, color, max(object_radius, 0.18), count)
}

def spawn_enemy_boat(pos : float3) {
    create_entity() @(eid, cmp) {
        apply_decs_template(cmp, EnemyBoat(
            pos = pos,
            vel = float3(0.0),
            health = 1,
            shoot_timer = random_range(0.5, BOAT_SHOOT_INTERVAL),
            patrol_dir = random_sign(),
            lane_prefer_x = pos.x
        ))
    }
}

def spawn_enemy_plane(pos : float3) {
    let spd = PLANE_SPEED * section_speed_mult()
    create_entity() @(eid, cmp) {
        apply_decs_template(cmp, EnemyPlane(
            pos = pos,
            vel = float3(random_sign() * spd, 0.0, 0.0),
            health = 1,
            shoot_timer = random_range(0.3, PLANE_SHOOT_INTERVAL)
        ))
    }
}

def spawn_enemy_heli(pos : float3) {
    create_entity() @(eid, cmp) {
        apply_decs_template(cmp, EnemyHelicopter(
            pos = pos,
            vel = float3(0.0),
            health = 2,
            shoot_timer = random_range(0.5, ENEMY_HELI_SHOOT_INTERVAL),
            hover_phase = random_f() * 2.0 * PI,
            target_x = pos.x
        ))
    }
}

def spawn_fuel_depot(pos : float3) {
    create_entity() @(eid, cmp) {
        apply_decs_template(cmp, FuelDepot(pos = pos))
    }
}

def spawn_bridge(pos : float3) {
    let seg = sample_river_segment(pos.y)
    let bounds = (
        seg.split
            ? (random_f() < 0.5
                ? float2(seg.left_bank_x, seg.split_left_x)
                : float2(seg.split_right_x, seg.right_bank_x))
            : float2(seg.left_bank_x, seg.right_bank_x)
    )
    let center = (bounds.x + bounds.y) * 0.5
    let width = bounds.y - bounds.x
    let gap_half = BRIDGE_GAP_WIDTH * 0.5
    let min_gap_center = bounds.x + gap_half + 0.1
    let max_gap_center = bounds.y - gap_half - 0.1
    let r = random_f()
    let gap_raw = (
        r < 0.33
            ? bounds.x + width * 0.2
            : (r < 0.66 ? center : bounds.y - width * 0.2)
    )
    let gap_x = (
        min_gap_center <= max_gap_center
            ? clamp(gap_raw, min_gap_center, max_gap_center)
            : center
    )
    let bridge_health = min(
        BRIDGE_HEALTH_MAX,
        BRIDGE_HEALTH_MIN + current_section / BRIDGE_HEALTH_STEP_SECTIONS
    )
    create_entity() @(eid, cmp) {
        apply_decs_template(cmp, Bridge(
            pos = pos,
            gap_center_x = gap_x,
            left_x = bounds.x,
            right_x = bounds.y,
            health = bridge_health
        ))
    }
}

def spawn_island(pos : float3) {
    create_entity() @(eid, cmp) {
        apply_decs_template(cmp, Island(
            pos = pos,
            size = random_range(ISLAND_SIZE_MIN, ISLAND_SIZE_MAX)
        ))
    }
}

// Pick what grows here. Reeds only make sense at the waterline, boulders and
// bushes fill the near bank, and the taller silhouettes go further back -- which
// is also how a real bank stacks up from the water.
def flora_kind_for(shore_dist : float) : int {
    let r = random_f()
    if (shore_dist < 2.6) {
        return (r < 0.62 ? FLORA_REEDS : (r < 0.85 ? FLORA_BUSH : FLORA_ROCK))
    }
    if (shore_dist < 7.0) {
        if (r < 0.30) { return FLORA_BUSH; }
        if (r < 0.46) { return FLORA_ROCK; }
        if (r < 0.72) { return FLORA_BROADLEAF; }
        if (r < 0.86) { return FLORA_CONIFER; }
        return FLORA_DEAD
    }
    if (r < 0.42) { return FLORA_CONIFER; }
    if (r < 0.70) { return FLORA_BROADLEAF; }
    if (r < 0.82) { return FLORA_BUSH; }
    if (r < 0.92) { return FLORA_ROCK; }
    return FLORA_DEAD
}

def spawn_flora(pos : float3; kind : int; size : float) {
    var ground = terrain_height(pos.x, pos.y)
    for (dx in fixed_array(-size * 0.4, size * 0.4)) {
        ground = max(ground, terrain_height(pos.x + dx, pos.y))
    }
    let grounded = float3(pos.x, pos.y, ground)
    create_entity() @(eid, cmp) {
        apply_decs_template(cmp, RiverTree(
            pos = grounded,
            size = size,
            tiers = 3,
            green_tint = random_f(),
            green_shift = random_range(-0.12, 0.12),
            trunk_ratio = 0.35,
            kind = kind,
            seed = int(random_f() * 1000.0),
            yaw = random_f() * 2.0 * PI
        ))
    }
}

def spawn_river_tree(pos : float3; size : float; tiers : int; green_tint, green_shift, trunk_ratio : float) {
    let grounded = float3(pos.x, pos.y, terrain_height(pos.x, pos.y))
    create_entity() @(eid, cmp) {
        apply_decs_template(cmp, RiverTree(
            pos = grounded,
            size = size,
            tiers = tiers,
            green_tint = green_tint,
            green_shift = green_shift,
            trunk_ratio = trunk_ratio,
            kind = FLORA_CONIFER,
            seed = int(random_f() * 1000.0),
            yaw = random_f() * 2.0 * PI
        ))
    }
}

def spawn_river_house(pos : float3; size, body_tint, roof_tint : float) {
    // Sample the footprint corners and stand on the highest one, so a house on
    // a slope rests on the ground instead of being half-buried by it.
    var ground = terrain_height(pos.x, pos.y)
    for (dx in fixed_array(-size, size)) {
        for (dy in fixed_array(-size, size)) {
            ground = max(ground, terrain_height(pos.x + dx, pos.y + dy))
        }
    }
    let grounded = float3(pos.x, pos.y, ground)
    create_entity() @(eid, cmp) {
        apply_decs_template(cmp, RiverHouse(
            pos = grounded,
            size = size,
            body_tint = body_tint,
            roof_tint = roof_tint
        ))
    }
}

def spawn_bonus_pickup(pos : float3; bonus_type : BonusType) {
    create_entity() @(eid, cmp) {
        apply_decs_template(cmp, BonusPickup(
            pos = pos,
            bonus_type = bonus_type,
            bob_phase = random_f() * 2.0 * PI
        ))
    }
}

def find_safe_respawn_pos(from_pos : float3) : float3 {
    // Keep respawn roughly where player died (same Y), only nudge X to a safe lane.
    let seg = sample_river_segment(from_pos.y)
    let margin = PLAYER_SIZE * 1.8
    if (!seg.split) {
        return float3(
            clamp(from_pos.x, seg.left_bank_x + margin, seg.right_bank_x - margin),
            from_pos.y,
            PLAYER_Z
        )
    }

    let lx0 = seg.left_bank_x + margin
    let lx1 = seg.split_left_x - margin
    let rx0 = seg.split_right_x + margin
    let rx1 = seg.right_bank_x - margin

    // If already inside one split lane, keep that lane.
    if (from_pos.x >= lx0 && from_pos.x <= lx1) {
        return float3(clamp(from_pos.x, lx0, lx1), from_pos.y, PLAYER_Z)
    }
    if (from_pos.x >= rx0 && from_pos.x <= rx1) {
        return float3(clamp(from_pos.x, rx0, rx1), from_pos.y, PLAYER_Z)
    }

    // Otherwise choose nearest lane center with minimal displacement.
    let lc = (lx0 + lx1) * 0.5
    let rc = (rx0 + rx1) * 0.5
    let x = (abs(from_pos.x - lc) < abs(from_pos.x - rc) ? lc : rc)
    return float3(x, from_pos.y, PLAYER_Z)
}

def spawn_section_objects() {
    // Determine a Y range ahead of player to place objects
    let base_y = player_pos.y + 40.0
    let section_mult = float(min(current_section + 1, 5))
    spawn_section_bridges(base_y, section_mult)
    spawn_section_islands(base_y, section_mult)
    spawn_section_trees(base_y)
    spawn_section_houses(base_y)
    spawn_section_depots(base_y)
    spawn_section_enemies(base_y, section_mult)
}

def spawn_section_bridges(base_y, section_mult : float) {
    let bridge_count = int(1.0 + section_mult * 0.4)
    for (i in range(bridge_count)) {
        let by = base_y + SECTION_LENGTH * (float(i + 1) / float(bridge_count + 1))
        spawn_bridge(float3(0.0, by, BRIDGE_Z))
    }
    commit()
}

def spawn_section_islands(base_y, section_mult : float) {
    let island_count = int(section_mult * 0.8 + random_f() * 2.0)
    for (_i in range(island_count)) {
        // six tries; the island is skipped when only split terrain gets sampled
        for (_try in range(6)) {
            let iy = base_y + random_range(10.0, SECTION_LENGTH - 10.0)
            let seg = sample_river_segment(iy)
            if (seg.split) {
                continue
            }
            let ibounds = river_clamp_x_for(iy, random_sign() * 20.0)
            let icx = (ibounds.x + ibounds.y) * 0.5 + random_range(-1.5, 1.5)
            spawn_island(float3(icx, iy, ISLAND_Z))
            break
        }
    }
    commit()
}

// River-side trees — denser and varied.
// Bank dressing. The old pass scattered ~12 conifers over a 240m section, which
// is why the banks read as empty fields with the occasional lollipop. This one
// plants roughly six times as much and draws from the whole flora vocabulary,
// clustered rather than evenly sprinkled -- real banks come in thickets with
// gaps between them, not in a uniform scatter.
def spawn_section_trees(base_y : float) {
    let clumps = int(26.0 + random_f() * 12.0)
    for (_c in range(clumps)) {
        let cy = base_y + random_range(6.0, SECTION_LENGTH - 6.0)
        let left_side = random_f() < 0.5
        let members = 1 + int(random_f() * 4.0)
        for (_m in range(members)) {
            let ty = cy + random_range(-4.5, 4.5)
            let seg = sample_river_segment(ty)
            let bank_x = (left_side ? seg.left_bank_x : seg.right_bank_x)
            // Distance inland, biased toward the waterline where the eye is.
            let r = random_f()
            let away = (r < 0.30 ? random_range(0.2, 2.6)
                : (r < 0.72 ? random_range(2.6, 7.0) : random_range(7.0, 16.0)))
            let tx = bank_x + (left_side ? -away : away)
            let kind = flora_kind_for(away)
            let base_size = (kind == FLORA_REEDS ? random_range(0.8, 1.4)
                : (kind == FLORA_BUSH ? random_range(0.7, 1.2)
                : (kind == FLORA_ROCK ? random_range(0.35, 0.95)
                : random_range(0.95, 2.2))))
            spawn_flora(float3(tx, ty, 0.0), kind, base_size)
        }
    }
    commit()
}

// Occasional houses on banks.
def spawn_section_houses(base_y : float) {
    let house_attempts = int(3.0 + random_f() * 3.0)
    for (_i in range(house_attempts)) {
        if (random_f() > 0.42) {
            continue
        }
        // eight tries; the house is skipped when no clear spot near the bank turns up
        for (_try in range(8)) {
            let hy = base_y + random_range(12.0, SECTION_LENGTH - 12.0)
            let seg = sample_river_segment(hy)
            let left_side = random_f() < 0.5
            let bank_x = (left_side ? seg.left_bank_x : seg.right_bank_x)
            let away = random_range(2.8, 9.0)
            let hx = bank_x + (left_side ? -away : away)
            let hsize = random_range(0.8, 1.55)

            var clear_of_trees = true
            query() $(t : RiverTree) {
                if (clear_of_trees) {
                    let min_dist = hsize * 1.5 + t.size * 0.9
                    let d2 = dist2d_sq(float3(hx, hy, 0.0), t.pos)
                    if (d2 < min_dist * min_dist) {
                        clear_of_trees = false
                    }
                }
            }

            if (!clear_of_trees) {
                continue
            }

            let body_tint = random_range(0.0, 1.0)
            let roof_tint = random_range(0.0, 1.0)
            spawn_river_house(float3(hx, hy, 0.0), hsize, body_tint, roof_tint)
            break
        }
    }
    commit()
}

// Fuel depots — spaced evenly and never on top of islands
// At drain=1.8/s and min speed 4u/s, one section takes ~60s = 108 fuel total
def spawn_section_depots(base_y : float) {
    let depot_spacing = 70.0
    let depot_count = int(SECTION_LENGTH / depot_spacing)
    for (i in range(depot_count)) {
        for (_try in range(12)) {
            let depot_y = base_y + depot_spacing * (float(i) + 0.3 + random_f() * 0.4)
            let depot_bounds = river_clamp_x_for(depot_y, random_sign() * 20.0)
            let shore_margin = FUEL_DEPOT_SIZE * 1.25 + 0.5
            if (depot_bounds.y - depot_bounds.x <= shore_margin * 2.0) {
                continue
            }
            let depot_cx = random_range(depot_bounds.x + shore_margin, depot_bounds.y - shore_margin)
            let candidate = float3(depot_cx, depot_y, FUEL_DEPOT_Z)
            if (is_out_of_river(candidate)) {
                continue
            }

            var blocked = false
            find_query() $(isle : Island) {
                let rr = isle.size + FUEL_DEPOT_SIZE + 0.45
                if (dist2d_sq(isle.pos, candidate) < rr * rr) {
                    blocked = true
                    return true
                }
                return false
            }

            if (!blocked) {
                spawn_fuel_depot(candidate)
                break
            }
        }
    }
    commit()
}

// A boat may not spawn on an island, on a fuel depot, or under a bridge span.
def boat_spot_blocked(candidate : float3) : bool {
    var blocked = false
    find_query() $(isle : Island) {
        let rr = isle.size + BOAT_SIZE + 0.4
        if (dist2d_sq(isle.pos, candidate) < rr * rr) {
            blocked = true
            return true
        }
        return false
    }
    if (!blocked) {
        find_query() $(d : FuelDepot) {
            let rr = FUEL_DEPOT_SIZE + BOAT_SIZE + 0.45
            if (dist2d_sq(d.pos, candidate) < rr * rr) {
                blocked = true
                return true
            }
            return false
        }
    }
    if (!blocked) {
        find_query() $(br : Bridge) {
            if (
                abs(candidate.y - br.pos.y) < 1.2 &&
                candidate.x > br.left_x - BOAT_SIZE &&
                candidate.x < br.right_x + BOAT_SIZE
            ) {
                blocked = true
                return true
            }
            return false
        }
    }
    return blocked
}

// Enemies — staggered ahead
def spawn_section_enemies(base_y, section_mult : float) {
    let boat_count = int(2.0 + section_mult * 1.0)
    let plane_count = (current_section >= 1 ? int(1.0 + section_mult * 0.6) : 0)
    let heli_count = (current_section >= 2 ? int(section_mult * 0.5) : 0)

    for (_i in range(boat_count)) {
        for (_try in range(10)) {
            let ey = base_y + random_range(5.0, SECTION_LENGTH - 5.0)
            let bounds = river_clamp_x_for(ey, random_sign() * 20.0)
            let ex = random_range(bounds.x + 1.0, bounds.y - 1.0)
            let candidate = float3(ex, ey, BOAT_Z)
            if (!boat_spot_blocked(candidate)) {
                spawn_enemy_boat(candidate)
                break
            }
        }
    }
    for (_i in range(plane_count)) {
        let ey = base_y + random_range(5.0, SECTION_LENGTH - 5.0)
        let bounds = river_clamp_x_for(ey, random_sign() * 20.0)
        let side_x = (random_f() < 0.5 ? bounds.x - 3.0 : bounds.y + 3.0)
        spawn_enemy_plane(float3(side_x, ey, PLANE_Z))
    }
    for (_i in range(heli_count)) {
        let ey = base_y + random_range(15.0, SECTION_LENGTH - 15.0)
        let bounds = river_clamp_x_for(ey, random_sign() * 20.0)
        let hx = (bounds.x + bounds.y) * 0.5 + random_range(-2.0, 2.0)
        spawn_enemy_heli(float3(hx, ey, ENEMY_HELI_Z))
    }
    commit()
}

// --- Reset ---

def reset_game() {
    var inscope to_delete : array<EntityId>
    query() $(eid : EntityId) {
        to_delete |> push(eid)
    }
    for (eid in to_delete) {
        delete_entity(eid)
    }
    commit()

    score = 0
    player_lives = 3
    player_pos = float3(0.0, 0.0, PLAYER_Z)
    player_vel = float3(0.0)
    player_fwd_speed = PLAYER_FWD_SPEED_MIN
    player_fuel = PLAYER_FUEL_MAX
    player_invuln_timer = 0.0
    player_respawn_timer = 0.0
    player_shoot_cooldown = 0.0
    player_multishot_timer = 0.0
    player_fastshot_timer = 0.0
    cam_x = 0.0
    current_section = 0
    section_dist = 0.0
    section_banner_timer = 0.0
    restart_input_lock = 0.0
    screen_shake_amount = 0.0
    slow_mo_timer = 0.0
    low_fuel_beep_timer = 0.0
    refuel_sound_timer = 0.0
    set_env_immediate(0)

    global_rng_seed = uint(get_uptime() * 1000.0)
    init_river()
    spawn_section_objects()
    game_state = GameState.playing
}

// --- Player Update ---

def update_player_input() {
    // Keyboard
    let key_left = (
        glfwGetKey(live_window, GLFW_KEY_A) == GLFW_PRESS ||
        glfwGetKey(live_window, GLFW_KEY_LEFT) == GLFW_PRESS
    )
    let key_right = (
        glfwGetKey(live_window, GLFW_KEY_D) == GLFW_PRESS ||
        glfwGetKey(live_window, GLFW_KEY_RIGHT) == GLFW_PRESS
    )
    let key_up = (
        glfwGetKey(live_window, GLFW_KEY_W) == GLFW_PRESS ||
        glfwGetKey(live_window, GLFW_KEY_UP) == GLFW_PRESS
    )
    let key_down = (
        glfwGetKey(live_window, GLFW_KEY_S) == GLFW_PRESS ||
        glfwGetKey(live_window, GLFW_KEY_DOWN) == GLFW_PRESS
    )

    // Compose strafe and speed inputs
    var strafe_input = 0.0
    if (key_left) { strafe_input--; }
    if (key_right) { strafe_input++; }

    var speed_input = 0.0
    if (key_up) { speed_input++; }
    if (key_down) { speed_input--; }

    player_vel.x += strafe_input * PLAYER_STRAFE_SPEED * tick_dt * 6.0
    player_fwd_speed = clamp(
        player_fwd_speed + speed_input * 4.0 * tick_dt,
        PLAYER_FWD_SPEED_MIN,
        PLAYER_FWD_SPEED_MAX * section_speed_mult()
    )
}

def spawn_player_bullet(pos, vel : float3) {
    create_entity() @(eid, cmp) {
        apply_decs_template(cmp, PlayerBullet(
            pos = pos,
            vel = vel,
            age = 0.0
        ))
    }
}

def rotate_xy(v : float2; angle : float) : float2 {
    let ca = cos(angle)
    let sa = sin(angle)
    return float2(v.x * ca - v.y * sa, v.x * sa + v.y * ca)
}

def update_player_shoot() {
    player_shoot_cooldown = max(player_shoot_cooldown - tick_dt, 0.0)
    let fire = glfwGetKey(live_window, GLFW_KEY_SPACE) == GLFW_PRESS
    if (fire && player_shoot_cooldown <= 0.0 && player_respawn_timer <= 0.0) {
        let max_lateral = BULLET_SPEED * 0.259  // sin(15°) ≈ 0.259
        let lateral = clamp(player_vel.x, -max_lateral, max_lateral)
        let fwd = sqrt(BULLET_SPEED * BULLET_SPEED - lateral * lateral)
        let muzzle = player_pos + float3(0.0, 0.8, 0.0)
        let base_dir = float2(lateral, fwd)
        if (player_multishot_timer > 0.0) {
            for (deg in fixed_array(-7.5, 0.0, 7.5)) {
                let d = rotate_xy(base_dir, deg * PI / 180.0)
                spawn_player_bullet(muzzle, float3(d.x, d.y, 0.0))
            }
        } else {
            spawn_player_bullet(muzzle, float3(base_dir.x, base_dir.y, 0.0))
        }
        play_sfx(snd_shoot)
        player_shoot_cooldown = (player_fastshot_timer > 0.0 ? FIRE_COOLDOWN * 0.5 : FIRE_COOLDOWN)
    }
}

def update_player_movement() {
    player_vel.x *= PLAYER_FRICTION
    player_vel.y = player_fwd_speed

    player_pos += player_vel * tick_dt

    // Clamp to river
    let bounds = river_clamp_x(player_pos.y)
    let margin = PLAYER_SIZE * 1.5
    let bank_hit = player_pos.x < bounds.x + margin || player_pos.x > bounds.y - margin
    player_pos.x = clamp(player_pos.x, bounds.x + margin, bounds.y - margin)
    player_pos.z = PLAYER_Z

    if (bank_hit && player_invuln_timer <= 0.0 && player_respawn_timer <= 0.0) {
        on_player_killed()
    }

    // Fuel
    if (!god_mode) {
        player_fuel = max(player_fuel - PLAYER_FUEL_DRAIN * tick_dt, 0.0)
    }
    if (player_fuel <= 0.0 && player_respawn_timer <= 0.0) {
        on_player_killed()
    }
}

def on_player_killed() {
    if (god_mode) {
        player_invuln_timer = max(player_invuln_timer, PLAYER_INVULN_TIME)
        player_fuel = PLAYER_FUEL_MAX
        return
    }
    player_lives--
    spawn_explosion(player_pos, float3(1.0, 0.55, 0.18), PLAYER_SIZE * 2.6, 2.2)
    play_sfx(snd_player_hit)
    add_screen_shake(SHAKE_MED)
    if (player_lives > 0) {
        slow_mo_timer = SLOW_MO_DURATION
    }
    player_respawn_timer = PLAYER_RESPAWN_HIDE + PLAYER_RESPAWN_BLINK
    player_invuln_timer = player_respawn_timer + PLAYER_INVULN_TIME
    player_fuel = PLAYER_FUEL_MAX * 0.7
    player_pos = find_safe_respawn_pos(player_pos)
    player_vel.x = 0.0
    if (player_lives <= 0) {
        play_sfx(snd_game_over)
        game_state = GameState.game_over_state
        restart_input_lock = 1.5
    }
}

def update_player(var _esc_just : bool) {
    if (game_state != GameState.playing) {
        return
    }

    player_respawn_timer = max(player_respawn_timer - tick_dt, 0.0)
    player_invuln_timer = max(player_invuln_timer - tick_dt, 0.0)
    player_multishot_timer = max(player_multishot_timer - tick_dt, 0.0)
    player_fastshot_timer = max(player_fastshot_timer - tick_dt, 0.0)

    // Low fuel beep
    if (player_fuel < LOW_FUEL_THRESHOLD) {
        low_fuel_beep_timer = max(low_fuel_beep_timer - tick_dt, 0.0)
        if (low_fuel_beep_timer <= 0.0) {
            play_sfx(snd_low_fuel_beep)
            low_fuel_beep_timer = 0.5
        }
    }

    if (player_respawn_timer > PLAYER_RESPAWN_BLINK) {
        // Hidden/respawning — still scroll forward but no input
        player_pos.y += player_fwd_speed * tick_dt
        return
    }

    update_player_input()
    update_player_shoot()
    update_player_movement()
}

// --- Section Management ---

def update_section() {
    if (game_state != GameState.playing) {
        return
    }

    section_dist += player_fwd_speed * tick_dt
    section_banner_timer = max(section_banner_timer - tick_dt, 0.0)
    if (section_color_blend_t < 1.0) {
        // The whole lighting environment crossfades, not just the water tint,
        // so a section change reads as flying into different weather.
        section_color_blend_t = min(section_color_blend_t + tick_dt * 0.35, 1.0)
        refresh_env()
    }

    if (section_dist >= SECTION_LENGTH) {
        section_dist -= SECTION_LENGTH
        current_section++
        section_banner_timer = WAVE_BANNER_DURATION
        play_sfx(snd_section_clear)
        score += 500 * (current_section)
        begin_env_transition(section_idx())

        if (current_section >= MAX_SECTIONS) {
            game_state = GameState.win_state
            restart_input_lock = 1.5
        } else {
            spawn_section_objects()
        }
    }
}

// --- Enemy Updates ---

def enemy_shoot_at_player(from : float3; btype : int = 0) {
    let dir = normalize(player_pos - from)
    let spd = (btype == 1 ? ENEMY_HELI_BULLET_SPEED : ENEMY_BULLET_SPEED)
    create_entity() @(eid, cmp) {
        apply_decs_template(cmp, EnemyBullet(
            pos = from + dir * 0.8,
            vel = dir * spd,
            age = 0.0,
            bullet_type = btype
        ))
    }
}

def update_enemies() {
    let sm = section_speed_mult()
    update_enemy_boats(sm)
    update_enemy_planes(sm)
    update_enemy_helis(sm)
    update_bullets()
    age_out_particles()
}

// Boats: patrol horizontally, shoot at player
def update_enemy_boats(sm : float) {
    query() $(eid : EntityId; var b : EnemyBoat) {
        // Scroll with world — boats stay at their world Y, player moves to them
        b.shoot_timer -= tick_dt
        // Patrol
        let bounds = river_clamp_x_for(b.pos.y, b.lane_prefer_x)
        let lane_width = max(bounds.y - bounds.x, 0.01)
        let margin = clamp(lane_width * 0.5 - 0.08, 0.05, BOAT_SIZE * 2.0)
        let lane_min = bounds.x + margin
        let lane_max = bounds.y - margin
        b.vel.x = b.patrol_dir * BOAT_PATROL_SPEED * sm
        b.pos.x += b.vel.x * tick_dt
        if (lane_min >= lane_max) {
            b.patrol_dir = -b.patrol_dir
            b.pos.x = (bounds.x + bounds.y) * 0.5
        } elif (b.pos.x <= lane_min || b.pos.x >= lane_max) {
            b.patrol_dir = -b.patrol_dir
            b.pos.x = clamp(b.pos.x, lane_min, lane_max)
        }
        // Avoid islands — reverse patrol direction and back up if overlapping
        find_query() $(isle : Island) {
            let r = isle.size + BOAT_SIZE + 0.3
            let dx = b.pos.x - isle.pos.x
            let dy = b.pos.y - isle.pos.y
            if (dx * dx + dy * dy < r * r) {
                b.patrol_dir = -b.patrol_dir
                b.pos.x -= b.vel.x * tick_dt * 2.0
                return true
            }
            return false
        }
        // Avoid fuel depots (platforms) so ships don't clip through them
        find_query() $(d : FuelDepot) {
            let r = FUEL_DEPOT_SIZE + BOAT_SIZE + 0.35
            let dx = b.pos.x - d.pos.x
            let dy = b.pos.y - d.pos.y
            if (dx * dx + dy * dy < r * r) {
                b.patrol_dir = -b.patrol_dir
                b.pos.x += (dx >= 0.0 ? 1.0 : -1.0) * tick_dt * BOAT_PATROL_SPEED * sm * 2.5
                return true
            }
            return false
        }
        // Shoot when in range
        if (b.shoot_timer <= 0.0 && abs(b.pos.y - player_pos.y) < 20.0) {
            enemy_shoot_at_player(b.pos, 0)
            b.shoot_timer = BOAT_SHOOT_INTERVAL / sm
        }
    }
    commit()
}

// Planes: fly sideways, shoot when passing player Y
def update_enemy_planes(sm : float) {
    query() $(eid : EntityId; var p : EnemyPlane) {
        p.pos += p.vel * tick_dt
        p.shoot_timer -= tick_dt
        // Bounce off banks + terrain
        let bounds = river_clamp_x_for(p.pos.y, p.pos.x)
        if (p.pos.x < bounds.x - 4.0 || p.pos.x > bounds.y + 4.0) {
            p.vel.x = -p.vel.x
            p.pos.x = clamp(p.pos.x, bounds.x - 4.0, bounds.y + 4.0)
        }
        if (p.shoot_timer <= 0.0 && abs(p.pos.y - player_pos.y) < 15.0) {
            enemy_shoot_at_player(p.pos, 2)
            p.shoot_timer = PLANE_SHOOT_INTERVAL / sm
        }
    }
    commit()
}

// Helicopters: hover and orbit player
def update_enemy_helis(sm : float) {
    query() $(eid : EntityId; var h : EnemyHelicopter) {
        h.hover_phase += tick_dt * 1.8
        let hover_z = ENEMY_HELI_Z + sin(h.hover_phase) * 0.4

        // Slowly track player X
        let tx = player_pos.x + sin(h.hover_phase * 0.5) * 2.5
        h.pos.x += (tx - h.pos.x) * tick_dt * ENEMY_HELI_SPEED
        h.pos.z = hover_z

        h.shoot_timer -= tick_dt
        if (h.shoot_timer <= 0.0 && abs(h.pos.y - player_pos.y) < ENEMY_HELI_RANGE) {
            enemy_shoot_at_player(h.pos, 1)
            h.shoot_timer = ENEMY_HELI_SHOOT_INTERVAL_FAST / sm
        }
    }
    commit()
}

// Move player bullets; delete on expiry or island collision
def update_bullets() {
    var inscope pb_island_hits : array<tuple<EntityId; float3>>
    query() $(eid : EntityId; var b : PlayerBullet) {
        b.age += tick_dt
        b.pos += b.vel * tick_dt
        spawn_trail(b.pos, float3(0.15, 0.85, 0.8))
        if (b.age > BULLET_LIFETIME) {
            delete_entity(eid)
        } else {
            find_query() $(isle : Island) {
                let r = isle.size + BULLET_SIZE
                let dx = b.pos.x - isle.pos.x
                let dy = b.pos.y - isle.pos.y
                if (dx * dx + dy * dy < r * r) {
                    pb_island_hits |> push(eid => b.pos)
                    return true
                }
                return false
            }
        }
    }
    commit()
    for (h in pb_island_hits) {
        delete_entity(h._0)
        spawn_particles(h._1, float3(1.0, 1.0, 0.6), 0.2, 4)
    }
    commit()

    query() $(eid : EntityId; var b : EnemyBullet) {
        b.age += tick_dt
        b.pos += b.vel * tick_dt
        let tc = (b.bullet_type == 1 ? float3(0.9, 0.1, 0.1) : (b.bullet_type == 2 ? float3(0.9, 0.55, 0.1) : float3(0.8, 0.6, 0.2)))
        spawn_trail(b.pos, tc)
        if (b.age > ENEMY_BULLET_LIFETIME) {
            delete_entity(eid)
        }
    }
    commit()
}

// Age out particles
def step_particles(dt : float) {
    query() $(eid : EntityId; var p : Particle) {
        p.lifetime -= dt
        // Drag and gravity ride on the particle, so one integrator covers
        // rising smoke, arcing embers and tumbling debris. A negative gravity
        // is buoyancy: that is how the hot layers climb.
        if (p.drag > 0.0) {
            p.vel *= max(1.0 - p.drag * dt, 0.0)
        }
        if (p.gravity != 0.0) {
            p.vel.z -= p.gravity * dt
        }
        p.pos += p.vel * dt
        // Debris that reaches the water stops there and skids out.
        if (p.kind == ParticleKind.debris && p.pos.z < 0.05) {
            p.pos.z = 0.05
            p.vel = float3(p.vel.x * 0.4, p.vel.y * 0.4, 0.0)
        }
        if (p.lifetime <= 0.0) {
            delete_entity(eid)
        }
    }
    commit()
}

def age_out_particles() {
    if (fx_freeze) {
        return
    }
    step_particles(tick_dt)
}

// --- Culling: remove entities far behind player ---

def cull_far_entities() {
    let cull_y = player_pos.y - 20.0
    var inscope to_del : array<EntityId>
    query() $(eid : EntityId; b : EnemyBoat) {
        if (b.pos.y < cull_y) { to_del |> push(eid); }
    }
    query() $(eid : EntityId; p : EnemyPlane) {
        if (p.pos.y < cull_y) { to_del |> push(eid); }
    }
    query() $(eid : EntityId; h : EnemyHelicopter) {
        if (h.pos.y < cull_y) { to_del |> push(eid); }
    }
    query() $(eid : EntityId; b : Bridge) {
        if (b.pos.y < cull_y) { to_del |> push(eid); }
    }
    query() $(eid : EntityId; isle : Island) {
        if (isle.pos.y < cull_y) { to_del |> push(eid); }
    }
    query() $(eid : EntityId; d : FuelDepot) {
        if (d.pos.y < cull_y) { to_del |> push(eid); }
    }
    query() $(eid : EntityId; t : RiverTree) {
        if (t.pos.y < cull_y) { to_del |> push(eid); }
    }
    query() $(eid : EntityId; h : RiverHouse) {
        if (h.pos.y < cull_y) { to_del |> push(eid); }
    }
    query() $(eid : EntityId; b : BonusPickup) {
        if (b.pos.y < cull_y) { to_del |> push(eid); }
    }
    for (eid in to_del) {
        delete_entity(eid)
    }
    commit()
}

// --- Collision Detection ---

def dist2d_sq(a, b : float3) : float {
    let dx = a.x - b.x
    let dy = a.y - b.y
    return dx * dx + dy * dy
}

def process_collisions() {
    if (game_state != GameState.playing || player_respawn_timer > PLAYER_RESPAWN_BLINK) {
        return
    }
    var inscope enemy_hits <- collect_bullet_hits()
    resolve_enemy_hits(enemy_hits)
    enemy_bullets_vs_player()
    player_vs_fuel_depots()
    player_vs_bonuses()
    player_vs_islands()
    player_vs_bridges()
}

// Player bullets vs enemies — hits are collected first because a decs query
// may not mutate the entity set it is walking.
def collect_bullet_hits() : array<EnemyHitInfo> {   // nolint:STYLE037,STYLE038 - one find_query arm per enemy kind, in hit-priority order
    var enemy_hits : array<EnemyHitInfo>
    query() $(eid aka b_eid : EntityId; pb : PlayerBullet) {
        var bullet_hit = false
        // vs boats
        find_query() $(eid : EntityId; b : EnemyBoat) {
            let r = BOAT_SIZE + BULLET_SIZE
            if (dist2d_sq(pb.pos, b.pos) < r * r) {
                enemy_hits |> push(EnemyHitInfo(
                    bullet_eid = b_eid,
                    enemy_eid = eid,
                    pos = b.pos,
                    enemy_kind = EnemyKind.boat,
                    score_value = 100,
                    blast_radius = BOAT_SIZE
                ))
                bullet_hit = true
                return true
            }
            return false
        }
        // vs planes
        if (!bullet_hit) {
            find_query() $(eid : EntityId; p : EnemyPlane) {
                let r = PLANE_SIZE + BULLET_SIZE
                if (dist2d_sq(pb.pos, p.pos) < r * r) {
                    enemy_hits |> push(EnemyHitInfo(
                        bullet_eid = b_eid,
                        enemy_eid = eid,
                        pos = p.pos,
                        enemy_kind = EnemyKind.plane,
                        score_value = 150,
                        blast_radius = PLANE_SIZE
                    ))
                    bullet_hit = true
                    return true
                }
                return false
            }
        }
        // vs enemy helis
        if (!bullet_hit) {
            find_query() $(eid : EntityId; h : EnemyHelicopter) {
                let r = ENEMY_HELI_SIZE + BULLET_SIZE
                if (dist2d_sq(pb.pos, h.pos) < r * r) {
                    enemy_hits |> push(EnemyHitInfo(
                        bullet_eid = b_eid,
                        enemy_eid = eid,
                        pos = h.pos,
                        enemy_kind = EnemyKind.helicopter,
                        score_value = 200,
                        blast_radius = ENEMY_HELI_SIZE
                    ))
                    bullet_hit = true
                    return true
                }
                return false
            }
        }
        // vs bridges
        if (!bullet_hit) {
            find_query() $(eid : EntityId; br : Bridge) {
                if (abs(pb.pos.y - br.pos.y) < 0.8) {
                    let gap_l = br.gap_center_x - BRIDGE_GAP_WIDTH * 0.5
                    let gap_r = br.gap_center_x + BRIDGE_GAP_WIDTH * 0.5
                    let in_left_span = pb.pos.x >= br.left_x - BULLET_SIZE && pb.pos.x <= gap_l + BULLET_SIZE
                    let in_right_span = pb.pos.x >= gap_r - BULLET_SIZE && pb.pos.x <= br.right_x + BULLET_SIZE
                    if (in_left_span || in_right_span) {
                        enemy_hits |> push(EnemyHitInfo(
                            bullet_eid = b_eid,
                            enemy_eid = eid,
                            pos = br.pos,
                            enemy_kind = EnemyKind.bridge,
                            score_value = 50,
                            blast_radius = (br.right_x - br.left_x) * 0.5
                        ))
                        bullet_hit = true
                        return true
                    }
                }
                return false
            }
        }
        // vs fuel depots — one-shot explodable
        if (!bullet_hit) {
            find_query() $(eid : EntityId; d : FuelDepot) {
                let r = FUEL_DEPOT_SIZE + BULLET_SIZE
                if (dist2d_sq(pb.pos, d.pos) < r * r) {
                    enemy_hits |> push(EnemyHitInfo(
                        bullet_eid = b_eid,
                        enemy_eid = eid,
                        pos = d.pos,
                        enemy_kind = EnemyKind.depot,
                        score_value = 75,
                        blast_radius = FUEL_DEPOT_SIZE
                    ))
                    return true
                }
                return false
            }
        }
    }
    return <- enemy_hits
}

def resolve_enemy_hits(enemy_hits : array<EnemyHitInfo>) {
    var inscope killed_this_tick : array<EntityId>
    for (h in enemy_hits) {
        delete_entity(h.bullet_eid)
        var already_killed = false
        for (eid in killed_this_tick) {
            if (eid == h.enemy_eid) {
                already_killed = true
                break
            }
        }
        if (already_killed) {
            continue
        }
        spawn_particles(h.pos, float3(1.0, 0.95, 0.5), max(0.15, h.blast_radius * 0.2), 4)
        var is_depot = false
        if (damage_enemy(h, is_depot)) {
            delete_entity(h.enemy_eid)
            killed_this_tick |> push(h.enemy_eid)
            on_enemy_destroyed(h, is_depot)
        }
    }
    commit()
}

// Applies one hit to whichever enemy kind owns h.enemy_eid; true once its health runs out.
def damage_enemy(h : EnemyHitInfo; var is_depot : bool&) : bool {   // nolint:STYLE037 - one find_query arm per enemy kind
    var killed = false
    find_query() $(eid : EntityId; var b : EnemyBoat) {
        if (eid == h.enemy_eid) {
            if (b.health > 0) {
                b.health--
                if (b.health <= 0) { killed = true; }
            }
            return true
        }
        return false
    }
    find_query() $(eid : EntityId; var p : EnemyPlane) {
        if (eid == h.enemy_eid) {
            if (p.health > 0) {
                p.health--
                if (p.health <= 0) { killed = true; }
            }
            return true
        }
        return false
    }
    find_query() $(eid : EntityId; var hh : EnemyHelicopter) {
        if (eid == h.enemy_eid) {
            if (hh.health > 0) {
                hh.health--
                if (hh.health <= 0) { killed = true; }
            }
            return true
        }
        return false
    }
    // Bridges now take multiple hits.
    find_query() $(eid : EntityId; var br : Bridge) {
        if (eid == h.enemy_eid) {
            if (br.health > 0) {
                br.health--
                if (br.health <= 0) {
                    killed = true
                }
            }
            return true
        }
        return false
    }
    // Fuel depots — always one-shot
    find_query() $(eid : EntityId; d : FuelDepot) {
        if (eid == h.enemy_eid) {
            killed = true
            is_depot = true
            return true
        }
        return false
    }
    return killed
}

// Death effects and loot for an enemy whose entity has just been deleted.
def on_enemy_destroyed(h : EnemyHitInfo; is_depot : bool) {
    var is_bridge = false
    find_query() $(eid : EntityId; br : Bridge) {
        if (eid == h.enemy_eid) { is_bridge = true; return true; }
        return false
    }
    if (is_depot) {
        // A depot goes up twice: the tank at deck level, then a second bloom
        // above it a beat later.
        spawn_explosion(h.pos, float3(1.0, 0.52, 0.06), h.blast_radius * 1.2, 1.8)
        spawn_explosion(h.pos + float3(0.0, 0.0, 1.1), float3(0.95, 0.28, 0.02), h.blast_radius * 0.8, 1.2)
    } else {
        spawn_explosion(h.pos, float3(1.0, 0.66, 0.12), h.blast_radius, (is_bridge ? 1.6 : 1.0))
    }
    play_sfx(is_bridge ? snd_bridge_destroy : snd_enemy_explode)
    if (is_bridge) {
        add_screen_shake(SHAKE_BIG)
    } elif (is_depot) {
        add_screen_shake(SHAKE_MED)
    } else {
        add_screen_shake(SHAKE_SMALL)
    }
    score += h.score_value

    let drop_chance = (
        h.enemy_kind == EnemyKind.helicopter ? 0.75
            : (h.enemy_kind == EnemyKind.plane ? 0.55
                : (h.enemy_kind == EnemyKind.boat ? BONUS_DROP_CHANCE : 0.0))
    )
    if (drop_chance > 0.0 && random_f() < drop_chance) {
        let r = random_f()
        let bt = (
            r < 0.3
                ? BonusType.fuel
                : (r < 0.6 ? BonusType.multishot : (r < 0.9 ? BonusType.fastshot : BonusType.life))
        )
        let drop_bounds = river_clamp_x(h.pos.y)
        let drop_x = clamp(h.pos.x, drop_bounds.x + BONUS_PICKUP_RADIUS, drop_bounds.y - BONUS_PICKUP_RADIUS)
        spawn_bonus_pickup(float3(drop_x, h.pos.y, 0.42), bt)
    }
}

// Enemy bullets vs player
def enemy_bullets_vs_player() {
    if (player_invuln_timer <= 0.0) {
        var player_hit = false
        query() $(eid : EntityId; eb : EnemyBullet) {
            if (!player_hit) {
                let r = PLAYER_SIZE + ENEMY_BULLET_SIZE
                if (dist2d_sq(eb.pos, player_pos) < r * r) {
                    player_hit = true
                    delete_entity(eid)
                }
            }
        }
        if (player_hit) {
            on_player_killed()
        }
        commit()
    }
}

// Player vs fuel depots
def player_vs_fuel_depots() {
    query() $(eid : EntityId; d : FuelDepot) {
        let r = FUEL_DEPOT_SIZE + PLAYER_SIZE + REFUEL_RADIUS
        if (dist2d_sq(d.pos, player_pos) < r * r && player_fuel < PLAYER_FUEL_MAX) {
            player_fuel = min(player_fuel + REFUEL_RATE * tick_dt, PLAYER_FUEL_MAX)
            refuel_sound_timer -= tick_dt
            if (refuel_sound_timer <= 0.0) {
                play_sfx(snd_refuel)
                refuel_sound_timer = 0.7
            }
        }
    }
}

// Player vs bonuses — collect burst origins/colors and spawn AFTER the
// query (create_entities can't run inside a decs query).
def player_vs_bonuses() {
    var burst_origins : array<float3>
    var burst_colors : array<float3>
    query() $(eid : EntityId; b : BonusPickup) {
        let r = PLAYER_SIZE + BONUS_PICKUP_RADIUS
        if (dist2d_sq(b.pos, player_pos) < r * r) {
            if (b.bonus_type == BonusType.fuel) {
                player_fuel = min(player_fuel + BONUS_FUEL_AMOUNT, PLAYER_FUEL_MAX)
                play_sfx(snd_bonus_fuel)
            } elif (b.bonus_type == BonusType.multishot) {
                player_multishot_timer = max(player_multishot_timer, BONUS_MULTISHOT_TIME)
                play_sfx(snd_bonus_multishot)
            } elif (b.bonus_type == BonusType.fastshot) {
                player_fastshot_timer = max(player_fastshot_timer, BONUS_FASTSHOT_TIME)
                play_sfx(snd_bonus_fastshot)
            } else {
                player_lives++
                play_sfx(snd_bonus_life)
            }
            burst_origins |> push(b.pos + float3(0.0, 0.0, 0.55))
            burst_colors |> push(bonus_color(b.bonus_type))
            delete_entity(eid)
        }
    }
    for (origin, color in burst_origins, burst_colors) {
        spawn_pickup_burst(origin, color)
    }
    commit()
}

// Player vs islands
def player_vs_islands() {
    if (player_invuln_timer <= 0.0) {
        var island_hit = false
        query() $(isle : Island) {
            if (!island_hit) {
                let r = isle.size + PLAYER_SIZE
                let d2 = (
                    (player_pos.x - isle.pos.x) * (player_pos.x - isle.pos.x) +
                    (player_pos.y - isle.pos.y) * (player_pos.y - isle.pos.y)
                )
                if (d2 < r * r) {
                    island_hit = true
                }
            }
        }
        if (island_hit) {
            on_player_killed()
        }
    }
}

// Player through bridge — must pass through gap
def player_vs_bridges() {
    if (player_invuln_timer <= 0.0) {
        var bridge_hit = false
        query() $(br : Bridge) {
            if (!bridge_hit && abs(player_pos.y - br.pos.y) < 1.0) {
                let in_span = player_pos.x > br.left_x && player_pos.x < br.right_x
                let in_gap = (
                    player_pos.x > br.gap_center_x - BRIDGE_GAP_WIDTH * 0.5 &&
                    player_pos.x < br.gap_center_x + BRIDGE_GAP_WIDTH * 0.5
                )
                if (in_span && !in_gap) {
                    bridge_hit = true
                }
            }
        }
        if (bridge_hit) {
            on_player_killed()
        }
    }
}

// --- Rendering ---

def quat_y_rot(angle : float) : float4 {
    return float4(0.0, sin(angle * 0.5), 0.0, cos(angle * 0.5))
}

def quat_x_rot(angle : float) : float4 {
    return float4(sin(angle * 0.5), 0.0, 0.0, cos(angle * 0.5))
}

def quat_z_rot(angle : float) : float4 {
    return float4(0.0, 0.0, sin(angle * 0.5), cos(angle * 0.5))
}

// Rotate unit Z to direction v (for aligning cylinders along a velocity vector)
def quat_align_z(v : float3) : float4 {
    let from = float3(0.0, 0.0, 1.0)
    let d = dot(from, v)
    if (d > 0.9999) { return float4(0.0, 0.0, 0.0, 1.0); }
    if (d < -0.9999) { return float4(1.0, 0.0, 0.0, 0.0); }
    let axis = normalize(cross(from, v))
    let half = acos(d) * 0.5
    return float4(axis * sin(half), cos(half))
}

// --- Model palettes ---
//
// Enemy factions share the player's meshes under different paint, which keeps
// the vocabulary readable: silhouette says what it is, colour says whose it is.

def private player_palette(inv : bool; pulse : float) {
    let base = (inv ? float3(1.0, 0.86, 0.35) : float3(0.72, 0.80, 0.88))
    set_model_palette(
        base * pulse,
        float3(0.16, 0.30, 0.38),
        float3(0.52, 0.56, 0.60),
        float3(0.10, 0.12, 0.14)
    )
}

def private enemy_heli_palette() {
    set_model_palette(
        float3(0.52, 0.14, 0.10),
        float3(0.14, 0.10, 0.10),
        float3(0.40, 0.38, 0.36),
        float3(0.08, 0.07, 0.07)
    )
}

def private boat_palette() {
    set_model_palette(
        float3(0.28, 0.30, 0.27),
        float3(0.46, 0.42, 0.30),
        float3(0.48, 0.47, 0.45),
        float3(0.09, 0.10, 0.11)
    )
}

def private jet_palette() {
    set_model_palette(
        float3(0.40, 0.42, 0.47),
        float3(0.14, 0.18, 0.22),
        float3(0.55, 0.55, 0.58),
        float3(0.08, 0.08, 0.09)
    )
}

// Craft bank into a turn. Purely cosmetic, but it is most of what makes the
// helicopter feel like it is being flown rather than slid.
def private player_orientation() : float4 {
    let roll = clamp(-player_vel.x * 0.075, -0.55, 0.55)
    let pitch = clamp((player_fwd_speed - PLAYER_FWD_SPEED_MIN) * 0.018, 0.0, 0.22)
    return quat_mul(quat_y_rot(roll), quat_x_rot(-pitch))
}

// --- Shadow map pass ---
//
// Everything solid re-draws into the light's depth buffer. The old fake blob
// shadows are gone: props now shade each other and the banks.

// Aircraft and boats: the movers, whose shadows track the action.
def private shadow_cast_craft() {
    if (player_respawn_timer <= PLAYER_RESPAWN_BLINK) {
        draw_shadow_caster(player_pos, float3(PLAYER_SIZE * 1.85), player_orientation())
        geo_heli |> draw_geometry_fragment()
    }

    query() $(b : EnemyBoat) {
        draw_shadow_caster(b.pos, float3(BOAT_SIZE * 1.25), quat_z_rot(b.patrol_dir * 0.12))
        geo_gunboat |> draw_geometry_fragment()
    }

    query() $(p : EnemyPlane) {
        draw_shadow_caster(p.pos, float3(PLANE_SIZE * 1.35))
        geo_jet |> draw_geometry_fragment()
    }

    query() $(h : EnemyHelicopter) {
        draw_shadow_caster(h.pos, float3(ENEMY_HELI_SIZE * 1.7), quat_y_rot(sin(h.hover_phase) * 0.2))
        geo_heli |> draw_geometry_fragment()
    }

}

// Bank scenery: trees and houses, which throw the long streaks across the
// ground that give the low sun its scale.
def private shadow_cast_scenery() {
    let per_kind = max(length(geo_flora) / FLORA_VARIANTS, 1)
    query() $(t : RiverTree) {
        if (empty(geo_flora)) { return ; }
        draw_shadow_caster(t.pos, float3(t.size), quat_z_rot(t.yaw))
        let idx = clamp(t.kind, 0, FLORA_VARIANTS - 1) * per_kind + (t.seed % per_kind)
        geo_flora[clamp(idx, 0, length(geo_flora) - 1)] |> draw_geometry_fragment()
    }

    query() $(h : RiverHouse) {
        let body_h = h.size
        draw_shadow_caster(h.pos + float3(0.0, 0.0, body_h * 0.5),
            float3(h.size * 0.95, h.size * 1.1, body_h * 0.5))
        geo_cube |> draw_geometry_fragment()
        draw_shadow_caster(h.pos + float3(0.0, 0.0, body_h + h.size * 0.25),
            float3(h.size * 1.05, h.size * 1.18, h.size * 0.55))
        geo_prism |> draw_geometry_fragment()
    }

}

// Obstacles standing in the water.
def private shadow_cast_obstacles() {
    query() $(isle : Island) {
        draw_shadow_caster(isle.pos, float3(isle.size, isle.size, isle.size * 0.35))
        geo_sphere |> draw_geometry_fragment()
    }

    query() $(d : FuelDepot) {
        draw_shadow_caster(d.pos, float3(FUEL_DEPOT_SIZE, FUEL_DEPOT_SIZE * 1.8, FUEL_DEPOT_SIZE * 0.25))
        geo_cube |> draw_geometry_fragment()
        draw_shadow_caster(d.pos + float3(0.0, 0.0, FUEL_DEPOT_SIZE * 0.9),
            float3(0.14, 0.14, FUEL_DEPOT_SIZE * 0.9))
        geo_cube |> draw_geometry_fragment()
    }

    query() $(br : Bridge) {
        let pylon_h = BRIDGE_SPAN_Z + 0.5
        for (px in fixed_array(br.left_x, br.right_x)) {
            draw_shadow_caster(float3(px, br.pos.y, pylon_h * 0.5), float3(0.4, 0.3, pylon_h))
            geo_cube |> draw_geometry_fragment()
        }
        let gap_left = br.gap_center_x - BRIDGE_GAP_WIDTH * 0.5
        let gap_right = br.gap_center_x + BRIDGE_GAP_WIDTH * 0.5
        let lw = gap_left - br.left_x
        if (lw > 0.1) {
            draw_shadow_caster(float3((br.left_x + gap_left) * 0.5, br.pos.y, BRIDGE_SPAN_Z),
                float3(lw * 0.5, 0.35, 0.25))
            geo_cube |> draw_geometry_fragment()
        }
        let rw = br.right_x - gap_right
        if (rw > 0.1) {
            draw_shadow_caster(float3((gap_right + br.right_x) * 0.5, br.pos.y, BRIDGE_SPAN_Z),
                float3(rw * 0.5, 0.35, 0.25))
            geo_cube |> draw_geometry_fragment()
        }
    }
}

def render_shadow_casters() {
    glUseProgram(shadow_program)
    active_program = shadow_program
    render_river_shadow()
    shadow_cast_craft()
    shadow_cast_scenery()
    shadow_cast_obstacles()
}

// --- Opaque scene ---

def render_player(game_time : float) {
    if (player_respawn_timer > PLAYER_RESPAWN_BLINK) {
        return
    }
    let blinking = player_respawn_timer > 0.0
    if (blinking && int(game_time * 16.0) % 2 == 0) {
        return
    }
    let inv = player_invuln_timer > 0.0
    let pulse = (inv ? 0.75 + 0.35 * abs(sin(game_time * 8.0)) : 1.0)

    use_prop_program()
    player_palette(inv, pulse)
    draw_prop(player_pos, float3(PLAYER_SIZE * 1.85), float3(1.0), player_orientation(), MAT_HULL)
    geo_heli |> draw_geometry_fragment()
    clear_model_palette()
}

def render_enemies(game_time : float) {
    use_prop_program()

    query() $(b : EnemyBoat) {
        boat_palette()
        // Heel the hull into its patrol direction and let it ride the swell.
        let heel = quat_y_rot(b.patrol_dir * 0.14)
        let bob = quat_x_rot(sin(game_time * 1.7 + b.pos.x) * 0.05)
        draw_prop(b.pos, float3(BOAT_SIZE * 1.25), float3(1.0), quat_mul(heel, bob), MAT_HULL)
        geo_gunboat |> draw_geometry_fragment()
    }

    query() $(p : EnemyPlane) {
        jet_palette()
        let bank = quat_y_rot(clamp(p.vel.x * 0.10, -0.5, 0.5))
        draw_prop(p.pos, float3(PLANE_SIZE * 1.35), float3(1.0), bank, MAT_METAL)
        geo_jet |> draw_geometry_fragment()
    }

    query() $(h : EnemyHelicopter) {
        enemy_heli_palette()
        let drift = quat_y_rot(clamp((h.target_x - h.pos.x) * 0.06, -0.45, 0.45))
        draw_prop(h.pos, float3(ENEMY_HELI_SIZE * 1.7), float3(1.0), drift, MAT_HULL)
        geo_heli |> draw_geometry_fragment()
    }

    clear_model_palette()
}

def render_obstacles(game_time : float) {
    use_prop_program()
    render_bridges()
    render_islands()
    render_fuel_depots(game_time)
    render_trees()
    render_houses()
    render_bonuses(game_time)
}

// Bridges: two pylons + two half-spans, with a deck lip so the gap reads.
def render_bridges() {
    let pylon_color = float3(0.42, 0.40, 0.37)
    let span_color = float3(0.50, 0.47, 0.43)
    query() $(br : Bridge) {
        let pylon_h = BRIDGE_SPAN_Z + 0.5
        for (px in fixed_array(br.left_x, br.right_x)) {
            draw_prop(float3(px, br.pos.y, pylon_h * 0.5), float3(0.4, 0.3, pylon_h),
                pylon_color, float4(0.0, 0.0, 0.0, 1.0), MAT_STONE, REFLECT_PAINT)
            geo_cube |> draw_geometry_fragment()
        }

        let gap_left = br.gap_center_x - BRIDGE_GAP_WIDTH * 0.5
        let gap_right = br.gap_center_x + BRIDGE_GAP_WIDTH * 0.5
        // Damage reads as the deck darkening and losing its highlight.
        let wear = saturate(float(br.health) / float(BRIDGE_HEALTH_MAX))
        let deck = span_color * (0.55 + wear * 0.45)

        let lw = gap_left - br.left_x
        if (lw > 0.1) {
            let cx = (br.left_x + gap_left) * 0.5
            draw_prop(float3(cx, br.pos.y, BRIDGE_SPAN_Z), float3(lw * 0.5, 0.35, 0.25),
                deck, float4(0.0, 0.0, 0.0, 1.0), MAT_STONE)
            geo_cube |> draw_geometry_fragment()
            draw_prop(float3(cx, br.pos.y - 0.36, BRIDGE_SPAN_Z + 0.22),
                float3(lw * 0.5, 0.05, 0.12), deck * 1.2, float4(0.0, 0.0, 0.0, 1.0), MAT_STONE)
            geo_cube |> draw_geometry_fragment()
        }
        let rw = br.right_x - gap_right
        if (rw > 0.1) {
            let cx = (gap_right + br.right_x) * 0.5
            draw_prop(float3(cx, br.pos.y, BRIDGE_SPAN_Z), float3(rw * 0.5, 0.35, 0.25),
                deck, float4(0.0, 0.0, 0.0, 1.0), MAT_STONE)
            geo_cube |> draw_geometry_fragment()
            draw_prop(float3(cx, br.pos.y - 0.36, BRIDGE_SPAN_Z + 0.22),
                float3(rw * 0.5, 0.05, 0.12), deck * 1.2, float4(0.0, 0.0, 0.0, 1.0), MAT_STONE)
            geo_cube |> draw_geometry_fragment()
        }
    }
}

// Islands: a flattened dome, tinted with the section's bank colour so a rock in
// the ash channel is not the same green as one in the forest.
def render_islands() {
    query() $(isle : Island) {
        let island_color = get_bank_color() * 1.15 + float3(0.04)
        draw_prop(isle.pos, float3(isle.size, isle.size, isle.size * 0.35), island_color,
            float4(0.0, 0.0, 0.0, 1.0), MAT_STONE)
        geo_sphere |> draw_geometry_fragment()
    }
}

// Fuel depots: barge, tank, and a beacon that goes green while refuelling.
def render_fuel_depots(game_time : float) {
    query() $(d : FuelDepot) {
        let rr = FUEL_DEPOT_SIZE + PLAYER_SIZE + REFUEL_RADIUS
        let refueling = dist2d_sq(d.pos, player_pos) < rr * rr
        let beacon_pulse = 0.55 + 0.45 * abs(sin(game_time * 3.5))

        draw_prop(d.pos, float3(FUEL_DEPOT_SIZE, FUEL_DEPOT_SIZE * 1.8, FUEL_DEPOT_SIZE * 0.25),
            float3(0.30, 0.31, 0.33), float4(0.0, 0.0, 0.0, 1.0), MAT_METAL, REFLECT_PAINT)
        geo_cube |> draw_geometry_fragment()

        // Storage tank lying on the deck; the cylinder gives the depot a
        // silhouette you can pick out of the bank clutter at range, and the
        // chrome reflection is what makes it read as a pressure vessel rather
        // than a painted drum.
        draw_prop(d.pos + float3(0.0, 0.0, FUEL_DEPOT_SIZE * 0.45),
            float3(FUEL_DEPOT_SIZE * 0.42, FUEL_DEPOT_SIZE * 0.42, FUEL_DEPOT_SIZE * 0.95),
            float3(0.70, 0.66, 0.30), quat_x_rot(PI * 0.5), MAT_METAL, REFLECT_CHROME)
        geo_cylinder |> draw_geometry_fragment()

        let beacon_color = (
            refueling ? float3(0.25, 1.0, 0.35) : float3(1.0, 0.72, 0.18)
        ) * beacon_pulse
        draw_prop(d.pos + float3(0.0, FUEL_DEPOT_SIZE * 0.9, FUEL_DEPOT_SIZE * 0.9),
            float3(0.09, 0.09, FUEL_DEPOT_SIZE * 0.55), float3(0.22, 0.22, 0.24),
            float4(0.0, 0.0, 0.0, 1.0), MAT_METAL, REFLECT_METAL)
        geo_cube |> draw_geometry_fragment()
        draw_prop(d.pos + float3(0.0, FUEL_DEPOT_SIZE * 0.9, FUEL_DEPOT_SIZE * 1.5),
            float3(0.16), beacon_color, float4(0.0, 0.0, 0.0, 1.0), MAT_GLOW)
        geo_sphere |> draw_geometry_fragment()
    }
}

// Bank flora. One welded mesh per prop, tinted through the model palette, so a
// dressed bank costs one draw call per plant instead of one per cone.
// Takes fields, not the template: a DECS query variable can only be accessed
// by field, never passed whole.
def private flora_palette(kind : int; green_tint, green_shift : float; tint : float3) {
    if (kind == FLORA_ROCK) {
        let stone = lerp(float3(0.34, 0.33, 0.31), get_bank_color() * 1.4, 0.35)
        set_model_palette(stone, stone * 1.12, stone, stone * (0.86 + green_shift))
        return
    }
    if (kind == FLORA_DEAD) {
        let wood = float3(0.26 + green_shift * 0.4, 0.20, 0.14) * tint
        set_model_palette(wood, wood * 1.15, wood, wood * 0.8)
        return
    }
    let dark = float3(0.05, 0.20, 0.07)
    let light = float3(0.15, 0.42, 0.15)
    var green = lerp(dark, light, green_tint)
    if (kind == FLORA_REEDS) {
        green = lerp(green, float3(0.42, 0.44, 0.20), 0.55)
    } elif (kind == FLORA_BUSH) {
        green = lerp(green, float3(0.12, 0.30, 0.10), 0.35)
    }
    green = (green + float3(green_shift * 0.22, green_shift * 0.06, -green_shift * 0.12)) * tint
    let bark = float3(0.22 + green_shift * 0.18, 0.15, 0.09) * tint
    // Two foliage tones plus bark: the accent tone is what gives a crown depth
    // instead of reading as one flat silhouette.
    set_model_palette(green, green * 1.28 + float3(0.02, 0.05, 0.01), green, bark)
}

def render_trees() {
    if (empty(geo_flora)) {
        return
    }
    let tint = env_now.foliage_tint
    let per_kind = length(geo_flora) / FLORA_VARIANTS
    query() $(t : RiverTree) {
        flora_palette(t.kind, t.green_tint, t.green_shift, tint)
        let mat = (t.kind == FLORA_ROCK ? MAT_STONE : MAT_FOLIAGE)
        draw_prop(t.pos, float3(t.size), float3(1.0), quat_z_rot(t.yaw), mat)
        let idx = clamp(t.kind, 0, FLORA_VARIANTS - 1) * per_kind + (t.seed % max(per_kind, 1))
        geo_flora[clamp(idx, 0, length(geo_flora) - 1)] |> draw_geometry_fragment()
    }
    clear_model_palette()
}

// Houses: body + prism roof + a chimney, so the roofline is not a bare wedge.
def render_houses() {
    query() $(h : RiverHouse) {
        let body_color = lerp(float3(0.68, 0.62, 0.53), float3(0.52, 0.58, 0.64), h.body_tint)
        let roof_color = lerp(float3(0.52, 0.19, 0.16), float3(0.20, 0.23, 0.29), h.roof_tint)

        let body_h = h.size
        draw_prop(h.pos + float3(0.0, 0.0, body_h * 0.5),
            float3(h.size * 0.95, h.size * 1.1, body_h * 0.5), body_color,
            float4(0.0, 0.0, 0.0, 1.0), MAT_MATTE)
        geo_cube |> draw_geometry_fragment()

        let roof_h = h.size * 0.55
        draw_prop(h.pos + float3(0.0, 0.0, body_h + roof_h * 0.45),
            float3(h.size * 1.05, h.size * 1.18, roof_h), roof_color,
            float4(0.0, 0.0, 0.0, 1.0), MAT_MATTE)
        geo_prism |> draw_geometry_fragment()

        draw_prop(h.pos + float3(h.size * 0.42, h.size * 0.3, body_h + roof_h * 0.9),
            float3(h.size * 0.12, h.size * 0.12, roof_h * 0.6), roof_color * 0.7,
            float4(0.0, 0.0, 0.0, 1.0), MAT_MATTE)
        geo_cube |> draw_geometry_fragment()
    }
}

// Bonuses.
//
// A pickup has to look worth flying across the channel for, so it is built in
// layers like the explosions are: a machined metal canister that takes the
// environment reflection, a hot emissive core, and -- in the effects pass -- a
// halo ring and a pool of light on the water under it. Each type also gets its
// own silhouette, so what you are collecting is readable before the colour is.

def private bonus_pulse(game_time : float; phase : float) : float {
    return 0.88 + 0.12 * sin(game_time * 4.0 + phase)
}

def private bonus_body_rot(game_time : float; phase : float) : float4 {
    return quat_mul(quat_z_rot(game_time * BONUS_SPIN_SPEED + phase), quat_y_rot(PI * 0.5))
}

def render_bonuses(game_time : float) {
    query() $(b : BonusPickup) {
        let bob = sin(game_time * BONUS_BOB_SPEED + b.bob_phase) * BONUS_BOB_AMPLITUDE
        let p = b.pos + float3(0.0, 0.0, 0.72 + bob)
        let color = bonus_color(b.bonus_type)
        let rot = bonus_body_rot(game_time, b.bob_phase)
        let pulse = bonus_pulse(game_time, b.bob_phase)

        // Machined shell: chrome, so it picks up the sky and the sun rather than
        // sitting flat the way the old painted cylinder did.
        let shell = float3(0.72, 0.76, 0.80)
        draw_prop(p, float3(0.36, 0.36, 0.60) * pulse, shell, rot, MAT_METAL, REFLECT_CHROME)
        geo_cylinder |> draw_geometry_fragment()

        // End caps in the pickup's own colour: the type reads off the ends even
        // when the shell is busy reflecting.
        for (side in fixed_array(-1.0, 1.0)) {
            let cap = p + quat_mul_vec(rot, float3(0.0, 0.0, side * 0.60)) * pulse
            draw_prop(cap, float3(0.40, 0.40, 0.09) * pulse, color * 1.4, rot, MAT_GLOW)
            geo_cylinder |> draw_geometry_fragment()
        }

        // Type badge: a distinct shape bolted to the middle of the shell.
        if (b.bonus_type == BonusType.multishot) {
            // Three barrels.
            for (i in range(3)) {
                let a = game_time * BONUS_SPIN_SPEED + b.bob_phase + float(i) * (2.0 * PI / 3.0)
                let off = float3(cos(a), sin(a), 0.0) * 0.27
                draw_prop(p + off, float3(0.10, 0.10, 0.42) * pulse, color, rot, MAT_METAL, REFLECT_METAL)
                geo_cylinder |> draw_geometry_fragment()
            }
        } elif (b.bonus_type == BonusType.fastshot) {
            // A forward chevron.
            draw_prop(p + float3(0.0, 0.40, 0.0), float3(0.27, 0.27, 0.33) * pulse,
                color, quat_x_rot(-PI * 0.5), MAT_GLOW)
            geo_cone |> draw_geometry_fragment()
        } elif (b.bonus_type == BonusType.life) {
            // A cross.
            draw_prop(p, float3(0.46, 0.13, 0.13) * pulse, color, rot, MAT_GLOW)
            geo_cube |> draw_geometry_fragment()
            draw_prop(p, float3(0.13, 0.13, 0.46) * pulse, color, rot, MAT_GLOW)
            geo_cube |> draw_geometry_fragment()
        } else {
            // Fuel: a banded collar.
            draw_prop(p, float3(0.41, 0.41, 0.16) * pulse, color, rot, MAT_GLOW)
            geo_cylinder |> draw_geometry_fragment()
        }

        // The core, always hot.
        draw_prop(p, float3(0.22) * pulse, color * 2.2, rot, MAT_GLOW)
        geo_sphere |> draw_geometry_fragment()
    }
}

// Additive layers for the pickups: a halo ring that spins on its own axis, and a
// pool of light where the pickup is reflected on the water. Drawn with the
// effects, after the opaque scene.
def render_bonus_fx(game_time : float) {
    // Light pool on the water first -- a soft billboard laid flat.
    use_puff_program()
    query() $(b : BonusPickup) {
        let color = bonus_color(b.bonus_type)
        let pulse = bonus_pulse(game_time, b.bob_phase)
        draw_puff(float3(b.pos.x, b.pos.y, 0.05), 1.7 * pulse, color, 0.38, 1.8,
            game_time * 0.3 + b.bob_phase, b.bob_phase)
        geo_disc |> draw_geometry_fragment()
    }

    glUseProgram(ring_program)
    active_program = ring_program
    query() $(b : BonusPickup) {
        let bob = sin(game_time * BONUS_BOB_SPEED + b.bob_phase) * BONUS_BOB_AMPLITUDE
        let p = b.pos + float3(0.0, 0.0, 0.72 + bob)
        let color = bonus_color(b.bonus_type)
        let pulse = bonus_pulse(game_time, b.bob_phase)
        // Two rings on different axes, counter-rotating: reads as a containment
        // field rather than as a decal.
        let tilt_a = quat_mul(quat_z_rot(game_time * 1.6 + b.bob_phase), quat_x_rot(PI * 0.42))
        let tilt_b = quat_mul(quat_z_rot(-game_time * 1.1 - b.bob_phase), quat_y_rot(PI * 0.5))
        for (rot, radius in fixed_array(tilt_a, tilt_b), fixed_array(0.92, 0.74)) {
            v_model = compose(p, rot, float3(radius * pulse, radius * pulse, 1.0))
            f_Color = float4(color * 1.7, 0.80)
            f_Material = float4(0.13, 0.0, 1.9, 0.0)
            vs_ring_bind_uniform(active_program)
            fs_ring_bind_uniform(active_program)
            geo_disc |> draw_geometry_fragment()
        }
    }
    use_unlit_program()
}

// --- Transparent scene ---

// Explosion layers draw in three passes so they composite correctly: smoke and
// debris alpha-blended first (they occlude), then everything hot additively on
// top, which is also what feeds the bloom.

def private draw_smoke(game_time : float) {
    use_puff_program()
    query() $(p : Particle) {
        if (p.kind != ParticleKind.smoke) { return ; }
        let life = p.lifetime / p.max_life
        // Smoke expands as it ages and fades from the far end of its life, so a
        // puff thins out instead of blinking off.
        let grow = 1.0 + (1.0 - life) * 2.2
        let alpha = saturate(life * 1.5) * 0.5
        let spin = p.spin_phase + p.spin_speed * (1.0 - life) + game_time * 0.08
        draw_puff(p.pos, p.size * grow, p.color, alpha, 0.0, spin, p.spin_phase)
        geo_disc |> draw_geometry_fragment()
    }
    use_unlit_program()
}

def private draw_hot_layers() {
    // Flash and fireballs are soft billboards; only the streaking embers below
    // want real geometry.
    use_puff_program()

    // Flash: one frame-and-a-bit of blowout at the instant of the hit.
    query() $(p : Particle) {
        if (p.kind != ParticleKind.flash) { return ; }
        let life = p.lifetime / p.max_life
        let grow = 0.5 + (1.0 - life) * 0.9
        draw_puff(p.pos, p.size * grow, p.color, life * life * 0.9, 4.5 * life, 0.0, 0.0)
        geo_disc |> draw_geometry_fragment()
    }

    // Fireballs: expand and cool white -> orange -> deep red.
    query() $(p : Particle) {
        if (p.kind != ParticleKind.fire) { return ; }
        let life = p.lifetime / p.max_life
        let age = 1.0 - life
        let grow = 0.6 + age * 1.05
        var tint = lerp(float3(1.0, 0.48, 0.09), float3(1.0, 0.97, 0.86), saturate(life * 2.6 - 1.6))
        tint = lerp(float3(0.34, 0.045, 0.015), tint, saturate(life * 1.9))
        draw_puff(p.pos, p.size * grow, tint, saturate(life * 1.4), 4.2 * life * life,
            p.spin_phase, p.spin_phase * 0.7)
        geo_disc |> draw_geometry_fragment()
    }

    use_unlit_program()

    // Embers: small, bright, and they streak along their own velocity.
    query() $(p : Particle) {
        if (p.kind != ParticleKind.ember && p.kind != ParticleKind.trail) { return ; }
        let life = p.lifetime / p.max_life
        let speed = length(p.vel)
        let tint = lerp(float3(0.9, 0.18, 0.04), p.color, saturate(life * 1.4))
        if (p.kind == ParticleKind.ember && speed > 0.5) {
            let dir = p.vel / speed
            let stretch = clamp(speed * 0.035, 1.0, 4.5)
            draw_unlit(p.pos, float3(p.size, p.size, p.size * stretch), tint,
                saturate(life * 2.0), quat_align_z(dir), 3.4 * life)
            geo_cylinder |> draw_geometry_fragment()
        } else {
            draw_unlit(p.pos, float3(max(p.size, BULLET_SIZE * 0.8)), tint,
                saturate(life * 2.0), float4(0.0, 0.0, 0.0, 1.0), 2.2 * life)
            geo_sphere |> draw_geometry_fragment()
        }
    }
}

def private draw_shockwaves() {
    glUseProgram(ring_program)
    active_program = ring_program
    query() $(p : Particle) {
        if (p.kind != ParticleKind.shock) { return ; }
        let life = p.lifetime / p.max_life
        let age = 1.0 - life
        // The front races out fast and early, then eases; the band narrows as
        // it goes, which is what makes it read as travelling rather than growing.
        let radius = p.size * (0.10 + sqrt(age) * 0.90)
        let width = lerp(0.20, 0.045, age)
        v_model = compose(p.pos, float4(0.0, 0.0, 0.0, 1.0), float3(radius, radius, 1.0))
        f_Color = float4(p.color, life * life * 0.55)
        f_Material = float4(width, 0.0, 1.1 * life, 0.0)
        vs_ring_bind_uniform(active_program)
        fs_ring_bind_uniform(active_program)
        geo_disc |> draw_geometry_fragment()
    }
}

def render_projectiles(game_time : float) {
    use_unlit_program()
    draw_smoke(game_time)

    // Player tracers: a bright core with a longer, dimmer streak behind it.
    query() $(b : PlayerBullet) {
        let dir = normalize(b.vel)
        let rot = quat_align_z(dir)
        draw_unlit(b.pos, float3(BULLET_SIZE * 0.9, BULLET_SIZE * 0.9, BULLET_SIZE * 3.6),
            float3(0.35, 1.0, 0.92), 1.0, rot, 2.4)
        geo_cylinder |> draw_geometry_fragment()
        draw_unlit(b.pos - dir * BULLET_SIZE * 4.0,
            float3(BULLET_SIZE * 0.55, BULLET_SIZE * 0.55, BULLET_SIZE * 5.0),
            float3(0.18, 0.75, 0.85), 0.35, rot, 1.0)
        geo_cylinder |> draw_geometry_fragment()
    }

    query() $(b : EnemyBullet) {
        let flicker = 0.8 + 0.2 * sin(game_time * 30.0 + b.pos.x)
        if (b.bullet_type == 0) {
            draw_unlit(b.pos, float3(ENEMY_BULLET_SIZE * 0.85), float3(1.0, 0.80, 0.28),
                1.0, float4(0.0, 0.0, 0.0, 1.0), 1.6 * flicker)
            geo_sphere |> draw_geometry_fragment()
        } else {
            let dir = normalize(b.vel)
            let rot = quat_align_z(dir)
            let tint = (b.bullet_type == 1 ? float3(1.0, 0.16, 0.12) : float3(1.0, 0.55, 0.10))
            draw_unlit(b.pos, float3(ENEMY_BULLET_SIZE * 0.5, ENEMY_BULLET_SIZE * 0.5,
                ENEMY_BULLET_SIZE * 2.6), tint, 1.0, rot, 1.6 * flicker)
            geo_cylinder |> draw_geometry_fragment()
        }
    }

    // Additive from here on: fire adds light, it does not occlude.
    glBlendFunc(GL_SRC_ALPHA, uint(GL_ONE))
    use_unlit_program()
    draw_hot_layers()
    draw_shockwaves()
    render_bonus_fx(game_time)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
}

def private draw_disc(center : float3; radius : float; rot : float4; color : float3; alpha, phase : float) {
    v_model = compose(center, rot, float3(radius, radius, radius))
    f_Color = float4(color, alpha)
    f_Material = float4(1.0, 0.0, phase, 0.0)
    vs_rotor_bind_uniform(active_program)
    fs_rotor_bind_uniform(active_program)
    geo_disc |> draw_geometry_fragment()
}

// Rotor discs. One alpha-blended disc per aircraft, with the blade phase driven
// through the material's emissive slot so the shader can smear it.
def render_rotors(game_time : float) {
    glDisable(GL_CULL_FACE)
    glUseProgram(rotor_program)
    active_program = rotor_program

    if (player_respawn_timer <= PLAYER_RESPAWN_BLINK) {
        let blinking = player_respawn_timer > 0.0
        if (!blinking || int(game_time * 16.0) % 2 != 0) {
            let orient = player_orientation()
            let up_offset = PLAYER_SIZE * 1.85 * 0.70
            draw_disc(player_pos + float3(0.0, 0.0, up_offset), PLAYER_ROTOR_SIZE * 1.05,
                orient, float3(0.82, 0.88, 0.96), 0.55, game_time)
            // Tail rotor, spinning in the vertical plane on the fin.
            draw_disc(player_pos + float3(-PLAYER_SIZE * 0.20, -PLAYER_SIZE * 2.10, up_offset * 0.62),
                PLAYER_ROTOR_SIZE * 0.34, quat_mul(orient, quat_y_rot(PI * 0.5)),
                float3(0.78, 0.84, 0.92), 0.45, game_time * 2.4)
        }
    }

    query() $(h : EnemyHelicopter) {
        let up_offset = ENEMY_HELI_SIZE * 1.7 * 0.70
        draw_disc(h.pos + float3(0.0, 0.0, up_offset), PLAYER_ROTOR_SIZE * 0.92,
            float4(0.0, 0.0, 0.0, 1.0), float3(0.85, 0.42, 0.32), 0.5,
            game_time + h.hover_phase)
    }

    glEnable(GL_CULL_FACE)
}

// Explosion debris draws with the scene, not with the effects: it is solid
// torn plating, so it wants the lit prop program, real shading and the metal
// reflection -- as an unlit alpha quad it read as flying paper.
def render_debris() {
    if (empty(geo_shards)) {
        return
    }
    use_prop_program()
    query() $(p : Particle) {
        if (p.kind != ParticleKind.debris) { return ; }
        let life = p.lifetime / p.max_life
        let tumble = quat_mul(quat_y_rot(p.spin_phase + p.spin_speed * (1.0 - life)),
            quat_x_rot(p.spin_phase * 1.7 + p.spin_speed * 0.6 * (1.0 - life)))
        // Chunks glow briefly at the flash, then go cold. Shrinking over the
        // last of their life hides the pop when they are culled.
        let heat = saturate((life - 0.82) * 5.5)
        let tint = lerp(p.color * 0.7, float3(1.0, 0.62, 0.24), heat)
        let fade = saturate(life * 4.0)
        draw_prop(p.pos, float3(p.size * fade), tint, tumble,
            float4(0.45, 0.55, heat * 1.4, 0.30), REFLECT_METAL)
        let shard_idx = int(p.spin_phase * 1.9) % length(geo_shards)
        geo_shards[shard_idx] |> draw_geometry_fragment()
    }
}

def render_all(game_time : float) {
    render_player(game_time)
    render_enemies(game_time)
    render_obstacles(game_time)
    render_debris()
}

def render_effects(game_time : float) {
    render_projectiles(game_time)
    render_rotors(game_time)
}
