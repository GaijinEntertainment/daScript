.. _utils_find_dupes:

.. index::
   single: Utils; find_dupes
   single: Utils; Duplicate detection
   single: Utils; Code similarity

=====================================================
 find_dupes --- Cross-file similar-function detector
=====================================================

``find_dupes`` walks one or more directories of ``.das`` files,
normalises every user function into an alpha-renamed token stream
(identifiers, types, and literals all collapsed), and reports
near-identical functions across the corpus.  It is useful for
surfacing test-suite boilerplate that could be factored, near-clones
that drifted apart, or copy-pasted helpers that escaped review.

Two interfaces ship together: a CLI (``utils/find_dupes/main.das``)
and two MCP tools (``export_corpus`` and ``find_duplicates``) that
expose the same engine to AI coding assistants.

.. contents::
   :local:
   :depth: 2


What it reports
===============

Every output is one of two kinds of match:

* **Exact-clone clusters** --- functions whose canonical token
  streams are byte-identical.  Pure structural duplicates, modulo
  identifier names, types, and literal values.
* **Fuzzy near-duplicates** --- pairs scored as
  ``sqrt(jaccard × len_ratio)`` over a 64-slot MinHash signature,
  with a hard ``len_ratio >= threshold`` gate.  The length gate
  suppresses MinHash false-positives on highly periodic boilerplate
  (otherwise a 4-statement and a 7-statement copy of the same
  ``t |> run(...)`` block both look 100% identical to MinHash).

The geometric-mean score admits a Jaccard somewhat below
``threshold`` when lengths match closely.  This is intentional and
biases toward recall.


Quick start
===========

CLI::

   bin/daslang utils/find_dupes/main.das -- -p tests --json /tmp/dupes.json
   bin/daslang utils/find_dupes/main.das -- -p tests/strings -t 0.85 -n 20
   bin/daslang utils/find_dupes/main.das -- -p daslib --no-fuzzy --min-tokens 32
   bin/daslang utils/find_dupes/main.das -- -p tests --keep all
   bin/daslang utils/find_dupes/main.das -- -?

JIT works too --- ``bin/daslang -jit utils/find_dupes/main.das -- ...``
(net runtime improvement is modest because per-file cost is
dominated by interpreter compilation of the scanned files).


Flags
=====

.. list-table::
   :header-rows: 1
   :widths: 25 12 63

   * - Flag
     - Default
     - Meaning
   * - ``-p / --path``
     - required
     - File or directory to scan; repeatable
   * - ``-t / --threshold``
     - 0.7
     - Fuzzy similarity floor (0..1).  Score is
       ``sqrt(jaccard × len_ratio)`` plus a hard
       ``len_ratio >= threshold`` gate
   * - ``-n / --top``
     - 20
     - Top-N entries shown in stdout summary
   * - ``--json``
     - off
     - Path for full JSON report
   * - ``-x / --no-fuzzy``
     - off
     - Skip MinHash pass --- exact clusters only
   * - ``--min-tokens``
     - 8
     - Drop functions with fewer than N tokens (filters trivial
       wrappers)
   * - ``-L / --lambdas-only``
     - off
     - Skip top-level functions, keep only lambdas --- useful for
       clustering dastest ``t |> run("…") @(t) { … }`` bodies
   * - ``--export-functions``
     - off
     - Write all extracted functions to a JSON file and exit before
       clustering
   * - ``--import-functions``
     - off
     - Load functions from a JSON file (produced by
       ``--export-functions``) instead of compiling.  Mutually
       exclusive with ``--path`` and ``--export-functions``
   * - ``--baseline``
     - off
     - B1: load corpus JSON; tag records whose member identity
       (``file:line:name``) isn't in the baseline as candidates and
       filter to those
   * - ``--baseline-strict``
     - off
     - B1 modifier: also drop clusters whose canonical exists in the
       baseline (only fully-new clusters survive)
   * - ``--against``
     - off
     - B2 candidate path (file or directory).  Repeatable.
       Compiled in-process; their functions are tagged candidates
       and the report is filtered
   * - ``--against-from-stdin``
     - off
     - B2: read newline-delimited candidate paths from stdin
   * - ``--check``
     - off
     - Exit non-zero when the post-filter report contains any
       clusters/pairs (CI gate)
   * - ``--flat``
     - off
     - In ``--against`` mode, force the flat clusters/pairs writer
       (default in ``--against`` mode is the per-candidate rollup)
   * - ``-k / --keep``
     - off
     - Pattern name to KEEP despite default skip (repeatable).
       Special value ``all`` disables pattern filtering entirely.
       See :ref:`utils_find_dupes_patterns`
   * - ``-v / --verbose``
     - off
     - Per-file progress
   * - ``-?``
     -
     - Show help

``builtin.das``, ``daslib/debugger.das``, and ``daslib/profiler.das``
are skipped automatically --- the latter two install thread-local
debug agents at compile time, which would abort the scanner on the
second use.


.. _utils_find_dupes_patterns:

Pattern filter
==============

A "pattern" is a structural shape that signals "this function is
boilerplate, not real code" --- typically a wrapper or dispatcher
whose canonical token stream contains zero unique signal beyond its
repetition count.  Pattern-matched functions are dropped from
clustering by default, on the theory that they explode cluster
sizes and fuzzy-pair counts without surfacing real duplicates.

Override per-pattern via ``--keep <name>`` (repeatable), or disable
filtering wholesale via ``--keep all``.

Currently shipped patterns:

.. list-table::
   :header-rows: 1
   :widths: 15 35 50

   * - Name
     - Detects
     - Why it's boilerplate
   * - ``dispatch``
     - Function whose body is N >= 2 byte-identical top-level
       statement chunks
     - dastest's ``t |> run("X") @(t) { … }`` outer functions.
       Lambda bodies are collapsed to ``ADDR`` upstream, so two
       ``run`` calls look identical regardless of what the lambdas
       do.  Same shape catches ``t |> bench(…)``, repeated-init
       blocks, and any uniform call list.

The summary line surfaces what was filtered::

   collected 66 record(s); 0 compile failure(s), 1 skipped (expect-directive)
   patterns skipped: 35 dispatch (--keep <name>|all to include)

``--verbose`` prints one ``pattern-skip [name] file:line func (note)``
line per filtered record --- useful for confirming the filter isn't
dropping real signal on a new corpus.


Canonical form
==============

Each function emits a flat tag stream.  Examples:

.. list-table::
   :header-rows: 1
   :widths: 50 50

   * - Source
     - Canonical (excerpt)
   * - ``def add(a,b:int):int { return a+b }``
     - ``FN ARG <var_0> TYP ARG <var_1> TYP TYP BODY BLK STMT RET OP2:+ <var_0> <var_1> ENDBLK ENDFN``
   * - ``def add(a,b:float):float { return a+b }``
     - (same --- types collapse)
   * - ``def double(a:int) { return a*2 }``
     - ``FN ARG <var_0> TYP BODY BLK STMT RET OP2:* <var_0> LIT ENDBLK ENDFN``

User identifiers become ``<var_0>``, ``<var_1>``, ...  All types
collapse to ``TYP``.  All literals become ``LIT``.  Field/swizzle
names use ``.FLD`` / ``.SWZ``.  Called function names are kept
(``CALL:push`` vs ``CALL:emplace`` is real signal).


Modes
=====

The flat report from ``-p`` is the firehose --- useful once on a
new corpus to calibrate, less useful day-to-day.  Two filtered
modes are layered on top via a single ``is_candidate`` flag inside
``FuncRecord``.  A cluster or fuzzy pair is **kept** iff at least
one of its members is a candidate.


B1 --- baseline diff (CI gate)
------------------------------

Snapshot the corpus once, commit the JSON, and on every PR re-scan
and diff::

   # one-off: build the baseline (commit this)
   bin/daslang utils/find_dupes/main.das -- -p tests --export-functions tests_baseline.json

   # CI: scan again, surface only what isn't in the baseline
   bin/daslang utils/find_dupes/main.das -- -p tests --baseline tests_baseline.json --check

Records are tagged candidate when their **member identity**
(``file:line:name``) is absent from the baseline.  A cluster
appears in the report if any of its members is a candidate, which
catches both (a) brand-new canonicals and (b) growth --- a new copy
of an already-tracked canonical added in a new location.

Use ``--baseline-strict`` to drop case (b) --- strict additionally
filters out clusters whose canonical was already in the baseline,
so only fully-new canonicals survive.  Pairs aren't strict-filtered
(the baseline doesn't carry MinHash signatures), so strict is
cluster-only.

``file:line:name`` keying means an unrelated edit that shifts line
numbers will look like a "new member" and surface its cluster ---
acceptable for CI, since touched code is the right default to
re-check.

``--check`` turns the filtered report into a CI gate --- non-zero
exit when any cluster or pair survives the filter.


B2 --- PR-files / interactive
-----------------------------

"Did I just write something that already exists?"  Compare a file
list against a pre-built corpus::

   bin/daslang utils/find_dupes/main.das -- \
       --import-functions tests_baseline.json --against tests/strings/new_helper.das

   # git pipeline:
   git diff --name-only master | grep '\.das$' | \
       bin/daslang utils/find_dupes/main.das -- \
           --import-functions tests_baseline.json --against-from-stdin

When ``--against`` and ``--import-functions`` are both set, corpus
records whose ``file`` matches any candidate path are dropped first,
then the candidate is freshly compiled --- so the file is compared
against the rest of the world, never against its own stale copy in
the baseline.  Look for the ``dropped N corpus records overridden``
line.

The default writer in ``--against`` mode is a per-candidate rollup
("for each function in the focus set, here are its top siblings").
Use ``--flat`` to revert to the legacy clusters+pairs view.


Export / import
===============

Compilation is the expensive step; the canonical-form computation
is deterministic.  To hand the function list off to an external
tool (visualizer, custom clusterer), or to shard compilation
across machines and merge later, dump the post-canonicalization
records and reload them::

   bin/daslang utils/find_dupes/main.das -- -p tests --export-functions /tmp/funcs.json
   bin/daslang utils/find_dupes/main.das -- --import-functions /tmp/funcs.json --json /tmp/dupes.json

``--import-functions`` is mutually exclusive with both ``--path``
and ``--export-functions``.  ``--export-functions`` always exits
before the clustering pass.

The on-disk schema is a small envelope:

.. code-block:: json

   {
     "schema_version": 1,
     "functions": [
       {
         "name": "add_int",
         "file": "tests/foo.das",
         "line": 4,
         "is_lambda": false,
         "canonical": "FN ARG <var_0> TYP ..."
       }
     ]
   }

MinHash signatures are not included --- they're recomputed on
import (deterministic and cheap).  On import, ``--no-fuzzy`` and
``--min-tokens`` apply just like in the compile path.


MCP integration
===============

The :ref:`utils_mcp` server exposes two tools that wrap the engine
end-to-end --- no shelling out:

.. list-table::
   :header-rows: 1
   :widths: 25 75

   * - Tool
     - Purpose
   * - ``export_corpus``
     - Scan ``paths`` (files / directories / globs), compile each
       ``.das`` file in-process, and write a corpus JSON to ``out``
       in the same shape ``find_duplicates`` expects.
       Replacement for the CLI ``--export-functions``.
   * - ``find_duplicates``
     - Wraps B2 mode.  Pass ``paths`` (newline- or
       comma-delimited, or a glob) and ``corpus`` (a JSON from
       ``export_corpus`` / ``--export-functions``); receive a
       per-candidate JSON envelope with corpus stats, pattern-skip
       counts, and a per-candidate report containing the top-N
       exact and fuzzy matches.

Both tools surface the pattern filter via a ``keep`` parameter
that mirrors the CLI's ``--keep`` flag.  The envelope also
reports ``candidate_functions_pre_filter``, so the caller can
distinguish "no candidates compiled" from "all candidates were
pattern-filtered out".


Implementation
==============

.. list-table::
   :header-rows: 1
   :widths: 30 70

   * - File
     - Role
   * - ``canonical.das``
     - ``CanonicalVisitor`` (extends ``daslib/ast`` ``AstVisitor``)
       and ``tokenize_canonical``
   * - ``minhash.das``
     - 64-slot MinHash signatures over 5-grams, Jaccard estimate
   * - ``cluster.das``
     - Exact-bucket clustering + fuzzy all-pairs with length gate
   * - ``report.das``
     - JSON + stdout summary writer
   * - ``main.das``
     - CLI (``daslib/clargs``), file scan, compile-and-collect
       orchestration
   * - ``pipeline.das``
     - ``compile_and_collect`` / ``collect_from_program`` ---
       compile-and-extract orchestration, shared by ``main.das``
       and the test suite.  Also hosts ``apply_pattern_filter``
       and the filesystem scan helpers
   * - ``patterns.das``
     - Pattern matchers --- ``classify(canonical) → PatternHit``.
       Default-skipped shapes (dispatchers etc.); override via
       ``--keep <name>``
   * - ``exchange.das``
     - On-disk JSON schema + writer/reader for
       ``--export-functions`` / ``--import-functions``
   * - ``fixture/synth.das``
     - Hand-crafted fixture for smoke-testing the visitor
       end-to-end
   * - ``fixture/canonical_cases.das``
     - Narrowly-targeted fixture (one function per
       canonicalization concern) for unit-testing
       ``CanonicalVisitor``
   * - ``test_find_dupes.das``
     - dastest suite --- run with
       ``bin/daslang dastest/dastest.das -- --test utils/find_dupes/test_find_dupes.das``


Notes
=====

* Compile policy mirrors ``utils/lint``: ``ignore_shared_modules``,
  ``export_all``.  Optimisations and infer-time folding stay ON so
  dastest macros (e.g. ``unroll``) compile.
* **Default mode** drops everything ``generated`` (which includes
  lambdas).  The dispatcher already references each lambda via an
  ``ADDR`` token, so the lambda's structural fingerprint is
  partially preserved in the parent function.  Use ``-L`` to flip
  this and cluster the lambda bodies themselves.
* **Lambda-only mode (``-L``)** is dominated at the top by linq's
  ``each`` macro emissions (a 100+-token
  ``GOTO/LABEL/_builtin_iterator_first/next/close`` shell that
  recurs hundreds of times).  Real test-body signal starts a few
  clusters down; sort/grep accordingly.
* Functions whose ``at.fileInfo`` points outside the compiled file
  are filtered out --- without this, reified generics from required
  modules (e.g. ``dastest/testing.das``) flood the report.
* Per-source-line dedup: a generic reified for N types becomes N
  ``FunctionPtr`` instances all pointing at the same
  ``(file, line)``.  ``find_dupes`` keeps the first to avoid the
  same source location being counted N times.


Out of scope
============

* LSH / banding (4.4K\ :sup:`2` is fine; revisit at >50K functions).
* Embedding-based similarity (would need an external service).
* Auto-fix or refactor suggestions --- discovery only.
