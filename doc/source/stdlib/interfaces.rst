
.. _stdlib_interfaces:

==========
Interfaces
==========

.. das:module:: interfaces

The INTERFACES module implements interface-based polymorphism for daslang.
It provides the ``[interface]`` annotation for defining abstract interfaces
with virtual method tables, supporting multiple implementations and dynamic
dispatch without class inheritance.

**Features:**

- Interface inheritance (``class IChild : IParent``)
- Default method implementations (non-abstract methods)
- Compile-time completeness checking (error 30111 on missing methods)
- ``is``/``as``/``?as`` operators via the ``InterfaceAsIs`` variant macro

All functions and symbols are in "interfaces" module, use require to get access to it.

.. code-block:: das

    require daslib/interfaces

Example:

.. code-block:: das

    require daslib/interfaces

    [interface]
    class IGreeter {
        def abstract greet(name : string) : string
    }

    [implements(IGreeter)]
    class MyGreeter {
        def IGreeter`greet(name : string) : string {
            return "Hello, {name}!"
        }
    }

    [export]
    def main() {
        var obj = new MyGreeter()
        var greeter = obj as IGreeter
        print("{greeter->greet("world")}\n")
    }
    // output: Hello, world!

See also: :ref:`Interfaces tutorial <tutorial_interfaces>` for a
complete walkthrough.



++++++++++++++
Variant macros
++++++++++++++

.. _call-macro-interfaces-InterfaceAsIs:

.. das:attribute:: InterfaceAsIs

Variant macro that enables ``is``, ``as``, and ``?as`` operators for
interface types declared with ``[interface]`` / ``[implements]``.

* ``ptr is IFoo``  — compile-time check (``true`` when the struct implements ``IFoo``)
* ``ptr as IFoo``  — obtains the interface proxy via the generated getter
* ``ptr ?as IFoo`` — null-safe version: returns ``null`` when the pointer is null



++++++++++++++++
Structure macros
++++++++++++++++

.. _handle-interfaces-interface:

.. das:attribute:: interface

Verifies that the annotated class is a valid interface — it may only contain
function-typed fields (no data members). Applied via ``[interface]`` annotation.


.. _handle-interfaces-implements:

.. das:attribute:: implements

Generates interface bindings for a struct. Creates a proxy class that delegates
interface method calls to the struct's own methods, and adds a ``get`InterfaceName``
method that lazily constructs the proxy. Applied via ``[implements(InterfaceName)]``.

If the interface inherits from a parent interface (``class IChild : IParent``),
ancestor getters are generated automatically so that ``is``/``as``/``?as`` work
with all ancestor interfaces.

At ``finish`` time, verifies that all abstract interface methods are implemented.
Methods with default bodies in the interface class are optional — the proxy
inherits them via class hierarchy.



