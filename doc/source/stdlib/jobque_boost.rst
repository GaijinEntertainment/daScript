
.. _stdlib_jobque_boost:

==================================
Boost package for jobs and threads
==================================

The JOBQUE_BOOST module provides high-level job queue abstractions built on
the low-level ``jobque`` primitives. It includes ``with_job``, ``with_job_status``,
and channel-based patterns for simplified concurrent programming.

See also :doc:`jobque` for the low-level job queue primitives.

All functions and symbols are in "jobque_boost" module, use require to get access to it. ::

    require daslib/jobque_boost

Example: ::

    require daslib/jobque_boost

        [export]
        def main() {
            with_job_status(1) <| $(status) {
                new_thread <| @() {
                    print("from thread\n")
                    status |> notify_and_release()
                }
                status |> join()
                print("thread done\n")
            }
        }
        // output:
        // from thread
        // thread done

++++++++++++++++++++
Function annotations
++++++++++++++++++++

.. _handle-jobque_boost-NewJobMacro:

.. das:attribute:: NewJobMacro

this macro handles `new_job` and `new_thread` calls.
the call is replaced with `new_job_invoke` and `new_thread_invoke` accordingly.
a cloning infrastructure is generated for the lambda, which is invoked in the new context.

.. _handle-jobque_boost-ParallelForJobMacro:

.. das:attribute:: ParallelForJobMacro

Base macro for parallel_for, parallel_for_each, and parallel_map.
Wraps the block body in ``new_job`` and redirects to the runtime implementation.

.. _handle-jobque_boost-ParallelForEachJobMacro:

.. das:attribute:: ParallelForEachJobMacro

This macro handles `parallel_for_each`.
Wraps block body in ``new_job`` and redirects to ``_parallel_for_each``.

.. _handle-jobque_boost-ParallelMapJobMacro:

.. das:attribute:: ParallelMapJobMacro

This macro handles `parallel_map`.
Wraps block body in ``new_job`` and redirects to ``_parallel_map``.

+++++++++++
Invocations
+++++++++++

  *  :ref:`new_job (var l: lambda\<():void\>) <function-jobque_boost_new_job_lambda_ls__c_void_gr_>` 
  *  :ref:`new_thread (var l: lambda\<():void\>) <function-jobque_boost_new_thread_lambda_ls__c_void_gr_>` 

.. _function-jobque_boost_new_job_lambda_ls__c_void_gr_:

.. das:function:: new_job(l: lambda<():void>)

Create a new job.
    * new context is cloned from the current context.
    * lambda is cloned to the new context.
    * new job is added to the job queue.
    * once new job is invoked, lambda is invoked on the new context on the job thread.

:Arguments: * **l** : lambda<void>

.. _function-jobque_boost_new_thread_lambda_ls__c_void_gr_:

.. das:function:: new_thread(l: lambda<():void>)

Create a new thread
    * new context is cloned from the current context.
    * lambda is cloned to the new context.
    * new thread is created.
    * lambda is invoked on the new context on the new thread.

:Arguments: * **l** : lambda<void>

+++++++++
Iteration
+++++++++

  *  :ref:`each (var channel: Channel?; tinfo: auto(TT)) : auto <function-jobque_boost_each_Channel_q__autoTT_0x152>` 
  *  :ref:`each_clone (var channel: Channel?; tinfo: auto(TT)) : auto <function-jobque_boost_each_clone_Channel_q__autoTT_0x16c>` 
  *  :ref:`for_each (channel: Channel?; blk: block\<(res:auto(TT)#):void\>) : auto <function-jobque_boost_for_each_Channel_q__block_ls_res_c_autoTT_hh__c_void_gr_>` 
  *  :ref:`for_each_clone (channel: Channel?; blk: block\<(res:auto(TT)#):void\>) : auto <function-jobque_boost_for_each_clone_Channel_q__block_ls_res_c_autoTT_hh__c_void_gr_>` 

.. _function-jobque_boost_each_Channel_q__autoTT_0x152:

.. das:function:: each(channel: Channel?; tinfo: auto(TT)) : auto

.. warning:: 
  This function is deprecated.

this iterator is used to iterate over the channel in order it was pushed.
iterator stops once channel is depleted (internal entry counter is 0)
iteration can happen on multiple threads or jobs at the same time.

:Arguments: * **channel** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **tinfo** : auto(TT)

.. _function-jobque_boost_each_clone_Channel_q__autoTT_0x16c:

.. das:function:: each_clone(channel: Channel?; tinfo: auto(TT)) : auto

this iterator is used to iterate over the channel in order it was pushed.
iterator stops once channel is depleted (internal entry counter is 0)
iteration can happen on multiple threads or jobs at the same time.

:Arguments: * **channel** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **tinfo** : auto(TT)

.. _function-jobque_boost_for_each_Channel_q__block_ls_res_c_autoTT_hh__c_void_gr_:

.. das:function:: for_each(channel: Channel?; blk: block<(res:auto(TT)#):void>) : auto

.. warning:: 
  This function is deprecated.

reads input from the channel (in order it was pushed) and invokes the block on each input.
stops once channel is depleted (internal entry counter is 0)
this can happen on multiple threads or jobs at the same time.

:Arguments: * **channel** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **blk** : block<(res:auto(TT)#):void>

.. _function-jobque_boost_for_each_clone_Channel_q__block_ls_res_c_autoTT_hh__c_void_gr_:

.. das:function:: for_each_clone(channel: Channel?; blk: block<(res:auto(TT)#):void>) : auto

reads input from the channel (in order it was pushed) and invokes the block on each input.
stops once channel is depleted (internal entry counter is 0)
this can happen on multiple threads or jobs at the same time.

:Arguments: * **channel** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **blk** : block<(res:auto(TT)#):void>

++++++++++++
Passing data
++++++++++++

  *  :ref:`push (channel: Channel?; data: auto?) : auto <function-jobque_boost_push_Channel_q__auto_q_>` 
  *  :ref:`push_batch (channel: Channel?; data: array\<auto?\>) : auto <function-jobque_boost_push_batch_Channel_q__array_ls_auto_q__gr_>` 
  *  :ref:`push_batch_clone (channel: Channel?; data: array\<auto(TT)\>) : auto <function-jobque_boost_push_batch_clone_Channel_q__array_ls_autoTT_gr_>` 
  *  :ref:`push_clone (channel: Channel?; data: auto(TT)) : auto <function-jobque_boost_push_clone_Channel_q__autoTT_0xfa>` 

.. _function-jobque_boost_push_Channel_q__auto_q_:

.. das:function:: push(channel: Channel?; data: auto?) : auto

pushes value to the channel (at the end)

:Arguments: * **channel** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **data** : auto?

.. _function-jobque_boost_push_batch_Channel_q__array_ls_auto_q__gr_:

.. das:function:: push_batch(channel: Channel?; data: array<auto?>) : auto

pushes values to the channel (at the end)

:Arguments: * **channel** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **data** : array<auto?>

.. _function-jobque_boost_push_batch_clone_Channel_q__array_ls_autoTT_gr_:

.. das:function:: push_batch_clone(channel: Channel?; data: array<auto(TT)>) : auto

clones data and pushes values to the channel (at the end)

:Arguments: * **channel** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **data** : array<auto(TT)>

.. _function-jobque_boost_push_clone_Channel_q__autoTT_0xfa:

.. das:function:: push_clone(channel: Channel?; data: auto(TT)) : auto

clones data and pushes value to the channel (at the end)

:Arguments: * **channel** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **data** : auto(TT)

++++++++++++++
Receiving data
++++++++++++++

  *  :ref:`gather (ch: Channel?; blk: block\<(arg:auto(TT)#):void\>) : auto <function-jobque_boost_gather_Channel_q__block_ls_arg_c_autoTT_hh__c_void_gr_>` 
  *  :ref:`gather_and_forward (ch: Channel?; toCh: Channel?; blk: block\<(arg:auto(TT)#):void\>) : auto <function-jobque_boost_gather_and_forward_Channel_q__Channel_q__block_ls_arg_c_autoTT_hh__c_void_gr_>` 
  *  :ref:`gather_ex (ch: Channel?; blk: block\<(arg:auto(TT)#;info:TypeInfo const?;var ctx:Context):void\>) : auto <function-jobque_boost_gather_ex_Channel_q__block_ls_arg_c_autoTT_hh_;info_c_TypeInfo_const_q_;var_ctx_c_Context_c_void_gr_>` 
  *  :ref:`peek (ch: Channel?; blk: block\<(arg:auto(TT)#):void\>) : auto <function-jobque_boost_peek_Channel_q__block_ls_arg_c_autoTT_hh__c_void_gr_>` 
  *  :ref:`pop_and_clone_one (channel: Channel?; blk: block\<(res:auto(TT)#):void\>) : auto <function-jobque_boost_pop_and_clone_one_Channel_q__block_ls_res_c_autoTT_hh__c_void_gr_>` 
  *  :ref:`pop_one (channel: Channel?; blk: block\<(res:auto(TT)#):void\>) : auto <function-jobque_boost_pop_one_Channel_q__block_ls_res_c_autoTT_hh__c_void_gr_>` 
  *  :ref:`pop_with_timeout (channel: Channel?; timeout_ms: int; blk: block\<(res:auto(TT)#):void\>) : bool <function-jobque_boost_pop_with_timeout_Channel_q__int_block_ls_res_c_autoTT_hh__c_void_gr_>` 
  *  :ref:`pop_with_timeout_clone (channel: Channel?; timeout_ms: int; blk: block\<(res:auto(TT)#):void\>) : bool <function-jobque_boost_pop_with_timeout_clone_Channel_q__int_block_ls_res_c_autoTT_hh__c_void_gr_>` 
  *  :ref:`try_pop (channel: Channel?; blk: block\<(res:auto(TT)#):void\>) : bool <function-jobque_boost_try_pop_Channel_q__block_ls_res_c_autoTT_hh__c_void_gr_>` 
  *  :ref:`try_pop_clone (channel: Channel?; blk: block\<(res:auto(TT)#):void\>) : bool <function-jobque_boost_try_pop_clone_Channel_q__block_ls_res_c_autoTT_hh__c_void_gr_>` 

.. _function-jobque_boost_gather_Channel_q__block_ls_arg_c_autoTT_hh__c_void_gr_:

.. das:function:: gather(ch: Channel?; blk: block<(arg:auto(TT)#):void>) : auto

reads input from the channel (in order it was pushed) and invokes the block on each input.
afterwards input is consumed

:Arguments: * **ch** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **blk** : block<(arg:auto(TT)#):void>

.. _function-jobque_boost_gather_and_forward_Channel_q__Channel_q__block_ls_arg_c_autoTT_hh__c_void_gr_:

.. das:function:: gather_and_forward(ch: Channel?; toCh: Channel?; blk: block<(arg:auto(TT)#):void>) : auto

reads input from the channel (in order it was pushed) and invokes the block on each input.
afterwards input is consumed

:Arguments: * **ch** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **toCh** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **blk** : block<(arg:auto(TT)#):void>

.. _function-jobque_boost_gather_ex_Channel_q__block_ls_arg_c_autoTT_hh_;info_c_TypeInfo_const_q_;var_ctx_c_Context_c_void_gr_:

.. das:function:: gather_ex(ch: Channel?; blk: block<(arg:auto(TT)#;info:TypeInfo const?;var ctx:Context):void>) : auto

reads input from the channel (in order it was pushed) and invokes the block on each input.
afterwards input is consumed

:Arguments: * **ch** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **blk** : block<(arg:auto(TT)#;info: :ref:`TypeInfo <handle-rtti-TypeInfo>` ?;ctx: :ref:`Context <handle-rtti-Context>` ):void>

.. _function-jobque_boost_peek_Channel_q__block_ls_arg_c_autoTT_hh__c_void_gr_:

.. das:function:: peek(ch: Channel?; blk: block<(arg:auto(TT)#):void>) : auto

reads input from the channel (in order it was pushed) and invokes the block on each input.
afterwards input is not consumed

:Arguments: * **ch** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **blk** : block<(arg:auto(TT)#):void>

.. _function-jobque_boost_pop_and_clone_one_Channel_q__block_ls_res_c_autoTT_hh__c_void_gr_:

.. das:function:: pop_and_clone_one(channel: Channel?; blk: block<(res:auto(TT)#):void>) : auto

reads one command from channel

:Arguments: * **channel** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **blk** : block<(res:auto(TT)#):void>

.. _function-jobque_boost_pop_one_Channel_q__block_ls_res_c_autoTT_hh__c_void_gr_:

.. das:function:: pop_one(channel: Channel?; blk: block<(res:auto(TT)#):void>) : auto

.. warning:: 
  This function is deprecated.

reads one command from channel

:Arguments: * **channel** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **blk** : block<(res:auto(TT)#):void>

.. _function-jobque_boost_pop_with_timeout_Channel_q__int_block_ls_res_c_autoTT_hh__c_void_gr_:

.. das:function:: pop_with_timeout(channel: Channel?; timeout_ms: int; blk: block<(res:auto(TT)#):void>) : bool

Pop from channel with timeout in milliseconds.
Returns true if an item was available within the timeout, false if timed out or channel exhausted.

:Arguments: * **channel** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **timeout_ms** : int

            * **blk** : block<(res:auto(TT)#):void>

.. _function-jobque_boost_pop_with_timeout_clone_Channel_q__int_block_ls_res_c_autoTT_hh__c_void_gr_:

.. das:function:: pop_with_timeout_clone(channel: Channel?; timeout_ms: int; blk: block<(res:auto(TT)#):void>) : bool

Pop from channel with timeout and clone. Returns true if an item was available within the timeout.
The popped value is cloned to the current context before invoking the block.

:Arguments: * **channel** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **timeout_ms** : int

            * **blk** : block<(res:auto(TT)#):void>

.. _function-jobque_boost_try_pop_Channel_q__block_ls_res_c_autoTT_hh__c_void_gr_:

.. das:function:: try_pop(channel: Channel?; blk: block<(res:auto(TT)#):void>) : bool

Non-blocking pop from channel. Returns true if an item was available, false if channel was empty.
Does not wait for data — returns immediately.

:Arguments: * **channel** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **blk** : block<(res:auto(TT)#):void>

.. _function-jobque_boost_try_pop_clone_Channel_q__block_ls_res_c_autoTT_hh__c_void_gr_:

.. das:function:: try_pop_clone(channel: Channel?; blk: block<(res:auto(TT)#):void>) : bool

Non-blocking pop with clone from channel. Returns true if an item was available, false if channel was empty.
The popped value is cloned to the current context before invoking the block.

:Arguments: * **channel** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **blk** : block<(res:auto(TT)#):void>

+++++++++++++++
Synchronization
+++++++++++++++

  *  :ref:`done (var status: JobStatus?&) <function-jobque_boost_done_JobStatus_q_>` 
  *  :ref:`with_wait_group (blk: block\<(var status:JobStatus?):void\>) <function-jobque_boost_with_wait_group_block_ls_var_status_c_JobStatus_q__c_void_gr_>` 
  *  :ref:`with_wait_group (count: int; blk: block\<(var status:JobStatus?):void\>) <function-jobque_boost_with_wait_group_int_block_ls_var_status_c_JobStatus_q__c_void_gr_>` 

.. _function-jobque_boost_done_JobStatus_q_:

.. das:function:: done(status: JobStatus?&)

Mark one unit of work as done in a wait group.
Alias for ``notify_and_release``. Decrements the notification counter and releases the reference.
Sets the pointer to null, preventing double-release.

:Arguments: * **status** :  :ref:`JobStatus <handle-jobque-JobStatus>` ?&


with_wait_group
^^^^^^^^^^^^^^^

.. _function-jobque_boost_with_wait_group_block_ls_var_status_c_JobStatus_q__c_void_gr_:

.. das:function:: with_wait_group(blk: block<(var status:JobStatus?):void>)

Creates a wait group starting at count 0 with auto-join.
Use ``append`` to dynamically add expected notifications before dispatching work.
The block returns only after all notifications have been received.

:Arguments: * **blk** : block<(status: :ref:`JobStatus <handle-jobque-JobStatus>` ?):void>

.. _function-jobque_boost_with_wait_group_int_block_ls_var_status_c_JobStatus_q__c_void_gr_:

.. das:function:: with_wait_group(count: int; blk: block<(var status:JobStatus?):void>)

----

++++++++++++++++++
Parallel execution
++++++++++++++++++

  *  :ref:`_parallel_for (range_begin: int; range_end: int; num_jobs: int; blk: block\<(job_begin:int;job_end:int;var wg:JobStatus?):void\>) <function-jobque_boost__parallel_for_int_int_int_block_ls_job_begin_c_int;job_end_c_int;var_wg_c_JobStatus_q__c_void_gr_>` 
  *  :ref:`_parallel_for_each (arr: array\<auto(TT)\>; num_jobs: int; blk: block\<(job_begin:int;job_end:int;var wg:JobStatus?):void\>) : auto <function-jobque_boost__parallel_for_each_array_ls_autoTT_gr__int_block_ls_job_begin_c_int;job_end_c_int;var_wg_c_JobStatus_q__c_void_gr_>` 
  *  :ref:`_parallel_map (arr: array\<auto(TT)\>; num_jobs: int; var results_channel: Channel?; blk: block\<(job_begin:int;job_end:int;var ch:Channel?;var wg:JobStatus?):void\>) : auto <function-jobque_boost__parallel_map_array_ls_autoTT_gr__int_Channel_q__block_ls_job_begin_c_int;job_end_c_int;var_ch_c_Channel_q_;var_wg_c_JobStatus_q__c_void_gr_>` 
  *  :ref:`parallel_for (range_begin: int; range_end: int; num_jobs: int; blk: block\<(job_begin:int;job_end:int;var wg:JobStatus?):void\>) <function-jobque_boost_parallel_for_int_int_int_block_ls_job_begin_c_int;job_end_c_int;var_wg_c_JobStatus_q__c_void_gr_>` 
  *  :ref:`parallel_for_each (arr: array\<auto(TT)\>; num_jobs: int; blk: block\<(job_begin:int;job_end:int;var wg:JobStatus?):void\>) : auto <function-jobque_boost_parallel_for_each_array_ls_autoTT_gr__int_block_ls_job_begin_c_int;job_end_c_int;var_wg_c_JobStatus_q__c_void_gr_>` 
  *  :ref:`parallel_map (arr: array\<auto(TT)\>; num_jobs: int; var results_channel: Channel?; blk: block\<(job_begin:int;job_end:int;var ch:Channel?;var wg:JobStatus?):void\>) : auto <function-jobque_boost_parallel_map_array_ls_autoTT_gr__int_Channel_q__block_ls_job_begin_c_int;job_end_c_int;var_ch_c_Channel_q_;var_wg_c_JobStatus_q__c_void_gr_>` 

.. _function-jobque_boost__parallel_for_int_int_int_block_ls_job_begin_c_int;job_end_c_int;var_wg_c_JobStatus_q__c_void_gr_:

.. das:function:: _parallel_for(range_begin: int; range_end: int; num_jobs: int; blk: block<(job_begin:int;job_end:int;var wg:JobStatus?):void>)

Partitions ``[range_begin..range_end)`` into ``num_jobs`` chunks and invokes ``blk`` once per chunk
on the calling thread with ``(chunk_begin, chunk_end, wg)``. The block is expected to dispatch work
via ``new_job`` and call ``wg |> notify_and_release`` when each job finishes.
``parallel_for`` blocks until all notifications are received (via internal ``with_wait_group``).
Requires ``with_job_que`` context.

:Arguments: * **range_begin** : int

            * **range_end** : int

            * **num_jobs** : int

            * **blk** : block<(job_begin:int;job_end:int;wg: :ref:`JobStatus <handle-jobque-JobStatus>` ?):void>

.. _function-jobque_boost__parallel_for_each_array_ls_autoTT_gr__int_block_ls_job_begin_c_int;job_end_c_int;var_wg_c_JobStatus_q__c_void_gr_:

.. das:function:: _parallel_for_each(arr: array<auto(TT)>; num_jobs: int; blk: block<(job_begin:int;job_end:int;var wg:JobStatus?):void>) : auto

Runtime implementation for ``parallel_for_each``.
Partitions array indices ``[0..length(arr))`` into ``num_jobs`` chunks and invokes ``blk``
with ``(chunk_begin_idx, chunk_end_idx, wg)`` on the calling thread.
The block should dispatch ``new_job`` calls that process ``arr[i]`` for ``i`` in ``[chunk_begin_idx..chunk_end_idx)``,
then call ``wg |> notify_and_release``.
Blocks until all jobs finish. Requires ``with_job_que`` context.

:Arguments: * **arr** : array<auto(TT)>

            * **num_jobs** : int

            * **blk** : block<(job_begin:int;job_end:int;wg: :ref:`JobStatus <handle-jobque-JobStatus>` ?):void>

.. _function-jobque_boost__parallel_map_array_ls_autoTT_gr__int_Channel_q__block_ls_job_begin_c_int;job_end_c_int;var_ch_c_Channel_q_;var_wg_c_JobStatus_q__c_void_gr_:

.. das:function:: _parallel_map(arr: array<auto(TT)>; num_jobs: int; results_channel: Channel?; blk: block<(job_begin:int;job_end:int;var ch:Channel?;var wg:JobStatus?):void>) : auto

Runtime implementation for ``parallel_map``.
Partitions array indices ``[0..length(arr))`` into ``num_jobs`` chunks and invokes ``blk``
on the calling thread with ``(chunk_begin_idx, chunk_end_idx, results_channel, wg)``.
Blocks until all jobs finish. Results are available in ``results_channel`` after this call returns.
Requires ``with_job_que`` context.

:Arguments: * **arr** : array<auto(TT)>

            * **num_jobs** : int

            * **results_channel** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **blk** : block<(job_begin:int;job_end:int;ch: :ref:`Channel <handle-jobque-Channel>` ?;wg: :ref:`JobStatus <handle-jobque-JobStatus>` ?):void>

.. _function-jobque_boost_parallel_for_int_int_int_block_ls_job_begin_c_int;job_end_c_int;var_wg_c_JobStatus_q__c_void_gr_:

.. das:function:: parallel_for(range_begin: int; range_end: int; num_jobs: int; blk: block<(job_begin:int;job_end:int;var wg:JobStatus?):void>)

this one is stub for _parallel_for

:Arguments: * **range_begin** : int

            * **range_end** : int

            * **num_jobs** : int

            * **blk** : block<(job_begin:int;job_end:int;wg: :ref:`JobStatus <handle-jobque-JobStatus>` ?):void>

.. _function-jobque_boost_parallel_for_each_array_ls_autoTT_gr__int_block_ls_job_begin_c_int;job_end_c_int;var_wg_c_JobStatus_q__c_void_gr_:

.. das:function:: parallel_for_each(arr: array<auto(TT)>; num_jobs: int; blk: block<(job_begin:int;job_end:int;var wg:JobStatus?):void>) : auto

Convenience wrapper around ``parallel_for`` for arrays.
Partitions array indices ``[0..length(arr))`` into ``num_jobs`` chunks.
The block body is automatically wrapped in ``new_job``.
Blocks until all jobs finish. Requires ``with_job_que`` context.

:Arguments: * **arr** : array<auto(TT)>

            * **num_jobs** : int

            * **blk** : block<(job_begin:int;job_end:int;wg: :ref:`JobStatus <handle-jobque-JobStatus>` ?):void>

.. _function-jobque_boost_parallel_map_array_ls_autoTT_gr__int_Channel_q__block_ls_job_begin_c_int;job_end_c_int;var_ch_c_Channel_q_;var_wg_c_JobStatus_q__c_void_gr_:

.. das:function:: parallel_map(arr: array<auto(TT)>; num_jobs: int; results_channel: Channel?; blk: block<(job_begin:int;job_end:int;var ch:Channel?;var wg:JobStatus?):void>) : auto

Partitions array indices ``[0..length(arr))`` into ``num_jobs`` chunks.
The block body is automatically wrapped in ``new_job``.
Blocks until all jobs finish. Results are available in ``results_channel`` after this call returns.
Requires ``with_job_que`` context.

:Arguments: * **arr** : array<auto(TT)>

            * **num_jobs** : int

            * **results_channel** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **blk** : block<(job_begin:int;job_end:int;ch: :ref:`Channel <handle-jobque-Channel>` ?;wg: :ref:`JobStatus <handle-jobque-JobStatus>` ?):void>

++++++++++++++++++
LockBox operations
++++++++++++++++++

  *  :ref:`clear (box: LockBox?; type_: auto(TT)) : auto <function-jobque_boost_clear_LockBox_q__autoTT_0x143>` 
  *  :ref:`get (box: LockBox?; blk: block\<(res:auto(TT)#):void\>) : auto <function-jobque_boost_get_LockBox_q__block_ls_res_c_autoTT_hh__c_void_gr_>` 
  *  :ref:`set (box: LockBox?; data: auto?) : auto <function-jobque_boost_set_LockBox_q__auto_q_>` 
  *  :ref:`set (box: LockBox?; data: auto(TT)) : auto <function-jobque_boost_set_LockBox_q__autoTT_0x11a>` 
  *  :ref:`update (box: LockBox?; blk: block\<(var res:auto(TT)#):void\>) : auto <function-jobque_boost_update_LockBox_q__block_ls_var_res_c_autoTT_hh__c_void_gr_>` 

.. _function-jobque_boost_clear_LockBox_q__autoTT_0x143:

.. das:function:: clear(box: LockBox?; type_: auto(TT)) : auto

clear value from the lock box

:Arguments: * **box** :  :ref:`LockBox <handle-jobque-LockBox>` ?

            * **type_** : auto(TT)

.. _function-jobque_boost_get_LockBox_q__block_ls_res_c_autoTT_hh__c_void_gr_:

.. das:function:: get(box: LockBox?; blk: block<(res:auto(TT)#):void>) : auto

reads value from the lock box and invokes the block on it

:Arguments: * **box** :  :ref:`LockBox <handle-jobque-LockBox>` ?

            * **blk** : block<(res:auto(TT)#):void>


set
^^^

.. _function-jobque_boost_set_LockBox_q__auto_q_:

.. das:function:: set(box: LockBox?; data: auto?) : auto

sets value to the lock box

:Arguments: * **box** :  :ref:`LockBox <handle-jobque-LockBox>` ?

            * **data** : auto?

.. _function-jobque_boost_set_LockBox_q__autoTT_0x11a:

.. das:function:: set(box: LockBox?; data: auto(TT)) : auto

----

.. _function-jobque_boost_update_LockBox_q__block_ls_var_res_c_autoTT_hh__c_void_gr_:

.. das:function:: update(box: LockBox?; blk: block<(var res:auto(TT)#):void>) : auto

update value in the lock box and invokes the block on it

:Arguments: * **box** :  :ref:`LockBox <handle-jobque-LockBox>` ?

            * **blk** : block<(res:auto(TT)#):void>

++++++++++++++++++++++++
Internal capture details
++++++++++++++++++++++++

  *  :ref:`capture_jobque_channel (var ch: Channel?) : Channel? <function-jobque_boost_capture_jobque_channel_Channel_q_>` 
  *  :ref:`capture_jobque_job_status (var js: JobStatus?) : JobStatus? <function-jobque_boost_capture_jobque_job_status_JobStatus_q_>` 
  *  :ref:`capture_jobque_lock_box (var js: LockBox?) : LockBox? <function-jobque_boost_capture_jobque_lock_box_LockBox_q_>` 
  *  :ref:`release_capture_jobque_channel (ch: Channel?) <function-jobque_boost_release_capture_jobque_channel_Channel_q_>` 
  *  :ref:`release_capture_jobque_job_status (js: JobStatus?) <function-jobque_boost_release_capture_jobque_job_status_JobStatus_q_>` 
  *  :ref:`release_capture_jobque_lock_box (js: LockBox?) <function-jobque_boost_release_capture_jobque_lock_box_LockBox_q_>` 

.. _function-jobque_boost_capture_jobque_channel_Channel_q_:

.. das:function:: capture_jobque_channel(ch: Channel?) : Channel?

this function is used to capture a channel that is used by the jobque.

:Arguments: * **ch** :  :ref:`Channel <handle-jobque-Channel>` ?

.. _function-jobque_boost_capture_jobque_job_status_JobStatus_q_:

.. das:function:: capture_jobque_job_status(js: JobStatus?) : JobStatus?

this function is used to capture a job status that is used by the jobque.

:Arguments: * **js** :  :ref:`JobStatus <handle-jobque-JobStatus>` ?

.. _function-jobque_boost_capture_jobque_lock_box_LockBox_q_:

.. das:function:: capture_jobque_lock_box(js: LockBox?) : LockBox?

this function is used to capture a lock box that is used by the jobque.

:Arguments: * **js** :  :ref:`LockBox <handle-jobque-LockBox>` ?

.. _function-jobque_boost_release_capture_jobque_channel_Channel_q_:

.. das:function:: release_capture_jobque_channel(ch: Channel?)

this function is used to release a channel that is used by the jobque.

:Arguments: * **ch** :  :ref:`Channel <handle-jobque-Channel>` ?

.. _function-jobque_boost_release_capture_jobque_job_status_JobStatus_q_:

.. das:function:: release_capture_jobque_job_status(js: JobStatus?)

this function is used to release a job status that is used by the jobque.

:Arguments: * **js** :  :ref:`JobStatus <handle-jobque-JobStatus>` ?

.. _function-jobque_boost_release_capture_jobque_lock_box_LockBox_q_:

.. das:function:: release_capture_jobque_lock_box(js: LockBox?)

this function is used to release a lock box that is used by the jobque.

:Arguments: * **js** :  :ref:`LockBox <handle-jobque-LockBox>` ?


