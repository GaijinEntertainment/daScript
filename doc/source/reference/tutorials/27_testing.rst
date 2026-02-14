.. _tutorial_testing:

========================
Testing with dastest
========================

.. index::
    single: Tutorial; Testing
    single: Tutorial; dastest
    single: Tutorial; Assertions

This tutorial covers the daScript testing framework: writing tests with
``[test]``, assertion functions, sub-tests, and running tests.

Setup
=====

Import the testing framework::

  require dastest/testing_boost public

Writing tests
=============

Annotate functions with ``[test]``. Each test receives a ``T?`` context::

  [test]
  def test_arithmetic(t : T?) {
      t |> equal(2 + 2, 4)
      t |> equal(3 * 3, 9, "multiplication")
  }

Assertions
==========

.. list-table::
   :header-rows: 1
   :widths: 35 65

   * - Function
     - Behavior
   * - ``t |> equal(a, b)``
     - Check ``a == b``; non-fatal on failure
   * - ``t |> strictEqual(a, b)``
     - Check ``a == b``; **fatal** on failure (stops test)
   * - ``t |> success(cond)``
     - Check condition is truthy; non-fatal
   * - ``t |> failure("msg")``
     - Always fails; non-fatal
   * - ``t |> accept(value)``
     - Suppress unused warnings (no assertion)

All assertion functions accept an optional message string as the last
argument.

Sub-tests
=========

Group related checks with ``run``::

  [test]
  def test_strings(t : T?) {
      t |> run("concat") <| @@(t : T?) {
          t |> equal("ab" + "cd", "abcd")
      }
      t |> run("length") <| @@(t : T?) {
          t |> equal(length("hello"), 5)
      }
  }

Use ``@@(t : T?) { ... }`` (no-capture lambda) for the callback.

Running tests
=============

From the command line::

  daslang.exe dastest/dastest.das -- --test path/to/test.das

To test an entire directory::

  daslang.exe dastest/dastest.das -- --test path/to/tests/

.. seealso::

   Full source: :download:`tutorials/language/27_testing.das <../../../../tutorials/language/27_testing.das>`
