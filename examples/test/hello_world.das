options log = true // , logNodes = true // , optimize=true, printRef = true, printVarAccess = true, logInferPasses = true

struct Foo
    x, y = 0
    set = @Foo_setXY

def Foo_setXY(this:Foo;x,y:int)
    this.x = x
    this.y = y

struct Foo3D : Foo
    z = 3
    override set = cast<auto> @Foo3D_setXY

def Foo3D_setXY(this:Foo3D;x,y:int)
    this.x = x
    this.y = y
    this.z = 0

[export]
def test
    let f = Foo()
    f->set(1,2)
    assert(f.x==1 & f.y==2)
    let f3d = Foo3D()
    assert(f3d.x==0 & f3d.y==0 & f3d.z==3)
    f3d->set(1,2)
    assert(f3d.x==1 & f3d.y==2 & f3d.z==0)
    return true
