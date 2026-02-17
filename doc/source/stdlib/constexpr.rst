
.. _stdlib_constant_expression:

============================================
Constant expression checker and substitution
============================================

.. das:module:: constant_expression

The CONSTANT_EXPRESSION module provides the ``[constant_expression]`` function
annotation. Functions marked with this annotation are evaluated at compile
time when all arguments are constants, replacing the call with the computed
result.

All functions and symbols are in "constant_expression" module, use require to get access to it. ::

    require daslib/constant_expression



++++++++++++++++++++
Function annotations
++++++++++++++++++++

.. _handle-constant_expression-constexpr:

.. das:attribute:: constexpr

This macro implements a constexpr function argument checker. Given list of arguments to verify, it will fail for every one where non-constant expression is passed. For example::

    [constexpr (a)]
    def foo ( t:string; a : int )
        print("{t} = {a}\n")
    var BOO = 13
    [export]
    def main
        foo("blah", 1)
        foo("ouch", BOO)    // compilation error: `a is not a constexpr, BOO`


.. _handle-constant_expression-constant_expression:

.. das:attribute:: constant_expression

This function annotation implements constant expression folding for the given arguments.
When argument is specified in the annotation, and is passed as a constant expression,
custom version of the function is generated, and an argument is substituted with a constant value.
This allows using of static_if expression on the said arguments, as well as other optimizations.
For example::

    [constant_expression(constString)]
    def take_const_arg(constString:string)
        print("constant string is = {constString}\n")   // note - constString here is not an argument




+++++++++++++
Macro helpers
+++++++++++++

  *  :ref:`isConstantExpression (expr: ExpressionPtr) : bool <function-constant_expression_isConstantExpression_ExpressionPtr>`

.. _function-constant_expression_isConstantExpression_ExpressionPtr:

.. das:function:: isConstantExpression(expr: ExpressionPtr) : bool

This macro function returns true if the expression is a constant expression


:Arguments: * **expr** :  :ref:`ExpressionPtr <alias-ExpressionPtr>`


