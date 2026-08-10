The FLAT_HASH_TABLE module implements a flat hash table — open addressing with
linear probing. ``TFlatHashTable`` keeps keys, hashes, and values in parallel
arrays, which is cache-friendly and performs well for small to medium tables.

All functions and symbols are in "flat_hash_table" module, use require to get access to it.

.. code-block:: das

    require daslib/flat_hash_table

Example:

.. code-block:: das

    require daslib/flat_hash_table public

    typedef IntMap = TFlatHashTable<int; string>

    [export]
    def main() {
        var m <- IntMap()
        m[1] = "one"
        m[2] = "two"
        m[3] = "three"
        print("length = {m.data_length}\n")
        print("m[2] = {m[2]}\n")
        m.clear()
        print("after clear: {m.data_length}\n")
    }
    // output:
    // length = 3
    // m[2] = two
    // after clear: 0
