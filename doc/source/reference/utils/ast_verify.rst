.. _utils_ast_verify:

.. index::
   single: Utils; ast-verify
   single: Utils; AST verifier

=====================================
 ast-verify --- AST verifier gate
=====================================

``ast-verify`` runs the compiler's AST verifier over one file or over the files a
branch changed, and turns what it reports into an exit code.

The verifier itself is a compiler flag, ``--ast-verify-batch``, which checks the
finished tree for shapes the compiler later dereferences unguarded --- a null
child, a node with no source location --- and is cheap enough to gate many files.
The flag **prints its findings and exits 0**, so a plain ``daslang
--ast-verify-batch file.das`` passes whatever it found. This tool reads the
output and decides.

.. contents::
   :local:
   :depth: 2


Quick start
===========

Verify one file::

    daslang utils/ast-verify/main.das -- --file path/to/file.das

Verify everything a branch changed against ``origin/master``::

    daslang utils/ast-verify/main.das -- --changed

The exit code is 0 when nothing was reported and 1 otherwise, so the tool drops
straight into a build rule or a pre-push hook.


Options
=======

.. list-table::
   :header-rows: 1
   :widths: 25 75

   * - Option
     - Meaning
   * - ``--file <path>``
     - Verify this one file.
   * - ``--changed``
     - Verify every ``.das`` that ``git diff --diff-filter=AM origin/<base-ref>...HEAD`` reports.
   * - ``--base-ref <ref>``
     - Base the changed set is taken against. Defaults to ``$DAS_CI_BASE_REF``, then ``master``.


What counts as a failure
========================

A finding is not the same thing as a compile error, and a compile error is not a
finding --- many test files assert one. The tool separates them:

.. list-table::
   :header-rows: 1
   :widths: 40 60

   * - In the output
     - Verdict
   * - an ``AST verify`` line
     - **fail**, with up to five distinct lines quoted
   * - ``error[20510]``
     - pass --- the file is inside the verifier's own ``require`` closure and cannot be verified this way
   * - a timeout
     - **fail**
   * - ``CRASH:``
     - **fail**, with the first frames quoted
   * - exit code 128 or above
     - **fail** --- killed with no crash report: memory, an external kill, or a stack overflow off the main thread

Files whose name begins with ``cant_``, ``failed_`` or ``invalid_`` are skipped:
they exist to fail compilation, so there is no finished tree to verify.
