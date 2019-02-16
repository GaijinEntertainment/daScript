options log = true // , logInferPasses=true

struct Point
    x, y : float = 0.0f
    magnitude : function<(p:Point):float> = @point_magnitude
    dot : function<(p,dp:Point):float> = @point_dot

def Point(X,Y)
    return [[Point() x=X,y=Y]]

def point_magnitude(this:Point):float
    return this.x*this.x + this.y*this.y

def point_dot(this,p:Point):float
    return this.x*p.x + this.y*p.y

[export]
def test
    let p  = Point(2.,3.)
    let t = p->magnitude()
    let d = p->dot(Point(1.,1.))
    print("t={t} d={d}\n")
    return true


