
.. _stdlib_apply:

========================
Apply reflection pattern
========================

.. das:module:: apply

The APPLY module provides the ``apply`` macro for iterating over struct, tuple,
and variant fields at compile time. Each field is visited with its name and
a reference to its value, enabling generic per-field operations like
serialization, printing, and validation.

All functions and symbols are in "apply" module, use require to get access to it.

.. code-block:: das

    require daslib/apply

Example:

.. code-block:: das

    require daslib/apply

        struct Foo {
            a : int
            b : float
            c : string
        }

        [export]
        def main() {
            var foo = Foo(a = 42, b = 3.14, c = "hello")
            apply(foo) $(name, field) {
                print("{name} = {field}\n")
            }
        }
        // output:
        // a = 42
        // b = 3.14
        // c = hello

.. seealso::

   :ref:`tutorial_apply` — comprehensive tutorial covering structs,
   tuples, variants, ``static_if`` dispatch, mutation, generic
   ``describe``, and the 3-argument annotation form.



+++++++++++
Call macros
+++++++++++

.. _call-macro-apply-apply:

.. das:attribute:: apply

This macro implements the ``apply()`` pattern. For each field in a structure, variant, or tuple,
the block is invoked with the field name and value. An optional third block argument receives
per-field annotations as ``array<tuple<name:string; data:RttiValue>>``.

.. code-block:: das

    struct Bar {
        x, y : float
    }
    apply(Bar(x=1.0, y=2.0)) $(name, field) {
        print("{name} = {field} ")
    }

Would print ``x = 1 y = 2``



