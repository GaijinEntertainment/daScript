.. _contexts:

=======
Context
=======

.. index::
    single: contexts

Daslang environments are organized into contexts. Compiling a Daslang program produces a ``Program`` object, which can then be simulated into a ``Context``.

``Context`` consists of
    * name and flags
    * functions code
    * global variables data
    * shared global variable data
    * stack
    * dynamic memory heap
    * dynamic string heap
    * constant string heap
    * runtime debug information
    * locks
    * miscellaneous lookup infrastructure

In some sense, a ``Context`` can be viewed as a Daslang virtual machine — the object responsible for executing code and maintaining state.
It can also be viewed as an instance of a class whose ``[export]``-marked functions act as its externally callable methods.

Function code, the constant string heap, runtime debug information, and shared global variables are shared between cloned contexts.
This allows each context instance to maintain a relatively small memory profile.

The stack can optionally be shared between multiple contexts of different types, keeping the memory profile even smaller.

Initialization and shutdown
---------------------------

Throughout its lifetime, a ``Context`` goes through initialization and shutdown phases.
Context initialization is implemented in ``Context::runInitScript``, and shutdown is implemented in ``Context::runShutdownScript``.
These functions are called automatically when a ``Context`` is created, cloned, or destroyed.
Depending on the application and the ``CodeOfPolicies``, they may also be called when ``Context::restart`` or ``Context::restartHeaps`` is called.

It is initialized in the following order:
    1. All global variables are initialized in the order they are declared, per module.
    2. All functions tagged with ``[init]`` are called in the order they are declared, per module, except for specifically ordered ones.
    3. All specifically ordered functions tagged with ``[init]`` are called in the order they appear after topological sort.

The topological sort order for the init functions is specified in the init annotation.
    * ``tag`` attribute puts the function into the named pass
    * ``before`` attribute names a pass that runs **before** this function, so the function is scheduled after every function of that pass
    * ``after`` attribute names a pass that runs **after** this function, so the function is scheduled before every function of that pass

Consider the following example:

.. das-doc: given var order : array<string>
.. code-block:: das

    [init(before="middle")]
    def a {
        order |> push("a")
    }
    [init(tag="middle")]
    def b {
        order |> push("b")
    }
    [init(tag="middle")]
    def c {
        order |> push("c")
    }
    [init(after="middle")]
    def d {
        order |> push("d")
    }

The functions will execute in the following order:
    1. d
    2. b or c, in any order
    3. a

During shutdown, the context runs all functions marked with ``[finalize]`` in the order they are declared, per module.
Those marked ``[finalize(late=true)]`` run after all the regular ones.

Macro contexts
--------------

For each module that contains macros, an individual context is created and initialized.
In addition to regular functions, functions tagged with ``[macro]`` or ``[_macro]`` are called during initialization.

Functions tagged with ``[macro_function]`` are excluded from the regular context and only appear in macro contexts.

Unless a macro module is marked as shared, it will be shut down after compilation.
Shared macro modules are initialized during their first compilation, and are shut down during the environment shutdown.

Locking
-------

A context can carry a ``recursive_mutex``, and is locked and unlocked with the ``lock_context`` or ``lock_this_context`` RAII blocks.
Cross-context calls via ``invoke_in_context`` automatically lock the target context.

The mutex is only created when the program needs it: when it contains a cross-context call,
when the ``threadlock_context`` option or policy is set, or when the debugger is enabled.
Locking a context that has none reports ``threadlock_context is not set``.

Lookups
-------

Global variables and functions can be looked up by name or by mangled name hash on both Daslang and C++ side.

Memory allocation and garbage collection
----------------------------------------

Memory allocation strategies for both the string heap and the regular heap are specified in the ``CodeOfPolicies`` and options.

To allow garbage collection from inside the context, the following options are necessary:

.. code-block:: das

    options persistent_heap // this one enables garbage-collectable heap
    options gc              // this one enables garbage collection for the variables on the stack

To collect garbage, from the inside of the context:

.. code-block:: das

    var collect_string_heap = true
    var validate_after_collect = false
    unsafe {
        heap_collect(collect_string_heap, validate_after_collect)
    }

The collector finds live values by walking the interpreter call stack. A local is
considered live by its **frame position interval**: at simulate time every potential
park point gets a position in its function's numbering, each local records the
interval in which it is initialized and in scope, and the walk compares integers.
Positions are exact for compiler-manufactured code — inlined bodies, constructors,
comprehensions, one-line blocks — where source line ranges are not, and each stamped
position also names its **owning function**, so a frame gated with another function's
numbers is detected rather than misread.

Every call between your program's entry point and ``heap_collect`` must be a regular
interpreted call. The compiler guarantees the common case: every function that can
reach ``heap_collect`` through direct calls keeps a real stack frame (never
fastcall), so collect wrappers — and wrappers of wrappers — just work. The residual —
a collect reached only through a **block, lambda, or function pointer** that a
frameless (fastcall) wrapper invokes — is caught by the position owner check and
refused instead of walking wrong.

Chains the collector cannot attribute are refused with a runtime error instead of
collecting:

* an interpreted frame reached **through a compiled (AOT/JIT) frame**, entered from
  C++ **without line info** (see below), or gated by a **foreign function's
  positions** (a frameless carrier in the chain). An all-compiled chain — a
  standalone compiled executable — is allowed and collects from globals. Note that a
  compiled function entered from the interpreter carries an interpreter-shaped frame
  the collector cannot distinguish; the gc test suites therefore run interpreted-only.

The current position at any call site is readable as ``frame_position()`` — nonzero
only when the program carries positions (``options gc`` or a debugger).

To do the same from the C++ side:

.. code-block:: cpp

    context->collectHeap(line_info_ptr, collect_string_heap, validate_after_collect);

Passing ``nullptr`` for the line info is allowed only when no daslang frames are on the
context's stack (for example between updates, or on a macro context). When C++ code
re-enters daslang below live frames, use an entry point that takes a line —
``context->callOrFastcall(fn, args, line)`` or ``das_invoke_function<...>::invoke(ctx,
at, fn)`` — and pass a real ``at``: a collect below a null-line re-entry is refused,
because the locals of the frames above it can no longer be attributed. (``Context::eval``
and ``evalWithCatch`` carry no line parameter and are only safe as stack-root entries.)

.. seealso::

    :ref:`Annotations <annotations>` for ``[init]``, ``[finalize]``, and ``[export]`` annotations,
    :ref:`Program structure <program_structure>` for the overall compilation and initialization lifecycle,
    :ref:`Locks <locks>` for context and container locking details,
    :ref:`Options <options>` for ``persistent_heap``, ``gc``, and memory allocation policies,
    :ref:`Macros <macros>` for macro context initialization.


