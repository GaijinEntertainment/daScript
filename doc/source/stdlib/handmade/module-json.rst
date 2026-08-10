The JSON module implements JSON parsing and serialization.
It provides ``read_json`` for parsing JSON text into a ``JsonValue`` tree,
``write_json`` for serializing back to text, and ``JV`` helpers for constructing
JSON values from daslang types. ``write_json`` pretty-prints (one element per
line, tab-indented); ``write_json_compact`` emits the same tree on a single line.

See also :doc:`json_boost` for automatic struct-to-JSON conversion and the ``%json~`` reader macro.
See :ref:`tutorial_json` for a hands-on tutorial.

All functions and symbols are in "json" module, use require to get access to it.

.. code-block:: das

    require daslib/json

Example:

.. code-block:: das

    require daslib/json

    [export]
    def main() {
        let data = "[1, 2, 3]"
        var error = ""
        var js <- read_json(data, error)
        print("compact: {write_json_compact(js)}\n")
        print("pretty: {write_json(js)}\n")
        unsafe {
            delete js
        }
    }
    // output:
    // compact: [1, 2, 3]
    // pretty: [
    //     1,
    //     2,
    //     3
    // ]
