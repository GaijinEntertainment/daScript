struct Point
    x, y  = 0

[export]
def test
    let p = Point()
    with p
        x = 1
        y = 2
    assert(p.x==1 & p.y==2)
    return true

