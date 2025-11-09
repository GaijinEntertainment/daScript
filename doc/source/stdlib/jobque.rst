
.. _stdlib_jobque:

================
Jobs and threads
================

Apply module implements job que and threading.

All functions and symbols are in "jobque" module, use require to get access to it. ::

    require jobque

++++++++++++++++++
Handled structures
++++++++++++++++++

.. _handle-jobque-JobStatus:

.. das:attribute:: JobStatus

.. _function-jobque__dot__rq_isReady_JobStatus_implicit:

.. das:function:: JobStatus implicit.isReady() : bool

Weather or not the job is completed.

.. _function-jobque__dot__rq_isValid_JobStatus_implicit:

.. das:function:: JobStatus implicit.isValid() : bool

If the job status object is valid.

.. _function-jobque__dot__rq_size_JobStatus_implicit:

.. das:function:: JobStatus implicit.size() : int

Number of remaining elements, which were previously appended.

:Properties: * **isReady** : bool

             * **isValid** : bool

             * **size** : int

 Job status indicator (ready or not, as well as entry count).


.. _handle-jobque-Channel:

.. das:attribute:: Channel

.. _function-jobque__dot__rq_isEmpty_Channel_implicit:

.. das:function:: Channel implicit.isEmpty() : bool

Weather there are no remaining elements in the pipe.

.. _function-jobque__dot__rq_total_Channel_implicit:

.. das:function:: Channel implicit.total() : int

Total number of elements in the pipe.

:Properties: * **isEmpty** : bool

             * **total** : int

 Channel provides a way to communicate between multiple contexts, including threads and jobs. Channel has internal entry count.


.. _handle-jobque-LockBox:

.. das:attribute:: LockBox

 Lockbox. Similar to channel, only for single object.


.. _handle-jobque-Atomic32:

.. das:attribute:: Atomic32

 Atomic 32 bit integer.


.. _handle-jobque-Atomic64:

.. das:attribute:: Atomic64

 Atomic 64 bit integer.


+++++++++++++++++++++++++++
Channel, JobStatus, Lockbox
+++++++++++++++++++++++++++

  *  :ref:`lock_box_create () : LockBox? <function-jobque_lock_box_create>` 
  *  :ref:`lock_box_remove (box: LockBox?& implicit) <function-jobque_lock_box_remove_LockBox_q__implicit>` 
  *  :ref:`append (channel: JobStatus? implicit; size: int) : int <function-jobque_append_JobStatus_q__implicit_int>` 
  *  :ref:`channel_create () : Channel? <function-jobque_channel_create>` 
  *  :ref:`channel_remove (channel: Channel?& implicit) <function-jobque_channel_remove_Channel_q__implicit>` 
  *  :ref:`add_ref (status: JobStatus? implicit) <function-jobque_add_ref_JobStatus_q__implicit>` 
  *  :ref:`release (status: JobStatus?& implicit) <function-jobque_release_JobStatus_q__implicit>` 
  *  :ref:`join (job: JobStatus? implicit) <function-jobque_join_JobStatus_q__implicit>` 
  *  :ref:`notify (job: JobStatus? implicit) <function-jobque_notify_JobStatus_q__implicit>` 
  *  :ref:`notify_and_release (job: JobStatus?& implicit) <function-jobque_notify_and_release_JobStatus_q__implicit>` 
  *  :ref:`job_status_create () : JobStatus? <function-jobque_job_status_create>` 
  *  :ref:`job_status_remove (jobStatus: JobStatus?& implicit) <function-jobque_job_status_remove_JobStatus_q__implicit>` 

.. _function-jobque_lock_box_create:

.. das:function:: lock_box_create() : LockBox?

 Creates lockbox.

.. _function-jobque_lock_box_remove_LockBox_q__implicit:

.. das:function:: lock_box_remove(box: LockBox?& implicit)

.. warning:: 
  This is unsafe operation.

 Destroys lockbox.

:Arguments: * **box** :  :ref:`LockBox <handle-jobque-LockBox>` ?& implicit

.. _function-jobque_append_JobStatus_q__implicit_int:

.. das:function:: append(channel: JobStatus? implicit; size: int) : int

 Increase entry count to the channel.

:Arguments: * **channel** :  :ref:`JobStatus <handle-jobque-JobStatus>` ? implicit

            * **size** : int

.. _function-jobque_channel_create:

.. das:function:: channel_create() : Channel?

.. warning:: 
  This is unsafe operation.

 Creates channel.

.. _function-jobque_channel_remove_Channel_q__implicit:

.. das:function:: channel_remove(channel: Channel?& implicit)

.. warning:: 
  This is unsafe operation.

 Destroys channel.

:Arguments: * **channel** :  :ref:`Channel <handle-jobque-Channel>` ?& implicit

.. _function-jobque_add_ref_JobStatus_q__implicit:

.. das:function:: add_ref(status: JobStatus? implicit)

 Increase reference count of the job status or channel.

:Arguments: * **status** :  :ref:`JobStatus <handle-jobque-JobStatus>` ? implicit

.. _function-jobque_release_JobStatus_q__implicit:

.. das:function:: release(status: JobStatus?& implicit)

 Decrease reference count of the job status or channel. Object is delete when reference count reaches 0.

:Arguments: * **status** :  :ref:`JobStatus <handle-jobque-JobStatus>` ?& implicit

.. _function-jobque_join_JobStatus_q__implicit:

.. das:function:: join(job: JobStatus? implicit)

 Wait until channel entry count reaches 0.

:Arguments: * **job** :  :ref:`JobStatus <handle-jobque-JobStatus>` ? implicit

.. _function-jobque_notify_JobStatus_q__implicit:

.. das:function:: notify(job: JobStatus? implicit)

 Notify channel that entry is completed (decrease entry count).

:Arguments: * **job** :  :ref:`JobStatus <handle-jobque-JobStatus>` ? implicit

.. _function-jobque_notify_and_release_JobStatus_q__implicit:

.. das:function:: notify_and_release(job: JobStatus?& implicit)

 Notify channel or job status that entry is completed (decrease entry count) and decrease reference count of the job status or channel.

:Arguments: * **job** :  :ref:`JobStatus <handle-jobque-JobStatus>` ?& implicit

.. _function-jobque_job_status_create:

.. das:function:: job_status_create() : JobStatus?

 Creates job status.

.. _function-jobque_job_status_remove_JobStatus_q__implicit:

.. das:function:: job_status_remove(jobStatus: JobStatus?& implicit)

.. warning:: 
  This is unsafe operation.

 Destroys job status.

:Arguments: * **jobStatus** :  :ref:`JobStatus <handle-jobque-JobStatus>` ?& implicit

+++++++
Queries
+++++++

  *  :ref:`get_total_hw_jobs () : int <function-jobque_get_total_hw_jobs>` 
  *  :ref:`get_total_hw_threads () : int <function-jobque_get_total_hw_threads>` 
  *  :ref:`is_job_que_shutting_down () : bool <function-jobque_is_job_que_shutting_down>` 

.. _function-jobque_get_total_hw_jobs:

.. das:function:: get_total_hw_jobs() : int

 Total number of hardware threads supporting job system.

.. _function-jobque_get_total_hw_threads:

.. das:function:: get_total_hw_threads() : int

 Total number of hardware threads available.

.. _function-jobque_is_job_que_shutting_down:

.. das:function:: is_job_que_shutting_down() : bool

 Returns true if job que infrastructure is shut-down or not initialized.

++++++++++++++++++++
Internal invocations
++++++++++++++++++++

  *  :ref:`new_job_invoke (lambda: lambda\<():void\>; function: function\<():void\>; lambdaSize: int) <function-jobque_new_job_invoke_lambda_ls__c_void_gr__function_ls__c_void_gr__int>` 
  *  :ref:`new_thread_invoke (lambda: lambda\<():void\>; function: function\<():void\>; lambdaSize: int) <function-jobque_new_thread_invoke_lambda_ls__c_void_gr__function_ls__c_void_gr__int>` 
  *  :ref:`new_debugger_thread (block: block\<():void\>) <function-jobque_new_debugger_thread_block_ls__c_void_gr_>` 

.. _function-jobque_new_job_invoke_lambda_ls__c_void_gr__function_ls__c_void_gr__int:

.. das:function:: new_job_invoke(lambda: lambda<():void>; function: function<():void>; lambdaSize: int)

 Creates clone of the current context, moves attached lambda to it.

:Arguments: * **lambda** : lambda<void>

            * **function** : function<void>

            * **lambdaSize** : int

.. _function-jobque_new_thread_invoke_lambda_ls__c_void_gr__function_ls__c_void_gr__int:

.. das:function:: new_thread_invoke(lambda: lambda<():void>; function: function<():void>; lambdaSize: int)

 Creates clone of the current context, moves attached lambda to it.

:Arguments: * **lambda** : lambda<void>

            * **function** : function<void>

            * **lambdaSize** : int

.. _function-jobque_new_debugger_thread_block_ls__c_void_gr_:

.. das:function:: new_debugger_thread(block: block<():void>)

 Creates a new thread for debugging purposes (tick thread).

:Arguments: * **block** : block<void> implicit

++++++++++++
Construction
++++++++++++

  *  :ref:`with_lock_box (block: block\<(LockBox?):void\>) <function-jobque_with_lock_box_block_ls_LockBox_q__c_void_gr_>` 
  *  :ref:`with_channel (block: block\<(Channel?):void\>) <function-jobque_with_channel_block_ls_Channel_q__c_void_gr_>` 
  *  :ref:`with_channel (count: int; block: block\<(Channel?):void\>) <function-jobque_with_channel_int_block_ls_Channel_q__c_void_gr_>` 
  *  :ref:`with_job_status (total: int; block: block\<(JobStatus?):void\>) <function-jobque_with_job_status_int_block_ls_JobStatus_q__c_void_gr_>` 
  *  :ref:`with_job_que (block: block\<():void\>) <function-jobque_with_job_que_block_ls__c_void_gr_>` 

.. _function-jobque_with_lock_box_block_ls_LockBox_q__c_void_gr_:

.. das:function:: with_lock_box(block: block<(LockBox?):void>)

 Creates `LockBox`, makes it available inside the scope of the block.

:Arguments: * **block** : block<( :ref:`LockBox <handle-jobque-LockBox>` ?):void> implicit

.. _function-jobque_with_channel_block_ls_Channel_q__c_void_gr_:

.. das:function:: with_channel(block: block<(Channel?):void>)

 Creates `Channel`, makes it available inside the scope of the block.

:Arguments: * **block** : block<( :ref:`Channel <handle-jobque-Channel>` ?):void> implicit

.. _function-jobque_with_channel_int_block_ls_Channel_q__c_void_gr_:

.. das:function:: with_channel(count: int; block: block<(Channel?):void>)

 Creates `Channel`, makes it available inside the scope of the block.

:Arguments: * **count** : int

            * **block** : block<( :ref:`Channel <handle-jobque-Channel>` ?):void> implicit

.. _function-jobque_with_job_status_int_block_ls_JobStatus_q__c_void_gr_:

.. das:function:: with_job_status(total: int; block: block<(JobStatus?):void>)

 Creates `JobStatus`, makes it available inside the scope of the block.

:Arguments: * **total** : int

            * **block** : block<( :ref:`JobStatus <handle-jobque-JobStatus>` ?):void> implicit

.. _function-jobque_with_job_que_block_ls__c_void_gr_:

.. das:function:: with_job_que(block: block<():void>)

 Makes sure jobque infrastructure is available inside the scope of the block.

:Arguments: * **block** : block<void> implicit

++++++
Atomic
++++++

  *  :ref:`atomic32_create () : Atomic32? <function-jobque_atomic32_create>` 
  *  :ref:`atomic32_remove (atomic: Atomic32?& implicit) <function-jobque_atomic32_remove_Atomic32_q__implicit>` 
  *  :ref:`with_atomic32 (block: block\<(Atomic32?):void\>) <function-jobque_with_atomic32_block_ls_Atomic32_q__c_void_gr_>` 
  *  :ref:`set (atomic: Atomic32? implicit; value: int) <function-jobque_set_Atomic32_q__implicit_int>` 
  *  :ref:`get (atomic: Atomic32? implicit) : int <function-jobque_get_Atomic32_q__implicit>` 
  *  :ref:`inc (atomic: Atomic32? implicit) : int <function-jobque_inc_Atomic32_q__implicit>` 
  *  :ref:`dec (atomic: Atomic32? implicit) : int <function-jobque_dec_Atomic32_q__implicit>` 
  *  :ref:`atomic64_create () : Atomic64? <function-jobque_atomic64_create>` 
  *  :ref:`atomic64_remove (atomic: Atomic64?& implicit) <function-jobque_atomic64_remove_Atomic64_q__implicit>` 
  *  :ref:`with_atomic64 (block: block\<(Atomic64?):void\>) <function-jobque_with_atomic64_block_ls_Atomic64_q__c_void_gr_>` 
  *  :ref:`set (atomic: Atomic64? implicit; value: int64) <function-jobque_set_Atomic64_q__implicit_int64>` 
  *  :ref:`get (atomic: Atomic64? implicit) : int64 <function-jobque_get_Atomic64_q__implicit>` 
  *  :ref:`inc (atomic: Atomic64? implicit) : int64 <function-jobque_inc_Atomic64_q__implicit>` 
  *  :ref:`dec (atomic: Atomic64? implicit) : int64 <function-jobque_dec_Atomic64_q__implicit>` 

.. _function-jobque_atomic32_create:

.. das:function:: atomic32_create() : Atomic32?

 Creates atomic 32 bit integer.

.. _function-jobque_atomic32_remove_Atomic32_q__implicit:

.. das:function:: atomic32_remove(atomic: Atomic32?& implicit)

.. warning:: 
  This is unsafe operation.

 Destroys atomic 32 bit integer.

:Arguments: * **atomic** :  :ref:`Atomic32 <handle-jobque-Atomic32>` ?& implicit

.. _function-jobque_with_atomic32_block_ls_Atomic32_q__c_void_gr_:

.. das:function:: with_atomic32(block: block<(Atomic32?):void>)

 Creates `Atomic32`, makes it available inside the scope of the block.

:Arguments: * **block** : block<( :ref:`Atomic32 <handle-jobque-Atomic32>` ?):void> implicit

.. _function-jobque_set_Atomic32_q__implicit_int:

.. das:function:: set(atomic: Atomic32? implicit; value: int)

 Set atomic integer value.

:Arguments: * **atomic** :  :ref:`Atomic32 <handle-jobque-Atomic32>` ? implicit

            * **value** : int

.. _function-jobque_get_Atomic32_q__implicit:

.. das:function:: get(atomic: Atomic32? implicit) : int

 Get atomic integer value.

:Arguments: * **atomic** :  :ref:`Atomic32 <handle-jobque-Atomic32>` ? implicit

.. _function-jobque_inc_Atomic32_q__implicit:

.. das:function:: inc(atomic: Atomic32? implicit) : int

 Increase atomic integer value and returns result.

:Arguments: * **atomic** :  :ref:`Atomic32 <handle-jobque-Atomic32>` ? implicit

.. _function-jobque_dec_Atomic32_q__implicit:

.. das:function:: dec(atomic: Atomic32? implicit) : int

 Decrease atomic integer value and returns result.

:Arguments: * **atomic** :  :ref:`Atomic32 <handle-jobque-Atomic32>` ? implicit

.. _function-jobque_atomic64_create:

.. das:function:: atomic64_create() : Atomic64?

 Creates atomic 64 bit integer.

.. _function-jobque_atomic64_remove_Atomic64_q__implicit:

.. das:function:: atomic64_remove(atomic: Atomic64?& implicit)

.. warning:: 
  This is unsafe operation.

 Destroys atomic 64 bit integer.

:Arguments: * **atomic** :  :ref:`Atomic64 <handle-jobque-Atomic64>` ?& implicit

.. _function-jobque_with_atomic64_block_ls_Atomic64_q__c_void_gr_:

.. das:function:: with_atomic64(block: block<(Atomic64?):void>)

 Creates `Atomic64`, makes it available inside the scope of the block.

:Arguments: * **block** : block<( :ref:`Atomic64 <handle-jobque-Atomic64>` ?):void> implicit

.. _function-jobque_set_Atomic64_q__implicit_int64:

.. das:function:: set(atomic: Atomic64? implicit; value: int64)

 Set atomic integer value.

:Arguments: * **atomic** :  :ref:`Atomic64 <handle-jobque-Atomic64>` ? implicit

            * **value** : int64

.. _function-jobque_get_Atomic64_q__implicit:

.. das:function:: get(atomic: Atomic64? implicit) : int64

 Get atomic integer value.

:Arguments: * **atomic** :  :ref:`Atomic64 <handle-jobque-Atomic64>` ? implicit

.. _function-jobque_inc_Atomic64_q__implicit:

.. das:function:: inc(atomic: Atomic64? implicit) : int64

 Increase atomic integer value and returns result.

:Arguments: * **atomic** :  :ref:`Atomic64 <handle-jobque-Atomic64>` ? implicit

.. _function-jobque_dec_Atomic64_q__implicit:

.. das:function:: dec(atomic: Atomic64? implicit) : int64

 Decrease atomic integer value and returns result.

:Arguments: * **atomic** :  :ref:`Atomic64 <handle-jobque-Atomic64>` ? implicit


