.. _pointers:

========
Pointers
========

.. index::
    single: Pointers

Daslang provides nullable pointer types for heap-allocated data, optional
references, and low-level memory access.  Pointer operations split into
two categories: **safe** operations that work without ``unsafe``, and
**unsafe** operations that require an ``unsafe`` block.

.. _pointer_types:

--------------
Pointer types
--------------

=============  =====================================================
Type           Description
=============  =====================================================
``T?``         Nullable pointer to type ``T``
``T? const``   Const pointer — cannot modify the pointed-to value
``T?#``        Temporary pointer (from ``safe_addr``) — cannot escape scope
``void?``      Untyped pointer — must ``reinterpret`` to use
=============  =====================================================

Pointer types are declared by appending ``?`` to any type:

.. das-doc: given struct Point { x, y : float }
.. code-block:: das

    var p : int?               // pointer to int — null by default
    var ps : Point?            // pointer to struct
    var vp : void?             // void pointer

All pointers default to ``null`` when uninitialized.

.. _pointer_creation:

-----------------
Creating pointers
-----------------

new
^^^

``new`` allocates on the heap and returns ``T?``:

.. code-block:: das

    var p = new Point(x = 3.0, y = 4.0)   // p is Point?
    var q = new Point()                    // default field values

Heap pointers must be released with ``delete`` (see :ref:`pointer_delete`)
or declared with ``var inscope`` for automatic cleanup.  Because a raw
``new T()`` pointer is deleted at scope exit, and deleting a raw pointer
requires ``unsafe``, the ``var inscope`` declaration itself must be inside an
``unsafe`` block:

.. code-block:: das

    unsafe {
        var inscope pt = new Point(x = 1.0, y = 2.0)
        // pt is automatically deleted at scope exit
    }

addr
^^^^

``addr(x)`` returns a pointer to an existing variable.  **Requires unsafe.**

.. code-block:: das

    var n = 42
    unsafe {
        var pn = addr(n)   // pn is int?
        *pn = 100          // modifies n
    }

The pointer is valid only while the variable is alive — using it after
the variable goes out of scope is undefined behavior.

safe_addr
^^^^^^^^^

``safe_addr`` from ``daslib/safe_addr`` returns a temporary pointer (``T?#``)
without requiring ``unsafe``.  The compiler validates that the argument is a
local or global variable (not a field of a temporary):

.. code-block:: das

    require daslib/safe_addr
    var a = 13
    var sp = safe_addr(a)   // sp is int?# (temporary pointer)
    print("{*sp}\n")

Temporary pointers cannot be stored in containers or returned from functions.

.. _pointer_deref:

--------------
Dereferencing
--------------

``*p`` or ``deref(p)`` follows the pointer to the value.  Both panic if the
pointer is null:

.. code-block:: das

    *p         // dereference
    deref(p)   // same thing

For struct pointers, ``.`` auto-dereferences — no ``->`` operator is needed:

.. code-block:: das

    unsafe {
        var inscope pt = new Point(x = 5.0, y = 6.0)
        print("{pt.x}\n")     // 5 — same as (*pt).x
        pt.x = 10.0           // modify through auto-deref
    }

.. _pointer_null_safety:

-----------
Null safety
-----------

Null checks
^^^^^^^^^^^

Pointers can be compared to ``null``:

.. code-block:: das

    if (p != null) {
        print("{*p}\n")      // safe — we checked
    }

Null dereference panics at runtime and can be caught with ``try``/``recover``:

.. code-block:: das

    try {
        var np : int?
        print("{*np}\n")
    } recover {
        print("caught null dereference\n")
    }

Safe navigation ``?.``
^^^^^^^^^^^^^^^^^^^^^^

``?.`` returns ``null`` instead of panicking when the pointer is null:

.. code-block:: das

    struct Segment {
        head : Point?
    }

    var seg : Segment?

    seg?.head          // returns head if seg is non-null, null otherwise
    seg?.head?.x       // chains — short-circuits on first null

Safe navigation results are themselves nullable, so combine with ``??``
for a concrete fallback:

.. code-block:: das

    let val = p?.x ?? -1.0     // -1 if p is null

Null coalescing ``??``
^^^^^^^^^^^^^^^^^^^^^^

``??`` dereferences the pointer on the left, and falls back to the value on
the right when that pointer is null.  The default is therefore a value of the
**pointee** type — not another pointer — and so is the result:

.. code-block:: das

    var origin = Point(x = 0.0, y = 0.0)
    let pos = p ?? origin     // Point; origin if p is null

The same holds for a pointer with an integer or other scalar pointee:

.. code-block:: das

    var pi : int?
    let count = pi ?? 0    // int; 0 if pi is null

.. _pointer_delete:

--------
Deletion
--------

``delete`` frees heap memory and sets the pointer to null.  **Requires unsafe.**

.. code-block:: das

    var dp = new Point()
    unsafe {
        delete dp       // frees memory, dp becomes null
    }

Prefer ``var inscope`` for automatic cleanup — it adds a ``finally`` block
that deletes the pointer when the scope exits.  Since the generated delete is
unsafe for a raw heap pointer, the ``var inscope`` declaration must live inside
an ``unsafe`` block:

.. code-block:: das

    unsafe {
        var inscope p = new Point()
        // p is automatically deleted at end of scope
    }

.. _pointer_arithmetic:

-------------------
Pointer arithmetic
-------------------

All pointer arithmetic **requires unsafe**.  No bounds checking is performed.

Indexing
^^^^^^^^

``p[i]`` accesses the ``i``-th element at the pointer's address:

.. code-block:: das

    var arr <- [10, 20, 30, 40, 50]
    unsafe {
        var p = addr(arr[0])
        print("{p[0]}, {p[2]}\n")     // 10, 30
    }

Increment and addition
^^^^^^^^^^^^^^^^^^^^^^

.. das-doc: alt
.. code-block:: das

    var data <- [10, 20, 30, 40, 50]
    unsafe {
        var p = addr(data[0])
        ++ p              // advance pointer by one element
        p += 3            // advance by three elements
        print("{*p}\n")   // 50
    }

.. warning::

   Pointer arithmetic can easily cause out-of-bounds access or invalid
   pointer states.  Use array bounds-checked access whenever possible.

.. _pointer_const_argument:

--------------------------------
Const widening when passing args
--------------------------------

When passing a pointer as a function argument, a non-const pointer ``T?`` is
accepted where the parameter is declared as ``T const?``.  The callee promises
not to modify the pointee, so narrowing the caller's permission is safe:

.. code-block:: das

    def inspect(p : Point const?) {   // read-only view
        print("{p.x}, {p.y}\n")
    }

    var pt = new Point(x = 1.0, y = 2.0)
    inspect(pt)                       // Point?  ->  Point const?  — OK

The same widening applies when the pointer is wrapped in a :ref:`tuple <tuples>`
at the top of the parameter type:

.. code-block:: das

    var ats : array<tuple<string; Point const?>>

    def push_named(var dst : array<tuple<string; Point const?>>;
                   e   : tuple<string; Point const?>) {
        dst |> push(e)
    }

    var p = new Point()
    push_named(ats, ("origin", p))    // tuple<string; Point?>  accepted for
                                      // tuple<string; Point const?>

The rule is one-directional: a ``T const?`` argument is **not** accepted where
a non-const ``T?`` is expected.  The widening also only applies at the top
level of the argument type — pointers nested inside arrays, tables, or deeper
structures must match constness exactly.  See ``TypeDecl::isSameType`` (the
``isPassType`` parameter) in ``src/ast/ast_typedecl.cpp`` for the
implementation.

.. _pointer_void:

--------------
Void pointers
--------------

``void?`` is an untyped pointer — equivalent to ``void*`` in C/C++.  It is
used for opaque handles and C/C++ interop.  You must ``reinterpret`` it back
to a typed pointer before dereferencing:

.. code-block:: das

    unsafe {
        var raw = 123
        var praw = addr(raw)
        var vraw : void? = reinterpret<void?>(praw)   // erase type
        var praw2 = reinterpret<int?>(vraw)           // restore type
        print("{*praw2}\n")                           // 123
    }

.. _pointer_intptr:

------
intptr
------

``intptr(p)`` converts any pointer (raw or smart) to a ``uint64`` integer
representing its memory address:

.. code-block:: das

    let address = intptr(p)   // uint64

Useful for debugging, logging, pointer identity comparisons, or hashing.

.. _pointer_reinterpret:

-----------
reinterpret
-----------

``reinterpret<T>`` performs a raw bit cast between types of the same size.
**Requires unsafe.**  It does not convert values — it reinterprets the raw bits:

.. code-block:: das

    unsafe {
        let f = 1.0
        let bits = reinterpret<int>(f)      // IEEE 754: 0x3f800000
        let back = reinterpret<float>(bits) // 1.0
    }

Can also cast between pointer types:

.. code-block:: das

    unsafe {
        var i = 7
        var ip : int? = addr(i)
        var ivp = reinterpret<void?>(ip)    // to void?
        var ip2 = reinterpret<int?>(ivp)    // back to int?
    }

.. _pointer_typeinfo:

---------
Type info
---------

Several ``typeinfo`` queries test pointer properties at compile time:

.. code-block:: das

    typeinfo is_pointer(p)       // true if p is a pointer type
    typeinfo is_smart_ptr(p)     // true if p is a smart_ptr<T>
    typeinfo is_void_pointer(p)  // true if p is void?
    typeinfo can_delete(p)       // true if delete is valid for p

``can_delete_ptr`` asks the same question of a **pointee** type rather than of
a pointer, so it is written against the dereferenced value —
``typeinfo can_delete_ptr(*p)`` is true exactly when ``delete p`` is legal.

.. _pointer_summary:

-------
Summary
-------

**Safe (no unsafe required):**

* ``new T()`` — heap allocate, returns ``T?``
* ``*p`` / ``deref(p)`` — dereference (panics if null)
* ``p.field`` — auto-deref field access
* ``p?.field`` — safe navigation (null-propagating)
* ``p ?? default`` — null coalescing
* ``safe_addr(x)`` — temporary pointer (``T?#``)
* ``intptr(p)`` — pointer to integer

**Unsafe (requires unsafe block):**

* ``addr(x)`` — address of variable
* ``delete p`` — free heap memory
* ``var inscope p = new T()`` — automatic cleanup (generates an unsafe ``delete``)
* ``p[i]`` — pointer indexing
* ``++ p`` / ``p += N`` — pointer arithmetic
* ``reinterpret<T>`` — raw bit cast

.. seealso::

    :ref:`Unsafe <unsafe>` for the full list of unsafe operations.

    :ref:`Values and Data Types <datatypes_and_values>` for smart pointers
    (``smart_ptr<T>``).

    :ref:`Temporary types <temporary>` for temporary pointers (``T?#``) and
    ``safe_addr``.

    :ref:`Tutorial: Pointers <tutorial_pointers>` for a hands-on walkthrough.
