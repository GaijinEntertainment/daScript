.. _structs:


=================
Structs
=================

.. index::
    single: Structs

daScript implements a struct mechanism similar to languages like C/C++, Java, C#, etc.
however because it differs in several aspects.
Structs are first class objects like integer or strings and can be stored in
table slots, structs and local variables, arrays and passed as function parameters.

------------------
Struct Declaration
------------------

.. index::
    pair: declaration; Struct
    single: Struct Declaration


A struct object is created through the keyword 'struct' . ::

    struct Foo
        x, y: int
        xf: float

Structs instances are created through a 'new expression' or a 'variable declatation statement'::

    let foo: Foo
    let foo: Foo? = new Foo

There are intentionally no 'member functions', only data members, as it is data-type itself.
However, struct can handle function typed member as data (meaning it's pointer can be changed during execution).
There are kinda 'constructors', allowing to simplify writing structure initialization with complex time.
Basically function with same name as struct itself will work as a constructor.
Compiler will generate 'default' constructor if you have some of members with initializator::

    struct Foo
        x: int = 1
        y: int = 2

Structs are also initialized as zero by default, regardless of 'initializators' for members, unless you specfically call constructor::

    let fZero:Foo       // no constructor is called, x, y = 0
    let fInited = Foo() // constructor is called, x = 1, y = 2
	
Structure field types are infered, where possible::

	stryct Foo
		x = 1	// infered as integer
		y = 2.	// infered as float

Explicit structure initialization during creation will left all un-inited members zeroed::

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

-----------------------
Struct Function Members
-----------------------

daScript doesn't have specific struct member functions, virtual (that can be overriden in derived structs) or non-virtual.
For ease of Objected Oriented Programming, non-virtual member functions can be easily emulated with pipe operator '|>'::

    struct Foo
        x, y: int = 0

    def setXY(var this: Foo; x, y: int)
        this.x = x
        this.y = y
    ...
    var foo: Foo
    foo |> setXY(10, 11)   // this is syntactic sugar for setXY(foo, 10, 11)
    setXY(foo, 10, 11)     // exactly same thing as above line

Since function pointer is a thing, one can emulate 'virtual' functions by storing function pointers as member::

    struct Foo
        x, y: int = 0
        set = setXY

    def setXY(var this: Foo; x, y: int)
        this.x = x
        this.y = y
    ...
    var foo: Foo = Foo()
    foo->set(1, 2)  // this one can call something else, if overriden in derived class.
                    // It is also just syntactic sugar for function pointer call
    invoke(foo.set, foo, 1, 2)  // exactly same thing as above

This makes explicit difference between virtual and non-virtuall calls in OOP paradigm.

-----------
Inheritance
-----------

.. index::
    pair: inheritance; Struct
    single: Inheritance

daScript's struct support single inheritance by adding the ' : ', followed by parent struct name, in the struct declaration.
The syntax for a derived struct is the following ::

    struct Bar: Foo
        yf: float

When a derived struct is declared, daScript first copies all base's members in the
new struct then proceeds with evaluating the rest of the declaration.

A derived struct has all members of it's base. It is just a sytax sugar for copying all members manually first.

OOP.
It's possible to override method of the base class by override syntax.
Here an example: ::

    struct Foo
        x, y: int = 0
        set = @Foo_setXY

    def Foo_setXY(var this: Foo; x, y: int)
        this.x = x
        this.y = y

    struct Foo3D: Foo
        z: int = 3
        override set = cast<auto> @Foo3D_setXY

    def Foo3D_setXY(var this: Foo3D; x, y: int)
        this.x = x
        this.y = y
        this.z = -1

It is safe to use 'cast' keyword to cast derived struct instance to reference to it's parent type::

    var f3d: Foo3D = Foo3D()
    (cast<Foo> f3d).y = 5

It is unsafe to 'cast' to cast base struct to it's dereived ::

    var f3d: Foo3D = Foo3D()
    def foo(foo: Foo)
      (cast<Foo3d> foo).z = 5  // error, won't compile

if needed, the upcast can be used with [unsafe] annotation ::

    struct Foo
      x: int
    struct Foo2
      y: int
    [unsafe]
    def setY(foo: Foo; y: int)  // Warning! Can make awful things to your app if not-really Foo2 is passed!
      (upcast<Foo3d> foo).y = y

As the example above is very dangerours, and in order to make it safer, you can modify it to following::

    struct Foo
      x: int
      typeTag: uint = hash("Foo")

    struct Foo2
      y: int
      typeTag: uint = hash("Foo2")

    [unsafe]
    def setY(foo: Foo; y: int)  // this won't do anything really bad, but will panic on wrong reference
        if foo.typeTag == hash("Foo2")
            (cast<Foo3d> foo).y = y
        else
            assert(0, "Not Foo2 type references was passed")
