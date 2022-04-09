
.. _stdlib_jobque_boost:

==================================
Boost package for jobs and threads
==================================

.. include:: detail/jobque_boost.rst

|module-jobque_boost|

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

  *  :ref:`new_job (l:lambda\<\> -const) : void <function-_at_jobque_boost_c__c_new_job__at_>` 
  *  :ref:`new_thread (l:lambda\<\> -const) : void <function-_at_jobque_boost_c__c_new_thread__at_>` 

.. _function-_at_jobque_boost_c__c_new_job__at_:

.. das:function:: new_job(l: lambda<>)

+--------+-------------+
+argument+argument type+
+========+=============+
+l       +lambda<>     +
+--------+-------------+


Create a new job.
    * new context is cloned from the current context.
    * lambda is cloned to the new context.
    * new job is added to the job queue.
    * once new job is invoked, lambda is invoked on the new context on the job thread.

.. _function-_at_jobque_boost_c__c_new_thread__at_:

.. das:function:: new_thread(l: lambda<>)

+--------+-------------+
+argument+argument type+
+========+=============+
+l       +lambda<>     +
+--------+-------------+


Create a new thread
    * new context is cloned from the current context.
    * lambda is cloned to the new context.
    * new thread is created.
    * lambda is invoked on the new context on the new thread.

+++++++++
Iteration
+++++++++

  *  :ref:`for_each (channel:jobque::Channel? const;blk:block\<(res:auto(TT) const#):void\> const) : auto <function-_at_jobque_boost_c__c_for_each_C1_ls_H_ls_jobque_c__c_Channel_gr__gr_?_CN_ls_res_gr_0_ls_C_hh_Y_ls_TT_gr_._gr_1_ls_v_gr__builtin_>` 
  *  :ref:`each (channel:jobque::Channel? const;tinfo:auto(TT) const) : auto <function-_at_jobque_boost_c__c_each_C1_ls_H_ls_jobque_c__c_Channel_gr__gr_?_CY_ls_TT_gr_.>` 

.. _function-_at_jobque_boost_c__c_for_each_C1_ls_H_ls_jobque_c__c_Channel_gr__gr_?_CN_ls_res_gr_0_ls_C_hh_Y_ls_TT_gr_._gr_1_ls_v_gr__builtin_:

.. das:function:: for_each(channel: jobque::Channel? const; blk: block<(res:auto(TT) const#):void> const)

for_each returns auto

+--------+-------------------------------------------------------+
+argument+argument type                                          +
+========+=======================================================+
+channel + :ref:`jobque::Channel <handle-jobque-Channel>` ? const+
+--------+-------------------------------------------------------+
+blk     +block<(res:auto(TT) const#):void> const                +
+--------+-------------------------------------------------------+


reads input from the channel (in order it was pushed) and invokes the block on each input.
stops once channel is depleted (internal entry counter is 0)
this can happen on multiple threads or jobs at the same time.

.. _function-_at_jobque_boost_c__c_each_C1_ls_H_ls_jobque_c__c_Channel_gr__gr_?_CY_ls_TT_gr_.:

.. das:function:: each(channel: jobque::Channel? const; tinfo: auto(TT) const)

each returns auto

+--------+-------------------------------------------------------+
+argument+argument type                                          +
+========+=======================================================+
+channel + :ref:`jobque::Channel <handle-jobque-Channel>` ? const+
+--------+-------------------------------------------------------+
+tinfo   +auto(TT) const                                         +
+--------+-------------------------------------------------------+


this iterator is used to iterate over the channel in order it was pushed.
iterator stops once channel is depleted (internal entry counter is 0)
iteration can happen on multiple threads or jobs at the same time.

++++++++++++
Passing data
++++++++++++

  *  :ref:`push_clone (channel:jobque::Channel? const;data:auto(TT) const) : auto <function-_at_jobque_boost_c__c_push_clone_C1_ls_H_ls_jobque_c__c_Channel_gr__gr_?_CY_ls_TT_gr_.>` 
  *  :ref:`push (channel:jobque::Channel? const;data:auto? const) : auto <function-_at_jobque_boost_c__c_push_C1_ls_H_ls_jobque_c__c_Channel_gr__gr_?_C1_ls_._gr_?>` 

.. _function-_at_jobque_boost_c__c_push_clone_C1_ls_H_ls_jobque_c__c_Channel_gr__gr_?_CY_ls_TT_gr_.:

.. das:function:: push_clone(channel: jobque::Channel? const; data: auto(TT) const)

push_clone returns auto

+--------+-------------------------------------------------------+
+argument+argument type                                          +
+========+=======================================================+
+channel + :ref:`jobque::Channel <handle-jobque-Channel>` ? const+
+--------+-------------------------------------------------------+
+data    +auto(TT) const                                         +
+--------+-------------------------------------------------------+


clones data and pushed value to the channel (at the end)

.. _function-_at_jobque_boost_c__c_push_C1_ls_H_ls_jobque_c__c_Channel_gr__gr_?_C1_ls_._gr_?:

.. das:function:: push(channel: jobque::Channel? const; data: auto? const)

push returns auto

+--------+-------------------------------------------------------+
+argument+argument type                                          +
+========+=======================================================+
+channel + :ref:`jobque::Channel <handle-jobque-Channel>` ? const+
+--------+-------------------------------------------------------+
+data    +auto? const                                            +
+--------+-------------------------------------------------------+


pushes value to the channel (at the end)


