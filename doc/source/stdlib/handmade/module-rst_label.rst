The RST_LABEL module spells the RST label of a function and the short type names that
label hashes. It is the leaf both ``rst`` and the ``rst_comment`` reader share: the reader is
a parser macro, and every module parsed before it registers loses its ``//!`` comments, so this
module requires only builtin modules and its reference pages are written by hand.

All functions and symbols are in "rst_label" module, use require to get access to it.

.. code-block:: das

    require daslib/rst_label
