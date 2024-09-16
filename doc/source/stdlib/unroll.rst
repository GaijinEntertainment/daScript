
.. _stdlib_unroll:

==============
Loop unrolling
==============

The unroll module implements loop unrolling infrastructure.

All functions and symbols are in "unroll" module, use require to get access to it. ::

    require daslib/unroll

++++++++++++++++++++
Function annotations
++++++++++++++++++++

.. _handle-unroll-UnrollMacro:

.. das:attribute:: UnrollMacro

|detail/function_annotation-unroll-UnrollMacro|

+++++++++
Unrolling
+++++++++

  *  :ref:`unroll (blk: block\<\>) <function-_at_unroll_c__c_unroll_C_builtin_>` 

.. _function-_at_unroll_c__c_unroll_C_builtin_:

.. das:function:: unroll(blk: block<>)

Unrolls the for loop (with fixed range)


:Arguments: * **blk** : block<void>


