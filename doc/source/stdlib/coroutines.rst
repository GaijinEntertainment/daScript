
.. _stdlib_coroutines:

===========================================
Coroutines and additional generator support
===========================================

The COROUTINES module exposes coroutine infrastructure, as well as additional yielding facilities.

The following example illustrates iterating over the elements of a tree. `each_async_generator` implements straight up iterator,
where 'yield_from' helper is used to continue iterating over leafs. `[coroutine]` annotation converts function into coroutine.
If need be, return type of the function can specify coroutine yield type::

    require daslib/coroutines

    struct Tree
        data : int
        left, right : Tree?

    // yield from example
    def each_async_generator(tree : Tree?)
        return <- generator<int>() <|
            if tree.left != null
                yeild_from <| each_async_generator(tree.left)
            yield tree.data
            if tree.right != null
                yeild_from <| each_async_generator(tree.right)
            return false

    // coroutine as function
    [coroutine]
    def each_async(tree : Tree?) : int
        if tree.left != null
            co_await <| each_async(tree.left)
        yield tree.data
        if tree.right != null
            co_await <| each_async(tree.right)

All functions and symbols are in "coroutines" module, use require to get access to it. ::

    require daslib/coroutines

++++++++++++
Type aliases
++++++++++++

.. _alias-Coroutine:

.. das:attribute:: Coroutine = iterator<bool>

A coroutine is a generator that yields bool to indicate if it is still running.

.. _alias-Coroutines:

.. das:attribute:: Coroutines = array<iterator<bool>>

An array of coroutines.

++++++++++++++++++++
Function annotations
++++++++++++++++++++

.. _handle-coroutines-coroutine:

.. das:attribute:: coroutine

This macro converts coroutine function into generator, adds return false.
Daslang impelmentation of coroutine is generator based. Function is converted into a state machine,
which can be resumed and suspended. The function is converted into a generator.
Generator yields bool if its a void coroutine, and yields the return type otherwise.
If return type is specified coroutine can serve as an advanced form of a generator.

+++++++++++
Call macros
+++++++++++

.. _call-macro-coroutines-co_continue:

.. das:attribute:: co_continue

This macro converts co_continue to yield true.
The idea is that coroutine without specified type is underneath a coroutine which yields bool.
That way co_continue() does not distract from the fact that it is a generator<bool>.

.. _call-macro-coroutines-co_await:

.. das:attribute:: co_await

This macro converts co_await(sub_coroutine) into::

    for t in subroutine
        yield t

The idea is that coroutine or generator can wait for a sub-coroutine to finish.

.. _call-macro-coroutines-yeild_from:

.. das:attribute:: yeild_from

This macro converts yield_from(THAT) expression into::

    for t in THAT
        yield t

The idea is that coroutine or generator can continuesly yield from another sub-coroutine or generator.

++++++++++++++++++++++++++++++
Top level coroutine evaluation
++++++++++++++++++++++++++++++

  *  :ref:`cr_run (var a: Coroutine) <function-coroutines_cr_run_Coroutine>` 
  *  :ref:`cr_run_all (var a: Coroutines) <function-coroutines_cr_run_all_Coroutines>` 

.. _function-coroutines_cr_run_Coroutine:

.. das:function:: cr_run(a: Coroutine)

This function runs coroutine until it is finished.

:Arguments: * **a** :  :ref:`Coroutine <alias-Coroutine>` 

.. _function-coroutines_cr_run_all_Coroutines:

.. das:function:: cr_run_all(a: Coroutines)

This function runs all coroutines until they are finished.

:Arguments: * **a** :  :ref:`Coroutines <alias-Coroutines>` 


