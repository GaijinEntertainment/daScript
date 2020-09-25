.. _tables:


=====
Table
=====

.. index::
    single: Tables

Tables are associative containers implemented as a set of key/value pairs::

    var tab: table<string; int>
    tab["10"] = 10
    tab["20"] = 20
    tab["some"] = 10
    tab["some"] = 20  // replaces the value for 'some' key


List of relevant builtin functions: clear, key_exists, find, erase.
for safety, find doesn't return anything. Instead it works with block as last argument. It can be worked with rbpipe operator ::

    var tab: table<string; int>
    tab["some"] = 10
    find(tab,"some") <| $(var pValue: int? const)
        if pValue != null
            assert(deref(pValue) == 10)

It is done so, because otherwise find would have to return pointer to value, which would continue to point 'somewhere', even if data is deleted.
Consider this hypothetical find and the following example ::

    var tab: table<string; int>
    tab["some"] = 10
    var v: int? = find(tab,"some")
    assert(v)      // not null!
    tab |> clear()
    deref(v) = 10  // where we will write this 10? UB and segfault!

So, if you just want to check for existence of key in table, use key_exists(table, key).

Tables (as well as arrays, structs, and handled types) will be passed to functions by reference only.

Tables can not be assigned, only cloned or moved. ::

  def clone_table(var a, b: table<string, int>)
    a := b      // a is not deep copy of b
    clone(a, b) // same as above
    a = b       // error

  def move_table(var a, b: table<string, int>)
    a <- b  //a is no points to same data as b, and b is empty.

Table key can be not only string, but any other 'workhorse' type as well.

Tables can be constructed inline ::

	let tab <- {{ "one"=>1; "two"=>2 }}

Which is syntax sugar for ::

	let tab : table<string;int> <- to_table_move([[tuple<string;int>[2] "one"=>1; "two"=>2]])



