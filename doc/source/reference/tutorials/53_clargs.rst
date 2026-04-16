.. _tutorial_clargs:

==========================================
Command-Line Argument Parsing (clargs)
==========================================

.. index::
    single: Tutorial; clargs
    single: Tutorial; Command-Line Arguments
    single: Tutorial; CLI Flags
    single: Tutorial; CommandLineArgs

This tutorial covers ``daslib/clargs`` — a structure macro that generates a
type-safe CLI argument parser from an annotated struct.  Declare your flags as
struct fields; the macro generates a ``parse_args`` function and runtime
reflection helpers automatically.

Prerequisites: familiarity with structs, enums, and arrays.

.. code-block:: das

    options gen2

    require daslib/clargs


Defining a CLI args struct
===========================

Annotate any struct with ``[CommandLineArgs]``.  The macro generates three
functions for it:

* ``parse_args(var dst; args : array<string>) : string`` — parse a provided list
* ``parse_args(var dst) : string`` — parse from the process command line
* ``get_command_info(type<T>) : CommandInfo`` — runtime flag metadata

Field names map to flag names with underscores converted to dashes
(``output_file`` → ``--output-file``).

.. code-block:: das

    [CommandLineArgs]
    struct Config {
        name    : string  // --name
        count   : int     // --count
        verbose : bool    // --verbose
        timeout : float   // --timeout
    }

    var cfg = Config()
    let err = parse_args(cfg, ["--name", "Alice", "--count=42", "--verbose", "--timeout=1.5"])
    // err == ""  (success)
    // cfg.name    == "Alice"
    // cfg.count   == 42
    // cfg.verbose == true
    // cfg.timeout == 1.5

Each flag accepts two forms: ``--flag value`` (space-separated) and
``--flag=value`` (equals sign, no space).


Supported field types
======================

+---------------------+-----------------------------------------------+
| Field type          | Accepted flag values                          |
+=====================+===============================================+
| ``string``          | Any string                                    |
+---------------------+-----------------------------------------------+
| ``int``             | Decimal integer (optional leading ``+``/``-``)|
+---------------------+-----------------------------------------------+
| ``float``           | Decimal float with optional exponent          |
+---------------------+-----------------------------------------------+
| ``bool``            | Bare flag (true), ``=true``, or ``=false``    |
+---------------------+-----------------------------------------------+
| ``enum E``          | Enum entry name as a string (e.g. ``"Red"``)  |
+---------------------+-----------------------------------------------+
| ``array<string>``   | Flag may appear multiple times                |
+---------------------+-----------------------------------------------+


Bool flags
===========

A bare ``--verbose`` sets the field to ``true``.  Use ``=true`` or ``=false``
to be explicit:

.. code-block:: das

    parse_args(cfg, ["--verbose"])          // verbose = true
    parse_args(cfg, ["--verbose=false"])    // verbose = false


Enum flags
===========

Pass the enum entry name as a string.  An unknown name returns an error:

.. code-block:: das

    enum LogLevel { Debug; Info; Warning; Error }

    [CommandLineArgs]
    struct LogConfig {
        level : LogLevel  // --level  (accepts "Debug", "Info", "Warning", "Error")
    }

    var cfg = LogConfig()
    parse_args(cfg, ["--level", "Warning"])
    // cfg.level == LogLevel.Warning

    let err = parse_args(cfg, ["--level", "Verbose"])
    // err == "--level: invalid enum value 'Verbose'"


Array flags
============

An ``array<string>`` field collects every occurrence of the flag into the
array.  Both forms (``--tag value`` and ``--tag=value``) are supported:

.. code-block:: das

    [CommandLineArgs]
    struct BuildConfig {
        tags : array<string>
    }

    var cfg = BuildConfig()
    parse_args(cfg, ["--tags=debug", "--tags", "release", "--tags=profile"])
    // cfg.tags == ["debug", "release", "profile"]


Required flags
===============

``@clarg_required`` makes a flag mandatory.  ``parse_args`` returns an error if
the flag is absent:

.. code-block:: das

    [CommandLineArgs]
    struct DeployConfig {
        host : string

        @clarg_required
        token : string
    }

    var cfg = DeployConfig()

    let err1 = parse_args(cfg, ["--host=prod.example.com"])
    // err1 == "--token: missing required flag"

    let err2 = parse_args(cfg, ["--host=prod.example.com", "--token=secret"])
    // err2 == ""  (success)


Field-level attributes
=======================

Three field annotations fine-tune parsing behaviour:

``@clarg_name = "flag"``
    Overrides the auto-generated flag name.

``@clarg_doc = "text"``
    Attaches a description used by help generators (see introspection below).

``@clarg_skip``
    Excludes the field from CLI parsing entirely (set it in code directly).

.. code-block:: das

    [CommandLineArgs]
    struct AppConfig {
        @clarg_name = "output-dir"
        @clarg_doc = "Directory to write output files"
        out_path : string  // flag is --output-dir, not --out-path

        @clarg_doc = "Number of parallel workers (default: 1)"
        workers : int

        @clarg_skip
        internal_id : int  // not a CLI flag
    }

    var cfg = AppConfig()
    cfg.internal_id = 99
    parse_args(cfg, ["--output-dir=/tmp/out", "--workers=4"])
    // cfg.out_path    == "/tmp/out"
    // cfg.workers     == 4
    // cfg.internal_id == 99


Error handling
===============

``parse_args`` returns an empty string on success or a descriptive error message
on the first failure:

.. code-block:: das

    let err = parse_args(cfg, ["--count", "not_a_number"])
    // err == "--count: invalid int value 'not_a_number'"

    if (err != "") {
        print("usage error: {err}\n")
        return
    }

Common error forms:

* ``"--flag: invalid int value 'abc'"``
* ``"--flag: invalid float value 'abc'"``
* ``"--flag: invalid enum value 'Unknown'"``
* ``"--flag: missing required flag"``
* ``"--flag: invalid bool value: 'yes'"``


Introspection with ``get_command_info``
========================================

``get_command_info(type<T>)`` returns a ``CommandInfo`` value containing a
``CommandArgumentInfo`` entry for each parsed flag.  Use it to implement
``--help`` output without duplicating the flag list:

.. code-block:: das

    def print_help(info : CommandInfo) {
        for (arg in info.args) {
            let req = arg.is_required ? " (required)" : ""
            let doc = !empty(arg.doc_string) ? "  — {arg.doc_string}" : ""
            print("  {arg.flag_name}{req}{doc}\n")
            if (!empty(arg.enum_values)) {
                for (v in arg.enum_values) {
                    print("      {v}\n")
                }
            }
        }
    }

    let info <- get_command_info(type<AppConfig>)
    print_help(info)
    // output:
    //   --output-dir  — Directory to write output files
    //   --workers  — Number of parallel workers (default: 1)

``CommandArgumentInfo`` fields:

+-----------------+------------------+---------------------------------------------+
| Field           | Type             | Description                                 |
+=================+==================+=============================================+
| ``flag_name``   | ``string``       | Full flag string (e.g. ``"--output-dir"``)  |
+-----------------+------------------+---------------------------------------------+
| ``field_name``  | ``string``       | Struct field name                           |
+-----------------+------------------+---------------------------------------------+
| ``doc_string``  | ``string``       | ``@clarg_doc`` text, or ``""``              |
+-----------------+------------------+---------------------------------------------+
| ``is_required`` | ``bool``         | ``true`` if ``@clarg_required``             |
+-----------------+------------------+---------------------------------------------+
| ``is_array``    | ``bool``         | ``true`` for ``array<string>`` fields       |
+-----------------+------------------+---------------------------------------------+
| ``value_type``  | ``Type``         | Base type (``tString``, ``tInt``, etc.)     |
+-----------------+------------------+---------------------------------------------+
| ``enum_values`` | ``array<string>``| Entry names for enum fields, empty otherwise|
+-----------------+------------------+---------------------------------------------+


Reading process arguments
==========================

In a real program call ``parse_args(cfg)`` (no second argument) to parse from
the process command line.  Arguments must follow ``--`` on the command line;
``get_cli_arguments()`` returns only that suffix:

.. code-block:: bash

    daslang.exe my_script.das -- --name Alice --count 5

.. code-block:: das

    [export]
    def main() {
        var cfg = Config()
        let err = parse_args(cfg)  // reads from process command line
        if (err != "") {
            print("error: {err}\n")
            return
        }
        print("Hello, {cfg.name}!\n")
    }

Without ``--``, ``get_cli_arguments()`` returns an empty array and all fields
keep their default values.


.. seealso::

    Full source: :download:`tutorials/language/53_clargs.das <../../../../tutorials/language/53_clargs.das>`

    Previous tutorial: :ref:`tutorial_option_and_result`

    :ref:`tutorial_structs`, :ref:`tutorial_enumerations`, :ref:`tutorial_arrays`, :ref:`tutorial_annotations`
