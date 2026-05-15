.. _utils_lint:

.. index::
   single: Utils; lint
   single: Utils; Linting
   single: Utils; perf_lint
   single: Utils; style_lint

==========================================
 lint --- Paranoid, Performance, and Style
==========================================

The lint utility runs daslang's three lint passes — **paranoid**
(``daslib/lint``), **performance** (``daslib/perf_lint``), and **style**
(``daslib/style_lint``) — over one or more files and reports every
issue. It is the same machinery the MCP ``lint`` tool drives and the
same machinery a future CI gate will use to enforce a warning-free
``daslib/``.

.. contents::
   :local:
   :depth: 2


Quick start
===========

Lint a single file::

   daslang utils/lint/main.das -- path/to/script.das

Lint a directory tree (every reachable ``.das`` file)::

   daslang utils/lint/main.das -- path/to/dir

Lint the whole stdlib::

   daslang utils/lint/main.das -- daslib

The runner skips files that declare ``expect <code>:<count>`` directives
(used by dastest to assert intentional compile errors) since the lint
passes need a successfully-compiled AST. Their rule coverage comes from
dedicated tests under ``utils/lint/tests/``.

Flags
=====

- ``-q`` / ``--quiet`` --- suppress per-file ``PASS`` lines and
  progress messages; only ``WARN`` and the summary are printed.
- ``--comment-hygiene`` --- enable the opt-in comment-length checks
  ``STYLE014`` (multi-line ``//`` / ``//!`` blocks at module/public
  scope) and ``STYLE015`` (multi-line comments inside ``def private``).
  Off by default.
- ``--paranoid-only`` --- run only the paranoid pass.
- ``--perf-only`` --- run only the performance pass.
- ``--style-only`` --- run only the style pass.
- ``-?`` / ``--help`` --- show usage and exit.

Compile policies
================

The runner compiles each file with a ``CodeOfPolicies`` tuned for lint:

- ``lint_check = true`` --- marks the program as lint-only, skipping
  some passes that would normally enforce production-only invariants.
- ``no_optimizations = true`` --- disables the optimizer.
- ``no_infer_time_folding = true`` --- disables infer-time constant
  folding.

These flags matter for rules that inspect the source-level shape of an
expression. ``PERF019`` (``int(T.a) | int(T.b)`` cast collapse),
``STYLE022`` (bitfield ``|= BfT.m`` → field assignment), and
``STYLE023`` (bitfield ``int_cast(bf & BfT.m) != 0`` → ``bf.m``) all
depend on the un-folded AST being available; without these policies the
optimizer collapses the very patterns they look for.

Rule list
=========

The full rule catalogue lives in :ref:`lint`. Each rule has its own
section there with bad/good examples and detection notes.

Suppression
===========

Add ``// nolint:RULExxx`` to the line that fires the warning::

   let ch = character_at(s, idx) // nolint:PERF003 — single indexed access

The suppression is exact: ``// nolint:PERF003`` only suppresses
``PERF003``. Stack multiple rules on the same line if needed:
``// nolint:PERF003,STYLE001``.

Planned CI gate
===============

Once ``daslib/`` and the bundled modules under ``modules/`` are
warning-free, a CI workflow will gate every PR on::

   daslang utils/lint/main.das -- daslib modules/dasSQLITE/daslib

Existing rule exceptions are recorded via ``// nolint:`` annotations in
the source itself; the gate will not maintain an external allow-list.
