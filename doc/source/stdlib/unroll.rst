
.. _stdlib_unroll:

==============
Loop unrolling
==============

.. das:module:: unroll

The UNROLL module implements compile-time loop unrolling. The ``unroll``
macro replaces a ``for`` loop with a constant ``range`` bound by stamping
out each iteration as separate inlined code, eliminating loop overhead.

All functions and symbols are in "unroll" module, use require to get access to it.

.. code-block:: das

    require daslib/unroll

Example:

.. code-block:: das

    require daslib/unroll

        [export]
        def main() {
            unroll <| $() {
                for (i in range(4)) {
                    print("step {i}\n")
                }
            }
        }
        // output:
        // step 0
        // step 1
        // step 2
        // step 3



++++++++++++++++++++
Function annotations
++++++++++++++++++++

.. _handle-unroll-UnrollMacro:

.. das:attribute:: UnrollMacro

This macro implements loop unrolling in the form of `unroll` function.
Unroll function expects block with the single for loop in it.
Moveover only range for is supported, and only with the fixed range.
For example:::

    var n : float4[9]
    unroll <|   // contents of the loop will be replaced with 9 image load instructions.
        for i in range(9)
            n[i] = imageLoad(c_bloom_htex, xy + int2(0,i-4))



+++++++++
Unrolling
+++++++++

  *  :ref:`unroll (blk: block\<():void\>) <function-unroll_unroll_block_ls__c_void_gr_>`

.. _function-unroll_unroll_block_ls__c_void_gr_:

.. das:function:: unroll(blk: block<():void>)

Unrolls the for loop (with fixed range)


:Arguments: * **blk** : block<void>


