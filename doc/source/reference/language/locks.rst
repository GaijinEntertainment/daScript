.. _locks:


=====
Locks
=====

.. index:: single: locks

There are several locking mechanisms available in Daslang, designed to ensure the runtime safety of the code.

----------------
Context locks
----------------

A ``Context`` can be locked and unlocked via the ``lock`` and ``unlock`` functions from the C++ side.
When locked, a ``Context`` cannot be restarted. ``tryRestartAndLock`` restarts the context if it is not locked, and then locks it regardless.
The main reason to lock a context is when data on the heap is accessed externally. Heap collection is safe to perform on a locked context.

------------------------------
Array and Table locks
------------------------------

An ``Array`` or ``Table`` can be locked and unlocked explicitly. When locked, they cannot be modified.
Calling ``resize``, ``reserve``, ``push``, ``emplace``, ``erase``, etc. on a locked ``Array`` will cause a ``panic``.
Accessing a locked ``Table``'s elements via the ``[]`` operator will also cause a ``panic``.

Arrays are locked when iterated over, preventing modification during iteration.
The ``keys`` and ``values`` iterators lock a ``Table`` as well. Tables are also locked during ``find*`` operations.

------------------------------
Array and Table lock checking
------------------------------

Array and table lock checking extends to data structures that internally contain other arrays or tables.

Consider the following example:

.. code-block:: das

    var a : array < array<int> >
    ...
    for ( b in a[0] ) {
        a |> resize(100500)
    }

The ``resize`` operation on the ``a`` array will cause ``panic`` because ``a[0]`` is locked during the iteration.
This test, however, can only happen in runtime. The compiler generates custom ``resize`` code, which verifies locks:

.. code-block:: das

    def private builtin`resize ( var Arr:array<array<int> aka numT> explicit; newSize:int const ) {
        _builtin_verify_locks(Arr)
        __builtin_array_resize(Arr,newSize,24,__context__)
    }

The ``_builtin_verify_locks`` function iterates over the provided data and verifies that each ``Array`` or ``Table`` is not locked.
If any is locked, a ``panic`` occurs.

Custom operations will only be generated, if the underlying type needs lock checks.

The following operations perform lock checks on data structures:

.. code-block:: text

    * a <- b
    * return <- a
    * resize
    * reserve
    * push
    * push_clone
    * emplace
    * pop
    * erase
    * clear
    * insert
    * for the ``Table``

Lock checking can be explicitly disabled:
    * for a specific ``Array`` or ``Table`` by using the ``set_verify_array_locks`` and ``set_verify_table_locks`` functions
    * for a structure type with the ``[skip_field_lock_check]`` annotation
    * for an entire function with the ``[skip_lock_check]`` annotation
    * for the entire context with ``options skip_lock_checks``
    * for the entire context with the ``set_verify_context_locks`` function

.. seealso::

    :ref:`Arrays <arrays>` and :ref:`Tables <tables>` for the container types that support locking,
    :ref:`Iterators <iterators>` for iteration patterns that lock containers,
    :ref:`Annotations <annotations>` for ``[skip_lock_check]`` and ``[skip_field_lock_check]``,
    :ref:`Options <options>` for ``skip_lock_checks``,
    :ref:`Contexts <contexts>` for context-level locking.


