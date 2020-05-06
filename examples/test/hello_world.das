options log = true

class Foo
    x, y : int = 0
    def set(X,Y:int)
        x = X
        y = Y

class Foo3D : Foo
    z : int = 3
    override def set(X,Y:int)
        Foo`set(cast<Foo> self,X,Y)
        z = 0

[export]
def test
    var f = Foo()
    f->set(1,2)
    assert(f.x==1 & f.y==2)
    var f3d = Foo3D()
    assert(f3d.x==0 & f3d.y==0 & f3d.z==3)
    f3d->set(1,2)
    assert(f3d.x==1 & f3d.y==2 & f3d.z==0)
    f3d.z = 13
    f3d->set(3,4)
    assert(f3d.x==3 & f3d.y==4 & f3d.z==0)
    return true
