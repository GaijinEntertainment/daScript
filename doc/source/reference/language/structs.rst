.. _structs:


======
Struct
======

.. index::
    single: Structs

daScript implements a structure mechanism similar to languages like C/C++, Java, C#, etc.
However there are some important difference.
Structures are first class objects like integer or strings and can be stored in
table slots, other structures, local variables, arrays, tuples, variants, etc, and passed as function parameters.

------------------
Struct Declaration
------------------

.. index::
    pair: declaration; Struct
    single: Struct Declaration

A structure object is created through the keyword 'struct' . ::

    struct Foo
        x, y: int
        xf: float

Sturctures can be `private` or `public` ::

    struct private Foo
        x, y: int

    struct public Bar
        xf: float

If not specified structures inherit module publicity. I.e. in public modules structures are public,
and in private modules structures are private.

Structures instances are created through a 'new expression' or a 'variable declaration statement'::

    let foo: Foo
    let foo: Foo? = new Foo

There are intentionally no 'member functions'. There are only data members for it is a data-type itself.
Structure can handle function typed member as data (meaning it's pointer can be changed during execution).
There are initializers allowing to simplify writing complex structure initialization.
Basically function with same name as structure itself will work as a initializer.
Compiler will generate 'default' initializer if there are any members with initializer::

    struct Foo
        x: int = 1
        y: int = 2

Structure fields are also initialized as zero by default, regardless of 'initializers' for members, unless you specifically call the initializer::

    let fZero : Foo     // no initializer is called, x, y = 0
    let fInited = Foo() // initializer is called, x = 1, y = 2

Structure field types are inferred, where possible::

    struct Foo
        x = 1    // inferred as int
        y = 2.0    // inferred as float

Explicit structure initialization during creation will leave all uninitialized members zeroed::

    let fExplicit = [[Foo x=13]]  // x = 13, y = 0

the previous code example is a syntactic sugar for::

    let fExplicit: Foo
    fExplicit.x = 13

Post construction initialization only needs to specify overwritten fields::

    let fPostConstruction = [[Foo() x=13]]  // x = 13, y = 2

the previous code example is a syntactic sugar for::

    let fPostConstruction: Foo
    fPostConstruction.x = 13
    fPostConstruction.y = 2

"Clone initializer" is useful patter for creating clone of an existing structure, when both structures are on the heap::

    def Foo ( p : Foo? )                // "clone initializer" takes pointer to existing structure
        var self := *p
        return <- self
    ...
    let a = new [[Foo x=1, y=2.]]       // create new instance of Foo on the heap, initialize it
    let b = new Foo(a)                  // clone of b is created here

--------------------------
Structure Function Members
--------------------------

daScript doesn't have embedded structure member functions, virtual (that can be overridden in inherited structures) or non-virtual.
Those features are implemented for classes.
For ease of Objected Oriented Programming non-virtual member functions can be easily emulated with the pipe operator ``|>``::

    struct Foo
        x, y: int = 0

    def setXY(var thisFoo: Foo; X, Y: int)
        with thisFoo
            x = X
            y = Y

    var foo: Foo
    foo |> setXY(10, 11)   // this is syntactic sugar for setXY(foo, 10, 11)
    setXY(foo, 10, 11)     // exactly same thing as the line above

Since function pointer is a thing, one can emulate 'virtual' functions by storing function pointers as members::

    struct Foo
        x, y: int = 0
        set = @@setXY

    def setXY(var thisFoo: Foo; X, Y: int)
        with thisFoo
            x = X
            y = Y
    ...
    var foo: Foo = Foo()
    foo->set(1, 2)  // this one can call something else, if overridden in derived class.
                    // It is also just syntactic sugar for function pointer call
    invoke(foo.set, foo, 1, 2)  // exactly same thing as above

This makes explicit difference between virtual and non-virtual calls in OOP paradigm.
In fact daScript classes implement virtual functions in exactly that manner.

-----------
Inheritance
-----------

.. index::
    pair: inheritance; Struct
    single: Inheritance

daScript's structure support single inheritance by adding the ' : ', followed by parent structure name in the structure declaration.
The syntax for a derived struct is the following ::

    struct Bar: Foo
        yf: float

When a derived structure is declared, daScript first copies all base's members to the
new structure and then proceeds with evaluating the rest of the declaration.

A derived structure has all members of it's base structure. It is just a syntax sugar for copying all members manually first.

.. _structs_alignment:

---------
Alignment
---------

Structure size and alignment are similar to that of the C++

* individual members are aligned individually
* overall structure alignment is that of the largest member alignment

Inherited structure alignment can be controlled via [cpp_layout] annotation::

    [cpp_layout (pod=false)]
    struct CppS1
        vtable : void?              // we are simulating C++ class
        b : int64 = 2l
        c : int = 3

    [cpp_layout (pod=false)]
    struct CppS2 : CppS1            // d will be aligned on the class bounds
        d : int = 4

---
OOP
---

There is sufficient amount of infrastructure to support basic OOP on top of the structures.
However its already available in form of classes with some fixed memory overhead (see :ref:`Classes <classes>`).

It's possible to override method of the base class by override syntax.
Here an example: ::

    struct Foo
        x, y: int = 0
        set = @@Foo_setXY

    def Foo_setXY(var this: Foo; x, y: int)
        this.x = x
        this.y = y

    struct Foo3D: Foo
        z: int = 3
        override set = cast<auto> @@Foo3D_setXY

    def Foo3D_setXY(var thisFoo: Foo3D; x, y: int)
        thisFoo.x = x
        thisFoo.y = y
        thisFoo.z = -1

It is safe to use 'cast' keyword to cast derived structure instance to reference to it's parent type::

    var f3d: Foo3D = Foo3D()
    (cast<Foo> f3d).y = 5

It is unsafe to 'cast' to cast base struct to it's derived child::

    var f3d: Foo3D = Foo3D()
    def foo(var foo: Foo)
        (cast<Foo3D> foo).z = 5  // error, won't compile

if needed, the upcast can be used with unsafe keyword::

    struct Foo
        x: int

    struct Foo2:Foo
        y: int

    def setY(var foo: Foo; y: int)  // Warning! Can make awful things to your app if its not really Foo2
        unsafe
            (upcast<Foo2> foo).y = y

As the example above is very dangerous, and in order to make it safer, you can modify it to following::

    struct Foo
        x: int
        typeTag: uint = hash("Foo")

    struct Foo2:Foo
        y: int
        override typeTag: uint = hash("Foo2")

    def setY(var foo: Foo; y: int)  // this won't do anything really bad, but will panic on wrong reference
        unsafe
            if foo.typeTag == hash("Foo2")
                (upcast<Foo2> foo).y = y
                print("Foo2 type references was passed\n")
            else
                assert(false, "Not Foo2 type references was passed\n")

