The LINQ_FOLD module provides the ``_fold`` and ``_old_fold`` call macros
that rewrite LINQ pipelines into optimized loop forms — ``_fold`` is the
active fusion macro, ``_old_fold`` is a frozen pre-rewrite baseline kept
for benchmark comparison as ``_fold`` evolves toward splice-mode fusion.

The dispatch infrastructure (``linqCalls`` table, ``fold_*`` helpers,
``fold_linq_default``) is private to this module; only the two macros
are user-facing. ``daslib/linq_boost`` re-exports them via
``require daslib/linq_fold public`` so existing call sites that
``require daslib/linq_boost`` continue to resolve ``_fold(...)`` without
change.

See also :doc:`linq` for the underlying query operations and
:doc:`linq_boost` for the surrounding macro family.

All functions and symbols are in "linq_fold" module, use require to get
access to it.

.. code-block:: das

    require daslib/linq_fold

Example:

.. code-block:: das

    require daslib/linq
    require daslib/linq_boost

    [export]
    def main() {
        let arr = [1, 2, 3, 4, 5]
        let n = _fold(each(arr)._where(_ > 2).count())
        print("{n}\n")
    }
    // output:
    // 3
