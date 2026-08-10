.. _tutorial_clargs:

==========================================
Command-Line Argument Parsing (clargs)
==========================================

.. index::
    single: Tutorial; clargs
    single: Tutorial; Command-Line Arguments
    single: Tutorial; CLI Flags
    single: Tutorial; CommandLineArgs

This tutorial covers ``daslib/clargs`` — a structure macro that turns an
annotated struct into a type-safe CLI parser.  Declare your flags as struct
fields; the macro generates the parse functions and the runtime flag metadata
for that struct.

Prerequisites: structs, enums, arrays, and ``Result`` / ``Option``
(:ref:`tutorial_option_and_result`).

.. code-block:: das

    options gen2

    require daslib/clargs


Defining a CLI args struct
===========================

Annotate any struct with ``[CommandLineArgs]``.  The macro adds these functions
to your module:

* ``parse_args(type<T>; args : array<string>) : Result<T; string>`` — parse a
  provided list
* ``parse_args(type<T>) : Result<T; string>`` — parse the process command line,
  read through ``get_user_args()``
* ``parse_args_with_help(var dst : T; prog_name : string) : int`` — parse, and
  answer ``--help`` / ``-h`` on the way (generated only for structs that
  declare neither flag themselves)
* ``get_command_info(type<T>) : CommandInfo`` — runtime flag metadata

``parse_args`` builds a fresh ``T`` and hands it back inside a ``Result``.
``move_unwrap`` takes the struct out on success; ``unwrap_err`` gives a one-line
message naming the flag on failure.

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

    var res <- parse_args(type<Config>, ["--name", "Alice", "--count=42", "--verbose", "--timeout=1.5"])
    let cfg <- res |> move_unwrap
    // cfg.name    == "Alice"
    // cfg.count   == 42
    // cfg.verbose == true
    // cfg.timeout == 1.5

Each flag accepts two forms: ``--flag value`` (space-separated) and
``--flag=value`` (equals sign, no space).  A flag given twice keeps the last
value, so a wrapper script can set a default that the user overrides.  Tokens
that look like flags but match no field are ignored, which lets another parser
share the same argv.


Supported field types
======================

+---------------------+----------------------------------------------------+
| Field type          | Accepted flag values                               |
+=====================+====================================================+
| ``string``          | Any string                                         |
+---------------------+----------------------------------------------------+
| ``int``             | Decimal integer (optional leading ``+``/``-``)     |
+---------------------+----------------------------------------------------+
| ``float``           | Decimal float with optional exponent               |
+---------------------+----------------------------------------------------+
| ``bool``            | Bare flag (true), ``=true``, or ``=false``         |
+---------------------+----------------------------------------------------+
| ``enum E``          | Enum entry name as a string (e.g. ``"Red"``)       |
+---------------------+----------------------------------------------------+
| ``array<string>``   | Flag may appear multiple times                     |
+---------------------+----------------------------------------------------+
| ``Option<T>``       | Same as ``T``; ``none`` when the flag is absent    |
+---------------------+----------------------------------------------------+


Bool flags
===========

A bare ``--verbose`` sets the field to ``true``.  Use ``=true`` or ``=false``
to be explicit:

.. code-block:: das

    let on  <- parse_args(type<Config>, ["--verbose"]) |> move_unwrap
    let off <- parse_args(type<Config>, ["--verbose=false"]) |> move_unwrap
    // on.verbose  == true
    // off.verbose == false


Enum flags
===========

Pass the enum entry name as a string.  An unknown name is an error:

.. code-block:: das

    enum LogLevel {
        Debug
        Info
        Warning
        Error
    }

    [CommandLineArgs]
    struct LogConfig {
        level : LogLevel  // --level accepts "Debug", "Info", "Warning", "Error"
    }

    var res <- parse_args(type<LogConfig>, ["--level", "Warning"])
    let cfg <- res |> move_unwrap
    // cfg.level == LogLevel.Warning

    let bad <- parse_args(type<LogConfig>, ["--level", "Verbose"])
    // bad |> unwrap_err == "--level: invalid enum value 'Verbose'"


Array flags
============

An ``array<string>`` field collects every occurrence of the flag into the
array.  Both forms (``--tag value`` and ``--tag=value``) are supported:

.. code-block:: das

    [CommandLineArgs]
    struct BuildConfig {
        tags : array<string>
    }

    var res <- parse_args(type<BuildConfig>, ["--tags=debug", "--tags", "release", "--tags=profile"])
    let cfg <- res |> move_unwrap
    // cfg.tags == ["debug", "release", "profile"]


Required flags
===============

``@clarg_required`` makes a flag mandatory.  ``parse_args`` fails when the flag
is absent:

.. code-block:: das

    [CommandLineArgs]
    struct DeployConfig {
        host : string

        @clarg_required
        token : string
    }

    let missing <- parse_args(type<DeployConfig>, ["--host=prod.example.com"])
    // missing |> unwrap_err == "--token: missing required flag"

    var res <- parse_args(type<DeployConfig>, ["--host=prod.example.com", "--token=secret"])
    let cfg <- res |> move_unwrap
    // cfg.token == "secret"


Optional fields
================

An absent ``int`` field parses as ``0``, which is also a value a user can type.
Declare the field ``Option<T>`` when the two cases must stay apart:

.. code-block:: das

    [CommandLineArgs]
    struct RetryConfig {
        @clarg_doc = "retry count; none when the flag is absent"
        retries : Option<int>
    }

    var res <- parse_args(type<RetryConfig>, ["--retries=3"])
    let cfg <- res |> move_unwrap
    if (cfg.retries |> is_some) {
        print("retries = {cfg.retries |> unwrap}\n")
    } else {
        print("no --retries; the built-in policy decides\n")
    }

``string``, ``int``, ``float``, ``bool``, and ``array<string>`` all wrap.  An
enum field does not — wrap one and the macro fails at compile time.


Field-level attributes
=======================

Field annotations fine-tune the flag a field becomes:

``@clarg_name = "flag"``
    Overrides the auto-generated flag name.  clargs prepends the ``--``.

``@clarg_short = "X"``
    Attaches a single-character short flag.

``@clarg_doc = "text"``
    Description for the help renderer and for ``get_command_info``.

``@clarg_skip``
    Excludes the field from the CLI schema.  Set it in code instead.

``@clarg_required``
    The flag must be supplied.

``@clarg_positional``
    Fills the field from a bare token instead of a flag.

``@clarg_count``
    Sums every occurrence of the flag into a plain ``int`` field.

``@clarg_mutex_group = "name"``
    Puts the flag in a group whose members exclude each other.

``@clarg_env = "NAME"``
    Reads an environment variable when the flag is absent.

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

    var res <- parse_args(type<AppConfig>, ["--output-dir=/tmp/out", "--workers=4"])
    var cfg <- res |> move_unwrap
    cfg.internal_id = 99   // set in code; there is no --internal-id
    // cfg.out_path == "/tmp/out"
    // cfg.workers  == 4


Positional arguments
=====================

``@clarg_positional`` fields take the bare tokens of the command line, in
declaration order.  A plain ``string`` positional is required, an
``Option<string>`` one is optional, and an ``array<string>`` one swallows every
remaining token:

.. code-block:: das

    [CommandLineArgs]
    struct PkgConfig {
        @clarg_positional
        @clarg_doc = "subcommand"
        command : string

        @clarg_positional
        @clarg_doc = "package name"
        pkg : Option<string>

        @clarg_doc = "project root"
        root : string = "."
    }

    var res <- parse_args(type<PkgConfig>, ["--root", "/tmp", "install", "dasImgui"])
    let cfg <- res |> move_unwrap
    // cfg.command       == "install"
    // cfg.pkg |> unwrap == "dasImgui"
    // cfg.root          == "/tmp"

Flags and positionals interleave freely: clargs first removes every flag it
knows (with its value, when the ``--flag value`` form is used), then reads what
is left in order.  Unknown flag-shaped tokens are dropped rather than counted
as positionals.

The macro rejects orders it cannot fill: an ``array<string>`` positional must
be last, a required positional cannot follow an optional one, and
``@clarg_positional`` combines with none of ``@clarg_short``, ``@clarg_count``,
or ``@clarg_env``.  A missing required positional reports
``"<command>: missing required positional argument"``.


Counting occurrences
=====================

``@clarg_count`` on a plain ``int`` field counts how often the flag appears —
the ``-v -v -v`` idiom for verbosity levels.  Long and short forms sum together:

.. code-block:: das

    [CommandLineArgs]
    struct VerbosityConfig {
        @clarg_count
        @clarg_short = "v"
        @clarg_doc = "verbosity; repeat to raise the level"
        verbose : int
    }

    var res <- parse_args(type<VerbosityConfig>, ["-v", "-v", "--verbose"])
    let cfg <- res |> move_unwrap
    // cfg.verbose == 3

Count flags carry no value, so ``--verbose=2`` reports
``"--verbose: count flag does not accept a value"``.  Bundling is not
implemented: ``-vvv`` matches nothing and leaves the field at ``0``.


Mutually exclusive flags
=========================

Flags that share a ``@clarg_mutex_group`` name may not appear together:

.. code-block:: das

    [CommandLineArgs]
    struct OutputConfig {
        @clarg_mutex_group = "color"
        color : bool

        @clarg_name = "no-color"
        @clarg_mutex_group = "color"
        no_color : bool
    }

    let clash <- parse_args(type<OutputConfig>, ["--color", "--no-color"])
    // clash |> unwrap_err == "--color, --no-color: mutually exclusive (group 'color')"

Groups are independent — one flag from each of two groups is fine.  The check
reads the command line only, so a value that arrived from an environment twin
never collides with an explicit flag.


Environment twins
==================

Any flag can also read an environment variable.  ``@clarg_env = "NAME"`` names
one per field; ``[CommandLineArgs(env_prefix = "TOOL")]`` derives
``TOOL_LONG_NAME`` from every long flag name, with hyphens becoming
underscores.  ``@clarg_env = ""`` opts a single field out of that derivation:

.. code-block:: das

    [CommandLineArgs(env_prefix = "MYTOOL")]
    struct ServeConfig {
        @clarg_doc = "listen port"
        port : int          // --port, or MYTOOL_PORT

        @clarg_env = "MYTOOL_ROOT_DIR"
        @clarg_doc = "document root"
        root : string       // --root, or MYTOOL_ROOT_DIR

        @clarg_env = ""
        @clarg_doc = "debug logging"
        debug : bool        // --debug only
    }

The command line wins over the variable, and the variable wins over the field
initializer.  Booleans read ``""``, ``0``, ``false``, ``off``, and ``no`` (any
case) as false and anything else as true.  A variable that is set but empty
counts as unset.  Garbage in an int, float, or enum variable is an error, the
same way it is on the command line.  ``@clarg_required`` is satisfied by either
carrier.  Positional, count, and repeatable fields have no environment form.

Libraries have no argv at all, so their ambient knobs use the sibling
annotation ``[EnvConfig]``, which reads the same ``@clarg_doc`` /
``@clarg_env`` vocabulary and generates ``env_config(type<T>) : T``.


Error handling
===============

``parse_args`` returns ``Result<T; string>``.  ``is_err`` reports the outcome,
``unwrap_err`` gives the message, and ``move_unwrap`` takes the parsed struct:

.. code-block:: das

    [CommandLineArgs]
    struct TypedConfig {
        count : int
    }

    def load_config(argv : array<string>) : bool {
        var res <- parse_args(type<TypedConfig>, argv)
        if (res |> is_err) {
            print("usage error: {res |> unwrap_err}\n")
            return false
        }
        let cfg <- res |> move_unwrap
        print("count = {cfg.count}\n")
        return true
    }

    // load_config(["--count", "not_a_number"]) prints
    //   usage error: --count: invalid int value 'not_a_number'

Parsing stops at the first failure.  Common messages:

* ``"--flag: invalid int value 'abc'"``
* ``"--flag: invalid float value 'abc'"``
* ``"--flag: invalid enum value 'Unknown'"``
* ``"--flag: invalid bool value: 'yes'"``
* ``"--flag: missing value"`` — the flag is there, its value is not
* ``"--flag: missing required flag"``
* ``"<name>: missing required positional argument"``
* ``"--flag: count flag does not accept a value"``
* ``"--fast, --slow: mutually exclusive (group 'mode')"``


Short flags
============

``@clarg_short = "X"`` attaches a single-character short flag.  Both the long
and short forms are recognised, with identical value syntax (``-X value``,
``-X=value``, or bare ``-X`` for booleans):

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

    var res <- parse_args(type<ServerConfig>, ["-p", "8080", "-v", "-t=alpha", "-t=beta"])
    let cfg <- res |> move_unwrap
    // cfg.port    == 8080
    // cfg.verbose == true
    // cfg.tags    == ["alpha", "beta"]

Mixing long and short occurrences of an array flag preserves command-line order:
``--tags=a -t b --tags=c`` collects ``["a", "b", "c"]``.

The macro rejects a short flag it cannot parse back: two fields sharing one
character, a value longer than one character, and ``-``, ``=``, or whitespace
as the character are all compile-time errors.


Introspection with ``get_command_info``
========================================

``get_command_info(type<T>)`` returns a ``CommandInfo`` value containing a
``CommandArgumentInfo`` entry for each parsed flag — the same data the
help renderer uses, exposed for programmatic inspection (custom help formats,
validation rules, configuration dumps, shell completion):

.. code-block:: das

    let info <- get_command_info(type<ServerConfig>)
    for (arg in info.args) {
        print("  {arg.short_flag_name}, {arg.flag_name}  ({arg.value_type})  {arg.doc_string}\n")
    }
    // output:
    //   -p, --port  (tInt)  listen port
    //   -v, --verbose  (tBool)  verbose logging
    //   -t, --tags  (tString)  tag (repeated)

``CommandInfo`` carries ``args`` plus ``has_user_help``, which is ``true`` when
the struct declares its own ``--help`` or ``-h`` flag.

``CommandArgumentInfo`` fields:

+------------------------+--------------------+----------------------------------------------------+
| Field                  | Type               | Description                                        |
+========================+====================+====================================================+
| ``field_name``         | ``string``         | Struct field name                                  |
+------------------------+--------------------+----------------------------------------------------+
| ``flag_name``          | ``string``         | ``--output-dir``, or ``<name>`` for a positional   |
+------------------------+--------------------+----------------------------------------------------+
| ``short_flag_name``    | ``string``         | ``-X`` from ``@clarg_short``, or ``""``            |
+------------------------+--------------------+----------------------------------------------------+
| ``env_name``           | ``string``         | Environment twin, or ``""``                        |
+------------------------+--------------------+----------------------------------------------------+
| ``doc_string``         | ``string``         | ``@clarg_doc`` text, or ``""``                     |
+------------------------+--------------------+----------------------------------------------------+
| ``default_doc``        | ``string``         | Default as text; ``[EnvConfig]`` structs only      |
+------------------------+--------------------+----------------------------------------------------+
| ``value_type``         | ``Type``           | Base type (``tString``, ``tInt``, ...)             |
+------------------------+--------------------+----------------------------------------------------+
| ``is_path``            | ``bool``           | ``@clarg_path``; ``[EnvConfig]`` structs only      |
+------------------------+--------------------+----------------------------------------------------+
| ``is_array``           | ``bool``           | ``true`` for ``array<string>`` fields              |
+------------------------+--------------------+----------------------------------------------------+
| ``is_required``        | ``bool``           | ``true`` if ``@clarg_required``                    |
+------------------------+--------------------+----------------------------------------------------+
| ``is_positional``      | ``bool``           | ``true`` if ``@clarg_positional``                  |
+------------------------+--------------------+----------------------------------------------------+
| ``is_optional_wrap``   | ``bool``           | ``true`` for ``Option<T>`` fields                  |
+------------------------+--------------------+----------------------------------------------------+
| ``is_count``           | ``bool``           | ``true`` if ``@clarg_count``                       |
+------------------------+--------------------+----------------------------------------------------+
| ``mutex_group``        | ``string``         | ``@clarg_mutex_group`` name, or ``""``             |
+------------------------+--------------------+----------------------------------------------------+
| ``enum_values``        | ``array<string>``  | Entry names for enum fields, empty otherwise       |
+------------------------+--------------------+----------------------------------------------------+


Help rendering
===============

The library ships a ``--help`` renderer over ``CommandInfo``:

* ``print_help(info, prog_name)`` — writes the formatted help to stdout.
* ``format_help(info, prog_name) : string`` — returns the same text, useful
  in tests or when redirecting into a logger.
* ``format_help_with_auto_help(info, prog_name) : string`` — the same text
  with a ``-h, --help`` row appended, unless the struct declares its own.

``parse_args_with_help`` wires all of that up for a struct with no help flag of
its own.  It prints the help and returns ``0`` when it sees ``--help`` or
``-h``, returns ``-1`` after a clean parse (your struct is populated), and
returns ``1`` on a parse error it has already logged at ``LOG_ERROR``:

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
    }

    [export]
    def main() : int {
        var cfg = DemoConfig()
        let rc = parse_args_with_help(cfg, "demo")
        return rc if (rc >= 0)
        print("hello, {cfg.name}\n")
        return 0
    }

The rendered output, from a standalone ``daslang -exe`` binary:

.. code-block:: text

    Usage: demo [flags]

    Flags:
      -n, --name=STRING    user's display name
          --count=INT      iteration count
      -v, --verbose        verbose logging
      -h, --help           show this help and exit

Format rules:

* Per-flag line: ``-X, --long=PLACEHOLDER  doc_string``.  Fields with no
  ``@clarg_short`` leave the short slot blank to keep the long flags vertically
  aligned.
* ``=PLACEHOLDER`` is the uppercased type name (``STRING`` / ``INT`` / ``FLOAT``
  / ``ENUM``).  Bool and count flags omit it.
* Enum values render inline as ``(V1|V2|V3)``.
* The doc column then picks up the markers that apply: ``(env: NAME)``,
  ``(required)``, ``(repeated)`` for array flags, ``(repeats)`` for count
  flags, and ``(mutex: group)``.
* Positionals get their own ``Positional arguments:`` block and appear in the
  usage line as ``<name>``, ``[<name>]``, or ``[<name>...]`` for the array tail.
* The usage line follows the host.  A standalone binary owns argv, so it reads
  ``Usage: demo [flags]``; under the script host the same call renders
  ``Usage: daslang demo -- [flags]``, which is the line a user can copy.
* Defaults are not rendered.  Write them into ``@clarg_doc`` when they matter.

Under the script host, ``daslang`` itself takes ``-h`` and ``--help`` before it
forwards anything to your script, so the auto help flag is reachable only from
a standalone binary.  A script that needs a help flag declares its own and
wires it to ``-?``, which the host leaves alone.  A field that spells
``--help`` or ``-h`` also turns ``parse_args_with_help`` off for that struct —
once you name the flag, the exit policy is yours:

.. code-block:: das

    [CommandLineArgs]
    struct ScriptConfig {
        @clarg_short = "n"
        @clarg_doc = "user's display name"
        name : string

        @clarg_short = "?"
        @clarg_name = "show-help"
        @clarg_doc = "show this help and exit"
        show_help : bool
    }

    var res <- parse_args(type<ScriptConfig>)
    if (res |> is_err) {
        print("error: {res |> unwrap_err}\n\n")
        print_help(get_command_info(type<ScriptConfig>), "demo")
    } else {
        let cfg <- res |> move_unwrap
        if (cfg.show_help) {
            print_help(get_command_info(type<ScriptConfig>), "demo")
        }
    }


Reading process arguments
==========================

``parse_args(type<T>)`` reads argv through ``get_user_args()``, which picks the
slice that belongs to your program:

``get_user_args()``
    ``argv[1..]`` for a standalone ``daslang -exe`` binary, and the slice after
    the ``--`` separator under the interpreter or the JIT.  One spelling works
    in all three, which is why the generated ``parse_args`` uses it.

``get_cli_arguments() / get_cli_arguments(argv)`` — script-style
    Returns the slice **after** the ``--`` separator in argv (or empty if no
    ``--``).  This is what daslang script invocations look like, where daslang
    itself owns argv up to the ``--`` and the script gets everything after:

    .. code-block:: bash

        daslang.exe my_script.das -- --name Alice --count 5

``get_program_args() / get_program_args(argv)`` — standalone-tool style
    Returns ``argv[1..]`` — the full argv with the program name stripped.  Use
    this for AOT'd binaries that own the full argv themselves and have no
    ``--`` separator (das-fmt, daspkg, lint, aot-style tools):

    .. code-block:: das

        [CommandLineArgs]
        struct FmtConfig {
            @clarg_doc = "rewrite files in place"
            write : bool

            @clarg_positional
            @clarg_doc = "files to format"
            files : array<string>
        }

        var res <- parse_args(type<FmtConfig>, get_program_args())
        if (res |> is_err) {
            print("error: {res |> unwrap_err}\n")
        } else {
            let cfg <- res |> move_unwrap
            print("formatting {length(cfg.files)} files\n")
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

    Next tutorial: :ref:`tutorial_glob`

    :ref:`tutorial_structs`, :ref:`tutorial_enumerations`, :ref:`tutorial_arrays`, :ref:`tutorial_annotations`
