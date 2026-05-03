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
            spin_phase = 0.0
        ))
    }
}

def spawn_particles(origin, color : float3; object_radius : float; count : int) {
    create_entities`Particle(count) $(eid : EntityId; i : int; var p : Particle) {
        let angle = random_f() * 2.0 * PI
        let r = max(object_radius, 0.18)
        let local = float3(
            random_range(-r, r),
            random_range(-r, r),
            random_range(-r * 0.35, r * 0.35)
        )
        let away = normalize(local + float3(cos(angle) * 0.35, sin(angle) * 0.35, random_range(-0.25, 0.25)))
        let speed = random_range(2.0, 6.5) * (0.7 + r * 0.5)
        p.pos = origin + local
        p.vel = away * speed + float3(0.0, 0.0, random_range(0.8, 2.8))
        p.color = color
        p.lifetime = random_range(PARTICLE_LIFETIME * 0.65, PARTICLE_LIFETIME * 1.25)
        p.max_life = p.lifetime
        p.size = random_range(0.07, 0.26) * (0.7 + r * 0.55)
        p.spin_speed = random_range(-13.0, 13.0)
        p.spin_phase = random_f() * 2.0 * PI
    }
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
    let bounds = float2(seg.left_bank_x, seg.right_bank_x)
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
    create_entity() @(eid, cmp) {
        apply_decs_template(cmp, Bridge(
            pos = pos,
            gap_center_x = gap_x,
            left_x = bounds.x,
            right_x = bounds.y,
            health = 1
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

    // Bridges
    let bridge_count = int(1.0 + section_mult * 0.4)
    for (i in range(bridge_count)) {
        let by = base_y + SECTION_LENGTH * (float(i + 1) / float(bridge_count + 1))
        spawn_bridge(float3(0.0, by, BRIDGE_Z))
    }
    commit()

    // Islands
    let island_count = int(section_mult * 0.8 + random_f() * 2.0)
    for (_i in range(island_count)) {
        var placed = false
        for (_try in range(6)) {
            let iy = base_y + random_range(10.0, SECTION_LENGTH - 10.0)
            let seg = sample_river_segment(iy)
            if (seg.split) {
                continue
            }
            let ibounds = river_clamp_x_for(iy, random_sign() * 20.0)
            let icx = (ibounds.x + ibounds.y) * 0.5 + random_range(-1.5, 1.5)
            spawn_island(float3(icx, iy, ISLAND_Z))
            placed = true
            break
        }
        if (!placed) {
            // Skip island if only split terrain was sampled in this pass.
        }
    }
    commit()

    // Fuel depots — spaced evenly and never on top of islands
    // At drain=1.8/s and min speed 4u/s, one section takes ~60s = 108 fuel total
    let depot_spacing = 70.0
    let depot_count = int(SECTION_LENGTH / depot_spacing)
    for (i in range(depot_count)) {
        var placed = false
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
                placed = true
                break
            }
        }
    }
    commit()

    // Enemies — staggered ahead
    let boat_count = int(2.0 + section_mult * 1.0)
    let plane_count = (current_section >= 1 ? int(1.0 + section_mult * 0.6) : 0)
    let heli_count = (current_section >= 2 ? int(section_mult * 0.5) : 0)

    for (_i in range(boat_count)) {
        var placed = false
        for (_try in range(10)) {
            let ey = base_y + random_range(5.0, SECTION_LENGTH - 5.0)
            let bounds = river_clamp_x_for(ey, random_sign() * 20.0)
            let ex = random_range(bounds.x + 1.0, bounds.y - 1.0)
            let candidate = float3(ex, ey, BOAT_Z)

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

            if (!blocked) {
                spawn_enemy_boat(candidate)
                placed = true
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
    cam_x = 0.0
    current_section = 0
    section_dist = 0.0
    section_banner_timer = 0.0
    low_fuel_beep_timer = 0.0
    refuel_sound_timer = 0.0
    river_color_from = section_colors[0]
    river_color_to   = section_colors[0]
    bank_color_from  = bank_colors[0]
    bank_color_to    = bank_colors[0]
    fog_color_from   = fog_colors[0]
    fog_color_to     = fog_colors[0]
    section_color_blend_t = 1.0

    global_rng_seed = uint(get_uptime() * 1000.0)
    init_river()
    spawn_section_objects()
    game_state = GameState.playing
}

// --- Player Update ---

def update_player_input() {
    // Keyboard
    let key_left = (
        glfwGetKey(live_window, int(GLFW_KEY_A)) == int(GLFW_PRESS) ||
        glfwGetKey(live_window, int(GLFW_KEY_LEFT)) == int(GLFW_PRESS)
    )
    let key_right = (
        glfwGetKey(live_window, int(GLFW_KEY_D)) == int(GLFW_PRESS) ||
        glfwGetKey(live_window, int(GLFW_KEY_RIGHT)) == int(GLFW_PRESS)
    )
    let key_up = (
        glfwGetKey(live_window, int(GLFW_KEY_W)) == int(GLFW_PRESS) ||
        glfwGetKey(live_window, int(GLFW_KEY_UP)) == int(GLFW_PRESS)
    )
    let key_down = (
        glfwGetKey(live_window, int(GLFW_KEY_S)) == int(GLFW_PRESS) ||
        glfwGetKey(live_window, int(GLFW_KEY_DOWN)) == int(GLFW_PRESS)
    )

    // Mouse delta
    let mpos = glfwGetCursorPos(live_window)
    let cx = mpos.x
    let cy = mpos.y
    var mdx = 0.0
    var mdy = 0.0
    if (mouse_pos_initialized) {
        mdx = clamp(cx - mouse_prev_x, -80.0, 80.0)
        mdy = clamp(cy - mouse_prev_y, -80.0, 80.0)
    }
    mouse_prev_x = cx
    mouse_prev_y = cy
    mouse_pos_initialized = true

    // Compose strafe and speed inputs
    var strafe_input = 0.0
    if (key_left) { strafe_input -= 1.0; }
    if (key_right) { strafe_input += 1.0; }
    strafe_input += mdx * MOUSE_STRAFE_SENS

    var speed_input = 0.0
    if (key_up) { speed_input += 1.0; }
    if (key_down) { speed_input -= 1.0; }
    speed_input -= mdy * MOUSE_SPEED_SENS

    player_vel.x += strafe_input * PLAYER_STRAFE_SPEED * tick_dt * 6.0
    player_fwd_speed = clamp(
        player_fwd_speed + speed_input * 4.0 * tick_dt,
        PLAYER_FWD_SPEED_MIN,
        PLAYER_FWD_SPEED_MAX * section_speed_mult()
    )
}

def update_player_shoot() {
    player_shoot_cooldown = max(player_shoot_cooldown - tick_dt, 0.0)
    let fire = (
        glfwGetKey(live_window, int(GLFW_KEY_SPACE)) == int(GLFW_PRESS) ||
        glfwGetMouseButton(live_window, int(GLFW_MOUSE_BUTTON_1)) == int(GLFW_PRESS)
    )
    if (fire && player_shoot_cooldown <= 0.0 && player_respawn_timer <= 0.0) {
        let max_lateral = BULLET_SPEED * 0.259  // sin(15°) ≈ 0.259
        let lateral = clamp(player_vel.x, -max_lateral, max_lateral)
        let fwd = sqrt(BULLET_SPEED * BULLET_SPEED - lateral * lateral)
        create_entity() @(eid, cmp) {
            apply_decs_template(cmp, PlayerBullet(
                pos = player_pos + float3(0.0, 0.8, 0.0),
                vel = float3(lateral, fwd, 0.0),
                age = 0.0
            ))
        }
        play_sfx(snd_shoot)
        player_shoot_cooldown = FIRE_COOLDOWN
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
    player_fuel = max(player_fuel - PLAYER_FUEL_DRAIN * tick_dt, 0.0)
    if (player_fuel <= 0.0 && player_respawn_timer <= 0.0) {
        on_player_killed()
    }
}

def on_player_killed() {
    player_lives -= 1
    spawn_particles(player_pos, float3(1.0, 0.5, 0.2), PLAYER_SIZE * 2.6, 34)
    play_sfx(snd_player_hit)
    player_respawn_timer = PLAYER_RESPAWN_HIDE + PLAYER_RESPAWN_BLINK
    player_invuln_timer = player_respawn_timer + PLAYER_INVULN_TIME
    player_fuel = PLAYER_FUEL_MAX * 0.7
    player_pos = find_safe_respawn_pos(player_pos)
    player_vel.x = 0.0
    if (player_lives <= 0) {
        play_sfx(snd_game_over)
        game_state = GameState.game_over_state
    }
}

def update_player(var esc_just : bool) {
    if (game_state != GameState.playing) {
        return
    }

    player_respawn_timer = max(player_respawn_timer - tick_dt, 0.0)
    player_invuln_timer = max(player_invuln_timer - tick_dt, 0.0)

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
    section_color_blend_t = min(section_color_blend_t + tick_dt * 0.7, 1.0)

    if (section_dist >= SECTION_LENGTH) {
        section_dist -= SECTION_LENGTH
        current_section += 1
        section_banner_timer = WAVE_BANNER_DURATION
        play_sfx(snd_section_clear)
        score += 500 * (current_section)
    // Start smooth color transition to new section palette
    river_color_from = get_river_color()
    bank_color_from  = get_bank_color()
    fog_color_from   = get_fog_color()
    river_color_to   = section_colors[section_idx()]
    bank_color_to    = bank_colors[section_idx()]
    fog_color_to     = fog_colors[section_idx()]
    section_color_blend_t = 0.0

        if (current_section >= MAX_SECTIONS) {
            game_state = GameState.win_state
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

    // Boats: patrol horizontally, shoot at player
    query() $(eid : EntityId; var b : EnemyBoat) {
        // Scroll with world — boats stay at their world Y, player moves to them
        b.shoot_timer -= tick_dt
        // Patrol
        let bounds = river_clamp_x_for(b.pos.y, b.lane_prefer_x)
        let margin = BOAT_SIZE * 2.0
        b.vel.x = b.patrol_dir * BOAT_PATROL_SPEED * sm
        b.pos.x += b.vel.x * tick_dt
        if (b.pos.x <= bounds.x + margin || b.pos.x >= bounds.y - margin) {
            b.patrol_dir = -b.patrol_dir
            b.pos.x = clamp(b.pos.x, bounds.x + margin, bounds.y - margin)
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

    // Planes: fly sideways, shoot when passing player Y
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

    // Helicopters: hover and orbit player
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

    // Move player bullets; delete on expiry or island collision
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

    // Age out particles
    query() $(eid : EntityId; var p : Particle) {
        p.lifetime -= tick_dt
        p.pos += p.vel * tick_dt
        if (p.lifetime <= 0.0) {
            delete_entity(eid)
        }
    }
    commit()
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

    // Player bullets vs enemies — collect hits first
    var inscope enemy_hits : array<EnemyHitInfo>

    query() $(eid aka b_eid : EntityId; pb : PlayerBullet) {
        var bullet_hit = false
        // vs boats
        find_query() $(eid : EntityId; b : EnemyBoat) {
            let r = BOAT_SIZE + BULLET_SIZE
            if (dist2d_sq(pb.pos, b.pos) < r * r) {
                enemy_hits |> push(EnemyHitInfo(bullet_eid = b_eid, enemy_eid = eid, pos = b.pos, score_value = 100, blast_radius = BOAT_SIZE))
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
                    enemy_hits |> push(EnemyHitInfo(bullet_eid = b_eid, enemy_eid = eid, pos = p.pos, score_value = 150, blast_radius = PLANE_SIZE))
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
                    enemy_hits |> push(EnemyHitInfo(bullet_eid = b_eid, enemy_eid = eid, pos = h.pos, score_value = 200, blast_radius = ENEMY_HELI_SIZE))
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
                    enemy_hits |> push(EnemyHitInfo(bullet_eid = b_eid, enemy_eid = eid, pos = d.pos, score_value = 75, blast_radius = FUEL_DEPOT_SIZE))
                    return true
                }
                return false
            }
        }
    }

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
        // damage enemy
        var killed = false
        find_query() $(eid : EntityId; var b : EnemyBoat) {
            if (eid == h.enemy_eid) {
                if (b.health > 0) {
                    b.health -= 1
                    if (b.health <= 0) { killed = true; }
                }
                return true
            }
            return false
        }
        find_query() $(eid : EntityId; var p : EnemyPlane) {
            if (eid == h.enemy_eid) {
                if (p.health > 0) {
                    p.health -= 1
                    if (p.health <= 0) { killed = true; }
                }
                return true
            }
            return false
        }
        find_query() $(eid : EntityId; var hh : EnemyHelicopter) {
            if (eid == h.enemy_eid) {
                if (hh.health > 0) {
                    hh.health -= 1
                    if (hh.health <= 0) { killed = true; }
                }
                return true
            }
            return false
        }
        // Bridges always destroyed by one hit
        find_query() $(eid : EntityId; br : Bridge) {
            if (eid == h.enemy_eid) {
                killed = true
                return true
            }
            return false
        }
        // Fuel depots — always one-shot
        var is_depot = false
        find_query() $(eid : EntityId; d : FuelDepot) {
            if (eid == h.enemy_eid) {
                killed = true
                is_depot = true
                return true
            }
            return false
        }
        if (killed) {
            delete_entity(h.enemy_eid)
            killed_this_tick |> push(h.enemy_eid)
            var is_bridge = false
            find_query() $(eid : EntityId; br : Bridge) {
                if (eid == h.enemy_eid) { is_bridge = true; return true; }
                return false
            }
            if (is_depot) {
                let c1 = int(24.0 + h.blast_radius * 15.0)
                let c2 = int(14.0 + h.blast_radius * 10.0)
                spawn_particles(h.pos, float3(1.0, 0.5, 0.05), h.blast_radius * 1.2, c1)
                spawn_particles(h.pos + float3(0.0, 0.0, 1.0), float3(0.9, 0.2, 0.0), h.blast_radius * 0.8, c2)
            } else {
                let c = int(10.0 + h.blast_radius * 12.0)
                spawn_particles(h.pos, float3(1.0, 0.65, 0.1), h.blast_radius, c)
            }
            play_sfx(is_bridge ? snd_bridge_destroy : snd_enemy_explode)
            score += h.score_value
        }
    }
    commit()

    // Enemy bullets vs player
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

    // Player vs fuel depots
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

    // Player vs islands
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

    // Player through bridge — must pass through gap
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

def render_player(game_time : float) {
    if (player_respawn_timer > PLAYER_RESPAWN_BLINK) {
        return
    }
    let blinking = player_respawn_timer > 0.0
    if (blinking && int(game_time * 16.0) % 2 == 0) {
        return
    }
    let inv = player_invuln_timer > 0.0
    let pulse = (inv ? 0.7 + 0.3 * abs(sin(game_time * 8.0)) : 1.0)
    let body_color = (inv ? float3(pulse, pulse * 0.8, 0.2) : float3(0.7, 0.85, 1.0))

    glUseProgram(phong_program)
    active_program = phong_program

    // Body sphere
    draw_with_phong(player_pos, float3(PLAYER_SIZE), body_color)
    geo_sphere |> draw_geometry_fragment()
}

def render_enemies(game_time : float) {
    glUseProgram(phong_program)
    active_program = phong_program

    // Boats: elongated hull + small turret
    query() $(b : EnemyBoat) {
        let hull_color = float3(0.6, 0.35, 0.15)
        draw_with_phong(b.pos, float3(BOAT_SIZE * 0.6, BOAT_SIZE * 1.6, BOAT_SIZE * 0.5), hull_color)
        geo_cube |> draw_geometry_fragment()
        // Turret
        let turret_pos = b.pos + float3(0.0, 0.0, BOAT_SIZE * 0.5)
        draw_with_phong(turret_pos, float3(BOAT_SIZE * 0.3), float3(0.4, 0.2, 0.1))
        geo_cube |> draw_geometry_fragment()
    }

    // Planes: sphere fuselage + wing cubes
    query() $(p : EnemyPlane) {
        let body_color = float3(0.5, 0.5, 0.7)
        draw_with_phong(p.pos, float3(PLANE_SIZE * 0.6), body_color)
        geo_sphere |> draw_geometry_fragment()
        // Wings
        let wing_l = p.pos + float3(-PLANE_SIZE * 1.0, 0.0, 0.0)
        let wing_r = p.pos + float3(PLANE_SIZE * 1.0, 0.0, 0.0)
        draw_with_phong(wing_l, float3(PLANE_SIZE * 0.9, PLANE_SIZE * 0.25, PLANE_SIZE * 0.1), float3(0.45, 0.45, 0.65))
        geo_cube |> draw_geometry_fragment()
        draw_with_phong(wing_r, float3(PLANE_SIZE * 0.9, PLANE_SIZE * 0.25, PLANE_SIZE * 0.1), float3(0.45, 0.45, 0.65))
        geo_cube |> draw_geometry_fragment()
    }

    // Enemy helis: sphere + rotor
    query() $(h : EnemyHelicopter) {
        let enemy_color = float3(0.8, 0.25, 0.15)
        draw_with_phong(h.pos, float3(ENEMY_HELI_SIZE), enemy_color)
        geo_sphere |> draw_geometry_fragment()
        // Rotor — transparent cross
    }
}

def render_obstacles(game_time : float) {
    glUseProgram(phong_program)
    active_program = phong_program

    // Bridges: two pylons + two half-spans
    query() $(br : Bridge) {
        let pylon_h = BRIDGE_SPAN_Z + 0.5
        let pylon_color = float3(0.55, 0.5, 0.45)
        let span_color = float3(0.6, 0.55, 0.5)

        // Left pylon
        let lp = float3(br.left_x, br.pos.y, pylon_h * 0.5)
        draw_with_phong(lp, float3(0.4, 0.3, pylon_h), pylon_color)
        geo_cube |> draw_geometry_fragment()

        // Right pylon
        let rp = float3(br.right_x, br.pos.y, pylon_h * 0.5)
        draw_with_phong(rp, float3(0.4, 0.3, pylon_h), pylon_color)
        geo_cube |> draw_geometry_fragment()

        // Left half-span (from left pylon to gap left edge)
        let gap_left = br.gap_center_x - BRIDGE_GAP_WIDTH * 0.5
        let left_span_cx = (br.left_x + gap_left) * 0.5
        let left_span_w = gap_left - br.left_x
        if (left_span_w > 0.1) {
            let sp = float3(left_span_cx, br.pos.y, BRIDGE_SPAN_Z)
            draw_with_phong(sp, float3(left_span_w * 0.5, 0.35, 0.25), span_color)
            geo_cube |> draw_geometry_fragment()
        }

        // Right half-span (from gap right edge to right pylon)
        let gap_right = br.gap_center_x + BRIDGE_GAP_WIDTH * 0.5
        let right_span_cx = (gap_right + br.right_x) * 0.5
        let right_span_w = br.right_x - gap_right
        if (right_span_w > 0.1) {
            let sp = float3(right_span_cx, br.pos.y, BRIDGE_SPAN_Z)
            draw_with_phong(sp, float3(right_span_w * 0.5, 0.35, 0.25), span_color)
            geo_cube |> draw_geometry_fragment()
        }
    }

    // Islands: flattened sphere
    query() $(isle : Island) {
        let island_color = float3(0.3, 0.45, 0.2)
        draw_with_phong(isle.pos, float3(isle.size, isle.size, isle.size * 0.3), island_color)
        geo_sphere |> draw_geometry_fragment()
    }

    // Fuel depots: flat barge + tall beacon
    query() $(d : FuelDepot) {
        let rr = FUEL_DEPOT_SIZE + PLAYER_SIZE + REFUEL_RADIUS
        let refueling = dist2d_sq(d.pos, player_pos) < rr * rr
        let barge_color = float3(0.5, 0.5, 0.5)
        let beacon_pulse = 0.6 + 0.4 * abs(sin(game_time * 3.5))
        let beacon_color = (
            refueling ? float3(0.2, beacon_pulse, 0.2) : float3(beacon_pulse, 0.8, 0.2)
        )
        // Barge
        draw_with_phong(d.pos, float3(FUEL_DEPOT_SIZE, FUEL_DEPOT_SIZE * 1.8, FUEL_DEPOT_SIZE * 0.25), barge_color)
        geo_cube |> draw_geometry_fragment()
        // Beacon pole
        let beacon_pos = d.pos + float3(0.0, 0.0, FUEL_DEPOT_SIZE * 0.8)
        draw_with_phong(beacon_pos, float3(0.1, 0.1, FUEL_DEPOT_SIZE * 0.8), beacon_color)
        geo_cube |> draw_geometry_fragment()
    }
}

def render_projectiles() {
    // Flat pass for particles and trails (drawn before rotors)
    glUseProgram(flat_program)
    active_program = flat_program
    glEnable(GL_BLEND)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
    glDepthMask(false)

    // Explosion chunks: transparent and spinning
    query() $(p : Particle) {
        let is_trail = p.max_life <= TRAIL_LIFETIME + 0.01
        if (is_trail) { return ; }
        let alpha = p.lifetime / p.max_life
        let spin_t = 1.0 - alpha
        let s = max(p.size * (0.45 + alpha * 0.8), 0.03)
        let rot = quat_y_rot(p.spin_phase + p.spin_speed * spin_t)
        v_model = compose(p.pos, rot, float3(s))
        f_Color = float4(p.color, alpha * 0.72)
        vs_flat_bind_uniform(active_program)
        fs_flat_bind_uniform(active_program)
        geo_cube |> draw_geometry_fragment()
    }

    // Trail particles (blend based on remaining life)
    query() $(p : Particle) {
        let is_trail = p.max_life <= TRAIL_LIFETIME + 0.01
        if (!is_trail) { return ; }
        let alpha = p.lifetime / p.max_life
        let s = max(p.size, BULLET_SIZE * 1.0)
        v_model = compose(p.pos, float4(0.0, 0.0, 0.0, 1.0), float3(s))
        f_Color = float4(p.color, alpha * 0.7)
        vs_flat_bind_uniform(active_program)
        fs_flat_bind_uniform(active_program)
        geo_sphere |> draw_geometry_fragment()
    }

    // Player bullets: thin cyan cylinder elongated along velocity
    glUseProgram(phong_program)
    active_program = phong_program
    query() $(b : PlayerBullet) {
        let dir = normalize(b.vel)
        let rot = quat_align_z(dir)
        draw_with_phong(b.pos, float3(BULLET_SIZE, BULLET_SIZE, BULLET_SIZE * 3.5), float3(0.2, 1.0, 0.9), rot)
        geo_cylinder |> draw_geometry_fragment()
    }

    // Enemy bullets: type-dependent shape + color
    query() $(b : EnemyBullet) {
        if (b.bullet_type == 0) {
            // Boat: small yellowish sphere
            draw_with_phong(b.pos, float3(ENEMY_BULLET_SIZE * 1.2), float3(0.9, 0.7, 0.2))
            geo_sphere |> draw_geometry_fragment()
        } elif (b.bullet_type == 1) {
            // Heli: bright red thin cylinder
            let dir = normalize(b.vel)
            let rot = quat_align_z(dir)
            draw_with_phong(b.pos, float3(ENEMY_BULLET_SIZE, ENEMY_BULLET_SIZE, ENEMY_BULLET_SIZE * 3.5), float3(1.0, 0.08, 0.08), rot)
            geo_cylinder |> draw_geometry_fragment()
        } else {
            // Plane: orange thin cylinder
            let dir = normalize(b.vel)
            let rot = quat_align_z(dir)
            draw_with_phong(b.pos, float3(ENEMY_BULLET_SIZE, ENEMY_BULLET_SIZE, ENEMY_BULLET_SIZE * 3.5), float3(1.0, 0.5, 0.05), rot)
            geo_cylinder |> draw_geometry_fragment()
        }
    }

    glDepthMask(true)
    glDisable(GL_BLEND)
}

def render_rotors(game_time : float) {
    glEnable(GL_BLEND)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
    glDepthMask(false)
    glUseProgram(rotor_program)
    active_program = rotor_program

    // Player rotor
    if (player_respawn_timer <= PLAYER_RESPAWN_BLINK) {
        let blinking = player_respawn_timer > 0.0
        if (!blinking || int(game_time * 16.0) % 2 != 0) {
            let rotor_angle = game_time * 12.0
            let rotor_rot = quat_y_rot(rotor_angle)
            let rotor_pos = player_pos + float3(0.0, 0.0, PLAYER_SIZE * 0.8)
            v_model = compose(rotor_pos, rotor_rot, float3(PLAYER_ROTOR_SIZE, 0.08, PLAYER_ROTOR_SIZE))
            f_Color = float4(0.9, 0.9, 0.95, 0.9)
            vs_rotor_bind_uniform(active_program)
            fs_rotor_bind_uniform(active_program)
            geo_plane_xz |> draw_geometry_fragment()
        }
    }

    // Enemy helicopter rotors
    query() $(h : EnemyHelicopter) {
        let rotor_angle = game_time * 15.0 + h.hover_phase
        let rotor_rot = quat_y_rot(rotor_angle)
        let rotor_pos = h.pos + float3(0.0, 0.0, ENEMY_HELI_SIZE * 0.8)
        v_model = compose(rotor_pos, rotor_rot, float3(PLAYER_ROTOR_SIZE * 0.85, 0.06, PLAYER_ROTOR_SIZE * 0.85))
        f_Color = float4(0.7, 0.3, 0.2, 0.85)
        vs_rotor_bind_uniform(active_program)
        fs_rotor_bind_uniform(active_program)
        geo_plane_xz |> draw_geometry_fragment()
    }

    glDepthMask(true)
    glDisable(GL_BLEND)
}

def render_all(game_time : float) {
    render_player(game_time)
    render_enemies(game_time)
    render_obstacles(game_time)
    render_projectiles()
    render_rotors(game_time)
}
