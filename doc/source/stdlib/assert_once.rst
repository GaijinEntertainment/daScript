
.. _stdlib_assert_once:

===========
Assert once
===========

The assert_once module implements single-time assertion infrastructure.

All functions and symbols are in "assert_once" module, use require to get access to it. ::

    require daslib/assert_once

++++++++++++++++++++
Function annotations
++++++++++++++++++++

.. _handle-assert_once-AssertOnceMacro:

.. das:attribute:: AssertOnceMacro

|detail/function_annotation-assert_once-AssertOnceMacro|

+++++++++
Assertion
+++++++++

  *  :ref:`assert_once (expr: bool; message: string = "") <function-_at_assert_once_c__c_assert_once_Cb_Cs>` 

.. _function-_at_assert_once_c__c_assert_once_Cb_Cs:

.. das:function:: assert_once(expr: bool; message: string = "")

Same as assert, only the check will be not be repeated after the asseretion failed the first time.

:Arguments: * **expr** : bool

            * **message** : string


