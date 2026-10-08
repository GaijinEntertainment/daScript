.. _tutorial_pattern_matching:

========================
Pattern Matching
========================

.. index::
    single: Tutorial; Pattern Matching
    single: Tutorial; Match
    single: Tutorial; Variable Binding

This tutorial covers daslang's pattern matching with the ``match``
macro from ``daslib/match``.

Basic match
===========

``match (expr) { ... }`` dispatches on values. Each arm is
``pattern => body``; a body of several statements is a ``$ { }`` block,
and ``return`` inside it returns from the enclosing function::

  require daslib/match

  def describe(n : int) : string {
      match (n) {
          0 => $ { return "zero" }
          1 => $ { return "one" }
          _ => $ { return "other" }
      }
      return "unreachable"
  }

match as a value
================

In value position each arm body is an expression, and the arms must
cover every value. A match over an enum, ``bool`` or variant must name
every value or end with ``_``; an arm an earlier one already covers is
a compile error::

  def color_code(c : Color) : int {
      return match (c) {
          Color.red => 1
          Color.green => 2
          _ => 0
      }
  }

Wildcards and binding
=====================

- ``_`` — wildcard, matches anything
- a bare name — binds the matched value to a new variable::

    return match (point) {
        Point(x = 0, y = 0) => "origin"
        Point(x = x, y = y) => "({x}, {y})"
    }

A bare name never compares against an existing variable; use
``match_expr(name)`` for that.

Guards
======

Add ``&&`` after a pattern for extra conditions::

  return match (shape) {
      Shape(size = sz) && sz > 100 => "large"
      _ => "small"
  }

OR patterns
===========

Match multiple alternatives with ``|``::

  return match (color) {
      Color.red | Color.green | Color.blue => true
      _ => false
  }

Several values
==============

``match (a, b)`` takes one tuple-shaped pattern per arm::

  return match (x, y) {
      (0, 0) => "origin"
      (0, _) | (_, 0) => "on an axis"
      (a, b) && a > 0 && b > 0 => "first"
      _ => "elsewhere"
  }

Variant matching
================

A variant pattern is spelled like the variant constructor::

  return match (value) {
      Value(i = i) => "int {i}"
      Value(f = f) => "float {f}"
      _ => "other"
  }

Tuple matching
==============

Match tuple elements positionally::

  return match (pair) {
      (0, "zero") => "exact"
      (1, _) => "starts with one"
      (n, "hello") => "hello #{n}"
      _ => "other"
  }

Array matching
==============

Static arrays match element-by-element. Dynamic arrays match head
elements; use ``...`` to ignore the tail::

  return match (sa) {
      fixed_array<int>(0, 0, 0) => "zeros"
      fixed_array<int>(1, b, c) => "1,{b},{c}"
      _ => "other"
  }

  return match (da) {
      array<int>(0, 0, ...) => "starts with 0,0"
      array<int>(x, y, ...) => "starts with {x},{y}"
      _ => "other"
  }

match_expr — computed patterns
===============================

``match_expr(expression)`` evaluates at runtime instead of matching
literally. Useful when the expected value depends on a bound name::

  return match (t) {
      (a, match_expr(a + 1), match_expr(a + 2)) => true   // consecutive triples
      _ => false
  }

Bool matching
=============

``match`` works on booleans::

  return match (b) {
      true => "yes"
      _ => "no"
  }

static_match
============

- ``static_match`` — silently drops type-mismatched arms (for generics)

.. seealso::

   :ref:`Pattern matching <pattern-matching>` in the language reference.

   Full source: :download:`tutorials/language/24_pattern_matching.das <../../../../tutorials/language/24_pattern_matching.das>`

   Next tutorial: :ref:`tutorial_annotations`
