.. _tutorial_integration_cpp_handle_registry:

.. index::
   single: Tutorial; C++ Integration; Handle Registry

==========================================================
 C++ Integration: Binding ``shared_ptr`` via ``Handle<T>``
==========================================================

This tutorial shows how to expose ``std::shared_ptr<T>``-owned C++ objects
to daslang as value-typed handles, **without** modifying the C++ class
to inherit ``das::ptr_ref_count``.  Topics covered:

* ``das::Handle<T>`` — a 64-bit, generation-checked value handle
* ``das::HandleRegistry<T>`` — per-T singleton that owns the live ``shared_ptr``
* ``addHandleAnnotation<T>`` — one call registers the type plus
  ``==``, ``!=``, ``is_alive``, and an optional destructor
* Automatic leak-at-shutdown reporting
* When to pick ``Handle<T>`` vs ``smart_ptr<T>``

This is the pattern used by ``modules/dasHV`` for ``WebSocketClient``,
``WebSocketServer``, ``HttpServer`` and similar types whose ownership
lives inside libhv.


Prerequisites
=============

* Tutorial 11 — :ref:`tutorial_integration_cpp_context_variables`.
* Tutorial 12 — :ref:`tutorial_integration_cpp_smart_pointers` — read it
  first to see the intrusive-refcount alternative, then decide which
  pattern fits your C++ type.


When to use ``Handle<T>`` vs ``smart_ptr<T>``
=============================================

+------------------------------------+---------------------------------+--------------------------------+
| Question                           | ``smart_ptr<T>`` (Tutorial 12)  | ``Handle<T>`` (this tutorial)  |
+====================================+=================================+================================+
| Can I modify the C++ class?        | yes — inherit ``ptr_ref_count`` | no — use ``shared_ptr`` as-is  |
+------------------------------------+---------------------------------+--------------------------------+
| Ownership primarily lives in…      | daslang                         | C++ engine                     |
+------------------------------------+---------------------------------+--------------------------------+
| Script-side cost per copy          | refcount bump                   | 64-bit value copy              |
+------------------------------------+---------------------------------+--------------------------------+
| Use-after-free safety              | refcount (lifetime)             | generation check               |
+------------------------------------+---------------------------------+--------------------------------+
| Registry thread safety             | n/a                             | yes — mutex inside registry    |
+------------------------------------+---------------------------------+--------------------------------+
| Requires ``var inscope`` in script | yes                             | no — plain ``var h = …``       |
+------------------------------------+---------------------------------+--------------------------------+


Defining the C++ type
=====================

``Actor`` is a plain ``struct`` — no base class, no refcount:

.. code-block:: cpp

   struct Actor {
       std::string name;
       float       x = 0.f, y = 0.f;
       int32_t     health = 100;
       explicit Actor(const char * n) : name(n ? n : "unnamed") { }
       ~Actor() { }
       void move(float dx, float dy)   { x += dx; y += dy; }
       void take_damage(int32_t dmg)   { health -= dmg; if (health < 0) health = 0; }
       bool is_dead() const            { return health <= 0; }
   };

This is the whole point of the tutorial: you do **not** need to touch
``Actor`` to expose it to daslang.


Declaring ``typeName<T>``
=========================

``Handle<T>`` carries a ``cast<>`` / ``typeFactory<>`` specialization
for itself, but the leak-dump path reads the **inner** type name via
``typeName<T>::name()``.  Provide it at file scope:

.. code-block:: cpp

   MAKE_TYPE_FACTORY(Actor, Actor)

(In dasHV the equivalent line is
``MAKE_EXTERNAL_TYPE_FACTORY(WebSocketClient, hv::WebSocketClient)`` in
``dasHV.h`` + ``IMPLEMENT_EXTERNAL_TYPE_FACTORY(...)`` in ``dasHV.cpp``
— use the external variant only if the declaration has to cross a
header/translation-unit boundary.)


``addHandleAnnotation<T>``
==========================

.. code-block:: cpp

   #include "daScript/misc/handle_registry.h"   // Handle, HandleRegistry
   #include "daScript/ast/ast_handle.h"         // addHandleAnnotation

   addHandleAnnotation<Actor>(this, lib, "Actor",
       "destroy_actor",              // optional — daslang destructor name
       "das::Handle<Actor>");        // name AOT emits into generated C++

One call registers:

+--------------------------+--------------------------------------------------+
| ``Actor`` annotation     | ``ManagedHandleAnnotation<Actor>`` —             |
|                          | a value annotation wrapping ``Handle<Actor>``    |
+--------------------------+--------------------------------------------------+
| ``==`` / ``!=``          | structural comparison of the 64-bit value        |
+--------------------------+--------------------------------------------------+
| ``is_alive``             | generation-checked validity probe                |
+--------------------------+--------------------------------------------------+
| ``destroy_actor``        | only if ``destroyFnName`` (the argument after    |
|                          | the type name) is non-empty — calls              |
|                          | ``HandleRegistry<Actor>::release(h)``            |
+--------------------------+--------------------------------------------------+
| leak-dump hook           | wired automatically via                          |
|                          | ``handleRegistry_registerDump<Actor>``           |
+--------------------------+--------------------------------------------------+


Factory — ``acquire``
=====================

The factory creates a fresh ``shared_ptr`` and hands it to the registry:

.. code-block:: cpp

   static Handle<Actor> make_actor(const char * name, float x, float y) {
       auto sp = std::make_shared<Actor>(name);
       sp->x = x;
       sp->y = y;
       return HandleRegistry<Actor>::instance().acquire(sp);
   }

``HandleRegistry<T>::instance()`` is a per-``T`` singleton; every module
and DLL that references ``HandleRegistry<Actor>`` sees the same storage.


Method — ``lookup`` + null-check
================================

Bound "methods" are free functions that take ``Handle<Actor>`` by value
and resolve it through ``lookup``:

.. code-block:: cpp

   static void actor_move(Handle<Actor> h, float dx, float dy) {
       if ( auto p = HandleRegistry<Actor>::instance().lookup(h) )
           p->move(dx, dy);
   }

``lookup`` returns an empty ``shared_ptr`` for null, stale, or
slot-reused handles — the null-check is a guaranteed
use-after-free guard.

.. warning::

   ``Handle<T>`` is passed **by value**, so a bound function that
   mutates the underlying object cannot use
   ``SideEffects::modifyArgument`` (which requires a non-const
   reference argument — you will get ``can't add function ... modify
   argument requires non-const ref argument`` at module registration).
   Use ``SideEffects::modifyExternal`` instead: the handle itself is
   not modified, the external state behind it is.


Explicit destroy and ``is_alive``
=================================

The daslang name passed as ``destroyFnName`` becomes a script-callable
destructor that unregisters the handle:

.. code-block:: das

   var goblin = make_actor("Goblin", 10.0, 5.0)
   destroy_actor(goblin)
   assert(!is_alive(goblin))

``destroy_actor`` does **not** necessarily destroy the ``Actor`` — it
releases only the registry's ``shared_ptr``.  If your engine holds
another ``shared_ptr``, the object survives until the engine drops it.

.. warning::

   ``Handle<T>`` has **no scope-based auto-release** — unlike
   ``smart_ptr<T>`` (Tutorial 12), going out of scope does nothing.
   Every handle the script acquired via a factory must be explicitly
   destroyed (or handed off to the engine for release), or it will
   leak and the runtime will print a ``Handle<T> idx=... (rc=...)``
   line at shutdown.


Leak dump at shutdown
=====================

``addHandleAnnotation`` calls ``handleRegistry_registerDump<T>`` once,
registering a per-``T`` dump callback.  ``Module::Shutdown(bool
dumpHandleLeaks = true)`` calls ``handleRegistry_dumpAll()`` directly
and the runtime prints any live handles:

.. code-block:: text

   Handle<Actor> idx=3 gen=1 (rc=1)
   total 1 leaked handles of type Actor

The tutorial script is designed to end with zero leaked handles — if
you see output above, the script is holding a live handle past shutdown.


AOT compatibility
=================

``Handle<T>`` is value-typed via a ``cast<Handle<T>>`` specialization
in ``ast_handle.h`` that maps directly to ``uint64_t``.  AOT-generated
C++ passes the handle in a raw register — no boxing, no allocations.

The ``cppTypeName`` argument to ``addHandleAnnotation``
(``"das::Handle<Actor>"`` above) is the string AOT writes into the
generated stub, so the emitted C++ compiles against the same header
your module uses.


Building and running
====================

::

   cmake --build build --config Release --target integration_cpp_23
   bin\Release\integration_cpp_23.exe

Expected output::

   === Create actors ===
     [C++] Actor('Hero') constructed
     [C++] Actor('Goblin') constructed
   hero:   Hero hp=100 alive=true
   goblin: Goblin hp=100 alive=true

   === Value semantics ===
   hero == hero_copy : true
   hero == goblin    : false

   === Combat ===
   hero hp = 100
   goblin hp after 30 damage = 70
   goblin hp after lethal    = 0  is_dead=true

   === Explicit destroy ===
   is_alive(goblin) before destroy = true
     [C++] Actor('Goblin') destroyed
   is_alive(goblin) after  destroy = false
   dead goblin.health = 0
   dead goblin.name   = ''

   === Engine-owned lifetime ===
     [C++] Actor('Shopkeeper') constructed
   after destroy: is_alive(npc) = false
     [C++] Actor('Hero') destroyed

   === End of test — expect zero leak-dump output below ===
     [C++] Actor('Shopkeeper') destroyed

Observe: Goblin and Hero die when ``destroy_actor`` drops the
registry's last ``shared_ptr``.  Shopkeeper's destructor runs *after*
the "End of test" line — that is the engine dropping its vector in
``main`` after ``Module::Shutdown``, proving that registry release
and engine ownership are independent.  The key property is **zero**
``Handle<Actor> idx=…`` leak lines.


.. seealso::

   Full source:
   :download:`23_handle_registry.cpp <../../../../tutorials/integration/cpp/23_handle_registry.cpp>`,
   :download:`23_handle_registry.das <../../../../tutorials/integration/cpp/23_handle_registry.das>`

   Previous tutorial: :ref:`tutorial_integration_cpp_namespace_integration`

   Compare with the intrusive-refcount pattern:
   :ref:`tutorial_integration_cpp_smart_pointers`
