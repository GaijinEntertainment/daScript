.. _clone:

=====
Clone
=====

Clone infrastructure is designed to implement deep copy of the data.
Cloning is invoked via clone operator ``:=``::

    a := b

Cloning can be also invoked via clone initializer for the variable declaration::

    var x := y

which in turn expands into ``clone_to_move``::

    var x <- clone_to_move(y)

(see :ref:`clone_to_move <clone_to_move>`).

----------------------------------------
Cloning rules and implementation details
----------------------------------------

Cloning obeys the following rules.

Certain types like block, lambda, or iterator can't be cloned at all.

However if the custom clone function exists then its immediately called regardless of type cloneability::

    struct Foo
        a : int

    def clone ( var x : Foo; y : Foo )
        x.a = y.a
        print("cloned\n")

    var l = [[Foo a=1]]
    var cl : Foo
    cl := l                 // invokes clone(cl,l)


Cloning is typically allowed between regular and temporary types (see :ref:`Temporary types <temporary>`).

Pod types are copied instead of cloned::

    var a,b : int
    var c,d : int[10]
    a := b
    c := d

expands to::

    a = b
    c = d

Handled types provide their own clone functionality via ``canClone``, ``simulateClone``,
and appropriate ``das_clone`` C++ infrastructure (see :ref:`Handles <handles>`).

For the static array ``clone_dim`` generic is called,
and for the dynamic array ``clone`` generic is called.
Those in turn clone each of the array elements::

    struct Foo
        a : array<int>
        b : int

    var a, b : array<Foo>
    b := a
    var c, d : Foo[10]
    c := d

expands to::

    def builtin`clone ( var a:array<Foo aka TT> explicit; b:array<Foo> const )
        resize(a,length(b))
        for aV,bV in a,b
            aV := bV

    def builtin`clone_dim ( var a:Foo[10] explicit; b:Foo const[10] implicit explicit )
        for aV,bV in a,b
            aV := bV

For the table ``clone`` generic is called, which in turn clones its values::

    var a, b : table<string;Foo>
    b := a

expands to::

    def builtin`clone ( var a:table<string aka KT;Foo aka VT> explicit; b:table<string;Foo> const )
        clear(a)
        for k,v in keys(b),values(b)
            a[k] := v

For the structure default ``clone`` function is generated, in which each element is cloned::

    struct Foo
        a : array<int>
        b : int

expands to::

    def clone ( var a:Foo explicit; b:Foo const )
        a.a := b.a
        a.b = b.b   // note copy instead of clone

For the tuple each individual element is cloned::

    var a, b : tuple<int;array<int>;string>
    b := a

expands to::

    def clone ( var dest:tuple<int;array<int>;string> -const; src:tuple<int;array<int>;string> const -const )
        dest._0 = src._0
        dest._1 := src._1
        dest._2 = src._2

For the variant only currently active element is cloned::

    var a, b : variant<i:int;a:array<int>;s:string>
    b := a

expands to::

    def clone ( var dest:variant<i:int;a:array<int>;s:string> -const; src:variant<i:int;a:array<int>;s:string> const -const )
        if src is i
            set_variant_index(dest,0)
            dest.i = src.i
        else if src is a
            set_variant_index(dest,1)
            dest.a := src.a
        else if src is s
            set_variant_index(dest,2)
            dest.s = src.s

.. _clone_to_move:

----------------------------
clone_to_move implementation
----------------------------

``clone_to_move`` is implemented via regular generic as part of builtin module::

    def clone_to_move(clone_src:auto(TT)) : TT -const
        var clone_dest : TT
        clone_dest := clone_src
        return <- clone_dest

Note, that for the non-cloneable type daScript will not promote ``:=`` initialize into ``clone_to_move``.
