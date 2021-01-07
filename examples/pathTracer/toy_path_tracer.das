// options log
// options logCpp
// options log_nodes

module tpt

require math
require random
require strings
require daslib/tga
require daslib/jobque_boost

let
    USE_JOB_QUE = true

let
    DO_SAMPLES_PER_PIXEL = 4
    LIGHT_SPHERE_ID = 8

let
    kMinT = 0.001
    kMaxT = 1.0e7f
    kMaxDepth = 10

var
    RAND_SEED : int4

def schlick(cosine,ri:float)
    var r0 = (1. - ri) / (1. + ri)
    return lerp ( r0*r0, 1., pow(1. - cosine,5.))

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

let
    TOTAL_SPHERES = 9

var
    s_Spheres = [[Sphere[TOTAL_SPHERES]
        center = float3(0.,-100.5,-1.), radius = 100.;
        center = float3(2.,0.,-1.),     radius = 0.5;
        center = float3(0.,0.,-1.),     radius = 0.5;
        center = float3(-2.,0.,-1.),    radius = 0.5;
        center = float3(2.,0.,1.),      radius = 0.5;
        center = float3(0.,0.,1.),      radius = 0.5;
        center = float3(-2.,0.,1.),     radius = 0.5;
        center = float3(0.5,1.,0.5),    radius = 0.5;
        center = float3(-1.5,1.5,0.),   radius = 0.3
    ]]

let
    s_SphereMats = [[Material[TOTAL_SPHERES]
        mtype = Type Lambert,    albedo = float3(0.8, 0.8, 0.8), emissive = float3(0,0,0),     roughness = 0.,  ri = 0.;
        mtype = Type Lambert,    albedo = float3(0.8, 0.4, 0.4), emissive = float3(0,0,0),     roughness = 0.,  ri = 0.;
        mtype = Type Lambert,    albedo = float3(0.4, 0.8, 0.4), emissive = float3(0,0,0),     roughness = 0.,  ri = 0.;
        mtype = Type Metal,      albedo = float3(0.4, 0.4, 0.8), emissive = float3(0,0,0),     roughness = 0.,  ri = 0.;
        mtype = Type Metal,      albedo = float3(0.4, 0.8, 0.4), emissive = float3(0,0,0),     roughness = 0.,  ri = 0.;
        mtype = Type Metal,      albedo = float3(0.4, 0.8, 0.4), emissive = float3(0,0,0),     roughness = 0.2, ri = 0.;
        mtype = Type Metal,      albedo = float3(0.4, 0.8, 0.4), emissive = float3(0,0,0),     roughness = 0.6, ri = 0.;
        mtype = Type Dielectric, albedo = float3(0.4, 0.4, 0.4), emissive = float3(0,0,0),     roughness = 0.,  ri = 1.5;
        mtype = Type Lambert,    albedo = float3(0.8, 0.6, 0.2), emissive = float3(30,25,15),  roughness = 0.,  ri = 0.
    ]]

def hitSpheres(r:Ray const;tMin,tMax:float;var outHit:Hit)
    var hitT = tMax
    var id = -1
    var i = 0
    for sph in s_Spheres
        let co = sph.center - r.orig
        let nb = dot(co,r.dir)
        let discr = mad(nb,nb, sph.radius2 - length_sq(co))
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

def hitWorld(r:Ray const;tMin,tMax:float;var outHit:Hit;var outID:int&)
    outID = r|>hitSpheres(tMin,tMax,outHit)
    return outID != -1

def scatter(mat:Material const; r_in:Ray const; rec:Hit; var attenuation:float3&; var scattered:Ray; var outLightE:float3&; var inoutRayCount:int&)
    outLightE = float3(0)
    if mat.mtype==Type Lambert
        let target = rec.pos + rec.normal + random_unit_vector(RAND_SEED)
        scattered = [[Ray orig=rec.pos, dir=normalize(target - rec.pos)]]
        attenuation = mat.albedo
        let s : Sphere & = s_Spheres[LIGHT_SPHERE_ID]
        let sw = normalize(s.center-rec.pos)
        let su = normalize(cross(abs(sw.x)>0.01 ? float3(0,1,0) : float3(1,0,0),sw))
        let sv = cross(sw,su)
        let cosAMax = sqrt(saturate(1.-s.radius*s.radius*inv_distance_sq(rec.pos,s.center)))
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
        if dot(r_in.dir,rec.normal)>0.
            if refract(r_in.dir,-rec.normal,mat.ri,refr)
                reflProb = schlick(mat.ri * dot(r_in.dir, rec.normal),mat.ri)
            else
                reflProb = 1.
        else
            if refract(r_in.dir,rec.normal,1.0/mat.ri,refr)
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

def trace(ray:Ray;depth:int;var inoutRayCount:int&;var doMaterialE:bool = true)
    var rec : Hit
    var id = 0
    ++inoutRayCount
    if ray|>hitWorld(kMinT,kMaxT,rec,id)
        var scattered : Ray
        var attenuation : float3
        var lightE : float3
        let mat : Material & = s_SphereMats[id]
        var matE = mat.emissive
        if depth<kMaxDepth && mat|>scatter(ray,rec,attenuation,scattered,lightE,inoutRayCount)
            if !doMaterialE
                matE = float3(0)
            doMaterialE = mat.mtype != Type Lambert
            return  mad ( attenuation, trace(scattered,depth+1,inoutRayCount,doMaterialE), matE + lightE )
        else
            return matE
    else
        let t = mad(ray.dir,0.5,float3(1.))
        return  lerp(0.3*float3(1.), 0.3*float3(0.5, 0.7, 1.0), t)

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
                col += cam |> getRay(uv.x,uv.y) |> trace(0, rayCount)
            col *= 1. / float(DO_SAMPLES_PER_PIXEL)
            backbuffer[backbufferIdx] = lerp(col, backbuffer[backbufferIdx], lerpFac)
            backbufferIdx ++
    return rayCount

def draw(frameCount,screenWidth,screenHeight,ymin,ymax:int;var backbuffer:array<float3>;var outRayCount:int&)
    var rayCount = 0
    let lookfrom = float3(0,2,3)
    let lookat = float3(0)
    let distToFocus = 3.
    let aperture = 0.1
    let cam = Camera(lookfrom,lookat,float3(0,1,0),60.,float(screenWidth)/float(screenHeight),aperture,distToFocus)
    rayCount += trace(screenWidth,screenHeight,frameCount,ymin,ymax,backbuffer,cam)
    outRayCount = rayCount

def prepare
    for sph in s_Spheres
        sph.radius2 = sph.radius * sph.radius
        sph.iRadius = 1. / sph.radius

[export]
def main
    let width = 320
    let height = 240
    let kFrameCount = 16
    var backbuffer : array<float3>
    resize(backbuffer,width*height)
    print("\ntracing...\n")
    var i = 0
    if USE_JOB_QUE
        with_job_que <|
            var t0 = ref_time_ticks()
            var totalRays = 0
            profile(kFrameCount,"ray-trace frame") <|
                var pbb : array<float3>?
                var ptr : int?
                unsafe
                    pbb = addr(backbuffer)                  // NOTE: we actually share data here
                    ptr = addr(totalRays)
                let hwJobs = get_total_hw_jobs()
                with_job_status(hwJobs) <| $ ( status )
                    let chunk = (height+hwJobs-1) / hwJobs
                    for c in range(hwJobs)
                        let yMin = c * chunk
                        let yMax = min((c + 1)*chunk, height)
                        new_job <| @
                            prepare()
                            var rayCount = 0
                            draw(i, width, height, yMin, yMax, *pbb, rayCount)
                            *ptr += rayCount
                            status |> notify
                    status |> join
                    i ++
            var dt = get_time_usec(t0)
            var sec = double(dt)/1000000.0lf
            print("{totalRays} in {sec} sec, {format("%.3f",double(totalRays)/double(dt))} mrays/sec\n")
    else
        prepare()
        profile(kFrameCount,"ray-trace frame") <|
            var rayCount = 0
            draw(i, width, height, 0, height, backbuffer, rayCount)
            i ++
    save_TGA("path_tracer.tga",width,height,backbuffer)
