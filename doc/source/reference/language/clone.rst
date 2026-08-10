.. _clone:

=====
Clone
=====

Clone is designed to create a deep copy of the data.

For an overview of when to use copy (``=``), move (``<-``), and clone (``:=``),
see :ref:`Move, Copy, and Clone <move_copy_clone>`.

Cloning is invoked via the clone operator ``:=``:

.. das-doc: given var a, b, y : array<int>
.. code-block:: das

    a := b

Cloning can be also invoked via the clone initializer in a variable declaration:

.. code-block:: das

    var x := y

This in turn expands into ``clone_to_move``:

.. das-doc: alt
.. code-block:: das

    var x <- clone_to_move(y)

(see :ref:`clone_to_move <clone_to_move>`).

----------------------------------------
Cloning rules and implementation details
----------------------------------------

Cloning obeys the following rules.

Certain types like blocks, lambdas, and iterators can't be cloned at all.

However, if a custom clone function exists, it is immediately called regardless of the type's cloneability:

.. code-block:: das

    struct Foo {
        a : int
    }

    def clone ( var x : Foo; y : Foo ) {
        x.a = y.a
        print("cloned\n")
    }

    var l = Foo(a=1)
    var cl : Foo
    cl := l                 // invokes clone(cl,l)


Cloning is permitted across the regular/temporary boundary: a temporary value can be cloned into a regular one and vice versa (see :ref:`Temporary types <temporary>`).

POD types are copied instead of cloned:

.. code-block:: das

    var a,b : int
    var c,d : int[10]
    a := b
    c := d

This expands to:

.. code-block:: das

    a = b
    c = d

Handled types provide their own clone functionality via ``canClone``, ``simulateClone``,
and appropriate ``das_clone`` C++ infrastructure (see :ref:`Handles <handles>`).

For static arrays, the ``clone_dim`` generic is called,
and for dynamic arrays, the ``clone`` generic is called.
Those in turn clone each of the array elements:

.. code-block:: das

    struct Foo {
        a : array<int>
        b : int
    }

    var a, b : array<Foo>
    b := a
    var c, d : Foo[10]
    c := d

This expands to (as printed by ``daslang -log``, which shows the instantiated generics):

.. das-doc: skip
.. code-block:: das

    def builtin`clone ( var a:array<Foo aka TT> explicit; var b:array<Foo aka TT> ==const ) {
        let ln = length(b)
        resize(a,ln)
        return if ( ln == 0 )
        for ( aV,bV in a,b ) {
            aV := bV
        }
    }

    def builtin`clone_dim ( var a:Foo[10] explicit; b:Foo const[10] explicit ) {
        for ( aV,bV in a,b ) {
            aV := bV
        }
    }

When the element type is POD, both generics substitute a single ``memcpy`` for the
element-by-element loop.

For tables, the ``clone`` generic is called, which in turn clones its values:

.. code-block:: das

    var a, b : table<string;Foo>
    b := a

This expands to:

.. das-doc: skip
.. code-block:: das

    def builtin`clone ( var a:table<string;Foo aka VT> explicit; var b:table<string;Foo aka VT> ==const ) {
        clear(a)
        for ( k,v in keys(b),values(b) ) {
            let kk := k     // the string-key overload clones the key as well
            a[kk] := v
        }
    }

For structures, the default ``clone`` function is generated, in which each element is cloned:

.. code-block:: das

    struct Foo {
        a : array<int>
        b : int
    }

This expands to:

.. das-doc: skip
.. code-block:: das

    def clone ( var a:Foo explicit; b:Foo const implicit ) {
        a.a := b.a
        a.b = b.b   // note copy instead of clone
    }

For tuples, each individual element is cloned:

.. code-block:: das

    var a, b : tuple<int;array<int>;string>
    b := a

This expands to:

.. das-doc: skip
.. code-block:: das

    def clone ( var dest:tuple<int;array<int>;string> -const; var src:tuple<int;array<int>;string> implicit ==const -const ) {
        dest._0 = src._0
        dest._1 := src._1
        dest._2 = src._2
    }

For variants, only the currently active element is cloned:

.. code-block:: das

    var a, b : variant<i:int;a:array<int>;s:string>
    b := a

This expands to:

.. das-doc: skip
.. code-block:: das

    def clone ( var dest:variant<i:int;a:array<int>;s:string> -const; var src:variant<i:int;a:array<int>;s:string> implicit ==const -const ) {
        if ( src is i ) {
            set_variant_index(dest,0)
            dest.i = src.i
        } elif ( src is a ) {
            set_variant_index(dest,1)
            dest.a := src.a
        } elif ( src is s ) {
            set_variant_index(dest,2)
            dest.s = src.s
        }
    }

.. _clone_to_move:

----------------------------
clone_to_move implementation
----------------------------

``clone_to_move`` is implemented via regular generics as part of the builtin module.
The ``| #`` in the argument type accepts a temporary source, and ``-#`` on the return
type makes the result a regular (non-temporary) value:

.. das-doc: signatures
.. code-block:: das

    def clone_to_move(clone_src : auto(TT) ==const | #) : TT -const -# {
        unsafe {
            var clone_dest : TT -#
            clone_dest := clone_src
            return <- clone_dest
        }
    }

A second overload with a ``var`` source exists, so that a mutable value can be cloned too.

Note that for non-cloneable types, Daslang will not promote ``:=`` initialize into ``clone_to_move``.

.. seealso::

    :ref:`Move, copy, and clone <move_copy_clone>` for move, copy, and assignment rules,
    :ref:`Finalizers <finalizers>` for delete and finalization semantics,
    :ref:`Structs <structs>` and :ref:`Variants <variants>` for custom clone expansion.
