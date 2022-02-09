
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

.. _function-_at_jobque_c__c_append_CI1_ls_H_ls_jobque_c__c_Channel_gr__gr_?_Ci:

.. das:function:: append(channel: jobque::Channel? const implicit; size: int const)

append returns int

+--------+----------------------------------------------------------------+
+argument+argument type                                                   +
+========+================================================================+
+channel + :ref:`jobque::Channel <handle-jobque-Channel>` ? const implicit+
+--------+----------------------------------------------------------------+
+size    +int const                                                       +
+--------+----------------------------------------------------------------+


|function-jobque-append|

.. _function-_at_jobque_c__c_with_channel_CI0_ls_1_ls_H_ls_jobque_c__c_Channel_gr__gr_?_gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: with_channel(block: block<(jobque::Channel?):void> const implicit)

+--------+------------------------------------------------------------------------------+
+argument+argument type                                                                 +
+========+==============================================================================+
+block   +block<( :ref:`jobque::Channel <handle-jobque-Channel>` ?):void> const implicit+
+--------+------------------------------------------------------------------------------+


|function-jobque-with_channel|

.. _function-_at_jobque_c__c_with_channel_Ci_CI0_ls_1_ls_H_ls_jobque_c__c_Channel_gr__gr_?_gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: with_channel(count: int const; block: block<(jobque::Channel?):void> const implicit)

+--------+------------------------------------------------------------------------------+
+argument+argument type                                                                 +
+========+==============================================================================+
+count   +int const                                                                     +
+--------+------------------------------------------------------------------------------+
+block   +block<( :ref:`jobque::Channel <handle-jobque-Channel>` ?):void> const implicit+
+--------+------------------------------------------------------------------------------+


|function-jobque-with_channel|

.. _function-_at_jobque_c__c_join_CI1_ls_H_ls_jobque_c__c_Channel_gr__gr_?:

.. das:function:: join(channel: jobque::Channel? const implicit)

+--------+----------------------------------------------------------------+
+argument+argument type                                                   +
+========+================================================================+
+channel + :ref:`jobque::Channel <handle-jobque-Channel>` ? const implicit+
+--------+----------------------------------------------------------------+


|function-jobque-join|

.. _function-_at_jobque_c__c_notify_CI1_ls_H_ls_jobque_c__c_Channel_gr__gr_?:

.. das:function:: notify(channel: jobque::Channel? const implicit)

+--------+----------------------------------------------------------------+
+argument+argument type                                                   +
+========+================================================================+
+channel + :ref:`jobque::Channel <handle-jobque-Channel>` ? const implicit+
+--------+----------------------------------------------------------------+


|function-jobque-notify|

.. _function-_at_jobque_c__c_with_job_status_Ci_CI0_ls_1_ls_H_ls_jobque_c__c_JobStatus_gr__gr_?_gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: with_job_status(total: int const; block: block<(jobque::JobStatus?):void> const implicit)

+--------+----------------------------------------------------------------------------------+
+argument+argument type                                                                     +
+========+==================================================================================+
+total   +int const                                                                         +
+--------+----------------------------------------------------------------------------------+
+block   +block<( :ref:`jobque::JobStatus <handle-jobque-JobStatus>` ?):void> const implicit+
+--------+----------------------------------------------------------------------------------+


|function-jobque-with_job_status|

.. _function-_at_jobque_c__c_join_CI1_ls_H_ls_jobque_c__c_JobStatus_gr__gr_?:

.. das:function:: join(job: jobque::JobStatus? const implicit)

+--------+--------------------------------------------------------------------+
+argument+argument type                                                       +
+========+====================================================================+
+job     + :ref:`jobque::JobStatus <handle-jobque-JobStatus>` ? const implicit+
+--------+--------------------------------------------------------------------+


|function-jobque-join|

.. _function-_at_jobque_c__c_notify_CI1_ls_H_ls_jobque_c__c_JobStatus_gr__gr_?:

.. das:function:: notify(job: jobque::JobStatus? const implicit)

+--------+--------------------------------------------------------------------+
+argument+argument type                                                       +
+========+====================================================================+
+job     + :ref:`jobque::JobStatus <handle-jobque-JobStatus>` ? const implicit+
+--------+--------------------------------------------------------------------+


|function-jobque-notify|

.. _function-_at_jobque_c__c_new_job_invoke_C_at__C_at__at__Ci_C_c_C_l:

.. das:function:: new_job_invoke(lambda: lambda<> const; function: function<> const; lambdaSize: int const)

+----------+----------------+
+argument  +argument type   +
+==========+================+
+lambda    +lambda<> const  +
+----------+----------------+
+function  +function<> const+
+----------+----------------+
+lambdaSize+int const       +
+----------+----------------+


|function-jobque-new_job_invoke|

.. _function-_at_jobque_c__c_with_job_que_CI1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: with_job_que(block: block<void> const implicit)

+--------+----------------------+
+argument+argument type         +
+========+======================+
+block   +block<> const implicit+
+--------+----------------------+


|function-jobque-with_job_que|

.. _function-_at_jobque_c__c_get_total_hw_jobs_C_c_C_l:

.. das:function:: get_total_hw_jobs()

get_total_hw_jobs returns int

|function-jobque-get_total_hw_jobs|

.. _function-_at_jobque_c__c_get_total_hw_threads:

.. das:function:: get_total_hw_threads()

get_total_hw_threads returns int

|function-jobque-get_total_hw_threads|

.. _function-_at_jobque_c__c_new_thread_invoke_C_at__C_at__at__Ci_C_c_C_l:

.. das:function:: new_thread_invoke(lambda: lambda<> const; function: function<> const; lambdaSize: int const)

+----------+----------------+
+argument  +argument type   +
+==========+================+
+lambda    +lambda<> const  +
+----------+----------------+
+function  +function<> const+
+----------+----------------+
+lambdaSize+int const       +
+----------+----------------+


|function-jobque-new_thread_invoke|

.. _function-_at_jobque_c__c_is_job_que_shutting_down:

.. das:function:: is_job_que_shutting_down()

is_job_que_shutting_down returns bool

|function-jobque-is_job_que_shutting_down|


