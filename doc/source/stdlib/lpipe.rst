
.. _stdlib_lpipe:

===========
lpipe macro
===========

.. das:module:: lpipe

The LPIPE module provides the ``lpipe`` macro for passing multiple block
arguments to a single function call. While ``<|`` handles the first block
argument, ``lpipe`` adds subsequent blocks on following lines.

All functions and symbols are in "lpipe" module, use require to get access to it.

.. code-block:: das

    require daslib/lpipe

Example:

.. code-block:: das

    require daslib/lpipe

        def take2(a, b : block) {
            invoke(a)
            invoke(b)
        }

        [export]
        def main() {
            take2() {
                print("first\n")
            }
            lpipe() {
                print("second\n")
            }
        }
        // output:
        // first
        // second



+++++++++++
Call macros
+++++++++++

.. _call-macro-lpipe-lpipe:

.. das:attribute:: lpipe

This macro will implement the lpipe function. It allows piping blocks the previous line call. For example::

    def take2(a,b:block)
        invoke(a)
        invoke(b)
    ...
    take2 <|
        print("block1\n")
    lpipe <|    // this block will pipe into take2
        print("block2\n")



