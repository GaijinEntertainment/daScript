.. _introduction:

************
Introduction
************

.. index::
    single: introduction

daScript is high-performance statically strong typed scripting language, designed to be high-performance
as embeddable "scripting" language for real-time applications (like games).

daScript offers a wide range of features like strong static typing, generic programming with iterative type inference,
Ruby-like blocks, semantic indenting, native machine types, ahead-of-time "compilation" to C++ and fast and simplified bindings to C++ program.

It's philosophy is build around modified Zen of Python.

* *Performance counts.*
* *But not at the cost of safety.*
* *Unless is explicitly unsafe to be performant.*
* *Readability counts.*
* *Explicit is better than implicit.*
* *Simple is better than complex.*
* *Complex is better than complicated.*
* *Flat is better than nested.*

daScript is supposed to work as "host data processor".
While it is technically possible to maintain persistent state within script context (with a certain option set),
daScript is designed to transform your host (C++) data/implement scripted behaviors.

In a certain sense, it is pure functional - i.e. all persistent state is out of scope of scripting context, and script's state is temporal by it's nature.
So, the memory model and management of persistent state becoming are or responsibility of application, and leads to extremely simple and fast memory model in the daScript itself.

+++++++++++++
Performance.
+++++++++++++

In a real world scenarios, it's interpretation is 10+ times faster than LuaJIT without JIT (and can be even faster than LuaJIT with JIT).
Which is probably even more important for embedded scripting languages, it's (both-ways) interop with C++ is extremely fast, order of magnitude faster than such in most popular scripting languages.
Fast "call from C++ to daScript" allows to use daScript for simple stored procedures, and makes it's ECS/Data Oriented Design friendly language.
Fast call C++ from daScript allows to write performant scripts which are processing host (C++) data, or relies on bound host (C++) functions.

It also allows Ahead-of-Time compilation, which is not only possible on all platforms (unlike JIT), but also always faster/not-slower (JIT is known to sometimes slow down scripts).

daScript already has implemented AoT (C++ transpiler) which produces code more or less similar with C++11 performance.

`Table with performance comparisons on a syntetic samples
<https://docs.google.com/spreadsheets/d/1y1G4exD4J9o3kPYw6Y-eaVoffbJ5h_mWVG121wp2k9s/htmlview>`_.

+++++++++++++
How it looks?
+++++++++++++

Mandatory fibonacci samples ::

    def fibR(n)
       if (n < 2)
           return n
       else
           return fibR(n - 1) + fibR(n - 2)

    def fibI(n)
       var last = 0
       var cur = 1
       for i in range(0, n - 1)
           let tmp = cur
           cur += last
           last = tmp
       return cur

++++++++++++++++++++++++++++++++++++
Generic programming and type system
++++++++++++++++++++++++++++++++++++

Although above sample seem to be dynamically typed, it is actually a generic programming.
The actual instance of fibI/fibR function is strong typed and basically is just accepting and returning int. This is similar to templates in C++ (although C++ is not a strong-typed language) or ML.
Generic programming in daScript allows very powerful compile-time type reflection mechanisms, significantly simplifying writing optimal and clear code.
Unlike C++ with it's SFINAE, you can use common conditions (if) in order to change instance of function depending on type info of arguments.
Consider the following example::

    def setSomeField(var obj; val)
        static_if typeinfo(has_field<someField> obj)
            obj.someField = val

this function will set someField in provided argument *if* it is a struct with someField member.

For more info see :ref:`Generic programming <generic_programming>`).

+++++++++++++++++++++++
Compilation time macros
+++++++++++++++++++++++

daScript does a lot of heavy lifting during compilation time, so that it does not have to at run time.
In fact daScript compiler runs daScript interpreter for each module, and has entire AST available to it.

The following example modifies function call at compilation time to add precomputed hash of a constant string argument::

    [tag_function_macro(tag="get_hint_tag")]
    class GetHintFnMacro : AstFunctionAnnotation
        [unsafe] def override transform ( var call : smart_ptr<ExprCall>;
            var errors : das_string ) : ExpressionPtr
            if call.arguments[1] is ExprConstString
                let arg2 = reinterpret<ExprConstString?>(call.arguments[1])
                var mkc <- new [[ExprConstUInt() at=arg2.at, value=hash("{arg2.value}")]]
                push(call.arguments, ExpressionPtr(mkc))
                return <- ExpressionPtr(call)
            return [[ExpressionPtr]]


++++++++++++++++++++++++++++++++++++
Features
++++++++++++++++++++++++++++++++++++
It's (not)full list of features includes:

* strong typing
* Ruby-like blocks and lambda
* tables
* arrays
* string-builder
* native (C++ friendly) interop
* generics
* classes
* macros, including reader macros
* semantic indenting
* ECS-friendly interop
* easy-to-extend type system
* etc.
