
.. _stdlib_async_boost:

============================
Async/await coroutine macros
============================

.. das:module:: async_boost

The ASYNC_BOOST module implements an async/await pattern for daslang using
generator-based cooperative multitasking. It provides the ``[async]`` function
annotation, ``await`` for waiting on results, and ``await_next_frame`` for
suspending until the next step. Under the hood every ``[async]`` function is
transformed into a state-machine generator — no threads, channels, or job
queues are involved.

All functions and symbols are in "async_boost" module, use require to get access to it.

.. code-block:: das

    require daslib/async_boost

.. seealso::

   :ref:`tutorial_async` — Tutorial 49: Async / Await.

   :ref:`stdlib_coroutines` — coroutines module (underlying generator framework).



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

The idea is that coroutine or generator can continuously yield from another sub-coroutine or generator.


.. _handle-async_boost-async:

.. das:attribute:: async

This macro converts function into generator.
Generator yields bool if its a void function (coroutine), and yields the return type otherwise (async return).
async function can wait for another async function using await(<async fn call>).
use 'return false' to immediately return from the generator.



++++++++
Awaiting
++++++++

  *  :ref:`await (var a: iterator\<bool\>) : bool <function-async_boost_await_iterator_ls_bool_gr_>`
  *  :ref:`await (var a: iterator\<variant\<res:auto(T);wait:bool\>\>) : T <function-async_boost_await_iterator_ls_variant_ls_res_c_autoT;wait_c_bool_gr__gr_>`
  *  :ref:`await_next_frame () <function-async_boost_await_next_frame>`


await
^^^^^

.. _function-async_boost_await_iterator_ls_bool_gr_:

.. das:function:: await(a: iterator<bool>) : bool

This function is used to wait for the result of the async function.


:Arguments: * **a** : iterator<bool>

.. _function-async_boost_await_iterator_ls_variant_ls_res_c_autoT;wait_c_bool_gr__gr_:

.. das:function:: await(a: iterator<variant<res:auto(T);wait:bool>>) : T

----

.. _function-async_boost_await_next_frame:

.. das:function:: await_next_frame()

This function is used to suspend coroutine until next frame.



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


+++++++++++++
Uncategorized
+++++++++++++

.. _function-async_boost_async_timeout_iterator_ls_auto_gr__int:

.. das:function:: async_timeout(a: iterator<auto>; max_frames: int) : bool

This function runs an async function for at most `max_frames` frames.
Returns ``true`` if the async function completed within the limit,
``false`` if it was terminated due to timeout.


:Arguments: * **a** : iterator<auto>

            * **max_frames** : int

.. _function-async_boost_async_race_iterator_ls_auto_gr__iterator_ls_auto_gr_:

.. das:function:: async_race(a: iterator<auto>; b: iterator<auto>) : int

This function runs two async functions concurrently and returns the
index (0 or 1) of whichever finishes first. The other is abandoned.


:Arguments: * **a** : iterator<auto>

            * **b** : iterator<auto>


