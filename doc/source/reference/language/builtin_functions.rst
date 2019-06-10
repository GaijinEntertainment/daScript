.. _builtin_functions:


==================
Built-in Functions
==================

.. index::
    single: Built-in Functions
    pair: Global Symbols; Built-in Functions


^^^^^^^^^^^^^^
Misc
^^^^^^^^^^^^^^
.. js:function:: panic

    will cause panic. The program will be determinated if there is no recover.
    Panic is not a error handling mechanism and can not be used as such. It is indeed panic, fatal error.
    It is not supposed that program can completely correctly recover from panic, recover construction is provided so program can try to correcly shut-down or report fatal error.
    If there is no recover withing script, it will be called in calling eval (in c++ callee code).

.. js:function:: print

    print prints any provided argument, provided that type has DataWalker 'to string' (all PODs do have it).

.. js:function:: stackwalk

    stackwalk prints call stack and local variables values

.. js:function:: terminate

    terminates program execution

.. js:function:: breakpoint

    breakpoint will call os_debugbreakpoint, which is link-time unresolved dependency. It's supposed to call breakpoint in debugger tool, as sample implementation does.

.. js:function:: heap_bytes_allocated

    heap_bytes_allocated will return bytes allocated on heap (i.e. really used, not reserved)



assert
verify
static_assert
debug

invoke

^^^^^^^^
Arrays
^^^^^^^^

push, emplace, resize, erase, length, clear, capacity

(see :ref:`Arrays <arrays>`).

^^^^^^^^
Tables
^^^^^^^^

clear
length
capacity
erase
find
key_exists

(see :ref:`Tables <tables>`).

^^^^^^^^^
Functions
^^^^^^^^^

invoke(fn, 1) // call function fn with argument 1

(see :ref:`Functions <functions>`).
