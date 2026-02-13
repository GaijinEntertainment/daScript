
.. _stdlib_validate_code:

===========================
Code validation annotations
===========================

The VALIDATE_CODE module implements AST validation passes that check for
common code quality issues, unreachable code, missing return statements,
and other semantic errors beyond what the type checker verifies.

All functions and symbols are in "validate_code" module, use require to get access to it. ::

    require daslib/validate_code

++++++++++++++++++++
Function annotations
++++++++++++++++++++

.. _handle-validate_code-verify_completion:

.. das:attribute:: verify_completion

|detail/function_annotation-validate_code-verify_completion|


