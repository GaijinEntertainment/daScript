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
  (post-``--`` slice; see *Reading process arguments* below)
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

``@clarg_short = "X"``
    Attaches a single-character short flag (see *Short flags* below).

``@clarg_doc = "text"``
    Attaches a description used by help generators (see *Help rendering* below).

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


Short flags
============

``@clarg_short = "X"`` attaches a single-character short flag.  Both the long
and short forms are recognised by ``parse_args``, with identical value syntax
(``-X value``, ``-X=value``, or bare ``-X`` for booleans):

.. code-block:: das

    [CommandLineArgs]
    struct ServerConfig {
        @clarg_short = "p"
        @clarg_doc = "listen port"
        port : int

        @clarg_short = "v"
        @clarg_doc = "verbose logging"
        verbose : bool

        @clarg_short = "t"
        @clarg_doc = "tag (repeated)"
        tags : array<string>
    }

    var cfg = ServerConfig()
    parse_args(cfg, ["-p", "8080", "-v", "-t=alpha", "-t=beta"])
    // cfg.port    == 8080
    // cfg.verbose == true
    // cfg.tags    == ["alpha", "beta"]

Mixing long and short occurrences of an array flag preserves command-line order:
``--tags=a -t b --tags=c`` collects ``["a", "b", "c"]``.

Two fields cannot share a short flag.  ``@clarg_short`` must be exactly one
character; both are compile-time errors from the macro.


Introspection with ``get_command_info``
========================================

``get_command_info(type<T>)`` returns a ``CommandInfo`` value containing a
``CommandArgumentInfo`` entry for each parsed flag — the same data the
help renderer below uses, but exposed for programmatic inspection (custom
help formats, validation rules, configuration dumps, etc.):

.. code-block:: das

    let info <- get_command_info(type<ServerConfig>)
    for (arg in info.args) {
        print("  {arg.short_flag_name}, {arg.flag_name}  ({arg.value_type})  {arg.doc_string}\n")
    }
    // output:
    //   -p, --port  (tInt)  listen port
    //   -v, --verbose  (tBool)  verbose logging
    //   -t, --tags  (tString)  tag (repeated)

``CommandArgumentInfo`` fields:

+---------------------+------------------+--------------------------------------------------+
| Field               | Type             | Description                                      |
+=====================+==================+==================================================+
| ``flag_name``       | ``string``       | Full flag string (e.g. ``"--output-dir"``)       |
+---------------------+------------------+--------------------------------------------------+
| ``short_flag_name`` | ``string``       | ``"-X"`` from ``@clarg_short``, or ``""``        |
+---------------------+------------------+--------------------------------------------------+
| ``field_name``      | ``string``       | Struct field name                                |
+---------------------+------------------+--------------------------------------------------+
| ``doc_string``      | ``string``       | ``@clarg_doc`` text, or ``""``                   |
+---------------------+------------------+--------------------------------------------------+
| ``is_required``     | ``bool``         | ``true`` if ``@clarg_required``                  |
+---------------------+------------------+--------------------------------------------------+
| ``is_array``        | ``bool``         | ``true`` for ``array<string>`` fields            |
+---------------------+------------------+--------------------------------------------------+
| ``value_type``      | ``Type``         | Base type (``tString``, ``tInt``, etc.)          |
+---------------------+------------------+--------------------------------------------------+
| ``enum_values``     | ``array<string>``| Entry names for enum fields, empty otherwise     |
+---------------------+------------------+--------------------------------------------------+


Help rendering
===============

The library ships a ``--help`` renderer over ``CommandInfo``:

* ``print_help(info, prog_name)`` — writes the formatted help to stdout.
* ``format_help(info, prog_name) : string`` — returns the same text, useful
  in tests or when redirecting into a logger.

``parse_args`` does **not** auto-recognise ``--help`` — declare an explicit
``help`` field and check it after parsing.  This keeps ``parse_args`` a pure
parser and leaves the exit policy to the caller:

.. code-block:: das

    [CommandLineArgs]
    struct DemoConfig {
        @clarg_short = "n"
        @clarg_doc = "user's display name"
        name : string

        @clarg_doc = "iteration count"
        count : int

        @clarg_short = "v"
        @clarg_doc = "verbose logging"
        verbose : bool

        @clarg_short = "h"
        @clarg_doc = "show this help and exit"
        help : bool
    }

    [export]
    def main() : int {
        var cfg = DemoConfig()
        let err = parse_args(cfg)
        if (err != "") {
            print("error: {err}\n")
            return 1
        }
        if (cfg.help) {
            print_help(get_command_info(type<DemoConfig>), "demo")
            return 0
        }
        return 0
    }

The rendered output:

.. code-block:: text

    Usage: demo [flags]

    Flags:
      -n, --name=STRING    user's display name
          --count=INT      iteration count
      -v, --verbose        verbose logging
      -h, --help           show this help and exit

Format rules:

* Per-flag line: ``-X, --long=PLACEHOLDER  doc_string``.  Fields with no
  ``@clarg_short`` indent the short slot blank to keep the long flags vertically
  aligned.
* ``=PLACEHOLDER`` is the uppercased type name (``STRING`` / ``INT`` / ``FLOAT``
  / ``ENUM``).  Bool flags omit it.
* Enum values render inline as ``(V1|V2|V3)``.
* ``(required)`` / ``(repeated)`` markers are appended to the doc column for
  required flags and array flags respectively.
* Defaults are not shown — ``CommandInfo`` does not currently carry them.


Reading process arguments
==========================

Two helpers feed ``argv`` into ``parse_args``, depending on how the program is
invoked.  Each has a zero-argument form (operating on the live process command
line) and a one-argument form (taking an explicit ``argv`` array, useful in
tests):

``get_cli_arguments() / get_cli_arguments(argv)`` — script-style
    Returns the slice **after** the ``--`` separator in argv (or empty if no
    ``--``).  This is what daslang script invocations look like, where daslang
    itself owns argv up to the ``--`` and the script gets everything after:

    .. code-block:: bash

        daslang.exe my_script.das -- --name Alice --count 5

    The no-argument ``parse_args(cfg)`` overload generated by the macro calls
    this internally.

``get_program_args() / get_program_args(argv)`` — standalone-tool style
    Returns ``argv[1..]`` — the full argv with the program name stripped.  Use
    this for AOT'd binaries that own the full argv themselves and have no
    ``--`` separator (das-fmt, daspkg, lint, aot-style tools):

    .. code-block:: das

        [export]
        def main() : int {
            var cfg = FmtConfig()
            let err = parse_args(cfg, get_program_args())
            if (err != "") {
                print("error: {err}\n")
                return 1
            }
            return 0
        }

The explicit-argv overloads make the splitting logic unit-testable without
touching the live process state:

.. code-block:: das

    let scripted <- get_cli_arguments(["host", "script.das", "--", "--foo", "bar"])
    // scripted == ["--foo", "bar"]

    let standalone <- get_program_args(["fmt.exe", "--write", "file.das"])
    // standalone == ["--write", "file.das"]


.. seealso::

    Full source: :download:`tutorials/language/53_clargs.das <../../../../tutorials/language/53_clargs.das>`

    Previous tutorial: :ref:`tutorial_option_and_result`

    :ref:`tutorial_structs`, :ref:`tutorial_enumerations`, :ref:`tutorial_arrays`, :ref:`tutorial_annotations`
