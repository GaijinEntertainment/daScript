
.. _stdlib_jobque:

================
Jobs and threads
================

.. include:: detail/jobque.rst

|module-jobque|

++++++++++++++++++
Handled structures
++++++++++++++++++

.. _handle-jobque-Channel:

.. das:attribute:: Channel

Channel properties are

+-------+----+
+size   +int +
+-------+----+
+isEmpty+bool+
+-------+----+
+isReady+bool+
+-------+----+


|structure_annotation-jobque-Channel|

.. _handle-jobque-JobStatus:

.. das:attribute:: JobStatus

JobStatus properties are

+-------+----+
+isReady+bool+
+-------+----+


|structure_annotation-jobque-JobStatus|

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_jobque_c__c_append__hh_const_hh__hh_implicit_hh__hh_ptr_hh__hh_handle_hh_Channel__hh_const_hh_int:

.. das:function:: append(arg0: jobque::Channel? const implicit; arg1: int const)

append returns int

+--------+----------------------------------------------------------------+
+argument+argument type                                                   +
+========+================================================================+
+arg0    + :ref:`jobque::Channel <handle-jobque-Channel>` ? const implicit+
+--------+----------------------------------------------------------------+
+arg1    +int const                                                       +
+--------+----------------------------------------------------------------+


|function-jobque-append|

.. _function-_at_jobque_c__c_get_total_hw_jobs__hh_const_hh___context__hh_const_hh___lineInfo:

.. das:function:: get_total_hw_jobs()

get_total_hw_jobs returns int

|function-jobque-get_total_hw_jobs|

.. _function-_at_jobque_c__c_get_total_hw_threads:

.. das:function:: get_total_hw_threads()

get_total_hw_threads returns int

|function-jobque-get_total_hw_threads|

.. _function-_at_jobque_c__c_join__hh_const_hh__hh_implicit_hh__hh_ptr_hh__hh_handle_hh_Channel:

.. das:function:: join(arg0: jobque::Channel? const implicit)

+--------+----------------------------------------------------------------+
+argument+argument type                                                   +
+========+================================================================+
+arg0    + :ref:`jobque::Channel <handle-jobque-Channel>` ? const implicit+
+--------+----------------------------------------------------------------+


|function-jobque-join|

.. _function-_at_jobque_c__c_join__hh_const_hh__hh_implicit_hh__hh_ptr_hh__hh_handle_hh_JobStatus:

.. das:function:: join(arg0: jobque::JobStatus? const implicit)

+--------+--------------------------------------------------------------------+
+argument+argument type                                                       +
+========+====================================================================+
+arg0    + :ref:`jobque::JobStatus <handle-jobque-JobStatus>` ? const implicit+
+--------+--------------------------------------------------------------------+


|function-jobque-join|

.. _function-_at_jobque_c__c_new_job_invoke__hh_const_hh__hh_lambda__hh_const_hh__hh_function__hh_const_hh_int__hh_const_hh___context__hh_const_hh___lineInfo:

.. das:function:: new_job_invoke(arg0: lambda<> const; arg1: function<> const; arg2: int const)

+--------+----------------+
+argument+argument type   +
+========+================+
+arg0    +lambda<> const  +
+--------+----------------+
+arg1    +function<> const+
+--------+----------------+
+arg2    +int const       +
+--------+----------------+


|function-jobque-new_job_invoke|

.. _function-_at_jobque_c__c_new_thread_invoke__hh_const_hh__hh_lambda__hh_const_hh__hh_function__hh_const_hh_int__hh_const_hh___context:

.. das:function:: new_thread_invoke(arg0: lambda<> const; arg1: function<> const; arg2: int const)

+--------+----------------+
+argument+argument type   +
+========+================+
+arg0    +lambda<> const  +
+--------+----------------+
+arg1    +function<> const+
+--------+----------------+
+arg2    +int const       +
+--------+----------------+


|function-jobque-new_thread_invoke|

.. _function-_at_jobque_c__c_notify__hh_const_hh__hh_implicit_hh__hh_ptr_hh__hh_handle_hh_Channel:

.. das:function:: notify(arg0: jobque::Channel? const implicit)

+--------+----------------------------------------------------------------+
+argument+argument type                                                   +
+========+================================================================+
+arg0    + :ref:`jobque::Channel <handle-jobque-Channel>` ? const implicit+
+--------+----------------------------------------------------------------+


|function-jobque-notify|

.. _function-_at_jobque_c__c_notify__hh_const_hh__hh_implicit_hh__hh_ptr_hh__hh_handle_hh_JobStatus:

.. das:function:: notify(arg0: jobque::JobStatus? const implicit)

+--------+--------------------------------------------------------------------+
+argument+argument type                                                       +
+========+====================================================================+
+arg0    + :ref:`jobque::JobStatus <handle-jobque-JobStatus>` ? const implicit+
+--------+--------------------------------------------------------------------+


|function-jobque-notify|

.. _function-_at_jobque_c__c_with_channel__hh_const_hh__hh_implicit_hh__hh_block_hh__hh_ptr_hh__hh_handle_hh_Channel_hh__c_void__hh_const_hh___context:

.. das:function:: with_channel(arg0: block<(jobque::Channel?):void> const implicit)

+--------+------------------------------------------------------------------------------+
+argument+argument type                                                                 +
+========+==============================================================================+
+arg0    +block<( :ref:`jobque::Channel <handle-jobque-Channel>` ?):void> const implicit+
+--------+------------------------------------------------------------------------------+


|function-jobque-with_channel|

.. _function-_at_jobque_c__c_with_channel__hh_const_hh_int__hh_const_hh__hh_implicit_hh__hh_block_hh__hh_ptr_hh__hh_handle_hh_Channel_hh__c_void__hh_const_hh___context:

.. das:function:: with_channel(arg0: int const; arg1: block<(jobque::Channel?):void> const implicit)

+--------+------------------------------------------------------------------------------+
+argument+argument type                                                                 +
+========+==============================================================================+
+arg0    +int const                                                                     +
+--------+------------------------------------------------------------------------------+
+arg1    +block<( :ref:`jobque::Channel <handle-jobque-Channel>` ?):void> const implicit+
+--------+------------------------------------------------------------------------------+


|function-jobque-with_channel|

.. _function-_at_jobque_c__c_with_job_que__hh_const_hh__hh_implicit_hh__hh_block_hh__c_void__hh_const_hh___context__hh_const_hh___lineInfo:

.. das:function:: with_job_que(arg0: block<void> const implicit)

+--------+----------------------+
+argument+argument type         +
+========+======================+
+arg0    +block<> const implicit+
+--------+----------------------+


|function-jobque-with_job_que|

.. _function-_at_jobque_c__c_with_job_status__hh_const_hh_int__hh_const_hh__hh_implicit_hh__hh_block_hh__hh_ptr_hh__hh_handle_hh_JobStatus_hh__c_void__hh_const_hh___context__hh_const_hh___lineInfo:

.. das:function:: with_job_status(arg0: int const; arg1: block<(jobque::JobStatus?):void> const implicit)

+--------+----------------------------------------------------------------------------------+
+argument+argument type                                                                     +
+========+==================================================================================+
+arg0    +int const                                                                         +
+--------+----------------------------------------------------------------------------------+
+arg1    +block<( :ref:`jobque::JobStatus <handle-jobque-JobStatus>` ?):void> const implicit+
+--------+----------------------------------------------------------------------------------+


|function-jobque-with_job_status|


