
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

.. _function-_at_jobque_c__c__dot__rq_isReady_IH_ls_jobque_c__c_JobStatus_gr_:

.. das:function:: JobStatus implicit.isReady() : bool

// stub


.. _function-_at_jobque_c__c__dot__rq_isValid_CIH_ls_jobque_c__c_JobStatus_gr_:

.. das:function:: JobStatus implicit.isValid() : bool

// stub


.. _function-_at_jobque_c__c__dot__rq_size_CIH_ls_jobque_c__c_JobStatus_gr_:

.. das:function:: JobStatus implicit.size() : int

// stub


:Properties: * **isReady** : bool

             * **isValid** : bool

             * **size** : int

// stub


.. _handle-jobque-Channel:

.. das:attribute:: Channel

.. _function-_at_jobque_c__c__dot__rq_isEmpty_CIH_ls_jobque_c__c_Channel_gr_:

.. das:function:: Channel implicit.isEmpty() : bool

// stub


.. _function-_at_jobque_c__c__dot__rq_total_CIH_ls_jobque_c__c_Channel_gr_:

.. das:function:: Channel implicit.total() : int

// stub


:Properties: * **isEmpty** : bool

             * **total** : int

// stub


.. _handle-jobque-LockBox:

.. das:attribute:: LockBox

// stub


.. _handle-jobque-Atomic32:

.. das:attribute:: Atomic32

// stub


.. _handle-jobque-Atomic64:

.. das:attribute:: Atomic64

// stub


+++++++++++++++++++++++++++
Channel, JobStatus, Lockbox
+++++++++++++++++++++++++++

  *  :ref:`lock_box_create () : LockBox? <function-_at_jobque_c__c_lock_box_create_C_c_C_l>` 
  *  :ref:`lock_box_remove (box: LockBox?& implicit) <function-_at_jobque_c__c_lock_box_remove_&I1_ls_H_ls_jobque_c__c_LockBox_gr__gr__qm__C_c_C_l>` 
  *  :ref:`append (channel: JobStatus? implicit; size: int) : int <function-_at_jobque_c__c_append_CI1_ls_H_ls_jobque_c__c_JobStatus_gr__gr__qm__Ci_C_c_C_l>` 
  *  :ref:`channel_create () : Channel? <function-_at_jobque_c__c_channel_create_C_c_C_l>` 
  *  :ref:`channel_remove (channel: Channel?& implicit) <function-_at_jobque_c__c_channel_remove_&I1_ls_H_ls_jobque_c__c_Channel_gr__gr__qm__C_c_C_l>` 
  *  :ref:`add_ref (status: JobStatus? implicit) <function-_at_jobque_c__c_add_ref_CI1_ls_H_ls_jobque_c__c_JobStatus_gr__gr__qm__C_c_C_l>` 
  *  :ref:`release (status: JobStatus?& implicit) <function-_at_jobque_c__c_release_&I1_ls_H_ls_jobque_c__c_JobStatus_gr__gr__qm__C_c_C_l>` 
  *  :ref:`join (job: JobStatus? implicit) <function-_at_jobque_c__c_join_CI1_ls_H_ls_jobque_c__c_JobStatus_gr__gr__qm__C_c_C_l>` 
  *  :ref:`notify (job: JobStatus? implicit) <function-_at_jobque_c__c_notify_CI1_ls_H_ls_jobque_c__c_JobStatus_gr__gr__qm__C_c_C_l>` 
  *  :ref:`notify_and_release (job: JobStatus?& implicit) <function-_at_jobque_c__c_notify_and_release_&I1_ls_H_ls_jobque_c__c_JobStatus_gr__gr__qm__C_c_C_l>` 
  *  :ref:`job_status_create () : JobStatus? <function-_at_jobque_c__c_job_status_create_C_c_C_l>` 
  *  :ref:`job_status_remove (jobStatus: JobStatus?& implicit) <function-_at_jobque_c__c_job_status_remove_&I1_ls_H_ls_jobque_c__c_JobStatus_gr__gr__qm__C_c_C_l>` 

.. _function-_at_jobque_c__c_lock_box_create_C_c_C_l:

.. das:function:: lock_box_create() : LockBox?

// stub


.. _function-_at_jobque_c__c_lock_box_remove_&I1_ls_H_ls_jobque_c__c_LockBox_gr__gr__qm__C_c_C_l:

.. das:function:: lock_box_remove(box: LockBox?& implicit)

.. warning:: 
  This is unsafe operation.

// stub



:Arguments: * **box** :  :ref:`LockBox <handle-jobque-LockBox>` ?& implicit

.. _function-_at_jobque_c__c_append_CI1_ls_H_ls_jobque_c__c_JobStatus_gr__gr__qm__Ci_C_c_C_l:

.. das:function:: append(channel: JobStatus? implicit; size: int) : int

// stub



:Arguments: * **channel** :  :ref:`JobStatus <handle-jobque-JobStatus>` ? implicit

            * **size** : int

.. _function-_at_jobque_c__c_channel_create_C_c_C_l:

.. das:function:: channel_create() : Channel?

.. warning:: 
  This is unsafe operation.

// stub


.. _function-_at_jobque_c__c_channel_remove_&I1_ls_H_ls_jobque_c__c_Channel_gr__gr__qm__C_c_C_l:

.. das:function:: channel_remove(channel: Channel?& implicit)

.. warning:: 
  This is unsafe operation.

// stub



:Arguments: * **channel** :  :ref:`Channel <handle-jobque-Channel>` ?& implicit

.. _function-_at_jobque_c__c_add_ref_CI1_ls_H_ls_jobque_c__c_JobStatus_gr__gr__qm__C_c_C_l:

.. das:function:: add_ref(status: JobStatus? implicit)

// stub



:Arguments: * **status** :  :ref:`JobStatus <handle-jobque-JobStatus>` ? implicit

.. _function-_at_jobque_c__c_release_&I1_ls_H_ls_jobque_c__c_JobStatus_gr__gr__qm__C_c_C_l:

.. das:function:: release(status: JobStatus?& implicit)

// stub



:Arguments: * **status** :  :ref:`JobStatus <handle-jobque-JobStatus>` ?& implicit

.. _function-_at_jobque_c__c_join_CI1_ls_H_ls_jobque_c__c_JobStatus_gr__gr__qm__C_c_C_l:

.. das:function:: join(job: JobStatus? implicit)

// stub



:Arguments: * **job** :  :ref:`JobStatus <handle-jobque-JobStatus>` ? implicit

.. _function-_at_jobque_c__c_notify_CI1_ls_H_ls_jobque_c__c_JobStatus_gr__gr__qm__C_c_C_l:

.. das:function:: notify(job: JobStatus? implicit)

// stub



:Arguments: * **job** :  :ref:`JobStatus <handle-jobque-JobStatus>` ? implicit

.. _function-_at_jobque_c__c_notify_and_release_&I1_ls_H_ls_jobque_c__c_JobStatus_gr__gr__qm__C_c_C_l:

.. das:function:: notify_and_release(job: JobStatus?& implicit)

// stub



:Arguments: * **job** :  :ref:`JobStatus <handle-jobque-JobStatus>` ?& implicit

.. _function-_at_jobque_c__c_job_status_create_C_c_C_l:

.. das:function:: job_status_create() : JobStatus?

// stub


.. _function-_at_jobque_c__c_job_status_remove_&I1_ls_H_ls_jobque_c__c_JobStatus_gr__gr__qm__C_c_C_l:

.. das:function:: job_status_remove(jobStatus: JobStatus?& implicit)

.. warning:: 
  This is unsafe operation.

// stub



:Arguments: * **jobStatus** :  :ref:`JobStatus <handle-jobque-JobStatus>` ?& implicit

+++++++
Queries
+++++++

  *  :ref:`get_total_hw_jobs () : int <function-_at_jobque_c__c_get_total_hw_jobs_C_c_C_l>` 
  *  :ref:`get_total_hw_threads () : int <function-_at_jobque_c__c_get_total_hw_threads>` 
  *  :ref:`is_job_que_shutting_down () : bool <function-_at_jobque_c__c_is_job_que_shutting_down>` 

.. _function-_at_jobque_c__c_get_total_hw_jobs_C_c_C_l:

.. das:function:: get_total_hw_jobs() : int

// stub


.. _function-_at_jobque_c__c_get_total_hw_threads:

.. das:function:: get_total_hw_threads() : int

// stub


.. _function-_at_jobque_c__c_is_job_que_shutting_down:

.. das:function:: is_job_que_shutting_down() : bool

// stub


++++++++++++++++++++
Internal invocations
++++++++++++++++++++

  *  :ref:`new_job_invoke (lambda: lambda\<\>; function: function\<\>; lambdaSize: int) <function-_at_jobque_c__c_new_job_invoke_C_at__C_at__at__Ci_C_c_C_l>` 
  *  :ref:`new_thread_invoke (lambda: lambda\<\>; function: function\<\>; lambdaSize: int) <function-_at_jobque_c__c_new_thread_invoke_C_at__C_at__at__Ci_C_c_C_l>` 
  *  :ref:`new_debugger_thread (block: block\<\> implicit) <function-_at_jobque_c__c_new_debugger_thread_CI_builtin__C_c_C_l>` 

.. _function-_at_jobque_c__c_new_job_invoke_C_at__C_at__at__Ci_C_c_C_l:

.. das:function:: new_job_invoke(lambda: lambda<>; function: function<>; lambdaSize: int)

// stub



:Arguments: * **lambda** : lambda<void>

            * **function** : function<void>

            * **lambdaSize** : int

.. _function-_at_jobque_c__c_new_thread_invoke_C_at__C_at__at__Ci_C_c_C_l:

.. das:function:: new_thread_invoke(lambda: lambda<>; function: function<>; lambdaSize: int)

// stub



:Arguments: * **lambda** : lambda<void>

            * **function** : function<void>

            * **lambdaSize** : int

.. _function-_at_jobque_c__c_new_debugger_thread_CI_builtin__C_c_C_l:

.. das:function:: new_debugger_thread(block: block<> implicit)

// stub



:Arguments: * **block** : block<void> implicit

++++++++++++
Construction
++++++++++++

  *  :ref:`with_lock_box (block: block\<(var arg0:LockBox?):void\> implicit) <function-_at_jobque_c__c_with_lock_box_CI0_ls_1_ls_H_ls_jobque_c__c_LockBox_gr__gr__qm__gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`with_channel (block: block\<(var arg0:Channel?):void\> implicit) <function-_at_jobque_c__c_with_channel_CI0_ls_1_ls_H_ls_jobque_c__c_Channel_gr__gr__qm__gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`with_channel (count: int; block: block\<(var arg0:Channel?):void\> implicit) <function-_at_jobque_c__c_with_channel_Ci_CI0_ls_1_ls_H_ls_jobque_c__c_Channel_gr__gr__qm__gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`with_job_status (total: int; block: block\<(var arg0:JobStatus?):void\> implicit) <function-_at_jobque_c__c_with_job_status_Ci_CI0_ls_1_ls_H_ls_jobque_c__c_JobStatus_gr__gr__qm__gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`with_job_que (block: block\<void\> implicit) <function-_at_jobque_c__c_with_job_que_CI1_ls_v_gr__builtin__C_c_C_l>` 

.. _function-_at_jobque_c__c_with_lock_box_CI0_ls_1_ls_H_ls_jobque_c__c_LockBox_gr__gr__qm__gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: with_lock_box(block: block<(var arg0:LockBox?):void> implicit)

// stub



:Arguments: * **block** : block<( :ref:`LockBox <handle-jobque-LockBox>` ?):void> implicit

.. _function-_at_jobque_c__c_with_channel_CI0_ls_1_ls_H_ls_jobque_c__c_Channel_gr__gr__qm__gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: with_channel(block: block<(var arg0:Channel?):void> implicit)

// stub



:Arguments: * **block** : block<( :ref:`Channel <handle-jobque-Channel>` ?):void> implicit

.. _function-_at_jobque_c__c_with_channel_Ci_CI0_ls_1_ls_H_ls_jobque_c__c_Channel_gr__gr__qm__gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: with_channel(count: int; block: block<(var arg0:Channel?):void> implicit)

// stub



:Arguments: * **count** : int

            * **block** : block<( :ref:`Channel <handle-jobque-Channel>` ?):void> implicit

.. _function-_at_jobque_c__c_with_job_status_Ci_CI0_ls_1_ls_H_ls_jobque_c__c_JobStatus_gr__gr__qm__gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: with_job_status(total: int; block: block<(var arg0:JobStatus?):void> implicit)

// stub



:Arguments: * **total** : int

            * **block** : block<( :ref:`JobStatus <handle-jobque-JobStatus>` ?):void> implicit

.. _function-_at_jobque_c__c_with_job_que_CI1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: with_job_que(block: block<void> implicit)

// stub



:Arguments: * **block** : block<void> implicit

++++++
Atomic
++++++

  *  :ref:`atomic32_create () : Atomic32? <function-_at_jobque_c__c_atomic32_create_C_c_C_l>` 
  *  :ref:`atomic32_remove (atomic: Atomic32?& implicit) <function-_at_jobque_c__c_atomic32_remove_&I1_ls_H_ls_jobque_c__c_Atomic32_gr__gr__qm__C_c_C_l>` 
  *  :ref:`with_atomic32 (block: block\<(var arg0:Atomic32?):void\> implicit) <function-_at_jobque_c__c_with_atomic32_CI0_ls_1_ls_H_ls_jobque_c__c_Atomic32_gr__gr__qm__gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`set (atomic: Atomic32? implicit; value: int) <function-_at_jobque_c__c_set_CI1_ls_H_ls_jobque_c__c_Atomic32_gr__gr__qm__Ci_C_c_C_l>` 
  *  :ref:`get (atomic: Atomic32? implicit) : int <function-_at_jobque_c__c_get_CI1_ls_H_ls_jobque_c__c_Atomic32_gr__gr__qm__C_c_C_l>` 
  *  :ref:`inc (atomic: Atomic32? implicit) : int <function-_at_jobque_c__c_inc_CI1_ls_H_ls_jobque_c__c_Atomic32_gr__gr__qm__C_c_C_l>` 
  *  :ref:`dec (atomic: Atomic32? implicit) : int <function-_at_jobque_c__c_dec_CI1_ls_H_ls_jobque_c__c_Atomic32_gr__gr__qm__C_c_C_l>` 
  *  :ref:`atomic64_create () : Atomic64? <function-_at_jobque_c__c_atomic64_create_C_c_C_l>` 
  *  :ref:`atomic64_remove (atomic: Atomic64?& implicit) <function-_at_jobque_c__c_atomic64_remove_&I1_ls_H_ls_jobque_c__c_Atomic64_gr__gr__qm__C_c_C_l>` 
  *  :ref:`with_atomic64 (block: block\<(var arg0:Atomic64?):void\> implicit) <function-_at_jobque_c__c_with_atomic64_CI0_ls_1_ls_H_ls_jobque_c__c_Atomic64_gr__gr__qm__gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`set (atomic: Atomic64? implicit; value: int64) <function-_at_jobque_c__c_set_CI1_ls_H_ls_jobque_c__c_Atomic64_gr__gr__qm__Ci64_C_c_C_l>` 
  *  :ref:`get (atomic: Atomic64? implicit) : int64 <function-_at_jobque_c__c_get_CI1_ls_H_ls_jobque_c__c_Atomic64_gr__gr__qm__C_c_C_l>` 
  *  :ref:`inc (atomic: Atomic64? implicit) : int64 <function-_at_jobque_c__c_inc_CI1_ls_H_ls_jobque_c__c_Atomic64_gr__gr__qm__C_c_C_l>` 
  *  :ref:`dec (atomic: Atomic64? implicit) : int64 <function-_at_jobque_c__c_dec_CI1_ls_H_ls_jobque_c__c_Atomic64_gr__gr__qm__C_c_C_l>` 

.. _function-_at_jobque_c__c_atomic32_create_C_c_C_l:

.. das:function:: atomic32_create() : Atomic32?

// stub


.. _function-_at_jobque_c__c_atomic32_remove_&I1_ls_H_ls_jobque_c__c_Atomic32_gr__gr__qm__C_c_C_l:

.. das:function:: atomic32_remove(atomic: Atomic32?& implicit)

.. warning:: 
  This is unsafe operation.

// stub



:Arguments: * **atomic** :  :ref:`Atomic32 <handle-jobque-Atomic32>` ?& implicit

.. _function-_at_jobque_c__c_with_atomic32_CI0_ls_1_ls_H_ls_jobque_c__c_Atomic32_gr__gr__qm__gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: with_atomic32(block: block<(var arg0:Atomic32?):void> implicit)

// stub



:Arguments: * **block** : block<( :ref:`Atomic32 <handle-jobque-Atomic32>` ?):void> implicit

.. _function-_at_jobque_c__c_set_CI1_ls_H_ls_jobque_c__c_Atomic32_gr__gr__qm__Ci_C_c_C_l:

.. das:function:: set(atomic: Atomic32? implicit; value: int)

// stub



:Arguments: * **atomic** :  :ref:`Atomic32 <handle-jobque-Atomic32>` ? implicit

            * **value** : int

.. _function-_at_jobque_c__c_get_CI1_ls_H_ls_jobque_c__c_Atomic32_gr__gr__qm__C_c_C_l:

.. das:function:: get(atomic: Atomic32? implicit) : int

// stub



:Arguments: * **atomic** :  :ref:`Atomic32 <handle-jobque-Atomic32>` ? implicit

.. _function-_at_jobque_c__c_inc_CI1_ls_H_ls_jobque_c__c_Atomic32_gr__gr__qm__C_c_C_l:

.. das:function:: inc(atomic: Atomic32? implicit) : int

// stub



:Arguments: * **atomic** :  :ref:`Atomic32 <handle-jobque-Atomic32>` ? implicit

.. _function-_at_jobque_c__c_dec_CI1_ls_H_ls_jobque_c__c_Atomic32_gr__gr__qm__C_c_C_l:

.. das:function:: dec(atomic: Atomic32? implicit) : int

// stub



:Arguments: * **atomic** :  :ref:`Atomic32 <handle-jobque-Atomic32>` ? implicit

.. _function-_at_jobque_c__c_atomic64_create_C_c_C_l:

.. das:function:: atomic64_create() : Atomic64?

// stub


.. _function-_at_jobque_c__c_atomic64_remove_&I1_ls_H_ls_jobque_c__c_Atomic64_gr__gr__qm__C_c_C_l:

.. das:function:: atomic64_remove(atomic: Atomic64?& implicit)

.. warning:: 
  This is unsafe operation.

// stub



:Arguments: * **atomic** :  :ref:`Atomic64 <handle-jobque-Atomic64>` ?& implicit

.. _function-_at_jobque_c__c_with_atomic64_CI0_ls_1_ls_H_ls_jobque_c__c_Atomic64_gr__gr__qm__gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: with_atomic64(block: block<(var arg0:Atomic64?):void> implicit)

// stub



:Arguments: * **block** : block<( :ref:`Atomic64 <handle-jobque-Atomic64>` ?):void> implicit

.. _function-_at_jobque_c__c_set_CI1_ls_H_ls_jobque_c__c_Atomic64_gr__gr__qm__Ci64_C_c_C_l:

.. das:function:: set(atomic: Atomic64? implicit; value: int64)

// stub



:Arguments: * **atomic** :  :ref:`Atomic64 <handle-jobque-Atomic64>` ? implicit

            * **value** : int64

.. _function-_at_jobque_c__c_get_CI1_ls_H_ls_jobque_c__c_Atomic64_gr__gr__qm__C_c_C_l:

.. das:function:: get(atomic: Atomic64? implicit) : int64

// stub



:Arguments: * **atomic** :  :ref:`Atomic64 <handle-jobque-Atomic64>` ? implicit

.. _function-_at_jobque_c__c_inc_CI1_ls_H_ls_jobque_c__c_Atomic64_gr__gr__qm__C_c_C_l:

.. das:function:: inc(atomic: Atomic64? implicit) : int64

// stub



:Arguments: * **atomic** :  :ref:`Atomic64 <handle-jobque-Atomic64>` ? implicit

.. _function-_at_jobque_c__c_dec_CI1_ls_H_ls_jobque_c__c_Atomic64_gr__gr__qm__C_c_C_l:

.. das:function:: dec(atomic: Atomic64? implicit) : int64

// stub



:Arguments: * **atomic** :  :ref:`Atomic64 <handle-jobque-Atomic64>` ? implicit


