.. _utils_benchctl:

.. index::
   single: Utils; benchctl
   single: Utils; Benchmarks

=========================================
 benchctl --- Benchmark Result Database
=========================================

benchctl stores daslang benchmark results in a local SQLite database and
compares result sets with statistical significance (a port of Go's
benchstat Welch's t-test).  It reads the JSON files the benchmark runner
emits (``dastest --bench --bench-format json``), tags records by commit
hash, and computes per-benchmark and geometric-mean deltas between two
sets.

Requires a daslang build with the sqlite module
(``-DDAS_SQLITE_DISABLED=off``).  The SDK bundle also carries a prebuilt
``bin/benchctl.exe``.

Quick start
===========

::

   daslang utils/benchctl/main.das -- <command> [options...]

Commands: ``reset`` (initialize the database — drops existing data),
``insert`` (load benchmark JSON files, tagged by commit), ``query``
(filter records by ``--commit`` / ``--tag``), ``compare`` (two result
sets, significance-tested).  All commands accept ``--db <path>``
(default ``benchdata.db``).

The database is plain SQLite — browse or export it with any sqlite3
client.

.. seealso::

   :ref:`utils_dastest` -- the benchmark runner that produces the JSON input
