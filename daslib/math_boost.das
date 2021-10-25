options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4

module math_boost shared public

// _rh, _lh - right handed vs left handed
//  OpenGL, Vulkan - RH

require math public

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

def RGBA_TO_UCOLOR(x,y,z,w:float)
    return pack_float_to_byte(float4(x,y,z,w)*255.)

def UCOLOR_TO_RGBA(x:uint)
    return float4(
        float((x>>16u)&255u)/255.0,
        float((x>> 8u)&255u)/255.0,
        float((x>> 0u)&255u)/255.0,
        float((x>>24u)&255u)/255.0)

def UCOLOR_TO_RGB(x:uint)
    return float3(
        float((x>>16u)&255u)/255.0,
        float((x>> 8u)&255u)/255.0,
        float((x>> 0u)&255u)/255.0)

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

def look_at_lh ( Eye,At,Up:float3 )
    let zaxis = normalize(At - Eye)
    let xaxis = normalize(cross(Up, zaxis))
    let yaxis = cross(zaxis, xaxis)
    var tm : float4x4
    tm[0] = float4(xaxis.x,yaxis.x,zaxis.x,0.)
    tm[1] = float4(xaxis.y,yaxis.y,zaxis.y,0.)
    tm[2] = float4(xaxis.z,yaxis.z,zaxis.z,0.)
    tm[3] = float4( -dot(xaxis, Eye),
                    -dot(yaxis, Eye),
                    -dot(zaxis, Eye),1.)
    return tm

def look_at_rh ( Eye,At,Up:float3 )
    let zaxis = normalize(Eye - At)
    let xaxis = normalize(cross(Up, zaxis))
    let yaxis = cross(zaxis, xaxis)
    var tm : float4x4
    tm[0] = float4(xaxis.x,yaxis.x,zaxis.x,0.)
    tm[1] = float4(xaxis.y,yaxis.y,zaxis.y,0.)
    tm[2] = float4(xaxis.z,yaxis.z,zaxis.z,0.)
    tm[3] = float4( -dot(xaxis, Eye),
                    -dot(yaxis, Eye),
                    -dot(zaxis, Eye),1.)
    return tm

def perspective_lh ( fovy,aspect,zn,zf : float )
	let yScale = 1. / tan(fovy/2.)
    let xScale = yScale / aspect
    var Result : float4x4
    identity(Result)
    Result[0][0] = xScale
    Result[1][1] = yScale
    Result[2][2] = zf/(zf-zn)
    Result[2][3] = -1.0
    Result[3][2] = -zn*zf/(zn-zf)
    return Result

def perspective_rh ( fovy,aspect,zn,zf : float )
	let yScale = 1. / tan(fovy/2.)
    let xScale = yScale / aspect
    var Result : float4x4
    identity(Result)
    Result[0][0] = xScale
    Result[1][1] = yScale
    Result[2][2] = zf/(zn-zf)
    Result[2][3] = -1.0
    Result[3][2] = zn*zf/(zn-zf)
    return Result

def perspective_rh_opengl ( fovy,aspect,zn,zf : float )
	let yScale = 1. / tan(fovy/2.)
    let xScale = yScale / aspect
    var Result : float4x4
    identity(Result)
    Result[0][0] = xScale
    Result[1][1] = yScale
    Result[2][2] = (zf+zn)/(zn-zf)
    Result[2][3] = -1.0
    Result[3][2] = 2.*zn*zf/(zn-zf)
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
