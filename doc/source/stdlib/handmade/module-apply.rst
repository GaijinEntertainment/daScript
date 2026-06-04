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

When the block has no function-escaping ``return``, it runs inline once per field — no helper function
and no per-field block invoke — so it is cheap enough for hot paths like serialization. A block that
uses ``return`` to skip a field falls back transparently to a generated per-field helper, where
``return`` keeps its original block-local "skip this field" meaning; nothing the caller writes changes.
``apply_imm`` is a struct-only, slightly faster variant (it aliases the block params rather than binding
reference locals); use it for hot *struct* field walks and ``apply`` for tuples, variants, a
side-effecting source value, or a block that uses ``return`` (``apply_imm`` always inlines, so it
cannot accept one).

.. seealso::

   :ref:`tutorial_apply` — comprehensive tutorial covering structs,
   tuples, variants, ``static_if`` dispatch, mutation, generic
   ``describe``, and the 3-argument annotation form.
