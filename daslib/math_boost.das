options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4

module math_boost shared public

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

def look_at ( cameraPosition,cameraTarget,cameraUpVector:float3 )
    let zaxis = -normalize(cameraTarget - cameraPosition)
    let xaxis = normalize(cross(zaxis,cameraUpVector))
    let yaxis = cross(xaxis, zaxis)
    var tm : float4x4
    tm[0] = float4(xaxis.x,yaxis.x,zaxis.x,0.)
    tm[1] = float4(xaxis.y,yaxis.y,zaxis.y,0.)
    tm[2] = float4(xaxis.z,yaxis.z,zaxis.z,0.)
    tm[3] = float4( -dot(xaxis, cameraPosition),
                    -dot(yaxis, cameraPosition),
                    -dot(zaxis, cameraPosition),1.)
    return tm

def perspective_rh ( fovy,aspect,zNear,zFar : float )
	let tanHalfFovy = tan(fovy / 2.0);
    var Result : float4x4
    identity(Result)
    Result[0][0] = 1.0 / (aspect * tanHalfFovy)
    Result[1][1] = 1.0 / (tanHalfFovy)
    Result[2][2] = - (zFar + zNear) / (zFar - zNear)
    Result[2][3] = - 1.0
    Result[3][2] = - ( 2.0 * zFar * zNear) / (zFar - zNear)
    return Result

def ortho_rh ( left,right,bottom,top,zNear,zFar:float )
    var Result : float4x4
    identity(Result)
    Result[0][0] =  2. / ( right - left )
    Result[1][1] =  2. / ( top - bottom )
    Result[2][2] = -2. / ( zFar - zNear )
    Result[3][0] = - ( right + left ) / ( right - left )
    Result[3][1] = - ( top + bottom ) / ( top - bottom )
    Result[3][2] = - ( zFar + zNear ) / ( zFar - zNear )
    return Result

def plane_dot ( Plane,Vec:float4 )
    return dot(Plane,Vec)

def plane_normalize ( Plane:float4 )
    let len = length(Plane.xyz)
    return len!=0.0 ? Plane / len : float4(0)

def plane_from_point_normal ( p, n : float3 )
    return float4(n.x,n.y,n.z,-dot(p,n))

def planar_shadow ( Light, Plane : float4 )
    let P = plane_normalize(Plane)
    let L = Light
    let d = -dot(P, L)
    var Result : float4x4
    Result[0] = float4(P.x * L.x + d,  P.x * L.y,      P.x * L.z,      P.x * L.w)
    Result[1] = float4(P.y * L.x,      P.y * L.y + d,  P.y * L.z,      P.y * L.w)
    Result[2] = float4(P.z * L.x,      P.z * L.y,      P.z * L.z + d,  P.z * L.w)
    Result[3] = float4(P.w * L.x,      P.w * L.y,      P.w * L.z,      P.w * L.w + d)
    return Result
/*

    let NPlane = plane_normalize(Plane)
    let Dot = plane_dot(NPlane, Light);
    var Result : float4x4
    Result[0][0] = Dot - NPlane.x * Light.x
    Result[0][1] = -NPlane.x * Light.y
    Result[0][2] = -NPlane.x * Light.z
    Result[0][3] = -NPlane.x * Light.w
    Result[1][0] = -NPlane.y * Light.x
    Result[1][1] = Dot - NPlane.y * Light.y
    Result[1][2] = -NPlane.y * Light.z
    Result[1][3] = -NPlane.y * Light.w
    Result[2][0] = -NPlane.z * Light.x
    Result[2][1] = -NPlane.z * Light.y
    Result[2][2] = Dot - NPlane.z * Light.z
    Result[2][3] = -NPlane.z * Light.w
    Result[3][0] = -NPlane.w * Light.x
    Result[3][1] = -NPlane.w * Light.y
    Result[3][2] = -NPlane.w * Light.z
    Result[3][3] = Dot - NPlane.w * Light.w
    return Result
*/