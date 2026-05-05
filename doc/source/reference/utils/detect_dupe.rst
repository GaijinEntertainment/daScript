.. _utils_detect_dupe:

.. index::
   single: Utils; detect-dupe
   single: Utils; Duplicate detection
   single: Utils; Code similarity

=====================================================
 detect-dupe --- Cross-file similar-function detector
=====================================================

``detect-dupe`` walks one or more directories of ``.das`` files,
normalises every user function into an alpha-renamed token stream
(identifiers, types, and literals all collapsed), and reports
near-identical functions across the corpus.  It is useful for
surfacing test-suite boilerplate that could be factored, near-clones
that drifted apart, or copy-pasted helpers that escaped review.

Two interfaces ship together: a CLI (``utils/detect-dupe/main.das``)
and two MCP tools (``export_corpus`` and ``detect_duplicates``) that
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

   bin/daslang utils/detect-dupe/main.das -- -p tests --json /tmp/dupes.json
   bin/daslang utils/detect-dupe/main.das -- -p tests/strings -t 0.85 -n 20
   bin/daslang utils/detect-dupe/main.das -- -p daslib --no-fuzzy --min-tokens 32
   bin/daslang utils/detect-dupe/main.das -- -p tests --keep all
   bin/daslang utils/detect-dupe/main.das -- -?

JIT works too --- ``bin/daslang -jit utils/detect-dupe/main.das -- ...``
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
     - required\*
     - File or directory to scan; repeatable.  ``*`` one of ``-p``,
       ``--paths-from``, ``--paths-stdin`` is required (or
       ``--import-functions`` / ``--against``)
   * - ``--paths-from``
     - off
     - Read newline-delimited paths from a file (``#``-comments and
       blank lines skipped).  Composes with ``-p``.  Each entry can
       be a file or directory; directories recurse via the same
       scanner as ``-p``
   * - ``--paths-stdin``
     - off
     - Read newline-delimited paths from stdin (``#``-comments and
       blank lines skipped).  Composes with ``-p``.  Mutually
       exclusive with ``--against-from-stdin`` (one stdin reader per
       run)
   * - ``-j / --workers``
     - 0 (auto)
     - Worker count for parallel ``--export-functions`` runs.  0 =
       hardware threads.  1 = sequential.  Files are sorted, split
       into N contiguous chunks, each compiled by a child detect-dupe
       process; shards are merged in chunk-index order so output is
       byte-identical across worker counts.  Below 16 input files the
       export stays sequential regardless.  Ignored without
       ``--export-functions``
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
       See :ref:`utils_detect_dupe_patterns`
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


.. _utils_detect_dupe_patterns:

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
   * - ``visitor``
     - Class-method whose hook name starts with ``visit``,
       ``preVisit``, ``postVisit``, ``before``, or ``after``
       (matched by name, regardless of body)
     - ``AstVisitor`` overrides --- the dispatch contract requires
       one method per AST node type, so cross-class duplication at
       the canonical level is structural, not actionable.  Common
       in ``daslib/aot_cpp.das``, ``daslib/ast_print.das``,
       ``daslib/templates_boost.das``, ``daslib/rst_comment.das``,
       ``daslib/perf_lint.das``.
   * - ``dispatch``
     - Function whose body is N >= 2 byte-identical top-level
       statement chunks
     - dastest's ``t |> run("X") @(t) { … }`` outer functions.
       Lambda bodies are collapsed to ``ADDR`` upstream, so two
       ``run`` calls look identical regardless of what the lambdas
       do.  Same shape catches ``t |> bench(…)``, repeated-init
       blocks, and any uniform call list.
   * - ``emit``
     - 1..6 top-level statements, each a single trivial
       ``CALL:foo(...)`` (literal/var/field args only --- no nested
       calls, no control flow) or a ``RET ...``
     - Emitter shells like
       ``def visitX(...) { write(*ss, ")") ; return that }``.
       Catches free-function emitter wrappers that the ``visitor``
       matcher (which is name-based) doesn't cover.

Match order in ``classify()`` is name-first
(``visitor``), then body-shape (``dispatch``, ``emit``) --- the
first match wins.  A visitor method whose body happens to fit the
``emit`` shape is still classified as ``visitor``.

The summary line surfaces what was filtered::

   collected 3097 record(s); 0 compile failure(s), 0 skipped (expect-directive)
   patterns skipped: 6 dispatch, 90 emit, 601 visitor (--keep <name>|all to include)

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

An AI judge (:ref:`utils_find_dupe`) can consume the resulting JSON
report and triage clusters into real duplicates, partial matches, and
false positives --- useful when a flat report is too noisy to walk
manually.


B1 --- baseline diff (CI gate)
------------------------------

Snapshot the corpus once, commit the JSON, and on every PR re-scan
and diff::

   # one-off: build the baseline (commit this)
   bin/daslang utils/detect-dupe/main.das -- -p tests --export-functions tests_baseline.json

   # CI: scan again, surface only what isn't in the baseline
   bin/daslang utils/detect-dupe/main.das -- -p tests --baseline tests_baseline.json --check

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

   bin/daslang utils/detect-dupe/main.das -- \
       --import-functions tests_baseline.json --against tests/strings/new_helper.das

   # git pipeline:
   git diff --name-only master | grep '\.das$' | \
       bin/daslang utils/detect-dupe/main.das -- \
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

   bin/daslang utils/detect-dupe/main.das -- -p tests --export-functions /tmp/funcs.json
   bin/daslang utils/detect-dupe/main.das -- --import-functions /tmp/funcs.json --json /tmp/dupes.json

``--import-functions`` is mutually exclusive with both ``--path``
and ``--export-functions``.  ``--export-functions`` always exits
before the clustering pass.

Parallel export (``-j / --workers``)
------------------------------------

For large corpora the dominant cost is per-file compilation.
``--workers N`` fans the export across N child detect-dupe
processes:

* The full file list is sorted, split into N contiguous chunks,
  and written to per-worker temp files.
* Each child runs ``--paths-from <chunk> --export-functions
  <shard> --workers 1`` and produces its own envelope.
* The parent reads shards back **in chunk-index order** and
  concatenates their ``functions`` arrays.  Output is therefore
  byte-identical to a sequential ``--workers 1`` run on the same
  inputs.
* Below 16 input files the export stays sequential regardless ---
  child-process startup dominates the savings on small lists.

Default (``-j 0``) is auto, equal to the host's hardware thread
count.  Compile is CPU-bound, so oversubscription does not help.
``-j 1`` forces sequential.  Compile failures in any child fail
the whole export (no partial corpus shipped) --- same gate as the
sequential path.

Explicit file-list inputs
-------------------------

``--paths-from <file>`` and ``--paths-stdin`` accept newline-
delimited file lists (``#``-comments and blank lines skipped).
They compose with ``-p`` (union, deduplicated) and are typically
used to scope an export to the files in a PR diff:

.. code-block:: sh

   # via file (avoids ARG_MAX on big PRs)
   git diff --name-only master | grep '\.das$' > /tmp/pr.txt
   bin/daslang utils/detect-dupe/main.das -- \
       --paths-from /tmp/pr.txt --export-functions pr.json

   # via stdin (same idea, no temp file)
   git diff --name-only master | grep '\.das$' | \
       bin/daslang utils/detect-dupe/main.das -- \
           --paths-stdin --export-functions pr.json

Each entry can be a file or a directory; directories recurse via
the same scanner ``-p`` uses.  ``--paths-stdin`` is mutually
exclusive with ``--against-from-stdin`` (one stdin reader per
run).

For tools that take a comma- or newline-separated list of files,
directories, and globs in a single argument (the format used by
all MCP file/glob tools), the canonical expander lives in
``daslib/fio.parse_file_list`` --- it strips whitespace, dispatches
literals through and globs through ``expand_glob``, and preserves
the order of plain entries across glob expansions.

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
       in the same shape ``detect_duplicates`` expects.
       Replacement for the CLI ``--export-functions``.
   * - ``detect_duplicates``
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
     - Pattern matchers ---
       ``classify(name, canonical) → PatternHit``.
       Default-skipped shapes (visitor methods, dispatchers,
       emitters); override via ``--keep <name>``
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
   * - ``test_detect_dupe.das``
     - dastest suite --- run with
       ``bin/daslang dastest/dastest.das -- --test utils/detect-dupe/test_detect_dupe.das``


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
  ``(file, line)``.  ``detect-dupe`` keeps the first to avoid the
  same source location being counted N times.


Out of scope
============

* LSH / banding (4.4K\ :sup:`2` is fine; revisit at >50K functions).
* Embedding-based similarity (would need an external service).
* Auto-fix or refactor suggestions --- discovery only.
