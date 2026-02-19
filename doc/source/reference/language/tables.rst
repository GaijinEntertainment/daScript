.. _tables:


=====
Table
=====

.. index::
    single: Tables

Tables are associative containers implemented as a set of key/value pairs:

.. code-block:: das

    var tab: table<string,int>
    unsafe {
        tab["10"] = 10
        tab["20"] = 20
        tab["some"] = 10
        tab["some"] = 20  // replaces the value for 'some' key
    }

Accessing a table element via the index operator is unsafe, because Daslang containers store unboxed values.
Consider the following example:

.. code-block:: das

    tab["1"] = tab["2"]               // this potentially breaks the table

What happens is table may get resized either after tab["1"] or tab["2"] if either key is missing (similar to C++ STL hash_map).

It is possible to suppress this unsafe error via ``CodeOfPolicies``, or by using the following option:

.. code-block:: das

    options unsafe_table_lookup = false

Safe navigation of the table is safe, since it does not create missing keys:

.. code-block:: das

    var tab <- { "one"=>1, "two"=>2 }
    let t = tab?["one"] ?? -1             // this is safe, since it does not create missing keys
    assert(t==1)

It can also be written to:

.. code-block:: das

    var tab <- { "one"=>1, "two"=>2 }
    var dummy = 0
    tab?["one"] ?? dummy = 10   // if "one" is not in the table, dummy will be set to 10

A collection of safe functions is available for working with tables, even if the table is empty or gets resized.

There are several relevant builtin functions: ``clear``, ``key_exists``, ``get``, and ``erase``.
``get`` works with block as last argument, and returns if value has been found. It can be used with the rbpipe operator:

.. code-block:: das

    let tab <- { "one"=>1, "two"=>2 }
    let found = get(tab,"one") $(val) {
        assert(val==1)
    }
    assert(found)

A non-constant version is available as well:

.. code-block:: das

    var tab <- { "one"=>1, "two"=>2 }
    let found = get(tab,"one") $(var val) {
        val = 123
    }
    let t = tab |> get_value("one")
    assert(t==123)

``insert``, ``insert_clone``, and ``emplace`` are safe ways to add elements to a table:

.. code-block:: das

    var tab <- { "one"=>1, "two"=>2 }
    tab |> insert("three",3)         // insert new key/value pair
    tab |> insert_clone("four",4)    // insert new key/value pair, but clone the value
    var five = 5
    tab |> emplace("five",five)      // insert new key/value pair, but move the value

Tables (as well as arrays, structs, and handled types) are passed to functions by reference only.

Tables cannot be assigned, only cloned or moved.

.. code-block:: das

    def clone_table(var a, b: table<string, int>) {
        a := b      // a is now a deep copy of b
        clone(a, b) // same as above
        a = b       // error
    }

    def move_table(var a, b: table<string, int>) {
        a <- b  // a now points to the same data as b, and b is empty
    }

Table keys can be not only strings, but any other 'workhorse' type as well.

Tables can be constructed inline:

.. code-block:: das

    let tab <- { "one"=>1, "two"=>2 }

This is syntax sugar for:

.. code-block:: das

    let tab : table<string,int> <- to_table_move(fixed_array<tuple<string,int>>(("one",1),("two",2)))

Alternative syntax is:

.. code-block:: das

    let tab <- table("one"=>1, "two"=>2)
    let tab <- table<string,int>("one"=>1, "two"=>2)

A table that holds no associative data can also be declared:

.. code-block:: das

    var tab : table<int>
    tab |> insert(1)        // this is how we insert a key into such table

A table can be iterated over with the ``for`` loop:

.. code-block:: das

    var tab <- { "one"=>1, "two"=>2 }
    for ( key, value in keys(tab), values(tab) ) {
        print("key: {key}, value: {value}\n")
    }

A table that holds no associative data only has keys.

.. seealso::

    :ref:`Datatypes <datatypes_and_values>` for a list of key and value types,
    :ref:`Iterators <iterators>` for ``keys`` and ``values`` iterator patterns,
    :ref:`Move, copy, and clone <clone_to_move>` for table move and clone semantics,
    :ref:`Locks <locks>` for table locking during iteration and find,
    :ref:`Unsafe <unsafe>` for ``unsafe_table_lookup`` and pointer-related operations,
    :ref:`Tuples <tuples>` for the inline table construction syntax.

