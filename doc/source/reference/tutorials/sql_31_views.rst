.. _tutorial_sql_views:

==========================================
SQL-31 --- Views
==========================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; sql_view
    single: Tutorial; views
    single: Tutorial; create view

A view is a saved ``SELECT``: a virtual table whose rows are produced
on demand by re-running its body. dasSQLITE represents a view with the
same struct-driven story as a table --- declare a struct describing the
projected columns, attach ``[sql_view]``, and the same read-side helpers
work without any view-specific dispatch.

The contract
============

* ``[sql_view(name = "...")]`` on a struct registers it as a view shape.
  Field annotations are the read-only subset of ``[sql_table]``:
  ``@sql_column = "..."`` (rename) and ``@sql_json`` / ``@sql_blob``
  (non-scalar storage). DDL-affecting annotations on the underlying
  table --- keys, uniqueness, computed columns, defaults, foreign keys
  --- are rejected at compile time.
* ``_create_view(type<V>, chain)`` builds the ``CREATE VIEW`` from a
  LINQ-shaped chain. The macro walks it, validates the projection
  against ``V``'s fields (count + per-position type), and emits a
  ``db |> exec`` of ``CREATE VIEW name(c1, ...) AS SELECT ...``.
* Reading the view uses ``select_from(type<V>)`` --- the same call site
  as a table. ``column_info(type<V>)`` works too (note ``is_pk`` is
  always ``false`` for views).
* ``drop_view_if_exists(type<V>)`` is the idempotent teardown; the
  ``try_`` form returns ``SqlError`` instead of panicking.

Mutation paths
==============

Views are read-only. Any mutation attempt is blocked:

* **Compile-time rejection.** ``_sql_update(type<V>, ...)``,
  ``_sql_delete(type<V>, ...)``, and ``_sql_upsert(viewRow, ...)`` fail
  with: *"cannot mutate [sql_view] '<name>' --- views are read-only.
  Mutate the underlying table(s) and re-query the view."*
* **Runtime rejection.** Row-form mutations
  (``insert(viewRow)`` / ``update(...)`` / ``delete_(...)``) panic with
  the same message.

Real mutating workflows update the underlying tables and re-query the
view to see the new state.

End-to-end
==========

.. code-block:: das

    [sql_table(name = "Customers")]
    struct Customer {
        @sql_primary_key Id : int
        Name : string
        City : string
    }

    [sql_table(name = "Orders")]
    struct Order {
        @sql_primary_key Id : int
        @sql_references = "Customer" CustomerId : int
        Amount : int
        Status : string
    }

    [sql_view(name = "BigOrders")]
    struct BigOrder {
        Id : int
        CustomerId : int
        Amount : int
        Status : string
    }

    [export]
    def main {
        with_sqlite(":memory:") $(db) {
            db |> create_table(type<Customer>)
            db |> create_table(type<Order>)
            // ... insert rows ...

            // CREATE VIEW BigOrders(Id, CustomerId, Amount, Status) AS
            //   SELECT Id, CustomerId, Amount, Status FROM Orders
            //   WHERE Amount >= 100
            db |> _create_view(type<BigOrder>,
                db |> select_from(type<Order>) |> _where(_.Amount >= 100))

            for (b in db |> select_from(type<BigOrder>)) {
                to_log(LOG_INFO, "BigOrder #{b.Id}: amt={b.Amount}\n")
            }
        }
    }

A second view with a join + named-tuple projection follows the same
shape; the view's struct supplies the column names and the
``_select((... = ...))`` aliases just have to match by position and
type:

.. code-block:: das

    [sql_view(name = "OrderSummary")]
    struct OrderSummary {
        OrderId  : int
        Customer : string
        Amount   : int
    }

    db |> _create_view(type<OrderSummary>,
        db |> select_from(type<Order>)
            |> _join(db |> select_from(type<Customer>),
                     $(o : Order, c : Customer) => o.CustomerId == c.Id,
                     $(o : Order, c : Customer) =>
                        (OrderId = o.Id, Customer = c.Name, Amount = o.Amount)))

Bound parameters are rejected
=============================

SQLite stores view bodies as text in ``sqlite_schema``, so ``?``
placeholders are not allowed inside a view definition. ``_create_view``
rejects chains that reference captured locals at compile time:

.. code-block:: das

    let cutoff = 100
    db |> _create_view(type<BigOrder>,                       // compile error
        db |> select_from(type<Order>) |> _where(_.Amount >= cutoff))

The fix is to inline literals into the view body and apply runtime
filtering at the query site:

.. code-block:: das

    db |> _create_view(type<BigOrder>,
        db |> select_from(type<Order>) |> _where(_.Amount >= 100))

    let recent <- _sql(db |> select_from(type<BigOrder>)
                         |> _where(_.Amount >= cutoff))      // bind here

Raw-SQL escape hatch
====================

``_create_view`` accepts the same chain shapes as ``_sql`` (single
source or join, FullRow / NamedTuple / GroupedNamedTuple /
SingleColumn / Aggregate). Anything outside that --- window functions,
recursive CTEs, custom SQL functions --- needs raw
``db |> exec("CREATE VIEW ... AS ...")``. The view is queryable
through the same ``select_from(type<V>)`` rail as long as a matching
``[sql_view]`` struct is declared:

.. code-block:: das

    db |> exec(
        "CREATE VIEW IF NOT EXISTS CustomerRank AS " +
        "SELECT CustomerId, " +
        "       ROW_NUMBER() OVER (ORDER BY SUM(Amount) DESC) AS Rank, " +
        "       SUM(Amount) AS Total " +
        "FROM Orders GROUP BY CustomerId")

Production tip
==============

In production, view DDL belongs in a migration body so the schema
lives alongside table DDL. The migration story ships in a future
chunk; for now the inline ``_create_view`` form is fine for app setup
and tests.

.. seealso::

    Full source: :download:`tutorials/sql/31-views.das <../../../../tutorials/sql/31-views.das>`

    Previous tutorial: :ref:`tutorial_sql_list_tables`

    Next tutorial: :ref:`tutorial_sql_register_function`
