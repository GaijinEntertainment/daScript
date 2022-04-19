
.. _stdlib_constexpr:

===========================
Constant expression checker
===========================

.. include:: detail/constexpr.rst

The constexpr module implements `constant expression` function argument check.

All functions and symbols are in "constexpr" module, use require to get access to it. ::

    require daslib/constexpr

++++++++++++++++++++
Function annotations
++++++++++++++++++++

.. _handle-constexpr-constexpr:

.. das:attribute:: constexpr

This macro implements a constexpr function argument checker. Given list of arguments to verify, it will fail for every one where non-constant expression is passed. For example::

    [constexpr (a)]
    def foo ( t:string; a : int )
        print("{t} = {a}\n")
    var BOO = 13
    [export]
    def main
        foo("blah", 1)
        foo("ouch", BOO)    // comilation error: `a is not a constexpr, BOO`


