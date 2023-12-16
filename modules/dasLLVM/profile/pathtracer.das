// options log
// options logCpp
// options log_nodes


require math
require strings
require stbimage

/*
[jit]
def reflect_jit(i,n:float3)
    return i - 2.0 * n * dot(n,i)

[jit]
def refract_jit( i,n:float3; eta:float )
    let cosi = dot(-i, n)
    let cost2 = 1.0f - eta * eta * (1.0f - cosi*cosi)
    let t = eta*i + ((eta*cosi - sqrt(abs(cost2))) * n)
    return cost2>0. ? t : float3(0.)
*/

// random

let LCG_RAND_MAX = 32767
let LCG_IRAND_MAX_FLT = 1.0f / float(LCG_RAND_MAX)

[jit]
def random_int4 ( var seed:int4& )
    //! random int4, each component is 0..32767 (LCG_RAND_MAX)
    seed = int4(214013)*seed+int4(2531011)
    return (seed>>16) & int4(LCG_RAND_MAX)

[jit]
def random_float4 ( var seed:int4& )
    //! random float4, each component is 0..1
    return float4(random_int4(seed)) * float4(LCG_IRAND_MAX_FLT)

[jit]
def random_in_unit_disk ( var seed:int4& )
    //! random float3 unit vector (length=1) which happens to be inside a disk R=1, Z=0
    while true
        let R = random_float4(seed).xy
        let r = float3(R.x,R.y,0.)
        let p = float3(2.)*r - float3(1.,1.,0.)
        if length_sq(p) <= 1.0
            return p
    return float3(0.)

[jit]
def random_unit_vector ( var seed:int4& )
    //! random float3 unit vector (length=1.)
    let zz = random_float4(seed)
    let z = zz.x * 2. - 1.
    let a = zz.y * ( 2. * PI )
    let r = sqrt(1. - z*z)
    var x, y : float
    sincos(a, x, y)
    return float3( r * x, r * y, z )

[jit]
def random_in_unit_sphere ( var seed:int4& )
    //! random float3 unit vector (length=1) which happens to be inside a sphere R=1
    while true
        let r = random_float4(seed).xyz
        let p = float3(2.)*r - float3(1.)
        if length_sq(p) <= 1.0
            return p
    return float3(0.)

[jit]
def random_float ( var seed:int4& )
    //! random float 0..1
    return float(random_int(seed)) * LCG_IRAND_MAX_FLT

[jit]
def random_seed ( seed:int )
    //! constructs seed vector out of single integer seed
    return int4(seed,seed+1,seed+2,seed+3)

[jit]
def random_int ( var seed:int4& )
    //! random integer 0..32767 (LCG_RAND_MAX)
    seed.x = 214013*seed.x+2531011
    return (seed.x>>16) & LCG_RAND_MAX

// pathtracer

let
    DO_SAMPLES_PER_PIXEL = 32
    LIGHT_SPHERE_ID = 8

let
    kMinT = 0.001
    kMaxT = 1.0e7f
    kMaxDepth = 10

var RAND_SEED : int4

[jit]
def schlick(cosine,ri:float)
    var r0 = (1. - ri) / (1. + ri)
    var t = 1. - cosine
    var t2 = t * t
    return lerp ( r0*r0, 1., t2 * t2 * t)

struct Sphere
    center : float3
    radius : float
    radius2 : float
    iRadius : float
    pad1,pad2 : float

struct Ray
    orig : float3
    pad1 : float
    dir  : float3
    pad2 : float

[jit,hint(alwaysinline)]
def pointAt(ray:Ray const;t:float)
    return ray.orig + ray.dir*t

struct Hit
    pos    : float3
    pad    : float
    normal : float3
    t      : float

struct Camera
    origin          : float3
    lowerLeftCorner : float3
    horizontal      : float3
    vertical        : float3
    u, v, w         : float3
    lensRadius      : float

[jit,hint(alwaysinline)]
def Camera(lookFrom,lookAt,vup:float3;vfov,aspect,aperture,focusDist:float)
    var that : Camera
    let theta = vfov * PI / 180.
    let halfHeight = tan(theta / 2.)
    let halfWidth = aspect * halfHeight
    with that
        lensRadius = aperture / 2.
        origin = lookFrom
        w = normalize(lookFrom - lookAt)
        u = normalize(cross(vup, w))
        v = cross(w, u)
        lowerLeftCorner = origin - halfWidth*focusDist*u - halfHeight*focusDist*v - focusDist*w
        horizontal = 2. * halfWidth * focusDist * u
        vertical = 2. * halfHeight * focusDist * v
    return that

[jit,hint(alwaysinline)]
def getRay(that:Camera;s,t:float)
    with that
        let rd = lensRadius * random_in_unit_disk(RAND_SEED)
        let offset = mad(u,rd.x,v*rd.y)
        return [[Ray orig = origin + offset,
            dir = normalize(mad(vertical,t,mad(horizontal,s,lowerLeftCorner)) - origin - offset) ]]

enum Type
    Lambert
    Metal
    Dielectric

struct Material
    mtype : Type
    albedo : float3
    emissive : float3
    roughness : float
    ri : float

let TOTAL_SPHERES = 9

var [[groupshared]] s_Spheres : Sphere[TOTAL_SPHERES]
var [[groupshared]] s_SphereMats : Material[TOTAL_SPHERES]

def init_path_tracer
    s_Spheres[0] = [[Sphere center = float3(0.,-100.5,-1.), radius = 100.,  radius2=100.0*100., iRadius = 1. / 100.]]
    s_Spheres[1] = [[Sphere center = float3(2.,0.,-1.),     radius = 0.5,   radius2=0.5*0.5,    iRadius = 1. / .5]]
    s_Spheres[2] = [[Sphere center = float3(0.,0.,-1.),     radius = 0.5,   radius2=0.5*0.5,    iRadius = 1. / .5]]
    s_Spheres[3] = [[Sphere center = float3(-2.,0.,-1.),    radius = 0.5,   radius2=0.5*0.5,    iRadius = 1. / .5]]
    s_Spheres[4] = [[Sphere center = float3(2.,0.,1.),      radius = 0.5,   radius2=0.5*0.5,    iRadius = 1. / .5]]
    s_Spheres[5] = [[Sphere center = float3(0.,0.,1.),      radius = 0.5,   radius2=0.5*0.5,    iRadius = 1. / .5]]
    s_Spheres[6] = [[Sphere center = float3(-2.,0.,1.),     radius = 0.5,   radius2=0.5*0.5,    iRadius = 1. / .5]]
    s_Spheres[7] = [[Sphere center = float3(0.5,1.,0.5),    radius = 0.5,   radius2=0.5*0.5,    iRadius = 1. / .5]]
    s_Spheres[8] = [[Sphere center = float3(-1.5,1.5,0.),   radius = 0.3,   radius2=0.3*0.3,    iRadius = 1. / .3]]
    s_SphereMats[0] = [[Material mtype = Type Lambert,    albedo = float3(0.8, 0.8, 0.8), emissive = float3(0,0,0),     roughness = 0.,  ri = 0.]]
    s_SphereMats[1] = [[Material mtype = Type Lambert,    albedo = float3(0.8, 0.4, 0.4), emissive = float3(0,0,0),     roughness = 0.,  ri = 0.]]
    s_SphereMats[2] = [[Material mtype = Type Lambert,    albedo = float3(0.4, 0.8, 0.4), emissive = float3(0,0,0),     roughness = 0.,  ri = 0.]]
    s_SphereMats[3] = [[Material mtype = Type Metal,      albedo = float3(0.4, 0.4, 0.8), emissive = float3(0,0,0),     roughness = 0.,  ri = 0.]]
    s_SphereMats[4] = [[Material mtype = Type Metal,      albedo = float3(0.4, 0.8, 0.4), emissive = float3(0,0,0),     roughness = 0.,  ri = 0.]]
    s_SphereMats[5] = [[Material mtype = Type Metal,      albedo = float3(0.4, 0.8, 0.4), emissive = float3(0,0,0),     roughness = 0.2, ri = 0.]]
    s_SphereMats[6] = [[Material mtype = Type Metal,      albedo = float3(0.4, 0.8, 0.4), emissive = float3(0,0,0),     roughness = 0.6, ri = 0.]]
    s_SphereMats[7] = [[Material mtype = Type Dielectric, albedo = float3(0.4, 0.4, 0.4), emissive = float3(0,0,0),     roughness = 0.,  ri = 1.5]]
    s_SphereMats[8] = [[Material mtype = Type Lambert,    albedo = float3(0.8, 0.6, 0.2), emissive = float3(30,25,15),  roughness = 0.,  ri = 0.]]

[jit,hint(unsafe_range_check)]
def hitSpheres(r:Ray const;tMin,tMax:float;var outHit:Hit)
    var hitT = tMax
    var id = -1
    var i = 0
    for sph in s_Spheres
        let co = sph.center - r.orig
        let nb = dot(co,r.dir)
        let discr = mad(nb,nb,sph.radius2 - length_sq(co))
        if discr > 0.
            let discrSq = sqrt(discr)
            var t = nb - discrSq
            if t<=tMin
                t = nb + discrSq
            if t>tMin && t<hitT
                id = i
                hitT = t
        ++ i
    if id != -1
        let atPos = pointAt(r,hitT)
        outHit = [[Hit pos = atPos,
            normal = (atPos - s_Spheres[id].center) * s_Spheres[id].iRadius,
        t = hitT]]
    return id

[jit,hint(alwaysinline)]
def hitWorld(r:Ray const;tMin,tMax:float;var outHit:Hit;var outID:int&)
    outID = r|>hitSpheres(tMin,tMax,outHit)
    return outID != -1

[jit,hint(unsafe_range_check)]
def scatter(mat:Material const; r_in:Ray const; rec:Hit; var attenuation:float3&; var scattered:Ray; var outLightE:float3&; var inoutRayCount:int&)
    outLightE = float3(0)
    if mat.mtype==Type Lambert
        let target = rec.pos + rec.normal + random_unit_vector(RAND_SEED)
        scattered = [[Ray orig=rec.pos, dir=normalize(target - rec.pos)]]
        attenuation = mat.albedo
        let sc = s_Spheres[LIGHT_SPHERE_ID].center
        let sw = normalize(sc-rec.pos)
        let su = normalize(cross(abs(sw.x)>0.01 ? float3(0,1,0) : float3(1,0,0),sw))
        let sv = cross(sw,su)
        let sr = s_Spheres[LIGHT_SPHERE_ID].radius
        let cosAMax = sqrt(saturate(1.-sr*sr*inv_distance_sq(rec.pos,sc)))
        let eps = random_float4(RAND_SEED)
        let cosA = mad(eps.x,cosAMax,1.) - eps.x
        let sinA = sqrt ( 1. - cosA*cosA )
        var sinPhi, cosPhi : float
        sincos(2. * PI * eps.y, sinPhi, cosPhi)
        let l = su*(cosPhi*sinA) + sv*(sinPhi*sinA) + sw*cosA
        var lightHit : Hit
        var hitID = 0
        ++inoutRayCount
        let ray = [[Ray orig=rec.pos, dir=l]]
        if ray|>hitWorld(kMinT,kMaxT,lightHit,hitID) && hitID==LIGHT_SPHERE_ID
            let nl = dot(rec.normal,r_in.dir) < 0. ? rec.normal : -rec.normal
            outLightE += (mat.albedo*s_SphereMats[LIGHT_SPHERE_ID].emissive) * ( saturate(dot(l,nl))*mad(-2.,cosAMax,2.) )
        return true
    elif mat.mtype == Type Metal
        let refl = reflect(r_in.dir,rec.normal)
        scattered = [[Ray orig=rec.pos, dir=normalize(mad(random_in_unit_sphere(RAND_SEED),mat.roughness,refl))]]
        attenuation = mat.albedo
        return dot(scattered.dir,rec.normal) > 0.
    elif mat.mtype == Type Dielectric
        attenuation = float3(1)
        var refr : float3
        var reflProb : float
        let dott = dot(r_in.dir,rec.normal)
        if dot(r_in.dir,rec.normal) >0. // here
            refr = refract(r_in.dir,-rec.normal,mat.ri)
            if length_sq(refr) > 0.
                reflProb = schlick(mat.ri * dot(r_in.dir, rec.normal),mat.ri)
            else
                reflProb = 1.
        else
            refr = refract(r_in.dir,rec.normal,1.0/mat.ri)
            if length_sq(refr) > 0.
                reflProb = schlick(-dot(r_in.dir, rec.normal),mat.ri)
            else
                reflProb = 1.
        if random_float(RAND_SEED) < reflProb
            scattered = [[Ray orig=rec.pos, dir=normalize(reflect(r_in.dir, rec.normal))]]
        else
            scattered = [[Ray orig=rec.pos, dir=normalize(refr)]]
    else
        attenuation = float3(1,0,1)
        scattered = [[Ray orig=float3(0.), dir=float3(0.)]]
        return false
    return true

[jit,hint(unsafe_range_check)]
def trace(r:Ray;var inoutRayCount:int&)
    var ray = r
    var col = float3(0.)
    var curAtten = float3(1.)
    var doMaterialE = true
    for depth in range(kMaxDepth)
        var rec : Hit
        var id = 0
        ++inoutRayCount
        if ray|>hitWorld(kMinT,kMaxT,rec,id)
            var scattered : Ray
            var attenuation : float3
            var lightE : float3
            var matE = s_SphereMats[id].emissive
            if s_SphereMats[id] |>scatter(ray,rec,attenuation,scattered,lightE,inoutRayCount)
                if !doMaterialE
                    matE = float3(0)
                doMaterialE = s_SphereMats[id].mtype != Type Lambert
                col += curAtten * (matE + lightE)
                curAtten *= attenuation
                ray = scattered
            else
                col += curAtten * matE
                break
        else
            let t = mad(ray.dir,0.5,float3(1.))
            let skyCol = lerp(0.3*float3(1.), 0.3*float3(0.5, 0.7, 1.0), t)
            col += curAtten * skyCol
            break
    return col

[jit,hint(unsafe_range_check)]
def trace(screenWidth,screenHeight,frameCount,ymin,ymax:int;var backbuffer:array<float3>;cam:Camera)
    let invWH = float(1) / float2(screenWidth, screenHeight)
    let lerpFac = float3(float(frameCount) / float(frameCount + 1))
    var rayCount = 0
    var backbufferIdx = ymin * screenWidth
    for y in range(ymin,ymax)
        RAND_SEED = random_seed(y*117 + frameCount*13)
        for x in range(screenWidth)
            var col = float3(0)
            for s in range(DO_SAMPLES_PER_PIXEL)
                let uv = (float2(x,y) + random_float4(RAND_SEED).xy) * invWH
                col += cam |> getRay(uv.x,uv.y) |> trace(rayCount)
            col *= 1. / float(DO_SAMPLES_PER_PIXEL)
            backbuffer[backbufferIdx] = lerp(col, backbuffer[backbufferIdx], lerpFac)
            backbufferIdx ++
    return rayCount

[jit]
def draw(frameCount,screenWidth,screenHeight,ymin,ymax:int;var backbuffer:array<float3>;var outRayCount:int&)
    var rayCount = 0
    let lookfrom = float3(0,2,3)
    let lookat = float3(0)
    let distToFocus = 3.
    let aperture = 0.1
    let cam = Camera(lookfrom,lookat,float3(0,1,0),60.,float(screenWidth)/float(screenHeight),aperture,distToFocus)
    rayCount += trace(screenWidth,screenHeight,frameCount,ymin,ymax,backbuffer,cam)
    outRayCount = rayCount

def linear_to_SRGB ( x:float )
    if x <= 0.00031308
        return 12.92 * x
    else
        return 1.055*pow(x,(1.0 / 2.4) ) - 0.055

def linear_to_SRGB ( c:float4 )
    return float4(linear_to_SRGB(c.x),linear_to_SRGB(c.y),linear_to_SRGB(c.z),c.w)

def RGBA_TO_UCOLOR(xyzw:float4)
    return pack_float_to_byte(xyzw*255.)

[export]
def main
    let width = 256
    let height = 256
    let kFrameCount = 1
    var backbuffer : array<float3>
    resize(backbuffer,width*height)
    print("\ntracing...\n")
    var i = 0

    init_path_tracer()
    // trace_ray(width, height, 132, 0)

    var t0 = ref_time_ticks()
    var totalRays = 0
    draw(i, width, height, 0, height, backbuffer, totalRays)
    var dt = get_time_usec(t0)
    var sec = double(dt)/1000000.0lf
    print("JIT: {totalRays} in {sec} sec, {format("%.3f",double(totalRays)/double(dt))} mrays/sec\n")

    var pixels : array<uint>
    pixels |> reserve(width*height)
    for y in range(height)
        for x in range(width)
            let srgb = linear_to_SRGB(float4(backbuffer[x + (height-1-y)*width].xyz,1.0f))
            pixels |> push <| RGBA_TO_UCOLOR(srgb)
    let path = "{get_das_root()}/modules/dasLLVM/profile/path_tracer.png"
    unsafe
        stbi_write_png(path, width, height, 4, addr(pixels[0]), width*4  )
        print("image saved to {path}\n")

