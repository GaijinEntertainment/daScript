options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module sdf public

require daslib/math_boost

def private dot2( v:float2 )
    return dot(v,v)

def private dot2 ( v:float3 )
    return dot(v,v)

def private ndot ( a,b:float2 )
    return a.x*b.x - a.y*b.y

def sdPlane ( p:float3 )
    return p.y

def sdSphere( p:float3; s:float )
    return length(p)-s

def sdBox ( p,b:float3 )
    let d = abs(p) - b
    return min(max(d.x,max(d.y,d.z)),0.0) + length(max(d,float3(0.0)))

def sdBoundingBox ( var p,b:float3; e:float )
    p = abs(p)-b
    let q = abs(p+float3(e))-float3(e)
    return min(min(
        length(max(float3(p.x,q.y,q.z),float3(0.0)))+min(max(p.x,max(q.y,q.z)),0.0),
        length(max(float3(q.x,p.y,q.z),float3(0.0)))+min(max(q.x,max(p.y,q.z)),0.0)),
        length(max(float3(q.x,q.y,p.z),float3(0.0)))+min(max(q.x,max(q.y,p.z)),0.0))

def sdEllipsoid ( p,r:float3 )
    let k0 = length(p/r)
    let k1 = length(p/(r*r))
    return k0*(k0-1.0)/k1

def sdTorus( p:float3; t:float2  )
    return length( float2(length(p.xz)-t.x,p.y) )-t.y

def sdCappedTorus ( var p:float3; sc:float2; ra,rb:float )
    p.x = abs(p.x)
    let k = (sc.y*p.x>sc.x*p.y) ? dot(p.xy,sc) : length(p.xy)
    return sqrt( dot(p,p) + ra*ra - 2.0*ra*k ) - rb

def sdHexPrism( var p:float3; h:float2 )
    let q = abs(p)
    let k = float3(-0.8660254, 0.5, 0.57735)
    p = abs(p)
    p.xy -= 2.0*min(dot(k.xy, p.xy), 0.0)*k.xy;
    let d = float2(
       length(p.xy - float2(clamp(p.x, -k.z*h.x, k.z*h.x), h.x))*sign(p.y - h.x),
       p.z-h.y )
    return min(max(d.x,d.y),0.0) + length(max(d,float2(0.0)))

def sdOctogonPrism ( var p:float3; r,h:float )
    let k = float3(-0.9238795325,   // sqrt(2+sqrt(2))/2
                        0.3826834323,   // sqrt(2-sqrt(2))/2
                        0.4142135623 ); // sqrt(2)-1
    p = abs(p)
    p.xy -= 2.0*min(dot(float2( k.x,k.y),p.xy),0.0)*float2( k.x,k.y)
    p.xy -= 2.0*min(dot(float2(-k.x,k.y),p.xy),0.0)*float2(-k.x,k.y)
    p.xy -= float2(clamp(p.x, -k.z*r, k.z*r), r)
    let d = float2( length(p.xy)*sign(p.y), p.z-h )
    return min(max(d.x,d.y),0.0) + length(max(d,float2(0.0)));

def sdCapsule( p,a,b:float3; r:float  )
	let pa = p-a
    let ba = b-a
	let h = saturate( dot(pa,ba)/dot(ba,ba) )
	return length( pa - ba*h ) - r

def sdRoundCone ( p:float3; r1,r2,h:float )
    let q = float2( length(p.xz), p.y )
    let b = (r1-r2)/h
    let a = sqrt(1.0-b*b)
    let k = dot(q,float2(-b,a))
    if k < 0.0
        return length(q) - r1
    elif k > a*h
        return length(q-float2(0.0,h)) - r2
    else
        return dot(q, float2(a,b) ) - r1

def sdRoundCone ( p,a,b:float3; r1,r2:float )
    let  ba = b - a
    let l2 = dot(ba,ba)
    let rr = r1 - r2
    let a2 = l2 - rr*rr
    let il2 = 1.0/l2
    let pa = p - a
    let y = dot(pa,ba)
    let z = y - l2
    let x2 = dot2( pa*l2 - ba*y )
    let y2 = y*y*l2
    let z2 = z*z*l2
    let k = sign(rr)*rr*rr*x2
    if sign(z)*a2*z2 > k
        return  sqrt(x2 + z2)*il2 - r2
    elif sign(y)*a2*y2 < k
        return  sqrt(x2 + y2)*il2 - r1
    else
        return (sqrt(x2*a2*il2)+y*rr)*il2 - r1

def sdTriPrism ( var p:float3; var h:float2 )
    let k = sqrt(3.0)
    h.x *= 0.5*k
    p.xy /= h.x
    p.x = abs(p.x) - 1.0
    p.y = p.y + 1.0/k
    if p.x+k*p.y>0.0
        p.xy=float2(p.x-k*p.y,-k*p.x-p.y)/2.0
    p.x -= clamp( p.x, -2.0, 0.0 )
    let d1 = length(p.xy)*sign(-p.y)*h.x
    let d2 = abs(p.z)-h.y
    return length(max(float2(d1,d2),float2(0.0))) + min(max(d1,d2), 0.)

def sdCylinder ( p:float3; h:float2 )
    let d = abs(float2(length(p.xz),p.y)) - h
    return min(max(d.x,d.y),0.0) + length(max(d,float2(0.0)))

def sdCylinder ( p,a,b:float3; r:float )
    let pa = p - a
    let ba = b - a
    let baba = dot(ba,ba)
    let paba = dot(pa,ba)
    let x = length(pa*baba-ba*paba) - r*baba
    let y = abs(paba-baba*0.5)-baba*0.5
    let x2 = x*x
    let y2 = y*y*baba
    let d = (max(x,y)<0.0)?-min(x2,y2):(((x>0.0)?x2:0.0)+((y>0.0)?y2:0.0))
    return sign(d)*sqrt(abs(d))/baba

def sdCone ( p:float3; c:float2; h:float )
    let q = h*float2(c.x,-c.y)/c.y
    let w = float2( length(p.xz), p.y )
	let a = w - q*saturate( dot(w,q)/dot(q,q) )
    let b = w - q*float2( saturate( w.x/q.x ), 1.0 )
    let k = sign( q.y )
    let d = min(dot( a, a ),dot(b, b))
    let s = max( k*(w.x*q.y-w.y*q.x),k*(w.y-q.y)  )
	return sqrt(d)*sign(s)

def sdCappedCone ( p:float3; h,r1,r2:float )
    let q = float2( length(p.xz), p.y )
    let k1 = float2(r2,h)
    let k2 = float2(r2-r1,2.0*h)
    let ca = float2(q.x-min(q.x,(q.y < 0.0)?r1:r2), abs(q.y)-h)
    let cb = q - k1 + k2*saturate( dot(k1-q,k2)/dot2(k2) )
    let s = (cb.x < 0.0 && ca.y < 0.0) ? -1.0 : 1.0
    return s*sqrt( min(dot2(ca),dot2(cb)) )

def sdCappedCone ( p,a,b:float3; ra,rb:float )
    let rba  = rb-ra
    let baba = dot(b-a,b-a)
    let papa = dot(p-a,p-a)
    let paba = dot(p-a,b-a)/baba
    let x = sqrt( papa - paba*paba*baba )
    let cax = max(0.0,x-((paba<0.5)?ra:rb))
    let cay = abs(paba-0.5)-0.5
    let k = rba*rba + baba
    let f = saturate( (rba*(x-ra)+paba*baba)/k)
    let cbx = x-ra - f*rba
    let cby = paba - f
    let s = (cbx < 0.0 && cay < 0.0) ? -1.0 : 1.0
    return s*sqrt( min(cax*cax + cay*cay*baba,
                       cbx*cbx + cby*cby*baba) )

def sdSolidAngle ( pos:float3; c:float2; ra:float )
    let p = float2( length(pos.xz), pos.y )
    let l = length(p) - ra
	let m = length(p - c*clamp(dot(p,c),0.0,ra) )
    return max(l,m*sign(c.y*p.x-c.x*p.y))

def sdOctahedron( var p:float3; s:float )
    p = abs(p)
    let m = p.x + p.y + p.z - s
 	var q : float3
    if 3.0*p.x < m
        q = p.xyz
    elif 3.0*p.y < m
        q = p.yzx
    elif 3.0*p.z < m
        q = p.zxy
    else
        return m*0.57735027
    let k = clamp(0.5*(q.z-q.y+s),0.0,s)
    return length(float3(q.x,q.y-s+k,q.z-k))

def sdPyramid( var p:float3; h:float )
    let m2 = h*h + 0.25
    p.x = abs(p.x)
    p.z = abs(p.z)
    if p.z > p.x
        let px = p.x
        let pz = p.z
        p.x = pz
        p.z = px
    p.x -= 0.5
    p.z -= 0.5
    let q = float3( p.z, h*p.y - 0.5*p.x, h*p.x + 0.5*p.y)
    let s = max(-q.x,0.0)
    let t = clamp( (q.y-0.5*p.z)/(m2+0.25), 0.0, 1.0 )
    let a = m2*(q.x+s)*(q.x+s) + q.y*q.y
	let b = m2*(q.x+0.5*t)*(q.x+0.5*t) + (q.y-m2*t)*(q.y-m2*t)
    let d2 = min(q.y,-q.x*m2-q.y*0.5) > 0.0 ? 0.0 : min(a,b)
    return sqrt( (d2+q.z*q.z)/m2 ) * sign(max(q.z,-p.y))

def sdRhombus ( var p:float3; la,lb,h,ra:float )
    p = abs(p)
    let b = float2(la,lb)
    let f = clamp( (ndot(b,b-2.0*p.xz))/dot(b,b), -1.0, 1.0 )
	let q = float2(length(p.xz-0.5*b*float2(1.0-f,1.0+f))*sign(p.x*b.y+p.z*b.x-b.x*b.y)-ra, p.y-h)
    return min(max(q.x,q.y),0.0) + length(max(q,float2(0.0)))
