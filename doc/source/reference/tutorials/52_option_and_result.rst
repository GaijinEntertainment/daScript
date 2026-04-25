.. _tutorial_option_and_result:

==========================================
Option<T> and Result<T, E>
==========================================

.. index::
    single: Tutorial; Option
    single: Tutorial; Result
    single: Tutorial; Monadic types
    single: Tutorial; Error handling

``daslib/option`` and ``daslib/result`` are two small template-structure modules
that give daslang a principled way to express "value or nothing" and "value or
error" for ordinary value types. They compose through the existing ``|>`` pipe.

``Option<T>`` models **absence**. ``Result<T, E>`` models **failure with a
reason**. Prefer them over sentinel return values (``-1``, ``""``) or nullable
pointers (``T?``).

The payload may be any type, including non-copyable ones (``array<T>``,
``table<K;V>``, lambdas) — see `Non-copyable payloads`_.

Prerequisites: familiarity with template structures, blocks, and the pipe
operator ``|>``.

.. code-block:: das

    options gen2

    require daslib/option
    require daslib/result


Option<T>
==========

Construction
------------

``some(v)`` wraps a value; the payload type is inferred from ``v``.
``none(type<T>)`` declares an empty option — the type witness is required
because there is no value to infer from.

.. code-block:: das

    let a = some(42)              // Option<int>
    let b = none(type<int>)       // Option<int>, empty
    let c = some("hello")         // Option<string>

Queries
-------

``is_some(o)`` and ``is_none(o)`` return ``bool``. Pipe them through ``|>`` for
readability.

.. code-block:: das

    if (a |> is_some) {
        // ...
    }

Transforming values — ``map`` and ``and_then``
----------------------------------------------

``map`` applies a block to the payload and re-wraps the result.  ``none``
propagates unchanged.

.. code-block:: das

    let doubled = some(21) |> map() $(x : int) { return x * 2; }
    // doubled |> unwrap == 42

``and_then`` is the monadic bind — the block itself returns an ``Option``.  Use
it to chain fallible steps; the chain short-circuits on the first ``none``.

.. code-block:: das

    def safe_parse_positive(s : string) : Option<int> {
        if (s == "1") { return some(1); }
        return none(type<int>)
    }

    let r = some("1") |> and_then() $(s : string) { return safe_parse_positive(s); }
    // r |> unwrap_or(-1) == 1

Filtering and fallbacks
-----------------------

.. code-block:: das

    let kept = some(10) |> filter() $(x : int) { return x > 5; }  // some(10)
    let gone = some(3)  |> filter() $(x : int) { return x > 5; }  // none

    let eager = none(type<int>) |> or_value(99)                    // some(99)
    let lazy  = none(type<int>) |> or_else() $ { return some(expensive()); }

Extraction
----------

.. code-block:: das

    some(5) |> unwrap              // 5   — panics on none
    none(type<int>) |> unwrap_or(7)   // 7
    some(5) |> unwrap_or_else() $ { return compute(); }   // 5 (block not called)
    none(type<string>) |> unwrap_or_default                // ""

Operators
---------

``??`` reads like "unwrap or this default":

.. code-block:: das

    some(5) ?? 0              // 5
    none(type<int>) ?? 42     // 42

``==`` is structural — same tag, and when ``some``, equal payloads:

.. code-block:: das

    some(5) == some(5)                       // true
    some(5) == some(6)                       // false
    none(type<int>) == none(type<int>)       // true

Side-effect combinators
-----------------------

.. code-block:: das

    some(3) |> if_some() $(x : int) { do_work(x); }
    none(type<int>) |> if_none() $ { report_missing(); }

Pairing two options with ``zip``
--------------------------------

.. code-block:: das

    let paired = zip(some(1), some(2.5f))    // some(tuple(1, 2.5f))
    let nope   = zip(some(1), none(type<float>))   // none


Result<T, E>
=============

Construction
------------

``ok(v, type<E>)`` and ``err(e, type<T>)`` build the two sides.  The second
argument is a type witness for the "other" side:

.. code-block:: das

    let r = ok(42, type<string>)      // Result<int, string>, ok
    let e = err("boom", type<int>)    // Result<int, string>, err

Queries
-------

.. code-block:: das

    r |> is_ok       // true
    e |> is_err      // true

Transforming
------------

``map`` rewrites the ok payload; ``map_err`` rewrites the err payload.  The
other side passes through unchanged.

.. code-block:: das

    let doubled = ok(3, type<string>) |> map() $(x : int) { return x * 2; }
    // doubled |> unwrap == 6

    let wrapped = err("boom", type<int>) |> map_err() $(s : string) { return "<{s}>"; }
    // wrapped |> unwrap_err == "<boom>"

Monadic chaining — ``and_then`` and ``or_else``
-----------------------------------------------

``and_then`` chains ok values through more fallible computation; ``or_else``
recovers from errors.

.. code-block:: das

    def parse_digit(s : string) : Result<int; string> {
        if (s == "1") { return ok(1, type<string>); }
        return err("not a digit: {s}", type<int>)
    }

    let good = ok("1", type<string>) |> and_then() $(s : string) { return parse_digit(s); }
    // good |> unwrap_or(-1) == 1

    let recovered = err("boom", type<int>) |> or_else() $(e : string) {
        return ok(99, type<string>)
    }
    // recovered |> unwrap == 99

Extraction
----------

Same shape as ``Option``, plus ``unwrap_err`` for the error side:

.. code-block:: das

    ok(42, type<string>) |> unwrap                 // 42
    err("e", type<int>) |> unwrap_or(17)           // 17
    err("xx", type<int>) |> unwrap_or_else() $(e : string) { return length(e); }  // 2
    err("boom", type<int>) |> unwrap_err           // "boom"

Bridging to Option
------------------

``to_option`` discards the error; ``err_to_option`` discards the value:

.. code-block:: das

    ok(42, type<string>) |> to_option           // some(42)
    err("x", type<int>)  |> to_option           // none
    err("x", type<int>)  |> err_to_option       // some("x")


Non-copyable payloads
======================

``Option<T>`` and ``Result<T, E>`` work for any payload type, including
non-copyable ones such as ``array<T>``, ``table<K;V>``, and lambdas. The
constructors and combinators dispatch internally on
``static_if (typeinfo can_copy(...))`` and clone (or move) on the non-copyable
branch — call sites look identical to the workhorse-type case.

.. code-block:: das

    var arr <- [1, 2, 3]
    let o = some(arr)               // arr is cloned; arr is still [1, 2, 3]
    let n = o |> map() $(v : array<int>) { return length(v); }
    // n |> unwrap == 3

When you want to **move** a non-copyable payload into the option (instead of
cloning it), use ``move_some``. After ``move_some(src)`` the source is left
empty:

.. code-block:: das

    var src <- [1, 2, 3]
    let o = move_some(src)          // src is now []
    // o |> unwrap == [1, 2, 3]

``Result`` provides the same pair on each side: ``ok`` / ``err`` clone, while
``move_ok`` / ``move_err`` move:

.. code-block:: das

    var payload <- [4, 5, 6]
    let r = move_ok(payload, type<string>)   // payload is now []
    // r |> unwrap == [4, 5, 6]

For workhorse types (``int``, ``float``, ``bool``, ``string``, …) ``move_some`` /
``move_ok`` / ``move_err`` are equivalent to their non-``move`` siblings —
prefer the plain form for readability and use the move-variants only when
you genuinely want to drain a non-copyable source.


API reference at a glance
==========================

Option<T>
---------

+-------------------------------+-------------------------------------------------+
| Function                      | Meaning                                         |
+===============================+=================================================+
| ``some(v)``                   | Wrap a value (clones non-copyable payload)      |
+-------------------------------+-------------------------------------------------+
| ``move_some(v)``              | Wrap a value by moving out of a mutable source  |
+-------------------------------+-------------------------------------------------+
| ``none(type<T>)``             | Empty option of given payload type              |
+-------------------------------+-------------------------------------------------+
| ``is_some(o)``, ``is_none(o)``| Tag queries                                     |
+-------------------------------+-------------------------------------------------+
| ``map(o, f)``                 | Transform payload; ``none`` passes through      |
+-------------------------------+-------------------------------------------------+
| ``and_then(o, f)``            | Monadic bind                                    |
+-------------------------------+-------------------------------------------------+
| ``filter(o, p)``              | Drop ``some`` when predicate fails              |
+-------------------------------+-------------------------------------------------+
| ``or_value(o, v)``            | Eager fallback on ``none``                      |
+-------------------------------+-------------------------------------------------+
| ``or_else(o, f)``             | Lazy fallback on ``none``                       |
+-------------------------------+-------------------------------------------------+
| ``unwrap(o)``                 | Value or panic                                  |
+-------------------------------+-------------------------------------------------+
| ``unwrap_or(o, d)``           | Value or eager default                          |
+-------------------------------+-------------------------------------------------+
| ``unwrap_or_else(o, f)``      | Value or computed default                       |
+-------------------------------+-------------------------------------------------+
| ``unwrap_or_default(o)``      | Value or ``default<T>``                         |
+-------------------------------+-------------------------------------------------+
| ``expect_value(o, msg)``      | Value or panic with custom message              |
+-------------------------------+-------------------------------------------------+
| ``if_some(o, f)``             | Side effect on ``some``                         |
+-------------------------------+-------------------------------------------------+
| ``if_none(o, f)``             | Side effect on ``none``                         |
+-------------------------------+-------------------------------------------------+
| ``zip(a, b)``                 | Pair two options; some iff both some            |
+-------------------------------+-------------------------------------------------+
| ``o ?? d``                    | Unwrap-or-default operator                      |
+-------------------------------+-------------------------------------------------+
| ``a == b``                    | Structural equality                             |
+-------------------------------+-------------------------------------------------+
| ``o == v``, ``v == o``        | True iff ``some(x)`` and ``x == v``             |
+-------------------------------+-------------------------------------------------+
| ``o != v``, ``v != o``        | Negation of the above                           |
+-------------------------------+-------------------------------------------------+

Result<T, E>
------------

+---------------------------------+-------------------------------------------------+
| Function                        | Meaning                                         |
+=================================+=================================================+
| ``ok(v, type<E>)``              | Wrap a success value (clones non-copyable)      |
+---------------------------------+-------------------------------------------------+
| ``move_ok(v, type<E>)``         | Wrap success by moving out of a mutable source  |
+---------------------------------+-------------------------------------------------+
| ``err(e, type<T>)``             | Wrap an error (clones non-copyable)             |
+---------------------------------+-------------------------------------------------+
| ``move_err(e, type<T>)``        | Wrap error by moving out of a mutable source    |
+---------------------------------+-------------------------------------------------+
| ``is_ok(r)``, ``is_err(r)``     | Tag queries                                     |
+---------------------------------+-------------------------------------------------+
| ``map(r, f)``                   | Transform ok payload                            |
+---------------------------------+-------------------------------------------------+
| ``map_err(r, f)``               | Transform err payload                           |
+---------------------------------+-------------------------------------------------+
| ``and_then(r, f)``              | Monadic bind on ok                              |
+---------------------------------+-------------------------------------------------+
| ``or_else(r, f)``               | Monadic recovery on err                         |
+---------------------------------+-------------------------------------------------+
| ``unwrap(r)``                   | Ok value or panic                               |
+---------------------------------+-------------------------------------------------+
| ``unwrap_or(r, d)``             | Ok value or eager default                       |
+---------------------------------+-------------------------------------------------+
| ``unwrap_or_else(r, f)``        | Ok value or computed-from-error default         |
+---------------------------------+-------------------------------------------------+
| ``unwrap_or_default(r)``        | Ok value or ``default<T>``                      |
+---------------------------------+-------------------------------------------------+
| ``unwrap_err(r)``               | Err value or panic                              |
+---------------------------------+-------------------------------------------------+
| ``expect_value(r, msg)``        | Ok value or panic with custom message           |
+---------------------------------+-------------------------------------------------+
| ``expect_err(r, msg)``          | Err value or panic with custom message          |
+---------------------------------+-------------------------------------------------+
| ``if_ok(r, f)``, ``if_err(r,f)``| Side effect on either side                      |
+---------------------------------+-------------------------------------------------+
| ``to_option(r)``                | Discard error; ``Option<T>``                    |
+---------------------------------+-------------------------------------------------+
| ``err_to_option(r)``            | Discard value; ``Option<E>``                    |
+---------------------------------+-------------------------------------------------+
| ``r ?? d``                      | Unwrap-or-default operator                      |
+---------------------------------+-------------------------------------------------+
| ``a == b``                      | Structural equality                             |
+---------------------------------+-------------------------------------------------+
| ``r == v``, ``v == r``          | True iff ``ok(x)`` and ``x == v``               |
+---------------------------------+-------------------------------------------------+
| ``r != v``, ``v != r``          | Negation of the above                           |
+---------------------------------+-------------------------------------------------+


.. seealso::

    Full source: :download:`tutorials/language/52_option_and_result.das <../../../../tutorials/language/52_option_and_result.das>`

    Previous tutorial: :ref:`tutorial_delegate`

    Next tutorial: :ref:`tutorial_clargs`

    :ref:`tutorial_error_handling`, :ref:`tutorial_generics`, :ref:`tutorial_pattern_matching`
