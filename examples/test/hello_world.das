require UnitTest

[export]
def test
    var foo : array<Point3>
    push(foo,float3(1,2,3))
    push(foo,float3(2,3,4))
    push(foo,float3(3,4,5))
    temp_array_alias_example(foo) <| $(tfoo)
        var f = float3(0)
        for it in tfoo
            f += it
        assert(f==float3(6,9,12))
    return true
