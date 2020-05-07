var
    delFoo = 0
    delFoo3D = 0

class Foo
    x, y : int = 0
    [export] def Foo                    // custom initializer
        Foo`set(self,1,1)
    def Foo(T:int)                      // custom initializer
        self->set(T,T)                  // calling 'polymorph' method
    def Foo(X,Y:int)                    // custom initializer
        Foo`set(self,X,Y)
    def set(X,Y:int)                    // inline method variable
        x = X
        y = Y
    def ups()
        x = -1
        y = -1
    def finalize                        // custom finalizer
        delFoo ++

class Foo3D : Foo
    z : int = 13
    def Foo3D                           // overriding default initializer
        assert(x==0 & y==0)             // Foo initializer is NOT called by default
        assert(z==13)
        Foo`Foo(cast<Foo> self)         // call parents initializer explicitly
        assert(x==1 & y==1)
        z = 3
    def override set(X,Y:int)           // overriding method variable
        Foo`set(cast<Foo> self,X,Y)     // calling generated method function directly
        z = 0
    def override ups()                  // for testing 'polymorphism'
        x = -2
        y = -2
        z = -2
    def finalize                        // custom finalizer
        delete cast<Foo> self           // calling Foo finalizer
        delFoo3D ++

def test_foo
    var f = Foo()
    assert(f.x==1 & f.y==1)
    f->set(1,2)
    assert(f.x==1 & f.y==2)
    f = Foo(11,12)
    assert(f.x==11 & f.y==12)
    f = Foo(13)
    assert(f.x==13 & f.y==13)
    var f3d = Foo3D()
    assert(f3d.x==1 & f3d.y==1 & f3d.z==3)
    f3d->set(1,2)
    assert(f3d.x==1 & f3d.y==2 & f3d.z==0)
    f3d.z = 13
    f3d->set(3,4)
    assert(f3d.x==3 & f3d.y==4 & f3d.z==0)
    assert(delFoo==0 & delFoo3D==0)
    delete f
    assert(delFoo==1 & delFoo3D==0)
    delete f3d
    assert(delFoo==2 & delFoo3D==1)
    delFoo = 0
    delFoo3D = 0

[unsafe]
def test_foo_ptr
    var f = new Foo()
    f->ups()
    assert(f.x==-1 & f.y==-1)
    assert(delFoo==0 & delFoo3D==0)
    delete f
    assert(delFoo==1 & delFoo3D==0)
    f = cast<Foo?> new Foo3D()
    f->ups()
    assert(f.x==-2 & f.y==-2)
    delete f
    assert(delFoo==2 & delFoo3D==1)
    delFoo = 0
    delFoo3D = 0

[export]
def test
    test_foo()
    test_foo_ptr()
    return true
