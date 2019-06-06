.. _introduction:

************
Introduction
************

.. index::
    single: introduction

daScript is high-performance statically strong typed scripting language, designed to be high-performance
as embeddable 'scripting' language for real-time applications (like games).

daScript offers a wide range of features like strong static typing, generic programming with iterative type inference,
Ruby-like blocks, semantic indenting, native machine types, ahead-of-time 'compilation' to c++ and fast and simplified bindings to C++ program.

It's philsophy is build around modified Zen of Python.

* *Performance counts.*
* *But not at the cost of safety.*
* *Unless is explicitly unsafe to be performant.*
* *Readability counts.*
* *Explicit is better than implicit.*
* *Simple is better than complex.*
* *Complex is better than complicated.*
* *Flat is better than nested.*

In a real world scenarios, it's interpretation is 10+ times faster than lua-jit without JIT (and can be even faster than luajit with JIT).
It also allows Ahead-of-Time compilation, which is not only possible on all platforms (unlike JIT), but also always faster/not-slower (JIT is known to sometimes slow down scripts).

It already has implemented AoT (c++ transpiler) produces more or less similar with c++11 performance.

`Table with performance comparisons on a syntetic samples
<https://docs.google.com/spreadsheets/d/1y1G4exD4J9o3kPYw6Y-eaVoffbJ5h_mWVG121wp2k9s/htmlview>`_.


Mandatory fibonacci samples ::

    def fibR(n)
       if (n < 2)
           return n
       else
           return fibR(n - 1) + fibR(n - 2)

    def fibI(n)
       let last = 0
       let cur = 1
       for i in range(0, n-1)
           let tmp = cur
           cur += last
           last = tmp
       return cur

It's (not)full list of features includes:

* strong typing
* Ruby-like blocks
* tables
* arrays
* string-builder
* native (c++ friendly) interop
* generics
* semantic indenting
* ECS-friendly interop
* etc, etc
