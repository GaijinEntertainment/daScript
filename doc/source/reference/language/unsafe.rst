.. _unsafe:


======
Unsafe
======

.. index::
    single: Unsafe

The ``unsafe`` keyword denotes unsafe contents, which is required for operations, but could potentially crash the application:

.. das-doc: given struct Foo { a : int }
.. das-doc: given variant VB { Bar : float; Baz : int }
.. das-doc: given class TestClass { cx : int }
.. das-doc: given class Goo { gy : int }
.. das-doc: given var x = 42
.. das-doc: given var v = 0x3f800000
.. das-doc: given var vb : VB
.. code-block:: das

    unsafe {
        let px = addr(x)                        // error[31000] without unsafe
    }

Expressions (and subexpressions) can also be unsafe:

.. code-block:: das

    let px = unsafe(addr(x))

The ``unsafe`` keyword is followed by a block that can include such operations. Nested unsafe sections are allowed. ``unsafe`` is not inherited in lambdas, generators, or local functions, but it is inherited in local blocks.

Individual expressions raise an error from the ``unsafe_*`` family (``error[31000]`` through
``error[31037]``) unless they are part of the unsafe section. Additionally, macros can explicitly
set the ``ExprGenFlags::alwaysSafe`` flag.

The address of expression is unsafe:

.. code-block:: das

    def return_stack_address() : int? {
        unsafe {
            var n = 13
            var pn = addr(n)                    // error[31000] without unsafe
            return pn                           // accessing *pn can potentially corrupt stack
        }
    }

Lambdas or generators require unsafe sections for the implicit capture by move or by reference:

.. code-block:: das

    var values : array<int>
    unsafe {
        var counter <- @ (extra:int) : int {
            return values[0] + extra            // error[31003]: values is implicitly moved
        }
    }

Deleting any pointer requires an unsafe section:

.. code-block:: das

    var p = new Foo()
    var q = p
    unsafe {
        delete p                                // error[31009] without unsafe;
                                                // accessing q can potentially corrupt memory
    }

Upcast and reinterpret cast require an unsafe section:

.. code-block:: das

    def erase_type() : void? {
        unsafe {
            return reinterpret<void?>(13)       // error[31004] without unsafe;
                                                // reinterpret can create unsafe pointers
        }
    }

``addr<T?>(x)`` — sugar for ``reinterpret<T?>(addr(x))`` — needs only one
``unsafe`` covering both the address-of and the reinterpret:

.. code-block:: das

    let pv = unsafe(addr<float?>(v))            // instead of unsafe(reinterpret<float?>(unsafe(addr(v))))

Indexing into a pointer is unsafe:

.. code-block:: das

    def read_out_of_bounds() : Foo {
        unsafe {
            var pf = new Foo()
            return pf[13]                       // error[31023] without unsafe; accessing an out of
                                                // bounds pointer can potentially corrupt memory
        }
    }

A safe index is unsafe when not followed by the null coalescing operator:

.. code-block:: das

    def unsafe_safe_index() : int? {
        var tab = { 13 => 12 }
        let t = tab?[13] ?? 1234                // safe
        unsafe {
            // safe index is a form of 'addr' operation, so it can create
            // pointers to temporary objects
            return tab?[13]
        }
    }

Without the ``unsafe`` section it does not compile:

.. das-doc: expect error[31034]
.. code-block:: das

    def bare_safe_index() : int? {
        var tab = { 13 => 12 }
        // error[31034]: safe-index of table<> must be inside the 'unsafe' block
        return tab?[13]
    }

Variant ``?as`` on local variables is unsafe when not followed by the null coalescing operator:

.. code-block:: das

    def unsafe_safe_as() : float? {
        unsafe {
            return vb ?as Bar                   // safe as is a form of 'addr' operation
        }
    }

Without the ``unsafe`` section it does not compile:

.. das-doc: expect error[31036]
.. code-block:: das

    def bare_safe_as() : float? {
        // error[31036]: variant ?as on non-pointer requires unsafe
        return vb ?as Bar
    }

Variant ``?.field`` is unsafe when not followed by the null coalescing operator:

.. code-block:: das

    def unsafe_safe_field() : float? {
        unsafe {
            // safe navigation of a variant is a form of 'addr' operation;
            // error[31036] without unsafe
            return vb?.Bar
        }
    }


Variant ``.field`` is unsafe:

.. code-block:: das

    def unsafe_field() : float {
        unsafe {
            return vb.Bar                       // this is potentially a reinterpret cast
        }
    }

Without the ``unsafe`` section it does not compile:

.. das-doc: expect error[31035]
.. code-block:: das

    def bare_field() : float {
        return vb.Bar                           // error[31035]: variant.field requires unsafe
    }

Certain functions and operators are inherently unsafe or marked unsafe via the [unsafe_operation] annotation:

.. code-block:: das

    unsafe {
        var ptr : int?
        ptr += 13                               // error[31013]: pointer arithmetic can create
                                                // invalid pointers
        var boo : int[13]
        var it = each(boo)                      // error[31013]: each() of array is unsafe,
                                                // for it does not capture
    }

A move statement whose source is a smart pointer **value** requires unsafe, unless that value
comes from the ``new`` operator. Moving from a smart pointer **reference** stays safe.
``TestObjectSmart`` below is a C++ smart pointer type, so this block has no standalone context:

.. das-doc: fragment
.. code-block:: das

    var a <- new TestObjectSmart()              // safe, its explicitly new
    var b : TestObjectSmart?
    unsafe {
        b <- someSmartFunction()                // error[31021] without unsafe,
                                                // since lifetime is not obvious
    }
    b <- a                                      // safe, moving from a reference

Moving or copying classes is unsafe:

.. code-block:: das

    def move_class ( var src : TestClass ) {
        unsafe {
            var dst : TestClass
            dst <- src                          // error[31005] without unsafe;
                                                // potentially moving from derived class
        }
    }

Local class variables are unsafe:

.. code-block:: das

    unsafe {
        var g = Goo()                           // error[31017] without unsafe;
                                                // potential lifetime issues
    }

implicit
--------

``implicit`` keyword is used to specify that type can be either temporary or regular type. The parameter is treated as the type written in the declaration, while also accepting the other form (temporary or regular) as an argument.
For example:

.. das-doc: signatures
.. code-block:: das

    def foo ( a : Foo implicit )    // a will be treated as Foo, but will also accept Foo# as argument
    def foo ( a : Foo# implicit )   // a will be treated as Foo#, but will also accept Foo as argument

``implicit`` switches off the temporary-type lifetime check for that parameter, and nothing in
the compiler can verify what the body then does with the value. Declaring it does not require an
``unsafe`` section — it is a promise by the author that the value is never cached (copied, moved,
or stored).

other cases
-----------

There are several additional cases where ``unsafe`` is required.
They are typically controlled via CodeOfPolicies or an appropriate option:

.. das-doc: alt
.. code-block:: das

    options unsafe_table_lookup // makes ALL table indexing unsafe. refers to CodeOfPolicies::unsafe_table_lookup

    var names <- { 1=>"one", 2=>"two" }
    unsafe {
        names[3] = "three"  // error[31033] without unsafe, when unsafe_table_lookup is enabled
    }

By default ``unsafe_table_lookup`` is ``false`` — individual table lookups are safe. However, the compiler
still detects dangerous patterns where the same table is indexed more than once in a single expression
(see :ref:`Tables <tables>` for details).

.. seealso::

    :ref:`Lambdas <lambdas>` for capture-by-reference and move requiring unsafe,
    :ref:`Classes <classes>` for local class variables being unsafe,
    :ref:`Expressions <expressions>` for ``addr``, ``reinterpret``, and ``upcast`` operators,
    :ref:`Temporary types <temporary>` for ``implicit`` and temporary type qualifiers,
    :ref:`Tables <tables>` for unsafe table lookup,
    :ref:`Options <options>` for ``unsafe_table_lookup`` and related policies.

