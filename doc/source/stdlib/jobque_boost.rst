
.. _stdlib_jobque_boost:

==================================
Boost package for jobs and threads
==================================

The JOBQUE_BOOST module provides high-level job queue abstractions built on
the low-level ``jobque`` primitives. It includes ``with_job``, ``with_job_status``,
and channel-based patterns for simplified concurrent programming.

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
a cloning infastructure is generated for the lambda, which is invoked in the new context.

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

  *  :ref:`for_each (channel: Channel?; blk: block\<(res:auto(TT)#):void\>) : auto <function-jobque_boost_for_each_Channel_q__block_ls_res_c_autoTT_hh__c_void_gr_>` 
  *  :ref:`each (var channel: Channel?; tinfo: auto(TT)) : auto <function-jobque_boost_each_Channel_q__autoTT>` 

.. _function-jobque_boost_for_each_Channel_q__block_ls_res_c_autoTT_hh__c_void_gr_:

.. das:function:: for_each(channel: Channel?; blk: block<(res:auto(TT)#):void>) : auto

.. warning:: 
  This function is deprecated.

reads input from the channel (in order it was pushed) and invokes the block on each input.
stops once channel is depleted (internal entry counter is 0)
this can happen on multiple threads or jobs at the same time.

:Arguments: * **channel** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **blk** : block<(res:auto(TT)#):void>

.. _function-jobque_boost_each_Channel_q__autoTT:

.. das:function:: each(channel: Channel?; tinfo: auto(TT)) : auto

.. warning:: 
  This function is deprecated.

this iterator is used to iterate over the channel in order it was pushed.
iterator stops once channel is depleted (internal entry counter is 0)
iteration can happen on multiple threads or jobs at the same time.

:Arguments: * **channel** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **tinfo** : auto(TT)

++++++++++++
Passing data
++++++++++++

  *  :ref:`push_clone (channel: Channel?; data: auto(TT)) : auto <function-jobque_boost_push_clone_Channel_q__autoTT>` 
  *  :ref:`push (channel: Channel?; data: auto?) : auto <function-jobque_boost_push_Channel_q__auto_q_>` 

.. _function-jobque_boost_push_clone_Channel_q__autoTT:

.. das:function:: push_clone(channel: Channel?; data: auto(TT)) : auto

clones data and pushed value to the channel (at the end)

:Arguments: * **channel** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **data** : auto(TT)

.. _function-jobque_boost_push_Channel_q__auto_q_:

.. das:function:: push(channel: Channel?; data: auto?) : auto

pushes value to the channel (at the end)

:Arguments: * **channel** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **data** : auto?

++++++++++++++++++++++++
Internal capture details
++++++++++++++++++++++++

  *  :ref:`capture_jobque_channel (var ch: Channel?) : Channel? <function-jobque_boost_capture_jobque_channel_Channel_q_>` 
  *  :ref:`capture_jobque_job_status (var js: JobStatus?) : JobStatus? <function-jobque_boost_capture_jobque_job_status_JobStatus_q_>` 
  *  :ref:`release_capture_jobque_channel (ch: Channel?) <function-jobque_boost_release_capture_jobque_channel_Channel_q_>` 
  *  :ref:`release_capture_jobque_job_status (js: JobStatus?) <function-jobque_boost_release_capture_jobque_job_status_JobStatus_q_>` 

.. _function-jobque_boost_capture_jobque_channel_Channel_q_:

.. das:function:: capture_jobque_channel(ch: Channel?) : Channel?

this function is used to capture a channel that is used by the jobque.

:Arguments: * **ch** :  :ref:`Channel <handle-jobque-Channel>` ?

.. _function-jobque_boost_capture_jobque_job_status_JobStatus_q_:

.. das:function:: capture_jobque_job_status(js: JobStatus?) : JobStatus?

this function is used to capture a job status that is used by the jobque.

:Arguments: * **js** :  :ref:`JobStatus <handle-jobque-JobStatus>` ?

.. _function-jobque_boost_release_capture_jobque_channel_Channel_q_:

.. das:function:: release_capture_jobque_channel(ch: Channel?)

this function is used to release a channel that is used by the jobque.

:Arguments: * **ch** :  :ref:`Channel <handle-jobque-Channel>` ?

.. _function-jobque_boost_release_capture_jobque_job_status_JobStatus_q_:

.. das:function:: release_capture_jobque_job_status(js: JobStatus?)

this function is used to release a job status that is used by the jobque.

:Arguments: * **js** :  :ref:`JobStatus <handle-jobque-JobStatus>` ?

+++++++++++++
Uncategorized
+++++++++++++

.. _function-jobque_boost_capture_jobque_lock_box_LockBox_q_:

.. das:function:: capture_jobque_lock_box(js: LockBox?) : LockBox?

this function is used to capture a lock box that is used by the jobque.

:Arguments: * **js** :  :ref:`LockBox <handle-jobque-LockBox>` ?

.. _function-jobque_boost_release_capture_jobque_lock_box_LockBox_q_:

.. das:function:: release_capture_jobque_lock_box(js: LockBox?)

this function is used to release a lock box that is used by the jobque.

:Arguments: * **js** :  :ref:`LockBox <handle-jobque-LockBox>` ?

.. _function-jobque_boost_gather_Channel_q__block_ls_arg_c_autoTT_hh__c_void_gr_:

.. das:function:: gather(ch: Channel?; blk: block<(arg:auto(TT)#):void>) : auto

reads input from the channel (in order it was pushed) and invokes the block on each input.
afterwards input is consumed

:Arguments: * **ch** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **blk** : block<(arg:auto(TT)#):void>

.. _function-jobque_boost_gather_ex_Channel_q__block_ls_arg_c_autoTT_hh_;info_c_TypeInfo_const_q_;var_ctx_c_Context_c_void_gr_:

.. das:function:: gather_ex(ch: Channel?; blk: block<(arg:auto(TT)#;info:TypeInfo const?;var ctx:Context):void>) : auto

reads input from the channel (in order it was pushed) and invokes the block on each input.
afterwards input is consumed

:Arguments: * **ch** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **blk** : block<(arg:auto(TT)#;info: :ref:`TypeInfo <handle-rtti-TypeInfo>` ?;ctx: :ref:`Context <handle-rtti-Context>` ):void>

.. _function-jobque_boost_gather_and_forward_Channel_q__Channel_q__block_ls_arg_c_autoTT_hh__c_void_gr_:

.. das:function:: gather_and_forward(ch: Channel?; toCh: Channel?; blk: block<(arg:auto(TT)#):void>) : auto

reads input from the channel (in order it was pushed) and invokes the block on each input.
afterwards input is consumed

:Arguments: * **ch** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **toCh** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **blk** : block<(arg:auto(TT)#):void>

.. _function-jobque_boost_peek_Channel_q__block_ls_arg_c_autoTT_hh__c_void_gr_:

.. das:function:: peek(ch: Channel?; blk: block<(arg:auto(TT)#):void>) : auto

reads input from the channel (in order it was pushed) and invokes the block on each input.
afterwards input is not consumed

:Arguments: * **ch** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **blk** : block<(arg:auto(TT)#):void>

.. _function-jobque_boost_for_each_clone_Channel_q__block_ls_res_c_autoTT_hh__c_void_gr_:

.. das:function:: for_each_clone(channel: Channel?; blk: block<(res:auto(TT)#):void>) : auto

reads input from the channel (in order it was pushed) and invokes the block on each input.
stops once channel is depleted (internal entry counter is 0)
this can happen on multiple threads or jobs at the same time.

:Arguments: * **channel** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **blk** : block<(res:auto(TT)#):void>

.. _function-jobque_boost_pop_one_Channel_q__block_ls_res_c_autoTT_hh__c_void_gr_:

.. das:function:: pop_one(channel: Channel?; blk: block<(res:auto(TT)#):void>) : auto

.. warning:: 
  This function is deprecated.

reads one command from channel

:Arguments: * **channel** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **blk** : block<(res:auto(TT)#):void>

.. _function-jobque_boost_pop_and_clone_one_Channel_q__block_ls_res_c_autoTT_hh__c_void_gr_:

.. das:function:: pop_and_clone_one(channel: Channel?; blk: block<(res:auto(TT)#):void>) : auto

reads one command from channel

:Arguments: * **channel** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **blk** : block<(res:auto(TT)#):void>

.. _function-jobque_boost_push_batch_clone_Channel_q__array_ls_autoTT_gr_:

.. das:function:: push_batch_clone(channel: Channel?; data: array<auto(TT)>) : auto

clones data and pushed values to the channel (at the end)

:Arguments: * **channel** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **data** : array<auto(TT)>

.. _function-jobque_boost_push_batch_Channel_q__array_ls_auto_q__gr_:

.. das:function:: push_batch(channel: Channel?; data: array<auto?>) : auto

pushes values to the channel (at the end)

:Arguments: * **channel** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **data** : array<auto?>

.. _function-jobque_boost_set_LockBox_q__autoTT:

.. das:function:: set(box: LockBox?; data: auto(TT)) : auto

clones data and sets value to the lock box

:Arguments: * **box** :  :ref:`LockBox <handle-jobque-LockBox>` ?

            * **data** : auto(TT)

.. _function-jobque_boost_set_LockBox_q__auto_q_:

.. das:function:: set(box: LockBox?; data: auto?) : auto

sets value to the lock box

:Arguments: * **box** :  :ref:`LockBox <handle-jobque-LockBox>` ?

            * **data** : auto?

.. _function-jobque_boost_get_LockBox_q__block_ls_res_c_autoTT_hh__c_void_gr_:

.. das:function:: get(box: LockBox?; blk: block<(res:auto(TT)#):void>) : auto

reads value from the lock box and invokes the block on it

:Arguments: * **box** :  :ref:`LockBox <handle-jobque-LockBox>` ?

            * **blk** : block<(res:auto(TT)#):void>

.. _function-jobque_boost_update_LockBox_q__block_ls_var_res_c_autoTT_hh__c_void_gr_:

.. das:function:: update(box: LockBox?; blk: block<(var res:auto(TT)#):void>) : auto

update value in the lock box and invokes the block on it

:Arguments: * **box** :  :ref:`LockBox <handle-jobque-LockBox>` ?

            * **blk** : block<(res:auto(TT)#):void>

.. _function-jobque_boost_clear_LockBox_q__autoTT:

.. das:function:: clear(box: LockBox?; type_: auto(TT)) : auto

clear value from the lock box

:Arguments: * **box** :  :ref:`LockBox <handle-jobque-LockBox>` ?

            * **type_** : auto(TT)

.. _function-jobque_boost_each_clone_Channel_q__autoTT:

.. das:function:: each_clone(channel: Channel?; tinfo: auto(TT)) : auto

this iterator is used to iterate over the channel in order it was pushed.
iterator stops once channel is depleted (internal entry counter is 0)
iteration can happen on multiple threads or jobs at the same time.

:Arguments: * **channel** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **tinfo** : auto(TT)


