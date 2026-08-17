.. _utils_fix_lint_errors:

.. index::
   single: Utils; fix-lint-errors
   single: Utils; Lint auto-fixer

=======================================
 fix-lint-errors --- Lint Auto-Fixer
=======================================

fix-lint-errors rewrites source to resolve mechanical lint findings.  It
walks each input file's AST, collects edits for the fixable rules, and
writes them back — then compile-verifies the result and auto-reverts a
file whose rewrite broke it.

Fixable rules: STYLE005 (``if (c) { return }`` → ``if (c) return``),
STYLE017 (boolean return ladders), STYLE018 (``b == true`` shapes),
PERF013 (``a += 1`` → ``a++``), PERF015/PERF016 (ternary min/max/abs),
PERF017 (``length(x) == 0`` → ``empty(x)``).  Rules that need
cross-statement restructuring are out of scope — fix those by hand from
the lint report.

Quick start
===========

::

   daslang utils/fix-lint-errors/main.das -- <files...>

Flags: ``--dry-run`` (print edits, write nothing), ``-q`` (suppress
per-file lines), ``--help`` for the rest.

.. seealso::

   :ref:`utils_lint` -- the lint runner that reports the findings
