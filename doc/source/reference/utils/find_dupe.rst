.. _utils_find_dupe:

.. index::
   single: Utils; find_dupe
   single: Utils; AI duplicate judge
   single: Utils; Cluster triage

==========================================================
 find_dupe --- AI judge for detect-dupe clusters
==========================================================

``find_dupe`` (directory: ``utils/find-dupe/``) consumes a
:ref:`detect-dupe <utils_detect_dupe>` JSON report and asks Claude (via
the ``das-claude`` daspkg, module ``anthropic/anthropic``) to
**partition** each cluster into real-duplicate groups vs false
positives, with a one-line reason.  Output is JSON (machine, for CI
gates / future tooling) + Markdown (human, for review).

The tool is an install-then-run script, *not* a built-into-binary
utility --- the ``anthropic/anthropic`` dependency is fetched at
runtime via :ref:`daspkg <utils_daspkg>`, so it can't be linked into
``daslang.exe``'s ``all_utils_exe`` bundle at build time.

.. warning::

   ``find_dupe`` **sends source code to Anthropic's API.** Each
   cluster's full function bodies (verbatim, with file paths and line
   numbers) are uploaded as part of every judge call.  Do not run this
   tool on proprietary, confidential, or otherwise restricted code
   unless your project's data-handling policy permits sending that
   source to Anthropic.  See `Anthropic's terms
   <https://www.anthropic.com/legal>`_.  Use ``--dry-run`` to preview
   cluster size and token estimates without making any API calls.

.. contents::
   :local:
   :depth: 2


Install
=======

From the project root::

   bin/daslang utils/daspkg/main.das -- install --root utils/find-dupe

This fetches the ``das-claude`` package per ``utils/find-dupe/.das_package``
and unpacks it under ``utils/find-dupe/modules/``.  If you skip this
step, ``bin/daslang utils/find-dupe/main.das`` fails at compile time
with ``module 'anthropic/anthropic' not found`` --- re-run the install
command above.  See :ref:`utils_daspkg` for daspkg semantics.


API key
=======

Set ``ANTHROPIC_API_KEY`` so daslang's non-interactive subshells can
see it.  On macOS the easiest path is ``~/.zshenv`` (sourced by every
zsh invocation, interactive or not)::

   echo 'export ANTHROPIC_API_KEY="sk-ant-..."' >> ~/.zshenv

``~/.zshrc`` works for terminals you opened yourself but **not** for
GUI-launched processes (VS Code, Claude Code's MCP server) --- those
inherit env from launchctl. ``~/.zshenv`` covers both.


Smoke test
==========

Verify the wiring before running over real clusters::

   bin/daslang utils/find-dupe/main.das -- --smoke-test

Expected output::

   Running das-claude smoke test (model=claude-haiku-4-5-20251001)...
   Reply: OK
   Tokens: in=15 out=4
   Smoke test PASSED.


Workflow
========

1. Run :ref:`detect-dupe <utils_detect_dupe>` to produce a JSON report::

      bin/daslang utils/detect-dupe/main.das -- -p <paths> --json ./dupes.json

2. Dry-run ``find_dupe`` for a cost preview (no API calls)::

      bin/daslang utils/find-dupe/main.das -- --input ./dupes.json --dry-run -v

   Output: cluster counts, estimated input/output tokens, dollar
   estimate.

3. Live run to produce verdicts::

      bin/daslang utils/find-dupe/main.das -- --input ./dupes.json -v

   Writes ``find_dupe_verdicts.json`` (machine) and
   ``find_dupe_verdicts.md`` (human) to ``./find-dupe-out/`` (override
   with ``--out``).

Run from the project root.  ``detect-dupe`` records source paths
relative to its cwd; ``find_dupe`` extracts each function's body from
disk using those paths, so its cwd must match.


Flags
=====

.. list-table::
   :header-rows: 1
   :widths: 25 18 57

   * - Flag
     - Default
     - Meaning
   * - ``-i``, ``--input``
     - required
     - detect-dupe JSON report file
   * - ``-o``, ``--out``
     - ``./find-dupe-out``
     - output directory
   * - ``--model``
     - ``haiku``
     - ``haiku`` (Haiku 4.5) or ``sonnet`` (Sonnet 4.6)
   * - ``--min-lines``
     - ``6``
     - skip clusters where every member is shorter than this
   * - ``--max-clusters``
     - ``0``
     - hard cap on clusters analyzed (0 = no cap)
   * - ``--sort-by``
     - ``default``
     - ``default`` (composite), ``size``, or ``fuzzy_first``
   * - ``--parallel``
     - ``8``
     - concurrent judge calls (1 = sequential)
   * - ``--positives-only``
     - off
     - filter reports to actionable rows only: real + partial
   * - ``--dry-run``
     - off
     - estimate tokens & cost without making API calls
   * - ``--smoke-test``
     - off
     - one-shot API call to verify the env wiring
   * - ``-v``, ``--verbose``
     - off
     - per-cluster progress to stdout
   * - ``-?``, ``--show-help``
     -
     - print help and exit

The default sort puts fuzzy pairs first (highest AI value), then
larger clusters, then larger total span --- useful with
``--max-clusters`` and when interrupting a long run.


Output schema
=============

``find_dupe_verdicts.json`` --- top-level fields:

.. list-table::
   :header-rows: 1
   :widths: 25 15 60

   * - Field
     - Type
     - Description
   * - ``schema_version``
     - int
     - bumps on schema changes
   * - ``model``
     - string
     - resolved model id
   * - ``summary``
     - object
     - totals + token usage
   * - ``verdicts``
     - array
     - one ``VerdictRow`` per cluster

Each ``VerdictRow`` carries ``cluster_id``, ``kind``
(``exact``/``fuzzy``), ``similarity`` (for fuzzy), ``members`` (with
file/line/end_line), ``verdict``
(``real``/``partial``/``false_positive``/``skipped``/``error``),
``groups`` (the partition), ``false_positives`` (indices), ``reason``,
and per-call token usage.

``find_dupe_verdicts.md`` --- human-readable, per-cluster sections
with verdict tag, reason, member list, partition, and the original
source (collapsed under ``<details>``).


MCP integration
===============

The :ref:`utils_mcp` server exposes two tools that wrap this CLI.
Both shell out to ``daslang utils/find-dupe/main.das`` --- the
``anthropic/anthropic`` dependency is fetched by daspkg at runtime, so
in-process ``require`` would force every MCP user to install daspkg
packages they may never use.  When the subprocess fails because the
package isn't installed, the tool surfaces a structured "anthropic
daspkg not installed" error with the exact install command.

.. list-table::
   :header-rows: 1
   :widths: 25 75

   * - Tool
     - Purpose
   * - ``judge_duplicates``
     - Take a detect-dupe JSON report and return the verdict envelope.
       Parameters: ``input`` (required), ``out``, ``model``,
       ``parallel``, ``max_clusters``, ``min_lines``, ``positives_only``,
       ``dry_run``.
   * - ``find_dupe``
     - Convenience: run detect-dupe against ``paths`` and judge the
       resulting clusters in a single call.  Parameters: ``paths``
       (required), ``out``, ``model``, ``threshold``, ``parallel``,
       ``max_clusters``, ``min_lines``, ``dry_run``.

Both tools accept ``dry_run=true`` to estimate cluster count and token
cost without making any API calls --- the safe default for "is this
worth running?" probes.


Pricing
=======

Approximate, as of 2026-04:

.. list-table::
   :header-rows: 1
   :widths: 25 35 40

   * - Model
     - Input ($/MTok)
     - Output ($/MTok)
   * - Haiku 4.5
     - 1.00
     - 5.00
   * - Sonnet 4.6
     - 3.00
     - 15.00

A typical cluster (3-4 functions, 50 lines total) consumes ~1 KTok in
+ 0.3 KTok out → ~$0.0025 on Haiku.  Use ``--dry-run`` first if a run
looks large.


Implementation
==============

.. list-table::
   :header-rows: 1
   :widths: 30 70

   * - File
     - Role
   * - ``main.das``
     - CLI (``daslib/clargs``), pipeline orchestration, parallel
       fan-out via ``daslib/jobque_boost``
   * - ``cluster_input.das``
     - detect-dupe JSON shadow structs +
       ``read_input_report``
   * - ``source_extract.das``
     - ``FileCache`` + ``extract_source(path, start_line, end_line)``
       for pulling function bodies from disk
   * - ``judge.das``
     - ``Verdict`` shape, ``parse_verdict``, ``classify_verdict``,
       prompt formatter, JSON-Schema for the ``report_verdict`` tool
   * - ``judge_live.das``
     - ``live_judge`` --- the actual Anthropic API call.  Imports
       ``anthropic/anthropic`` (daspkg).
   * - ``report.das``
     - ``OutputReport`` shape, JSON + Markdown writers
   * - ``tests/test_find_dupe.das``
     - dastest suite (hermetic; no API calls)


See also
========

* :ref:`utils_detect_dupe` --- the cluster-producing pipeline whose
  JSON ``find_dupe`` consumes.
* :ref:`utils_mcp` --- the MCP server that wraps both tools.
* :ref:`utils_daspkg` --- the package manager that fetches
  ``das-claude``.
