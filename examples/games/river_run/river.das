options gen2
options persistent_heap

require rr_globals public

// River bank geometry generation + rendering.
// River is defined by a ring of RiverSegment, each with left/right bank X at a world Y.
// Banks are rendered as a triangle strip rebuilt each frame.

def river_section_amplitude() : float {
    return 1.5 + float(min(current_section, 5)) * 0.5
}

def river_section_frequency() : float {
    return 0.018 + float(min(current_section, 6)) * 0.003
}

def river_target_half_width() : float {
    let s = float(min(current_section, 8))
    // Start wider/easier in section 0 and gradually narrow with progression.
    let base = (RIVER_HALF_WIDTH_MAX + 1.15) - s * 0.33
    return clamp(base, RIVER_HALF_WIDTH_MIN + 0.75, RIVER_HALF_WIDTH_MAX + 1.8)
}

def pick_split_half() : float {
    let s = float(min(current_section, 9))
    // Section 0 keeps split center narrow (wider channels); later sections can narrow channels more.
    let min_half = 0.68 + s * 0.09
    let max_half = min(3.15, 1.42 + s * 0.17)
    let hi = max(max_half, min_half + 0.35)
    if (random_f() < 0.35) {
        return random_range((min_half + hi) * 0.55, hi)
    }
    return random_range(min_half, hi)
}

def init_river() {
    river_segments |> clear()
    river_gen_y = player_pos.y - SEGMENT_LENGTH * 2.0
    river_center_x = 0.0
    river_half_width = RIVER_HALF_WIDTH_MAX
    river_center_target = 0.0
    river_width_target = RIVER_HALF_WIDTH_MAX
    // Guarantee a split in section 0, but not right at the start.
    river_split_target = false
    river_split_blend = 0.0
    river_split_half = 0.0
    river_split_hold = 0.0
    river_first_split_pending = true
    river_first_split_start_y = player_pos.y + random_range(95.0, 150.0)

    for (_i in range(RIVER_SEGMENTS_COUNT)) {
        advance_river_one()
    }
    river_dirty = true
}

def advance_river_one() {
    // Gradually drift center and width toward targets
    river_center_x += (river_center_target - river_center_x) * 0.15
    river_half_width += (river_width_target - river_half_width) * 0.12

    // Occasionally pick new targets
    if (random_f() < 0.12) {
        let max_drift = RIVER_CENTER_DRIFT_MAX - float(min(current_section, 5)) * 0.3
        river_center_target = random_range(-max_drift, max_drift)
    }
    if (random_f() < 0.12) {
        let lo = max(RIVER_HALF_WIDTH_MIN + 0.6, river_target_half_width() - 2.2)
        let hi = min(RIVER_HALF_WIDTH_MAX + 2.2, river_target_half_width() + 2.4)
        river_width_target = random_range(lo, hi)
    }

    // Force one split in section 0 once we get away from spawn.
    if (river_first_split_pending && river_gen_y >= river_first_split_start_y) {
        river_split_target = true
        river_split_half = pick_split_half()
        river_split_hold = random_range(90.0, 170.0)
        river_first_split_pending = false
    }

    // Split / merge events are allowed from section 0 onward.
    let split_chance = 0.035 + float(min(current_section, 6)) * 0.008
    if (!river_first_split_pending && !river_split_target && river_split_blend <= 0.01 && random_f() < split_chance) {
        river_split_target = true
        river_split_half = pick_split_half()
        river_split_hold = random_range(90.0, 190.0)
    }
    if (river_split_target && river_split_blend >= 0.99) {
        river_split_hold -= SEGMENT_LENGTH
        if (river_split_hold <= 0.0) {
            river_split_target = false
        }
    }
    if (river_split_target && random_f() < 0.10) {
        river_split_half = pick_split_half()
    }

    let split_speed = 0.10
    river_split_blend += (river_split_target ? split_speed : -split_speed)
    river_split_blend = clamp(river_split_blend, 0.0, 1.0)

    if (river_split_blend <= 0.001) {
        river_split_half = 0.0
    }

    // Add sine wave ripple on top
    let amp = river_section_amplitude()
    let freq = river_section_frequency()
    let ripple = sin(river_gen_y * freq) * amp

    let center_line = river_center_x + ripple
    let left_x = center_line - river_half_width
    let right_x = center_line + river_half_width

    // Early sections guarantee roomier split channels; later sections may get tighter.
    let min_channel_half = max(2.1, 3.25 - float(min(current_section, 8)) * 0.14)
    let max_split_half = max(river_half_width - min_channel_half, 0.0)
    let split_wave = 0.9 + 0.1 * sin(river_gen_y * 0.03)
    let split_half = min(max_split_half, river_split_half * river_split_blend * split_wave)
    let split_active = split_half > 0.35

    let seg = RiverSegment(
        left_bank_x = left_x,
        right_bank_x = right_x,
        split = split_active,
        split_left_x = center_line - split_half,
        split_right_x = center_line + split_half,
        world_y = river_gen_y
    )
    river_segments |> push(seg)
    river_gen_y += SEGMENT_LENGTH
}

def advance_river() {
    // Drop segments that are behind the camera
    let keep_y = player_pos.y - SEGMENT_LENGTH * 4.0
    var drop = 0
    for (seg in river_segments) {
        if (seg.world_y < keep_y) {
            drop += 1
        } else {
            break
        }
    }
    for (_i in range(drop)) {
        river_segments |> erase(0)
        advance_river_one()
    }
    if (drop > 0) {
        river_dirty = true
    }
}

def sample_river_segment(world_y : float) : RiverSegment {
    if (length(river_segments) < 2) {
        return RiverSegment(
            left_bank_x = -5.0,
            right_bank_x = 5.0,
            split = false,
            split_left_x = 0.0,
            split_right_x = 0.0,
            world_y = world_y
        )
    }
    // Find surrounding segments
    var best_lo = 0
    for (i in range(length(river_segments) - 1)) {
        if (river_segments[i].world_y <= world_y) {
            best_lo = i
        } else {
            break
        }
    }
    let hi = min(best_lo + 1, length(river_segments) - 1)
    let s0 = river_segments[best_lo]
    let s1 = river_segments[hi]
    let t = (s1.world_y > s0.world_y
        ? clamp((world_y - s0.world_y) / (s1.world_y - s0.world_y), 0.0, 1.0)
        : 0.0)
    let split_blend = (s0.split ? 1.0 : 0.0) * (1.0 - t) + (s1.split ? 1.0 : 0.0) * t
    return RiverSegment(
        left_bank_x = s0.left_bank_x + (s1.left_bank_x - s0.left_bank_x) * t,
        right_bank_x = s0.right_bank_x + (s1.right_bank_x - s0.right_bank_x) * t,
        split = split_blend > 0.4,
        split_left_x = s0.split_left_x + (s1.split_left_x - s0.split_left_x) * t,
        split_right_x = s0.split_right_x + (s1.split_right_x - s0.split_right_x) * t,
        world_y = world_y
    )
}

// Returns clamped X range for the channel nearest prefer_x.
def river_clamp_x_for(world_y, prefer_x : float) : float2 {
    let s = sample_river_segment(world_y)
    if (!s.split) {
        return float2(s.left_bank_x, s.right_bank_x)
    }
    let a = float2(s.left_bank_x, s.split_left_x)
    let b = float2(s.split_right_x, s.right_bank_x)
    if (prefer_x >= a.x && prefer_x <= a.y) {
        return a
    }
    if (prefer_x >= b.x && prefer_x <= b.y) {
        return b
    }
    let ac = (a.x + a.y) * 0.5
    let bc = (b.x + b.y) * 0.5
    return (abs(prefer_x - ac) < abs(prefer_x - bc) ? a : b)
}

def river_clamp_x_secondary(world_y, prefer_x : float) : float2 {
    let s = sample_river_segment(world_y)
    if (!s.split) {
        return float2(1.0, -1.0)
    }
    let a = float2(s.left_bank_x, s.split_left_x)
    let b = float2(s.split_right_x, s.right_bank_x)
    let p = river_clamp_x_for(world_y, prefer_x)
    if (p.x == a.x && p.y == a.y) {
        return b
    }
    return a
}

// Compatibility helper used by existing gameplay code.
def river_clamp_x(world_y : float) : float2 {
    return river_clamp_x_for(world_y, player_pos.x)
}

// Check if world position is outside river banks
def is_out_of_river(pos : float3) : bool {
    let p = river_clamp_x_for(pos.y, pos.x)
    if (pos.x >= p.x && pos.x <= p.y) {
        return false
    }
    let s = river_clamp_x_secondary(pos.y, pos.x)
    if (s.x <= s.y && pos.x >= s.x && pos.x <= s.y) {
        return false
    }
    return true
}

// --- Geometry Rebuild ---

// Terrain extends this far outward from each bank
let TERRAIN_EXTEND = 90.0

def gen_river_banks() : GeometryFragment {
    var frag : GeometryFragment
    let n = length(river_segments)
    if (n < 2) {
        frag.prim = GeometryFragmentType.triangles
        gen_bbox(frag)
        return <- frag
    }
    // We build triangle strips for left bank and right bank:
    // Left bank: each segment pair produces a quad from (left_bank_x, y0) to (left_bank_x - extend, y1)
    // Right bank: quad from (right_bank_x, y0) to (right_bank_x + extend, y1)

    frag.prim = GeometryFragmentType.triangles
    let Z = 0.0
    let slices = n - 1
    // Reserve for worst case where each slice includes left, right, and split-center strips.
    frag.vertices |> reserve(slices * 12)
    frag.indices |> reserve(slices * 18)

    let zero_n = float3(0.0, 0.0, 1.0)
    let zero_uv = float2(0.0, 0.0)

    for (i in range(n - 1)) {
        let s0 = river_segments[i]
        let s1 = river_segments[i + 1]
        let y0 = s0.world_y
        let y1 = s1.world_y

        // Left bank quad: (lx0,y0) to (lx1,y1), extends leftward
        let lx0 = s0.left_bank_x
        let lx1 = s1.left_bank_x
        let lext0 = lx0 - TERRAIN_EXTEND
        let lext1 = lx1 - TERRAIN_EXTEND

        let base = length(frag.vertices)
        frag.vertices |> push(GeometryPreviewVertex(xyz = float3(lext0, y0, Z), normal = zero_n, uv = zero_uv))
        frag.vertices |> push(GeometryPreviewVertex(xyz = float3(lx0,   y0, Z), normal = zero_n, uv = zero_uv))
        frag.vertices |> push(GeometryPreviewVertex(xyz = float3(lext1, y1, Z), normal = zero_n, uv = zero_uv))
        frag.vertices |> push(GeometryPreviewVertex(xyz = float3(lx1,   y1, Z), normal = zero_n, uv = zero_uv))
        frag.indices |> push(base + 0)
        frag.indices |> push(base + 1)
        frag.indices |> push(base + 2)
        frag.indices |> push(base + 2)
        frag.indices |> push(base + 1)
        frag.indices |> push(base + 3)

        // Right bank quad: extends rightward
        let rx0 = s0.right_bank_x
        let rx1 = s1.right_bank_x
        let rext0 = rx0 + TERRAIN_EXTEND
        let rext1 = rx1 + TERRAIN_EXTEND

        let base2 = length(frag.vertices)
        frag.vertices |> push(GeometryPreviewVertex(xyz = float3(rx0,   y0, Z), normal = zero_n, uv = zero_uv))
        frag.vertices |> push(GeometryPreviewVertex(xyz = float3(rext0, y0, Z), normal = zero_n, uv = zero_uv))
        frag.vertices |> push(GeometryPreviewVertex(xyz = float3(rx1,   y1, Z), normal = zero_n, uv = zero_uv))
        frag.vertices |> push(GeometryPreviewVertex(xyz = float3(rext1, y1, Z), normal = zero_n, uv = zero_uv))
        frag.indices |> push(base2 + 0)
        frag.indices |> push(base2 + 1)
        frag.indices |> push(base2 + 2)
        frag.indices |> push(base2 + 2)
        frag.indices |> push(base2 + 1)
        frag.indices |> push(base2 + 3)

        // Center island strip when river is split at this slice.
        if (s0.split || s1.split) {
            let ml0 = s0.split_left_x
            let mr0 = s0.split_right_x
            let ml1 = s1.split_left_x
            let mr1 = s1.split_right_x

            let base3 = length(frag.vertices)
            frag.vertices |> push(GeometryPreviewVertex(xyz = float3(ml0, y0, Z), normal = zero_n, uv = zero_uv))
            frag.vertices |> push(GeometryPreviewVertex(xyz = float3(mr0, y0, Z), normal = zero_n, uv = zero_uv))
            frag.vertices |> push(GeometryPreviewVertex(xyz = float3(ml1, y1, Z), normal = zero_n, uv = zero_uv))
            frag.vertices |> push(GeometryPreviewVertex(xyz = float3(mr1, y1, Z), normal = zero_n, uv = zero_uv))
            frag.indices |> push(base3 + 0)
            frag.indices |> push(base3 + 1)
            frag.indices |> push(base3 + 2)
            frag.indices |> push(base3 + 2)
            frag.indices |> push(base3 + 1)
            frag.indices |> push(base3 + 3)
        }
    }

    gen_bbox(frag)
    return <- frag
}

def gen_river_surface() : GeometryFragment {
    var frag : GeometryFragment
    let n = length(river_segments)
    if (n < 2) {
        frag.prim = GeometryFragmentType.triangles
        gen_bbox(frag)
        return <- frag
    }
    frag.prim = GeometryFragmentType.triangles
    let Z = -0.02
    let zero_n = float3(0.0, 0.0, 1.0)
    let zero_uv = float2(0.0, 0.0)

    for (i in range(n - 1)) {
        let s0 = river_segments[i]
        let s1 = river_segments[i + 1]

        if (s0.split || s1.split) {
            // Left channel surface.
            let base = length(frag.vertices)
            frag.vertices |> push(GeometryPreviewVertex(xyz = float3(s0.left_bank_x,  s0.world_y, Z), normal = zero_n, uv = zero_uv))
            frag.vertices |> push(GeometryPreviewVertex(xyz = float3(s0.split_left_x, s0.world_y, Z), normal = zero_n, uv = zero_uv))
            frag.vertices |> push(GeometryPreviewVertex(xyz = float3(s1.left_bank_x,  s1.world_y, Z), normal = zero_n, uv = zero_uv))
            frag.vertices |> push(GeometryPreviewVertex(xyz = float3(s1.split_left_x, s1.world_y, Z), normal = zero_n, uv = zero_uv))
            frag.indices |> push(base + 0)
            frag.indices |> push(base + 1)
            frag.indices |> push(base + 2)
            frag.indices |> push(base + 2)
            frag.indices |> push(base + 1)
            frag.indices |> push(base + 3)

            // Right channel surface.
            let base2 = length(frag.vertices)
            frag.vertices |> push(GeometryPreviewVertex(xyz = float3(s0.split_right_x, s0.world_y, Z), normal = zero_n, uv = zero_uv))
            frag.vertices |> push(GeometryPreviewVertex(xyz = float3(s0.right_bank_x,  s0.world_y, Z), normal = zero_n, uv = zero_uv))
            frag.vertices |> push(GeometryPreviewVertex(xyz = float3(s1.split_right_x, s1.world_y, Z), normal = zero_n, uv = zero_uv))
            frag.vertices |> push(GeometryPreviewVertex(xyz = float3(s1.right_bank_x,  s1.world_y, Z), normal = zero_n, uv = zero_uv))
            frag.indices |> push(base2 + 0)
            frag.indices |> push(base2 + 1)
            frag.indices |> push(base2 + 2)
            frag.indices |> push(base2 + 2)
            frag.indices |> push(base2 + 1)
            frag.indices |> push(base2 + 3)
        } else {
            let base = length(frag.vertices)
            frag.vertices |> push(GeometryPreviewVertex(xyz = float3(s0.left_bank_x,  s0.world_y, Z), normal = zero_n, uv = zero_uv))
            frag.vertices |> push(GeometryPreviewVertex(xyz = float3(s0.right_bank_x, s0.world_y, Z), normal = zero_n, uv = zero_uv))
            frag.vertices |> push(GeometryPreviewVertex(xyz = float3(s1.left_bank_x,  s1.world_y, Z), normal = zero_n, uv = zero_uv))
            frag.vertices |> push(GeometryPreviewVertex(xyz = float3(s1.right_bank_x, s1.world_y, Z), normal = zero_n, uv = zero_uv))
            frag.indices |> push(base + 0)
            frag.indices |> push(base + 1)
            frag.indices |> push(base + 2)
            frag.indices |> push(base + 2)
            frag.indices |> push(base + 1)
            frag.indices |> push(base + 3)
        }
    }

    gen_bbox(frag)
    return <- frag
}

def rebuild_river_geometry() {
    finalize(river_bank_geo)
    river_bank_geo <- create_geometry_fragment <| gen_river_banks()
    finalize(river_surface_geo)
    river_surface_geo <- create_geometry_fragment <| gen_river_surface()
}

// --- Rendering ---

def render_river(game_time : float) {
    // River surface
    glUseProgram(river_program)
    active_program = river_program
    v_model = compose(float3(0.0), float4(0.0, 0.0, 0.0, 1.0), float3(1.0))
    let rc = get_river_color()
    f_RiverColor = float4(rc, 1.0)
    f_GameTime = game_time
    vs_river_bind_uniform(active_program)
    fs_river_bind_uniform(active_program)
    river_surface_geo |> draw_geometry_fragment()

    // Banks (grass shader, terrain color + per-pixel noise tint)
    glUseProgram(grass_program)
    active_program = grass_program
    v_model = compose(float3(0.0), float4(0.0, 0.0, 0.0, 1.0), float3(1.0))
    f_Color = float4(get_bank_color(), 1.0)
    vs_grass_bind_uniform(active_program)
    fs_grass_bind_uniform(active_program)
    river_bank_geo |> draw_geometry_fragment()
}
