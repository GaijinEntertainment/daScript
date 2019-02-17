struct Point
    x, y = 0.0f
    magnitude  = @point_magnitude
    dot = @point_dot

def Point(X,Y)
    return [[Point() x=X,y=Y]]

def point_magnitude(this:Point)
    return this.x*this.x + this.y*this.y

def point_dot(this,p:Point)
    return this.x*p.x + this.y*p.y

[export]
def test
    let p  = Point(2.,3.)
    let t = p->magnitude()
    let d = p->dot(Point(1.,1.))
    assert(t==13. & d==5.)
    return true
