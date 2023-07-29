
.. _stdlib_jobque:

================
Jobs and threads
================

.. include:: detail/jobque.rst

Apply module implements job que and threading.

All functions and symbols are in "jobque" module, use require to get access to it. ::

    require jobque



++++++++++++++++++
Handled structures
++++++++++++++++++

.. _handle-jobque-Channel:

.. das:attribute:: Channel

Channel property operators are

+-------+----+
+isEmpty+bool+
+-------+----+
+isReady+bool+
+-------+----+
+size   +int +
+-------+----+
+total  +int +
+-------+----+


|structure_annotation-jobque-Channel|

.. _handle-jobque-JobStatus:

.. das:attribute:: JobStatus

JobStatus property operators are

+-------+----+
+isReady+bool+
+-------+----+


|structure_annotation-jobque-JobStatus|

+++++++++++++++++++++
Channel and JobStatus
+++++++++++++++++++++

  *  :ref:`append (channel:jobque::Channel? const implicit;size:int const;context:__context const;line:__lineInfo const) : int <function-_at_jobque_c__c_append_CI1_ls_H_ls_jobque_c__c_Channel_gr__gr_?_Ci_C_c_C_l>` 
  *  :ref:`channel_create (context:__context const;line:__lineInfo const) : jobque::Channel? <function-_at_jobque_c__c_channel_create_C_c_C_l>` 
  *  :ref:`channel_remove (channel:jobque::Channel? const implicit;context:__context const;line:__lineInfo const) : void <function-_at_jobque_c__c_channel_remove_CI1_ls_H_ls_jobque_c__c_Channel_gr__gr_?_C_c_C_l>` 
  *  :ref:`add_ref (channel:jobque::Channel? const implicit;context:__context const;line:__lineInfo const) : void <function-_at_jobque_c__c_add_ref_CI1_ls_H_ls_jobque_c__c_Channel_gr__gr_?_C_c_C_l>` 
  *  :ref:`release (channel:jobque::Channel?& implicit;context:__context const;line:__lineInfo const) : void <function-_at_jobque_c__c_release_&I1_ls_H_ls_jobque_c__c_Channel_gr__gr_?_C_c_C_l>` 
  *  :ref:`join (channel:jobque::Channel? const implicit;context:__context const;line:__lineInfo const) : void <function-_at_jobque_c__c_join_CI1_ls_H_ls_jobque_c__c_Channel_gr__gr_?_C_c_C_l>` 
  *  :ref:`notify (channel:jobque::Channel? const implicit;context:__context const;line:__lineInfo const) : void <function-_at_jobque_c__c_notify_CI1_ls_H_ls_jobque_c__c_Channel_gr__gr_?_C_c_C_l>` 
  *  :ref:`notify_and_release (channel:jobque::Channel?& implicit;context:__context const;line:__lineInfo const) : void <function-_at_jobque_c__c_notify_and_release_&I1_ls_H_ls_jobque_c__c_Channel_gr__gr_?_C_c_C_l>` 
  *  :ref:`add_ref (status:jobque::JobStatus? const implicit;context:__context const;line:__lineInfo const) : void <function-_at_jobque_c__c_add_ref_CI1_ls_H_ls_jobque_c__c_JobStatus_gr__gr_?_C_c_C_l>` 
  *  :ref:`release (status:jobque::JobStatus?& implicit;context:__context const;line:__lineInfo const) : void <function-_at_jobque_c__c_release_&I1_ls_H_ls_jobque_c__c_JobStatus_gr__gr_?_C_c_C_l>` 
  *  :ref:`join (job:jobque::JobStatus? const implicit;context:__context const;line:__lineInfo const) : void <function-_at_jobque_c__c_join_CI1_ls_H_ls_jobque_c__c_JobStatus_gr__gr_?_C_c_C_l>` 
  *  :ref:`notify (job:jobque::JobStatus? const implicit;context:__context const;line:__lineInfo const) : void <function-_at_jobque_c__c_notify_CI1_ls_H_ls_jobque_c__c_JobStatus_gr__gr_?_C_c_C_l>` 
  *  :ref:`notify_and_release (job:jobque::JobStatus?& implicit;context:__context const;line:__lineInfo const) : void <function-_at_jobque_c__c_notify_and_release_&I1_ls_H_ls_jobque_c__c_JobStatus_gr__gr_?_C_c_C_l>` 

.. _function-_at_jobque_c__c_append_CI1_ls_H_ls_jobque_c__c_Channel_gr__gr_?_Ci_C_c_C_l:

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

.. _function-_at_jobque_c__c_channel_create_C_c_C_l:

.. das:function:: channel_create()

channel_create returns  :ref:`jobque::Channel <handle-jobque-Channel>` ?

.. warning:: 
  This is unsafe operation.

|function-jobque-channel_create|

.. _function-_at_jobque_c__c_channel_remove_CI1_ls_H_ls_jobque_c__c_Channel_gr__gr_?_C_c_C_l:

.. das:function:: channel_remove(channel: jobque::Channel? const implicit)

.. warning:: 
  This is unsafe operation.

+--------+----------------------------------------------------------------+
+argument+argument type                                                   +
+========+================================================================+
+channel + :ref:`jobque::Channel <handle-jobque-Channel>` ? const implicit+
+--------+----------------------------------------------------------------+


|function-jobque-channel_remove|

.. _function-_at_jobque_c__c_add_ref_CI1_ls_H_ls_jobque_c__c_Channel_gr__gr_?_C_c_C_l:

.. das:function:: add_ref(channel: jobque::Channel? const implicit)

+--------+----------------------------------------------------------------+
+argument+argument type                                                   +
+========+================================================================+
+channel + :ref:`jobque::Channel <handle-jobque-Channel>` ? const implicit+
+--------+----------------------------------------------------------------+


|function-jobque-add_ref|

.. _function-_at_jobque_c__c_release_&I1_ls_H_ls_jobque_c__c_Channel_gr__gr_?_C_c_C_l:

.. das:function:: release(channel: jobque::Channel?& implicit)

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+channel + :ref:`jobque::Channel <handle-jobque-Channel>` ?& implicit+
+--------+-----------------------------------------------------------+


|function-jobque-release|

.. _function-_at_jobque_c__c_join_CI1_ls_H_ls_jobque_c__c_Channel_gr__gr_?_C_c_C_l:

.. das:function:: join(channel: jobque::Channel? const implicit)

+--------+----------------------------------------------------------------+
+argument+argument type                                                   +
+========+================================================================+
+channel + :ref:`jobque::Channel <handle-jobque-Channel>` ? const implicit+
+--------+----------------------------------------------------------------+


|function-jobque-join|

.. _function-_at_jobque_c__c_notify_CI1_ls_H_ls_jobque_c__c_Channel_gr__gr_?_C_c_C_l:

.. das:function:: notify(channel: jobque::Channel? const implicit)

+--------+----------------------------------------------------------------+
+argument+argument type                                                   +
+========+================================================================+
+channel + :ref:`jobque::Channel <handle-jobque-Channel>` ? const implicit+
+--------+----------------------------------------------------------------+


|function-jobque-notify|

.. _function-_at_jobque_c__c_notify_and_release_&I1_ls_H_ls_jobque_c__c_Channel_gr__gr_?_C_c_C_l:

.. das:function:: notify_and_release(channel: jobque::Channel?& implicit)

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+channel + :ref:`jobque::Channel <handle-jobque-Channel>` ?& implicit+
+--------+-----------------------------------------------------------+


|function-jobque-notify_and_release|

.. _function-_at_jobque_c__c_add_ref_CI1_ls_H_ls_jobque_c__c_JobStatus_gr__gr_?_C_c_C_l:

.. das:function:: add_ref(status: jobque::JobStatus? const implicit)

+--------+--------------------------------------------------------------------+
+argument+argument type                                                       +
+========+====================================================================+
+status  + :ref:`jobque::JobStatus <handle-jobque-JobStatus>` ? const implicit+
+--------+--------------------------------------------------------------------+


|function-jobque-add_ref|

.. _function-_at_jobque_c__c_release_&I1_ls_H_ls_jobque_c__c_JobStatus_gr__gr_?_C_c_C_l:

.. das:function:: release(status: jobque::JobStatus?& implicit)

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+status  + :ref:`jobque::JobStatus <handle-jobque-JobStatus>` ?& implicit+
+--------+---------------------------------------------------------------+


|function-jobque-release|

.. _function-_at_jobque_c__c_join_CI1_ls_H_ls_jobque_c__c_JobStatus_gr__gr_?_C_c_C_l:

.. das:function:: join(job: jobque::JobStatus? const implicit)

+--------+--------------------------------------------------------------------+
+argument+argument type                                                       +
+========+====================================================================+
+job     + :ref:`jobque::JobStatus <handle-jobque-JobStatus>` ? const implicit+
+--------+--------------------------------------------------------------------+


|function-jobque-join|

.. _function-_at_jobque_c__c_notify_CI1_ls_H_ls_jobque_c__c_JobStatus_gr__gr_?_C_c_C_l:

.. das:function:: notify(job: jobque::JobStatus? const implicit)

+--------+--------------------------------------------------------------------+
+argument+argument type                                                       +
+========+====================================================================+
+job     + :ref:`jobque::JobStatus <handle-jobque-JobStatus>` ? const implicit+
+--------+--------------------------------------------------------------------+


|function-jobque-notify|

.. _function-_at_jobque_c__c_notify_and_release_&I1_ls_H_ls_jobque_c__c_JobStatus_gr__gr_?_C_c_C_l:

.. das:function:: notify_and_release(job: jobque::JobStatus?& implicit)

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+job     + :ref:`jobque::JobStatus <handle-jobque-JobStatus>` ?& implicit+
+--------+---------------------------------------------------------------+


|function-jobque-notify_and_release|

+++++++
Queries
+++++++

  *  :ref:`get_total_hw_jobs (context:__context const;line:__lineInfo const) : int <function-_at_jobque_c__c_get_total_hw_jobs_C_c_C_l>` 
  *  :ref:`get_total_hw_threads () : int <function-_at_jobque_c__c_get_total_hw_threads>` 
  *  :ref:`is_job_que_shutting_down () : bool <function-_at_jobque_c__c_is_job_que_shutting_down>` 

.. _function-_at_jobque_c__c_get_total_hw_jobs_C_c_C_l:

.. das:function:: get_total_hw_jobs()

get_total_hw_jobs returns int

|function-jobque-get_total_hw_jobs|

.. _function-_at_jobque_c__c_get_total_hw_threads:

.. das:function:: get_total_hw_threads()

get_total_hw_threads returns int

|function-jobque-get_total_hw_threads|

.. _function-_at_jobque_c__c_is_job_que_shutting_down:

.. das:function:: is_job_que_shutting_down()

is_job_que_shutting_down returns bool

|function-jobque-is_job_que_shutting_down|

++++++++++++++++++++
Internal invocations
++++++++++++++++++++

  *  :ref:`new_job_invoke (lambda:lambda\<\> const;function:function\<\> const;lambdaSize:int const;context:__context const;line:__lineInfo const) : void <function-_at_jobque_c__c_new_job_invoke_C_at__C_at__at__Ci_C_c_C_l>` 
  *  :ref:`new_thread_invoke (lambda:lambda\<\> const;function:function\<\> const;lambdaSize:int const;context:__context const;line:__lineInfo const) : void <function-_at_jobque_c__c_new_thread_invoke_C_at__C_at__at__Ci_C_c_C_l>` 

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

++++++++++++
Construction
++++++++++++

  *  :ref:`with_channel (block:block\<(var arg0:jobque::Channel?):void\> const implicit;context:__context const;line:__lineInfo const) : void <function-_at_jobque_c__c_with_channel_CI0_ls_1_ls_H_ls_jobque_c__c_Channel_gr__gr_?_gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`with_channel (count:int const;block:block\<(var arg0:jobque::Channel?):void\> const implicit;context:__context const;line:__lineInfo const) : void <function-_at_jobque_c__c_with_channel_Ci_CI0_ls_1_ls_H_ls_jobque_c__c_Channel_gr__gr_?_gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`with_job_status (total:int const;block:block\<(var arg0:jobque::JobStatus?):void\> const implicit;context:__context const;line:__lineInfo const) : void <function-_at_jobque_c__c_with_job_status_Ci_CI0_ls_1_ls_H_ls_jobque_c__c_JobStatus_gr__gr_?_gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`with_job_que (block:block\<void\> const implicit;context:__context const;line:__lineInfo const) : void <function-_at_jobque_c__c_with_job_que_CI1_ls_v_gr__builtin__C_c_C_l>` 

.. _function-_at_jobque_c__c_with_channel_CI0_ls_1_ls_H_ls_jobque_c__c_Channel_gr__gr_?_gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: with_channel(block: block<(var arg0:jobque::Channel?):void> const implicit)

+--------+------------------------------------------------------------------------------+
+argument+argument type                                                                 +
+========+==============================================================================+
+block   +block<( :ref:`jobque::Channel <handle-jobque-Channel>` ?):void> const implicit+
+--------+------------------------------------------------------------------------------+


|function-jobque-with_channel|

.. _function-_at_jobque_c__c_with_channel_Ci_CI0_ls_1_ls_H_ls_jobque_c__c_Channel_gr__gr_?_gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: with_channel(count: int const; block: block<(var arg0:jobque::Channel?):void> const implicit)

+--------+------------------------------------------------------------------------------+
+argument+argument type                                                                 +
+========+==============================================================================+
+count   +int const                                                                     +
+--------+------------------------------------------------------------------------------+
+block   +block<( :ref:`jobque::Channel <handle-jobque-Channel>` ?):void> const implicit+
+--------+------------------------------------------------------------------------------+


|function-jobque-with_channel|

.. _function-_at_jobque_c__c_with_job_status_Ci_CI0_ls_1_ls_H_ls_jobque_c__c_JobStatus_gr__gr_?_gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: with_job_status(total: int const; block: block<(var arg0:jobque::JobStatus?):void> const implicit)

+--------+----------------------------------------------------------------------------------+
+argument+argument type                                                                     +
+========+==================================================================================+
+total   +int const                                                                         +
+--------+----------------------------------------------------------------------------------+
+block   +block<( :ref:`jobque::JobStatus <handle-jobque-JobStatus>` ?):void> const implicit+
+--------+----------------------------------------------------------------------------------+


|function-jobque-with_job_status|

.. _function-_at_jobque_c__c_with_job_que_CI1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: with_job_que(block: block<void> const implicit)

+--------+----------------------+
+argument+argument type         +
+========+======================+
+block   +block<> const implicit+
+--------+----------------------+


|function-jobque-with_job_que|


