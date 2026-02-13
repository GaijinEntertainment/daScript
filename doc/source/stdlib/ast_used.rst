
.. _stdlib_ast_used:

=========================
AST type usage collection
=========================

The AST_USED module implements analysis passes that determine which AST nodes
are actually used in the program. This information is used for dead code
elimination, tree shaking, and optimizing generated output.

All functions and symbols are in "ast_used" module, use require to get access to it. ::

    require daslib/ast_used

++++++++++
Structures
++++++++++

.. _struct-ast_used-OnlyUsedTypes:

.. das:attribute:: OnlyUsedTypes

Collection of all structure and enumeration types that are used in the AST.

:Fields: * **st** : table< :ref:`Structure <handle-ast-Structure>` ?;void> - all structure types used

         * **en** : table< :ref:`Enumeration <handle-ast-Enumeration>` ?;void> - all enumeration types used


+++++++++++++++++++++++++++
Collecting type information
+++++++++++++++++++++++++++

  *  :ref:`collect_used_types (vfun: array\<Function?\>; vvar: array\<Variable?\>; blk: block\<(usedTypes:OnlyUsedTypes):void\>) <function-ast_used_collect_used_types_array_ls_Function_q__gr__array_ls_Variable_q__gr__block_ls_usedTypes_c_OnlyUsedTypes_c_void_gr_>` 

.. _function-ast_used_collect_used_types_array_ls_Function_q__gr__array_ls_Variable_q__gr__block_ls_usedTypes_c_OnlyUsedTypes_c_void_gr_:

.. das:function:: collect_used_types(vfun: array<Function?>; vvar: array<Variable?>; blk: block<(usedTypes:OnlyUsedTypes):void>)

Goes through list of functions `vfun` and variables `vvar` and collects list of which enumeration and structure types are used in them.
Calls `blk` with said list.

:Arguments: * **vfun** : array< :ref:`Function <handle-ast-Function>` ?>

            * **vvar** : array< :ref:`Variable <handle-ast-Variable>` ?>

            * **blk** : block<(usedTypes: :ref:`OnlyUsedTypes <struct-ast_used-OnlyUsedTypes>` ):void>


