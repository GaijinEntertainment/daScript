
.. _stdlib_macro_boost:

=====================================
Boost package for macro manipulations
=====================================

The MACRO_BOOST module provides utility macros for macro authors, including
pattern matching on AST nodes, code generation helpers, and common
transformation patterns used when writing compile-time code.

All functions and symbols are in "macro_boost" module, use require to get access to it. ::

    require daslib/macro_boost

++++++++++
Structures
++++++++++

.. _struct-macro_boost-CapturedVariable:

.. das:attribute:: CapturedVariable

Stored captured variable together with the `ExprVar` which uses it

:Fields: * **variable** :  :ref:`Variable <handle-ast-Variable>`? - captured variable

         * **expression** :  :ref:`ExprVar <handle-ast-ExprVar>`? - expression which uses the variable

         * **eref** : bool - this one indicates if its used by reference and does not come from argument. its only used in JIT


++++++++++++++++++++
Function annotations
++++++++++++++++++++

.. _handle-macro_boost-MacroVerifyMacro:

.. das:attribute:: MacroVerifyMacro

This macro convert macro_verify(expr,message,prog,at) to the following code::
   if !expr
       macro_error(prog,at,message)
       return [[ExpressionPtr]]

+++++++++++
Call macros
+++++++++++

.. _call-macro-macro_boost-return_skip_lockcheck:

.. das:attribute:: return_skip_lockcheck

this is similar to regular return <-, but it does not check for locks

++++++++++++++++++++++
Implementation details
++++++++++++++++++++++

  *  :ref:`macro_verify (expr: bool; prog: ProgramPtr; at: LineInfo; message: string) <function-macro_boost_macro_verify_bool_ProgramPtr_LineInfo_string>`

.. _function-macro_boost_macro_verify_bool_ProgramPtr_LineInfo_string:

.. das:function:: macro_verify(expr: bool; prog: ProgramPtr; at: LineInfo; message: string)

Same as verify, only the check will produce macro error, followed by return [[ExpressionPtr]]

:Arguments: * **expr** : bool

            * **prog** :  :ref:`ProgramPtr <alias-ProgramPtr>`

            * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>`

            * **message** : string

++++++++++++++
Block analysis
++++++++++++++

  *  :ref:`capture_block (expr: ExpressionPtr) : array\<CapturedVariable\> <function-macro_boost_capture_block_ExpressionPtr>`
  *  :ref:`collect_finally (expr: ExpressionPtr; alwaysFor: bool = false) : array\<ExprBlock?\> <function-macro_boost_collect_finally_ExpressionPtr_bool>`
  *  :ref:`collect_labels (expr: ExpressionPtr) : array\<int\> <function-macro_boost_collect_labels_ExpressionPtr>`

.. _function-macro_boost_capture_block_ExpressionPtr:

.. das:function:: capture_block(expr: ExpressionPtr) : array<CapturedVariable>

Collect all captured variables in the expression.

:Arguments: * **expr** :  :ref:`ExpressionPtr <alias-ExpressionPtr>`

.. _function-macro_boost_collect_finally_ExpressionPtr_bool:

.. das:function:: collect_finally(expr: ExpressionPtr; alwaysFor: bool = false) : array<ExprBlock?>

Collect all finally blocks in the expression.
Returns array of ExprBlock? with all the blocks which have `finally` section
Does not go into 'make_block' expression, such as `lambda`, or 'block' expressions

:Arguments: * **expr** :  :ref:`ExpressionPtr <alias-ExpressionPtr>`

            * **alwaysFor** : bool

.. _function-macro_boost_collect_labels_ExpressionPtr:

.. das:function:: collect_labels(expr: ExpressionPtr) : array<int>

Collect all labels in the expression. Returns array of integer with label indices
Does not go into 'make_block' expression, such as `lambda`, or 'block' expressions

:Arguments: * **expr** :  :ref:`ExpressionPtr <alias-ExpressionPtr>`


