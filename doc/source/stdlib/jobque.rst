
.. _stdlib_jobque:

================
Jobs and threads
================

.. das:module:: jobque

The JOBQUE module provides low-level job queue and threading primitives.
It includes thread-safe channels for inter-thread communication, lock boxes
for shared data access, job status tracking, and fine-grained thread
management. For higher-level job abstractions, see ``jobque_boost``.

See :ref:`tutorial_jobque` for a hands-on tutorial.

All functions and symbols are in "jobque" module, use require to get access to it.

.. code-block:: das

    require jobque

Example:

.. code-block:: das

    require jobque

        [export]
        def main() {
            with_atomic32() $(counter) {
                counter |> set(10)
                print("value = {counter |> get}\n")
                let after_inc = counter |> inc
                print("after inc = {after_inc}\n")
                let after_dec = counter |> dec
                print("after dec = {after_dec}\n")
            }
        }
        // output:
        // value = 10
        // after inc = 11
        // after dec = 10



++++++++++++++++++
Handled structures
++++++++++++++++++

.. _handle-jobque-JobStatus:

.. das:attribute:: JobStatus

.. _function-jobque__dot__rq_isReady_JobStatus:

.. das:function:: JobStatus.isReady() : bool

Whether the job has completed execution.


.. _function-jobque__dot__rq_isValid_JobStatus:

.. das:function:: JobStatus.isValid() : bool

Whether the job status object refers to a valid, active job.


.. _function-jobque__dot__rq_size_JobStatus:

.. das:function:: JobStatus.size() : int

Returns the current entry count of the JobStatus or Channel.


:Properties: * **isReady** : bool

             * **isValid** : bool

             * **size** : int

 Job status indicator (ready or not, as well as entry count).



.. _handle-jobque-Channel:

.. das:attribute:: Channel

.. _function-jobque__dot__rq_isEmpty_Channel:

.. das:function:: Channel.isEmpty() : bool

Whether the channel or pipe contains no remaining elements.


.. _function-jobque__dot__rq_total_Channel:

.. das:function:: Channel.total() : int

Total number of elements that have been added to the pipe.


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

  *  :ref:`add_ref (status: JobStatus?) <function-jobque_add_ref_JobStatus_q_>`
  *  :ref:`append (channel: JobStatus?; size: int) : int <function-jobque_append_JobStatus_q__int>`
  *  :ref:`channel_create () : Channel? <function-jobque_channel_create>`
  *  :ref:`channel_remove (channel: Channel?&) <function-jobque_channel_remove_Channel_q__ref_>`
  *  :ref:`job_status_create () : JobStatus? <function-jobque_job_status_create>`
  *  :ref:`job_status_remove (jobStatus: JobStatus?&) <function-jobque_job_status_remove_JobStatus_q__ref_>`
  *  :ref:`join (job: JobStatus?) <function-jobque_join_JobStatus_q_>`
  *  :ref:`lock_box_create () : LockBox? <function-jobque_lock_box_create>`
  *  :ref:`lock_box_remove (box: LockBox?&) <function-jobque_lock_box_remove_LockBox_q__ref_>`
  *  :ref:`notify (job: JobStatus?) <function-jobque_notify_JobStatus_q_>`
  *  :ref:`notify_and_release (job: JobStatus?&) <function-jobque_notify_and_release_JobStatus_q__ref_>`
  *  :ref:`release (status: JobStatus?&) <function-jobque_release_JobStatus_q__ref_>`

.. _function-jobque_add_ref_JobStatus_q_:

.. das:function:: add_ref(status: JobStatus?)

Increases the reference count of a ``JobStatus`` or ``Channel``, preventing premature deletion.


:Arguments: * **status** :  :ref:`JobStatus <handle-jobque-JobStatus>`? implicit

.. _function-jobque_append_JobStatus_q__int:

.. das:function:: append(channel: JobStatus?; size: int) : int

Increases the entry count of the channel, signaling that new work has been added.


:Arguments: * **channel** :  :ref:`JobStatus <handle-jobque-JobStatus>`? implicit

            * **size** : int

.. _function-jobque_channel_create:

.. das:function:: channel_create() : Channel?

.. warning::
  This is unsafe operation.

Creates a new ``Channel`` for inter-thread communication and synchronization.


.. _function-jobque_channel_remove_Channel_q__ref_:

.. das:function:: channel_remove(channel: Channel?&)

.. warning::
  This is unsafe operation.

Destroys a ``Channel`` and releases its resources.


:Arguments: * **channel** :  :ref:`Channel <handle-jobque-Channel>`?\ & implicit

.. _function-jobque_job_status_create:

.. das:function:: job_status_create() : JobStatus?

Creates a new ``JobStatus`` object for tracking the completion state of asynchronous jobs.


.. _function-jobque_job_status_remove_JobStatus_q__ref_:

.. das:function:: job_status_remove(jobStatus: JobStatus?&)

.. warning::
  This is unsafe operation.

Destroys a ``JobStatus`` object and releases its resources.


:Arguments: * **jobStatus** :  :ref:`JobStatus <handle-jobque-JobStatus>`?\ & implicit

.. _function-jobque_join_JobStatus_q_:

.. das:function:: join(job: JobStatus?)

Blocks the current thread until the job or channel's entry count reaches zero, indicating all work is complete.


:Arguments: * **job** :  :ref:`JobStatus <handle-jobque-JobStatus>`? implicit

.. _function-jobque_lock_box_create:

.. das:function:: lock_box_create() : LockBox?

Creates a new ``LockBox`` for thread-safe shared access to a single value.


.. _function-jobque_lock_box_remove_LockBox_q__ref_:

.. das:function:: lock_box_remove(box: LockBox?&)

.. warning::
  This is unsafe operation.

Destroys a ``LockBox`` and releases its resources.


:Arguments: * **box** :  :ref:`LockBox <handle-jobque-LockBox>`?\ & implicit

.. _function-jobque_notify_JobStatus_q_:

.. das:function:: notify(job: JobStatus?)

Decreases the channel's entry count, signaling that one unit of work has completed.

Use ``notify`` when the caller does **not** own a reference to the channel — for example
when a ``Channel?`` is passed as a plain function argument via ``invoke_in_context``.
In that scenario no lambda captures the channel, so no extra reference was added and
there is nothing to release.

Compare with ``notify_and_release``, which additionally releases a reference and should
be used inside lambdas that captured the channel (adding a reference).


:Arguments: * **job** :  :ref:`JobStatus <handle-jobque-JobStatus>`? implicit

.. _function-jobque_notify_and_release_JobStatus_q__ref_:

.. das:function:: notify_and_release(job: JobStatus?&)

Decreases the entry count **and** the reference count of a ``Channel`` or ``JobStatus``
in a single operation.  After the call the channel/status variable is set to ``null``.

Use ``notify_and_release`` inside lambdas that captured the channel.  Capturing adds a
reference, so the lambda must release it when done.  This function combines
``notify`` + ``release`` into one atomic step and nulls the variable to prevent
accidental reuse.

If the caller does **not** own a reference (e.g. the channel was passed as a plain
argument via ``invoke_in_context``, with no lambda capture), use ``notify`` instead —
calling ``notify_and_release`` in that case would release a reference the caller never
added, leading to a premature free.


:Arguments: * **job** :  :ref:`JobStatus <handle-jobque-JobStatus>`?\ & implicit

.. _function-jobque_release_JobStatus_q__ref_:

.. das:function:: release(status: JobStatus?&)

Decreases the reference count of a ``JobStatus`` or ``Channel``; the object is deleted when the count reaches zero.


:Arguments: * **status** :  :ref:`JobStatus <handle-jobque-JobStatus>`?\ & implicit


+++++++
Queries
+++++++

  *  :ref:`get_total_hw_jobs () : int <function-jobque_get_total_hw_jobs>`
  *  :ref:`get_total_hw_threads () : int <function-jobque_get_total_hw_threads>`
  *  :ref:`is_job_que_shutting_down () : bool <function-jobque_is_job_que_shutting_down>`

.. _function-jobque_get_total_hw_jobs:

.. das:function:: get_total_hw_jobs() : int

Returns the total number of hardware threads allocated to the job system.


.. _function-jobque_get_total_hw_threads:

.. das:function:: get_total_hw_threads() : int

Returns the total number of hardware threads available on the system.


.. _function-jobque_is_job_que_shutting_down:

.. das:function:: is_job_que_shutting_down() : bool

Returns ``true`` if the job queue infrastructure is shutting down or has not been initialized.



++++++++++++++++++++
Internal invocations
++++++++++++++++++++

  *  :ref:`new_debugger_thread (block: block\<():void\>) <function-jobque_new_debugger_thread_block_ls__c_void_gr_>`
  *  :ref:`new_job_invoke (lambda: lambda\<():void\>; function: function\<():void\>; lambdaSize: int) <function-jobque_new_job_invoke_lambda_ls__c_void_gr__function_ls__c_void_gr__int>`
  *  :ref:`new_thread_invoke (lambda: lambda\<():void\>; function: function\<():void\>; lambdaSize: int) <function-jobque_new_thread_invoke_lambda_ls__c_void_gr__function_ls__c_void_gr__int>`

.. _function-jobque_new_debugger_thread_block_ls__c_void_gr_:

.. das:function:: new_debugger_thread(block: block<():void>)

Creates a new debugger tick thread for servicing debug connections.


:Arguments: * **block** : block<void> implicit

.. _function-jobque_new_job_invoke_lambda_ls__c_void_gr__function_ls__c_void_gr__int:

.. das:function:: new_job_invoke(lambda: lambda<():void>; function: function<():void>; lambdaSize: int)

Clones the current context, moves the attached lambda into it, and submits it to the job queue.


:Arguments: * **lambda** : lambda<void>

            * **function** : function<void>

            * **lambdaSize** : int

.. _function-jobque_new_thread_invoke_lambda_ls__c_void_gr__function_ls__c_void_gr__int:

.. das:function:: new_thread_invoke(lambda: lambda<():void>; function: function<():void>; lambdaSize: int)

Clones the current context, moves the attached lambda into it, and runs it on a new dedicated thread.


:Arguments: * **lambda** : lambda<void>

            * **function** : function<void>

            * **lambdaSize** : int


++++++++++++
Construction
++++++++++++

  *  :ref:`with_channel (block: block\<(Channel?):void\>) <function-jobque_with_channel_block_ls_Channel_q__c_void_gr_>`
  *  :ref:`with_channel (count: int; block: block\<(Channel?):void\>) <function-jobque_with_channel_int_block_ls_Channel_q__c_void_gr_>`
  *  :ref:`with_job_que (block: block\<():void\>) <function-jobque_with_job_que_block_ls__c_void_gr_>`
  *  :ref:`with_job_status (total: int; block: block\<(JobStatus?):void\>) <function-jobque_with_job_status_int_block_ls_JobStatus_q__c_void_gr_>`
  *  :ref:`with_lock_box (block: block\<(LockBox?):void\>) <function-jobque_with_lock_box_block_ls_LockBox_q__c_void_gr_>`


with_channel
^^^^^^^^^^^^

.. _function-jobque_with_channel_block_ls_Channel_q__c_void_gr_:

.. das:function:: with_channel(block: block<(Channel?):void>)

Creates a ``Channel`` scoped to the given block and automatically destroys it afterward.


:Arguments: * **block** : block<( :ref:`Channel <handle-jobque-Channel>`?):void> implicit

.. _function-jobque_with_channel_int_block_ls_Channel_q__c_void_gr_:

.. das:function:: with_channel(count: int; block: block<(Channel?):void>)

----

.. _function-jobque_with_job_que_block_ls__c_void_gr_:

.. das:function:: with_job_que(block: block<():void>)

Ensures job queue infrastructure is initialized for the duration of the block.


:Arguments: * **block** : block<void> implicit

.. _function-jobque_with_job_status_int_block_ls_JobStatus_q__c_void_gr_:

.. das:function:: with_job_status(total: int; block: block<(JobStatus?):void>)

Creates a ``JobStatus`` scoped to the given block and automatically destroys it afterward.


:Arguments: * **total** : int

            * **block** : block<( :ref:`JobStatus <handle-jobque-JobStatus>`?):void> implicit

.. _function-jobque_with_lock_box_block_ls_LockBox_q__c_void_gr_:

.. das:function:: with_lock_box(block: block<(LockBox?):void>)

Creates a ``LockBox`` scoped to the given block and automatically destroys it afterward.


:Arguments: * **block** : block<( :ref:`LockBox <handle-jobque-LockBox>`?):void> implicit


++++++
Atomic
++++++

  *  :ref:`atomic32_create () : Atomic32? <function-jobque_atomic32_create>`
  *  :ref:`atomic32_remove (atomic: Atomic32?&) <function-jobque_atomic32_remove_Atomic32_q__ref_>`
  *  :ref:`atomic64_create () : Atomic64? <function-jobque_atomic64_create>`
  *  :ref:`atomic64_remove (atomic: Atomic64?&) <function-jobque_atomic64_remove_Atomic64_q__ref_>`
  *  :ref:`dec (atomic: Atomic32?) : int <function-jobque_dec_Atomic32_q_>`
  *  :ref:`dec (atomic: Atomic64?) : int64 <function-jobque_dec_Atomic64_q_>`
  *  :ref:`get (atomic: Atomic32?) : int <function-jobque_get_Atomic32_q_>`
  *  :ref:`get (atomic: Atomic64?) : int64 <function-jobque_get_Atomic64_q_>`
  *  :ref:`inc (atomic: Atomic32?) : int <function-jobque_inc_Atomic32_q_>`
  *  :ref:`inc (atomic: Atomic64?) : int64 <function-jobque_inc_Atomic64_q_>`
  *  :ref:`set (atomic: Atomic32?; value: int) <function-jobque_set_Atomic32_q__int>`
  *  :ref:`set (atomic: Atomic64?; value: int64) <function-jobque_set_Atomic64_q__int64>`
  *  :ref:`with_atomic32 (block: block\<(Atomic32?):void\>) <function-jobque_with_atomic32_block_ls_Atomic32_q__c_void_gr_>`
  *  :ref:`with_atomic64 (block: block\<(Atomic64?):void\>) <function-jobque_with_atomic64_block_ls_Atomic64_q__c_void_gr_>`

.. _function-jobque_atomic32_create:

.. das:function:: atomic32_create() : Atomic32?

Creates an ``Atomic32`` — a thread-safe 32-bit integer for lock-free concurrent access.


.. _function-jobque_atomic32_remove_Atomic32_q__ref_:

.. das:function:: atomic32_remove(atomic: Atomic32?&)

.. warning::
  This is unsafe operation.

Destroys an ``Atomic32`` and releases its resources.


:Arguments: * **atomic** :  :ref:`Atomic32 <handle-jobque-Atomic32>`?\ & implicit

.. _function-jobque_atomic64_create:

.. das:function:: atomic64_create() : Atomic64?

Creates an ``Atomic64`` — a thread-safe 64-bit integer for lock-free concurrent access.


.. _function-jobque_atomic64_remove_Atomic64_q__ref_:

.. das:function:: atomic64_remove(atomic: Atomic64?&)

.. warning::
  This is unsafe operation.

Destroys an ``Atomic64`` and releases its resources.


:Arguments: * **atomic** :  :ref:`Atomic64 <handle-jobque-Atomic64>`?\ & implicit


dec
^^^

.. _function-jobque_dec_Atomic32_q_:

.. das:function:: dec(atomic: Atomic32?) : int

Atomically decrements the integer value and returns the result.


:Arguments: * **atomic** :  :ref:`Atomic32 <handle-jobque-Atomic32>`? implicit

.. _function-jobque_dec_Atomic64_q_:

.. das:function:: dec(atomic: Atomic64?) : int64

----


get
^^^

.. _function-jobque_get_Atomic32_q_:

.. das:function:: get(atomic: Atomic32?) : int

Returns the current value of the atomic integer.


:Arguments: * **atomic** :  :ref:`Atomic32 <handle-jobque-Atomic32>`? implicit

.. _function-jobque_get_Atomic64_q_:

.. das:function:: get(atomic: Atomic64?) : int64

----


inc
^^^

.. _function-jobque_inc_Atomic32_q_:

.. das:function:: inc(atomic: Atomic32?) : int

Atomically increments the integer value and returns the result.


:Arguments: * **atomic** :  :ref:`Atomic32 <handle-jobque-Atomic32>`? implicit

.. _function-jobque_inc_Atomic64_q_:

.. das:function:: inc(atomic: Atomic64?) : int64

----


set
^^^

.. _function-jobque_set_Atomic32_q__int:

.. das:function:: set(atomic: Atomic32?; value: int)

Sets the atomic integer to the specified value.


:Arguments: * **atomic** :  :ref:`Atomic32 <handle-jobque-Atomic32>`? implicit

            * **value** : int

.. _function-jobque_set_Atomic64_q__int64:

.. das:function:: set(atomic: Atomic64?; value: int64)

----

.. _function-jobque_with_atomic32_block_ls_Atomic32_q__c_void_gr_:

.. das:function:: with_atomic32(block: block<(Atomic32?):void>)

Creates an ``Atomic32`` scoped to the given block and automatically destroys it afterward.


:Arguments: * **block** : block<( :ref:`Atomic32 <handle-jobque-Atomic32>`?):void> implicit

.. _function-jobque_with_atomic64_block_ls_Atomic64_q__c_void_gr_:

.. das:function:: with_atomic64(block: block<(Atomic64?):void>)

Creates an ``Atomic64`` scoped to the given block and automatically destroys it afterward.


:Arguments: * **block** : block<( :ref:`Atomic64 <handle-jobque-Atomic64>`?):void> implicit


