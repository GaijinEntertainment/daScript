The CMP module holds comparisons: it writes lexicographic orderings from a list of keys. The
``[ordered]`` structure annotation generates the structure's ``operator <``, so
``sort`` and everything else that compares with ``<`` orders it by default; the
``cmp_by`` call macro writes a comparator block for one call, with each key an
expression over ``_``, the element, and a leading ``-`` for descending order.

All functions and symbols are in "cmp" module, use require to get access to it.

.. code-block:: das

    require daslib/cmp
