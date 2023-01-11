require llvm/llvm_jit

require daslib/unroll

require math
require testProfile
//ported from https://benchmarksgame-team.pages.debian.net/benchmarksgame/program/nbody-gcc-2.html

let
    SOLAR_MASS = 4.*PI*PI

let
    DAYS_PER_YEAR = 365.24

struct body
    x:float4
    v:float4

var
    g_bodies = [[body
            /*sun*/
              x=float4(0,0,0,0), v=float4(0.,0.,0., SOLAR_MASS);
            /*jupiter*/
              x=float4(4.84143144246472090,-1.16032004402742839,-1.03622044471123109e-01,0.),
              v=float4(1.66007664274403694e-03 * DAYS_PER_YEAR, 7.69901118419740425e-03 * DAYS_PER_YEAR, -6.90460016972063023e-05 * DAYS_PER_YEAR, 9.54791938424326609e-04 * SOLAR_MASS);
            /*saturn*/
              x = float4(8.34336671824457987e+00, 4.12479856412430479e+00, -4.03523417114321381e-01,0.),
              v = float4( -2.76742510726862411e-03 * DAYS_PER_YEAR, 4.99852801234917238e-03 * DAYS_PER_YEAR, 2.30417297573763929e-05 * DAYS_PER_YEAR, 2.85885980666130812e-04 * SOLAR_MASS);
            /*uranus*/
              x = float4( 1.28943695621391310e+01, -1.51111514016986312e+01, -2.23307578892655734e-01,0.),
              v = float4( 2.96460137564761618e-03 * DAYS_PER_YEAR, 2.37847173959480950e-03 * DAYS_PER_YEAR, -2.96589568540237556e-05 * DAYS_PER_YEAR, 4.36624404335156298e-05 * SOLAR_MASS);
            /*neptune*/
              x = float4( 1.53796971148509165e+01, -2.59193146099879641e+01, 1.79258772950371181e-01,0.),
              v = float4( 2.68067772490389322e-03 * DAYS_PER_YEAR, 1.62824170038242295e-03 * DAYS_PER_YEAR, -9.51592254519715870e-05 * DAYS_PER_YEAR, 5.15138902046611451e-05 * SOLAR_MASS)
            ]]
let
    nbodies = 5

[jit]
def offset_momentum(var bodies:body[5])
    var px:float3
    for b in bodies
        px -= b.v.xyz * b.v.w
    bodies[0].v.xyz = px / SOLAR_MASS

[jit,hint(alwaysinline,hot,noalias=bodies,unsafe_range_check)]
def advance(var bodies:body[5])
    unroll <|
        for i in range(nbodies)
            var b:body = unsafe(bodies[i])
            unroll <|
                for j in range(i+1, nbodies)
                    var b2:body& = unsafe(bodies[j])
                    let dx = b.x - b2.x
                    let inv_distance = inv_length(dx.xyz)
                    let mag = inv_distance * inv_distance * inv_distance
                    b.v -= dx * (b2.v.w*mag)
                    b2.v += dx * (b.v.w*mag)
            b.x.xyz += b.v.xyz
            bodies[i] = b

[jit]
def energy(var bodies:body[5])
    var e = 0.0
    var i = 0
    for b in bodies
        e += 0.5 * b.v.w * length_sq(b.v.xyz)
        ++i
        for j in range(i, nbodies)
            let b2 = bodies[j]
            e -= (b.v.w * b2.v.w) / distance(b.x.xyz, b2.x.xyz)
    return e

[jit]
def scale_bodies(scale; var bodies:body[5])
    for b in bodies
        b.v.w *= scale * scale
        b.v.xyz *= scale

[jit]
def nbodies(n:int)
    scale_bodies(0.01,g_bodies)
    for i in range(n)
        advance(g_bodies)
    scale_bodies(1./0.01,g_bodies)

[export]
def main
    offset_momentum(g_bodies)
    // print("\ninitial energy {energy(g_bodies)}\n")
    energy(g_bodies)
    profile(10, "n-bodies vectorized jit")  <|
        nbodies(500000)
    // print("\nresult energy {energy(g_bodies)}\n")
    energy(g_bodies)
    testNBodiesInit()
    profile(10, "n-bodies, C++")  <|
        testNBodiesS(500000)

