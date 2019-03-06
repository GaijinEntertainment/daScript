// options log=true, logNodes=true

require math
require PathTracerHelper

let
    PI : float const = 3.14159265358979323846

let
    DO_LENGTH : int const = 9
    DO_SAMPLES_PER_PIXEL : int const = 4
    DO_LIGHT_SAMPLING : bool const = true
    LIGHT_SPHERE_ID : int const = 8

def schlick(cosine,ri:float)
    let r0 = (1. - ri) / (1. + ri);
    r0 = r0 * r0;
    return r0 + (1. - r0) * pow(1. - cosine, 5.);

def xorShift32(state:uint&)
    let x = state
    x ^= x << 13u
    x ^= x >> 17u
    x ^= x << 15u
    state = x
    return x

def randomFloat01(state:uint&)
    return float(xorShift32(state) & 0xFFFFFF) / 16777216.0

def reflect(v,n)
    return v - float(2.) * dot(v, n) * n

def refract(v,n:float3;nint:float;outRefracted:float3&)
    let dt = dot(v,n)
    let discr = 1. - nint*nint*(1.-dt*dt)
    if discr > 0.
        outRefracted = nint*(v-n*dt)-n*sqrt(discr)
        return true
    return false

def randomUnitVector(state:uint&)
    let z = randomFloat01(state) * 2.0 - 1.0
    let a = randomFloat01(state) * 2.0 * PI
    let r = sqrt(1.0f - z * z)
    let x = sin(a)
    let y = cos(a)
    return float3(r * x, r * y, z)

def randomInUnitSphere(state:uint&)
    while true
        let p = 2.0 * float3(randomFloat01(state),randomFloat01(state),randomFloat01(state)) - float3(1)
        if lengthSq(p) <= 1.0
            return p

def randomInUnitDisk(state:uint&)
    while true
        let p = 2.0 * float3(randomFloat01(state),randomFloat01(state),0.) - float3(1,1,0)
        if lengthSq(p) <= 1.0
            return p

struct Sphere
    center : float3
    radius : float

struct Ray
    orig : float3
    dir  : float3

def pointAt(ray:Ray const;t:float)
    return ray.orig + ray.dir*t

struct Hit
    pos    : float3
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
    let this : Camera
    this.lensRadius = aperture / 2.
    let theta = vfov * PI / 180.
    let halfHeight = tan(theta / 2.)
    let halfWidth = aspect * halfHeight
    this.origin = lookFrom
    this.w = normalize(lookFrom - lookAt)
    this.u = normalize(cross(vup, this.w))
    this.v = cross(this.w, this.u)
    this.lowerLeftCorner = this.origin - halfWidth*focusDist*this.u - halfHeight*focusDist*this.v - focusDist*this.w
    this.horizontal = 2. * halfWidth * focusDist * this.u
    this.vertical = 2. * halfHeight * focusDist * this.v
    return this

def getRay(this:Camera;s,t:float;state:uint&)
    let rd = this.lensRadius * randomInUnitDisk(state)
    let offset = this.u*rd.x + this.v*rd.y
    return [[Ray orig = this.origin + offset,
        dir = normalize(this.lowerLeftCorner + s*this.horizontal + t*this.vertical - this.origin - offset) ]]

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
    s_Spheres = [[Sphere[9]
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
    s_SphereMats = [[Material[9]
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

let
    kMinT = 0.001
    kMaxT = 1.0e7f
    kMaxDepth = 10

def hitSpheres(r:Ray const;tMin,tMax:float;outHit:Hit)
    let hitT = tMax
    let id = -1
    let i = 0
    for sph in s_Spheres
        let co = sph.center - r.orig
        let nb = dot(co,r.dir)
        let c = lengthSq(co) - sph.radius*sph.radius    // todo: sqRadius?
        let discr = nb*nb - c
        if discr > 0.
            let discrSq = sqrt(discr)
            let t = nb - discrSq
            if t<=tMin
                t = nb + discrSq
            if t>tMin & t<hitT
                id = i
                hitT = t
        ++ i
    if id != -1
        outHit.pos = pointAt(r,hitT)
        outHit.normal = (outHit.pos - s_Spheres[id].center) / s_Spheres[id].radius  // todo: invRadius?
        outHit.t = hitT
    return id

def hitWorld(r:Ray const;tMin,tMax:float;outHit:Hit;outID:int&)
    outID = hitSpheres(r,tMin,tMax,outHit)
    return outID != -1

def scatter(mat:Material const; r_in:Ray const; rec:Hit; attenuation:float3&; scattered:Ray; outLightE:float3&; inoutRayCount:int&; state:uint&)
    outLightE = float3(0)
    if mat.mtype==Type Lambert
        let target = rec.pos + rec.normal + randomUnitVector(state)
        scattered = [[Ray orig=rec.pos, dir=normalize(target - rec.pos)]]
        attenuation = mat.albedo
        if DO_LIGHT_SAMPLING
            let s = s_Spheres[LIGHT_SPHERE_ID]
            let sw = normalize(s.center-rec.pos)
            let su = normalize(cross(abs(sw.x)>0.01 ? float3(0,1,0) : float3(1,0,0),sw))
            let sv = cross(sw,su)
            let cosAMax = sqrt(max(0.,1.-s.radius*s.radius/lengthSq(rec.pos-s.center)))
            let eps1 = randomFloat01(state)
            let eps2 = randomFloat01(state)
            let cosA = 1. - eps1 + eps1 * cosAMax
            let sinA = sqrt ( 1. - cosA*cosA )
            let phi = 2. * PI * eps2
            let l = su*cos(phi)*sinA + sv*sin(phi)*sinA + sw*cosA
            let lightHit : Hit
            let hitID = 0
            ++inoutRayCount
            let ray = [[Ray orig=rec.pos, dir=l]]
            if hitWorld(ray,kMinT,kMaxT,lightHit,hitID) & hitID==LIGHT_SPHERE_ID
                let omega = 2.*PI*(1.-cosAMax)
                let rdir = r_in.dir
                // todo: assert is_normalized(rdir)
                let nl = dot(rec.normal,rdir) < 0.  ? rec.normal : -rec.normal
                outLightE += (mat.albedo*s_SphereMats[LIGHT_SPHERE_ID].emissive) * ( max(0.,dot(l,nl))*omega/PI )
        return true
    elif mat.mtype == Type Metal
        // Debug.Assert(r_in.dir.IsNormalized); Debug.Assert(rec.normal.IsNormalized);
        let refl = reflect(r_in.dir,rec.normal)
        scattered = [[Ray orig=rec.pos, dir=normalize(refl + mat.roughness*randomInUnitSphere(state))]]
        attenuation = mat.albedo
        return dot(scattered.dir,rec.normal) > 0.
    elif mat.mtype == Type Dielectric
        // Debug.Assert(r_in.dir.IsNormalized); Debug.Assert(rec.normal.IsNormalized);
        let outwardN : float3
        let rdir = r_in.dir
        let refl = reflect(rdir, rec.normal)
        let nint : float
        attenuation = float3(1)
        let refr : float3
        let reflProb : float
        let cosine : float
        if dot(rdir,rec.normal)>0.
            outwardN = -rec.normal
            nint = mat.ri
            cosine = mat.ri * dot(rdir, rec.normal)
        else
            outwardN = rec.normal
            nint = 1.0 / mat.ri
            cosine = - dot(rdir, rec.normal)
        if refract(rdir,outwardN,nint,refr)
            reflProb = schlick(cosine,mat.ri)
        else
            reflProb = 1.
        if randomFloat01(state) < reflProb
            scattered = [[Ray orig=rec.pos, dir=normalize(refl)]]
        else
            scattered = [[Ray orig=rec.pos, dir=normalize(refr)]]
    else
        attenuation = float3(1,0,1)
        scattered = [[Ray orig=float3(0.), dir=float3(0.)]]
        return false
    return true

def trace(r:Ray;depth:int;inoutRayCount:int&;state:uint&;doMaterialE:bool = true)
    let rec : Hit
    let id = 0
    ++inoutRayCount
    if hitWorld(r,kMinT,kMaxT,rec,id)
        let scattered : Ray
        let attenuation : float3
        let lightE : float3
        let mat : Material & = s_SphereMats[id]
        let matE = mat.emissive
        if depth<kMaxDepth & scatter(mat,r,rec,attenuation,scattered,lightE,inoutRayCount,state)
            if DO_LIGHT_SAMPLING
                if !doMaterialE
                    matE = float3(0)
                doMaterialE = mat.mtype != Type Lambert
            return matE + lightE + attenuation * trace(scattered,depth+1,inoutRayCount,state,doMaterialE)
        else
            return matE
    else
        let unitDir = r.dir
        let t = 0.5 * (unitDir + float3(1.))
        return  lerp(0.3*float3(1.), 0.3*float3(0.5, 0.7, 1.0), t)

def trace(screenWidth,screenHeight,frameCount:int;backbuffer:array<float3>;cam:Camera)
    let invWidth = 1. / float(screenWidth)
    let invHeight = 1. / float(screenHeight)
    let lerpFac = float3(float(frameCount) / float(frameCount + 1))
    let rayCount = 0
    let backbufferIdx = 0
    for y in range(0,screenHeight)
        let state : uint = uint((y*9781 + frameCount*6271) | 1)
        for x in range(0,screenWidth)
            let col = float3(0)
            for s in range(0,DO_SAMPLES_PER_PIXEL)
                let u = (float(x) + randomFloat01(state)) * invWidth
                let v = (float(y) + randomFloat01(state)) * invHeight
                let r = getRay(cam, u, v, state)
                col += trace(r, 0, rayCount, state)
            col *= 1. / float(DO_SAMPLES_PER_PIXEL)
            backbuffer[backbufferIdx] = lerp(col, backbuffer[backbufferIdx], lerpFac)
            backbufferIdx ++
    return rayCount

def draw(time:float;frameCount,screenWidth,screenHeight:int;backbuffer:array<float3>;outRayCount:int&)
    let rayCount = 0
    let lookfrom = float3(0,2,3)
    let lookat = float3(0)
    let distToFocus = 3.
    let aperture = 0.1
    let cam = Camera(lookfrom,lookat,float3(0,1,0),60.,float(screenWidth)/float(screenHeight),aperture,distToFocus)
    rayCount += trace(screenWidth,screenHeight,frameCount,backbuffer,cam)
    outRayCount = rayCount

[export]
def test
    let width = 320
    let height = 240
    let kFrameCount = 16
    let backbuffer : array<float3>
    resize(backbuffer,width*height)
    let s = 0.
    let totalRayCount = 0
    print("\ntracing...\n")
    let sw0 = getClock()
    for i in range(0,kFrameCount)
        let rayCount = 0
        draw(s, i, width, height, backbuffer, rayCount)
        totalRayCount += rayCount
        let sw1 = getClock()
        s = sw1 - sw0
        let frames = i + 1
        let ms = int(s / float(frames) * 1000.)
        let mrayS = float(totalRayCount) / s * 1.0e-6f
        let mrayFr = float(rayCount) * 10e-6f
        print("{ms}ms\t{mrayS} Mrays/s\t{mrayFr} Mrays/frame\tframes {frames}\n")
    saveTga("path_tracer.tga",backbuffer,width,height)
    return true

