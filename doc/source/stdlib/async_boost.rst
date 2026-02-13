
.. _stdlib_async_boost:

============================
Async/await coroutine macros
============================

The ASYNC_BOOST module implements an async/await pattern for daScript using
channels and coroutines. It provides ``async`` for launching concurrent tasks
and ``await`` for waiting on their results, built on top of the job queue
infrastructure.

All functions and symbols are in "async_boost" module, use require to get access to it. ::

    require daslib/async_boost

++++++++++++++++++++
Function annotations
++++++++++++++++++++

.. _handle-async_boost-AwaitMacro:

.. das:attribute:: AwaitMacro

Function annotation that implements coroutine await semantics.

.. _handle-async_boost-AwaitCoroutineMacro:

.. das:attribute:: AwaitCoroutineMacro

This macro converts await(<coroutine>) expression into::

    for t in THAT
        yield t

The idea is that coroutine or generator can continuesly yield from another sub-coroutine or generator.

.. _handle-async_boost-async:

.. das:attribute:: async

This macro converts function into generator.
Generator yields bool if its a void function (coroutine), and yields the return type otherwise (async return).
async function can wait for another async function using await(<async fn call>).
use 'return false' to immediately return from the generator.

++++++++
Awaiting
++++++++

  *  :ref:`await_next_frame () <function-async_boost_await_next_frame>` 
  *  :ref:`await (var a: iterator\<bool\>) : bool <function-async_boost_await_iterator_ls_bool_gr_>` 
  *  :ref:`await (var a: iterator\<variant\<res:auto(T);wait:bool\>\>) : T <function-async_boost_await_iterator_ls_variant_ls_res_c_autoT;wait_c_bool_gr__gr_>` 

.. _function-async_boost_await_next_frame:

.. das:function:: await_next_frame()

This function is used to suspend coroutine until next frame.

.. _function-async_boost_await_iterator_ls_bool_gr_:

.. das:function:: await(a: iterator<bool>) : bool

This function is used to wait for the result of the async function.

:Arguments: * **a** : iterator<bool>

.. _function-async_boost_await_iterator_ls_variant_ls_res_c_autoT;wait_c_bool_gr__gr_:

.. das:function:: await(a: iterator<variant<res:auto(T);wait:bool>>) : T

This function is used to wait for the result of the async function.

:Arguments: * **a** : iterator<variant<res:auto(T);wait:bool>>

+++++++++++++++++++
Running async tasks
+++++++++++++++++++

  *  :ref:`async_run (var a: iterator\<auto\>) : auto <function-async_boost_async_run_iterator_ls_auto_gr_>` 
  *  :ref:`async_run_all (var a: array\<iterator\<auto\>\>) : auto <function-async_boost_async_run_all_array_ls_iterator_ls_auto_gr__gr_>` 

.. _function-async_boost_async_run_iterator_ls_auto_gr_:

.. das:function:: async_run(a: iterator<auto>) : auto

This function runs async function until it is finished.

:Arguments: * **a** : iterator<auto>

.. _function-async_boost_async_run_all_array_ls_iterator_ls_auto_gr__gr_:

.. das:function:: async_run_all(a: array<iterator<auto>>) : auto

This function runs all async function until they are finished (in parallel, starting from the last one).

:Arguments: * **a** : array<iterator<auto>>


