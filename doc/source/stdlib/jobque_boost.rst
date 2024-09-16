
.. _stdlib_jobque_boost:

==================================
Boost package for jobs and threads
==================================

The JOBQUE boost module implements collection of helper macros and functions to accompany :ref:`JOBQUE <stdlib_jobque>`.

All functions and symbols are in "jobque_boost" module, use require to get access to it. ::

    require daslib/jobque_boost


++++++++++++++++++++
Function annotations
++++++++++++++++++++

.. _handle-jobque_boost-NewJobMacro:

.. das:attribute:: NewJobMacro

|detail/function_annotation-jobque_boost-NewJobMacro|

+++++++++++
Invocations
+++++++++++

  *  :ref:`new_job (var l: lambda\<\>) <function-_at_jobque_boost_c__c_new_job__at_>` 
  *  :ref:`new_thread (var l: lambda\<\>) <function-_at_jobque_boost_c__c_new_thread__at_>` 

.. _function-_at_jobque_boost_c__c_new_job__at_:

.. das:function:: new_job(l: lambda<>)

Create a new job.
    * new context is cloned from the current context.
    * lambda is cloned to the new context.
    * new job is added to the job queue.
    * once new job is invoked, lambda is invoked on the new context on the job thread.


:Arguments: * **l** : lambda<void>

.. _function-_at_jobque_boost_c__c_new_thread__at_:

.. das:function:: new_thread(l: lambda<>)

Create a new thread
    * new context is cloned from the current context.
    * lambda is cloned to the new context.
    * new thread is created.
    * lambda is invoked on the new context on the new thread.


:Arguments: * **l** : lambda<void>

+++++++++
Iteration
+++++++++

  *  :ref:`for_each (channel: Channel?; blk: block\<(res:auto(TT) const#):void\>) : auto <function-_at_jobque_boost_c__c_for_each_C1_ls_H_ls_jobque_c__c_Channel_gr__gr__qm__CN_ls_res_gr_0_ls_C_hh_Y_ls_TT_gr__dot__gr_1_ls_v_gr__builtin_>` 
  *  :ref:`each (var channel: Channel?; tinfo: auto(TT)) : auto <function-_at_jobque_boost_c__c_each_1_ls_H_ls_jobque_c__c_Channel_gr__gr__qm__CY_ls_TT_gr__dot_>` 

.. _function-_at_jobque_boost_c__c_for_each_C1_ls_H_ls_jobque_c__c_Channel_gr__gr__qm__CN_ls_res_gr_0_ls_C_hh_Y_ls_TT_gr__dot__gr_1_ls_v_gr__builtin_:

.. das:function:: for_each(channel: Channel?; blk: block<(res:auto(TT) const#):void>) : auto

.. warning:: 
  This function is deprecated.

reads input from the channel (in order it was pushed) and invokes the block on each input.
stops once channel is depleted (internal entry counter is 0)
this can happen on multiple threads or jobs at the same time.


:Arguments: * **channel** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **blk** : block<(res:auto(TT)#):void>

.. _function-_at_jobque_boost_c__c_each_1_ls_H_ls_jobque_c__c_Channel_gr__gr__qm__CY_ls_TT_gr__dot_:

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

  *  :ref:`push_clone (channel: Channel?; data: auto(TT)) : auto <function-_at_jobque_boost_c__c_push_clone_C1_ls_H_ls_jobque_c__c_Channel_gr__gr__qm__CY_ls_TT_gr__dot_>` 
  *  :ref:`push (channel: Channel?; data: auto?) : auto <function-_at_jobque_boost_c__c_push_C1_ls_H_ls_jobque_c__c_Channel_gr__gr__qm__C1_ls__dot__gr__qm_>` 

.. _function-_at_jobque_boost_c__c_push_clone_C1_ls_H_ls_jobque_c__c_Channel_gr__gr__qm__CY_ls_TT_gr__dot_:

.. das:function:: push_clone(channel: Channel?; data: auto(TT)) : auto

clones data and pushed value to the channel (at the end)


:Arguments: * **channel** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **data** : auto(TT)

.. _function-_at_jobque_boost_c__c_push_C1_ls_H_ls_jobque_c__c_Channel_gr__gr__qm__C1_ls__dot__gr__qm_:

.. das:function:: push(channel: Channel?; data: auto?) : auto

pushes value to the channel (at the end)


:Arguments: * **channel** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **data** : auto?

++++++++++++++++++++++++
Internal capture details
++++++++++++++++++++++++

  *  :ref:`capture_jobque_channel (ch: Channel?) : Channel? <function-_at_jobque_boost_c__c_capture_jobque_channel_C1_ls_H_ls_jobque_c__c_Channel_gr__gr__qm_>` 
  *  :ref:`capture_jobque_job_status (js: JobStatus?) : JobStatus? <function-_at_jobque_boost_c__c_capture_jobque_job_status_C1_ls_H_ls_jobque_c__c_JobStatus_gr__gr__qm_>` 
  *  :ref:`release_capture_jobque_channel (ch: Channel?) <function-_at_jobque_boost_c__c_release_capture_jobque_channel_C1_ls_H_ls_jobque_c__c_Channel_gr__gr__qm_>` 
  *  :ref:`release_capture_jobque_job_status (js: JobStatus?) <function-_at_jobque_boost_c__c_release_capture_jobque_job_status_C1_ls_H_ls_jobque_c__c_JobStatus_gr__gr__qm_>` 

.. _function-_at_jobque_boost_c__c_capture_jobque_channel_C1_ls_H_ls_jobque_c__c_Channel_gr__gr__qm_:

.. das:function:: capture_jobque_channel(ch: Channel?) : Channel?

this function is used to capture a channel that is used by the jobque.


:Arguments: * **ch** :  :ref:`Channel <handle-jobque-Channel>` ?

.. _function-_at_jobque_boost_c__c_capture_jobque_job_status_C1_ls_H_ls_jobque_c__c_JobStatus_gr__gr__qm_:

.. das:function:: capture_jobque_job_status(js: JobStatus?) : JobStatus?

this function is used to capture a job status that is used by the jobque.


:Arguments: * **js** :  :ref:`JobStatus <handle-jobque-JobStatus>` ?

.. _function-_at_jobque_boost_c__c_release_capture_jobque_channel_C1_ls_H_ls_jobque_c__c_Channel_gr__gr__qm_:

.. das:function:: release_capture_jobque_channel(ch: Channel?)

this function is used to release a channel that is used by the jobque.


:Arguments: * **ch** :  :ref:`Channel <handle-jobque-Channel>` ?

.. _function-_at_jobque_boost_c__c_release_capture_jobque_job_status_C1_ls_H_ls_jobque_c__c_JobStatus_gr__gr__qm_:

.. das:function:: release_capture_jobque_job_status(js: JobStatus?)

this function is used to release a job status that is used by the jobque.


:Arguments: * **js** :  :ref:`JobStatus <handle-jobque-JobStatus>` ?

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_jobque_boost_c__c_capture_jobque_lock_box_C1_ls_H_ls_jobque_c__c_LockBox_gr__gr__qm_:

.. das:function:: capture_jobque_lock_box(js: LockBox?) : LockBox?

this function is used to capture a lock box that is used by the jobque.


:Arguments: * **js** :  :ref:`LockBox <handle-jobque-LockBox>` ?

.. _function-_at_jobque_boost_c__c_release_capture_jobque_lock_box_C1_ls_H_ls_jobque_c__c_LockBox_gr__gr__qm_:

.. das:function:: release_capture_jobque_lock_box(js: LockBox?)

this function is used to release a lock box that is used by the jobque.


:Arguments: * **js** :  :ref:`LockBox <handle-jobque-LockBox>` ?

.. _function-_at_jobque_boost_c__c_gather_C1_ls_H_ls_jobque_c__c_Channel_gr__gr__qm__CN_ls_arg_gr_0_ls_C_hh_Y_ls_TT_gr__dot__gr_1_ls_v_gr__builtin_:

.. das:function:: gather(ch: Channel?; blk: block<(arg:auto(TT) const#):void>) : auto

reads input from the channel (in order it was pushed) and invokes the block on each input.
afterwards input is consumed


:Arguments: * **ch** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **blk** : block<(arg:auto(TT)#):void>

.. _function-_at_jobque_boost_c__c_gather_ex_C1_ls_H_ls_jobque_c__c_Channel_gr__gr__qm__CN_ls_arg;info;ctx_gr_0_ls_C_hh_Y_ls_TT_gr__dot_;C1_ls_CH_ls_rtti_c__c_TypeInfo_gr__gr__qm_;H_ls_rtti_c__c_Context_gr__gr_1_ls_v_gr__builtin_:

.. das:function:: gather_ex(ch: Channel?; blk: block<(arg:auto(TT) const#;info:TypeInfo const? const;var ctx:Context):void>) : auto

reads input from the channel (in order it was pushed) and invokes the block on each input.
afterwards input is consumed


:Arguments: * **ch** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **blk** : block<(arg:auto(TT)#;info: :ref:`TypeInfo <handle-rtti-TypeInfo>` ?;ctx: :ref:`Context <handle-rtti-Context>` ):void>

.. _function-_at_jobque_boost_c__c_gather_and_forward_C1_ls_H_ls_jobque_c__c_Channel_gr__gr__qm__C1_ls_H_ls_jobque_c__c_Channel_gr__gr__qm__CN_ls_arg_gr_0_ls_C_hh_Y_ls_TT_gr__dot__gr_1_ls_v_gr__builtin_:

.. das:function:: gather_and_forward(ch: Channel?; toCh: Channel?; blk: block<(arg:auto(TT) const#):void>) : auto

reads input from the channel (in order it was pushed) and invokes the block on each input.
afterwards input is consumed


:Arguments: * **ch** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **toCh** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **blk** : block<(arg:auto(TT)#):void>

.. _function-_at_jobque_boost_c__c_peek_C1_ls_H_ls_jobque_c__c_Channel_gr__gr__qm__CN_ls_arg_gr_0_ls_C_hh_Y_ls_TT_gr__dot__gr_1_ls_v_gr__builtin_:

.. das:function:: peek(ch: Channel?; blk: block<(arg:auto(TT) const#):void>) : auto

reads input from the channel (in order it was pushed) and invokes the block on each input.
afterwards input is not consumed


:Arguments: * **ch** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **blk** : block<(arg:auto(TT)#):void>

.. _function-_at_jobque_boost_c__c_for_each_clone_C1_ls_H_ls_jobque_c__c_Channel_gr__gr__qm__CN_ls_res_gr_0_ls_C_hh_Y_ls_TT_gr__dot__gr_1_ls_v_gr__builtin_:

.. das:function:: for_each_clone(channel: Channel?; blk: block<(res:auto(TT) const#):void>) : auto

reads input from the channel (in order it was pushed) and invokes the block on each input.
stops once channel is depleted (internal entry counter is 0)
this can happen on multiple threads or jobs at the same time.


:Arguments: * **channel** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **blk** : block<(res:auto(TT)#):void>

.. _function-_at_jobque_boost_c__c_pop_one_C1_ls_H_ls_jobque_c__c_Channel_gr__gr__qm__CN_ls_res_gr_0_ls_C_hh_Y_ls_TT_gr__dot__gr_1_ls_v_gr__builtin_:

.. das:function:: pop_one(channel: Channel?; blk: block<(res:auto(TT) const#):void>) : auto

.. warning:: 
  This function is deprecated.

reads one command from channel


:Arguments: * **channel** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **blk** : block<(res:auto(TT)#):void>

.. _function-_at_jobque_boost_c__c_pop_and_clone_one_C1_ls_H_ls_jobque_c__c_Channel_gr__gr__qm__CN_ls_res_gr_0_ls_C_hh_Y_ls_TT_gr__dot__gr_1_ls_v_gr__builtin_:

.. das:function:: pop_and_clone_one(channel: Channel?; blk: block<(res:auto(TT) const#):void>) : auto

reads one command from channel




:Arguments: * **channel** :  :ref:`Channel <handle-jobque-Channel>` ? -  channel to read from

            * **blk** : block<(res:auto(TT)#):void> -  block to invoke on the read value


:Returns: true if value was read, false if channel is empty


.. _function-_at_jobque_boost_c__c_push_batch_clone_C1_ls_H_ls_jobque_c__c_Channel_gr__gr__qm__C1_ls_Y_ls_TT_gr__dot__gr_A:

.. das:function:: push_batch_clone(channel: Channel?; data: array<auto(TT)>) : auto

clones data and pushed values to the channel (at the end)


:Arguments: * **channel** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **data** : array<auto(TT)>

.. _function-_at_jobque_boost_c__c_push_batch_C1_ls_H_ls_jobque_c__c_Channel_gr__gr__qm__C1_ls_1_ls__dot__gr__qm__gr_A:

.. das:function:: push_batch(channel: Channel?; data: array<auto?>) : auto

pushes values to the channel (at the end)


:Arguments: * **channel** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **data** : array<auto?>

.. _function-_at_jobque_boost_c__c_set_C1_ls_H_ls_jobque_c__c_LockBox_gr__gr__qm__CY_ls_TT_gr__dot_:

.. das:function:: set(box: LockBox?; data: auto(TT)) : auto

clones data and sets value to the lock box


:Arguments: * **box** :  :ref:`LockBox <handle-jobque-LockBox>` ?

            * **data** : auto(TT)

.. _function-_at_jobque_boost_c__c_set_C1_ls_H_ls_jobque_c__c_LockBox_gr__gr__qm__C1_ls__dot__gr__qm_:

.. das:function:: set(box: LockBox?; data: auto?) : auto

sets value to the lock box


:Arguments: * **box** :  :ref:`LockBox <handle-jobque-LockBox>` ?

            * **data** : auto?

.. _function-_at_jobque_boost_c__c_get_C1_ls_H_ls_jobque_c__c_LockBox_gr__gr__qm__CN_ls_res_gr_0_ls_C_hh_Y_ls_TT_gr__dot__gr_1_ls_v_gr__builtin_:

.. das:function:: get(box: LockBox?; blk: block<(res:auto(TT) const#):void>) : auto

reads value from the lock box and invokes the block on it


:Arguments: * **box** :  :ref:`LockBox <handle-jobque-LockBox>` ?

            * **blk** : block<(res:auto(TT)#):void>

.. _function-_at_jobque_boost_c__c_update_C1_ls_H_ls_jobque_c__c_LockBox_gr__gr__qm__CN_ls_res_gr_0_ls__hh_Y_ls_TT_gr__dot__gr_1_ls_v_gr__builtin_:

.. das:function:: update(box: LockBox?; blk: block<(var res:auto(TT)#):void>) : auto

update value in the lock box and invokes the block on it


:Arguments: * **box** :  :ref:`LockBox <handle-jobque-LockBox>` ?

            * **blk** : block<(res:auto(TT)#):void>

.. _function-_at_jobque_boost_c__c_clear_C1_ls_H_ls_jobque_c__c_LockBox_gr__gr__qm__CY_ls_TT_gr__dot_:

.. das:function:: clear(box: LockBox?; type_: auto(TT)) : auto

clear value from the lock box


:Arguments: * **box** :  :ref:`LockBox <handle-jobque-LockBox>` ?

            * **type_** : auto(TT)

.. _function-_at_jobque_boost_c__c_each_clone_1_ls_H_ls_jobque_c__c_Channel_gr__gr__qm__CY_ls_TT_gr__dot_:

.. das:function:: each_clone(channel: Channel?; tinfo: auto(TT)) : auto

this iterator is used to iterate over the channel in order it was pushed.
iterator stops once channel is depleted (internal entry counter is 0)
iteration can happen on multiple threads or jobs at the same time.


:Arguments: * **channel** :  :ref:`Channel <handle-jobque-Channel>` ?

            * **tinfo** : auto(TT)


