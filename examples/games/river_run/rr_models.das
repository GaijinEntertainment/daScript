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
