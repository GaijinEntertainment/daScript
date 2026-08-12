The STRINGS module implements string formatting, conversion, searching, and modification
routines. It provides functions for building strings (``build_string``), parsing
(``to_int``, ``to_float``), character classification (``is_alpha``, ``is_number``),
and low-level string manipulation.

Most searching, slicing and parsing functions come in two forms. One takes a ``string``.
The other takes a byte view — an ``array<uint8>``, which is what ``peek_data`` hands to its
block. A view carries its own length, so the view form never scans for a terminating NUL:
one ``strlen`` at ``peek_data`` covers the whole loop, and a NUL inside the bytes is data
like any other byte. The forms that produce text (``slice``, ``chop``, ``strip``, ``trim``,
``write_string``) copy the bytes out, so the result is an ordinary temporary string.
Offsets are ``int``, so a view longer than ``INT_MAX`` bytes panics instead of answering
with a wrapped offset.

Whitespace means the C ``isspace`` set throughout this module — space, tab, CR, LF, FF and
VT. ``strip``, ``trim``, the parse functions and ``is_white_space`` all use that one set.

All functions and symbols are in "strings" module, use require to get access to it.

.. code-block:: das

    require strings

Example:

.. code-block:: das

    require strings

    [export]
    def main() {
        peek_data("key = value") $(d) {
            let eq = find(d, '=')
            print("[{trim(slice(d, 0, eq))}] -> [{trim(slice(d, eq + 1))}]\n")
        }
    }
    // output:
    // [key] -> [value]
