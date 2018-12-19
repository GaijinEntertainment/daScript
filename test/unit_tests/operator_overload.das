// TODO:
//  when we can return & and when we can do return-by-copy and return-by-move
//  then we can do proper operators

struct iVec2
    x,y:int

def operator - ( a: iVec2 ) : int
    return -a.x

def operator + ( a,b:iVec2 ) : int
    return a.x + b.x

def test:bool
    let a,b:iVec2
    a.x = 4
    b.x = 6
    assert(-a==-4)
    assert(a+b==10)
    return true
