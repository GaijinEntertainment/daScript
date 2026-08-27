options gen2
options persistent_heap
options indenting = 4

require rr_shaders public

// Composite meshes for the things the player actually looks at.
//
// Each model is welded from transformed primitives into ONE GeometryFragment,
// with the part index baked into uv.x and a gloss modifier into uv.y. The prop
// shader resolves uv.x against four tint uniforms, so a whole painted hull is a
// single draw call and an enemy is the same mesh under a different palette.
//
// Local frames are nose-forward (+y), up (+z), and roughly unit-scaled, so a
// call site scales by one radius and gets sane proportions.

// Part slots, shared by every model: 0 body, 1 canopy/accent, 2 metal, 3 dark.
let PART_BODY = 0.0
let PART_ACCENT = 1.0
let PART_METAL = 2.0
let PART_DARK = 3.0

let GLOSS_MATTE = 0.1
let GLOSS_SEMI = 0.45
let GLOSS_SHINY = 1.0

// Weld `src` into `dst` under (pos, rot, scale). Normals go through
// rotation * (n / scale), which is the inverse-transpose for a rot-scale pair --
// without the division a stretched fuselage would light as if it were round.
def private weld(var dst : GeometryFragment; src : GeometryFragment;
                 pos, scale : float3; rot : float4; part, gloss : float) {
    let base = length(dst.vertices)
    let m = compose(pos, rot, scale)
    let inv_scale = float3(1.0 / scale.x, 1.0 / scale.y, 1.0 / scale.z)
    dst.vertices |> reserve(base + length(src.vertices))
    for (v in src.vertices) {
        let p = m * float4(v.xyz, 1.0)
        let n = m * float4(v.normal * inv_scale, 0.0)
        dst.vertices |> push(GeometryPreviewVertex(
            xyz = p.xyz,
            normal = normalize(n.xyz),
            uv = float2(part, gloss)
        ))
    }
    dst.indices |> reserve(length(dst.indices) + length(src.indices))
    for (i in src.indices) {
        dst.indices |> push(base + i)
    }
}

def private no_rot() : float4 {
    return float4(0.0, 0.0, 0.0, 1.0)
}

def private axis_rot(axis : float3; angle : float) : float4 {
    let h = angle * 0.5
    let s = sin(h)
    return float4(axis.x * s, axis.y * s, axis.z * s, cos(h))
}

// A flat disc in the XY plane with uv in [0,1]^2, used for the rotor sweep.
def gen_disc(segments : int) : GeometryFragment {
    var frag : GeometryFragment
    frag.prim = GeometryFragmentType.triangles
    frag.vertices |> reserve(segments + 2)
    frag.indices |> reserve(segments * 3)
    // Fan centre first, then the rim.
    frag.vertices |> push(GeometryPreviewVertex(
        xyz = float3(0.0),
        normal = float3(0.0, 0.0, 1.0),
        uv = float2(0.5, 0.5)
    ))
    for (i in range(segments + 1)) {
        let a = float(i) * (2.0 * PI / float(segments))
        let c = float2(cos(a), sin(a))
        frag.vertices |> push(GeometryPreviewVertex(
            xyz = float3(c.x, c.y, 0.0),
            normal = float3(0.0, 0.0, 1.0),
            uv = c * 0.5 + float2(0.5)
        ))
    }
    for (i in range(segments)) {
        // One fan triangle per rim step: centre, this rim vertex, the next.
        frag.indices |> push_from([0, i + 1, i + 2])
    }
    gen_bbox(frag)
    return <- frag
}

// ============================================================================
// Helicopter
// ============================================================================

def gen_helicopter() : GeometryFragment {
    var frag : GeometryFragment
    frag.prim = GeometryFragmentType.triangles

    var body <- gen_sphere(14, 10, false)
    var tube <- gen_cylinder(GenDirection.xy, 12)
    var box <- gen_cube()

    let y_axis = float3(0.0, 1.0, 0.0)
    let x_axis = float3(1.0, 0.0, 0.0)

    // Fuselage: a stretched ellipsoid, widest at the cabin and tapering aft.
    weld(frag, body, float3(0.0, 0.02, 0.30), float3(0.32, 0.58, 0.30), no_rot(), PART_BODY, GLOSS_SEMI)
    // Chin / nose, dropped and pushed forward so the silhouette has a beak.
    weld(frag, body, float3(0.0, 0.46, 0.24), float3(0.24, 0.26, 0.20), no_rot(), PART_BODY, GLOSS_SEMI)
    // Canopy glass, sitting proud of the fuselage.
    weld(frag, body, float3(0.0, 0.34, 0.40), float3(0.24, 0.30, 0.20), no_rot(), PART_ACCENT, GLOSS_SHINY)

    // Tail boom running aft, then the fin and stabiliser that read as a tail
    // even at the distance the camera keeps.
    weld(frag, tube, float3(0.0, -0.62, 0.32), float3(0.075, 0.075, 0.62),
        axis_rot(x_axis, PI * 0.5), PART_BODY, GLOSS_SEMI)
    weld(frag, box, float3(0.0, -1.12, 0.46), float3(0.035, 0.16, 0.22), no_rot(), PART_BODY, GLOSS_SEMI)
    weld(frag, box, float3(0.0, -1.02, 0.34), float3(0.30, 0.09, 0.028), no_rot(), PART_BODY, GLOSS_SEMI)
    // Tail-rotor hub on the left face of the fin.
    weld(frag, tube, float3(-0.07, -1.14, 0.46), float3(0.05, 0.05, 0.05),
        axis_rot(y_axis, PI * 0.5), PART_METAL, GLOSS_SHINY)

    // Engine deck and rotor mast.
    weld(frag, box, float3(0.0, -0.10, 0.56), float3(0.15, 0.26, 0.09), no_rot(), PART_DARK, GLOSS_MATTE)
    weld(frag, tube, float3(0.0, -0.02, 0.66), float3(0.05, 0.05, 0.10), no_rot(), PART_METAL, GLOSS_SHINY)
    // Exhaust stub.
    weld(frag, tube, float3(0.13, -0.30, 0.50), float3(0.045, 0.045, 0.11),
        axis_rot(x_axis, PI * 0.5), PART_DARK, GLOSS_MATTE)

    // Landing skids: two rails on four struts. They give the hull a ground
    // plane to sit against and read strongly in the shadow silhouette.
    for (side in fixed_array(-1.0, 1.0)) {
        weld(frag, tube, float3(side * 0.26, 0.02, 0.045), float3(0.028, 0.028, 0.44),
            axis_rot(x_axis, PI * 0.5), PART_METAL, GLOSS_SEMI)
        for (fore in fixed_array(-0.24, 0.22)) {
            weld(frag, tube, float3(side * 0.20, fore, 0.15), float3(0.022, 0.022, 0.12),
                axis_rot(y_axis, side * 0.42), PART_METAL, GLOSS_SEMI)
        }
    }

    delete body
    delete tube
    delete box
    gen_bbox(frag)
    return <- frag
}

// ============================================================================
// Gunboat
// ============================================================================

def gen_gunboat() : GeometryFragment {
    var frag : GeometryFragment
    frag.prim = GeometryFragmentType.triangles

    var box <- gen_cube()
    var tube <- gen_cylinder(GenDirection.xy, 10)
    var wedge <- gen_prism(GenDirection.xy)
    var body <- gen_sphere(12, 8, false)

    let x_axis = float3(1.0, 0.0, 0.0)
    let z_axis = float3(0.0, 0.0, 1.0)

    // Hull: a slab with a wedge bow, so the boat has a direction on the water.
    weld(frag, box, float3(0.0, -0.10, 0.15), float3(0.30, 0.72, 0.15), no_rot(), PART_BODY, GLOSS_SEMI)
    weld(frag, wedge, float3(0.0, 0.78, 0.15), float3(0.30, 0.34, 0.15),
        axis_rot(z_axis, -PI * 0.5), PART_BODY, GLOSS_SEMI)
    // Gunwale strip: a lighter band along the top edge that catches the sun.
    weld(frag, box, float3(0.0, -0.10, 0.30), float3(0.31, 0.73, 0.025), no_rot(), PART_ACCENT, GLOSS_SEMI)

    // Deck house with a windscreen.
    weld(frag, box, float3(0.0, -0.44, 0.46), float3(0.21, 0.26, 0.17), no_rot(), PART_ACCENT, GLOSS_SEMI)
    weld(frag, box, float3(0.0, -0.19, 0.50), float3(0.17, 0.02, 0.10), no_rot(), PART_DARK, GLOSS_SHINY)

    // Forward turret and barrel -- the part that tells the player it shoots.
    weld(frag, body, float3(0.0, 0.28, 0.38), float3(0.19, 0.19, 0.14), no_rot(), PART_METAL, GLOSS_SEMI)
    weld(frag, tube, float3(0.0, 0.62, 0.42), float3(0.045, 0.045, 0.30),
        axis_rot(x_axis, PI * 0.5), PART_METAL, GLOSS_SHINY)

    // Mast.
    weld(frag, tube, float3(0.0, -0.60, 0.76), float3(0.018, 0.018, 0.16), no_rot(), PART_METAL, GLOSS_SEMI)

    delete box
    delete tube
    delete wedge
    delete body
    gen_bbox(frag)
    return <- frag
}

// ============================================================================
// Jet
// ============================================================================

def gen_jet() : GeometryFragment {
    var frag : GeometryFragment
    frag.prim = GeometryFragmentType.triangles

    var body <- gen_sphere(12, 9, false)
    var tube <- gen_cylinder(GenDirection.xy, 10)
    var cone <- gen_cone(GenDirection.xy, 10)
    var wedge <- gen_prism(GenDirection.xy)
    var box <- gen_cube()

    let x_axis = float3(1.0, 0.0, 0.0)
    let z_axis = float3(0.0, 0.0, 1.0)

    // Fuselage plus a sharp nose cone.
    weld(frag, body, float3(0.0, 0.0, 0.0), float3(0.15, 0.62, 0.15), no_rot(), PART_BODY, GLOSS_SEMI)
    weld(frag, cone, float3(0.0, 0.80, 0.0), float3(0.13, 0.13, 0.34),
        axis_rot(x_axis, -PI * 0.5), PART_BODY, GLOSS_SEMI)

    // Delta wings, swept back and slightly anhedral.
    for (side in fixed_array(-1.0, 1.0)) {
        weld(frag, wedge, float3(side * 0.46, -0.12, -0.02), float3(0.44, 0.34, 0.035),
            axis_rot(z_axis, side * PI * 0.5), PART_BODY, GLOSS_SEMI)
        // Wingtip pods read as ordnance and stop the wing from ending in a line.
        weld(frag, tube, float3(side * 0.80, -0.06, -0.02), float3(0.035, 0.035, 0.16),
            axis_rot(x_axis, PI * 0.5), PART_METAL, GLOSS_SEMI)
        // Intakes.
        weld(frag, box, float3(side * 0.20, 0.10, -0.05), float3(0.055, 0.20, 0.075),
            no_rot(), PART_DARK, GLOSS_MATTE)
    }

    // Canopy and twin tail fins.
    weld(frag, body, float3(0.0, 0.20, 0.13), float3(0.10, 0.24, 0.09), no_rot(), PART_ACCENT, GLOSS_SHINY)
    for (side in fixed_array(-1.0, 1.0)) {
        weld(frag, box, float3(side * 0.13, -0.62, 0.16), float3(0.022, 0.16, 0.16),
            axis_rot(float3(0.0, 1.0, 0.0), side * 0.22), PART_BODY, GLOSS_SEMI)
    }

    // Exhaust nozzle; the call site drives its glow through the emissive slot.
    weld(frag, tube, float3(0.0, -0.70, 0.0), float3(0.11, 0.11, 0.09),
        axis_rot(x_axis, PI * 0.5), PART_DARK, GLOSS_MATTE)

    delete body
    delete tube
    delete cone
    delete wedge
    delete box
    gen_bbox(frag)
    return <- frag
}

// ============================================================================
// Palette binding
// ============================================================================

// Bind a four-slot palette for the next model draw. `tint` multiplies every
// slot, which is how the damage flash and the invulnerability pulse are applied
// without a second material.
def set_model_palette(body, accent, metal, dark : float3) {
    f_PaletteOn = 1.0
    f_Tint0 = body
    f_Tint1 = accent
    f_Tint2 = metal
    f_Tint3 = dark
}

def clear_model_palette() {
    f_PaletteOn = 0.0
}

// ============================================================================
// Debris shards
// ============================================================================
//
// Explosion chunks were unit cubes, which read as flying dice. A shard is a cube
// whose eight CORNERS are jittered -- corners, not vertices, so shared corners
// move together and the hull stays closed -- then re-emitted with flat per-face
// normals. The faceting is the point: an angular chunk catching the sun on one
// face reads as torn plating, where a smooth-shaded one reads as a pebble.

let SHARD_VARIANTS = 6

def private shard_hash(i : int; salt : float) : float {
    let h = sin(float(i) * 12.9898 + salt * 78.233) * 43758.545
    return h - floor(h)
}

def private gen_shard(seed : int) : GeometryFragment {
    var frag : GeometryFragment
    frag.prim = GeometryFragmentType.triangles

    // Eight jittered corners, plus a per-variant squash so the family spans
    // plates, splinters and blocks rather than eight near-identical dice.
    let squash = float3(
        0.55 + shard_hash(seed, 1.0) * 0.85,
        0.55 + shard_hash(seed, 2.0) * 0.85,
        0.35 + shard_hash(seed, 3.0) * 0.6
    )
    var corner : float3[8]
    for (i in range(8)) {
        let sx = ((i & 1) != 0 ? 1.0 : -1.0)
        let sy = ((i & 2) != 0 ? 1.0 : -1.0)
        let sz = ((i & 4) != 0 ? 1.0 : -1.0)
        let jitter = float3(
            shard_hash(seed * 8 + i, 11.0) - 0.5,
            shard_hash(seed * 8 + i, 23.0) - 0.5,
            shard_hash(seed * 8 + i, 37.0) - 0.5
        ) * 0.7
        corner[i] = (float3(sx, sy, sz) + jitter) * squash
    }

    // Cube faces as corner-index quads, wound counter-clockwise from outside.
    let faces = fixed_array(
        int4(1, 3, 7, 5),    // +x
        int4(2, 0, 4, 6),    // -x
        int4(3, 2, 6, 7),    // +y
        int4(0, 1, 5, 4),    // -y
        int4(4, 5, 7, 6),    // +z
        int4(2, 3, 1, 0)     // -z
    )
    frag.vertices |> reserve(6 * 6)
    frag.indices |> reserve(6 * 6)
    for (f in faces) {
        let a = corner[f.x]
        let b = corner[f.y]
        let c = corner[f.z]
        let d = corner[f.w]
        // One flat normal per triangle, so every facet catches the light on its
        // own terms.
        for (tri in fixed_array(int3(0, 1, 2), int3(0, 2, 3))) {
            let p = fixed_array(a, b, c, d)
            let v0 = p[tri.x]
            let v1 = p[tri.y]
            let v2 = p[tri.z]
            let n = normalize(cross(v1 - v0, v2 - v0))
            let base = length(frag.vertices)
            for (v in fixed_array(v0, v1, v2)) {
                frag.vertices |> push(GeometryPreviewVertex(xyz = v, normal = n, uv = float2(0.0, 0.35)))
            }
            frag.indices |> push_from([base, base + 1, base + 2])
        }
    }
    gen_bbox(frag)
    return <- frag
}

def gen_shard_variant(i : int) : GeometryFragment {
    return <- gen_shard(i % SHARD_VARIANTS)
}

// ============================================================================
// Bank flora
// ============================================================================
//
// The banks were a cone on a stick, repeated. Two things made that read as
// cartoon: every tree was the same perfect cone, and a tree was the only thing
// growing. These are six variants of a shared silhouette vocabulary -- conifer,
// broadleaf, dead, bush, boulder, reeds -- each welded into ONE mesh so a
// crowded bank is still one draw call per prop.
//
// Foliage rides PART_BODY and PART_ACCENT, wood rides PART_DARK. PART_METAL is
// deliberately unused: the prop shader auto-reflects that slot, and a chrome
// tree trunk is not the goal.

let FLORA_CONIFER = 0
let FLORA_BROADLEAF = 1
let FLORA_DEAD = 2
let FLORA_BUSH = 3
let FLORA_ROCK = 4
let FLORA_REEDS = 5
let FLORA_VARIANTS = 6
// Distinct pre-generated meshes per kind. Four is enough that a bank does not
// read as a repeat, and cheap enough to build every one at load.
let FLORA_INSTANCES = 4

def private flora_hash(i : int; salt : float) : float {
    let h = sin(float(i) * 45.164 + salt * 91.377) * 43758.545
    return h - floor(h)
}

// A trunk that actually tapers, leans a little and is not perfectly round.
def private weld_trunk(var frag : GeometryFragment; src : GeometryFragment;
                       height, radius, lean : float; seed : int) {
    let segs = 3
    for (i in range(segs)) {
        let t0 = float(i) / float(segs)
        let t1 = float(i + 1) / float(segs)
        let r0 = radius * (1.0 - t0 * 0.55)
        let mid = (t0 + t1) * 0.5
        let off = float3(lean * mid * mid, lean * 0.4 * mid * mid, 0.0)
        weld(frag, src, off + float3(0.0, 0.0, height * (t0 + t1) * 0.5),
            float3(r0, r0 * (0.85 + flora_hash(seed * 4 + i, 3.0) * 0.3), height * (t1 - t0) * 0.5),
            no_rot(), PART_DARK, GLOSS_MATTE)
    }
}

def private gen_conifer(seed : int) : GeometryFragment {
    var frag : GeometryFragment
    frag.prim = GeometryFragmentType.triangles
    var cone <- gen_cone(GenDirection.xy, 9)
    var tube <- gen_cylinder(GenDirection.xy, 7)

    let lean = (flora_hash(seed, 1.0) - 0.5) * 0.22
    weld_trunk(frag, tube, 0.55, 0.075, lean, seed)

    // Four to six tiers, each nudged off-axis and rolled, so no two conifers
    // present the same silhouette.
    let tiers = 4 + int(flora_hash(seed, 2.0) * 2.99)
    for (i in range(tiers)) {
        let t = float(i) / float(max(tiers - 1, 1))
        let h = 0.9 - t * 0.34
        let r = 0.62 - t * 0.30
        let jitter = float3(
            (flora_hash(seed * 7 + i, 4.0) - 0.5) * 0.12,
            (flora_hash(seed * 7 + i, 5.0) - 0.5) * 0.12,
            0.0
        )
        let z = 0.34 + t * 1.05 + h * 0.5
        let part = (i % 2 == 0 ? PART_BODY : PART_ACCENT)
        weld(frag, cone, jitter + float3(lean * 0.8, lean * 0.3, z),
            float3(r, r * (0.88 + flora_hash(seed * 7 + i, 6.0) * 0.24), h * 0.5),
            axis_rot(float3(0.0, 0.0, 1.0), flora_hash(seed * 7 + i, 7.0) * 6.28),
            part, GLOSS_MATTE)
    }
    delete cone
    delete tube
    gen_bbox(frag)
    return <- frag
}

def private gen_broadleaf(seed : int) : GeometryFragment {
    var frag : GeometryFragment
    frag.prim = GeometryFragmentType.triangles
    var blob <- gen_sphere(10, 7, false)
    var tube <- gen_cylinder(GenDirection.xy, 7)

    let lean = (flora_hash(seed, 1.0) - 0.5) * 0.3
    weld_trunk(frag, tube, 0.85, 0.085, lean, seed)

    // Crown built from overlapping squashed blobs rather than one ball: the
    // lumpy union is what stops it reading as a lollipop.
    let blobs = 4 + int(flora_hash(seed, 2.0) * 2.99)
    for (i in range(blobs)) {
        let a = flora_hash(seed * 11 + i, 3.0) * 6.28
        let rad = 0.16 + flora_hash(seed * 11 + i, 4.0) * 0.26
        let up = 1.02 + flora_hash(seed * 11 + i, 5.0) * 0.42
        let size = 0.32 + flora_hash(seed * 11 + i, 6.0) * 0.24
        let part = (i % 3 == 0 ? PART_ACCENT : PART_BODY)
        weld(frag, blob,
            float3(cos(a) * rad + lean, sin(a) * rad + lean * 0.4, up),
            float3(size, size * (0.85 + flora_hash(seed * 11 + i, 7.0) * 0.3), size * 0.82),
            no_rot(), part, GLOSS_MATTE)
    }
    delete blob
    delete tube
    gen_bbox(frag)
    return <- frag
}

def private gen_deadtree(seed : int) : GeometryFragment {
    var frag : GeometryFragment
    frag.prim = GeometryFragmentType.triangles
    var tube <- gen_cylinder(GenDirection.xy, 6)

    let lean = (flora_hash(seed, 1.0) - 0.5) * 0.4
    weld_trunk(frag, tube, 1.25, 0.075, lean, seed)

    // Bare branches angled off the trunk. A few dead trees among the green ones
    // do more for "this bank is a place" than another dozen conifers.
    let branches = 3 + int(flora_hash(seed, 2.0) * 2.99)
    for (i in range(branches)) {
        let a = flora_hash(seed * 13 + i, 3.0) * 6.28
        let up = 0.55 + flora_hash(seed * 13 + i, 4.0) * 0.7
        let len = 0.22 + flora_hash(seed * 13 + i, 5.0) * 0.3
        let tilt = 0.5 + flora_hash(seed * 13 + i, 6.0) * 0.5
        let dir = float3(cos(a), sin(a), 0.0)
        weld(frag, tube, dir * len * 0.6 + float3(lean, lean * 0.4, up),
            float3(0.03, 0.03, len),
            axis_rot(normalize(float3(-dir.y, dir.x, 0.0)), tilt),
            PART_DARK, GLOSS_MATTE)
    }
    delete tube
    gen_bbox(frag)
    return <- frag
}

def private gen_bush(seed : int) : GeometryFragment {
    var frag : GeometryFragment
    frag.prim = GeometryFragmentType.triangles
    var blob <- gen_sphere(9, 6, false)
    let blobs = 3 + int(flora_hash(seed, 1.0) * 2.99)
    for (i in range(blobs)) {
        let a = flora_hash(seed * 17 + i, 2.0) * 6.28
        let rad = flora_hash(seed * 17 + i, 3.0) * 0.3
        let size = 0.26 + flora_hash(seed * 17 + i, 4.0) * 0.2
        let part = (i % 2 == 0 ? PART_BODY : PART_ACCENT)
        weld(frag, blob, float3(cos(a) * rad, sin(a) * rad, size * 0.72),
            float3(size, size * 0.9, size * 0.66), no_rot(), part, GLOSS_MATTE)
    }
    delete blob
    gen_bbox(frag)
    return <- frag
}

def private gen_boulder(seed : int) : GeometryFragment {
    // Boulders reuse the debris shard: an irregular flat-shaded hull is exactly
    // what a rock wants, and it costs nothing extra.
    var frag <- gen_shard(seed + 3)
    for (v in frag.vertices) {
        v.xyz = float3(v.xyz.x, v.xyz.y, v.xyz.z * 0.6 + 0.55)
        v.uv = float2(PART_DARK, GLOSS_MATTE)
    }
    gen_bbox(frag)
    return <- frag
}

def private gen_reeds(seed : int) : GeometryFragment {
    var frag : GeometryFragment
    frag.prim = GeometryFragmentType.triangles
    var cone <- gen_cone(GenDirection.xy, 5)
    // Stalks fanning outward from a clump: they go right at the waterline,
    // which is the one place the old banks had nothing at all.
    let stalks = 7 + int(flora_hash(seed, 1.0) * 6.99)
    for (i in range(stalks)) {
        let a = flora_hash(seed * 19 + i, 2.0) * 6.28
        let rad = flora_hash(seed * 19 + i, 3.0) * 0.26
        let h = 0.42 + flora_hash(seed * 19 + i, 4.0) * 0.5
        let tilt = (flora_hash(seed * 19 + i, 5.0) - 0.5) * 0.5
        let dir = float3(cos(a), sin(a), 0.0)
        let part = (i % 3 == 0 ? PART_ACCENT : PART_BODY)
        weld(frag, cone, dir * rad + float3(0.0, 0.0, h * 0.5),
            float3(0.035, 0.035, h * 0.5),
            axis_rot(normalize(float3(-dir.y, dir.x, 0.0)), tilt),
            part, GLOSS_MATTE)
    }
    delete cone
    gen_bbox(frag)
    return <- frag
}

// `variant` is a reserved word in gen2, hence `kind`.
def gen_flora(kind, seed : int) : GeometryFragment {
    if (kind == FLORA_CONIFER) {
        return <- gen_conifer(seed)
    } elif (kind == FLORA_BROADLEAF) {
        return <- gen_broadleaf(seed)
    } elif (kind == FLORA_DEAD) {
        return <- gen_deadtree(seed)
    } elif (kind == FLORA_BUSH) {
        return <- gen_bush(seed)
    } elif (kind == FLORA_ROCK) {
        return <- gen_boulder(seed)
    }
    return <- gen_reeds(seed)
}
