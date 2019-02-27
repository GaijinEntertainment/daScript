struct Foo
    x, y : int
    set = @Foo_setXY

def Foo_setXY(this:Foo;x,y:int)
    this.x = x
    this.y = y

struct Foo3D : Foo
    z : int = 3
    override set = @Foo3D_setXY

def Foo3D_setXY(that:Foo;x,y:int)
    let this : Foo3D & = upcast<Foo3D> that
    this.x = x
    this.y = y
    this.z = 0

[export]
def test
    let f = Foo()
    f->set(1,2)
    assert(f.x==1 & f.y==2)
    let f3d = Foo3D()
    assert(f3d.z==3)
    f3d->set(1,2)
    assert(f3d.x==1 & f3d.y==2 & f3d.z==0)
    return true

