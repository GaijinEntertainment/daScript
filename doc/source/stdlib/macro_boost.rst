
.. _stdlib_macro_boost:

======================================================
Boost package for the miscelanious macro manipulations
======================================================

.. include:: detail/macro_boost.rst

Apply module implements miscellaneous infrastructure which simplifies writing of macros.

All functions and symbols are in "macro_boost" module, use require to get access to it. ::

    require daslib/macro_boost



++++++++++++++++++++
Function annotations
++++++++++++++++++++

.. _handle-macro_boost-MacroVerifyMacro:

.. das:attribute:: MacroVerifyMacro

This macro implements `macro_verify` macro. It's equivalent to a function call::

    def macro_verify ( expr:bool; prog:ProgramPtr; at:LineInfo; message:string )

However, result will be substituted with::

    if !expr
        macro_error( prog, at, message )
        return [[ExpressionPtr]]
++++++++++++++++++++++
Implementation details
++++++++++++++++++++++

  *  :ref:`macro_verify (expr:bool const;prog:smart_ptr\<rtti::Program\> const;at:rtti::LineInfo const;message:string const) : void <function-_at_macro_boost_c__c_macro_verify_Cb_CY_ls_ProgramPtr_gr_1_ls_H_ls_rtti_c__c_Program_gr__gr_?M_CH_ls_rtti_c__c_LineInfo_gr__Cs>` 

.. _function-_at_macro_boost_c__c_macro_verify_Cb_CY_ls_ProgramPtr_gr_1_ls_H_ls_rtti_c__c_Program_gr__gr_?M_CH_ls_rtti_c__c_LineInfo_gr__Cs:

.. das:function:: macro_verify(expr: bool const; prog: ProgramPtr; at: LineInfo const; message: string const)

+--------+----------------------------------------------------+
+argument+argument type                                       +
+========+====================================================+
+expr    +bool const                                          +
+--------+----------------------------------------------------+
+prog    + :ref:`ProgramPtr <alias-ProgramPtr>`               +
+--------+----------------------------------------------------+
+at      + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`  const+
+--------+----------------------------------------------------+
+message +string const                                        +
+--------+----------------------------------------------------+


Same as verify, only the check will produce macro error, followed by return [[ExpressionPtr]]


