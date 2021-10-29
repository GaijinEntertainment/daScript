options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module geom_gen public

require daslib/safe_addr
require math

struct GeometryPreviewVertex
    xyz : float3
    normal : float3
    uv : float2

enum GeometryFragmentType
    none
    triangles

struct GeometryFragment
    vertices : array<GeometryPreviewVertex>
    indices : array<int>
    prim : GeometryFragmentType

struct ImageSurface
    width, height : int
    pixels : array<uint>

def private get_uv ( xyz:float3; uv:float2; cube:bool )
    if cube
        let XYZ = abs(xyz)
        var UV : float2
        if XYZ.x >= XYZ.y && XYZ.x >= XYZ.z
            UV = xyz.yz*sign(xyz.x)
        elif XYZ.y >= XYZ.z
            UV = xyz.xz*sign(xyz.y)
        else
            UV = xyz.xy*sign(xyz.z)
        return UV*0.5 + float2(0.5)
    else
        return uv

def gen_sphere ( sectorCount,stackCount:int; cubeUV : bool )
    var frag : GeometryFragment
    let sectorStep = 2. * PI / float(sectorCount)
    let stackStep = PI / float(stackCount)
    for i in range(stackCount+1)
        let stackAngle = PI / 2. -  float(i) * stackStep
        let xy = cos(stackAngle)
        let z  = sin(stackAngle)
        for j in range(sectorCount+1)
            let sectorAngle = float(j) * sectorStep
            let x = xy * cos(sectorAngle)
            let y = xy * sin(sectorAngle)
            let s = float(j) / float(sectorCount)
            let t = float(i) / float(stackCount)
            frag.vertices |> push <| [[GeometryPreviewVertex
                xyz=float3(x,y,z),
                normal=normalize(float3(x,y,z)),
                uv=get_uv(float3(x,y,z),float2(s,t),cubeUV)
            ]]
    var indices : array<int>
    var k1, k2 : int
    for i in range(stackCount)
        k1 = i * (sectorCount + 1)
        k2 = k1 + sectorCount + 1
        for j in range(sectorCount)
            if i != 0
                frag.indices |> push(k1)
                frag.indices |> push(k2)
                frag.indices |> push(k1 + 1)
            if i != (stackCount-1)
                frag.indices |> push(k1 + 1)
                frag.indices |> push(k2)
                frag.indices |> push(k2 + 1)
            ++k1
            ++k2
    frag.prim = GeometryFragmentType triangles
    return <- frag

enum GenDirection
    xy
    xz
    yz

def private apply_gen_direction_tm ( plt : GenDirection; var frag:GeometryFragment )
    var tm : float3x3
    identity(tm)
    if plt==GenDirection xy
        pass
    elif plt==GenDirection xz
        swap(tm[1],tm[2])
    else    // yz
        swap(tm[0],tm[2])       // todo: verify
        tm[2][0] = -1.
    for vtx in frag.vertices
        vtx.xyz = tm * vtx.xyz
        vtx.normal = normalize ( tm*vtx.normal )

def gen_plane ( plt : GenDirection )
    var frag : GeometryFragment
    frag.vertices <- [{GeometryPreviewVertex
        xyz=float3( 1, 1, 0), normal=float3(  0, 0, 1),  uv=float2(0,0);
        xyz=float3(-1, 1, 0), normal=float3(  0, 0, 1),  uv=float2(1,0);
        xyz=float3(-1,-1, 0), normal=float3(  0, 0, 1),  uv=float2(1,1);
        xyz=float3( 1,-1, 0), normal=float3(  0, 0, 1),  uv=float2(0,1)
    }]
    apply_gen_direction_tm(plt, frag)
    frag.indices <- [{int 0; 1; 2; 2; 3; 0 }]
    frag.prim = GeometryFragmentType triangles
    return <- frag

def private get_unit_circle_veritces ( sectorCount:int )
    let sectorStep = 2. * PI / float(sectorCount)
    var unitCircleVertices : array<float3>
    unitCircleVertices |> reserve(sectorCount+1)
    for i in range(sectorCount+1)
        let sectorAngle = sectorStep * float(i)
        unitCircleVertices |> push(float3(cos(sectorAngle), sin(sectorAngle), 0.))
    return <- unitCircleVertices

def gen_cylinder(plt:GenDirection; sectorCount:int)
    var frag : GeometryFragment
    var unitVertices <- get_unit_circle_veritces(sectorCount)
    // vertices
    for tb in range(2)
        let isTB = tb!=0
        for i in range(2)
            let h = float(i) * 2. - 1.
            let t = 1.0 - float(i)
            for j in range(sectorCount+1)                   // sectorCount + 1 vertices for top or bottom
                let u = unitVertices[j]
                let s = float(j) / float(sectorCount)
                frag.vertices |> push([[GeometryPreviewVertex
                    xyz = float3(u.x,u.y,h),
                    normal = isTB ? normalize(float3(0.,0.,h)) : normalize(float3(u.x,u.y,0.)),
                    uv = float2(s, t)
                ]])
            frag.vertices |> push([[GeometryPreviewVertex           // + center
                xyz = float3(0.,0.,h),
                normal = normalize(float3(0.,0.,h)),
                uv = float2(0.5, 0.5)
            ]])
    apply_gen_direction_tm(plt, frag)
    // indices
    for i in range(sectorCount)
        let k1 = i
        let k2 = i + sectorCount + 2
        // triangle 1
        frag.indices |> push(k1)
        frag.indices |> push(k2)
        frag.indices |> push(k1+1)
        // triangle 2
        frag.indices |> push(k2)
        frag.indices |> push(k2+1)
        frag.indices |> push(k1+1)
    // top
    let tbOfs = (sectorCount + 2) * 2
    let kc1 = sectorCount + 1 + tbOfs
    for i in range(sectorCount)
        var k = i + tbOfs
        frag.indices |> push(kc1)
        frag.indices |> push(k)
        frag.indices |> push(k+1)
    // bottom
    let kc2 = ((sectorCount + 2) + sectorCount + 1) + tbOfs
    for i in range(sectorCount)
        var k = (sectorCount + 2) + i + tbOfs
        frag.indices |> push(kc2)
        frag.indices |> push(k+1)
        frag.indices |> push(k)
    delete unitVertices
    frag.prim = GeometryFragmentType triangles
    return <- frag

def gen_cube
    // cube ////////
    //    v6----- v5
    //   /|      /|
    //  v1------v0|
    //  | |     | |
    //  | |v7---|-|v4
    //  |/      |/
    //  v2------v3
    var frag : GeometryFragment
    frag.vertices <- [{GeometryPreviewVertex
        xyz=float3( 1, 1, 1), normal=float3(  0, 0, 1),  uv=float2(0,0);              // v0 (front)
        xyz=float3(-1, 1, 1), normal=float3(  0, 0, 1),  uv=float2(1,0);              // v1
        xyz=float3(-1,-1, 1), normal=float3(  0, 0, 1),  uv=float2(1,1);              // v2
        xyz=float3( 1,-1, 1), normal=float3(  0, 0, 1),  uv=float2(0,1);              // v3
        xyz=float3( 1, 1, 1), normal=float3(  1, 0, 0),  uv=float2(0,0);              // v0 (right)
        xyz=float3( 1,-1, 1), normal=float3(  1, 0, 0),  uv=float2(1,0);              // v3
        xyz=float3( 1,-1,-1), normal=float3(  1, 0, 0),  uv=float2(1,1);              // v4
        xyz=float3( 1, 1,-1), normal=float3(  1, 0, 0),  uv=float2(0,1);              // v5
        xyz=float3( 1, 1, 1), normal=float3(  0, 1, 0),  uv=float2(0,0);              // v0 (top)
        xyz=float3( 1, 1,-1), normal=float3(  0, 1, 0),  uv=float2(1,0);              // v5
        xyz=float3(-1, 1,-1), normal=float3(  0, 1, 0),  uv=float2(1,1);              // v6
        xyz=float3(-1, 1, 1), normal=float3(  0, 1, 0),  uv=float2(0,1);              // v1
        xyz=float3(-1, 1, 1), normal=float3( -1, 0, 0),  uv=float2(0,0);              // v1 (left)
        xyz=float3(-1, 1,-1), normal=float3( -1, 0, 0),  uv=float2(1,0);              // v6
        xyz=float3(-1,-1,-1), normal=float3( -1, 0, 0),  uv=float2(1,1);              // v7
        xyz=float3(-1,-1, 1), normal=float3( -1, 0, 0),  uv=float2(0,1);              // v2
        xyz=float3(-1,-1,-1), normal=float3(  0,-1, 0),  uv=float2(0,0);              // v7 (bottom)
        xyz=float3( 1,-1,-1), normal=float3(  0,-1, 0),  uv=float2(1,0);              // v4
        xyz=float3( 1,-1, 1), normal=float3(  0,-1, 0),  uv=float2(1,1);              // v3
        xyz=float3(-1,-1, 1), normal=float3(  0,-1, 0),  uv=float2(0,1);              // v2
        xyz=float3( 1,-1,-1), normal=float3(  0, 0,-1),  uv=float2(0,0);              // v4 (back)
        xyz=float3(-1,-1,-1), normal=float3(  0, 0,-1),  uv=float2(1,0);              // v7
        xyz=float3(-1, 1,-1), normal=float3(  0, 0,-1),  uv=float2(1,1);              // v6
        xyz=float3( 1, 1,-1), normal=float3(  0, 0,-1),  uv=float2(0,1) }]            // v5
    frag.indices <- [{int
         0; 1; 2;   2; 3; 0;      // front
         4; 5; 6;   6; 7; 4;      // right
         8; 9;10;  10;11; 8;      // top
        12;13;14;  14;15;12;      // left
        16;17;18;  18;19;16;      // bottom
        20;21;22;  22;23;20 }]    // back
    frag.prim = GeometryFragmentType triangles
    return <- frag

def gen_image_checkerboard(w,h:int; c1,c2:uint)
    var surface <- [[ImageSurface width=w, height=h]]
    surface.pixels |> resize(w*h)
    for y in range(h)
        for x in range(w)
            surface.pixels[y*w+x] = ((x^y)&1)!=0 ? c1 : c2
    return <- surface

