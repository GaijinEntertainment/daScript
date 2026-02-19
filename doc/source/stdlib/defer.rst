
.. _stdlib_defer:

=============================
defer and defer_delete macros
=============================

.. das:module:: defer

The DEFER module implements the ``defer`` pattern — the ability to schedule cleanup
code to run at scope exit, similar to Go's ``defer``. The deferred block is moved
to the ``finally`` section of the enclosing scope at compile time.

All functions and symbols are in "defer" module, use require to get access to it.

.. code-block:: das

    require daslib/defer

Example:

.. code-block:: das

    require daslib/defer

        [export]
        def main() {
            print("start\n")
            defer() <| $() {
                print("cleanup runs last\n")
            }
            print("middle\n")
        }
        // output:
        // start
        // middle
        // cleanup runs last



++++++++++++++++++++
Function annotations
++++++++++++++++++++

.. _handle-defer-DeferMacro:

.. das:attribute:: DeferMacro

This macro converts defer() <| block expression
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



