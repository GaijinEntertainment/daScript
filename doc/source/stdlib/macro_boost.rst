
.. _stdlib_macro_boost:

======================================================
Boost package for the miscelanious macro manipulations
======================================================

Apply module implements miscellaneous infrastructure which simplifies writing of macros.

All functions and symbols are in "macro_boost" module, use require to get access to it. ::

    require daslib/macro_boost



++++++++++
Structures
++++++++++

.. _struct-macro_boost-CapturedVariable:

.. das:attribute:: CapturedVariable

Stored captured variable together with the `ExprVar` which uses it
this one indicates if its used by reference and does not come from argument. its only used in JIT

:Fields: * **variable** :  :ref:`Variable <handle-ast-Variable>` ?

         * **expression** :  :ref:`ExprVar <handle-ast-ExprVar>` ?

         * **eref** : bool

Stored captured variable together with the `ExprVar` which uses it
this one indicates if its used by reference and does not come from argument. its only used in JIT

++++++++++++++++++++
Function annotations
++++++++++++++++++++

.. _handle-macro_boost-MacroVerifyMacro:

.. das:attribute:: MacroVerifyMacro

|detail/function_annotation-macro_boost-MacroVerifyMacro|

+++++++++++
Call macros
+++++++++++

.. _call-macro-macro_boost-return_skip_lockcheck:

.. das:attribute:: return_skip_lockcheck

this is similar to regular return <-, but it does not check for locks

++++++++++++++++++++++
Implementation details
++++++++++++++++++++++

  *  :ref:`macro_verify (expr: bool; prog: ProgramPtr; at: LineInfo; message: string) <function-_at_macro_boost_c__c_macro_verify_Cb_CY_ls_ProgramPtr_gr_1_ls_H_ls_rtti_c__c_Program_gr__gr__qm_M_CH_ls_rtti_c__c_LineInfo_gr__Cs>` 

.. _function-_at_macro_boost_c__c_macro_verify_Cb_CY_ls_ProgramPtr_gr_1_ls_H_ls_rtti_c__c_Program_gr__gr__qm_M_CH_ls_rtti_c__c_LineInfo_gr__Cs:

.. das:function:: macro_verify(expr: bool; prog: ProgramPtr; at: LineInfo; message: string)

Same as verify, only the check will produce macro error, followed by return [[ExpressionPtr]]


:Arguments: * **expr** : bool

            * **prog** :  :ref:`ProgramPtr <alias-ProgramPtr>` 

            * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

            * **message** : string

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_macro_boost_c__c_capture_block_CY_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M:

.. das:function:: capture_block(expr: ExpressionPtr) : array<CapturedVariable>

Collect all captured variables in the expression.


:Arguments: * **expr** :  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

.. _function-_at_macro_boost_c__c_collect_finally_CY_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M_Cb:

.. das:function:: collect_finally(expr: ExpressionPtr; alwaysFor: bool = false) : array<ExprBlock?>

Collect all finally blocks in the expression.
Returns array of ExprBlock? with all the blocks which have `finally` section
Does not go into 'make_block' expression, such as `lambda`, or 'block' expressions


:Arguments: * **expr** :  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

            * **alwaysFor** : bool

.. _function-_at_macro_boost_c__c_collect_labels_CY_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M:

.. das:function:: collect_labels(expr: ExpressionPtr) : array<int>

Collect all labels in the expression. Returns array of integer with label indices
Does not go into 'make_block' expression, such as `lambda`, or 'block' expressions


:Arguments: * **expr** :  :ref:`ExpressionPtr <alias-ExpressionPtr>` 


