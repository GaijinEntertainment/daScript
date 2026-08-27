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
            drop++
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
    return !(s.x <= s.y && pos.x >= s.x && pos.x <= s.y)
}

// --- Terrain profile ---
//
// The banks are no longer a flat plane. Height is a function of distance from
// the waterline: a wet beach, a shoulder, then rolling hills that carry the eye
// out to the fog. Gameplay shares the same function (terrain_height) so props
// sit on the ground instead of hovering over it.

// Terrain reaches this far outward from each bank, far enough that the fog has
// swallowed the mesh edge before it can read as a horizon line.
let TERRAIN_EXTEND = 150.0
// Lateral spans per bank. Distance is distributed quadratically, so the
// shoreline -- where the silhouette matters -- gets most of the vertices.
let TERRAIN_SPANS = 12
let ISLAND_SPANS = 4

def private thash(p : float2) : float {
    let h = sin(dot(p, float2(127.1, 311.7))) * 43758.545
    return h - floor(h)
}

def private tnoise(p : float2) : float {
    let i = floor(p)
    let f = p - i
    let u = f * f * (float2(3.0) - 2.0 * f)
    let a = thash(i)
    let b = thash(i + float2(1.0, 0.0))
    let c = thash(i + float2(0.0, 1.0))
    let d = thash(i + float2(1.0, 1.0))
    return lerp(lerp(a, b, u.x), lerp(c, d, u.x), u.y)
}

// Height above the waterline at `d` metres inland, sampled at world (wx, wy).
def bank_profile(d, wx, wy : float) : float {
    if (d <= 0.0) {
        return 0.0
    }
    let p = float2(wx, wy)
    // Wet sand lifting out of the water, then the bank shoulder.
    var h = smoothstep(0.0, 2.6, d) * 0.42
    h += smoothstep(1.8, 11.0, d) * 1.75
    // Mid-scale dunes/mounds, then the far hills that build the skyline.
    h += smoothstep(4.0, 26.0, d) * 1.9 * (tnoise(p * 0.055) - 0.35)
    h += smoothstep(14.0, 95.0, d) * 11.0 * (tnoise(p * 0.017 + float2(19.0, 7.0)) - 0.30)
    return max(h, 0.0)
}

def private island_profile(d : float) : float {
    if (d <= 0.0) {
        return 0.0
    }
    return smoothstep(0.0, 1.4, d) * 0.55 + smoothstep(0.6, 3.2, d) * 0.7
}

// Distance along y to the nearest END of the split run containing world_y.
//
// The banks only ever have to blend sideways -- they run the whole length of
// the river. The centre island does not: it starts and stops, and if its height
// only tapers laterally it meets open water at full crown height, which is the
// seam that runs down the river. Feeding min(lateral, longitudinal) into the
// same profile rounds the ends off too, so the island blends in every
// direction.
//
// A run that continues past the generated river returns a large distance rather
// than tapering, otherwise the island would grow a moving wall at the far end
// as new segments scroll in.
let ISLAND_OPEN_END = 1.0e6

def island_end_distance(world_y : float) : float {
    let n = length(river_segments)
    if (n < 2) {
        return 0.0
    }
    var here = -1
    for (i in range(n - 1)) {
        if (river_segments[i].world_y <= world_y && world_y < river_segments[i + 1].world_y) {
            here = i
            break
        }
    }
    if (here < 0) {
        return ISLAND_OPEN_END
    }

    var ahead = ISLAND_OPEN_END
    for (step in range(n)) {
        let i = here + 1 + step
        if (i >= n) {
            break
        }
        if (!river_segments[i].split) {
            ahead = max(river_segments[i].world_y - world_y, 0.0)
            break
        }
    }

    var behind = ISLAND_OPEN_END
    for (step in range(n)) {
        let i = here - step
        if (i < 0) {
            break
        }
        if (!river_segments[i].split) {
            behind = max(world_y - river_segments[i].world_y, 0.0)
            break
        }
    }
    return min(ahead, behind)
}

// Ground height at any world position: 0 on open water, the bank profile
// outside the channel, a low crown on the centre island of a split.
def terrain_height(wx, wy : float) : float {
    let s = sample_river_segment(wy)
    if (wx <= s.left_bank_x) {
        return bank_profile(s.left_bank_x - wx, wx, wy)
    }
    if (wx >= s.right_bank_x) {
        return bank_profile(wx - s.right_bank_x, wx, wy)
    }
    if (s.split && wx >= s.split_left_x && wx <= s.split_right_x) {
        let d = min(wx - s.split_left_x, s.split_right_x - wx)
        return island_profile(min(d, island_end_distance(wy)))
    }
    return 0.0
}

// --- Geometry Rebuild ---

// Corners are given with their own normals. Each surface knows which height
// field it belongs to, so normals are computed at emit time rather than by a
// single after-the-fact pass -- that pass could only guess, and it guessed
// bank_profile for the centre-island vertices too.
def private push_strip_quad(var frag : GeometryFragment; p0, p1, p2, p3 : float3;
                            n0, n1, n2, n3 : float3; uv0, uv1, uv2, uv3 : float2) {
    let base = length(frag.vertices)
    frag.vertices |> push(GeometryPreviewVertex(xyz = p0, normal = n0, uv = uv0))  // nolint:STYLE012 - appends to the accumulating fragment
    frag.vertices |> push(GeometryPreviewVertex(xyz = p1, normal = n1, uv = uv1))
    frag.vertices |> push(GeometryPreviewVertex(xyz = p2, normal = n2, uv = uv2))
    frag.vertices |> push(GeometryPreviewVertex(xyz = p3, normal = n3, uv = uv3))
    frag.indices |> push(base + 0)  // nolint:STYLE012 - appends to the accumulating fragment
    frag.indices |> push(base + 1)
    frag.indices |> push(base + 2)
    frag.indices |> push(base + 2)
    frag.indices |> push(base + 1)
    frag.indices |> push(base + 3)
}

// Central-difference normal of the analytic height field. Cheaper and smoother
// than averaging face normals, and it matches what the gameplay code samples.
def private profile_normal(d, wx, wy, side : float) : float3 {
    let e = 0.6
    let hx0 = bank_profile(max(d - e, 0.0), wx - side * e, wy)
    let hx1 = bank_profile(d + e, wx + side * e, wy)
    let hy0 = bank_profile(d, wx, wy - e)
    let hy1 = bank_profile(d, wx, wy + e)
    let dx = (hx1 - hx0) / (2.0 * e) * side
    let dy = (hy1 - hy0) / (2.0 * e)
    return normalize(float3(-dx, -dy, 1.0))
}

// Quadratic span distribution: dense at the waterline, sparse out in the haze.
def private span_dist(i : int; count : int; extent : float) : float {
    let t = float(i) / float(count)
    return extent * t * t
}

def private emit_bank(var frag : GeometryFragment; y0, y1, edge0, edge1, side : float) {
    for (i in range(TERRAIN_SPANS)) {
        let d0 = span_dist(i, TERRAIN_SPANS, TERRAIN_EXTEND)
        let d1 = span_dist(i + 1, TERRAIN_SPANS, TERRAIN_EXTEND)
        let x00 = edge0 + side * d0
        let x01 = edge0 + side * d1
        let x10 = edge1 + side * d0
        let x11 = edge1 + side * d1
        let p00 = float3(x00, y0, bank_profile(d0, x00, y0))
        let p01 = float3(x01, y0, bank_profile(d1, x01, y0))
        let p10 = float3(x10, y1, bank_profile(d0, x10, y1))
        let p11 = float3(x11, y1, bank_profile(d1, x11, y1))
        let n00 = profile_normal(d0, x00, y0, side)
        let n01 = profile_normal(d1, x01, y0, side)
        let n10 = profile_normal(d0, x10, y1, side)
        let n11 = profile_normal(d1, x11, y1, side)
        // Winding depends on which side we are on, so both banks face up.
        if (side > 0.0) {
            push_strip_quad(frag, p00, p01, p10, p11, n00, n01, n10, n11,
                float2(d0, y0), float2(d1, y0), float2(d0, y1), float2(d1, y1))
        } else {
            push_strip_quad(frag, p01, p00, p11, p10, n01, n00, n11, n10,
                float2(d1, y0), float2(d0, y0), float2(d1, y1), float2(d0, y1))
        }
    }
}

// Central-difference normal of the island crown, taken through terrain_height so
// it follows the same field the geometry and the gameplay both use.
def private island_normal(wx, wy : float) : float3 {
    let e = 0.35
    let hx1 = terrain_height(wx + e, wy)
    let hx0 = terrain_height(wx - e, wy)
    let hy1 = terrain_height(wx, wy + e)
    let hy0 = terrain_height(wx, wy - e)
    return normalize(float3(-(hx1 - hx0) / (2.0 * e), -(hy1 - hy0) / (2.0 * e), 1.0))
}

def private emit_island(var frag : GeometryFragment; y0, y1, l0, r0, l1, r1 : float) {
    let half0 = (r0 - l0) * 0.5
    let half1 = (r1 - l1) * 0.5
    let c0 = (l0 + r0) * 0.5
    let c1 = (l1 + r1) * 0.5
    // Longitudinal distance to the run's ends, so the crown rounds off there.
    let end0 = island_end_distance(y0)
    let end1 = island_end_distance(y1)
    for (i in range(ISLAND_SPANS)) {
        let t0 = float(i) / float(ISLAND_SPANS)
        let t1 = float(i + 1) / float(ISLAND_SPANS)
        // Two mirrored halves so the crown runs down the middle of the island.
        for (s in fixed_array(-1.0, 1.0)) {
            let x00 = c0 + s * half0 * (1.0 - t0)
            let x01 = c0 + s * half0 * (1.0 - t1)
            let x10 = c1 + s * half1 * (1.0 - t0)
            let x11 = c1 + s * half1 * (1.0 - t1)
            // Each slice measures distance-from-edge against ITS OWN half width.
            // Using the near slice's width for the far edge left neighbouring
            // slices disagreeing about the height of the edge they share, which
            // stepped the island along y while it stayed smooth across x.
            let d00 = min(half0 * t0, end0)
            let d01 = min(half0 * t1, end0)
            let d10 = min(half1 * t0, end1)
            let d11 = min(half1 * t1, end1)
            let p00 = float3(x00, y0, island_profile(d00))
            let p01 = float3(x01, y0, island_profile(d01))
            let p10 = float3(x10, y1, island_profile(d10))
            let p11 = float3(x11, y1, island_profile(d11))
            let n00 = island_normal(x00, y0)
            let n01 = island_normal(x01, y0)
            let n10 = island_normal(x10, y1)
            let n11 = island_normal(x11, y1)
            if (s > 0.0) {
                push_strip_quad(frag, p01, p00, p11, p10, n01, n00, n11, n10,
                    float2(d01, y0), float2(d00, y0), float2(d11, y1), float2(d10, y1))
            } else {
                push_strip_quad(frag, p00, p01, p10, p11, n00, n01, n10, n11,
                    float2(d00, y0), float2(d01, y0), float2(d10, y1), float2(d11, y1))
            }
        }
    }
}

def gen_river_banks() : GeometryFragment {
    var frag : GeometryFragment
    frag.prim = GeometryFragmentType.triangles
    let n = length(river_segments)
    if (n < 2) {
        gen_bbox(frag)
        return <- frag
    }
    let slices = n - 1
    let per_slice = (TERRAIN_SPANS * 2 + ISLAND_SPANS * 2)
    frag.vertices |> reserve(slices * per_slice * 4)
    frag.indices |> reserve(slices * per_slice * 6)

    for (i in range(slices)) {
        let s0 = river_segments[i]
        let s1 = river_segments[i + 1]
        emit_bank(frag, s0.world_y, s1.world_y, s0.left_bank_x, s1.left_bank_x, -1.0)
        emit_bank(frag, s0.world_y, s1.world_y, s0.right_bank_x, s1.right_bank_x, 1.0)
        if (s0.split || s1.split) {
            emit_island(frag, s0.world_y, s1.world_y,
                s0.split_left_x, s0.split_right_x, s1.split_left_x, s1.split_right_x)
        }
    }

    gen_bbox(frag)
    return <- frag
}

// Water is subdivided laterally so uv.x can carry distance-to-nearest-bank; the
// shader turns that into the shallow-water tint and the foam band.
let WATER_SPANS = 10

def private emit_water_channel(var frag : GeometryFragment; y0, y1, l0, r0, l1, r1, z : float) {
    let w0 = r0 - l0
    let w1 = r1 - l1
    if (w0 <= 0.01 && w1 <= 0.01) {
        return
    }
    for (i in range(WATER_SPANS)) {
        let t0 = float(i) / float(WATER_SPANS)
        let t1 = float(i + 1) / float(WATER_SPANS)
        let x00 = l0 + w0 * t0
        let x01 = l0 + w0 * t1
        let x10 = l1 + w1 * t0
        let x11 = l1 + w1 * t1
        let d00 = min(x00 - l0, r0 - x00)
        let d01 = min(x01 - l0, r0 - x01)
        let d10 = min(x10 - l1, r1 - x10)
        let d11 = min(x11 - l1, r1 - x11)
        let up = float3(0.0, 0.0, 1.0)
        push_strip_quad(frag,
            float3(x00, y0, z), float3(x01, y0, z),
            float3(x10, y1, z), float3(x11, y1, z),
            up, up, up, up,
            float2(d00, y0), float2(d01, y0), float2(d10, y1), float2(d11, y1))
    }
}

def gen_river_surface() : GeometryFragment {
    var frag : GeometryFragment
    frag.prim = GeometryFragmentType.triangles
    let n = length(river_segments)
    if (n < 2) {
        gen_bbox(frag)
        return <- frag
    }
    let slices = n - 1
    frag.vertices |> reserve(slices * WATER_SPANS * 8)
    frag.indices |> reserve(slices * WATER_SPANS * 12)
    let z = -0.03

    for (i in range(slices)) {
        let s0 = river_segments[i]
        let s1 = river_segments[i + 1]
        if (s0.split || s1.split) {
            emit_water_channel(frag, s0.world_y, s1.world_y,
                s0.left_bank_x, s0.split_left_x, s1.left_bank_x, s1.split_left_x, z)
            emit_water_channel(frag, s0.world_y, s1.world_y,
                s0.split_right_x, s0.right_bank_x, s1.split_right_x, s1.right_bank_x, z)
        } else {
            emit_water_channel(frag, s0.world_y, s1.world_y,
                s0.left_bank_x, s0.right_bank_x, s1.left_bank_x, s1.right_bank_x, z)
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
    glUseProgram(water_program)
    active_program = water_program
    v_model = compose(float3(0.0), float4(0.0, 0.0, 0.0, 1.0), float3(1.0))
    f_RiverColor = float4(get_river_color(), 1.0)
    f_GameTime = game_time
    vs_water_bind_uniform(active_program)
    fs_water_bind_uniform(active_program)
    river_surface_geo |> draw_geometry_fragment()

    glUseProgram(terrain_program)
    active_program = terrain_program
    v_model = compose(float3(0.0), float4(0.0, 0.0, 0.0, 1.0), float3(1.0))
    f_BankColor = float4(get_bank_color(), 1.0)
    f_ShoreColor = env_now.shore_color
    f_RockColor = env_now.rock_color
    f_Material = float4(0.95, 0.02, 0.0, 0.10)
    vs_terrain_bind_uniform(active_program)
    fs_terrain_bind_uniform(active_program)
    river_bank_geo |> draw_geometry_fragment()
}

// Banks cast into the shadow map as well, so a hill throws shade across the
// water at a low sun.
def render_river_shadow() {
    v_model = compose(float3(0.0), float4(0.0, 0.0, 0.0, 1.0), float3(1.0))
    vs_shadow_bind_uniform(active_program)
    river_bank_geo |> draw_geometry_fragment()
}
