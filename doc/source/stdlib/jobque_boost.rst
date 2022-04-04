
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

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_jobque_boost_c__c_new_job__at_:

.. das:function:: new_job(l: lambda<>)

+--------+-------------+
+argument+argument type+
+========+=============+
+l       +lambda<>     +
+--------+-------------+


|function-jobque_boost-new_job|

.. _function-_at_jobque_boost_c__c_new_thread__at_:

.. das:function:: new_thread(l: lambda<>)

+--------+-------------+
+argument+argument type+
+========+=============+
+l       +lambda<>     +
+--------+-------------+


|function-jobque_boost-new_thread|

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


|function-jobque_boost-for_each|

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


|function-jobque_boost-push_clone|

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


|function-jobque_boost-push|

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


|function-jobque_boost-each|


