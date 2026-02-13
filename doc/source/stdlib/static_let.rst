
.. _stdlib_static_let:

================
static_let macro
================

The STATIC_LET module implements the ``static_let`` pattern — local variables
that persist across function calls, similar to C ``static`` variables. The
variable is initialized once on first call and retains its value in subsequent
invocations.

All functions and symbols are in "static_let" module, use require to get access to it. ::

    require daslib/static_let

Example: ::

    require daslib/static_let

        def counter() : int {
            static_let <| $() {
                var count = 0
            }
            count ++
            return count
        }

        [export]
        def main() {
            print("{counter()}\n")
            print("{counter()}\n")
            print("{counter()}\n")
        }
        // output:
        // 1
        // 2
        // 3

++++++++++++++++++++
Function annotations
++++++++++++++++++++

.. _handle-static_let-StaticLetMacro:

.. das:attribute:: StaticLetMacro

This macro implements the `static_let` and `static_let_finalize` functions.

++++++++++++++++++++++++++++
Static variable declarations
++++++++++++++++++++++++++++

  *  :ref:`static_let (name: string; blk: block\<():void\>) <function-static_let_static_let_string_block_ls__c_void_gr_>` 
  *  :ref:`static_let (blk: block\<():void\>) <function-static_let_static_let_block_ls__c_void_gr_>` 
  *  :ref:`static_let_finalize (blk: block\<():void\>) <function-static_let_static_let_finalize_block_ls__c_void_gr_>` 


static_let
^^^^^^^^^^

.. _function-static_let_static_let_string_block_ls__c_void_gr_:

.. das:function:: static_let(name: string; blk: block<():void>)

Given a scope with the variable declarations, this function will make those variables global.
Variable will be renamed under the hood, and all local access to it will be renamed as well.

:Arguments: * **name** : string

            * **blk** : block<void>

.. _function-static_let_static_let_block_ls__c_void_gr_:

.. das:function:: static_let(blk: block<():void>)

----

.. _function-static_let_static_let_finalize_block_ls__c_void_gr_:

.. das:function:: static_let_finalize(blk: block<():void>)

This is very similar to regular static_let, but additionally the variable will be deleted on the context shutdown.

:Arguments: * **blk** : block<void>


