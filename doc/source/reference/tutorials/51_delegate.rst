.. _tutorial_delegate:

==========================================
Delegates
==========================================

.. index::
    single: Tutorial; Delegates
    single: Tutorial; Multicast Callbacks
    single: Tutorial; Events
    single: Tutorial; delegate

This tutorial covers ``daslib/delegate`` — a type macro that generates multicast
callback structs from a function signature.  A delegate holds a list of handlers
(lambdas or function pointers) that are all invoked when the delegate is called.
Think C# events/delegates.

Prerequisites: familiarity with lambdas, function pointers, and typedefs.

.. code-block:: das

    options gen2

    require daslib/delegate
    require math


Declaring delegate types
=========================

Use ``typedef`` with the ``delegate()`` type macro.  Pass either
``function<...>`` or ``lambda<...>`` — both produce the same delegate type
(internally all handlers are stored as lambdas).

.. code-block:: das

    typedef OnDamage  = delegate(type<function<(target : string; amount : int) : int>>)
    typedef OnNotify  = delegate(type<function<(msg : string) : void>>)
    typedef OnTick    = delegate(type<function<() : void>>)


Construction from lambdas
==========================

Pass a lambda directly to the constructor.  A delegate holds an array of
lambdas, so bind it with ``<-``:

.. code-block:: das

    var del <- OnDamage(@(target : string; amount : int) : int {
        print("hit {target} for {amount}\n")
        return amount
    })
    let result = del.invoke("goblin", 10)
    // output:
    //   hit goblin for 10


Construction from function pointers
=====================================

Pass a function pointer with ``@@``:

.. code-block:: das

    def apply_damage(target : string; amount : int) : int {
        print("apply_damage({target}, {amount})\n")
        return amount * 2
    }

    var del <- OnDamage(@@apply_damage)
    let result = del.invoke("dragon", 25)
    // prints: apply_damage(dragon, 25)
    // result == 50

The function pointer is automatically wrapped in a lambda internally.


Assignment (``:=``) — replace all handlers
===========================================

The ``:=`` operator clears the invocation list and sets a single handler:

.. code-block:: das

    var del <- OnDamage()
    del := @(target : string; amount : int) : int {
        print("handler A\n")
        return amount
    }
    // length is 1

    del := @(target : string; amount : int) : int {
        print("handler B\n")
        return amount * 3
    }
    // still length 1 — previous handler was replaced

You can also assign a function pointer: ``del := @@my_function``.


Append (``+=``) — add handlers
===============================

The ``+=`` operator appends a handler to the invocation list:

.. code-block:: das

    var del <- OnDamage()
    del += @(target : string; amount : int) : int {
        print("handler 1: {amount}\n")
        return amount
    }
    del += @(target : string; amount : int) : int {
        print("handler 2: {amount * 2}\n")
        return amount * 2
    }
    del += @@apply_damage

    // All handlers are called; the last handler's return value is returned
    let result = del.invoke("orc", 10)
    // prints:
    //   handler 1: 10
    //   handler 2: 20
    //   apply_damage(orc, 10)
    // result == 20 — the value from the LAST handler

For non-void delegates, all handlers execute in order but only the **last**
handler's return value is returned.  Invoking a delegate with no handlers
registered is not an error: it returns a default-constructed value.


Void delegates
===============

Void delegates work the same way, but ``invoke`` returns nothing:

.. code-block:: das

    typedef OnNotify = delegate(type<function<(msg : string) : void>>)

    var del <- OnNotify()
    del += @(msg : string) {
        print("log: {msg}\n")
    }
    del += @(msg : string) {
        print("alert: {msg}\n")
    }
    del.invoke("server started")
    // output:
    //   log: server started
    //   alert: server started


Utilities: ``empty``, ``length``, ``clear``
============================================

.. code-block:: das

    var del <- OnTick()
    del.empty()     // true
    del.length()    // 0

    del += @() { print("tick!\n") }
    del += @() { print("tock!\n") }
    del.empty()     // false
    del.length()    // 2

    del.clear()
    del.empty()     // true


Iterating handlers
===================

Delegates support ``for``-loop iteration via the ``each`` iterator:

.. code-block:: das

    var del <- OnNotify()
    del += @(msg : string) { print("A\n") }
    del += @(msg : string) { print("B\n") }

    var count = 0
    for (handler in del) {
        count++
    }
    // count = 2

The loop walks the underlying lambda list.  ``each`` is declared
``var self : T ==const``, so the delegate must be a ``var`` --- a ``let``
delegate cannot be iterated at all --- and the loop variable is the
stored handler itself, not a copy.


Practical example — event system
==================================

.. code-block:: das

    typedef OnPlayerHit = delegate(type<function<(player : string; damage : int) : int>>)

    [unused_argument(player)]
    def armor_reduction(player : string; damage : int) : int {
        let reduced = damage - 5
        print("armor: {damage} -> {reduced}\n")
        return max(reduced, 0)
    }

    def log_hit(player : string; damage : int) : int {
        print("log: {player} took {damage} damage\n")
        return damage
    }

    var on_hit <- OnPlayerHit()
    on_hit += @@armor_reduction
    on_hit += @@log_hit

    let final_damage = on_hit.invoke("Hero", 12)
    // prints:
    //   armor: 12 -> 7
    //   log: Hero took 12 damage
    // final_damage == 12 — log_hit ran last and returned the damage

    // Replace all handlers — god mode!
    on_hit := @(player : string; damage : int) : int {
        return 0
    }
    on_hit.invoke("Hero", 100)  // returns 0


API summary
============

+-----------------------------------+--------------------------------------------------+
| Operation                         | Description                                      |
+===================================+==================================================+
| ``typedef T = delegate(type<F>)`` | Declare a delegate type from function signature  |
+-----------------------------------+--------------------------------------------------+
| ``T()``                           | Construct empty delegate                         |
+-----------------------------------+--------------------------------------------------+
| ``T(lambda)``                     | Construct with one lambda handler                |
+-----------------------------------+--------------------------------------------------+
| ``T(@@func)``                     | Construct with one function pointer              |
+-----------------------------------+--------------------------------------------------+
| ``del := handler``                | Clear and set single handler                     |
+-----------------------------------+--------------------------------------------------+
| ``del += handler``                | Append handler                                   |
+-----------------------------------+--------------------------------------------------+
| ``del.invoke(args...)``           | Call all handlers, return last result            |
+-----------------------------------+--------------------------------------------------+
| ``del.empty()``                   | True if no handlers registered                   |
+-----------------------------------+--------------------------------------------------+
| ``del.length()``                  | Number of registered handlers                    |
+-----------------------------------+--------------------------------------------------+
| ``del.clear()``                   | Remove all handlers                              |
+-----------------------------------+--------------------------------------------------+
| ``for (h in del)``                | Iterate over handlers                            |
+-----------------------------------+--------------------------------------------------+


.. seealso::

    Full source: :download:`tutorials/language/51_delegate.das <../../../../tutorials/language/51_delegate.das>`

    Previous tutorial: :ref:`tutorial_soa`

    Next tutorial: :ref:`tutorial_option_and_result`

    :ref:`tutorial_lambdas`, :ref:`tutorial_function_pointers`, :ref:`tutorial_blocks`
