struct Foo
    x, y = 0
    set = @Foo_setXY
    set2 = @Foo_setXY

def Foo_setXY(this:Foo;x,y:int)
    this.x = x
    this.y = y

struct Foo3D : Foo
    z = 3
    override set = cast<auto> @Foo3D_setXY
    override set2 = @Foo3D_setXY2

def Foo3D_setXY(this:Foo3D;x,y:int)
    this.x = x
    this.y = y
    this.z = 0

[unsafe]
def Foo3D_setXY2(that:Foo;x,y:int)
    let this : Foo3D& = upcast<Foo3D> that
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
    f3d.z = 13
    f3d->set2(3,4)
    assert(f3d.x==3 & f3d.y==4 & f3d.z==0)
    return true
