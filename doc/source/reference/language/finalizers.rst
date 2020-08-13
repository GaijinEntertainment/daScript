.. _finalizers:

=========
Finalizer
=========

Finalizer is a special function, which is called in exactly two cases

``delete`` is called explicitly on a data type::

    var f <- [{int 1;2;3;4}]
    delete f

Lambda based iterator or generator is sequenced out::

    var src <- [{int 1;2;3;4}]
    var gen <- generator<int&> [[<-src]] () <| $ ()
        for w in src
            yield w
        return false
    for t in gen
        print("t = {t}\n")
    // finalizer is called on captured version of src

By default finalizers are called recursively on subtypes.

If memory models allows deallocation, standard finalizers will also free the memory::

    options persistent_heap = true

    var src <- [{int 1;2;3;4}]
    delete src                      // memory of src will be freed here

Custom finalizer can be defined for any type by overriding ``finalize`` function.
Generic custom finalizers are also allowed::

    struct Foo
        a : int

    def finalize ( var foo : Foo )
        print("we kill foo\n")

    var f = [[Foo a = 5]]
    delete f                    // prints 'we kill foo' here

--------------------------------
Rules and implementation details
--------------------------------

Finalizers obey the following rules.

If custom ``finalize`` is available, its called instead of default one.

Pointer finalizer expands to calling ``finalize`` on dereferenced pointer,
and then calling native memory finalizer on the result::

    var pf = new Foo
    unsafe
        delete pf

expands to::

    def finalize ( var __this:Foo?& explicit -const )
        if __this != null
            _::finalize(deref(__this))
            delete /*native*/ __this
            __this = null

Static array calls ``finalize_dim`` generic, which finalizes all its values::

    var f : Foo[5]
    delete f

expands to::

    def builtin`finalize_dim ( var a:Foo aka TT[5] explicit )
        for aV in a
            _::finalize(aV)

Dynamic array calls ``finalize`` generic, which finalizes all its values::

    var f : array<Foo>
    delete f

expands to::

    def builtin`finalize ( var a:array<Foo aka TT> explicit )
        for aV in a
            _::finalize(aV)
        __builtin_array_free(a,4,__context__)

Table calls ``finalize`` generic, which finalizes all its values, but not keys::

    var f : table<string;Foo>
    delete f

expands to::

    def builtin`finalize ( var a:table<string aka TK;Foo aka TV> explicit )
        for aV in values(a)
            _::finalize(aV)
        __builtin_table_free(a,8,4,__context__)

Custom finalizer is generated for structure. Fields annotated as [[do_not_delete]] are ignored.
``memzero`` clears structure memory at the end::

    struct Goo
        a : Foo
        [[do_not_delete]] b : array<int>

    var g <- [[Goo]]
    delete g

expands to::

    def finalize ( var __this:Goo explicit )
        _::finalize(__this.a)
        __::builtin`finalize(__this.b)
        memzero(__this)

Tuple behaves similar to structure. There is no way to ignore individual fields::

    var t : tuple<Foo; int>
    delete t

expands to::

    def finalize ( var __this:tuple<Foo;int> explicit -const )
        _::finalize(__this._0)
        memzero(__this)

Variant behaves similar to tuple. Only currently active variant is finalized::

    var t : variant<f:Foo; i:int; ai:array<int>>
    delete t

epxands to::

    def finalize ( var __this:variant<f:Foo;i:int;ai:array<int>> explicit -const )
        if __this is f
            _::finalize(__this.f)
        else if __this is ai
            __::builtin`finalize(__this.ai)
        memzero(__this)

Lambdas and generators have their capture structure finalized.
Lambda can have custom finalizer defined as well (see :ref:`Lambdas <lambdas_finalizer>`).

Classes can define custom finalizer inside the class body (see :ref:`Classes <classes_finalizer>`).