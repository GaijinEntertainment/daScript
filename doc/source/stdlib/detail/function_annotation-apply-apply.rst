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
