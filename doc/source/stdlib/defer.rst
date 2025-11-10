
.. _stdlib_defer:

=============================
defer and defer_delete macros
=============================

Apply module implements `defer` and `defer_delete` pattern, i.e. ability to attach a bit of code or a delete operation to a finally section of the block, without leaving the context of the code.

All functions and symbols are in "defer" module, use require to get access to it. ::

    require daslib/defer

++++++++++++++++++++
Function annotations
++++++++++++++++++++

.. _handle-defer-DeferMacro:

.. das:attribute:: DeferMacro

This macro covnerts defer() <| block expression
into {}, and move block to the finally section of the current block

+++++++++++
Call macros
+++++++++++

.. _call-macro-defer-defer_delete:

.. das:attribute:: defer_delete

This macro converts defer_delete() expression
into {}, and add delete expression to the finally section of the current block

+++++
Defer
+++++

  *  :ref:`defer (blk: block\<():void\>) <function-defer_defer_block_ls__c_void_gr_>` 

.. _function-defer_defer_block_ls__c_void_gr_:

.. das:function:: defer(blk: block<():void>)

defer a block of code. For example::

    var a = fopen("filename.txt","r")
    defer <|
        fclose(a)

Will close the file when 'a' is out of scope.

:Arguments: * **blk** : block<void>

++++
Stub
++++

  *  :ref:`nada () <function-defer_nada>` 

.. _function-defer_nada:

.. das:function:: nada()

helper function which does nothing and will be optimized out


