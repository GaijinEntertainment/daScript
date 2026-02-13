
.. _stdlib_assert_once:

===========
Assert once
===========

The ASSERT_ONCE module provides the ``assert_once`` macro — an assertion that
triggers only on its first failure. Subsequent failures at the same location
are silently ignored, preventing assertion storms in loops or frequently
called code.

All functions and symbols are in "assert_once" module, use require to get access to it. ::

    require daslib/assert_once

++++++++++++++++++++
Function annotations
++++++++++++++++++++

.. _handle-assert_once-AssertOnceMacro:

.. das:attribute:: AssertOnceMacro

This macro convert assert_once(expr,message) to the following code::

    var __assert_once_I = true  // this is a global variable
    if __assert_once_I && !expr
        __assert_once_I = false
        assert(false,message)

+++++++++
Assertion
+++++++++

  *  :ref:`assert_once (expr: bool; message: string = "") <function-assert_once_assert_once_bool_string>` 

.. _function-assert_once_assert_once_bool_string:

.. das:function:: assert_once(expr: bool; message: string = "")

Same as assert, only the check will be not be repeated after the asseretion failed the first time.

:Arguments: * **expr** : bool

            * **message** : string


