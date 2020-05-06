options log = true

class Foo
    x, y : int = 0
    def Foo(X,Y:int)                    // custom c-tor
        x = X
        y = Y
    def set(X,Y:int)                    // method
        x = X
        y = Y

class Foo3D : Foo
    z : int = 13
    def Foo3D                           // overriding default c-tor
        assert(z==13)
        z = 3
    override def set(X,Y:int)           // overriding method
        Foo`set(cast<Foo> self,X,Y)     // calling member function directly
        z = 0

[export]
def test
    var f = Foo()
    f->set(1,2)
    assert(f.x==1 & f.y==2)
    f = Foo(11,12)
    assert(f.x==11 & f.y==12)
    var f3d = Foo3D()
    assert(f3d.x==0 & f3d.y==0 & f3d.z==3)
    f3d->set(1,2)
    assert(f3d.x==1 & f3d.y==2 & f3d.z==0)
    f3d.z = 13
    f3d->set(3,4)
    assert(f3d.x==3 & f3d.y==4 & f3d.z==0)
    return true
