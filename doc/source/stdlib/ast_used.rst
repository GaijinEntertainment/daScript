
.. _stdlib_ast_used:

==========================
AST type ussage collection
==========================

The ast_used module implements type collecting infrasturcture. It allows to determine, if enumeration and structure types are used in the code.

All functions and symbols are in "ast_used" module, use require to get access to it. ::

    require daslib/ast_used

++++++++++
Structures
++++++++++

.. _struct-ast_used-OnlyUsedTypes:

.. das:attribute:: OnlyUsedTypes

Collection of all structure and enumeration types that are used in the AST.

:Fields: * **st** : table< :ref:`Structure <handle-ast-Structure>` ?;bool>

         * **en** : table< :ref:`Enumeration <handle-ast-Enumeration>` ?;bool>

Collection of all structure and enumeration types that are used in the AST.

+++++++++++++++++++++++++++
Collecting type information
+++++++++++++++++++++++++++

  *  :ref:`collect_used_types (vfun: array\<Function?\>; vvar: array\<Variable?\>; blk: block\<(usedTypes:OnlyUsedTypes const):void\>) <function-_at_ast_used_c__c_collect_used_types_C1_ls_1_ls_H_ls_ast_c__c_Function_gr__gr__qm__gr_A_C1_ls_1_ls_H_ls_ast_c__c_Variable_gr__gr__qm__gr_A_CN_ls_usedTypes_gr_0_ls_CS_ls_ast_used_c__c_OnlyUsedTypes_gr__gr_1_ls_v_gr__builtin_>` 

.. _function-_at_ast_used_c__c_collect_used_types_C1_ls_1_ls_H_ls_ast_c__c_Function_gr__gr__qm__gr_A_C1_ls_1_ls_H_ls_ast_c__c_Variable_gr__gr__qm__gr_A_CN_ls_usedTypes_gr_0_ls_CS_ls_ast_used_c__c_OnlyUsedTypes_gr__gr_1_ls_v_gr__builtin_:

.. das:function:: collect_used_types(vfun: array<Function?>; vvar: array<Variable?>; blk: block<(usedTypes:OnlyUsedTypes const):void>)

Goes through list of functions `vfun` and variables `vvar` and collects list of which enumeration and structure types are used in them.
Calls `blk` with said list.

:Arguments: * **vfun** : array< :ref:`Function <handle-ast-Function>` ?>

            * **vvar** : array< :ref:`Variable <handle-ast-Variable>` ?>

            * **blk** : block<(usedTypes: :ref:`OnlyUsedTypes <struct-ast_used-OnlyUsedTypes>` ):void>


