
.. _stdlib_apply:

========================
Apply reflection pattern
========================

.. das:module:: apply

The APPLY module provides the ``apply`` macro for iterating over struct, tuple,
and variant fields at compile time. Each field is visited with its name and
a reference to its value, enabling generic per-field operations like
serialization, printing, and validation.

All functions and symbols are in "apply" module, use require to get access to it. ::

    require daslib/apply

Example: ::

    require daslib/apply

        struct Foo {
            a : int
            b : float
            c : string
        }

        [export]
        def main() {
            var foo = Foo(a = 42, b = 3.14, c = "hello")
            apply(foo) <| $(name, field) {
                print("{name} = {field}\n")
            }
        }
        // output:
        // a = 42
        // b = 3.14
        // c = hello



+++++++++++
Call macros
+++++++++++

.. _call-macro-apply-apply:

.. das:attribute:: apply

This macro implements the apply() pattern. The idea is that for each entry in the structure, variant, or tuple,
the block will be invoked. Both element name, and element value are passed to the block.
For example

    struct Bar
        x, y : float
    apply([[Bar x=1.,y=2.]]) <| $ ( name:string; field )
        print("{name} = {field} ")

Would print x = 1.0 y = 2.0



