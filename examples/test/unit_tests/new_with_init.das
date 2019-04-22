struct Ball
    center : float3 = float3(7,8,9)
    radius : float = 1.234f

def Ball(center:float3; radius:float)
    var b : Ball
    b.center = center
    b.radius = radius
    return b

[export,unsafe]
def test
	var in scope b0 = new Ball
	assert(b0.center==float3() & b0.radius==0.0f)
    var in scope b1 = new Ball()
	assert(b1.center==float3(7,8,9) & b1.radius==1.234f)
    var in scope b2 = new Ball(float3(1,2,3),4.0f)
    assert(b2.center==float3(1,2,3) & b2.radius==4.0f)
    var in scope b3 = new Ball[3] (float3(3,2,1),0.5f)
    for b in b3
		assert(b.center==float3(3,2,1) & b.radius==0.5f)
    return true

