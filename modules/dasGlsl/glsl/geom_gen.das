options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module geom_gen public

require daslib/safe_addr
require daslib/strings_boost
require fio
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

def private s_to_int ( s : string )
    return empty(s) ? -1 : int(s)

def private split_sptn ( sptn:string ) : tuple<p:int;t:int;n:int>
    var s : string
    var ptn : array<string>
    for ch in sptn
        if ch=='/'
            ptn |> push(s)
            s = ""
        else
            s += to_char(ch)
    if !empty(s)
        ptn |> push(s)
    if length(ptn)==1
        return [[auto s_to_int(ptn[0]),-1,-1]]
    else
        return [[auto s_to_int(ptn[0]), s_to_int(ptn[1]), s_to_int(ptn[2])]]

def load_obj_mesh ( fname:string )
    var frag : GeometryFragment
    frag.prim = GeometryFragmentType triangles
    var v : array<float3>
    var vt : array<float2>
    var vn : array<float3>
    var uvtx : table<string; int>
    fopen(fname,"rb") <| $ ( f )
        while !feof(f)
            let st = strip(fgets(f))
            if st |> starts_with("#")
                continue
            elif st |> starts_with("v ")
                var xyz <- st |> split(" ")
                v |> push(float3(float(xyz[1]),float(xyz[2]),float(xyz[3])))
                delete xyz
            elif st |> starts_with("vt ")
                var xy <- st |> split(" ")
                vt |> push(float2(float(xy[1]),float(xy[2])))
                delete xy
            elif st |> starts_with("vn ")
                var xyz <- st |> split(" ")
                vn |> push(float3(float(xyz[1]),float(xyz[2]),float(xyz[3])))
                delete xyz
            elif st |> starts_with("f ")
                var f012 <- st |> split(" ")
                var v012 <- [{for v in range(1,length(f012)); split_sptn(f012[v])}]
                var i012 : array<int>
                for pvi in v012
                    var vtx = [[GeometryPreviewVertex
                        xyz = pvi.p==-1 ? float3() : v[pvi.p-1],
                        normal = pvi.n==-1 ? float3(1,0,0) : vn[pvi.n-1],
                        uv = pvi.t==-1 ? float2() : vt[pvi.t-1]
                    ]]
                    let svtx = "{vtx}"
                    if uvtx |> key_exists(svtx)
                        i012 |> push(uvtx[svtx])
                    else
                        let i = frag.vertices |> length
                        uvtx[svtx] = i
                        i012 |> push(i)
                        frag.vertices |> push(vtx)
                let li = length(i012)
                if li==3
                    frag.indices |> push(i012[0])
                    frag.indices |> push(i012[1])
                    frag.indices |> push(i012[2])
                elif li==4
                    frag.indices |> push(i012[0])
                    frag.indices |> push(i012[1])
                    frag.indices |> push(i012[2])
                    frag.indices |> push(i012[0])
                    frag.indices |> push(i012[2])
                    frag.indices |> push(i012[3])
                else
                    to_log(LOG_ERROR,"invaid face `{st}`\n")
                delete i012
                delete v012
                delete f012
    return <- frag
