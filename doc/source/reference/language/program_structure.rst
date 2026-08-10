.. _program_structure:

=================
Program Structure
=================

.. index::
    single: Program Structure
    single: module
    single: require
    single: options

A daslang source file is a sequence of top-level declarations. This page describes the overall
layout of a file and the key declarations that control how it interacts with the rest of the program.

--------------------
File Layout
--------------------

A typical daslang file follows this layout:

.. code-block:: das

    options gen2                              // compilation options

    module my_module shared public            // module declaration

    require math                              // imports
    require daslib/strings_boost

    struct MyData {                           // type declarations
        value : int
        name : string
    }

    enum Color {
        red
        green
        blue
    }

    let MAX_COUNT = 100                       // global constants

    var total : int = 0                       // global variables

    typedef IntArray = array<int>             // type aliases

    def helper(x : int) : int {               // functions
        return x * 2
    }

    [export]                                  // entry point
    def main {
        print("hello\n")
    }

The parser does not enforce a strict ordering among ``options``, ``module``, and ``require``.
However, the ``module`` declaration must appear before any type declarations (structs, enums,
functions, global variables, type aliases). By convention, ``options`` lines come first,
followed by ``module``, then ``require``.

.. note::
    The order above is a convention, not a hard rule. The only enforced constraint is that
    ``module`` precedes type declarations.

--------------------
Module Declaration
--------------------

The ``module`` declaration names the current file's module:

.. das-doc: fragment

.. code-block:: das

    module my_module

If omitted, the module name defaults to the file name (without extension).

^^^^^^^^^^^^^^^^^^^
Modifiers
^^^^^^^^^^^^^^^^^^^

The ``module`` declaration supports several modifiers:

``shared``
    Promotes the module to a built-in module. Only one instance is created per compilation
    environment, and it is shared across contexts:

    .. das-doc: fragment

    .. code-block:: das

        module my_lib shared

``public`` / ``private``
    Sets the default visibility of all declarations in the module. Functions, structs,
    enums, and globals inherit this default unless they specify their own visibility:

    .. das-doc: fragment

    .. code-block:: das

        module my_lib public          // all declarations are public by default
        module my_lib private         // all declarations are private by default

    If neither is specified, the module uses the environment's default (typically ``public``).

``inscope``
    Makes the module visible to all modules in the project without an explicit ``require``.
    This uses the ``!inscope`` syntax:

    .. das-doc: fragment

    .. code-block:: das

        module my_lib !inscope

Modifiers can be combined, in the order ``shared``, then ``public``/``private``, then
``!inscope``:

.. das-doc: fragment

.. code-block:: das

    module my_lib shared public

--------------------
Require Declaration
--------------------

The ``require`` declaration imports another module:

.. code-block:: das

    require math
    require daslib/ast_boost

Module names can contain ``/`` and ``.`` separators. The project is responsible for
resolving module names into file paths.

^^^^^^^^^^^^^^^^^^^
Re-exporting
^^^^^^^^^^^^^^^^^^^

By default, required modules are private — they are only visible within the current module.
The ``public`` modifier re-exports the module, making it transitively visible to any module
that requires the current one:

.. code-block:: das

    require dastest/testing_boost public

^^^^^^^^^^^^^^^^^^^
Aliasing
^^^^^^^^^^^^^^^^^^^

The ``as`` keyword gives a required module a local qualifier of your choosing. Given a
sibling module file ``event.das``:

.. das-doc: file event.das

.. code-block:: das

    module event

    def public process {
        print("event\n")
    }

a require can bind it to any local name:

.. code-block:: das

    require ./event.das as sub_event

    def handle {
        sub_event::process()          // qualified call using the alias
    }

An explicit ``as`` alias registers for every require form, module-name requires
(``require daslib/random as rng``) included. Without ``as``, a **path** require — one
beginning with ``./``, ``../`` or ``%/`` and naming the ``.das`` file explicitly — still
registers the file stem as a qualifier, so ``event::process()`` resolves in the example
above as well; a module-name require registers no implicit qualifier. In either case the
module's own declared name keeps working as a qualifier, so ``random::random_seed(seed)``
resolves whether or not an alias was given.

(see :ref:`Modules <modules>` for details on module function visibility and the ``_`` / ``__``
module prefixes).

--------------------
Options Declaration
--------------------

The ``options`` declaration sets compiler options for the file:

.. code-block:: das

    options gen2
    options no_unused_block_arguments = false

Multiple options can appear on one line, separated by commas:

.. code-block:: das

    options no_aot = true, rtti = true

A bare option name (without ``= value``) is shorthand for ``= true``:

.. code-block:: das

    options gen2          // equivalent to: options gen2 = true

(see :ref:`Options <options>` for the complete list of recognized options).

-----------------------
Top-Level Declarations
-----------------------

After the header declarations, the rest of the file consists of:

- **Type aliases** — ``typedef``, named ``tuple``, ``variant``, ``bitfield``
  (see :ref:`Type Aliases <aliases>`)
- **Enumerations** — ``enum`` (see :ref:`Constants and Enumerations <constants_and_enumerations>`)
- **Structures and classes** — ``struct``, ``class``
  (see :ref:`Structs <structs>`, :ref:`Classes <classes>`)
- **Global variables** — ``let`` (constant) and ``var`` (mutable)
  (see :ref:`Constants and Enumerations <constants_and_enumerations>`)
- **Functions** — ``def`` (see :ref:`Functions <functions>`)
- **Top-level reader macros** — ``%macro_name~...~``

All of these are peers in the grammar and can appear in any order, interleaved freely.

--------------------
Visibility
--------------------

Each top-level declaration can be marked ``public`` or ``private``:

.. das-doc: alt

.. code-block:: das

    def public helper(x : int) : int {     // visible to other modules
        return x * 2
    }

    struct private Internal {               // only visible within this module
        data : int
    }

If no visibility is specified, the declaration inherits the module's default visibility.

Shared global variables use the ``shared`` keyword and are shared across cloned contexts:

.. code-block:: das

    let shared GLOBAL_TABLE : table<string; int> <- { "a" => 1 }

--------------------
Entry Points
--------------------

A daslang program is compiled and simulated by the host application (a C++ executable).
The host decides which functions to call. Several annotations mark functions with special roles.

^^^^^^^^^^^^^^^^^^^
[export]
^^^^^^^^^^^^^^^^^^^

Marks a function as callable from the host application. The host invokes exported functions
by name through the context API:

.. code-block:: das

    [export]
    def main {
        print("hello world\n")
    }

There is nothing special about the name ``main`` — it is purely a convention. The host chooses
which exported function(s) to call and in what order.

^^^^^^^^^^^^^^^^^^^
[init]
^^^^^^^^^^^^^^^^^^^

Marks a function to run automatically during context initialization. ``[init]`` functions
cannot have arguments and cannot return a value:

.. code-block:: das

    [init]
    def setup {
        print("initializing\n")
    }

Multiple ``[init]`` functions execute in declaration order. Ordering can be controlled with
attributes:

.. code-block:: das

    [init(tag="db")]
    def init_database {
        pass
    }

    [init(after="db")]
    def init_cache {
        pass
    }

    [init(before="db")]
    def init_logging {
        pass
    }

The option ``no_init`` forbids ``[init]`` outright — with it set, declaring an ``[init]``
(or ``[finalize]``) function, or giving a global a non-constant initializer, is
``error[30164]``.

^^^^^^^^^^^^^^^^^^^
[finalize]
^^^^^^^^^^^^^^^^^^^

Marks a function to run automatically during context shutdown. Same constraints as ``[init]`` —
no arguments, no return value:

.. code-block:: das

    [finalize]
    def cleanup {
        print("shutting down\n")
    }

--------------------
Expect Declaration
--------------------

The ``expect`` declaration records the compilation errors a file is *supposed* to produce.
It does not silence them: the compiler still reports each error and still fails the file.
What ``expect`` does is attach the list to the resulting ``Program``, where the test runner
reads it back (through ``for_each_expected_error``) and turns a failed compile into a passing
test — provided every reported error is covered by an ``expect`` entry and no expected count
is left unused. Running such a file directly with ``daslang`` still exits non-zero.

This is used by negative test suites, run through ``dastest``, to verify that the compiler
correctly rejects invalid code:

.. das-doc: fragment

.. code-block:: das

    expect 50501:3              // expect error 50501 exactly 3 times
    expect 30308, 30341         // expect each error once (count defaults to 1)

The syntax is:

.. code-block:: text

    expect <error_code> [: <count>] [, <error_code> [: <count>] ...]

Multiple ``expect`` declarations can appear in the same file. Error codes are numeric
identifiers; the leading digit is the compilation stage that raised the error:

+------------------+--------------------------------------------------------+
| Range            | Stage                                                  |
+==================+========================================================+
| ``1xxxx``        | Lexer — bad literals, mismatched brackets, size limits |
+------------------+--------------------------------------------------------+
| ``2xxxx``        | Parser — syntax, module/require, duplicate declaration |
+------------------+--------------------------------------------------------+
| ``3xxxx``        | Semantic analysis — types, lookup, unsafe operations   |
+------------------+--------------------------------------------------------+
| ``40500``        | Lint — AOT side effects (the only ``4xxxx`` code)      |
+------------------+--------------------------------------------------------+
| ``5xxxx``        | Integration — options, AOT link, internal errors       |
+------------------+--------------------------------------------------------+

Within a stage the codes are grouped by category. In the semantic stage, for example,
``30100–30298`` are ``invalid_*``, ``30300–30352`` ``missing_*``, ``30400–30413``
``mismatching_*``, ``30500–30515`` ``exceeds_*``, ``30600–30615`` ``ambiguous_*``,
``30700–30709`` ``already_declared_*``, ``30800–30840`` ``lookup_*``, ``30900–30952``
``cant_*``, ``31000–31037`` ``unsafe_*``, ``31100–31106`` ``recursion_*``,
``31200–31211`` ``runtime_*``, and ``31300–31336`` ``not_*``. The authoritative list of
names and codes is ``include/daScript/ast/compilation_errors.h``.

For example, a test that verifies the compiler rejects copying an array:

.. das-doc: fragment

.. code-block:: das

    expect 30197    // invalid_local_init (array can only be move-initialized)

    [export]
    def main {
        var a <- [1, 2, 3]
        var b = a           // error: can't copy array
    }

--------------------
Program vs. Module
--------------------

The same file format is used for both programs and modules. The distinction is how the file
is used:

**Program (entry point)**
    The top-level file compiled by the host application. Typically contains ``[export]`` functions.
    May omit the ``module`` declaration.

**Module (library)**
    A file imported via ``require`` by other files. Typically has a ``module`` declaration
    and provides types, functions, and globals for reuse.

A file can serve both roles simultaneously.

--------------------
Execution Lifecycle
--------------------

1. The host compiles a source file into a ``Program``
2. The program is simulated into a ``Context``
3. The context is initialized:

   - Global variables are initialized in declaration order, per module
   - ``[init]`` functions run in declaration order (or topologically, if ordering attributes are used)

4. The host calls ``[export]`` functions as needed
5. The context is shut down:

   - ``[finalize]`` functions run

--------------------
Complete Example
--------------------

The following example shows a complete program with all structural elements:

.. code-block:: das

    options gen2

    require math
    require daslib/strings_boost

    struct Particle {
        pos : float3
        vel : float3
        life : float
    }

    enum State {
        alive
        dead
    }

    let private GRAVITY = float3(0.0, -9.8, 0.0)

    var private particles : array<Particle>

    def update_particle(var p : Particle; dt : float) : State {
        p.vel += GRAVITY * dt
        p.pos += p.vel * dt
        p.life -= dt
        if (p.life > 0.0) {
            return State.alive
        }
        return State.dead
    }

    [init]
    def setup {
        for (i in range(100)) {
            particles |> push(Particle(pos=float3(0), vel=float3(0, 10.0, 0), life=5.0))
        }
    }

    [export]
    def main {
        let dt = 0.016
        for (p in particles) {
            update_particle(p, dt)
        }
        print("particles: {length(particles)}\n")
    }

    [finalize]
    def cleanup {
        delete particles
    }

Expected output:

.. code-block:: text

    particles: 100

.. seealso::

    :ref:`Modules <modules>` for module declaration and ``require`` semantics,
    :ref:`Options <options>` for compiler and runtime options,
    :ref:`Annotations <annotations>` for ``[init]``, ``[finalize]``, and ``[export]``,
    :ref:`Contexts <contexts>` for the execution context lifecycle,
    :ref:`Constants and enumerations <constants_and_enumerations>` for global declarations.
