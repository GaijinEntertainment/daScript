options log=true, logStack=true, optimize=false

[export]
def test
    let t = translation(float3(1,2,3))
	print("t={t}")
    return true
