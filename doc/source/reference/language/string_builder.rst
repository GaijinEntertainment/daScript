.. _string_builder:


==============
String Builder
==============

.. index::
    single: String Builder

Daslang provides built-in string interpolation for constructing strings from
expressions at runtime.  Any string literal can contain embedded expressions
enclosed in curly brackets ``{}``, which are evaluated, converted to text,
and spliced into the resulting string.

This approach is more readable, compact, and type-safe than ``printf``-like
formatting.

---------------------
Basic Usage
---------------------

Embed any expression inside ``{`` and ``}`` within a string literal:

.. code-block:: das

    let name = "world"
    let greeting = "Hello, {name}!"          // "Hello, world!"
    let result = "1 + 2 = {1 + 2}"          // "1 + 2 = 3"

The expression inside ``{}`` can be arbitrarily complex:

.. code-block:: das

    var items : array<string>
    push(items, "apple")
    push(items, "banana")
    print("count = {length(items)}, first = {items[0]}\n")

If the expression is a compile-time constant, the string builder result is
computed at compile time.

---------------------
Supported Types
---------------------

Any type can appear inside ``{}``, including:

* Numeric types (``int``, ``uint``, ``float``, ``double``, ``int64``, ``uint64``)
* Booleans
* Strings
* Enumerations (printed as their name)
* Vectors (``float2``, ``int3``, etc.)
* Structures and classes (via the Data Walker)
* Handled (extern) types, provided they implement ``DataWalker``

All built-in POD types have ``DataWalker`` support by default.

---------------------
Format Specifiers
---------------------

A format specifier can be added after a colon ``:`` to control the output
representation of the interpolated value:

.. code-block:: das

    let pi = 3.14159
    print("pi = {pi:5.2f}\n")          // fixed-point, 5 wide, 2 decimals

Specifiers use the libfmt (Python ``format``) replacement-field syntax, **not** C
``printf`` syntax: the compiler rewrites ``{expression:spec}`` into
``fmt(":spec", expression)``. The general form is:

.. code-block:: text

    {expression:[[fill]align][sign][#][0][width][.precision][type]}

Where:

* **fill / align** — ``<`` left-align, ``>`` right-align, ``^`` center, each optionally
  preceded by the character to pad with (``{v:*>8}``)
* **sign** — ``+`` prints a sign for positive numbers too; ``-`` (the default) only for
  negative ones; a space puts a space where ``+`` would go
* ``#`` — alternate form: ``0x`` / ``0b`` / ``0`` prefix for hex, binary, octal
* ``0`` — zero-pad to the field width
* **width** — minimum field width
* **precision** — number of decimal places, for floating-point
* **type** — conversion character:
    * ``d`` — decimal integer
    * ``b`` or ``B`` — binary
    * ``o`` — octal
    * ``x`` / ``X`` — hexadecimal (lowercase / uppercase)
    * ``c`` — the integer's character
    * ``f`` or ``F`` — fixed-point decimal
    * ``e`` / ``E`` — scientific notation (lowercase / uppercase)
    * ``g`` / ``G`` — general (shortest of ``f`` or ``e``)
    * ``a`` / ``A`` — hexadecimal floating-point

A specifier only applies to numeric values — ``fmt`` has no ``string`` or ``bool``
overload, so ``"{name:>8}"`` fails to compile with ``error[30341]``; pad text with
``pad_left`` / ``pad_right`` from ``daslib/strings_boost``. A type character libfmt does
not know — ``i`` and ``u``, which C ``printf`` accepts, among them — panics at runtime
with ``fmt error: invalid format specifier``.

Examples:

.. code-block:: das

    print("{42:08x}\n")            // "0000002a"  — 8-digit zero-padded hex
    print("{42:08X}\n")            // "0000002A"  — uppercase hex
    print("{3.14159:.2f}\n")       // "3.14"      — 2 decimal places
    print("{7:+d}\n")              // "+7"        — sign forced on a positive number
    print("{255:#x}\n")            // "0xff"      — with 0x prefix
    print("{42:*>8}\n")            // "******42"  — right-aligned, '*' fill

-----------------------
Escaping Curly Brackets
-----------------------

To include a literal ``{`` or ``}`` in a string, escape them with a backslash:

.. code-block:: das
    :force:

    print("Use \{curly\} brackets\n")   // prints: Use {curly} brackets

---------------------
Multi-line Strings
---------------------

String interpolation works in multi-line (heredoc) strings as well:

.. das-doc: given var value1, value2, value3 : int

.. code-block:: das

    let msg = "Line 1: {value1}
    Line 2: {value2}
    Line 3: {value3}"

---------------------
Implementation Notes
---------------------

String builder expressions are compiled as a sequence of ``write`` calls to an
internal string buffer.  This means:

* Each ``{}`` expression creates one write call, not a separate string allocation.
* The entire string builder expression produces a single temporary string.
* If all parts are compile-time constants, the result is folded into a single
  string constant.

---------------------
Relationship to print
---------------------

The ``print`` function accepts string builder strings directly:

.. das-doc: given var x, y : int

.. code-block:: das

    print("x = {x}, y = {y}\n")

This is the most common use of string builders.  The ``print`` function will
output the result using the host application's output handler.

.. seealso::

    :ref:`Datatypes <datatypes_and_values>` for the built-in types supported inside ``{}``,
    :ref:`Expressions <expressions>` for expression syntax used in string interpolation.
