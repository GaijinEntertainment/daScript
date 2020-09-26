.. _builtin_functions:


==================
Built-in Functions
==================

.. index::
    single: Built-in Functions
    pair: Global Symbols; Built-in Functions

Builtin functions are function-like expressions, which are available without any modules.
They implement inherit mechanisms of the language, in available in ast as separate expressions.
They are different from standard functions (see :ref:`built-in functions <stdlib__builtin>`).

^^^^^^
Invoke
^^^^^^

.. das:function:: invoke(block_or_function, arguments)

    invoke will call block, lambda, or pointer to function (`block_or_function`) with provided list of arguments

(see :ref:`Functions <functions>`, :ref:`Blocks <blocks>`, :ref:`Lambdas <lambdas>`).

^^^^^^^^^^^^^^
Misc
^^^^^^^^^^^^^^

.. das:function:: assert(x, str)

    assert will cause application defined assert if `x` argument is false.
    assert can and probably will be removed from release builds.
    That's why assert will not compile, if `x` argument has side effect (for example, calling function with side effects).

.. das:function:: verify(x, str)

    verify will cause application defined assert if `x` argument is false.
    verify check can be removed from release builds, but execution of `x` argument stays.
    That's why verify, unlike assert can have side effects in evaluating `x`

.. das:function:: static_assert(x, str)

    static_assert will cause compiler to stop compilation if `x` argument is false.
    That's why `x` has to be compile-time known constant.
    static_assert will be removed from compiled program.

.. das:function:: concept_assert(x, str)

    concept_assert is similar to static_assert, however error will be reported one level above the assert.
    that way application can report contract errors.

.. das:function:: debug(x, str)

    debug will print string str and value of x (like print). However, debug also returns value of x, which makes it suitable for debugging expressions::

        let mad = debug(x, "x") * debug(y, "y") + debug(z, "z") // x*y + z





