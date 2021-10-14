options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4

module math_boost shared

require math

struct AARect
    min, max : float2

struct AABox
    min, max : float3

def degrees(f:float)
    return f*180./PI

def radians(f:float)
    return f*PI/180.

def is_intersecting ( a, b : AARect ) // A.LO<=B.HI && A.HI>=B.LO
    return (a.min.x<=b.max.x && a.min.y<=b.max.y &&
        a.max.x>=b.min.x && a.max.y>=b.min.y)

def is_intersecting ( a, b : AABox )  // A.LO<=B.HI && A.HI>=B.LO
    return (a.min.x<=b.max.x && a.min.y<=b.max.y && a.min.z<=b.max.z &&
        a.max.x>=b.min.x && a.max.y>=b.min.y && a.max.z>=b.min.z)
