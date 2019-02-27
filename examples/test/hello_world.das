// options log = true, logNodes = true, optimize=true, printRef = true, printVarAccess = true, logOptimizationPasses = true

options log=true

struct Foo
    x, y : int
    set = @Foo_setXY

def Foo_setXY(this:Foo;x,y:int)
    this.x = x
    this.y = y

struct Foo3D : Foo
    z : int = 3
    override set = @Foo3D_setXY

def Foo3D_setXY(foo:Foo;x,y:int)
    let this = cast<Foo3D> foo
    this.x = x
    this.y = x

[export]
def test
    let f = Foo()
    f->set(1,2)
    assert(f.x==1 & f.y==2)
    let f3d = Foo3D()
    f3d->set(1,2)
    assert(f3d.x==1 & f3d.y==1 & f3d.z==3)
    return true
