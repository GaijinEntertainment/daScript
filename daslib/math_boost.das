options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4

module math_boost shared

require math

struct AABR
    min, max : float2

struct AABB
    min, max : float3

struct Ray
    dir, origin : float3

def degrees(f:float)
    return f*180./PI

def radians(f:float)
    return f*PI/180.

def is_intersecting ( a, b : AABR ) // A.LO<=B.HI && A.HI>=B.LO
    return (a.min.x<=b.max.x && a.min.y<=b.max.y &&
        a.max.x>=b.min.x && a.max.y>=b.min.y)

def is_intersecting ( a, b : AABB )  // A.LO<=B.HI && A.HI>=B.LO
    return (a.min.x<=b.max.x && a.min.y<=b.max.y && a.min.z<=b.max.z &&
        a.max.x>=b.min.x && a.max.y>=b.min.y && a.max.z>=b.min.z)

def is_intersecting ( ray:Ray; aabb:AABB; Tmin:float=0.; Tmax:float=FLT_MAX )
    let invD = rcp(ray.dir)
	let t0s = (aabb.min - ray.origin) * invD
  	let t1s = (aabb.max - ray.origin) * invD
  	let tsmaller = min(t0s, t1s);
    let tbigger  = max(t0s, t1s);
    let tmin = max(Tmin, max(tsmaller[0], max(tsmaller[1], tsmaller[2])))
    let tmax = min(Tmax, min(tbigger[0], min(tbigger[1], tbigger[2])))
	return tmin < tmax
