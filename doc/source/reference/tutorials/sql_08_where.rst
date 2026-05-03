.. _tutorial_sql_where:

==================================================
SQL-08 --- ``_where`` Predicates: the Full Surface
==================================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; _where
    single: Tutorial; LIKE
    single: Tutorial; starts_with
    single: Tutorial; ends_with
    single: Tutorial; contains

Inside ``_sql(...)``, ``_where(predicate)`` accumulates a ``WHERE``
clause via a recursive AST walk. Multiple ``_where`` calls compose
with ``AND``. This tutorial walks every predicate shape the analyzer
recognizes; anything outside this surface raises a compile-time
``macro_error`` pointing at the offending node.

Recognized predicate shapes
===========================

============================================  ============================================================
Source shape                                  Lowered SQL
============================================  ============================================================
``_.FieldName``                               column reference (quoted identifier)
``someVar``, ``42``, ``"alice"``              bind parameter (``?``)
``==``, ``!=``                                ``=``, ``<>``
``<``, ``<=``, ``>``, ``>=``                  same operators
``&&``, ``||``, ``!``                         ``AND``, ``OR``, ``NOT``
``s |> starts_with(p)``                       ``s LIKE ? || '%'``        (``p`` bound)
``s |> ends_with(p)``                         ``s LIKE '%' || ?``        (``p`` bound)
``s |> contains(p)``                          ``s LIKE '%' || ? || '%'`` (``p`` bound)
``s |> to_lower()``, ``s |> to_upper()``      ``LOWER(s)``, ``UPPER(s)``
``length(s)``                                 ``LENGTH(s)``
``x |> abs()``                                ``ABS(x)``
============================================  ============================================================

Captured-variable equality
==========================

The simplest pattern --- a free variable on one side, a column ref on
the other:

.. code-block:: das

    let target = 3
    let one <- _sql(db |> select_from(type<Car>)
                      |> _where(_.Id == target))
    // emits:  ... WHERE "Id" = ?     binds: [3]

Composition with multiple ``_where``
====================================

Each ``_where`` adds an AND-clause; chain freely:

.. code-block:: das

    let cheap_F <- _sql(db |> select_from(type<Car>)
                          |> _where(_.Price < 1000)
                          |> _where(_.Name |> starts_with("F")))
    // emits:  ... WHERE "Price" < ? AND "Name" LIKE ? || '%'
    // binds:  [1000, "F"]

Boolean operators
=================

``&&``, ``||``, ``!`` lower to SQL ``AND`` / ``OR`` / ``NOT``. Use
parentheses to disambiguate precedence:

.. code-block:: das

    let cheap_or_F <- _sql(db |> select_from(type<Car>)
                             |> _where(_.Price < 100
                                       || (_.Name |> starts_with("F"))))

String-tests via ``LIKE``
=========================

``starts_with`` / ``ends_with`` / ``contains`` lower to a ``LIKE``
pattern:

.. code-block:: das

    let ends_da   <- _sql(db |> select_from(type<Car>)
                            |> _where(_.Name |> ends_with("da")))
    let contains_o <- _sql(db |> select_from(type<Car>)
                             |> _where(_.Name |> contains("o")))

Case folding
============

``to_lower`` / ``to_upper`` lower to ``LOWER`` / ``UPPER``:

.. code-block:: das

    let case_match <- _sql(db |> select_from(type<Car>)
                             |> _where(_.Name |> to_lower() == "ford"))

String length and numeric scalar
================================

``length(s)`` and ``x |> abs()`` lower to ``LENGTH(...)`` and
``ABS(...)``:

.. code-block:: das

    let long_names <- _sql(db |> select_from(type<Car>)
                             |> _where(length(_.Name) > 5))

    let dear <- _sql(db |> select_from(type<Car>)
                       |> _where((_.Price |> abs()) > 5000))

Inspecting the emitted SQL
==========================

``_sql_text`` returns the SQL the macro would emit instead of running
it --- useful when the chain grows complex:

.. code-block:: das

    let sql = _sql_text(db |> select_from(type<Car>)
                          |> _where((_.Price |> abs()) > 5000))
    // sql == 'SELECT "Id", "Name", "Price" FROM "Cars" WHERE ABS("Price") > ?'

Untranslatable predicates
=========================

If you write a shape the analyzer doesn't recognize (a user-defined
function, a math op without a builtin lowering, a regex), the macro
raises ``macro_error`` at compile time pointing at the offending
node. For those cases, drop down to the raw-SQL escape hatch
(``query_one``, ``query_scalar``, ``exec``) covered in
:ref:`tutorial_sql_parametrized`.

.. seealso::

    Full source: :download:`tutorials/sql/08-where.das <../../../../tutorials/sql/08-where.das>`

    Previous tutorial: :ref:`tutorial_sql_anatomy`

    Next tutorial: :ref:`tutorial_sql_select`
