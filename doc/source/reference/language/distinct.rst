.. _distinct_types:

==============
Distinct Types
==============

.. index::
    single: Distinct Types
    single: distinct
    single: newtype

A distinct type promotes a plain "weak" type (``int``, ``float``, ``void?``, or any other
workhorse type) to a unique nominal type. It has the exact ABI of its underlying type — same
size, alignment, and pass-by-value semantics — but it does not interconvert with it:
functions taking ``int`` won't take a distinct type over ``int``, and vice versa. This makes
handles, IDs, and unit-like values type-safe at zero runtime cost.

.. code-block:: das

    typedef distinct EntityId = int
    typedef distinct Meters = float
    typedef distinct FileHandle = void?

Distinct types are declared at module scope with ``typedef distinct`` — the ``typedef``
family form that creates a *nominal* type instead of a transparent alias (``distinct``
itself is not a keyword, so identifiers named ``distinct`` — like the linq function —
keep working). The underlying type must be a workhorse type (a value type that passes
by value — ``int``/``uint`` and friends, ``float``, ``double``, ``bool``, ``string``,
pointers, vector types like ``float3``, ranges). A distinct type cannot be defined in
terms of another distinct type.

----------------------
Construction and Deref
----------------------

The only way *in* is explicit construction from a value of exactly the underlying type — a
pure compile-time relabel with no runtime cost:

.. code-block:: das

    var id = EntityId(5)
    var m : Meters = Meters(2.5)
    // var bad : EntityId = 5      // error: no implicit conversion

The only way *out* is the dereference operator ``*``, which peels exactly one distinct level
and yields the underlying type as a reference. Constness flows from the handle: dereferencing
a ``var`` gives writable access, a ``let`` gives const access.

.. code-block:: das

    var id = EntityId(5)
    print("{*id}\n")        // output: 5
    *id = 7                 // writes through - *id is int&
    if (*id == 7) {
        print("seven\n")    // output: seven
    }

For a distinct type over a pointer, ``*`` peels the distinct level, not the pointer:
``*fileHandle`` yields ``void?``.

``var id : EntityId`` zero-initializes, and ``default<EntityId>`` is the zero value.

---------
Operators
---------

Only ``==`` and ``!=`` are borrowed from the underlying type, both sides must be the
*same* distinct type, and an ``operator ==`` or ``operator !=`` you define for the distinct
type takes precedence over the borrow. Everything else — arithmetic, ordering — is deliberately absent:
``EntityId + EntityId`` is meaningless, so it doesn't compile. Where an operation is
meaningful, define it explicitly:

.. code-block:: das

    def operator + (a, b : Meters) : Meters => Meters(*a + *b)

    let d = Meters(2.0) + Meters(3.0)   // Meters(5.0)

A distinct type keys a table when its underlying type can: the key hashes as the underlying
value, and the nominal wall still holds, so ``table<EntityId; string>`` refuses a plain ``int``
key and a key of any other distinct type over ``int`` at compile time. Arrays, struct fields,
tuple and variant fields all work as well.

---------
Overloads
---------

Because the mangled names differ, overloads on the distinct type and its underlying type
coexist — this is the primary use case:

.. code-block:: das

    def describe(x : int) : string => "int {x}"
    def describe(x : EntityId) : string => "entity {*x}"

--------------------
Modules and Privacy
--------------------

Distinct types are module-nominal: same-named distinct types in two modules are different
types. ``typedef private distinct Foo = int`` limits visibility to the declaring module, like
``enum private``.

---------------
Runtime Erasure
---------------

Distinct types exist only in the type system. At runtime — in the interpreter, JIT, AOT,
and reflection — values carry their underlying type: ``print(id)`` prints ``7``, not
``EntityId(7)``, and RTTI reports the underlying type. The compile-time relabel is free in
all three execution tiers.

----------------
C++ Integration
----------------

A C++ module can register a distinct type with ``DistinctTypeAnnotation`` and expose extern
functions whose signatures carry it:

.. code-block:: cpp

    // struct NativeId { int32_t value; ... };  with das::cast/WrapType specializations
    MAKE_DISTINCT_TYPE_FACTORY(NativeId, NativeId);
    // in the module constructor:
    addAnnotation(new DistinctTypeAnnotation("NativeId", makeType<int32_t>(lib), "NativeId"));
    addExtern<DAS_BIND_FUN(nativeIdNext)>(*this, lib, "native_id_next", SideEffects::none, "nativeIdNext");

On the das side ``NativeId`` behaves exactly like a das-declared distinct type: constructed
with ``NativeId(v)``, dereferenced with ``*``, and never interchangeable with ``int``.

.. seealso::

    :ref:`Type Aliases <aliases>` for transparent (non-nominal) type names,
    :ref:`Datatypes <datatypes_and_values>` for the workhorse types that can be made distinct,
    :ref:`Constants and Enumerations <constants_and_enumerations>` for the other nominal value types.
