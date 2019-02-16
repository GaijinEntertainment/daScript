options log = true // , logInferPasses=true

struct Point
    x, y : float = 0.0f
    magnitude : function<(p:Point):float> = @point_magnitude

def Point(X,Y)
    return [[Point() x=X,y=Y]]

def point_magnitude(p:Point):float
    return p.x*p.x + p.y*p.y

[export]
def test
    let p  = Point(2.,3.)
    let t = invoke(p.magnitude, p)
    print("t={t}\n")
    return true


