
.. _stdlib_ast_block_to_loop:

=======================
DECS, AST block to loop
=======================

.. das:module:: ast_block_to_loop

The AST_BLOCK_TO_LOOP module provides an AST transformation macro that
converts block-based iteration patterns into explicit loop constructs.
Used internally by other macro libraries for optimization.

All functions and symbols are in "ast_block_to_loop" module, use require to get access to it. ::

    require daslib/ast_block_to_loop



++++++++++++++++++++++++
Block to loop conversion
++++++++++++++++++++++++

  *  :ref:`convert_block_to_loop (var blk: smart_ptr\<Expression\>; failOnReturn: bool; replaceReturnWithContinue: bool; requireContinueCond: bool) <function-ast_block_to_loop_convert_block_to_loop_smart_ptr_ls_Expression_gr__bool_bool_bool>`

.. _function-ast_block_to_loop_convert_block_to_loop_smart_ptr_ls_Expression_gr__bool_bool_bool:

.. das:function:: convert_block_to_loop(blk: smart_ptr<Expression>; failOnReturn: bool; replaceReturnWithContinue: bool; requireContinueCond: bool)

Converts closure block to loop.
If `failOnReturn` is true, then returns are not allowed inside the block.
If `replaceReturnWithContinue` is true, then `return cond;` are replaced with `if cond; continue;`.
If `requireContinueCond` is false, then `return;` is replaced with `continue;`, otherwise it is an error.


:Arguments: * **blk** : smart_ptr< :ref:`Expression <handle-ast-Expression>`>

            * **failOnReturn** : bool

            * **replaceReturnWithContinue** : bool

            * **requireContinueCond** : bool


