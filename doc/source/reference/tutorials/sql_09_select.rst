.. _tutorial_sql_select:

==================================
SQL-09 --- ``_select`` Projections
==================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; _select
    single: Tutorial; projection
    single: Tutorial; named tuple
    single: Tutorial; tuple recordNames

``_select(...)`` controls *what* columns the SQL emits and *what shape*
the result has. Three forms ship in chunk 3:

==============================================================  ==========================================================
Form                                                            Result shape
==============================================================  ==========================================================
default (no ``_select``)                                        ``array<Car>`` --- all columns, source struct
``_select(_.FieldName)``                                        ``array<FieldType>`` --- one column, that column's type
``_select((Name=_.Name, Price=_.Price))``                       ``array<tuple<Name:string;Price:int>>`` --- named tuple
==============================================================  ==========================================================

Default projection: full row
============================

No ``_select`` --- the macro emits ``SELECT`` of every column declared
in the ``[sql_table]`` struct, in declaration order, and materializes
each row as the source struct:

.. code-block:: das

    let cars <- _sql(db |> select_from(type<Car>))
    // emits:  SELECT "Id", "Name", "Price" FROM "Cars"
    // result: array<Car>

Single-column projection
========================

``_select(_.Field)`` projects exactly one column. The result element
type matches the column's daslang type (``int``, ``string``, ...):

.. code-block:: das

    let names <- _sql(db |> select_from(type<Car>) |> _select(_.Name))
    // emits:  SELECT "Name" FROM "Cars"
    // result: array<string>

Named-tuple projection
======================

``_select((Name=_.Name, Price=_.Price))`` projects multiple columns
into a daslang ``tuple`` whose ``recordNames`` preserve the chosen
names. You read tuple fields by the chosen name --- the source field
name no longer matters at the use site:

.. code-block:: das

    let pairs <- _sql(db |> select_from(type<Car>)
                        |> _select((Name=_.Name, Price=_.Price)))
    // emits:  SELECT "Name", "Price" FROM "Cars"
    // result: array<tuple<Name:string;Price:int>>

    for (p in pairs) {
        to_log(LOG_INFO, "  {p.Name} (price={p.Price})\n")
    }

The recordNames live on the result tuple's ``TypeDecl.argNames``;
``build_row_builder`` constructs that ``recordType`` explicitly because
the ``ExprMakeTuple``'s own recordNames vector isn't bound to daslang
yet.

Renaming via named tuple
========================

Because the tuple's recordNames are independent of the source columns,
you can rename freely. The result fields can shadow keywords or just
match the domain language better than the SQL column names do:

.. code-block:: das

    let renamed <- _sql(db |> select_from(type<Car>)
                          |> _select((Identifier=_.Id, Label=_.Name)))
    // emits:  SELECT "Id", "Name" FROM "Cars"
    // result: array<tuple<Identifier:int;Label:string>>

    for (r in renamed) {
        to_log(LOG_INFO, "  Identifier={r.Identifier} Label={r.Label}\n")
    }

Combining with terminals
========================

``_select`` composes with ``_first`` / ``_first_opt`` / ``count`` /
``_where`` exactly as you'd expect --- it's just another chain stage:

.. code-block:: das

    let head = _sql(db |> select_from(type<Car>)
                      |> _select((Name=_.Name, Price=_.Price))
                      |> _first())
    // result: tuple<Name:string;Price:int>

Inspecting the emitted SQL
==========================

``_sql_text`` returns the SQL the macro would emit:

.. code-block:: das

    let sql = _sql_text(db |> select_from(type<Car>)
                          |> _select((Name=_.Name, Price=_.Price)))
    // sql == 'SELECT "Name", "Price" FROM "Cars"'

Quick reference
===============

==============================================================  ==========================================================
Form                                                            Description
==============================================================  ==========================================================
default (no ``_select``)                                        Full row; ``array<T>``
``_select(_.Field)``                                            Single column; ``array<FieldType>``
``_select((Name=_.X, Other=_.Y))``                              Named tuple; ``array<tuple<Name:..;Other:..>>``
``_select((Renamed=_.Id, ...))``                                Result fields can rename source columns
==============================================================  ==========================================================

Deferred to chunk 4: struct-type projection ``_select(type<T2>)`` for
projecting into a different ``[sql_table]`` struct. Today users get
the same result via named-tuple projection.

.. seealso::

    Full source: :download:`tutorials/sql/09-select.das <../../../../tutorials/sql/09-select.das>`

    Previous tutorial: :ref:`tutorial_sql_where`

    Next tutorial: :ref:`tutorial_sql_order_by`
