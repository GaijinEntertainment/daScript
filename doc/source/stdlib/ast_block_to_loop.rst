
.. _stdlib_ast_block_to_loop:

=======================
DECS, AST block to loop
=======================

The ast_block_to_loop module implements block to loop conversion as part of the DECS infrastructure.

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

:Arguments: * **blk** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

            * **failOnReturn** : bool

            * **replaceReturnWithContinue** : bool

            * **requireContinueCond** : bool


